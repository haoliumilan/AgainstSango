//
//  TapJoyCenter.m
//  AgainstWar
//
//  Created by 海桅 王 on 12-9-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import "TapJoyCenter.h"
#import <Tapjoy/Tapjoy.h>
#import "AppController.h"

static TapJoyCenter *m_tapJoy = NULL;
@implementation TapJoyCenter

@synthesize tapJoyRepStatus;
+ (TapJoyCenter *)shareInstance
{
    if(m_tapJoy == NULL)
    {
        m_tapJoy = [[TapJoyCenter alloc] init];
        
    }
    return m_tapJoy;
}


- (id)init
{
    if([super init])
    {
        isRequseting = false;
        tapJoyRepStatus = 0;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(backToGame) name:TJC_VIEW_CLOSED_NOTIFICATION object:nil];
        return self;
    }
    return nil;
}

- (void)getTapPoint
{
    
    [[NSNotificationCenter defaultCenter] addObserver:self 
                                             selector:@selector(getUpdatedPoints:) 
                                                 name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION          
                                               object:nil];

    [Tapjoy getTapPoints];
    isRequseting =true;
}

- (void)getUpdatedPoints:(NSNotification*)notifyObj
{
    isRequseting = false;
    //NSNumber *tapPoints = notifyObj.object;
	//NSString *tapPointsStr = [NSString stringWithFormat:@"Tap Points: %d", [tapPoints intValue]];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:TJC_TAP_POINTS_RESPONSE_NOTIFICATION object:nil];
    
}

- (void)setUserId:(NSString *)uid
{
    [Tapjoy setUserID:uid];
}


- (void)backToGame
{
    tapJoyRepStatus = 2;
}

- (void)resetTapjoyStatus
{
    tapJoyRepStatus = 0;
}


//显示tapjoy的积分墙
-(void) showOfferWall
{
    AppController *appController =(AppController *) [UIApplication sharedApplication].delegate;
    [Tapjoy showOffersWithViewController:(UIViewController *)appController.viewController];
    tapJoyRepStatus = 1;
    [self getTapPoint];
}
@end
