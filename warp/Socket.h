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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

class Socket 
{
	
public:
	
	Socket() { };
	
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
	
	bool connect_to(const std::string& host, unsigned int port)
	{
		struct hostent *server = gethostbyname(host.c_str());
		
		if (server == NULL)
		{
			std::cerr << "ERROR, no such host" << std::endl;
			return false;
		}
		
		struct sockaddr_in server_address;
		bzero((char *) &server_address, sizeof(server_address));
		server_address.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
		server_address.sin_port = htons(port);
		
		socket_ = socket(AF_INET, SOCK_STREAM, 0);
		
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
	
};

#endif