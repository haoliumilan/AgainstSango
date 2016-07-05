//
//  SettingLayer.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//
/////////////////////////////////////////////////////////
#include "SettingLayer.h"
#include "CGamedata.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "CommonDef.h"
#include "TextNode.h"
/////////////////////////////////////////////////////////

# define kMusicOnTag        41
# define kMusicOffTag       42
# define kSoundOnTag        43
# define kSoundOffTag       44
# define kSkillSoundOnTag   45
# define kSkillSoundOffTag  46

/////////////////////////////////////////////////////////
using namespace CocosDenshion;

SettingLayer::SettingLayer(CCObject* target, SEL_CallFuncO selector)
{
    //callBack
    m_pListener = target;
    m_pfnSelector = selector;

    m_backGround = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    m_backGround->setScaleX(2);
    m_backGround->setScaleY(2.8);
    m_backGround->setPosition(ccp(320, 480));
    addChild(m_backGround);
    int ajustY = 240;

    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    backItem =  CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(SettingLayer::menuCallback));
    backItem->setAnchorPoint(ccp(0, 0));
    backItem->setPosition(ccp(0, -60));
    backItem->setTag(1);
  
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("backTip"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    
    CCPoint p1 = itemLabel->getPosition();
    p1.x-=2;
    p1.y+=2;
    backItem->addChild(itemLabel);
    
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setPosition(p1);
    
    backItem->setAnchorPoint(ccp(0, 0));
    
    TextNode *settingLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("otherMainTip2"), 50);
    settingLabel->setPosition(ccp(320, 490+ajustY));
    settingLabel->setColor(ccc3(123, 36, 8));
    addChild(settingLabel);
    
    TextNode *musicSettingLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("settingTip1"), 30);
    musicSettingLabel->setPosition(ccp(320, 400+ajustY));
    musicSettingLabel->setColor(ccc3(82, 50, 12));
    addChild(musicSettingLabel);
    
    TextNode *soundSettingLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("settingTip2"), 30);
    soundSettingLabel->setPosition(ccp(320, 280+ajustY));
    soundSettingLabel->setColor(ccc3(82, 50, 12));
    addChild(soundSettingLabel);
    
    TextNode* skill_setting_label = TextNode::textWithString(CGameData::Inst()->getLanguageValue("settingTip4"), 30);
    skill_setting_label->setPosition(ccp(320, 160+ajustY));
    skill_setting_label->setColor(ccc3(82, 50, 12));
    addChild(skill_setting_label);

    // 背景音乐
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    CCSprite* spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_open2.png");
    musicOpenItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                         this, menu_selector(SettingLayer::musicSoundTurn));
    musicOpenItem->setPosition(ccp(-100, 290));
    musicOpenItem->setAnchorPoint(ccp(0, 0));
    musicOpenItem->setTag(kMusicOnTag);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_close2.png");
    musicCloseItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                         this, menu_selector(SettingLayer::musicSoundTurn));
    musicCloseItem->setPosition(ccp(100, 290));
    musicCloseItem->setAnchorPoint(ccp(0, 0));
    musicCloseItem->setTag(kMusicOffTag);
    
    musicOpenItem->setIsEnabled(!CGameData::Inst()->isMusicOn);
    musicCloseItem->setIsEnabled(CGameData::Inst()->isMusicOn);
    
    // 音效
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_open2.png");
    soundOpenItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                          this, menu_selector(SettingLayer::musicSoundTurn));
    soundOpenItem->setPosition(ccp(-100, 170));
    soundOpenItem->setAnchorPoint(ccp(0, 0));
    soundOpenItem->setTag(kSoundOnTag);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_close2.png");
    soundCloseItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                         this, menu_selector(SettingLayer::musicSoundTurn));

    soundCloseItem->setPosition(ccp(100, 170));
    soundCloseItem->setAnchorPoint(ccp(0, 0));
    soundCloseItem->setTag(kSoundOffTag);
    
    soundOpenItem->setIsEnabled(!CGameData::Inst()->isSoundOn);
    soundCloseItem->setIsEnabled(CGameData::Inst()->isSoundOn);
    
    // 技能特效
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_open2.png");
    skillOpenItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                          this, menu_selector(SettingLayer::musicSoundTurn));
    skillOpenItem->setPosition(ccp(-100, 50));
    skillOpenItem->setAnchorPoint(ccp(0, 0));
    skillOpenItem->setTag(kSkillSoundOnTag);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_close2.png");
    skillClostItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                           this, menu_selector(SettingLayer::musicSoundTurn));
    
    skillClostItem->setPosition(ccp(100, 50));
    skillClostItem->setAnchorPoint(ccp(0, 0));
    skillClostItem->setTag(kSkillSoundOffTag);
    
    skillOpenItem->setIsEnabled(!CGameData::Inst()->isSkillOn);
    skillClostItem->setIsEnabled(CGameData::Inst()->isSkillOn);

    // menu
    CCMenu *menu = CCMenu::menuWithItems (backItem,musicOpenItem,musicCloseItem,
                                          soundOpenItem,soundCloseItem, skillOpenItem, skillClostItem,NULL);
    menu->setPosition(ccp(216+24, 20+ajustY));
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu);
    
}

