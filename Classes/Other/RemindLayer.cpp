//
//  RemindLayer.cpp
//  AgainstWar
//
//  Created by hejian on 13-10-15.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RemindLayer.h"
#include "CGamedata.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "CommonDef.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "CommDlg.h"

#define kCntryCdOnTag       41
#define kCntryCdOffTag      42
#define kActivityOnTag      43
#define kActivityOffTag     44
#define kTwoupOnTag         45
#define kTwoupOffTag        46
#define kPowerOnTag         47
#define kPowerOffTag        48
#define kCntryCdTag         49
#define kActivityTag        50
#define kTwoupTag           51
#define kPowerTag           52

/////////////////////////////////////////////////////////
using namespace CocosDenshion;

RemindLayer::RemindLayer(CCObject* target, SEL_CallFuncO selector)
{
    //callBack
    m_pListener = target;
    m_pfnSelector = selector;
    
    moveUp = true;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -100, true);
    
    m_remindList = new CListLayer(this, callfuncO_selector(RemindLayer::cbList), 3, 182, 630);
    addChild(m_remindList, -1);
    m_remindList->release();
    m_remindList->setReloadFunc(callfuncO_selector(RemindLayer::insertItemByIdx));
    
    showRemindList();
}

void RemindLayer::cbList(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    printf("button : %d", pRet->iBtnSel);
}

void RemindLayer::showRemindList(){
    CCLayer* layer = createRLayer(CGameData::Inst()->getLanguageValue("activity_remind"), EN_REMIND_ACTIVITY);
    m_remindList->push_back(layer);
    if (layer) {
        layer->release();
    }
    layer = createRLayer(CGameData::Inst()->getLanguageValue("up_remind"), EN_REMIND_TWOUP);
    m_remindList->push_back(layer);
    if (layer) {
        layer->release();
    }
    layer = createRLayer(CGameData::Inst()->getLanguageValue("cd_remind"), EN_REMIND_CNTRYCD);
    m_remindList->push_back(layer);
    if (layer) {
        layer->release();
    }
    layer = createRLayer(CGameData::Inst()->getLanguageValue("power_remind"), EN_REMIND_POWER);
    m_remindList->push_back(layer);
    if (layer) {
        layer->release();
    }
}

bool RemindLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    
    if (!moveUp) {
        powerNumber->detachWithIME();
        layerMoveUpOrDown(moveUp);
        moveUp = !moveUp;
        return true;
    }
    
    return false;
}

void RemindLayer::insertItemByIdx(cocos2d::CCObject *pObj){

}

CCLayer* RemindLayer::createRLayer(const char *tStr, int remindType){
    CCLayer* layer = new CCLayer();
    int tagOpen = 0;
    int tagClose = 0;
    bool typeOn;
    switch (remindType) {
        case EN_REMIND_ACTIVITY:
            tagOpen = kActivityOnTag;
            tagClose = kActivityOffTag;
            typeOn = CGameData::Inst()->isActivityRemind;
            break;
        case EN_REMIND_TWOUP:
            tagOpen = kTwoupOnTag;
            tagClose = kTwoupOffTag;
            typeOn = CGameData::Inst()->isTwoUpRemind;
            break;
        case EN_REMIND_CNTRYCD:
            tagOpen = kCntryCdOnTag;
            tagClose = kCntryCdOffTag;
            typeOn = CGameData::Inst()->isCntryCdRemind;
            break;
        case EN_REMIND_POWER:{
            tagOpen = kPowerOnTag;
            tagClose = kPowerOffTag;
            typeOn = CGameData::Inst()->intPowerRemind;

            CCSprite* spMenu1 = CCSprite::spriteWithFile("pushConfig_tilizhi.png");
            CCSprite* spMenu2 = CCSprite::spriteWithFile("pushConfig_tilizhi.png");
            CCMenuItem* item = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,
                                                                      this, menu_selector(RemindLayer::MoveUpKeyboard));
            item->setPosition(ccp(0,0));
            item->setAnchorPoint(CCPointZero);
            CCMenu *menu = CCMenu::menuWithItems (item, NULL);
            menu->setPosition(ccp(-50,20));
            layer->addChild(menu, 2);
            
            char buf[100];
            snprintf(buf, 99,"%d", CGameData::Inst()->intPowerRemind);
            powerNumber = CCTextFieldTTF::textFieldWithPlaceHolder(buf,CCSizeMake(100, 200) ,CCTextAlignmentCenter,"huakangfont.ttf", 32);
            powerNumber->setPosition(ccp(12, 40));
            powerNumber->setColor(ccWHITE);
            powerNumber->setDelegate(this);
            layer->addChild(powerNumber,10);
            CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
            pGlView->setIMEKeyboardNumber();
            break;
        }
        default:
            tagOpen = kActivityOnTag;
            tagClose = kActivityOffTag;
            typeOn = CGameData::Inst()->isCntryCdRemind;
            break;
    }
    CCSprite* spFrame = CCSprite::spriteWithFile("pushConfig_Bg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    layer->addChild(spFrame);
    TextNode *remindLabel = TextNode::textWithString(tStr, 32);
    remindLabel->setColor(ccc3(255, 255, 255));
    remindLabel->setShadowColor(ccc3(0, 0, 0));
    remindLabel->setPosition(ccp(0,40));
    layer->addChild(remindLabel);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_open.png");
    CCSprite* spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_open2.png");
    CCMenuItem* item1 = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                          this, menu_selector(RemindLayer::switchOnOff));
    item1->setPosition(ccp(-170, -50));
    item1->setAnchorPoint(ccp(0, 0));
    item1->setTag(tagOpen);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_close.png");
    spMenu3 = CCSprite::spriteWithSpriteFrameName("fr_close2.png");
    CCMenuItem* item2 = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2,spMenu3,
                                                           this, menu_selector(RemindLayer::switchOnOff));
    item2->setPosition(ccp(30, -50));
    item2->setAnchorPoint(ccp(0, 0));
    item2->setTag(tagClose);
    
    item1->setIsEnabled(!typeOn);
    item2->setIsEnabled(typeOn);
    
    CCMenu *menu = CCMenu::menuWithItems (item1,item2,
                                          NULL);
    menu->setPosition(ccp(0, 0));
    layer->addChild(menu);
    return layer;
}


