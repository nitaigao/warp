//
//  RecentMenu.h
//  warp
//
//  Created by Nicholas Kostelnik on 21/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface StatusMenu : NSObject {
	
	IBOutlet NSMenu* main_menu;
	IBOutlet NSMenu* recent_menu;
  IBOutlet NSMenuItem* network_item;
  IBOutlet NSMenuItem* network_seperator_item;
  
  NSStatusItem* statusItem;
  bool is_open;

}

- (void)add_recent_item:(NSString*)item_address;
- (void)add_network_item:(NSString*)item_address;
- (void)show_menu;
- (bool)isOpen;

- (void)start_searching;
- (void)stop_searching;

@end
