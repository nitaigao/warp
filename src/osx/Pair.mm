//
//  Pair.m
//  warp
//
//  Created by Nicholas Kostelnik on 23/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Pair.h"


@implementation Pair

@synthesize time;
@synthesize value;

- (id)initWithTime:(int)theTime andValue:(NSString*)theValue {
  self = [super init];
  self.time = theTime;
  self.value = theValue;
  return self;
}

@end
