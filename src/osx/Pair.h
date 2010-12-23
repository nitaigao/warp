//
//  Pair.h
//  warp
//
//  Created by Nicholas Kostelnik on 23/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Pair : NSObject {

  int time;
  NSString* value;
  
}

@property(nonatomic, assign) int time;
@property(nonatomic, assign) NSString* value;

- (id)initWithTime:(int)theTime andValue:(NSString*)value;

@end
