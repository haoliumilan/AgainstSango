//
//  RwdLevelUpLayer.cpp
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RwdLevelUpLayer.h"
#include "OcProxy.h"

RwdLevelUpLayer::RwdLevelUpLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgAlert = NULL;
    m_commDlg = NULL;
    
    m_dlgNoTip = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_LevelUpList = new CListLayer(this, callfuncO_selector(RwdLevelUpLayer::cbList),4,144,650);
    addChild(m_LevelUpList, -1);
    m_LevelUpList->release();
    m_LevelUpList->setReloadFunc(callfuncO_selector(RwdLevelUpLayer::insertItemByIdx));
    
    showLevelUpList();
}

RwdLevelUpLayer::~RwdLevelUpLayer()
{
    
}

void RwdLevelUpLayer::insertItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<RwdLevelUpInf*> * pLevelUpInfArr = CGameData::Inst()->getRwdLevelUpArr();
    
    if(pLevelUpInfArr->count() == 0)
        return;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(pLevelUpInfArr->getObjectAtIndex(i));
    
    m_LevelUpList->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(RwdLevelUpLayer::cbGetRwdItem));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    itemShow->setPosition(ccp(90,-58));
    
    char buf[100];
    if (pLevelUpInfArr->getObjectAtIndex(i)->needShare)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("share_and_lingqu"));
    else
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
    TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
    if (pLevelUpInfArr->getObjectAtIndex(i)->needShare) {
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

void RwdLevelUpLayer::showLevelUpList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<RwdLevelUpInf*> * pLevelUpInfArr = CGameData::Inst()->getRwdLevelUpArr();
    
    for (int i = 0; i < pLevelUpInfArr->count(); i++)
    {
        if (i >= m_LevelUpList->getPreTopShow()
            && i <= m_LevelUpList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pLevelUpInfArr->getObjectAtIndex(i));
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(RwdLevelUpLayer::cbGetRwdItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(90,-58));
            
            char buf[100];
            if (pLevelUpInfArr->getObjectAtIndex(i)->needShare)
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("share_and_lingqu"));
            else
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            if (pLevelUpInfArr->getObjectAtIndex(i)->needShare) {
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
        
        m_LevelUpList->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
    }

}

void RwdLevelUpLayer::cbList(CCObject* pObj)
{
    
}

void RwdLevelUpLayer::cbGetRwdItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_iListSel = mItem->getTag() / 10;
    
    GetRwdItem();
}

void RwdLevelUpLayer::GetRwdItem()
{
    if (CGameData::Inst()->getUsrInfo()->lv < CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(m_iListSel)->lv){
        
    }else{
        char buf[50];
        snprintf(buf, 49, "%hd",CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(m_iListSel)->lv);
        
        if(strcmp(OcProxy::Inst()->getValueForKey("platform"), "oc") == 0 && CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(m_iListSel)->needShare)
            showAlertNoCombineDlg();
        else{
            if (CGameData::Inst()->reqGetAwardBonus("5", string(buf)))
            {
                if (CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(m_iListSel)->needShare) {
                    OcProxy::Inst()->sendWeibo(CGameData::Inst()->getCommonInfo()->voice_1, false);
                }
                m_awardDsp = CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(m_iListSel)->awardDsp;
                scheduleUpdate();
            }
        }
    }
}

void RwdLevelUpLayer::showAlertNoCombineDlg()
{
    if (m_dlgNoTip == NULL)
    {
        m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(RwdLevelUpLayer::cbNoCombineDlg));
        addChild(m_dlgNoTip);
        m_dlgNoTip->release();
        m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("NoCombine_Dlg"));
        m_dlgNoTip->setPosition(CCPoint(320, 300));
        m_dlgNoTip->setScale(0.0);
        CCActionInterval* tempAction = CCScaleTo::actionWithDuration(0.5, 1.0);
        m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
}

void RwdLevelUpLayer::cbNoCombineDlg()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
}


void RwdLevelUpLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RwdLevelUpLayer::cbCommDlg),
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
            m_commDlg = new CommDlg(this, callfuncO_selector(RwdLevelUpLayer::cbCommDlg), enCommDlgTp_retry);
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
void RwdLevelUpLayer::cbCommDlg(CCObject* pObj)
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


void RwdLevelUpLayer::showAlertGetSuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(RwdLevelUpLayer::cbAlertDlg),
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

void RwdLevelUpLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    m_LevelUpList->clearAll();
    showLevelUpList();
    
}





