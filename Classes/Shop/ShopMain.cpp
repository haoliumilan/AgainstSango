//
//  ShopMain.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "ShopMain.h"
#include "CMenuBtn.h"
#include "TextNode.h"
#include "TitleBar.h"
#include "MainScene.h"
#include "CommDlg.h"
#include "SimpleAudioEngine.h"
#include "OcProxy.h"
#include "TDCCVirtualCurrency.h"

#include "TDCCItem.h"

using namespace CocosDenshion;

#define kPayResultDlgTag  0x913
#define kTapjoyDlgTag     0x914

#define kPayBtnTagBase    0x999
#define kTapjoyBtnTag     0x888


//******************************************************************************
// Construct
//******************************************************************************
ShopMain::ShopMain()
{
    
}

void ShopMain::beginExtendFriendNum()
{
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    innerExit();
    if (CGameData::Inst()->getUsrInfo()->coin>=CGameData::Inst()->getCommonInfo()->friend_extend_coin)
    {     
        //好友数
        CCLog("f_e_n%d max%d",CGameData::Inst()->getUsrInfo()->friend_extend_num, CGameData::Inst()->getCommonInfo()->max_friend_extend_num);
        if (CGameData::Inst()->getUsrInfo()->friend_extend_num  >= CGameData::Inst()->getCommonInfo()->max_friend_extend_num )
        {
            m_goTo = false;
            infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip23"),false);
        }
        else
        {
            if(m_extendDlg == NULL){
                const char* title = CGameData::Inst()->getLanguageValue("shopTip21");
                char buf[200];
                CCLOG("maxExtend %d myExtend %d",CGameData::Inst()->getCommonInfo()->max_friend_extend_num,CGameData::Inst()->getUsrInfo()->friend_extend_num);
                sprintf(buf,CGameData::Inst()->getLanguageValue("shopTip20"),CGameData::Inst()->getCommonInfo()->friend_extend_coin,CGameData::Inst()->getCommonInfo()->friend_extend_num,CGameData::Inst()->getUsrInfo()->friend_extend_num,CGameData::Inst()->getCommonInfo()->max_friend_extend_num);
                m_extendDlg = new Dialog2(this, callfuncO_selector(ShopMain::extendFriendNum), title, buf, enDl2TwoBtn);
                m_extendDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
                addChild(m_extendDlg);
                m_extendDlg->release();
            }
        }
    }
    else
    {
        infoDialog(callfuncO_selector(ShopMain::dialog1CallbackGoToShop),CGameData::Inst()->getLanguageValue("shopTip7"),false);
        
    }
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_friendEnlarge");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
 
    
}

void ShopMain::beginCostMonthCard()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    innerExit();
    if (CGameData::Inst()->getUsrInfo()->coin>=10)
    {
        if(m_extendDlg == NULL){
            char cBuf[256];
            if (CUserInfo *pUserInfo = CGameData::Inst()->getUsrInfo())
            {
                time_t curTime = time(NULL);
                if (curTime<pUserInfo->month_endTime && 0<pUserInfo->month_append_cost)
                {
                     sprintf(cBuf,CGameData::Inst()->getLanguageValue("shop_cost_month_card2"),CGameData::Inst()->getCommonInfo()->month_cost_coin);
                }
                else
                {
                    sprintf(cBuf,CGameData::Inst()->getLanguageValue("shop_cost_month_card"),CGameData::Inst()->getCommonInfo()->month_cost_coin,CGameData::Inst()->getCommonInfo()->month_item_cost);
                }
            }
            m_extendDlg = new Dialog2(this, callfuncO_selector(ShopMain::okCostMonthCard),
                                      CGameData::Inst()->getLanguageValue("title_cost_month_card"), cBuf, enDl2TwoBtn);
            m_extendDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            addChild(m_extendDlg);
            m_extendDlg->release();
        }
    }
    else
    {
        infoDialog(callfuncO_selector(ShopMain::dialog1CallbackGoToShop),CGameData::Inst()->getLanguageValue("shopTip7"),false);
    }
    
//    char cBuf[256];
//    sprintf(cBuf,CGameData::Inst()->getLanguageValue("navi_cost_month_card"),CGameData::Inst()->getCommonInfo()->month_item_cost);
   
    MainScene::Inst()->showBottomPromptInfo("navi_cost_month_card");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

void ShopMain::okCostMonthCard(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(m_extendDlg)
    {
        removeChild(m_extendDlg, true);
        m_extendDlg = NULL;
    }
    
    if(pRet->bOk == true)
    {
        if(CGameData::Inst()->askCostMonthCard())
        {
            m_iReqType = enShopReqType_AppendCost;
            scheduleUpdate();
        }
    }
    else
    {
        enterShop();
    }
}

