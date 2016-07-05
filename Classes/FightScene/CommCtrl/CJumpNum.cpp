//
//  CJumpNum.cpp
//
//  Created by Hao Zhang on 11-11-10.
//  Copyright (c) 2011 zhanghao. All rights reserved.
//

#include "CJumpNum.h"

CJumpNum::CJumpNum(bool isMain):m_iStartNum(0),
                   m_iCurNum(0),
                   m_iDstNum(0),
                   m_bActionFlg(false),
                   m_bJump(true),
                   m_bScale(true)
{
    m_bIsMain = isMain;
    scheduleUpdate();
}

CJumpNum * CJumpNum::initNumber(const char* filename, bool isMain)
{
    CJumpNum *pRet = new CJumpNum(isMain);
    
    CCSprite* spTemp = CCSprite::spriteWithFile(filename);
    
    if(pRet && pRet->initWithString("0", filename, spTemp->getContentSize().width / 10, spTemp->getContentSize().height, '0'))
    {
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet)
    return NULL;
}

void CJumpNum::setPosition(const CCPoint& newPosition)
{
    m_position = newPosition;
    
    CCNode::setPositionInPixels(newPosition);
}

void CJumpNum::setJump(bool bJump)
{
    m_bJump = bJump;
}

void CJumpNum::setIsScale(bool bScale)
{
    m_bScale = bScale;
}

void CJumpNum::setNumber(int num, float fDur, ccTime dt)
{
    m_iDstNum = num;
    
    if(m_bActionFlg == true)
        m_iStartNum = m_iCurNum;
    
    m_fDur = fDur;
    m_fStartTime = 0.0;
    m_fElapse = -dt;
    
    char buf[100];
    sprintf(buf, "%d", num);
   // setString(buf);
    
    m_bActionFlg = true;
}

void CJumpNum::update(ccTime dt)
{
    if(!m_bActionFlg)
        return;
    
    if(m_fDur < 0.01){
        m_bActionFlg = false;
        m_iStartNum = m_iDstNum;
        m_iCurNum = m_iDstNum;
        return;
    }
    
    m_fElapse += dt;
    if(m_fElapse < 0)
        return;
    
    float fProgress = (m_fElapse - m_fStartTime) / m_fDur;
    
    m_iCurNum = (m_iDstNum - m_iStartNum) * fProgress + m_iStartNum;
    
    if(fProgress > 1.0){
        m_bActionFlg = false;
        m_iStartNum = m_iDstNum;
        m_iCurNum = m_iDstNum;
        fProgress = 1.0;
    }
    
    char buf[100];
    sprintf(buf, "%d", m_iCurNum);
    setString(buf);

    if(fProgress > 0.5)
        fProgress = 1 - fProgress;
    
    setAnchorPoint(CCPointMake(0.5, 0.5));
    
    if(m_bJump){
        CCPoint newPt;
        newPt.x = m_position.x;
        
        if (m_bIsMain)
        newPt.y = m_position.y + m_uItemHeight * 1.5 * (1 - (1 - fProgress*2)*(1 - fProgress*2));
        else
        newPt.y = m_position.y + m_uItemHeight * 1.5 *0.5 * (1 - (1 - fProgress*2)*(1 - fProgress*2));
        CCNode::setPosition(newPt);
    }
    
    if(m_bScale)
    {
        if (m_bIsMain)
        setScale(1 + fProgress);
        else
        setScale(0.7 + fProgress);
    }
}

void CJumpNum::onExit()
{
    unscheduleAllSelectors();
    CCLabelAtlas::onExit();
}
