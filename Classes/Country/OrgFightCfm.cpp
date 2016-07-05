//
//  OrgFightCfm.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgFightCfm.h"
#include "CGameData.h"
#include "OrgRankList.h"
#include "TextNode.h"
#include "CMonsterBlood.h"
#include "MainScene.h"
#include "TitleBar.h"
#include "Dialog2.h"
#include "Dialog1.h"
#include "SimpleAudioEngine.h"
#include "CommDlg.h"

using namespace CocosDenshion;

enum enOFC_ReqType{
    enOFC_ReqNull,
    enOFC_ReqRcvPwr,
};

#define kDianJiTiaoZhanTag  874

OrgFightCfm::OrgFightCfm(CCObject* target, SEL_CallFuncO selector, string dg_id)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_dlgTip = NULL;
    m_dlgRecover = NULL;
    m_iCurReqType = enOFC_ReqNull;
    m_commDlg = NULL;
    m_iCntDwn = 0;
    m_spBtnJiFeng = NULL;
    m_spBtnLuanWu = NULL;
    m_spBtnBg = NULL;
    m_rankLy = NULL;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    
    CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
    CCntryDgInfo* pDgInf = NULL;
    for (int i = 0; i < pCntryDgInfArr->count(); i++) {
        pDgInf = pCntryDgInfArr->getObjectAtIndex(i);
        
        if (dg_id.compare(pDgInf->dg_id) == 0)
            break;
        else
            pDgInf = NULL;
    }
   
    
    //排名面板
    CCSprite* sp = CCSprite::spriteWithFile("rankpanel.png");
    if (sp) {
        addChild(sp);
        sp->setAnchorPoint(ccp(0, 0));
        sp->setPositionInPixels(ccp(0, 145));
    }
    
    //敌将背景
    sp = CCSprite::spriteWithFile("cj_org.jpg");
    if(sp){
        sp->setPosition(CCPointMake(320, 426));
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setScale(1/0.79);
        addChild(sp, -20);
    }
    
    CCardBaseInfo* pCardInf = NULL;
    if (pDgInf) {
        pCardInf = CGameData::Inst()->getCardBaseInfByCid(pDgInf->cid);
        int iElem = 0;
        if (pCardInf) {
            iElem = pCardInf->ctype;
            
            CCSprite* spStar;
            for(int i = 0; i < pCardInf->star; i++){
                spStar = CCSprite::spriteWithSpriteFrameName("id_star.png");
                if (spStar) {
                    float starBeginPosX = 320 - spStar->getContentSize().width * (pCardInf->star - 1) / 2;
                    spStar->setPosition(CCPointMake(starBeginPosX + spStar->getContentSize().width * i, 715));
                    addChild(spStar, 1);
                }
            }
        }
        
        TitleBar * spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
        addChild(spTitle);
        spTitle->setTitleString(pDgInf->dg_name);
        spTitle->setPosition(CCPointMake(320, 766));
        
        //敌将
        CCSprite* spBody = CGameData::Inst()->getBodySprite(pDgInf->cid);
        spBody->setAnchorPoint(CCPointMake(0.5, 0));
        spBody->setPositionInPixels(CCPointMake(320, 436));
        addChild(spBody);

        //点击挑战按钮
        bool bShowBtn = false;
        if (strcmp(pDgInf->dg_type, "0") == 0
            && pDgInf->bIsFinish == false)
            bShowBtn = true;
        else if (strcmp(pDgInf->dg_type, "2") == 0
                 && pDgInf->bIsFinish == false)
            bShowBtn = true;
        else if (strcmp(pDgInf->dg_type, "1") == 0)
        {
            long curTime = CGameData::Inst()->getCurrentTime();
            if ((pDgInf->start_time <= curTime
                 && pDgInf->start_time + pDgInf->last_time * 3600 >= curTime)
                && (pDgInf->dungeon_tms < pDgInf->dungeon_tms_config
                    || !pDgInf->bIsFinish))
                bShowBtn = true;
        }else if (strcmp(pDgInf->dg_type, "3") == 0 ){
//            long curTime = CGameData::Inst()->getCurrentTime();
            if ( !pDgInf->bIsFinish){
                bShowBtn = true;
          }
        }
        if (bShowBtn) {
            CCPoint pt = spBody->getPosition();
            CCSize sz = spBody->getContentSize();
            pt.y += sz.height*0.5;
            pt.y += 30;
            CMonsterBlood* mblood = new CMonsterBlood(pt, sz, iElem);
            mblood->setBlood(pDgInf->curBossHp, pDgInf->totalBossHp, 0);
            addChild(mblood);
            mblood->release();
            
            char buf[100];
            int iPerc = (int)(pDgInf->curBossHp*1.0/pDgInf->totalBossHp * 100);
            iPerc = (iPerc < 1)? 1:iPerc;
            
            snprintf(buf, 99, "%d%%", iPerc);
            TextNode* text = TextNode::textWithString(buf, 20);
            text->setPositionInPixels(CCPointMake(mblood->getRightPosX() + 10, 466));
            text->setColor(ccc3(255, 255, 255));
            text->setShadowColor(ccBLACK);
            text->setAnchorPoint(CCPointMake(0, 0.5));
            addChild(text);
            
            m_txtCntDwn = TextNode::textWithString("", 20);
            m_txtCntDwn->setPositionInPixels(CCPointMake(214, 444));
            m_txtCntDwn->setColor(ccc3(255, 255, 255));
            m_txtCntDwn->setAnchorPoint(CCPointMake(0, 0.5));
            m_txtCntDwn->setShadowColor(ccBLACK);
            addChild(m_txtCntDwn);
            
            if (strcmp(pDgInf->dg_type, "0") == 0)
                m_fCntDwnTtl = pDgInf->open_time + pDgInf->last_time * 3600 - CGameData::Inst()->getCurrentTime();
            else if (strcmp(pDgInf->dg_type, "3") == 0)
               //  m_fCntDwnTtl = pDgInf->open_time + pDgInf->last_time * 3600 - CGameData::Inst()->getCurrentTime();
                m_fCntDwnTtl = pDgInf->closed_time - CGameData::Inst()->getCurrentTime();
            else
                m_fCntDwnTtl = pDgInf->start_time + pDgInf->last_time * 3600 - CGameData::Inst()->getCurrentTime();
            
            if (m_fCntDwnTtl > 0) {
                m_fElapse = 0;
                m_iCntDwn = m_fCntDwnTtl;
                scheduleUpdate();
            }
            
            CCSprite* spOk = CCSprite::spriteWithFile("dianjitiaozhan.png");
            if (spOk) {
                addChild(spOk, 0, kDianJiTiaoZhanTag);
                spOk->setPosition(ccp(316, 394));
            }
        }
    }
        
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCMenuItem* miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(OrgFightCfm::cbBack) );
    miBack->setAnchorPoint(CCPointZero);
    miBack->setPosition(CCPointMake(1, 730));
    
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCMenuItem *miAcRcv = CCMenuItemImage::itemFromNormalSprite(sp1, sp2, this, menu_selector(OrgFightCfm::cbAcRcv));
    miAcRcv->setAnchorPoint(CCPointZero);
    miAcRcv->setPosition(ccp(530, 730));
    
    TextNode* tnRecover = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_recover"),
                                                   sp1->getContentSize(),
                                                   CCTextAlignmentCenter,
                                                   24);
    tnRecover->setShadowColor(ccBLACK);
    tnRecover->setPosition(CCPointMake(sp1->getContentSize().width * 0.5,
                                       sp1->getContentSize().height * 0.5 - 28));
    miAcRcv->addChild(tnRecover);

    CCMenu* menu = CCMenu::menuWithItems(miBack, miAcRcv, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    if (strcmp(pDgInf->dg_type, "1") == 0
        || strcmp(pDgInf->dg_type, "2") == 0){
        //排行、名字、时间(用时)
        TextNode* text = TextNode::textWithString(CGameData::Inst()->getLanguageValue("orgftcfm_rank"), 24);
        text->setPositionInPixels(CCPointMake(22, 300));
        text->setAnchorPoint(ccp(0, 0.5));
        text->setColor(ccc3(42, 184, 248));
        addChild(text);
        
        text = TextNode::textWithString(CGameData::Inst()->getLanguageValue("orgftcfm_name"), 24);
        text->setPositionInPixels(CCPointMake(190, 300));
        text->setAnchorPoint(ccp(0, 0.5));
        text->setColor(ccc3(42, 184, 248));
        addChild(text);

        m_txtJiFengTime = TextNode::textWithString("", 24);
        m_txtJiFengTime->setPositionInPixels(CCPointMake(426, 300));
        m_txtJiFengTime->setAnchorPoint(ccp(0, 0.5));
        m_txtJiFengTime->setColor(ccc3(42, 184, 248));
        addChild(m_txtJiFengTime);
        
        showJiFengLuanWuBtn(pDgInf->dg_type);
    }
    else
    {
        char buf[100];
        snprintf(buf, 99, "%s:%ld",
                 CGameData::Inst()->getLanguageValue("orgftcfm_dsp"),
                 CGameData::Inst()->getMyDamageFromRankInf());
        
        TextNode* text = TextNode::textWithString(buf, 20);
        text->setPositionInPixels(CCPointMake(320, 354));
        text->setColor(ccc3(249, 196, 15));
        addChild(text);
        
        text = TextNode::textWithString(CGameData::Inst()->getLanguageValue("orgftcfm_rank"), 24);
        text->setPositionInPixels(CCPointMake(22, 326));
        text->setAnchorPoint(ccp(0, 0.5));
        text->setColor(ccc3(42, 184, 248));
        addChild(text);
        
        text = TextNode::textWithString(CGameData::Inst()->getLanguageValue("orgftcfm_name"), 24);
        text->setPositionInPixels(CCPointMake(190, 326));
        text->setAnchorPoint(ccp(0, 0.5));
        text->setColor(ccc3(42, 184, 248));
        addChild(text);
        
        snprintf(buf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("orgftcfm_dmg"));
        
        text = TextNode::textWithString(buf, 24);
        text->setPositionInPixels(CCPointMake(426, 326));
        text->setAnchorPoint(ccp(0, 0.5));
        text->setColor(ccc3(42, 184, 248));
        addChild(text);
        
        m_rankLy = new OrgRankList(pDgInf->dg_type);
        addChild(m_rankLy, 10);
        m_rankLy->release();
    }
}

