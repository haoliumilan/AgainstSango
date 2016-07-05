//
//  PagingListLayer.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-29.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "PagingListLayer.h"
#include "CListLayer.h"
#include "CScrollBar.h"
#include "CMenuBtn.h"
#include "TextNode.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define PList_MaxNum   20  // paginglist 每页最多项数

#define CUR_PAGE "currpage"

#define PListBtn_PrevPage   3001    // 上一页
#define PListBtn_NextPage   3002    // 下一页

PagingListLayer::PagingListLayer(CCObject* pListener, SEL_CallFuncO pfnSelector)
{
    m_pListener = pListener;
    m_pfnSelector = pfnSelector;
    
    m_curPage = 0;
    m_maxLines = 0;
    m_list = NULL;
    m_pagingMenu = NULL;
    
    positionY = 0;
}


PagingListLayer::~PagingListLayer()
{

}

void PagingListLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void PagingListLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}

void PagingListLayer::showList(enMemberListType memListType,int localY,int mListSel)
{
    m_memListType = memListType;
    if (mListSel>0) {
        m_curPage = mListSel/PList_MaxNum;
    }
    positionY = localY;
    switch (m_memListType) {
        case enMemberListType_normal:
            showMemberList();
            break;
            
        case enMemberListType_assign:
            showAssignSupplyList();
            break;
            
        case enMemberListType_reward:
            showRewardMemberList();
            break;
            
        default:
            break;
    }
}

void PagingListLayer::showMemberList()
{
    m_list = new CListLayer(this, callfuncO_selector(PagingListLayer::cbList),
                            4, 145.0, 660.0);
    addChild(m_list, -1);
    m_list->release();
    
    m_maxLines = CGameData::Inst()->getMemberList()->count();
    
    CCSprite* frNumBg = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    addChild(frNumBg, 1);
    frNumBg->setPosition(CCPointMake(535, 167));
    
    char buf[100];
    sprintf(buf, "%d/%d", CGameData::Inst()->getCntryInfo()->member_cur_num,
            CGameData::Inst()->getCntryInfo()->member_max_num);
    TextNode* frInfoLabel = TextNode::textWithString(buf, 24);
    frInfoLabel->setPosition(ccp(120, 18));
    frNumBg->addChild(frInfoLabel);
    
    updatePaingList();
}

void PagingListLayer::changeListRange()
{
    m_curPage = 0;
    updatePaingList();
}

void PagingListLayer::showAssignSupplyList()
{
    m_list = new CListLayer(this, callfuncO_selector(PagingListLayer::cbList),
                             4, 145.0, 660.0);
    addChild(m_list, -1);
    m_list->release();
    
//    m_maxLines = CGameData::Inst()->getMemberList()->count() + 2;
    m_maxLines = CGameData::Inst()->getMemberList()->count() + 3;
    
    CCSprite* frNumBg = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    addChild(frNumBg, 1);
    frNumBg->setPosition(CCPointMake(535, 167));
    
    char buf[100];
    sprintf(buf, "%d/%d", CGameData::Inst()->getCntryInfo()->member_cur_num,
            CGameData::Inst()->getCntryInfo()->member_max_num);
    TextNode* frInfoLabel = TextNode::textWithString(buf, 24);
    frInfoLabel->setPosition(ccp(120, 18));
    frNumBg->addChild(frInfoLabel);
    
    updatePaingList();
}

void PagingListLayer::showRewardMemberList()
{
    m_list = new CListLayer(this, callfuncO_selector(PagingListLayer::cbList),
                            4, 145.0, 660.0);
    addChild(m_list, -1);
    m_list->release();
    
    m_maxLines = CGameData::Inst()->getRewardMemberList()->count();
    
    updatePaingList();

}

