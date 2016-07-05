//
//  ChatRoom.cpp
//  AgainstWar
//
//  Created by liang jun on 13-4-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "ChatRoom.h"
#include "OcProxy.h"
#include "CommDlg.h"
#include "MainScene.h"

#define CHATINPUTTAG 201304023

#define kReqTypeSendMsg 988

//******************************************************************************
// Construct
//******************************************************************************
ChatRoom::ChatRoom(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_commDlg = NULL;
    m_bImeShowing = false;
    m_iReqType = 0;
    
    addAllchild();
    textFieldInit();
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    schedule(schedule_selector(ChatRoom::update), 0.2f);
    
    CGameData::Inst()->startMonitorChatRx();
    
    OcProxy::Inst()->showWebWithType(enWebviewLoadTypeHtmlStr, "",CCRect(82, 310, 480, 370));
}


ChatRoom::~ChatRoom()
{
    CGameData::Inst()->stopMonitorChatRx();
    OcProxy::Inst()->removeWebview();
}


void ChatRoom::update(ccTime dt)
{
    if (m_bImeShowing) {
        if(!CCDirector::sharedDirector()->getOpenGLView()->getIMEKeyboardState()){
            m_bImeShowing = false;
            
            stopAllActions();
            setPosition(ccp(0.0f, 0.0f));
            
            OcProxy::Inst()->setFrame(CCRect(82, 310, 480, 370));
        }
    }
    
    if (CGameData::Inst()->isNewChatRxCame()) {
        string strMsg = CGameData::Inst()->getChatRxString();
        OcProxy::Inst()->showHtmlStr(strMsg.c_str(), true);
    }
    
    if (m_iReqType == kReqTypeSendMsg) {
        int comState = CGameData::Inst()->getReqStat();
        if(comState == kReqStatRequesting){
            MainScene::Inst()->enableBottomPanel(false);
            return;
        }
        
        MainScene::Inst()->enableBottomPanel(true);
        
        if(comState == kReqStatOk){
            m_textField->setString("");
            m_textField->setPosition(ccp(90,m_textField->getPosition().y));
            m_textField->setAnchorPoint(ccp(0, 0.5));
            
            m_iReqType = 0;
        }
        else{
            if(comState == 1){
                CGameData::Inst()->pushErrMsgToChatLst();
            }
            
            m_iReqType = 0;
            CGameData::Inst()->clearReqStat();
        }
    }
}

void ChatRoom::cbCommDlg(CCObject* )
{
    
}

void ChatRoom::textFieldInit()
{
    CCSize size ;
    size.height = 80;
    size.width = 640;
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder(CGameData::Inst()->getLanguageValue("cntry_click_input"),"Helvetica", 26);
    m_textField->setAnchorPoint(ccp(0, 0.5));
    m_textField->setPosition(ccp(90, 245));
    m_textField->setColor(ccWHITE);
    m_textField->setDelegate(this);
    addChild(m_textField,100);
    
    
    CCPoint pos(0,0);
    float fFactor = 1.0;
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    if(winsz.width > 1000)
    {
        fFactor = 4.0;
        pos = CCPointMake(128, 64);
    }
    else if(winsz.width == 768)
    {
        fFactor = 2.0;
        pos = CCPointMake(64, 32);
    }
    else if(winsz.width == 640)
    {
        fFactor = 2.0;
    }
    
    m_textField->setClipRect(1,45*fFactor+pos.x, 0, 196*fFactor, 3000);
    
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNamePhonePad();
}

void ChatRoom::addAllchild()
{
    CCSprite* spBg = CCSprite::spriteWithFile("fr_cntryDetails.png");
    spBg->setPosition(CCPointMake(320, 438));
    spBg->setScaleY(0.85f);
    addChild(spBg);
    
    CCSprite* spTitle = CCSprite::spriteWithFile("chatroomtitle.png");
    spTitle->setPosition(ccp(315,693));
    addChild(spTitle);
    
    CCSprite* inputBox = CCSprite::spriteWithFile("chatinput.png");
    inputBox->setPosition(ccp(320,245));
    addChild(inputBox,0,CHATINPUTTAG);
    
    CCMenuItemImage* menuIMG = CCMenuItemImage::itemFromNormalImage("chatsend.png", "chatsend.png", this, menu_selector(ChatRoom::btnCallBack));
    menuIMG->setAnchorPoint(ccp(0, 0));
    menuIMG->setPosition(ccp(490,221));
    
    CCMenu *menu = CCMenu::menuWithItems(menuIMG,NULL);
    menu->setPosition(ccp(0,0));
    addChild(menu);
}


