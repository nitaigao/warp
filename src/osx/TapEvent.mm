//
//  TapEvent.m
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TapEvent.h"


@implementation TapEvent

@synthesize type;
@synthesize event;

- (id)initWithType:(CGEventType)theType andEvent:(CGEventRef)theEvent {
  self = [super init];
  self.type = theType;
  self.event = theEvent;
  return self;
}

@end
