//
//  FrdGacha10Show.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-14.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_FrdGacha10Show_h
#define AgainstWar_FrdGacha10Show_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class FrdGacha10ShowRet :public CCObject{
    
public:
    bool isGoCompGacha;
    FrdGacha10ShowRet() : isGoCompGacha(false) {}
};

class CShowCardInf;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class FrdGacha10Show : public CCLayer
{
    int             m_CardCnt;
    CShowCardInf*   m_showCard;
    bool m_isGachaMaterial;
    
    
public:
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    FrdGacha10ShowRet m_ret;
    FrdGacha10Show(CCObject* listener,SEL_CallFuncO selector);
    
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void show();
    void hide();
    void goCompGacha();
private:
    void cbActionEnd();
public:
    CCMenuItem *    m_shareBtn;
    void shareToWeibo();
    // yang share sanguo
    void shareSanGuo();
    void dl2CallBack(CCObject *sender);
private:
    int sharestar; // 星星大与3的时候显示分享
    
};

#endif
