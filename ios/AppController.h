//
//  AgainstWarAppController.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-13.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#include "CrashController.h"
#import "WXApi.h"

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate,CrashSaveDelegate,WXApiDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
    bool isCrash;
}

@property (nonatomic,retain) RootViewController	*viewController;

@end

