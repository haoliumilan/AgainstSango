//
//  CountryDialog.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-27.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CountryDialog.h"
#include "TextNode.h"
#include "SimpleAudioEngine.h"
#include "OcProxy.h"
#include "FbdWords.h"
#include "CountryMain.h"
#include "dialog0.h"
#include "Dialog1.h"

using namespace CocosDenshion;

#define enCCBtn_InPut           1001
#define enCCBtn_OK              1002
#define enCCBtn_Shadow          1003
#define enCCBtn_Cancel          1004

CountryDialog::CountryDialog(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnOSelector = selector;
    m_bShowKeyboard = false;
    m_textField = NULL;
    m_tnCntrySupply = NULL;
    m_bPutInError = false;
    m_fbdwords    = NULL;
    m_mItemOK = NULL;
    m_mItemCancel = NULL;
    m_dlgAlert = NULL;
    m_type = enCDialogType_null;
}

CountryDialog::CountryDialog(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_bShowKeyboard = false;
    m_textField = NULL;
    m_tnCntrySupply = NULL;
    m_bPutInError = false;
    m_fbdwords    = NULL;
    m_mItemOK = NULL;
    m_mItemCancel = NULL;
    m_dlgAlert = NULL;
    m_type = enCDialogType_null;
}

CountryDialog::~CountryDialog()
{

}

void CountryDialog::onEnter()
{
    CCLayer::onEnter();
    
}

void CountryDialog::onExit()
{
    if (m_bShowKeyboard) {
        hidekeyboard();
    }

    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();

    OcProxy::Inst()->removeWebview();

    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

void CountryDialog::showDonateCoinDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    
    m_type  = enCDialogType_DonateCoin;
    
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScaleX(2.2);
    spBg->setScaleY(2.3);
    spBg->setPosition(CCPointMake(320, 560));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_num_of_stone"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 670));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_num_of_stone_detail"),
                                        CCSizeMake(600, 25),
                                        CCTextAlignmentCenter,
                                        25);
    tempText->setPosition(CCPointMake(320, 635));
    tempText->setColor(ccRED);
    addChild(tempText);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    CCMenuItem* mItemInPut = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(CountryDialog::menuCallBack));
    mItemInPut->setTag(enCCBtn_InPut);
    mItemInPut->setAnchorPoint(CCPointZero);
    mItemInPut->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                        530));
    
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                           spMenu1->getContentSize(),
                                                           CCTextAlignmentCenter,
                                                           "huakangfont.ttf",
                                                           26);
    m_textField->setPosition(CCPointMake(320, 550));
    m_textField->setDelegate(this);
    m_textField->setColor(ccBLACK);
    addChild(m_textField, 1);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    m_mItemOK->setTag(enCCBtn_OK);
    m_mItemOK->setAnchorPoint(CCPointZero);
    m_mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     430));
    
    m_mItemOK->setIsEnabled(false);

    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_donate_alone"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tempText->setShadowColor(ccBLACK);
    m_mItemOK->addChild(tempText);
    
    CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
    spShadow->setTag(enCCBtn_Shadow);
    spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    m_mItemOK->addChild(spShadow);
    
    
    CCMenu* menu = CCMenu::menuWithItems(mItemInPut, m_mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    char buf[100];
    lCnt = 0;
    strItem = "1101_item";
    CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
    for (int i = 0 ; i < itemCardInf->count(); i ++) //i < itemCardInf->count()
    {
        if (itemCardInf->getObjectAtIndex(i)->strType=="stone") {
            lCnt = itemCardInf->getObjectAtIndex(i)->lCnt;
            strItem = itemCardInf->getObjectAtIndex(i)->strItemId;
            break;
        }
    }
    snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_num_of_yuanbao_and_stone"),CGameData::Inst()->getUsrInfo()->can_contribute_coin>CGameData::Inst()->getUsrInfo()->coin?CGameData::Inst()->getUsrInfo()->coin:CGameData::Inst()->getUsrInfo()->can_contribute_coin,lCnt);
    tempText = TextNode::textWithString(buf,
                                         25);
    tempText->setPosition(CCPointMake(320, 605));
    tempText->setColor(ccRED);
    addChild(tempText);
    
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    showKeyboard();
}

void CountryDialog::setSameDonateDialog()
{
}
void CountryDialog::showFbdwords()
{
    
    
    if (m_fbdwords == NULL)
    {
        m_fbdwords = new Dialog0(this, callfunc_selector(CountryDialog::cbshowFbdwords), CGameData::Inst()->getLanguageValue("cntry_fbdwords_content1"));
        addChild(m_fbdwords);
        m_fbdwords -> setPosition(CCPointMake(320, 440));
        m_fbdwords -> autorelease();
    }

}

void CountryDialog::cbshowFbdwords()
{
    if (m_fbdwords)
        m_fbdwords->removeFromParentAndCleanup(true);
    m_fbdwords = NULL;
}

