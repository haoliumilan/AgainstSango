//
//  CHurtNum.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CHurtNum.h"


CHurtNum::CHurtNum()
{
    setAnchorPoint(CCPointMake(0.5, 0.5));
}


CHurtNum* CHurtNum::initNumber(const char* filename)
{
    CHurtNum *pRet = new CHurtNum();
    
    CCSprite* spTemp = CCSprite::spriteWithFile(filename);
    
    if(pRet && pRet->initWithString("0", filename, spTemp->getContentSize().width / 10, spTemp->getContentSize().height, '0'))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet)
    return NULL;
}

void CHurtNum::setPosition(const CCPoint& newPosition)
{
    CCNode::setPosition(newPosition);
}

void CHurtNum::setNumber(int startnum, int endnum, ccTime dt)
{
    int iRandX = rand() % (kHurtArea * 2) - kHurtArea;
    int iRandY = rand() % kHurtArea;
    CCPoint pt1(iRandX, iRandY);
    
    iRandX = rand() % (kHurtArea * 2) - kHurtArea;
    iRandY = rand() % (kHurtArea * 2);
    
    float fScale = 0.8;
    if(endnum < startnum){
        iRandY = -iRandY;
        fScale = 0.5;
    }
    else if(endnum > startnum){
        fScale = 1.5;
    }
    
    CCPoint pt2(iRandX, iRandY);
    
    char buf[100];
    sprintf(buf, "%d", startnum);
    setString(buf);
    setScale(0.8);
    
    this->setOpacity(0);
    this->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                        CCFadeIn::actionWithDuration(0.1),
                                        CCMoveBy::actionWithDuration(kHurtNumDur1, pt1),
                                        CCSpawn::actions(CCMoveBy::actionWithDuration(kHurtNumDur2, pt2),
                                                         CCScaleTo::actionWithDuration(kHurtNumDur2, fScale),NULL),
                                        CCFadeOut::actionWithDuration(0.1),
                                        NULL));
    
    m_iStartNum = startnum;
    m_iEndNum = endnum;
    m_fElapse = 0;
    m_fNumChgDur = dt + 0.1 + kHurtNumDur1;
    
    scheduleUpdate();
}

void CHurtNum::update(ccTime dt)
{
    m_fElapse += dt;
    
    if(m_fElapse < m_fNumChgDur)
        return;
    
    float fProgress = (m_fElapse - m_fNumChgDur) / (kHurtNumDur2 - 0.2);
    
    if(fProgress > 1.0){
        fProgress = 1.0;
        unscheduleUpdate();
    }
    
    int iNum = (m_iEndNum - m_iStartNum) * fProgress + m_iStartNum;
    
    char buf[100];
    sprintf(buf, "%d", iNum);
    setString(buf);
}



