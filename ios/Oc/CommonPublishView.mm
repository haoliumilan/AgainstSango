//
//  CommonPublishView.cpp
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CommonPublishView.h"
#import "PublishWeiboView.h"
#import "AppController.h"
#include "OcProxy.h"

static CommonPublishView *cpInstance = NULL;
static PublishWeiboView* m_publishWeiboView = NULL;

CommonPublishView* CommonPublishView::Inst()
{
    if(cpInstance == NULL)
    {
        cpInstance = new CommonPublishView();
    }
    return cpInstance;
}

void CommonPublishView::showPublishView(enWeiboType weiboType)
{
    NSString *weiboStr;
    
    if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin || CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin)
    {
        NSString *replaceStr ;
        if(weiboType == enWeiboTypeTujian)
        {
            replaceStr = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_1];
            // replaceStr = [NSString stringWithFormat:@"%@%s",tempStr,CGameData::Inst()->getCommonInfo()->app_url];
            
        }else if(weiboType == enWeiboTypeGacha){
            NSString *cardName = [NSString stringWithUTF8String:CGameData::Inst()->getGachaCardName()];
            NSString *tempString  = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_2];
            replaceStr  =  [tempString stringByReplacingOccurrencesOfString:@"||" withString:cardName];
            //replaceStr = [NSString stringWithFormat:@"%@%s",temStr,CGameData::Inst()->getCommonInfo()->app_url];
        }else if(weiboType == enWeiboTypeTeam){
            replaceStr = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_3];
            //  replaceStr = [NSString stringWithFormat:@"%@%s",tempStr,CGameData::Inst()->getCommonInfo()->app_url];
            
        }else if(weiboType == enWeiboTypeGeneral){
            replaceStr = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_4];
            //  replaceStr = [NSString stringWithFormat:@"%@%s",tempStr,CGameData::Inst()->getCommonInfo()->app_url];
        }else if(weiboType == enWeiboTypeCompGacha){
            replaceStr = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_6];
            //  replaceStr = [NSString stringWithFormat:@"%@%s",tempStr,CGameData::Inst()->getCommonInfo()->app_url];
        }else if(weiboType==enWeiboTypeTenCompGacha){
            replaceStr = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_7];
            // replaceStr = [NSString stringWithFormat:@"%@%s",tempStr,CGameData::Inst()->getCommonInfo()->app_url];
        }
        NSString *inviteCode = [NSString stringWithUTF8String:CGameData::Inst()->getUsrInfo()->invite_code];
        weiboStr =  [NSString stringWithString:[replaceStr stringByReplacingOccurrencesOfString:@"&&" withString:inviteCode]];
        NSString *repAppUrlStr = [NSString stringWithFormat:@"%s",CGameData::Inst()->getCommonInfo()->app_url];
        NSString *repAndrodUrlStr = [NSString stringWithFormat:@"%s",CGameData::Inst()->getCommonInfo()->androidsg_download];
        weiboStr = [NSString stringWithString:[weiboStr stringByReplacingOccurrencesOfString:@"<<ios>>" withString: repAppUrlStr]];
        weiboStr = [NSString stringWithString:[weiboStr stringByReplacingOccurrencesOfString:@"<<android>>" withString: repAndrodUrlStr]];
        
        // qqOrSina = 1;
    }
    
    m_strWeiboContent = [weiboStr UTF8String];
    
    if(m_publishWeiboView == NULL)
    {
        if(OcProxy::Inst()->isIpadDevice())
        {
            m_publishWeiboView = [[PublishWeiboView alloc] initWithFrame:CGRectMake(170, 380, 440, 240) withText:weiboStr  withImage:nil];
            AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
            [((UIViewController *)appCtr.viewController).view  addSubview:m_publishWeiboView];
            [m_publishWeiboView release];
            
        }else {
            m_publishWeiboView = [[PublishWeiboView alloc] initWithFrame:CGRectMake(50, 176+OcProxy::Inst()->deltaHeightOf568(), 220, 120) withText:weiboStr
                                                               withImage:nil];
            AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
            [((UIViewController *)appCtr.viewController).view  addSubview:m_publishWeiboView];
            [m_publishWeiboView release];
        }
    }
}

void CommonPublishView::removePublishView()
{
    if(m_publishWeiboView)
    {
        [m_publishWeiboView removeFromSuperview];
        m_publishWeiboView = NULL;
    }
}

void CommonPublishView::setPublishViewHiden(bool isHide)
{
    [m_publishWeiboView setHidden:isHide];
}

const char* CommonPublishView::getWeiboContent()
{
    return m_strWeiboContent.c_str();
}
