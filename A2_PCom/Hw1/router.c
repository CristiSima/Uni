#include "queue.h"
#include "list.h"
#include "skel.h"

#define RTABLE_LEN 65000
#define IP_ICMP_TTLEXEDED_BUFF 0x40
#define IP_ICMP_NOROUTE_BUFF 0x40

void hexdump(void *data, int len)
{
	for(int i=0;i<len;i++)
		printf("%02x ", ((uint8_t*)data)[i]);
	puts("");
}

struct arp_entry get_next_hop_mac(struct route_table_entry dest ,struct arp_entry *arp_table, int arp_table_size)
{
	for(int i=0;i<arp_table_size;i++)
		if(dest.next_hop==arp_table[i].ip)
			return arp_table[i];

	return (struct arp_entry){.ip=0};
}

void add_arp_entry(struct arp_entry *arp_table, int *arp_table_size, struct arp_entry arp_entry)
{
	// struct arp_entry temp;
	for(int i=0;i<arp_table_size[0];i++)
		if(arp_entry.ip==arp_table[i].ip)
		{
			// IP in table, update MAC
			memcpy(arp_table[i].mac, arp_entry.mac, ETH_ALEN);
			return;
		}

	arp_table[arp_table_size[0]++]=arp_entry;
}

void send_ARP(__u32 dest_ip, int interface)
{
	packet m;
	memset(&m,0,sizeof(packet));

	m.interface=interface;
	m.len=sizeof(struct ether_header)+sizeof(struct arp_header);

	struct ether_header *eth=(struct ether_header*) m.payload;
	get_interface_mac(interface, eth->ether_shost);
	memset(eth->ether_dhost, 0xff, 6);
	eth->ether_type=htons(0x0806);  // ARP_TYPE

	struct arp_header *arp=((void *) eth) + sizeof(struct ether_header);
	arp->htype=htons(1);
	arp->ptype=htons(0x0800); // IPv4_TYPE
	arp->hlen=6;
	arp->plen=4;
	arp->op=htons(ARPOP_REQUEST);

	get_interface_mac(interface, arp->sha);
	arp->spa=inet_addr(get_interface_ip(interface));
	memset(arp->tha, 0, 6); // unkdown target
	arp->tpa=dest_ip;

	send_packet(&m);
}

struct arp_waiting_entry
{
	packet m;
	int32_t next_hop;
};

struct LPM_node
{
	int has_entry;
	struct route_table_entry entry;
	struct LPM_node *next[2];
};

struct LPM_node *LPM_node_constructor()
{
	struct LPM_node *node=malloc(sizeof(struct LPM_node));
	node->has_entry=0;
	node->next[0]=node->next[1]=NULL;

	return node;
}

struct LPM_node *LPM_node_entry_constructor(struct route_table_entry entry)
{
	struct LPM_node *node=malloc(sizeof(struct LPM_node));
	node->has_entry=1;
	node->entry=entry;
	node->next[0]=node->next[1]=NULL;

	return node;
}


struct LPM_node *route_LPM_init(char *filename)
{
	struct route_table_entry *rtable=malloc(RTABLE_LEN*sizeof(struct route_table_entry));
	int rtable_size=read_rtable(filename, rtable);

	struct LPM_node *temp, *root=LPM_node_constructor();

	uint32_t mask;
	int pos;

	for(int i=0;i<rtable_size;i++)
	{
		mask=1;
		temp=root;

		// assumming that all 0's and all 1's CAN'T exist (bc it's highly discourage)
		for(int j=0;j<32;j++)
		{
			pos=(mask&rtable[i].prefix)!=0;

			if((mask<<1)&rtable[i].mask)
			{
				if(!temp->next[pos])
					temp->next[pos]=LPM_node_constructor();

				temp=temp->next[pos];
			}
			else
			{
				// the end
				if(!temp->next[pos])
					temp->next[pos]=LPM_node_entry_constructor(rtable[i]);
				else
					temp->next[pos]->entry=rtable[i];

				break;
			}

			mask<<=1;
		}
	}

	free(rtable);
	return root;
}

