//
//  FightMngr.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "FightMngr.h"
#include "CGameState.h"
#include "CSkillLib.h"
#include "CTipsDlg.h"
#include "MainScene.h"
#include "DeadDlg.h"
#include "CommDlg.h"
#include "CMenuBtn.h"
#include "TitleBar.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "SaveData.h"
#include "FsGuide.h"
#include "RecordHelper.h"
#include "OcProxy.h"

#include "TDCCVirtualCurrency.h"

#include "TDCCItem.h"

#include "TDCCMission.h"
#include "DeadReviveDlg.h"

#define kPayResultDlgTag  0x933
#define kPayBtnTagBase    0x999
#define kFsGuideTag       0xaaaa

#define kNewbie           99999

using namespace CocosDenshion;

CGameState* CGameState::s_pcIf = NULL;

//******************************************************************************
// Construct
//******************************************************************************
FightMngr::FightMngr(CCObject* target, SEL_CallFuncO selector)
{
    m_iReqType = enFightMngrReqType_Null;
    m_fanhuanDlg = NULL;
    m_isUseAwakeSkill29 = true;
    m_iSL_type = 0;
    m_checkRevDlg = NULL;
    m_commDlg = NULL;
    m_spFightScene = NULL;
    m_player = NULL;
    m_enemyGrp = NULL;
    pauseMenu = NULL;
    m_deadDlg = NULL;
    m_deadReviveDlg = NULL;
    m_starEffect = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
   
}

FightMngr::~FightMngr()
{
    exit();
    CCLog("remove all FightMngr");
}

void FightMngr::cbList(cocos2d::CCObject *pObj)
{

}

void FightMngr::btnCallback(CCObject* sender)
{
    closeCoinList();
    showDeathDlg();
}

//******************************************************************************
// startNewFight
//******************************************************************************
void FightMngr::startNewFight(int type, enSpecialDgSkillType skillType, bool cntryBuff)
{
    m_lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 0), 640, 960);
    addChild(m_lcCover, -100);
    
    m_spShowStep = CCSprite::spriteWithSpriteFrameName("battle_title.png");
    m_spShowStep->setPosition(CCPointMake(-1000, 480));
    addChild(m_spShowStep, 100);
    
    m_lbShowStep = TextNode::textWithString("", CCSizeMake(270, 50), CCTextAlignmentCenter, 48);
    m_lbShowStep->setPosition(CCPointMake(220, 80));
    m_spShowStep->addChild(m_lbShowStep);
    
    m_spGold = CCSprite::spriteWithSpriteFrameName("id_coin.png");
    m_spGold->setAnchorPoint(CCPointMake(0.5, 1));
    m_spGold->setPosition(CCPointMake(-1000, -1000));
    addChild(m_spGold, 10);
    
    m_spBox = CCSprite::spriteWithSpriteFrameName("goldbox.png");
    m_spBox->setAnchorPoint(CCPointMake(0.5, 1));
    m_spBox->setPosition(CCPointMake(-1000, -1000));
    addChild(m_spBox, 10);
    
    CCSprite* spTemp = CCSprite::spriteWithFile("yellownum.png");
    
    m_lbGold = CJumpNum::initNumber("yellownum.png");
    m_lbGold->setPosition(CCPointMake(-1000, -1000));
    m_lbGold->setAnchorPoint(CCPointMake(0, 1));
    m_lbGold->setColor(ccYELLOW);
    m_lbGold->setJump(false);
    m_lbGold->setIsScale(false);
    addChild(m_lbGold, 10);
    
    m_lbBox = CCLabelAtlas::labelWithString("0", "yellownum.png",
                                            spTemp->getContentSize().width/10,
                                            spTemp->getContentSize().height,
                                            '0');
    m_lbBox->setPosition(CCPointMake(-1000, -1000));
    m_lbBox->setAnchorPoint(CCPointMake(0, 1));
    m_lbBox->setColor(ccWHITE);
    addChild(m_lbBox, 10);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("menu.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("menu.png");
    m_menu = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                   spMenu2,
                                                   this,
                                                   menu_selector(FightMngr::menuCallback));
    m_menu->setAnchorPoint(ccp(0, 0));
    m_menu->setPosition(CCPointMake(-1000, -1000));
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("triangle.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("triangle.png");
    m_btnCaiDan = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                        spMenu2,
                                                        this,
                                                        menu_selector(FightMngr::cbCaiDan));
    m_btnCaiDan->setAnchorPoint(CCPointZero);
    m_btnCaiDan->setPosition(CCPointMake(-1000, -1000));
    
    spMenu1 = CCSprite::spriteWithFile("triangle2.png");
    spMenu2 = CCSprite::spriteWithFile("triangle2.png");
    m_btnNormal = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                        spMenu2,
                                                        this,
                                                        menu_selector(FightMngr::cbCaiDan));
    m_btnNormal->setAnchorPoint(CCPointZero);
    m_btnNormal->setPosition(CCPointMake(-1000, -1000));
    
    CCMenu* menu = CCMenu::menuWithItems(m_menu, m_btnCaiDan, m_btnNormal, NULL);
    addChild(menu, 10);
    menu->setPosition(CCPointZero);
    
    m_tips = new CTipsDlg(this, callfunc_selector(FightMngr::cbTipsDlg));
    m_tips->setPosition(CCPointMake(320, 260));
    addChild(m_tips, 300);
    m_tips->release();
    
    m_iDropItemCnt = 0;
    m_iDropGoldTotal = 0;
    
    m_listCoin = new CListLayer(this, callfuncO_selector(FightMngr::cbList));
    addChild(m_listCoin, 1200);
    m_listCoin->release();
    
    
    
    //back button
    spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FightMngr::btnCallback) );
    m_miBack->setTag(4);
    m_miBack->setAnchorPoint(CCPointZero);
    menu = CCMenu::menuWithItems( m_miBack, NULL);
    addChild(menu, 1230);
    menu->setPosition(CCPointZero);
    
    
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    
    //title
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 1200);
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    
    SaveData::Inst()->initStepInfo(skillType);
    SaveData::Inst()->initPlayerInfo(cntryBuff);

    m_lcCover->setOpacity(0);
    m_lcCover->runAction(CCSequence::actions(CCFadeIn::actionWithDuration(0.5),
                                             CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::cbStartFight)),
                                             NULL));
    fightSceneType = type;
}

//******************************************************************************
// getCoin
//******************************************************************************
int FightMngr::getCoin()
{
    return m_iDropGoldTotal;
}

//******************************************************************************
// cbPlayer
//******************************************************************************
void FightMngr::cbPlayer(CCObject* pObj)
{
    CAttackInfo* pAtkInfo = (CAttackInfo*) pObj;
    
    
    if(pAtkInfo->bAtkOvr && m_player->isDead() && CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
    {//死亡弹出框
        CGameState::Instance()->setState(enStateGameOver);
        
        if (m_enemyGrp && m_enemyGrp->isBuffEffect(enEnemyBuff_shenfa))
        {
            m_tips->show();
        }
        else
        {
            schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
        }
        
        return;
    }
    
    
    if(CGameState::Instance()->getState() >= enStateEnemyDead)
        return;
    
    if(pAtkInfo->bAtkOvr == true)
    {
        CGameState::Instance()->setState(enStateEnemyAttack);
        
        float f = 0.0f;
        if (m_enemyGrp && (!m_enemyGrp->isAllEnemyDead())){
            m_player->getStoneGrid()->checkStoneGrid();
        }

        if (f > 0.1f)
            schedule(schedule_selector(FightMngr::PlayerEndEnemyBegin), f);
        else
            PlayerEndEnemyBegin(0);
    }
    else
    {
        stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pAtkInfo->iManSkillID);
        stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(pAtkInfo->iAutoSkillID);
        
        if(((pAtkInfo->iManSkillID == kInvalidValue) && (pAtkInfo->iAutoSkillID == kInvalidValue))
        || ((pManSkill) && (pManSkill->iType == enManSkillTp_atk))
        || ((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_elemburn))
        || ((pManSkill) && (pManSkill->iType == enManSkillTp_stonerand)))
        {
            CCPoint* ptQueue = m_enemyGrp->queryAtkPos(pAtkInfo);
            while (ptQueue->y != 0)
            {
                shoot(pAtkInfo->srcPt, *ptQueue);
                ptQueue++;
            }
        }
        
        int iAtkPt = m_enemyGrp->beAttacked(pAtkInfo, 0.2);
        
        //巧变
        if(pAtkInfo->iManSkillID == enManSkill_QiaoBian)
        {
            if (!m_player->isDead())
            {
                m_player->addBlood(iAtkPt);
                m_player->updateRoundStartHp();
            }
        }
        else if (pAtkInfo->iManSkillID == enManSkill_Atk20Rcv05)
        {
            m_player->addBlood(iAtkPt * 0.05);
            m_player->updateRoundStartHp();
        }
        else if (pAtkInfo->iManSkillID == enManSkill_235)
        {
            m_player->addBlood(iAtkPt * 0.5);
            m_player->updateRoundStartHp();
        }
        else if (pAtkInfo->iManSkillID == enManSkill_230)
        {
            m_player->addBlood(iAtkPt * 0.1);
            m_player->updateRoundStartHp();
        }
        else if (pAtkInfo->iManSkillID == enManSkill_273){
            m_player->addBlood(iAtkPt * 0.15);
            m_player->updateRoundStartHp();
        }
        
        if(m_enemyGrp->isAllEnemyDead()) {
            SaveData::Inst()->setCurTouchCnt(0);
            SaveData::Inst()->OneRoundOvrNotify();
        }
    }
    
    if(m_enemyGrp->isAllEnemyDead())
    {
        if((CGameData::Inst()->isNewbie()) && (m_iCurStep == 0)){
            //新手引导第一步打完之后需要显示msg提示
            //msg显示完，bAtkOvr变为true, 直接进下一局
            if(pAtkInfo->bAtkOvr){
                int cardnum = m_enemyGrp->collectCard();
                m_iDropItemCnt += cardnum;
                m_iDropGoldTotal += m_enemyGrp->collectGold();
                updateInfBar(m_iDropGoldTotal, m_iDropItemCnt);
                
                m_iCurStep++;
                startStep();
            }
        }
        else
        {
            //if (!pAtkInfo->bAtkOvr)
            {
                m_player->getStoneGrid()->checkStoneGrid();
            }
            
            CGameState::Instance()->setState(enStateEnemyDead);
            m_ret.bClear = true;
            m_iCollectStep = 0;
            if (CGameData::Inst()->isNewbie()) {
                schedule(schedule_selector(FightMngr::cbCollect), 2.0f);
            }
            else {
                schedule(schedule_selector(FightMngr::cbCollect), 1.2f);
            }
        }
    }
    else
    {
        int iGold = m_iDropGoldTotal + m_enemyGrp->collectGold();
        updateInfBar(iGold, m_iDropItemCnt);
    }
}

void FightMngr::PlayerEndEnemyBegin(ccTime dt)
{
   // CGameState::Instance()->setState(enStateEnemyAttack);
    SaveData::Inst()->setCurTouchCnt(SaveData::Inst()->getCurTouchCnt() + 1);
    // enEnemyBuff_83,             //83 进入回合起10回合不被击杀，直接自爆造成99999伤害
    for (int i = 0; i < m_enemyGrp->getEnemyCount(); i++) {
        Enemy *tempEnemy = m_enemyGrp->getEnemyByIdx(i);
        if (tempEnemy->isAlive()) {
            for(int i = 0; i < MAX_BUFF_ITEMS; i++) {
                if (tempEnemy->getBuffId(i) == enEnemyBuff_83 && SaveData::Inst()->getCurTouchCnt() == 10) {
                    tempEnemy->enemyBuff_83();
                    tempEnemy->changeCurHPAction(-(tempEnemy->getCurHp()), 0.5f);
                    tempEnemy->checkDie(0.5);
                    break;
                }
            }
           
        }
    }
    if (m_enemyGrp->getEnemyCount() > 0) {
        m_enemyGrp->attack();
        
        SaveData::Inst()->OneRoundOvrNotify();
    }
}

void FightMngr::ddFreeReviveCallback(CCObject* pSender)
{
    DeadDlgRet *ret =(DeadDlgRet *)pSender;
    if(ret->iBtnsel == 111)
    {
        m_iReqType=enFightMngrReqType_Revive;
        if (SaveData::Inst()->hasValidSavedData()
            && SaveData::Inst()->getReviveTime() != 0) {
            m_lReviveRty = SaveData::Inst()->getReviveTime();
        }
        else {
            m_lReviveRty = CGameData::Inst()->getCurrentTime();
            SaveData::Inst()->setReviveTime(m_lReviveRty);
            SaveData::Inst()->flush();
        }
        SaveData::Inst()->setFreeRevive(1);
        
        CGameData::Inst()->reviveRequest(m_lReviveRty);
        scheduleUpdate();
     
    }
    else
    {
        if(!m_checkRevDlg)
        {
            m_checkRevDlg = new Dialog2(this, callfuncO_selector(FightMngr::checkReviceCb), CGameData::Inst()->getLanguageValue("confirmDeadTip1"), CGameData::Inst()->getLanguageValue("confirmDeadTip2"), enDl2TwoBtn);
            addChild(m_checkRevDlg,2000);
            m_checkRevDlg->setPosition(ccp(320, 500-240));
            m_checkRevDlg->setScale(0);
            m_checkRevDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8));
            m_checkRevDlg->release();
        }
    }
    m_deadDlg->removeFromParentAndCleanup(true);
    m_deadDlg = NULL;
   
}