void CountryDialog::showSureNameCardDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_type = enCDialogType_SureNameCard;
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_cardName_title"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        45);
    tempText->setPosition(CCPointMake(320, 650));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_cardName_sure"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 590));
    tempText->setColor(ccc3(100, 30, 0));
    addChild(tempText);
    
    CCLabelTTF* lbNameCard = CCLabelTTF::labelWithString(CGameData::Inst()->getNameCard(),
                                                         CCSizeMake(600, 30),
                                                         CCTextAlignmentCenter,
                                                         FONTSTHEITISCM,
                                                         30);
    lbNameCard->setPosition(CCPointMake(320, 560));
    lbNameCard->setColor(ccc3(100, 30, 0));
    addChild(lbNameCard);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemCancel = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                      spMenu2,
                                                      this,
                                                      menu_selector(CountryDialog::menuCallBack));
    m_mItemCancel->setTag(enCCBtn_Cancel);
    m_mItemCancel->setAnchorPoint(CCPointZero);
    m_mItemCancel->setPosition(CCPointMake(440 - spMenu1->getContentSize().width * 0.5,
                                       440));
        
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    m_mItemCancel->addChild(tempText);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                      spMenu2,
                                                      this,
                                                      menu_selector(CountryDialog::menuCallBack));
    m_mItemOK->setTag(enCCBtn_OK);
    m_mItemOK->setAnchorPoint(CCPointZero);
    m_mItemOK->setPosition(CCPointMake(200 - spMenu1->getContentSize().width * 0.5,
                                       440));
        
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    m_mItemOK->addChild(tempText);
        
    CCMenu* menu = CCMenu::menuWithItems(m_mItemCancel, m_mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);    
}

void CountryDialog::showSetNameCardDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_type  = enCDialogType_SetNameCard;
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_cardName_title"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        45);
    tempText->setPosition(CCPointMake(320, 650));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_cardName_rule"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 610));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    
    CCMenuItem* mItemInPut = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(CountryDialog::menuCallBack));
    mItemInPut->setTag(enCCBtn_InPut);
    mItemInPut->setAnchorPoint(CCPointZero);
    mItemInPut->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                        530));
    
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                           CCSizeMake(spMenu1->getContentSize().width - 10,
                                                                      spMenu1->getContentSize().height - 10),
                                                           CCTextAlignmentCenter,
                                                           FONTSTHEITISCM,
                                                           26);
    m_textField->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                         spMenu1->getContentSize().height * 0.5));
    m_textField->setDelegate(this);
    m_textField->setColor(ccBLACK);
    mItemInPut->addChild(m_textField);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                      spMenu2,
                                                      this,
                                                      menu_selector(CountryDialog::menuCallBack));
    m_mItemOK->setTag(enCCBtn_OK);
    m_mItemOK->setAnchorPoint(CCPointZero);
    m_mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                       440));
    
    m_mItemOK->setIsEnabled(false);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    m_mItemOK->addChild(tempText);
    
    CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
    spShadow->setTag(enCCBtn_Shadow);
    spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    m_mItemOK->addChild(spShadow);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemInPut, m_mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNamePhonePad();
    showKeyboard();
}

void CountryDialog::showSetDeclarationDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_type  = enCDialogType_SetDeclaration;
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");

    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 580));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_declaration"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 690));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    spMenu1 = CCSprite::spriteWithFile("bg_declaration.png");
    spMenu2 = CCSprite::spriteWithFile("bg_declaration.png");
    
    CCMenuItem* mItemInPut = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(CountryDialog::menuCallBack));
    mItemInPut->setTag(enCCBtn_InPut);
    mItemInPut->setAnchorPoint(CCPointZero);
    mItemInPut->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                        520));
    
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                           CCSizeMake(spMenu1->getContentSize().width - 10,
                                                                      spMenu1->getContentSize().height - 10),
                                                           CCTextAlignmentCenter,
                                                           FONTSTHEITISCM,
                                                           26);
    m_textField->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                         spMenu1->getContentSize().height * 0.5));
    m_textField->setDelegate(this);
    m_textField->setColor(ccBLACK);
    mItemInPut->addChild(m_textField);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    m_mItemOK->setTag(enCCBtn_OK);
    m_mItemOK->setAnchorPoint(CCPointZero);
    m_mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     440));
    
    m_mItemOK->setIsEnabled(false);
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.53));
    tempText->setShadowColor(ccBLACK);
    m_mItemOK->addChild(tempText);
    
    CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
    spShadow->setTag(enCCBtn_Shadow);
    spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    m_mItemOK->addChild(spShadow);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemInPut, m_mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNamePhonePad();
    showKeyboard();
}

