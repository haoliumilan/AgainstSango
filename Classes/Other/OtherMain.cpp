//
//  OtherMain.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TextNode.h"
#include "OtherMain.h"
#include "CGamedata.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CScrollBar.h"
#include "SettingLayer.h"
#include "Package.h"
#include "OtherCardLayer.h"
#include "CShowCardInf.h"
#include "MainScene.h"
#include "RaidersLayer.h"
#include "CommDlg.h"
#include "TitleBar.h"
#include "SimpleAudioEngine.h"
#include "Dialog1.h"
#include "PublishWeiboLayer.h"
#include "Dialog2.h"
#include "OcProxy.h"
#include "CDKeyExchange.h"
#include "MyVideoLayer.h"
#include "RecordData.h"
#include "ScrollViewLayer.h"
#include "RemindLayer.h"


#define kBindResultDlgTag  0x919
using namespace CocosDenshion;

enum enOtherReqType{
    enOtherReqType_Null = 0,
    enOtherReqType_QQLogin,
    enOtherReqType_SinaLogin,
    enOtherReqType_Collection,
    enOtherReqType_ExchangeCDKey,
    enOtherReqType_AllCards,
};

//******************************************************************************
// Construct
//******************************************************************************
OtherMain::OtherMain()
{
    cardLayerY=0;
    m_confirmSwitchUser = NULL;
}

void OtherMain::initMainList()
{
    mainList->clearAll();
    CMenuBtn *btn;
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_tujian"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("record_other_main_title"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_set"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_remind"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_guide"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    m_helpBtn = btn;
    btn->release();
    
    //视频攻略
    if (CGameData::Inst()->getCommonInfo()->m_bIsShowVideoStrategy)
    {
        btn = new CMenuBtn("ma_button.png",
                           CGameData::Inst()->getLanguageValue("title_video_strategy"),
                           CCTextAlignmentCenter);
        mainList->push_back(btn);
        btn->release();
    }
#if defined(kPltfmPp)
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_pp_center"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
#else
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_forum"),
                       CCTextAlignmentCenter);
    mainList->push_back(btn);
    btn->release();
#endif
    if (CGameData::Inst()->getCommonInfo()->bShowCDKey)
    {
        btn = new CMenuBtn("ma_button.png",
                           CGameData::Inst()->getLanguageValue("title_cdKey"),
                           CCTextAlignmentCenter);
        mainList->push_back(btn);
        btn->release();
    }

    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        if (CGameData::Inst()->SinaAccountYes()&&m_sinaBind==NULL) {
            m_sinaBind= new CMenuBtn("ma_button.png",
                                     CGameData::Inst()->getLanguageValue("title_sinawb"),
                                     CCTextAlignmentCenter);
            mainList->push_back(m_sinaBind);
            m_sinaBind->release();
        }
        if (CGameData::Inst()->QQAccountYes()&&m_qqBind==NULL) {
            m_qqBind= new CMenuBtn("ma_button.png",
                                   CGameData::Inst()->getLanguageValue("title_qqwb"),
                                   CCTextAlignmentCenter);
            mainList->push_back(m_qqBind);
            m_qqBind->release();
        }
    }
    else {
        btn =  new CMenuBtn("ma_button.png",
                            CGameData::Inst()->getLanguageValue("title_qiehuan"),
                            CCTextAlignmentCenter);
        mainList->push_back(btn);
        btn->release();
    }
    
}

void OtherMain::shareToWeibo()
{
    
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(OtherMain::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("weibo_authened"),
                                      CGameData::Inst()->getLanguageValue("weibo_authened_info"),
                                      enDl2TwoBtn);
        addChild(dialog);
        dialog->setTag(111);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else {
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCRenderTexture *target = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
        target->begin();
        MainScene::Inst()->visit();
        target->end();
        target->saveBuffer(kCCImageFormatPNG,"captureImage.png");
        MainScene::Inst()->showWeiboPublishInfoWithType(enWeiboTypeTujian);
    }
}
void OtherMain::cbDL2SwitchUser(CCObject* OKorCancel)
{
    Dialog2Ret *ret = (Dialog2Ret *)OKorCancel ;
    if (ret -> bOk == true)
    {
        MainScene::Inst()->switchAccount();
    }
    else{
        innerEnter();
        m_confirmSwitchUser -> removeFromParentAndCleanup(true);
        m_confirmSwitchUser = NULL;
    }
}

