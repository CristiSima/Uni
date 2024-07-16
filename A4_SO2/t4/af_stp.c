// SPDX-License-Identifier: GPL-2.0+

/*
 * stp.c - Sauce Transport Protocol
 *
 * Author: Eduard Marin <marin.eduard.c@gmail.com>
 * Author: Cristian Simache <cristiansimache@gmail.com>
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

#include <linux/net.h>
#include <net/protocol.h>
#include <net/sock.h>
#include <linux/skbuff.h>

#include <linux/hashtable.h>

#include "stp.h"

#define procfs_stp_stats		"stp_stats"

MODULE_DESCRIPTION("Sauce Transport Protocol");
MODULE_AUTHOR("Eduard Marin");
MODULE_AUTHOR("Cristian Simache");
MODULE_LICENSE("GPL");

enum {
	STP_CLOSED = 1,
	STP_UNUSED,
	STP_BOUND,
	STP_CONNECTED,

	STP_MAX_STATES	/* Leave at the end! */
};

struct proc_dir_entry *proc_stp_stats_file;

// local identifier(dst for incomming/bind param)  ->  socket(struct stp_sock)
static DEFINE_HASHTABLE(bound_sockets, sizeof(__be16) << 2);	// (* 8 / 2) use hald of the value's bits
static DEFINE_RWLOCK(bound_lock);

static const struct proto_ops stp_ops;

static struct stp_stats {
	u64 rx_pkts;
	u64 hdr_err;
	u64 csum_err;
	u64 no_sock;
	u64 no_buffs;
	u64 tx_pkts;
} stp_stats;

// Wrapper over struct sock
// this is a l3 protocol
// Reference: https://elixir.bootlin.com/linux/latest/source/include/net/inet_sock.h#L209
struct stp_sock {
	// struct sock has to be the fist member for composition inheritence
	struct sock sk;

	uint16_t lport;
	uint8_t laddr[6];

	uint16_t rport;
	uint8_t raddr[6];

	struct hlist_node hlist;
};

static inline struct stp_sock *stp_sk(const struct sock *sk)
{
	return (struct stp_sock *)sk;
}

static void stp_unhash(struct sock *sk);

static struct proto stp_prot = {
	.name			= "STP",
	.owner			= THIS_MODULE,

	.unhash			= &stp_unhash,

	.max_header		= sizeof(struct stp_hdr),
	.obj_size		= sizeof(struct stp_sock),
};

static void stp_unhash(struct sock *sk)
{
	struct stp_sock *stp_sock = stp_sk(sk);

	if (sk->sk_state  != STP_BOUND)
		return;

	sk->sk_state = STP_CLOSED;
	lock_sock(sk);
	write_lock(&bound_lock);

	hash_del(&stp_sock->hlist);

	write_unlock(&bound_lock);
	release_sock(sk);
}

static int stp_release(struct socket *sock)
{
	struct sock *sk = sock->sk;

	if (sk) {
		sk_common_release(sk);
		sock->sk = NULL;
	}
	return 0;
}

static int stp_bind(struct socket *sock, struct sockaddr *myaddr,
					int sockaddr_len)
{
	struct sockaddr_stp *addr = (struct sockaddr_stp *)myaddr;
	struct stp_sock *node, *stp_sock;

	stp_sock = stp_sk(sock->sk);

	// Check for double bind
	if (stp_sock && stp_sock->sk.sk_state == STP_BOUND)
		return -EBUSY;

	if (sockaddr_len < sizeof(struct sockaddr_in))
		return -EINVAL;

	if (addr->sas_family != AF_STP)
		return -EAFNOSUPPORT;

	if (!addr->sas_port)
		return -EACCES;

	lock_sock(sock->sk);
	write_lock(&bound_lock);
	hash_for_each_possible(bound_sockets, node, hlist, (addr->sas_port)) {
		if (node->lport != addr->sas_port)
			continue;

		if (node->sk.sk_bound_dev_if == addr->sas_ifindex ||
			!node->sk.sk_bound_dev_if ||
			!addr->sas_ifindex) {
			write_unlock(&bound_lock);
			release_sock(sock->sk);

			return -EBUSY;
		}
	}

	hash_add(bound_sockets, &stp_sock->hlist, addr->sas_port);

	memcpy(stp_sock->laddr, addr->sas_addr, 6);
	stp_sock->lport = addr->sas_port;
	stp_sock->sk.sk_state = STP_BOUND;
	sock->sk->sk_bound_dev_if = addr->sas_ifindex;

	write_unlock(&bound_lock);
	release_sock(sock->sk);

	return 0;
}

