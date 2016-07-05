//
//  RwdHuodongLayer.cpp
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RwdHuodongLayer.h"

RwdHuodongLayer::RwdHuodongLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgAlert = NULL;
    m_commDlg = NULL;
    m_background = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_BonusList = new CListLayer(this, callfuncO_selector(RwdHuodongLayer::cbList),1.2,460,525);
    addChild(m_BonusList, -1);
    m_BonusList->release();
    m_BonusList->setReloadFunc(callfuncO_selector(RwdHuodongLayer::insertItemByIdx));
    
    showBonusList();
}

RwdHuodongLayer::~RwdHuodongLayer()
{
    
}

void RwdHuodongLayer::insertItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<RwdHuodongInf*> * pHuodongInfArr = CGameData::Inst()->getRwdHuodongInfArr();
    
    if(pHuodongInfArr->count() == 0)
        return;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(pHuodongInfArr->getObjectAtIndex(i));
    
    m_BonusList->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(RwdHuodongLayer::cbGetRwdItem));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    itemShow->setPosition(ccp(90,-100));
    
    char buf[100];
    CCPoint pt;
    if (pHuodongInfArr->getObjectAtIndex(i)->bCanGet) {
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqued"));
        pt = CCPointMake(40,14);
    }else{
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
        pt = CCPointMake(50, 14);
    }
    TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(pt);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    itemShow->addChild(lbText, 1);
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
}

void RwdHuodongLayer::showAlertNoTaskDlg()
{
    CCPoint point ;
    m_background = CCSprite::spriteWithFile("fr_window.png");
    m_background->setScale(2.0);
    addChild(m_background);
    point.x = 320;
    point.y = 480;
    m_background->setPosition(point);
    
    TextNode* infoLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("no_activity"), 30);
    infoLabel->setColor(ccc3(122, 37, 8));
    addChild(infoLabel);
    infoLabel->setPosition(point);
}

void RwdHuodongLayer::showBonusList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<RwdHuodongInf*> * pHuodongInfArr = CGameData::Inst()->getRwdHuodongInfArr();
    if (pHuodongInfArr->count()<1) {
        showAlertNoTaskDlg();
        return;
    }
    for (int i = 0; i < pHuodongInfArr->count(); i++)
    {
        if (i >= m_BonusList->getPreTopShow()
            && i <= m_BonusList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pHuodongInfArr->getObjectAtIndex(i));
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(RwdHuodongLayer::cbGetRwdItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(90,-100));
            
            char buf[100];
            CCPoint pt;
            if (pHuodongInfArr->getObjectAtIndex(i)->bCanGet) {
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqued"));
                pt = CCPointMake(40,14);
            }else{
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("rwd_sign_lingqu"));
                pt = CCPointMake(50, 14);
            }
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            lbText->setAnchorPoint(CCPointZero);
            lbText->setPosition(pt);
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

void RwdHuodongLayer::cbList(CCObject* pObj)
{
    
}

void RwdHuodongLayer::cbGetRwdItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_iListSel = mItem->getTag() / 10;
    
    GetRwdItem();
}

void RwdHuodongLayer::GetRwdItem()
{
    if (CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->bCanGet)
        return;
    if (CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->loginCounts < CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->awardDays)
        return;
    
    char buf[10];
    snprintf(buf, 9, "%d",CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->awardDays);
    if (CGameData::Inst()->reqGetAwardBonus(CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->awardType, CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->strDate, buf))
    {
        m_awardDsp = CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(m_iListSel)->awardDsp;
        scheduleUpdate();
    }
}


void RwdHuodongLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RwdHuodongLayer::cbCommDlg),
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
    
    if(comState == kReqStatOk)
    {
        showAlertGetSuccessDlg();
        
        //如果有card 则更新all card
        if (CCMutableArray<RwdHuodongInf*> *pHuodongInfArr = CGameData::Inst()->getRwdHuodongInfArr())
        {
            for (int i = 0; i < pHuodongInfArr->count(); i++)
            {
                if (RwdHuodongInf *pRwdHuodongInf = pHuodongInfArr->getObjectAtIndex(i))
                {
                    if (0<(int)pRwdHuodongInf->WujiangCardList.size())
                    {
                        CGameData::Inst()->allCardRequest();
                        break;
                    }
                }
            }
        }
         MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    else
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RwdHuodongLayer::cbCommDlg), enCommDlgTp_retry);
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
void RwdHuodongLayer::cbCommDlg(CCObject* pObj)
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


void RwdHuodongLayer::showAlertGetSuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(RwdHuodongLayer::cbAlertDlg),
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

void RwdHuodongLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    m_BonusList->clearAll();
    showBonusList();
}


