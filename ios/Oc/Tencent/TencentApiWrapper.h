//
//  TencentApiWrapper.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OpenSdkOauth.h"
#import "OpenApi.h"

/*
 * 获取oauth2.0票据的key
 */
#define oauth2TokenKey @"access_token="
#define oauth2OpenidKey @"openid="
#define oauth2OpenkeyKey @"openkey="
#define oauth2ExpireInKey @"expire_in="


@interface TencentApiWrapper : NSObject<UIWebViewDelegate>
{
    OpenSdkOauth *qqWeiboOauth;
    UIView *qqAuthView;
    UIWebView *qqAuthWeb;
    UIActivityIndicatorView *qqWeiboLoadIndicator;
    OpenApi *m_openApi;
    int loginStatus;
    bool isGachaWeibo;
}
@property(nonatomic,readonly) int loginStatus;
@property(nonatomic,readonly) NSString *m_openId;
@property(nonatomic,readonly) NSString *m_accessToken;
@property(nonatomic,readwrite) bool isGachaWeibo;

- (id)init;
- (void)login;
- (void) webViewShow;
- (void) backFromQQLogin;
-(void) sendWeiboWithText:(NSString *)str WithImageExist:(bool) isImageExist;

@end