void RemindLayer::refreshRemind()
{
    /*CCPoint point;
    musicOpenItem->setIsEnabled(!CGameData::Inst()->isMusicOn);
    musicCloseItem->setIsEnabled(CGameData::Inst()->isMusicOn);
    
    soundOpenItem->setIsEnabled(!CGameData::Inst()->isSoundOn);
    soundCloseItem->setIsEnabled(CGameData::Inst()->isSoundOn);
    
    skillOpenItem->setIsEnabled(!CGameData::Inst()->isSkillOn);
    skillClostItem->setIsEnabled(CGameData::Inst()->isSkillOn);
    */
    
}

void RemindLayer::update(ccTime ct)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if (comState == kReqHoldOn) {
        return;
    }
    if (comState == kReqCancel) {
        
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
            MainScene::Inst()->enableBottomPanel(true);
        }
        unscheduleUpdate();
        MainScene::Inst()->enableBottomPanel(true);
        CGameData::Inst()->clearReqStat();
        return;
    }

    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RemindLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
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
        // 刷新 界面状态，处理可能由于数值设置不容许的情况
        m_remindList->clearAll();
        showRemindList();
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RemindLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void RemindLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk){
        // 重新请求
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else {
        // 当最后请求失败时，恢复为请求之前的状态
        m_remindList->clearAll();
        if (changeNumber == 0) {
            CGameData::Inst()->setActivityRemindValue(1 - changeSum);
        } else if (changeNumber == 1) {
            CGameData::Inst()->setTwoupRemindValue(1 - changeSum);
        } else if (changeNumber == 2) {
            CGameData::Inst()->setCntryCdRemindValue(1 - changeSum);
        } else if (changeNumber == 3) {
            CGameData::Inst()->setPowerRemindValue(1 - changeSum);
        }
        showRemindList();
        
        MainScene::Inst()->showBottomPanel(true);
        CGameData::Inst()->clearReqStat();
        return;
    }
}