void ShopMain::extendFriendNum(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(m_extendDlg)
    {
        removeChild(m_extendDlg, true);
        m_extendDlg = NULL;
    }
    
    if(pRet->bOk == true)
    {
        if(CGameData::Inst()->frdExtendRequest())
        {
            m_iReqType = enShopReqType_extendFrd;
            scheduleUpdate();
        }
    }else{
        enterShop();
    }
    
   
}

//******************************************************************************
// enter
//******************************************************************************
void ShopMain::enter()
{
    if (CGameData::Inst()->isFighting())
        return;
    
    m_isPPPurchase = false;
    m_isTabjoy = false;
    m_sellDlg = NULL;
    m_commDlg = NULL;
    m_extendDlg = NULL;
    m_fanhuanDlg = NULL;

    m_flexibleLayer = NULL;
    
    m_isReqAllCard = false;
    m_goTo = true;
    payTag = 0;
    memset(payArr, 0, enPayfor_max);
    
    //title
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    //main menu
    CCMenuItem*       m_btn1;
    CCMenuItem*       m_btn2;
    CCMenuItem*       m_btn3;
    CCMenuItem*       m_btn4;
    CCMenuItem*       m_btn5;
    newBtn(m_btn1,
           CGameData::Inst()->getLanguageValue("title_yuanbao"),
           "ma_button.png",
           menu_selector(ShopMain::beginCoinShop),
           CCPointMake(35, 606));
    newBtn(m_btn2,
           CGameData::Inst()->getLanguageValue("title_huifu"),
           "ma_button.png",
           menu_selector(ShopMain::beginStaminaRecover),
           CCPointMake(35, 490));
    newBtn(m_btn3,
           CGameData::Inst()->getLanguageValue("title_expand"),
           "ma_button.png",
           menu_selector(ShopMain::beginBoxEnlarge),
           CCPointMake(35, 374));
    newBtn(m_btn4,
           CGameData::Inst()->getLanguageValue("title_friend expand"),
           "ma_button.png",
           menu_selector(ShopMain::beginExtendFriendNum),
           CCPointMake(35,374-116));
    
    newBtn(m_btn5,
           CGameData::Inst()->getLanguageValue("title_cost_month_card"),
           "ma_button.png",
           menu_selector(ShopMain::beginCostMonthCard),
           CCPointMake(35,374-116-116));
    
    m_btnMenu = CCMenu::menuWithItems( m_btn1,m_btn2,m_btn3,m_btn4,m_btn5, NULL);
    m_btnMenu->setPosition(CCPointMake(-1000, -1000));
    addChild(m_btnMenu, 40);
    
    if (CGameData::Inst()->isShopHuoDong())
    {
        if (CGameData::Inst()->getCommonInfo()->charge_type == 1)
        {
            m_listCoin = new CListLayer(this, callfuncO_selector(ShopMain::cbList),3,116,434);
        }
        else if (CGameData::Inst()->getCommonInfo()->charge_type == 2)
        {
            m_listCoin = new CListLayer(this, callfuncO_selector(ShopMain::cbList),4,116,536);
        }
    }
    else
    {
        m_listCoin = new CListLayer(this, callfuncO_selector(ShopMain::cbList));
    }
    addChild(m_listCoin, -1);
    m_listCoin->release();
    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ShopMain::btnCallback) );
    m_miBack->setTag(4);
    m_miBack->setAnchorPoint(CCPointZero);
    CCMenu* menu = CCMenu::menuWithItems( m_miBack, NULL);
    addChild(menu, 30);
    menu->setPosition(CCPointZero);
    
    
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    
    enterShop();
}

void ShopMain::beginCoinShop()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    innerExit();
#if defined (apppay)
    if (CGameData::Inst()->getStoreStatus() != 2) {
        if (CGameData::Inst()->getStoreStatus() == -1) {
            CGameData::Inst()->loadStore();
        }
        
        showPayResult(PaymentTimeOut);
    }
    else
    {
        if (CGameData::Inst()->isShopHuoDong())
        {
            reqChargeAward();
        }
        else
        {
            enterCoinList();
        }
    }
#elif defined (alipay)|| defined (kPltfmPp)
    if (CGameData::Inst()->isShopHuoDong())
    {
        reqChargeAward();
    }
    else
    {
        enterCoinList();
    }
#endif
}

void ShopMain::showAlipayOver(int payType)
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    //支付失败
    if (payType!=0) {
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else
    {
        if(CGameData::Inst()->reqChargeAward()){
            m_iReqType = enShopReqType_RefreshAlipay;
            scheduleUpdate();
        }
    }
}

void ShopMain::reqChargeAward()
{
    if (CGameData::Inst()->reqChargeAward())
    {
        m_iReqType = enShopReqType_ChargeAward;
        scheduleUpdate();
    }
}

void ShopMain::reqAllcard_update() //新增  1.13
{
    if (CGameData::Inst()->allCardRequest()) {
        m_iReqType = enAllCardReqType_Update;
        scheduleUpdate();
    }
}

