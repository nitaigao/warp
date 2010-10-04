//
//  MainView.h
//  warp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "Client.h"


@interface MainView : NSView {
	
	Client* client;
	bool input_allowed;

}

- (void)allow_input:(bool)allowed;
- (void)set_client:(Client*) the_client;

@end