void OrgFightCfm::showJiFengLuanWuBtn(string dgType)
{
    if (m_rankLy && m_rankLy->getDgType() == dgType)
        return;
    
    if (!m_spBtnJiFeng)
    {
        m_spBtnJiFeng = CCSprite::spriteWithFile("fr_button_jifeng.png");
        m_spBtnJiFeng->setPosition(CCPointMake(170, 340));
        addChild(m_spBtnJiFeng, 1);
    }
    
    if (!m_spBtnLuanWu)
    {
        m_spBtnLuanWu = CCSprite::spriteWithFile("fr_button_luanwu.png");
        m_spBtnLuanWu->setPosition(CCPointMake(470, 340));
        addChild(m_spBtnLuanWu, 1);
    }
    
    if (!m_spBtnBg)
    {
        m_spBtnBg = CCSprite::spriteWithFile("fr_bg_unselected.png");
        addChild(m_spBtnBg);
    }
    
    if (dgType == "1")
        m_spBtnBg->setPosition(m_spBtnLuanWu->getPosition());
    else
        m_spBtnBg->setPosition(m_spBtnJiFeng->getPosition());
    
    
    if (m_rankLy)
    {
        m_rankLy->removeFromParentAndCleanup(true);
        m_rankLy = NULL;
    }
    
    m_rankLy = new OrgRankList(dgType);
    addChild(m_rankLy, 10);
    m_rankLy->release();
    
    if (dgType == "1")
        m_txtJiFengTime->setString(CGameData::Inst()->getLanguageValue("jiFengLuanWu_end_time"));
    
    else
        m_txtJiFengTime->setString(CGameData::Inst()->getLanguageValue("jiFengLuanWu_time"));
}

