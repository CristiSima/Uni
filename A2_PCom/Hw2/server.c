#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "helpers.h"

#include "linked_list.h"
#include "trie_dic.h"

#define MAX_CLIENTS 10
#define CLIENT_ID_LEN 10

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}

struct client_info
{
	uint32_t sockfd;
	linked_list send_queue;

	uint8_t is_connected :1;
};

struct client_info client_info_create()
{
	return (struct client_info){.sockfd=0, .is_connected=0, .send_queue=ll_create()};
}

struct topic_subscriber_entry
{
	char client_id[CLIENT_ID_LEN];

	uint8_t has_SF :1;
};

// fancy macro definitions
ll_pop_front_p_instance(topic_subscriber_pop_front_p, struct topic_subscriber_entry)

ll_pop_front_p_instance(client_send_queue_pop_front_p, struct info_msg_info)
#define client_send_queue_pop_front(list) client_send_queue_pop_front_p(&(list))

trie_dic_get_instance(sockfd_info_get, sizeof(int), char);
trie_dic_set_instance(sockfd_info_set, sizeof(int), char);

trie_dic_get_instance(client_info_get, CLIENT_ID_LEN, struct client_info);
trie_dic_set_instance(client_info_set, CLIENT_ID_LEN, struct client_info);


trie_dic_get_instance(topics_subscribers_get, TOPIC_LEN, linked_list);
trie_dic_set_instance(topics_subscribers_set, TOPIC_LEN, linked_list);

// topic_name	-> [(client_id, 	SF  )]
// String 		-> [(String,		bool)] / [topic_subscriber_entry]
trie_dic topics_subscribers;

// client_id -> (fd, is_connected, [queueed_msgs] )
// String 	 -> (fd, bool		 , [src++info_msg]) / client_info
trie_dic client_info;

// int 	-> String
// fd	-> client_id
trie_dic sockfd_info;


fd_set read_fds;

struct info_msg_info *info_msg_info_init()
{
	// smart pointer init

	struct info_msg_info *info=calloc(1, sizeof(struct info_msg_info));

	info->uses=0;
	info->header.packet_type=INFO_INFO;

	return info;
}

void info_msg_info_use(struct info_msg_info *info)
{
	// smart pointer add referenec
	info->uses++;
}

void info_msg_info_smart_free(struct info_msg_info *info)
{
	// remove 1 referenec
	info->uses--;
	info_msg_info_check_free(info);
}

void info_msg_info_check_free(struct info_msg_info *info)
{
	// check if it is freable(no references)
	if(info->uses==0)
		free(info);
}

void client_info_free(void *client)
{
	// use smart free to avoid use after free or double free
	ll_free(((struct client_info *)client)->send_queue, info_msg_info_smart_free);
	free(client);
}

void topics_subscribers_free(void *list)
{
	ll_free_p(list, free);
	free(list);
}

#define SUBSCRIBE_ALREADY_SUBSCRIBED 1
int subscribe(char *topic, char *client_id, uint8_t flag_SF)
{
	// get list of subscribed clients to topic
	linked_list *topic_subscribers=topics_subscribers_get(topics_subscribers, topic);

	if(topic_subscribers==NULL)
	{
		// initialize and add list list
		topic_subscribers=ll_create_p();
		topics_subscribers_set(topics_subscribers, topic, topic_subscribers);
	}

	// iterate over subscribed clients
	ll_iterator_p_def(topic_subscribers, node)
	{
		// if client is already subscribed, return error
		if(!memcmp(((struct topic_subscriber_entry*)node->data)->client_id, client_id, CLIENT_ID_LEN))
			return SUBSCRIBE_ALREADY_SUBSCRIBED;
	}

	// create topic_entry
	struct topic_subscriber_entry *topic_entry=calloc(1, sizeof(struct topic_subscriber_entry));
	memcpy(topic_entry->client_id, client_id, CLIENT_ID_LEN);
	topic_entry->has_SF=flag_SF;

	// add to list
	ll_add_end_p(topic_subscribers, topic_entry);

	return 0;
}