void FightMngr::ddReviveCallback(CCObject *pSender)
{
    DeadReviveDlgRet *ret =(DeadReviveDlgRet *)pSender;
    if(ret->iBtnsel == 111)
    {
        if(ret->iOptionsel >= 201 && ret->iOptionsel <= 203)
        {
            int need;
            long hold;
            
            need = ret->iCost;
            
            if(CGameData::Inst()->getUsrInfo()->login_time > 0){
                hold =  CGameData::Inst()->getUsrInfo()->coin;
                SaveData::Inst()->setCurCoin(hold);
            }
            else{
                hold = SaveData::Inst()->getCurCoin();
            }
            
            if(hold < need)
            {
#if defined (apppay)
                if(CGameData::Inst()->getStoreStatus() < 1)
#endif
                    CGameData::Inst()->loadStore();
                beginCoinShop();
            }
            else
            {
                m_iReqType=enFightMngrReqType_Revive;
                if (SaveData::Inst()->hasValidSavedData()
                    && SaveData::Inst()->getReviveTime() != 0) {
                    m_lReviveRty = SaveData::Inst()->getReviveTime();
                }
                else {
                    m_lReviveRty = CGameData::Inst()->getCurrentTime();
                    SaveData::Inst()->setReviveTime(m_lReviveRty);
                    SaveData::Inst()->flush();
                }
                
                CGameData::Inst()->reviveRequest(m_lReviveRty,ret->strItemId);
                scheduleUpdate();
            }
        }
    }else
    {
        if(!m_checkRevDlg)
        {
            m_checkRevDlg = new Dialog2(this, callfuncO_selector(FightMngr::checkReviceCb), CGameData::Inst()->getLanguageValue("confirmDeadTip1"), CGameData::Inst()->getLanguageValue("confirmDeadTip2"), enDl2TwoBtn);
            addChild(m_checkRevDlg,2000);
            m_checkRevDlg->setPosition(ccp(320, 500-240));
            m_checkRevDlg->setScale(0);
            m_checkRevDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8));
            m_checkRevDlg->release();
        }
    }

    m_deadReviveDlg->removeFromParentAndCleanup(true);
    m_deadReviveDlg = NULL;
}

//DeadDlg callback
void FightMngr::ddCallback(CCObject *pSender)
{
    DeadDlgRet *ret =(DeadDlgRet *)pSender;
    if(ret->iBtnsel == 111)
    {
        int need;
        long hold;
        if (CGameData::Inst()->chkLogin()) {
            need =  CGameData::Inst()->getCommonInfo()->revive_coin;
        }
        else {
            need = SaveData::Inst()->getReviveCoin();
        }
        
        if(CGameData::Inst()->getUsrInfo()->login_time > 0){
            hold =  CGameData::Inst()->getUsrInfo()->coin;
            SaveData::Inst()->setCurCoin(hold);
        }
        else{
            hold = SaveData::Inst()->getCurCoin();
        }
        
        if(hold < need)
        {
#if defined (apppay)
            if(CGameData::Inst()->getStoreStatus() < 1)
#endif
                CGameData::Inst()->loadStore();
            beginCoinShop();
        }
        else
        {
            m_iReqType=enFightMngrReqType_Revive;
            if (SaveData::Inst()->hasValidSavedData()
                && SaveData::Inst()->getReviveTime() != 0) {
                m_lReviveRty = SaveData::Inst()->getReviveTime();
            }
            else {
                m_lReviveRty = CGameData::Inst()->getCurrentTime();
                SaveData::Inst()->setReviveTime(m_lReviveRty);
                SaveData::Inst()->flush();
            }

            CGameData::Inst()->reviveRequest(m_lReviveRty);
            scheduleUpdate();
        }
    }
    else {
        //就这儿
        if(!m_checkRevDlg)
        {
            m_checkRevDlg = new Dialog2(this, callfuncO_selector(FightMngr::checkReviceCb), CGameData::Inst()->getLanguageValue("confirmDeadTip1"), CGameData::Inst()->getLanguageValue("confirmDeadTip2"), enDl2TwoBtn);
            addChild(m_checkRevDlg,2000);
            m_checkRevDlg->setPosition(ccp(320, 500-240));
            m_checkRevDlg->setScale(0);
            m_checkRevDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8));
            m_checkRevDlg->release();
        }

    }
    
    m_deadDlg->removeFromParentAndCleanup(true);
    m_deadDlg = NULL;
}

void FightMngr::checkReviceCb(CCObject *sender)
{
    
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    if(ret->bOk)
    {
        string tt = SaveData::Inst()->getDgType();
        
        if (
            CGameData::Inst()->getUsrInfo()->lv <= 20
            && CGameData::Inst()->getUsrInfo()->dungeon_fail != -1 && CGameData::Inst()->getUsrInfo()->dungeon_fail != 3
            && tt.compare("normal") == 0
            
            )
        {
            if(CGameData::Inst()->newGiveWuJiangRequest("2"))
            {
                
                m_iReqType = enFightMngrReqType_GiveWuJiang;
                scheduleUpdate();
            }
            
            

        }
        else
        {
            CGameState::Instance()->setState(enStateGameOver);
            m_tips->show();
            StopRecordScreen(false);
        }
        

        m_ret.bClear = false;
    }else
    {
        showDeathDlg();
    }
    
    removeChild(m_checkRevDlg, true);
    m_checkRevDlg = NULL;
}
void FightMngr::closeCoinList()
{
    m_spTitle->stopAllActions();
    m_listCoin->stopAllActions();
    m_miBack->stopAllActions();
    
    m_listCoin->setPosition(CCPointMake(-1000, -1000));
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    
}


void FightMngr::beginCoinShop()
{
#if defined (apppay)
    if (CGameData::Inst()->getStoreStatus() != 2) {
        showPayResult(PaymentTimeOut);
    }
    else
    {
        if (SaveData::Inst()->hasValidSavedData())
        {
            getShopInfo();
        }
        else 
        {
            enterCoinList();
        }
    }
#elif defined (alipay)  || defined (kPltfmPp)
    if (SaveData::Inst()->hasValidSavedData())
    {
        getShopInfo();
    }
    else
    {
        enterCoinList();
    }
#endif
}

void FightMngr::getShopInfo()
{
    if(CGameData::Inst()->getShopRequest()){
        m_iReqType = enFightMngrReqType_GetShopInfo;
        scheduleUpdate();
    }
}

void FightMngr::enterCoinList()
{
    m_listCoin->clearAll();
    CGameData::Inst()->updateCoin();
    
    for (int i=0; i<CGameData::Inst()->getProductCnt(); i++) {
        coinBtnAdd(CGameData::Inst()->getIapIdAtIdx(i),
                   CGameData::Inst()->getPriceAtIdx(i),
                   i + kPayBtnTagBase);
    }

    m_listCoin->setPosition(CCPointMake(640, 0));
    m_listCoin->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0, 0)),0.8f));
        
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_yuanbao"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));

}

void FightMngr::coinBtnAdd(const char *iap_id,const char *price,int index)
{
    if(strncmp(iap_id, kProductId00, 99) == 0
       || strncmp(iap_id, kProductId09, 99) == 0
       || strncmp(iap_id, kProductId11, 99) == 0
       || strncmp(iap_id, kProductId12, 99) == 0
       || strncmp(iap_id, kProductId13, 99) == 0)
    {
        return;
    }
    
    CMenuBtn* btn;
    
    btn = new CMenuBtn(price, iap_id,false);
    m_listCoin->push_back(btn);
    btn->release();
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    
    if(spMenu1 && spMenu2){
        CCMenuItem *buybtn = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                   spMenu2,
                                                                   this,
                                                                   menu_selector(FightMngr::payBtnCB));
        buybtn->setTag(index);
        
        TextNode *itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip14"),
                                                       kBtnTitleHeight);
        itemLabel->setPosition(ccp(77, 42));
        itemLabel->setShadowColor(ccBLACK);
        buybtn->addChild(itemLabel);
        buybtn->setAnchorPoint(ccp(0, 0));
        
        CCMenu *menu = CCMenu::menuWithItems(buybtn,NULL);
        btn->addChild(menu);
        menu->setPosition(CCPointZero);
        buybtn->setPosition(ccp(132,-44));
        buybtn->setScale(0.9);
        
        
        CCSprite* spShadowMenu = NULL;
        
        if ( strncmp(iap_id, kProductId11, 99) == 0 ){
            
            if ( CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_30RMBMonth_11] == false){
                spShadowMenu = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            }else{
                payArr[enPayforzhaocaijinbao] = index;
            }
            buybtn->setTag(DialogType_zhaocaijinbao);
            
        }
        else if ( strncmp(iap_id, kProductId09, 99) == 0  ){
            if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_198RMBMonth_09] == false) {
                spShadowMenu = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            }else{
                payArr[enPayforcaiyungungun] = index;
            }
            buybtn->setTag(DialogType_caiyungungun);
        }
        else if (strncmp(iap_id, kProductId12, 99) == 0 ){
            payArr[enPayformengjiangwushuang] = index;
            buybtn->setTag(DialogType_mengjiangwushuang);
        }
        else if (strncmp(iap_id, kProductId13, 99) == 0 ){
            payArr[enPayforluanshishenjiang] = index;
            buybtn->setTag(DialogType_luanshishenjiang);
        }else{
            
        }

        if (spShadowMenu) {
            spShadowMenu->setPosition(ccp(77, 42));
            buybtn->addChild(spShadowMenu);
        }

        
    }
    
    
    
    
}

void FightMngr::payBtnCB(CCObject *pSender)
{
    
//    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_zhaocaijinbao) {
//        if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_30RMBMonth_11] == false ) {
//            return;
//        }
//        ((CCMenuItemImage *)pSender)->setTag(payArr[enPayforzhaocaijinbao]);
//    }
//    
//    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_caiyungungun) {
//        if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_198RMBMonth_09] == false ) {
//            return;
//        }
//        ((CCMenuItemImage *)pSender)->setTag(payArr[enPayforcaiyungungun]);
//    }
    
    
    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_zhaocaijinbao) {
        if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_30RMBMonth_11] == false ) {
            return;
        }
        payTag = payArr[enPayforzhaocaijinbao];
    
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip29");
            
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(FightMngr::showGiftBage), title, CGameData::Inst()->getLanguageValue("shopTip32"), enDl2TwoBtn);
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg,2000);
            m_fanhuanDlg->release();
        }
        return;
    }
    
    if ( ((CCMenuItemImage *)pSender)->getTag() == DialogType_caiyungungun) {
        if (CGameData::Inst()->getUsrInfo()->bSpecialIap[enSpecialIap_198RMBMonth_09] == false ) {
            return;
        }
        
        payTag = payArr[enPayforcaiyungungun];
      
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip29");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(FightMngr::showGiftBage), title, CGameData::Inst()->getLanguageValue("shopTip33"), enDl2TwoBtn);
            
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg,2000);
            m_fanhuanDlg->release();
        }
        return;
    }
    
    if (((CCMenuItemImage *)pSender)->getTag() == DialogType_mengjiangwushuang) {
        payTag = payArr[enPayformengjiangwushuang];
     
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip34");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(FightMngr::showGiftBage), title, CGameData::Inst()->getLanguageValue("shopTip35"), enDl2TwoBtn);
            
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg,2000);
            m_fanhuanDlg->release();
        }
        return;
    }
    if (((CCMenuItemImage *)pSender)->getTag() == DialogType_luanshishenjiang) {
        payTag = payArr[enPayforluanshishenjiang];
     
        if (m_fanhuanDlg == NULL) {
            const char* title = CGameData::Inst()->getLanguageValue("shopTip34");
            m_fanhuanDlg = new Dialog2(this, callfuncO_selector(FightMngr::showGiftBage), title, CGameData::Inst()->getLanguageValue("shopTip36"), enDl2TwoBtn);
            
            m_fanhuanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
            
            addChild(m_fanhuanDlg,2000);
            m_fanhuanDlg->release();
        }
        return;
    }

    
#if !defined(kPltfmPp)
    closeCoinList();
#endif
    
    if(m_commDlg == NULL){
        MainScene::Inst()->enableBottomPanel(false);
        m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::cbCommDlg), enCommDlgTp_connecting);
        addChild(m_commDlg, 1000);
        m_commDlg->setPosition(CCPointMake(320, 480));
        m_commDlg->release();
        //m_commDlg->setScale(0.0);
        //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
    payForCoin(((CCMenuItemImage *)pSender)->getTag());
    
}


void FightMngr::showGiftBage(CCObject* pSender){
    Dialog2Ret* pRet = (Dialog2Ret*) pSender;
    
    if(m_fanhuanDlg)
    {
        removeChild(m_fanhuanDlg, true);
        m_fanhuanDlg = NULL;
    }
    
    if(pRet->bOk == true)
    {
        
        closeCoinList();
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        if (payTag > 0) {
            payForCoin(payTag);
          
        }else{
            CCLOG("请求充值的数据有问题");
        }
        
        
    }else{
       
    }
}


void FightMngr::showPayResult(int result)
{
    Dialog1* payResultDlg;
    switch (result) {
        case PaymentFailed:
            payResultDlg= new Dialog1(this, callfuncO_selector(FightMngr::dialog1Callback), CGameData::Inst()->getLanguageValue("shopTip15"),false);   
            break;
        case PaymentTimeOut:
            payResultDlg= new Dialog1(this, callfuncO_selector(FightMngr::dialog1Callback),CGameData::Inst()->getLanguageValue("shopTip16"),false);
            break;
        case PaymentAliSuccess:
            payResultDlg= new Dialog1(this, callfuncO_selector(FightMngr::dialog1Callback),CGameData::Inst()->getLanguageValue("buy_success"),false);
            break;
        default:
            break;
    }
    addChild(payResultDlg, 500,kPayResultDlgTag);
    payResultDlg->setPosition(CCPointMake(320, 480));
    payResultDlg->setScale(0.0);
    payResultDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    payResultDlg->release();
    
}

void FightMngr::dialog1Callback(CCObject* sender)
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    CCNode* payResultDlg = getChildByTag(kPayResultDlgTag);
    if(payResultDlg)
    {
        removeChild(payResultDlg, true);
    }
    showDeathDlg();
}