void CountryDialog::showCntrySearchDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    
    TextNode* tempText;
    CCSprite* spMenu1;
    CCSprite* spMenu2;
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_ID"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tempText->setPosition(CCPointMake(320, 640));
    tempText->setColor(ccBLACK);
    addChild(tempText);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    CCMenuItem* mItemInPut = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(CountryDialog::menuCallBack));
    mItemInPut->setTag(enCCBtn_InPut);
    mItemInPut->setAnchorPoint(CCPointZero);
    mItemInPut->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                        560));
    
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                             spMenu1->getContentSize(),
                                                             CCTextAlignmentCenter,
                                                             FONTSTHEITISCM,
                                                             26);
    m_textField->setPosition(CCPointMake(320, 580));
    m_textField->setDelegate(this);
    m_textField->setColor(ccBLACK);
    addChild(m_textField, 1);
        
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     440));
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_search_alone"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tempText->setShadowColor(ccBLACK);
    mItemOK->addChild(tempText);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemInPut, mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNamePhonePad();
    showKeyboard();
}

void CountryDialog::showCntryLevelUpDialog()
{    
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    char buf[100];
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    
    snprintf(buf, 99, "%s：", CGameData::Inst()->getLanguageValue("cntry_country_up_exp"));
    TextNode* tnNeedExpTitle = TextNode::textWithString(buf,
                                                       CCSizeMake(400, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnNeedExpTitle->setColor(ccc3(52, 3, 9));
    tnNeedExpTitle->setPosition(CCPointMake(320, 665));
    addChild(tnNeedExpTitle);
    
    CCSprite* spNeedExpBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spNeedExpBg->setPosition(CCPointMake(320, 620));
    addChild(spNeedExpBg);
    
    snprintf(buf, 99, "%ld", cntryInfo->need_exp);
    TextNode* tnNeedExp = TextNode::textWithString(buf,
                                                  spNeedExpBg->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  30);
    tnNeedExp->setColor(ccBLACK);
    tnNeedExp->setPosition(CCPointMake(spNeedExpBg->getContentSize().width * 0.5,
                                       spNeedExpBg->getContentSize().height * 0.5));
    spNeedExpBg->addChild(tnNeedExp);
        
    snprintf(buf, 99, "%s：", CGameData::Inst()->getLanguageValue("cntry_country_current_exp"));
    TextNode* tnCurExpTitle = TextNode::textWithString(buf,
                                                       CCSizeMake(400, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnCurExpTitle->setColor(ccc3(52, 3, 9));
    tnCurExpTitle->setPosition(CCPointMake(320, 575));
    addChild(tnCurExpTitle);
    
    CCSprite* spCurExpBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spCurExpBg->setPosition(CCPointMake(320, 530));
    addChild(spCurExpBg);
    
    snprintf(buf, 99, "%ld", cntryInfo->cur_exp);
    TextNode* tnCurExp = TextNode::textWithString(buf,
                                                  spNeedExpBg->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  30);
    tnCurExp->setColor(ccBLACK);
    tnCurExp->setPosition(CCPointMake(spNeedExpBg->getContentSize().width * 0.5,
                                      spNeedExpBg->getContentSize().height * 0.5));
    spCurExpBg->addChild(tnCurExp);
    
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     420));
    
    TextNode* tnSearch = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_levelup_alone"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tnSearch->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tnSearch->setShadowColor(ccBLACK);
    mItemOK->addChild(tnSearch);
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    addChild(menu);
    menu->setPosition(CCPointZero);

    if (cntryInfo->need_exp > cntryInfo->cur_exp) {
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                           CCFadeTo::actionWithDuration(0.2, 255),
                                                                           NULL);
        tnNeedExp->setColor(ccc3(100, 30, 0));
        tnNeedExp->setShadowColor(ccBLACK);
        tnNeedExp->runActionInside(CCRepeatForever::actionWithAction(action));
        
        mItemOK->setIsEnabled(false);
        
        CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                          spMenu1->getContentSize().height * 0.5));
        mItemOK->addChild(spShadow);
    }
    else if (cntryInfo->lv >= 10)
    {
        mItemOK->setIsEnabled(false);
        
        CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                          spMenu1->getContentSize().height * 0.5));
        mItemOK->addChild(spShadow);
    }
}

