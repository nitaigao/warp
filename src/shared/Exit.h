#ifndef EXIT_H_
#define EXIT_H_

	#include <map>

	#include "IExitCommand.hpp"
  #include "IRecvSocket.hpp"
  
	class Exit
	{
		typedef std::map<int, IExitCommand*> MessageTypeList;
				
	public:
		    
    Exit();

		void receive_input();
    void receive_search();
    
    void shutdown();
		
	private:

		IRecvSocket* exit_socket_;
		MessageTypeList message_types_;

	};

#endif
