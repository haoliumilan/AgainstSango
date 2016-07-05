//
//  CardShop.h
//  AgainstWar
//
//  Created by user on 13-12-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CardShop__
#define __AgainstWar__CardShop__

#include "cocos2d.h"
#include "CGameData.h"
#include "CardConfirm.h"
#include "CommDlg.h"
#include "Dialog1.h"
#include "CScrollBar.h"
class CScrollViewLayer;
class CShowCardInf;

USING_NS_CC;

#define CARD_TAG  1221


enum enReqSCType
{
    enReqSCNull = 0,
    enReqSCBuy,
    enReqSCGetAllCard,
   
};

class CardShop : public CCLayer
{
    
    CScrollBar*     m_pScrollBar;
  
    CShowCardInf*   m_showCard;

    CUserCardInfo* m_pUserCardInfo;
    int  m_shopCardNum;
    
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfSelector;
    
    CCMutableArray<CShopCardInfo*> * pShopCardsArr;
    
    CCSize          m_cardSize;
    int             m_iGap;
    int             m_iColumn;
    CCPoint         m_ptStart;
    int             m_iRmBtn;
    int             m_iSelNo;   // 记录选中的卡片序数
    
    bool            m_bTouchMoveFlg;//是否为滑动
    
    enReqSCType     m_ReqType;
    
    CardConfirm*  m_cfmLayer;
    CommDlg*            m_commDlg;
    
    Dialog1*            m_dlgAlert;
    
    
    
public:
    CardShop(CCObject* listener, SEL_CallFunc selector);
    ~CardShop();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    bool goback();
    void  removem_cfmLayer();
    bool  isExistForcfmLayer();
    
    CUserCardInfo* getNewUserCardFromShopCard(CShopCardInfo * cshopcardinfo);
    
private:
    void update(ccTime dt);
    
    void showCardItem();
    void showChooseCard(int idx);
    void cbCfm(CCObject* pSender);
    void Dlg2Callback(CCObject* pSender);
    void buyCard();
    void cbCommDlg(CCObject* pSender);
    void cbAlertDlg(CCObject* pSender);
    void showSuccessAlert();
    
    //新增
    void inertiaMove(float dt);
    void waitShowInfo();
    void setThisPosition(const CCPoint& pos, bool bRedraw = false);
     CCPoint beganPoint;
     CCPoint         m_ptStartTouch;
     CCPoint         last_moveTouch;
     float           m_lastXm;
    float            m_lastYm;
    bool             isLongPress;
    float           m_fUpLimitY;
    int              m_iPreThisPosY;
    void  moveY(float dt);
 
    
    
};

#endif /* defined(__AgainstWar__CardShop__) */