void ShopMain::beginStaminaRecover()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    innerExit();
    

    if (CGameData::Inst()->getUsrInfo()->stamina == CGameData::Inst()->getUsrInfo()->max_stamina) {
        infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip4"),false);
    }
    else
    {
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->recover_stamina_coin){
            if(m_sellDlg == NULL){
                const char* title = CGameData::Inst()->getLanguageValue("shopTip2");
                char buf[100];
                snprintf(buf,99,"%s%hu%s", CGameData::Inst()->getLanguageValue("shopTip5"),CGameData::Inst()->getCommonInfo()->recover_stamina_coin, CGameData::Inst()->getLanguageValue("shopTip6"));
                m_sellDlg = new Dialog2(this, callfuncO_selector(ShopMain::cbStaminaRecoverDlg), title, buf,enDl2TwoBtn);
                addChild(m_sellDlg, 100);
                m_sellDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
                m_sellDlg->release();
                
                
            }
        }
        else{
            infoDialog(callfuncO_selector(ShopMain::dialog1CallbackGoToShop),CGameData::Inst()->getLanguageValue("shopTip7"),false);
        }
    }

    
    MainScene::Inst()->showBottomPromptInfo("naviText_staminaRecover");
 //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
}
void ShopMain::beginBoxEnlarge()
{   
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    innerExit();
    if (CGameData::Inst()->getUsrInfo()->coin>=CGameData::Inst()->getCommonInfo()->card_extend_coin) {
        if(m_sellDlg == NULL){
            const char* title = CGameData::Inst()->getLanguageValue("shopTip3");
            char buf[100];
            //使用%d个元宝,可使军营扩容%d个位置,现在最大
            snprintf(buf, 99,"%s%hu%s%hu%s%hu",CGameData::Inst()->getLanguageValue("shopTip8"),CGameData::Inst()->getCommonInfo()->card_extend_coin,CGameData::Inst()->getLanguageValue("shopTip9"),CGameData::Inst()->getCommonInfo()->card_extend_num,CGameData::Inst()->getLanguageValue("shopTip10"), CGameData::Inst()->getUsrInfo()->max_card_num);
            m_sellDlg = new Dialog2(this, callfuncO_selector(ShopMain::cbBoxEnlargeDlg), title, buf,enDl2TwoBtn);
            addChild(m_sellDlg, 100);
            m_sellDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            m_sellDlg->release();
        }
    }
    else
    {
        infoDialog(callfuncO_selector(ShopMain::dialog1CallbackGoToShop),CGameData::Inst()->getLanguageValue("shopTip11"),false);
    }
//    char buf[100];
//    snprintf(buf, 99, "naviText_boxEnlarge_%d", int(CCRANDOM_0_1() * 2) + 1);
    
   
    MainScene::Inst()->showBottomPromptInfo("naviText_boxEnlarge_");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
}


void ShopMain::infoDialog(SEL_CallFuncO selector, const char* dsp,bool isBigWindow)
{
    Dialog1* infodialog;
    if(isBigWindow)
    {
        infodialog= new Dialog1(this, selector, dsp);
        addChild(infodialog, 500,kTapjoyDlgTag);
    }
    else 
    {
        infodialog= new Dialog1(this, selector, dsp,false);
        addChild(infodialog, 500,kPayResultDlgTag);
        
    }
    infodialog->setPosition(CCPointMake(320, 480));
    infodialog->setScale(0.0);
    infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    infodialog->release();
}




//******************************************************************************
// exit
//******************************************************************************
void ShopMain::exit()
{
    innerExit();
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// onExit
//******************************************************************************
void ShopMain::onExit()
{
    CCLayer::onExit();
}

//******************************************************************************
// innerExit
//******************************************************************************
void ShopMain::innerExit()
{    
    m_spTitle->stopAllActions();
    m_btnMenu->stopAllActions();
    m_listCoin->stopAllActions();
    m_miBack->stopAllActions();
     
    m_listCoin->setPosition(CCPointMake(-1000, -1000));
   // m_btnFriendMenu->setPosition(CCPointMake(-1000, -1000));
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    m_btnMenu->setPosition(CCPointMake(-1000, -1000));
    //    m_btn3->setPosition(CCPointMake(-1000, -1000));
    //    m_btn4->setPosition(CCPointMake(-1000, -1000));
    
    if (m_flexibleLayer != NULL)
    {
        removeChild(m_flexibleLayer, true);
        m_flexibleLayer = NULL;
    }
    
}

void  ShopMain::newBtn(CCMenuItem *&m_btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition)
{
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName(pic);
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName(pic);
    
    m_btn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, selector);
    m_btn->setPosition(newPosition);
    m_btn->setAnchorPoint(CCPointZero);
    
    CCSprite* spTitle = CCSprite::spriteWithSpriteFrameName(label);
    if (spTitle) {
        spTitle->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                         spMenu1->getContentSize().height * 0.5));
        m_btn->addChild(spTitle);
    }
    else {
        TextNode* lbText = TextNode::textWithString(label, CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
        lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                        spMenu1->getContentSize().height * 0.5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        m_btn->addChild(lbText, 2);
    }
}


