//
//  RewardMain.cpp
//  AgainstWar
//
//  Created by user on 13-8-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
////////////////////////////////////////////////////////////
#include "RewardMain.h"
#include "ScrollHorizontal.h"
#include "RwdSighLayer.h"
#include "RwdLevelUpLayer.h"
#include "RwdMonthLayer.h"
#include "RwdBonusLayer.h"
#include "RwdHuodongLayer.h"
#include "RwdBuchangLayer.h"
#include "OcProxy.h"

#include "Arrow.h"
////////////////////////////////////////////////////////////
#define TAGDELTARWDBTN 998
#define SCORLLTAG      99
#define NUMTAG         98

#define kNewbie           82
////////////////////////////////////////////////////////////
RewardMain::RewardMain()
{
    defaultLabel = 0;
}


RewardMain::~RewardMain()
{
    
}


void RewardMain::enter()
{
    m_RightArrow = NULL;
    m_LeftArrow = NULL;
    
    m_rewardSignLayer = NULL;
    m_rewardLevelUpLayer = NULL;
    m_rewardMonthLayer = NULL;
    m_rewardLeiJiLayer = NULL;
    m_rewardHuodongLayer = NULL;
    m_rewardBuchangLayer = NULL;
    
    signNum = NULL;
    
    createAllBtn();
    showLabel(defaultLabel);
    
    
    
    
    
    
    
    
    if(CGameData::Inst()->getUsrInfo()->add_award == 0)
    {
        //引导
//        MainScene::Inst()->showTouchTip();
        MainScene::Inst()->enableBottomPanel(false);
        
        MainScene::Inst()->new_MyNew = true;
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(RewardMain::newbie),
                                            CGameData::Inst()->getLanguageValue("lingJiang_new3"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 245));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, kNewbie + 99999);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));

    }
    else if(MainScene::Inst()->getChildByTag(98997))
    {
        MainScene::Inst()->removeChildByTag(98997, true);
        
        
        
        //引导
//        MainScene::Inst()->showTouchTip();
        MainScene::Inst()->enableBottomPanel(false);
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(RewardMain::newbie2),
                                            CGameData::Inst()->getLanguageValue("lingJiang_new6"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 245));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, 98996);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    
    
}



//******************************************************************************
// cbCommDlg
//******************************************************************************
void RewardMain::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        
        switch (m_iReqType)
        {
            case enRawReqType_add_award:
                if(CGameData::Inst()->newChangeRequest("add_award"))
                {
                    m_iReqType = enRawReqType_add_award;
                    scheduleUpdate();
                }
                break;
                
            case enRawReqType_add_lv_5:
                if(CGameData::Inst()->newChangeRequest("add_lv_5"))
                {
                    
                    m_iReqType = enRawReqType_add_lv_5;
                    scheduleUpdate();
                }
                break;
                
              
            default:
                break;
        }
        
        
    }
    else {
        CGameData::Inst()->clearReqStat();
        
        MainScene::Inst()->showBottomPanel(true);
    }
}

void RewardMain::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RewardMain::cbCommDlg), enCommDlgTp_connecting);
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
        
        switch (m_iReqType)
        {
                case enRawReqType_add_award:
                
                MainScene::Inst()->enableBottomPanel(false);
                
                break;
                
            case enRawReqType_add_lv_5:
                
                MainScene::Inst()->enableBottomPanel(false);
                
                break;
                
                default:
                
                break;

                
        }
        
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RewardMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


//******************************************************************************
// newbie
//******************************************************************************
void RewardMain::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);

    
    
    Arrow* pArrow = new Arrow(30, 255);
    
    pArrow->setPosition(CCPointMake(411, 635));
    
    
    
    pArrow->setRotation(180);
    //    pArrow->setScale(0.6f);
    
    MainScene::Inst()->addChild(pArrow, 50000,  99998);
    
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));
    
    
    if(CGameData::Inst()->newChangeRequest("add_award"))
    {
        
        MainScene::Inst()->new_MyNew = true;
        
        
        m_iReqType = enRawReqType_add_award;
        scheduleUpdate();
    }

    
    
