//
//  CTipsDlg.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//


#include "CTipsDlg.h"
#include "TextNode.h"
#include "CGamedata.h"

const char *gTips[]= {
    "tipsDlgTip1",
    "tipsDlgTip2",
    "tipsDlgTip3",
    "tipsDlgTip4",
    "tipsDlgTip5",
    "tipsDlgTip6",
    "tipsDlgTip7",
    "tipsDlgTip8",
    "tipsDlgTip9",
    "tipsDlgTip10",
    "tipsDlgTip11",
    "tipsDlgTip12",
    "tipsDlgTip13",
    "tipsDlgTip14",
    "tipsDlgTip15",
    "tipsDlgTip16",
    "tipsDlgTip17",
    "tipsDlgTip18",
    "tipsDlgTip19",
    "tipsDlgTip20",
    "tipsDlgTip21",
    "tipsDlgTip22",
    "tipsDlgTip23",
    "tipsDlgTip24",
    "tipsDlgTip25",
    "tipsDlgTip26",
    "tipsDlgTip27",
    "tipsDlgTip28",
    "tipsDlgTip29",
    "tipsDlgTip30",
    "tipsDlgTip31",
};


//******************************************************************************
// Construct
//******************************************************************************
CTipsDlg::CTipsDlg(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    sp->setScale(2.0);
    addChild(sp);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* item = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CTipsDlg::menuCallback) );
    item->setAnchorPoint(ccp(0, 0));
    CCMenu* menu = CCMenu::menuWithItems(item,  NULL);
    addChild(menu, 10);
    menu->setPosition(ccp(-80, -220));
    //item->setPosition(CCPointMake(0, -0.3 * sz.height));
    
    
    TextNode *titleLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("gameEnd"),
                                                    52);
    titleLabel->setColor(ccc3(122, 37, 8));
    addChild(titleLabel);
    titleLabel->setPosition(ccp(0, 170));
    
    m_lbTipBk = TextNode::textWithString("", CCSizeMake(480, 230), CCTextAlignmentCenter, 30);
    m_lbTipBk->setPosition(CCPointMake(2, -2));
    m_lbTipBk->setColor(ccc3(63, 23, 13));
    addChild(m_lbTipBk, 9);

    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    item->addChild(itemLabel);
    this->setScale(0);
}

CTipsDlg::~CTipsDlg()
{
    CCLog("CTipsDlg::~CTipsDlg");
}

//******************************************************************************
// get1tip
//******************************************************************************
const char* CTipsDlg::get1tip()
{
    int iTipCnt = sizeof(gTips)/ sizeof(char*);
    
    int iRand = rand() % iTipCnt;
        
    return CGameData::Inst()->getLanguageValue(gTips[iRand]);
}

//******************************************************************************
// onExit
//******************************************************************************
void CTipsDlg::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

//******************************************************************************
// show
//******************************************************************************
void CTipsDlg::show()
{
    const char* tip = get1tip();
    m_lbTipBk->setString(tip);
    
    this->setIsVisible(true);
    this->setScale(0);
    this->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8));
}

//******************************************************************************
// menuCallback
//******************************************************************************
void CTipsDlg::menuCallback(CCObject* sender)
{
    this->stopAllActions();
    this->setIsVisible(false);
    
    (m_pListener->*m_pfnSelector)();
}








