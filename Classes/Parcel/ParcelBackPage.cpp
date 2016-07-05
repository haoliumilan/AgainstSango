//
//  ParcelBackPage.cpp
//  AgainstWar
//
//  Created by AllenSK on 14-2-11.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "ParcelBackPage.h"


ParcelBackPage::ParcelBackPage()
{

}

void ParcelBackPage::NtyShow()
{
    this->setPosition(CCPointMake(0, 0));
    
}

void ParcelBackPage::NtyHide()
{
    
    this->setPosition(CCPointMake(-10000, -10000));

}



void ParcelBackPage::enter()
{
    
//    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
//                                                           callfuncO_selector(ParcelBackPage::NtyShow),  // 处理的消息的回调函数
//                                                           kNotifyShowBtn,                    // 感兴趣的消息名称
//                                                           NULL);
//    
//    
//    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
//                                                           callfuncO_selector(ParcelBackPage::NtyHide),  // 处理的消息的回调函数
//                                                           kNotifyHideBtn,                    // 感兴趣的消息名称
//                                                           NULL);
//    

    
    
    m_choiceChild = false;
    
    m_pParcalBack = CCSprite::spriteWithFile("parcal_back.png");
    
    m_pParcalBack->setPosition(CCPointMake(320, 460));
    
    addChild(m_pParcalBack , 20);
    
    
    CCSprite *tempSprite1 = CCSprite::spriteWithFile("parcal_itemOn.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithFile("parcal_itemOn.png");
    
    m_item_on = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(ParcelBackPage::cbCheckItemBtn));
    m_item_on->setAnchorPoint(CCPointZero);
    m_item_on->setPosition(CCPointMake(40, 720));
    CCMenu *rangeMenu = CCMenu::menuWithItem(m_item_on);
    addChild(rangeMenu,30);
    rangeMenu->setPosition(CCPointZero);
    
    
    
    tempSprite1 = CCSprite::spriteWithFile("parcal_childOff.png");
    tempSprite2 = CCSprite::spriteWithFile("parcal_childOff.png");
    
    m_child_off = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(ParcelBackPage::cbCheckChildBtn));
    m_child_off->setAnchorPoint(CCPointZero);
    m_child_off->setPosition(CCPointMake(220, 720 + 4));
    CCMenu *rangeMenu1 = CCMenu::menuWithItem(m_child_off);
    addChild(rangeMenu1,10);
    rangeMenu1->setPosition(CCPointZero);
    
    
    
    
    
    
    
    tempSprite1 = CCSprite::spriteWithFile("parcal_itemOff.png");
    tempSprite2 = CCSprite::spriteWithFile("parcal_itemOff.png");
    
    m_item_off = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(ParcelBackPage::cbCheckItemBtn));
    m_item_off->setAnchorPoint(CCPointZero);
    m_item_off->setPosition(CCPointMake(-10000, -10000));
    CCMenu *rangeMenu2 = CCMenu::menuWithItem(m_item_off);
    addChild(rangeMenu2,10);
    rangeMenu2->setPosition(CCPointZero);
    
    
    
    tempSprite1 = CCSprite::spriteWithFile("parcal_childOn.png");
    tempSprite2 = CCSprite::spriteWithFile("parcal_childOn.png");
    
    m_child_on = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(ParcelBackPage::cbCheckChildBtn));
    m_child_on->setAnchorPoint(CCPointZero);
    m_child_on->setPosition(CCPointMake(-10000, -10000));
    CCMenu *rangeMenu3 = CCMenu::menuWithItem(m_child_on);
    addChild(rangeMenu3,30);
    rangeMenu3->setPosition(CCPointZero);

}


void ParcelBackPage::cbCheckItemBtn()
{
    if(!m_choiceChild)
        return;
    

    m_item_on->setPosition(CCPointMake(40, 720));
    m_child_off->setPosition(CCPointMake(220, 720 + 4));
    
    
    
    m_item_off->setPosition(CCPointMake(-10000, -10000));
    m_child_on->setPosition(CCPointMake(-10000, -10000));

    
    m_choiceChild = false;
    
    CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyItem);
    
}


void ParcelBackPage::cbCheckChildBtn()
{
    if(m_choiceChild)
        return;
    
    
    m_item_off->setPosition(CCPointMake(40, 720 + 4));
    m_child_on->setPosition(CCPointMake(220, 720));
    
    
    
    m_item_on->setPosition(CCPointMake(-10000, -10000));
    m_child_off->setPosition(CCPointMake(-10000, -10000));
    
    
    m_choiceChild = true;
    
    CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyChild);
}

void ParcelBackPage::exit()
{
    removeAllChildrenWithCleanup(true);
}