#define UNSUBSCRIBE_NOT_SUBSCRIBED 1
int unsubscribe(char *topic, char *client_id)
{
	// get list of subscribed clients to topic
	linked_list *topic_subscribers=topics_subscribers_get(topics_subscribers, topic);

	// if topic has no subscriber => client isn't subscribed => not subscribed error
	if(topic_subscribers==NULL)
		return UNSUBSCRIBE_NOT_SUBSCRIBED;

	// remove first is it's the client
	if(	topic_subscribers->front &&
		!memcmp(((struct topic_subscriber_entry*)topic_subscribers->front->data)->client_id, client_id, CLIENT_ID_LEN))
	{
		free(topic_subscriber_pop_front_p(topic_subscribers));
		return 0;
	}

	// iterate over subscribed clients
	ll_iterator_p_def(topic_subscribers, node)
		if(	node->next &&
			!memcmp(((struct topic_subscriber_entry*)node->next->data)->client_id, client_id, CLIENT_ID_LEN))
		{
			// remove entry for client
			struct ll_node *temp=node->next;
			node->next=temp->next;

			// check if it was the last and move the end of the list
			if(topic_subscribers->end==temp)
				topic_subscribers->end=node;

			// free memory
			free(temp->data);
			free(temp);
			return 0;
		}

	// if client wasn't in the list return not subscribe error
	return UNSUBSCRIBE_NOT_SUBSCRIBED;
}


#define DISCONNECT_CLIENT_NOT_CONNECTED 1
int disconnect_client(char *client_id)
{
	// get client info from client_id
	struct client_info *client=client_info_get(client_info, client_id);

	if(client->is_connected)
	{
		// reset flags
		client->is_connected=0;

		// make the big stdout announcement
		printf("Client %s disconnected.\n", client_id);

		// remove all it's fd entries
		close(client->sockfd);
		FD_CLR(client->sockfd, &read_fds);

		// dissociate the fd with the client
		free(sockfd_info_get(sockfd_info, &client->sockfd));
		sockfd_info_set(sockfd_info, &client->sockfd, NULL);
		client->sockfd=0;
	}
	else
		// can't disconnect an unconnected client
		return DISCONNECT_CLIENT_NOT_CONNECTED;

	return 0;
}

#define FORWARD_CLIENT_INFO_FAIL 1
int forward_to_client_queue_pos(struct info_msg_info *forward_packet, char *client_id, uint8_t SF_flag, uint8_t add_front_flag)
{
	// get client info from client_id
	struct client_info *forward_client_info=client_info_get(client_info, client_id);
	DIE(forward_client_info==NULL, "forward to unknown forward_client_info");

	int rc;
	if(forward_client_info->is_connected)
	{
		// the client can receive

		rc= send(forward_client_info->sockfd, &forward_packet->header, forward_packet->len, 0);

		// check if everything is ok
		if(rc==ECONNRESET || rc==0)
		{
			disconnect_client(client_id);
		}

		if(rc==forward_packet->len)
		{
			// forward successfull
			return 0;
		}
	}

	// client not connected and Store&Forward
	if(!forward_client_info->is_connected && SF_flag)
	{
		info_msg_info_use(forward_packet);

		// for generality
		if(add_front_flag)
			ll_add_front(forward_client_info->send_queue, forward_packet);
		else
			ll_add_end(forward_client_info->send_queue, forward_packet);
	}
	return FORWARD_CLIENT_INFO_FAIL;
}
// default forward that will add to the end of the queue
#define forward_to_client(forward_packet, client_id, SF_flag) forward_to_client_queue_pos(forward_packet, client_id, SF_flag, 0)


#define UPDATE_CLIENT_ALREADY_CONNECTED 1
int update_client(char *client_id, int sockfd)
{
	// get client info from client_id
	struct client_info *client=client_info_get(client_info, client_id);

	if(client==NULL)
	{
		// create client_info entry
		// THE ONLY PLACE WHERE IT'S OK TO GET A NEW CLIENT_ID
		struct client_info new_client=client_info_create();
		new_client.sockfd=sockfd;
		new_client.is_connected=1;

		// add entry
		client_info_set(client_info, client_id, memdup(&new_client, sizeof(struct client_info)));
	}
	else
	{
		// check already connected
		if(client->is_connected)
			return UPDATE_CLIENT_ALREADY_CONNECTED;

		// update values
		client->is_connected=1;
		client->sockfd=sockfd;
	}

	// associate fd with client
	sockfd_info_set(sockfd_info, &sockfd, memdup(client_id, CLIENT_ID_LEN));

	return 0;
}

