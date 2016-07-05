//
//  CSearchFriend.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CSeachFriend.h"
#include "CommonDef.h"
#include "TextNode.h"

void CSeachFriend::enter()
{
    isAttach = true; 
    textField->setString("");
    char myId[100];
    CUserInfo *myInfo = CGameData::Inst()->getUsrInfo();
    sprintf(myId, "%s%s",CGameData::Inst()->getLanguageValue("searchFriendTip1"),myInfo->uid);
    myIdLabel->setString(myId);

    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    textField->attachWithIME();

    runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 565)), 0.8f));
   
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

void CSeachFriend::exit()
{
    setPosition(ccp(-700, 565));
    removeDispacher();
}

void CSeachFriend::onExit()
{
    removeDispacher();
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

CSeachFriend::CSeachFriend(CCObject* target, SEL_CallFunc selector)
{
    int delta = 120+160;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    isAttach = false;
    m_background = CCSprite::spriteWithFile("fr_window3.png");
    m_background->setScale(2);
    m_background->setPosition(ccp(320, 0));
    addChild(m_background);
  
    CCSize size ;
    size.height = 80;
    size.width = 640;
    
    CCSprite *frIdBack = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    frIdBack->setPosition(ccp(320, 250-delta));
    addChild(frIdBack);
    
    TextNode *tintLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("searchFriendTip2"), 26);
    addChild(tintLabel);
    tintLabel->setPosition(ccp(320, 300-delta));
    tintLabel->setColor(ccc3(82, 48, 11));
    
    textField = CCTextFieldTTF::textFieldWithPlaceHolder("",size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
    textField->setPosition(ccp(320, 250-delta));
    
    textField->setDelegate(this);
    addChild(textField);
    
    CCSprite *myIdBack = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    myIdBack->setPosition(ccp(320, 370-delta));
    addChild(myIdBack);
    
    myIdLabel = TextNode::textWithString("", 26);
    myIdLabel->setColor(ccc3(82, 28, 11));
    myIdLabel->setPosition(ccp(320, 370-delta));
    addChild(myIdLabel);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    searchItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                       spMenu2,
                                                       this,
                                                       menu_selector(CSeachFriend::menuCallback));
    
    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("searchFriendTip3"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPoint(spMenu1->getContentSize().width * 0.5 - 2,
                                   spMenu1->getContentSize().height * 0.5 + 2));
    searchItem->addChild(itemLabel);

    itemLabel->setColor(ccc3(255, 255, 255));

    searchItem->setAnchorPoint(ccp(0, 0));
    CCMenu *menu = CCMenu::menuWithItem(searchItem);
    menu->setPosition(ccp(216+24, 140-delta-5));
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu);

   
    setPosition(ccp(-700, 565));

}


CSeachFriend::~CSeachFriend()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);

}


void CSeachFriend::menuCallback(CCObject *pSender)
{
    if (strlen(textField->getString()) == 0)
        return;
    
    textField->detachWithIME();
    
    (m_pListener->*m_pfnSelector)();
}

void CSeachFriend::removeDispacher()
{
    textField->detachWithIME();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
    isAttach = false;

}

bool CSeachFriend::ccTouchBegan(CCTouch *touch, CCEvent* event){
    if(isAttach == false)
    {
        textField->attachWithIME();
    }
    else { 
        textField->detachWithIME();
    }
    isAttach = !isAttach;
 
    return true;
}

bool  CSeachFriend::requestSearchFriend()
{
   return CGameData::Inst()->schFriendRequest(textField->getString());
}

#pragma mark -
#pragma mark CCTextFieldDelegate
bool CSeachFriend::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());

    if (sender->getCharCount() + nLen > 9) {
        return true;
    }
    if (*text <= 57 && *text >= 48) {
        return false;
    }
    else {
        return true;
    }
}

bool CSeachFriend::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen) {
    return false;
}

