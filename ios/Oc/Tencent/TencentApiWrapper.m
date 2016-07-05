//
//  TencentApiWrapper.m
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import "TencentApiWrapper.h"

@implementation TencentApiWrapper
@synthesize loginStatus;
@synthesize m_openId;
@synthesize m_accessToken;
@synthesize isGachaWeibo;

- (id)init
{
    if(self == [super init])
    {
        loginStatus = 0;
        m_openApi = nil;
        qqWeiboOauth = nil;
    }
    return self;
}

- (void)dealloc
{
    if (qqWeiboOauth) {
        [qqWeiboOauth release];
        qqWeiboOauth = nil;
    }
        
    [super dealloc];
}


- (void)login
{
    uint16_t authorizeType = InWebView;
    loginStatus = 1;
    if(qqWeiboOauth == nil)
    {
        qqWeiboOauth = [[OpenSdkOauth alloc] initAppKey:[OpenSdkBase getAppKey] appSecret:[OpenSdkBase getAppSecret]];
    }
    
    qqWeiboOauth.oauthType = authorizeType;
    BOOL didOpenOtherApp = NO;
    switch (authorizeType) {
        case InSafari:  //浏览器方式登录授权，不建议使用
        {
            didOpenOtherApp = [qqWeiboOauth doSafariAuthorize:didOpenOtherApp];
            break;
        }
        case InWebView:  //webView方式登录授权，采用oauth2.0授权鉴权方式
        {
            if(!didOpenOtherApp){
                if (([OpenSdkBase getAppKey] == (NSString *)[NSNull null]) || ([OpenSdkBase getAppKey].length == 0)) {
                    [OpenSdkBase showMessageBox:@"client_id为空，请到OPenSdkBase中填写您应用的appkey"];
                }
                else
                {
                    [self webViewShow];
                }
                [qqWeiboOauth doWebViewAuthorize:qqAuthWeb];
                
                break;
            }
        }
        default:
            break;
    }

}


//******************************************************************************
// webViewShow
//******************************************************************************
- (void) webViewShow
{
    if(qqAuthView == nil)
    {
        UIWindow* window = [UIApplication sharedApplication].keyWindow;
        if (!window) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        float  deltaH=0;
        CGRect rect = [window bounds];
        if(rect.size.height == 568)
            deltaH = 44;
        else
            deltaH = 0;
        
        UIView *bar = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 44)];
        [bar setBackgroundColor:[UIColor blackColor]];
        
        UIButton *btn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [btn setFrame:CGRectMake(15, 7, 60, 30)];
        [btn setTitle:@"返回" forState:UIControlStateNormal];
        [btn addTarget:self action:@selector(backFromQQLogin) forControlEvents:UIControlEventTouchUpInside];
        
        qqAuthView = [[UIView alloc] initWithFrame:CGRectMake(0, 0+deltaH, 320, 480)];
        
        qqAuthWeb = [[UIWebView alloc] initWithFrame:CGRectMake(0,44, 320, 436)];
        qqAuthWeb.scalesPageToFit = YES;
        qqAuthWeb.userInteractionEnabled = YES;
        qqAuthWeb.delegate = self;
        qqAuthWeb.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        
        qqWeiboLoadIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        [qqWeiboLoadIndicator setFrame:CGRectMake(140, 240, 40, 40)];
        [qqWeiboLoadIndicator startAnimating];
        
        [qqAuthView addSubview:qqAuthWeb];
        [qqAuthView addSubview:bar];
        [qqAuthView addSubview:btn];
        [qqAuthView addSubview:qqWeiboLoadIndicator];
        [bar release];
        [qqAuthWeb release];
        [qqWeiboLoadIndicator release];
        [window addSubview:qqAuthView];
        [qqAuthView release];
        NSString *deviceModel = [NSString stringWithFormat:@"%@",[[UIDevice currentDevice] model]];
        
        if([[deviceModel substringToIndex:4] isEqualToString:@"iPad"])
        {
            qqAuthView.frame = CGRectMake(224, 262, 320, 480);
        };
    }
}


