//
//  CScrollBar.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CScrollBar.h"



CScrollBar::CScrollBar()
{
    m_spScrollBarBk = CCSprite::spriteWithSpriteFrameName("scrollbarbk.png");
    addChild(m_spScrollBarBk);
    
    m_spScrollBar = CCSprite::spriteWithSpriteFrameName("scrollbar.png");
    addChild(m_spScrollBar);
    m_spScrollBar->setAnchorPoint(CCPointMake(0.5, 1.0));
    m_spScrollBar->setPosition(CCPointMake(0, m_spScrollBar->getContentSize().height * 0.5));
}

void CScrollBar::setBarSize(float fsize)
{
    m_fBarSize = fsize;
    
    if(fsize < 0.0)
        m_fBarSize = 0.01;
    
    if(fsize > 1.0)
        m_fBarSize = 1.0;
    
    m_spScrollBar->setScaleY(m_fBarSize);
}


void CScrollBar::setPercent(float fPercent)
{
    float fRange = m_spScrollBar->getContentSize().height * (1.0 - m_fBarSize);
    
    CCPoint pt(0, m_spScrollBar->getContentSize().height * 0.5);
    
    pt.y -= fRange * fPercent;
    
    m_spScrollBar->setPosition(pt);
}