int main(int argc, char *argv[])
{
	// init globals
	topics_subscribers=trie_dic_create();
	client_info=trie_dic_create();
	sockfd_info=trie_dic_create();

	// call check
    if (argc < 2) {
		usage(argv[0]);
	}

    int rc;

	// dissable buffering
    rc=setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    DIE(rc!=0, "setvbuf");

    int portno;

    int tcp_sockfd, tcp_newsockfd;
    int udp_sockfd;
    int maxfd;

    char buffer[BUFLEN];
    int msg_size;

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

	fd_set ready_fds;

	struct info_packet_header header;

	// init sets
    FD_ZERO(&read_fds);
	FD_ZERO(&ready_fds);

	// get sockets
    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(tcp_sockfd < 0, "tcp_socket");
    udp_sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	DIE(udp_sockfd < 0, "udp_socket");

	// dissable nagle algorithm
    rc=1;
    rc=setsockopt(tcp_sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &rc, sizeof(int));
    DIE(rc < 0, "setsockopt TCP_NODELAY");

	// get port from argv
	portno = atoi(argv[1]);
	DIE(portno == 0, "atoi");

	// construct bind address
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// bind
    rc = bind(tcp_sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(rc < 0, "tcp_bind");
    rc = bind(udp_sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));
	DIE(rc < 0, "udp_bind");

	// set listen
    rc = listen(tcp_sockfd, MAX_CLIENTS);
	DIE(rc < 0, "listen");

	// add sockets to set
    FD_SET(tcp_sockfd, &read_fds);
    FD_SET(udp_sockfd, &read_fds);
    maxfd=udp_sockfd;

	// add stdio to set
	FD_SET(0, &read_fds);

	// needed for stop
	int on=1;
    while(on)
    {
        ready_fds=read_fds;

        rc = select(maxfd + 1, &ready_fds, NULL, NULL, NULL);
		DIE(rc < 0, "select");

        for(int i=0;i<=maxfd;i++)
            if(FD_ISSET(i, &ready_fds))
            {
				if(i==0)
				{
					// set close
					on=0;
					break;
				}
                if(i==tcp_sockfd)
                {
                    // new subscriber

					// accept
                    clilen = sizeof(cli_addr);
                    tcp_newsockfd = accept(tcp_sockfd, (struct sockaddr *) &cli_addr, &clilen);
                    DIE(tcp_newsockfd < 0, "accept");

					char client_id[CLIENT_ID_LEN+1]={0};

					// assume the client is nice and will send his id right away
					rc = recv(tcp_newsockfd, client_id, CLIENT_ID_LEN, 0);
					DIE(rc < 0, "sub recv");

					if(rc==0)
                    {
						close(tcp_newsockfd);
						continue;
					}

					// try to update client information
					rc=update_client(client_id, tcp_newsockfd);

					if(rc==UPDATE_CLIENT_ALREADY_CONNECTED)
					{
						// another client is connected with that id
						printf("Client %s already connected.\n", client_id);

						// msg for client
						char *reply="Already connected.";
						send(tcp_newsockfd, reply, strlen(reply), 0);
						close(tcp_newsockfd);
						continue;
					}

					// send ok
					send(tcp_newsockfd, "Connected.", 10, 0);

					// add new socket to set
					FD_SET(tcp_newsockfd, &read_fds);
					if (tcp_newsockfd > maxfd)
						maxfd = tcp_newsockfd;

					// announce to stdout
                    printf("New client %s connected from %s:%d\n",	client_id,
						                            			inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

                }
                else if(i==udp_sockfd)
                {
                    // UDP Client msg

					// struct info_msg_info forward_packet={.packet_type=INFO_INFO};
					struct info_msg_info *forward_packet=info_msg_info_init();

					// recv msg and populate src_addr
					clilen=sizeof(forward_packet->src_addr);
                    rc = recvfrom(udp_sockfd, &forward_packet->msg_only, sizeof(forward_packet->msg_only), 0,
                                        (struct sockaddr *)&forward_packet->src_addr, &clilen);

                    DIE(rc < 0, "UDP recvfrom");
                    msg_size=rc;

					// save length of the forwarded msg
					// 				packet_type ++ src_addr ++ info_msg
					forward_packet->len=sizeof(forward_packet->header)+sizeof(forward_packet->src_addr)+msg_size;
					forward_packet->header.len=forward_packet->len-sizeof(forward_packet->header);
					forward_packet->header.len=ntohs(forward_packet->header.len);


					// get list of sibscribed clients
					linked_list *topic_list=topics_subscribers_get(topics_subscribers, forward_packet->msg_only.topic);

					// there is no list => noone is subscribed => job done
					if(topic_list==NULL)
					{
						// no uses => free
						info_msg_info_check_free(forward_packet);
						continue;
					}

					// iterate over all subscribed clients
					ll_iterator_p_def(topic_list, node)
					{
						struct topic_subscriber_entry *forward_info=node->data;

						// forward packet
						forward_to_client(forward_packet, forward_info->client_id, forward_info->has_SF);
					}

					// free if it wasn't stored
					info_msg_info_check_free(forward_packet);
                }
                else
                {
                    // subscriber

					memset(buffer, 0, BUFLEN);
					rc = recv(i, &header, sizeof(header), 0);
					DIE(rc < 0, "sub recv");

					header.len=ntohs(header.len);

					// don't try to rev is disconnected or there is no content
					if(rc && header.len)
						rc = recv(i, buffer, (uint32_t)header.len, 0);
					DIE(rc < 0, "sub recv");

                    msg_size=rc;
                    // NEVER TRUST USER INPUT :]
                    buffer[msg_size] = '\0';

					char *client_id=sockfd_info_get(sockfd_info, &i);
					DIE(client_id==NULL, "couldn't find client_id from sockfd");


                    if(msg_size==0)
                    {
						disconnect_client(client_id);
						continue;
                    }

					if(header.packet_type==INFO_CMD)
                    {
						// parse command as needed
						char *cmd=strtok(buffer, " ");

						if(cmd==NULL)
							continue;

						if(!strcmp("subscribe", cmd))
						{
							// subscribe

							// copy topic to buffer of size topic so that it's padded with \0 to TOPIC_LEN
							char topic[TOPIC_LEN+1]={0};
							char *topic_p=strtok(NULL, " ");

							if(topic_p!=NULL)
								strncpy(topic, topic_p, TOPIC_LEN);

							char *SF_flag=strtok(NULL, " ");
							char *reply;

							if(topic==NULL || topic[0]==0 || SF_flag==NULL)
								reply=INFO_REPLY_BUILD("Not enough parameters.");
							else
							{
								rc=subscribe(topic, client_id, SF_flag[0]=='1');

								if(rc==SUBSCRIBE_ALREADY_SUBSCRIBED)
									reply=INFO_REPLY_BUILD("Already subscribed.");
								else
									reply=INFO_REPLY_BUILD("Subscribed to topic.");
							}

							send(i, reply, INFO_HEADER_SIZE+strlen(reply+INFO_HEADER_SIZE), 0);
						}
						else if(!strcmp("unsubscribe", cmd))
						{
							// copy topic to buffer of size topic so that it's padded with \0 to TOPIC_LEN
							char topic[TOPIC_LEN+1]={0};
							char *topic_p=strtok(NULL, " ");

							if(topic_p!=NULL)
								strncpy(topic, topic_p, TOPIC_LEN);

							char *reply;

							if(topic==NULL || topic[0]==0)
								reply=INFO_REPLY_BUILD("Not enough parameters.");
							else
							{
								rc=unsubscribe(topic, client_id);

								if(rc==UNSUBSCRIBE_NOT_SUBSCRIBED)
									reply=INFO_REPLY_BUILD("Not subscribed.");
								else
									reply=INFO_REPLY_BUILD("Unsubscribed from topic.");
							}

							send(i, reply, INFO_HEADER_SIZE+strlen(reply+INFO_HEADER_SIZE), 0);
						}
						else if(!strcmp("exit", cmd))
						{
							// disconnect
							disconnect_client(client_id);
							continue;
						}
						else
						{
							char *reply=INFO_REPLY_BUILD("Unknown command.");
							printf("%s\n", reply+INFO_HEADER_SIZE);
							send(i, reply, INFO_HEADER_SIZE+strlen(reply+INFO_HEADER_SIZE), 0);
						}
                    }

					if(header.packet_type==INFO_GET_SF)
					{
						// send all packets in the S&F queue
						struct client_info *peer_client_info=client_info_get(client_info, client_id);

						// nothing in queue
						if(peer_client_info->send_queue.front==NULL)
							continue;

						struct info_msg_info* queued_msg;
						rc=0;

						for(queued_msg=client_send_queue_pop_front(peer_client_info->send_queue);
							queued_msg!=NULL && rc==0;
							queued_msg=client_send_queue_pop_front(peer_client_info->send_queue))
						{
							// has_SF is set to 1 bc it's already in queue
							rc=forward_to_client_queue_pos(queued_msg, client_id, 1, 1);
							// other clients might have this in their queue
							info_msg_info_smart_free(queued_msg);
						}
					}
                }
            }
    }

	// close all fd
	for(int i=1;i<=maxfd;i++)
		if(FD_ISSET(i, &read_fds))
			close(i);

	trie_dic_free(sockfd_info, free);
	trie_dic_free(client_info, client_info_free);
	trie_dic_free(topics_subscribers, topics_subscribers_free);
    return 0;
}
