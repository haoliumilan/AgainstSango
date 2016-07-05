//
//  BonusList.cpp
//  test
//
//  Created by mac on 13-6-5.
//
//

#include "BonusList.h"
#include "BonusItem.h"
#include "MainScene.h"
#include "CGameData.h"

#define kBonusItemTagBase   9999

enum enBonusListSortType{
    enBLST_RecentScore,
    enBLST_LastScore,
    enBLST_Max,
};

BonusList::BonusList(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_ptLeftTop = CCPointMake(30, -152);
    m_fHeight = 440.0f;
    m_iSortType = enBLST_RecentScore;

    m_pJiFengRecordArr = CGameData::Inst()->getJiFengRecordArr();
    
    m_iItemCnt = m_pJiFengRecordArr->count();
    
    for (int i = 0; i < m_iItemCnt; i++) {
        BonusItem* bitm = new BonusItem(i,this, callfuncO_selector(BonusList::cbBonusItem));
        addChild(bitm, 0, kBonusItemTagBase+i);
        
        bitm->setPosition(ccp(m_ptLeftTop.x,
                               m_ptLeftTop.y - i * 62));
        
        bitm->release();
    }
    
    m_fUpLimitY = m_iItemCnt * 62 - m_fHeight;
    m_fUpLimitY = (m_fUpLimitY > 0)? m_fUpLimitY:0;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    toggleSort();
}

void BonusList::setPosition(const CCPoint& pos)
{
    CCPoint pt = pos;
    pt.y = clampf(pos.y, 0, m_fUpLimitY);
    
    CCNode::setPosition(pt);
    
    
    for (int i = 0; i < m_iItemCnt; i++) {
        BonusItem* pBItm = (BonusItem*)getChildByTag(kBonusItemTagBase+i);
        if (pBItm) {
            CCPoint ptItm = pBItm->getPosition();
            if ((ptItm.y + pt.y - m_ptLeftTop.y > 70)
                ||(m_ptLeftTop.y - ptItm.y - pt.y > m_fHeight)){
                pBItm->hide();
            }
            else{
                JiFengRecordInf* pInf = m_pJiFengRecordArr->getObjectAtIndex(i);
                pBItm->show(pInf);
            }
        }
    }
}

bool BonusList::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(BonusList::inertiaMove));
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptStartNode = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    m_lastYm = 0;
    
    CCPoint ptThis = this->getPosition();
    if ((m_ptStartNode.y + ptThis.y < -88)
        &&(m_ptStartNode.y + ptThis.y > -530)){
        return true;
    }
    else{
        return false;
    }
}

void BonusList::ccTouchMoved(CCTouch* touch, CCEvent* event)
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

void BonusList::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        m_lastYm *= 1.5;
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        schedule(schedule_selector(BonusList::inertiaMove));
        return;
    }
}

void BonusList::inertiaMove(float dt)
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
        
        unschedule(schedule_selector(BonusList::inertiaMove));
    }
}

void BonusList::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    setPosition(thisPt);
}

void BonusList::visit()
{
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
    glScissor(0 + pos.x,
              95 * fFactor + pos.y + 30,
              300  * fFactor,
              220  * fFactor);
    CCLayer::visit();
    glDisable(GL_SCISSOR_TEST);
}

void BonusList::toggleSort()
{
    unschedule(schedule_selector(BonusList::inertiaMove));
    
    m_iSortType++;
    if (m_iSortType >= enBLST_Max) {
        m_iSortType = enBLST_RecentScore;
    }
    
    for (int i = 0; i < m_iItemCnt-1; i++) {
        for (int j = i+1; j < m_iItemCnt; j++){
            JiFengRecordInf* pInf1 = m_pJiFengRecordArr->getObjectAtIndex(i);
            JiFengRecordInf* pInf2 = m_pJiFengRecordArr->getObjectAtIndex(j);
            
            bool bSwitch = false;
            if (m_iSortType == enBLST_RecentScore) {
                bSwitch = (pInf2->lRecentScore > pInf1->lRecentScore);
            }
            else if (m_iSortType == enBLST_LastScore) {
                bSwitch = (pInf2->lLastScore > pInf1->lLastScore);
            }
            
            if (bSwitch) {
                pInf1->retain();
                m_pJiFengRecordArr->replaceObjectAtIndex(i, pInf2);
                m_pJiFengRecordArr->replaceObjectAtIndex(j, pInf1);
                pInf1->release();
            }
        }
    }
    
    //强制重绘
    for (int i = 0; i < m_iItemCnt; i++) {
        BonusItem* pBItm = (BonusItem*)getChildByTag(kBonusItemTagBase+i);
        if (pBItm)
            pBItm->hide();
    }
    setPosition(CCPointZero);
}

void BonusList::cbBonusItem(CCObject* pObj)
{
    (m_pListener->*m_pfnSelector)(pObj);
}




