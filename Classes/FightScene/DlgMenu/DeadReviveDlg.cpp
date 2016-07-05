//
//  DeadReviveDlg.cpp
//  AgainstWar
//
//  Created by jingfeng wu on 14-3-14.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "DeadReviveDlg.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "SaveData.h"

DeadReviveDlg::DeadReviveDlg(CCObject *listener,SEL_CallFuncO selector)
{
    m_listener = listener;
    m_selector = selector;
    
    m_backGround = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    m_backGround->setScale(2.0);
    addChild(m_backGround);
    m_backGround->setPosition(ccp(320, 500));
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(DeadReviveDlg::menuCallback));
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
    strncpy(buf, CGameData::Inst()->getLanguageValue("sureTip"), 99);
    TextNode *sureLabel = TextNode::textWithString(buf, 42);
    sureLabel->setColor(ccc3(255, 255, 255));
    sureLabel->setShadowColor(ccBLACK);
    sureLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    sureItem->addChild(sureLabel);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    cancelItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(DeadReviveDlg::menuCallback));
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
    
    float height_arr[] = {35,35,35};
    float lbl_shift_arr[] = {0,0,0};
    
    for(int i=0;i<3;i++)
    {
        str_ReviveMinusCoin & item = SaveData::Inst()->getReviveMinusCoinArr()[i];

        optionLabel = TextNode::textWithString(item.item_dsp, CCSize(450, height_arr[i]), CCTextAlignmentLeft, 30);
        optionLabel->setColor(ccc3(63, 23, 13));
        
        NoScaleMenuItemLabel* pOptionItem1  = NoScaleMenuItemLabel::itemWithLabel(optionLabel, this, menu_selector(DeadReviveDlg::optionCallback));
        //用于修正ios坐标自定义自体时的高度bug
        optionLabel->setPositionY(lbl_shift_arr[i]);
        pOptionItem1->setTag(201+i);
        pOptionItem1->setContentSizeInPixels(CCSize(450, height_arr[i]));
        
        CCMenu* pOptionMenu1 = CCMenu::menuWithItem(pOptionItem1);
        pOptionMenu1->setPosition(ccp(350, 600-i*height_arr[i]));
        addChild(pOptionMenu1);
    }

    
}


void DeadReviveDlg::menuCallback(CCObject *pSender)
{
    CCMenuItemImage *item = (CCMenuItemImage *)pSender;
    m_ret.iBtnsel = item->getTag();
    
    (m_listener->*m_selector)(&m_ret);
}

void DeadReviveDlg::optionCallback(CCObject *pSender)
{
    NoScaleMenuItemLabel *item = (NoScaleMenuItemLabel *)pSender;
    m_ret.iOptionsel = item->getTag();
    CommonInfo* pComInf = CGameData::Inst()->getCommonInfo();
    pComInf->revive_select_tag = m_ret.iOptionsel;
    if (m_ret.iOptionsel-201>=0) {
        str_ReviveMinusCoin &item = SaveData::Inst()->getReviveMinusCoinArr()[m_ret.iOptionsel-201];
        m_ret.iCost = item.item_coin;
        m_ret.strItemId = item.item_id;
    }
}