void CountryDialog::showCntryCreateTipDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 480));
    
    TextNode* tempText;
    // title
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_hint"),
                                        CCSizeMake(560, 130),
                                        CCTextAlignmentCenter,
                                        55);
    tempText->setPosition(CCPointMake(320, 660));
    tempText->setColor(ccc3(122, 37, 8));
    addChild(tempText);
    
    // tip content
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_content0"),
                                        CCSizeMake(520, 400),
                                        CCTextAlignmentCenter,
                                        24);
    tempText->setPosition(CCPointMake(320, 480));
    tempText->setColor(ccc3(63, 23, 13));
    addChild(tempText);
    
    // ok button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     260));
    
    tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                        spMenu1->getContentSize(),
                                        CCTextAlignmentCenter,
                                        40);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tempText->setShadowColor(ccBLACK);
    mItemOK->addChild(tempText);
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void CountryDialog::showBuffLevelUpDialog(enBuffTypeDef buffType)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
 
    char buf[100];
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));
    
    snprintf(buf, 99, "%s：", CGameData::Inst()->getLanguageValue("cntry_country_current_exp"));
    TextNode* tnCurExpTitle = TextNode::textWithString(buf,
                                                       CCSizeMake(400, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnCurExpTitle->setColor(ccc3(52, 3, 9));
    tnCurExpTitle->setPosition(CCPointMake(320, 660));
    addChild(tnCurExpTitle);
    
    CCSprite* spSmallBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spSmallBg->setPosition(CCPointMake(320, 610));
    addChild(spSmallBg);
    
    snprintf(buf, 99, "%ld", cntryInfo->cur_exp);
    TextNode* tnCurExp = TextNode::textWithString(buf,
                                                  spSmallBg->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  30);
    tnCurExp->setColor(ccBLACK);
    tnCurExp->setPosition(CCPointMake(spSmallBg->getContentSize().width * 0.5,
                                      spSmallBg->getContentSize().height * 0.5));
    spSmallBg->addChild(tnCurExp);
    
    snprintf(buf, 99, "%s：%d",CGameData::Inst()->getLanguageValue("cntry_levelup_gold"), cntryInfo->buffInfo[buffType].coin);
    TextNode* tnNeedCoin = TextNode::textWithString(buf,
                                                    CCSizeMake(400, 30),
                                                    CCTextAlignmentCenter,
                                                    30);
    tnNeedCoin->setColor(ccBLACK);
    tnNeedCoin->setPosition(CCPointMake(320, 560));
    addChild(tnNeedCoin);
    
    CBuffInfo* nextLvBuff = CGameData::Inst()->getBuffInfo(buffType, cntryInfo->buffInfo[buffType].lv + 1);
    if (buffType == enBuffType_Stamina)
    {
        float effect = nextLvBuff->effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%s%.1f%%%s",CGameData::Inst()->getLanguageValue("cntry_next_time_decrease"), effect, CGameData::Inst()->getLanguageValue("cntry_recover_time"));
        else
            snprintf(buf, 99, "%s%d%%%s", CGameData::Inst()->getLanguageValue("cntry_next_time_decrease"), int(effect), CGameData::Inst()->getLanguageValue("cntry_recover_time"));
    }
    else if (buffType == enBuffType_Attack)
    {
        float effect = nextLvBuff->effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%s%.1f%%%s", CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), effect, CGameData::Inst()->getLanguageValue("cntry_attack_alone"));
        else
            snprintf(buf, 99, "%s%d%%%s", CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), int(effect), CGameData::Inst()->getLanguageValue("cntry_attack_alone"));
    }
    else if (buffType == enBuffType_Recover)
    {
        float effect = nextLvBuff->effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%s%.1f%%%s", CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), effect, CGameData::Inst()->getLanguageValue("cntry_recover_alone"));
        else
            snprintf(buf, 99, "%s%d%%%s", CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), int(effect), CGameData::Inst()->getLanguageValue("cntry_recover_alone"));
    }
    else if (buffType == enBuffType_Hp)
    {
        float effect = nextLvBuff->effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%s%.1f%%%s", CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), effect, CGameData::Inst()->getLanguageValue("cntry_hp_alone"));
        else
            snprintf(buf, 99, "%s%d%%%s",CGameData::Inst()->getLanguageValue("cntry_next_level_promote"), int(effect), CGameData::Inst()->getLanguageValue("cntry_hp_alone"));
    }
    TextNode* tnEffect = TextNode::textWithString(buf,
                                                  CCSizeMake(600, 30),
                                                  CCTextAlignmentCenter,
                                                  30);
    tnEffect->setColor(ccc3(81, 150, 44));
    tnEffect->setPosition(CCPointMake(320, 520));
    addChild(tnEffect);
    
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     420));
    
    TextNode* tnSearch = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_level_up"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tnSearch->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tnSearch->setShadowColor(ccBLACK);
    mItemOK->addChild(tnSearch);
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    if (cntryInfo->cur_exp < cntryInfo->buffInfo[buffType].exp
        || cntryInfo->cntry_coin < cntryInfo->buffInfo[buffType].coin) {
        mItemOK->setIsEnabled(false);
        CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                          spMenu1->getContentSize().height * 0.5));
        mItemOK->addChild(spShadow);
        
        if (cntryInfo->cur_exp < cntryInfo->buffInfo[buffType].exp) {
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                               CCFadeTo::actionWithDuration(0.2, 255),
                                                                               NULL);
            tnCurExp->setColor(ccc3(100, 30, 0));
            tnCurExp->setShadowColor(ccBLACK);
            tnCurExp->runActionInside(CCRepeatForever::actionWithAction(action));
        }
        
        if (cntryInfo->cntry_coin < cntryInfo->buffInfo[buffType].coin) {
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                               CCFadeTo::actionWithDuration(0.2, 255),
                                                                               NULL);
            tnNeedCoin->setColor(ccc3(100, 30, 0));
            tnNeedCoin->setShadowColor(ccBLACK);
            tnNeedCoin->runActionInside(CCRepeatForever::actionWithAction(action));
        }
    }
}