void ChatRoom::GoManagePage(CCObject* pSender)
{
     (m_pListener->*m_pfnSelector)();
}

void ChatRoom::btnCallBack(CCObject* pSender)
{
    if ((m_textField->getString() == NULL)
        || (*(m_textField->getString()) == 0)){
        return;
    }
    
    if((m_iReqType == 0) && CGameData::Inst()->sendMsg(m_textField->getString()))
        m_iReqType = kReqTypeSendMsg;
    
    if (m_bImeShowing)
        hideIMEKeyboard();
}

void ChatRoom::showIMEKeyboard()
{
    m_textField->attachWithIME();
    
    float fHeight = 286.0f;
    
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(winsz.width > 1000 || winsz.width == 768)
    {
        fHeight = 150.0f;
    }
    
    CCAction* moveUp = CCMoveTo::actionWithDuration(0.2f, ccp(0.0f, fHeight));
    runAction(moveUp);
    
    OcProxy::Inst()->setFrame(CCRect(82, 240, 480, 370 - fHeight + 66));
    
    m_bImeShowing = true;
}

void ChatRoom::hideIMEKeyboard()
{
    m_textField->detachWithIME();
    
    CCAction* moveDown = CCMoveTo::actionWithDuration(0.2f, ccp(0.0f, 0.0f));
    runAction(moveDown);
    
    OcProxy::Inst()->setFrame(CCRect(82, 310, 480, 370));
    
    m_bImeShowing = false;
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool ChatRoom::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    if (!CCDirector::sharedDirector()->getOpenGLView()->getIMEKeyboardState())
    {
        CCSprite* SP = (CCSprite*)(getChildByTag(CHATINPUTTAG));
        
        if (CCRect::CCRectContainsPoint(CCRect(SP->getPosition().x-SP->getContentSize().width/2, SP->getPosition().y - SP->getContentSize().height/2, SP->getContentSize().width, SP->getContentSize().height), pos))
        {
            showIMEKeyboard();
        }
    }
    else
    {
        hideIMEKeyboard();
    }
        

    return true;
    
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void ChatRoom::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void ChatRoom::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
  
}

//******************************************************************************
// menuCallBack
//******************************************************************************
//******************************************************************************
// onEnter
//******************************************************************************
void ChatRoom::onEnter()
{
    CCLayer::onEnter();
}

//******************************************************************************
// onExit
//******************************************************************************
void ChatRoom::onExit()
{
    m_textField->detachWithIME();
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}


#pragma mark -
#pragma mark CCTextFieldDelegate
bool ChatRoom::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    
    if (1 == nLen &&  (*text == 0x0A))
    {
        CCAction* moveDown = CCMoveTo::actionWithDuration(0.2f, ccp(0.0f, 0.0f));
        runAction(moveDown);
        
        OcProxy::Inst()->setFrame(CCRect(82, 310, 480, 370));
    }
    
//    float w = m_textField->getContentSize().width;
//    
//    CCLabelTTF* label = CCLabelTTF::labelWithString(text, "Helvetica", 26);
//    if (w + label->getContentSize().width > (45+196)*2)
//    {
//        m_textField->setPosition(ccp((45+196)*2,m_textField->getPosition().y));
//        m_textField->setAnchorPoint(ccp(1, 0.5));
//    }
//    else
//    {
//        m_textField->setPosition(ccp(92,m_textField->getPosition().y));
//        m_textField->setAnchorPoint(ccp(0, 0.5));
//    }

    return false;
}

bool ChatRoom::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}


void ChatRoom::afterUpdate()
{
    if (m_textField->getContentSize().width > (45+196)*2)
    {
        m_textField->setPosition(ccp((45+196)*2,m_textField->getPosition().y));
        m_textField->setAnchorPoint(ccp(1, 0.5));
    }
    else
    {
        m_textField->setPosition(ccp(90,m_textField->getPosition().y));
        m_textField->setAnchorPoint(ccp(0, 0.5));
    }
}





