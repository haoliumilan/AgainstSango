//
//  DeadReviveDlg.h
//  AgainstWar
//
//  Created by jingfeng wu on 14-3-14.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__DeadReviveDlg__
#define __AgainstWar__DeadReviveDlg__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class DeadReviveDlgRet :public CCObject
{
public:
    int iBtnsel;
    int iOptionsel;
    int iCost;
    string strItemId;
};

class NoScaleMenuItemLabel:public CCMenuItemLabel
{
public:
    static NoScaleMenuItemLabel * itemWithLabel(CCNode*label, CCObject* target, SEL_MenuHandler selector)
    {
        NoScaleMenuItemLabel *pRet = new NoScaleMenuItemLabel();
        pRet->initWithLabel(label, target, selector);
        pRet->autorelease();
        return pRet;
    }
    virtual void selected(){};
};


class DeadReviveDlg :public CCLayer{
    CCObject *m_listener;
    SEL_CallFuncO m_selector;
    DeadReviveDlgRet m_ret;
    
    CCSprite *m_backGround;
    CCMenuItem *sureItem;
    CCMenuItem *cancelItem;
//    CCMenuItem *Item;
    void menuCallback(CCObject *pSender);
    void optionCallback(CCObject *pSender);
//    TextNode *  infoLabel;
//    TextNode *  myYuanbaoLabel;
    
    CCMenuItem *optionItem1,*optionItem2,*optionItem3;
    TextNode   *optionLabel;
public:
    DeadReviveDlg(CCObject *listener,SEL_CallFuncO selector);
};


#endif /* defined(__AgainstWar__DeadReviveDlg__) */