void  FightMngr::showDeathDlg()
{
    CGameState::Instance()->setState(enStateGameOver);
    saveCurFightInfo();
    
    char msg[40];
    sprintf(msg, "%s%d&&%d", SaveData::Inst()->getDgType() , SaveData::Inst()->getFloorId() , SaveData::Inst()->getRoomId());
    
    
#ifdef TalkData
    TDCCMission::onFailed(msg , "fail");
#else
    
#endif
    
    CUserInfo *cuUsrInfo = CGameData::Inst()->getUsrInfo();
    int preLV = cuUsrInfo->lv;
    //等级低于free_revive_lv 并且不是不同战场的时候，可以免费复活一次
    CCLog("preLV  %d ",preLV);
    CCLog("SaveData::Inst()->getFreeRevive()  %d ",SaveData::Inst()->getFreeRevive());
    CCLog("SaveData::Inst()->getDgType()  %s ",SaveData::Inst()->getDgType());
    if(preLV <= CGameData::Inst()->getCommonInfo()->free_revive_lv && SaveData::Inst()->getFreeRevive() == 0
       && (strcmp(SaveData::Inst()->getDgType(), "normal") != 0))
    {
        m_deadDlg = new DeadDlg(this,callfuncO_selector(FightMngr::ddFreeReviveCallback),enDeadType_free);
        addChild(m_deadDlg);
        m_deadDlg->setPosition(CCPointMake(0, -240));
        m_deadDlg->release();
        
    }
    else if(SaveData::Inst()->getSelectRevive())
    {
        m_deadReviveDlg = new DeadReviveDlg(this,callfuncO_selector(FightMngr::ddReviveCallback));
        addChild(m_deadReviveDlg);
        m_deadReviveDlg->setPosition(CCPointMake(0, -240));
        m_deadReviveDlg->release();
    }
    else
    {
        m_deadDlg = new DeadDlg(this,callfuncO_selector(FightMngr::ddCallback),enDeadType_normal);
        addChild(m_deadDlg);
        m_deadDlg->setPosition(CCPointMake(0, -240));
        m_deadDlg->release();
    }
}