void OtherMain::dl2CallBack(CCObject *sender)
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

void OtherMain::cbList(CCObject *object)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    CListLayerRet* pRet = (CListLayerRet*)object;
    bool bIsShowVideoStrategy = CGameData::Inst()->getCommonInfo()->m_bIsShowVideoStrategy;
    if (0==pRet->iBtnSel)
    {
        innerExit();
        m_curScene = ots_card;
        if(CGameData::Inst()->needToReqCollection()){
            CGameData::Inst()->collectionRequest();
            scheduleUpdate();
            m_iReqType = enOtherReqType_Collection;
        }
        else {
            enterCollection();
        }
    }
    else if (1==pRet->iBtnSel)
    {
        enterUploadView();
    }
    else if (2==pRet->iBtnSel)
    {
        innerExit();
        m_curScene = ots_seeting;
        m_settingLayer->setPosition(CCPointMake(640, 0));
        m_settingLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
        m_settingLayer->refreshSetting();
        
        MainScene::Inst()->showBottomPromptInfo("naviText_setting");
     //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    else if (3==pRet->iBtnSel)
    {
        innerExit();
        m_curScene = ots_remind;
        
        if(m_remindLayer == NULL)
        {
            m_remindLayer = new RemindLayer(this,callfuncO_selector(OtherMain::cbRemindLayer));
            addChild(m_remindLayer);
            m_remindLayer->release();
        }
        
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("naviText_remind"));
        m_spTitle->setPosition(CCPointMake(-210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        
        m_backItem->setPosition(CCPointMake(-529, 731));
        m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
        
        m_remindLayer->setPosition(CCPointMake(320, 0));
        m_remindLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
        m_remindLayer->refreshRemind();
        MainScene::Inst()->showBottomPromptInfo(CGameData::Inst()->getLanguageValue("naviText_remind"));
    }
    else if (4==pRet->iBtnSel)
    {
        innerExit();
        m_curScene = ots_raider;
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("text_guide_png"));
        m_spTitle->setPosition(CCPointMake(-210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        
        m_backItem->setPosition(CCPointMake(-529, 731));
        m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
        
       
        MainScene::Inst()->showBottomPromptInfo("naviText_raider");
    //     MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        
        
        textLayer->setPosition(CCPointMake(640, 0));
        textLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
        textLayer->enterWithType(enUrlTypeHelp);
        CGameData::Inst()->setIsEnterHelp();
        MainScene::Inst()->showTips();
    }
    else if (5==pRet->iBtnSel && bIsShowVideoStrategy)   //使用视频攻略
    {
        enterVideoStrategy();
    }
    else if ((6==pRet->iBtnSel && bIsShowVideoStrategy) || (5==pRet->iBtnSel && !bIsShowVideoStrategy))
    {
#if defined(kPltfmPp)
        OcProxy::Inst()->showPPCenter();
#else
        OcProxy::Inst()->linkToSafriWithUrlStr(CGameData::Inst()->getCommonInfo()->bbsurl);
#endif
    }
    else if ((7==pRet->iBtnSel && bIsShowVideoStrategy) || (6==pRet->iBtnSel && !bIsShowVideoStrategy))
    {
        if (CGameData::Inst()->getCommonInfo()->bShowCDKey)
            showCDKeyDialog();
        else
        {
            if (CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin
                || CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin) {
                char buf[100];
                sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip18"));
                m_confirmSwitchUser = new Dialog2(this, callfuncO_selector(OtherMain::cbDL2SwitchUser), CGameData::Inst()->getLanguageValue("alert"), CGameData::Inst()->getLanguageValue("otherMainTip9"), enDl2TwoBtn);
                // m_confirmSwitchUser = new Dialog3(this, callfuncN_selector(OtherMain::cbDL2SwitchUser), );
                m_confirmSwitchUser->setPosition(CCPointMake(320, 480));
                addChild(m_confirmSwitchUser);
                m_confirmSwitchUser->release();
                m_confirmSwitchUser->setScale(0.0f);
                m_confirmSwitchUser->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
                innerExit();
            }
            else {
                if(m_sinaBind!=NULL)
                {
                    sendSinaLogin();
                }
                else if(m_qqBind!=NULL)
                {
                    sendQQLogin();
                }
                innerExit();
            }
        }
    }
    else if ((8==pRet->iBtnSel && bIsShowVideoStrategy) || (7==pRet->iBtnSel && !bIsShowVideoStrategy))
    {
#if defined(kPltfmPp)
        OcProxy::Inst()->PPlogout();
        innerExit();
        MainScene::Inst()->switchAccount();
        OcProxy::Inst()->resetLogoutStatus();
#else
        if (CGameData::Inst()->getCommonInfo()->bShowCDKey)
        {
            if (CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin
                || CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin) {
                char buf[100];
                sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip18"));
                m_confirmSwitchUser = new Dialog2(this, callfuncO_selector(OtherMain::cbDL2SwitchUser), CGameData::Inst()->getLanguageValue("alert"), CGameData::Inst()->getLanguageValue("otherMainTip9"), enDl2TwoBtn);
                // m_confirmSwitchUser = new Dialog3(this, callfuncN_selector(OtherMain::cbDL2SwitchUser), );
                m_confirmSwitchUser->setPosition(CCPointMake(320, 480));
                addChild(m_confirmSwitchUser);
                m_confirmSwitchUser->release();
                m_confirmSwitchUser->setScale(0.0f);
                m_confirmSwitchUser->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
                innerExit();
            }
            else {
                if(m_sinaBind!=NULL)
                {
                    sendSinaLogin();
                }
                else if(m_qqBind!=NULL)
                {
                    sendQQLogin();
                }
                innerExit();
            }
        }
        else
        {
            if(m_qqBind!=NULL)
            {
                sendQQLogin();
            }
            
            innerExit();
        }
#endif
    }
    else if ((9==pRet->iBtnSel && bIsShowVideoStrategy) || (8==pRet->iBtnSel && !bIsShowVideoStrategy))
    {
        if(m_qqBind!=NULL)
        {
            sendQQLogin();
        }
        innerExit();
    }
    
//    switch (pRet->iBtnSel) {
//            
//        case 0:
//        {
//            innerExit();
//            m_curScene = ots_card;
//            if(CGameData::Inst()->needToReqCollection()){
//                CGameData::Inst()->collectionRequest();
//                scheduleUpdate();
//                m_iReqType = enOtherReqType_Collection;
//            }
//            else {
//                enterCollection();
//            }
//        }
//            break;
//        case 1:
//            enterUploadView();
//            break;
//        case 2:
//        {
//            innerExit();
//            m_curScene = ots_seeting;
//            m_settingLayer->setPosition(CCPointMake(640, 0));
//            m_settingLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
//            m_settingLayer->refreshSetting();
//            
//            MainScene::Inst()->showBottomPromptInfo(CGameData::Inst()->getLanguageValue("naviText_setting"));
//        }
//            break;
//        case 3:
//        {
//            innerExit();
//            m_curScene = ots_raider;
//            m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("text_guide_png"));
//            m_spTitle->setPosition(CCPointMake(-210, 766));
//            m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
//            
//            m_backItem->setPosition(CCPointMake(-529, 731));
//            m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
//            
//            MainScene::Inst()->showBottomPromptInfo(CGameData::Inst()->getLanguageValue("naviText_raider"));
//            
//            textLayer->setPosition(CCPointMake(640, 0));
//            textLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
//            textLayer->enterWithType(enUrlTypeHelp);
//            CGameData::Inst()->setIsEnterHelp();
//            MainScene::Inst()->showTips();
//            break;
//        }
//        case 4:
//        {
//            OcProxy::Inst()->linkToSafriWithUrlStr(CGameData::Inst()->getCommonInfo()->bbsurl);
//        }
//            break;
//            
//        case 5:
//            if (CGameData::Inst()->getCommonInfo()->bShowCDKey)
//                showCDKeyDialog();
//            else
//            {
//                if (CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin
//                    || CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin) {
//                    char buf[100];
//                    sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip18"));
//                    m_confirmSwitchUser = new Dialog2(this, callfuncO_selector(OtherMain::cbDL2SwitchUser), CGameData::Inst()->getLanguageValue("alert"), CGameData::Inst()->getLanguageValue("otherMainTip9"), enDl2TwoBtn);
//                    // m_confirmSwitchUser = new Dialog3(this, callfuncN_selector(OtherMain::cbDL2SwitchUser), );
//                    m_confirmSwitchUser->setPosition(CCPointMake(320, 480));
//                    addChild(m_confirmSwitchUser);
//                    m_confirmSwitchUser->release();
//                    m_confirmSwitchUser->setScale(0.0f);
//                    m_confirmSwitchUser->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
//                    innerExit();
//                }
//                else {
//                    if(m_sinaBind!=NULL)
//                    {
//                        sendSinaLogin();
//                    }
//                    else if(m_qqBind!=NULL)
//                    {
//                        sendQQLogin();
//                    }
//                    innerExit();
//                }
//            }
//            break;
//            
//        case 6:
//        {
//            if (CGameData::Inst()->getCommonInfo()->bShowCDKey)
//            {
//                if (CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin
//                    || CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin) {
//                    char buf[100];
//                    sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip18"));
//                    m_confirmSwitchUser = new Dialog2(this, callfuncO_selector(OtherMain::cbDL2SwitchUser), CGameData::Inst()->getLanguageValue("alert"), CGameData::Inst()->getLanguageValue("otherMainTip9"), enDl2TwoBtn);
//                    // m_confirmSwitchUser = new Dialog3(this, callfuncN_selector(OtherMain::cbDL2SwitchUser), );
//                    m_confirmSwitchUser->setPosition(CCPointMake(320, 480));
//                    addChild(m_confirmSwitchUser);
//                    m_confirmSwitchUser->release();
//                    m_confirmSwitchUser->setScale(0.0f);
//                    m_confirmSwitchUser->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
//                    innerExit();
//                }
//                else {
//                    if(m_sinaBind!=NULL)
//                    {
//                        sendSinaLogin();
//                    }
//                    else if(m_qqBind!=NULL)
//                    {
//                        sendQQLogin();
//                    }
//                    innerExit();
//                }
//            }
//            else
//            {
//                if(m_qqBind!=NULL)
//                {
//                    sendQQLogin();
//                }
//                
//                innerExit();
//            }
//        }
//            break;
//        case 7:
//        {
//            if(m_qqBind!=NULL)
//            {
//                sendQQLogin();
//            }
//            
//            innerExit();
//        }
//        default:
//            break;
//    }
}

void OtherMain::sendQQLogin()
{
    OcProxy::Inst()->deleteCookies();
    if(CGameData::Inst()->loginByTencentRequest())
    {
        // setIsTouchEnabled(false);
        MainScene::Inst()->enableBottomPanel(false);
        m_iReqType = enOtherReqType_QQLogin;
//        char buf[100];
//        snprintf(buf, 99, "naviText_weibo_%d", int(CCRANDOM_0_1() * 2) + 1);
        
   
       
        MainScene::Inst()->showBottomPromptInfo("naviText_weibo_");
      //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        scheduleUpdate();
    }
}

void OtherMain::sendSinaLogin()
{
    OcProxy::Inst()->deleteCookies();
    if(CGameData::Inst()->loginBySinaRequest())
    {
        MainScene::Inst()->enableBottomPanel(false);
        m_iReqType = enOtherReqType_SinaLogin;
        //setIsTouchEnabled(false);
//        char buf[100];
//        snprintf(buf, 99, "naviText_weibo_%d", int(CCRANDOM_0_1() * 2) + 1);
        
      
        MainScene::Inst()->showBottomPromptInfo("naviText_weibo_");
     //    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        scheduleUpdate();
    }
}



void OtherMain::update(ccTime ct)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if (comState == kReqHoldOn) {
        return;
    }
    if (comState == kReqCancel) {
        
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
            MainScene::Inst()->enableBottomPanel(true);
        }
        
        m_iReqType=enOtherReqType_Null;
        unscheduleUpdate();
        innerEnter();
        MainScene::Inst()->enableBottomPanel(true);
        CGameData::Inst()->clearReqStat();
        return;
    }
    
    
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(OtherMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        switch (m_iReqType) {
            case enOtherReqType_QQLogin:
            case enOtherReqType_SinaLogin:
                //setIsTouchEnabled(true);
                showBindResult();
             
                
                break;
                
            case enOtherReqType_Collection:
                enterCollection();
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enOtherReqType_ExchangeCDKey:
                if (CGameData::Inst()->getGiftInfo()->strUcidList.size() > 0)
                    getAllCards();
                else
                    showExchangeSuccessDlg();
                break;
                
            case enOtherReqType_AllCards:
                showExchangeSuccessDlg();
                break;
                
            default:
                break;
        }
        
        
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(OtherMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void OtherMain::cbOtherCardLayer(CCObject *pSender)
{
    OtherCardSelRet *cbRet = (OtherCardSelRet *)pSender;
    CCardBaseInfo *cardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(cbRet->iBtnSel+1);
    printf("otherCallBack %d\n",cbRet->iBtnSel);
    // show base card info
    if(cardBaseInfo){
        if (m_pScrollViewLayer)
        {
            m_pScrollViewLayer->show(cardBaseInfo);
        }
        else
        {
            m_showCardInfo->show(cardBaseInfo);
        }
        MainScene::Inst()->showBottomPanel(false);
    }
}
void OtherMain::cbSettignLayer(CCObject *object)
{
    CSettingLayerRet *ret = (CSettingLayerRet *)object;
    switch (ret->iBtnSel) {
        case 1:
        {
            //   printf("hahahahah call back\n");
            m_settingLayer->setPosition(ccp(-1000, -1000));
            innerEnter();
        }
            break;
            
        default:
            break;
            
    }
}
void OtherMain::cbRemindLayer(CCObject *object)
{
    /*CRemindLayerRet *ret = (CRemindLayerRet *)object;
    switch (ret->iBtnSel) {
        case 1:
        {
            //   printf("hahahahah call back\n");
            m_remindLayer->setPosition(ccp(-1000, -1000));
            innerEnter();
        }
            break;
            
        default:
            break;
            
    }*/
}
void OtherMain::btnCallback(CCObject *pSender)
{
    if (m_pScrollViewLayer && m_pScrollViewLayer->getIsVisible())
    {
        m_pScrollViewLayer->hide();
        return;
    }
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    if(m_curScene == ots_card)
    {
        //推出之前保存 图鉴位置
        if(cardLayer != NULL){
            //推出前 保存cardLayer Y值
            if (cardLayer) {
                cardLayerY=cardLayer->getPosition().y;
            }
            cardLayer->removeAllChildrenWithCleanup(true);
            cardLayer->removeFromParentAndCleanup(true);
            cardLayer = NULL;
        }
        
    }else if(m_curScene == ots_raider){
        textLayer->setPosition(ccp(-1000, -1000));
        textLayer->exit();
    }else if(m_curScene == ots_about)
    {
        
    }else if (m_curScene == ots_remind){
        if(m_remindLayer != NULL){
            m_remindLayer->layerMoveUpOrDown(false);
            m_remindLayer->removeAllChildrenWithCleanup(true);
            m_remindLayer->removeFromParentAndCleanup(true);
            m_remindLayer = NULL;
        }
    }
    else if (m_curScene == ots_cdkey)
    {
        if (m_pCDKeyDialog)
        {
            m_pCDKeyDialog->removeFromParentAndCleanup(true);
            m_pCDKeyDialog = NULL;
        }
    }
    
    if (m_bFromHome)
        MainScene::Inst()->GoToHomePage();
    else
    {
        innerExit();
        innerEnter();
    }
}
//******************************************************************************
// enter
//******************************************************************************
void OtherMain::enter()
{
    if (CGameData::Inst()->isFighting())
        return;
    
    m_bFromHome = false;
    m_pCDKeyDialog = NULL;
    numberTips = NULL;
    m_sinaBind = NULL;
    m_qqBind = NULL;
    cardLayer = NULL;
    m_commDlg = NULL;
    m_remindLayer = NULL;
    m_pScrollViewLayer= NULL;
    m_iReqType = enOtherReqType_Null;
    m_curScene = 0;
    
    mainList = new CListLayer(this, callfuncO_selector(OtherMain::cbList));
    
    addChild(mainList, -1);
    mainList->release();


    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_backItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(OtherMain::btnCallback));
    m_backItem->setTag(1);
    m_backItem->setAnchorPoint(CCPointZero);
    CCMenu* menu = CCMenu::menuWithItems(m_backItem, NULL);
    addChild(menu, 30);
    menu->setPosition(CCPointZero);
    
    
    //setting
    m_settingLayer = new SettingLayer(this,menu_selector(OtherMain::cbSettignLayer));
    addChild(m_settingLayer);
    m_settingLayer->release();
    
    // upload video
    m_upload_layer = MyVideoLayer::Create(this, menu_selector(OtherMain::cbUploadVideo));
    addChild(m_upload_layer);
    m_upload_layer->release();
    
    m_showCardInfo = new CShowCardInf();
    addChild(m_showCardInfo,100);
    m_showCardInfo->release();
    
    m_pScrollViewLayer = CScrollViewLayer::create(this);
    addChild(m_pScrollViewLayer,101);
    m_pScrollViewLayer->release();
    
    textLayer = new RaidersLayer();
    addChild(textLayer);
    textLayer ->setPosition(ccp(-1000, -1000));
    
    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    m_shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(OtherMain::shareSanGuo));
    m_shareBtn->setAnchorPoint(CCPointZero);
    CCMenu *shareMenu = CCMenu::menuWithItem(m_shareBtn);
    addChild(shareMenu,2);
    shareMenu->setPosition(CCPointZero);
    TextNode *shareLabel;
    shareLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shareTip"), 30);
    shareLabel->setPosition(ccp(53, 35));
    shareLabel->setShadowColor(ccBLACK);
    m_shareBtn->addChild(shareLabel);
    
    m_backItem ->setPosition(ccp(-1000, -1000));
    mainList->setPosition(ccp(-1000, -1000));
    m_spTitle->setPosition(ccp(-1000, -1000));
    m_settingLayer->setPosition(ccp(-1000, -1000));
    m_upload_layer->setPosition(ccp(-1000, -1000));
    m_shareBtn->setPosition(ccp(-1000,-1000));
    
    
    if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
        shareMenu ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
        shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else {
        shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
    }
    
    
    innerEnter();
}

//******************************************************************************
// exit
//******************************************************************************
void OtherMain::exit()
{
    innerExit();
    
    textLayer->exit();
    textLayer->setPosition(ccp(-1000,-1000));
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// onExit
//******************************************************************************
void OtherMain::onExit()
{
    CCLayer::onExit();
}

//******************************************************************************
// innerEnter
//******************************************************************************
void OtherMain::innerEnter()
{
    initMainList();
    m_shareBtn->setPosition(ccp(-1000,-1000));
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_others"));
    
    mainList->setPosition(CCPointMake(640, 0));
    mainList->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
   
    MainScene::Inst()->showBottomPromptInfo("naviText_other");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_upload_layer->setPosition(ccp(-1000, -1000));
    
    showTips();
}

//******************************************************************************
// innerExit
//******************************************************************************
void OtherMain::innerExit()
{
    if (m_pCDKeyDialog)
    {
        m_pCDKeyDialog->removeFromParentAndCleanup(true);
        m_pCDKeyDialog = NULL;
    }
    
    m_spTitle->stopAllActions();
    m_backItem->stopAllActions();
    mainList->stopAllActions();
    m_settingLayer->stopAllActions();
    m_remindLayer->stopAllActions();
    m_upload_layer->stopAllActions();
    m_shareBtn->setPosition(ccp(-1000,-1000));
    m_shareBtn->stopAllActions();
    
    m_backItem ->setPosition(ccp(-1000, -1000));
    mainList->setPosition(ccp(-1000, -1000));
    m_spTitle->setPosition(ccp(-1000, -1000));
    
    mainList->clearAll();
    
    m_sinaBind = NULL;
    m_qqBind = NULL;
    m_helpBtn = NULL;
    numberTips = NULL;
}

//******************************************************************************
// showCDKeyDilog
//******************************************************************************
void OtherMain::showCDKeyDialog()
{
    innerExit();
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("title_cdKey"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_cdkey_1");
   // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16

    m_curScene = ots_cdkey;
    
    if (m_pCDKeyDialog == NULL)
    {
        m_pCDKeyDialog = new CDKeyExchange(this, menu_selector(OtherMain::cbCDKeyDialog));
        addChild(m_pCDKeyDialog);
        m_pCDKeyDialog->release();
    }
    
    m_pCDKeyDialog->showCDKeyExchangeDlg();
    
    m_pCDKeyDialog->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0));
    m_pCDKeyDialog->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

//******************************************************************************
// cbCDKeyDialog
//******************************************************************************
void OtherMain::cbCDKeyDialog()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    exchangeCDKey();
}

//******************************************************************************
// getAllCards
//******************************************************************************
void OtherMain::getAllCards()
{
    if (CGameData::Inst()->allCardRequest())
    {
        m_iReqType = enOtherReqType_AllCards;
        scheduleUpdate();
    }
}

//******************************************************************************
// exchangeCDKey
//******************************************************************************
void OtherMain::exchangeCDKey()
{
    if (m_pCDKeyDialog->exchangeCDKey())
    {
        m_iReqType = enOtherReqType_ExchangeCDKey;
        scheduleUpdate();
    }
}

//******************************************************************************
// showExchangeSuccessDlg
//******************************************************************************
void OtherMain::showExchangeSuccessDlg()
{
    if (m_pCDKeyDialog)
    {
        m_pCDKeyDialog->removeFromParentAndCleanup(true);
        m_pCDKeyDialog = NULL;
    }
    
    if (m_pCDKeyDialog == NULL)
    {
        m_pCDKeyDialog = new CDKeyExchange(this, menu_selector(OtherMain::cbExchangeSuccessDlg));
        addChild(m_pCDKeyDialog);
        m_pCDKeyDialog->release();
    }
    
    m_pCDKeyDialog->showExchangeSuccessDlg();
    
    m_pCDKeyDialog->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0));
    m_pCDKeyDialog->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

