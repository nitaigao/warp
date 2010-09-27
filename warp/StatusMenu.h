//
//  RecentMenu.h
//  warp
//
//  Created by Nicholas Kostelnik on 21/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface StatusMenu : NSStatusItem {
	
	IBOutlet NSMenu* main_menu;
	IBOutlet NSMenu* recent_menu;

}

- (void)add_recent_item:(NSString*)item_address;

@end