void FightMngr::waitPurchaseCb(ccTime dt)
{
    int iPurchaseStatus = CGameData::Inst()->getPurchaseStatus();
    
    if(iPurchaseStatus == 1)
        return;
    
    //支付失败
    if(iPurchaseStatus == -1){
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else if(iPurchaseStatus == 2){
        if(CGameData::Inst()->sendPaySuccessInfo()){
            m_iReqType = enFightMngrReqType_PaySuccess;
            scheduleUpdate();
        }
    }
    
    CGameData::Inst()->resetPurchaseStatus();
    unschedule(schedule_selector(FightMngr::waitPurchaseCb));
}

void FightMngr::payForCoin(int index)
{
    m_iIndex = index;
#if defined (alipay)
    m_iIndex = index;
    if (CGameData::Inst()->getOrderNo(CGameData::Inst()->getIapIdAtIdx(index-kPayBtnTagBase), CGameData::Inst()->getUsrInfo()->uid)){
        m_iReqType = enFightMngrReqType_GetTradeNo;
        scheduleUpdate();
    }
#elif defined (apppay)
    CGameData::Inst()->payForCoin(index-kPayBtnTagBase);
    schedule(schedule_selector(FightMngr::waitPurchaseCb), 0.5);
#elif defined (kPltfmPp)
    if(CGameData::Inst()->reqPPBilloNo(OcProxy::Inst()->getIapIdAtIdx(index-kPayBtnTagBase)))
    {
        m_iReqType = enFightMngrReqType__PayppBillNo;
        schedule(schedule_selector(FightMngr::waitPPBilloNo), 0.5);
    }
#endif
}

void FightMngr::waitPPBilloNo(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(comState == kReqStatOk){
        unschedule(schedule_selector(FightMngr::waitPPBilloNo));
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        schedule(schedule_selector(FightMngr::waitPPPurchaseCb), 0.5);
        OcProxy::Inst()->buyPp(OcProxy::Inst()->getIntPriceAtIdx(m_iIndex-kPayBtnTagBase),CGameData::Inst()->getPPBilloNo().c_str(),OcProxy::Inst()->getTitleAtIdx(m_iIndex-kPayBtnTagBase),CGameData::Inst()->getUserId());
    }
}

void FightMngr::waitPPPurchaseCb(ccTime dt)
{
    int iPurchaseStatus = OcProxy::Inst()->getPpPurchaseStatus();
    if(iPurchaseStatus == 1)
        return;
    //支付失败
    if(iPurchaseStatus == -1){
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else if(iPurchaseStatus == 2){
        if(CGameData::Inst()->getShopRequest())
        {
            m_iReqType =  enFightMngrReqType_RefreshAlipay;
            scheduleUpdate();
        }
    }
    
    OcProxy::Inst()->resetPpPurchaseStatus();
    unschedule(schedule_selector(FightMngr::waitPPPurchaseCb));
}


void FightMngr::showAlipayOver(int payType)
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    //支付失败
    if (payType!=0) {
        showPayResult(PaymentFailed);
    }
    //扣款成功
    else
    {
        if(CGameData::Inst()->reqChargeAward()){
            m_iReqType = enFightMngrReqType_RefreshAlipay;
            scheduleUpdate();
        }
    }
}




//******************************************************************************
// cbEnemyGrp
//******************************************************************************
void FightMngr::cbEnemyGrp(CCObject* pObj)
{
    
    CAttackInfo* pAtkInfo = (CAttackInfo*) pObj;
   
    
    if(CGameState::Instance()->getState() >= enStateEnemyDead) {
        if (m_enemyGrp->isAllEnemyDead() && m_player->isDead() && m_iCurStep == m_iMaxStep - 1) {
            pAtkInfo->bAtkOvr = true;
        } else {
            return;
        }
    }
    
    getEnemyGrp()->updateEnemyBuff();
    
    if(pAtkInfo->bAtkOvr == true)
    {
        m_enemyGrp->ResetEnemyLianFanState();//新增
    
        if (m_enemyGrp->getIsEnemyGrpBuffByIdx(enEnemyBuff_shuxinghudun))
            m_enemyGrp->ChangeShuxinghudun();
        
        if (m_enemyGrp->getIsEnemyGrpBuffByIdx(enEnemyBuff_shuxinghudun2))
            m_enemyGrp->ChangeShuxinghudun2();
        
        m_player->enemyAtkOvr();
        
        if (pAtkInfo->fristAtkOver == true) {
            
        }else{
          if(!m_enemyGrp->isAllEnemyDead())
              updateEnemyGrpBuff();
        }
        
        if(m_player->isDead() && CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
        {//死亡弹出框
            CGameState::Instance()->setState(enStateGameOver);
            if(CGameData::Inst()->isNewbie()){
                m_player->revive();
            }
            else
            {
                if (m_enemyGrp->isBuffEffect(enEnemyBuff_shenfa))
                {
                    m_tips->show();
                }
                else
                {
                    schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
                }
                return;
            }
        }
        
        if(m_enemyGrp->isAllEnemyDead())
        {
            CGameState::Instance()->setState(enStateEnemyDead);
            m_ret.bClear = true;
            m_iCollectStep = 0;
            if (CGameData::Inst()->isNewbie()) {
                schedule(schedule_selector(FightMngr::cbCollect), 2.0f);
            }
            else {
                schedule(schedule_selector(FightMngr::cbCollect), 1.2f);
            }
            
            return;
        }
        
        if(m_enemyGrp->isAllEnemyDead() == false)
        {
            if (pAtkInfo->fristAtkOver == true) {
                
            }else{
                m_player->startNewRound(false);
                setEnemyBuffEffect();
                saveCurFightInfo();
            }
           
        }
    }
    else if (pAtkInfo->iAttackPt > 0)
    {
  //      shoot(pAtkInfo->srcPt, m_player->getPosition());
  //      m_player->beAttacked(pAtkInfo);
 
        attackDetail(pAtkInfo);
    }
}



void FightMngr::changeStoneAndAtk(CAttackInfo* pAtkInfo)
{
    if(CGameState::Instance()->getState() >= enStateEnemyDead)
        return;
    
    if (pAtkInfo->iAttackPt > 0)
    {
        attackDetail(pAtkInfo);
    }
    
    if(m_player->isDead() && CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
    {//死亡弹出框
        CGameState::Instance()->setState(enStateGameOver);
        
        if (m_enemyGrp->isBuffEffect(enEnemyBuff_shenfa))
        {
            m_tips->show();
        }
        else
        {
            schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
        }
    }
    
}



void FightMngr::attackDetail(CAttackInfo* pAtkInfo)
{
    switch (pAtkInfo->iManSkillID)
    {
        case enEnemySkillHuoXiJin:       //8 封印火系武将,随机2-4回合
        case enEnemySkillHuoXiJin40:
        case enEnemySkillHuoXiJin10:
        case enEnemySkillHuoXiJin1_3:
        case enEnemySkill_126:
        case enEnemySkill_242:
        case enEnemySkill_245:
        case enEnemySkill_254:
        case enEnemySkill_353:
        case enEnemySkill_393:
        case enEnemySkill_471:
        {
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getElement()== enElementFire || m_player->getCardByIdx(i)->getSubElement()== enElementFire))
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    
                   // m_player->fengYinAutoSkill(i);
                    if (pAtkInfo->iManSkillID==enEnemySkillHuoXiJin1_3)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_126==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_242==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,(SaveData::Inst()->getrand()%3 + 2));
                    }
                    else if (enEnemySkill_245==pAtkInfo->iManSkillID)
                    {
                         m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,(SaveData::Inst()->getrand()%3 + 2));
                    }
                    else if (enEnemySkill_254==pAtkInfo->iManSkillID || enEnemySkill_353==pAtkInfo->iManSkillID || enEnemySkill_369==pAtkInfo->iManSkillID || enEnemySkill_393==pAtkInfo->iManSkillID || enEnemySkill_471==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,(SaveData::Inst()->getrand()%3 + 2));
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,(SaveData::Inst()->getrand()%3 + 2));
                    }
                }
            }
        }
            break;
            
        case enEnemySkillShuiXiJin:      //9 封印水系武将,随机2-4回合
        case enEnemySkillShuiXiJin40:
        case enEnemySkillShuiXiJin10:
        case enEnemySkillShuiXiJin1_3:
        case enEnemySkill_127:
        case enEnemySkill_213:
        case enEnemySkill_219:
        case enEnemySkill_227:
        case enEnemySkill_469:
        {
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getElement()== enElementWater || m_player->getCardByIdx(i)->getSubElement()== enElementWater))
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                 //   m_player->fengYinAutoSkill(i);
                    if (enEnemySkillShuiXiJin1_3==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_127==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_213==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN, SaveData::Inst()->getrand()%3 + 2);
                    }
                    else if (enEnemySkill_219==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN, SaveData::Inst()->getrand()%3 + 2);
                    }
                    else if (enEnemySkill_227==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN, 10);
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN, SaveData::Inst()->getrand()%3 + 2);
                    }
                }
            }
        }
            break;
            
        case enEnemySkillMuXiJin:        //10 封印木系武将,随机2-4回合
        case enEnemySkillMuXiJin40:
        case enEnemySkillMuXiJin10:
        case enEnemySkillMuXiJin1_3:
        case enEnemySkill_191:
        case enEnemySkill_284:
        case enEnemySkill_470:
        {
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getElement()== enElementWood || m_player->getCardByIdx(i)->getSubElement()== enElementWood))
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    
                 //   m_player->fengYinAutoSkill(i);
                    if (enEnemySkillMuXiJin1_3==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_191==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_284==pAtkInfo->iManSkillID || enEnemySkill_470==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                }
            }
        }
            break;
            
        case enEnemySkillGuangXiJin:     //11 封印光系武将,随机2-4回合
        case enEnemySkillGuangXiJin40:
        case enEnemySkillGuangXiJin10:
        case enEnemySkillGuangXiJin1_3:
        case enEnemySkill_153:
        case enEnemySkill_192:
        case enEnemySkill_258:
        case enEnemySkill_347:
        case enEnemySkill_473:
        {
            bool isEnemySkill_347 = false;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getElement()== enElementLight || m_player->getCardByIdx(i)->getSubElement()== enElementLight))
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    
              //      m_player->fengYinAutoSkill(i);
                    if (enEnemySkillGuangXiJin1_3==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_192==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_258==pAtkInfo->iManSkillID || enEnemySkill_473==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                    else if (enEnemySkill_347 == pAtkInfo->iManSkillID){
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,7);
                        isEnemySkill_347 = true;
                       
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                }
            }
            if (isEnemySkill_347) {
                shoot(pAtkInfo->srcPt, m_player->getPosition());
                m_player->beAttacked(pAtkInfo);
            }
            
        }
            break;
            
        case enEnemySkillAnXiJin:        //12 封印暗系武将,随机2-4回合
        case enEnemySkillAnXiJin40:
        case enEnemySkillAnXiJin10:
        case enEnemySkillAnXiJin1_3:
        case enEnemySkill_193:
        case enEnemySkill_223:
        case enEnemySkill_341:
        case enEnemySkill_369:
        case enEnemySkill_472:
        {
            bool isenEnemySkill_341 = false;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getElement()== enElementDark || m_player->getCardByIdx(i)->getSubElement()== enElementDark))
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    
             //       m_player->fengYinAutoSkill(i);
                    if (enEnemySkillAnXiJin1_3==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_193==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_223==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    }
                    else if (enEnemySkill_341 == pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,7);
                        isenEnemySkill_341 = true;
                       
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                }
            }
            if (isenEnemySkill_341) {
                shoot(pAtkInfo->srcPt, m_player->getPosition());
                m_player->beAttacked(pAtkInfo);
            }
            
        }
            break;
            
        case enEnemySkillShenJin:        //13 封印神系武将,随机2-4回合
        case enEnemySkillShenJin40:
        case enEnemySkillShenJin10:
        case enEnemySkillShenJin1_3:
        case enEnemySkill_120:
        case enEnemySkill_121:
        case enEnemySkill_270:
        case enEnemySkill_387:
        {
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getCategory()== enGeneralTp_God || m_player->getCardByIdx(i)->getSubCategory()== enGeneralTp_God)
                    )
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    
             //       m_player->fengYinAutoSkill(i);
                    if (enEnemySkillShenJin1_3==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
                    }
                    else if (enEnemySkill_120==pAtkInfo->iManSkillID || enEnemySkill_387==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%2 + 3);
                    }
                    else if (enEnemySkill_121==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                    else if (enEnemySkill_270==pAtkInfo->iManSkillID)
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                    else
                    {
                        m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                    }
                }
            }
        }
            break;
            
        case enEnemySkillLuanJin:        //14 随机封印2-4个武将,随机2-4回合:
        case enEnemySkillLuanJin40:
        case enEnemySkillLuanJin10:
        case enEnemySkillLuanJin3_23:
        case enEnemySkillLuanJinRun2_3_23:
        {
            int fengYinNum = 0;
            if (enEnemySkillLuanJin3_23==pAtkInfo->iManSkillID ||
                enEnemySkillLuanJinRun2_3_23==pAtkInfo->iManSkillID)
            {
                fengYinNum = 3;
            }
            else
            {
                fengYinNum = SaveData::Inst()->getrand()%3 + 2;
            }
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
           //     m_player->fengYinAutoSkill(result[i]);
                
                if (enEnemySkillLuanJin3_23==pAtkInfo->iManSkillID ||
                    enEnemySkillLuanJinRun2_3_23==pAtkInfo->iManSkillID)
                {
                     m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%2+2);
                }
                else
                {
                    m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                }
               
            }
        }
            break;
            
        case enEnemySkillZhuQueQi:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
          //      m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
            }
        }
            break;
            
        case enEnemySkill_211:
           
        {
            int fengYinNum = 1;
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                
                    m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,10);
                    
            }
            break;
        }
            case enEnemySkill_501:
        {
            int fengYinNum = 1;
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL && (m_player->getCardByIdx(i)->getElement() == enElementLight || m_player->getCardByIdx(i)->getSubElement() == enElementLight))
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL && (m_player->getCardByIdx(i)->getElement() == enElementLight || m_player->getCardByIdx(i)->getSubElement() == enElementLight))
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
            }
 
        }
            break;
         case enEnemySkill_516:
        {
            int fengYinNum = 1;
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL && (m_player->getCardByIdx(i)->getElement() == enElementDark || m_player->getCardByIdx(i)->getSubElement() == enElementDark))
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL && (m_player->getCardByIdx(i)->getElement() == enElementDark || m_player->getCardByIdx(i)->getSubElement() == enElementDark))
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
            }
        }
            break;
        case enEnemySkill_260:
            case enEnemySkill_323:
        {
            int fengYinNum = 1;
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //      m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,5);
            }
            break;
        }
        case enEnemySkill_263:
        {
            int fengYinNum = 3;
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //      m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,3);
            }
            break;
        }
        case enEnemySkill_233:
            case enEnemySkill_291://lvgenquan
            case enEnemySkill_310:
        {
            int fengYinNum = 4;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //      m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%2 + 1);
            }
            break;
        }
        case enEnemySkill_243:
        {
            int fengYinNum = 1;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //      m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,5);
            }
            break;
        }
        case enEnemySkillZhuQueQi3:
        case enEnemySkill_440:
        case enEnemySkill_441:
        case enEnemySkill_442:
        {
            int fengYinNum = 1;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
        //        m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,5);
            }
        }
            break;
        case enEnemySkill_363:
        {
            int fengYinNum = 1;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,3);
            }
        }
            break;
        case enEnemySkill_374:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,2);
            }
        }
            break;
        case enEnemySkill_379:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,3);
            }
        }
            break;
        case enEnemySkill_395:
        {
            int fengYinNum = 3;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
            }
        }
            break;
        case enEnemySkill_401:
        case enEnemySkill_406:
        {
            int fengYinNum = 1;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,5);
            }
        }
            break;
        case enEnemySkill_436:
            case enEnemySkill_526:
        {
            int fengYinNum = 6;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                if (enEnemySkill_526 == pAtkInfo->iManSkillID) {
                   m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 3);
                }else
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 1);
            }
        }
            break;
        case enEnemySkill_452:
        {
            int fengYinNum = 3;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,3);
            }
        }
            break;
        case enEnemySkill_478:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
            }
        }
            break;
        case enEnemySkill_148:
        case enEnemySkill_317:
        {
            int fengYinNum = 1;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //        m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,10);
            }
        }
            break;
            
        case enEnemySkill_160:
        {
            int fengYinNum = 4;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //          m_player->fengYinAutoSkill(result[i]);
                
                int i_cout = SaveData::Inst()->getrand()%2 + 1;
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,i_cout);
            }
        }
            break;
          
        case enEnemySkill_155:
        {
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL
                    && (m_player->getCardByIdx(i)->getCategory()== enGeneralTp_GuoShi || m_player->getCardByIdx(i)->getSubCategory()== enGeneralTp_GuoShi)
                    )
                {
                    shoot(pAtkInfo->srcPt, m_player->getCardByIdx(i)->getCardPos());
                    m_player->getCardByIdx(i)->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
                }
            }
        }
            break;
            
        case enEnemySkill_152:
        {
            int fengYinNum = 6;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //          m_player->fengYinAutoSkill(result[i]);
                
                int i_cout = SaveData::Inst()->getrand()%3 + 2;
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,i_cout);
            }
        }
            break;

        case enEnemySkillBingFengShiKe:
        {
            int fengYinNum = 5;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
         //       m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,SaveData::Inst()->getrand()%3 + 2);
            }
        }
            break;
            
        case enEnemySkill_113:
        case enEnemySkill_114:
            case enEnemySkill_505:
        {
            int fengYinNum = 0;
            
            switch (pAtkInfo->iManSkillID)
            {
                case enEnemySkill_113:
                    fengYinNum = 5;
                    break;
                    
                case enEnemySkill_114:
                    case enEnemySkill_505:
                    fengYinNum = 3;
                    break;
                    
                default:
                    fengYinNum = 5;
                    break;
            }
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
      //          m_player->fengYinAutoSkill(result[i]);
                
                int i_cout = SaveData::Inst()->getrand()%2 + 4;
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,i_cout);
            }
        }
            break;
            
        case enEnemySkillBingFengShiKe2:
        {
            int fengYinNum = 5;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
        //        m_player->fengYinAutoSkill(result[i]);
                int fengyCd = SaveData::Inst()->getrand()%2 + 1;
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,fengyCd);
            }
        }
            break;
            
        case enEnemySkillZhuQueQi2:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
        //        m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,5);
            }
        }
            break;
        case enEnemySkill_339://lvgenquan
        {
            int fengYinNum = 6;
            int result[fengYinNum];
            for (int i = 0; i < fengYinNum; i++) {
                result[i] = i;
            }
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                int fengyCd = SaveData::Inst()->getrand()%5 + 2;
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,fengyCd);
            }
        

            break;
        }
        case enEnemySkill_324:
            case enEnemySkill_504:
        {
            int fengYinNum = 2;
            
            int result[fengYinNum];
            for (int i=0; i<fengYinNum; i++)
            {
                result[i] = i;
            }
            
            int cardNum = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardNum++;
                }
            }
            
            int cardPos[cardNum];
            int cout = 0;
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (m_player->getCardByIdx(i) != NULL)
                {
                    cardPos[cout++] = i;
                }
            }
            
            fengYinNum = fengYinNum<cardNum?fengYinNum:cardNum;
            
            for (int i=0; i<fengYinNum ; i++)
            {
                int idx = SaveData::Inst()->getrand()%cardNum;
                result[i] = cardPos[idx];
                
                for (int j=idx; j<cardNum; j++)
                {
                    cardPos[j] = cardPos[j+1];
                }
                
                cardNum--;
            }
            
            for (int i=0; i<fengYinNum ; i++)
            {
                shoot(pAtkInfo->srcPt, m_player->getCardByIdx(result[i])->getCardPos());
                //        m_player->fengYinAutoSkill(result[i]);
                m_player->getCardByIdx(result[i])->setCardState(EN_CARD_STATE_FNEGYIN,4);
            }
        }
            break;
  
        case enEnemySkillZhiYu:          //15 当血量低于30%,100%回复自己50%的血量
        case enEnemySkillFuHuo:          //16 当血量低于30%，100%回复自己100%的血量
        case enEnemySkillFuHuo2:
        case enEnemySkill_279:
        case enEnemySkill_282:
        case enEnemySkill_475:
        case enEnemySkill_476:
            break;
        case enEnemySkillZhenFuHuo:
        {
            for (int j=0; j < m_enemyGrp->getEnemyCount(); j++)
            {
                if (!m_enemyGrp->getEnemyByIdx(j)->isAlive())
                {
                    m_enemyGrp->getEnemyByIdx(j)->reLive();
                }
            }
        }
            break;
            
        case enEnemySkillkengdiezhuanshui:
        case enEnemySkillkengdiezhuanshui2:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementWood,enElementDark);
            break;
            
        case enEnemySkillkengdiezhuanhuo:  //87  HP低于75%，25%几率发动 屏幕內所有水宝珠转光宝珠
        case enEnemySkillkengdiezhuanhuo2:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementWater,enElementLight);
            break;
        
        case enEnemySkillkengdiezhuanmu:   //87  HP低于75%，25%几率发动 屏幕內所有火宝珠转木宝珠
        case enEnemySkillkengdiezhuanmu2:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementFire,enElementWood);
            break;
            
        case enEnemySkillkengdiezhuanguang://87  HP低于75%，25%几率发动 屏幕內所有光宝珠转火宝珠
        case enEnemySkillkengdiezhuanguang2:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementLight,enElementFire);
            break;
            
        case enEnemySkillkengdiezhuanan:   //87  HP低于75%，25%几率发动 屏幕內所有暗宝珠转水宝珠
        case enEnemySkillkengdiezhuanan2:
        case enEnemySkill_135:
        case enEnemySkill_255:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementDark,enElementWater);
            break;
            
        case enEnemySkill_134:
        case enEnemySkill_259:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementFire,enElementDark);
            break;
            
        case enEnemySkill_136:
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementWater,enElementWood);
            break;
        case enEnemySkill_139:
        {
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementFire,enElementWood);
            
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
            break;
        }
        case enEnemySkill_234:
        {
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementDark,enElementLight);
            break;
        }
            case enEnemySkill_391://暗宝珠变成光宝珠，并造成伤害
        {
             m_player->getStoneGrid()->chgStoneFromEnemy(enElementDark,enElementLight);
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
        }
            break;
            case enEnemySkill_511:
        {
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementLight, enElementInvalid);
            pAtkInfo->iAttackPt = 20000;
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
            break;
        }
        case enEnemySkill_140:
        {
            m_player->getStoneGrid()->chgStoneFromEnemy(SaveData::Inst()->getrand()%6,enElementLight);
            
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
            
            break;
        }
        case enEnemySkill_150:
            case enEnemySkill_507:
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(SaveData::Inst()->getrand()%6,enElementInvalid);
            
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
            break;
        }
        case enEnemySkill_162:
        {
            m_player->getStoneGrid()->StoneShowBlack();
            break;
        }
        case enEnemySkill_167:
        {
            if (countAwakeSkill25())break;
            int nSize = enElementMax-2;
            for (int i = 0, nTemp = -1; i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (nTemp!=nRand)
                {
                    if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                    {
                        m_player->getStoneGrid()->chgStoneFromEnemy(nRand,nSize);
                        if (-1!=nTemp)break;    //只用两次
                        nTemp = nRand;
                    }
                }
            }
            break;
        }
        case enEnemySkill_196:
        case enEnemySkill_265:
        case enEnemySkill_281: //随机一种宝珠变成无效块，并造成伤害
        case enEnemySkill_404:
            case enEnemySkill_416:
            case enEnemySkill_437:
            case enEnemySkill_465:
            case enEnemySkill_503:
        {
            if (countAwakeSkill25())break;
            int nSize = enElementMax-2;
            int i = 0;
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,nSize);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }

            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementInvalid);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,nSize);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            break;
        }
            
            
        case enEnemySkillNiZhuanHuo:     //17 将火宝珠转换成无效块
        case enEnemySkill_278:
        case enEnemySkill_288: //lvgenquan
            case enEnemySkill_313:
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementFire,enElementInvalid);
            if (enEnemySkill_288 == pAtkInfo->iManSkillID || enEnemySkill_313 == pAtkInfo->iManSkillID) {
                shoot(pAtkInfo->srcPt, m_player->getPosition());
                m_player->beAttacked(pAtkInfo);
            }
        }
            break;
            
        case enEnemySkillNiZhuanShui:    //18 将水宝珠转换成无效块
            case enEnemySkill_289: //lvgenquan
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementWater,enElementInvalid);
            if (enEnemySkill_289 == pAtkInfo->iManSkillID) {
                shoot(pAtkInfo->srcPt, m_player->getPosition());
                m_player->beAttacked(pAtkInfo);
            }
        }
            break;
            
        case enEnemySkillNiZhuanMu:      //19 将木宝珠转换成无效块
            case enEnemySkill_290: //lvgenquan
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementWood,enElementInvalid);
            if (enEnemySkill_290 == pAtkInfo->iManSkillID) {
                shoot(pAtkInfo->srcPt, m_player->getPosition());
                m_player->beAttacked(pAtkInfo);
            }
        }
            break;
            
        case enEnemySkillNiZhuanGuang:   //20 将光宝珠转换成无效块
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementLight,enElementInvalid);
        }
            break;
            
        case enEnemySkillNiZhuanAn:      //21 将暗宝珠转换成无效块
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementDark,enElementInvalid);
        }
            break;
            
        case enEnemySkillNiZhuanHuiFu:   //22 将回复宝珠转换成无效块
        {
            if (countAwakeSkill25())break;
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementHeart,enElementInvalid);
            break;
        }
        case enEnemySkill_522:
        {
            m_player->getStoneGrid()->stoneChangeToLight();
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
        }
            break;
            case enEnemySkill_525:
        {
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementLight, enElementInvalid);
            m_player->getStoneGrid()->chgStoneFromEnemy(enElementDark, enElementInvalid);
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
        }
            break;
        case enEnemySkillBaoZhuYinCang:  //所有珠子变黑
        case enEnemySkill_333: //lvgenquan
            case enEnemySkill_304:
            case enEnemySkill_308:
            case enEnemySkill_378:
            case enEnemySkill_459:
            case enEnemySkill_411:
            case enEnemySkill_417:
            case enEnemySkill_443:
            case enEnemySkill_500:
            case enEnemySkill_517:
        {
//            m_player->getStoneGrid()->StoneShowBlack();
//            break;
            unsigned short ushSkillCount = m_player->getNumByAwakeId(enAwakeSkill_24);
            unsigned short awakeskill_31count = m_player->getNumByAwakeId(enAwakeSkill_31);
            if (0<ushSkillCount || awakeskill_31count > 0)
            {
                if (awakeskill_31count > 0) {
                    break;
                }
                if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_24))
                {
                    int nOdds = ushSkillCount;
                    nOdds *= pAwakeSkill->cOdds;
//                    if (100<=nOdds)
//                    {
//                        nOdds = StoneGrid::getOddsAwake10(((float)pAwakeSkill->cOdds)/100.0f,ushSkillCount);
//                    }
                    if (SaveData::Inst()->getrand()<nOdds)
                    {
                        break;
                    }
                    else
                    {
                        m_player->getStoneGrid()->StoneShowBlack();
                    }
                }
            }
            else
            {
                m_player->getStoneGrid()->StoneShowBlack();
            }
            break;
        }
        case enEnemySkillShiguangnizhuan:
            m_player->resetSkillCD();
            break;
            
        case enEnemySkillNiZhuanTianXia: //23 随机将一种宝珠转化成无效块
            case enEnemySkill_293: //lvgenquan
            case enEnemySkill_295: //lvgenquan
            case enEnemySkill_346:
            case enEnemySkill_373:
            case enEnemySkill_362:
            case enEnemySkill_381:
        {
            if (countAwakeSkill25())break;
            int nSize = enElementMax-2;
            int i = 0;
            for (; i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                   
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementInvalid);
                    if (enEnemySkill_295 == pAtkInfo->iManSkillID
                        || enEnemySkill_373 == pAtkInfo->iManSkillID
                        || enEnemySkill_362 == pAtkInfo->iManSkillID
                        || enEnemySkill_381 == pAtkInfo->iManSkillID) {
                        shoot(pAtkInfo->srcPt, m_player->getPosition());
                        m_player->beAttacked(pAtkInfo);
                    }
                    break;
                }
            }
            
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementInvalid);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementInvalid);
                    if (enEnemySkill_295 == pAtkInfo->iManSkillID
                        || enEnemySkill_373 == pAtkInfo->iManSkillID
                        || enEnemySkill_362 == pAtkInfo->iManSkillID
                        || enEnemySkill_381 == pAtkInfo->iManSkillID) {
                        shoot(pAtkInfo->srcPt, m_player->getPosition());
                        m_player->beAttacked(pAtkInfo);
                    }
                }
            }
            
            
        }
              break;
        
        case enEnemySkill_117:
        {
            int i_cout = 0;
            int xx = 0;
            while (i_cout <= 2 && xx < 100)
            {
                int nsize = enElementMax-2;
                int nRand = SaveData::Inst()->getrand()%nsize;
                if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->addStoneSpecial(nRand,enSpecialStoneBomb);
                    i_cout++;
                }
                
                xx++;
            }
        }
            break;
            
        case enEnemySkillNiZhuanTianXia2: //随机两种宝珠，变成无效块
            case enEnemySkill_322:
            case enEnemySkill_400:
        {
            if (countAwakeSkill25())break;
            int nSize = enElementMax-2;
            for (int i = 0, nTemp = -1; i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (nTemp!=nRand)
                {
                    if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                    {
                        m_player->getStoneGrid()->chgStoneFromEnemy(nRand,nSize);
                        if (-1!=nTemp)break;    //只用两次
                        nTemp = nRand;
                    }
                }
            }
            break;
        }
        case enEnemySkill_203:      //随机一种宝珠变成水宝珠，并造成伤害
            case enEnemySkill_301:
            case enEnemySkill_326:
        {
            int i = 0;
            int nSize = enElementMax-2;
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (enElementWater!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementWater);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementWater);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementWater);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            
            break;
        }
        case enEnemySkill_207:   //随机一种宝珠变成光宝珠，并造成伤害
        case enEnemySkill_492:
            case enEnemySkill_494:
        {
            int nSize = enElementMax-2;
            int i = 0;
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (enElementLight!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                   
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementLight);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }

            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementLight);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementLight);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            break;
        }
        case enEnemySkill_342:  // 随机一种宝珠变成水宝珠，并造成伤害
            case enEnemySkill_414:
            case enEnemySkill_419:
            case enEnemySkill_484:
            case enEnemySkill_486:
        {
            
            int nSize = enElementMax-2;
            int i = 0;
            for (; i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementWater);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementWater);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementWater);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);

                }
            }
            
            break;
        }
            case enEnemySkill_343:
            case enEnemySkill_502:
          
        {
            
            if (countAwakeSkill25())break;
           
            m_player->getStoneGrid()->chgStoneNumberFromEnemy(6, enElementInvalid);
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);

            
            break;
        }
            case enEnemySkill_531:
        {
            if (countAwakeSkill25())break;
            
            m_player->getStoneGrid()->chgStoneNumberFromEnemy(12, enElementInvalid);
            
        }
            break;
        case enEnemySkill_348:{
            
            m_player->getStoneGrid()->chgStoneNumberFromEnemy(6, enElementMax,true);
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
            break;
        }
        case enEnemySkill_349:  //随机一种宝珠变成暗宝珠，并造成伤害
            case enEnemySkill_496:
            case enEnemySkill_498:
        {
            int nSize = enElementMax-2;
            int i = 0;
            
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (enElementDark!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementDark);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementDark);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementDark);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            break;
        }
        case enEnemySkill_356:  //随机一种宝珠变成木宝珠，并造成伤害
            case enEnemySkill_422:
            case enEnemySkill_449:
            case enEnemySkill_453:
            case enEnemySkill_488:
            case enEnemySkill_490:
        {
            int nSize = enElementMax-2;
            int i = 0;
            
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (enElementWood!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementWood);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementWood);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementWood);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            break;
        }
        case enEnemySkill_402:  //随机一种宝珠变成火宝珠，并造成伤害
            case enEnemySkill_405:
            case enEnemySkill_480:
            case enEnemySkill_482:
        {
            int nSize = enElementMax-2;
            int i = 0;
            
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (enElementFire!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,enElementFire);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(enElementFire);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,enElementFire);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            }
            break;
        }
        case enEnemySkill_434:{ //随机一种宝珠变成随机宝珠，并造成伤害
            
            int nSize = enElementMax-2;
            int i = 0;
            int nRand_dst = SaveData::Inst()->getrand()%nSize;
            for (;  i < 100; ++i)
            {
                int nRand = SaveData::Inst()->getrand()%nSize;
                if (nRand_dst!=nRand && m_player->getStoneGrid()->haveStone(nRand,nRand))
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nRand,nRand_dst);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                    break;
                }
            }
            if (100<=i)
            {
                int nElement = m_player->getStoneGrid()->getOneBeElement(nRand_dst);
                if (-1!=nElement)
                {
                    m_player->getStoneGrid()->chgStoneFromEnemy(nElement,nRand_dst);
                    shoot(pAtkInfo->srcPt, m_player->getPosition());
                    m_player->beAttacked(pAtkInfo);
                }
            } 
            
            break;
        }
        case enEnemySkillWuShengHuYou:
        case enEnemySkillShenLinBiYou:
        case enEnemySkillXuShiDaiFa:
        case enEnemySkillJiNu:
        case enEnemySkillKuangBao:
        case enEnemySkillGongJiZiTai:
        case enEnemySkillBaoNu:
        case enEnemySkillShaShouJian:
        case enEnemySkillWoHenYing:
        case enEnemySkillTaGengYing:
        case enEnemySkillHaoYin:
        case enEnemySkillHaoYin2:
        case enEnemySkillTu:
        case enEnemySkillshuohua20:
        case enEnemySkillMuTengJia:
        case enEnemySkillxinjinganghuti:
        case enEnemySkillJiNu2:
        case enEnemySkillBaoZou:
        case enEnemySkillMuTengJia2:
        case enEnemySkillXingYunYiJi:
        case enEnemySkillShiJianNingJie:
        case enEnemySkillMuTengJia3:
        case enEnemySkillxinjinganghuti2:
        case enEnemySkillQuYan:
        case enEnemySkillDuanLiu:
        case enEnemySkillPoMu:
        case enEnemySkillZhanLei:
        case enEnemySkillShiHun:
        case enEnemySkillJiNu3:
        case enEnemySkill_118:
        case enEnemySkill_122:
        case enEnemySkill_133:
        case enEnemySkill_137:
        case enEnemySkill_142:
        case enEnemySkill_146:
        case enEnemySkill_149:
        case enEnemySkill_145:
        case enEnemySkill_123:
        case enEnemySkill_124:
        case enEnemySkill_157:
        case enEnemySkill_159:
        case enEnemySkill_171:
        case enEnemySkill_172:
        case enEnemySkill_173:
        case enEnemySkill_174:
        case enEnemySkill_175:
        case enEnemySkill_176:
        case enEnemySkill_177:
        case enEnemySkill_178:
        case enEnemySkill_179:
        case enEnemySkill_180:
        case enEnemySkill_181:
        case enEnemySkill_182:
        case enEnemySkill_183:
        case enEnemySkill_184:
        case enEnemySkill_185:
        case enEnemySkill_186:
        case enEnemySkill_187:
        case enEnemySkill_188:
        case enEnemySkill_189:
        case enEnemySkill_190:
        case enEnemySkill_201:
        case enEnemySkill_202:
        case enEnemySkill_206:
        case enEnemySkill_209:
        case enEnemySkill_217:
        case enEnemySkill_224:
        case enEnemySkill_226:
        case enEnemySkill_230:
        case enEnemySkill_232:
        case enEnemySkill_235:
        case enEnemySkill_236:
        case enEnemySkill_238:
        case enEnemySkill_240:
        case enEnemySkill_247:
        case enEnemySkill_249:
        case enEnemySkill_250:
        case enEnemySkill_252:
        case enEnemySkill_256:
        case enEnemySkill_266:
        case enEnemySkill_272:
        case enEnemySkill_274:
        case enEnemySkill_244:
        case enEnemySkill_165:
        case enEnemySkill_287: //lvgenquan
        case enEnemySkill_294: //lvgenquan
        case enEnemySkill_340: //lvgenquan
        case enEnemySkill_344:
        case enEnemySkill_297:
        case enEnemySkill_303:
        case enEnemySkill_306:
        case enEnemySkill_311:
        case enEnemySkill_325:
        case enEnemySkill_314:
        case enEnemySkill_316:
        case enEnemySkill_321:
        case enEnemySkill_355:
        case enEnemySkill_365:
        case enEnemySkill_376:
        case enEnemySkill_408:
        case enEnemySkill_409:
        case enEnemySkill_412:
        case enEnemySkill_424:
        case enEnemySkill_425:
        case enEnemySkill_427:
        case enEnemySkill_428:
        case enEnemySkill_430:
        case enEnemySkill_438:
        case enEnemySkill_446:
        case enEnemySkill_447:
        case enEnemySkill_352:
        case enEnemySkill_368:
        case enEnemySkill_390:
        case enEnemySkill_435:
        case enEnemySkill_456:
        case enEnemySkill_460:
        case enEnemySkill_463:
        case enEnemySkill_464:
        case enEnemySkill_479:
        case enEnemySkill_468:
        case enEnemySkill_477:
            case enEnemySkill_508:
            case enEnemySkill_510:
            case enEnemySkill_514:
            case enEnemySkill_515:
            case enEnemySkill_532:
            case enEnemySkill_533:
            case enEnemySkill_534:
            break;
            
        default:
        {
            if (pAtkInfo->iManSkillID == enEnemySkill_Null) {
                unsigned short awakeskill_30count =m_player-> getNumByAwakeId(enAwakeSkill_30);
                if (awakeskill_30count > 0) {
                    if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_30))
                    {
                        int nOdds = awakeskill_30count;
                        nOdds *= pAwakeSkill->cOdds;
                        if (SaveData::Inst()->getrand() < nOdds) {
                            pAtkInfo->iAttackPt = 0;
                            m_player->setAnimal_awakeskill_30();
                            
                        }
                    }
                }

            }
            shoot(pAtkInfo->srcPt, m_player->getPosition());
            m_player->beAttacked(pAtkInfo);
        }
            break;
    }
}

