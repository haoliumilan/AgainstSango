//
//  ShowSelCountry.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "ShowSelCountry.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "CGamedata.h"

using namespace CocosDenshion;

#define kShowSelCtryBtnOkTag        0xfa
#define kShowSelCtryBtnCancelTag    0xfb
//******************************************************************************
// Constructor
//******************************************************************************
ShowSelCountry::ShowSelCountry(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

//******************************************************************************
// show
//******************************************************************************
void ShowSelCountry::show(int iCountry)
{
    removeAllChildrenWithCleanup(true);
    
    CCSprite* sp = CCSprite::spriteWithFile("fr_light.png");
    if(sp){
        sp->setScale(4.0);
        addChild(sp);
        sp->setPosition(CCPointMake(320, 530));
        
        CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCScaleTo::actionWithDuration(1.0, 4.0 * 1.2),
                                                                          CCScaleTo::actionWithDuration(1.0, 4.0 * 1.0),
                                                                          NULL);
        sp->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    //ok button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* okBtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ShowSelCountry::btnCallback) );
    okBtn->setTag(kShowSelCtryBtnOkTag);
    okBtn->setAnchorPoint(CCPointZero);
    okBtn->setPosition(CCPointMake(108, 125));
    
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(108, 125));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    //cancel button
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* cancelBtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ShowSelCountry::btnCallback) );
    cancelBtn->setTag(kShowSelCtryBtnCancelTag);
    cancelBtn->setAnchorPoint(CCPointZero);
    cancelBtn->setPosition(CCPointMake(355, 125));
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(355, 125));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    CCMenu* menu = CCMenu::menuWithItems(okBtn, cancelBtn, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCSprite* spBody;
    CCSprite* spStone;
    switch (iCountry) {
        case 1:
            spBody = CCSprite::spriteWithFile("fr_caocao.png");
            spStone = CCSprite::spriteWithSpriteFrameName("stone1.png");
            
            lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip2"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
            break;
            
        case 2:
            spBody = CCSprite::spriteWithFile("fr_liubei.png");
            spStone = CCSprite::spriteWithSpriteFrameName("stone2.png");
            
            lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip1"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
            break;
            
        case 3:
            spBody = CCSprite::spriteWithFile("fr_sunquan.png");
            spStone = CCSprite::spriteWithSpriteFrameName("stone3.png");
            
            lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip3"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
            break;
            
        default:
            break;
    }
    
    if(spBody){
        addChild(spBody, 1);
        spBody->setPosition(CCPointMake(320, 525));
    }
    
    if(spStone){
        addChild(spStone, 1);
        spStone->setPosition(CCPointMake(260, 790));
        spStone->setScale(0.5);
    }
    
    //名字
    lbText->setPosition(CCPointMake(330, 790));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    //是否确定选择此属性主公
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip6"), CCSizeMake(640, 80), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(320, 240));
    lbText->setColor(ccWHITE);
    addChild(lbText, 2);
    
    //影子
    sp = CCSprite::spriteWithFile("fr_shadow.png");
    if(sp){
        addChild(sp);
        sp->setPosition(CCPointMake(320, 292));
    }
}

//******************************************************************************
// show
//******************************************************************************
void ShowSelCountry::btnCallback(CCObject* sender)
{
    CCMenuItem* pMI = (CCMenuItem*) sender;
    
    switch (pMI->getTag()) {
        case kShowSelCtryBtnOkTag:
            m_ret.bOk = true;
            break;
            
        case kShowSelCtryBtnCancelTag:
            m_ret.bOk = false;
            break;
            
        default:
            break;
    }
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}

//******************************************************************************
// onExit
//******************************************************************************
void ShowSelCountry::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}