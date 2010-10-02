#ifndef SERVER_H_
#define SERVER_H_

	#include <iostream>
	#include <sstream>
	#include <string>

	#include <stdio.h>
	#include <sys/types.h> 
	#include <sys/socket.h>
	#include <netinet/in.h>

	#include <ApplicationServices/ApplicationServices.h>

	#include <map>

	#include "Message.h"
	#include "IServerCommand.h"


	class Server
	{
		typedef std::map<int, IServerCommand*> MessageTypeList;
		
		int max_socket_;
		int listen_sock_;
		fd_set read_sockets_;
		
		MessageTypeList message_types_;
		
	public:
		
		Server()
		{		
			message_types_[LEFT_UP]						= new LeftUpCommand();
			message_types_[LEFT_DOWN]					= new LeftDownCommand();
			message_types_[RIGHT_UP]					= new RightUpCommand();
			message_types_[RIGHT_DOWN]				= new RightDownCommand();
			message_types_[KEY_UP]						= new KeyUpCommand();
			message_types_[KEY_DOWN]					= new KeyDownCommand();
			message_types_[MOUSE_MOVED]				= new MouseMovedCommand();
			message_types_[LEFT_DRAGGED]			= new LeftDraggedCommand();
			message_types_[RIGHT_DRAGGED]			= new RightDraggedCommand();
			message_types_[FLAGS_CHANGED]			= new FlagsChangedCommand();
			message_types_[SCROLL_WHEEL]			= new ScrollWheelCommand();
			message_types_[LEFT_DOUBLE_CLICK]	= new LeftDoubleClickCommand();
		}

		void start_listening(unsigned int port)
		{  
			std::clog << "enabled" << std::endl;
			listen_sock_ = create_socket(port);
			max_socket_ = listen_sock_;
			FD_SET(listen_sock_, &read_sockets_);
			
			listen(listen_sock_, 5);
		};
		
		bool receive() 
		{ 
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
				for (int i = 0; i < max_socket_ + 1; i++)
				{      
					if (FD_ISSET(i, &working_socks))
					{
						if (i == listen_sock_)
						{
							int incoming_socket = accept(listen_sock_, NULL, NULL);

							if (incoming_socket < 0)
							{							
								std::cerr << "ERROR accepting incoming connection: " << strerror(errno) << std::endl;
								
								return false;
							}

							FD_SET(incoming_socket, &read_sockets_);
							max_socket_ = incoming_socket;
						}
						else
						{       
							char buffer[256];
							bzero(buffer, 256);
						
							int result = read(i, buffer, sizeof(buffer));
				
							if (result < 0)
							{ 
								
								std::cerr << "ERROR reading from socket" << strerror(errno) << std::endl;
							}
							
							if (result == 0)
							{
								close(i);
								shutdown(i, 5);
								FD_CLR(i, &read_sockets_);
							}
						
							if (result > 0)
							{
								Message message;
								memcpy(&message, &buffer, sizeof(Message));
								
								if (message_types_.find(message.type) != message_types_.end())
								{							
									message_types_[message.type]->Execute(message);
								}
							}
						}
					}
				}
			}
			
			return true;
		};
		
	private:
		
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
		
		int open_socket()
		{    
			int sock = socket(AF_INET, SOCK_STREAM, 0);

			if (sock < 0)
			{
				std::cerr << "ERROR opening socket" << std::endl;
			}
			
			return sock;
		}
		
		int open_clear_socket()
		{
			int sock = open_socket();
			shutdown(sock, 1);
			return open_socket();
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
			int sock = open_clear_socket();
			set_non_blocking(sock);
			bind_socket(sock, port);
			return sock;    
		}
			
	};

#endif