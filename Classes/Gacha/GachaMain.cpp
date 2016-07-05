//
//  GachaMain.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012Âπ?OneClick Co.,Ltd. All rights reserved.
//


#include "GachaMain.h"
#include "GachaGet.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "CommDlg.h"
#include "TitleBar.h"
#include "Arrow.h"
#include "DialogForGacha.h"
#include "GachaWeb.h"
#include "Dialog1.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "Dialog2.h"
#include "CompGachaWeb.h"
#include "CListLayer.h"
#include "RareGacha10Show.h"
#include "FrdGacha10Show.h"
#include "OcProxy.h"

#include "TDCCItem.h"

using namespace CocosDenshion;


#define GACHAWEB
#define kNewbieArrowTag     0xabcd

//******************************************************************************
// Construct
//******************************************************************************
GachaMain::GachaMain()
: m_isGachaPlus(false)
, m_isShowGoCompGachaBtn(false)
, m_isGotUnknowCompGachaHero(false)
{

}

//******************************************************************************
// enter
//******************************************************************************
void GachaMain::enter()
{
    if (CGameData::Inst()->isFighting())
        return;
    
    
    
    xxxx = 0;
    m_isGachaPlus = false;
    m_isShowGoCompGachaBtn = false;
    m_isGotUnknowCompGachaHero = false;
    m_compGachaWeb = NULL;
    m_compGachaList = NULL;
    pGachaLayer = NULL;
    m_commDlg = NULL;
    compGachaBgBottom = NULL;
    compGachaBgMiddle = NULL;
    compGachaBgTop = NULL;
    frNumberTips = NULL;
    unsNumberTips = NULL;
    m_CardShop = NULL;
    //title
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    
    newBtn(m_btn1,
           CGameData::Inst()->getLanguageValue("title_freegacha"),
           "ma_button.png",
           menu_selector(GachaMain::enterFriendGacha),
           CCPointMake(35, 606));

    
    if (CGameData::Inst()->getCommonInfo()->bOpen_cards_shop)
    {
        newBtn(m_btn2,
               "",
               "ma_button.png",
               menu_selector(GachaMain::enterCardShop),
               CCPointMake(35, 490));
        
        CCSprite* sp = CCSprite::spriteWithFile("card_shop_title.png");
        sp->setPosition(CCPointMake(277, 66));
        m_btn2->addChild(sp,100);
        
        m_btnMenu = CCMenu::menuWithItems( m_btn1,m_btn2, NULL);
        m_btnMenu->setPosition(CCPointMake(-1000, -1000));
        addChild(m_btnMenu, 40);
    }
    else
    {
        newBtn(m_btn2,
               "",
               "ma_button.png",
               menu_selector(GachaMain::enterUnusualGacha),
               CCPointMake(35, 490));
        newBtn(m_btn3,
               "",
               "ma_button.png",
               menu_selector(GachaMain::reqGachaConfig),
               CCPointMake(35, 374));
        
//        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("gachagod.png");
//        sp->setPosition(CCPointMake(277, 66));
//        m_btn2->addChild(sp,100);

        CCSprite* sp = CCSprite::spriteWithFile("newAdd_QianJiang.png");
        sp->setPosition(CCPointMake(277, 66));
        m_btn2->addChild(sp,100);

        
        sp = CCSprite::spriteWithSpriteFrameName("title_swap.png");
        sp->setPosition(CCPointMake(277 + 8, 66));
        m_btn3->addChild(sp,100);
        
        m_btnMenu = CCMenu::menuWithItems( m_btn1,m_btn2,m_btn3, NULL);
        m_btnMenu->setPosition(CCPointMake(-1000, -1000));
        addChild(m_btnMenu, 40);
    }
    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(GachaMain::btnCallback) );
    m_miBack->setTag(4);
    m_miBack->setAnchorPoint(CCPointZero);
    CCMenu* menu = CCMenu::menuWithItems( m_miBack, NULL);
    addChild(menu, 50);
    menu->setPosition(CCPointZero);
    
    
#ifdef GACHAWEB
    
    m_pWebBg = CCSprite::spriteWithFile("ma_prompt.png");
    m_pWebBg->setPosition(ccp(-1000, -1000));
    
    addChild(m_pWebBg, 30);
    
    gachaWeb = new GachaWeb();
    addChild(gachaWeb);
    gachaWeb ->setPosition(ccp(-1000, -1000));
    
#endif
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    
    //分享按钮
    
    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    m_shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(GachaMain::shareSanGuo));
    m_shareBtn->setAnchorPoint(CCPointZero);
    CCMenu *shareMenu = CCMenu::menuWithItem(m_shareBtn);
    addChild(shareMenu,2);
    shareMenu->setPosition(CCPointZero);
    TextNode *shareLabel;
    shareLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shareTip"), 30);
    shareLabel->setPosition(ccp(53, 35));
    shareLabel->setShadowColor(ccBLACK);
    m_shareBtn->addChild(shareLabel);
    m_shareBtn->setPosition(CCPoint(-1000, -1000));
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 150);
    m_showCard->release();

    if(CGameData::Inst()->isNewbie()){
        Arrow* pArrow = new Arrow(30, 255);
        pArrow->setPosition(CCPointMake(470, 550));
        addChild(pArrow, 500, kNewbieArrowTag);
        pArrow->release();

        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if (!CGameData::Inst()->getCommonInfo()->bOpen_cards_shop)
        m_btn3->setIsVisible(CGameData::Inst()->getCommonInfo()->compgacha_fg);
        
    unsGachaClist = new CListLayer(this, callfuncO_selector(GachaMain::unsGachaClistCb));
    addChild(unsGachaClist);
    unsGachaClist->release();
    unsGachaClist->setPosition(ccp(640, 0));
    
    frdGachaClist = new CListLayer(this, callfuncO_selector(GachaMain::frdGachaClistCb));
    addChild(frdGachaClist);
    frdGachaClist->release();
    frdGachaClist->setPosition(ccp(640, 0));
    
    m_RareGacha10Show = new RareGacha10Show(this,callfuncO_selector(GachaMain::RareGacha10ShowCb));
    addChild(m_RareGacha10Show);
    m_RareGacha10Show->release();
    
    m_FrdGacha10Show = new FrdGacha10Show(this,callfuncO_selector(GachaMain::FrdGacha10ShowCb));
    addChild(m_FrdGacha10Show);
    m_FrdGacha10Show->release();

    enterGacha();
}


void GachaMain::FrdGacha10ShowCb()
{
    backEnter();
    hideGachaInfo();
    m_FrdGacha10Show->hide();
    MainScene::Inst()->showBottomPanel(true);
    reqGachaConfig();
}

void GachaMain::RareGacha10ShowCb()
{
    backEnter();
    hideGachaInfo();
    m_RareGacha10Show->hide();
    MainScene::Inst()->showBottomPanel(true);
    reqGachaConfig();
}
void GachaMain::frdGachaClistCb(CCObject *sender)
{
    CListLayerRet* pRet = (CListLayerRet*)sender;
    
    if(pRet->iBtnSel==0){
        DialogForGacha* infodialog;
        infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginFriendGachaGet),
                                       CGameData::Inst()->getUsrInfo()->gacha_pt,Dlg_GachaType_Frd1);
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }
    else{
        DialogForGacha* infodialog;
        infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginFriendGachaGet10),
                                       CGameData::Inst()->getUsrInfo()->gacha_pt,Dlg_GachaType_Frd10);
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }
    
    frdGachaClist->clearAll();
}




