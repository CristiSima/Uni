trie_dic and linked_list are c generic libraries for datastructures made by myself

######

packet:
    header:
        1b packet type
        2b contents length
    contents:
        depends on type
INFO_INFO   :   header ++ source address(struct sockaddr_in) ++ msg from UDP client(struct info_msg)
                                            the msg from the UDP client is truncated to it's actual content's size
INFO_CMD    :   header ++ command(char[])
INFO_REPLY  :   header ++ reply(char[])
INFO_GET_SF :   header(size 0)

as part of the initial handshake the client MUST send it's client id
the server will respond with "Connected." if everything is ok or with an error msg that should be printed to the user

in order to receive all messages in the S&F queue the client must send a GET_SF packet

struct info_msg
    UDP msg representation

struct info_msg_info
    stores the packet that needs to be forwarded together with it's size
    bc more clients might have S&F on the same topic and be disconnected, this structure implements smart pointers
        this is possible bc each subscriber receives the same message

struct client_info
    stores the file descriptor, a queue of unsend packets(info_msg_info)
    and client state indicators

struct topic_subscriber_entry
    stores a client id and a flag for Store&Forward

topics_subscribers
    global dictionary
    associates the topic name(char[TOPIC_LEN]) to a list of topic_subscriber_entry

client_info
    global dictionary
    associates a client id(char[CLIENT_ID_LEN]) to a instance of client_info

sockfd_info
    global dictionary
    associates a file descriptor to a client id(char[CLIENT_ID_LEN])

these dictionaries are global to avoid function parameter pollution

info_msg_info_use:
    mark another reference
info_msg_info_smart_free:
    removes one reference and attempts to remove the object
info_msg_info_check_free:
    attempts to remove the object

subscribe:
    subscribes a client to a topic

unsubscribe:
    unsubscribes a client from a topic

disconnect_client
    disconnects a client and clears internal references

forward_to_client_queue_pos
    tries to forward a msg to a client
    if not successfull calls disconnect_client
        if the S&F is set adds the msg to the send queue
    the last parameter specifies where in the queue to add the packet(front/end)

forward_to_client
    wrapper for forward_to_client_queue_pos, allways add to end

update_client
    sets internal references between the client and it's current socket

the server:
    receives input from stdin => exit

    receives a packet from the listening TCP socket
        get it's client id
        update client and socket info

    receives a packet from a UDP client
        build a info_msg_info(smart pointer)
        get a list of subscribed clients(from topics_subscribers)
        forward the info_msg_info to all clients in the list
        try to free the smart pointer

    receives a packet from a subscriber
        if it is a command => process it
        if it is a SF queue request => try to forward_to_client it's entire queue


CLIENT:

show_info
    parses UDP msg and prints it

the client:
    forwards message from stdin to the server
                and  from the server directly to stdout or to show_info
