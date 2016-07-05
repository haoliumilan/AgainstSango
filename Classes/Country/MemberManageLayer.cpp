//
//  MemberManageLayer.cpp
//  AgainstWar
//
//  Created by liang jun on 13-04-25.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "MemberManageLayer.h"
#include "CMenuBtn.h"
#include "TextNode.h"
#include "MainScene.h"

#define MEMBERTAGDELTA   999

MemberManageLayer::MemberManageLayer(CCObject* pListener, SEL_CallFuncO pfnSelector)
{
    m_pListener = pListener;
    m_pfnSelector = pfnSelector;
    
    m_commDlg = NULL;
    m_list = NULL;
    
    m_iUid = 0;
    m_on_off = true;
    m_bPingbi = false;
    showMemberList();
}


MemberManageLayer::~MemberManageLayer()
{

}

void MemberManageLayer::onEnter()
{
    CCLayer::onEnter();
    
    m_commDlg = NULL;
}

void MemberManageLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}

void MemberManageLayer::showMemberList()
{
    m_list = new CListLayer(this, callfuncO_selector(MemberManageLayer::cbList), 4, 145.0, 660.0);
    addChild(m_list, -1);
    m_list->release();
    
    for (int i=0; i<CGameData::Inst()->getMemberList()->count(); i++)
    {
        showMemberItem(i);
    }
    
    CCSprite* frNumBg = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    addChild(frNumBg, 1);
    frNumBg->setPosition(CCPointMake(535, 167));
    
    char buf[100];
    sprintf(buf, "%d/%d", CGameData::Inst()->getCntryInfo()->member_cur_num,
            CGameData::Inst()->getCntryInfo()->member_max_num);
    TextNode* frInfoLabel = TextNode::textWithString(buf, 24);
    frInfoLabel->setPosition(ccp(120, 18));
    frNumBg->addChild(frInfoLabel);
}


void MemberManageLayer::showMemberItem(int index)
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(index);
    CMenuBtn* newMenuBtn = new CMenuBtn(memberInfo,true);
    m_list->push_back(newMenuBtn);
    newMenuBtn->setTag(MEMBERTAGDELTA+index);
    newMenuBtn->release();
    
    const char* userId = CGameData::Inst()->getUsrInfo()->uid;
    const char* memId = memberInfo->mid;
    if (strcmp(userId, memId) == 0)
    {
        if (!memberInfo->VoiceOn)
        {
            CCSprite* jinyanSp = CCSprite::spriteWithFile("jinyan.png");
            jinyanSp->setAnchorPoint(CCPointZero);
            jinyanSp->setPosition(ccp(138,-54));
            newMenuBtn->addChild(jinyanSp);
        }
        
        return;
    }
    
    //增加禁言按钮
    
    if (CGameData::Inst()->getCntryInfo()->status==1
        ||(CGameData::Inst()->getCntryInfo()->status ==2 && memberInfo->status == 3)
        )
    {
        CCMenuItemImage* btnIMG1 = CCMenuItemImage::itemFromNormalImage("jinyan.png", "jinyan.png", NULL, NULL);
        CCMenuItemImage* btnIMG2 = CCMenuItemImage::itemFromNormalImage("nojinyan.png", "nojinyan.png", NULL, NULL);
        
        CCMenuItemToggle* btnTog = CCMenuItemToggle::itemWithTarget(this, menu_selector(MemberManageLayer::JinYanControl), btnIMG1,btnIMG2,NULL);
        
        if (memberInfo->VoiceOn)
        {
            btnTog->setSelectedIndex(1);
        }
        else
        {
            btnTog->setSelectedIndex(0);
        }
        
        
        btnTog->setAnchorPoint(CCPointZero);
        btnTog->setPosition(ccp(138,-54));
        btnTog->setTag(MEMBERTAGDELTA);
        CCMenu* menu1 = CCMenu::menuWithItem(btnTog);
        menu1->setAnchorPoint(CCPointZero);
        menu1->setPosition(CCPointZero);
        newMenuBtn->addChild(menu1,0,MEMBERTAGDELTA);
    }
    
    //增加屏蔽按钮
    
    CCMenuItemImage* btnIMG3 = CCMenuItemImage::itemFromNormalImage("pingbi.png", "pingbi.png", NULL, NULL);
    CCMenuItemImage* btnIMG4 = CCMenuItemImage::itemFromNormalImage("nopingbi.png", "nopingbi.png", NULL, NULL);
    CCMenuItemToggle* btnTog2 = NULL;
    btnTog2 = CCMenuItemToggle::itemWithTarget(this, menu_selector(MemberManageLayer::PingBiControl), btnIMG3,btnIMG4,NULL);
    
    
    if (memberInfo->bePingbi)
    {
        btnTog2->setSelectedIndex(0);
    }
    else
    {
        btnTog2->setSelectedIndex(1);
    }
    
    btnTog2->setAnchorPoint(CCPointZero);
    btnTog2->setPosition(ccp(210,-54));
    btnTog2->setTag(MEMBERTAGDELTA);
    CCMenu* menu2 = CCMenu::menuWithItem(btnTog2);
    menu2->setAnchorPoint(CCPointZero);
    menu2->setPosition(CCPointZero);
    newMenuBtn->addChild(menu2,0,MEMBERTAGDELTA+1);
}


