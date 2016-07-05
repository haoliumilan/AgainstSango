//
//  TencentApiMM.m
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TencentApiMM.h"
#include "TencentApiWrapper.h"


static  TencentApiMM*  s_pcIf = NULL;

static TencentApiWrapper * s_pTencetApi = nil;

TencentApiMM* TencentApiMM::Inst(){
    
    if(s_pcIf == NULL){
        s_pcIf = new TencentApiMM();
        
    }
    
    return s_pcIf;
}

TencentApiMM::TencentApiMM()
{
    s_pTencetApi = [[TencentApiWrapper alloc] init];
}

 void TencentApiMM::login()
{
     
    if(s_pTencetApi==nil)
    {
        s_pTencetApi = [[TencentApiWrapper alloc] init];
    }
    
    [s_pTencetApi login];
}

int TencentApiMM::getLoginStatus()
{
    return  [s_pTencetApi loginStatus];
}

const char * TencentApiMM::getOpenId()
{
    return [[s_pTencetApi m_openId] UTF8String];
}

const char * TencentApiMM::getAccessToken()
{
    return [[s_pTencetApi m_accessToken] UTF8String];
}

void TencentApiMM::sendWeibo(const char *str ,bool isImageExist)
{
    NSString *weibo = [NSString stringWithUTF8String:str];
    [s_pTencetApi sendWeiboWithText:weibo WithImageExist:isImageExist];
    
}

void TencentApiMM::setIsGachaWeibo(bool isOk)
{
    if(s_pTencetApi)
        [s_pTencetApi setIsGachaWeibo:isOk];
}

void TencentApiMM::clearOauthData()
{
    if (s_pTencetApi) {
        [s_pTencetApi release];
        s_pTencetApi = nil;
    }
}
