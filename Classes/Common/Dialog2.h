//
//  Dialog2.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-2.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_Dialog2_h
#define AgainstWar_Dialog2_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

#define kShowFoodInTeam 2012
#define kShowFoodUpThreeStarsOk 2013
#define kShowFoodUpThreeStarsCancel 2014

enum Dialog2Type {
    enDl2OneBtn,
    enDl2TwoBtn,
    enDl2ThreeBtn,
    enDl2FullCostBtn,
    enDl2FoodInTeam,
    enDl2FoodUpThreeStars,
    enDl2TwoBtnJuanXian,
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Dialog2Ret : public CCObject
{
public:
    bool bOk;
    int  bSel;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Dialog2 : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    Dialog2Type     m_type;

    CCSprite*       m_spOk;
    CCSprite*       m_spCancel;
    
    Dialog2Ret      m_ret;
public:
    Dialog2(CCObject* target, SEL_CallFuncO selector, const char* title,const char* dsp,Dialog2Type type);
   // Dialog2(CCObject* target, SEL_CallFuncO selector, const char* dsp);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

    virtual void onExit();
    
private:
    void menuCallBack(CCObject *pSender);
    void cbSwitchUser(CCNode* a);
};

#endif
