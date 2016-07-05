//
//  CDKeyExchange.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-4-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "CDKeyExchange.h"
#include "TextNode.h"
#include "CGameData.h"
#include "jsoncpp.h"

#define enCDKeyBtn_Shadow           2001
#define enCDKeyBtn_Exchange         2002
#define enCDKeyBtn_Back             2003

CDKeyExchange::CDKeyExchange(CCObject* target, SEL_MenuHandler selector)
{
    m_textField = NULL;
    m_mItemExchange = NULL;
    m_pfnSelector = selector;
    m_pListener = target;
}

CDKeyExchange::~CDKeyExchange()
{

}

void CDKeyExchange::onEnter()
{
    CCLayer::onEnter();
    
}

void CDKeyExchange::onExit()
{
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
    hideKeyboard();
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

void CDKeyExchange::showCDKeyExchangeDlg()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    addChild(spBg);
    
    CCSprite* spTitle = CCSprite::spriteWithFile("text_get_reward.png");
    spTitle->setPosition(CCPointMake(320, 640));
    addChild(spTitle);
    
    CCSprite* spTextBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spTextBg->setPosition(CCPointMake(320, 570));
    addChild(spTextBg);
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder(CGameData::Inst()->getLanguageValue("cdkey_placeholder"),
                                                           spTextBg->getContentSize(),
                                                           CCTextAlignmentCenter,
                                                           "huakangfont.ttf",
                                                           26);
    m_textField->setDelegate(this);
    m_textField->setPosition(CCPointMake(spTextBg->getContentSize().width * 0.5,
                                         spTextBg->getContentSize().height * 0.5));
    spTextBg->addChild(m_textField);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemExchange = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                            spMenu2,
                                                            m_pListener,
                                                            m_pfnSelector);
    m_mItemExchange->setAnchorPoint(CCPointZero);
    m_mItemExchange->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                           440));
    m_mItemExchange->setTag(enCDKeyBtn_Exchange);
    m_mItemExchange->setIsEnabled(false);
    
    TextNode* tnOk = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                              spMenu1->getContentSize(),
                                              CCTextAlignmentCenter,
                                              40);
    tnOk->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                  spMenu1->getContentSize().height * 0.5));
    tnOk->setShadowColor(ccBLACK);
    m_mItemExchange->addChild(tnOk);
    
    CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
    spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    m_mItemExchange->addChild(spShadow, 1, enCDKeyBtn_Shadow);
    
    CCMenu* menu = CCMenu::menuWithItem(m_mItemExchange);
    menu->setPosition(CCPointZero);
    addChild(menu);
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    showKeyboard();

}

const char* CDKeyExchange::getGiftContent()
{
    GiftInfo* pGiftInfo = CGameData::Inst()->getGiftInfo();

    static string strGift;
    strGift.clear();
    char buf[100];
    bool bFirst = true;
    if (pGiftInfo->coin > 0)
    {
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cdkey_result_coin"),
                 pGiftInfo->coin);
        strGift.append(buf);
        bFirst = false;
    }
    
    if (pGiftInfo->gold > 0)
    {
        if (bFirst)
            bFirst = false;
        else
            strGift.append("、");
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cdkey_result_gold"),
                 pGiftInfo->gold);
        strGift.append(buf);
    }
    
    if (pGiftInfo->wealth > 0)
    {
        if (bFirst)
            bFirst = false;
        else
            strGift.append("、");
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cdkey_result_wealth"),
                 pGiftInfo->wealth);
        strGift.append(buf);
    }

    if (pGiftInfo->gacha_pt > 0)
    {
        if (bFirst)
            bFirst = false;
        else
            strGift.append("、");
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cdkey_result_gacha_pt"),
                 pGiftInfo->gacha_pt);
        strGift.append(buf);
    }
    
    Json::Value giftCardsDic;
    
    if (pGiftInfo->strUcidList.size() > 0)
    {
        for (list<string>::iterator ucidIt = pGiftInfo->strUcidList.begin();
             ucidIt != pGiftInfo->strUcidList.end();
             ++ucidIt) {
            string cardUcid = *ucidIt;
            CUserCardInfo* userCard = CGameData::Inst()->getUsrCardInfByUcid(cardUcid.c_str());

            if (userCard == NULL)
                continue;
            snprintf(buf, 99, "%d", userCard->cid);
            int cardCount = giftCardsDic[buf].asInt();
            cardCount++;
            giftCardsDic[buf] = cardCount;
        }
    }
    
    if (pGiftInfo->strUcidList.size() > 0)
    {
        for (list<string>::iterator ucidIt = pGiftInfo->strUcidList.begin();
             ucidIt != pGiftInfo->strUcidList.end();
             ++ucidIt) {
            string cardUcid = *ucidIt;
            CUserCardInfo* userCard = CGameData::Inst()->getUsrCardInfByUcid(cardUcid.c_str());
            
            char cidBuf[100];
            snprintf(cidBuf, 99, "%d", userCard->cid);
            
            if (!giftCardsDic.isMember(cidBuf))
                continue;
            
            if (userCard == NULL)
                continue;
            
            if (bFirst)
                bFirst = false;
            else
                strGift.append("、");
            
            snprintf(buf, 99,
                     CGameData::Inst()->getLanguageValue("cdkey_result_card"),
                     userCard->star, userCard->name,
                     giftCardsDic[cidBuf].asInt());
            strGift.append(buf);
            giftCardsDic.removeMember(cidBuf);
        }
    }

    int nSize = (int)pGiftInfo->m_vrGuildItem.size();
    for (int i = 0; i < nSize; ++i)
    {
        if (0==i && !bFirst)
        {
            strGift.append(",");
        }
        snprintf(buf, 99,"%s x %d",pGiftInfo->m_vrGuildItem[i].first.c_str(),pGiftInfo->m_vrGuildItem[i].second);
        strGift.append(buf);
        if (1<nSize && i!=(nSize-1))
        {
            strGift.append(",");
        }
        
        bFirst = false;
    }
    
    return strGift.c_str();
}

