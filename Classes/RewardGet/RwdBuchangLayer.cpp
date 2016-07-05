//
//  RwdBuchangLayer.cpp
//  AgainstWar
//
//  Created by caojiye on 13-8-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RwdBuchangLayer.h"

RwdBuchangLayer::RwdBuchangLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_BuchangList = new CListLayer(this, callfuncO_selector(RwdBuchangLayer::cbList),4,144,630);
    addChild(m_BuchangList, -1);
    m_BuchangList->release();
    m_BuchangList->setReloadFunc(callfuncO_selector(RwdBuchangLayer::insertItemByIdx));
    
    showBuchangList();
}

void RwdBuchangLayer::insertItemByIdx(CCObject* pObj)
{
    
}

void RwdBuchangLayer::showBuchangList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<RwdBuchangInf*> * pRwdBuchangInf = CGameData::Inst()->getRwdBuchangInfArr();
    
    for (int i = 0; i < pRwdBuchangInf->count(); i++)
    {
        if (i >= m_BuchangList->getPreTopShow()
            && i <= m_BuchangList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pRwdBuchangInf->getObjectAtIndex(i));
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(RwdBuchangLayer::cbGetRwdItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(90,-58));
            
            char buf[100];
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            lbText->setAnchorPoint(CCPointZero);
            lbText->setPosition(CCPointMake(50,14));
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
        
        m_BuchangList->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
    }
    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
}

void RwdBuchangLayer::cbList(CCObject* pObj)
{
    
}

void RwdBuchangLayer::cbGetRwdItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_iListSel = mItem->getTag() / 10;
    
    GetRwdItem();
}

void RwdBuchangLayer::GetRwdItem()
{
    if (!CGameData::Inst()->getRwdBuchangInfArr()->getObjectAtIndex(m_iListSel)->bCanGet)
        return;
    
    if (CGameData::Inst()->reqGetAwardBonus("2", CGameData::Inst()->getRwdBuchangInfArr()->getObjectAtIndex(m_iListSel)->strDate))
    {
        scheduleUpdate();
    }
}


void RwdBuchangLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RwdBuchangLayer::cbCommDlg),
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
            m_commDlg = new CommDlg(this, callfuncO_selector(RwdBuchangLayer::cbCommDlg), enCommDlgTp_retry);
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
void RwdBuchangLayer::cbCommDlg(CCObject* pObj)
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


void RwdBuchangLayer::showAlertGetSuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(RwdBuchangLayer::cbAlertDlg),
                                 CGameData::Inst()->getRwdSignInfList()->getObjectAtIndex(m_iListSel)->awardDsp.c_str(),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    
    (m_pListener->*m_pfnSelector)(this);
}

void RwdBuchangLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    m_BuchangList->clearAll();
    showBuchangList();
}










