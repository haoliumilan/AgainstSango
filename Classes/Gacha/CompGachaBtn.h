//
//  CompGachaBtn.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CompGachaBtn_h
#define AgainstWar_CompGachaBtn_h


#define MATERIALTAG 200
#define SUPERHEROTAG 300
#define CELLHEIGHT 110
#define EXCHANGEBTNHEIGHT 80
#include "cocos2d.h"
#include "TextNode.h"
#include "CGamedata.h"

using namespace cocos2d;

class CompGachaBtnRet : public CCObject
{
public:
    int     iBtnSel;
    bool    isUnKnowHero;
    CompGachaBtnRet() : isUnKnowHero(false) {}
};

class CompGachaBtn :public CCLayer {
private:
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CompGachaBtnRet m_ret;
    CCSprite *btnBgTop;
    CCSprite *btnBgMiddle;
    CCSprite *btnBgBottom;
    CompGachaInfo  *m_compGachaInfo;
    int cardLines;
    void menuCallback(CCObject *sender);
    
    //label
    TextNode *exchangeCountDown;
    TextNode *exchangeTimes;
    bool isExchangeAble;
public:
    void addHead();
    CompGachaBtn(CompGachaInfo *info,CCObject* target, SEL_CallFuncO selector);
    virtual void onExit();
    int getCardLines();
    void showNewCardEffect(CCSprite *sp);
    CCSize getSize();
    float getDefalutDelta();
};

#endif
