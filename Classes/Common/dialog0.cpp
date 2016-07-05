//
//  dialog0.cpp
//  AgainstWar
//
//  Created by caojiye on 13-2-4.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "dialog0.h"
#include "TextNode.h"
#include "CGameData.h"

Dialog0::Dialog0(CCObject* target, SEL_CallFunc selector, const char* dsp)
{
    m_pListener   = target;
    m_pfnSelector = selector;
    m_content     = dsp;
    this->SetBackground();
}
void Dialog0::SetBackground()
{
    CCSprite* sp_bg = CCSprite::spriteWithSpriteFrameName("commdlg.png");
    if(sp_bg)
    {
        addChild(sp_bg);
        sp_bg->setPosition(CCPointMake(0, 50));
    }
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog0::cbSure));
    char buf[20];
    snprintf(buf, 19, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
    TextNode* itemLabel = TextNode::textWithString(buf, CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(-10, 20));
    itemLabel->setAnchorPoint(CCPointZero);
    sureItem->addChild(itemLabel);
    sureItem->setPosition(CCPointMake(140, 20));
    sureItem->setAnchorPoint(ccp(0, 0));
    CCMenu *menu = CCMenu::menuWithItem(sureItem);
    menu->setPosition(CCPointZero);
    menu->setAnchorPoint(ccp(0, 0));
    sp_bg->addChild(menu);
    
    TextNode* text0 = TextNode::textWithString(m_content, CCSizeMake(500, 100), CCTextAlignmentCenter, kBtnTitleHeight-10);
    text0 -> setPosition(CCPointMake(sp_bg->getContentSize().width/2, sp_bg->getContentSize().height/2));
    sp_bg -> addChild(text0);
    
    
    
}
void Dialog0::cbSure()
{
    (m_pListener->*m_pfnSelector)();
}