void ShopMain::enterShop()
{
    m_isReqAllCard = false;
    MainScene::Inst()->enableBottomPanel(true);//新增 1.13
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_shop"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    m_btnMenu->setPosition(CCPointMake(640, 0));
    m_btnMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)),0.8f));
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_shop");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}


void ShopMain::cbStaminaRecoverDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    
    if(pRet->bOk == true){
        if(CGameData::Inst()->getUsrInfo()->coin>0)
        {
            if(CGameData::Inst()->recoverStaminaRequest())
            {
                m_iReqType = enShopReqType_StaminaRecover;
                scheduleUpdate();
            }
            else
            {
//                removeChild(m_sellDlg, true);
//                m_sellDlg = NULL;
//                return;
            }
        }
    }   
    else {
            enterShop();    
    }
    
    removeChild(m_sellDlg, true);
    m_sellDlg = NULL;
   // enterShop();
}

void ShopMain::cbBoxEnlargeDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(CGameData::Inst()->getUsrInfo()->coin>0)
    {
        if(pRet->bOk == true){
            if(CGameData::Inst()->packExtendRequest()){
                m_iReqType = enShopReqType_BoxEnlarge;
                scheduleUpdate();
            }
        }   
        else {
            enterShop();
        }
    }
  //  m_sellDlg->exit();
    removeChild(m_sellDlg, true);
    m_sellDlg = NULL;
    //enterShop();
}


void ShopMain::update(ccTime dt)
{
    
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL && (m_iReqType!=enShopReqType_GetCoinList ||( m_iReqType==enShopReqType_GetCoinList &&comState != kReqStatOk))){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    payTag = 0;
    unscheduleUpdate();
    if(comState == kReqStatOk){
        MainScene::Inst()->showTips();
        switch (m_iReqType)
        {
            case enShopReqType_ChargeAward:
            {
//               enterCoinList();  
//                MainScene::Inst()->enableBottomPanel(true);
                reqAllcard_update();
            }
                break;
                
            case enShopReqType_GetCoinList:
                 sendCoinlistToApple();
                break;
            case enShopReqType_PaySuccess:
                
                //talkingdata
                
#ifdef TalkData
                TDCCVirtualCurrency::onChargeSuccess(OcProxy::Inst()->getReceipt());
#else
                
#endif
                
//                CCLog("%s" , OcProxy::Inst()->getReceipt());
                
                CGameData::Inst()->finishPayment();
                CGameData::Inst()->updateCoin();
                

                
                //showPayResult(true);
                if(m_commDlg == NULL){
                    MainScene::Inst()->enableBottomPanel(false);
                    m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::dialog1Callback), enCommDlgTp_retry);
                    addChild(m_commDlg, 1000);
                    m_commDlg->setPosition(CCPointMake(320, 480));
                    m_commDlg->release();
                    m_commDlg->setScale(0.0);
                    m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                }
                
                break;
            case enShopReqType_RefreshAlipay:
                CGameData::Inst()->updateCoin();
                showPayResult(PaymentAliSuccess);
                break;
            case enShopReqType_StaminaRecover:
                m_goTo = false;
                infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip12"),false);
                
                
#ifdef TalkData
                TDCCItem::onPurchase("回复体力", 1, CGameData::Inst()->getCommonInfo()->recover_stamina_coin);
#else
                
#endif
                
                
                break;
            case enShopReqType_BoxEnlarge:
                m_goTo = false;
                infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip13"),false);
                
                
#ifdef TalkData
                TDCCItem::onPurchase("扩充军营", 1, CGameData::Inst()->getCommonInfo()->card_extend_coin);
#else
                
#endif
                
                
                break;
            case enShopReqType_PayRefresh:
                //enterCoinList();
                
                if (CGameData::Inst()->isShopHuoDong())
                {
                    reqChargeAward();
                }
                else
                {
                    enterCoinList();
                }
                
                MainScene::Inst()->enableBottomPanel(true);
                break;
            case enShopReqType_extendFrd:
                infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip22"),false);
                
                
#ifdef TalkData
                TDCCItem::onPurchase("好友上限", 1, CGameData::Inst()->getCommonInfo()->friend_extend_coin);
#else
                
#endif
                
                
                break;
            case enShopReqType_AppendCost:
            {
                infoDialog(callfuncO_selector(ShopMain::dialog1Callback),CGameData::Inst()->getLanguageValue("shop_cost_up_success"),false);
                
                
#ifdef TalkData
                TDCCItem::onPurchase("统御力月卡", 1, CGameData::Inst()->getCommonInfo()->month_cost_coin);
#else
                
#endif
                
                
                
                
                break;
            }
                
            case enShopReqType_getTradeNo:
                CGameData::Inst()->payForCoin(m_iIndex-kPayBtnTagBase);
                //schedule(schedule_selector(ShopMain::waitPurchaseCb), 0.5);
                break;
            case enAllCardReqType_Update:{ //新增 1.13
                
                 enterCoinList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            }
            default:
                break;
        }
    }
    else {
        //付费失败
        if((CGameData::Inst()->isPaying()) && (comState == 1)){
            CGameData::Inst()->finishPayment();
        }
        
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            m_commDlg->setNoCancel();
        }
    }
    
}