//******************************************************************************
// cbExchangeSuccessDlg
//******************************************************************************
void OtherMain::cbExchangeSuccessDlg()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    innerExit();
    innerEnter();
     MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// enterCollection
//******************************************************************************
void OtherMain::enterCollection()
{
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("text_tujian_png"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    m_shareBtn->setPosition(ccp(1000, 740));
    m_shareBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
    
    if(cardLayer == NULL)
    {
        cardLayer = new OtherCardLayer(this,callfuncO_selector(OtherMain::cbOtherCardLayer));
        addChild(cardLayer);
        cardLayer->release();
    }
    cardLayer->setIsVisible(true);
    CCLOG("cardLayerY=%f",cardLayerY);
    if (cardLayer) {
       cardLayer->setThisPosition(ccp(1000, cardLayerY),true);
    }
    cardLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1,CCPointMake(0, cardLayer->getPosition().y)), 0.8f));
    
//    char buf[100];
//    snprintf(buf, 99, "naviText_collection_%d", int(CCRANDOM_0_1() * 3) + 1);
    
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_collection_");
   //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void OtherMain::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk){
        switch (m_iReqType) {
            case enOtherReqType_QQLogin:
                CGameData::Inst()->loginByTencentRequest();
                scheduleUpdate();
                break;
                
            case enOtherReqType_SinaLogin:
                CGameData::Inst()->loginBySinaRequest();
                scheduleUpdate();
                
                break;
                
            case enOtherReqType_Collection:
                CGameData::Inst()->collectionRequest();
                scheduleUpdate();
                break;
                
            case enOtherReqType_ExchangeCDKey:
                exchangeCDKey();
                break;
                
            default:
                break;
        }
    }
    else {
        if (m_iReqType == enOtherReqType_ExchangeCDKey)
        {
            MainScene::Inst()->showBottomPanel(true);
            CGameData::Inst()->clearReqStat();

            return;
        }
        
        if(m_iReqType == enOtherReqType_Collection)
        {
            CGameData::Inst()->setIsNeedToReqCollection(true);
        }
        innerExit();
        innerEnter();
        MainScene::Inst()->showBottomPanel(true);
        CGameData::Inst()->clearReqStat();
        
    }
}

