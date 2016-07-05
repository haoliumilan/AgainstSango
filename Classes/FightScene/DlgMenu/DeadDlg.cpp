//
//  DeadDlg.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-16.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "DeadDlg.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "SaveData.h"

DeadDlg::DeadDlg(CCObject *listener,SEL_CallFuncO selector,int type)
{
   // CCLayerColor *layer = CCLayerColor::layerWithColor)
    m_listener = listener;
    m_selector = selector;
    
    m_backGround = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    m_backGround->setScale(2.0);
    addChild(m_backGround);
    m_backGround->setPosition(ccp(320, 500));
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(DeadDlg::menuCallback)); 
    sureItem->setTag(111);
    sureItem->setPosition(ccp(88, 284));
    
    int need;
    long hold;
    if (CGameData::Inst()->chkLogin()) {
        need =  CGameData::Inst()->getCommonInfo()->revive_coin;
    }
    else {
        need = SaveData::Inst()->getReviveCoin();
    }
    
    if(CGameData::Inst()->getUsrInfo()->login_time > 0){
        hold =  CGameData::Inst()->getUsrInfo()->coin;
        SaveData::Inst()->setCurCoin(hold);
    }
    else{
        hold = SaveData::Inst()->getCurCoin();
    }
    char buf[100];
    if(type == enDeadType_normal)
    {
        if (hold<need) {
            strncpy(buf, CGameData::Inst()->getLanguageValue("shopTip14"), 99);
        }
        else
        {
            strncpy(buf, CGameData::Inst()->getLanguageValue("sureTip"), 99);
        }
    }
    else if(type == enDeadType_free)
    {
       strncpy(buf, CGameData::Inst()->getLanguageValue("sureTip"), 99);
    }
    
    
    TextNode *sureLabel = TextNode::textWithString(buf, 42);
    sureLabel->setColor(ccc3(255, 255, 255));
    sureLabel->setShadowColor(ccBLACK);
    sureLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    sureItem->addChild(sureLabel);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    cancelItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(DeadDlg::menuCallback)); 
    cancelItem->setTag(112);
    cancelItem->setPosition(ccp(394, 284));
    
    TextNode *cancelLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), 42);
    cancelLabel->setColor(ccc3(255, 255, 255));
    cancelLabel->setShadowColor(ccBLACK);
    cancelLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                         spMenu1->getContentSize().height * 0.5 + 2));
    cancelItem->addChild(cancelLabel);
    
    sureItem->setAnchorPoint(ccp(0, 0));
    cancelItem->setAnchorPoint(ccp(0, 0));
    
    CCMenu *menu = CCMenu::menuWithItems(sureItem,cancelItem,NULL);
    menu->setPosition(ccp(0, 0));
    addChild(menu);
    
    TextNode *titleLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("deadDlgtip1"), 55);
    titleLabel->setColor(ccc3(122, 37, 8));
    addChild(titleLabel);
    titleLabel->setPosition(ccp(320, 420+255));
    
    if(type == enDeadType_normal)
    {
        if (hold<need) {
            snprintf(buf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("deadDlgtip2"),need,CGameData::Inst()->getLanguageValue("deadDlgtip3"));
        }
        else
        {
            snprintf(buf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("deadDlgtip2"),need,CGameData::Inst()->getLanguageValue("deadDlgtip4"));
        }
        infoLabel = TextNode::textWithString(buf, CCSize(388, 70), CCTextAlignmentCenter, 30);
        infoLabel->setPosition(ccp(320, 300+255));
        infoLabel->setColor(ccc3(63, 23, 13));
        addChild(infoLabel);
    }
    else if(type == enDeadType_free)
    {
         snprintf(buf, 99,CGameData::Inst()->getLanguageValue("deadDlgtip7"),CGameData::Inst()->getCommonInfo()->free_revive_lv);
        infoLabel = TextNode::textWithString(buf, CCSize(388, 110), CCTextAlignmentCenter, 30);
        infoLabel->setPosition(ccp(320, 300+205));
        infoLabel->setColor(ccc3(63, 23, 13));
        addChild(infoLabel);
    }
    
    
   
    
    if(type == enDeadType_normal)
    {
        snprintf(buf, 99,"%s%ld%s!",CGameData::Inst()->getLanguageValue("deadDlgtip5"),hold,CGameData::Inst()->getLanguageValue("deadDlgtip6"));
        
        myYuanbaoLabel = TextNode::textWithString(buf, CCSize(388, 70), CCTextAlignmentCenter, 30);
        myYuanbaoLabel->setPosition(ccp(320, 190+255));
        myYuanbaoLabel->setColor(ccc3(63, 23, 13));
        addChild(myYuanbaoLabel);
    }
}


void DeadDlg::menuCallback(CCObject *pSender)
{
    CCMenuItemImage *item = (CCMenuItemImage *)pSender;
    m_ret.iBtnsel = item->getTag();
    
    (m_listener->*m_selector)(&m_ret);
//    if(item->getTag() == 111)
//    {
//        
//    }else {
//        
//    }
}