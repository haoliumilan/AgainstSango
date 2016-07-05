//
//  Dialog1.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Dialog1.h"
#include "TextNode.h"
#include "CGamedata.h"
#include "OcProxy.h"

#define kDlg1BtnTag 0x911
//******************************************************************************
// Constructor
//******************************************************************************
Dialog1::Dialog1(CCObject* target, SEL_CallFuncO selector, const char* dsp, bool isWelcomeDlg)
{
    m_pListener = target;
    m_pfnSelector = selector;
    char buf[100];
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
    
    if (isWelcomeDlg) {
        CCSprite* sp;
        sp = CCSprite::spriteWithFile("firstlogindlg.png");
        sp->setPosition(CCPointMake(0, 50));
        if(sp)
            addChild(sp);
        
        //ok
        sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0, -128));
            addChild(sp, 0, kDlg1BtnTag);
        }
            
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(0, -123));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);      
    }
    else {
        CCSprite* sp;
        sp = CCSprite::spriteWithFile("ma_prompt.png");
        if(sp)
            addChild(sp);
        
        //ok
        sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0, -62));
            addChild(sp, 0, kDlg1BtnTag);
        }
        if (strlen(CGameData::Inst()->getLanguageValue("sureTip")) != 0 ) {
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
        }else{
            snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("sureTip"));
        }

       // snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter, 36);
        lbText->setPosition(CCPointMake(0, -62));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);

        //dsp
        lbText = TextNode::textWithString(dsp, CCSizeMake(560, 130), CCTextAlignmentCenter, 30);
        lbText->setPosition(CCPointMake(0, 40));
        lbText->setColor(ccc3(63, 23, 13));
        addChild(lbText, 1);
    }
    
}

Dialog1::Dialog1(CCObject* target, SEL_CallFuncO selector, const char* dsp)
{
    m_pListener = target;
    m_pfnSelector = selector;
    char buf[100];
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
    
    
    CCSprite* sp;
    sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    if(sp)
        addChild(sp);
    sp->setScale(2.0);
    //ok
    sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(sp){
        sp->setPosition(CCPointMake(0, -185));
        addChild(sp, 0, kDlg1BtnTag);
    }
    
    if (strlen(CGameData::Inst()->getLanguageValue("sureTip")) != 0 ) {
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
    }else{
        snprintf(buf, 99, "%s", OcProxy::Inst()->getLocalizedString("sureTip"));
    }
//    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter, kBtnTitleHeight);
    lbText->setPosition(CCPointMake(sp->getContentSize().width * 0.5, sp->getContentSize().height * 0.5));
    lbText->setScale(1.1);
    sp->addChild(lbText, 1);
    
    //dsp
    lbText = TextNode::textWithString(dsp, CCSizeMake(500, 130), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, -330));
    lbText->setColor(kShowInfoColor);
    addChild(lbText, 1);
    
    
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool Dialog1::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* node = getChildByTag(kDlg1BtnTag);
    if(node == NULL)
        return true;
    
    CCPoint pt = node->getPosition();
    CCSize sz = node->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    
    if(bRet){
        m_ret.bOk = true;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    
    return true;
}

//******************************************************************************
// onExit
//******************************************************************************
void Dialog1::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}