//    MainScene::Inst()->removeTouchTip();
//    MainScene::Inst()->enableBottomPanel(true);
    
    


    
    
}


void RewardMain::newbie1(CCObject* pObj)
{
    
    //引导领奖结束
    
    
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);

    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(99996);
    if(temp)
        temp->removeFromParentAndCleanup(true);

    
    
//    MainScene::Inst()->enterNormalArenal();
    
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    
    MainScene::Inst()->new_MyNew = false;
    
    
}

void RewardMain::newbie2(CCObject* pObj)
{
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98996);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    //引导
//    MainScene::Inst()->showTouchTip();
    MainScene::Inst()->enableBottomPanel(false);
    
    Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(RewardMain::newbie3),
                                        CGameData::Inst()->getLanguageValue("lingJiang_new7"),false);
    addChild(newbieEndDlg, 500, kNewbie);
    newbieEndDlg->release();
    newbieEndDlg->setPosition(CCPointMake(320, 300));
    newbieEndDlg->setScale(0.0);
    newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    
    
    
    Arrow* pArrow = new Arrow(30, 255);
    
    pArrow->setPosition(CCPointMake(231, 245));
    
    
    
    pArrow->setRotation(180);
    //    pArrow->setScale(0.6f);
    
    MainScene::Inst()->addChild(pArrow, 50000, 98995);
    
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));


    
    if(CGameData::Inst()->newChangeRequest("add_lv_5"))
    {
        
        MainScene::Inst()->new_MyNew = true;
        
        m_iReqType = enRawReqType_add_lv_5;
        scheduleUpdate();
    }
    
}


void RewardMain::newbie3(CCObject* pObj)
{
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98995);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
//    MainScene::Inst()->enterNormalArenal();
    
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    
    
    MainScene::Inst()->new_MyNew = false;
}






void RewardMain::showLabel(int label){
    if (label == EN_RWDSIGHLAYER)
    {
        showRwdSignLayer();
    }
    else if (label == EN_RWDLEVELUP)
    {
        showLevelUpLayer();
    }
    else if (label == EN_RWDMONTH)
    {
        showMonthLayer();
    }
    else if (label == EN_RWDLEIJI)
    {
        showLeiJiLayer();
    }
    else if (label == EN_RWDHUODONG)
    {
        showHuodongLayer();
    }
}

void RewardMain::exit()
{
   // innerExit();
    
    removeAllChildrenWithCleanup(true);
}

void RewardMain::innerExit()
{
    if (m_rewardSignLayer!=NULL)
    {
        m_rewardSignLayer->removeFromParentAndCleanup(true);
        m_rewardSignLayer = NULL;
    }
    if (m_rewardLevelUpLayer!=NULL)
    {
        m_rewardLevelUpLayer->removeFromParentAndCleanup(true);
        m_rewardLevelUpLayer = NULL;
    }
    if (m_rewardMonthLayer!=NULL)
    {
        m_rewardMonthLayer->removeFromParentAndCleanup(true);
        m_rewardMonthLayer = NULL;
    }
    if (m_rewardLeiJiLayer!=NULL)
    {
        m_rewardLeiJiLayer->removeFromParentAndCleanup(true);
        m_rewardLeiJiLayer = NULL;
    }
    if (m_rewardHuodongLayer!=NULL)
    {
        m_rewardHuodongLayer->removeFromParentAndCleanup(true);
        m_rewardHuodongLayer = NULL;
    }
    if (m_rewardBuchangLayer!=NULL)
    {
        m_rewardBuchangLayer->removeFromParentAndCleanup(true);
        m_rewardBuchangLayer = NULL;
    }
}

