#ifndef SERVER_H_
#define SERVER_H_

	#include <map>

	#include "IExitCommand.hpp"

	#include "ISocket.hpp"

	class Exit
	{
		typedef std::map<int, IExitCommand*> MessageTypeList;
				
	public:
		
		Exit();

		void start_listening(unsigned int port);
		
		bool receive();
		
	private:
		
		ISocket* socket_;
		
		MessageTypeList message_types_;

	};

#endif
