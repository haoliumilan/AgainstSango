//
//  PackSelBtn.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-30.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "PackSelBtn.h"
#include "TextNode.h"
#include "CGamedata.h"

//******************************************************************************
// Constructor
//******************************************************************************
PackSelBtn::PackSelBtn(char* capacity)
{
    m_iSelCnt = 0;
    m_iGoldSell = 0;
    m_iGoldPerCard = 0;
    initBtn();
    
    char buf[100];
    TextNode* lbText;
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("sell_all_price"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 226));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("sell_select_num"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 203));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("package_contain_num"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 180));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    m_lbText3 = TextNode::textWithString(capacity, CCSizeMake(256, 23), CCTextAlignmentRight, 20);
    m_lbText3->setAnchorPoint(CCPointMake(0, 1));
    m_lbText3->setPosition(CCPointMake(190, 180));
    m_lbText3->setColor(ccWHITE);
    m_lbText3->setShadowColor(ccBLACK);
    addChild(m_lbText3, 1);
    
    m_lbText1->setString("0");
    m_lbText2->setString("0/10");
}

//******************************************************************************
// Constructor
//******************************************************************************
PackSelBtn::PackSelBtn(char* capacity, int goldPerCard, int plus)
{
    m_iSelCnt = 0;
    m_iTotalExp = 0;
    m_iGoldPerCard = goldPerCard;
    m_basePlus = plus;
    m_allPlus = m_basePlus;
    initBtn();
    
    char buf[100];
    TextNode* lbText;
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("coin_need"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 226));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("pwUp_exp_will_get"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 203));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("package_contain_num"));
    lbText = TextNode::textWithString(buf, CCSizeMake(256, 23), CCTextAlignmentLeft, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 180));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    lbText = TextNode::textWithString(capacity, CCSizeMake(256, 23), CCTextAlignmentRight, 20);
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setPosition(CCPointMake(190, 180));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
        
    m_lbText1->setString("0");
    m_lbText2->setString("0");
}