void ShopMain::sendCoinlistToApple()
{
    //m_listCoin->clearAll();
    
    //CGameData::Inst()->setPayResponse(payCB);
    
    //CGameData::Inst()->loadStore();
    enterCoinList();
}



void ShopMain::enterCoinList()
{
    m_listCoin->clearAll();
    m_isReqAllCard = false;
    
    // 如果有6元大礼包或者按月返元宝礼包，tapjoy显示在最下面，否则tapjoy显示在最上面；当然，前提是tapjoy可以显示；
    CUserInfo* pUserInfo = CGameData::Inst()->getUsrInfo();
    CommonInfo* pCommonInfo = CGameData::Inst()->getCommonInfo();
    bool bShowTapjoy = (pCommonInfo->tapjoy_fg == true && pUserInfo->max_fg == false);
    
    CGameData::Inst()->updateCoin();
    
    if (pUserInfo->bSpecialIap[enSpecialIap_6RMBGift_00] == false
        && pUserInfo->bSpecialIap[enSpecialIap_30RMBGift_12] == false
        && pUserInfo->bSpecialIap[enSpecialIap_198RMBGift_13] == false
        && pUserInfo->bSpecialIap[enSpecialIap_198RMBMonth_09] == false
        && pUserInfo->bSpecialIap[enSpecialIap_30RMBMonth_11] == false
        && bShowTapjoy)
    {
        coinBtnAdd("free", "free", kTapjoyBtnTag, true);
    }
    
    if (CGameData::Inst()->isShopHuoDong() && m_flexibleLayer == NULL)
    {
        m_flexibleLayer = new FlexibleLayer();
        addChild(m_flexibleLayer);
        m_flexibleLayer->release();
        m_flexibleLayer->setPosition(ccp(640, 0));
        m_flexibleLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)), 0.8f));
    }
    

    for (int i=0; i<CGameData::Inst()->getProductCnt(); i++) {
        


        
        coinBtnAdd(CGameData::Inst()->getIapIdAtIdx(i),
                   CGameData::Inst()->getPriceAtIdx(i),
                   i + kPayBtnTagBase,
                   false);
    }
    
    if ((pUserInfo->bSpecialIap[enSpecialIap_6RMBGift_00]
         || pUserInfo->bSpecialIap[enSpecialIap_30RMBGift_12]
         || pUserInfo->bSpecialIap[enSpecialIap_198RMBGift_13]
         || pUserInfo->bSpecialIap[enSpecialIap_198RMBMonth_09]
         || pUserInfo->bSpecialIap[enSpecialIap_30RMBMonth_11])
        && bShowTapjoy)
    {
        coinBtnAdd("free", "free", kTapjoyBtnTag, true);
    }
    
    if(!m_isPPPurchase)
    {
        m_isPPPurchase = false;
        m_listCoin->setPosition(CCPointMake(640, 0));
        
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_yuanbao"));
        m_spTitle->setPosition(CCPointMake(-210, 766));
        
        m_miBack->setPosition(CCPointMake(-529, 731));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        m_listCoin->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
        m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    }
    else
    {
        m_listCoin->setPosition(CCPointZero);
        
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_yuanbao"));
        m_spTitle->setPosition(CCPointMake(320, 766));
        
        m_miBack->setPosition(CCPointMake(1, 731));
    }
    
//    char buf[100];
//    snprintf(buf, 99, "naviText_coin_%d", int(CCRANDOM_0_1() * 3) + 1);
  
     MainScene::Inst()->enableBottomPanel(true);
    
    MainScene::Inst()->showBottomPromptInfo("naviText_coin_");
   // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

void ShopMain::btnCallback(CCObject* sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    innerExit();
    enterShop();
}


void ShopMain::dialog1Callback(CCObject* sender)
{
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(m_isTabjoy == false)
    {      
        CCNode* payResultDlg = getChildByTag(kPayResultDlgTag);
        if(payResultDlg)
            removeChild(payResultDlg, true);
        innerExit();
#if defined (apppay)
        if (CGameData::Inst()->getStoreStatus() == 2)
        {
            
            if (CGameData::Inst()->isShopHuoDong())
            {
                reqChargeAward();
            }
            else
            {
                if (m_isReqAllCard) {
                    reqAllcard_update(); //新增 1.13
                }else{
                  enterCoinList();
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                }
              
            }
        }
        else
            enterShop();
#elif defined (alipay)  || defined (kPltfmPp)
        if (m_goTo) {
            m_goTo = true;
            if (CGameData::Inst()->isShopHuoDong())
            {
                reqChargeAward();
            }
            else
            {
                if (m_isReqAllCard) {
                    reqAllcard_update(); //新增 1.13
                }else
                    enterCoinList();
            }
        }else
            enterShop();
        
#endif
        
        

    }else {
        CCNode* payResultDlg = getChildByTag(kTapjoyDlgTag);
        if(payResultDlg)
            removeChild(payResultDlg, true);
        innerExit();
        OcProxy::Inst()->showOfferWall();
        MainScene::Inst()->enableBottomPanel(false);
       schedule(schedule_selector(ShopMain::waitTapJoyCb), 0.5);
        
    }
   
    m_isTabjoy =false;
}

