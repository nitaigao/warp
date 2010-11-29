//
//  warpAppDelegate.m
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "warpAppDelegate.h"
#import <Carbon/Carbon.h>

#include "Constants.hpp"

@implementation WarpAppDelegate

- (void)server_update {
	while (!quit) {
    exit_->receive();
    entrance->update(1000);
    
    StringList host_list = entrance->network_hosts();
    for(StringList::iterator i = host_list.begin(); i != host_list.end(); ++i) {
      [status_menu add_network_item:[[NSString alloc] initWithCString:(*i).c_str()]];
    }
	}
}

- (IBAction)show_connect:(id)sender {
	[[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
	[connect_window makeKeyAndOrderFront:self];
}

- (void)recent:(id)sender {
	NSMenuItem* menu_item = sender;
	[status_menu add_recent_item:[menu_item title]];	
	entrance->send_to([[menu_item title] cStringUsingEncoding:NSASCIIStringEncoding], SERVER_PORT);
}

- (void)refresh:(id)sender {
  [status_menu show_menu];
  entrance->search_for_exits();
}

- (IBAction)connect:(id)sender {
	[connect_window orderOut:self];
	
	if (entrance->send_to([[address stringValue] cStringUsingEncoding:NSASCIIStringEncoding], SERVER_PORT)) {	
		[status_menu add_recent_item:[address stringValue]];
	}
	else {
		[connect_window makeKeyAndOrderFront:self]; 
	}
}

- (IBAction)quit:(id)sender {
  entrance->disable();
	quit = true;
	exit(0);
}

- (void)screen_locked {
	entrance->disable();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {		

  ISocket* tcp_socket = new TCPSocket(SERVER_PORT);
  
  ISocket* m_send_socket = new MultiSocket(1724);
  m_send_socket->listen_on();
  
  ISocket* m_recv_socket = new MultiSocket(1725);
  m_recv_socket->listen_on();
	
  exit_ = new Exit(tcp_socket, m_send_socket, m_recv_socket);
	exit_->start_listening();
	
	client = new Client(tcp_socket, m_send_socket, m_recv_socket);
	[input_view set_client:client];
	
	entrance = new Entrance(client, connected_window);
	entrance->tap();
	
	[NSThread detachNewThreadSelector:@selector(server_update) toTarget:self withObject:nil];
	
	NSDistributedNotificationCenter * center = [NSDistributedNotificationCenter defaultCenter];	
	[center addObserver:self selector:@selector(screen_locked) name:@"com.apple.screenIsLocked" object:nil];
}

@end