static int stp_connect(struct socket *sock, struct sockaddr *vaddr,
					   int sockaddr_len, int flags)
{
	struct stp_sock *stp;
	struct sockaddr_stp *sstp;

	lock_sock(sock->sk);
	sstp = (struct sockaddr_stp *) vaddr;
	stp = stp_sk(sock->sk);

	if (sockaddr_len < sizeof(struct sockaddr_in)) {
		release_sock(sock->sk);
		return -EINVAL;
	}

	if (sstp->sas_family != AF_STP) {
		release_sock(sock->sk);
		return -EAFNOSUPPORT;
	}

	sk_dst_reset(sock->sk);

	// Copy address, port and interface for socket
	memcpy(stp->raddr, sstp->sas_addr, sizeof(stp->raddr));
	stp->rport = sstp->sas_port;
	stp->sk.sk_state = STP_CONNECTED;

	release_sock(sock->sk);

	return 0;
}

static int stp_sendmsg(struct socket *sock, struct msghdr *m,
											 size_t total_len)
{
	struct stp_hdr *header_payload;
	struct stp_sock *stp = stp_sk(sock->sk);
	struct sockaddr_stp *sstp = (struct sockaddr_stp *) m->msg_name;
	struct net *net = sock_net(sock->sk);
	struct sk_buff *skb;
	size_t layer_2_header_size;
	uint8_t csum = 0;
	int i;

	// Has to be freed
	// https://www.kernel.org/doc/html/v4.18/networking/kapi.html?highlight=dev_put#c.dev_get_by_index
	struct net_device *dev = dev_get_by_index(net, sock->sk->sk_bound_dev_if);

	if (!dev)
		return -ENODEV;

	// Reference: https://elixir.bootlin.com/linux/v5.10.217/source/net/packet/af_packet.c#L1973
	layer_2_header_size = LL_RESERVED_SPACE(dev);

	// SKB guide: http://vger.kernel.org/~davem/skb_data.html
	skb = sock_wmalloc(sock->sk,
		total_len + sizeof(struct stp_hdr) + layer_2_header_size + dev->needed_tailroom,
		0, GFP_KERNEL);

	if (!skb)
		return -ENOBUFS;

	skb->dev = dev;
	skb->protocol = (ETH_P_STP);

	skb_reserve(skb, layer_2_header_size + sizeof(struct stp_hdr));
	skb_reset_network_header(skb);

	memcpy_from_msg(skb_put(skb, total_len), m, total_len);

	// Add the header
	header_payload = skb_push(skb, sizeof(struct stp_hdr));

	header_payload->src = stp->lport;
	header_payload->dst = stp->sk.sk_state == STP_BOUND ? sstp->sas_port : stp->rport;
	header_payload->len = htons(total_len + sizeof(struct stp_hdr));
	header_payload->csum = 0;

	for (i = 0; i < total_len + sizeof(struct stp_hdr) ; i++)
		csum ^= ((uint8_t *)header_payload)[i];

	header_payload->csum = csum;

	// populate L2 header
	dev_hard_header(skb, dev, (ETH_P_STP),
		stp->sk.sk_state == STP_BOUND ? sstp->sas_addr : stp->raddr,
		stp->laddr, total_len + sizeof(struct stp_hdr));

	// update stats
	stp_stats.tx_pkts++;

	// Send data to dev queue
	dev_queue_xmit(skb);
	dev_put(dev);

	return total_len;
}

static int stp_recvmsg(struct socket *sock, struct msghdr *m,
											 size_t total_len, int flags)
{
	struct stp_sock *stp_sock = stp_sk(sock->sk);
	struct sk_buff *skb;
	size_t copied = 0;
	int err;

	skb = skb_recv_datagram(&stp_sock->sk, flags, 0, &err);
	if (!skb)
		return err;

	copied = skb->len;
	err = skb_copy_datagram_msg(skb, sizeof(struct stp_hdr), m, skb->len - sizeof(struct stp_hdr));
	if (err) {
		skb_free_datagram(&stp_sock->sk, skb);
		stp_stats.no_buffs++;

		return err;
	}

	stp_stats.rx_pkts++;
	skb_free_datagram(&stp_sock->sk, skb);

	return skb->len - sizeof(struct stp_hdr);
}

static const struct proto_ops stp_ops = {
	.family = PF_STP,
	.owner = THIS_MODULE,
	.release = stp_release,
	.bind = stp_bind,
	.connect = stp_connect,
	.socketpair = sock_no_socketpair,
	.accept = sock_no_accept,
	.getname = sock_no_getname,
	.poll = datagram_poll,
	.ioctl = sock_no_ioctl,
	.listen = sock_no_listen,
	.shutdown = sock_no_shutdown,
	.sendmsg = stp_sendmsg,
	.recvmsg = stp_recvmsg,
	.mmap = sock_no_mmap,
	.sendpage = sock_no_sendpage,
};