void FightMngr::AtkPlayerDirect(CAttackInfo atkInfo)
{
    shoot(atkInfo.srcPt, m_player->getPosition());
    m_player->beDirAtk(&atkInfo);
    
    if(m_player->isDead() && CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
    {//死亡弹出框
        CGameState::Instance()->setState(enStateGameOver);
        
        if (m_enemyGrp->isBuffEffect(enEnemyBuff_shenfa))
        {
            m_tips->show();
        }
        else
        {
            schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
        }
    }
}

//******************************************************************************
// shoot
//******************************************************************************
void FightMngr::shoot(const CCPoint& src, const CCPoint& dst)
{
    CCSprite* spBult = CCSprite::spriteWithSpriteFrameName("bullet.png");
    spBult->setPosition(src);
    addChild(spBult, 2);
    
    CCPoint bezierPos1 = dst;
    CCPoint bezierPos2 = dst;
    bezierPos1.x = (src.x + dst.x) * 0.5;
    bezierPos2.x = (src.x + dst.x) * 0.5;
    ccBezierConfig bezier;
    bezier.controlPoint_1 = bezierPos1;
    bezier.controlPoint_2 = bezierPos2;
    bezier.endPosition = dst;
    spBult->runAction(CCSequence::actions(CCBezierTo::actionWithDuration(0.2, bezier),
                                          CCFadeOut::actionWithDuration(0.2),
                                          NULL) );
    
    spBult->setTag(kTagToBeDel);
}

//******************************************************************************
// cbStartFight
//******************************************************************************
void FightMngr::cbStartFight()
{
    if(m_player == NULL){
        m_player = new Player(this,callfuncO_selector(FightMngr::cbPlayer));
        addChild(m_player);
        m_player->release();
    }
    
    if (SaveData::Inst()->getIsUseSaveData() == false
        && SaveData::Inst()->getDgSkillType() == 1)
    {   // 不是s&l的时候
        m_player->setCardsNoCd();
    }
    
    m_iMaxStep = SaveData::Inst()->getStepCountMax();
    m_iCurStep = SaveData::Inst()->getCurStep();
    m_iDropItemCnt = SaveData::Inst()->getCurItemCnt();
    m_iDropGoldTotal = SaveData::Inst()->getCurDgGold();
    
    bool isUseChristmasFightScene;
    short fightSceneChange;
    
    if (SaveData::Inst()->getIsUseSaveData())
    {
        m_isUseAwakeSkill29 = false;
        m_iSL_type = SaveData::Inst()->getiMoment();
        startStepFromSaveData();
        isUseChristmasFightScene = SaveData::Inst()->getIsUseChristmasFightScene();
        fightSceneChange = SaveData::Inst()->getFightSceneChange();
        
        if (m_iSL_type == 1)
        {
            CGameState::Instance()->setState(enStateTouchEnd);
            m_player->getStoneGrid()->actAfterMove(0.8f);
        }
        
        m_iSL_type = 0;
    }
    else
    {
        startStep();
        isUseChristmasFightScene = CGameData::Inst()->getCommonInfo()->bChrisBg;
        fightSceneChange = CGameData::Inst()->getCommonInfo()->bg_change;
    }
    
    char buf[100];
    
    if(isUseChristmasFightScene){
        snprintf(buf, 99, "cj0017.jpg");
    }
    else{
        switch (fightSceneChange) {
            case 0:
                sprintf(buf, "cj00%d%d.jpg", rand() % 2, fightSceneType);
                break;
                
            case 2:
                sprintf(buf, "cj001%d.jpg",fightSceneType);
                break;
            
            case 1:
            default:
                sprintf(buf, "cj000%d.jpg",fightSceneType);
                break;
        }
    }
    
    if(m_spFightScene == NULL){
        m_spFightScene = CCSprite::spriteWithFile(buf);
        m_spFightScene->setPosition(CCPointMake(320, 761));
        m_spFightScene->setScale(1/0.79);
        addChild(m_spFightScene, -20);
        m_spFightScene->setOpacity(0);
        m_spFightScene->runAction(CCFadeIn::actionWithDuration(1.0));
    }
    

    m_spGold->setPosition(CCPointMake(34, 960));
    m_spBox->setPosition(CCPointMake(475, 960));
    m_lbGold->setPosition(CCPointMake(128, 945));
    m_lbBox->setPosition(CCPointMake(500, 960));
    m_menu->setPosition(CCPointMake(544, 930));
    if (OcProxy::Inst()->getBoolValueForKey("caidan"))
        m_btnNormal->setPosition(CCPointMake(564, 819));
    else
        m_btnCaiDan->setPosition(CCPointMake(564, 819));

    if (m_enemyGrp) {
        updateInfBar(m_iDropGoldTotal + m_enemyGrp->collectGold(), m_iDropItemCnt);
    }
    else {
        updateInfBar(m_iDropGoldTotal, m_iDropItemCnt);
    }
    
    if (RecordHelper::Inst()->CheckRecording() && !SaveData::Inst()->hasValidSavedData())
    {
        schedule(schedule_selector(FightMngr::startRecordScreen), 1.0);
    }
}

int FightMngr::getCurStep()
{
    return m_iCurStep;
}

void FightMngr::StopRecordScreen(bool saveFile)
{
    if (!RecordHelper::Inst()->CheckRecording())
        return;
    
    if (saveFile)
        OcProxy::Inst()->stopRecordScreen();
    else
        OcProxy::Inst()->cancelRecordScreen();
    

    RecordHelper::Inst()->SetPauseRecord(false);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this,"CHECK_RECORDSCREEN_INTERRUPT");
}

