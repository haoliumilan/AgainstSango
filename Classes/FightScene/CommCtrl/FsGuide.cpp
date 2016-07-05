//
//  FsGuide.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-4-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "FsGuide.h"
#include "TextNode.h"

//******************************************************************************
// onEnter
//******************************************************************************
void FsGuide::onEnter()
{
    CCSprite* spGuideBg = CCSprite::spriteWithFile("guide_bg.png");
    spGuideBg->setAnchorPoint(ccp(0, 0));
    spGuideBg->setPosition(ccp(0, 0));
    addChild(spGuideBg);
    
    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    spFrame->setScale(1.5);
    spFrame->setAnchorPoint(CCPointZero);
    spFrame->setPosition(ccp(14, 58));
    addChild(spFrame);
    
    CCSprite* spWs = CCSprite::spriteWithFile("npc_ws.png");
    spWs->setAnchorPoint(ccp(1, 0));
    spWs->setPosition(ccp(640, 0));
    addChild(spWs);
    
    m_txtGuide = TextNode::textWithString("", CCSizeMake(384, 90), CCTextAlignmentLeft, 25);
    m_txtGuide->setPosition(CCPointMake(42, 190));
    m_txtGuide->setAnchorPoint(ccp(0, 1));
    m_txtGuide->setColor(ccWHITE);
    m_txtGuide->setShadowColor(ccBLACK);
    addChild(m_txtGuide, 1);
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1000, true);
}

//******************************************************************************
// setGuideString
//******************************************************************************
void FsGuide::setGuideString(CCMutableArray<CCString*> * pTipsArr,
                             CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_pTipsArr = pTipsArr;
    
    if (pTipsArr->count() == 0) {
        (m_pListener->*m_pfnSelector)();
    }
    else{
        m_txtGuide->setString(pTipsArr->getObjectAtIndex(0)->toStdString().c_str());
        m_idx = 0;
    }
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool FsGuide::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    m_idx++;
    
    if (m_idx >= m_pTipsArr->count()) {
        (m_pListener->*m_pfnSelector)();
    }
    else{
        m_txtGuide->setString(m_pTipsArr->getObjectAtIndex(m_idx)->toStdString().c_str());
    }
    
    return true;
}





