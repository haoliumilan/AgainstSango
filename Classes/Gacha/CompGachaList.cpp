//
//  CompGachaList.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CompGachaList.h"
#include "CompGachaBtn.h"
#include "OcProxy.h"

#define FIRSTCELLPOSY 140
#define MenuTag 100
CompGachaList::CompGachaList(CCObject *listener,SEL_CallFuncO selector)
{
    isShowCardInfo = false;
    m_pListener = listener;
    m_pfnSelector = selector; 
    targetId = -1;
    isFirstPush = true;
    sDeltaPosY = 0;
    m_itemCnt = 0;
    m_bTouchMvFlg = false;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    tagPos = FIRSTCELLPOSY;
    m_totoalDelta = 0;
    scrollBarBg = CCSprite::spriteWithFile("cg_scrollbarbk.png");
    addChild(scrollBarBg,10);
    scrollBarBg->setPosition(ccp(277, -43));

    scrollBar = CCSprite::spriteWithFile("cg_scrollbar.png");
    addChild(scrollBar,10);
    scrollBar->setPosition(ccp(277, -43+211));
    scrollBar->setAnchorPoint(CCPoint(0.5, 1.0));

}

void CompGachaList::pushBack(CompGachaBtn *btn)
{
    if(m_itemCnt == 0)
        m_firstTouchPos = this->getPosition();

    this->addChild(btn);
    CCPoint pos = btn->getPosition();
    btn->setTag(MenuTag+m_itemCnt);
    if(m_itemCnt>0)
    {
        int lines = lastBtn->getCardLines();
        tagPos -= 22+(CELLHEIGHT *lines+55);
        if(lines == 1)
        {
            tagPos-=EXCHANGEBTNHEIGHT;
        }
    }
    pos.y += tagPos;
    btn->setPosition(pos);
    m_itemCnt++;
    lastBtn = btn;
    CCLog("%f",lastBtn->getPosition().y);
    
    if(isFirstPush)
    {
        firstBtn = btn;
        isFirstPush = false;
    }
    if(lastBtn->getCardLines()!=1)
    {
        m_listLimtH = 200-(lastBtn->getPosition().y-(CELLHEIGHT *lastBtn->getCardLines()+31)/2+480);
        contentHeight = 180-lastBtn->getPosition().y+CELLHEIGHT *lastBtn->getCardLines()+50;
    }
    else {
        m_listLimtH = 200-(lastBtn->getPosition().y-(CELLHEIGHT *lastBtn->getCardLines()+EXCHANGEBTNHEIGHT+31)/2+480);
        contentHeight = 180-lastBtn->getPosition().y+CELLHEIGHT *lastBtn->getCardLines()+50+EXCHANGEBTNHEIGHT;
    }
   // int scScale = 
    scrollBar->setScaleY(450.0/contentHeight);
}

