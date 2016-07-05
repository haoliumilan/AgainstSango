
//
//  CNotice.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CNotice.h"
#include "sys/utsname.h"
#include "CGamedata.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "OcProxy.h"

#define  kAgreeTag    81
#define  kRefuseTag   82
#define  kCoverTag    83
#define  kMzsmdgTag   84


static CNotice *m_cNotice = NULL;

using namespace CocosDenshion;


CNotice::CNotice(CCObject *listener,SEL_CallFuncO selector)
{
    m_cNotice = this;
    m_pListener = listener;
    m_pfnSelector = selector;
    touchTime = 0;
    
    coverLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 126), 640,960);
    addChild(coverLayer);
    coverLayer->setAnchorPoint(CCPoint(0.5, 0.5));
    coverLayer->setPosition(CCPoint(-1110, 480));
    
    noticeBack = CCSprite::spriteWithFile("noticeBoard.png");
    noticeBack->setPosition(ccp(320, 480));
    addChild(noticeBack);
    
    titleLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("noticeTip1"), 50);
    titleLabel->setPosition(ccp(320, 740));
    addChild(titleLabel,50);
    titleLabel->setColor(ccc3(122, 37, 8));
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CNotice::callBackMenu));
    m_fr_button = CCMenu::menuWithItem(sureItem);
    sureItem->setAnchorPoint(ccp(0, 0));
    m_fr_button->setPosition(CCPointMake(240, 95));
    addChild(m_fr_button);
    
    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setPosition(ccp(spMenu1->getContentSize().width * 0.5 - 2,
                               spMenu1->getContentSize().height * 0.5 + 2));
    itemLabel->setShadowColor(ccBLACK);
    sureItem->addChild(itemLabel);
    
}

void CNotice::callBackMenu(CCObject *pSender)
{
    m_ret = new CNoticeRet;
    m_ret->iBtnSel = 1;
    (m_pListener->*m_pfnSelector)(m_ret);
    
}

void CNotice::cbCheckMenu(CCObject *pSender)
{
    CCMenuItemImage *checkItem = (CCMenuItemImage *)pSender;
    if (checkItem->getTag() == kRefuseTag) {
        sureItem->getChildByTag(kCoverTag)->setIsVisible(false);
        checkItem->setTag(kAgreeTag);
        getChildByTag(kMzsmdgTag)->setIsVisible(true);
        sureItem->setIsEnabled(true);
    }else{
        sureItem->getChildByTag(kCoverTag)->setIsVisible(true);
        checkItem->setTag(kRefuseTag);
        getChildByTag(kMzsmdgTag)->setIsVisible(false);
        sureItem->setIsEnabled(false);
    }
}


void CNotice::onEnter()
{
    CCLayer::onEnter();
    
    scheduleUpdate();
}

void CNotice::update(ccTime dt)
{
    unscheduleUpdate();
    
    CCTouchDispatcher::sharedDispatcher()->setPriority(-160, m_fr_button);
}

void CNotice::onExit() {
    exit();
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(m_fr_button);
    CCLayer::onExit();
}

void CNotice::exit()
{
    
    
}

void CNotice::enterWithType(enNoticeType type)
{
    if (cureNoticeType == enNoticeTypeClaim) {
        checkMenu->removeAllChildrenWithCleanup(true);
        isRead->removeFromParentAndCleanup(true);
        noticeBack->initWithFile("noticeBoard.png");
        titleLabel->setPosition(ccp(320, 740));
        getChildByTag(kMzsmdgTag)->removeFromParentAndCleanup(true);
    }
    cureNoticeType = type;
    coverLayer->setPosition(CCPoint(0, 480));
    touchTime=0;
    char temp[2000];
    if (type == enNoticeTypeClaim) {
        sprintf(temp, "%s%ld",CGameData::Inst()->getCommonInfo()->mzsm_url.c_str(),CGameData::Inst()->getCurrentTime());
        
        noticeBack->initWithFile("mzsmBoard.png");
        
        titleLabel->setString(CGameData::Inst()->getLanguageValue("use_title"));
        titleLabel->setPosition(ccp(320, 680));
        
        CCSprite* spMenu3 = CCSprite::spriteWithFile("mzsmdgk.png");
        CCSprite* spMenu4 = CCSprite::spriteWithFile("mzsmdgk.png");
        
        checkItem = CCMenuItemImage::itemFromNormalSprite(spMenu3, spMenu4, this, menu_selector(CNotice::cbCheckMenu));
        checkMenu = CCMenu::menuWithItem(checkItem);
        checkItem->setTag(kRefuseTag);
        
        CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
        if(sz.width == 640||sz.width>1000)
            checkMenu->setPosition(CCPointMake(220, 225));
        else
            checkMenu->setPosition(CCPointMake(196, 200));
        addChild(checkMenu);
        
        CCSprite* mzsmDg = CCSprite::spriteWithFile("mzsmdg.png");
        mzsmDg->setScale(0.9);
        mzsmDg->setTag(kMzsmdgTag);
        mzsmDg->setIsVisible(false);
        mzsmDg->setPosition(CCPointMake(196, 200));
        addChild(mzsmDg);
        
        isRead = TextNode::textWithString(CGameData::Inst()->getLanguageValue("is_read"), 30);
        isRead->setPosition(ccp(340, 200));
        addChild(isRead);
        isRead->setColor(ccc3(122, 37, 8));
        
        
        CCSprite* black = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        black->setPosition(CCPointMake(78,42));
        black->setTag(kCoverTag);
        sureItem->addChild(black);
        sureItem->setIsEnabled(false);
    }
    else if(type == enNoticeTypeSystem)
    {
        sprintf(temp, "%s",CGameData::Inst()->getNoticeString());
        titleLabel->setString(CGameData::Inst()->getLanguageValue("noticeTip1"));
    }
    else if(type == enNoticeTypeGift)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("noticeTip2"));
        sprintf(temp, "%s",CGameData::Inst()->getNoticeGiftString());
        
    }else if(type == enNoticeTypeLendInfo)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("noticeTip3"));
        sprintf(temp, "%s",CGameData::Inst()->getLendInfo());
    }
    else if (type == enNoticeTypeInviteAward) {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("noticeTip4"));
        if(CGameData::Inst()->getNoticeInviteAward())
            sprintf(temp, "%s", CGameData::Inst()->getNoticeInviteAward());
        else
            sprintf(temp, "%s", " ");
    }
    
    
    
    if(type == enNoticeTypeSystem)
    {
        
       // OcProxy::Inst()->showNoticeWebViewWithType(enWebviewLoadTypeRequest, CGameData::Inst()->getUrlDepDevice(temp));
        
        OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                         CGameData::Inst()->getUrlDepDevice(temp),
                                         CCRect(81, 136*2, 240*2, 244*2),
                                         true);
    }else if(type == enNoticeTypeClaim){
        OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest, CGameData::Inst()->getUrlDepDevice(temp),CCRect(81, 170*2, 240*2, 190*2));
    }else
        
      //  OcProxy::Inst()->showNoticeWebViewWithType(enWebviewLoadTypeHtmlStr, temp);
    
        OcProxy::Inst()->showWebWithType(enWebviewLoadTypeHtmlStr, temp,CCRect(81, 136*2, 240*2, 244*2));
    
}
