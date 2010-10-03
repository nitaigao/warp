#ifndef WINSOCKET_H
#define WINSOCKET_H

	#include <iostream>
	#include <WinSock.h>
	#include <vector>

  #include "ISocket.hpp"

  class WinSocket : public ISocket
  {

  	int listen_socket_;
  	int max_socket_;
  	int client_socket_;

  	fd_set read_socks_;

  public:

  	WinSocket()
  	{
  		FD_ZERO(&read_socks_);
  	}

	void dispose(received_data* data)
	{

	}

  	void listen_(unsigned int port)
  	{
  		int iResult;
		
  		WSADATA wsadata;
  		iResult = WSAStartup(MAKEWORD(2,2), &wsadata);

  		listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  		
		unsigned long block = 1;
		ioctlsocket(listen_socket_, FIONBIO, &block);
  		active_sockets_.push_back(listen_socket_);
  		FD_SET(listen_socket_, &read_socks_);

  		if (listen_socket_ == INVALID_SOCKET)
  		{
  			std::cerr << "ERROR creating socket: " << WSAGetLastError() << std::endl;
  		}

  		struct sockaddr_in serv_addr;
  		serv_addr.sin_family = AF_INET;
  		serv_addr.sin_addr.s_addr = INADDR_ANY;
  		serv_addr.sin_port = htons(port);

  		if (bind(listen_socket_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  		{
  			std::cerr << "ERROR on binding" << std::endl;
  		}

  		if(listen(listen_socket_, 5) == SOCKET_ERROR)
  		{
  			std::cerr << "ERROR on listen" << std::endl;
  		}

  		//client_socket_ = accept(listen_socket_, NULL, NULL);
  	}

  	typedef std::vector<int> SocketList;
	
  	SocketList active_sockets_;

  	std::vector<char*>* receive()
  	{
		//std::clog << "tick" << std::endl;
  		std::vector<char*>* return_data = new std::vector<char*>();

  		struct timeval timeout;
  		timeout.tv_sec = 1;
  		timeout.tv_usec = 0;

  		fd_set working_socks;
  		FD_ZERO(&working_socks);
  		memcpy(&working_socks, &read_socks_, sizeof(read_socks_));

  		int readsocks = select(0, &working_socks, NULL, NULL, &timeout);

  		if (readsocks < 0)
  		{
  			std::cerr << "ERROR, on accept" << std::endl;
  		}

  		SocketList new_sockets;
  		new_sockets = active_sockets_;

  		if (readsocks > 0)
  		{
  			for (SocketList::iterator i = active_sockets_.begin(); i != active_sockets_.end(); ++i)
  			{
  				int sock = (*i);

  				if (FD_ISSET(sock, &working_socks))
  				{
  					if (sock == listen_socket_)
  					{
  						int incoming_socket = accept(listen_socket_, NULL, NULL);

  						if (incoming_socket < 0)
  						{
  							std::cerr << "ERROR, accepting incoming connection" << std::endl;
  							continue;
  						}

  						std::clog << "Client connected" << std::endl;

						unsigned long block = 1;
						ioctlsocket(incoming_socket, FIONBIO, &block);
  						FD_SET(incoming_socket, &read_socks_);
  						new_sockets.push_back(incoming_socket);
  					}
  					else
  					{
						int size = sizeof(Message);
  						char* buffer = new char[size];
  						ZeroMemory(buffer, size);

  						int result = recv(sock, buffer, size, 0);

  						if (result < 0)
  						{
  							std::cerr << "ERROR reading from socket" << std::endl;
  						}

  						if (result == 0)
  						{
  							shutdown(sock, 5);
  							FD_CLR(sock, &read_socks_);
  							std::clog << "disconnect" << std::endl;
  						}

  						if (result > 0)
  						{
  							return_data->push_back(buffer);
  						}
  					}
  				}
  			}
  		}

  		active_sockets_ = new_sockets;

  		return return_data;
  	}
  };
  
#endif