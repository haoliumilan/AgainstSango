//
//  UseItemCard.h
//  AgainstWar
//
//  Created by Hao Zhang on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_UseItemCard_h
#define AgainstWar_UseItemCard_h

#include "cocos2d.h"
#include "CGameData.h"

USING_NS_CC;

enum enUseItemCardRetType {
    enUIC_Use,
    enUIC_UseAll,
    enUIC_Back
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class UseItemCardRet : public CCObject
{
public:
    int     iBtnSel;
    int     lCnt;               //可以使用的总数量
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class UseItemCard : public CCLayer
{

    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    UseItemCardRet  m_ret;
    
    int            m_needBatch;

    CCLayer*        m_mvLayer;
public:
    UseItemCard(CCObject* target, SEL_CallFuncO selector, ItemCardInf* itemCard ,string cardsType,int needBatch = 0);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    bool isSelectedForPos(CCPoint pos, int btnCount);
};

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

class BatchUseCardRet : public CCObject
{
public:
    int iBtnSel;
};


class BatchUseCard : public CCLayer,CCTextFieldDelegate
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    BatchUseCardRet m_ret;
    
    int             m_lCnt;               //最大使用个数
    string          cardType;             //卡类型 "stone"彩石
    
private:
    CCSprite *      m_background;
    CCSprite *      m_confirmBtn;
    CCSprite *      m_cancelBtn;
//    bool            isAttach;
    
public:
    CCTextFieldTTF *textField;
    BatchUseCard(CCObject* target, SEL_CallFuncO selector,int lCnt,string useCardType);
    ~BatchUseCard();
    virtual void exit();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void removeDispacher();
    void menuCallback(CCObject* pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
};






#endif
