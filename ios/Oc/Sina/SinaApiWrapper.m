//
//  SinaApiWrapper.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "SinaApiWrapper.h"

@implementation SinaApiWrapper

@synthesize weiBoEngine;
@synthesize openId;
@synthesize accessToken;
@synthesize iLoginStatus;
@synthesize isGachaWeibo;

- (id)init
{
    if(self = [super init]){
        iLoginStatus = 0;
        openId = nil;
        accessToken = nil;
        
        NSString *weiboSecretKey = [NSString stringWithFormat:@"%@%@%@%@%@",
                                                            SINA_SECRET_KEY1,
                                                            SINA_SECRET_KEY2,
                                                            SINA_SECRET_KEY3,
                                                            SINA_SECRET_KEY4,
                                                            SINA_SECRET_KEY5];
        
        weiBoEngine = [[WBEngine alloc] initWithAppKey:SINA_APP_ID
                                             appSecret:weiboSecretKey];
        [weiBoEngine setDelegate:self];
        [weiBoEngine setRedirectURI:@"http://"];
        [weiBoEngine setIsUserExclusive:NO];
    }
    
    return self;
}

- (void)dealloc
{
    [openId release];
    [accessToken release];
    [super dealloc];
}

//******************************************************************************
// Login
//******************************************************************************
-(void) Login
{
    iLoginStatus = 1;
    [weiBoEngine logIn];
}

//******************************************************************************
// engineDidLogIn
//******************************************************************************
- (void)engineDidLogIn:(WBEngine *)engine
{
    openId = [NSString stringWithString:engine.userID];
    [openId retain];
    accessToken = [NSString stringWithString:engine.accessToken];
    [accessToken retain];
    
    iLoginStatus = 2;
}

//******************************************************************************
// engine
//******************************************************************************
- (void)engine:(WBEngine *)engine didFailToLogInWithError:(NSError *)error
{
    iLoginStatus = -1;
}

//******************************************************************************
// engineCancel
//******************************************************************************
- (void)engineCancel:(WBEngine *)engine
{
    iLoginStatus = -1;
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
    
    UIImage *sImage = [UIImage imageWithContentsOfFile:path];
    if(isImageExist)
        [weiBoEngine sendWeiBoWithText:str image:sImage];
    else
        [weiBoEngine sendWeiBoWithText:str image:nil];
}

@end