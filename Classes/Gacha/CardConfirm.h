//
//  CardConfirm.h
//  AgainstWar
//
//  Created by user on 13-12-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CardConfirm__
#define __AgainstWar__CardConfirm__

#include "cocos2d.h"
#include "CGameData.h"

USING_NS_CC;

class CardConfirmRet : public CCObject
{
public:
    int iBtnSel;
};

class CardConfirm : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CardConfirmRet  m_ret;
    
    CShopCardInfo*  m_pShopCardInf;
    
public:
    CardConfirm(CCObject* listener, SEL_CallFuncO selector, CShopCardInfo* pShopCard);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    bool isSelectedForPos(CCPoint pos, int btnCount);
    void drawCardInf(short sCardId);
};


#endif /* defined(__AgainstWar__CardConfirm__) */
