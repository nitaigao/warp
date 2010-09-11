//
//  MainView.h
//  iwarp
//
//  Created by Nicholas Kostelnik on 11/09/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Client.h"


@interface MainView : UIView {

	Client* client;
	bool moved;
	
}

@end
