//
//  RecentMenu.h
//  warp
//
//  Created by Nicholas Kostelnik on 21/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "BezelWindow.h"

@interface StatusMenu : NSObject {
	
	IBOutlet NSMenu* main_menu;
	IBOutlet NSMenu* recent_menu;

  IBOutlet NSMenuItem* network_item;
  IBOutlet NSMenuItem* network_seperator_item;
  
  NSMutableArray* network_items;
  
  NSStatusItem* statusItem;
  bool is_open;
  id delegate;
}

- (void)set_delegate:(id)theDelegate;

- (IBAction)quit:(id)sender;
- (IBAction)recent:(id)sender;

- (void)add_recent_item:(NSString*)item_address;
- (void)add_network_item:(NSString*)item_address time:(int)time;
- (void)remove_network_item:(NSString*)item_address;


- (void)update:(int)delta_milliseconds;

- (void)show_menu;

- (void)start_searching;
- (void)stop_searching;

@end
