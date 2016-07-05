//
//  CListLayer.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CListLayer.h"
#include "CScrollBar.h"
#include "MainScene.h"
#include "CMenuBtn.h"
#include "OcProxy.h"
//******************************************************************************
// Constructor
//******************************************************************************
CListLayer::CListLayer(CCObject* target, SEL_CallFuncO selector,
                       float cellShowCount, float cellHeight, float startHeight)
{
    m_fUpLimitY = 0;
    m_startHeight = startHeight;
    m_cellHeight = cellHeight;
    m_cellShowCount = cellShowCount;
    m_iItemCnt = 0;
    m_bTouchMvFlg = false;

    topShow = -2;
    bottomShow = 7;
    startTopShow = topShow;
    startBottomShow = bottomShow;
    
    m_pScrollBar = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_pfnSelectorBegin = NULL;
    m_pfnSelectorEnd = NULL;
    m_pfnSelectorRemove = NULL;
    
    m_pfnReload = NULL;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(CListLayer::NtyGoTop),  // 处理的消息的回调函数
                                                           kNotifyGoTop,                    // 感兴趣的消息名称
                                                           NULL);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(CListLayer::NtyGoBottom),  // 处理的消息的回调函数
                                                           kNotifyGoBottom,                    // 感兴趣的消息名称
                                                           NULL);
}


void CListLayer::addTBobserver()
{
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(CListLayer::NtyGoTop),  // 处理的消息的回调函数
                                                           kNotifyGoTop,                    // 感兴趣的消息名称
                                                           NULL);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(CListLayer::NtyGoBottom),  // 处理的消息的回调函数
                                                           kNotifyGoBottom,                    // 感兴趣的消息名称
                                                           NULL);
}

void CListLayer::removeTBobserver()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoTop);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoBottom);
}


void CListLayer::NtyGoTop(CCObject* pSender)
{
    if (getPosition().y < 0)
    {
        return;
    }
    
    stopAllActions();
    setPosition(ccp(0,0));
    ChkObjectShow();
}

void CListLayer::NtyGoBottom(CCObject* pSender)
{
    if (getPosition().y < 0)
    {
        return;
    }
    if (0>m_fUpLimitY)return;
    
    stopAllActions();
    setPosition(ccp(0,m_fUpLimitY));
    ChkObjectShow();
}

void CListLayer::setReloadFunc(SEL_CallFuncO selector)
{
    m_pfnReload = selector;
}

void CListLayer::setTouchBeginFunc(SEL_CallFuncO selector)
{
    m_pfnSelectorBegin = selector;
}

void CListLayer::setTouchEndFunc(SEL_CallFuncO selector)
{
    m_pfnSelectorEnd = selector;
}

void CListLayer::setTouchRemoveFunc(SEL_CallFuncO selector)
{
    m_pfnSelectorRemove = selector;
}

void CListLayer::reloadItemByIdx(CCNode* node, int idx)
{
    CCPoint pt(320, m_startHeight);
    pt.y += -(m_cellHeight * idx);
    this->addChild(node);
    node->setPosition(pt);
    node->setTag(idx+deltaTag);
}

int CListLayer::getPreTopShow()
{
    return topShow;
}

int CListLayer::getPreBottomShow()
{
    return bottomShow;
}

void CListLayer::setPreTopShow(int topshow)
{
    topShow = topshow;
}
void CListLayer::setPreBottomShow(int bottonshow)
{
    bottomShow = bottonshow;
}

int CListLayer::getMenuBtnIndex(int nTab2)
{
    for (int i = 0; i < m_iItemCnt; ++i)
    {
        if (CMenuBtn *pMenuBtn = dynamic_cast<CMenuBtn*>(getChildByTag(deltaTag+i)))
        {
            if (nTab2==pMenuBtn->getUITab2())
            {
                return i;
            }
        }
    }
    return -1;
}

//reset ListLayer
void CListLayer::list_reset()
{
    setPosition(ccp(0,0));
    topShow = -2;
    bottomShow = 7;
    startTopShow = topShow;
    startBottomShow = bottomShow;
}

void CListLayer::updatePosition()
{
    if (bottomShow - 2 > MAX(m_iItemCnt, 5))
    {   // 列表项数量减少了
        CCPoint pos = CCNode::getPosition();
        float fHeightVar = m_cellHeight * (bottomShow - 2 - m_iItemCnt);
        setPosition(CCPointMake(pos.x, pos.y - fHeightVar));
        bottomShow = m_iItemCnt + 2;
        ChkObjectShow();
    }    
}

void CListLayer::setPosition(const CCPoint& newPosition)
{
    CCNode::setPosition(newPosition);
    
    if (m_pScrollBar != NULL)
    {
        float fPercent = getPosition().y / m_fUpLimitY;
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(622, SCOROLLBARSTARTX-getPosition().y));
    }
}