void ShopMain::dialog1CallbackGoToShop(CCObject* sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    CCNode* payResultDlg = getChildByTag(kPayResultDlgTag);
    if(payResultDlg)
        removeChild(payResultDlg, true);
    innerExit();
    MainScene::Inst()->enableBottomPanel(true);
    beginCoinShop();
}


void ShopMain::coinBtnAdd(const char *iap_id, const char * price,int index,bool isTapjoy)
{
    CMenuBtn* btn;
    
    btn = new CMenuBtn(price, iap_id, isTapjoy);
    m_listCoin->push_back(btn);
    
    if(isTapjoy == false)
    {
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        
        
        CCMenuItem *buybtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ShopMain::payBtnCB));
        buybtn->setTag(index);
        TextNode *itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip14"), kBtnTitleHeight);
        itemLabel->setPosition(ccp(77, 42));
        itemLabel->setShadowColor(ccBLACK);
        buybtn->addChild(itemLabel);
        buybtn->setAnchorPoint(ccp(0, 0));
        
        CCSprite* spShadowMenu = NULL;
        
        if ( strncmp(iap_id, kProductId11, 99) == 0 ){
            
            if ( CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_30RMBMonth_11] == false){
                spShadowMenu = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            }else{
                payArr[enPayforzhaocaijinbao] = index;
            }
            buybtn->setTag(DialogType_zhaocaijinbao);
        
        }
        else if ( strncmp(iap_id, kProductId09, 99) == 0  ){
            if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_198RMBMonth_09] == false) {
                  spShadowMenu = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            }else{
                 payArr[enPayforcaiyungungun] = index;
            }
            buybtn->setTag(DialogType_caiyungungun);
        }
        else if (strncmp(iap_id, kProductId12, 99) == 0 ){
            payArr[enPayformengjiangwushuang] = index;
            buybtn->setTag(DialogType_mengjiangwushuang);
        }
        else if (strncmp(iap_id, kProductId13, 99) == 0 ){
            payArr[enPayforluanshishenjiang] = index;
            buybtn->setTag(DialogType_luanshishenjiang);
        }
        else{
        }
        
        if (spShadowMenu) {
            spShadowMenu->setPosition(ccp(77, 42));
            buybtn->addChild(spShadowMenu);
        }
        
        
        CCMenu *menu = CCMenu::menuWithItems(buybtn,NULL);
        btn->addChild(menu);
        menu->setPosition(CCPointZero);
        buybtn->setPosition(ccp(132,-44));
        buybtn->setScale(0.9);
        
        
        
    }else{
        CCSprite* spMenu1 = CCSprite::spriteWithFile("button_diamonds.png");
        CCSprite* spMenu2 = CCSprite::spriteWithFile("button_diamonds.png");
        if(spMenu1 && spMenu2){
            CCMenuItem *buybtn = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                       spMenu2,
                                                                       this,
                                                                       menu_selector(ShopMain::payBtnCB));
            buybtn->setTag(index);
            
            buybtn->setAnchorPoint(ccp(0, 0));
            CCMenu *menu = CCMenu::menuWithItems(buybtn,NULL);
            btn->addChild(menu, -1);
            menu->setPosition(CCPointZero);
            buybtn->setPosition(ccp(-spMenu1->getContentSize().width * 0.5 - 6,
                                    -spMenu1->getContentSize().height * 0.5));
        }
    }
}

