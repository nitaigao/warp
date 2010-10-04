//
//  iwarpViewController.h
//  iwarp
//
//  Created by Nicholas Kostelnik on 27/09/2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "Client.h"

@interface iwarpViewController : UIViewController {
	
	IBOutlet UITextField* addressField;
	IBOutlet UIBarButtonItem* connectButton;
	IBOutlet UIView* connectView;
	IBOutlet UIView* sessionView;
	IBOutlet UITextField* textView;
	
	Client* client;

}

- (IBAction)connect:(id)sender;

@end

