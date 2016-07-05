//
//  RwdMonthLayer.h
//  AgainstWar
//
//  Created by caojiye on 13-8-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdMonthLayer__
#define __AgainstWar__RwdMonthLayer__

#include "cocos2d.h"

using namespace cocos2d;


class RwdMonthLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;

public:
    
    static RwdMonthLayer* Create(CCObject* target, SEL_CallFuncO selector);
    
    //virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    
    RwdMonthLayer(CCObject* target, SEL_CallFuncO selector);
    
    void Set25Cell();
    CCSprite* GetCellSprite(int _number);
    string ConvertCardToPng(string card);
    
    
    
};


#endif /* defined(__AgainstWar__RwdMonthLayer__) */
