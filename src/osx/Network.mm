#import "Network.h"
#import "Exit.h"
#import "ZeroMQContext.hpp"

@implementation Network

- (id) init {
  self = [super init];
  quit = false;
  
  ZeroMQContext::init();
  entrance = new Entrance();
    
  [NSThread detachNewThreadSelector:@selector(exit_thread) toTarget:self withObject:nil];
  return self;
}

- (void)on_event:(CGEventType)eventType withEvent:(CGEventRef)event {
  entrance->on_event(eventType, event);
}

- (void)connected_test {
  if (![self is_connected]) {
    [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
    CGAssociateMouseAndMouseCursorPosition(false);
    CGDisplayHideCursor(kCGDirectMainDisplay);
    [bezel_window show_bezel:true];
  }
  else {
    [bezel_window show_bezel:false];
    CGDisplayShowCursor(kCGDirectMainDisplay);
    CGAssociateMouseAndMouseCursorPosition(true);
  }  
}

- (void)connect_to:(NSString*)address withPort:(unsigned int)port {
  [self connected_test];
  entrance->connect_to([address cStringUsingEncoding:NSASCIIStringEncoding], port);
}

- (void)toggle {
  [self connected_test];
  entrance->toggle();
}

- (bool)is_connected {
  return entrance->is_enabled();
}

- (bool)understands:(CGEventType)eventType {
  return entrance->understands(eventType);
}

- (void)stop {
  quit = true; 
}

- (void)exit_thread {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  Exit exit;
	while (!quit) {
    exit.receive_input();
	}
  exit.shutdown();
  [pool release];
}

@end
