//
//  FirstLayer.h
//  AgainstWar
//
//  Created by Liu Hao on 13-8-1.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__FirstLayer__
#define __AgainstWar__FirstLayer__

#include <iostream>
#include "cocos2d.h"
#include "CGameData.h"
#include "Dialog1.h"

using namespace cocos2d;


class FirstLayer : public CUiLayerBase
{
    
    CCSprite        *awardTips;
public:
    FirstLayer();
    
    void showAwardTips();
    
    virtual void enter();
    virtual void exit();
    Dialog1*                m_dlgAlert;
public:
    
private:
    ~FirstLayer();
    
    virtual void onEnter();
    virtual void onExit();
    
    void innerEnter();
    void innerExit();
    
    void showBianDui();
    void showCardHead(CUserCardInfo* pUsrCardInfo, float posX);
    void cbShowCardHead(CCObject* pObj);
    
    void newBtn(CCMenuItem *&menuItem, const CCPoint& position, int tag, const char* image);
    void btnCallback(CCObject* pObj);
    
    
    void showAlertCanotAward();
    void cbAlertDlg();

private:

};

#endif /* defined(__AgainstWar__FirstLayer__) */
