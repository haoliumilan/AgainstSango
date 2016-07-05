//
//  SinaApiWrapper.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_SinaApiWrapper_h
#define AgainstWar_SinaApiWrapper_h

#import "WBEngine.h"
#import "WBSendView.h"
#import "WBLogInAlertView.h"

#define SINA_APP_ID         @"1399456723" 
#define SINA_SECRET_KEY5    @"ab5598d3f0"

#define SINA_SECRET_KEY2    @"20ce951b7"

#define SINA_SECRET_KEY1    @"aa0f1"

#define SINA_SECRET_KEY4    @"b3dc8"

#define SINA_SECRET_KEY3    @"c8b"

@interface SinaApiWrapper : NSObject<WBEngineDelegate>
{
    WBEngine *weiBoEngine;
    
    NSString* openId;
    NSString* accessToken;
    
    int iLoginStatus;
    bool isGachaWeibo;
}

@property (nonatomic, retain) WBEngine *weiBoEngine;
@property (nonatomic, readonly) NSString* openId;
@property (nonatomic, readonly) NSString* accessToken;
@property (nonatomic, readwrite) int iLoginStatus;
@property (nonatomic, readwrite) bool isGachaWeibo;
-(void) Login;
-(void) sendWeiboWithText:(NSString *)str WithImageExist:(bool) isImageExist;

@end

#endif
