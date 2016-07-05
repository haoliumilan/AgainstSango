//
//  OtherCardLayer.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_OtherCardLayer_h
#define AgainstWar_OtherCardLayer_h


#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"

using namespace cocos2d;

class CUserCardInfo;
class CScrollBar;
class CConfirmLayer;
class PackSelBtn;
class Dialog2;
class TextNode;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OtherCardSelRet : public CCObject
{
public:
    int iBtnSel;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OtherCardLayer : public CCLayer
{
    friend class CScrollViewLayer;
    //cb
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    OtherCardSelRet m_ret;
    CScrollBar*     m_pScrollBar;
    CCSprite*       m_spMaxCard;
    TextNode*       m_lbMaxCard;
    CCSize          m_cardSize;
    int             m_iGap;
    int             m_iColumn;
    CCPoint         m_ptStart;
    bool            m_bTouchMvFlg;
    CCPoint         m_ptStartNode;
    CCPoint         m_ptStartGl;
    
    float           m_fUpLimitY;

    CCPoint           last_moveTouch;
  //  CCMutableArray<CCSprite *> *showArray;
    float fDownLimitY;
    
    float m_lastYm;
    
    int             m_iPreThisPosY;
    
 //   OtherCardLayer();
public:
    static int getCardTagBase();
    OtherCardLayer(CCObject* target, SEL_CallFuncO selector);
    ~OtherCardLayer();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
            
    virtual void onExit();
    
    void inertiaMove();
    
    void refreshShowState();
    
    void setThisPosition(const CCPoint& pt, bool bRedraw = false);
    
    void NtyGoTop(CCObject* obj);
    void NtyGoBottom(CCObject* obj);

private:
    void initCapacity(char*);
    
    void redrawCapacity();
    
    void inertiaMove(float dt);
    
    void moveY(float dt);
    
    bool isShowClctCard(int nIndex);
    
};

#endif

