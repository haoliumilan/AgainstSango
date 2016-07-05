//
//  RwdBonusLayer.cpp
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RwdBonusLayer.h"
#include "OcProxy.h"

RwdBonusLayer::RwdBonusLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgAlert = NULL;
    m_commDlg = NULL;
    
    m_dlgNoTip = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_BonusList = new CListLayer(this, callfuncO_selector(RwdBonusLayer::cbList),4,144,650);
    addChild(m_BonusList, -1);
    m_BonusList->release();
    m_BonusList->setReloadFunc(callfuncO_selector(RwdBonusLayer::insertItemByIdx));
    
    showBonusList();
}

RwdBonusLayer::~RwdBonusLayer()
{
    
}

void RwdBonusLayer::insertItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<RwdBonusInf*> * pBonusInfArr = CGameData::Inst()->getRwdBonusInfArr();
    
    if(pBonusInfArr->count() == 0)
        return;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(pBonusInfArr->getObjectAtIndex(i),i);
    
    m_BonusList->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(RwdBonusLayer::cbGetRwdItem));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    itemShow->setPosition(ccp(90,-58));
    
    char buf[100];
    if (pBonusInfArr->getObjectAtIndex(i)->needShare)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("share_and_lingqu"));
    else
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
    TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
    if (pBonusInfArr->getObjectAtIndex(i)->needShare) {
        lbText = TextNode::textWithString(buf, kBtnTitleHeight-15);
        lbText->setPosition(CCPointMake(30, 20));
        lbText->setAnchorPoint(CCPointZero);
    }else{
        lbText->setPosition(CCPointMake(50,14));
        lbText->setAnchorPoint(CCPointZero);
    }
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    itemShow->addChild(lbText, 1);
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
}

void RwdBonusLayer::showBonusList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<RwdBonusInf*> * pBonusInfArr = CGameData::Inst()->getRwdBonusInfArr();
    
    for (int i = 0; i < pBonusInfArr->count(); i++)
    {
        if (i >= m_BonusList->getPreTopShow()
            && i <= m_BonusList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pBonusInfArr->getObjectAtIndex(i),i);
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(RwdBonusLayer::cbGetRwdItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(90,-58));
            
            char buf[100];
            if (pBonusInfArr->getObjectAtIndex(i)->needShare)
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("share_and_lingqu"));
            else
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            if (pBonusInfArr->getObjectAtIndex(i)->needShare) {
                lbText = TextNode::textWithString(buf, kBtnTitleHeight-15);
                lbText->setPosition(CCPointMake(30, 20));
                lbText->setAnchorPoint(CCPointZero);
            }else{
                lbText->setPosition(CCPointMake(50,14));
                lbText->setAnchorPoint(CCPointZero);
            }
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            itemShow->addChild(lbText, 1);
            
            CCMenu *menu = CCMenu::menuWithItem(itemShow);
            newMenuBtn->addChild(menu);
            menu->setPosition(CCPointZero);
        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_BonusList->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
    }
    
}

void RwdBonusLayer::cbList(CCObject* pObj)
{
    
}

void RwdBonusLayer::cbGetRwdItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_iListSel = mItem->getTag() / 10;
    
    GetRwdItem();
}

void RwdBonusLayer::GetRwdItem()
{
    if (CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->iDays < CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->rewardDays){
    }else{
        char buf[50];
        snprintf(buf, 49, "%d",CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->rewardDays);
        
        if(strcmp(OcProxy::Inst()->getValueForKey("platform"), "oc") == 0 && CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->needShare)
            showAlertNoCombineDlg();
        else{
            if (CGameData::Inst()->reqGetAwardBonus("1", buf))
            {
                if (CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->needShare) {
                    OcProxy::Inst()->sendWeibo(CGameData::Inst()->getCommonInfo()->voice_1, false);
                }
                m_awardDsp = CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(m_iListSel)->awardDsp;
                scheduleUpdate();
            }
        }
        
    }
}

void RwdBonusLayer::showAlertNoCombineDlg()
{
    if (m_dlgNoTip == NULL)
    {
        m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(RwdBonusLayer::cbNoCombineDlg));
        addChild(m_dlgNoTip);
        m_dlgNoTip->release();
        m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("NoCombine_Dlg"));
        m_dlgNoTip->setPosition(CCPoint(320, 300));
        m_dlgNoTip->setScale(0.0);
        CCActionInterval* tempAction = CCScaleTo::actionWithDuration(0.5, 1.0);
        m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
}

void RwdBonusLayer::cbNoCombineDlg()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
}


void RwdBonusLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RwdBonusLayer::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
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
        showAlertGetSuccessDlg();
         MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    else
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RwdBonusLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


//******************************************************************************
// cbCommDlg
//******************************************************************************
void RwdBonusLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk)
    {
        GetRwdItem();
    }
    else
    {
        CGameData::Inst()->clearReqStat();
    }
}


void RwdBonusLayer::showAlertGetSuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(RwdBonusLayer::cbAlertDlg),
                                 m_awardDsp.c_str(),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    
    (m_pListener->*m_pfnSelector)(this);
}

void RwdBonusLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    m_BonusList->clearAll();
    showBonusList();
}




