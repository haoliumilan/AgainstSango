//
//  EvoOverShow.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-1.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "EvoOverShow.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "TextNode.h"
#include "CShowCardInf.h"

//******************************************************************************
// Constructor
//******************************************************************************
EvoOverShow::EvoOverShow(CCObject* target, SEL_CallFunc selector, int iEvoType)
{
    CGameData::Inst()->addBigImageByRGBA4444("evoluteAni.plist");
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    m_pListener = target;
    m_pfnSelector = selector;
    
    int cid_before = CGameData::Inst()->getCidBeforeEvo();
    int cid_after = CGameData::Inst()->getCidAfterEvo();
    
    CCSprite* sp = CCSprite::spriteWithFile("show_bg.jpg");
    if(sp != NULL){
        sp->setScale(1/0.8);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp);
    }
    
    switch (iEvoType) {
        case enEvoType_Nml:
            nmlEvoAnim(cid_before, cid_after);
            break;
            
        case enEvoType_Sevo:
            superEvoAnim(cid_before, cid_after);
            break;
            
        case enEvoType_Wake:
            wakeEvoAnim(cid_before, cid_after);
            break;
            
        default:
            break;
    }
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
}

//******************************************************************************
// nmlEvoAnim
//******************************************************************************
void EvoOverShow::nmlEvoAnim(int src_cid, int tgt_cid)
{
    char buf[100];
    CCSprite* sp = CGameData::Inst()->getBodySprite(src_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5f),
                                          CCFadeOut::actionWithDuration(1.5f),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showTorchParticle)),
                                          NULL));
    }
    
    CCAnimation* animation;
	CCActionInterval*  action;
    CCSpriteFrame* frame;
    
    animation = CCAnimation::animation();
    for (int i = 1; i <= 16; i++) {
        snprintf(buf, 99, "shine00%02d.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
        animation->addFrame(frame);
    }
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(1.0, animation, false);
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        sp->setScale(4.0f);
        addChild(sp, 1);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5f),
                                          action,
                                          NULL));
    }
    
    
    
    //光效
    sp = CCSprite::spriteWithFile("evolutionlight.png");
    if(sp != NULL){
        sp->setScale(4.0);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 11);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.0f),
                                          CCFadeIn::actionWithDuration(1.0f),
                                          CCDelayTime::actionWithDuration(0.3f),
                                          CCFadeOut::actionWithDuration(0.5f),
                                          NULL));
    }
    
    
    //新产品
    sp = CGameData::Inst()->getBodySprite(tgt_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp, 10);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                          CCFadeIn::actionWithDuration(1.0),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showCardInfo)),
                                          NULL));
    }
}

//******************************************************************************
// superEvoAnim
//******************************************************************************
void EvoOverShow::superEvoAnim(int src_cid, int tgt_cid)
{
    CGameData::Inst()->addBigImageByRGBA4444("superII.plist");
    CGameData::Inst()->addBigImageByRGBA4444("superlight.plist");
    
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255, 0), 640, 960);
    if (lc) {
        addChild(lc, 10);
        
        lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.3),
                                          CCFadeIn::actionWithDuration(0.4),
                                          CCDelayTime::actionWithDuration(0.2),
                                          CCFadeOut::actionWithDuration(0.2),
                                          NULL));
    }
    
    
    //光效
    CCSprite* sp = CCSprite::spriteWithFile("evolutionlight.png");
    if(sp != NULL){
        sp->setScale(4.0);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 9);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.1f),
                                          CCFadeIn::actionWithDuration(0.3f),
                                          CCDelayTime::actionWithDuration(0.5f),
                                          CCFadeOut::actionWithDuration(0.3f),
                                          NULL));
    }
    
    //animation 1
    CCActionInterval* action = getActionFromPlist("superlignt00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 480));
        sp->setScale(2.0f);
        addChild(sp, 8);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.8f),
                                          action,
                                          NULL));
    }
    
    //animation 2
    action = getActionFromPlist("superII00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        sp->setScale(2.0f);
        addChild(sp, 7);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.9f),
                                          action,
                                          NULL));
    }
    
    //animation star
    action = getActionFromPlist("star00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        sp->setScale(2.0f);
        addChild(sp, 6);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.9f),
                                          action,
                                          NULL));
    }
    
    //源武将
    sp = CGameData::Inst()->getBodySprite(src_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp, 5);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.6),
                                          CCHide::action(),
                                          NULL));
    }
    
    //新产品
    sp = CGameData::Inst()->getBodySprite(tgt_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp, 4);
        sp->setIsVisible(false);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.7),
                                          CCShow::action(),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showTorchParticle)),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showCardInfo)),
                                          NULL));
    }
}

