#include "skel.h"

/* Array of router interfaces (e.g. 0,1,2,3) */
int interfaces[ROUTER_NUM_INTERFACES];

/* Routing table */
struct rtable_entry *rtable;
int rtable_len;

/* Mac table */
struct nei_entry *nei_table;
int nei_table_len;

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given protocol and destination address. Or NULL if there is no matching route.
*/

static inline int check_entry(struct in_addr dest_ip, struct rtable_entry entry)
{
	return (dest_ip.s_addr & entry.netmask.s_addr)==entry.network.s_addr;
}

struct rtable_entry *get_best_route(uint16_t proto, struct in_addr dest_ip, struct in6_addr dest_ip6) {
	/* TODO 1: Implement the function. We don't use dest_ip6 at this exercise */
	for(int i=0;i<rtable_len;i++)
		if(proto==rtable[i].proto && check_entry(dest_ip, rtable[i]))
		{
			return rtable+i;
		}

	return NULL;
}

/*
 Returns a pointer (eg. &nei_table[i]) to the best matching neighbor table entry.
 for the given protocol and destination address. Or NULL if there is no matching route.
*/

static inline int check_nei(struct in_addr dest_ip, struct nei_entry entry)
{
	return dest_ip.s_addr==entry.ip.s_addr;
}

struct nei_entry *get_nei_entry(uint16_t proto, struct in_addr dest_ip, struct in6_addr dest_ip6) {
    /* TODO 2: Implement the function. We don't use dest_ip6 at this exercise. */
    for(int i=0;i<nei_table_len;i++)
		if(proto==nei_table[i].proto && check_nei(dest_ip, nei_table[i]))
			return nei_table+i;
	return NULL;
}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();

	rtable = malloc(sizeof(struct rtable_entry) * 100);
	DIE(rtable == NULL, "memory");

	nei_table = malloc(sizeof(struct  nei_entry) * 100);
	DIE(nei_table == NULL, "memory");
	
	/* Read the static routing table and the MAC table */
	rtable_len = read_rtable(rtable);
	nei_table_len = read_nei_table(nei_table);

	while (1) {
		/* Receives a packet from an interface */
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		
		/* Extract the Ethernet header from the packet. Since protocols are stacked, the first header is the ethernet header,
		 * the next header is at m.payload + sizeof(struct ether_header) */
		struct ether_header *eth_hdr = (struct ether_header *) m.payload;
		struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));

		/* TODO 3: DONE: Check if this is an IPv4 or IPv6 packet and route accordingly. For now we will drop IPv6 packets and forward only IPv4.*/
		/* We check if the packet is IPV4. Watch out for the usage of ntohs, why do we need it? Hint: Network Order */
		if(ip_hdr->protocol != 4)	continue;
		if (ntohs(eth_hdr->ether_type) == ETHERTYPE_IP) {
		/* TODO 4: Check the checksum as required by IPv4  */
			if(ip_checksum(ip_hdr, sizeof(struct iphdr)) != ip_hdr->check)	continue;

		/* TODO 5: Check TTL >= 1 */
			if(ip_hdr->ttl ==0)	continue;
			struct in_addr dest_addr;
			struct in6_addr dest6_addr;
			dest_addr.s_addr=ip_hdr->daddr;
		/* TODO 6: Find best matching route (using the function you wrote at TODO 1) */
			struct rtable_entry *rt_entry =get_best_route(ip_hdr->protocol, dest_addr, dest6_addr);

		/* TODO 7: Find matching neighbour table entry (using the function you wrote at TODO 2)*/
			struct neitable_entry *nei_entry =get_nei_entry(ip_hdr->protocol, dest_addr, dest6_addr);
			
		/* TODO 8: Update TTL and recalculate the checksum */
			ip_hdr->ttl--;
			ip_hdr->check=ip_checksum(ip_hdr, sizeof(struct iphdr));

		/* TODO 9: Update the Ethernet addresses */
			//eth_hdr->
		/* TODO 10: Forward the pachet to best_route->interface */
		}

	}
}
