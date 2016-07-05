//
//  CommDlg.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CommDlg.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "HttpComm.h"
#include "MainScene.h"
#include "OrgSaveData.h"

#define kCommBtnTagOk       0xbb
#define kCommBtnTagCancel   0xcc
#define kCommTextTagCancel  0xaa
#define kCommTextTagOk      0xdd

//******************************************************************************
// Constructor
//******************************************************************************
CommDlg::CommDlg(CCObject* target, SEL_CallFuncO selector, CommDlgType type)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
    
    m_iRc = CGameData::Inst()->getReqStat();
    
    switch (type) {
        case enCommDlgTp_connecting:
            initConnecting();
            break;
            
        case enCommDlgTp_retry:
            initRetry();
            break;
            
        default:
            break;
    }

}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CommDlg::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* nodeOk = getChildByTag(kCommBtnTagOk);
    CCNode* nodeCancel = getChildByTag(kCommBtnTagCancel);
    
    CCPoint pt;
    CCSize sz;
    bool bRet = false;
    
    if(nodeOk){
        pt = nodeOk->getPosition();
        sz = nodeOk->getContentSize();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                           pt.y - sz.height * 0.5, 
                                                           sz.width, 
                                                           sz.height), 
                                                pos);
        
        if(bRet){
            m_ret.bOk = true;
            const char* errMsg = CGameData::Inst()->getErrMsg();
            if(errMsg != NULL){
                string eMsg = errMsg;
                if (eMsg.find("武将不存在")!=string::npos)
                    MainScene::Inst()->reLogin();
                else
                    (m_pListener->*m_pfnSelector)(&m_ret);
            }else
                (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    

    if((!bRet) && (nodeCancel)){
        pt = nodeCancel->getPosition();
        sz = nodeCancel->getContentSize();
        
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                      pt.y - sz.height * 0.5, 
                                                      sz.width, 
                                                      sz.height), 
                                           pos);
        
        if(bRet){
            if (m_iRc == kReloginRc) {
                MainScene::Inst()->forceExitFight();
                SaveData::Inst()->destorySavedData();
                SaveData::Inst()->destorySavedFile();
                OrgSaveData::Inst()->destorySavedData();
                OrgSaveData::Inst()->destorySavedFile();
                MainScene::Inst()->reLogin();
                
                if (m_pParent) {
                    removeFromParentAndCleanup(true);
                }
            }
            else{
                m_ret.bOk = false;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }
        }
    }

    return true;
}

//******************************************************************************
// initConnecting
//******************************************************************************
void CommDlg::initConnecting()
{
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("commdlg.png");
    if(sp){
        addChild(sp);
    }
    
    char buf[100];
    
    CCSprite *temSp = CCSprite::spriteWithSpriteFrameName("loading0001.png");
    temSp->setPosition(CCPointMake(-120 - 15, 30));
    addChild(temSp);
    
    CCAnimation *loadingAnim = CCAnimation::animation();
    char loadingAnimName[100] = {0};
    for (int i = 1; i <= 8 ; i++) {
        sprintf(loadingAnimName, "loading00%02d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( loadingAnimName );
        loadingAnim->addFrame(frame);
    }
    loadingAnim->setDelay(1.0f / 8);
    CCAnimate* loadingAction = CCAnimate::actionWithAnimation(loadingAnim, true);
    temSp->runAction(CCRepeatForever::actionWithAction(loadingAction));
    
    snprintf(buf, 99, "%s",OcProxy::Inst()->localizedStringStatic("loading_title"));
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(200, 45), CCTextAlignmentCenter,45);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    lbText->setPosition(CCPointMake(60 - 45, 20));
    addChild(lbText, 2);
    
    CCLabelTTF* ttfText = CCLabelTTF::labelWithString(OcProxy::Inst()->localizedStringStatic("loading_prompt"), CCSizeMake(360, 60), CCTextAlignmentCenter, "default.ttf", 20);
    ttfText->setColor(ccc3(233, 183, 72));
    ttfText->setPosition(CCPointMake(0, -80));
    addChild(ttfText);
}

