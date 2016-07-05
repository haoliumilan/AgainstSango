//
//  FightPauseMenuy.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "FightPauseMenu.h"
#include "CGamedata.h"
#include "Dialog2.h"
#include "TextNode.h"

FightPauseMenu::FightPauseMenu(CCObject *target,SEL_CallFuncO selector)
{
    m_listener = target;
    m_selctor = selector;
    char buf[100];
    
    exitBackground = NULL;
    
    coverLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 0), 640, 960);
    coverLayer->setOpacity(170);
    addChild(coverLayer);
    setIsTouchEnabled(true);
    
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -127, true);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem  *settingItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FightPauseMenu::callBackMenu));
    settingItem->setTag(11);
    settingItem->setPosition(ccp(-100, 400));
     
    snprintf(buf, 99, "%s",
             CGameData::Inst()->getLanguageValue("otherMainTip2"));
    TextNode *settingLabel = TextNode::textWithString(buf, 40);
    settingLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                          spMenu1->getContentSize().height * 0.5));
    settingItem->addChild(settingLabel);

    
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem  *exitItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FightPauseMenu::callBackMenu));
    exitItem->setTag(12);
    exitItem->setPosition(ccp(-100, 300));
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("exit"));
    TextNode *exitLabel = TextNode::textWithString(buf, 40);
    exitLabel->setPosition(spMenu1->getContentSize().width * 0.5,
                           spMenu1->getContentSize().height * 0.5);
    exitItem->addChild(exitLabel);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem  *backItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FightPauseMenu::callBackMenu));
    backItem->setTag(13);
    backItem->setPosition(ccp(-100, 200));
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("backTip"));
    TextNode *backLabel = TextNode::textWithString(buf, 40);
    backLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                       spMenu1->getContentSize().height * 0.5));
    backItem->addChild(backLabel);
    
    
    
    settingItem->setAnchorPoint(ccp(0, 0));
    exitItem->setAnchorPoint(ccp(0, 0));
    backItem->setAnchorPoint(ccp(0, 0));
    menu1 = CCMenu::menuWithItems(settingItem,exitItem,backItem,NULL);
    menu1->setPosition(ccp(320, 100));
    addChild(menu1);

    titleLabel = TextNode::textWithString("", 50);
    titleLabel->setPosition(CCPoint(320, 750));
    addChild(titleLabel);
    
    settingLayer = new SettingLayer(this, callfuncO_selector(FightPauseMenu::setCallBack));
    addChild(settingLayer);
    settingLayer->setPosition(1040,0);
}
void FightPauseMenu::setCallBack(CCObject *pSender)
{
    menu1->setIsTouchEnabled(true);
    settingLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(0.5,CCPoint(1040, 0)),0.8f));
}

void FightPauseMenu::exitCallBack(CCObject *pSender)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pSender;
    
    if(pRet->bOk == true){
        iRet.iBtnSel = 21;
        (m_listener->*m_selctor)(&iRet);
    }
    else{
        menu1->setIsTouchEnabled(true);
    }
    
    removeChild(exitBackground, true);
    exitBackground = NULL;
}
bool FightPauseMenu::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void FightPauseMenu::callBackMenu(CCObject *pSender)
{
    
    CCMenuItem* menuItem = (CCMenuItem*) pSender;
    CCLog("pauseMenu %d",menuItem->getTag());
    if(menuItem->getTag() == 11)
    {
        menu1->setIsTouchEnabled(false);
        settingLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(0.5,CCPoint(0, 0)),0.8f));
    }
    else if(menuItem->getTag() == 12)
    {
        if(!CGameData::Inst()->isNewbie())
        {
            if(exitBackground == NULL){
                exitBackground = new Dialog2(this, callfuncO_selector(FightPauseMenu::exitCallBack), 
                                             NULL, CGameData::Inst()->getLanguageValue("exit_prompt"),enDl2TwoBtn);
                addChild(exitBackground);
                exitBackground->release();
                exitBackground->setPosition(CCPointMake(320, 480));
                exitBackground->setScale(0);
                exitBackground->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            }

        }else 
            return;
        menu1->setIsTouchEnabled(false);

//        exitBackground->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(0.5, CCPoint(320,430)),0.8f));
//         menu1->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(0.5, CCPoint(-640,100)),0.8f));
    }
    else  
    {
        iRet.iBtnSel = menuItem->getTag();
        (m_listener->*m_selctor)(&iRet);
    }
}

void FightPauseMenu::setTitleString(const char* titleStr)
{
    titleLabel->setString(titleStr);
}

void FightPauseMenu::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}