void OtherMain::showBindResult()
{
    Dialog1* bindResultDlg;
    
    bindResultDlg= new Dialog1(this, callfuncO_selector(OtherMain::bindSuccess), CGameData::Inst()->getLanguageValue("otherMainTip7"),false);
    
    addChild(bindResultDlg, 500,kBindResultDlgTag);
    bindResultDlg->setPosition(CCPointMake(320, 480));
    bindResultDlg->setScale(0.0);
    bindResultDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    bindResultDlg->release();
    
}

void OtherMain::bindSuccess()
{
    CCNode* payResultDlg = getChildByTag(kBindResultDlgTag);
    if(payResultDlg)
    {
        removeChild(payResultDlg, true);
    }
    MainScene::Inst()->enableBottomPanel(true);
    MainScene::Inst()->reLogin();
}

void OtherMain::showTips()
{
    if(numberTips!=NULL)
    {
        numberTips->removeFromParentAndCleanup(true);
        numberTips= NULL;
    }
    
    if(!CGameData::Inst()->isEnterHelp())
    {
        numberTips = OcProxy::Inst()->showTipsAndNum(10);
        numberTips->setPosition(ccp(191, 5));
        m_helpBtn->addChild(numberTips);
    }
}

//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void OtherMain::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(OtherMain::shareToWeibo));
    
}

//******************************************************************************
// upload
//******************************************************************************
void OtherMain::enterUploadView()
{   
    innerExit();
    
    m_upload_layer->setPosition(CCPointZero);
    m_upload_layer->Refresh();
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("record_other_main_title"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
}

void OtherMain::cbUploadVideo()
{
    // 点击了 返回  这个 没有视频的回掉
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_bFromHome)
        MainScene::Inst()->GoToHomePage();
    else
    {
        innerExit();
        innerEnter();
        m_upload_layer->setPosition(CCPointMake(-1000, -1000));
    }
}

//******************************************************************************
// enterMyVideo
//******************************************************************************
void OtherMain::enterMyVideo()
{
    m_bFromHome = true;
    enterUploadView();
}

//******************************************************************************
// enterVideoStrategy
//******************************************************************************
void OtherMain::enterVideoStrategy()
{
    innerExit();
    
    m_upload_layer->setPosition(CCPointZero);
    m_upload_layer->RefreshStrategy();
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("title_video_strategy"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
}

