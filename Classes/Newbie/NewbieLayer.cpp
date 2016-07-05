//
//  NewbieLayer.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "NewbieLayer.h"
#include "ShowSelCountry.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "CommDlg.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"

using namespace CocosDenshion;

#define kLiubeiHeadTag  0x1001
#define kLiubeiBtnTag   0x1002
#define kCaocaoHeadTag  0x1003
#define kCaocaoBtnTag   0x1004
#define kSunquanHeadTag 0x1005
#define kSunquanBtnTag  0x1006
#define kBgImgTag       0x1009
#define kCoverTag       0x1010
#define kMenuTag        0x1011

//******************************************************************************
// Construct
//******************************************************************************
NewbieLayer::NewbieLayer()
{
    m_iCountry = 0;
    m_showSelCtry = NULL;
    m_commDlg = NULL;
}

//******************************************************************************
// show
//******************************************************************************
void NewbieLayer::show()
{
    removeAllChildrenWithCleanup(true);
    
    //liubei
    CCSprite* spMenu1 = CCSprite::spriteWithFile("fr_liubei_head.png");
    CCSprite* spMenu2 = CCSprite::spriteWithFile("fr_liubei_head.png");
    CCMenuItem* liubei = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    liubei->setTag(kLiubeiHeadTag);
    liubei->setAnchorPoint(CCPointZero);
    liubei->setPosition(CCPointMake(85, 600 - 10));
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* liubeiBtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    liubeiBtn->setTag(kLiubeiBtnTag);
    liubeiBtn->setAnchorPoint(CCPointZero);
    liubeiBtn->setPosition(CCPointMake(344, 678 - 10));
    
    TextNode *lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip1"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(0, 0));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    liubeiBtn->addChild(lbText, 2);
    
    //caocao
    spMenu1 = CCSprite::spriteWithFile("fr_caocao_head.png");
    spMenu2 = CCSprite::spriteWithFile("fr_caocao_head.png");
    CCMenuItem* caocao = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    caocao->setTag(kCaocaoHeadTag);
    caocao->setAnchorPoint(CCPointZero);
    caocao->setPosition(CCPointMake(85, 345 - 10));
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* caocaoBtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    caocaoBtn->setTag(kCaocaoBtnTag);
    caocaoBtn->setAnchorPoint(CCPointZero);
    caocaoBtn->setPosition(CCPointMake(344, 430 - 10));
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip2"), CCSizeMake(188, 80 - 6), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(0, 0));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    caocaoBtn->addChild(lbText, 2);
    
    //sunquan
    spMenu1 = CCSprite::spriteWithFile("fr_sunquan_head.png");
    spMenu2 = CCSprite::spriteWithFile("fr_sunquan_head.png");
    CCMenuItem* sunquan = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    sunquan->setTag(kSunquanHeadTag);
    sunquan->setAnchorPoint(CCPointZero);
    sunquan->setPosition(CCPointMake(85, 90 - 10));
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* sunquanBtn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NewbieLayer::btnCallback) );
    sunquanBtn->setTag(kSunquanBtnTag);
    sunquanBtn->setAnchorPoint(CCPointZero);
    sunquanBtn->setPosition(CCPointMake(344, 172 - 10));
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip3"), CCSizeMake(188, 80), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(0, 0));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    sunquanBtn->addChild(lbText, 2);
    
    
    CCMenu* menu = CCMenu::menuWithItems(liubei, liubeiBtn, caocao, caocaoBtn, sunquan, sunquanBtn, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    menu->setTag(kMenuTag);
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("newBieLayerTip4"), CCSizeMake(640, 80), CCTextAlignmentCenter, 36);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(0, 830));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
}

//******************************************************************************
// onExit
//******************************************************************************
void NewbieLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    CCLayer::onExit();
}

//******************************************************************************
// btnCallback
//******************************************************************************
void NewbieLayer::btnCallback(CCObject* sender)
{
    CCMenuItem* pMI = (CCMenuItem*) sender;
    
    switch (pMI->getTag()) {
        case kLiubeiHeadTag:
        case kLiubeiBtnTag:
            m_iCountry = 2;
            break;
            
        case kCaocaoHeadTag:
        case kCaocaoBtnTag:
            m_iCountry = 1;
            break;
            
        case kSunquanHeadTag:
        case kSunquanBtnTag:
            m_iCountry = 3;
            break;
            
        default:
            break;
    }
    
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 960);
    addChild(lc, 10, kCoverTag);
    
    if(m_showSelCtry == NULL){
        m_showSelCtry = new ShowSelCountry(this, callfuncO_selector(NewbieLayer::cbShowSelCtry));
        addChild(m_showSelCtry, 20);
        m_showSelCtry->release();
        m_showSelCtry->show(m_iCountry);
        m_showSelCtry->setPosition(CCPointMake(640, 0));
        m_showSelCtry->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.9f));
    }
    
    CCNode* node = getChildByTag(kMenuTag);
    if(node)
        node->setPosition(CCPointMake(-1000, -1000));
}


//******************************************************************************
// cbShowSelCtry
//******************************************************************************
void NewbieLayer::cbShowSelCtry(CCObject* pObj)
{
    ShowSelCtryRet* pRet = (ShowSelCtryRet*)pObj;
    
    
    
    if (pRet->bOk) {
        if(m_showSelCtry != NULL){
            removeChild(m_showSelCtry, true);
        }
        
        CGameData::Inst()->newbieRequest(m_iCountry, 1, NULL);
        scheduleUpdate();
    }
    else {
        removeChild(m_showSelCtry, true);
        m_showSelCtry = NULL;
        
        CCNode* node = getChildByTag(kCoverTag);
        if(node)
            removeChild(node, true);
        
        node = getChildByTag(kBgImgTag);
        if(node)
            removeChild(node, true);
        
        node = getChildByTag(kMenuTag);
        if(node)
            node->setPosition(CCPointZero);
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void NewbieLayer::cbCommDlg(CCObject* pObj)
{    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    CGameData::Inst()->newbieRequest(m_iCountry, 1, NULL);
    m_commDlg = new CommDlg(this, callfuncO_selector(NewbieLayer::cbCommDlg), enCommDlgTp_connecting);
    addChild(m_commDlg, 1000);
    m_commDlg->setPosition(CCPointMake(320, 480));
    m_commDlg->release();
    scheduleUpdate();
}

//******************************************************************************
// update
//******************************************************************************
void NewbieLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(NewbieLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        MainScene::Inst()->newbieLogin();
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(NewbieLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }

}









