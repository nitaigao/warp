//
//  ConnectWindow.h
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Network.h"

@interface ConnectWindow : NSWindowController {

  IBOutlet Network *network;
  IBOutlet NSTextField *address;
  IBOutlet NSWindowController *bezel_window;
  
}

- (IBAction) connect:(id)sender;

@end
