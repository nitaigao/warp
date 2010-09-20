/*
 *  Client.cpp
 *  warp
 *
 *  Created by Nicholas Kostelnik on 11/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Client.h"

#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <ApplicationServices/ApplicationServices.h>

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
}

bool Client::connec(const std::string& host, unsigned int port)
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

	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	set_non_blocking(sock);
	
	if (sock < 0)
  {
    std::cerr << "ERROR, unable to create socket" << std::endl;
		return false;
  }
	
	fd_set outgoing; 
	FD_ZERO(&outgoing); 
	FD_SET(sock, &outgoing); 

	setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, (void *)&outgoing, sizeof(int));	
	connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));
	
	struct timeval tv; 
	tv.tv_sec = 3; 
	tv.tv_usec = 0; 
	
	int result = select(sock + 1, NULL, &outgoing, NULL, &tv);
	
	if (result > 0)
	{
		socklen_t lon = sizeof(int);
		int valopt;
		
		if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) 
		{ 
			std::cerr << "ERROR, can't get socket options" << std::endl; 
		} 
		else if (valopt) 
		{ 
			std::cerr << "ERROR, connection refused" << std::endl;
		}
		else 
		{
			last_host_ = host;
			connected_ = true;
		}
	}
	
	return connected_;
}

void Client::reconnect()
{
	this->connec(last_host_, 6345);
}

void Client::disconnect()
{
	connected_ = false;
	shutdown(sock, 2);
}

int Client::send_message(const Message& message)
{	
	if (connected_)
	{
		char* data = new char[sizeof(Message)];
		memcpy(data, &message, sizeof(Message));
		int result = write(sock, data, sizeof(Message));

		if (result < 0)
		{
			std::cerr << "ERROR writing to socket" << std::endl;
			disconnect();
		}
		
		return result;
	}
	
	return 0;
}

int Client::send_left_double_click()
{
	Message message;
	message.type = LEFT_DOUBLE_CLICK;
	return send_message(message);
}

int Client::send_left_down()
{
	Message message;
	message.type = LEFT_DOWN;
	return send_message(message);
}

int Client::send_key_down(unsigned int flags, int key_code)
{
	Message message;
	message.type = KEY_DOWN;
	message.key_code = key_code;
	message.flags = flags;
	return send_message(message);
}

int Client::send_key_up(unsigned int flags, int key_code)
{
	Message message;
	message.type = KEY_UP;
	message.key_code = key_code;
	message.flags = flags;
	return send_message(message);
}

int Client::send_right_down()
{
	Message message;
	message.type = RIGHT_DOWN;
	return send_message(message);
}

int Client::send_left_up()
{
	Message message;
	message.type = LEFT_UP;
	return send_message(message);
}

int Client::send_right_up()
{
	Message message;
	message.type = RIGHT_UP;
	return send_message(message);
}

int Client::send_mouse_moved(int x, int y)
{
	Message message;
	message.type = MOUSE_MOVED;
	message.x = x;
	message.y = y;
	return send_message(message);
}

int Client::send_left_dragged(int x, int y)
{
	Message message;
	message.type = LEFT_DRAGGED;
	message.x = x;
	message.y = y;
	return send_message(message);
}

int Client::send_right_dragged(int x, int y)
{
	Message message;
	message.type = RIGHT_DRAGGED;
	message.x = x;
	message.y = y;
	return send_message(message);
}

int Client::send_flags(int key_code, unsigned int flags)
{
	Message message;
	message.type = FLAGS_CHANGED;
	message.key_code = key_code;
	message.flags = flags;
	return send_message(message);
}

int Client::send_scroll_wheel(int x, int y)
{
	Message message;
	message.type = SCROLL_WHEEL;
	message.x = x;
	message.y = y;	
	return send_message(message);
}