void CDKeyExchange::showExchangeSuccessDlg()
{
    GiftInfo* pGiftInfo = CGameData::Inst()->getGiftInfo();
    
    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    spBg->setPosition(CCPointMake(320, 480));
    spBg->setScale(2.0);
    addChild(spBg);
    
    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cdkey_success"),
                                                 40);
    tnTitle->setColor(ccc3(100, 30, 0));
    tnTitle->setPosition(CCPointMake(320, 660));
    addChild(tnTitle);
    
    TextNode* tnGetGift = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cdkey_getGift"),
                                                   CCSizeMake(200, 30),
                                                   CCTextAlignmentLeft,
                                                   30);
    tnGetGift->setAnchorPoint(CCPointMake(0, 0.5));
    tnGetGift->setPosition(CCPointMake(90, 585));
    tnGetGift->setColor(ccBLACK);
    addChild(tnGetGift);
    
    TextNode* tnGiftName = TextNode::textWithString(pGiftInfo->name,
                                                    CCSizeMake(260, 30),
                                                    CCTextAlignmentLeft,
                                                    30);
    tnGiftName->setAnchorPoint(CCPointMake(0, 0.5));
    tnGiftName->setPosition(CCPointMake(300, 585));
    tnGiftName->setColor(ccc3(100, 30, 0));
    addChild(tnGiftName);
    
    TextNode* tnGiftContent = TextNode::textWithString(getGiftContent(),
                                                       CCSizeMake(460, 200),
                                                       CCTextAlignmentLeft,
                                                       30);
    tnGiftContent->setColor(ccc3(100, 30, 0));
    tnGiftContent->setAnchorPoint(CCPointMake(0.5, 0.5));
    tnGiftContent->setPosition(CCPointMake(320, 465));
    addChild(tnGiftContent);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOk = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                            spMenu2,
                                                            m_pListener,
                                                            m_pfnSelector);
    mItemOk->setAnchorPoint(CCPointZero);
    mItemOk->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                             260));
    mItemOk->setTag(enCDKeyBtn_Back);
    
    TextNode* tnOk = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                              spMenu1->getContentSize(),
                                              CCTextAlignmentCenter,
                                              40);
    tnOk->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                  spMenu1->getContentSize().height * 0.5));
    tnOk->setShadowColor(ccBLACK);
    mItemOk->addChild(tnOk);
        
    CCMenu* menu = CCMenu::menuWithItem(mItemOk);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

bool CDKeyExchange::exchangeCDKey()
{
    hideKeyboard();
    
    return CGameData::Inst()->getGiftReq(string(m_textField->getString()));
}

void CDKeyExchange::showKeyboard()
{
    if (m_textField)
    {
        m_textField->attachWithIME();
        m_bAttach = true;
    }
}

void CDKeyExchange::hideKeyboard()
{
    if (m_textField)
    {
        m_textField->detachWithIME();
        m_bAttach = false;
    }
}

#pragma mark -
#pragma mark CCTouchDelegate
bool CDKeyExchange::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_bAttach)
        hideKeyboard();
    else
        showKeyboard();
    
    return true;
}

#pragma mark -
#pragma mark CCTextFieldDelegate
bool CDKeyExchange::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    CCSprite* spShadow = (CCSprite *)m_mItemExchange->getChildByTag(enCDKeyBtn_Shadow);

    if (sender->getCharCount() == 14)
    {
        return true;
    }
    
    if (sender->getCharCount() + nLen >= 14)
    {
        m_mItemExchange->setIsEnabled(true);
        spShadow->setIsVisible(false);
    }
    
    return false;
}

bool CDKeyExchange::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    CCSprite* spShadow = (CCSprite *)m_mItemExchange->getChildByTag(enCDKeyBtn_Shadow);
    
    if (sender->getCharCount() == 14)
    {
        m_mItemExchange->setIsEnabled(false);
        spShadow->setIsVisible(true);
    }
    
    return false;
}





