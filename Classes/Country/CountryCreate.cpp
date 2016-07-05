//
//  CountryCreate.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-26.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CountryCreate.h"
#include "CommonDef.h"
#include "TextNode.h"
#include "CGameData.h"
#include "SimpleAudioEngine.h"
#include "Dialog2.h"
#include "FbdWords.h"
#include "CountryDialog.h"
#include "dialog0.h"

using namespace CocosDenshion;

#define enCCBtn_Name        1001
#define enCCBtn_Manifesto   1002
#define enCCBtn_Create      1003
#define enCCBtn_Shadow    1004

CountryCreate::CountryCreate(CCObject* target, SEL_CallFunc selector)
{
    m_pListener      =  target;
    m_pfnSelector    =  selector;
    m_keyboardStatus =  enKeyboard_Hide;
    m_dlCoinAlert    =  NULL;
    m_FbdWords       =  NULL;
    
    m_bShowName      =  false;
    m_bShowMani      =  false;
}

CountryCreate::~CountryCreate()
{

}

void CountryCreate::onEnter()
{
    CCLayer::onEnter();
}

void CountryCreate::onExit()
{
    if (m_keyboardStatus != enKeyboard_Hide) {
        hidekeyboard();
    }
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void CountryCreate::showCntryCreatDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 480));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_createcontent1"),
                                        CCSizeMake(600, 40),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(320, 650));
    tempText->setColor(ccBLACK);
    addChild(tempText);
        
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    CCMenuItem* mItemName = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                  spMenu2,
                                                                  this,
                                                                  menu_selector(CountryCreate::menuCallBack));
    mItemName->setAnchorPoint(CCPointZero);
    mItemName->setTag(enCCBtn_Name);
    mItemName->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                       550));

    
    m_tfCntryName = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                             spMenu1->getContentSize(),
                                                             CCTextAlignmentCenter,
                                                             FONTSTHEITISCM,
                                                             26);
    m_tfCntryName->setPosition(CCPointMake(320, 570));
    m_tfCntryName->setDelegate(this);
    m_tfCntryName->setColor(ccBLACK);
    addChild(m_tfCntryName, 1);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_createcontent2"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 520));
    tempText->setColor(ccBLACK);
    addChild(tempText);
        
    spMenu1 = CCSprite::spriteWithFile("bg_declaration.png");
    spMenu2 = CCSprite::spriteWithFile("bg_declaration.png");
    CCMenuItem* mItemManifesto = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                       spMenu2,
                                                                       this,
                                                                       menu_selector(CountryCreate::menuCallBack));
    mItemManifesto->setAnchorPoint(CCPointZero);
    mItemManifesto->setTag(enCCBtn_Manifesto);
    mItemManifesto->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                            350));

    
    m_tfCntryManifesto = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                                  CCSizeMake(spMenu1->getContentSize().width - 10,
                                                                             spMenu1->getContentSize().height - 10),
                                                                  CCTextAlignmentCenter,
                                                                  FONTSTHEITISCM,
                                                                  26);
    m_tfCntryManifesto->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                                spMenu1->getContentSize().height * 0.5));
    m_tfCntryManifesto->setDelegate(this);
    m_tfCntryManifesto->setColor(ccBLACK);
    mItemManifesto->addChild(m_tfCntryManifesto, 1);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    
    
    
    m_mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryCreate::menuCallBack));
    m_mItemOK->setTag(enCCBtn_Create);
    m_mItemOK->setAnchorPoint(CCPointZero);
    m_mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     260));

    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_create_alone"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tempText->setShadowColor(ccBLACK);
    m_mItemOK->addChild(tempText);
    
    
    //默认ok按钮为disable 状态  tangaowen ，后续根据title栏的输入来变化
    //m_mItemOK->setIsEnabled(false);
    CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
    spShadow->setTag(enCCBtn_Shadow);
    spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    m_mItemOK->addChild(spShadow);
    
    
    
    CCMenu* menu = CCMenu::menuWithItems(mItemName, mItemManifesto, m_mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void CountryCreate::menuCallBack(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* pRet = (CCMenuItem*)pObj;
    switch (pRet->getTag()) {
        case enCCBtn_Name:
            if (m_keyboardStatus == enKeyboard_NameShow) {
                hidekeyboard();
            }
            else {
                if ( m_dlCoinAlert == NULL)
                {
                    showKeyboard(m_tfCntryName);
                    m_keyboardStatus = enKeyboard_NameShow;
                }
            }
            
            break;
            
        case enCCBtn_Manifesto:
            if (m_keyboardStatus == enKeyboard_ManifestoShow) {
                hidekeyboard();
            }
            else {
                if ( m_dlCoinAlert == NULL)
                {
                    showKeyboard(m_tfCntryManifesto);
                    m_keyboardStatus = enKeyboard_ManifestoShow;
                }
            }
            
            break;
            
        case enCCBtn_Create:
        {
            hidekeyboard();
            if (CGameData::Inst()->getUsrInfo()->coin < CGameData::Inst()->getCommonInfo()->cntry_create_coin) {
                showCoinAlert();
            }
            else {
                (m_pListener->*m_pfnSelector)();
            }
        }
            
            break;
            
        default:
            break;
    }

}

bool CountryCreate::creatCountry()
{
    bool bRet = false;
    string ss1 = (string) m_tfCntryName->getString();
    if (ss1 == "")
        return false;
    string ss2 = (string) m_tfCntryManifesto->getString();
    if (FbdWords::Ins()->CheckLegal(ss1) || FbdWords::Ins()->CheckLegal(ss2))
    {
        createCountryError();
        return false;
    }

    if (CheckAllNumber(ss1))
    {
        createCountryNumber();
        return false;
    }
    if (strlen(m_tfCntryName->getString())) {
        bRet = CGameData::Inst()->createCountryRequest(m_tfCntryName->getString(),
                                                       m_tfCntryManifesto->getString());
    }
    return bRet;
}

bool CountryCreate::CheckAllNumber(string str0)
{
    int n = str0.length();
    for (int i = 0; i < n; i++)
    {
        if (str0[i]>57 || str0[i] <48)  // '0' = 48   '9' = 57
            return false;
    }
    return true;
}

void CountryCreate::createCountryError()
{
    if (m_FbdWords == NULL)
    {
        char buf[50];
        snprintf(buf, 49, "%s", CGameData::Inst()->getLanguageValue("cntry_fbdwords_content1"));
        m_FbdWords = new Dialog0(this, callfunc_selector(CountryCreate::cbFbdWords), buf);
        m_FbdWords -> autorelease();
        addChild(m_FbdWords, 100);
        m_FbdWords -> setPosition(CCPointMake(320, 480));
    }
}

void CountryCreate::createCountryNumber()
{
    if (m_FbdWords == NULL)
    {
        char buf[50];
        snprintf(buf, 49, "%s", CGameData::Inst()->getLanguageValue("cntry_fbdwords_content2"));
        m_FbdWords = new Dialog0(this, callfunc_selector(CountryCreate::cbFbdWords), buf);
        m_FbdWords -> autorelease();
        addChild(m_FbdWords, 100);
        m_FbdWords -> setPosition(CCPointMake(320, 480));
    }
}

void CountryCreate::cbFbdWords()
{
    if (m_FbdWords)
        m_FbdWords -> removeFromParentAndCleanup(true);
    m_FbdWords = NULL;
}

void CountryCreate::showKeyboard(cocos2d::CCTextFieldTTF *&sender)
{
    //这个地方，运行的action会影响到 获得当前node的position，然后对position进行偏移的处理，所以
    //先停下actions,然后做偏移处理。tangaowen 2013.2.5
    this->stopAllActions();
    if (m_FbdWords != NULL)
        return;

    CCPoint    oldPoint = this->getPosition();
    
    int  tempHeight = 0;
    
    if (sender == m_tfCntryName)
    {
        if (m_bShowMani)
        {
            m_bShowMani = false;
            
            tempHeight -= MANI_FIELD_GAP;
        }
        
        tempHeight += NAME_FIELD_GAP;
        this->setPosition(CCPointMake(oldPoint.x, oldPoint.y +  tempHeight));
        m_bShowName =  true;
    }
    else 
    {
        if (m_bShowName)
        {
            m_bShowName = false;
            
            tempHeight -= NAME_FIELD_GAP;
        }
        
        tempHeight += MANI_FIELD_GAP;
        this->setPosition(CCPointMake(oldPoint.x, oldPoint.y + tempHeight));
        m_bShowMani = true;
    }
    

    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNamePhonePad();
    sender->attachWithIME();
    
    
}

void CountryCreate::hidekeyboard()
{
    CCPoint    oldPoint = this->getPosition();
    
    if (m_bShowName)
    {
        this->setPosition(CCPointMake(oldPoint.x, oldPoint.y- NAME_FIELD_GAP));
        
        m_bShowName = false;
    }
    
    else if (m_bShowMani)
    {
         this->setPosition(CCPointMake(oldPoint.x, oldPoint.y- MANI_FIELD_GAP));
        
        m_bShowMani  =  false;
    }
    
    
    m_tfCntryName->detachWithIME();
    m_tfCntryManifesto->detachWithIME();
    m_keyboardStatus = enKeyboard_Hide;
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
}

void CountryCreate::showCoinAlert()
{
    if (m_dlCoinAlert == NULL) {
        m_dlCoinAlert = new Dialog2(this,
                                    callfuncO_selector(CountryCreate::cbCoinAlert),
                                    CGameData::Inst()->getLanguageValue("cntry_hint"),
                                    CGameData::Inst()->getLanguageValue("cntry_hintcontent3"),
                                    enDl2OneBtn);
        addChild(m_dlCoinAlert, 100);
        m_dlCoinAlert->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_dlCoinAlert->release();
        m_dlCoinAlert->setScale(0.0);
        m_dlCoinAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
}

void CountryCreate::cbCoinAlert()
{
    m_dlCoinAlert->removeFromParentAndCleanup(true);
    m_dlCoinAlert = NULL;
}

#pragma mark -
#pragma mark CCTouchDelegate
bool CountryCreate::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_keyboardStatus != enKeyboard_Hide) {
        hidekeyboard();
    }
    return true;
}


