//
//  AgainstWarAppController.mm
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-13.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "ASIHTTPRequest.h"
#import "JSONKit.h"
#import "ASIFormDataRequest.h"
#include "CommonDef.h"
#import <CommonCrypto/CommonDigest.h>
#import "SaveData.h"
#import "NSData+Encryp.h"
#import "OcProxy.h"
#import "OrgSaveData.h"
#import "APIEngine.h"
#import <Tapjoy/Tapjoy.h>
#import "CAlipay.h"
#import <PPAppPlatformKit/PPAppPlatformKit.h>

#import "TalkingDataGA.h"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

@synthesize viewController;
// cocos2d application instance
static AppDelegate s_sharedApplication;
static CAlipay *s_alipay = NULL;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16_OES
                              preserveBackbuffer: YES
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    //[APIEngine activeEngineWithAppKey:@"3000002450" Secret:@"a8dab52764e4cf65"];
    [APIEngine activeEngineWithAppKey:@"3000002911" Secret:@"900d2562924661b0"];
    
    // Set RootViewController to window
    [window addSubview: viewController.view];
    [window makeKeyAndVisible];
    
    CGRect rct = [window bounds];
    
    if(rct.size.width == 320)
        [[UIApplication sharedApplication] setStatusBarHidden: YES];
    else
        [[UIApplication sharedApplication] setStatusBarHidden: NO];
        
    cocos2d::CCApplication::sharedApplication().run();
    
    
    //TalkingData
#ifdef TalkData
     [TalkingDataGA onStart:@"DEDD840D320A86CDA2FDE278E18CB2B0" withChannelId:@"IOS"];
#else

#endif

    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    //本地通知
    [[UIApplication sharedApplication] cancelAllLocalNotifications];//取消所有的通知
    //------通知；
    UILocalNotification *notification=[[UILocalNotification alloc] init]; 
    notification.applicationIconBadgeNumber = 0;
    
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
     (UIRemoteNotificationType )(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound |UIRemoteNotificationTypeAlert)];
    
     //tapjoy
    [Tapjoy requestTapjoyConnect:@"f0824287-8e19-4445-a3eb-8db180ec28ef"
							  secretKey:@"fuv5pYArQrFrOe0CPNfn"
								options:[NSDictionary dictionaryWithObjectsAndKeys:
										 [NSNumber numberWithInt:TJCTransitionExpand], TJC_OPTION_TRANSITION_EFFECT,
										 [NSNumber numberWithBool:YES], TJC_OPTION_ENABLE_LOGGING,
										 // If you are not using Tapjoy Managed currency, you would set your own user ID here.
										 //@"A_UNIQUE_USER_ID", TJC_OPTION_USER_ID,
										 nil]];
    
#ifdef kPltfmPp
    //------------------------------------pp-------------------------------------------//
    [[PPAppPlatformKit sharedInstance] setDelegate:viewController];
    [[PPAppPlatformKit sharedInstance] setAppId:2765 AppKey:@"b96ea48610eba74e10913233f87f556f"];
    [[PPAppPlatformKit sharedInstance] setIsNSlogData:YES];
    [[PPAppPlatformKit sharedInstance] setRechargeAmount:10];
    [[PPAppPlatformKit sharedInstance] setIsLongComet:YES];
    [[PPAppPlatformKit sharedInstance] setIsLogOutPushLoginView:YES];
    [[PPAppPlatformKit sharedInstance] setIsOpenRecharge:YES];
    [[PPAppPlatformKit sharedInstance] setCloseRechargeAlertMessage:@"关闭充值提示语"];
    [PPUIKit sharedInstance];
    
    
    [PPUIKit setIsDeviceOrientationLandscapeLeft:YES];
    [PPUIKit setIsDeviceOrientationLandscapeRight:YES];
    [PPUIKit setIsDeviceOrientationPortrait:YES];
    [PPUIKit setIsDeviceOrientationPortraitUpsideDown:YES];
    
    //------------------------------------pp-------------------------------------------//
#endif
    return YES;
}

//crashController,发邮件
- (void)onCrash
{
    if(!isCrash)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"crashTitle", NULL)
                                                        message:NSLocalizedString(@"crashInfo", NULL)
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"sureTip", NULL)
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
        isCrash = true;
    }
}


//解码
- (NSData *)base64Decode:(NSString *)base64String
{
    NSData *plainTextData = [NSData dataFromBase64String:base64String];
  
    return plainTextData;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    
    CCNotificationCenter::sharedNotifCenter()->postNotification("CHECK_TOUCH_MOVE_INTERRUPT");
    
	cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
	cocos2d::CCDirector::sharedDirector()->resume();
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;

}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    
    cocos2d::CCDirector::sharedDirector()->stopAnimation();
    cocos2d::CCApplication::sharedApplication().applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCDirector::sharedDirector()->startAnimation();
    cocos2d::CCApplication::sharedApplication().applicationWillEnterForeground();
}

//推送
- (void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    
    NSLog(@"My token is: %@", deviceToken);
    
    NSString *pushToken = [[[[deviceToken description]
                             stringByReplacingOccurrencesOfString:@"<" withString:@""]
                            stringByReplacingOccurrencesOfString:@">" withString:@""]
                           stringByReplacingOccurrencesOfString:@" " withString:@""] ;
    OcProxy::Inst()->setDeviceToken([pushToken UTF8String]);
}

- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
     NSLog(@"Failed to get token, error: %@", error);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //UIApplicationState state = [application applicationState];
    
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return NO;
}
- (void)dealloc {
    [super dealloc];
}




////////////////////////微信息

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    if ([ [ url absoluteString] rangeOfString: @"wx5e2fd0219cc58026" ].location !=NSNotFound) {
        return [WXApi handleOpenURL:url delegate:self];
    }else if ([ [ url absoluteString] rangeOfString: @"teiron2765" ].location !=NSNotFound)  {
        [[PPAppPlatformKit sharedInstance] alixPayResult:url];
        return YES;
    } else{
        if(s_alipay == NULL)
        {
            s_alipay =  [[CAlipay alloc] init];
        }
        int rCode = [s_alipay parseURL:url application:application];
        
        OcProxy::Inst()->setPayBack(rCode);
        
        return YES;
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    if (!url)
        return NO;
    
    if ([ [ url absoluteString] rangeOfString: @"wx5e2fd0219cc58026" ].location !=NSNotFound) {
        return [WXApi handleOpenURL:url delegate:self];
    } else if ([ [ url absoluteString] rangeOfString: @"teiron2765" ].location !=NSNotFound)  {
        [[PPAppPlatformKit sharedInstance] alixPayResult:url];
        return YES;
    } else{
        if(s_alipay == NULL)
        {
            s_alipay =  [[CAlipay alloc] init];
        }
        int rCode = [s_alipay parseURL:url application:application];
        
        OcProxy::Inst()->setPayBack(rCode);
        
        return YES;
    }
}

-(void) onReq:(BaseReq*)req
{
    
    
}

-(void) onResp:(BaseResp*)resp
{
    if([resp isKindOfClass:[SendMessageToWXResp class]])
    {
        OcProxy::Inst()->setweixinback(resp.errCode);
        
    }
}




@end