//******************************************************************************
// push_back
//******************************************************************************
void CListLayer::push_back(CCNode* node)
{
    push_back(node, 0);
}
void CListLayer::push_back(CCNode* node, int z_order)
{
    if (node!=NULL)
    {
        this->addChild(node, z_order);
    
        CCPoint pt(320, m_startHeight);
        int iStep = -m_cellHeight;

        pt.y += iStep * m_iItemCnt;
        node->setPosition(pt);
        node->setTag(m_iItemCnt+deltaTag);
    }
    
    m_iItemCnt++;
    
//=========设置ScrollBar=====================================
    
    if (m_iItemCnt <= m_cellShowCount)
    {
        return;
    }
    
    if (m_pScrollBar == NULL)
    {
        m_pScrollBar = new CScrollBar();
        addChild(m_pScrollBar,1000);
        m_pScrollBar->setPosition(CCPointMake(622, 438));
    }
    
    if (m_pScrollBar)
    {
        m_pScrollBar->setPosition(CCPointMake(622, SCOROLLBARSTARTX-getPosition().y));
        
        float fBarSize = 1.0 / ((m_iItemCnt / m_cellShowCount) + 1);
        m_pScrollBar->setBarSize(fBarSize);//fBarSize);
        
        if(m_fUpLimitY > 0&&m_pScrollBar)
        {
            float fPercent = getPosition().y / m_fUpLimitY;
            m_pScrollBar->setPercent(fPercent);
        }
        
        m_fUpLimitY = (m_iItemCnt - m_cellShowCount) * m_cellHeight;
    }
}

void CListLayer::increaseUpLimitY(float f_offy)
{
    m_fUpLimitY+= f_offy;
}

//******************************************************************************
// clearAll
//******************************************************************************
void CListLayer::clearAll()
{
    NtyGoTop(NULL);
    
    removeAllChildrenWithCleanup(true);
    m_pScrollBar = NULL;
    m_iItemCnt = 0;
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CListLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    m_bTouchMvFlg = false;
     unschedule(schedule_selector(CListLayer::inertiaMove));
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    m_ptStartTouch = convertToNodeSpace( touchLocation );
    
    CCPoint ptOffset = ccpSub(CCPointMake(320, m_startHeight + m_cellHeight * 0.5), m_ptStartTouch);
    
    if((ptOffset.x > kMenuBtnWidth*0.5) || (ptOffset.x < -kMenuBtnWidth*0.5))
        return false;
    
    if((ptOffset.y < 0) || (ptOffset.y > m_iItemCnt * m_cellHeight))
        return false;
    
    if (m_pfnSelectorBegin == NULL)
        return true;
    
    CCPoint screenPos = touchLocation;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        screenPos = ccpMult(screenPos, 2);
    if(sz.width < 1000)
        screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    screenPos.x -= MainScene::Inst()->getPosition().x;
    screenPos.y -= MainScene::Inst()->getPosition().y;
    
    
    m_ret.iBtnSel = ptOffset.y / m_cellHeight;
    m_ret.pos = ccp(m_ptStartTouch.x, ptOffset.y - (m_cellHeight * 0.5));
    m_ret.screenPos = screenPos;
    
    (m_pListener->*m_pfnSelectorBegin)(&m_ret);

    return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void CListLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
    m_bTouchMvFlg = true;
    
    if (!m_pScrollBar) {
        return;
    }
    
    if(m_iItemCnt == 1)
        return;

    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    float fOffsetY = pos.y - m_ptStartTouch.y;
    
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += fOffsetY;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    this->setPosition(thisPt);
    
    ChkObjectShow();
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void CListLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if (m_pfnSelectorBegin != NULL)
    {
        chkTouchEnd(touchLocation);
    }
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        if (m_pScrollBar != NULL)
        {
            if(fabs(m_lastYm) >50)
                m_lastYm = m_lastYm/fabs(m_lastYm) *50;
            schedule(schedule_selector(CListLayer::inertiaMove));
        }
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCPoint ptOffset = ccpSub(CCPointMake(320, m_startHeight + m_cellHeight * 0.5), pos);
    
    if((ptOffset.x > kMenuBtnWidth*0.5) || (ptOffset.x < -kMenuBtnWidth*0.5))
        return;
    
    if((ptOffset.y < 0) || (ptOffset.y > m_iItemCnt * m_cellHeight))
        return;
    
    CCPoint screenPos = touchLocation;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        screenPos = ccpMult(screenPos, 2);
    if(sz.width < 1000)
        screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    screenPos.x -= MainScene::Inst()->getPosition().x;
    screenPos.y -= MainScene::Inst()->getPosition().y;
    
    
    m_ret.iBtnSel = ptOffset.y / m_cellHeight;
    m_ret.pos = ccp(ptOffset.x, ptOffset.y - (m_cellHeight * 0.5));
    m_ret.screenPos = screenPos;
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}


