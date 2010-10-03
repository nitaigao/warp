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
#include <stdio.h>
#include <errno.h>

#include <vector>

class Socket 
{
	
public:
	
	Socket() { };
	
	void listen_(unsigned int port)
	{
		listen_sock_ = create_socket(port);
		max_socket_ = listen_sock_;
		FD_SET(listen_sock_, &read_sockets_);
		
		listen(listen_sock_, 5);
		
		std::clog << "listening on port: " << port << std::endl;
	}
	
	int open_socket()
	{    
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		
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
	
	
	int create_socket(unsigned int port)
	{
		int sock = open_socket();
		set_non_blocking(sock);
		bind_socket(sock, port);
		
		return sock;    
	}
	
	typedef std::vector<char*> received_data;
	
	
	received_data* receive()
	{
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
							return_data->push_back(buffer);
						}
					}
				}
			}
		}
		
		return return_data;
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
		for (Socket::received_data::iterator i = data->begin(); i != data->end(); ++i) 
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
	fd_set read_sockets_;
	
};

#endif