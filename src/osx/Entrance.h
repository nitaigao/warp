#ifndef ENTRANCE_H
#define ENTRANCE_H

	#include <ApplicationServices/ApplicationServices.h>
	#include <Carbon/Carbon.h>
	#include <iostream>
	#include <map>

	#include "IClientCommand.h"
	#include "Client.h"

  typedef std::vector<std::string> StringList; 

	class Entrance
	{	
		
		typedef std::map<int, IClientCommand*> ClientCommandList;
		
	public:
		
		Entrance();
    
    bool understands(const CGEventType& event_type);
    void on_event(CGEventType type, CGEventRef event);
		bool connect_to(const std::string& host, unsigned int port);
    
		void disable();
		void enable();
		
	private:		
		
    void scan_reconnect(CGEventType type, CGEventRef event);
		CGEventRef scan_input(CGEventType type, CGEventRef event);
		
		Client* client_;
		
		ClientCommandList client_commands_;
		bool enabled_;
    
	};

#endif
