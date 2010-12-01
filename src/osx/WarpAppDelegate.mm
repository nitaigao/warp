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

- (void)entrance_input_update {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    entrance->update_input(1000);
    
    StringList host_list = entrance->network_hosts();
    for(StringList::iterator i = host_list.begin(); i != host_list.end(); ++i) {
      NSString* host = [[NSString alloc] initWithCString:(*i).c_str()];
      [self performSelectorOnMainThread:@selector(add_network_item:) withObject:host waitUntilDone:FALSE];
      [host release];
    }
    
    [NSThread sleepForTimeInterval:1];
	}
  entrance->disable();
  [pool release]; 
}

- (void)add_network_item:(NSString*)host {
  [status_menu add_network_item:host];
}

- (void)entrance_search_update {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    entrance->update_search();
  }
  entrance->disable();
  [pool release]; 
}

- (void)exit_input_update {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    exit_->receive_input();
	}
  exit_->shutdown();
  [pool release];
}

- (void)exit_search_update {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    exit_->receive_search();
	}
  exit_->shutdown();
  [pool release];
}

- (void)continue_search {
  if ([status_menu isOpen]) {
    [NSThread detachNewThreadSelector:@selector(search_for_exits) toTarget:self withObject:nil];
  }
}

- (void) show_start_searching {
	[status_menu start_searching];
}

- (void) show_stop_searching {
	[status_menu stop_searching];
}

- (void)search_for_exits {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  if (search_count++ < 5) {
		[self performSelectorOnMainThread:@selector(show_start_searching) withObject:nil waitUntilDone:FALSE];
    [NSThread sleepForTimeInterval:1];
    entrance->search_for_exits();
    [self performSelectorOnMainThread:@selector(continue_search) withObject:nil waitUntilDone:FALSE];
  } 
  else {
		[self performSelectorOnMainThread:@selector(show_stop_searching) withObject:nil waitUntilDone:FALSE];
    [NSThread sleepForTimeInterval:5];
    [self performSelectorOnMainThread:@selector(continue_search) withObject:nil waitUntilDone:FALSE];
    search_count = 0;
  }
	[pool release];
}

- (void)refresh:(id)sender {
	search_count = 0;
  [NSThread detachNewThreadSelector:@selector(search_for_exits) toTarget:self withObject:nil];
  [status_menu show_menu];
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
  quit = true;
  [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

- (void)screen_locked {
	entrance->disable();
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {		

  ISocket* tcp_socket = new TCPSocket(SERVER_PORT);
  tcp_socket->listen_on();
  
  ISocket* m_send_socket = new MultiSocket(1724);
  m_send_socket->listen_on();
  
  ISocket* m_recv_socket = new MultiSocket(1725);
  m_recv_socket->listen_on();
	
  exit_ = new Exit(tcp_socket, m_send_socket, m_recv_socket);
	
	client = new Client(tcp_socket, m_send_socket, m_recv_socket);
	[input_view set_client:client];
	
	entrance = new Entrance(client, connected_window);
	entrance->tap();
  
  [NSThread detachNewThreadSelector:@selector(entrance_input_update) toTarget:self withObject:nil];
  [NSThread detachNewThreadSelector:@selector(entrance_search_update) toTarget:self withObject:nil];

  [NSThread detachNewThreadSelector:@selector(exit_input_update) toTarget:self withObject:nil];
  [NSThread detachNewThreadSelector:@selector(exit_search_update) toTarget:self withObject:nil];

	
	NSDistributedNotificationCenter * center = [NSDistributedNotificationCenter defaultCenter];	
	[center addObserver:self selector:@selector(screen_locked) name:@"com.apple.screenIsLocked" object:nil];
}

@end
