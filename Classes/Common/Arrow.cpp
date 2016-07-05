//
//  Arrow.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Arrow.h"

//******************************************************************************
// Constructor
//******************************************************************************
Arrow::Arrow(int length, GLubyte Opacity)
{
    length = (length < 0)?0: length;
    length = (length > 640)? 640 : length;
    
    float fscale = (length + 1) / 50.0;
    
    CCSprite* spHead = CCSprite::spriteWithFile("fr_arrow1.png");
    CCSize szHead;
    if(spHead){
        spHead->setAnchorPoint(CCPointMake(0, 0.5));
        spHead->setPosition(CCPointMake(1, 0));
        addChild(spHead,1);
        //spHead->setOpacity(128);
        
        szHead = spHead->getContentSize();
    }
    
    CCSprite* spBody = CCSprite::spriteWithFile("fr_arrow2.png");
    if(spBody){
        spBody->setAnchorPoint(CCPointMake(0, 0.5));
        spBody->setPosition(CCPointMake(szHead.width, 0.0));
        addChild(spBody);
        spBody->setScaleX(fscale);
        //spBody->setOpacity(255);
    }
    
    CCSprite* spTail = CCSprite::spriteWithFile("fr_arrow3.png");
    if(spTail){
        spTail->setAnchorPoint(CCPointMake(0, 0.5));
        spTail->setPosition(CCPointMake(szHead.width + length, 0.0));
        addChild(spTail);
        //spTail->setOpacity(128);
    }
}

//******************************************************************************
// onExit
//******************************************************************************
void Arrow::onExit()
{
    CCLayer::onExit();
}