//******************************************************************************
// initBtn
//******************************************************************************
void PackSelBtn::initBtn()
{
    CCSprite* sp = CCSprite::spriteWithFile("fr_substrate.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 190));
        addChild(sp);
    }
    
    TextNode* lbText;
    CCPoint pt;
    CCSize sz;
    
    m_spBtnOk = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    if(m_spBtnOk != NULL){
        m_spBtnOk->setPosition(CCPointMake(545, 190));
        addChild(m_spBtnOk);

        pt = m_spBtnOk->getPosition();
        sz = m_spBtnOk->getContentSize();
        lbText = TextNode::textWithString("OK", CCSizeMake(sz.width, 30), CCTextAlignmentCenter, 30);
        lbText->setPosition(pt);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }

    m_spCoverOk = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
    if(m_spCoverOk){
        addChild(m_spCoverOk, 3);
        m_spCoverOk->setPosition(CCPointMake(545, 190));
    }
    
    m_spBtnClear = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    if(m_spBtnClear != NULL){
        m_spBtnClear->setPosition(CCPointMake(95, 190));
        addChild(m_spBtnClear);
        
        pt = m_spBtnClear->getPosition();
        sz = m_spBtnClear->getContentSize();
        lbText = TextNode::textWithString("Clear", CCSizeMake(sz.width, 30), CCTextAlignmentCenter, 30);
        lbText->setPosition(pt);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
    
    m_spCoverClear = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
    if(m_spCoverClear){
        addChild(m_spCoverClear, 3);
        m_spCoverClear->setPosition(CCPointMake(95, 190));
    }
    
    m_lbText1 = TextNode::textWithString("", CCSizeMake(256, 23), CCTextAlignmentRight, 20);
    m_lbText1->setAnchorPoint(CCPointMake(0, 1));
    m_lbText1->setPosition(CCPointMake(190, 226));
    m_lbText1->setColor(ccWHITE);
    m_lbText1->setShadowColor(ccBLACK);
    addChild(m_lbText1, 1);
        
    m_lbText2 = TextNode::textWithString("", CCSizeMake(256, 23), CCTextAlignmentRight, 20);
    m_lbText2->setAnchorPoint(CCPointMake(0, 1));
    m_lbText2->setPosition(CCPointMake(190, 203));
    m_lbText2->setColor(ccWHITE);
    m_lbText2->setShadowColor(ccBLACK);
    addChild(m_lbText2, 1);
}

//******************************************************************************
// add1cardUp
//******************************************************************************
void PackSelBtn::add1cardUp(int exp, int plus)
{
    m_iSelCnt++;
    m_iTotalExp += exp;
    m_allPlus += plus;
    
    char buf[100];
    snprintf(buf, 99, "%d", m_iTotalExp);
    m_lbText2->setString(buf);
    
    if (m_allPlus > m_basePlus)
        snprintf(buf, 99, "%d", m_iSelCnt * m_iGoldPerCard + m_allPlus * 1000);
    else
        snprintf(buf, 99, "%d", m_iSelCnt * m_iGoldPerCard);
    m_lbText1->setString(buf);
    
    m_spCoverOk->setIsVisible(false);
    m_spCoverClear->setIsVisible(false);
}

//******************************************************************************
// rmv1cardUp
//******************************************************************************
void PackSelBtn::rmv1cardUp(int exp, int plus)
{
    m_iSelCnt--;
    m_iTotalExp -= exp;
    m_allPlus -= plus;
    
    if(m_iSelCnt < 0)
        m_iSelCnt = 0;
    if(m_iTotalExp < 0)
        m_iTotalExp = 0;
    
    char buf[100];
    snprintf(buf, 99, "%d", m_iTotalExp);
    m_lbText2->setString(buf);
    
    if (m_iSelCnt == 0) {
        snprintf(buf, 99, "%d", 0);
    }
    else {
        if (m_allPlus > m_basePlus)
            snprintf(buf, 99, "%d", m_iSelCnt * m_iGoldPerCard + m_allPlus * 1000);
        else
            snprintf(buf, 99, "%d", m_iSelCnt * m_iGoldPerCard);
    }
    m_lbText1->setString(buf);
    
    if(m_iSelCnt == 0){
        m_spCoverOk->setIsVisible(true);
        m_spCoverClear->setIsVisible(true);
    }
}

//******************************************************************************
// add1cardSell
//******************************************************************************
void PackSelBtn::add1cardSell(int gold)
{
    m_iSelCnt++;
    m_iGoldSell += gold;
    
    char buf[100];
    snprintf(buf, 99, "%d/10", m_iSelCnt);
    m_lbText2->setString(buf);
    
    snprintf(buf, 99, "%d", m_iGoldSell);
    m_lbText1->setString(buf);
    
    m_spCoverOk->setIsVisible(false);
    m_spCoverClear->setIsVisible(false);
}

//******************************************************************************
// rmv1cardSell
//******************************************************************************
void PackSelBtn::rmv1cardSell(int gold)
{
    m_iSelCnt--;
    m_iGoldSell -= gold;
    
    char buf[100];
    snprintf(buf, 99, "%d/10", m_iSelCnt);
    m_lbText2->setString(buf);
    
    snprintf(buf, 99, "%d", m_iGoldSell);
    m_lbText1->setString(buf);
    
    if(m_iSelCnt == 0){
        m_spCoverOk->setIsVisible(true);
        m_spCoverClear->setIsVisible(true);
    }
}

//******************************************************************************
// updateCardNum
//******************************************************************************
void PackSelBtn::setCardNum(char* capacity) {
    m_lbText3->setString(capacity);
}

//******************************************************************************
// clear
//******************************************************************************
void PackSelBtn::clear()
{
    
    
    m_iSelCnt = 0;
    m_iTotalExp = 0;
    m_iGoldSell = 0;
    m_allPlus = m_basePlus;
    
    m_lbText1->setString("0");
    
    if(m_iGoldPerCard != 0){
        m_lbText2->setString("0");
    }
    else{
        m_lbText2->setString("0/10");
    }
    
    m_spCoverOk->setIsVisible(true);
    m_spCoverClear->setIsVisible(true);
}

//******************************************************************************
// getGoldSell
//******************************************************************************
int PackSelBtn::getGoldSell()
{
    return m_iGoldSell;
}

//******************************************************************************
// onExit
//******************************************************************************
void PackSelBtn::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}