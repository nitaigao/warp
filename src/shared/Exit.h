#ifndef EXIT_H_
#define EXIT_H_

	#include <map>

	#include "IExitCommand.hpp"

	#include "ISocket.hpp"

	class Exit
	{
		typedef std::map<int, IExitCommand*> MessageTypeList;
				
	public:
		
		Exit(ISocket* send_socket, ISocket* m_recv_socket, ISocket* m_send_socket);

		void receive_input();
    void receive_search();
    
    void shutdown();
		
	private:
		
		ISocket* send_socket_;
    ISocket* listen_socket_;
    
    ISocket* m_send_socket_;
    ISocket* m_recv_socket_;
		
		MessageTypeList message_types_;

	};

#endif
