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
		
		Entrance(Client* client, NSWindow* window);
		
		void tap();
		
		void disable();
		void enable();
		
		bool send_to(const std::string& host, unsigned int port);
    void update(float delta);
		void search_for_exits();
    StringList network_hosts();
    
	private:
		
		static CGEventRef event_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
		CGEventRef on_event(CGEventType type, CGEventRef event);
		
		void scan_reconnect(CGEventType type, CGEventRef event);
		CGEventRef scan_input(CGEventType type, CGEventRef event);
		
		Client* client_;
		CFMachPortRef event_tap_;
		NSWindow* window_;

		ClientCommandList client_commands_;
		bool enabled_;
	};

#endif