void OrgFightCfm::update(ccTime dt)
{
    if (m_iCntDwn > 0) {
        m_fElapse += dt;
        
        int iCntDwn = (m_fCntDwnTtl - m_fElapse);
        
        if (iCntDwn == m_iCntDwn) {
            return;
        }
        
        m_iCntDwn = iCntDwn;
        
        char buf[100];
        snprintf(buf,
                 99,
                 "%s: %02d:%02d:%02d",
                 CGameData::Inst()->getLanguageValue("cntry_time_left"),
                 iCntDwn/3600,
                 (iCntDwn % 3600) / 60,
                 iCntDwn % 60);
        
        m_txtCntDwn->setString(buf);
    }
    
    if(m_iCurReqType != enOFC_ReqNull) {
        int comState = CGameData::Inst()->getReqStat();
        if(comState == kReqStatRequesting){
            if(m_commDlg == NULL){
                MainScene::Inst()->enableBottomPanel(false);
                m_commDlg = new CommDlg(this,
                                        callfuncO_selector(OrgFightCfm::cbCommDlg),
                                        enCommDlgTp_connecting);
                addChild(m_commDlg, 1000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
            }
            return;
        }
        else if(comState == kReqStatOk){
            m_iCurReqType = enOFC_ReqNull;
            
            if(m_commDlg != NULL){
                removeChild(m_commDlg, true);
                m_commDlg = NULL;
                MainScene::Inst()->enableBottomPanel(true);
            }
        }
        else {
            m_iCurReqType = enOFC_ReqNull;
            
            if(m_commDlg != NULL){
                removeChild(m_commDlg, true);
                m_commDlg = NULL;
            }
            
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(OrgFightCfm::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

bool OrgFightCfm::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
   
    if (m_spBtnJiFeng
        && CCRect::CCRectContainsPoint(CCRectMake(m_spBtnJiFeng->getPosition().x - m_spBtnJiFeng->getContentSize().width * 0.5,
                                                  m_spBtnJiFeng->getPosition().y - m_spBtnJiFeng->getContentSize().height * 0.5,
                                                  m_spBtnJiFeng->getContentSize().width,
                                                  m_spBtnJiFeng->getContentSize().height),
                                       nodePos))
    {
        showJiFengLuanWuBtn("1");
        
        return false;
    }
    
    if (m_spBtnLuanWu
        && CCRect::CCRectContainsPoint(CCRectMake(m_spBtnLuanWu->getPosition().x - m_spBtnLuanWu->getContentSize().width * 0.5,
                                                  m_spBtnLuanWu->getPosition().y - m_spBtnLuanWu->getContentSize().height * 0.5,
                                                  m_spBtnLuanWu->getContentSize().width,
                                                  m_spBtnLuanWu->getContentSize().height),
                                       nodePos))
    {
        showJiFengLuanWuBtn("2");
        
        return false;
    }
   
    if (m_iCntDwn == 0)
        return false;

    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(166, 370, 310, 60), nodePos);
    
    if (!bRet) {
        return false;
    }
    
    if (CGameData::Inst()->getLocalAcPoint() > 0){
        m_ret.bOk = true;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else
        showAcPointNotEnoughTip();

    return true;
}

void OrgFightCfm::cbBack(CCObject* sender)
{
    m_ret.bOk = false;
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void OrgFightCfm::cbAcRcv(CCObject* sender)
{
    if (CGameData::Inst()->getLocalAcPoint() < CGameData::Inst()->getUsrInfo()->max_acPoint)
    {
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->recover_action_coin)
            showRecoverAcPointTip();
        else
            showNoCoinForRecover();
    }
    else
        // 战斗力已满无需回复
        showAcPointFullTip();
}

void OrgFightCfm::showRecoverAcPointTip()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_recover"),
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(OrgFightCfm::cbRecoverAcPointTip),
                               CGameData::Inst()->getLanguageValue("actionPoint_recover_title"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void OrgFightCfm::cbRecoverAcPointTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true)
        recoverActionPoint();
    else {
        // 取消
    }
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void OrgFightCfm::showNoCoinForRecover()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_coin_notEnough"),
                 CGameData::Inst()->getUsrInfo()->coin,
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(OrgFightCfm::cbNoCoinForRecover),
                               CGameData::Inst()->getLanguageValue("arenaTip8"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}


void OrgFightCfm::cbNoCoinForRecover(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
    
    if(pRet->bOk == true)
        MainScene::Inst()->goToShop();
    else {
        // 取消
    }
}

void OrgFightCfm::showAcPointFullTip()
{
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(OrgFightCfm::cbAcPointFullTip),
                                   CGameData::Inst()->getLanguageValue("actionPoint_full"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    
}

void OrgFightCfm::cbAcPointFullTip()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
}

void OrgFightCfm::recoverActionPoint()
{
    CGameData::Inst()->recoverPowerRequest();
    m_iCurReqType = enOFC_ReqRcvPwr;
}

void OrgFightCfm::showAcPointNotEnoughTip()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_notEnough"),
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(OrgFightCfm::cbAcPointNotEnoughTip),
                               CGameData::Inst()->getLanguageValue("actionPoint_notEnough_title"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void OrgFightCfm::cbAcPointNotEnoughTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
    
    if(pRet->bOk == true)
    {
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->recover_action_coin)
            recoverActionPoint();
        else
            showNoCoinForRecover();
    }
    else {
    }
}

void OrgFightCfm::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if((isOk == true) && (m_iCurReqType == enOFC_ReqRcvPwr)){
        recoverActionPoint();
    }
    else {
        CGameData::Inst()->clearReqStat();
    }
}
