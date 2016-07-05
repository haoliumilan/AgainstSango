//
//  MemberInfoLayer.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-1-9.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "MemberInfoLayer.h"
#include "TextNode.h"
#include "CommonDef.h"
#include "CMenuBtn.h"

MemberInfoLayer::MemberInfoLayer(CCObject* target, SEL_MenuHandler selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_memLayerType = memLayerType_none;
}

MemberInfoLayer::~MemberInfoLayer()
{

}

void MemberInfoLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void MemberInfoLayer::onExit()
{
    
    CCLayer::onExit();
}

MemberInfoLayerType MemberInfoLayer::getMemLayerType()
{
    return m_memLayerType;
}

void MemberInfoLayer::showSameInfoLayer()
{
    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    spBg->setScale(2.0f);
    spBg->setPosition(CCPointMake(320, 490));
    addChild(spBg);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    
    CCMenuItem* sureItem;
    newBtn(sureItem,
           CGameData::Inst()->getLanguageValue("sureTip"),
           ccp(200 - spMenu1->getContentSize().width * 0.5, 270),
           enMemLayBtn_OK);
    
    CCMenuItem* cancelItem;
    newBtn(cancelItem,
           CGameData::Inst()->getLanguageValue("cancelTip"),
           ccp(440 - spMenu1->getContentSize().width * 0.5, 270),
           enMemLayBtn_Cancel);
    
    CCMenu *menu = CCMenu::menuWithItems(sureItem, cancelItem, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
        
    TextNode* tnTitle = TextNode::textWithString("",
                                                 CCSizeMake(600, 50),
                                                 CCTextAlignmentCenter,
                                                 55);
    tnTitle->setPosition(CCPointMake(320, 660));
    tnTitle->setColor(ccc3(122, 37, 8));
    addChild(tnTitle);
    
    TextNode* tnInfo = TextNode::textWithString("",
                                                CCSizeMake(600, 50),
                                                CCTextAlignmentCenter,
                                                30);
    tnInfo->setColor(ccBLACK);
    tnInfo->setPosition(CCPointMake(320, 450));
    addChild(tnInfo);
    
    setTitleString(tnTitle, tnInfo);
}

void MemberInfoLayer::showMemberInfoLayer(MemberInfoLayerType memInfoLayerType, CMember *memberInfo)
{
    m_memLayerType = memInfoLayerType;
    
    showSameInfoLayer();
    
    CMenuBtn *memInfoBtn = new CMenuBtn(memberInfo);
    memInfoBtn->setPosition(CCPointMake(320, 560));
    addChild(memInfoBtn);
}

void MemberInfoLayer::showCntryInfoLayer(MemberInfoLayerType memInfoLayerType, CCntryBaseInfo *cntryInfo)
{
    m_memLayerType = memInfoLayerType;
    
    showSameInfoLayer();
    
    CMenuBtn *cntryInfoBtn = new CMenuBtn(cntryInfo);
    cntryInfoBtn->setPosition(CCPointMake(320, 580));
    addChild(cntryInfoBtn);
}

void MemberInfoLayer::showFriendInfoLayer(MemberInfoLayerType memInfoLayerType, CFriendInf *friendInfo)
{
    m_memLayerType = memInfoLayerType;
    
    showSameInfoLayer();
    
    CMenuBtn *memInfoBtn = new CMenuBtn(friendInfo, enFriendRangePro);
    memInfoBtn->setPosition(CCPointMake(320, 580));
    addChild(memInfoBtn);
}

void MemberInfoLayer::setTitleString(TextNode* &tnTitle, TextNode* &tnInfo)
{
    switch (m_memLayerType) {
        case memLayerType_accept:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_accept"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_accept0"));
            break;
            
        case memLayerType_refuse:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_deny"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_deny0"));
            break;
        
        case memLayerType_applyFriend:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip7"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip8"));
            break;
        
        case memLayerType_delete:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_delete"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_delete0"));
            break;
            
        case memLayerType_setViceleader:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_vice"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_vice0"));
            break;
            
        case memLayerType_deleteViceleader:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_fire_pre"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_fire_pre0"));
            break;
            
        case memLayerType_giveUpLeader:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_giveup"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_giveup0"));
            break;
            
        case memLayerType_applyCountry:
            tnTitle->setString(CGameData::Inst()->getLanguageValue("cntry_info_apply"));
            tnInfo->setString(CGameData::Inst()->getLanguageValue("cntry_info_apply0"));
            break;
                    
        default:
            break;
    }
}

void MemberInfoLayer::newBtn(CCMenuItem *&menuItem, const char *label,
                         const CCPoint& position, int tag)
{
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    menuItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                     spMenu2,
                                                     m_pListener,
                                                     m_pfnSelector);
    menuItem->setTag(tag);
    menuItem->setAnchorPoint(CCPointZero);
    menuItem->setPosition(position);
    
    TextNode* tempText = TextNode::textWithString(label,
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  kBtnTitleHeight);
    tempText->setShadowColor(ccBLACK);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    menuItem->addChild(tempText);
}