void GachaMain::beginSpeedEnd()
{
    CCNode * tt = getChildByTag(kDlg1ForGacha);
    
    if(tt)
        tt->removeFromParentAndCleanup(true);

    
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    
    pGachaLayer = new GachaGet(UnusualGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
    
}

void GachaMain::beginSpeedEnd10()
{
    CCNode * tt = getChildByTag(kDlg1ForGacha);
    
    if(tt)
        tt->removeFromParentAndCleanup(true);
    
    
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    pGachaLayer = new GachaGet(UnusualGacha10Type,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
    
}
void GachaMain::beginSpeedSTEnd()
{
    
    CCNode * tt = getChildByTag(kDlg1ForGacha);
    
    if(tt)
        tt->removeFromParentAndCleanup(true);
    
    
    MainScene::Inst()->enableBottomPanel(true);
    
    
    pGachaLayer = new GachaGet(UnusualGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
}

void GachaMain::beginSpeedOTEnd()
{
    CCNode * tt = getChildByTag(kDlg1ForGacha);
    
    if(tt)
        tt->removeFromParentAndCleanup(true);
    
    
    MainScene::Inst()->enableBottomPanel(true);
    
    
    pGachaLayer = new GachaGet(UnusualGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
}


void GachaMain::beginSpeedOTEnd10()
{
    
    CCNode * tt = getChildByTag(kDlg1ForGacha);
    
    if(tt)
        tt->removeFromParentAndCleanup(true);
    
    
    MainScene::Inst()->enableBottomPanel(true);
    
    pGachaLayer = new GachaGet(UnusualGacha10Type,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);

}




void GachaMain::beginSpeed(CCObject *sender)
{
    
    DialogForGachaRet* pRet = (DialogForGachaRet*)sender;
    
    
    if(pRet->bOk)
    {
//        GachaMain::beginUnusualGachaGet(false);
        
        
        
        
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->gacha_cost_coin) {
            innerExit();
            
            removeGachaDlg();
            
            if(CGameData::Inst()->isSoundOn)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
            
            if(CGameData::Inst()->isNewbie()){
                CCNode* pArrow = getChildByTag(kNewbieArrowTag);
                if(pArrow)
                    removeChild(pArrow, true);
                
                if(CGameData::Inst()->newbieRequest(0, 4, NULL)){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_NewbieGacha;
                }
            }
            else{
                
//                if (SaveData::Inst()->hasValidSavedData()
//                    && SaveData::Inst()->getReqGachaTime() != 0) {
//                    ml_requestTime = SaveData::Inst()->getReqGachaTime();
//                }
//                else {
//                    ml_requestTime = CGameData::Inst()->getCurrentTime();
//                    SaveData::Inst()->setReqCachaTime(ml_requestTime);
//                    SaveData::Inst()->flush();
//                }
                 ml_requestTime = CGameData::Inst()->getCurrentTime();
                if(CGameData::Inst()->chargeGachaRequest(false,"",ml_requestTime , false)){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_RareGachaSpeed;
                }
            }
        }
        else
        {
            
            
            
            removeGachaDlg();
            
            DialogForGacha* infodialog;
            
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_NoMoney);
            
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();
        }
        

        
       
    }
    else
    {
//        GachaMain::beginUnusualGachaGet10();
        
        
        int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt;
        if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
            innerExit();
            
            removeGachaDlg();
            
            if(CGameData::Inst()->isSoundOn)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
            
            
//            if (SaveData::Inst()->hasValidSavedData()
//                && SaveData::Inst()->getReqGachaTime() != 0) {
//                ml_requestTime = SaveData::Inst()->getReqGachaTime();
//            }
//            else {
//                ml_requestTime = CGameData::Inst()->getCurrentTime();
//                SaveData::Inst()->setReqCachaTime(ml_requestTime);
//                SaveData::Inst()->flush();
//            }
            
             ml_requestTime = CGameData::Inst()->getCurrentTime();
            if(CGameData::Inst()->multiChargeGachaRequest("",ml_requestTime)){
                scheduleUpdate();
                m_iReqType = enGachaReqType_MutiRareGachaSpeed;
            }
        }
        else
        {
            removeGachaDlg();
            
            DialogForGacha* infodialog;
            
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_NoMoney);
            
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();

        }

        
        
        
        
    }
    
    
    
    MainScene::Inst()->enableBottomPanel(false);
    
}
void GachaMain::beginSpeedST()
{
    
    
    if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->gacha_cost_coin) {
        innerExit();
        
        removeGachaDlg();
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
//        if (SaveData::Inst()->hasValidSavedData()
//            && SaveData::Inst()->getReqGachaTime() != 0) {
//            ml_requestTime = SaveData::Inst()->getReqGachaTime();
//        }
//        else {
//            ml_requestTime = CGameData::Inst()->getCurrentTime();
 //           SaveData::Inst()->setReqCachaTime(ml_requestTime);
//            SaveData::Inst()->flush();
//        }
        
         ml_requestTime = CGameData::Inst()->getCurrentTime();
        if(CGameData::Inst()->chargeGachaRequest(true,"",ml_requestTime, false)){
            scheduleUpdate();
            m_iReqType = enGachaReqType_SuiTangGachaSpeed;
        }
    }
    else
    {
        
        
        
        removeGachaDlg();
        
        DialogForGacha* infodialog;
        
        infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                       CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_NoMoney);
        
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }
    
    

    
    
    
}

void GachaMain::beginSpeedOT()
{
    
    
    if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->gacha_cost_coin) {
        innerExit();
        
        removeGachaDlg();
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        
//        if (SaveData::Inst()->hasValidSavedData()
//            && SaveData::Inst()->getReqGachaTime() != 0) {
//            ml_requestTime = SaveData::Inst()->getReqGachaTime();
//        }
//        else {
//            ml_requestTime = CGameData::Inst()->getCurrentTime();
//            SaveData::Inst()->setReqCachaTime(ml_requestTime);
//            SaveData::Inst()->flush();
//        }
         ml_requestTime = CGameData::Inst()->getCurrentTime();
        if(CGameData::Inst()->chargeGachaRequest_Other(MainScene::Inst()->m_other_Gacha , "",ml_requestTime, false)){
            scheduleUpdate();
            m_iReqType = enGachaReqType_OtherGachaSpeed;
        }
    }
    else
    {
        
        
        
        removeGachaDlg();
        
        DialogForGacha* infodialog;
        
        infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                       CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_NoMoney);
        
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }
    

}

void GachaMain::unsGachaClistCb(CCObject *sender)
{
    CListLayerRet* pRet = (CListLayerRet*)sender;
    
    
    
    if(pRet->iBtnSel==0){
        if(CGameData::Inst()->isNewbie()){
            CCNode* pArrow = getChildByTag(kNewbieArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
            
            pArrow = new Arrow(30, 255);
            pArrow->setPosition(CCPointMake(370, 300));
            addChild(pArrow, 600, kNewbieArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
        }
        
        
        if(canGet)
        GachaMain::beginUnusualGachaGet(true);
        else
        {
            DialogForGacha* infodialog;
            
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSpeed),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_QianJiang);
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();
        }
        
        /*
        
        DialogForGacha* infodialog;
        
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->gacha_cost_coin) {
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginUnusualGachaGet),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare1);
        }
        else
        {
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare1);
        }
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
         
         */
    }else if (pRet->iBtnSel == 1) {
        if(CGameData::Inst()->isNewbie())
            return;

        if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
        {

            if(canGet)
                GachaMain::beginSuiTangGachaGet(true);
            else
            {
                DialogForGacha* infodialog;
                
                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSpeedST),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_QianJiangST);
                addChild(infodialog, 500,kDlg1ForGacha);
                infodialog->setPosition(CCPointMake(320, 480));
                infodialog->setScale(0.0);
                infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                infodialog->release();
            }
            
//            DialogForGacha* infodialog;
//            int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
//            if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
//                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSuiTangGachaGet),
//                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_SuiTang);
//            }
//            else
//            {
//                infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
//                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_SuiTang);
//            }
//            addChild(infodialog, 500,kDlg1ForGacha);
//            infodialog->setPosition(CCPointMake(320, 480));
//            infodialog->setScale(0.0);
//            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
//            infodialog->release();
        }
        else
        {

            
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha)
                MainScene::Inst()->m_other_Gacha = 1;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            
            
            
            if(canGet)
                GachaMain::beginOtherGachaGet(true);
            else
            {
                DialogForGacha* infodialog;
                
                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSpeedOT),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_QianJiangOT);
                addChild(infodialog, 500,kDlg1ForGacha);
                infodialog->setPosition(CCPointMake(320, 480));
                infodialog->setScale(0.0);
                infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                infodialog->release();

            }
            
//            DialogForGacha* infodialog;
//            int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
//            if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
//                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginOtherGachaGet),
//                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
//            }
//            else
//            {
//                infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
//                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
//            }
//            addChild(infodialog, 500,kDlg1ForGacha);
//            infodialog->setPosition(CCPointMake(320, 480));
//            infodialog->setScale(0.0);
//            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
//            infodialog->release();
            
            
            
            
            
        }

    }
    else if (pRet->iBtnSel == 2)
    {
        if(CGameData::Inst()->isNewbie())
            return;
        
        if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
        {
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha)
                MainScene::Inst()->m_other_Gacha = 1;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
        }
        else
        {
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            
        }
        
        
        if(canGet)
            GachaMain::beginOtherGachaGet(true);
        else
        {
            DialogForGacha* infodialog;
            
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSpeedOT),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_QianJiangOT);
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();
        }

        
