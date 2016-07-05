//
//  ItemCardLayer.h
//  AgainstWar
//
//  Created by user on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_ItemCardLayer_h
#define AgainstWar_ItemCardLayer_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CScrollBar.h"
#include "ItemCard.h"
#include "MainScene.h"
#include "UseItemCard.h"
#include "TextNode.h"
#include "GachaGet.h"
#include "CShowCardInf.h"
#include "CountryDialog.h"
#include "RareGacha10Show.h"
#include "TitleBar.h"

using namespace cocos2d;

class UseResultDialog;
class CommDlg;
class Dialog1;

enum enItemCardReqType {
    enReqType_Null = 0,
    enReqType_UseItemCard,
    enReqType_GetAllCards,
    enReqType_ChargeGacha,
    enReqType_ChargeGachaOver,
    enReqType_MutiChargeGacha,
    enReqType_MutiChargeGachaOver,
    enReqType_SetNameCard,
    enReqType_Stone,
    
    enReqType_COMPOUND,

    
    enReqType_GiveWuJiang_Item,
    enReqType_GiveWuJiangAllCards_Item,
    enReqType_card_update_Item,
};

enum enCountryMainDo
{
    enMoveTitleLeave = 0,
    enMoveTitelHere,
    enItmCardLyQuit,
};

class ItemCarSelRet : public CCObject
{
  public:
    int iBtnSel;
    int iActType;
};

class ItemCardLayer : public CCLayer
{
    int             m_rtyTime;          // 求将重试时间戳
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    ItemCarSelRet   m_ret;
    CScrollBar*     m_pScrollBar;
    CCSprite*       m_spMaxCard;
    CCLabelTTF*     m_lbMaxCard;
    CCSize          m_cardSize;
    int             m_iGap;
    int             m_iColumn;
    CCPoint         m_ptStart;
    bool            m_bTouchMvFlg;
    CCPoint         m_ptStartNode;
    CCPoint         m_ptStartGl;
    
    char            ucidbuf[100];
    bool            m_isGachaPlus;
    GachaGet*       pGachaLayer;
    
    float           m_fUpLimitY;
    
    CCPoint         last_moveTouch;
    
    float fDownLimitY;
    
    float m_lastYm;
    
    int                 m_iPreThisPosY;
    
    UseResultDialog*    m_dlgResult;
    
    CommDlg*            m_commDlg;
    
    CountryDialog*      m_cntryDialog;
    void                setNameCard();
    enItemCardReqType   m_curReqType;

    int                 m_preStatus;
    
    string              m_useCardName;
    string              m_useCardId;
    string              m_useCardType;
    int                 m_useCardCount;
    
    Dialog1*            m_dlgAlert;
    
    CShowCardInf *      m_showCard;
    
    RareGacha10Show*    m_RareGacha10Show;
    void                RareGacha10ShowCb();
    
    void moveY(float dt);
    void setThisPosition(const CCPoint& pt, bool bRedraw = false);
    void inertiaMove(float dt);
    void showItemCard();
    void BtnCallBack(CCObject* pSender);
    void Dlg2Callback(CCObject* pSender);
    void GoToYuanbao(CCObject* pSender);        //是否进入元宝商店
    
    int cardCanBatch(string cardTypeStr);
    
    CCMenu*             m_menu;
    TitleBar*           m_spTitle;
    
public:
    
    void newbie(CCObject* pObj);
    void newbie2(CCObject* pObj);

    
    
    void setTouchEnable();
    
    ItemCardLayer(CCObject* target, SEL_CallFuncO selector);
    ~ItemCardLayer();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void onExit();
    
private:
    
    void cbSetNameCardDlg();
    void showSureNameCardTips();
    void cbSureNameCardTips(CCObject* pObj);
    
    void cbUseItemCard(CCObject* pSender);
    void cbUsebatchItemCard(CCObject* pSender);
    
    void useItemCard();
    void getAllCards();
    
    void showUseResultDlg();
    void cbUseResultDlg();
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* );
    
    void gachaGetCallback(CCObject* pSender);
    void backEnter();
    void showInf();
    void cbShowCard();
    
    void showAlertNotEnoughDlg();
    void showAlertMoreThan10Dlg();
    void cbAlertDlg();

    void showSureUseCardDlg();
};




#endif
