#ifndef SOCKET_H
#define SOCKET_H

/*
 *  Socket.h
 *  warp
 *
 *  Created by Nicholas Kostelnik on 02/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>

#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include <vector>

#include "ISocket.hpp"

#define WORMHOLE_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

class NixSocket  : public ISocket
{
	
public:
	
	NixSocket() { };
	
	void listen_(unsigned int port)
	{
		listen_sock_ = create_listen_socket(port);
		max_socket_ = listen_sock_;
		FD_SET(listen_sock_, &read_sockets_);
    set_reuse_port(listen_sock_);
		listen(listen_sock_, 5);
    
    multicast_sock_ = create_multicast_socket(port);
		
		std::clog << "listening on port: " << port << std::endl;
	}
  
  int create_multicast_socket(unsigned int port)
  {
		int sock = open_socket(SOCK_DGRAM);
    set_non_blocking(sock);
		bind_socket(sock, port);    
    set_multicast(sock);
		return sock;    
  }
	
	int open_socket(int type)
	{    
		int sock = socket(AF_INET, type, 0);
		
		if (sock < 0)
		{
			std::cerr << "ERROR opening socket" << std::endl;
		}
		
		return sock;
	}
	
	void bind_socket(int sock, unsigned int port)
	{
		struct sockaddr_in serv_addr;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);
		
		if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
			std::cerr << "ERROR on binding" << std::endl;
		}
	}		
	
  void set_multicast(int sock)
  {
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(WORMHOLE_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
      std::cerr << "ERROR setting up multicast" << std::endl;
    }
  }
  
  void set_reuse_port(int sock)
	{
    u_int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
      std::cerr << "ERROR reusing ADDR" << std::endl;
    }
  }
  
	int create_listen_socket(unsigned int port)
	{
		int sock = open_socket(SOCK_STREAM);
		set_non_blocking(sock);
		bind_socket(sock, port);    
		return sock;    
	}
  
  std::string hostname()
  {
    struct addrinfo hints, *info;
    int gai_result;
    
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    
    if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_result));
      exit(1);
    }
    
    return info->ai_next->ai_canonname;
  }
	
	received_data* receive()
	{
    struct sockaddr_in addr;  
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(1723);
    socklen_t addrlen = sizeof(addr);
    
    char multicast_buffer[10000];
    if (recvfrom(multicast_sock_, multicast_buffer, MSGBUFSIZE, 0, (struct sockaddr *) &addr, &addrlen) < 0)
    {  
      if (errno == EWOULDBLOCK)
      {
//        std::cerr << "would block" << std::endl;
      }
      else {
        std::cerr << "ERROR reading from multicast group" << std::endl;
      }
    }
    else {

      struct sockaddr_in addr;  
      memset(&addr, 0, sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(WORMHOLE_GROUP);
      addr.sin_port = htons(1723);
      
      std::clog << hostname() << std::endl;
      sendto(multicast_sock_, hostname().c_str(), sizeof(hostname().c_str()), 0, (struct sockaddr *) &addr, addrlen);
    }

  
		received_data* return_data = new received_data();
		
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		
		fd_set working_socks;
		FD_ZERO(&working_socks);
		memcpy(&working_socks, &read_sockets_, sizeof(read_sockets_));
		
		int readsocks = select(max_socket_ + 1, &working_socks, NULL, NULL, &timeout);
		
		if (readsocks < 0)
		{ 
			std::cerr << "ERROR on accept" << std::endl;
		}
		
		if (readsocks > 0)
		{      
			for (int i = listen_sock_; i < max_socket_ + 1; i++)
			{      
				if (FD_ISSET(i, &working_socks))
				{
					if (i == listen_sock_)
					{
						int incoming_socket = accept(listen_sock_, NULL, NULL);
						
						if (incoming_socket < 0)
						{							
							std::cerr << "ERROR accepting incoming connection: " << strerror(errno) << std::endl;
						}
						
						FD_SET(incoming_socket, &read_sockets_);
						max_socket_ = incoming_socket;
					}
					else
					{       
						char* buffer = new char[256];
						
						int result = read(i, buffer, 256);
            
						if (result < 0)
						{ 
							std::cerr << "ERROR reading from socket: " << strerror(errno) << std::endl;
						}
						
						if (result == 0)
						{
							close(i);
							shutdown(i, 5);
							FD_CLR(i, &read_sockets_);
						}
						
						if (result > 0)
						{
              std::clog << buffer << std::endl;
							return_data->push_back(buffer);
						}
					}
				}
			}
		}
		
		return return_data;
	}
	
	void set_no_delay(int sock)
	{
		int no_delay = 1;
		setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&no_delay, sizeof(no_delay));	
	}
	
	void set_non_blocking(int sock)
	{
		int flags;  
		if ((flags = fcntl(sock, F_GETFL, 0)) < 0) 
		{
			std::cerr << "ERROR, couldn't get socket flags" << std::endl;
		}
		
		if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) 
		{ 
			std::cerr << "ERROR, couldn't set socket flags" << std::endl;
		}
	};
	
	void dispose(received_data* data)
	{		
		for (received_data::iterator i = data->begin(); i != data->end(); ++i) 
		{
			delete[] (*i);
		}				
		
		delete data;
	}
	
	bool connect_to(const std::string& host, unsigned int port)
	{
		struct hostent *server = gethostbyname(host.c_str());
		
		if (server == NULL)
		{
			std::cerr << "ERROR, no such host" << std::endl;
			return false;
		}
		
		std::clog << "connecting to: " << host << std::endl;
		
		struct sockaddr_in server_address;
		bzero((char *) &server_address, sizeof(server_address));
		server_address.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
		server_address.sin_port = htons(port);
		
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
		set_no_delay(socket_);
		set_non_blocking(socket_);
		
		if (socket_ < 0)
		{
			std::cerr << "ERROR, unable to create socket" << std::endl;
			return false;
		}
		
		fd_set outgoing; 
		FD_ZERO(&outgoing); 
		FD_SET(socket_, &outgoing); 
		
		setsockopt(socket_, SOL_SOCKET, SO_NOSIGPIPE, (void *)&outgoing, sizeof(int));
		connect(socket_, (struct sockaddr*)&server_address, sizeof(server_address));
		
		struct timeval tv; 
		tv.tv_sec = 3; 
		tv.tv_usec = 0; 
		
		int result = select(socket_ + 1, NULL, &outgoing, NULL, &tv);
		
		if (result > 0)
		{
			socklen_t lon = sizeof(int);
			int valopt;
			
			if (getsockopt(socket_, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) 
			{ 
				std::cerr << "ERROR, can't get socket options" << std::endl; 
			} 
			else if (valopt) 
			{ 
				std::cerr << "ERROR, connection refused" << std::endl;
			}
			else 
			{
				std::clog << "connected" << std::endl;
				return true;
			}
		}
		
		return false;
	}

				
	void terminate()
	{
		close(socket_);
		shutdown(socket_, 1);	
	}
	
	void send(const char* data)
	{		
		int result = write(socket_, data, sizeof(Message));
		
		if (result < 0)
		{
			std::cerr << "ERROR writing to socket" << std::endl;
		}
	}
	
private:
	
	int socket_;
	
	int max_socket_;
	int listen_sock_;
  int multicast_sock_;
	fd_set read_sockets_;
	
};

#endif