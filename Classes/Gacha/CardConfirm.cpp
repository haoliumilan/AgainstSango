//
//  CardConfirm.cpp
//  AgainstWar
//
//  Created by user on 13-12-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "CardConfirm.h"
#include "TextNode.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

static CCPoint btnPosition[3] =
{
    CCPointMake(320, 425), CCPointMake(320, 325), CCPointMake(320, 225),
};

CardConfirm::CardConfirm(CCObject* listener, SEL_CallFuncO selector, CShopCardInfo* pShopCard)
{
    m_pListener = listener;
    m_pfnSelector = selector;
    m_pShopCardInf = pShopCard;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true); //-130
    
    //三按钮

    const char* title[] = {
        "购买",
        "武将详情",
        CGameData::Inst()->getLanguageValue("con_back_")
    };
    
    for (int i = 0; i < 3; i++)
    {
        CCSprite* spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn)
        {
            TextNode* tnTemp = TextNode::textWithString(title[i],
                                                        spBtn->getContentSize(),
                                                        CCTextAlignmentCenter,
                                                        36);
            tnTemp->setShadowColor(ccBLACK);
            tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                            spBtn->getContentSize().height * 0.5));
            spBtn->addChild(tnTemp);
            
            
            spBtn->setPosition(btnPosition[i]);
            addChild(spBtn);
            
        }
    }
    
    drawCardInf(m_pShopCardInf->sCid);
 
    CCLayerColor* lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 196), 640, 960);
	addChild(lcCover, -10);
}

void CardConfirm::drawCardInf(short sCardId)
{
    //头像
    CCSprite* spCard = CGameData::Inst()->getHeadSprite(sCardId);
    if (spCard)
    {
        spCard->setPosition(CCPointMake(226, 574));
        addChild(spCard);
    }
    
    CCardBaseInfo * pCardBaseInf = CGameData::Inst()->getCardBaseInfByCid(sCardId);
    
    TextNode* lbText = TextNode::textWithString(pCardBaseInf->name, CCSizeMake(640, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(320, 650 + 30));
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    char buf[100];

    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("lv"));
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 596));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    snprintf(buf, 99, "%d", m_pShopCardInf->iStartLv);

    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 596));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("price"));
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 560));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    snprintf(buf, 99, "%d", m_pShopCardInf->iPrice);
    
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 560));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    
    CCSprite* spStar;
    for(int i = 0; i < pCardBaseInf->star; i++){
        spStar = CCSprite::spriteWithSpriteFrameName("id_star.png");
        float starBeginPosX = 320 - spStar->getContentSize().width * (pCardBaseInf->star - 1) / 2;
        spStar->setPosition(CCPointMake(starBeginPosX + spStar->getContentSize().width * i, 640));
        addChild(spStar);
    }
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CardConfirm::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    bool bRet = isSelectedForPos(pos, 3);
    
    if (bRet) {
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    
    return true;
}

//******************************************************************************
// isSelectedForPos
//******************************************************************************
bool CardConfirm::isSelectedForPos(CCPoint pos, int btnCount)
{
    bool bRet;
    
    m_ret.iBtnSel = kInvalidValue;
    
    
    for (int i = 0; i < btnCount; i++)
    {
        bRet = CCRect::CCRectContainsPoint(CCRectMake(btnPosition[i].x - 188 * 0.5,
                                                      btnPosition[i].y - 79 * 0.5,
                                                      188,
                                                      79),
                                           pos);
        if (bRet) {
            m_ret.iBtnSel = i;
            return bRet;
        }
    }
    
    
    return false;
}




