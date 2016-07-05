//
//  RareGacha10Show.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-13.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_RareGacha10Show_h
#define AgainstWar_RareGacha10Show_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class RareGacha10ShowRet :public CCObject{
    
public:
    bool isGoCompGacha;
    RareGacha10ShowRet() : isGoCompGacha(false) {}
};


class CShowCardInf;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class RareGacha10Show : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    RareGacha10ShowRet m_ret;
    int             m_CardCnt;
    CShowCardInf*   m_showCard;
    bool m_isGachaMaterial;
    CCMenuItem *    m_shareBtn;
    
public:
    
    RareGacha10Show(CCObject* listener,SEL_CallFuncO selector);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void show(bool isFromItemCard = false);
    void hide();
    void goCompGacha();
private:
    void cbActionEnd();
    
public:
    void shareToWeibo();
    // yang share sanguo
    void shareSanGuo();
    void dl2CallBack(CCObject *sender);
    
};

#endif