//        DialogForGacha* infodialog;
//        int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
//        if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
//            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginOtherGachaGet),
//                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
//        }
//        else
//        {
//            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
//                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
//        }
//        addChild(infodialog, 500,kDlg1ForGacha);
//        infodialog->setPosition(CCPointMake(320, 480));
//        infodialog->setScale(0.0);
//        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
//        infodialog->release();
        
    }
    
    /*
    if(pRet->iBtnSel==0){
        if(CGameData::Inst()->isNewbie()){
            CCNode* pArrow = getChildByTag(kNewbieArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
            
            pArrow = new Arrow(30, 255);
            pArrow->setPosition(CCPointMake(370, 300));
            addChild(pArrow, 600, kNewbieArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
        }
        
        DialogForGacha* infodialog;

        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->gacha_cost_coin) {
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginUnusualGachaGet),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare1);
        }
        else
        {
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare1);
        }
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }else if (pRet->iBtnSel == 1) {
        if(CGameData::Inst()->isNewbie())
            return;
        
        DialogForGacha* infodialog;
        int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt;
        if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginUnusualGachaGet10),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare10);
        }
        else
        {
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Rare10);
        }
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }
    else if (pRet->iBtnSel == 2)
    {
        if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
        {
            if(CGameData::Inst()->isNewbie())
                return;
            
            DialogForGacha* infodialog;
            int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
            if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginSuiTangGachaGet),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_SuiTang);
            }
            else
            {
                infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_SuiTang);
            }
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();
        }
        else
        {
            if(CGameData::Inst()->isNewbie())
                return;
            
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha)
                MainScene::Inst()->m_other_Gacha = 1;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            
            DialogForGacha* infodialog;
            int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
            if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
                infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginOtherGachaGet),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
            }
            else
            {
                infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                               CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
            }
            addChild(infodialog, 500,kDlg1ForGacha);
            infodialog->setPosition(CCPointMake(320, 480));
            infodialog->setScale(0.0);
            infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            infodialog->release();
            
            


            
        }

    }
    else if (pRet->iBtnSel == 3)
    {
        
        if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
        {
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha)
                MainScene::Inst()->m_other_Gacha = 1;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
        }
        else
        {
            if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha)
                MainScene::Inst()->m_other_Gacha = 2;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            else if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha && CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha)
                MainScene::Inst()->m_other_Gacha = 3;
            
        }
        
        if(CGameData::Inst()->isNewbie())
            return;
        
        DialogForGacha* infodialog;
        int iNeedCoin = CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
        if (CGameData::Inst()->getUsrInfo()->coin >= iNeedCoin) {
            infodialog= new DialogForGacha(this,callfuncO_selector(GachaMain::beginOtherGachaGet),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
        }
        else
        {
            infodialog= new DialogForGacha(MainScene::Inst(),callfuncO_selector(MainScene::goToShop),
                                           CGameData::Inst()->getUsrInfo()->coin, Dlg_GachaType_Other);
        }
        addChild(infodialog, 500,kDlg1ForGacha);
        infodialog->setPosition(CCPointMake(320, 480));
        infodialog->setScale(0.0);
        infodialog->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        infodialog->release();
    }

    */
    
    
    
    if(canGet)
    unsGachaClist->clearAll();
}
void GachaMain::gachaGetCallback(CCObject *sender)
{
    GachaGetRet *ret = (GachaGetRet *)sender;

    switch (ret->gachaType) {
        case FriendGachaType:
        case UnusualGachaType:
            if(ret->isGoCompGacha)
            {
                backEnter();
                hideGachaInfo();
                MainScene::Inst()->showBottomPanel(true);
                reqGachaConfig();
            }else
                backEnter();
            break;
            
        case CompGachaType:
            backEnter();
            hideGachaInfo();
            MainScene::Inst()->showBottomPanel(true);
            reqGachaConfig();
            break;
            
        case FriendGacha10Type:
            if (pGachaLayer) {
                pGachaLayer->removeFromParentAndCleanup(true);
                pGachaLayer = NULL;
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            MainScene::Inst()->showUsrInfPanel(true);
            
            m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("gachaTip5"));
            m_spTitle->setPosition(CCPointMake(-210, 766));
            m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
            
            m_miBack->setPosition(CCPointMake(-529, 731));
            m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));

            m_FrdGacha10Show->show();
            break;
            
        case UnusualGacha10Type:
            if (pGachaLayer) {
                pGachaLayer->removeFromParentAndCleanup(true);
                pGachaLayer = NULL;
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            MainScene::Inst()->showUsrInfPanel(true);
            
            m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("gachaTip6"));
            m_spTitle->setPosition(CCPointMake(-210, 766));
            m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
            
            m_miBack->setPosition(CCPointMake(-529, 731));
            m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));

            m_RareGacha10Show->show();
            break;
            
        default:
            break;
    }
}
void GachaMain::backEnter()
{
    if (pGachaLayer) {
        pGachaLayer->removeFromParentAndCleanup(true);
        pGachaLayer = NULL;
    }
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    showInf();
}

void GachaMain::enterGacha()
{
    showTips();
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_gacha"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    m_btnMenu->setPosition(CCPointMake(640, 0));
    m_btnMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)),0.8f));
 
#ifdef GACHAWEB
    m_pWebBg->setPosition(CCPointMake(-320, 320-60));
    m_pWebBg->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 320-60)),0.8f));
    
    gachaWeb->setPosition(CCPointMake(640, 0));
    gachaWeb->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    gachaWeb->enter();
#endif
    
//    char buf[100];
//    snprintf(buf, 99, "naviText_gacha_%d", int(CCRANDOM_0_1() * 2) + 1);

    
    MainScene::Inst()->showBottomPromptInfo("naviText_gacha_");
   // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}