bool CompGachaList::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bTouchMvFlg = false;
    unschedule(schedule_selector(CompGachaList:: waitShowInfo));
    unschedule(schedule_selector(CompGachaList::inertiaMove));
    CCPoint touchLocation = pTouch->locationInView(pTouch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_preTouchPos = touchLocation;
    //惯性移动部分
    last_moveTouch = touchLocation;
    //确认点击区域
    //转换坐标
    //pos.y += this->getPosition().y-m_firstTouchPos.y;
    //CCLog("touchLocation x %f y %f",pos.x,pos.y+(this->getPosition().y-480));
    CCPoint headPt ;
    CCSize  headSz ;
    for(int i=0;i<m_itemCnt;i++)
    {
        CompGachaBtn *btn = (CompGachaBtn *)getChildByTag(MenuTag+i);
        CCPoint pt = btn->getPosition();
        pt.y-=btn->getDefalutDelta();
        CCSize sz = btn->getSize();
        //CCLog("btn pos x  %f y%f",pt.x,pt.y);
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                           pt.y - sz.height * 0.5, 
                                                           sz.width, 
                                                           sz.height), 
                                                pos);
        //当行的compGachaInfo
        CompGachaInfo *tempCompGachaInfo = CGameData::Inst()->getCompGachaInfo()->getObjectAtIndex(i);
        if(bRet == true)
        {
            CCPoint pointInItem;
            pointInItem.x = pos.x;
            pointInItem.y = pos.y-btn->getPosition().y;
           // CCLog("pointInitem %f",pointInItem.y);
            //判断是否点击到某材料
            for(int j=0;j<tempCompGachaInfo->material->count();j++)
            {
                 headPt =  btn->getChildByTag(MATERIALTAG+j)->getPosition();
                 headSz = btn->getChildByTag(MATERIALTAG+j)->getContentSize();
                bRet = CCRect::CCRectContainsPoint(CCRectMake(headPt.x - headSz.width * 0.5, 
                                                              headPt.y - headSz.height * 0.5, 
                                                              headSz.width, 
                                                              headSz.height), 
                                                   pointInItem);
            
                if(bRet)
                {
                    isShowCardInfo = true;
                    touchedCardInfo = tempCompGachaInfo->material->getObjectAtIndex(j);
                    CCLog("material %d",j);
                    targetId = -1;
                    schedule(schedule_selector(CompGachaList::waitShowInfo), 0.3);
                }
            }
            
          //  CCPoint ptOffset = ccpSub(pos,pt);
             headPt =  btn->getChildByTag(SUPERHEROTAG)->getPosition();
             headSz = btn->getChildByTag(SUPERHEROTAG)->getContentSize();
            bRet = CCRect::CCRectContainsPoint(CCRectMake(headPt.x - headSz.width * 0.5, 
                                                          headPt.y - headSz.height * 0.5, 
                                                          headSz.width, 
                                                          headSz.height), 
                                               pointInItem);
            //CCLog("superHero pos %d %f %f",i,headPt.x,headPt.y);
            
            //是否点击到要获得的卡片
            if(bRet)
            {
                isShowCardInfo =  !tempCompGachaInfo->isHide ;
                targetId = tempCompGachaInfo->targetCid;
                CCLog("superHero");
                schedule(schedule_selector(CompGachaList::waitShowInfo), 0.3);
            }
//            CCLog("headPt x %f y %f",headPt.x,headPt.y);
            CCLog("bRet%d",i);
        }
        
    }
    return true;
}

void CompGachaList::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    unschedule(schedule_selector(CompGachaList::waitShowInfo));
    if( lastBtn->getCardLines() == 1)
    {
        if(lastBtn->getPosition().y-(CELLHEIGHT *lastBtn->getCardLines()+EXCHANGEBTNHEIGHT+31)/2+480>400)
            return;
    }else{
        if(lastBtn->getPosition().y-(CELLHEIGHT *lastBtn->getCardLines()+31)/2+480>400)
            return;
    }
    m_bTouchMvFlg = true;
    CCPoint touchLocation = pTouch->locationInView(pTouch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );

    float deltaPosY = touchLocation.y-m_preTouchPos.y;
    m_preTouchPos = touchLocation;
    
    //惯性
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    //
    CCPoint cuPoint = this->getPosition();
    cuPoint.y+=deltaPosY;
 //   sDeltaPosY += deltaPosY*422/contentHeight;
    if(cuPoint.y<=480)
    {
        cuPoint.y = 480;
        sDeltaPosY = 0;
    }
    else if(cuPoint.y>=480+contentHeight-450)
    {
        cuPoint.y = 480+contentHeight-450;
        sDeltaPosY =0;
        
    }else {
       
    }
    this->setPosition(cuPoint);

    CCPoint scBPoint = scrollBarBg->getPosition();
    scBPoint.y= -43-this->getPosition().y+480;
    scrollBarBg->setPosition(scBPoint);
    
   
    CCPoint scPoint =scrollBar->getPosition();
    scPoint.y=-43+211-this->getPosition().y+480-(this->getPosition().y-480.0)*422*(1-450.0/contentHeight)/(contentHeight - 450.0);
    scrollBar->setPosition(scPoint);
   
   // CCLog("contentH %f",contentHeight);

}

