//
//  UseResultDialog.h
//  AgainstWar
//
//  Created by Liu Hao on 13-4-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__UseResultDialog__
#define __AgainstWar__UseResultDialog__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

enum enUseCardType
{
    enUseCardType_coin = 1,     // 元宝
    enUseCardType_gold,         // 铜钱
    enUseCardType_exp,          // 经验
    enUseCardType_supply,       // 军粮
    enUseCardType_stamina,      // 体力
    enUseCardType_food,         // 素材
    enUseCardType_cutCd,        // 减CD
    enUseCardType_rename,       // 改名
    enUseCardType_gacha,        // 抽1次神将
    enUseCardType_gacha10,      // 抽10次神将
    enUseCardType_stone,        // 石头
    enUseCardType_cost_up,      // 统御力卡
    
};

class UseResultDialog : public CCLayer
{
public:
    UseResultDialog(CCObject* target, SEL_MenuHandler selector);
    ~UseResultDialog();
    
private:
    virtual void onEnter();
    virtual void onExit();
    
    void showTextDialog(enUseCardType useCardType,int useCardCount);
    void showImageDialog();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
public:
    void showUseResultDialog(string strType,int useCardCount);
private:
    CCObject*       m_pListener;
    SEL_MenuHandler    m_pfnSelector;

};

#endif /* defined(__AgainstWar__UseResultDialog__) */