//******************************************************************************
// exit
//******************************************************************************
void GachaMain::exit()
{
    innerExit();
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


void GachaMain::exitFriendOrUnusualGacha()
{
    innerExit();
    enterGacha();
}


void GachaMain::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(GachaMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        MainScene::Inst()->showTips();
        switch (m_iReqType) {
            case enGachaReqType_FreeGacha:{
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                if(CGameData::Inst()->getGachaRet()->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;

                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_FreeGachaOvr;
                }
            }
                break;
                
            case enGachaReqType_RareGacha:{
                
             
                    strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                    if(CGameData::Inst()->getGachaRet()->plus>0)
                        m_isGachaPlus = true;
                    else
                        m_isGachaPlus = false;
                    
                    if(CGameData::Inst()->allCardRequest()){
                        scheduleUpdate();
                        m_iReqType = enGachaReqType_RareGachaOvr;
                    }
                
               
               
                
       
                
#ifdef TalkData
                TDCCItem::onPurchase("求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
                
            }
                break;
                
            case enGachaReqType_RareGachaSpeed:{
                
                
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                if(CGameData::Inst()->getGachaRet()->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_RareGachaSpeedOvr;
                }
                
                
                
                
                
                
#ifdef TalkData
                TDCCItem::onPurchase("求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
                
            }
                break;
                
            case enGachaReqType_SuiTangGacha:{
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                if(CGameData::Inst()->getGachaRet()->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_SuiTangGachaOvr;
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("隋唐求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
            }
                break;
                
            case enGachaReqType_SuiTangGachaSpeed:{
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                if(CGameData::Inst()->getGachaRet()->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_SuiTangGachaSpeedOvr;
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("隋唐求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
            }
                break;
                
                
            case enGachaReqType_OtherGachaSpeed:
            {
                CCMutableArray<CGachaRet *>* pGachaArr = CGameData::Inst()->getMultiGachaRet();
                CGachaRet* pGachaRet = (CGachaRet*)pGachaArr->getObjectAtIndex(0);
                strcpy(ucidbuf, pGachaRet->ucid);
                if(pGachaRet->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    if (pGachaArr->count() > 1) {
                        m_iReqType = enGachaReqType_MutliOtherGachaSpeedOvr;
                    } else {
                        m_iReqType = enGachaReqType_OtherGachaSpeedOvr;
                    }
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("其他求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
                
                
            }
                break;
                
            case enGachaReqType_OtherGacha:
            {
                CCMutableArray<CGachaRet *>* pGachaArr = CGameData::Inst()->getMultiGachaRet();
                CGachaRet* pGachaRet = (CGachaRet*)pGachaArr->getObjectAtIndex(0);
                strcpy(ucidbuf, pGachaRet->ucid);
                if(pGachaRet->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                

                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    if (pGachaArr->count() > 1) {
                        m_iReqType = enGachaReqType_MutliOtherGachaOvr;
                    } else {
                        m_iReqType = enGachaReqType_OtherGachaOvr;
                    }
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("其他求将", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
#else
                
#endif
                
                

            }
                break;
        
            case enGachaReqType_FreeGachaOvr:
                pGachaLayer = new GachaGet(FriendGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
                addChild(pGachaLayer,200,GachaLayerTag);
                pGachaLayer->release();
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                break;
                
                
            case enGachaReqType_RareGachaSpeedOvr:
                
                
            {
                CCNode * tt = getChildByTag(kDlg1ForGacha);
                
                if(tt)
                    tt->removeFromParentAndCleanup(true);
                
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::beginSpeedEnd),
                                                    CGameData::Inst()->getLanguageValue("dbqj_qianJiangSuccess"),false);
                
                
                addChild(newbieEndDlg, 500,kDlg1ForGacha);
                newbieEndDlg->setPosition(CCPointMake(320, 480));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                newbieEndDlg->release();
                
            }
                
                
                break;
                
            case enGachaReqType_SuiTangGachaSpeedOvr:
            {
                CCNode * tt = getChildByTag(kDlg1ForGacha);
                
                if(tt)
                    tt->removeFromParentAndCleanup(true);
                
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::beginSpeedSTEnd),
                                                    CGameData::Inst()->getLanguageValue("dbqj_qianJiangSuccess"),false);
                
                
                addChild(newbieEndDlg, 500,kDlg1ForGacha);
                newbieEndDlg->setPosition(CCPointMake(320, 480));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                newbieEndDlg->release();
            }
                break;
                
            case enGachaReqType_OtherGachaSpeedOvr:
            {
                CCNode * tt = getChildByTag(kDlg1ForGacha);
                
                if(tt)
                    tt->removeFromParentAndCleanup(true);
                
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::beginSpeedOTEnd),
                                                    CGameData::Inst()->getLanguageValue("dbqj_qianJiangSuccess"),false);
                
                
                addChild(newbieEndDlg, 500,kDlg1ForGacha);
                newbieEndDlg->setPosition(CCPointMake(320, 480));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                newbieEndDlg->release();
            }
                break;
                
            case enGachaReqType_NewbieGacha:
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                m_isShowGoCompGachaBtn = CGameData::Inst()->getGachaRet()->is_compGachaMaterial;
                
                
            
            case enGachaReqType_RareGachaOvr:
            case enGachaReqType_SuiTangGachaOvr:
                

                
                
            case enGachaReqType_OtherGachaOvr:
                pGachaLayer = new GachaGet(UnusualGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
                addChild(pGachaLayer,200,GachaLayerTag);
                pGachaLayer->release();
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                break;
            case enGachaReqType_CompGacha:
                enterCompGacha();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            case enGachaReqType_ExchangeCard:{
                strcpy(ucidbuf, CGameData::Inst()->getExchangeCardInfo()->uCid);

                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_ExchangeCardOvr;
                }
            }
                break;
            case enGachaReqType_ExchangeCardOvr:
                beginExchangeCard();
                break;
                
            case enGachaReqType_MutiFreeGacha:{
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_MutiFreeGachaOvr;
                }
            }
                break;
                
            case enGachaReqType_MutiRareGacha:{
                CGameData::Inst()->getMultiGachaRet();
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_MutiRareGachaOvr;
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("十连抽", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt);
#else
                
#endif
                
                
                
            }
                break;
            case enGachaReqType_MutiRareGachaSpeed:{
                CGameData::Inst()->getMultiGachaRet();
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_MutiRareGachaSpeedOvr;
                }
                
#ifdef TalkData
                TDCCItem::onPurchase("十连抽", 1, CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt);
#else
                
#endif
                
                
                
            }
                break;
                
            case enGachaReqType_MutiFreeGachaOvr:
                pGachaLayer = new GachaGet(FriendGacha10Type,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
                addChild(pGachaLayer,200,GachaLayerTag);
                pGachaLayer->release();
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                break;
                
            case enGachaReqType_MutiRareGachaOvr:
                pGachaLayer = new GachaGet(UnusualGacha10Type,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
                addChild(pGachaLayer,200,GachaLayerTag);
                pGachaLayer->release();
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                break;
                
            case enGachaReqType_MutiRareGachaSpeedOvr:

            {
                CCNode * tt = getChildByTag(kDlg1ForGacha);
                
                if(tt)
                    tt->removeFromParentAndCleanup(true);
                
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::beginSpeedEnd10),
                                                    CGameData::Inst()->getLanguageValue("dbqj_qianJiangSuccess"),false);
                
                
                addChild(newbieEndDlg, 500,kDlg1ForGacha);
                newbieEndDlg->setPosition(CCPointMake(320, 480));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                newbieEndDlg->release();
            }
                break;
            case enGachaReqType_MutliOtherGachaOvr:
                pGachaLayer = new GachaGet(UnusualGacha10Type,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),m_isGachaPlus);
                addChild(pGachaLayer,200,GachaLayerTag);
                pGachaLayer->release();
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                break;
                
                
            case enGachaReqType_MutliOtherGachaSpeedOvr:
            {
                CCNode * tt = getChildByTag(kDlg1ForGacha);
                
                if(tt)
                    tt->removeFromParentAndCleanup(true);
                
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::beginSpeedOTEnd10),
                                                    CGameData::Inst()->getLanguageValue("dbqj_qianJiangSuccess"),false);
                
                
                addChild(newbieEndDlg, 500,kDlg1ForGacha);
                newbieEndDlg->setPosition(CCPointMake(320, 480));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                newbieEndDlg->release();
            }
                break;

                
            default:
                break;
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(GachaMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            
            m_commDlg->setNoCancel();
        }
    }
}

void GachaMain::removeGachaDlg()
{
    CCNode* gachaDlg = getChildByTag(kDlg1ForGacha);
    if(gachaDlg)
    {
        removeChild(gachaDlg, true);
    }
}


void GachaMain::beginExchangeCard()
{
    MainScene::Inst()->showBottomPanel(true);
    pGachaLayer = new GachaGet(CompGachaType,ucidbuf,this,callfuncO_selector(GachaMain::gachaGetCallback),false);
    pGachaLayer->setIsGotUnknowCompGachaHero(m_isGotUnknowCompGachaHero);
    addChild(pGachaLayer,200,GachaLayerTag);
    pGachaLayer->setIsTouchEnabled(false);   // 此处处理 为了防止配置不好设备 接下来点击 导致的崩溃
    pGachaLayer->release();
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
}

void GachaMain::beginFriendGachaGet()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    removeGachaDlg();
    innerExit();
    if(CGameData::Inst()->freeGachaRequest()){
        scheduleUpdate();
        m_iReqType = enGachaReqType_FreeGacha;
    }
    else
    {
        return;
    }

}

void GachaMain::beginFriendGachaGet10()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    removeGachaDlg();
    innerExit();
    if(CGameData::Inst()->multiFreeGachaRequest()){
        scheduleUpdate();
        m_iReqType = enGachaReqType_MutiFreeGacha;
    }
    else
    {
        return;
    }
}

void GachaMain::beginUnusualGachaGet(bool free)
{
    innerExit();
    
    removeGachaDlg();
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->isNewbie()){
        CCNode* pArrow = getChildByTag(kNewbieArrowTag);
        if(pArrow)
            removeChild(pArrow, true);

        if(CGameData::Inst()->newbieRequest(0, 4, NULL)){
            scheduleUpdate();
            m_iReqType = enGachaReqType_NewbieGacha;
        }
    }
    else{
        
//        if (SaveData::Inst()->hasValidSavedData()
//            && SaveData::Inst()->getReqGachaTime() != 0) {
//            ml_requestTime = SaveData::Inst()->getReqGachaTime();
//        }
//        else {
//            ml_requestTime = CGameData::Inst()->getCurrentTime();
//            SaveData::Inst()->setReqCachaTime(ml_requestTime);
//            SaveData::Inst()->flush();
//        }
         ml_requestTime = CGameData::Inst()->getCurrentTime();
        if(CGameData::Inst()->chargeGachaRequest(false,"",ml_requestTime , free)){
            scheduleUpdate();
            m_iReqType = enGachaReqType_RareGacha;
        }
    }
}

void GachaMain::beginSuiTangGachaGet(bool free)
{
    innerExit();
    
    removeGachaDlg();
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
//    if (SaveData::Inst()->hasValidSavedData()
//        && SaveData::Inst()->getReqGachaTime() != 0) {
//        ml_requestTime = SaveData::Inst()->getReqGachaTime();
//    }
//    else {
//        ml_requestTime = CGameData::Inst()->getCurrentTime();
//        SaveData::Inst()->setReqCachaTime(ml_requestTime);
//        SaveData::Inst()->flush();
//    }

     ml_requestTime = CGameData::Inst()->getCurrentTime();
    if(CGameData::Inst()->chargeGachaRequest(true,"",ml_requestTime, free)){
        scheduleUpdate();
        m_iReqType = enGachaReqType_SuiTangGacha;
    }

}

