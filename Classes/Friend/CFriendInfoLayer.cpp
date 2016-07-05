//
//  CFriendInfo.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CFriendInfoLayer.h"
#include "CMenuBtn.h"
#include "CListLayer.h"
#include "CScrollBar.h"
#include "CShowCardInf.h"
#include "MainScene.h"
#include "CommonDef.h"
#include "TextNode.h"

CFriendInfoLayer::CFriendInfoLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
     
    m_background= CCLayerColor::layerWithColorWidthHeight(ccc4(14, 11, 16, 0), 592, 483);
   // m_background = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
 //   m_background = CCSprite::spriteWithFile("fr_window2.png");
   // m_background->setScale(2.0);
    m_background->setAnchorPoint(ccp(0.5, 0.5));
    m_background->setPosition(ccp(960, 560));
    addChild(m_background);
    
    CCSprite *vBackGround = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    vBackGround->setScale(2.0);
    vBackGround->setPosition(ccp(296, 240));
    m_background->addChild(vBackGround);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CFriendInfoLayer::menuCallback)); 
    sureItem->setTag(enFILayerBtnOk);
    sureItem->setPosition(ccp(-20, 0));
    
    TextNode *sureLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
    sureLabel->setColor(ccc3(255, 255, 255));
    sureLabel->setShadowColor(ccBLACK);
    sureLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    sureItem->addChild(sureLabel);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    cancelItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CFriendInfoLayer::menuCallback)); 
    cancelItem->setTag(enFILayerBtnCancel);
    cancelItem->setPosition(ccp(180, 0));
    
    TextNode *cancelLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), kBtnTitleHeight);
    cancelLabel->setColor(ccc3(255, 255, 255));
    cancelLabel->setShadowColor(ccBLACK);
    cancelLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                         spMenu1->getContentSize().height * 0.5 + 2));
    cancelItem->addChild(cancelLabel);
  //  154274479
    sureItem->setAnchorPoint(ccp(0, 0));
    cancelItem->setAnchorPoint(ccp(0, 0));
    CCMenu *menu = CCMenu::menuWithItems(sureItem,cancelItem,NULL);
    menu->setPosition(ccp(145, 20));
    m_background->addChild(menu);
    
//    m_pScrollBar =  new CScrollBar();
//    addChild(m_pScrollBar);
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard);
    m_showCard->release();
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_backItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CFriendInfoLayer::backFromShow));
    m_backItem->setTag(1);
    m_backItem->setAnchorPoint(CCPointZero);
    CCMenu* backMenu = CCMenu::menuWithItems(m_backItem, NULL);
    addChild(backMenu);
    backMenu->setPosition(CCPointZero);
    
    m_backItem->setPosition(CCPointMake(-1000, -1000));

    //好友信息title
    titleLabel = TextNode::textWithString("", 55);
    titleLabel->setColor(ccc3(122, 37, 8));
    titleLabel->setPosition(ccp(296, 425));
    m_background->addChild(titleLabel);

    //好友信息
    
    infoLabel = TextNode::textWithString("", 30);
    infoLabel->setColor(ccc3(63, 23, 13));
    infoLabel->setPosition(ccp(296, 200));
    m_background->addChild(infoLabel);
    
    friendInfolist = new CListLayer(this, callfuncO_selector(CFriendInfoLayer::cbList));
    m_background->addChild(friendInfolist);
    friendInfolist->setPosition(ccp(-1000, -30));
    friendInfolist->release();
    //   CListLayer *listLayer = CListLay
    
    friendInfolist->setPosition(ccp(0, -30));
} 
void CFriendInfoLayer::backFromShow()
{
    m_backItem->setPosition(CCPointMake(-1000, -1000));
    MainScene::Inst()->showBottomPanel(true);
    m_showCard->hide();
}

bool CFriendInfoLayer::addFriend()
{
    return CGameData::Inst()->addFriendRequest(friendAddId);
}

void CFriendInfoLayer::cbList(CCObject *pObj)
{
   CListLayerRet* pRet = (CListLayerRet*)pObj;
    if(pRet->iBtnSel == 0)
    {
//        CUserCardInfo cardInfo = showFrInfo->card_info;
        m_showCard->show(&(showFrInfo->card_info), false);
        MainScene::Inst()->showBottomPanel(false);
    }
}

void CFriendInfoLayer::enter()
{
    m_background->setPosition(ccp(900, 280));
    m_background->setAnchorPoint(ccp(0.5, 0.5));
    m_background->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(28, 280)), 0.8f));
    friendInfolist->setPosition(CCPointMake(640, -315));
    friendInfolist->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(-25, -320)),0.8f));
    CFriendInf *schUsrInfo = CGameData::Inst()->getSchUsrInf();
    showFrInfo =  schUsrInfo;
    if(schUsrInfo)
    {
        friendInfolist->clearAll();
        CMenuBtn *btn = new CMenuBtn(schUsrInfo,enFriendRangePro);
        friendInfolist->push_back(btn);
        btn->release();
    }
}

void CFriendInfoLayer::enterWithType(FriendInfoLayerType type,CFriendInf *friendInfo)
{
    layerType = type;
    if(type == friInfo_accept)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip1"));
        infoLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip2"));
        
    }else if(type == friInfo_delete)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip3"));
        infoLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip4"));
        
    }else if(type == friInfo_refuse)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip5"));
        infoLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip6"));
        
    }else if(type == friInfo_addFriend)
    {
        titleLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip7"));
        infoLabel->setString(CGameData::Inst()->getLanguageValue("friendInfoLayerTip8"));
        enter();
        sprintf(friendAddId,"%s", friendInfo->fid);

        return;
    }
  //  layerType = type;
    m_background->setPosition(ccp(900, 280));
    m_background->setAnchorPoint(ccp(0.5, 0.5));
    m_background->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(28, 280)), 0.8f));
    friendInfolist->setPosition(CCPointMake(640, -315));
    friendInfolist->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(-20, -320)),0.8f));
     friendInfolist->clearAll();
    showFrInfo = friendInfo;
    CMenuBtn *btn = new CMenuBtn(friendInfo,enFriendRangePro);
    friendInfolist->push_back(btn);
    btn->release();
    
}

FriendInfoLayerType CFriendInfoLayer::getLayerType()
{
    return layerType;
}

void CFriendInfoLayer::exit()
{
    m_background->stopAllActions();
    friendInfolist->stopAllActions();
    m_background->setPosition(ccp(960, 280));
    friendInfolist->setPosition(CCPointMake(-2000, -1000));
    
    friendInfolist->clearAll();
}

void CFriendInfoLayer::clearFriendList()
{
    friendInfolist->clearAll();
}
void CFriendInfoLayer::menuCallback(CCObject *pSender)
{
  //  CCLog("%f %f")
    CCMenuItem* menuItem = (CCMenuItem*) pSender;
    
    m_ret.iBtnSel = menuItem->getTag();
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}

