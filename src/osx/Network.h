//
//  Network.h
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "ZeroMQContext.hpp"
#import "Entrance.h"
#import "Exit.h"

@interface Network : NSObject {

  bool quit;
  
  IBOutlet Entrance* entrance;
  Exit *exit;
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event;
- (void)connect_to:(NSString*)address withPort:(unsigned int)port;
- (bool)understands:(CGEventType)eventType;

- (void)stop;
- (void)exit_input_update;

@end
