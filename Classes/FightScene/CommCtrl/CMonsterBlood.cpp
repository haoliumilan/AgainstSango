//
//  CMonsterBlood.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CMonsterBlood.h"


CMonsterBlood::CMonsterBlood(CCPoint& pos, CCSize szEnemy, int iElem)
{
    m_nDelayMoveHP = 0;
    m_fDelayMoveTime = 0.0f;
    m_pEndBlood = NULL;
    m_iCurBlood = -1;
    CCSprite* spBlood3 = CCSprite::spriteWithSpriteFrameName("monsterblood3.png");
    CCSprite* spBlood5 = CCSprite::spriteWithSpriteFrameName("monsterblood5.png");
    CCSprite* spBlood8 = CCSprite::spriteWithSpriteFrameName("monsterblood8.png");
    CCSprite* spBlood15 = CCSprite::spriteWithSpriteFrameName("monsterblood15.png");
    
    CCPoint pt = pos;
    pt.y = pt.y - szEnemy.height * 0.5;
    
    char bufFilename[20];
    sprintf(bufFilename, "stone%ds.png", iElem);
    m_spElem = CCSprite::spriteWithSpriteFrameName(bufFilename);

    sprintf(bufFilename, "mblood%d.png", iElem);
    m_spBlood = CCSprite::spriteWithSpriteFrameName(bufFilename);
    
    if(szEnemy.width > spBlood15->getContentSize().width){
        m_spBloodBase = spBlood15;
        m_fScaleMax = 6.15;

        if (m_spElem) {
            pt.x = pt.x - m_spBloodBase->getContentSize().width * 0.5 + 20;
            m_spElem->setPosition(pt);
            addChild(m_spElem);
        }
        
        if (m_spBlood) {
            pt.x += 16;
            m_spBlood->setPosition(pt);
            m_spBlood->setScaleX(m_fScaleMax);
            m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
            addChild(m_spBlood);
        }
    }
    else if(szEnemy.width > spBlood8->getContentSize().width){
        m_spBloodBase = spBlood8;
        m_fScaleMax = 3.82;
        
        if (m_spElem) {
            pt.x = pt.x - m_spBloodBase->getContentSize().width * 0.5 + 18;
            m_spElem->setPosition(pt);
            addChild(m_spElem);
        }
        
        if (m_spBlood) {
            pt.x += 18;
            m_spBlood->setPosition(pt);
            m_spBlood->setScaleX(m_fScaleMax);
            m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
            addChild(m_spBlood);
        }
    }
    else if(szEnemy.width > spBlood5->getContentSize().width){
        m_spBloodBase = spBlood5;
        m_fScaleMax = 2.14;
        
        if (m_spElem) {
            pt.x = pt.x - m_spBloodBase->getContentSize().width * 0.5 + 18;
            m_spElem->setPosition(pt);
            addChild(m_spElem);
        }
        
        if (m_spBlood) {
            pt.x += 18;
            m_spBlood->setPosition(pt);
            m_spBlood->setScaleX(m_fScaleMax);
            m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
            addChild(m_spBlood);
        }
    }
    else
    {
        m_spBloodBase = spBlood3;
        m_fScaleMax = 1.0;
        
        if (m_spElem) {
            pt.x = pt.x - m_spBloodBase->getContentSize().width * 0.5 + 18;
            m_spElem->setPosition(pt);
            addChild(m_spElem);
        }
        
        if (m_spBlood) {
            pt.x += 18;
            m_spBlood->setPosition(pt);
            m_spBlood->setScaleX(m_fScaleMax);
            m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
            addChild(m_spBlood);
        }
    }
    
    addChild(m_spBloodBase, -1);
    m_spBloodBase->setPosition(ccp(pos.x, pos.y - szEnemy.height * 0.5));
}

bool CMonsterBlood::beElem(int nElem)
{
    char bufFilename[32];
    sprintf(bufFilename, "stone%ds.png",nElem);
    if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bufFilename))
    {
        sprintf(bufFilename, "mblood%d.png", nElem);
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(bufFilename))
        {
            return true;
        }
    }
    return false;
}

void CMonsterBlood::setElem(int iElem)
{
    CCPoint pos1 = m_spElem->getPosition();
    removeChild(m_spElem, true);
    char bufFilename[20];
    sprintf(bufFilename, "stone%ds.png", iElem);
    m_spElem = CCSprite::spriteWithSpriteFrameName(bufFilename);
    m_spElem->setPosition(pos1);
    addChild(m_spElem);
    
    CCPoint pos2 = m_spBlood->getPosition();
    float scaleX = m_spBlood->getScaleX();
    removeChild(m_spBlood, true);
    sprintf(bufFilename, "mblood%d.png", iElem);
    m_spBlood = CCSprite::spriteWithSpriteFrameName(bufFilename);
    m_spBlood->setPosition(pos2);
    m_spBlood->setScaleX(scaleX);
    m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
    addChild(m_spBlood);
}

void CMonsterBlood::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCNode::onExit();
}

void CMonsterBlood::setBlood(int iBlood, int iBloodBase, ccTime dt, int nDelayMoveHP /*= 0*/,float fDelayMoveTime /*= 0.0f*/)
{
    m_iBloodBase = iBloodBase;
    m_nDelayMoveHP = nDelayMoveHP;
    m_fDelayMoveTime = fDelayMoveTime;
    
    if(m_iCurBlood == iBlood && m_iCurBlood != 0)
        return;
    
    if(iBlood < 0)
        iBlood = 0;
    if (iBlood > iBloodBase) {
        iBlood = iBloodBase;
    }
    m_iCurBlood = iBlood;
    
    
    float fScaleX = m_iCurBlood * m_fScaleMax / m_iBloodBase;
    
    m_spBlood->stopAllActions();
    m_spBlood->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                             CCScaleTo::actionWithDuration(0.5, fScaleX, 1.0),
                                             CCCallFunc::actionWithTarget(this,callfunc_selector(CMonsterBlood::endBloodMove)),
                                             NULL));
}

void CMonsterBlood::fadeout(ccTime dt)
{
    m_spBloodBase->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCFadeOut::actionWithDuration(0.5),
                                                 NULL));
    
    m_spElem->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                            CCFadeOut::actionWithDuration(0.5),
                                            NULL));
    
    m_spBlood->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                             CCFadeOut::actionWithDuration(0.5),
                                             NULL));
}

void CMonsterBlood::hide()
{
    m_spBloodBase->setOpacity(0);
    m_spElem->setOpacity(0);
    m_spBlood->setOpacity(0);
}

void CMonsterBlood::endBloodMove()
{
    if (m_pEndBlood)
    {
        (m_pEndBlood->*fcEndBlood)(this,NULL);
    }
}

void CMonsterBlood::loginBloodMove(CCObject *pObj, SEL_CallFuncND func)
{
    m_pEndBlood = pObj;
    fcEndBlood = func;
}

void CMonsterBlood::fadein(ccTime dt)
{
    m_spBloodBase->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                               CCFadeIn::actionWithDuration(0.5),
                                               NULL));
    
    m_spElem->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCFadeIn::actionWithDuration(0.5),
                                                 NULL));
    
    m_spBlood->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCFadeIn::actionWithDuration(0.5),
                                                 NULL));
}


float CMonsterBlood::getLeftPosX()
{
    float fx = m_spBloodBase->getPosition().x - m_spBloodBase->getContentSize().width * 0.5;
    
    return fx;
}

float CMonsterBlood::getRightPosX()
{
    float fx = m_spBloodBase->getPosition().x + m_spBloodBase->getContentSize().width * 0.5;
    
    return fx;
}


