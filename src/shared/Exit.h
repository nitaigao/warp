#ifndef SERVER_H_
#define SERVER_H_

	#include <map>

	#include "IExitCommand.hpp"

	#include "ISocket.hpp"

	class Exit
	{
		typedef std::map<int, IExitCommand*> MessageTypeList;
				
	public:
		
		Exit(unsigned int port);

		void start_listening();
		
		bool receive();
		
	private:
		
		ISocket* send_socket_;
    ISocket* listen_socket_;
		
		MessageTypeList message_types_;

	};

#endif
