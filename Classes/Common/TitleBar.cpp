//
//  TitleBar.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TitleBar.h"
#include "MainScene.h"
#include "TextNode.h"

TitleBar::~TitleBar()
{
//    CCLog("TitleBar::~TitleBar()");
}

void TitleBar::setTitleString(const char* str)
{
    removePreTitle();
    
    TextNode* titleLabel = TextNode::textWithString(str, CCSizeMake(300, 40), CCTextAlignmentLeft, 40);
    titleLabel->setShadowColor(ccBLACK);
    titleLabel->setAnchorPoint(CCPointMake(0.0, 0.5));
    titleLabel->setPosition(CCPointMake(60, 45));
    addChild(titleLabel);
    titleLabel->setTag(kTitleBarLabel);
}


void TitleBar::setTitleStringLab(const char* str)
{
    removePreTitle();
    
    CCLabelTTF* titleLabel = CCLabelTTF::labelWithString(str,
                                                         CCSizeMake(300, 40),
                                                         CCTextAlignmentLeft,
                                                         FONTSTHEITISCM, 40);
    titleLabel->setAnchorPoint(CCPointMake(0.0, 0.5));
    titleLabel->setPosition(CCPointMake(60, 45));
    addChild(titleLabel);
    titleLabel->setTag(kTitleBarLabel);
}



void TitleBar::SetTitlePosition(CCPoint pos)
{
    if (this->getChildByTag(kTitleBarLabel))
    {
        this->getChildByTag(kTitleBarLabel)->setAnchorPoint(CCPointZero);
        this->getChildByTag(kTitleBarLabel)->setPosition(pos);
    }
}

void TitleBar::setTitleSprite(const char *name)
{
    removePreTitle();
    
    CCSprite *titleSp = CCSprite::spriteWithSpriteFrameName(name);
    if(strlen(name)==0)
        return;
    if (!titleSp) {
        titleSp = CCSprite::spriteWithFile(name);
    }
    if(titleSp)
    {
        titleSp->setPosition(ccp(58, 45));
        titleSp->setAnchorPoint(ccp(0, 0.5));
        addChild(titleSp);
        titleSp->setTag(kTitleBarSprite);
    }
    else {
        TextNode* titleLabel = TextNode::textWithString(name,
                                              CCSizeMake(300, 40),
                                              CCTextAlignmentLeft,
                                              40);
        titleLabel->setAnchorPoint(CCPointMake(0.0, 0.5));
        titleLabel->setPosition(CCPointMake(60, 45));
        titleLabel->setShadowColor(ccBLACK);
        addChild(titleLabel);
        titleLabel->setTag(kTitleBarLabel);
    }
    
}

void TitleBar::removePreTitle()
{
    CCSprite *preSp = (CCSprite*)getChildByTag(kTitleBarSprite);
    if (preSp) {
        preSp->removeFromParentAndCleanup(true);
        preSp = NULL;
    }
    
    TextNode *preLb = (TextNode*)getChildByTag(kTitleBarLabel);
    if (preLb) {
        preLb->removeFromParentAndCleanup(true);
        preLb = NULL;
    }
}

TitleBar* TitleBar::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
    if(pFrame == NULL)
        return NULL;
    else{
        TitleBar *pobSprite = new TitleBar();
        if (pobSprite && pobSprite->initWithSpriteFrame(pFrame))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }
}

void TitleBar::onEnter()
{
    CCSprite::onEnter();
    
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1, true);
}

void TitleBar::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    CCSprite::onExit();
}

bool TitleBar::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCSize sz = getContentSizeInPixels();

    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(0,
                                                       0,
                                                       sz.width,
                                                       sz.height),
                                            pos);
    
    if (bRet) {
        CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyGoTop);
        return true;
    }
    else{
        return false;
    }
}

/////////////////////////////////////////

TitleBarNoAction* TitleBarNoAction::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
    if(pFrame == NULL)
        return NULL;
    else{
        TitleBarNoAction *pobSprite = new TitleBarNoAction();
        if (pobSprite && pobSprite->initWithSpriteFrame(pFrame))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }
}

void TitleBarNoAction::onEnter()
{
    CCSprite::onEnter();
    
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -2, true);
}

void TitleBarNoAction::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    CCSprite::onExit();
}

bool TitleBarNoAction::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCSize sz = getContentSizeInPixels();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(0,
                                                       0,
                                                       sz.width,
                                                       sz.height),
                                            pos);
    
    if (bRet)
        return true;
    else
        return false;
}