//******************************************************************************
// wakeEvoAnim
//******************************************************************************
void EvoOverShow::wakeEvoAnim(int src_cid, int tgt_cid)
{
    CGameData::Inst()->addBigImageByRGBA4444("superII.plist");
    CGameData::Inst()->addBigImageByRGBA4444("wakeanim.plist");
    
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255, 0), 640, 960);
    if (lc) {
        addChild(lc, 10);
        
        lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.3),
                                          CCFadeIn::actionWithDuration(0.4),
                                          CCDelayTime::actionWithDuration(0.2),
                                          CCFadeOut::actionWithDuration(0.2),
                                          NULL));
    }
    
    
    //光效
    CCSprite* sp = CCSprite::spriteWithFile("evolutionlight.png");
    if(sp != NULL){
        sp->setScale(4.0);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 9);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.1f),
                                          CCFadeIn::actionWithDuration(0.3f),
                                          CCDelayTime::actionWithDuration(0.5f),
                                          CCFadeOut::actionWithDuration(0.3f),
                                          NULL));
    }
    
    //animation 1
    CCActionInterval* action = getActionFromPlist("wakeanim00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 480));
        sp->setScale(2.0f);
        addChild(sp, 8);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.8f),
                                          action,
                                          NULL));
    }
    
    //animation 2
    action = getActionFromPlist("superII00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        sp->setScale(2.0f);
        addChild(sp, 7);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.9f),
                                          action,
                                          NULL));
    }
    
    //animation star
    action = getActionFromPlist("star00%02d.png");
    
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        sp->setScale(2.0f);
        addChild(sp, 6);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.9f),
                                          action,
                                          NULL));
    }
    
    //源武将
    sp = CGameData::Inst()->getBodySprite(src_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp, 5);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.6),
                                          CCHide::action(),
                                          NULL));
    }
    
    //新产品
    sp = CGameData::Inst()->getBodySprite(tgt_cid);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 400));
        addChild(sp, 4);
        sp->setIsVisible(false);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.7),
                                          CCShow::action(),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showTorchParticle)),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(EvoOverShow::showCardInfo)),
                                          NULL));
    }
}

//******************************************************************************
// showCardInfo
//******************************************************************************
void EvoOverShow::showCardInfo()
{
    const char* newCardUcid = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)newCardUcid);
    
    m_showCard->setOnExitCB(this, callfunc_selector(EvoOverShow::cbShowCardInfo));
    m_showCard->show(pUsrCardInfo);
}

void EvoOverShow::cbShowCardInfo()
{
    (m_pListener->*m_pfnSelector)();
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool EvoOverShow::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    (m_pListener->*m_pfnSelector)();
    return true;
}

//******************************************************************************
// getActionFromFile
//****************************************************************
CCActionInterval* EvoOverShow::getActionFromFile(const char* filename)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithFile(buf);
        if (tmpSP)
            animation->addFrameWithFileName(buf);
        else
            break;
        
        i++;
    }
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/20.0, animation, false);
    
    return action;
}

CCActionInterval* EvoOverShow::getActionFromPlist(const char* filename)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    CCSpriteFrame* frame;
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithSpriteFrameName(buf);
        if (tmpSP){
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
            animation->addFrame(frame);
        }
        else
            break;
        
        i++;
    }
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/20.0, animation, false);
    
    return action;
}

//******************************************************************************
// showTorchParticle
//****************************************************************
void EvoOverShow::showTorchParticle() {
    CCParticleTorch *pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(50, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch);
    
    pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(590, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch);
}

//******************************************************************************
// onExit
//******************************************************************************
void EvoOverShow::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("evoluteAni.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("superII.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("superlight.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("wakeanim.plist");
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    CCLayer::onExit();
}