void CountryDialog::showCntryCurrentInfo()
{
    CCSprite* spBg = CCSprite::spriteWithFile("fr_cntryDetails.png");
    spBg->setPosition(CCPointMake(320, 480));
    addChild(spBg);

    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_recent_info"),
                                                 CCSizeMake(500, 50),
                                                 CCTextAlignmentCenter,
                                                 45);
    tnTitle->setColor(ccBLACK);
    tnTitle->setPosition(CCPointMake(320, 690));
    addChild(tnTitle);

  //  OcProxy::Inst()->showCntryCurrentInfoWebView(CGameData::Inst()->getCntryCurrentInfoURL());
    OcProxy::Inst()->showWebWithType(enWebviewLoadTypeHtmlStr,CGameData::Inst()->getCntryCurrentInfoURL(),CCRect(50*2, 156*2, 220*2, 176*2));
    
    
  
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     206));
    
    TextNode* tnSearch = TextNode::textWithString(CGameData::Inst()->getLanguageValue("backTip"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tnSearch->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tnSearch->setShadowColor(ccBLACK);
    mItemOK->addChild(tnSearch);
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void CountryDialog::showCntryRank()
{
    CCSprite* spBg = CCSprite::spriteWithFile("fr_cntryDetails.png");
    spBg->setPosition(CCPointMake(320, 480));
    addChild(spBg);
    
    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_rank_show"),
                                                 CCSizeMake(500, 50),
                                                 CCTextAlignmentCenter,
                                                 45);
    tnTitle->setColor(ccBLACK);
    tnTitle->setPosition(CCPointMake(320, 690));
    addChild(tnTitle);
    
   // OcProxy::Inst()->showCntryCurrentInfoWebView(CGameData::Inst()->getCntryDgRankStr().c_str());
    
    OcProxy::Inst()->showWebWithType(enWebviewLoadTypeHtmlStr,CGameData::Inst()->getCntryDgRankStr().c_str(),CCRect(50*2, 156*2, 220*2, 176*2));
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     206));
    
    TextNode* tnSearch = TextNode::textWithString(CGameData::Inst()->getLanguageValue("backTip"),
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  40);
    tnSearch->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                      spMenu1->getContentSize().height * 0.5));
    tnSearch->setShadowColor(ccBLACK);
    mItemOK->addChild(tnSearch);
    
    CCMenu* menu = CCMenu::menuWithItem(mItemOK);
    addChild(menu);
    menu->setPosition(CCPointZero);
}

void CountryDialog::showSupplyAssignOneDialog(CMember* memberInfo)
{
    m_type = enCDialogType_AssignOne;
    snprintf(m_memberId, 99, "%s", memberInfo->mid);
    
    setSameAssignDialog();
    
    char buf[100];
    
    snprintf(buf, 99, "%s: %s", CGameData::Inst()->getLanguageValue("cntry_deliver_target"), memberInfo->name);
    CCLabelTTF* lbName = CCLabelTTF::labelWithString(buf,
                                                     CCSizeMake(600, 30),
                                                     CCTextAlignmentCenter,
                                                     "STHeitiSC-Medium",
                                                     30);
    lbName->setPosition(CCPointMake(320, 660));
    lbName->setColor(ccc3(103, 43, 10));
    addChild(lbName);

    TextNode* tnSupplyTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_delivercontent1"),
                                        CCSizeMake(600, 30),
                                        CCTextAlignmentCenter,
                                        30);
    tnSupplyTitle->setPosition(CCPointMake(320, 620));
    tnSupplyTitle->setColor(ccBLACK);
    addChild(tnSupplyTitle);
}

