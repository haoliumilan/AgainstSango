//
//  OtherCardLayer.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "OtherCardLayer.h"
#include "CGamedata.h"
#include "CScrollBar.h"
#include "MainScene.h"
#include "CGeneralLayer.h"
#include "PackSelBtn.h"
#include "Dialog2.h"
#include "TextNode.h"
#include "ClctCard.h"

#define kCardTagBase    100

//******************************************************************************
// Constructor
//******************************************************************************
int OtherCardLayer::getCardTagBase()
{
    return kCardTagBase;
}

OtherCardLayer::OtherCardLayer(CCObject* target, SEL_CallFuncO selector)
{
    //callBack
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(OtherCardLayer::NtyGoTop),  // 处理的消息的回调函数
                                                           kNotifyGoTop,                    // 感兴趣的消息名称
                                                           NULL);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(OtherCardLayer::NtyGoBottom),  // 处理的消息的回调函数
                                                           kNotifyGoBottom,                    // 感兴趣的消息名称
                                                           NULL);
    
    m_pScrollBar = NULL;
    m_spMaxCard = NULL;
    m_lbMaxCard = NULL;
    fDownLimitY = 192.0;
    m_iPreThisPosY = 0;

    refreshShowState();

    if(m_fUpLimitY > 0)
    {
        m_pScrollBar = new CScrollBar();
        addChild(m_pScrollBar);
        m_pScrollBar->release();
        
        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
        m_pScrollBar->setBarSize(fBarSize);
        m_pScrollBar->setPercent(0.0);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435));
    }
 
    m_lastYm = 0;
}

OtherCardLayer::~OtherCardLayer()
{
}

void OtherCardLayer::refreshShowState()
{
    char* pColl = CGameData::Inst()->getCollection();

    CCSprite* sp = CGameData::Inst()->getHeadSprite(1);
    m_cardSize = sp->getContentSize();
    
    m_iColumn = 640 / m_cardSize.width;
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1);
    
    m_ptStart.x = m_iGap;
    m_ptStart.y = 720;
    
    CCPoint pt;
    
    for(int i = 0; i < kMaxCardNum; i++)
    {
        int state = *(pColl+1+i);

        ClctCard* card = new ClctCard(i+1, state);
        
        pt.x = m_ptStart.x + ((i ) % m_iColumn) * (m_iGap + m_cardSize.width);
        pt.y = m_ptStart.y - ((i ) / m_iColumn) * (m_iGap + m_cardSize.height);
        
        card->setPosition(pt);
        addChild(card, 1, kCardTagBase + i);
    }

    m_fUpLimitY = fDownLimitY - (pt.y - m_cardSize.height);
    
    setThisPosition(CCPointZero, true);
}
//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool OtherCardLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(OtherCardLayer::inertiaMove));
    m_lastYm = 0;
    m_bTouchMvFlg = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptStartNode = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    if((touchLocation.y > 720) || (touchLocation.y < 160))
        return false;
    else    
        return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void OtherCardLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    m_bTouchMvFlg = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;

    float fOffsetY = pos.y - m_ptStartNode.y;
    
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += fOffsetY;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0){ 
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435 - thisPt.y));
        
        redrawCapacity();
        
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void OtherCardLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
        
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        m_lastYm *= 1.5;
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        schedule(schedule_selector(OtherCardLayer::inertiaMove));
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = pos.x / (m_iGap + m_cardSize.width);
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
        
    int iSel = y * m_iColumn + x;

    char* pColl = CGameData::Inst()->getCollection();
    int state = *(pColl+1+iSel);
    if(state == 2 || state == 1) 
    {
        m_ret.iBtnSel = iSel;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }       

}

void OtherCardLayer::inertiaMove(float dt) {
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        
        unschedule(schedule_selector(OtherCardLayer::inertiaMove));
    }
}

void OtherCardLayer::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
     
    thisPt.y += dt;

    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0){ 
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435 - thisPt.y));
        
        redrawCapacity();        
    }
}

//******************************************************************************
// initCapacity
//******************************************************************************
void OtherCardLayer::initCapacity(char* buf)
{
    m_spMaxCard = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    m_spMaxCard->setPosition(CCPointMake(535, 167));
    addChild(m_spMaxCard, 10); 
    
    m_lbMaxCard = TextNode::textWithString(buf, CCSizeMake(200, 25), CCTextAlignmentCenter, 24);
    m_lbMaxCard->setPosition(m_spMaxCard->getPosition());
    m_lbMaxCard->setColor(ccWHITE);
    addChild(m_lbMaxCard, 12);
}

//******************************************************************************
// redrawCapacity
//******************************************************************************
void OtherCardLayer::redrawCapacity()
{
    if(m_spMaxCard == NULL)
        return;
    
    m_spMaxCard->setPosition(CCPointMake(535, 167 - this->getPosition().y));
    m_lbMaxCard->setPosition(m_spMaxCard->getPosition());
}

//******************************************************************************
// setThisPosition
//******************************************************************************
void OtherCardLayer::setThisPosition(const CCPoint& pt, bool bRedraw)
{
    this->setPosition(pt);
    
    //不强制要求重画，而且移动范围小于100
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pt.y) < 100))
        return;
    
    m_iPreThisPosY = pt.y;
    
    //改变m_pScrollBar位置
    //    CCLOG("m_pScrollBarY=%f",m_pScrollBarY);
    if (m_pScrollBar) {
        m_pScrollBar->setPosition(CCPointMake(630, 435 - pt.y));
        redrawCapacity();
        m_pScrollBar->setPercent(pt.y/m_fUpLimitY);
        redrawCapacity();
    }
    
    CCNode* node;
    for(int i = 0; i < kMaxCardNum; i++){
        node = getChildByTag(kCardTagBase + i);
        
        if(node){
            ClctCard* card = (ClctCard*) node;
            CCPoint ptCard = card->getPosition();
            
            if((ptCard.y + pt.y < 960) && (ptCard.y + pt.y > 0)){
                card->show();
            }
            else{
                card->hide();
            }
        }
    }
    
    //CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// NtyGoTop
//******************************************************************************
void OtherCardLayer::NtyGoTop(CCObject* obj)
{
    stopAllActions();
    setThisPosition(ccp(0,0));
}

//******************************************************************************
// NtyGoBottom
//******************************************************************************
void OtherCardLayer::NtyGoBottom(CCObject* obj)
{
    stopAllActions();
    setThisPosition(ccp(0,m_fUpLimitY));
}

//******************************************************************************
// onExit
//******************************************************************************
void OtherCardLayer::onExit()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoTop);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoBottom);
    
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

bool OtherCardLayer::isShowClctCard(int nIndex)
{
    if (kMaxCardNum<nIndex)return false;
    if (CCNode *pNode = getChildByTag(nIndex+kCardTagBase-1))
    {
        if (ClctCard *pClctCard = dynamic_cast<ClctCard*>(pNode))
        {
            return 0!=pClctCard->getStatus();
        }
    }
    return false;
}


