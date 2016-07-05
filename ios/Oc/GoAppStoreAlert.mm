//
//  GoAppStoreAlert.m
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import "GoAppStoreAlert.h"
#include "CGamedata.h"
#include "CGamedata.h"

#define EVALUATEAPPTAG 100
#define UPDATEAPPTAG 101

@implementation GoAppStoreAlert

- (id)init
{
    if(self == [super init])
    {
        m_alertView = NULL;
    }
    return self;
}

- (void)goToAppStoreEvaluate
{
    if(m_alertView == NULL)
    {
        NSString *str = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->praise];
        
        m_alertView = [[UIAlertView alloc] initWithTitle:nil message:str
                                              delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定",nil];
        m_alertView.delegate = self;
        m_alertView.tag = EVALUATEAPPTAG;
        [m_alertView show];
        [m_alertView release];
    }
    
}


- (void)goToAppStoreUpdate
{
    if(m_alertView == NULL)
    {
        const char* szErrMsg = CGameData::Inst()->getErrMsg();
        
        NSString *str;
        if (szErrMsg) {
            str = [NSString stringWithUTF8String:CGameData::Inst()->getErrMsg()];
        }
        else{
            str = NSLocalizedString(@"appCtrlTip2", NULL);
        }
        
        m_alertView = [[UIAlertView alloc] initWithTitle:nil message:str delegate:self cancelButtonTitle:NSLocalizedString(@"appCtrlTip3", NULL) otherButtonTitles:nil];
        m_alertView.delegate = self;
        m_alertView.tag = UPDATEAPPTAG;
        [m_alertView show];
        [m_alertView release];
    }
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView.tag == EVALUATEAPPTAG)
    {
        if(buttonIndex == 1)
        {
            if ([[[UIDevice currentDevice]systemVersion]floatValue] < 7.0 ) {
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=533744303"]];
            }else{
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"itms://itunes.apple.com/cn/app/ni-zhuan-san-guo/id533744303?mt=8"]];
            }
           
            
        }
        
    }else if(alertView.tag == UPDATEAPPTAG){
        NSString *str = [NSString stringWithUTF8String:CGameData::Inst()-> getAppUrl()];
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];

    }
    m_alertView = NULL;
}

@end
