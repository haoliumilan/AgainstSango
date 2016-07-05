//
//  NoFriendLayer.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-20.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "NoFriendLayer.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"

NoFriendLayer::NoFriendLayer(CCObject *listener,SEL_CallFuncO selector)

{
    m_listener = listener;
    m_selector = selector;

    CCPoint point ;
    m_background = CCSprite::spriteWithFile("fr_window.png");
    m_background->setScale(2.0);
    addChild(m_background);
    point = m_background->getPosition();
    point.y+=130;
    m_background->setPosition(point);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem *backItem =  CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this,callfuncO_selector(NoFriendLayer::menuCallback));
    backItem->setAnchorPoint(ccp(0, 0));
    backItem->setTag(11);
    
    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));

    backItem->addChild(itemLabel);
        
    CCMenu *menu = CCMenu::menuWithItem(backItem);
    addChild(menu);
    menu->setAnchorPoint(ccp(0, 0));
//    point = menu->getPosition();
//    point.y-=250;
//    point.x-=225;
    menu->setPosition(ccp(-80, 0));
    
    infoLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("noFriendLayerTip1"), 30);
    //  label->setPosition(ccp(320, 600));
    infoLabel->setColor(ccc3(122, 37, 8)); 
    addChild(infoLabel);
    point = infoLabel->getPosition();
    point.y+=180;
    infoLabel->setPosition(point);

}

void NoFriendLayer::setInfoLabel(const char* labelStr)
{
    infoLabel->setString(labelStr);
}
void NoFriendLayer::menuCallback()
{
    m_ret.iBSel = 11;
    (m_listener->*m_selector)(&m_ret);
}