void RewardMain::createAllBtn()
{
    ScrollHorizontal* hor = ScrollHorizontal::CreateHorizontalScroll(this, callfuncO_selector(RewardMain::cbScrollHorizontal), 640, 84);
    
    addChild(hor, 100,SCORLLTAG);
    hor->setAnchorPoint(CCPointZero);
    hor->setPosition(CCPointMake(0, 720));
    
    /*if (CGameData::Inst()->getRwdSignInfList()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDSIGHLAYER);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDSIGHLAYER).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }
    
    if (CGameData::Inst()->getRwdLevelUpArr()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDLEVELUP);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDLEVELUP).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }
    
    if (CGameData::Inst()->getRwdMonthInfoArr()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDMONTH);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDMONTH).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }
    
    if (CGameData::Inst()->getRwdBonusInfArr()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDLEIJI);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDLEIJI).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }
    
    if (CGameData::Inst()->getRwdHuodongInfArr()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDHUODONG);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDHUODONG).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }*/
    
/*    if (CGameData::Inst()->getRwdBuchangInfArr()->count()>0)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + EN_RWDBUCHANG);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(EN_RWDBUCHANG).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
    }
*/
    
    
    for (int i=0; i<EN_RWD_MAX; i++)
    {
        CCSprite* BTN1 = CCSprite::spriteWithFile("rwdbtnxuan.png");
        BTN1->setTag(TAGDELTARWDBTN + i);
        hor->PushBackItem(BTN1);
        TextNode* text = TextNode::textWithString(GetButtonLabel(i).c_str(), 30);
        BTN1->addChild(text, 10);
        text->setPosition(CCPointMake(BTN1->getContentSize().width/2-4, BTN1->getContentSize().height/2+4));
        if(i==0&&(CGameData::Inst()->getRwdSignInfList()->count()-1)>0){
            signUpNum = CGameData::Inst()->getRwdSignInfList()->count()-1;
            if (signUpNum>0) {
                signNum = OcProxy::Inst()->showTipsAndNum(CGameData::Inst()->getRwdSignInfList()->count()-1);
                signNum->setPosition(ccp(100, 65));
                BTN1->addChild(signNum,0,NUMTAG);
            }
        }else if(i==1&&CGameData::Inst()->getRwdLevelUpArr()->count()>0){
            levelUpNum = 0;
            for(int i=0;i<CGameData::Inst()->getRwdLevelUpArr()->count();++i){
                if(CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(i)->lv <= CGameData::Inst()->getUsrInfo()->lv){
                    levelUpNum += 1;
                }else
                    break;
            }
            if (levelUpNum>0) {
                signNum = OcProxy::Inst()->showTipsAndNum(levelUpNum);
                signNum->setPosition(ccp(100, 65));
                BTN1->addChild(signNum,0,NUMTAG);
            }
        }else if(i==3&&CGameData::Inst()->getRwdBonusInfArr()->count()>0){
            bonusNum = 0;
            for(int i=0;i<CGameData::Inst()->getRwdBonusInfArr()->count();++i){
                if(CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(i)->iDays >= CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(i)->rewardDays){
                    bonusNum += 1;
                }else
                    break;
            }
            if (bonusNum>0) {
                signNum = OcProxy::Inst()->showTipsAndNum(bonusNum);
                signNum->setPosition(ccp(100, 65));
                BTN1->addChild(signNum,0,NUMTAG);
            }
        }else if(i==4&&CGameData::Inst()->getRwdHuodongInfArr()->count()>0){
            sysNum = 0;
            for(int i=0;i<CGameData::Inst()->getRwdHuodongInfArr()->count();++i){
                if(!(CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->bCanGet)&&(CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->loginCounts>=CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->awardDays)){
                    sysNum += 1;
                }else
                    break;
            }
            if (sysNum>0) {
                signNum = OcProxy::Inst()->showTipsAndNum(sysNum);
                signNum->setPosition(ccp(100, 65));
                BTN1->addChild(signNum,0,NUMTAG);
            }
        }
    }
    
    hor->SetDefaultSelected(defaultLabel+TAGDELTARWDBTN);
}