void FightMngr::startRecordScreen(ccTime dt)
{
    unschedule(schedule_selector(FightMngr::startRecordScreen));
    if (RecordHelper::Inst()->CheckRecording())
    {
        OcProxy::Inst()->startRecordScreen();
        
        CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                               callfuncO_selector(FightMngr::CheckInterrupt),  // 处理的消息的回调函数
                                                               "CHECK_RECORDSCREEN_INTERRUPT",                    // 感兴趣的消息名称
                                                            NULL);
     
      //  m_player->StartUpdate();
    }
}

void FightMngr::CheckInterrupt(CCObject* pSender)
{
    StopRecordScreen(false);
}



//******************************************************************************
// newEnemyGrp
//******************************************************************************
void FightMngr::newEnemyGrp()
{
    if (SaveData::Inst()->hasValidSavedData()) {
        SaveData_EnemyGrp* pStepInfo = SaveData::Inst()->getEnemyGroupData(m_iCurStep);
        
        m_enemyGrp = new EnemyGrp(pStepInfo, this, callfuncO_selector(FightMngr::cbEnemyGrp));
        addChild(m_enemyGrp, -10);
        m_enemyGrp->release();
    }
    else {
        CCMutableArray<CDgStepInfo*> * pStepInfoArr = CGameData::Inst()->getDgStepInfo();
        CDgStepInfo* pStepInfo = pStepInfoArr->getObjectAtIndex(m_iCurStep);
        
        m_enemyGrp = new EnemyGrp(pStepInfo, this, callfuncO_selector(FightMngr::cbEnemyGrp));
        addChild(m_enemyGrp, -10);
        m_enemyGrp->release();
    }
}


//******************************************************************************
// startStep
//******************************************************************************
void FightMngr::startStep()
{
    //delete unused sprite
    removeGabage();
    
    SaveData::Inst()->setCurStep(m_iCurStep);
    
    if (m_iCurStep <= 5 && m_iCurStep > 0
        && SaveData::Inst()->getDgSkillType() == enSpecialDgSkillType_quanduiwuxianding)
    {
        m_player->changeTeamInfo();
        m_isUseAwakeSkill29 = true;
    }
    
    if(m_iCurStep + 1 == m_iMaxStep){   //boss
        CCSprite* spBossComing = CCSprite::spriteWithFile("bosscoming.png");
        if(spBossComing){
            addChild(spBossComing);
            spBossComing->setPosition(CCPointMake(320, 810));
            spBossComing->setScale(0.0);
            spBossComing->runAction(CCSequence::actions(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.4, 1.0), 0.8f),
                                                        CCDelayTime::actionWithDuration(0.8f),
                                                        CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::cbStartStep)),
                                                        CCFadeOut::actionWithDuration(0.5),
                                                        NULL));
        }

        if (SaveData::Inst()->getDgSkillType() == 1) 
            MainScene::Inst()->switchBgMusic(enBgMusicOrgFight);
        else
            MainScene::Inst()->switchBgMusic(enBgMusicBoss);
    }
    else{
        char buf[100];
        sprintf(buf, "%d/%d", m_iCurStep+1, m_iMaxStep);
        m_lbShowStep->setString(buf);
        m_spShowStep->setPosition(CCPointMake(-160, 838));
        m_spShowStep->setOpacity(0);
        
        if (m_iCurStep == 0) {
            m_spShowStep->runAction(CCSequence::actions(CCSpawn::actions(CCFadeIn::actionWithDuration(0.2),
                                                                         CCMoveTo::actionWithDuration(0.2, CCPointMake(320, 838)),
                                                                         NULL),
                                                        CCDelayTime::actionWithDuration(0.6),
                                                        CCSpawn::actions(CCFadeOut::actionWithDuration(0.2),
                                                                         CCMoveTo::actionWithDuration(0.2, CCPointMake(800, 838)),
                                                                         NULL),
                                                        CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::cbStartStep)),
                                                        NULL));
        }
        else {
            m_spShowStep->runAction(CCSequence::actions(CCSpawn::actions(CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::hideFightScene)),
                                                                         CCDelayTime::actionWithDuration(0.8f),
                                                                         NULL),
                                                        CCSpawn::actions(CCFadeIn::actionWithDuration(0.2),
                                                                         CCMoveTo::actionWithDuration(0.2, CCPointMake(320, 838)),
                                                                         CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::showFightScene)),
                                                                         NULL),
                                                        CCDelayTime::actionWithDuration(0.6),
                                                        CCSpawn::actions(CCFadeOut::actionWithDuration(0.2),
                                                                         CCMoveTo::actionWithDuration(0.2, CCPointMake(800, 838)),
                                                                         NULL),
                                                        CCCallFunc::actionWithTarget(this, callfunc_selector(FightMngr::cbStartStep)),
                                                        NULL));
        }
        
    }
    
    //初始化新手引导的宝石
    if(CGameData::Inst()->isNewbie()){
        m_player->initMatrix(m_iCurStep+1);
    }
    else {
        if (SaveData::Inst()->getIsUseSaveData()) {
            m_player->initMatrix(0);
        }
        else {
            if(m_iCurStep == 0)
                m_player->initMatrix(0);
        }
    }
    
    if(m_enemyGrp != NULL){
        removeChild(m_enemyGrp, true);
        
        m_enemyGrp = NULL;
    }
}

