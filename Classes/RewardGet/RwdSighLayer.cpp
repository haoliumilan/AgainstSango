//
//  RwdSighLayer.cpp
//  AgainstWar
//
//  Created by user on 13-8-7.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RwdSighLayer.h"
#include "Arrow.h"

RwdSighLayer::RwdSighLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgAlert = NULL;
    m_commDlg = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_SighList = new CListLayer(this, callfuncO_selector(RwdSighLayer::cbList),4,144,650);
    addChild(m_SighList, -1);
    m_SighList->release();
    m_SighList->setReloadFunc(callfuncO_selector(RwdSighLayer::insertItemByIdx));
    
    showSighList();
    
    
    

    
    
}


RwdSighLayer::~RwdSighLayer()
{
    
}


void RwdSighLayer::cbList(CCObject* pObj)
{
    
}

void RwdSighLayer::insertItemByIdx(CCObject* pObj)
{

    
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<RwdSignInf*> * pRwdSignInfArr = CGameData::Inst()->getRwdSignInfList();
    
    if(pRwdSignInfArr->count() == 0)
        return;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(pRwdSignInfArr->getObjectAtIndex(i),(i == pRwdSignInfArr->count()-1) ? true:false);
    
    m_SighList->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(RwdSighLayer::cbGetRwdItem));
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
    
    //itemShow->setScale(0.8);
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
}

void RwdSighLayer::showSighList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<RwdSignInf*> * pRwdSignInfArr = CGameData::Inst()->getRwdSignInfList();
    
    for (int i = 0; i < pRwdSignInfArr->count(); i++)
    {
        if (i >= m_SighList->getPreTopShow()
            && i <= m_SighList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pRwdSignInfArr->getObjectAtIndex(i),(i == pRwdSignInfArr->count()-1) ? true:false);
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(RwdSighLayer::cbGetRwdItem));
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
            
            //itemShow->setScale(0.8);
            
            CCMenu *menu = CCMenu::menuWithItem(itemShow);
            newMenuBtn->addChild(menu);
            menu->setPosition(CCPointZero);
        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_SighList->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
    }
}

void RwdSighLayer::GetRwdItem()
{
    //引导
    if(MainScene::Inst()->new_MyNew  && !MainScene::Inst()->getChildByTag(99998))
    {
        

        
        
        return;
    }
    
    if (m_iListSel == (CGameData::Inst()->getRwdSignInfList()->count()-1)) {
        return;
    }else{
        if (CGameData::Inst()->reqGetAwardBonus("0", CGameData::Inst()->getRwdSignInfList()->getObjectAtIndex(m_iListSel)->strDate)){
            m_awardDsp = CGameData::Inst()->getRwdSignInfList()->getObjectAtIndex(m_iListSel)->awardDsp;
            scheduleUpdate();
        }
    }
}

void RwdSighLayer::cbGetRwdItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_iListSel = mItem->getTag() / 10;
    
    GetRwdItem();
}

void RwdSighLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RwdSighLayer::cbCommDlg),
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
    
    if(MainScene::Inst()->new_MyNew)
        MainScene::Inst()->enableBottomPanel(false);
    
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
            m_commDlg = new CommDlg(this, callfuncO_selector(RwdSighLayer::cbCommDlg), enCommDlgTp_retry);
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
void RwdSighLayer::cbCommDlg(CCObject* pObj)
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


void RwdSighLayer::showAlertGetSuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(RwdSighLayer::cbAlertDlg),
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

void RwdSighLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    m_SighList->clearAll();
    showSighList();
   
    
    //引导
    if(MainScene::Inst()->new_MyNew )
    {
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(411, 765));
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000,  99997);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
        
        
        (m_pListener->*m_pfnSelector)(this);
        
    }
}