string RewardMain::GetButtonLabel(int _index)
{
    string text;
    if (_index == EN_RWDSIGHLAYER)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_qiandao");
    }
    else if (_index == EN_RWDLEVELUP)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_shengji");
    }
    else if (_index == EN_RWDMONTH)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_yuefen");
    }
    else if (_index == EN_RWDLEIJI)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_leiji");
    }
    else if (_index == EN_RWDHUODONG)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_huodong");
    }
    /*else if (_index == EN_RWDBUCHANG)
    {
        text = CGameData::Inst()->getLanguageValue("rwd_main_buchang");
    }
    */
    return text;
}

void RewardMain::showRwdSignLayer()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("only_save_7days");
    // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_rewardSignLayer == NULL)
    {
        m_rewardSignLayer = new RwdSighLayer(this,callfuncO_selector(RewardMain::cbRwdSignLayer));
        addChild(m_rewardSignLayer);
        m_rewardSignLayer->release();
    }
    
    m_rewardSignLayer->setPosition(ccp(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardSignLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}

void RewardMain::showLevelUpLayer()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("rwdleveluptips");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_rewardLevelUpLayer == NULL)
    {
        m_rewardLevelUpLayer = new RwdLevelUpLayer(this, callfuncO_selector(RewardMain::cbRwdLevelUpLayer));
        addChild(m_rewardLevelUpLayer);
        m_rewardLevelUpLayer->release();
    }
    
    m_rewardLevelUpLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardLevelUpLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}

void RewardMain::showMonthLayer()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("clear_firstday_of_month");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    
    if (m_rewardMonthLayer == NULL)
    {
        m_rewardMonthLayer = RwdMonthLayer::Create(this, callfuncO_selector(RewardMain::cbRwdMonthLayer));
        addChild(m_rewardMonthLayer);
    }
    
    m_rewardMonthLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardMonthLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}

void RewardMain::showLeiJiLayer()
{
    innerExit();
  
    MainScene::Inst()->showBottomPromptInfo("rwdleijitips");
   //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_rewardLeiJiLayer == NULL)
    {
        m_rewardLeiJiLayer = new RwdBonusLayer(this, callfuncO_selector(RewardMain::cbRwdLeijiLayer));
        addChild(m_rewardLeiJiLayer);
        m_rewardLeiJiLayer->release();
    }
    
    m_rewardLeiJiLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardLeiJiLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}

void RewardMain::showHuodongLayer()
{
    innerExit();
    
    MainScene::Inst()->showBottomPromptInfo("rwdhuodongtips");
   // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_rewardHuodongLayer == NULL)
    {
        m_rewardHuodongLayer = new RwdHuodongLayer(this, callfuncO_selector(RewardMain::cbRwdHuodongLayer));
        addChild(m_rewardHuodongLayer);
        m_rewardHuodongLayer->release();
    }
    
    m_rewardHuodongLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardHuodongLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}

void RewardMain::showBuchangLayer()
{
    innerExit();
    
    if (m_rewardBuchangLayer == NULL)
    {
        m_rewardBuchangLayer = new RwdBuchangLayer(this, callfuncO_selector(RewardMain::cbRwdBuchangLayer));
        addChild(m_rewardBuchangLayer);
        m_rewardBuchangLayer->release();
    }
    
    m_rewardBuchangLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_rewardBuchangLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
}



//  ------------------  call back -------------------------------

void RewardMain::cbRwdSignLayer(CCObject* pObj)
{
    
    //引导
    if(MainScene::Inst()->new_MyNew && MainScene::Inst()->getChildByTag(99997))
    {
        
       
        
    }
    
    else
    {
        signUpNum = signUpNum - 1;
        if (signUpNum>0) {
            signNum = OcProxy::Inst()->showTipsAndNum(signUpNum);
            signNum->setPosition(ccp(100, 65));
            getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN)->removeChildByTag(NUMTAG, true);
            getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN)->addChild(signNum,0,NUMTAG);
        }else
            getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN)->removeChildByTag(NUMTAG, true);
        CCLog("ccbRwdSignLayerb");
        
        
        //引导
        Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(99998);
        if(temp)
            temp->removeFromParentAndCleanup(true);
    }

    
    
    
    

    
}