//******************************************************************************
// startStepFromSaveData
//******************************************************************************
void FightMngr::startStepFromSaveData()
{
    m_player->initMatrix(0);
    cbStartStep();
    
    if(m_enemyGrp->isAllEnemyDead())
    {
        CGameState::Instance()->setState(enStateEnemyDead);
        m_ret.bClear = true;
        
        if(m_iCurStep < m_iMaxStep - 1)
        {
            m_iCurStep++;
            startStep();
        }
        else
        {
             showEnd();
        }
       
    }
    else if(m_player->isDead())
    {//死亡弹出框
        if ( CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
        {
            CGameState::Instance()->setState(enStateGameOver);
            
            if (m_enemyGrp->isBuffEffect(enEnemyBuff_shenfa))
            {
                m_tips->show();
            }
            else
            {
                schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
            }
        }
    }
    else
    {
        if (SaveData::Inst()->getDgSkillType() == 1)
            MainScene::Inst()->switchBgMusic(enBgMusicOrgFight);
        else
            MainScene::Inst()->switchBgMusic(enBgMusicBoss);
    }
}

//******************************************************************************
// cbNextStep
//******************************************************************************
void FightMngr::cbStartStep()
{
    newEnemyGrp();
    
    if (m_player->isDead() == false)
    {
        if (m_iSL_type != 1)
            m_player->startNewRound(true);
        else if (m_iSL_type == 1)
            m_player->startFromMove();
    }
    else
    {
        if (CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateEnemyDeadGameOver)
        {//死亡弹出框
            CGameState::Instance()->setState(enStateGameOver);
            schedule(schedule_selector(FightMngr::cbPlayerDeadDelay), 0.8);
        }
    }
    
    
    if (m_isUseAwakeSkill29) {
        m_isUseAwakeSkill29 = false;
        int awakeskillnum = m_player->getNumByAwakeId(enAwakeSkill_29);
        if (awakeskillnum > 0) {
            m_enemyGrp->changeEnemy_huihe(awakeskillnum);
        }
    }
    

    if (SaveData::Inst()->getIsUseSaveData()) {
        m_player->yanChiSkillCD(isEnemyBuffYanChi());
        if (m_enemyGrp->isBuffEffect(enEnemyBuff_canfei))
            schedule(schedule_selector(FightMngr::cbEnemyBuffCanFei), 1.5);
        else
            cbEnemyBuffCanFei(0);
        saveCurFightInfo();

    } else {
        setEnemyBuffEffect();
        
        saveCurFightInfo();
    }
    
    
   

    if ((m_iCurStep == 0) && (CGameData::Inst()->isNewbie() == false)){
        string strDgTp = CGameData::Inst()->getCurDgType();
        CDgRoomInfo* pRmInf = NULL;
        if (strDgTp.compare("normal") == 0) {
            CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
            
            int iFloorId = CGameData::Inst()->getCurFloor();
            CNmlDgInfo* pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(iFloorId -1);
            
            CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
            int iRoomId = CGameData::Inst()->getCurRoom();
            pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
        }
        else if (strDgTp.compare("special") == 0 || strDgTp.compare("weekly") == 0)
        {
            CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
            
            CSpecialDgInfo* pSpecialDgInfo = NULL;
            int iFloorId = CGameData::Inst()->getCurFloor();
            for (int i = 0; i < pSpecialDgInfoArr->count(); i++) {
                pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
                if (pSpecialDgInfo->iFloorId == iFloorId) {
                    break;
                }
            }
            if (pSpecialDgInfo) {
                CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
                int iRoomId = CGameData::Inst()->getCurRoom();
                pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
            }
        }
        else if (strDgTp.compare("sgyy") == 0)
        {
            
            CCMutableArray<CSangoYYDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
            
            int iFloorId = CGameData::Inst()->getCurFloor();
            CSangoYYDgInfo* pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(iFloorId -1);
            
            CCMutableArray<cSangoYYRoom*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
            int iRoomId = CGameData::Inst()->getCurRoom();
            pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
        }
        
        if ((pRmInf) && (pRmInf->m_pHeadTipsArr)) {
            FsGuide* pFsGd = FsGuide::node();
            addChild(pFsGd, 5000, kFsGuideTag);
            pFsGd->setGuideString(pRmInf->m_pHeadTipsArr,this, callfunc_selector(FightMngr::cbHeadTips));
        }
    }
    
   
}


//******************************************************************************
// cbHeadTips
//******************************************************************************
void FightMngr::cbHeadTips()
{
    removeChildByTag(kFsGuideTag, true);
}

//******************************************************************************
// fightClear
//******************************************************************************
void FightMngr::fightEnd()
{
    if(m_enemyGrp != NULL){
        removeChild(m_enemyGrp, true);
        
        m_enemyGrp = NULL;
    }
    
    if(m_player != NULL){
        removeChild(m_player, true);
        
        m_player = NULL;
    }
    
    
    m_spGold->setPosition(CCPointMake(-1000, -1000));
    m_spBox->setPosition(CCPointMake(-1000, -1000));
    m_lbGold->setPosition(CCPointMake(-1000, -1000));
    m_lbBox->setPosition(CCPointMake(-1000, -1000));
    m_menu->setPosition(CCPointMake(-1000, -1000));    
    m_btnNormal->setPosition(CCPointMake(-1000, -1000));
    m_btnCaiDan->setPosition(CCPointMake(-1000, -1000));

    
    if(m_spFightScene){
        removeChild(m_spFightScene, true);
        m_spFightScene = NULL;
    }
    
    //delete unused sprite
    removeGabage();
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}


//******************************************************************************
// cbCollect
//******************************************************************************
void FightMngr::cbCollect(ccTime dt)
{
    unschedule(schedule_selector(FightMngr::cbCollect));
    
    if(m_iCollectStep == 0){
        if(m_enemyGrp->isActionRunning()){
            schedule(schedule_selector(FightMngr::cbCollect), 0.2f);
            return;
        }
        
        int cardnum = m_enemyGrp->collectCard();
        
        if(m_enemyGrp->haveGoldBox())
            m_iDropItemCnt++;
        
        m_iDropItemCnt += cardnum;
        m_iDropGoldTotal += m_enemyGrp->collectGold();
        m_iCollectStep++;
        
        updateInfBar(m_iDropGoldTotal, m_iDropItemCnt);
        
        if((cardnum == 0) && (m_enemyGrp->haveGoldBox() == false)){
            updateInfBar(m_iDropGoldTotal, m_iDropItemCnt);
            
            if(m_iCurStep < m_iMaxStep - 1) {
                m_iCurStep++;
                startStep();
            }
            else{
                saveCurFightInfo();
                schedule(schedule_selector(FightMngr::showEnd), 1.0f);
            }
        }
        else {
            schedule(schedule_selector(FightMngr::cbCollect), 1.0f);
        }
    }
    else{
        if(m_iCurStep < m_iMaxStep - 1) {
            m_iCurStep++;
            startStep();
        }
        else {
            saveCurFightInfo();
            schedule(schedule_selector(FightMngr::showEnd), 1.0f);
        }
    }
}


//******************************************************************************
// menuCallback
//******************************************************************************
void FightMngr::updateInfBar(int gold, int item)
{
   // m_iDropGoldTotal= gold;
    char buf[20];
    snprintf(buf, 19, "%d", gold);
   // m_lbGold->setString(buf);
    
    m_lbGold->setNumber(gold, 0.4, 0.2);
    snprintf(buf, 19, "%d", item);
    m_lbBox->setString(buf);
}

void FightMngr::fmCallback(CCObject *sender)
{
    FightPauseMenuRet *iRet = (FightPauseMenuRet *)sender;

    switch (iRet->iBtnSel) {
        case 11:
    
            break;
        case 12:

            break;
        case 13:
            m_player->RecordControl(false, 0);
            
            pauseMenu->removeAllChildrenWithCleanup(true);
            pauseMenu->removeFromParentAndCleanup(true);
            pauseMenu = NULL;
            m_menu->setIsEnabled(true);
            m_btnNormal->setIsEnabled(true);
            m_btnCaiDan->setIsEnabled(true);

            break;
        case 21:

            m_ret.bClear = false;
            m_tips->show();
            StopRecordScreen(false);

            break;
        default:
            break;
    }
}
//******************************************************************************
// menuCallback
//******************************************************************************
void FightMngr::menuCallback(CCObject* sender)
{
    if(CGameState::Instance()->getState() == enStateWaitTouch)
    {
        char buf[50];
        sprintf(buf, "%s%d/%d",CGameData::Inst()->getLanguageValue("fightMngrTip1"),m_iCurStep+1,m_iMaxStep);
        pauseMenu = new FightPauseMenu(this,menu_selector(FightMngr::fmCallback));
        addChild(pauseMenu);
        pauseMenu->setTitleString(buf);
        m_menu->setIsEnabled(false);
        m_btnCaiDan->setIsEnabled(false);
        m_btnNormal->setIsEnabled(false);
        
        m_player->RecordControl(false, 2);
    }
}   


//******************************************************************************
// cbCaiDan
//******************************************************************************
void FightMngr::cbCaiDan(cocos2d::CCObject *sender)
{
    if (CGameState::Instance()->getState() != enStateWaitTouch)
        return;
    
    bool bCaiDan = OcProxy::Inst()->getBoolValueForKey("caidan");
    OcProxy::Inst()->setBoolValueForKey(!bCaiDan, "caidan");
    m_player->changeCaiDanStone();
    if (bCaiDan)
    {
        m_btnNormal->setPosition(CCPointMake(-1000, -1000));
        m_btnCaiDan->setPosition(CCPointMake(564, 819));
    }
    else
    {
        m_btnCaiDan->setPosition(CCPointMake(-1000, -1000));
        m_btnNormal->setPosition(CCPointMake(564, 819));
    }
}

//******************************************************************************
// cbTipsDlg
//******************************************************************************
void FightMngr::cbTipsDlg()
{
    unschedule(schedule_selector(FightMngr::showStarEffect));
    if (m_starEffect) {
        m_starEffect->removeFromParentAndCleanup(true);
        m_starEffect = NULL;
    }
   
    if(pauseMenu!=NULL)
    {
        pauseMenu->removeAllChildrenWithCleanup(true);
        pauseMenu->removeFromParentAndCleanup(true);
        pauseMenu= NULL;
        m_menu->setIsEnabled(true);
        m_btnNormal->setIsEnabled(true);
        m_btnCaiDan->setIsEnabled(true);
    }
    fightEnd();
}

//******************************************************************************
// showEnd
//******************************************************************************
void FightMngr::showEnd()
{
    unschedule(schedule_selector(FightMngr::showEnd));

    CDgRoomInfo* pRmInf = NULL;
    if(CGameData::Inst()->isNewbie() == false){
        string strDgTp = CGameData::Inst()->getCurDgType();
        
        if (strDgTp.compare("normal") == 0)
        {
            CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
            
            int iFloorId = CGameData::Inst()->getCurFloor();
            CNmlDgInfo* pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(iFloorId -1);
            
            CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
            int iRoomId = CGameData::Inst()->getCurRoom();
            pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
        }
        else if (strDgTp.compare("special") == 0 || strDgTp.compare("weekly") == 0)
        {
            CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
            
            CSpecialDgInfo* pSpecialDgInfo = NULL;
            int iFloorId = CGameData::Inst()->getCurFloor();
            for (int i = 0; i < pSpecialDgInfoArr->count(); i++) {
                pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
                if (pSpecialDgInfo->iFloorId == iFloorId) {
                    break;
                }
            }
            
            if (pSpecialDgInfo) {
                CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
                int iRoomId = CGameData::Inst()->getCurRoom();
                pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
            }
        }
        else if (strDgTp.compare("sgyy") == 0)
        {
            CCMutableArray<CSangoYYDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
            
            int iFloorId = CGameData::Inst()->getCurFloor();
            CSangoYYDgInfo* pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(iFloorId -1);
            
            CCMutableArray<cSangoYYRoom*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
            int iRoomId = CGameData::Inst()->getCurRoom();
            pRmInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
        }
    }

    if ((pRmInf) && (pRmInf->m_pEndTipsArr))
    {
        FsGuide* pFsGd = FsGuide::node();
        addChild(pFsGd, 5000, kFsGuideTag);
        pFsGd->setGuideString(pRmInf->m_pEndTipsArr,this, callfunc_selector(FightMngr::cbEndTips));
    }
    else
    {
        if (m_player->isDead() == false) {
            showEndAction(true);
            m_tips->show();
        }
    }
    RecordHelper::Inst()->EndFighting();
    StopRecordScreen(true);
}

//******************************************************************************
// cbEndTips
//******************************************************************************
void FightMngr::cbEndTips()
{
    removeChildByTag(kFsGuideTag, true);
    
    showEndAction(true);
    m_tips->show();
}

//******************************************************************************
// showEndAction
//******************************************************************************
void FightMngr::showEndAction(bool bClear)
{
    CCSprite* sp;
    CCPoint pt(320, 0);
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0);

    if(bClear){
        if(CGameData::Inst()->isSoundOn) {
            if (SaveData::Inst()->getDgSkillType() == 1)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(QianLiWin));
            else
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(success));
        }
        
        sp = CCSprite::spriteWithFile("success.png");
        pt.y = 760;
        
        schedule(schedule_selector(FightMngr::showStarEffect), 0.5f);
    }
    else{
        if(CGameData::Inst()->isSoundOn) {
            if (SaveData::Inst()->getDgSkillType() == 1)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(QianLiFail));
            else
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(fail));
        }
        
        sp = CCSprite::spriteWithFile("fail.png");
        pt.y = 700;
    }
    
    
    if(sp){
        addChild(sp, 1000, kTagToBeDel);
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 1500));
        sp->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(1.0f, pt), 0.6f));
    }
}

//******************************************************************************
// update
//******************************************************************************
void FightMngr::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::cbCommDlg), enCommDlgTp_connecting);
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
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        if (m_iReqType==enFightMngrReqType_Revive) {
            
            
#ifdef TalkData
            TDCCItem::onPurchase("复活", 1,CGameData::Inst()->getCommonInfo()->revive_coin);
#else
            
#endif
            
            
            

            
            SaveData::Inst()->setCurCoin(CGameData::Inst()->getUsrInfo()->coin);
            SaveData::Inst()->setReviveTime(0);
            
            removeGabage();
            
            if(SaveData::Inst()->getSelectRevive())
            {
                m_player->reviveByOption(CGameData::Inst()->getCommonInfo()->revive_select_tag-201);
            }else
            {
                m_player->revive();
            }
            
            if(m_enemyGrp){
                if (m_enemyGrp->isAllEnemyDead() == false) {
                    m_player->startNewRound(false);
                }
                else if(m_iCurStep + 1 >= m_iMaxStep){
                    showEndAction(true);
                    m_tips->show();
                }
            }
            
            saveCurFightInfo();
            
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0);
        }
        else if(m_iReqType == enFightMngrReqType_GiveWuJiang)
        {
            //刷所有武将
            
            if(CGameData::Inst()->allCardRequest()){
                scheduleUpdate();
                m_iReqType = enFightMngrReqType_GiveWuJiangAllCards;
            }
        }
        else if(m_iReqType == enFightMngrReqType_GiveWuJiangAllCards)
        {
            
            Dialog1* newbieEndDlg;
            
            
            
            if(CGameData::Inst()->getUsrInfo()->dungeon_fail == 1)
            {
                newbieEndDlg = new Dialog1(this, callfuncO_selector(FightMngr::newbie),
                                           CGameData::Inst()->getLanguageValue("fail_new10"),false);
            }
            else if(CGameData::Inst()->getUsrInfo()->dungeon_fail == 2)
            {
                newbieEndDlg = new Dialog1(this, callfuncO_selector(FightMngr::newbie),
                                           CGameData::Inst()->getLanguageValue("fail_new11"),false);
            }
            else if(CGameData::Inst()->getUsrInfo()->dungeon_fail == -1)
            {
                newbieEndDlg = new Dialog1(this, callfuncO_selector(FightMngr::newbie),
                                           CGameData::Inst()->getLanguageValue("fail_new12"),false);
            }
            
            
            addChild(newbieEndDlg, 500, kNewbie);
            newbieEndDlg->release();
            newbieEndDlg->setPosition(CCPointMake(320, 300));
            newbieEndDlg->setScale(0.0);
            newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));

            
        }
        else if(m_iReqType==enFightMngrReqType_PaySuccess)
        {
            
            //talkingdata 
#ifdef TalkData
            TDCCVirtualCurrency::onChargeSuccess(OcProxy::Inst()->getReceipt());
#else
            
#endif
//            CCLog("%s" , OcProxy::Inst()->getReceipt());
            
            CGameData::Inst()->finishPayment();
            CGameData::Inst()->updateCoin();
            SaveData::Inst()->setCurCoin(CGameData::Inst()->getUsrInfo()->coin);
            
            

            
            if(m_commDlg == NULL){
                MainScene::Inst()->enableBottomPanel(false);
                m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::dialog1Callback), enCommDlgTp_retry);
                addChild(m_commDlg, 1000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
                m_commDlg->setScale(0.0);
                m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            }
        }
        else if(m_iReqType==enFightMngrReqType_RefreshAlipay)
        {
            CGameData::Inst()->updateCoin();
            SaveData::Inst()->setCurCoin(CGameData::Inst()->getUsrInfo()->coin);
 #if defined(kPltfmPp)
                closeCoinList();
#endif
            showPayResult(PaymentAliSuccess);
        }
        else if (m_iReqType == enFightMngrReqType_GetShopInfo)
        {
            enterCoinList();
        }
        else if (m_iReqType == enFightMngrReqType_GetTradeNo)
        {
            OcProxy::Inst()->weixininit(this,true);
            CGameData::Inst()->payForCoin(m_iIndex-kPayBtnTagBase);
        }
        else if (m_iReqType == enFightMngrReqType_RefreshAlipay)
        {
            
            CGameData::Inst()->payForCoin(m_iIndex-kPayBtnTagBase);
        }

    }
    else {
        if (comState == 403)
        {
            MainScene::Inst()->reloginWeibo(4);
        }
        
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(FightMngr::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            m_commDlg->setNoCancel();
        }
    }
}


