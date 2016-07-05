//
//  TipsAwakeSkill.cpp
//  AgainstWar
//
//  Created by user on 13-10-29.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "TipsAwakeSkill.h"


TipsAwakeSkill::TipsAwakeSkill()
{
    
}

TipsAwakeSkill::~TipsAwakeSkill()
{
    
}

void TipsAwakeSkill::addTipString(char* icon, const char* strName, const char* Content, const char* idxTitle)
{
    float y = 82;
    
    TextNode* icon1 = TextNode::textWithString(idxTitle, 23);
    addChild(icon1, 10);
    icon1->setAnchorPoint(ccp(0, 0.5));
    icon1->setPosition(ccp(20,y));
    
    CCSprite* spIcon = CCSprite::spriteWithSpriteFrameName(icon);
    if (spIcon == NULL)
    {
        spIcon = CCSprite::spriteWithFile(icon);
    }
    addChild(spIcon,10);
    spIcon->setAnchorPoint(ccp(0, 0.5));
    spIcon->setPosition(ccp(20+icon1->getContentSize().width,y));
    
    TextNode* tName = TextNode::textWithString(strName, 23);
    addChild(tName, 10);
    tName->setAnchorPoint(ccp(0, 0.5));
    tName->setPosition(ccp(spIcon->getPosition().x+spIcon->getContentSize().width,y));
    
    TextNode* tContent = TextNode::textWithString(Content, CCSizeMake(getContentSize().width - 30, getContentSize().height - 10) , CCTextAlignmentLeft, 23);
    addChild(tContent, 10);
    tContent->setPosition(ccp((getContentSize().width - 30)/2 + 15, (getContentSize().height - 10)/2 - 12));
}


TipsAwakeSkill* TipsAwakeSkill::tipWithContent(char* icon, const char* strName, const char* Content, const char* idxTitle)
{
    TipsAwakeSkill* ret = new TipsAwakeSkill();
    if (ret && ret->initWithSpriteFrameName("propertyTip_dlg.png"))
    {
        ret->autorelease();
        
        ret->addTipString(icon, strName, Content, idxTitle);
        
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}