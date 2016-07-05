//
//  Dialog2.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-2.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Dialog2.h"
#include "TextNode.h"
#include "CGamedata.h"
#include "OcProxy.h"

//******************************************************************************
// Construct
//******************************************************************************
Dialog2::Dialog2(CCObject* target, SEL_CallFuncO selector, const char* title,const char* dsp,Dialog2Type type)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_type = type;
    char buf[100];
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    if(sp){
        sp->setScale(2.0);

        addChild(sp);
    }

    
    if(type == enDl2FullCostBtn || type == enDl2FoodInTeam)
    {
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                     spMenu2,
                                                                     this,
                                                                     menu_selector(Dialog2::menuCallBack));
        if (type == enDl2FullCostBtn) {
            sureItem->setTag(111);
        }
        else if (type == enDl2FoodInTeam) {
            sureItem->setTag(kShowFoodInTeam);
        }
        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("teamInf_title"));
        TextNode* itemLabel = TextNode::textWithString(buf,
                                                       CCSizeMake(127, 30),
                                                       CCTextAlignmentCenter,
                                                       kBtnTitleHeight);
        itemLabel->setColor(ccc3(255, 255, 255));
        itemLabel->setShadowColor(ccBLACK);
        itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                           spMenu1->getContentSize().height * 0.5 + 2));
        sureItem->addChild(itemLabel);
        
        itemLabel->setColor(ccc3(255, 255, 255));
        
        sureItem->setAnchorPoint(ccp(0, 0));
        CCMenu *menu = CCMenu::menuWithItem(sureItem);
        menu->setPosition(ccp(-80, -220));
        menu->setAnchorPoint(ccp(0, 0));
        addChild(menu);
    }
    else if (enDl2TwoBtnJuanXian == type)
    {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
        //ok
        m_spOk = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(m_spOk){
            m_spOk->setPosition(CCPointMake(-153, -157-20));
            addChild(m_spOk);
        }
        if (strlen(CGameData::Inst()->getLanguageValue("backTip")) != 0)
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("backTip"));
        else
            snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip1"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-156, -153-20));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        //cancel
        m_spCancel = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(m_spCancel){
            m_spCancel->setPosition(CCPointMake(153, -157-20));
            addChild(m_spCancel);
        }
        
        if (strlen(CGameData::Inst()->getLanguageValue("cntry_donate_alone")) != 0)
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_donate_alone"));
        else
            snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip2"));
        lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(150, -153-20));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);

    }
    else if(type == enDl2TwoBtn || type == enDl2FoodUpThreeStars)
    {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
        //ok
        m_spOk = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(m_spOk){
            m_spOk->setPosition(CCPointMake(-153, -157-20));
            addChild(m_spOk);
        }
        if (strlen(CGameData::Inst()->getLanguageValue("sureTip")) != 0)
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
        else
            snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip1"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-156, -153-20));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        //cancel
        m_spCancel = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(m_spCancel){
            m_spCancel->setPosition(CCPointMake(153, -157-20));
            addChild(m_spCancel);
        }
        
        if (strlen(CGameData::Inst()->getLanguageValue("cancelTip")) != 0)
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cancelTip"));
        else
            snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip2"));
        lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(150, -153-20));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
    else if(type == enDl2OneBtn){
        
     
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog2::menuCallBack));    
        sureItem->setTag(1);
        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
        TextNode* itemLabel = TextNode::textWithString(buf,
                                                       CCSizeMake(127, 30),
                                                       CCTextAlignmentCenter,
                                                       kBtnTitleHeight);
        itemLabel->setColor(ccc3(255, 255, 255));
        itemLabel->setShadowColor(ccBLACK);
        itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                           spMenu1->getContentSize().height * 0.5 + 2));
        sureItem->addChild(itemLabel);

        sureItem->setAnchorPoint(ccp(0, 0));
        CCMenu *menu = CCMenu::menuWithItem(sureItem);
        menu->setPosition(ccp(-80, -220));
        menu->setAnchorPoint(ccp(0, 0));
        addChild(menu);
        
    }else if(type == enDl2ThreeBtn){
        
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *item1 = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog2::menuCallBack));    
        item1->setTag(11);
        
        spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *item2 = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog2::menuCallBack));    
        item2->setTag(12);
        
        spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *item3 = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog2::menuCallBack));    
        item3->setTag(13);
        
        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("expansion"));
        TextNode* itemLabel1 = TextNode::textWithString(buf,
                                                        CCSizeMake(127, 30),
                                                        CCTextAlignmentCenter,
                                                        kBtnTitleHeight);
        itemLabel1->setColor(ccc3(255, 255, 255));
        itemLabel1->setShadowColor(ccBLACK);
        itemLabel1->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                            spMenu1->getContentSize().height * 0.5 + 2));
        item1->addChild(itemLabel1);

        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("pwUp_title"));
        TextNode* itemLabel2 = TextNode::textWithString(buf,
                                                        CCSizeMake(127, 30),
                                                        CCTextAlignmentCenter,
                                                        kBtnTitleHeight);
        itemLabel2->setColor(ccc3(255, 255, 255));
        itemLabel2->setShadowColor(ccBLACK);
        itemLabel2->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                            spMenu1->getContentSize().height * 0.5 + 2));
        item2->addChild(itemLabel2);

        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sale"));
        TextNode* itemLabel3 = TextNode::textWithString(buf,
                                                        CCSizeMake(127, 30),
                                                        CCTextAlignmentCenter,
                                                        kBtnTitleHeight);
        itemLabel3->setColor(ccc3(255, 255, 255));
        itemLabel3->setShadowColor(ccBLACK);
        itemLabel3->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                            spMenu1->getContentSize().height * 0.5 + 2));
        item3->addChild(itemLabel3);

        
        item1->setAnchorPoint(ccp(0, 0));
        item2->setAnchorPoint(ccp(0, 0));
        item3->setAnchorPoint(ccp(0, 0));
        item1->setPosition(ccp(-50, 0));
        item2->setPosition(ccp(130, 0));
        item3->setPosition(ccp(310, 0));
        
        CCMenu *menu = CCMenu::menuWithItems(item1,item2,item3,NULL);
        menu->setPosition(ccp(-206, -220));
        addChild(menu);
    }
    
    TextNode *lbText;
    //title
    if(title){
        lbText = TextNode::textWithString(title, CCSizeMake(500, 40), CCTextAlignmentCenter,55);
        lbText->setPosition(CCPointMake(0, 170));
        lbText->setColor(ccc3(122, 37, 8));
        addChild(lbText, 1);
    }

    //dsp
    if(dsp){
        if (strlen(dsp) > 146) {
           lbText = TextNode::textWithString(dsp, CCSizeMake(500, 200), CCTextAlignmentCenter,30);
        }else{
           lbText = TextNode::textWithString(dsp, CCSizeMake(500, 200), CCTextAlignmentCenter,35);
        }
        
        lbText->setAnchorPoint(CCPointMake(0.5, 1));
        lbText->setPosition(CCPointMake(0, 100));
        lbText->setColor(ccc3(63, 23, 13));
        addChild(lbText, 1);
    }

}