void RemindLayer::switchOnOff(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItemImage *temItem = (CCMenuItemImage *)pSender;
    
    if(temItem->getTag() == kActivityOnTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kActivityOnTag))->setIsEnabled(false);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kActivityOffTag))->setIsEnabled(true);
        changeNumber = 0;
        changeSum = 1;
        CGameData::Inst()->setActivityRemindValue(true);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else if (temItem->getTag() == kActivityOffTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kActivityOnTag))->setIsEnabled(true);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kActivityOffTag))->setIsEnabled(false);
        changeNumber = 0;
        changeSum = 0;
        CGameData::Inst()->setActivityRemindValue(false);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else if(temItem->getTag() == kTwoupOnTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kTwoupOnTag))->setIsEnabled(false);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kTwoupOffTag))->setIsEnabled(true);
        changeNumber = 1;
        changeSum = 1;
        CGameData::Inst()->setTwoupRemindValue(true);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else if (temItem->getTag() == kTwoupOffTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kTwoupOnTag))->setIsEnabled(true);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kTwoupOffTag))->setIsEnabled(false);
        changeNumber = 1;
        changeSum = 0;
        CGameData::Inst()->setTwoupRemindValue(false);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else if(temItem->getTag() == kCntryCdOnTag){
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kCntryCdOnTag))->setIsEnabled(false);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kCntryCdOffTag))->setIsEnabled(true);
        changeNumber = 2;
        changeSum = 1;
        CGameData::Inst()->setCntryCdRemindValue(true);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
        
    }else if(temItem->getTag() == kCntryCdOffTag){
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kCntryCdOnTag))->setIsEnabled(true);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kCntryCdOffTag))->setIsEnabled(false);
        changeNumber = 2;
        changeSum = 0;
        CGameData::Inst()->setCntryCdRemindValue(false);
        CGameData::Inst()->RemindReq();
        scheduleUpdate();
    }
    else if(temItem->getTag() == kPowerOnTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kPowerOnTag))->setIsEnabled(false);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kPowerOffTag))->setIsEnabled(true);
        CGameData::Inst()->setPowerRemindValue(atoi(powerNumber->getString()));
        
        // 推送 注册
        if (CGameData::Inst()->getUsrInfo()->stamina < CGameData::Inst()->intPowerRemind && CGameData::Inst()->intPowerRemind < CGameData::Inst()->getUsrInfo()->max_stamina) {
            changeNumber = 3;
            changeSum = 1;
            
            char buf[100];
            snprintf(buf, 99,CGameData::Inst()->getLanguageValue("pushinf_stamina2"), CGameData::Inst()->intPowerRemind);
            long stamina_time;
            unsigned short staminaRecoverTime;
            staminaRecoverTime = CGameData::Inst()->getCommonInfo()->stamina_recover_time * 60;
            if (staminaRecoverTime == 0) {
                
            }else{
                stamina_time = CGameData::Inst()->getUsrInfo()->stamina_upd_time + (CGameData::Inst()->intPowerRemind - CGameData::Inst()->getUsrInfo()->stamina -1) * staminaRecoverTime;
                OcProxy::Inst()->setPushInfo(buf,
                                             stamina_time,
                                             kPushInfStamina2);
            }
            
        }
        if (!moveUp) {
            powerNumber->detachWithIME();
            if (CGameData::Inst()->intPowerRemind == 0 && atoi(powerNumber->getString()) == 0) {
                CGameData::Inst()->RemindReq();
                scheduleUpdate();
            }
            layerMoveUpOrDown(moveUp);
            moveUp = !moveUp;
        } else {
            CGameData::Inst()->RemindReq();
            scheduleUpdate();
        }
    }
    else if (temItem->getTag() == kPowerOffTag)
    {
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kPowerOnTag))->setIsEnabled(true);
        ((CCMenuItemImage*)temItem->getParent()->getChildByTag(kPowerOffTag))->setIsEnabled(false);
        changeNumber = 3;
        changeSum = 0;
        CGameData::Inst()->setPowerRemindValue(0);
        if (!moveUp) {
            powerNumber->detachWithIME();
            layerMoveUpOrDown(moveUp);
            moveUp = !moveUp;
        } else {
            CGameData::Inst()->RemindReq();
            scheduleUpdate();
        }
    }
}

void RemindLayer::MoveUpKeyboard()
{
    if (moveUp) {
        powerNumber->attachWithIME();
        layerMoveUpOrDown(moveUp);
        moveUp = !moveUp;
    }
}

bool RemindLayer::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());

    if (sender->getCharCount()==0 && *text == 48) {
        return true;
    }
    
    if (sender->getCharCount() + nLen > 9) {
        return true;
    }
    
    if (*text <= 57 && *text >= 48) {
        
        char buf[100];
        snprintf(buf, 99,"%s%s", sender->getString(),text);
        int  num = atoi(buf);
        if (num > CGameData::Inst()->getUsrInfo()->max_stamina) {
            num = CGameData::Inst()->getUsrInfo()->max_stamina;
            snprintf(buf, 99,"%d", num);
            sender->setString(buf);
            return true;
        } else {
            return false;
        }
    }
    else {
        return true;
    }
}

bool RemindLayer::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}

void RemindLayer::layerMoveUpOrDown(bool up)
{
    CCAction* act;
    
    if (up)
        act = CCMoveTo::actionWithDuration(0.2, CCPointMake(getPosition().x, 200 + (OcProxy::Inst()->is568HighScreen()?0:50)));
    else {
        if (CGameData::Inst()->intPowerRemind && atoi(powerNumber->getString())) {
            CGameData::Inst()->setPowerRemindValue(atoi(powerNumber->getString()));
        }
        // 推送 注册
        if (CGameData::Inst()->getUsrInfo()->stamina < CGameData::Inst()->intPowerRemind && CGameData::Inst()->intPowerRemind < CGameData::Inst()->getUsrInfo()->max_stamina) {
            char buf[100];
            snprintf(buf, 99,CGameData::Inst()->getLanguageValue("pushinf_stamina2"), CGameData::Inst()->intPowerRemind);
            long stamina_time;
            unsigned short staminaRecoverTime;
            staminaRecoverTime = CGameData::Inst()->getCommonInfo()->stamina_recover_time * 60;
            if (staminaRecoverTime == 0) {
                
            }else{
                stamina_time = CGameData::Inst()->getUsrInfo()->stamina_upd_time + (CGameData::Inst()->intPowerRemind - CGameData::Inst()->getUsrInfo()->stamina -1) * staminaRecoverTime;
                OcProxy::Inst()->setPushInfo(buf,
                                             stamina_time,
                                             kPushInfStamina2);
            }
            
        }
        if (atoi(powerNumber->getString()) && CGameData::Inst()->intPowerRemind) {
            CGameData::Inst()->RemindReq();
            scheduleUpdate();
        }
        //
        act = CCMoveTo::actionWithDuration(0.2, ccp(getPosition().x,0));
    }
    runAction(act);
}

RemindLayer::~RemindLayer(){
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
}