void CompGachaList::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    unschedule(schedule_selector(CompGachaList::waitShowInfo));
    if(m_bTouchMvFlg == true )
    {
        if(fabs(m_lastYm) >50)
            m_lastYm = m_lastYm/fabs(m_lastYm) *50;
        schedule(schedule_selector(CompGachaList::inertiaMove));
        return;
    }
}


void CompGachaList::inertiaMove(float dt) {
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    //printf("m_lastYm %f",m_lastYm);
    // m_lastYm -= dt*
    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        
        unschedule(schedule_selector(CompGachaList::inertiaMove));
        // m_scrollBar->setIsVisible(false);
    }
}

void CompGachaList::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    sDeltaPosY += dt*422/contentHeight;

    
    if(thisPt.y<=480){
        thisPt.y = 480;
        sDeltaPosY = 0;
    }
    else if(thisPt.y>=480+contentHeight-450){
        thisPt.y = 480+contentHeight-450;
        sDeltaPosY = 0;
    }else {
//        CCPoint scPoint =scrollBar->getPosition();
//        scPoint.y=-43+211-this->getPosition().y+480-sDeltaPosY;
//        scrollBar->setPosition(scPoint);
        
    }
    this->setPosition(thisPt);

    CCPoint scBPoint = scrollBarBg->getPosition();
    scBPoint.y= -45-this->getPosition().y+480;
    scrollBarBg->setPosition(scBPoint);

    
    CCPoint scPoint =scrollBar->getPosition();
    scPoint.y=-43+211-this->getPosition().y+480-(this->getPosition().y-480.0)*422*(1-450.0/contentHeight)/(contentHeight - 450);
    scrollBar->setPosition(scPoint);

    
//    if(thisPt.y > m_fUpLimitY)
//        thisPt.y = m_fUpLimitY;
//    
//    if(thisPt.y < 0.0)
//        thisPt.y = 0.0;
    
    
//    if(m_fUpLimitY > 0&&m_pScrollBar){ 
//        float fPercent = thisPt.y / m_fUpLimitY;
//        
//        m_pScrollBar->setPercent(fPercent);
//        
//        //        if(enPkLkTp_PwUpFood == m_iLookTp)
//        //            m_pScrollBar->setPosition(CCPointMake(630, 482 - thisPt.y));
//        //        else
//        //   m_pScrollBar->setPosition(CCPointMake(630, 435 - thisPt.y));
//        
//        // redrawCapacity();
//        
//        //        if(m_packSelBtn != NULL)
//        //            m_packSelBtn->setPosition(CCPointMake(0, - thisPt.y));
//    }
    //  m_showCard->setPosition(CCPointMake(0, -this->getPosition().y));
   
}

void CompGachaList::visit()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(0,
              (CCDirector::sharedDirector()->getWinSizeInPixels().height- OcProxy::Inst()->deltaHeightOf568()*2)*105.0/480.0 + 30,//shikun_ios5
              
              CCDirector::sharedDirector()->getWinSizeInPixels().width,((230.0)/480.0*(CCDirector::sharedDirector()->getWinSizeInPixels().height-OcProxy::Inst()->deltaHeightOf568()*2)));//x, y, w, h
    CCLayer::visit();//调用自己的类方法
    glDisable(GL_SCISSOR_TEST);
    //CCLog("height %f",(250.0)/480.0*CCDirector::sharedDirector()->getWinSize().height);
}

void CompGachaList::waitShowInfo()
{
    if(targetId == -1)
    {      
        m_ret.isOwe =  touchedCardInfo->isOwe;
        m_ret.cid = touchedCardInfo->cid;
    }
    else {
       // CCLog("waitShowInfo target");
        m_ret.cid = targetId;
        m_ret.isOwe = false;
        
    }
    m_ret.isShow = isShowCardInfo;

    (m_pListener->*m_pfnSelector)(&m_ret);
    unschedule(schedule_selector(CompGachaList::waitShowInfo));
}

void CompGachaList::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}

