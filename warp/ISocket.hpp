#ifndef ISOCKET_HPP
#define ISOCKET_HPP

	#include <vector>

	class ISocket 
	{
		
	public:
		
		typedef std::vector<char*> received_data;
		
		virtual received_data* receive() = 0;
		
		virtual void listen_(unsigned int port) = 0;
		
		virtual void dispose(received_data* data) = 0;
		
	};

#endif