//
//  iwarpAppDelegate.h
//  iwarp
//
//  Created by Nicholas Kostelnik on 27/09/2010.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class iwarpViewController;

@interface iwarpAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    iwarpViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet iwarpViewController *viewController;

@end

