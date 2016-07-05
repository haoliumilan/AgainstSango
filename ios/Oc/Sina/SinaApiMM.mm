//
//  SinaApiMM.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "SinaApiMM.h"
#include "SinaApiWrapper.h"

SinaApiMM* SinaApiMM::s_pcIf = NULL;

static SinaApiWrapper* s_pSinaApi = nil;


SinaApiMM::SinaApiMM()
{
    s_pSinaApi = [[SinaApiWrapper alloc] init];
}

void SinaApiMM::Login()
{
    [s_pSinaApi Login];
}

int SinaApiMM::getLoginStatus()
{
    int iLoginStatus = [s_pSinaApi iLoginStatus];
    
    return iLoginStatus;
}

const char* SinaApiMM::getOpenId()
{
    const char* szOpenId = [[s_pSinaApi openId] UTF8String];
    
    return szOpenId;
}

const char* SinaApiMM::getAccessToken()
{
    const char* szAccessToken = [[s_pSinaApi accessToken] UTF8String];
    
    return szAccessToken;
}

void SinaApiMM::sendWeibo(const char *str ,bool isImageExist)
{
    NSString *weibo = [NSString stringWithUTF8String:str];
    [s_pSinaApi sendWeiboWithText:weibo WithImageExist:isImageExist];
}

void SinaApiMM::setIsGachaWeibo(bool isOk)
{
    [s_pSinaApi setIsGachaWeibo:isOk];
}

void SinaApiMM::setLoginStatus(int status)
{
    [s_pSinaApi setILoginStatus:status];
}