void CountryDialog::showJiFengAssignOneDlg(JiFengRecordInf* pInf)
{
    m_type = enCDialogType_JiFengAssignOne;
    snprintf(m_memberId, 99, "%s", pInf->strUid.c_str());

    setSameAssignDialog();
    
    char buf[100];
    
    snprintf(buf, 99, "%s: %s", CGameData::Inst()->getLanguageValue("cntry_deliver_target"), pInf->strName.c_str());
    CCLabelTTF* lbName = CCLabelTTF::labelWithString(buf,
                                                     CCSizeMake(600, 30),
                                                     CCTextAlignmentCenter,
                                                     "STHeitiSC-Medium",
                                                     30);
    lbName->setPosition(CCPointMake(320, 660));
    lbName->setColor(ccc3(103, 43, 10));
    addChild(lbName);
    
    TextNode* tnSupplyTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_assign_coin_lb"),
                                                       CCSizeMake(600, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnSupplyTitle->setPosition(CCPointMake(320, 620));
    tnSupplyTitle->setColor(ccBLACK);
    addChild(tnSupplyTitle);
}

void CountryDialog::showSupplyAssignOutDialog()
{
    m_type = enCDialogType_AssignOut;
    
    setSameAssignDialog();
    
    char buf[100];
    
    snprintf(buf, 99, "%s: %s", CGameData::Inst()->getLanguageValue("cntry_deliver_target"), CGameData::Inst()->getLanguageValue("cntry_allmember"));
    TextNode* tnName = TextNode::textWithString(buf,
                                                CCSizeMake(600, 30),
                                                CCTextAlignmentCenter,
                                                30);
    tnName->setPosition(CCPointMake(320, 660));
    tnName->setColor(ccc3(103, 43, 10));
    addChild(tnName);
    
    TextNode* tnSupplyTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_delivercontent3"),
                                                       CCSizeMake(600, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnSupplyTitle->setPosition(CCPointMake(320, 620));
    tnSupplyTitle->setColor(ccBLACK);
    addChild(tnSupplyTitle);
    
}

void CountryDialog::showSupplyAssignWeekDialog()
{
    m_type = enCDialogType_AssignWeek;
    
    setSameAssignDialog();
    
    char buf[100];
    
    snprintf(buf, 99, "%s: %s", CGameData::Inst()->getLanguageValue("cntry_deliver_target"), CGameData::Inst()->getLanguageValue("cntry_allmember"));
    TextNode* tnName = TextNode::textWithString(buf,
                                                CCSizeMake(600, 30),
                                                CCTextAlignmentCenter,
                                                30);
    tnName->setPosition(CCPointMake(320, 660));
    tnName->setColor(ccc3(103, 43, 10));
    addChild(tnName);
    
    TextNode* tnSupplyTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_delivercontent4"),
                                                       CCSizeMake(600, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnSupplyTitle->setPosition(CCPointMake(320, 620));
    tnSupplyTitle->setColor(ccBLACK);
    addChild(tnSupplyTitle);
    
}

void CountryDialog::showSupplyAssignAllDialog()
{
    m_type = enCDialogType_AssignAll;

    setSameAssignDialog();
    
    char buf[100];
    
    snprintf(buf, 99, "%s: %s", CGameData::Inst()->getLanguageValue("cntry_deliver_target"), CGameData::Inst()->getLanguageValue("cntry_allmember"));
    TextNode* tnName = TextNode::textWithString(buf,
                                                CCSizeMake(600, 30),
                                                CCTextAlignmentCenter,
                                                30);
    tnName->setPosition(CCPointMake(320, 660));
    tnName->setColor(ccc3(103, 43, 10));
    addChild(tnName);
    
    TextNode* tnSupplyTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_delivercontent2"),
                                                       CCSizeMake(600, 30),
                                                       CCTextAlignmentCenter,
                                                       30);
    tnSupplyTitle->setPosition(CCPointMake(320, 620));
    tnSupplyTitle->setColor(ccBLACK);
    addChild(tnSupplyTitle);
    
}

void CountryDialog::setSameAssignDialog()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_window3.png");
    addChild(spBg);
    spBg->setScale(2.0);
    spBg->setPosition(CCPointMake(320, 560));

    CCSprite* spMenu1;
    CCSprite* spMenu2;
    char buf[100];

    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    CCMenuItem* mItemInPut = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(CountryDialog::menuCallBack));
    mItemInPut->setTag(enCCBtn_InPut);
    mItemInPut->setAnchorPoint(CCPointZero);
    mItemInPut->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                        550));
    
    
    m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("",
                                                           spMenu1->getContentSize(),
                                                           CCTextAlignmentCenter,
                                                           "huakangfont.ttf",
                                                           26);
    m_textField->setPosition(CCPointMake(320, 570));
    m_textField->setDelegate(this);
    m_textField->setColor(ccBLACK);
    addChild(m_textField, 1);
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    if(m_type == enCDialogType_JiFengAssignOne)
        snprintf(buf, 99, "%s%ld", CGameData::Inst()->getLanguageValue("cntry_country_coin"), CGameData::Inst()->getGuildCoin());
    else
        snprintf(buf, 99, "%s%ld", CGameData::Inst()->getLanguageValue("cntry_country_money2"), cntryInfo->cntry_supply);
    m_tnCntrySupply = TextNode::textWithString(buf,
                                               CCSizeMake(600, 30),
                                               CCTextAlignmentCenter,
                                               30);
    m_tnCntrySupply->setPosition(CCPointMake(320, 530));
    m_tnCntrySupply->setColor(ccBLACK);
    addChild(m_tnCntrySupply);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* mItemOK = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                spMenu2,
                                                                this,
                                                                menu_selector(CountryDialog::menuCallBack));
    mItemOK->setTag(enCCBtn_OK);
    mItemOK->setAnchorPoint(CCPointZero);
    mItemOK->setPosition(CCPointMake(320 - spMenu1->getContentSize().width * 0.5,
                                     430));
    
    TextNode* tnOk = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                              spMenu1->getContentSize(),
                                              CCTextAlignmentCenter,
                                              40);
    tnOk->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                  spMenu1->getContentSize().height * 0.5));
    tnOk->setShadowColor(ccBLACK);
    mItemOK->addChild(tnOk);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemInPut, mItemOK, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    showKeyboard();
}

