//
//  GachaMain.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012Âπ?OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_GachaMain_h
#define AgainstWar_GachaMain_h

#define GachaLayerTag 10
#define kNewbieEndTag 82
#define kNewbieInfTag 182


#include "GachaGet.h"
#include "cocos2d.h"
#include "CGamedata.h"
#include "CMenuBtn.h"
#include "CShowCardInf.h"
#include "CompGachaList.h"
#include "CompGachaBtn.h"
#include "CardShop.h"


using namespace cocos2d;


class GachaWeb;
class Package;
class TeamInf;
class CommDlg;
class TitleBar;

enum enGachaReqType{
    enGachaReqType_Null,
    enGachaReqType_FreeGacha,
    enGachaReqType_RareGacha,
    
    enGachaReqType_RareGachaSpeed,
    enGachaReqType_MutiFreeGacha,
    enGachaReqType_MutiFreeGachaOvr,
    enGachaReqType_MutiRareGacha,
    enGachaReqType_MutiRareGachaSpeed,
    enGachaReqType_MutiRareGachaOvr,
    enGachaReqType_MutiRareGachaSpeedOvr,
    enGachaReqType_FreeGachaOvr,
    enGachaReqType_RareGachaOvr,
    enGachaReqType_RareGachaSpeedOvr,
    enGachaReqType_SuiTangGacha,
    enGachaReqType_SuiTangGachaOvr,
    
    enGachaReqType_SuiTangGachaSpeed,
    enGachaReqType_SuiTangGachaSpeedOvr,

    enGachaReqType_NewbieGacha,
    enGachaReqType_CompGacha,
    enGachaReqType_ExchangeCard,
    enGachaReqType_ExchangeCardOvr,
    
    
    enGachaReqType_OtherGacha,
    enGachaReqType_OtherGachaOvr,
    
    enGachaReqType_OtherGachaSpeed,
    enGachaReqType_OtherGachaSpeedOvr,

    
    //enGachaReqType_MutliOtherGacha,
    enGachaReqType_MutliOtherGachaOvr,
    enGachaReqType_MutliOtherGachaSpeedOvr,

};


class CompGachaWeb;
class RareGacha10Show;
class FrdGacha10Show;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class GachaMain : public CUiLayerBase
{
    TitleBar*       m_spTitle;
    CCMenuItem*     m_miBack;
    
    CCMenu*         m_btnMenu;
    GachaGet*       pGachaLayer;
    
    CardShop*       m_CardShop;
    
    CommDlg*        m_commDlg;
    CCMenuItem*     m_btn1;
    CCMenuItem*     m_btn2;
    CCMenuItem*     m_btn3;
    GachaWeb *      gachaWeb;
    CCSprite *      m_pWebBg;
    CCMenuItem *    m_shareBtn;
    CCSprite *      frNumberTips;
    CCSprite *      unsNumberTips;
    CompGachaList * m_compGachaList;
    CompGachaWeb *  m_compGachaWeb;
    CCSprite *      compGachaBgTop; 
    CCSprite *      compGachaBgMiddle; 
    CCSprite *      compGachaBgBottom; 
    CShowCardInf *  m_showCard;
    CListLayer *    frdGachaClist;
    CListLayer *    unsGachaClist;
    
    RareGacha10Show*    m_RareGacha10Show;
    FrdGacha10Show*     m_FrdGacha10Show;
    
    char ucidbuf[100];
    bool m_isGachaPlus;
    
    long ml_requestTime;
    
    int xxxx;
public:
    GachaMain();
    
    virtual void enter();
    virtual void exit();
    
    
    void enterGacha();
    
    void exitFriendOrUnusualGacha();
    
    
    void innerExit();
    
    void beginFriendGachaGet();
    void beginUnusualGachaGet(bool free);
    void beginFriendGachaGet10();
    void beginUnusualGachaGet10();
    void beginSuiTangGachaGet(bool free);
    
    void beginOtherGachaGet(bool free);

    
    
    void beginSpeed(CCObject *sender);
    void beginSpeedST();

    void beginSpeedOT();
    
    
    void beginSpeedEnd();
    void beginSpeedEnd10();
    void beginSpeedSTEnd();
    
    
    void beginSpeedOTEnd();
    void beginSpeedOTEnd10();

    
    void removeGachaDlg();
    
    void showTips();
    
    void hideGachaInfo();

    bool m_isShowGoCompGachaBtn;
    
    bool m_isGotUnknowCompGachaHero;
    
    
    bool canGet;
    
    void shuaXin();
    
    
    TextNode * subNode1;
    TextNode * subNode2;
    TextNode * subNode3;
    TextNode * subNode4;
    TextNode * subNode5;
    
    
    
    TextNode * node1;
    TextNode * node2;
    TextNode * node3;
    TextNode * node4;
    TextNode * node5;
    
    CCSprite * sp1;
    CCSprite * sp2;
    CCSprite * sp3;
    CCSprite * sp4;
    CCSprite * sp5;

    
    
    
    
    
private:
    
    

    int  m_iReqType;
    
    
    void update(ccTime dt);
    
    void btnCallback(CCObject* sender);
    
    void onExit();
    
    void newBtn(CCMenuItem *&m_btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition);
    void enterFriendGacha(CCObject* sender);

    void enterUnusualGacha(CCObject* sender);
    void enterCardShop(CCObject* pSender); //卡片商店进入
    void cbCardShop();
    void enterCompGacha();
    
    void backEnter();
    
    void showInf();

    void cbCommDlg(CCObject* );
    
    void newbieGachaEnd();
    
    void newbieAllEnd(CCObject* pObj);
    
    void newbieGachaInfDlg();
    
    void cbNewbieInfDlg(CCObject* );
    
    void shareToWeibo();
    // yang share sanguo
    void shareSanGuo();
    
    void compGachaBtnCallback(CCObject *sender);
    
    void compGachaExchange(CCObject *sender);
    
    void dl2CallBack(CCObject *sender);
    
    void reqGachaConfig();
    
    void beginExchangeCard();
    
    void showCardInfo(CCObject *sender);
    
    void gachaGetCallback(CCObject *sender);
    
    void compGachaShowCardCb();

    void unsGachaClistCb(CCObject *sender);
    void frdGachaClistCb(CCObject *sender);
    void RareGacha10ShowCb();
    void FrdGacha10ShowCb();
    void showOrHideCompGachaHelp();
};

#endif
