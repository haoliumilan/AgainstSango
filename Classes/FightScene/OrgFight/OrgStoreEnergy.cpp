//
//  OrgStoreEnergy.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgStoreEnergy.h"

#define kEnergyIncDur   0.5

#define kStemNumTag     3344

//******************************************************************************
// OrgStoreEnergy
//******************************************************************************
OrgStoreEnergy::OrgStoreEnergy()
{
    m_engyBar = NULL;
    m_fCurPerct = 0.0;
    m_fStartPerct = 0.0;
}

//******************************************************************************
// onEnter
//******************************************************************************
void OrgStoreEnergy::onEnter()
{
    CCLayer::onEnter();

    //进度条
    m_engyBar = CCProgressTimer::progressWithFile("dragon_progress.png");

    if(m_engyBar){
        m_engyBar->setType(kCCProgressTimerTypeVerticalBarBT);
        addChild(m_engyBar);
        m_engyBar->setAnchorPoint(CCPointZero);
        m_engyBar->setPosition(ccp(-20, 8));
        m_engyBar->setPercentage(0);
    }
    
    //龙背景
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("dragon_head.png");
    if(sp){
        addChild(sp);
        sp->setAnchorPoint(CCPointZero);
        sp->setPosition(ccp(-120,-24));
    }
    
    //step number
    CCSprite* spStepNum = CCSprite::spriteWithSpriteFrameName("org_combo_0.png");
    if (spStepNum) {
        addChild(spStepNum, 0, kStemNumTag);
        spStepNum->setPosition(ccp(36,60));
    }
}

//******************************************************************************
// setStep
//******************************************************************************
void OrgStoreEnergy::setStep(int iStep, float dt)
{
    unscheduleUpdate();
    
    m_fElapse = -dt;
    
    switch (iStep) {
        case 1:
            m_fStartPerct =0;
            m_fCurPerct = 0;
            m_fDestPerct = 33.3;
            break;
            
        case 2:
            m_fDestPerct = 66.6;
            break;
            
        case 3:
            m_fDestPerct = 100.0;
            break;
            
        default:
            break;
    }
    
    m_fStartPerct = m_fCurPerct;
    
    if (m_fDestPerct - m_fStartPerct > 1){
        scheduleUpdate();
    }
    
    
    CCNode* preStepNum = getChildByTag(kStemNumTag+iStep-1);
    if (preStepNum) {
        preStepNum->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                                  CCHide::action(),
                                                  NULL) );
    }
    
    char buf[100];
    snprintf(buf, 99, "org_combo_%d.png", iStep);
    CCSprite* spStepNum = CCSprite::spriteWithSpriteFrameName(buf);
    if (spStepNum) {
        addChild(spStepNum, 0, kStemNumTag+iStep);
        spStepNum->setPosition(ccp(36,60));
        spStepNum->setIsVisible(false);
        spStepNum->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                                 CCShow::action(),
                                                 CCScaleTo::actionWithDuration(0.1, 1.1),
                                                 CCScaleTo::actionWithDuration(0.1, 1.0),
                                                 NULL) );
    }
}

//******************************************************************************
// useEnergy
//******************************************************************************
void OrgStoreEnergy::useEnergy(float dt)
{
    schedule(schedule_selector(OrgStoreEnergy::cbUseEnergy), dt);
}

//******************************************************************************
// cbUseEnergy
//******************************************************************************
void OrgStoreEnergy::cbUseEnergy(ccTime dt)
{
    unscheduleAllSelectors();
    m_fElapse = 0.0;
    m_fStartPerct = m_fCurPerct;
    m_fDestPerct = 0.0;
    scheduleUpdate();
    
    for (int i = 0; i < 4; i++)
        removeChildByTag(kStemNumTag+i, true);
}

//******************************************************************************
// update
//******************************************************************************
void OrgStoreEnergy::update(ccTime dt)
{
    m_fElapse += dt;
    
    if(m_fElapse < 0)
        return;
    
    m_fCurPerct = m_fStartPerct + (m_fDestPerct - m_fStartPerct) * (m_fElapse / kEnergyIncDur);
    
    if (m_fElapse > kEnergyIncDur)
        m_fCurPerct = m_fDestPerct;
    
    m_engyBar->setPercentage(m_fCurPerct);
        
    if(m_fElapse >= kEnergyIncDur)
    {
        unscheduleUpdate();
        return;
    }
}