void PagingListLayer::showContributionItem()
{
    CMenuBtn* btn = new CMenuBtn("ma_button.png",
                                 CGameData::Inst()->getLanguageValue("cntry_assign_contribution"),
                                 CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
}

void PagingListLayer::showAverageItem()
{
    CMenuBtn* btn = new CMenuBtn("ma_button.png",
                                 CGameData::Inst()->getLanguageValue("cntry_deliver_equ"),
                                 CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
}

void PagingListLayer::showWeekItem()
{
    CMenuBtn* btn = new CMenuBtn("ma_button.png",
                                 CGameData::Inst()->getLanguageValue("cntry_week_allocation"),
                                 CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
}

void PagingListLayer::showAssignItem(int index)
{
//    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(index - 2);
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(index-3);
    CMenuBtn* newMenuBtn = new CMenuBtn(memberInfo);
    m_list->push_back(newMenuBtn);
    newMenuBtn->release();
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("button_assign.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("button_assign.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(PagingListLayer::btnCallback));
    mItemOK->setTag(index);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(130, -50));
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
}

void PagingListLayer::showMemberItem(int index)
{
    CMenuBtn* newMenuBtn = NULL;
    if (m_memListType == enMemberListType_reward)
    {
        CRewardMember* memberInfo = CGameData::Inst()->getRewardMemberList()->getObjectAtIndex(index);
        newMenuBtn = new CMenuBtn(memberInfo);
    }
    else
    {
        CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(index);
        newMenuBtn = new CMenuBtn(memberInfo);
    }
    m_list->push_back(newMenuBtn);
    newMenuBtn->release();    
}

void PagingListLayer::updatePaingList()
{
    if (m_list) {
        m_list->clearAll();
    }
    int page = CCUserDefault::sharedUserDefault()->getIntegerForKey(CUR_PAGE, 0);
    if (page>0) {
        CCUserDefault::sharedUserDefault()->setIntegerForKey(CUR_PAGE, 0);
        m_curPage = page;
    }
    int curPageStart = PList_MaxNum * m_curPage;   // 当前页起始项的序数
    int curPageEnd = curPageStart + PList_MaxNum - 1;    // 当前页结束项的序数
    if (m_maxLines - curPageStart < PList_MaxNum)
        curPageEnd = curPageStart + (m_maxLines - curPageStart) % PList_MaxNum -1;
    
    for (int i = curPageStart; i <= curPageEnd; i++) {
        if (m_memListType == enMemberListType_assign)
        {
            if (i == 0)
                showAverageItem();
            else if (i == 1)
                showContributionItem();
            else if (2==i)
                showWeekItem();
            else
                showAssignItem(i);
        }
        else
            showMemberItem(i);
    }
    
    if (m_maxLines > PList_MaxNum) {
        CCSprite* spPaging = CCSprite::spriteWithSpriteFrameName("ma_button.png");
        spPaging->setOpacity(0);
        m_list->push_back(spPaging);
        
        CCSprite* spTemp = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        
        float btnPositionY = spPaging->getContentSize().height * 0.5 - spTemp->getContentSize().height * 0.5;
        float btnPositionX1 = spPaging->getContentSize().width * 0.25 - spTemp->getContentSize().width * 0.5;
        float btnPositionX2 = spPaging->getContentSize().width * 0.75 - spTemp->getContentSize().width * 0.5;
        
        CCMenuItem *mItemPrev;
        newBtn(mItemPrev, CGameData::Inst()->getLanguageValue("cntry_prev_page"), CCPointMake(btnPositionX1, btnPositionY), PListBtn_PrevPage);
        
        if (m_curPage == 0) {
            CCSprite* spCover = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
            mItemPrev->addChild(spCover);
            spCover->setPosition(CCPointMake(spTemp->getContentSize().width * 0.5,
                                             spTemp->getContentSize().height * 0.5));
        }
        
        CCMenuItem *mItemNext;
        newBtn(mItemNext, CGameData::Inst()->getLanguageValue("cntry_next_page"), CCPointMake(btnPositionX2, btnPositionY), PListBtn_NextPage);
        
        if (m_curPage == m_maxLines / PList_MaxNum) {
            CCSprite* spCover = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
            mItemNext->addChild(spCover);
            spCover->setPosition(CCPointMake(spTemp->getContentSize().width * 0.5,
                                             spTemp->getContentSize().height * 0.5));
        }
        
        m_pagingMenu = CCMenu::menuWithItems(mItemPrev, mItemNext, NULL);
        m_pagingMenu->setPosition(CCPointZero);
        spPaging->addChild(m_pagingMenu);
        
        m_list->setPosition(CCPointMake(640, positionY));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, positionY));
        m_list->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        positionY = 0;
    }
}

void PagingListLayer::pagingToPrev()
{
    if (m_curPage > 0) {
        m_curPage--;
        updatePaingList();
    }
}

void PagingListLayer::pagingToNext()
{
    if (m_curPage < m_maxLines / PList_MaxNum)
    {
        m_curPage++;
        updatePaingList();
    }
}

void PagingListLayer::showCountryDetailInfo(CCObject* pObj)
{

}

void PagingListLayer::cbList(cocos2d::CCObject *pObj)
{
    
    if (m_memListType == enMemberListType_reward)
        return;
    
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    m_ret.iBtnSel = pRet->iBtnSel + m_curPage * PList_MaxNum;
    m_ret.pLocal = m_list->getPosition().y;
    
    if (pRet->iBtnSel == PList_MaxNum || m_ret.iBtnSel == m_maxLines)
        return;
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void PagingListLayer::btnCallback(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* pRet = (CCMenuItem*)pObj;
    switch (pRet->getTag()) {
        case PListBtn_PrevPage:
            pagingToPrev();
            break;
        
        case PListBtn_NextPage:
            pagingToNext();
            break;
            
        default:
        {
            if (m_memListType == enMemberListType_assign)
            {
                m_ret.iBtnSel = pRet->getTag();
                m_ret.pLocal = m_list->getPosition().y;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }
        }
            break;
    }
}

void PagingListLayer::newBtn(CCMenuItem *&menuItem, const char *label,
                         const CCPoint& position, int tag)
{
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    menuItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                     spMenu2,
                                                     this,
                                                     menu_selector(PagingListLayer::btnCallback));
    menuItem->setTag(tag);
    menuItem->setAnchorPoint(CCPointZero);
    menuItem->setPosition(position);
    
    TextNode* tempText = TextNode::textWithString(label,
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  35);
    tempText->setShadowColor(ccBLACK);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    menuItem->addChild(tempText);
}



