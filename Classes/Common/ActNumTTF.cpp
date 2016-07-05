//
//  ActNumTTF.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-7-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "ActNumTTF.h"
#include "TextNode.h"

//******************************************************************************
// Constructor
//******************************************************************************
ActNumTTF::ActNumTTF(const CCSize& dimensions, 
                     CCTextAlignment alignment, 
                     float fontSize, 
                     const ccColor3B& color3)
{
    m_lbNum = TextNode::textWithString("", dimensions, alignment, fontSize);
    m_lbNum->setColor(color3);
    addChild(m_lbNum);
    
    m_iPrevNum = 0;
    m_iCurNum = 0;
    m_iMaxNum = 0;
}

//******************************************************************************
// initNumber
//******************************************************************************
ActNumTTF* ActNumTTF::initNumber(const CCSize& dimensions, 
                                 CCTextAlignment alignment, 
                                 float fontSize, 
                                 const ccColor3B& color3)
{
    ActNumTTF *pRet = new ActNumTTF(dimensions,alignment, fontSize, color3);

    if(pRet){
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet)
    return NULL;
}

//******************************************************************************
// setNumber
//******************************************************************************
void ActNumTTF::setNumber(unsigned long long endnum, ccTime dt)
{
    m_iPrevNum = m_iCurNum;
    m_iEndNum = endnum;
    m_fElapse = 0;
    m_fDur = dt;
    
    unscheduleUpdate();
    scheduleUpdate();
}

//******************************************************************************
// reachNumInstantly
//******************************************************************************
void ActNumTTF::reachNumInstantly()
{
    m_fElapse = m_fDur;
}

//******************************************************************************
// setMaxNum
//******************************************************************************
void ActNumTTF::setMaxNum(unsigned long long maxNum)
{
    m_iMaxNum = maxNum;
}

//******************************************************************************
// update
//******************************************************************************
void ActNumTTF::update(ccTime dt)
{
    char buf[100];
    m_fElapse += dt;
    
    if(m_fElapse >= m_fDur){
        m_iCurNum = m_iEndNum;
        
        if(m_iMaxNum > 0)
            sprintf(buf, "%llu/%llu", m_iCurNum, m_iMaxNum);
        else
            sprintf(buf, "%llu", m_iCurNum);
        
        m_lbNum->setString(buf);
        unscheduleUpdate();
        return;
    }
    
    float fProgress = m_fElapse / m_fDur;
    
    if(fProgress > 1.0){
        fProgress = 1.0;
        unscheduleUpdate();
    }
    
    if (m_iEndNum > m_iPrevNum)
    {
        m_iCurNum = (m_iEndNum - m_iPrevNum) * fProgress + m_iPrevNum;
    }
    else
    {
        m_iCurNum = m_iPrevNum - (m_iPrevNum - m_iEndNum) * fProgress;
    }

    if(m_iMaxNum > 0)
        sprintf(buf, "%llu/%llu", m_iCurNum, m_iMaxNum);
    else
        sprintf(buf, "%llu", m_iCurNum);
    
    m_lbNum->setString(buf);
}

//******************************************************************************
// onExit
//******************************************************************************
void ActNumTTF::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}