#pragma mark -
#pragma mark CCTextFieldDelegate
bool CountryCreate::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    
    //判断如果是 国家标题，那么判断是否charCount 大于0 ,tangaowen  2013.2.5
    if (sender ==  m_tfCntryName)
    {
         CCSprite  *  shadowSp =  (CCSprite *)  m_mItemOK->getChildByTag(enCCBtn_Shadow);
        
        //判断是否disable
        if (sender->getCharCount()  +  nLen   > 0)
        {
            m_mItemOK->setIsEnabled(true);
            
            if (shadowSp )
            {
                shadowSp->setIsVisible(false);
            }
        }
        else
        {
            m_mItemOK->setIsEnabled(false);
            
            if (shadowSp )
            {
                shadowSp->setIsVisible(true);
            }
        }
    }
    
    return false;
}


/**
 @brief	If the sender doesn't want to delete the delText, return true;
 */
bool CountryCreate::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    //判断如果是 国家标题，那么判断是否charCount 大于0 , tangaowen  2013.2.5
    if (sender ==  m_tfCntryName)
    {
        CCSprite  *  shadowSp =  (CCSprite *)  m_mItemOK->getChildByTag(enCCBtn_Shadow);
        
        const  char  *   curChars = sender->getString();
        if (strlen(curChars) -  nLen  > 0)
        {
            m_mItemOK->setIsEnabled(true);
            
            if (shadowSp )
            {
                shadowSp->setIsVisible(false);
            }
        }
        else
        {
            m_mItemOK->setIsEnabled(false);
            
            if (shadowSp )
            {
                shadowSp->setIsVisible(true);
            }
        }
    }
    
    return false;
}