void CountryDialog::menuCallBack(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    CCMenuItem* pRet = (CCMenuItem*)pObj;
    switch (pRet->getTag()) {
        case enCCBtn_InPut:
        {
            if (m_bShowKeyboard) {
                hidekeyboard();
            }
            else {
                showKeyboard();
            }
        }
            break;
            
        case enCCBtn_OK:
            if (!m_bPutInError) {
                hidekeyboard();
                if (m_type == enCDialogType_SureNameCard)
                {
                    m_ret.bOk = true;
                    (m_pListener->*m_pfnOSelector)(&m_ret);
                }
                else
                {
                    if (m_type == enCDialogType_SetNameCard)
                    {
                        if (CGameData::Inst()->getUsrInfo()->gold < CGameData::Inst()->getCommonInfo()->modify_name_gold)
                        {
                            showAlertNoGold();
                            return;
                        }
                        CGameData::Inst()->setNameCard(m_textField->getString());
                    }
                    (m_pListener->*m_pfnSelector)();
                }
            }
            break;

        case enCCBtn_Cancel:
            m_ret.bOk = false;
            (m_pListener->*m_pfnOSelector)(&m_ret);
            break;

        default:
            break;
    }
}

void CountryDialog::showAlertNoGold()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(CountryDialog::cbAlertNoGold),
                                 CGameData::Inst()->getLanguageValue("cntry_cardName_alert"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryDialog::cbAlertNoGold()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
}

bool CountryDialog::searchCountry()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        bRet = CGameData::Inst()->searchCountryRequest(m_textField->getString());
    }

    return bRet;
}

bool CountryDialog::setDeclaration()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        if (FbdWords::Ins()->CheckLegal((string)m_textField->getString()))
            return false;
        bRet = CGameData::Inst()->setDeclarationRequest(m_textField->getString());
    }
    
    return bRet;
}

bool CountryDialog::donateCoin()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        int coin = atoi(m_textField->getString());
        if (coin > 0){
            if ((coin > lCnt+CGameData::Inst()->getUsrInfo()->can_contribute_coin)
                ||(coin > lCnt+CGameData::Inst()->getUsrInfo()->coin)) {
                char buf[100];
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_stone_yuanbaobuzu"));
                Dialog2* dlg2 = new Dialog2(this, callfuncO_selector(CountryDialog::Dlg2Callback), CGameData::Inst()->getLanguageValue("cntry_querenuse"), buf,enDl2TwoBtn);
                addChild(dlg2,15,888);
                dlg2->release();
                dlg2->setPosition(ccp(320,500 - getPosition().y));
            }else{
                bRet = CGameData::Inst()->useStoneReq(strItem, coin);
            }
        }
    }

    return bRet;
}

void CountryDialog::Dlg2Callback(CCObject* pSender)
{
    Dialog2Ret* ret = (Dialog2Ret*) pSender;
    
    if (ret->bOk)
    {
        MainScene::Inst()->GoToYuanbaoPage();
    }
    else
    {
        removeChildByTag(888, true);
    }
}

bool CountryDialog::assignOneSupply()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        long supply = atol(m_textField->getString());
        if (supply > 0)
            bRet = CGameData::Inst()->assignOneSupplyRequest(m_memberId, supply);
    }
    
    return bRet;
}

bool CountryDialog::assignOneCoin()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        long lCoin = atol(m_textField->getString());
        if (lCoin > 0)
            bRet = CGameData::Inst()->assignGuildCoin(m_memberId, lCoin);
    }
    
    return bRet;
}

bool CountryDialog::assignAllSupply()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        int memberCount = CGameData::Inst()->getMemberList()->count();
        long supply = atol(m_textField->getString()) * memberCount;
        if (supply > 0)
            bRet = CGameData::Inst()->assignAllSupplyRequest(supply);
    }
    
    return bRet;
}

bool CountryDialog::assignOutSupply()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        long supply = atol(m_textField->getString());
        if (supply > 0)
            bRet = CGameData::Inst()->assignOutSupplyRequest(supply);
    }
    
    return bRet;
}

bool CountryDialog::assignWeekSupply()
{
    bool bRet = false;
    if (strlen(m_textField->getString()) > 0) {
        long supply = atol(m_textField->getString());
        if (supply > 0)
            bRet = CGameData::Inst()->assignWeekSupplyRequest(supply);
    }
    
    return bRet;
}

bool CountryDialog::setNameCard()
{
    bool bRet = false;
    if (FbdWords::Ins()->CheckLegal(CGameData::Inst()->getNameCard()))
    {
        showFbdwords();
        return false;
    }

    bRet = CGameData::Inst()->setNameCardRequest();

    return bRet;
}

void CountryDialog::showKeyboard()
{
    if (m_fbdwords != NULL)
    {
        return;
    }
    m_textField->attachWithIME();
    m_bShowKeyboard = true;
}

void CountryDialog::hidekeyboard()
{
    if (m_textField) {
        m_textField->detachWithIME();
        m_bShowKeyboard = false;
    }
}

#pragma mark -
#pragma mark CCTouchDelegate
bool CountryDialog::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_bShowKeyboard) {
        hidekeyboard();
    }
    
    return true;//false;
}


