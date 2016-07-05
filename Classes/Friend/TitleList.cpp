//
//  TitleList.cpp
//  test
//
//  Created by Zhang Hao on 13-5-20.
//
//

#include "TitleList.h"
#include "TitleItem.h"
#include "CommonDef.h"

#define kTitleItemBaseTag   3344

TitleList::TitleList(CCRect rct)
{
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_displayRct = rct;
    m_iItmCnt = 0;
    m_fCurOffsetY = 0.0f;
    m_fLimitY = 0.0f;
}

//void TitleList::visit()
//{
//    float fFactor = 1.0;
//    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
//    
//    if(winsz.width > 1000){
//        fFactor = 4.0;
//    }
//    else if(winsz.width == 768){
//        fFactor = 2.0;
//    }
//    else if(winsz.width == 640){
//        fFactor = 2.0;
//    }
//    
//    CCPoint pos = ccp(0,0);//MainScene::Inst()->getPositionInPixels();
//    
//    glEnable(GL_SCISSOR_TEST);
//    glScissor(m_displayRct.origin.x * 0.5 * fFactor + pos.x,
//              m_displayRct.origin.y * 0.5 * fFactor + pos.y,
//              m_displayRct.size.width * 0.5  * fFactor,
//              m_displayRct.size.height * 0.5  * fFactor);
//    CCLayer::visit();
//    glDisable(GL_SCISSOR_TEST);
//}

bool TitleList::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(TitleList::inertiaMove));
    
    if (m_iItmCnt <= 0)
        return false;
    
    m_bMvFlg = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    m_ptStartTouch = convertToNodeSpace( touchLocation );

    m_fThisY = this->getPosition().y;
    
    nodePos.y += m_fThisY;
    bool bTouchInRct =
    CCRect::CCRectContainsPoint(m_displayRct, nodePos);
    
    if (bTouchInRct)
        return true;
    else
        return false;
}

void TitleList::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    m_bMvFlg = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    
    float fNewY = m_fThisY + touchLocation.y - m_ptStartGl.y;
    
    fNewY = clampf(fNewY, 0, m_fLimitY);
    
    this->setPosition(ccp(0, fNewY));
}

void TitleList::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bMvFlg && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        if (m_fLimitY > 0){
            if(fabs(m_lastYm) > 50)
                m_lastYm = m_lastYm/fabs(m_lastYm) *50;
            schedule(schedule_selector(TitleList::inertiaMove));
        }
        return;
    }

    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    for (int i = 0; i < m_iItmCnt; i++) {
        TitleItem* nodeItem = (TitleItem*)getChildByTag(kTitleItemBaseTag + i);
        if (nodeItem) {
            CCPoint itemPos = nodeItem->getPosition();
            CCSize itemSz = nodeItem->getContentSizeInPixels();
            
            bool bTouched =
            CCRect::CCRectContainsPoint(CCRectMake(itemPos.x - itemSz.width*0.5,
                                                   itemPos.y - itemSz.height,
                                                   itemSz.width,
                                                   itemSz.height),
                                        nodePos);
            
            if (bTouched) {
                if(nodeItem->procTouch(touchLocation))
                    redraw();
                break;
            }
        }
    }
}

void TitleList::inertiaMove(float dt)
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
        
        unschedule(schedule_selector(TitleList::inertiaMove));
    }
}

void TitleList::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    this->setPosition(thisPt);
    
    if(thisPt.y > m_fLimitY)
        thisPt.y = m_fLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    this->setPosition(thisPt);
}

void TitleList::push_back(CCNode* pTtlItm)
{
    setIsVisible(true);
    
    addChild(pTtlItm, 0, kTitleItemBaseTag+m_iItmCnt);
    m_iItmCnt++;
    
    pTtlItm->setPosition(ccp(m_displayRct.origin.x + m_displayRct.size.width * 0.5,
                             m_displayRct.origin.y + m_displayRct.size.height - m_fCurOffsetY));
    
    m_fCurOffsetY += pTtlItm->getContentSizeInPixels().height;
    
    if (m_fCurOffsetY > m_displayRct.size.height) {
        m_fLimitY = m_fCurOffsetY - m_displayRct.size.height;
    }
}

TitleItem* TitleList::getEquipedItem()
{
    TitleItem* pItem = NULL;
    
    for (int i = 0; i < m_iItmCnt; i++) {
        pItem = (TitleItem*)getChildByTag(kTitleItemBaseTag + i);
        if (pItem) {
            if (pItem->getEquipSt() == enTtlEquiped) {
                break;
            }
            else{
                pItem = NULL;
            }
        }
    }
    
    return pItem;
}

void TitleList::redraw()
{
    float fOffsetY = 0.0f;
    
    for (int i = 0; i < m_iItmCnt; i++) {
        TitleItem* nodeItem = (TitleItem*)getChildByTag(kTitleItemBaseTag + i);
        if (nodeItem) {
            nodeItem->setPosition(ccp(m_displayRct.origin.x + m_displayRct.size.width * 0.5,
                                      m_displayRct.origin.y + m_displayRct.size.height - fOffsetY));
            fOffsetY += nodeItem->getContentSizeInPixels().height;
        }
    }
    
    if (fOffsetY > m_displayRct.size.height) {
        m_fLimitY = fOffsetY - m_displayRct.size.height;
    }
    
    float fNewY = clampf(getPosition().y, 0, m_fLimitY);
    
    this->setPosition(ccp(0, fNewY));
}

void TitleList::clearAll()
{
    removeAllChildrenWithCleanup(true);
    
    m_iItmCnt = 0;
    m_fCurOffsetY = 0.0f;
    m_fLimitY = 0.0f;
    
    setIsVisible(false);
}

void TitleList::equipById(string title_id)
{
    for (int i = 0; i < m_iItmCnt; i++) {
        TitleItem* nodeItem = (TitleItem*)getChildByTag(kTitleItemBaseTag + i);
        if ((nodeItem)
            &&(nodeItem->getTitleId() == title_id)){
            nodeItem->setEquipSt(enTtlEquiped);
        }
    }
}


