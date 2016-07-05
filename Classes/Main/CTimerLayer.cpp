//
//  ShopMain.cpp
//  AgainstWar
//
//  Created by XIII on 12-6-21.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CTimerLayer.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "CommDlg.h"
#include "OcProxy.h"
#include "TDCCVirtualCurrency.h"

enum {
    enTimerReqType_Null=0,
    enTimerReqType_Inf,
    enTimerReqType_Cfg,
    enTimerReqType_Card,
    enTimerReqType_Monster,
    enTimerReqType_Pay
};

//******************************************************************************
// Construct
//******************************************************************************
CTimerLayer::CTimerLayer()
{
    m_commDlg = NULL;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1000, true);

}

//******************************************************************************
// enter
//******************************************************************************
void CTimerLayer::enter()
{
    
}

void CTimerLayer::exit()
{
    
}
void CTimerLayer::onExit()
{
    CCLayer::onExit();
}

bool CTimerLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(CGameData::Inst()->isNewbie())
        return false;
    
    if(CGameData::Inst()->getPurchaseStatus() == 10){
        if ((CGameData::Inst()->isValidTransId())) {
            if(CGameData::Inst()->sendPaySuccessInfo()){
                m_iReqType = enTimerReqType_Pay;
                scheduleUpdate();
                return true;
            }
        }
        
        CGameData::Inst()->resetPurchaseStatus();
    }
    
    if(MainScene::Inst()->getBottomPanelIsEnableOrNot()){
        switch (CGameData::Inst()->timeCompare()) {
            case enTimeNoChange:
                return false;
                break;
                
            case enDayChanged:
                schedule(schedule_selector(CTimerLayer::setTouchEnable), 30);
                setIsTouchEnabled(false);
                MainScene::Inst()->reLogin();
                return true;
                break;
                
            case enHourChanged:
                schedule(schedule_selector(CTimerLayer::setTouchEnable), 30);
                setIsTouchEnabled(false);

                if(CGameData::Inst()->reqInfoAsync()){
                    scheduleUpdate();
                    m_iReqType = enTimerReqType_Inf;
                }
                
                return true;
                break;
                
            default:
                break;
        }

        return true;
    }
    else
    {
        return false;
    }
    
}

void  CTimerLayer::setTouchEnable()
{
    unschedule(schedule_selector(CTimerLayer::setTouchEnable));
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1000, true);
    
}


void CTimerLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(CTimerLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 2000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    unscheduleUpdate();

    if(comState == kReqStatOk){
        
        switch (m_iReqType) {
            case enTimerReqType_Inf:
                if(CGameData::Inst()->needToUpdateCfg()) {
                    if(CGameData::Inst()->cfgRequest()){
                        m_iReqType = enTimerReqType_Cfg;
                        scheduleUpdate();
                    }
                }
                else if(CGameData::Inst()->needToUpdateCard()) {
                    if(CGameData::Inst()->reqCard()){
                        m_iReqType = enTimerReqType_Card;
                        scheduleUpdate();
                    }
                }
                else if(CGameData::Inst()->needToUpdateMonster()) {
                    if(CGameData::Inst()->reqMonster()){
                        m_iReqType = enTimerReqType_Monster;
                        scheduleUpdate();
                    }
                }
                else{
                    MainScene::Inst()->enableBottomPanel(true);
                }
                break;
                
            case enTimerReqType_Cfg:
                if(CGameData::Inst()->needToUpdateCard()) {
                    if(CGameData::Inst()->reqCard()){
                        m_iReqType = enTimerReqType_Card;
                        scheduleUpdate();
                    }
                }
                else if(CGameData::Inst()->needToUpdateMonster()) {
                    if(CGameData::Inst()->reqMonster()){
                        m_iReqType = enTimerReqType_Monster;
                        scheduleUpdate();
                    }
                }
                else{
                    m_iReqType = enTimerReqType_Null;
                    
                    //注册推送
                    MainScene::Inst()->enableBottomPanel(true);
                    MainScene::Inst()->refreshArena();
                    //注册推送
                    CCMutableArray<PushInfo *> *pushInfoArr= CGameData::Inst()->getPushInfo();
                    if(pushInfoArr->count()!=0){
                        PushInfo *pushInfo1 = pushInfoArr->getObjectAtIndex(0);
                        //  CCLog("pushtime %ld",pushInfo1->pushTime);
                        OcProxy::Inst()->setPushInfo(pushInfo1->msg, pushInfo1->pushTime, kPushInfDg);
                    }
                    
                    CGameData::Inst()->setUpdateCom(true);
                }
                break;
                
            case enTimerReqType_Card:
                if(CGameData::Inst()->needToUpdateMonster()) {
                    if(CGameData::Inst()->reqMonster()){
                        m_iReqType = enTimerReqType_Monster;
                        scheduleUpdate();
                    }
                }
                else{
                    m_iReqType = enTimerReqType_Null;
                    
                    //注册推送
                    MainScene::Inst()->enableBottomPanel(true);
                    MainScene::Inst()->refreshArena();
                    //注册推送
                    CCMutableArray<PushInfo *> *pushInfoArr= CGameData::Inst()->getPushInfo();
                    if(pushInfoArr->count()!=0){
                        PushInfo *pushInfo1 = pushInfoArr->getObjectAtIndex(0);
                        //  CCLog("pushtime %ld",pushInfo1->pushTime);
                        OcProxy::Inst()->setPushInfo(pushInfo1->msg, pushInfo1->pushTime, kPushInfDg);
                    }
                    
                    CGameData::Inst()->setUpdateCom(true);
                }
                break;
                
            case enTimerReqType_Monster:
                {
                    m_iReqType = enTimerReqType_Null;
                    
                    //注册推送
                    MainScene::Inst()->enableBottomPanel(true);
                    MainScene::Inst()->refreshArena();
                    //注册推送
                    CCMutableArray<PushInfo *> *pushInfoArr= CGameData::Inst()->getPushInfo();
                    if(pushInfoArr->count()!=0){
                        PushInfo *pushInfo1 = pushInfoArr->getObjectAtIndex(0);
                        //  CCLog("pushtime %ld",pushInfo1->pushTime);
                        OcProxy::Inst()->setPushInfo(pushInfo1->msg, pushInfo1->pushTime, kPushInfDg);
                    }
                    
                    CGameData::Inst()->setUpdateCom(true);
                }
                break;
                
            case enTimerReqType_Pay:
                
                
                //talkingdata
#ifdef TalkData
                TDCCVirtualCurrency::onChargeSuccess(OcProxy::Inst()->getReceipt());
#else
                
#endif
                
                
                
//                CCLog("%s" , OcProxy::Inst()->getReceipt());
                
                m_iReqType = enTimerReqType_Null;
                CGameData::Inst()->finishPayment();
                MainScene::Inst()->enableBottomPanel(true);
                CGameData::Inst()->resetPurchaseStatus();
                

                break;
                
            default:
                break;
        }
        
    }
    else {
        //付费失败
        if((m_iReqType == enTimerReqType_Pay) && (comState == 1)){
            CGameData::Inst()->clearReqStat();
            CGameData::Inst()->finishPayment();
            CGameData::Inst()->resetPurchaseStatus();
            MainScene::Inst()->enableBottomPanel(true);
        }
        else{
            if(m_commDlg == NULL){
                m_commDlg = new CommDlg(this, callfuncO_selector(CTimerLayer::cbCommDlg), enCommDlgTp_retry);
                addChild(m_commDlg, 2000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
                m_commDlg->setScale(0.0);
                m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                
                m_commDlg->setNoCancel();
            }
        }
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void CTimerLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        switch (m_iReqType) {
            case enTimerReqType_Inf:
                if(CGameData::Inst()->reqInfoAsync()){
                    scheduleUpdate();
                }
                break;
                
            case enTimerReqType_Cfg:
                if(CGameData::Inst()->cfgRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enTimerReqType_Pay:
                if(CGameData::Inst()->sendPaySuccessInfo()){
                    scheduleUpdate();
                }
                break;
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        MainScene::Inst()->enableBottomPanel(true);
    }
}