void GachaMain::beginOtherGachaGet(bool free)
{
    innerExit();
    
    removeGachaDlg();
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    
//    if (SaveData::Inst()->hasValidSavedData()
//        && SaveData::Inst()->getReqGachaTime() != 0) {
//        ml_requestTime = SaveData::Inst()->getReqGachaTime();
//    }
//    else {
//        ml_requestTime = CGameData::Inst()->getCurrentTime();
//        SaveData::Inst()->setReqCachaTime(ml_requestTime);
//        SaveData::Inst()->flush();
//    }

     ml_requestTime = CGameData::Inst()->getCurrentTime();
    if(CGameData::Inst()->chargeGachaRequest_Other(MainScene::Inst()->m_other_Gacha , "",ml_requestTime, free)){
        scheduleUpdate();
        m_iReqType = enGachaReqType_OtherGacha;
    }
    
}



void GachaMain::beginUnusualGachaGet10()
{
    innerExit();
    
    removeGachaDlg();
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
//    
//    if (SaveData::Inst()->hasValidSavedData()
//        && SaveData::Inst()->getReqGachaTime() != 0) {
//        ml_requestTime = SaveData::Inst()->getReqGachaTime();
//    }
//    else {
//        ml_requestTime = CGameData::Inst()->getCurrentTime();
//        SaveData::Inst()->setReqCachaTime(ml_requestTime);
//        SaveData::Inst()->flush();
//    }
    
      ml_requestTime = CGameData::Inst()->getCurrentTime();
    if(CGameData::Inst()->multiChargeGachaRequest("",ml_requestTime)){
        scheduleUpdate();
        m_iReqType = enGachaReqType_MutiRareGacha;
    }
}

//******************************************************************************
// onExit
//******************************************************************************
void GachaMain::onExit()
{
    CCLayer::onExit();
}

