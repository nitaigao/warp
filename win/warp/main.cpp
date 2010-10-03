
#include <iostream>
#include <WinSock.h>
#include <vector>

#include "../../warp/Constants.hpp"
#include "../../warp/Message.h"

class WinSocket
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

	void start_listening(unsigned int port)
	{
		int iResult;
		
		WSADATA wsadata;
		iResult = WSAStartup(MAKEWORD(2,2), &wsadata);

		listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ioctlsocket(listen_socket_, FIONBIO, 0);
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
		std::vector<char*>* return_data;

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		fd_set working_socks;
		FD_ZERO(&working_socks);
		memcpy(&working_socks, &read_socks_, sizeof(read_socks_));

		int readsocks = select(max_socket_ + 1, &working_socks, NULL, NULL, &timeout);

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

						FD_SET(incoming_socket, &read_socks_);
						new_sockets.push_back(incoming_socket);
					}
					else
					{
						char buffer[256];
						ZeroMemory(buffer, 256);

						int result = recv(sock, buffer, 256, 0);

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

#include <vector>

int main()
{
	WinSocket exit;
	exit.start_listening(SERVER_PORT);

	while(true)
	{
		std::vector<char*>* datas = exit.receive();

	


	};

	return 0;
}