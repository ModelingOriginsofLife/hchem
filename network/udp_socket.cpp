#ifndef UDP_SOCKET_CPP
#define UDP_SOCKET_CPP

#include <stdlib.h>

#include "udp_socket.h"

#include "address.cpp"
#include "packet.cpp"

// Constructors

udp_socket :: udp_socket()
{
}

udp_socket :: udp_socket(unsigned short port)
{   
    this->error = false;
    this->blocking = true;
    
	this->socket_id = socket(PF_INET, SOCK_DGRAM, 0);
    
	struct sockaddr_in bind_address;
	bind_address.sin_family = AF_INET;
	bind_address.sin_port = htons(port);
	bind_address.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(bind_address.sin_zero, '\0', sizeof(bind_address.sin_zero));
    
	if(::bind(this->socket_id, (struct sockaddr *) &bind_address, sizeof(sockaddr_in)))
		this->error = true;
    
	struct sockaddr_in detect_port;
	socklen_t detect_port_len = sizeof(sockaddr_in);
    
	if(getsockname(this->socket_id, (struct sockaddr *) &detect_port, &detect_port_len) == 0 && detect_port.sin_family == AF_INET && detect_port_len == sizeof(sockaddr_in))
	{
		this->port = ntohs(detect_port.sin_port);
		if(this->port != port)
			this->error = true;
	}
	else
		this->error = true;
}

udp_socket :: udp_socket(const udp_socket & s)
{
	this->socket_id = s.socket_id;
	this->port = s.port;
	this->error = s.error;
}

// Communication

void udp_socket :: send(packet p)
{
    if(p.content.length() > MAX_PACKET_LENGTH)
        return;
    
    struct sockaddr_in destination;
    destination = p.destination;
    
    sendto(this->socket_id, p.content, p.content.length(), 0, (struct sockaddr *) & destination, sizeof(sockaddr_in));
}

packet udp_socket :: receive(bool blocking)
{
    if(this->blocking != blocking)
    {
        this->blocking = blocking;
        if(this->blocking)
            fcntl(this->socket_id, F_SETFL, 0);
        else
            fcntl(this->socket_id, F_SETFL, O_NONBLOCK);
    }
    
    struct sockaddr_in source;
    socklen_t source_len = sizeof(source);
    
    char buffer[MAX_PACKET_LENGTH];
    
    long int bytes_received;
    bytes_received = recvfrom(this->socket_id, buffer, MAX_PACKET_LENGTH, 0, (sockaddr *) &source, &source_len);
    
    packet p;

    if(bytes_received >= 0)
    {
        p.source = source;
        p.content = data(buffer, bytes_received);
    }
    
    return p;
}

// Operators

void udp_socket :: operator = (udp_socket s)
{
    this->socket_id = s.socket_id;
    this->port = s.port;
    this->error = s.error;
    this->blocking = s.blocking;
}

// Casting

udp_socket :: operator bool()
{
    return !(this->error);
}

#endif