void ShopMain::payBtnCB(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    
    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_zhaocaijinbao) {
      if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_30RMBMonth_11] == false ) {
            return;
        }
        payTag = payArr[enPayforzhaocaijinbao];
        m_isReqAllCard = true;
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip29");
            
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(ShopMain::ExplanFanhuan), title, CGameData::Inst()->getLanguageValue("shopTip32"), enDl2TwoBtn);
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
           
            addChild(m_fanhuanDlg);
            m_fanhuanDlg->release();
        }
         return;
    }

    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_caiyungungun) {
        if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_198RMBMonth_09] == false ) {
            return;
        }
        
        payTag = payArr[enPayforcaiyungungun];
        m_isReqAllCard = true;
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip29");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(ShopMain::ExplanFanhuan), title, CGameData::Inst()->getLanguageValue("shopTip33"), enDl2TwoBtn);
           
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg);
            m_fanhuanDlg->release();
        }
        return;
    }

    if (((CCMenuItemImage *)pSender)->getTag() == DialogType_mengjiangwushuang) {
         payTag = payArr[enPayformengjiangwushuang];
         m_isReqAllCard = true;
         if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip34");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(ShopMain::ExplanFanhuan), title, CGameData::Inst()->getLanguageValue("shopTip35"), enDl2TwoBtn);
         
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg);
            m_fanhuanDlg->release();
        }
        return;
    }
    if (((CCMenuItemImage *)pSender)->getTag() == DialogType_luanshishenjiang) {
        payTag = payArr[enPayforluanshishenjiang];
        m_isReqAllCard = true;
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip34");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(ShopMain::ExplanFanhuan), title, CGameData::Inst()->getLanguageValue("shopTip36"), enDl2TwoBtn);
          
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg);
            m_fanhuanDlg->release();
        }
        return;
    }

    
    if(((CCMenuItemImage *)pSender)->getTag() != kTapjoyBtnTag){

        if(m_commDlg == NULL){
            //MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
       
        
    }
     MainScene::Inst()->enableBottomPanel(false);
#if !defined(kPltfmPp)
    innerExit();
#endif
    payForCoin(((CCMenuItemImage *)pSender)->getTag());
}

void ShopMain::ExplanFanhuan(CCObject* pSender)   //礼包卡返还的
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pSender;
    
    if(m_fanhuanDlg)
    {
        removeChild(m_fanhuanDlg, true);
        m_fanhuanDlg = NULL;
    }
    
    if(pRet->bOk == true)
    {
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
#if !defined(kPltfmPp)
        innerExit();
#endif
        if (payTag > 0) {
            payForCoin(payTag);
            MainScene::Inst()->enableBottomPanel(false);
        }else{
            CCLOG("请求充值的数据有问题");
        }
        
        
    }else{
        m_isReqAllCard = false;
       // enterShop();
    }

}

void ShopMain::showPayResult(int result)
{
    Dialog1* payResultDlg;
    switch (result) {
        case PaymentFailed:
            payResultDlg= new Dialog1(this, callfuncO_selector(ShopMain::dialog1CallbackGoToShop),CGameData::Inst()->getLanguageValue("shopTip15"),false);   
            break;
        case PaymentTimeOut:
            payResultDlg= new Dialog1(this, callfuncO_selector(ShopMain::dialog1Callback), CGameData::Inst()->getLanguageValue("shopTip16"),false);
            break;
        case PaymentAliSuccess:
            payResultDlg= new Dialog1(this, callfuncO_selector(ShopMain::dialog1Callback), CGameData::Inst()->getLanguageValue("buy_success"),false);
            
            break;
        default:
            break;
    }
    addChild(payResultDlg, 500,kPayResultDlgTag);
    payResultDlg->setPosition(CCPointMake(320, 480));
    payResultDlg->setScale(0.0);
    payResultDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    payResultDlg->release();
    
}



void ShopMain::payForCoin(int index)
{
    m_iIndex = index;
    if(index == kTapjoyBtnTag)
    {
        char buf[400];
        int points = CGameData::Inst()->getUsrInfo()->tjPoints;
        int coins = CGameData::Inst()->getUsrInfo()->tjCoins;
        int points_per_coin = CGameData::Inst()->getUsrInfo()->tjPoints_per_coin;
        int max_coins = CGameData::Inst()->getUsrInfo()->tjMax_coins;
        sprintf(buf,CGameData::Inst()->getLanguageValue("shopTip18"), points,coins,max_coins,points_per_coin);
        
        m_isTabjoy = true;
        infoDialog(callfuncO_selector(ShopMain::dialog1Callback),buf,true);
    }
    else{
#if defined (alipay)
        if (CGameData::Inst()->getOrderNo(CGameData::Inst()->getIapIdAtIdx(index-kPayBtnTagBase), CGameData::Inst()->getUsrInfo()->uid)){
            m_iReqType = enShopReqType_getTradeNo;
            scheduleUpdate();
        }
#elif defined (apppay)
        CGameData::Inst()->payForCoin(index-kPayBtnTagBase);
        schedule(schedule_selector(ShopMain::waitPurchaseCb), 0.5);
#elif defined (kPltfmPp)
        if(CGameData::Inst()->reqPPBilloNo(OcProxy::Inst()->getIapIdAtIdx(index-kPayBtnTagBase)))
        {
            m_iReqType = enShopReqType_PayppBillNo;
            schedule(schedule_selector(ShopMain::waitPPBilloNo), 0.5);
        }
#endif
    }
}