void Dialog2::menuCallBack(CCObject *pSender)
{
    CCMenuItemImage *item = (CCMenuItemImage *)pSender;
    if(item->getTag() == 11 || item->getTag() == 12 || item->getTag() == 13 
       ||item->getTag() == 111 || item->getTag() == kShowFoodInTeam)
    {
        m_ret.bSel = item->getTag();
        (m_pListener->*m_pfnSelector)(&m_ret);
    }else {
        m_ret.bOk = false;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
}
void Dialog2::cbSwitchUser(CCNode* a)
{
 //   CCMenuItem* a = (CCMenuItem*) OKorCancel;
    (m_pListener->*m_pfnSelector)(a);
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool Dialog2::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCPoint pt = m_spOk->getPosition();
    CCSize sz = m_spOk->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    
    if(bRet){
        m_ret.bOk = true;
        if (m_type == enDl2FoodUpThreeStars) {
            m_ret.bSel = kShowFoodUpThreeStarsOk;
        }
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else {
        pt = m_spCancel->getPosition();
        sz = m_spCancel->getContentSize();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                           pt.y - sz.height * 0.5, 
                                                           sz.width, 
                                                           sz.height), 
                                                pos);
        
        if(bRet){
            m_ret.bOk = false;
            if (m_type == enDl2FoodUpThreeStars) {
                m_ret.bSel = kShowFoodUpThreeStarsCancel;
            }
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    
    
    return true;
}

//******************************************************************************
// onExit
//******************************************************************************
void Dialog2::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}