#ifndef BLACKHOLE_H
#define BLACKHOLE_H

	/*
	 *  BlackHole.h
	 *  warp
	 *
	 *  Created by Nicholas Kostelnik on 14/09/2010.
	 *  Copyright 2010 __MyCompanyName__. All rights reserved.
	 *
	 */

	#include <ApplicationServices/ApplicationServices.h>
	#include <Carbon/Carbon.h>
	#include <iostream>
	#include <map>

	#include "IClientCommand.h"
	#include "Client.h"

	class BlackHole
	{	
		
		typedef std::map<int, IClientCommand*> ClientCommandList;
		
	public:
		
		BlackHole(Client* client, NSWindow* window);
		
		void tap();
		
		void disable();
		void enable();
		
		bool send_to(const std::string& host, unsigned int port);
		
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