void RewardMain::cbRwdLevelUpLayer(CCObject* obj)
{
    CCLog("cbRwdLevelUpLayer");
    levelUpNum = levelUpNum - 1;
    if (levelUpNum>0) {
        signNum = OcProxy::Inst()->showTipsAndNum(levelUpNum);
        signNum->setPosition(ccp(100, 65));
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+1)->removeChildByTag(NUMTAG, true);
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+1)->addChild(signNum,0,NUMTAG);
    }else
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+1)->removeChildByTag(NUMTAG, true);
}

void RewardMain::cbRwdMonthLayer(CCObject* obj)
{
    CCLog("cbRwdMonthLayer");
}

void RewardMain::cbRwdLeijiLayer(CCObject* obj)
{
    CCLog("cbRwdLeijiLayer");
    bonusNum = bonusNum - 1;
    if (bonusNum>0) {
        signNum = OcProxy::Inst()->showTipsAndNum(bonusNum);
        signNum->setPosition(ccp(100, 65));
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+3)->removeChildByTag(NUMTAG, true);
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+3)->addChild(signNum,0,NUMTAG);
    }else
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+3)->removeChildByTag(NUMTAG, true);
    
}

void RewardMain::cbRwdHuodongLayer(CCObject* obj)
{
    CCLog("cbRwdHuodongLayer");
    sysNum = sysNum - 1;
    if (sysNum>0) {
        signNum = OcProxy::Inst()->showTipsAndNum(sysNum);
        signNum->setPosition(ccp(100, 65));
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+4)->removeChildByTag(NUMTAG, true);
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+4)->addChild(signNum,0,NUMTAG);
    }else
        getChildByTag(SCORLLTAG)->getChildByTag(TAGDELTARWDBTN+4)->removeChildByTag(NUMTAG, true);
}

void RewardMain::cbRwdBuchangLayer(CCObject* obj)
{
    CCLog("cbRwdBuchangLayer");
}



void RewardMain::cbScrollHorizontal(CCObject* obj)
{
    
    //引导
    if(MainScene::Inst()->new_MyNew )
    {
        
        ScrollHorizontalRet* ret = (ScrollHorizontalRet*) obj;
        int tag = ret->node_tag - TAGDELTARWDBTN;
        
        
        
        if(tag == 3 && MainScene::Inst()->getChildByTag(99997))
        {
            showLabel(tag);
            
            
            //引导
            Arrow * temp11 = (Arrow *)MainScene::Inst()->getChildByTag(99997);
            if(temp11)
            {
                MainScene::Inst()->removeChildByTag(99997, true);
                
                //引导
//                MainScene::Inst()->showTouchTip();
                MainScene::Inst()->enableBottomPanel(false);
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(RewardMain::newbie1),
                                                    CGameData::Inst()->getLanguageValue("lingJiang_new4"),false);
                addChild(newbieEndDlg, 500, kNewbie);
                newbieEndDlg->release();
                newbieEndDlg->setPosition(CCPointMake(320, 300));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                
                
                
                Arrow* pArrow = new Arrow(30, 255);
                
                pArrow->setPosition(CCPointMake(231, 245));
                
                
                
                pArrow->setRotation(180);
                //    pArrow->setScale(0.6f);
                
                MainScene::Inst()->addChild(pArrow, 50000, 99996);
                
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));
            }
            
            
            
//            
//            //引导暂时中断
//            
//            
//            MainScene::Inst()->removeTouchTip();
//            MainScene::Inst()->enableBottomPanel(true);
            
            
            
            
        }
    }
    else
    {
        ScrollHorizontalRet* ret = (ScrollHorizontalRet*) obj;
        int tag = ret->node_tag - TAGDELTARWDBTN;
        CCLog("cbScrollHorizontal-tag = %d", tag);
        showLabel(tag);
    }
    
    

    /*else if (tag == EN_RWDBUCHANG)
    {
        showBuchangLayer();
    }*/
}

void RewardMain::setLabel(int label){
    defaultLabel = label;
}



