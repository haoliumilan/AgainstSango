//
//  FeatureAnim.cpp
//  AgainstWar
//
//  Created by user on 13-7-29.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "FeatureAnim.h"


FeatureAnim::FeatureAnim(const char* imgName,bool bTemp/* = false*/)
{
    m_roler = NULL;
    m_imgName = imgName;
    m_bTemp = bTemp;
    CreateAnim();
}


FeatureAnim::~FeatureAnim()
{
    
}


void FeatureAnim::CreateAnim()
{
//    CCSprite* animBG;
//    CCSprite* animInk;
    CCSprite* rainbow;
    CCSprite* effect;
    CCActionInterval*  action;
    
//    CCAnimation* animationBG = CCAnimation::animation();
//    CCSpriteFrame* frameBG;
//    frameBG = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0008.png");
//    animationBG->addFrame(frameBG);
//    frameBG = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0009.png");
//    animationBG->addFrame(frameBG);
//    frameBG = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying00010.png");
//    animationBG->addFrame(frameBG);
//    frameBG = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying00011.png");
//    animationBG->addFrame(frameBG);
//    frameBG = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
//    animationBG->addFrame(frameBG);
//    action = CCAnimate::actionWithDuration(1.0 * 5 / 20, animationBG, false);
//    
//    animBG = CCSprite::spriteWithSpriteFrameName("blank.png");
//  //          addChild(animBG,0);
//    animBG->setAnchorPoint(CCPointZero);
//    animBG->setPosition(CCPointZero);
//    animBG->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.0 * 8 / 20),
//                                                  action,
//                                                  NULL));
//    
//    CCAnimation* animationInk= CCAnimation::animation();
//    CCSpriteFrame* frameInk;
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0001.png");
//    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0002.png");
//    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0003.png");
//    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0004.png");
//    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0005.png");
//    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0006.png");
//    animationInk->addFrame(frameInk);
////    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("heiying0007.png");
////    animationInk->addFrame(frameInk);
//    frameInk = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
//    animationInk->addFrame(frameInk);
//    action = CCAnimate::actionWithDuration(1.0 * 8 / 20, animationInk, false);
//    
//    animInk = CCSprite::spriteWithSpriteFrameName("blank.png");
////    addChild(animInk,1);
//    animInk->setAnchorPoint(CCPointZero);
//    animInk->setPosition(CCPointZero);
//    animInk->runAction(action);
    
    CCAnimation* animationFendi= CCAnimation::animation();
    CCSpriteFrame* frameFendi;
    frameFendi = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fendi.png");
    animationFendi->addFrame(frameFendi);
    action = CCAnimate::actionWithDuration(1.0 * 1 / 20, animationFendi, false);
    rainbow = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(rainbow);
    rainbow->setAnchorPoint(CCPointZero);
    rainbow->setPosition(CCPointZero);
    CCAction* action2 = CCFadeOut::CCFadeOut::actionWithDuration(1.0 * 5 /20);
    rainbow->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.0 * 7 / 20),
                                          action,
                                           CCDelayTime::actionWithDuration(1.0 * 18 /20),
                                           action2,
                                          NULL));

    CCAnimation* animationEff= CCAnimation::animation();
    CCSpriteFrame* frameEff;
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0002.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0003.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0004.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0002.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0003.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0004.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0002.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0003.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0004.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0002.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0003.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0004.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0002.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0003.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0004.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0005.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("baisecaca0001.png");
    animationEff->addFrame(frameEff);
    frameEff = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animationEff->addFrame(frameEff);
    action = CCAnimate::actionWithDuration(1.0 * 28 / 20, animationEff, false);
    
    effect = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(effect,2);
    effect->setAnchorPoint(CCPointZero);
    effect->setPosition(CCPointZero);
    effect->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.35f),
                                           action,
                                           NULL));

    if (m_bTemp)
    {
        CCTexture2DPixelFormat emDefault = CCTexture2D::defaultAlphaPixelFormat();
        CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
        m_roler = CCSprite::spriteWithFile(m_imgName);
        CCTexture2D::setDefaultAlphaPixelFormat(emDefault);
    }
    else
    {
        m_roler = CCSprite::spriteWithFile(m_imgName);
    }
    
    //m_roler->setScale(0.);
    m_roler->setAnchorPoint(CCPointMake(0.5, 0.5));
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fendi.png");
    CCSize size = sp->getContentSize();
    
    m_roler->setPosition(CCPointMake(size.width/2-220, size.height/2+0));
    addChild(m_roler,3);
    m_roler->setOpacity(0);
    
    
    //CCScaleTo* scale_big = CCScaleTo::actionWithDuration(1.0* 5 /20, 1.0);
    CCMoveTo* move_to = CCMoveTo::actionWithDuration(1.0* 5 /20, CCPointMake(size.width/2, size.height/2));
    //CCAction* spawn = CCSpawn::actions(scale_big,move_to,NULL);
    CCAction* fadeout = CCFadeOut::actionWithDuration(1.0* 5 /20);
    

    m_roler->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                           CCFadeIn::actionWithDuration(0.1),
                                         move_to,
                                         CCDelayTime::actionWithDuration(1.0 * 22 / 20),
                                         fadeout,
                                         CCDelayTime::actionWithDuration(1.0 * 3 / 20),
                                        // CCCallFunc::actionWithTarget(this, callfunc_selector(FeatureAnim::HideRoler)),
                                         NULL));
    
    

    
}

void FeatureAnim::showRoler()
{

}

void FeatureAnim::HideRoler()
{
//    stopAllActions();
//    removeFromParentAndCleanup(true);
}