//******************************************************************************
// innerExit
//******************************************************************************
void GachaMain::innerExit()
{
    
    unschedule(schedule_selector(GachaMain::shuaXin));
    
    removeGachaDlg();
    m_shareBtn->stopAllActions();
    m_spTitle->stopAllActions();
    m_btnMenu->stopAllActions();
    m_miBack->stopAllActions();
    unsGachaClist->clearAll();
    frdGachaClist->clearAll();
#ifdef GACHAWEB
    
    m_pWebBg->stopAllActions();
    gachaWeb->stopAllActions();
    m_pWebBg->setPosition(CCPointMake(-1000, 1000));
    gachaWeb->setPosition(ccp(-1000, -1000));
    gachaWeb->exit();
#endif
    
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    m_btnMenu->setPosition(CCPointMake(-1000, -1000));
    m_shareBtn->setPosition(ccp(-1000, -1000));
    if(compGachaBgTop)
    {   
        compGachaBgTop->removeFromParentAndCleanup(true);
        compGachaBgTop = NULL;
    }
    if(compGachaBgMiddle)
    {
        compGachaBgMiddle->removeFromParentAndCleanup(true);
        compGachaBgMiddle = NULL;
    }
    if(compGachaBgBottom)
    {
        compGachaBgBottom->removeFromParentAndCleanup(true);
        compGachaBgBottom = NULL;
    }
    if(m_compGachaList)
    {
        removeChild(m_compGachaList, true);
        m_compGachaList = NULL;
    }
    
    if (CGameData::Inst()->isNewbie()) {
        MainScene::Inst()->removeTouchTip();
    }
    
    if (m_CardShop)
    {
        removeChild(m_CardShop, true);
        m_CardShop = NULL;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void GachaMain::btnCallback(CCObject* sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->isNewbie())
        return;
    if (m_CardShop != NULL) {
        if (m_CardShop->isExistForcfmLayer()) {
            m_CardShop->removem_cfmLayer();
            return;
        }
    }
    
    unschedule(schedule_selector(GachaMain::shuaXin));
    
    if(m_RareGacha10Show->getIsVisible() == true){
        m_RareGacha10Show->hide();
        MainScene::Inst()->showBottomPanel(true);
    }
    
    if(m_FrdGacha10Show->getIsVisible() == true){
        m_FrdGacha10Show->hide();
        MainScene::Inst()->showBottomPanel(true);
    }
    
    exitFriendOrUnusualGacha();
}

void GachaMain::enterFriendGacha(CCObject* sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    if(CGameData::Inst()->isNewbie())
        return;

    innerExit();
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_freegacha"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    frdGachaClist->clearAll();
    CMenuBtn *tempBtn1 = new CMenuBtn("ma_button.png",
                                      CGameData::Inst()->getLanguageValue("gachaDlgTip17"),
                                      CCTextAlignmentCenter);
    CMenuBtn *tempBtn2 = new CMenuBtn("ma_button.png",
                                      CGameData::Inst()->getLanguageValue("gachaDlgTip14"),
                                      CCTextAlignmentCenter);
    frdGachaClist->push_back(tempBtn1);
    frdGachaClist->push_back(tempBtn2);
    
    frdGachaClist->setPosition(CCPointMake(640, 0));
    frdGachaClist->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    MainScene::Inst()->showBottomPromptInfo("naviText_gacha_liang");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16

    return;

}

void GachaMain::cbCardShop()
{
    
}

void GachaMain::enterCardShop(CCObject* pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    innerExit();
    
    if (m_CardShop == NULL)
        m_CardShop = new CardShop(this, callfunc_selector(GachaMain::cbCardShop));
    addChild(m_CardShop);
    m_CardShop->release();
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("card_shop_title"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
}

void GachaMain::shuaXin()
{
    
    CUserInfo * tt = CGameData::Inst()->getUsrInfo();
    
    long tl = tt->free_cd_time;
    
    
    unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
    
    long lTimeOffset = tl - ulCurTime;
    
    
    
    if(lTimeOffset <= 0)
    {
        canGet = true;
        lTimeOffset = 0;
        
        
        
        if(subNode1)
            subNode1->setIsVisible(false);
        
        if(subNode2)
            subNode2->setIsVisible(false);
        
        if(subNode3)
            subNode3->setIsVisible(false);
        
        if(subNode4)
            subNode4->setIsVisible(false);
        
        if(subNode5)
            subNode5->setIsVisible(false);
        
        
        
        if(sp1)
            sp1->setIsVisible(false);
        
        if(sp2)
            sp2->setIsVisible(false);
        
        if(sp3)
            sp3->setIsVisible(false);
        
        if(sp4)
            sp4->setIsVisible(false);
        
        if(sp5)
            sp5->setIsVisible(false);
        
        
        
        if(node1)
            node1->setString(CGameData::Inst()->getLanguageValue("dbqj_qianJiangFree"));
        
        if(node2)
            node2->setString(CGameData::Inst()->getLanguageValue("dbqj_suiTangFree"));
        
        if(node3)
            node3->setString(CGameData::Inst()->getLanguageValue("dbqj_hero1Free"));
        
        if(node4)
            node4->setString(CGameData::Inst()->getLanguageValue("dbqj_hero2Free"));
        
        if(node5)
            node5->setString(CGameData::Inst()->getLanguageValue("dbqj_hero3Free"));
        
        unschedule(schedule_selector(GachaMain::shuaXin));

    }
    else
    {
        
        
        
        canGet = false;
    }
    
    
    
    int hour = lTimeOffset / 3600;
    int minute = lTimeOffset % 3600 / 60;
    int second = lTimeOffset % 60;
    char tBuf[100];
    
    if(minute < 10 &&  second < 10)
        snprintf(tBuf, 99, "%s%d:0%d:0%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else if(minute < 10)
        snprintf(tBuf, 99, "%s%d:0%d:%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else if(second < 10)
        snprintf(tBuf, 99, "%s%d:%d:0%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else
        snprintf(tBuf, 99, "%s%d:%d:%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    
    
    if(subNode1)
        subNode1->setString(tBuf);
    
    if(subNode2)
        subNode2->setString(tBuf);

    if(subNode3)
        subNode3->setString(tBuf);

    if(subNode4)
        subNode4->setString(tBuf);

    if(subNode5)
        subNode5->setString(tBuf);

    
    
    
}

void GachaMain::enterUnusualGacha(CCObject* sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->isNewbie()){
        CCNode* pArrow = getChildByTag(kNewbieArrowTag);
        if(pArrow)
            removeChild(pArrow, true);

        pArrow = new Arrow(30, 255);
        pArrow->setPosition(CCPointMake(370 + 150, 670));
        addChild(pArrow, 600, kNewbieArrowTag);
        pArrow->release();
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    innerExit();

    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_freegacha2"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));

    unsGachaClist->clearAll();
    
    
    
    subNode1 = NULL;
    subNode2 = NULL;
    subNode3 = NULL;
    subNode4 = NULL;
    subNode5 = NULL;
    
    
    
    node1 = NULL;
    node2 = NULL;
    node3 = NULL;
    node4 = NULL;
    node5 = NULL;
    
    sp1 = NULL;
    sp2 = NULL;
    sp3 = NULL;
    sp4 = NULL;
    sp5 = NULL;
    
    
    CUserInfo * tt = CGameData::Inst()->getUsrInfo();
    
    long tl = tt->free_cd_time;
    
    
    unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
    
    long lTimeOffset = tl - ulCurTime;
    
    
    
    if(lTimeOffset <= 0)
    {
        canGet = true;
        lTimeOffset = 0;
    }
    else
    {
        schedule(schedule_selector(GachaMain::shuaXin ), 1.0f);
        canGet = false;
    }
    
    
    
    int hour = lTimeOffset / 3600;
    int minute = lTimeOffset % 3600 / 60;
    int second = lTimeOffset % 60;
    char tBuf[100];
    
    
    if(minute < 10 &&  second < 10)
        snprintf(tBuf, 99, "%s%d:0%d:0%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else if(minute < 10)
        snprintf(tBuf, 99, "%s%d:0%d:%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else if(second < 10)
        snprintf(tBuf, 99, "%s%d:%d:0%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    else
    snprintf(tBuf, 99, "%s%d:%d:%d",CGameData::Inst()->getLanguageValue("dbqj_shengYu"), hour , minute , second);
    
    
    
    
    if(canGet)
    {
        CMenuBtn *tempBtn1 = new CMenuBtn("ma_button.png",
                                          "",
                                          CCTextAlignmentCenter);
        node1 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_qianJiangFree"),
                                                    CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
        tempBtn1->addChild(node1);
        
        node1->setPosition(CCPointMake(0, 5));
        node1->setColor(ccc3(255, 226, 0));
        node1->setShadowColor(ccBLACK);
        
        
        unsGachaClist->push_back(tempBtn1);
    }
    else
    {
        CMenuBtn *tempBtn1 = new CMenuBtn("ma_button.png",
                                          "",
                                          CCTextAlignmentCenter);
        node1 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_qianJiangCD"),
                                                    CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
        tempBtn1->addChild(node1);
        
        node1->setPosition(CCPointMake(0, 5));
        node1->setColor(ccc3(255, 226, 0));
        node1->setShadowColor(ccBLACK);
        
        
        sp1 = CCSprite::spriteWithFile("newAdd_DaoJiShi.png");
        sp1->setPosition(CCPointMake(140, -40));
        
        tempBtn1->addChild(sp1);
        

        
        
        subNode1 = TextNode::textWithString(tBuf, CCSizeMake(320, 100), CCTextAlignmentLeft, 22);
        subNode1->setPosition(CCPointMake(216  , -40));
        subNode1->setShadowColor(ccBLACK);
        
        
        
        tempBtn1->addChild(subNode1,3);

        
        unsGachaClist->push_back(tempBtn1);
    }
    
    
    
    
    
    

    
    
    
    
    

    
    
    
    
    int num = 0;
    
    
    if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
    {
        
        if(canGet)
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            node2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_suiTangFree"),
                                              CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            tempBtn3->addChild(node2);
            
            node2->setPosition(CCPointMake(0, 5));
            node2->setColor(ccc3(255, 226, 0));
            node2->setShadowColor(ccBLACK);
            
            
            
            unsGachaClist->push_back(tempBtn3);
        }
        else
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            node2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_suiTangCD"),
                                              CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            tempBtn3->addChild(node2);
            
            node2->setPosition(CCPointMake(0, 5));
            node2->setColor(ccc3(255, 226, 0));
            node2->setShadowColor(ccBLACK);
            
            sp2 = CCSprite::spriteWithFile("newAdd_DaoJiShi.png");
            sp2->setPosition(CCPointMake(140, -40));
            
            tempBtn3->addChild(sp2);
            
            
            
            
            
            
            
            
            subNode2 = TextNode::textWithString(tBuf, CCSizeMake(320, 100), CCTextAlignmentLeft, 22);
            subNode2->setPosition(CCPointMake(216  , -40));
            subNode2->setShadowColor(ccBLACK);
            tempBtn3->addChild(subNode2,3);
            
            
            
            
            unsGachaClist->push_back(tempBtn3);
        }
        
        
        
        
        num ++;
        
        
    }
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && num < 2)
    {
        
        if(canGet)
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node3 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero1Free"),
                                              CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node3);
            
            node3->setPosition(CCPointMake(0, 5));
            node3->setColor(ccc3(255, 226, 0));
            node3->setShadowColor(ccBLACK);
            
            unsGachaClist->push_back(tempBtn3);
        }
        else
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node3 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero1CD"),
                                              CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node3);
            
            node3->setPosition(CCPointMake(0, 5));
            node3->setColor(ccc3(255, 226, 0));
            node3->setShadowColor(ccBLACK);
            
            
            sp3 = CCSprite::spriteWithFile("newAdd_DaoJiShi.png");
            sp3->setPosition(CCPointMake(140, -40));
            
            tempBtn3->addChild(sp3);
            
            
            
            
            
            
            
            
            subNode3 = TextNode::textWithString(tBuf, CCSizeMake(320, 100), CCTextAlignmentLeft, 22);
            subNode3->setPosition(CCPointMake(216  , -40));
            subNode3->setShadowColor(ccBLACK);
            tempBtn3->addChild(subNode3,3);

            
            unsGachaClist->push_back(tempBtn3);
        }

        
        num ++;
        
        
    }
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha && num < 2)
    {
        if(canGet)
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node4 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero2Free"),
                                                         CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node4);
            
            node4->setPosition(CCPointMake(0, 5));
            node4->setColor(ccc3(255, 226, 0));
            node4->setShadowColor(ccBLACK);
            
            unsGachaClist->push_back(tempBtn3);
        }
        else
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node4 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero2CD"),
                                                         CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node4);
            
            node4->setPosition(CCPointMake(0, 5));
            node4->setColor(ccc3(255, 226, 0));
            node4->setShadowColor(ccBLACK);
            
            
            sp4 = CCSprite::spriteWithFile("newAdd_DaoJiShi.png");
            sp4->setPosition(CCPointMake(140, -40));
            
            tempBtn3->addChild(sp4);
            
            
            
            
            
            
            
            
            subNode4 = TextNode::textWithString(tBuf, CCSizeMake(320, 100), CCTextAlignmentLeft, 22);
            subNode4->setPosition(CCPointMake(216  , -40));
            subNode4->setShadowColor(ccBLACK);
            tempBtn3->addChild(subNode4,3);
            
            
            unsGachaClist->push_back(tempBtn3);
        }
        num ++;
        
        
    }
    
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha && num < 2)
    {
        if(canGet)
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node5 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero3Free"),
                                                         CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node5);
            
            node5->setPosition(CCPointMake(0, 5));
            node5->setColor(ccc3(255, 226, 0));
            node5->setShadowColor(ccBLACK);
            
            unsGachaClist->push_back(tempBtn3);
        }
        else
        {
            CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                               "",
                                               CCTextAlignmentCenter);
            
            node5 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_hero3CD"),
                                                         CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
            
            tempBtn3->addChild(node5);
            
            node5->setPosition(CCPointMake(0, 5));
            node5->setColor(ccc3(255, 226, 0));
            node5->setShadowColor(ccBLACK);
            
            
            sp5 = CCSprite::spriteWithFile("newAdd_DaoJiShi.png");
            sp5->setPosition(CCPointMake(140, -40));
            
            tempBtn3->addChild(sp5);
            
            
            
            
            
            
            
            
            subNode5 = TextNode::textWithString(tBuf, CCSizeMake(320, 100), CCTextAlignmentLeft, 22);
            subNode5->setPosition(CCPointMake(216  , -40));
            subNode5->setShadowColor(ccBLACK);
            tempBtn3->addChild(subNode5,3);
            
            
            unsGachaClist->push_back(tempBtn3);
        }
        
        num ++;
        
        
    }
    
    /*
    
    CMenuBtn *tempBtn1 = new CMenuBtn("ma_button.png",
                                      "",
                                      CCTextAlignmentCenter);
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("gachaDlgTip18"),
                                                CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
    tempBtn1->addChild(lbText);
    
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setColor(ccc3(255, 226, 0));
    lbText->setShadowColor(ccBLACK);

    
    CMenuBtn *tempBtn2 = new CMenuBtn("ma_button.png",
                                      "",
                                      CCTextAlignmentCenter);
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("gachaDlgTip15"),
                                                CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
    tempBtn2->addChild(lbText);
    
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setColor(ccc3(255, 226, 0));
    lbText->setShadowColor(ccBLACK);

    unsGachaClist->push_back(tempBtn1);
    unsGachaClist->push_back(tempBtn2);
    
    
    int num = 0;
    

    if (CGameData::Inst()->getCommonInfo()->bOpen_suitang_gacha)
    {
        CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                "",
                                CCTextAlignmentCenter);
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("suitang_gacha"),
                                          CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
        tempBtn3->addChild(lbText);
        
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccc3(255, 226, 0));
        lbText->setShadowColor(ccBLACK);
        
        unsGachaClist->push_back(tempBtn3);
        
        
        num ++;
        
        
    }
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_1_gacha && num < 2)
    {
        CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                           "",
                                           CCTextAlignmentCenter);
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("other_gacha1"),
                                              CCSizeMake(520, 100), CCTextAlignmentCenter, 42);

        tempBtn3->addChild(lbText);
        
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccc3(255, 226, 0));
        lbText->setShadowColor(ccBLACK);
        
        unsGachaClist->push_back(tempBtn3);

        num ++;
        
        
    }
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_2_gacha && num < 2)
    {
        CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                           "",
                                           CCTextAlignmentCenter);
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("other_gacha2"),
                                          CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
        
        tempBtn3->addChild(lbText);
        
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccc3(255, 226, 0));
        lbText->setShadowColor(ccBLACK);
        
        unsGachaClist->push_back(tempBtn3);
        
        num ++;
        
        
    }
    
    
    if(CGameData::Inst()->getCommonInfo()->bOpen_hero_3_gacha && num < 2)
    {
        CMenuBtn *tempBtn3  = new CMenuBtn("ma_button.png",
                                           "",
                                           CCTextAlignmentCenter);
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("other_gacha3"),
                                          CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
        
        tempBtn3->addChild(lbText);
        
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccc3(255, 226, 0));
        lbText->setShadowColor(ccBLACK);
        
        unsGachaClist->push_back(tempBtn3);
        
        num ++;
        
        
    }
    
    */

    
    
    unsGachaClist->setPosition(CCPointMake(640, 0));
    unsGachaClist->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    MainScene::Inst()->showBottomPromptInfo("naviText_gacha_shen");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}


