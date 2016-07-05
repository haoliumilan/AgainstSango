//
//  FriendInvitationLayer.cpp
//  AgainstWar
//
//  Created by Hao Liu on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include <iostream>
#include "FriendInvitationLayer.h"
#include "TextNode.h"
#include "CGamedata.h"

FriendInvitationLayer::FriendInvitationLayer(CCObject* target, SEL_CallFunc selector) {
    m_pListener = target;
    m_pfnSelector = selector;
    isEnableShareBtn = true;
}

FriendInvitationLayer::~FriendInvitationLayer() {

}

void FriendInvitationLayer::onEnter() {
    CCLayer::onEnter();
}

void FriendInvitationLayer::onExit() {
    
    CCLayer::onExit();
}

void FriendInvitationLayer::showInvitationCode(const char *invitationCode) {
    CCSprite *spInvitationBg = CCSprite::spriteWithFile("invitation_bg.png");
    spInvitationBg->setPosition(CCPointMake(320, 440));
    addChild(spInvitationBg);
    
    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
    CCMenuItem *menuItemSina = CCMenuItemImage::itemFromNormalSprite(tempSprite1, 
                                                                     tempSprite2,
                                                                     this, 
                                                                     menu_selector(FriendInvitationLayer::inviteFriendFromWeibo));
    menuItemSina->setAnchorPoint(CCPointMake(0, 0));
    CCMenu *menu = CCMenu::menuWithItem(menuItemSina);
    menu->setPosition(CCPointMake(280, 505));
    addChild(menu);
    
    if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
        menuItemSina ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
        menuItemSina->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        
    }else {
        menuItemSina->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
    }
    
    
    
    TextNode *lbInvitationCode = TextNode::textWithString(invitationCode, CCSizeMake(500, 40), CCTextAlignmentLeft, 32);
    lbInvitationCode->setColor(ccRED);
    lbInvitationCode->setPosition(CCPointMake(550, 646));
    addChild(lbInvitationCode);
}

void FriendInvitationLayer::enableShareBtn(bool isok)
{
    isEnableShareBtn = isok;
}
void FriendInvitationLayer::inviteFriendFromWeibo() {
    if(isEnableShareBtn)
    (m_pListener->*m_pfnSelector)();
}

