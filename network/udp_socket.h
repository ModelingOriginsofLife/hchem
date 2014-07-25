#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <fcntl.h>

#include "address.h"
#include "packet.h"

#define MAX_PACKET_LENGTH 1024

class udp_socket
{
    int socket_id;
    unsigned short port;
    
    bool error;
    bool blocking;
    
public:
    udp_socket();
    udp_socket(unsigned short);
    udp_socket(const udp_socket &);
    
    void send(packet);
    packet receive(bool = true);
    
    void operator = (udp_socket);
    operator bool();
};
#endif