//
//  RaidersLayer.cpp
//  AgainstWar
//
//  Created by XIII on 12-6-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "GachaWeb.h"
#include "CGamedata.h"
#include "OcProxy.h"



GachaWeb::GachaWeb()
{
    
}

void GachaWeb::enter()
{
    char *temp;
    temp = CGameData::Inst()->getCommonInfo()-> gacha_notice;
 //   OcProxy::Inst()->showGachaMainWebView(CGameData::Inst()->getUrlDepDevice(temp));
    
    OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                     CGameData::Inst()->getUrlDepDevice(temp),
                                     CCRect(20*2, 300*2, 280*2, 100*2),
                                     true);
}


void GachaWeb::exit()
{
    OcProxy::Inst()->removeWebview();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

