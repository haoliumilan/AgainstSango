//
//  UseResultDialog.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-4-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "UseResultDialog.h"
#include "TextNode.h"
#include "CGameData.h"


UseResultDialog::UseResultDialog(CCObject* target, SEL_MenuHandler selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

UseResultDialog::~UseResultDialog()
{

}

void UseResultDialog::onEnter()
{
    CCLayer::onEnter();
}

void UseResultDialog::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeAllTextures();
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);

    CCLayer::onExit();
}

void UseResultDialog::showTextDialog(enUseCardType useCardType,int useCardCount)
{
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 480));
    
    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_item_result"),
                                                 CCSizeMake(600, 30),
                                                 CCTextAlignmentCenter,
                                                 45);
    tnTitle->setPosition(CCPointMake(320, 570));
    tnTitle->setColor(ccBLACK);
    addChild(tnTitle);

    char buf[100];
    switch (useCardType) {
        case enUseCardType_exp:
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cntry_item_result_exp"),
                     CGameData::Inst()->getUseItemAward()->exp);
            break;

        case enUseCardType_gold:
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cntry_item_result_gold"),
                     CGameData::Inst()->getUseItemAward()->gold);
            break;

        case enUseCardType_coin:
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cntry_item_result_coin"),
                     CGameData::Inst()->getUseItemAward()->coin);
            break;

        case enUseCardType_supply:
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cntry_item_result_supply"),
                     CGameData::Inst()->getUseItemAward()->wealth);
            break;

        case enUseCardType_stamina:
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cntry_item_result_stamina"),
                     CGameData::Inst()->getUseItemAward()->stamina);
            break;
            
        case enUseCardType_cutCd:
            snprintf(buf, 99,"%s",CGameData::Inst()->getLanguageValue("cntry_item_result_cutcd"));
            break;
            
        case enUseCardType_rename:
            snprintf(buf, 99, "%s",CGameData::Inst()->getLanguageValue("cntry_item_result_rename"));
            break;
            
        case enUseCardType_gacha:
            snprintf(buf, 99,"%s",CGameData::Inst()->getLanguageValue("cntry_item_result_gacha"));
            break;
            
        case enUseCardType_gacha10:
            snprintf(buf, 99, "%s",CGameData::Inst()->getLanguageValue("cntry_item_result_gacha10"));
            break;
            
        case enUseCardType_stone:
            snprintf(buf, 99, "%s",CGameData::Inst()->getLanguageValue("cntry_donate_ok"));
            break;
            
        case enUseCardType_cost_up:
        {
             snprintf(buf, 99, "%s",CGameData::Inst()->getLanguageValue("cntry_use_cost_up_success"));
            break;
        }
        default:
            break;
    }
    
    TextNode* tnDsp = TextNode::textWithString(buf,
                                               CCSizeMake(600, 80),
                                               CCTextAlignmentCenter,
                                               30);
    tnDsp->setPosition(CCPointMake(320, 500));
    tnDsp->setColor(ccBLACK);
    addChild(tnDsp);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                m_pListener,
                                                                m_pfnSelector);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     360));
    
    TextNode* tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    mItemOK->addChild(tempText);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void UseResultDialog::showImageDialog()
{
    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 480));

    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_item_result"),
                                                 CCSizeMake(600, 30),
                                                 CCTextAlignmentCenter,
                                                 45);
    tnTitle->setPosition(CCPointMake(320, 650));
    tnTitle->setColor(ccBLACK);
    addChild(tnTitle);
    
    list<string> ucidArr = CGameData::Inst()->getUseItemAward()->strUcidLst;
    
    CCSprite* spNewCard = NULL;
    int i = 0;
    for (list<string>::iterator ucidIt = ucidArr.begin();
         ucidIt != ucidArr.end();
         ++ucidIt) {
        if (i > 9)
            break;
        
        string cardUcid = *ucidIt;
        CUserCardInfo* userCard = CGameData::Inst()->getUsrCardInfByUcid(cardUcid.c_str());
        
        if (userCard == NULL)
            continue;
        
        spNewCard = CGameData::Inst()->getHeadSprite(userCard->cid);
        spNewCard->setScale(0.8);
        spNewCard->setPosition(CCPointMake(120 + (i % 5) * 100,
                                           530 - (i / 5) * 100));
        addChild(spNewCard);

        i++;
    }

    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                m_pListener,
                                                                m_pfnSelector);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     260));
    
    TextNode* tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    mItemOK->addChild(tempText);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void UseResultDialog::showUseResultDialog(string strType,int useCardCount)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    enUseCardType useCardType;
    if (strType == "coin")
        useCardType = enUseCardType_coin;
    else if (strType == "gold")
        useCardType = enUseCardType_gold;
    else if (strType == "exp")
        useCardType = enUseCardType_exp;
    else if (strType == "wealth")
        useCardType = enUseCardType_supply;
    else if (strType == "stamina")
        useCardType = enUseCardType_stamina;
    else if (strType == "card")
        useCardType = enUseCardType_food;
    else if (strType == "cd")
        useCardType = enUseCardType_cutCd;
    else if (strType == "rename")
        useCardType = enUseCardType_rename;
    else if (strType == "gacha")
        useCardType = enUseCardType_gacha;
    else if (strType == "gacha_multi")
        useCardType = enUseCardType_gacha10;
    else if (strType == "stone")
        useCardType = enUseCardType_stone;
    else if (strType == "cost_up")
        useCardType = enUseCardType_cost_up;
    
    
    if (useCardType == enUseCardType_food)
        showImageDialog();
    else
        showTextDialog(useCardType,useCardCount);
}

#pragma mark -
#pragma mark CCTouchDelegate
bool UseResultDialog::ccTouchBegan(CCTouch* touch, CCEvent* event)
{    
    return true;
}



