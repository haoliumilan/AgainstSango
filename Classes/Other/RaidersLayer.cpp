//
//  RaidersLayer.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "RaidersLayer.h"
#include "CGamedata.h"
#include "OcProxy.h"

RaidersLayer::RaidersLayer()
{
    
}

void RaidersLayer::enterWithType(enUrlType type)
{
    
    char *temp;
    if(type == enUrlTypeHelp)
        temp = CGameData::Inst()->getCommonInfo()->war_help_url;
    else if (type == enUrlTypeCntryHelp)
        temp = CGameData::Inst()->getCommonInfo()->cntry_notice;
    else
        temp =  CGameData::Inst()->getCommonInfo()->aboutus;
    
    OcProxy::Inst()->showHelpInfoWebView(CGameData::Inst()->getUrlDepDevice(temp));
}


void RaidersLayer::exit()
{
    OcProxy::Inst()->removeWebview();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}