void SettingLayer::refreshSetting()
{
    CCPoint point;
    musicOpenItem->setIsEnabled(!CGameData::Inst()->isMusicOn);
    musicCloseItem->setIsEnabled(CGameData::Inst()->isMusicOn);
        
    soundOpenItem->setIsEnabled(!CGameData::Inst()->isSoundOn);
    soundCloseItem->setIsEnabled(CGameData::Inst()->isSoundOn);
    
    skillOpenItem->setIsEnabled(!CGameData::Inst()->isSkillOn);
    skillClostItem->setIsEnabled(CGameData::Inst()->isSkillOn);
    
        
}

void SettingLayer::setIsFighting(bool b)
{
    isFighting = b;
}
void SettingLayer::musicSoundTurn(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItemImage *temItem = (CCMenuItemImage *)pSender;
    if(temItem->getTag() == kMusicOnTag)
    {

        musicOpenItem->setIsEnabled(false);
        musicCloseItem->setIsEnabled(true);

        char buf[50];
        if(MainScene::Inst()->getBackMusic() == enBgMusicNml) {
            if (CGameData::Inst()->getIsUseChristmasUI()) {
                sprintf(buf, "normal_newyear.mp3");
            }
            else {
                sprintf(buf, "normal.mp3");
            }
        }
        else if(MainScene::Inst()->getBackMusic() == enBgMusicBattle)
            sprintf(buf, "battle.mp3");
        else if(MainScene::Inst()->getBackMusic() == enBgMusicOrgFight)
            sprintf(buf, "OrgFight.mp3");
        else 
            sprintf(buf, "bossbattle.mp3");
        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(buf, true);
        CGameData::Inst()-> setMusicValue(true);
    }else if(temItem->getTag() == kMusicOffTag){
        
        musicOpenItem->setIsEnabled(true);
        musicCloseItem->setIsEnabled(false);        
        SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CGameData::Inst()-> setMusicValue(false);

    }else if(temItem->getTag() == kSoundOnTag){
        soundOpenItem->setIsEnabled(false);
        soundCloseItem->setIsEnabled(true);
        CGameData::Inst()->setSoundValue(true);

    }else if(temItem->getTag() == kSoundOffTag){
        soundOpenItem->setIsEnabled(true);
        soundCloseItem->setIsEnabled(false);
        CGameData::Inst()->setSoundValue(false);
    }
    else if(temItem->getTag() == kSkillSoundOnTag)
    {
        skillOpenItem->setIsEnabled(false);
        skillClostItem->setIsEnabled(true);
        CGameData::Inst()->setSkillValue(true);
    }
    else if (temItem->getTag() == kSkillSoundOffTag)
    {
        skillOpenItem->setIsEnabled(true);
        skillClostItem->setIsEnabled(false);
        CGameData::Inst()->setSkillValue(false);
    }
}
void SettingLayer::menuCallback(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* menuItem = (CCMenuItem*) pSender;
    
    m_ret.iBtnSel = menuItem->getTag();
    
    (m_pListener->*m_pfnSelector)(&m_ret);
        
}