static int stp_create(struct net *net, struct socket *sock,
							 int protocol, int kern)
{
	struct sock *sk;
	struct stp_sock *stp_sock;

	if (protocol && protocol != PF_STP)
		return -EPROTONOSUPPORT;

	if (sock->type != SOCK_DGRAM)
		return -EPROTONOSUPPORT;

	sock->ops = &stp_ops;

	sk = sk_alloc(net, AF_STP, GFP_KERNEL, &stp_prot, kern);
	if (!sk)
		return -ENOBUFS;

	stp_sock = stp_sk(sk);
	stp_sock->sk.sk_state = STP_UNUSED;

	// https://elixir.bootlin.com/linux/v5.10/source/net/ipv4/af_inet.c#L350
	sock_init_data(sock, sk);
	sk->sk_family   = AF_STP;
	sk->sk_protocol = protocol;

	return 0;
}

static int show_stp_stats(struct seq_file *m, void *v)
{
	seq_puts(m, "RxPkts HdrErr CsumErr NoSock NoBuffs TxPkts\n");

	seq_printf(m, "%llu %llu %llu %llu %llu %llu\n",
		stp_stats.rx_pkts, stp_stats.hdr_err, stp_stats.csum_err,
		stp_stats.no_sock, stp_stats.no_buffs, stp_stats.tx_pkts);

	return 0;
}

static int stp_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, show_stp_stats, NULL);
}

static const struct proc_ops stats_pops = {
	.proc_open = stp_stats_open,
	.proc_read = seq_read,
	.proc_release = single_release,
};

static int stp_rcv(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt,
					struct net_device *orig_dev)
{
	struct stp_sock *stp_sock;
	const struct stp_hdr *stph;
	const u8 *header_payload;
	u8 csum = 0;
	u16 i, len;

	if (!pskb_may_pull(skb, sizeof(struct stp_hdr)))
		goto stphdr_error;

	stph = (struct stp_hdr *)skb_network_header(skb);
	len = ntohs(stph->len);

	if (!pskb_may_pull(skb, len))
		goto stphdr_error;

	header_payload = (u8 *)skb_network_header(skb);
	for (i = 0; i < len ; i++)
		csum ^= header_payload[i];

	// on recv csum(pack) must be 0
	if (csum)
		goto stp_inc_err;

	hash_for_each_possible(bound_sockets, stp_sock, hlist, stph->dst)
		if (stp_sock->lport == stph->dst) {
			if (sock_queue_rcv_skb(&stp_sock->sk, skb) < 0) {
				stp_stats.no_buffs++;
				kfree_skb(skb);

				return NET_RX_DROP;
			}

			return NET_RX_SUCCESS;
		}

	goto free_and_leave;

stp_inc_err:
	stp_stats.csum_err++;
	goto free_and_leave;

stphdr_error:
	stp_stats.no_buffs++;

free_and_leave:
	kfree_skb(skb);

	return NET_RX_DROP;
}

static struct packet_type stp_packet_type __read_mostly = {
	.type = htons(ETH_P_STP),
	.func = stp_rcv,
};

static const struct net_proto_family stp_family = {
	.family = PF_STP,
	.create = stp_create,
	.owner = THIS_MODULE,
};

// Reference: https://elixir.bootlin.com/linux/v5.10/source/net/ipv4/af_inet.c#L1946
static int __init stp_init(void)
{
	int ret = 0;

	ret = proto_register(&stp_prot, 1);
	if (ret)
		return ret;

	ret = sock_register(&stp_family);
	if (ret < 0)
		return ret;

	// Ref: https://stackoverflow.com/questions/16720168/create-procfs-entry-in-proc-net
	proc_stp_stats_file = proc_create(procfs_stp_stats, 0000, init_net.proc_net, &stats_pops);
	if (!proc_stp_stats_file)
		goto stp_proto_cleanup;

	dev_add_pack(&stp_packet_type);

	return 0;

stp_proto_cleanup:
	proto_unregister(&stp_prot);
	return -ENOMEM;
}

static void __exit stp_exit(void)
{
	proc_remove(proc_stp_stats_file);
	dev_remove_pack(&stp_packet_type);
	proto_unregister(&stp_prot);
	sock_unregister(PF_STP);
}

module_init(stp_init);
module_exit(stp_exit);
