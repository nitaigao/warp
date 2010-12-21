//
//  TapEvent.h
//  warp
//
//  Created by Nicholas Kostelnik on 20/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface TapEvent : NSObject {

  CGEventType type;
  CGEventRef event;
  
}

- (id)initWithType:(CGEventType)theType andEvent:(CGEventRef)theEvent;

@property(nonatomic) CGEventType type;
@property(nonatomic) CGEventRef event;

@end