struct route_table_entry LPM_node_get_next_hop(struct LPM_node *root, struct in_addr dest_ip)
{
	struct route_table_entry best_match={.interface=-1};
	uint32_t mask=1;
	struct LPM_node *temp=root;
	int pos;

	for(int i=0;i<32;i++)
	{
		pos=(mask&dest_ip.s_addr)!=0;

		// reached the end
		if(!temp->next[pos])
			return best_match;

		// walk
		temp=temp->next[pos];
		mask<<=1;

		// save new best_match
		if(temp->has_entry)
			best_match=temp->entry;
	}

	return best_match;
}

int main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	packet m;
	int rc;

	list arp_waiting_list=NULL;

	struct arp_entry *arp_table=malloc(101*sizeof(struct arp_entry));
	int arp_table_size=0;

	struct LPM_node *rLPM=route_LPM_init(argv[1]);

	// Do not modify this line
	init(argc - 2, argv + 2);

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");
		/* TODO */
		struct ether_header *eth=(struct ether_header*) m.payload;

		// uint8_t dest_mac[ETH_ALEN]; // unused
		// uint8_t  src_mac[ETH_ALEN]; // unused
		uint8_t  inf_mac[ETH_ALEN];

		get_interface_mac(m.interface, inf_mac);
		// CHECK

		// struct in_addr  inf_ip;
		struct in_addr  inf_ip={.s_addr=inet_addr(get_interface_ip(m.interface))};
		struct in_addr  src_ip;
		struct in_addr dest_ip;

		// IPv4
		if(ntohs(eth->ether_type)==0x0800)
		{
			struct iphdr *iph = ((void *) eth) + sizeof(struct ether_header);
			int forward=1;

			if (ip_checksum((void *) iph, sizeof(struct iphdr)) != 0)
				continue;

			if (iph->ttl <= 1)
			{
				// TIME LIMIT EXCEEDED
				uint8_t buff[IP_ICMP_TTLEXEDED_BUFF];
				memcpy(buff, m.payload, IP_ICMP_TTLEXEDED_BUFF);

				// reset IP  addresses
				iph->daddr=iph->saddr;
				iph->saddr=inf_ip.s_addr;

				iph->protocol=0x01;
				iph->tot_len=htons(sizeof(struct iphdr)+sizeof(struct icmphdr)+IP_ICMP_TTLEXEDED_BUFF);

				struct icmphdr *icmp=((void*)iph)+sizeof(struct iphdr);
				icmp->type=11;
				icmp->code=0;

				icmp->checksum=0;
				memcpy(&icmp->un, buff, IP_ICMP_TTLEXEDED_BUFF);
				m.len=((int)(((void*)icmp)-((void*)m.payload)))+0x10+IP_ICMP_TTLEXEDED_BUFF;
				icmp->checksum=icmp_checksum((void*)icmp, 0x10+IP_ICMP_TTLEXEDED_BUFF);
				forward=0;
			}

			src_ip.s_addr =iph->saddr;
			dest_ip.s_addr=iph->daddr;

			if(dest_ip.s_addr==inf_ip.s_addr)
			{
				// TO SELF
				forward=0;
				if(iph->protocol==0x01)
				{
					// ICMP
					struct icmphdr *icmp=((void*)iph)+sizeof(struct iphdr);

					if(icmp->type==8 && icmp->code==0)
					{
						// ECHO REQUEST
						// reset IP  addresses
						iph->saddr=dest_ip.s_addr;
						iph->daddr=src_ip.s_addr;

						src_ip.s_addr =iph->saddr;
						dest_ip.s_addr=iph->daddr;

						icmp->type=0;
						// icmp->code=0;  // already set

						icmp->checksum=0;
						icmp->checksum=icmp_checksum((void*)icmp, m.len-((int)(((void*)icmp)-((void*)m.payload))));
					}
					else
					{
						continue; // unkdown, disregard package
					}
				}
				else
				{
					continue; // unkdown, disregard package
				}
			}

			struct route_table_entry route=LPM_node_get_next_hop(rLPM, dest_ip);

			if(route.interface==-1)
			{
				// NO ROUTE
				forward=0;
				uint8_t buff[IP_ICMP_NOROUTE_BUFF];
				memcpy(buff, m.payload, IP_ICMP_NOROUTE_BUFF);

				// reset IP  addresses
				dest_ip.s_addr=iph->daddr=iph->saddr;
				src_ip.s_addr=iph->saddr=inf_ip.s_addr;

				iph->protocol=0x01;
				iph->tot_len=htons(sizeof(struct iphdr)+sizeof(struct icmphdr)+IP_ICMP_NOROUTE_BUFF);

				struct icmphdr *icmp=((void*)iph)+sizeof(struct iphdr);
				icmp->type=3;
				icmp->code=0;

				icmp->checksum=0;
				memcpy(&icmp->un, buff, IP_ICMP_NOROUTE_BUFF);
				m.len=((int)(((void*)icmp)-((void*)m.payload)))+0x10+IP_ICMP_NOROUTE_BUFF;
				icmp->checksum=icmp_checksum((void*)icmp, 0x10+IP_ICMP_NOROUTE_BUFF);

				// recalculate route back
				// assumming it is known
				route=LPM_node_get_next_hop(rLPM, dest_ip);
			}

			iph->ttl-=1;	// for some reason "--;" breaks things
			if(forward)
				// smart checksum update using RFC 1624
				iph->check=~((uint16_t)(((uint32_t)(~iph->check)+0x10000)-0x01));
			else
			{
				// not a forward => more then just the TTL changed
				iph->ttl=50;
				iph->check=0;
				iph->check=ip_checksum((void*)iph, sizeof(struct iphdr));
			}

			// next hop in local network
			struct arp_entry dest_arp=get_next_hop_mac(route, arp_table, arp_table_size);

			get_interface_mac(route.interface, eth->ether_shost);
			m.interface=route.interface;
			if(dest_arp.ip!=0)
			{
				memcpy(eth->ether_dhost, dest_arp.mac, 6);

				send_packet(&m);
			}
			else
			{
				struct arp_waiting_entry *temp=malloc(sizeof(struct arp_waiting_entry));
				temp->m=m;
				temp->next_hop=route.next_hop;
				arp_waiting_list=cons(temp, arp_waiting_list);

				send_ARP(temp->next_hop, m.interface);
			}
			continue;
		}

		// ARP
		if(ntohs(eth->ether_type)==0x0806)
		{
			struct arp_header *arp=((void *) eth) + sizeof(struct ether_header);

			// save in ARP_TABLE
			struct arp_entry arp_entry;
			arp_entry.ip=arp->spa;
			memcpy(&arp_entry.mac, arp->sha, ETH_ALEN);
			add_arp_entry(arp_table, &arp_table_size, arp_entry);

			// check queue
			switch (htons(arp->op))
			{
				case ARPOP_REQUEST:

					// filter out msgs for others
					if(arp->tpa!=inf_ip.s_addr)
						continue;

					// set mac address
					memcpy(arp->tha, arp->sha, ETH_ALEN);
					memcpy(arp->sha, inf_mac, ETH_ALEN);

					// set ip address
					arp->spa=arp->tpa;
					arp->tpa=inf_ip.s_addr;

					// set op code
					arp->op=ntohs(ARPOP_REPLY);

					// L2 changes
					memcpy(eth->ether_dhost, eth->ether_shost, ETH_ALEN);
					memcpy(eth->ether_shost, inf_mac, ETH_ALEN);

					// send
					send_packet(&m);

					break;
				case ARPOP_REPLY:; // can't use lable after statement

					list temp=cons(NULL, arp_waiting_list);
					// send packets on waiting list
					for(list i=temp; i->next!=NULL; )
					{
						if(((struct arp_waiting_entry*)(i->next->element))->next_hop==arp->spa)
						{
							struct arp_waiting_entry *entry=i->next->element;
							// set MAC
							memcpy(((struct ether_header*)(entry->m.payload))->ether_dhost, arp->sha, 6);
							send_packet(&entry->m);

							free(entry);
							i->next=cdr_and_free(i->next);
						}
						else
							i=i->next;
					}

					arp_waiting_list=cdr_and_free(temp);
					break;
				default:
					break;
			}
			continue;
		}
	}
}
