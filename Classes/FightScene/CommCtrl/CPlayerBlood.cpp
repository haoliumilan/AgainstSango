//
//  CPlayerBlood.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CPlayerBlood.h"
#include "SimpleAudioEngine.h"
#include "CGamedata.h"
#include "CHurtNum.h"
#include "CGameState.h"

using namespace CocosDenshion;



CPlayerBlood::CPlayerBlood(int iBloodBase, int iBloodCur)
{
    
    m_iBloodBase = iBloodBase;
    m_iCurBlood = iBloodCur;
    
    m_spBloodBase = CCSprite::spriteWithSpriteFrameName("bloodfrm.png");
    addChild(m_spBloodBase, -1);
    
    m_spBlood = CCSprite::spriteWithSpriteFrameName("playerblood.png");
    m_spBlood->setAnchorPoint(CCPointMake(0.0, 0.5));
    addChild(m_spBlood);
    m_spBlood->setScaleX(m_iCurBlood * 1.0 / m_iBloodBase);
    
    m_spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
    m_spAnim->setScale(2.0);
    addChild(m_spAnim);
     
    CCSprite* spTemp = CCSprite::spriteWithFile("lbhpnum.png");
    
    char buf[100];
    sprintf(buf, "%d/%d", iBloodCur,iBloodBase);
    m_lbHp = CCLabelAtlas::labelWithString(buf, "lbhpnum.png", spTemp->getContentSize().width / 11, spTemp->getContentSize().height, '/');
    addChild(m_lbHp);
    m_lbHp->setAnchorPoint(CCPoint(1.0, 0.0));
    
    
}


CPlayerBlood::~CPlayerBlood()
{
    CCLog("CPlayerBlood::~CPlayerBlood");
}


void CPlayerBlood::setBaseBlood(int iBloodBase)
{
    m_iBloodBase = iBloodBase;
}


void CPlayerBlood::setBlood(int iBlood, ccTime dt,bool updateByBaseHp)
{

    
    
    char buf[100] = {0};
    
    m_iStartNum = m_iCurBlood;
    m_fElapse = 0;
    m_fNumChgDur = dt;
    
    if(m_iCurBlood == iBlood && (!updateByBaseHp))
        return;
    
    if(m_iCurBlood < iBlood || updateByBaseHp)
    {
        CCAnimation* animation;
        
        CCActionInterval*  action;
        
        animation = CCAnimation::animation();
        for( int i=1;i<13;i++){
            sprintf(buf, "hp00%02d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
            animation->addFrame(frame);
        }
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
        action = CCAnimate::actionWithDuration(1.0, animation, false);
        m_spAnim->runAction(action);
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(hpup));
    }

    m_iCurBlood = iBlood;
    if(m_iCurBlood < 0)
        m_iCurBlood = 0;
    
    float fScaleX = m_iCurBlood * 1.0 / m_iBloodBase;

    m_spBlood->stopAllActions();
    m_spBlood->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                             CCScaleTo::actionWithDuration(0.3, fScaleX, 1.0),
                                             NULL));
    

    
    unscheduleUpdate();
    scheduleUpdate();
}



void CPlayerBlood::setPosition(const CCPoint& newPosition)
{
    m_spBloodBase->setPosition(newPosition);
    m_spAnim->setPosition(newPosition);
    m_spBlood->setPosition(CCPointMake(newPosition.x - m_spBloodBase->getContentSize().width*0.5 + 48, 
                                       newPosition.y+1));

    CCPoint pt;
    pt.x = newPosition.x + m_spBloodBase->getContentSize().width * 0.5;
    pt.y = newPosition.y - m_spBloodBase->getContentSize().height * 0.5;
    m_lbHp->setPosition(pt);
}

const CCPoint& CPlayerBlood::getPosition()
{
    return m_spBloodBase->getPosition();
}

void CPlayerBlood::update(ccTime dt)
{
    m_fElapse += dt;
    
    if(m_fElapse < m_fNumChgDur)
        return;
    
    float fProgress = (m_fElapse - m_fNumChgDur) / 0.3;
    
    if(fProgress > 1.0){
        fProgress = 1.0;
        unscheduleUpdate();
    }
    
    int iNum = (m_iCurBlood - m_iStartNum) * fProgress + m_iStartNum;
    
    char buf[100];
    sprintf(buf, "%d/%d", iNum, m_iBloodBase);
    m_lbHp->setString(buf);
}

void CPlayerBlood::setCanFeiColor(bool canfei)
{
    if (canfei)
        m_lbHp->setColor(ccRED);
    else
        m_lbHp->setColor(ccWHITE);
}

void CPlayerBlood::canFei(bool bCanFei, int iBaseBlood)
{
    if (m_iBloodBase == iBaseBlood)
        return;
    
    m_iBloodBase = iBaseBlood;
    
    if (bCanFei)
        m_lbHp->setColor(ccRED);
    else
        m_lbHp->setColor(ccWHITE);

    char buf[100];
    sprintf(buf, "%d/%d", m_iCurBlood, m_iBloodBase);
    m_lbHp->setString(buf);

    float fScaleX = m_iCurBlood * 1.0 / m_iBloodBase;
    m_spBlood->stopAllActions();
    m_spBlood->setScaleX(fScaleX);
}

void CPlayerBlood::onExit()
{
    unscheduleAllSelectors();
    removeAllChildrenWithCleanup(true);
    
    CCNode::onExit();
}