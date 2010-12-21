//
//  Network.m
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Network.h"


@implementation Network

- (id) init {
  self = [super init];
  quit = false;
  
  ZeroMQContext::init();
  entrance = new Entrance();
  exit = new Exit();  
    
  [NSThread detachNewThreadSelector:@selector(exit_input_update) toTarget:self withObject:nil];
  return self;
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event {
  entrance->on_event(eventType, event);
}

- (void)connect_to:(NSString*)address withPort:(unsigned int)port {
  entrance->connect_to([address cStringUsingEncoding:NSASCIIStringEncoding], port);
}

- (bool)understands:(CGEventType)eventType {
  return entrance->understands(eventType);
}

- (void)stop {
  quit = true; 
}

- (void)exit_input_update {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (!quit) {
    exit->receive_input();
	}
  exit->shutdown();
  [pool release];
}

@end
