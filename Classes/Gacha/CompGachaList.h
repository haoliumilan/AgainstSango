//
//  CompGachaList.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CompGachaList_h
#define AgainstWar_CompGachaList_h

#include "cocos2d.h"
#include "CompGachaBtn.h"


using namespace cocos2d;

class CompGachaCardInfo;

class CompGachaListRet : public CCObject
{
    public:
        bool isOwe;
        int cid;
        bool isShow;
};

class CompGachaList :public CCLayer {
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CompGachaListRet m_ret;
    int m_itemCnt;
    CCPoint m_preTouchPos;
    CCPoint m_firstTouchPos;
    bool isFirstPush;
    int tagPos;
    CompGachaBtn *lastBtn;
    CompGachaBtn *firstBtn;
    float m_listLimtH;
    float m_totoalDelta;
    float           m_lastYm;
    CCPoint         last_moveTouch;
    bool            m_bTouchMvFlg;
    CCSprite *scrollBarBg;
    CCSprite *scrollBar;
    float contentHeight;
    float sDeltaPosY;
    CompGachaCardInfo *touchedCardInfo;
    int  targetId;
    bool isShowCardInfo;
private: 
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    void inertiaMove(float dt);
    void moveY(float dt);
    void waitShowInfo();
public:
    void pushBack(CompGachaBtn *btn);
    CompGachaList(CCObject *listener,SEL_CallFuncO selector);
    virtual void visit();
    
    virtual void onExit();
};



#endif
