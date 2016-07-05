//
//  PwUpBase.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-30.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_PwUpBase_h
#define AgainstWar_PwUpBase_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class TextNode;
class CUserCardInfo;
class MsgBox;
class  CShowCardInf;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class PwUpBaseRet : public CCObject
{
public:
    int     iSel;   //0: add card     1: start  2: over
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class PwUpBase : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    PwUpBaseRet     m_ret;

    CCSprite*       m_spBtnAdd;
    CCSprite*       m_spBtnStart;
    
    CCSprite*       m_spExpBar;
    CCSprite*       m_spExpFrm;
    
    int             m_iFoodExp;
    bool            m_bOver;
    bool            m_bGoldEnough;
    TextNode        *m_lvLabel;
    TextNode        *m_hpLabel;
    TextNode        *m_recLabel;
    TextNode        *m_atLabel;
    TextNode        *m_expLabel;
    TextNode        *m_upExpLabel;
    TextNode        *m_upExpTitleLabel;
    CUserCardInfo   *cur_cardInfo;
    CCSprite*       spRedBar ;
    
    MsgBox*         m_msgbox;
    bool            m_bMoved;
    
    //长按显示详细信息
    void waitShowInfo();
    CCPoint beganPoint;
    bool isLongPress;
    CShowCardInf*   m_showCard;
    
    bool            m_bNiePan;
    bool            m_bCanNiePan;

public:
    PwUpBase(CCObject* target, SEL_CallFuncO selector,
             bool bNiePan, bool bOver = false);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    virtual void onExit();
    
private:
    void newbieEnd(CCObject* pObj);
    void showBaseCardInf();
    void showExpPanel(CUserCardInfo* pUsrCardInf);
    void showFood();
    void initButton();
    void lvlUpAnim();
    void setBaseCardInf(CUserCardInfo *userCard);
    
    void showNiePanInfo();
    void showNiePanFood();
    void showNiePanNeedFood();
    void showNiePanHaveFood();
    void showNiePanTarget();
    void showNiePanBtn();
    
    void showNiePanBtnAnim();
    CCActionInterval* getActionFromFile(const char* filename);
    CCActionInterval* getActionFromFile(const char* filename, float delta_time);
    CCAction* ScaleAction();
    CCAction* FlashAction();
    
    void showNoFoodInPackage();
    void cbNoFoodInPackage();

private:
    int tempLv;
    int aCount;
};

#endif