void MemberManageLayer::JinYanControl(CCObject* pSender)
{
    CMenuBtn* MenuIMG = (CMenuBtn*)(((CCMenuItemToggle*)pSender)->getParent()->getParent());
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(MenuIMG->getTag()-MEMBERTAGDELTA);
    
    m_iUid = MenuIMG->getTag()- MEMBERTAGDELTA;
    
    if (memberInfo->VoiceOn)
    {
        m_on_off = false;
        if (CGameData::Inst()->setUserVoiceOnOff(memberInfo->mid, false))
        {
            m_curReqType = enCReqTypeVoice;
            scheduleUpdate();
        }
        
    }
    else
    {
        m_on_off = true;
        if (CGameData::Inst()->setUserVoiceOnOff(memberInfo->mid, true))
        {
            m_curReqType = enCReqTypeVoice;
            scheduleUpdate();
        }
    }
}

void MemberManageLayer::PingBiControl(CCObject* pSender)
{
    CMenuBtn* MenuIMG = (CMenuBtn*)(((CCMenuItemToggle*)pSender)->getParent()->getParent());
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(MenuIMG->getTag()-MEMBERTAGDELTA);
    
    m_iUid = MenuIMG->getTag()-MEMBERTAGDELTA;
    
    
    if (memberInfo->bePingbi)
    {
        m_bPingbi = false;
        if (CGameData::Inst()->changeMemberPingbi(memberInfo->mid, m_bPingbi))
        {
            m_curReqType = enCReqTypePingbi;
            scheduleUpdate();
        }
    }
    else
    {
        m_bPingbi = true;
        if (CGameData::Inst()->changeMemberPingbi(memberInfo->mid, m_bPingbi))
        {
            m_curReqType = enCReqTypePingbi;
            scheduleUpdate();
        }
    }
}


void MemberManageLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(MemberManageLayer::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
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
        switch (m_curReqType)
        {
            case enCReqTypeVoice:
            {
                CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_iUid);
                memberInfo->VoiceOn = m_on_off;
            }
                break;
                
            case enCReqTypePingbi:
            {
                CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_iUid);
                memberInfo->bePingbi = m_bPingbi;
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        if( m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MemberManageLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


void MemberManageLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL)
    {
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true)
    {
        switch (m_curReqType)
        {
            case enCReqTypeVoice:
            {
                CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_iUid);
                if (CGameData::Inst()->setUserVoiceOnOff(memberInfo->mid, m_on_off))
                {
                    m_curReqType = enCReqTypeVoice;
                    scheduleUpdate();
                }
            }
                break;
                
            case enCReqTypePingbi:
            {
                CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_iUid);
                if (CGameData::Inst()->setUserVoiceOnOff(memberInfo->mid, m_bPingbi))
                {
                    m_curReqType = enCReqTypePingbi;
                    scheduleUpdate();
                }
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch (m_curReqType)
        {
            case enCReqTypeVoice:
            {
                CCMenuItemToggle* MenuTog = (CCMenuItemToggle*)(m_list->getChildByTag(m_iUid+MEMBERTAGDELTA)->getChildByTag(MEMBERTAGDELTA)->getChildByTag(MEMBERTAGDELTA));
                MenuTog->setSelectedIndex(MenuTog->getSelectedIndex()==0?1:0);
            }
                break;
                
            case enCReqTypePingbi:
            {
                CCMenuItemToggle* MenuTog = (CCMenuItemToggle*)(m_list->getChildByTag(m_iUid+MEMBERTAGDELTA)->getChildByTag(MEMBERTAGDELTA+1)->getChildByTag(MEMBERTAGDELTA));
                MenuTog->setSelectedIndex(MenuTog->getSelectedIndex()==0?1:0);
            }
                break;
                
            default:
                break;
        }

        CGameData::Inst()->clearReqStat();
    }
}


void MemberManageLayer::showContributionItem()
{
    CMenuBtn* btn = new CMenuBtn("ma_button.png",
                                 CGameData::Inst()->getLanguageValue("cntry_assign_contribution"),
                                 CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
}

void MemberManageLayer::showAverageItem()
{
    CMenuBtn* btn = new CMenuBtn("ma_button.png",
                                 CGameData::Inst()->getLanguageValue("cntry_deliver_equ"),
                                 CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
}


void MemberManageLayer::cbList(cocos2d::CCObject *pObj)
{

}