void GachaMain::reqGachaConfig()
{  
    if(!CGameData::Inst()->isNewbie())
    {
        innerExit();
        if(CGameData::Inst()->compGachaConfigRequest())
        {
            m_iReqType = enGachaReqType_CompGacha;
            scheduleUpdate();
        }
    }
    
}
void GachaMain::enterCompGacha()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    if(CGameData::Inst()->isNewbie())
        return;
    innerExit();
    MainScene::Inst()->showBottomPanel(true);
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_swap_title"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    
    m_shareBtn->setPosition(ccp(1000, 740));
    m_shareBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
    
    
    
    if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
        m_shareBtn ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
        m_shareBtn->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else {
        m_shareBtn->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
    }
    
    
    int tempPosY=0;
    if(NULL == compGachaBgTop)
    {
        compGachaBgTop = CCSprite::spriteWithFile("cg_bg_01.png");
        addChild(compGachaBgTop,50);
    }
    if(NULL == compGachaBgMiddle)
    {
        compGachaBgMiddle = CCSprite::spriteWithFile("cg_bg_02.png");
        addChild(compGachaBgMiddle,40);
    }
    if(NULL == compGachaBgBottom)
    {
        compGachaBgBottom = CCSprite::spriteWithFile("cg_bg_03.png");
        addChild(compGachaBgBottom,50);
        
        CCSprite *tempSp1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCSprite *tempSp2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        
        CCMenuItem *openHelpItem = CCMenuItemImage::itemFromNormalSprite(tempSp1, tempSp2, this, menu_selector(GachaMain::showOrHideCompGachaHelp));
        CCMenu *openHelpMenu = CCMenu::menuWithItem(openHelpItem);

        openHelpItem->setAnchorPoint(CCPointZero);
        openHelpItem->setPosition(CCPointMake(50, 12));
        
        compGachaBgBottom->addChild(openHelpMenu);
        openHelpMenu->setPosition(CCPointZero);
        openHelpItem->setScale(0.85);
        // openHelpMenu->setPosition(CCPoint(182, -60));
        // openHelpMenu->setScale(0.85);
        
        TextNode *helpT = TextNode::textWithString(CGameData::Inst()->getLanguageValue("compGachaTip8"), 36);
        openHelpItem->addChild(helpT);
        helpT->setPosition(CCPoint(100, 40));
        helpT->setColor(ccBLACK);
        
        TextNode *helpTbK = TextNode::textWithString(CGameData::Inst()->getLanguageValue("compGachaTip8"), 36);
        openHelpItem->addChild(helpTbK,10);
        helpTbK->setPosition(CCPoint(100-2, 40+2));
    }
    compGachaBgTop->setPosition(CCPointMake(2000, 700));
    compGachaBgTop->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 700)), 0.8f));
    tempPosY = compGachaBgTop->getPosition().y-compGachaBgTop->getContentSize().height/2-compGachaBgMiddle->getContentSize().height/2+3;
    
    compGachaBgMiddle->setPosition(CCPointMake(2000, tempPosY));
    compGachaBgMiddle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, tempPosY)), 0.8f));
    tempPosY -=compGachaBgMiddle->getContentSize().height/2+compGachaBgBottom->getContentSize().height/2;
    compGachaBgBottom->setPosition(CCPointMake(2000, tempPosY));
    compGachaBgBottom->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, tempPosY)), 0.8f));
    
    
    
    if(CGameData::Inst()->getIsFirstShowCompGachaHelp())
    {
        m_compGachaWeb = new CompGachaWeb();
        addChild(m_compGachaWeb,101);
        m_compGachaWeb->release();
        m_compGachaWeb->setPosition(CCPoint(320, 450));
        m_compGachaWeb->enter();
        CGameData::Inst()->setIsFirstShowCompGachaHelp(false);
        MainScene::Inst()->enableBottomPanel(false);
    }
    

    if(m_compGachaList == NULL)
    {
        m_compGachaList = new CompGachaList(this,callfuncO_selector(GachaMain::showCardInfo));
        addChild(m_compGachaList,45);
        m_compGachaList->setPosition(ccp(2000, 480));
        m_compGachaList->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 480)), 0.8f));
        CCMutableArray<CompGachaInfo *> *compGachaInfoArr = CGameData::Inst()->getCompGachaInfo();
        
        for(int i=0;i<compGachaInfoArr->count();i++)
        {
            CompGachaInfo *tempInfo = compGachaInfoArr->getObjectAtIndex(i);
            CompGachaBtn *btn = new CompGachaBtn(tempInfo,this,callfuncO_selector(GachaMain::compGachaBtnCallback));
//            btn->setCardLines(1);
            m_compGachaList->pushBack(btn);
            btn->release();
        }
    }
    
   
}


void GachaMain::showOrHideCompGachaHelp()
{
    m_compGachaWeb = new CompGachaWeb();
    addChild(m_compGachaWeb,101);
    m_compGachaWeb->release();
    m_compGachaWeb->setPosition(CCPoint(320, 450));
    m_compGachaWeb->enter();
    MainScene::Inst()->enableBottomPanel(false);

}
void GachaMain::showCardInfo(CCObject *sender)
{
    CompGachaListRet *ret = (CompGachaListRet *)sender;
    if(ret->isShow)
    {
        CCardBaseInfo *cardInfo = CGameData::Inst()->getCardBaseInfByCid(ret->cid);
        m_showCard->setOnExitCB(this, callfunc_selector(GachaMain::compGachaShowCardCb));
        m_showCard->show(cardInfo);
    }
}
void GachaMain::compGachaShowCardCb()
{
    
}
void GachaMain::compGachaBtnCallback(CCObject *sender)
{
    CompGachaBtnRet *ret = (CompGachaBtnRet *)sender;
    m_isGotUnknowCompGachaHero = ret->isUnKnowHero;
    Dialog2 *dlg=  new Dialog2(this, callfuncO_selector(GachaMain::compGachaExchange), CGameData::Inst()->getLanguageValue("compGachaTip6"), CGameData::Inst()->getLanguageValue("compGachaTip7"), enDl2TwoBtn);
    dlg->setPosition(ccp(320, 480));
    addChild(dlg,2000);
    dlg->setTag(1000);
}

