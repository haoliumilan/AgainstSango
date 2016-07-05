//
//  OrgRankList.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-20.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgRankList.h"
#include "TextNode.h"
#include "MainScene.h"
#include "CGameData.h"

OrgRankList::OrgRankList(string dg_type)
{
    m_dgtype = dg_type;
}

OrgRankList::~OrgRankList()
{

}

void OrgRankList::onEnter()
{
    CCLayer::onEnter();
    
    m_spScrollBg = NULL;
    m_spScrollBar = NULL;
    m_fBarPos0 = 195;
    m_fBarPos100 = 83;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    if (m_dgtype == "1" || m_dgtype == "2")
    {
        showSpecialDgRank(m_dgtype);
    }
    else
    {
        showCntryDgRank();
    }

}

string OrgRankList::getDgType()
{
    return m_dgtype;
}

void OrgRankList::showCntryDgRank()
{
    char buf[100];
    
    CCMutableArray<CCntryDgRankInfo* > * pRandLst = CGameData::Inst()->getCntryDgRankInfArr();
    int iItemCnt = pRandLst->count();

    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    for (int i = 0; i < iItemCnt; i++) {
        CCntryDgRankInfo* pInf = pRandLst->getObjectAtIndex(i);
        
        bool bIsMe = (pInf->strUid.compare(pUsrInf->uid) == 0);
        
        snprintf(buf, 99, "%d", i+1);
        CCLabelTTF* text = CCLabelTTF::labelWithString(buf, "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(30, 296 - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
        
        text = CCLabelTTF::labelWithString(pInf->strName.c_str(), "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(190, 296 - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
        
        snprintf(buf, 99, "%ld", pInf->lDamage);
        text = CCLabelTTF::labelWithString(buf, "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(426, 296 - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
    }
    
    
    string lastKillName = CGameData::Inst()->getCntryBossLastKillName();
    if (!lastKillName.empty())
    {
        CCLabelTTF* text1 = CCLabelTTF::labelWithString(lastKillName.c_str(), "STHeitiSC-Medium", 20);
        text1->setPositionInPixels(CCPointMake(30, 296 - iItemCnt * 22 - 10));
        text1->setAnchorPoint(ccp(0, 0.5));
        addChild(text1);
        text1->setColor(ccRED);
        CCLabelTTF* text2 = CCLabelTTF::labelWithString(CGameData::Inst()->getLanguageValue("orgft_last_kill"), "STHeitiSC-Medium", 20);
        text2->setPositionInPixels(CCPointMake(30 + text1->getContentSize().width + 6, text1->getPosition().y));
        text2->setAnchorPoint(ccp(0, 0.5));
        addChild(text2);
        
        iItemCnt += 2;
    }
    
    
    if (iItemCnt > 6) {
        m_fLimitY = (iItemCnt - 7) * 22 + 10;
        m_fBarBgPosY = 304;
        
        m_spScrollBg = CCSprite::spriteWithFile("rankscrollbg.png");
        if (m_spScrollBg) {
            addChild(m_spScrollBg);
            m_spScrollBg->setAnchorPoint(ccp(0, 1));
            m_spScrollBg->setPositionInPixels(ccp(600, m_fBarBgPosY));
            m_spScrollBg->setScaleY(0.72);
            
            m_spScrollBar = CCSprite::spriteWithFile("rankscrollbar.png");
            if (m_spScrollBar) {
                m_spScrollBg->addChild(m_spScrollBar);
                m_spScrollBar->setAnchorPoint(ccp(0, 1));
                m_spScrollBar->setPositionInPixels(ccp(5, m_fBarPos0));
            }
        }
    }
    else{
        m_fLimitY = 0;
    }
}

void OrgRankList::showSpecialDgRank(string dgType)
{
    float fPosY = 30.0;
    char buf[100];
    
    CCMutableArray<CSpecialDgRankInfo* > * pRandLst = CGameData::Inst()->getSpecialDgRankInfArr(dgType);
    int iItemCnt = pRandLst->count();
            
    
    
    int myRank = CGameData::Inst()->getMySpecialDgRank(dgType);
    bool bAddMyRank = false;
    if (myRank > iItemCnt)
    {
        iItemCnt++;
        bAddMyRank = true;
    }
    CCountryInfo* pCntryInfo = CGameData::Inst()->getCntryInfo();
    for (int i = 0; i < iItemCnt; i++) {
        CSpecialDgRankInfo* pInf = NULL;
        if (bAddMyRank && i == iItemCnt - 1) {
            snprintf(buf, 99, "%d", myRank);
            pInf = CGameData::Inst()->getMySpecialDgRankInf(dgType);
        }
        else
        {
            snprintf(buf, 99, "%d", i+1);
            pInf = pRandLst->getObjectAtIndex(i);
        }
        
        bool bIsMe = (pInf->strCid.compare(pCntryInfo->cid) == 0);
        
        CCLabelTTF* text = CCLabelTTF::labelWithString(buf, "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(30, 296 - fPosY - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
        
        text = CCLabelTTF::labelWithString(pInf->strName.c_str(), "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(190, 296 - fPosY - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
        
        text = CCLabelTTF::labelWithString(pInf->strTime.c_str(), "STHeitiSC-Medium", 20);
        text->setPositionInPixels(CCPointMake(426, 296 - fPosY - i * 22));
        text->setAnchorPoint(ccp(0, 0.5));
        if(bIsMe)
            text->setColor(ccc3(249, 196, 15));
        else
            text->setColor(ccc3(255, 255, 255));
        addChild(text);
    }
        
    if (iItemCnt > 5) {
        m_fLimitY = (iItemCnt - 6) * 22 + 16;
        m_fBarBgPosY = 276;
        
        m_spScrollBg = CCSprite::spriteWithFile("rankscrollbg.png");
        if (m_spScrollBg) {
            addChild(m_spScrollBg);
            m_spScrollBg->setAnchorPoint(ccp(0, 1));
            m_spScrollBg->setPositionInPixels(ccp(600, m_fBarBgPosY));
            m_spScrollBg->setScaleY(0.58);
            
            m_spScrollBar = CCSprite::spriteWithFile("rankscrollbar.png");
            if (m_spScrollBar) {
                m_spScrollBg->addChild(m_spScrollBar);
                m_spScrollBar->setAnchorPoint(ccp(0, 1));
                m_spScrollBar->setPositionInPixels(ccp(5, m_fBarPos0));
            }
        }
    }
    else{
        m_fLimitY = 0;
    }
}

void OrgRankList::visit()
{
    float fHeightValue = 74.0;
    if (m_dgtype == "1"
        || m_dgtype == "2")
        fHeightValue = 60.0;
    
    float fFactor = 1.0;
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    if(winsz.width > 1000){
        fFactor = 4.0;
    }
    else if(winsz.width == 768){
        fFactor = 2.0;
    }
    else if(winsz.width == 640){
        fFactor = 2.0;
    }

    CCPoint pos = MainScene::Inst()->getPositionInPixels();
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(10 * fFactor + pos.x,
              80 * fFactor + pos.y,
              300  * fFactor,
              fHeightValue  * fFactor);
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
}

bool OrgRankList::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(OrgRankList::inertiaMove));
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptStartNode = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    m_lastYm = 0;
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(20, 164, 600, 150),
                                            ccp(m_ptStartNode.x, m_ptStartNode.y + this->getPositionInPixels().y));
    
    if (!bRet) {
        return false;
    }
    
    m_ptStart = touchLocation;
    m_fThisY = this->getPositionInPixels().y;
    
    return true;
}

void OrgRankList::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    
    float fOffsetY = pos.y - m_ptStartNode.y;
    CCPoint thisPt = this->getPosition();
    thisPt.y += fOffsetY;
    setPosition(thisPt);
}

void OrgRankList::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        m_lastYm *= 1.5;
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        schedule(schedule_selector(OrgRankList::inertiaMove));
        return;
    }
}

void OrgRankList::inertiaMove(float dt)
{
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        
        unschedule(schedule_selector(OrgRankList::inertiaMove));
    }
}

void OrgRankList::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    setPosition(thisPt);
}

void OrgRankList::setPosition(const CCPoint& pos)
{
    CCPoint pt = pos;
    pt.y = clampf(pos.y, 0, m_fLimitY);
    
    CCNode::setPosition(pt);
    
    if (m_spScrollBg){
        m_spScrollBg->setPositionInPixels(ccp(600, m_fBarBgPosY - pt.y));
        
        float fPercent = pt.y / m_fLimitY;
        float fBarPos = m_fBarPos0 - (m_fBarPos0 - m_fBarPos100) * fPercent;
        m_spScrollBar->setPositionInPixels(ccp(5, fBarPos));
    }
}