#pragma mark -
#pragma mark CCTextFieldDelegate
bool CountryDialog::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s newText = %ld",
          text, nLen, sender->getCharCount(), sender->getString());
    switch (m_type) {
            
        case enCDialogType_DonateCoin:
        case enCDialogType_SetDeclaration:
        {
            if (sender == m_textField)
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
        }
            break;
            
        case enCDialogType_AssignOne:
        case enCDialogType_AssignOut:
        case enCDialogType_AssignWeek:
        {
            string newText(sender->getString());
            newText.append(text);
            long supply = atol(newText.c_str());

            if (supply > CGameData::Inst()->getCntryInfo()->cntry_supply)
            {
                if (!m_bPutInError)
                {
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                                       CCFadeTo::actionWithDuration(0.2, 255),
                                                                                       NULL);
                    m_tnCntrySupply->setColor(ccc3(100, 30, 0));
                    m_tnCntrySupply->runActionInside(CCRepeatForever::actionWithAction(action));
                    
                    m_bPutInError = true;
                }
                else
                    return true;
            }
        }
            break;
            
        case enCDialogType_JiFengAssignOne:
            {
                string newText(sender->getString());
                newText.append(text);
                long lCoin = atol(newText.c_str());
                
                if (lCoin > CGameData::Inst()->getGuildCoin())
                {
                    if (!m_bPutInError)
                    {
                        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                                           CCFadeTo::actionWithDuration(0.2, 255),
                                                                                           NULL);
                        m_tnCntrySupply->setColor(ccc3(100, 30, 0));
                        m_tnCntrySupply->runActionInside(CCRepeatForever::actionWithAction(action));
                        
                        m_bPutInError = true;
                    }
                    else
                        return true;
                }
            }
            break;
            
        case enCDialogType_AssignAll:
        {
            string newText(sender->getString());
            newText.append(text);
            long supply = atol(newText.c_str());

            int memberCount = CGameData::Inst()->getMemberList()->count();
            if (supply * memberCount > CGameData::Inst()->getCntryInfo()->cntry_supply)
            {
                if (!m_bPutInError)
                {
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                                       CCFadeTo::actionWithDuration(0.2, 255),
                                                                                       NULL);
                    m_tnCntrySupply->setColor(ccc3(100, 30, 0));
                    m_tnCntrySupply->runActionInside(CCRepeatForever::actionWithAction(action));
                    
                    m_bPutInError = true;
                }
                else
                    return true;
            }
        }
            break;
            
        case enCDialogType_SetNameCard:
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

            int strLength = nLen/3;
            if (strLength == 0)
                strLength = 1;
            if (sender->getCharCount() + strLength > 8)
                return true;
        }
            break;
            
        default:
            break;
    }
    
    return false;
}

bool CountryDialog::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *sender,
                                              const char *delText, int nLen)
{    
//    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s",
//          delText, nLen, sender->getCharCount(), sender->getString());
    
    switch (m_type) {
        case enCDialogType_DonateCoin:
        case enCDialogType_SetDeclaration:
        {
            if (sender == m_textField)
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
        }
            break;
            
        case enCDialogType_AssignOne:
        case enCDialogType_AssignOut:
        case enCDialogType_AssignWeek:
        {
            string newText(sender->getString());
            long supply = atol(newText.c_str()) / 10;
            
            if (supply <= CGameData::Inst()->getCntryInfo()->cntry_supply)
            {
                if (m_bPutInError)
                {
                    m_tnCntrySupply->stopAllActionsInside();
                    m_tnCntrySupply->setColor(ccBLACK);
                    m_tnCntrySupply->runActionInside(CCFadeTo::actionWithDuration(0.1, 255));
                    
                    m_bPutInError = false;
                }
            }
        }
            break;
            
        case enCDialogType_JiFengAssignOne:
            {
                string newText(sender->getString());
                long lCoin = atol(newText.c_str()) / 10;
                
                if (lCoin <= CGameData::Inst()->getGuildCoin())
                {
                    if (m_bPutInError)
                    {
                        m_tnCntrySupply->stopAllActionsInside();
                        m_tnCntrySupply->setColor(ccBLACK);
                        m_tnCntrySupply->runActionInside(CCFadeTo::actionWithDuration(0.1, 255));
                        
                        m_bPutInError = false;
                    }
                }
            }
            break;
            
        case enCDialogType_AssignAll:
        {
            string newText(sender->getString());
            long supply = atol(newText.c_str()) / 10;
            int memberCount = CGameData::Inst()->getMemberList()->count();

            if (supply * memberCount <= CGameData::Inst()->getCntryInfo()->cntry_supply)
            {
                if (m_bPutInError)
                {
                    m_tnCntrySupply->stopAllActionsInside();
                    m_tnCntrySupply->setColor(ccBLACK);
                    
                    m_bPutInError = false;
                }
            }
        }
            break;
        
        case enCDialogType_SetNameCard:
        {
            CCSprite  *  shadowSp =  (CCSprite *)  m_mItemOK->getChildByTag(enCCBtn_Shadow);
            
            const char  *curChars = sender->getString();
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
            break;
            
        default:
            break;
    }

    return false;
}