//******************************************************************************
// newbie
//******************************************************************************
void FightMngr::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    CGameState::Instance()->setState(enStateGameOver);
    m_tips->show();
    StopRecordScreen(false);
    
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void FightMngr::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        if (m_iReqType==enFightMngrReqType_Revive) {
            if(CGameData::Inst()->reviveRequest(m_lReviveRty))
                scheduleUpdate();
        }
        else if(m_iReqType==enFightMngrReqType_GiveWuJiang)
        {
            if(CGameData::Inst()->newGiveWuJiangRequest("2"))
            {
                
                m_iReqType = enFightMngrReqType_GiveWuJiang;
                scheduleUpdate();
            }
        }
        
        else if(m_iReqType==enFightMngrReqType_GiveWuJiangAllCards)
        {
            if(CGameData::Inst()->allCardRequest()){
                scheduleUpdate();
                m_iReqType = enFightMngrReqType_GiveWuJiangAllCards;
            }
        }

        else if(m_iReqType==enFightMngrReqType_PaySuccess){
            if(CGameData::Inst()->sendPaySuccessInfo())
                scheduleUpdate();
        }
        else if (m_iReqType == enFightMngrReqType_GetShopInfo)
        {
            getShopInfo();
        }
        else if (m_iReqType == enFightMngrReqType_GetTradeNo)
        {
            if (CGameData::Inst()->getOrderNo(CGameData::Inst()->getIapIdAtIdx(m_iIndex-kPayBtnTagBase), CGameData::Inst()->getUsrInfo()->uid)){
                m_iReqType = enFightMngrReqType_GetTradeNo;
                scheduleUpdate();
            }
        }
        else if (m_iReqType == enFightMngrReqType_RefreshAlipay)
        {
            if(CGameData::Inst()->reqChargeAward()){
                m_iReqType = enFightMngrReqType_RefreshAlipay;
                scheduleUpdate();
            }
        }
        else if(m_iReqType == enFightMngrReqType__PayppBillNo)
        {
            schedule(schedule_selector(FightMngr::waitPPBilloNo), 0.5);
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        CGameState::Instance()->setState(enStateGameOver);
        m_ret.bClear = false;

        showDeathDlg();
    }    
}

//******************************************************************************
// removeGabage
//******************************************************************************
void FightMngr::removeGabage()
{
    CCNode* node = this->getChildByTag(kTagToBeDel);
    
    while (node != NULL) {
        removeChild(node, true);
        node = this->getChildByTag(kTagToBeDel);
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// showFightScene
//******************************************************************************
void FightMngr::showFightScene() {
    m_spFightScene->runAction(CCSpawn::actions(CCScaleTo::actionWithDuration(0.0f, 1.0f / 0.79f),
                                               CCFadeIn::actionWithDuration(0.8f),
                                               NULL));
}

//******************************************************************************
// hideFightScene
//******************************************************************************
void FightMngr::hideFightScene()
{

    m_spFightScene->runAction(CCSpawn::actions(CCFadeOut::actionWithDuration(0.8f),
                                               CCScaleTo::actionWithDuration(0.8f, (1.0f / 0.79) * 1.2),
                                               NULL));
}

//******************************************************************************
// cbPlayerDeadDelay
//******************************************************************************
void FightMngr::cbPlayerDeadDelay(ccTime dt)
{
    unschedule(schedule_selector(FightMngr::cbPlayerDeadDelay));
    
    showEndAction(false);
    
    showDeathDlg();
    
    m_player->RecordControl(false, 2);
}

//******************************************************************************
// showStarEffect
//******************************************************************************
void FightMngr::showStarEffect() {
    m_starEffect = CCParticleVictoryStar::node();
    m_starEffect->setPosition(CCPointMake(320 * 2 * CCRANDOM_0_1(), 480 * 2 * CCRANDOM_0_1()));
    m_starEffect->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_stars.png"));
	addChild(m_starEffect, 2000);
}

//******************************************************************************
// haveEnemyWithNotElem
//******************************************************************************
bool FightMngr::haveEnemyWithNotElem(int iElem)
{
    return m_enemyGrp->haveEnemyWithNotElem(iElem);
}

//******************************************************************************
// haveEnemyWithElem
//******************************************************************************
bool FightMngr::haveEnemyWithElem(int iElem)
{
    bool bRet = m_enemyGrp->haveEnemyWithElem(iElem);
    
    return bRet;
}

//******************************************************************************
// haveEnemyWithElemInstead
//******************************************************************************
bool FightMngr::haveEnemyWithElemInstead(int iElem)
{
    bool bRet = m_enemyGrp->haveEnemyWithElemInstead(iElem);
    
    return bRet;
}

//******************************************************************************
// isEnemyBuffChenMo
//******************************************************************************
bool FightMngr::isEnemyBuffChenMo()
{
    return m_enemyGrp->isBuffEffect(enEnemyBuff_chenmo);
}

//******************************************************************************
// isEnemyBuffYanChi
//******************************************************************************
bool FightMngr::isEnemyBuffYanChi()
{
    return m_enemyGrp->isBuffEffect(enEnemyBuff_yanchi);
}

bool FightMngr::enemyGrpIsLive()
{
    if (NULL == m_enemyGrp)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//******************************************************************************
// isEnemySick
//******************************************************************************
bool FightMngr::isEnemySick()
{
    return m_enemyGrp->isSick();
}

//******************************************************************************
// isEnemyPoisned
//******************************************************************************
bool FightMngr::isEnemyPoisned()
{
    return m_enemyGrp->isPoison();
}

//******************************************************************************
// isEnemyDelayed
//******************************************************************************
bool FightMngr::isEnemyDelayed()
{
    return m_enemyGrp->isSbDelayed();
}

//******************************************************************************
// isEnemyMabi
//******************************************************************************
bool FightMngr::isEnemyMabi()
{
    return m_enemyGrp->isMabi();
}

//******************************************************************************
// isEnemyMabi
//******************************************************************************
bool FightMngr::isEnemyChangeElement()
{
    return m_enemyGrp->isChangeElement();
}

//******************************************************************************
// isEnemyArmorBroken
//******************************************************************************
bool FightMngr::isEnemyArmorBroken()
{
    return m_enemyGrp->isArmorBroken();
}

//******************************************************************************
// updateEnemyGrpBuff
//******************************************************************************
#pragma mark - Enemy Buff
void FightMngr::updateEnemyGrpBuff()
{
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_wenyihuanjing))
        m_player->beHitFromEnemyBuf(enEnemyBuff_wenyihuanjing);
    
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_wenyihuanjing2))
        m_player->beHitFromEnemyBuf(enEnemyBuff_wenyihuanjing2);
    
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_wenyihuanjing3))
        m_player->beHitFromEnemyBuf(enEnemyBuff_wenyihuanjing3);
    
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_wenyi))
        m_player->beHitFromEnemyBuf(enEnemyBuff_wenyi);
    
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_wenyi2))
        m_player->beHitFromEnemyBuf(enEnemyBuff_wenyi2);
    
}

void FightMngr::setEnemyBuffEffect()
{
    m_player->yanChiSkillCD(isEnemyBuffYanChi());
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_canfei))
        schedule(schedule_selector(FightMngr::cbEnemyBuffCanFei), 1.5);
    else
        cbEnemyBuffCanFei(0);
    
    // enEnemyBuff_71/enEnemyBuff_75    每回合转珠开始前，将我方主将和任意一名副将交换
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_71) || m_enemyGrp->isBuffEffect(enEnemyBuff_75)) {
        int index2 = 0;
        if (m_player->getCardByIdx(1) || m_player->getCardByIdx(2) || m_player->getCardByIdx(3) || m_player->getCardByIdx(4)) {
            while ((index2 = (int(CCRANDOM_0_1() * 1000) % 4 + 1))) {
                if (m_player->getCardByIdx(index2)) {
                    break;
                }
            }
        }
        if (index2) {
            m_player->changeZhuJiang(0, index2);
        }
    }
    
    // enEnemyBuff_72/81    每回合转珠开始前，随机封印一名武将的主动技1回合
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_72) || m_enemyGrp->isBuffEffect(enEnemyBuff_81)) {
        int index = SaveData::Inst()->getrand() % 4 + 1;
        
        bool bHaveFuJiang = false;
        for (int fuIndex = 1; fuIndex < 5; fuIndex++) {
            if (m_player->getCardByIdx(fuIndex))
            {   // 有副将
                bHaveFuJiang = true;
                break;
            }
        }
        
        if (bHaveFuJiang)
        {
            while (1) {
                if (m_player->getCardByIdx(index))
                {   // 选中这只副将
                    break;
                }
                index = SaveData::Inst()->getrand() % 4 + 1;
            }
            
            m_player->getCardByIdx(index)->setFengYinCD(m_player->getCardByIdx(index)->getFengYinCD() + 1);
            m_player->getCardByIdx(index)->setCardState(EN_CARD_STATE_FNEGYIN);
        }
    }
    
    // enEnemyBuff_74/79    每回合开始前，随机1种宝珠（不会随机到无效块，毒宝珠等六种基础属性之外的宝珠）变成无效块
    if (m_enemyGrp->isBuffEffect(enEnemyBuff_74) || m_enemyGrp->isBuffEffect(enEnemyBuff_79)) {
        int index = int(CCRANDOM_0_1() * 1000) % 6;
        m_player->getStoneGrid()->chgStone(index | 0xF0, 6 | 0xF0);
    }
    
}

void FightMngr::removeBuffEffect(int idx)
{
    m_player->removeBuffEffect(idx);
}

void FightMngr::addBuffEffect(int idx)
{
    m_player->addBuffEffect(idx);
}


//******************************************************************************
// cbEnemyBuffCanFei
//******************************************************************************
void FightMngr::cbEnemyBuffCanFei(ccTime dt)
{
    unschedule(schedule_selector(FightMngr::cbEnemyBuffCanFei));
    if (m_player->canFei(m_enemyGrp->isBuffEffect(enEnemyBuff_canfei)))
    {
        CCPoint pos = m_enemyGrp->useCanFei();
        
        shoot(pos, m_player->getPosition());
    }
}

//******************************************************************************
// saveCurFightInfo
//******************************************************************************
void FightMngr::saveCurFightInfo(bool isStoneMove)
{
    if (CGameData::Inst()->isNewbie())
    {
        return;
    }
    
    if (SaveData::Inst()->getIsUseSaveData()) {
        SaveData::Inst()->setIsUseSaveData(false);
        return;
    }
    
    SaveData::Inst()->setCurDgGold(m_iDropGoldTotal);
    SaveData::Inst()->setCurItemCnt(m_iDropItemCnt);
    m_player->saveCurPlayerInfo();
    
    m_player->getStoneGrid()->saveAllStones();
    
    if (isStoneMove)
    {
        SaveData::Inst()->setiMoment(1);
    }
    else
    {
        SaveData::Inst()->setiMoment(0);
    }

    if (m_enemyGrp)
    {
        for (int i = 0; i < m_enemyGrp->getEnemyCount(); i++)
        {
            int curHp = m_enemyGrp->getEnemyCurHp(i);
            int curRound = m_enemyGrp->getEnemyCurRound(i);
            int curState = m_enemyGrp->getEnemyCurState(i);
            int elem = m_enemyGrp->getEnemyByIdx(i)->getElement();

            SaveData::Inst()->setEnemyElement(elem,m_iCurStep,i);
            SaveData::Inst()->setEnemyCurState(curState,m_iCurStep,i);
            SaveData::Inst()->setEnemyCurHp(curHp, m_iCurStep, i);
            SaveData::Inst()->setEnemyCurRound(curRound, m_iCurStep, i);
            SaveData::Inst()->setEnemyLastSkillUse(m_enemyGrp->getEnemyByIdx(i)->getSkillUseLastID(),m_iCurStep,i);
            
            Enemy* pEnemy = m_enemyGrp->getEnemyByIdx(i);
            if (pEnemy->getSkill() != NULL)
            {
                for (int w=0; w<pEnemy->getSkill()->count(); w++)
                {
                    SaveData::Inst()->setEnemySkillOccTimes(pEnemy->getSkill()->getObjectAtIndex(w)->get_occurTimes(), w, m_iCurStep, i);
                    SaveData::Inst()->setEnemyRunCount(pEnemy->getSkill()->getObjectAtIndex(w)->get_runCounts(), w, m_iCurStep, i);
                }
            }
            
            if (pEnemy->isAlive() && isStoneMove)
            {
                pEnemy->saveEnemyBuff();
            }
        }
    }
    
    SaveData::Inst()->flush();
}

void FightMngr::forceExitFight()
{
    if(m_enemyGrp != NULL){
        removeChild(m_enemyGrp, true);
        
        m_enemyGrp = NULL;
    }
    
    if(m_player != NULL){
        removeChild(m_player, true);
        
        m_player = NULL;
    }

    if(m_spFightScene){
        removeChild(m_spFightScene, true);
        m_spFightScene = NULL;
    }
    
    //delete unused sprite
    removeGabage();
}

void FightMngr::exit()
{
    forceExitFight();
    removeAllChildrenWithCleanup(true);
}

void FightMngr::initPlayerRoundVars()
{
    if(m_player){
        m_player->updateRoundStartHp();
    }
}



bool FightMngr::countAwakeSkill25()
{
    unsigned short ushSkillCount = m_player->getNumByAwakeId(enAwakeSkill_25);
    if (0<ushSkillCount)
    {
        if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_25))
        {
            int nOdds = ushSkillCount;
            nOdds *= pAwakeSkill->cOdds;
            if (SaveData::Inst()->getrand()<nOdds)
            {
                return true;
            }
        }
    }
    return false;
}