#pragma qqWebView delegate
//******************************************************************************
// webView   * 当前网页视图被指示载入内容时得到通知，返回yes开始进行加载
//******************************************************************************
- (BOOL) webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSURL* url = request.URL;
	NSRange start = [[url absoluteString] rangeOfString:oauth2TokenKey];
    //如果找到tokenkey,就获取其他key的value值
	if (start.location != NSNotFound)
	{
        NSString *accessToken = [OpenSdkBase getStringFromUrl:[url absoluteString] needle:oauth2TokenKey];
        NSString *openid = [OpenSdkBase getStringFromUrl:[url absoluteString] needle:oauth2OpenidKey];
        NSString *openkey = [OpenSdkBase getStringFromUrl:[url absoluteString] needle:oauth2OpenkeyKey];
		NSString *expireIn = [OpenSdkBase getStringFromUrl:[url absoluteString] needle:oauth2ExpireInKey];
        
        m_openId = [NSString stringWithString:openid];
        m_accessToken = [NSString stringWithString:accessToken];
        
        //保存到本地数据
        NSString* platform = [[NSUserDefaults standardUserDefaults] objectForKey:@"platform"];
        if ([platform isEqualToString:@"oc"])
        {
            //绑定
//            [self bindRequestWithOpenid:openid andAccessToken:accessToken andPlatform:@"qq" andDate:nil];
        }
        else
        {
            [[NSUserDefaults standardUserDefaults] setObject:@"qq" forKey:@"platform"];
            [[NSUserDefaults standardUserDefaults] setObject:openid forKey:@"openid"];
            [[NSUserDefaults standardUserDefaults] setObject:accessToken forKey:@"access_token"];
            [[NSUserDefaults standardUserDefaults] setObject:expireIn forKey:@"acessdate"];
            
         //   [self QQOrSinaLoginFinished];
            
        }
        
		NSDate *expirationDate =nil;
		if (qqWeiboOauth.expireIn != nil) {
			int expVal = [qqWeiboOauth.expireIn intValue];
			if (expVal == 0) {
				expirationDate = [NSDate distantFuture];
			} else {
				expirationDate = [NSDate dateWithTimeIntervalSinceNow:expVal];
			}
		}
        
        if ((accessToken == (NSString *) [NSNull null]) || (accessToken.length == 0)
            || (openid == (NSString *) [NSNull null]) || (openkey.length == 0)
            || (openkey == (NSString *) [NSNull null]) || (openid.length == 0)) {
            [qqWeiboOauth oauthDidFail:InWebView success:YES netNotWork:NO];
        }
        else {
            [qqWeiboOauth oauthDidSuccess:accessToken accessSecret:nil openid:openid openkey:openkey expireIn:expireIn];
        }
        qqAuthWeb.delegate = nil;
        [qqAuthView removeFromSuperview];
        qqAuthView = nil;
        loginStatus = 2;
		return NO;
	}
	else
	{
        start = [[url absoluteString] rangeOfString:@"code="];
        if (start.location != NSNotFound) {
            [qqWeiboOauth refuseOauth:url];
        }
	}
    return YES;
}

//******************************************************************************
// webViewDidFinishLoad
//******************************************************************************
- (void) webViewDidFinishLoad:(UIWebView *)webView
{
    [qqWeiboLoadIndicator stopAnimating];
}

//******************************************************************************
// webView
//******************************************************************************
- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    [qqWeiboLoadIndicator stopAnimating];
    
}


-(void) sendWeiboWithText:(NSString *)str WithImageExist:(bool) isImageExist
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                         NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString* path = [documentsDirectory stringByAppendingPathComponent:
                      [NSString stringWithUTF8String:"captureImage.png"]];
    UIImage *weiboImage =  [UIImage imageWithContentsOfFile:path];
    
    CGRect rect;
    if(isGachaWeibo)
        rect = CGRectMake(0, weiboImage.size.height*0.170, weiboImage.size.width, weiboImage.size.height*0.5);
    else {
        rect = CGRectMake(0, weiboImage.size.height*0.242, weiboImage.size.width,weiboImage.size.height*0.61);
    }
    CGImageRef subImageRef = CGImageCreateWithImageInRect(weiboImage.CGImage, rect);
    CGRect smallBounds = CGRectMake(0, 0, CGImageGetWidth(subImageRef), CGImageGetHeight(subImageRef));
    
    UIGraphicsBeginImageContext(smallBounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, smallBounds, subImageRef);
    UIImage* smallImage = [UIImage imageWithCGImage:subImageRef];
    UIGraphicsEndImageContext();
    [UIImagePNGRepresentation(smallImage) writeToFile:path atomically:NO];
    
    if(m_openApi == nil)
        m_openApi= [[OpenApi alloc] initForApi:[OpenSdkBase getAppKey] appSecret:[OpenSdkBase getAppSecret] accessToken:m_accessToken accessSecret:nil openid:m_openId oauthType:InWebView];
    
    if(isImageExist)
        [m_openApi publishWeiboWithImage:path weiboContent:str jing:@"0" wei:@"0" format:@"xml" clientip:@"CLIENTIP" syncflag:@"0"];
    else
        [m_openApi publishWeibo:str jing:@"0" wei:@"0" format:@"xml" clientip:@"CLIENTIP" syncflag:@"0"];
    
	
}
//******************************************************************************
// backFromQQLogin
//******************************************************************************
- (void) backFromQQLogin
{
    loginStatus = -1;
   // iReqStat = kReqCancel;
    [qqAuthView removeFromSuperview];
    qqAuthView = nil;
    [qqWeiboOauth release];
    qqWeiboOauth = nil;
}

#pragma webViewDelegate


@end