void CListLayer::chkTouchEnd(CCPoint point)
{
//    CCPoint pos = convertToNodeSpace( point );
//    
//    CCPoint ptOffset = ccpSub(CCPointMake(320, m_startHeight + m_cellHeight * 0.5), pos);
//    
//    
//    CCPoint screenPos = point;
//    
//    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
//    if(sz.width == 320)
//        screenPos = ccpMult(screenPos, 2);
//    if(sz.width < 1000)
//        screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
//    
//    //ipad, iphone5
//    screenPos.x -= MainScene::Inst()->getPosition().x;
//    screenPos.y -= MainScene::Inst()->getPosition().y;
//    
//    
//    m_ret.iBtnSel = ptOffset.y / m_cellHeight;
//    m_ret.pos = ccp(ptOffset.x, ptOffset.y - (m_cellHeight * 0.5));
//    m_ret.screenPos = screenPos;
    
    (m_pListener->*m_pfnSelectorEnd)(NULL);
}

void CListLayer::ChkObjectShow()
{
    if (m_pfnReload == NULL)
    {
        return;
    }
    
    int disCount = (int)(getPosition().y/m_cellHeight);
    int new_top = startTopShow + disCount;
    int new_bottom = startBottomShow + disCount;
            
    if (new_top > topShow)
    {
        for (int i=topShow; i < new_top; i++)
        {
            if (i >=0 && i < m_iItemCnt)
            {
                if (m_pfnSelectorRemove != NULL)
                {
                    m_ret.iBtnSel = i;
                    (m_pListener->*m_pfnSelectorRemove)(&m_ret);
                }
                removeChildByTag(i+deltaTag, true);
            }
        }
                
                
        for (int i=bottomShow+1; i<=new_bottom; i++)
        {
            if (i >=0 && i < m_iItemCnt)
            {
                m_rdRet.iBtnSel = i;
                (m_pListener->*m_pfnReload)(&m_rdRet);
                        
            }
                        
        }
    }
    else if (new_top < topShow)
    {
        for (int i = new_top; i < topShow; i++)
        {
            if (i >=0 && i < m_iItemCnt)
            {
                m_rdRet.iBtnSel = i;
                (m_pListener->*m_pfnReload)(&m_rdRet);
            }
        }
        
        for (int i=new_bottom+1; i <= bottomShow; i++)
        {
            if (i >=0 && i < m_iItemCnt)
            {
                if (m_pfnSelectorRemove != NULL)
                {
                    m_ret.iBtnSel = i;
                    (m_pListener->*m_pfnSelectorRemove)(&m_ret);
                }
                removeChildByTag(i+deltaTag, true);
            }
        }
    }

    topShow = new_top;
    bottomShow = new_bottom;
}

void CListLayer::inertiaMove(float dt) {
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        
        unschedule(schedule_selector(CListLayer::inertiaMove));
    }
    
    ChkObjectShow();
}

void CListLayer::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    this->setPosition(thisPt);
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    this->setPosition(thisPt);
    
    if(m_fUpLimitY > 0&&m_pScrollBar){ 
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);        
    }    
}

void CListLayer::onExit()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoTop);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoBottom);
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}

void CListLayer::setNumber(int index, int number, bool isGray)
{
    CMenuBtn *node = dynamic_cast<CMenuBtn*>(getChildByTag(deltaTag+index));
    if (node == NULL) {
        return;
    }
    node->setHelperNo(isGray, number);
    
//    if (node->getChildByTag(101)) {
//        node->removeChildByTag(101, true);
//    }
//    if (isGray) {
//        CCLayerColor* grayLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 116);
//        grayLayer->setPosition(ccp(-320, -58));
//        grayLayer->setAnchorPoint(CCPointZero);
//        grayLayer->setTag(101);
//        node->addChild(grayLayer);
//    }
//    if (number) {
//        if (node->getChildByTag(100)) {
//            node->removeChildByTag(100, true);
//        }
//        CCSprite* numberTips = OcProxy::Inst()->showTipsAndNum(number);
//        numberTips->setPosition(ccp(-240, 0));
//        numberTips->setTag(100);
//        node->addChild(numberTips);
//    } else {
//        if (node->getChildByTag(100)) {
//            node->removeChildByTag(100, true);
//        }
//    }
}


CListLayerModel::CListLayerModel( int topShow,int bottomShow,float y)
{
    m_topShow = topShow;
    m_bottomShow = bottomShow;
    m_y = y;
}
int CListLayerModel::getTopShow()
{
    return m_topShow;
}
int CListLayerModel::getBottonShow()
{
    return m_bottomShow;
}
float CListLayerModel::getY()
{
    return m_y;
}

void CListLayerModel::setTopShow(int topShow)
{
      m_topShow = topShow;
}
void CListLayerModel::setBottonShow(int bottomShow)
{
    m_bottomShow = bottomShow;

}
void CListLayerModel::setY(float y)
{
     m_y = y;
}

//多选