//******************************************************************************
// initRetry
//******************************************************************************
void CommDlg::initRetry()
{
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("commdlg.png");
    if(sp)
        addChild(sp);
    char buf[100];
    initBtn();
     
    if(m_iRc == kReqConnectErr){
        //title
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("net_error"));
        TextNode* lbText = TextNode::textWithString(buf,45);
        lbText->setPosition(CCPointMake(0, 86));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        lbText = TextNode::textWithString(HttpComm::Inst()->getLocalErrStr().c_str(), CCSizeMake(400, 60), CCTextAlignmentCenter,26);
        lbText->setPosition(CCPointMake(0, 30));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("net_again"));
        lbText = TextNode::textWithString(buf,26);
        lbText->setPosition(CCPointMake(0, -16));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
    else{
        //服务器来的错误提示
        const char* errMsg = CGameData::Inst()->getErrMsg();
        
        if(errMsg != NULL){
            TextNode* lbText = TextNode::textWithString(errMsg, CCSizeMake(400, 120), CCTextAlignmentCenter,26);
            lbText->setAnchorPoint(CCPointMake(0.5, 1));
            lbText->setPosition(CCPointMake(0, 100));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }
}

//******************************************************************************
// initBtn
//******************************************************************************
void CommDlg::initBtn()
{
    char buf[100];
    //如果还未登录，只显示一个按钮，即要求一定要重试, //付费失败必须重试
    if( ((m_iRc == kReqConnectErr) || (m_iRc == 100) || (m_iRc == 401)) 
        && ((CGameData::Inst()->chkLogin() == false) || (CGameData::Inst()->isPaying() ))){
        //ok
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0, -78));
            addChild(sp, 0, kCommBtnTagOk);
        }
            
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-4, -74));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        lbText->setTag(kCommTextTagOk);
    }
    else if((m_iRc == kReqConnectErr) || (m_iRc == 100) || (m_iRc == 401)){
        //ok
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(-104, -78));
            addChild(sp, 0, kCommBtnTagOk);
        }
        
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-108, -74));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        lbText->setTag(kCommTextTagOk);
        
        //cancel
        sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(104, -78));
            addChild(sp, 0, kCommBtnTagCancel);
        }
        
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cancelTip"));
        lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
        lbText->setPosition(CCPointMake(100, -74));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        lbText->setTag(kCommTextTagCancel);
    }
    else if((m_iRc == 502) || (HttpComm::Inst()->getMaintenance() != 0)){
        //ok
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0, -78));
            addChild(sp, 0, kCommBtnTagOk);
        }
        
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-4, -74));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        lbText->setTag(kCommTextTagOk);
    }
    //服务器返回的提示，无需重试，仅显示取消按钮(文字显示为确定)
    else{
        const char* errMsg = CGameData::Inst()->getErrMsg();
        if(errMsg != NULL){
            string eMsg = errMsg;
            if (eMsg.find("武将不存在")!=string::npos) {
                //ok
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                if(sp){
                    sp->setPosition(CCPointMake(-104, -78));
                    addChild(sp, 0, kCommBtnTagOk);
                }
                
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("re_login"));
                TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
                lbText->setPosition(CCPointMake(-108, -74));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText, 1);
                lbText->setTag(kCommTextTagOk);
                
                //cancel
                sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                if(sp){
                    sp->setPosition(CCPointMake(104, -78));
                    addChild(sp, 0, kCommBtnTagCancel);
                }
                
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cancelTip"));
                lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
                lbText->setPosition(CCPointMake(100, -74));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText, 1);
                lbText->setTag(kCommTextTagCancel);
                return;
            }
        }
        
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0, -78));
            addChild(sp, 0, kCommBtnTagCancel);
        }
        
        snprintf(buf, 99, "%s", OcProxy::Inst()->localizedStringStatic("sureTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 36), CCTextAlignmentCenter,kBtnTitleHeight);
        lbText->setPosition(CCPointMake(-4, -74));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
}

void CommDlg::setNoCancel()
{
    CCNode *cancelBtn = getChildByTag(kCommBtnTagCancel);
    CCNode *cancelTxt = getChildByTag(kCommTextTagCancel);
    CCNode *okBtn = getChildByTag(kCommBtnTagOk);
    CCNode *okTxt = getChildByTag(kCommTextTagOk);
    
    //没有Ok按钮，就不要删cancel按钮了，否则没按钮了
    if (okBtn == NULL) {
        return;
    }
    
    if(cancelBtn)
    {
        cancelBtn->setIsVisible(false);
        cancelBtn->removeFromParentAndCleanup(true);
        cancelBtn=NULL;
    }
    
    if(cancelTxt)
        cancelTxt->setIsVisible(false);
    if(okBtn)
        okBtn->setPosition(CCPointMake(0, -78));
    if(okTxt)
        okTxt->setPosition(CCPointMake(-4, -74));
    
}
//******************************************************************************
// onExit
//******************************************************************************
void CommDlg::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}