void ShopMain::waitPPBilloNo(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ShopMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(comState == kReqStatOk){
        unschedule(schedule_selector(ShopMain::waitPPBilloNo));
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        MainScene::Inst()->enableBottomPanel(true);
        schedule(schedule_selector(ShopMain::waitPPPurchaseCb), 0.5);
        OcProxy::Inst()->buyPp(OcProxy::Inst()->getIntPriceAtIdx(m_iIndex-kPayBtnTagBase),CGameData::Inst()->getPPBilloNo().c_str(),OcProxy::Inst()->getTitleAtIdx(m_iIndex-kPayBtnTagBase),CGameData::Inst()->getUserId());
    }
}

void ShopMain::waitPPPurchaseCb(ccTime dt)
{
    int iPurchaseStatus = OcProxy::Inst()->getPpPurchaseStatus();
    if(iPurchaseStatus == 1)
        return;
    //支付失败
    if(iPurchaseStatus == -1){
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else if(iPurchaseStatus == 2){
        if(CGameData::Inst()->getShopRequest())
        {
            m_isPPPurchase = true;
            m_iReqType =  enShopReqType_PayRefresh;
            scheduleUpdate();
        }
    }
    
    OcProxy::Inst()->resetPpPurchaseStatus();
    unschedule(schedule_selector(ShopMain::waitPPPurchaseCb));
}



void ShopMain::waitTapJoyCb(ccTime dt)
{
    int iTapjoyStatus = CGameData::Inst()->getTapjoyStatus();
    
    if(iTapjoyStatus == 1)
        return;
    
    //tapJoy窗口回来
    if(iTapjoyStatus == 2)
    {

        if(CGameData::Inst()->getShopRequest())
        {
            m_iReqType =  enShopReqType_PayRefresh;
            MainScene::Inst()->enableBottomPanel(false);
            scheduleUpdate();
        }
    }
    
    CGameData::Inst()->resetTapjoyStatus();
    unschedule(schedule_selector(ShopMain::waitTapJoyCb));

}
void ShopMain::waitPurchaseCb(ccTime dt)
{

    int iPurchaseStatus = CGameData::Inst()->getPurchaseStatus();
    if(iPurchaseStatus == 1)
        return;
    
    //支付失败
    if(iPurchaseStatus == -1){
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else if(iPurchaseStatus == 2){
        if(CGameData::Inst()->sendPaySuccessInfo()){
            m_iReqType = enShopReqType_PaySuccess;
            scheduleUpdate();
        }
    }
    
    
    CGameData::Inst()->resetPurchaseStatus();
    unschedule(schedule_selector(ShopMain::waitPurchaseCb));
}

void  ShopMain::cbList(CCObject* pObj)
{

}

void ShopMain::cbCommDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
        
    if(isOk == true){
        switch (m_iReqType)
        {
            case enShopReqType_PayppBillNo:
                if(CGameData::Inst()->reqPPBilloNo(OcProxy::Inst()->getIapIdAtIdx(m_iIndex-kPayBtnTagBase)))
                {
                    schedule(schedule_selector(ShopMain::waitPPBilloNo), 0.5);
                }
                break;
            case enShopReqType_ChargeAward:
                if (CGameData::Inst()->reqChargeAward())
                {
                    scheduleUpdate();
                }
                break;
                
            case enShopReqType_GetCoinList:
                if(CGameData::Inst()->getShopRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enShopReqType_StaminaRecover:
                if(CGameData::Inst()->recoverStaminaRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enShopReqType_BoxEnlarge:
                if(CGameData::Inst()->packExtendRequest()){
                    scheduleUpdate();
                }
                break;
            case enShopReqType_PaySuccess:
                if(CGameData::Inst()->sendPaySuccessInfo()){
                    scheduleUpdate();
                }
                
                break;
            case enShopReqType_RefreshAlipay:
                if(CGameData::Inst()->reqChargeAward()){
                    scheduleUpdate();
                }
                
                break;
            case enShopReqType_PayRefresh:
                if(CGameData::Inst()->getShopRequest()){
                    scheduleUpdate();
                }
                break;
            case enShopReqType_extendFrd:
                if(CGameData::Inst()->frdExtendRequest())
                {
                    m_iReqType = enShopReqType_extendFrd;
                    scheduleUpdate();
                }
                break;
            case enShopReqType_getTradeNo:
                if (CGameData::Inst()->getOrderNo(CGameData::Inst()->getIapIdAtIdx(m_iIndex-kPayBtnTagBase), CGameData::Inst()->getUsrInfo()->uid))
                {
                    m_iReqType = enShopReqType_getTradeNo;
                    scheduleUpdate();
                }
                break;
            case enShopReqType_AppendCost:
            {
                if(CGameData::Inst()->askCostMonthCard())
                {
                    scheduleUpdate();
                }
                break;
            }
                case enAllCardReqType_Update:  //新增 1.13
            {
                if (CGameData::Inst()->allCardRequest()) {
                    m_iReqType = enAllCardReqType_Update;
                    scheduleUpdate();
                }
                
                break;
            }
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();

        MainScene::Inst()->showBottomPanel(true);
        MainScene::Inst()->showUsrInfPanel(true);
        innerExit();
        enterShop();
    }
}


