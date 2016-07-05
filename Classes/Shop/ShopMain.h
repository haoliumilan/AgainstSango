//
//  ShopMain.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_ShopMain_h
#define AgainstWar_ShopMain_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "Dialog2.h"
#include "CListLayer.h"
#include "CScrollBar.h"
#include "Dialog1.h"
#include "CGamedata.h"
#include "FlexibleLayer.h"



using namespace cocos2d;

class CommDlg;
class TitleBar;

enum {
    enShopReqType_Null=0,
    enShopReqType_GetCoinList,
    enShopReqType_StaminaRecover,
    enShopReqType_BoxEnlarge,
    enShopReqType_PaySuccess,
    enShopReqType_PayRefresh,
    enShopReqType_extendFrd,
    enShopReqType_ChargeAward,
    enShopReqType_getTradeNo,
    enShopReqType_RefreshAlipay,
    enShopReqType_AppendCost,
    enAllCardReqType_Update,  //新增  1.13
    enShopReqType_PayppBillNo,
    
    };

//enum {
//    enPayforzhaocaijinbao = 0,
//    enPayforcaiyungungun,
//    enPayformengjiangwushuang,
//    enPayforluanshishenjiang,
//    enPayfor_max,
//    
//};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ShopMain : public CUiLayerBase
{
    
public:
    ShopMain();
    
    virtual void enter();
    virtual void exit();
    virtual void onExit();
    
    void innerExit();
    
    Dialog2* m_sellDlg;
    Dialog2* m_extendDlg;
    Dialog2* m_fanhuanDlg; //解释分三十天返还
    int m_iReqType;
    
    TitleBar*       m_spTitle;
    CCMenuItem*     m_miBack;
    
    CCMenu*         m_btnMenu;

    CListLayer*     m_listCoin;
    CommDlg*        m_commDlg;
    
    bool m_isPPPurchase;
    
    void enterShop();
   // void beginShop();
    void beginCoinShop();
    void showAlipayOver(int payType);
    void beginStaminaRecover();
    void beginBoxEnlarge();

    
    void cbSellDlg(CCObject* pObj);
    
    
    void cbStaminaRecoverDlg(CCObject* pObj);
    void cbBoxEnlargeDlg(CCObject* pObj);
    
    void btnCallback(CCObject* sender);
    void dialog1Callback(CCObject* sender);
    
    void dialog1CallbackGoToShop(CCObject* sender);
    
    void cbCommDlg(CCObject* pObj);
    
    void sendCoinlistToApple();
    
    void payForCoin(int index);
    
    void coinBtnAdd(const char *iap_id, const char *price,int index,bool isTapjoy);
    
    void payBtnCB(CCObject *pSender);
    
    void showPayResult(int result);
    
    void infoDialog(SEL_CallFuncO selector, const char* dsp,bool isBigWindow);
    
    void addShopItem(const char *title,int price);
    
    bool  m_isTabjoy;

    void beginExtendFriendNum();
    
    void extendFriendNum(CCObject* pObj);
    
    void beginCostMonthCard();
    void okCostMonthCard(CCObject* pObj);
    
    void ExplanFanhuan(CCObject* pSender);
    
    void waitPPBilloNo(ccTime dt);
    void waitPPPurchaseCb(ccTime dt);
    

private:
    void waitPurchaseCb(ccTime dt);
    void waitTapJoyCb(ccTime dt);
    void update(ccTime dt);
    void enterCoinList();
    int             m_iFloorId;
    int             m_iRoomId;
    int             m_iRoomSel;
    const char*     m_szDgType;
    
    int             m_iIndex;
    
    bool            m_goTo;     //是否跳转到元宝商店
    
    long             payTag;
    long             payArr[enPayfor_max];
    bool            m_isReqAllCard;
    
 

    void  cbList(CCObject* pObj);
        
    void newBtn(CCMenuItem *&m_btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition);
    
    FlexibleLayer* m_flexibleLayer;
    void reqChargeAward();
    
    void reqAllcard_update();

};

#endif