void GachaMain::compGachaExchange(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    if(ret->bOk)
    {
        innerExit();
        if(CGameData::Inst()->exchangeCardRequest())
        {
            scheduleUpdate();
            m_iReqType = enGachaReqType_ExchangeCard;
        }
    }else {
        
    }
    Dialog2 *dlg = (Dialog2 *)getChildByTag(1000);
    removeChild(dlg, true);
    dlg = NULL;
}
void  GachaMain::newBtn(CCMenuItem *&m_btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition)
{
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName(pic);
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName(pic);
    
    if (!spMenu1)
        spMenu1 = CCSprite::spriteWithFile(pic);
    
    if (!spMenu2)
        spMenu2 = CCSprite::spriteWithFile(pic);
    
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
        TextNode* lbText = TextNode::textWithString(label, CCSizeMake(520, 100), CCTextAlignmentCenter, 38);
        lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                        spMenu1->getContentSize().height * 0.5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        m_btn->addChild(lbText, 2);
    }
}


void  GachaMain::showInf()
{
    
    if (CGameData::Inst()->chkLogin()) {
        MainScene::Inst()->showTips();
        m_showCard->setOnExitCB(this,callfunc_selector(GachaMain::enterGacha));
    }
    else
    {
        newbieGachaInfDlg();
        m_showCard->setOnExitCB(this,callfunc_selector(GachaMain::newbieGachaEnd));
    }
    if(CGameData::Inst()->getUsrCardInfByUcid(ucidbuf))
        m_showCard->show(CGameData::Inst()->getUsrCardInfByUcid(ucidbuf));
}

void GachaMain::newbieGachaInfDlg()
{
    const char* dsp = CGameData::Inst()->getLanguageValue("settingTip3");
    
    Dialog1* newbieInfDlg = new Dialog1(this, callfuncO_selector(GachaMain::cbNewbieInfDlg), dsp,false);
    addChild(newbieInfDlg, 500, kNewbieInfTag);
    newbieInfDlg->release();
    newbieInfDlg->setPosition(CCPointMake(320, 480));
    newbieInfDlg->setScale(0.0);
    newbieInfDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}


void GachaMain::newbieGachaEnd()
{
    //新手引导4步改5步，为了兼容暂时加此条件,
    //服务器改5步之后，可去掉else部分
    if(CGameData::Inst()->isNewbie()){
        MainScene::Inst()->goToGeneral();
    }
    else {
        const char* dsp = CGameData::Inst()->getLanguageValue("gachaTip4");
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(GachaMain::newbieAllEnd), dsp,false);
        addChild(newbieEndDlg, 500, kNewbieEndTag);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 480));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    }
}

void GachaMain::cbNewbieInfDlg(CCObject* )
{
    CCNode* newbieEndDlg = getChildByTag(kNewbieInfTag);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    MainScene::Inst()->showTouchTip();
}

void GachaMain::newbieAllEnd(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbieEndTag);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    MainScene::Inst()->reLogin();
    
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void GachaMain::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
   
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        switch (m_iReqType) {
            case enGachaReqType_FreeGacha:{
                

                if(CGameData::Inst()->freeGachaRequest()){
                    scheduleUpdate();
                }
            }
                break;
                
            case enGachaReqType_RareGacha:{
                
                if(CGameData::Inst()->chargeGachaRequest(false,"",ml_requestTime)){
                    scheduleUpdate();
                }
            }
                
            case enGachaReqType_RareGachaSpeed:{
                
                if(CGameData::Inst()->chargeGachaRequest(false,"",ml_requestTime , false)){
                    scheduleUpdate();
                }
            }
                break;
                
            case enGachaReqType_SuiTangGacha:{
                
                if (CGameData::Inst()->chargeGachaRequest(true,"",ml_requestTime))
                {
                    scheduleUpdate();
                }
            }
                break;
            case enGachaReqType_SuiTangGachaSpeed:{
                
                if (CGameData::Inst()->chargeGachaRequest(true,"",ml_requestTime , false))
                {
                    scheduleUpdate();
                }
            }
                break;
                
            case enGachaReqType_OtherGacha:
                if(CGameData::Inst()->chargeGachaRequest_Other(MainScene::Inst()->m_other_Gacha , "",ml_requestTime))
                {
                    scheduleUpdate();
                }
                break;
            case enGachaReqType_OtherGachaSpeed:
                if(CGameData::Inst()->chargeGachaRequest_Other(MainScene::Inst()->m_other_Gacha , "",ml_requestTime ,false))
                {
                    scheduleUpdate();
                }
                break;
                
            case enGachaReqType_FreeGachaOvr:
            case enGachaReqType_RareGachaOvr:
            case enGachaReqType_RareGachaSpeedOvr:
            case enGachaReqType_MutiFreeGachaOvr:
            case enGachaReqType_MutiRareGachaOvr:
            case enGachaReqType_MutiRareGachaSpeedOvr:
            case enGachaReqType_SuiTangGachaOvr:
            case enGachaReqType_SuiTangGachaSpeedOvr:
            case enGachaReqType_OtherGachaSpeedOvr:
            case enGachaReqType_OtherGachaOvr:{

                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                }
            }
                break;
            
            case enGachaReqType_NewbieGacha:
                if(CGameData::Inst()->newbieRequest(0, 4, NULL)){
                    scheduleUpdate();
                }
                break;
            case enGachaReqType_CompGacha:
                reqGachaConfig();
                break;
            case enGachaReqType_ExchangeCard:
                if(CGameData::Inst()->exchangeCardRequest())
                {
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_ExchangeCard;
                }
                break;
            case enGachaReqType_ExchangeCardOvr:
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enGachaReqType_ExchangeCardOvr;
                }
                break;
                
            case enGachaReqType_MutiFreeGacha:
                if(CGameData::Inst()->multiFreeGachaRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enGachaReqType_MutiRareGacha:
                if(CGameData::Inst()->multiChargeGachaRequest()){
                    scheduleUpdate();
                }
                break;
            case enGachaReqType_MutiRareGachaSpeed:
                if(CGameData::Inst()->multiChargeGachaRequest()){
                    scheduleUpdate();
                }
                break;
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        
        if (CGameData::Inst()->isNewbie()) {
            innerExit();
            enter();
        }
        else
        {
            MainScene::Inst()->showBottomPanel(true);
            MainScene::Inst()->showUsrInfPanel(true);
            innerExit();
            enterGacha();
        }
    }
}

void GachaMain::showTips()
{
    int number = CGameData::Inst()->getUsrInfo()->gacha_pt/CGameData::Inst()->getCommonInfo()->gacha_cost_pt;
    //
    int chargeNumber = CGameData::Inst()->getUsrInfo()->coin/CGameData::Inst()->getCommonInfo()->gacha_cost_coin;
    
    //友情点数gacha
    if(frNumberTips!=NULL)
    {
        frNumberTips->removeFromParentAndCleanup(true);
        frNumberTips= NULL;
    }
    
    if (number > 0)
    {
        frNumberTips = OcProxy::Inst()->showTipsAndNum(number);
        frNumberTips->setPosition(ccp(470, 66));
        m_btn1->addChild(frNumberTips);
    }
    
    //元宝gacha
    if (CGameData::Inst()->getCommonInfo()->bOpen_cards_shop == false)
    {
        if(unsNumberTips!=NULL)
        {
            unsNumberTips->removeFromParentAndCleanup(true);
            unsNumberTips= NULL;
        }
        
        if  (chargeNumber > 0)
        {
            unsNumberTips = OcProxy::Inst()->showTipsAndNum(chargeNumber);
            unsNumberTips->setPosition(ccp(470, 66));
            m_btn2->addChild(unsNumberTips);
        }
    }
}
 
void GachaMain::hideGachaInfo()
{
    m_showCard->hide();
}


void GachaMain::shareToWeibo()
{
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(GachaMain::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("weibo_authened"),
                                      CGameData::Inst()->getLanguageValue("weibo_authened_info"),
                                      enDl2TwoBtn);
        addChild(dialog,100);
        dialog->setTag(111);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else 
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCRenderTexture *target = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
        target->begin();
        MainScene::Inst()->visit();
        target->end();
        target->saveBuffer(kCCImageFormatPNG,"captureImage.png");
        MainScene::Inst()->showWeiboPublishInfoWithType(5);
    }
}

void GachaMain::dl2CallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(111);
    if(ret->bOk == true)
    {
        MainScene::Inst()-> gotoOtherMain();
    }else {
        dialog->removeFromParentAndCleanup(true);
    }
}


//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void GachaMain::shareSanGuo(){

    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(GachaMain::shareToWeibo));
}

