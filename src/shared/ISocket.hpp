#ifndef ISOCKET_HPP
#define ISOCKET_HPP

	#include <vector>

	class ISocket 
	{
		
	public:
		
		typedef std::vector<char*> received_data;
		
		virtual received_data* receive() = 0;
		
		virtual void listen_on() = 0;
		
		virtual void dispose(received_data* data) = 0;

      virtual void send(const char* data, unsigned int size) = 0;
		
	};

#endif
