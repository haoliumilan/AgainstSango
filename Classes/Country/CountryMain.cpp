//
//  CountryMain.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-24.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CountryMain.h"
#include "TitleBar.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "MainScene.h"
#include "CGameData.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "CountryCreate.h"
#include "CountryDialog.h"
#include "Dialog2.h"
#include "PagingListLayer.h"
#include "CommDlg.h"
#include "CountryDetails.h"
#include "OrgFightMngr.h"
#include "OrgFightCashIn.h"
#include "NoFriendLayer.h"
#include "CConfirmLayer.h"
#include "CShowCardInf.h"
#include "OcProxy.h"
#include "GachaGet.h"
#include "OrgSaveData.h"
#include "OrgFightCfm.h"
#include "RaidersLayer.h"
#include "BonusPanel.h"

#include "TDCCMission.h"


using namespace CocosDenshion;

#define enCntry_backBtn             1110
#define enCntry_Manage              1111
#define enCntry_MemberList          1112
#define enCntry_OrgFight            1113
#define enCntry_DetailInfo          1114
#define enCntry_RecentInfo          1115
#define enCntry_Rank                1116
#define enCntry_Donate              1117
#define enCntry_Exchange            1118
#define enCntry_Glass               1119
#define enCntry_Apply               1120
#define enCntry_Help                1121
#define enCntry_Chat                1122
#define enCntry_Post_Reward         1131

#define kOrgCashinTag               2000
#define kOrgFightCfmTag             2013

#define CHATROOMTAG                 20130422

CountryMain::CountryMain()
{

}

CountryMain::CountryMain(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

void CountryMain::init()
{
    m_fightSel = 0;
    m_teamEdit = NULL;
    m_spCntryBg = NULL;
    m_spTitle = NULL;
    m_backItem = NULL;
    m_list = NULL;
    m_listCntry = NULL;
    m_menuCntryMain = NULL;
    m_cntryCreate = NULL;
    m_cntryDialog = NULL;
    m_dlgTip = NULL;
    m_commDlg = NULL;
    m_cntryDeclaration = NULL;
    m_cntryDetails = NULL;
    m_curScreen = enCM_Null;
    m_curReqType = enCReqType_Null;
    m_orgFightMngr = NULL;
    m_dlgNoTip = NULL;
    m_memInfoLayer = NULL;
    m_memberManage = NULL;
    m_memberList = NULL;
    m_postRwdLayer = NULL;
    m_BaoKuLayer = NULL;
    m_FaBuLayer = NULL;
    m_RwdTopMemLayer = NULL;
    m_cfmLayer = NULL;
    m_lcCover = NULL;
    m_listSel = -1;
    m_pLocal = 0;
    m_showCard = NULL;
    m_listWelfare = NULL;
    m_welfareInfoArr = NULL;
    m_gachaLayer = NULL;
    m_bShowMemberList = true;
    m_strDgRtyId.clear();
    m_rangeMenu = NULL;
    m_memberManageBtn = NULL;
    m_rangeTitle = NULL;
    m_dlgRecover = NULL;
    m_bMailAll = false;
    m_tnDeclaration = NULL;
    m_lcCntryProBg = NULL;
    m_rlCntryHelp = NULL;
    m_tnDeleteTime = NULL;
    m_iDeleteTime = 0;
    m_bBackToFightRank = false;
    m_pJiFengBonusPanel = NULL;
    m_bCashin = false;
}

CountryMain::~CountryMain()
{

}

void CountryMain::onEnter()
{
    CCLayer::onEnter();

}

void CountryMain::onExit()
{
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    MainScene::Inst()->showUserInfo();
    
    CCLayer::onExit();
}

void CountryMain::enter()
{
    init();
    
    m_spCntryBg = CCSprite::spriteWithFile("fr_cntry_bg.jpg");
    m_spCntryBg->setPosition(CCPointMake(320, 480));
    addChild(m_spCntryBg, -1);
    m_spCntryBg->setOpacity(0);
    m_spCntryBg->runAction(CCFadeIn::actionWithDuration(0.2));

    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_backItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                       spMenu2,
                                                       this,
                                                       menu_selector(CountryMain::btnCallback));
    m_backItem->setAnchorPoint(CCPointZero);
    m_backItem->setTag(enCntry_backBtn);
    
    CCMenu* menu = CCMenu::menuWithItems(m_backItem, NULL);
    addChild(menu,30);
    menu->setPosition(CCPointZero);
    
    m_list = new CListLayer(this, callfuncO_selector(CountryMain::cbList));
    addChild(m_list, -1);
    m_list->release();
    
    m_listCntry = new CListLayer(this, callfuncO_selector(CountryMain::cbList));
    addChild(m_listCntry, -1);
    m_listCntry->release();
    m_listCntry->setPosition(CCPointMake(-1000, 0));
    m_listCntry->setReloadFunc(callfuncO_selector(CountryMain::insertItemByIdx));
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 200);
    m_showCard->release();
    
    if (OrgSaveData::Inst()->hasValidSavedData())
    {
        enterOrgFight();
    }
    else
        showMainOrNot();
    
}

void CountryMain::enterArenaEnd()
{
    m_orgFightMngr->enterArenaEnd();
}

void CountryMain::exit()
{
    innerExit();
    MainScene::Inst()->switchBgMusic(enBgMusicNml);
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    MainScene::Inst()->showUserInfo();
}

void CountryMain::innerEnter()
{
    CCActionInterval* tempAction;
    
    if ((m_curScreen == enCM_Main) || (m_curScreen == enCM_CntryMain))
    {
        m_spTitle->setPosition(CCPointMake(-210, 766));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
            
        m_backItem->setPosition(CCPointMake(-529, 731));
    }
    else {
        m_spTitle->setPosition(CCPointMake(-210, 766));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        
        m_backItem->setPosition(CCPointMake(-529, 731));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731));
        m_backItem->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    

    if (m_curScreen == enCM_Main
        || m_curScreen == enCM_CntryManage
        || m_curScreen == enCM_JoinCntry
        || m_curScreen == enCM_MemberApply
        || m_curScreen == enCM_BuffLvUpList
        || m_curScreen == enCM_SupplyAssignMain) {
        m_list->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_list->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (m_curScreen == enCM_ChatRoom)
    {
        m_memberManageBtn->setPosition(CCPoint(-115, 731));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731));
        m_memberManageBtn->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (enCM_PostReward == m_curScreen)
    {
        if (m_rangeMenu) {
            m_rangeMenu->setPosition(CCPoint(-115, 731));
            tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731));
            m_rangeMenu->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
        m_postRwdLayer->setPosition(ccp(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_postRwdLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
    }
    else if (enCM_RewardTopMembers ==  m_curScreen)
    {
        m_RwdTopMemLayer->setPosition(ccp(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_RwdTopMemLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
    }
    else if (enCM_FabuXuanShang == m_curScreen)
    {
        m_FaBuLayer->setPosition(ccp(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_FaBuLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
    }
    else if (enCM_BaoKuItem == m_curScreen)
    {
        m_BaoKuLayer->setPosition(ccp(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_BaoKuLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction,0.8f));
    }
    else if (m_curScreen == enCM_CntryList || m_curScreen == enCM_CntryRank)
    {
        m_listCntry->setPosition(CCPointMake(640, m_listCntry->getPosition().y));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1,
                                                  CCPointMake(0,
                                                              m_listCntry->getPosition().y));
        m_listCntry->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (m_curScreen == enCM_CntryMain)
    {
        m_cntryDeclaration->setPosition(CCPointMake(960, 560));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 560));
        m_cntryDeclaration->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        
        m_lcCntryProBg->setPosition(CCPointMake(640, 625));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 625));
        m_lcCntryProBg->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        
        m_menuCntryMain->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_menuCntryMain->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (m_curScreen == enCM_SearchCntry || m_curScreen == enCM_SetDeclaratin
             || m_curScreen == enCM_CntryDonate
             || m_curScreen == enCM_CntryLevelUp || m_curScreen == enCM_CreateCntry
             || m_curScreen == enCM_BuffLvUpDialog || m_curScreen == enCM_SupplyAssignDilog
             || m_curScreen == enCM_SetNameCard)
    {
        m_cntryDialog->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_cntryDialog->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (m_curScreen == enCM_MemberList || m_curScreen == enCM_SupplyAssignList
             || m_curScreen == enCM_RewardMemberList)
    {
        if (m_memberList)
        {
            m_memberList->setPosition(CCPointMake(640, 0));
            tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
            m_memberList->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
        
        if (m_rangeMenu)
        {
            m_rangeMenu->setPosition(CCPoint(-115, 731));
            tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731));
            m_rangeMenu->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
    }
    else if (m_curScreen == enCM_GetWelfare)
    {
        m_listWelfare->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_listWelfare->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if(m_curScreen  == enCM_OrgFightList)
    {
        m_list->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_list->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if (m_curScreen == enCM_CntryHelp)
    {
        m_rlCntryHelp->setPosition(CCPointMake(640, 0));
        tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
        m_rlCntryHelp->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    else if(m_curScreen == enCM_JiFengBonusPanel){
        if (m_pJiFengBonusPanel) {
            m_pJiFengBonusPanel->setPosition(CCPointMake(640, 720));
            tempAction = CCMoveTo::actionWithDuration(kUiActionDur1,CCPointMake(12,720));
            m_pJiFengBonusPanel->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
        
        if (m_rangeMenu) {
            m_rangeMenu->setPosition(CCPoint(-115, 731));
            tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731));
            m_rangeMenu->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
    }
}

void CountryMain::innerExit()
{
    m_spTitle->stopAllActions();
    m_spTitle->setPosition(CCPointMake(-1000, -1000));

    m_backItem->stopAllActions();
    m_backItem->setPosition(CCPointMake(-1000, -1000));
    
    if (m_curScreen == enCM_Main
        || m_curScreen == enCM_JoinCntry 
        || m_curScreen == enCM_BuffLvUpList
        || m_curScreen == enCM_SupplyAssignMain)
    {
        m_list->stopAllActions();
        m_list->setPosition(CCPointMake(-1000, -1000));
        m_list->clearAll();
    }
    else if (enCM_TeamEdit == m_curScreen)
    {
        if (m_teamEdit != NULL)
        {
            m_teamEdit->removeAllChildrenWithCleanup(true);
            m_teamEdit->removeFromParentAndCleanup(true);
            m_teamEdit = NULL;
        }
    }
    else if (enCM_ChatRoom == m_curScreen)
    {
        removeChildByTag(CHATROOMTAG, true);
        if (m_memberManageBtn != NULL)
        {
            removeChild(m_memberManageBtn, true);
            m_memberManageBtn = NULL;
        }
    }
    else if (enCM_MemberManage == m_curScreen)
    {
        if (m_memberManage != NULL)
        {
            m_memberManage->removeAllChildrenWithCleanup(true);
            m_memberManage->removeFromParentAndCleanup(true);
            m_memberManage = NULL;
        }
    }
    else if (m_curScreen == enCM_CntryManage)
    {
        m_list->stopAllActions();
        m_list->setPosition(CCPointMake(-1000, -1000));
        m_list->clearAll();

        if (m_dlgTip) {
            m_dlgTip->removeFromParentAndCleanup(true);
            m_dlgTip = NULL;
        }
        
        if (m_dlgRecover)
        {
            m_dlgRecover->removeFromParentAndCleanup(true);
            m_dlgRecover = NULL;
        }
    }
    else if (m_curScreen == enCM_MemberApply)
    {
        m_list->stopAllActions();
        m_list->setPosition(CCPointMake(-1000, -1000));
        m_list->clearAll();

        if (m_dlgNoTip) {
            m_dlgNoTip->removeFromParentAndCleanup(true);
            m_dlgNoTip = NULL;
        }
        
        if (m_memInfoLayer) {
            m_memInfoLayer->removeFromParentAndCleanup(true);
            m_memInfoLayer = NULL;
        }
    }
    else if (m_curScreen == enCM_CreateCntry)
    {
        if (m_cntryDialog) {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
        
        if (m_cntryCreate) {
            m_cntryCreate->removeFromParentAndCleanup(true);
            m_cntryCreate = NULL;
        }
    }
    else if (m_curScreen == enCM_PostReward)
    {
        if (m_rangeMenu)
        {
            m_rangeMenu->removeFromParentAndCleanup(true);
            m_rangeMenu = NULL;
        }
        
        removeChild(m_postRwdLayer, true);
        m_postRwdLayer = NULL;
    }
    else if(enCM_RewardTopMembers == m_curScreen)
    {
        removeChild(m_RwdTopMemLayer, true);
        m_RwdTopMemLayer = NULL;
    }
    else if (enCM_FabuXuanShang == m_curScreen)
    {
        removeChild(m_FaBuLayer, true);
        m_FaBuLayer = NULL;
    }
    else if (m_curScreen == enCM_BaoKuItem)
    {
        removeChild(m_BaoKuLayer, true);
        m_BaoKuLayer = NULL;
    }
    else if (m_curScreen == enCM_CntryRank || m_curScreen == enCM_CntryList)
    {
        m_listCntry->stopAllActions();
        m_listCntry->setPosition(CCPointMake(-1000,
                                             m_listCntry->getPosition().y));
        m_listCntry->clearAll();
        
        if (m_cntryDetails) {
            m_cntryDetails->removeFromParentAndCleanup(true);
            m_cntryDetails = NULL;
        }
        
        if (m_dlgNoTip) {
            m_dlgNoTip->removeFromParentAndCleanup(true);
            m_dlgNoTip = NULL;
        }
    }
    else if (m_curScreen == enCM_SearchCntry ) {
        if (m_cntryDialog) {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
        
        if (m_cntryDetails) {
            m_cntryDetails->removeFromParentAndCleanup(true);
            m_cntryDetails = NULL;
        }
        
    }
    else if (m_curScreen == enCM_SetDeclaratin || m_curScreen == enCM_CntryLevelUp
             || m_curScreen == enCM_BuffLvUpDialog || m_curScreen == enCM_SupplyAssignDilog)
    {
        if (m_cntryDialog) {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
    }
    else if (m_curScreen == enCM_CntryDonate)
    {
        if (m_cntryDialog) {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
        
        if (m_dlgRecover)
        {
            m_dlgRecover->removeFromParentAndCleanup(true);
            m_dlgRecover = NULL;
        }
    }
    else if (m_curScreen == enCM_CntryMain)
    {
        if (m_menuCntryMain) {
            m_menuCntryMain->removeFromParentAndCleanup(true);
            m_menuCntryMain = NULL;
        }
        
        if (m_cntryDeclaration) {
            m_cntryDeclaration->removeFromParentAndCleanup(true);
            m_cntryDeclaration = NULL;
        }
        
        if (m_lcCntryProBg)
        {
            m_lcCntryProBg->removeFromParentAndCleanup(true);
            m_lcCntryProBg = NULL;
        }
        
        if (m_cntryDetails)
        {
            m_cntryDetails->removeFromParentAndCleanup(true);
            m_cntryDetails = NULL;
        }
        
        unschedule(schedule_selector(CountryMain::updateDeleteTime));
    }
    else if (m_curScreen == enCM_MemberList || m_curScreen == enCM_SupplyAssignList
             || m_curScreen == enCM_RewardMemberList)
    {
        if (m_memberList) {
            m_memberList->removeFromParentAndCleanup(true);
            m_memberList = NULL;
        }
        
        if (m_rangeMenu) {
            m_rangeMenu->removeFromParentAndCleanup(true);
            m_rangeMenu = NULL;
        }
        
        if (m_rangeTitle)
        {
            m_rangeTitle->removeFromParentAndCleanup(true);
            m_rangeTitle = NULL;
        }
        
        if (m_dlgNoTip)
        {
            m_dlgNoTip->removeFromParentAndCleanup(true);
            m_dlgNoTip = NULL;
        }
    }
    else if (m_curScreen == enCM_CfmLayer)
    {
        if (m_memInfoLayer) {
            m_memInfoLayer->removeFromParentAndCleanup(true);
            m_memInfoLayer = NULL;
        }
        
        if (m_memberList) {
            m_memberList->removeFromParentAndCleanup(true);
            m_memberList = NULL;
        }
        
        if (m_rangeMenu) {
            m_rangeMenu->removeFromParentAndCleanup(true);
            m_rangeMenu = NULL;
        }
        
        if (m_rangeTitle)
        {
            m_rangeTitle->removeFromParentAndCleanup(true);
            m_rangeTitle = NULL;
        }
        
        if (m_lcCover) {
            m_lcCover->removeFromParentAndCleanup(true);
            m_lcCover = NULL;
        }
        
        if (m_cfmLayer) {
            m_cfmLayer->removeFromParentAndCleanup(true);
            m_cfmLayer = NULL;
        }
    }
    else if (m_curScreen == enCM_GetWelfare)
    {
        if (m_listWelfare) {
            m_listWelfare->removeFromParentAndCleanup(true);
            m_listWelfare = NULL;
        }
        
        if (m_welfareInfoArr) {
            m_welfareInfoArr->removeAllObjects();
            m_welfareInfoArr->release();
            m_welfareInfoArr = NULL;
        }
        
        if (m_dlgTip)
        {
            m_dlgTip->removeFromParentAndCleanup(true);
            m_dlgTip = NULL;
        }
        
        if (m_dlgRecover)
        {
            m_dlgRecover->removeFromParentAndCleanup(true);
            m_dlgRecover = NULL;
        }
        
        if (m_gachaLayer) {
            m_gachaLayer->removeFromParentAndCleanup(true);
            m_gachaLayer = NULL;
        }
    }
    else if( m_curScreen == enCM_OrgFightList)
    {
        m_list->stopAllActions();
        m_list->setPosition(CCPointMake(-1000, -1000));
        m_list->clearAll();
        
        if (this->getChildByTag(kCntryRecoverPowerMenu))
        {
            this->removeChildByTag(kCntryRecoverPowerMenu, true);
        }
        
        if (m_dlgTip)
        {
            m_dlgTip->removeFromParentAndCleanup(true);
            m_dlgTip = NULL;
        }
        
        if (m_dlgRecover)
        {
            m_dlgRecover->removeFromParentAndCleanup(true);
            m_dlgRecover = NULL;
        }
    }
    else if (m_curScreen == enCM_CntryHelp)
    {
        if (m_rlCntryHelp)
        {
            m_rlCntryHelp->exit();
            m_rlCntryHelp->removeFromParentAndCleanup(true);
            m_rlCntryHelp = NULL;
        }
        
        if (m_dlgRecover)
        {
            m_dlgRecover->removeFromParentAndCleanup(true);
            m_dlgRecover = NULL;
        }
    }
    else if (m_curScreen == enCM_SetNameCard)
    {
        if (m_dlgTip)
        {
            m_dlgTip->removeFromParentAndCleanup(true);
            m_dlgTip = NULL;
        }
        
        if (m_cntryDialog)
        {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
    }
    else if (m_curScreen == enCM_JiFengBonusPanel)
    {
        if (m_pJiFengBonusPanel) {
            m_pJiFengBonusPanel->removeFromParentAndCleanup(true);
            m_pJiFengBonusPanel = NULL;
        }
        
        if (m_rangeMenu) {
            m_rangeMenu->removeFromParentAndCleanup(true);
            m_rangeMenu = NULL;
        }
    }
}

void CountryMain::btnRecoverPower(CCObject* pObj)
{
    if (CGameData::Inst()->getLocalAcPoint() < CGameData::Inst()->getUsrInfo()->max_acPoint)
    {
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->recover_action_coin)
            showRecoverAcPointTip();
        else
            showNoCoinForRecover();
    }
    else
        // 战斗力已满无需回复
        showAcPointFullTip();
}

void CountryMain::btnCallback(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* pRet = (CCMenuItem*)pObj;
    switch (pRet->getTag())
    {
        case enCntry_Post_Reward:
            reqRewardTaskList();
            break;
            
        case enCntry_backBtn:
            // 返回键
            switch (m_curScreen) {
                case enCM_Main:
                case enCM_CntryMain:
                    // 返回到社交界面
                    (m_pListener->*m_pfnSelector)();
                    break;
                
                case enCM_CreateCntry:
                case enCM_JoinCntry:
                case enCM_SearchCntry:
                    // 返回到国家主界面 user is not one of the country
                    showMain();
                    break;

                case enCM_PostReward:
                case enCM_CntryRank:
                case enCM_MemberList:
                case enCM_GetWelfare:
                case enCM_CntryDonate:
                case enCM_OrgFightList:
                case enCM_SetDeclaratin:
                case enCM_MemberApply:
                case enCM_CntryManage:
                    // 返回到国家主界面 user is one of the country
                    showMainOrNot();
                    break;
                    
                case enCM_FabuXuanShang:
                case enCM_BaoKuItem:
                case enCM_RewardTopMembers:
                    showPostReward();
                    break;
                    
                case enCM_FabuXuanZe:
                    if (m_FaBuLayer)
                        m_FaBuLayer->removeChooseLayer();
                        break;
                    
                case enCM_TeamEdit:
                    reqCntryDgCfg();
                    break;
                    
                case enCM_ChatRoom:
                {
                    showMainOrNot();
                }
                    break;
                    
                case enCM_MemberManage:
                {
                    showChatRoom();
                }
                    break;
                
                case enCM_CntryList:
                    // 返回加入国家界面
                    showJoinCountry();
                    break;
                    
                case enCM_SupplyAssignList:
                    // 返回到分配福利大目录
                    showSupplyAssignMain();
                    break;
                    
                case enCM_SupplyAssignMain:
                case enCM_BuffLvUpList:
                case enCM_CntryLevelUp:
                case enCM_SetNameCard:
                    // 返回到内政管理界面
                    showCountryManage();
                    break;
                    
                case enCM_BuffLvUpDialog:
                    // 返回buff升级列表
                    showBuffLevelUpList();
                    break;
                    
                case enCM_SupplyAssignDilog:
                    // 返回福利分配列表
                    showSupplyAssignList();
                    break;
                    
                case enCM_CntryHelp:
                    if (CGameData::Inst()->getHasShowCntryNotice())
                        showMainOrNot();
                    else
                        showNotCntryHelp();
                    
                    break;
                    
                case enCM_JiFengBonusPanel:
                    if ((m_pJiFengBonusPanel) && (m_pJiFengBonusPanel->backBtnProc())) {
                        //panel内部处理返回按钮
                    }
                    else{
                        // 返回到分配福利大目录
                        showSupplyAssignMain();
                    }
                    
                    break;
                
                case enCM_RewardMemberList:
                    showPostReward();
                    break;
                    
                default:
                    break;
            }
            break;
                
        case enCntry_Manage:
            // 内政管理
            showCountryManage();
            break;
            
        case enCntry_MemberList:
            // 成员列表
            m_bShowMemberList = true;
            getMemberList();
            break;
            
        case enCntry_OrgFight:
            // 保卫战
            m_bBackToFightRank = false;
            reqCntryDgCfg();
            break;
            
        case enCntry_DetailInfo:
            // 国家详情
            showCountryDetails();
            break;
            
        case enCntry_RecentInfo:
            // 国家近况
            getCntryCurrentInfo();
            break;
        
        case enCntry_Rank:
            // 国家排名
            getAllCntryByLv();
            break;
            
        case enCntry_Donate:
            // 国库捐献
            showCountryDonate();
            break;
            
        case enCntry_Exchange:
            // 兑换福利
            showWelfareList();
            break;
            
        case enCntry_Chat:
            // 聊天室
            MainScene::Inst()->showCntryTips(true);
            showChatRoom();
            break;
            
        case enCntry_Glass:
            // 修改宣言
            showSetDeclaration();
            break;
            
        case enCntry_Apply:
            // 成员申请
            getMemberApplyList();
            break;
            
        case enCntry_Help:
            // 国家帮助
            showCntryHelp();
            break;
            
        default:
            break;
    }    
}

void CountryMain::showMainOrNot()
{
    m_preStatus = CGameData::Inst()->getCntryInfo()->status;
    if (CGameData::Inst()->getCntryInfo()->status == 0) {
        showMain();
    }
    else {
        showCountryMain();
    }
}

void CountryMain::showMain()
{
    innerExit();
    
    m_curScreen = enCM_Main;
   
    MainScene::Inst()->showBottomPromptInfo("cntry_country4_1");
     //MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_country"));
    
    const char* menuName[] = {CGameData::Inst()->getLanguageValue("cntry_create_country"), CGameData::Inst()->getLanguageValue("cntry_join_country"), CGameData::Inst()->getLanguageValue("cntry_find_country")};
    newMenuBtn(menuName, 3);
    
    innerEnter();
}

void CountryMain::showCountryMain()
{
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    MainScene::Inst()->switchBgMusic(enBgMusicOrgEnter);
    
    innerExit();
    
    m_curScreen = enCM_CntryMain;
    
    MainScene::Inst()->showCntryInfo();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_country4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleStringLab(CGameData::Inst()->getCntryInfo()->name);
    
    showCntryPro();
    showDeclaration();
    
    CCMenuItem *mItemDetailInfo;
    newBtn(mItemDetailInfo, "", CCPointMake(520, 725),
           enCntry_DetailInfo, "fr_cntry_details.png");
    
    TextNode* tempText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_country_info"),
                                                  CCSizeMake(189, 79),
                                                  CCTextAlignmentCenter,
                                                  28);
    tempText->setShadowColor(ccBLACK);
    tempText->setPosition(CCPointMake(95 - 2, 40 + 2));
    mItemDetailInfo->addChild(tempText);
    
    CCMenuItem *mItemGlass = NULL;
    CCMenuItem *mItemApply = NULL;
    if ((cntryInfo->status == 1) || (cntryInfo->status == 2))
    {
        newBtn(mItemGlass, "", CCPointMake(585, 525),
               enCntry_Glass, "fr_button_glass.png");
                
        CCActionInterval* scaleAction = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(0.5, 1.2),
                                                                 CCScaleTo::actionWithDuration(0.5, 1.0));
        mItemGlass->runAction(CCRepeatForever::actionWithAction(scaleAction));
    
        if (cntryInfo->member_apply_num > 0)
        {
            newBtn(mItemApply, "", CCPointMake(560, 670),
                   enCntry_Apply, "fr_button_memberApply.png");
        
            CCActionInterval* fadeAction = CCSequence::actionOneTwo(CCFadeOut::actionWithDuration(1.0),
                                                                    CCFadeIn::actionWithDuration(1.0));
            mItemApply->runAction(CCRepeatForever::actionWithAction(fadeAction));
            
            char buf[100];
            snprintf(buf, 99, "%d", cntryInfo->member_apply_num);
            TextNode* tnApplyNum = TextNode::textWithString(buf, 20);
            tnApplyNum->setColor(ccc3(5, 92, 166));
            tnApplyNum->setAnchorPoint(CCPointMake(0, 0.5));
            tnApplyNum->setPosition(CCPointMake(45, 20));
            mItemApply->addChild(tnApplyNum);
        }
    }
    
    CCMenuItem *mItemOrgFight;
    newBtn(mItemOrgFight, "", CCPointMake(128, 310),
           enCntry_OrgFight, "fr_button_orgFight.png");
    
    if (cntryInfo->org_dungeon_num > 0)
    {
        CCSprite* spNew = CCSprite::spriteWithSpriteFrameName("ma_new.png");
        mItemOrgFight->addChild(spNew, 1);
        spNew->setPosition(CCPointMake(110, 200));

        CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                          CCFadeIn::actionWithDuration(0.5),
                                                                          NULL);
        spNew->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    CCMenuItem *mItemPostReward;
    newBtn(mItemPostReward, "", CCPointMake(320, 310), enCntry_Post_Reward, "fr_button_reward.png");
    
    CCMenuItem *mItemExchange;
    newBtn(mItemExchange, "", CCPointMake(522, 310),
           enCntry_Exchange, "fr_button_exchangeWelfare.png");
    
    CCMenuItem *mItemDonate;
    newBtn(mItemDonate, CGameData::Inst()->getLanguageValue("cntry_new_donate"),
           CCPointMake(110, 230), enCntry_Donate, "fr_button_cntry.png");
    
    CCMenuItem *mItemRank;
    newBtn(mItemRank, CGameData::Inst()->getLanguageValue("cntry_new_rank"),
           CCPointMake(320, 230), enCntry_Rank, "fr_button_cntry.png");

    CCMenuItem *mItemRecentInfo;
    newBtn(mItemRecentInfo, CGameData::Inst()->getLanguageValue("cntry_new_recent"),
           CCPointMake(530, 230), enCntry_RecentInfo, "fr_button_cntry.png");
    
    CCMenuItem *mItemMemberList;
    newBtn(mItemMemberList, CGameData::Inst()->getLanguageValue("cntry_new_member"),
           CCPointMake(110, 150), enCntry_MemberList, "fr_button_cntry.png");

    CCMenuItem *mItemManage;
    newBtn(mItemManage, CGameData::Inst()->getLanguageValue("cntry_new_manage"),
           CCPointMake(320, 150), enCntry_Manage, "fr_button_cntry.png");

    CCMenuItem *mItemChat;
    newBtn(mItemChat, CGameData::Inst()->getLanguageValue("cntry_new_chat"),
           CCPointMake(530, 150), enCntry_Chat, "fr_button_cntry.png");
    
    if (CGameData::Inst()->haveNewMsg())
    {
        CCSprite* numberTips = OcProxy::Inst()->showTipsAndNum(10);
        numberTips->setPosition(ccp(170, 70));
        mItemChat->addChild(numberTips);
    }

    if (CGameData::Inst()->getHasShowCntryNotice())
    {
        m_menuCntryMain = CCMenu::menuWithItems(mItemPostReward, mItemMemberList, mItemOrgFight, mItemDetailInfo,
                                                mItemRecentInfo, mItemRank, mItemDonate,
                                                mItemManage, mItemChat,
                                                mItemExchange, mItemGlass, mItemApply,
                                                NULL);
    }
    else {
        CCMenuItem *mItemHelp;
        newBtn(mItemHelp, "", CCPointMake(610, 450), enCntry_Help, "fr_button_faq.png");
        CCActionInterval* scaleAction = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(0.5, 1.2),
                                                                 CCScaleTo::actionWithDuration(0.5, 1.0));
        mItemHelp->runAction(CCRepeatForever::actionWithAction(scaleAction));
        
        m_menuCntryMain = CCMenu::menuWithItems(mItemPostReward, mItemMemberList, mItemOrgFight, mItemDetailInfo,
                                                mItemRecentInfo, mItemRank, mItemDonate,
                                                mItemManage, mItemChat,
                                                mItemExchange, mItemHelp, mItemGlass, mItemApply,
                                                NULL);
    }
    
    m_menuCntryMain->setPosition(CCPointZero);
    addChild(m_menuCntryMain);
    
    innerEnter();
}

void CountryMain::showCntryPro()
{
    char buf[100];
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    m_lcCntryProBg = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128),
                                                             640, 94);
    m_lcCntryProBg->setPosition(CCPointMake(0, 625));
    addChild(m_lcCntryProBg);
    
    snprintf(buf, 99, "icon_cntry_lv_%d.png", cntryInfo->lv);
    CCSprite* spIconLv = CCSprite::spriteWithFile(buf);
    spIconLv->setPosition(CCPointMake(70, 47));
    m_lcCntryProBg->addChild(spIconLv);
    
    TextNode* tnLeaderName = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_por_leaderName"),
                                                      CCSizeMake(240, 25),
                                                      CCTextAlignmentLeft,
                                                      20);
    tnLeaderName->setPosition(CCPointMake(250, 65));
    tnLeaderName->setColor(ccc3(254, 206, 82));
    m_lcCntryProBg->addChild(tnLeaderName);
    
    CCLabelTTF* tnLeaderName2 = CCLabelTTF::labelWithString(cntryInfo->leader_name, CCSizeMake(180, 25), CCTextAlignmentLeft, FONTSTHEITISCM, 23);
    tnLeaderName2->setAnchorPoint(ccp(0,0.5));
    tnLeaderName2->setPosition(ccp(206,65));
    tnLeaderName2->setColor(ccc3(254, 206, 82));
    m_lcCntryProBg->addChild(tnLeaderName2);
    
    snprintf(buf, 99,
             CGameData::Inst()->getLanguageValue("cntry_por_cntryLv"),
             cntryInfo->lv);
    TextNode* tnCntryLv = TextNode::textWithString(buf,
                                                    CCSizeMake(240, 25),
                                                    CCTextAlignmentLeft,
                                                    20);
    tnCntryLv->setPosition(CCPointMake(250, 30));
    tnCntryLv->setColor(ccc3(254, 206, 82));
    m_lcCntryProBg->addChild(tnCntryLv);
    
    snprintf(buf, 99,
             CGameData::Inst()->getLanguageValue("cntry_por_memberNum"),
             cntryInfo->member_cur_num, cntryInfo->member_max_num);
    TextNode* tnMemberNum = TextNode::textWithString(buf,
                                                      CCSizeMake(240, 25),
                                                      CCTextAlignmentLeft,
                                                      20);
    tnMemberNum->setPosition(CCPointMake(490, 65));
    tnMemberNum->setColor(ccc3(254, 206, 82));
    m_lcCntryProBg->addChild(tnMemberNum);
    
    snprintf(buf, 99,
             CGameData::Inst()->getLanguageValue("cntry_por_cntryCoin"),
             cntryInfo->cntry_coin);
    TextNode* tnCntryCoin = TextNode::textWithString(buf,
                                                     CCSizeMake(240, 25),
                                                     CCTextAlignmentLeft,
                                                     20);
    tnCntryCoin->setPosition(CCPointMake(490, 30));
    tnCntryCoin->setColor(ccc3(254, 206, 82));
    m_lcCntryProBg->addChild(tnCntryCoin);
    
    m_tnDeleteTime = TextNode::textWithString("",
                                              CCSizeMake(500, 40),
                                              CCTextAlignmentLeft,
                                              20);
    m_tnDeleteTime->setColor(ccc3(151, 218, 71));
    m_tnDeleteTime->setAnchorPoint(CCPointMake(0, 0.5));
    m_tnDeleteTime->setPosition(CCPointMake(50, -20));
    m_lcCntryProBg->addChild(m_tnDeleteTime);
    
    CommonInfo* commonInfo = CGameData::Inst()->getCommonInfo();
    m_iDeleteTime = cntryInfo->cntry_delete_time + commonInfo->delete_guild_cache_time * 3600 - CGameData::Inst()->getCurrentTime();
    if (m_iDeleteTime > 0)
    {
        schedule(schedule_selector(CountryMain::updateDeleteTime), 1.0f);
    }
}

void CountryMain::updateDeleteTime(ccTime dt)
{
    m_iDeleteTime--;
    if (m_iDeleteTime > 0)
    {    
        char buf[100];
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cntry_delete_time"),
                 m_iDeleteTime/3600,
                 (m_iDeleteTime%3600) / 60,
                 m_iDeleteTime%60);
        m_tnDeleteTime->setString(buf);
    }
    else {
        unschedule(schedule_selector(CountryMain::updateDeleteTime));
        updateCntryInfo();
    }
}

void CountryMain::updateCntryInfo()
{
    if (CGameData::Inst()->getShopRequest())
    {
        m_curReqType = enCReqType_UpdateCntryInfo;
        scheduleUpdate();
    }
}

void CountryMain::setNameCard()
{
    if (m_cntryDialog->setNameCard())
    {
        m_curReqType = enCReqType_SetNameCard;
        scheduleUpdate();
    }
}

void CountryMain::showDeclaration()
{
    m_cntryDeclaration = CCSprite::spriteWithFile("fr_declaration.jpg");
    m_cntryDeclaration->setPosition(CCPointMake(320, 560));
    addChild(m_cntryDeclaration);
    
    float fDeclarationHeight = m_cntryDeclaration->getContentSize().height * 0.5;
    
    CCSprite* declarationLeft = CCSprite::spriteWithFile("fr_declaration_left.jpg");
    declarationLeft->setAnchorPoint(CCPointMake(0, 0.5));
    declarationLeft->setPosition(CCPointMake(0,
                                             fDeclarationHeight));
    m_cntryDeclaration->addChild(declarationLeft, 1);
    
    CCSprite* declarationRight = CCSprite::spriteWithFile("fr_declaration_right.jpg");
    declarationRight->setAnchorPoint(CCPointMake(1, 0.5));
    declarationRight->setPosition(CCPointMake(640,
                                              fDeclarationHeight));
    m_cntryDeclaration->addChild(declarationRight, 1);
    
//    m_tnDeclaration = TextNode::textWithString(CGameData::Inst()->getCntryInfo()->declaration,
//                                               26);
    
    m_tnDeclaration = CCLabelTTF::labelWithString(CGameData::Inst()->getCntryInfo()->declaration, FONTSTHEITISCM, 26);
    
    m_tnDeclaration->setAnchorPoint(CCPointMake(0, 0.5));
    m_tnDeclaration->setPosition(CCPointMake(120 + 640,
                                             fDeclarationHeight));
    m_cntryDeclaration->addChild(m_tnDeclaration);
    
    CCActionInstant* callbackAction = CCCallFunc::actionWithTarget(this,
                                                                   callfunc_selector(CountryMain::scrollDeclaration));
    m_tnDeclaration->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                   callbackAction,
                                                   NULL));
}

void CountryMain::scrollDeclaration()
{
    if (m_cntryDeclaration == NULL)
        return;
    
    float fDeclarationWidth = m_tnDeclaration->getContentSize().width;
    float fDeclarationHeight = m_cntryDeclaration->getContentSize().height * 0.5 + 4;
    m_tnDeclaration->setPosition(CCPointMake(120 + 640,
                                             fDeclarationHeight));
    
    CCActionInstant* callbackAction = CCCallFunc::actionWithTarget(this,
                                                                   callfunc_selector(CountryMain::scrollDeclaration));
    
    CCActionInterval* scrollAction = CCMoveBy::actionWithDuration(fDeclarationWidth / 30,
                                                                  CCPointMake(-fDeclarationWidth, 0));
    CCActionInterval* resetAction = CCMoveBy::actionWithDuration(0.2f,
                                                                 CCPointMake(-640, 0));
    
    m_tnDeclaration->runAction(CCSequence::actions(resetAction,
                                                   CCDelayTime::actionWithDuration(1.0f),
                                                   scrollAction,
                                                   callbackAction,
                                                   NULL));
}

void CountryMain::showCntryHelp()
{
    innerExit();
    
    m_curScreen = enCM_CntryHelp;
    
    MainScene::Inst()->showBottomPromptInfo("cntry_help_prompt_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_help_title"));
    
    if (m_rlCntryHelp == NULL)
    {
        m_rlCntryHelp = new RaidersLayer();
        addChild(m_rlCntryHelp);
    }
    
    m_rlCntryHelp->enterWithType(enUrlTypeCntryHelp);

    innerEnter();
}

void CountryMain::showNotCntryHelp()
{
    innerExit();
    
    CGameData::Inst()->setHasShowCntryNotice();
    
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbNotCntryHelp),
                                   CGameData::Inst()->getLanguageValue("cntry_help_not"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbNotCntryHelp()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
    
    showMainOrNot();
}

void CountryMain::showSetNameCard()
{
    innerExit();
    
    m_curScreen = enCM_SetNameCard;
   
    MainScene::Inst()->showBottomPromptInfo("cntry_cardName_prompt_1");
    // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_cardName"));

    if (CGameData::Inst()->getCommonInfo()->modify_name_gold > 0)
        showNameCardCoinTips();
    else
        showSetNameCardDlg();
}

void CountryMain::showSetNameCardDlg()
{
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::cbSetNameCardDlg));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSetNameCardDialog();

    innerEnter();
}

void CountryMain::cbSetNameCardDlg()
{
    if (CGameData::Inst()->getCommonInfo()->modify_name_gold > 0)
        showSureNameCardTips();
    else
        setNameCard();
}

void CountryMain::showNameCardCoinTips()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_cardName_coin"),
                 CGameData::Inst()->getCommonInfo()->modify_name_gold);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbNameCardCoinTips),
                               CGameData::Inst()->getLanguageValue("cntry_cardName_title"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbNameCardCoinTips(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true)
        showSetNameCardDlg();
    else {
        // 取消
        showCountryManage();
    }
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::showSureNameCardTips()
{
    innerExit();
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfuncO_selector(CountryMain::cbSureNameCardTips));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSureNameCardDialog();
    
    innerEnter();
}

void CountryMain::cbSureNameCardTips(CCObject* pObj)
{
    CntryDlgRet* ret = (CntryDlgRet*)pObj;
    if (ret->bOk)
        setNameCard();
    else
        showCountryManage();
}

void CountryMain::cbList(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    if (m_curScreen == enCM_Main) {
        switch (pRet->iBtnSel) {
            case 0:
                //判断是否已经绑定用户，如果没有，那么提示去绑定
                if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
                {
                    Dialog2 *dialog = new Dialog2(this,
                                                  callfuncO_selector(CountryMain::dl2CallBack),
                                                  CGameData::Inst()->getLanguageValue("cntry_authened"),
                                                  CGameData::Inst()->getLanguageValue("cntry_authened_info"),
                                                  enDl2TwoBtn);
                    addChild(dialog,100);
                    dialog->setTag(111);
                    dialog->setPosition(ccp(320, 480));
                    dialog->release();
                }
                else
                {
                    // 创建国家的提示
                    showCreateCntryTip();
                }
                break;
                
            case 1:
                
                //判断是否已经绑定用户，如果没有，那么提示去绑定
                if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
                {
                    Dialog2 *dialog = new Dialog2(this,
                                                  callfuncO_selector(CountryMain::dl2CallBack),
                                                  CGameData::Inst()->getLanguageValue("cntry_authened"),
                                                  CGameData::Inst()->getLanguageValue("cntry_authened_info"),
                                                  enDl2TwoBtn);
                    addChild(dialog,100);
                    dialog->setTag(111);
                    dialog->setPosition(ccp(320, 480));
                    dialog->release();
                }
                else
                {
                    // 加入国家
                    showJoinCountry();
                }
                break;
                
            case 2:
                // 查找国家
                showSearchCountry();
                break;
                
            default:
                break;
        }
    }
    else if (m_curScreen == enCM_JoinCntry)
    {
        switch (pRet->iBtnSel) {
            case 0:
                // 最新创建的国家
                getAllCntryByNewTime();
                break;
                
            case 1:
                // 实力强的国家
                getAllCntryByLv();
                break;
                
            default:
                break;
        }
    }
    else if (m_curScreen == enCM_CntryManage)
    {        
        if (cntryInfo->status == 3) {
            // member
            switch (pRet->iBtnSel) {
                case 0:
                    // 设置同盟名片
                    showSetNameCard();
                    break;

                case 1:
                    // 退出公会
                    showQuitCountryTip();
                    break;
                    
                default:
                    break;
            }
        }
        else if (cntryInfo->status == 1) {
            // leader
            switch (pRet->iBtnSel) {
                case 0:
                    // 国家升级
                    showCntryLevelUp();
                    break;
                
                case 1:
                    // buff升级
                    showBuffLevelUpList();
                    break;
                    
                case 2:
                    // 会员申请
                    getMemberApplyList();
                    break;
                    
                case 3:
                    // 修改宣言
                    showSetDeclaration();
                    break;
                    
                case 4:
                    // 福利分配
                    showSupplyAssignMain();
                    break;

                case 5:
                    // 发同盟邮件
                    showMailAll();
                    break;
                    
                case 6:
                    // 设置同盟名片
                    showSetNameCard();
                    break;
                    
                case 7:
                    if (cntryInfo->cntry_delete_time > 0)
                        // 取消解散公会
                        unDeleteCountry();
                    else
                        // 公会解散
                        showDeleteCountryTip();
                    break;
                    
                default:
                    break;
            }
        
        }
        else if (cntryInfo->status == 2) {
            // viceleader
            switch (pRet->iBtnSel) {
                case 0:
                    // 会员申请
                    getMemberApplyList();
                    break;
                                    
                case 1:
                    // 修改宣言
                    showSetDeclaration();
                    break;
                    
                case 2:
                    // 发同盟邮件
                    showMailAll();
                    break;

                case 3:
                    // 设置同盟名片
                    showSetNameCard();
                    break;

                case 4:
                    // 退出公会
                    showQuitCountryTip();
                    break;
                    
                default:
                    break;
            }
        }
    }
    else if (m_curScreen == enCM_SupplyAssignMain)
    {
        switch (pRet->iBtnSel) {
            case 0:
                // 分配战利品
                m_bShowMemberList = false;
                getMemberList();
                break;
                
            case 1:
                // 分配战利元宝
                getJifengRecord();
                break;
                
            default:
                break;
        }
    }
    else if (m_curScreen == enCM_OrgFightList)
    {
        CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
        CCntryDgInfo* pDgInf = pCntryDgInfArr->getObjectAtIndex(pRet->iBtnSel);
        m_fightSel = pRet->iBtnSel;
     

        if (!pDgInf->bIsFinish)
        {
            if ( strcmp(pDgInf->dg_type, "3") == 0) {
                m_bIsLS = true;
            }
            showTeamEdit();
        }
        else
        {
            m_fightitem = pDgInf->dg_id;
            m_fightname = pDgInf->dg_name;
            m_fighttype = pDgInf->dg_type;
            m_lEnemyCid = pDgInf->cid;
            
            FightRank();
        }
    }
}

void CountryMain::cbTeamEdit()
{
    innerExit();
    
    //确保当前没有战斗进行中才可以扣体力进战场，避免宝石重叠的问题
    CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
    CCntryDgInfo* pDgInf = pCntryDgInfArr->getObjectAtIndex(m_fightSel);
    m_fightitem = pDgInf->dg_id;
    m_fightname = pDgInf->dg_name;
    m_lEnemyCid = pDgInf->cid;
    m_fighttype = pDgInf->dg_type;
    FightRank();
}

void CountryMain::showChatRoom()
{
    innerExit();
    removeChildByTag(CHATROOMTAG, true);
    
    m_curScreen = enCM_ChatRoom;
    
    ChatRoom* pChatRoom = new ChatRoom(this, callfunc_selector(CountryMain::cbChatRoom));
    addChild(pChatRoom,10,CHATROOMTAG);
    pChatRoom->release();
    
    pChatRoom->setPosition(CCPointMake(640, 0));
    pChatRoom->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_chatroom"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    //增加 管理按钮
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(sp1,
                                                             sp2,
                                                             this,
                                                             menu_selector(CountryMain::cbChatRoom));
    item->setAnchorPoint(ccp(0, 0));
    
    TextNode* lbManageTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("chatmembermanage"),31);
    lbManageTitle->setPosition(ccp(50, 38));
    lbManageTitle->setShadowColor(ccc3(0, 0, 0));
    item->addChild(lbManageTitle);
    
    m_memberManageBtn = CCMenu::menuWithItem(item);
    addChild(m_memberManageBtn, 10);
    
    innerEnter();
}

void CountryMain::cbChatRoom()
{
    if (CGameData::Inst()->getMemberListRequest())
    {
        innerExit();
        
        m_curReqType = enCReqType_MemberManage;
        scheduleUpdate();
    }
}

void CountryMain::showTeamEdit()
{
    innerExit();
    
    m_curScreen = enCM_TeamEdit;
    
    if(m_teamEdit == NULL)
    {
        CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
        CCntryDgInfo* pDgInf = pCntryDgInfArr->getObjectAtIndex(m_fightSel);
        m_teamEdit = new OrgTeamEdit(this, callfunc_selector(CountryMain::cbTeamEdit),pDgInf->dg_name);
        addChild(m_teamEdit, 10);
        m_teamEdit->release();
        
        m_teamEdit->setPosition(CCPointMake(640, 0));
        m_teamEdit->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    }
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("arena_teamcfm"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
   // m_backItem->setTag(enCntry_OrgFight);
    
//    m_miBack->setPosition(CCPointMake(-529, 731));
//    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
//    
//    MainScene::Inst()->showBottomPromptInfo(CGameData::Inst()->getLanguageValue("naviText_teamConfirm"));
    innerEnter();
}

void CountryMain::showCreateCntryTip()
{
    innerExit();
    
    m_curScreen = enCM_CreateCntry;
   
    MainScene::Inst()->showBottomPromptInfo("cntry_create_country4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_create_country"));
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::showCreateCountry));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showCntryCreateTipDialog();
    
    innerEnter();
}

//******************************************************************************
// dl2CallBack
//******************************************************************************
void CountryMain::dl2CallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(111);
    if(ret->bOk == true)
    {
        MainScene::Inst()-> gotoOtherMain();
    }else {
        dialog->removeFromParentAndCleanup(true);
    }
}


void CountryMain::showCreateCountry()
{
    if (m_cntryDialog) {
        m_cntryDialog->removeFromParentAndCleanup(true);
        m_cntryDialog = NULL;
    }
    
    if (m_cntryCreate == NULL) {
        m_cntryCreate = new CountryCreate(this, callfunc_selector(CountryMain::createCountry));
        addChild(m_cntryCreate);
        m_cntryCreate->release();
    }
    
    m_cntryCreate->showCntryCreatDialog();
    m_cntryCreate->setPosition(640, 0);
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_cntryCreate->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

void CountryMain::createCountry()
{
    if (m_cntryCreate->creatCountry()) {
        m_curReqType = enCReqType_Create;
        scheduleUpdate();
    }
}

void CountryMain::showJoinCountry()
{
    innerExit();
    
    m_curScreen = enCM_JoinCntry;
    
    MainScene::Inst()->showBottomPromptInfo("cntry_join_country4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_join_country"));

    const char* menuName[] = {CGameData::Inst()->getLanguageValue("cntry_newest_country"),
        CGameData::Inst()->getLanguageValue("cntry_strongest_country")};
    newMenuBtn(menuName, 2);
    
    innerEnter();
}

void CountryMain::showSearchCountry()
{
    innerExit();
    
    m_curScreen = enCM_SearchCntry;
   
    MainScene::Inst()->showBottomPromptInfo("cntry_search_country4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_search_country"));
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::searchCountry));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showCntrySearchDialog();
    
    innerEnter();
}

void CountryMain::searchCountry()
{    
    m_curReqType = enCReqType_Search;
    if (m_cntryDialog->searchCountry()) {
        scheduleUpdate();
    }
}

void CountryMain::showCountryList()
{
    innerExit();
    
    m_curScreen = enCM_CntryList;
    
    MainScene::Inst()->showBottomPromptInfo("cntry_country_list4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_country_list"));
    
    CCMutableArray<CCntryBaseInfo*>* cntryInfoArr = CGameData::Inst()->getCntryList();
    if (cntryInfoArr && cntryInfoArr->count() > 0)
        showAllCntryInfo();
    else
    {
        if (m_dlgNoTip == NULL) {
            m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(CountryMain::cbNoDlg));
            addChild(m_dlgNoTip);
            m_dlgNoTip->release();
            m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("cntry_no_cntry"));
            m_dlgNoTip->setPosition(CCPoint(960, 400));
            CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
            m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
    }
    
    innerEnter();
}

void CountryMain::showAllCntryInfo()
{
    CMenuBtn* newMenuBtn;
    CCMutableArray<CCntryBaseInfo*>* cntryInfoArr = CGameData::Inst()->getCntryList();
    for (int i = 0; i < cntryInfoArr->count(); i++)
    {
        if (i >= m_listCntry->getPreTopShow()
            && i <= m_listCntry->getPreBottomShow())
        {
            CCntryBaseInfo* cntryBaseInfo = cntryInfoArr->getObjectAtIndex(i);
            newMenuBtn = new CMenuBtn(cntryBaseInfo);
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("button_cntryInfo.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("button_cntryInfo.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(CountryMain::cbCountryList));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            
            CCMenu *menu = CCMenu::menuWithItem(itemShow);
            newMenuBtn->addChild(menu);
            menu->setPosition(ccp(120, -42));
        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_listCntry->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();

    }
}

void CountryMain::insertItemByIdx(cocos2d::CCObject *pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<CCntryBaseInfo*>* cntryInfoArr = CGameData::Inst()->getCntryList();
    
    if(cntryInfoArr->count() == 0)
        return;
    
    CCntryBaseInfo* cntryBaseInfo = cntryInfoArr->getObjectAtIndex(i);
    CMenuBtn* newMenuBtn = new CMenuBtn(cntryBaseInfo);
    m_listCntry->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("button_cntryInfo.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("button_cntryInfo.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(CountryMain::cbCountryList));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(ccp(120, -42));
}

void CountryMain::cbCountryList(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_listSel = mItem->getTag() / 10;    
    CCntryBaseInfo* cntryInfo = CGameData::Inst()->getCntryList()->getObjectAtIndex(m_listSel);

    CCountryInfo* myCntryInfo = CGameData::Inst()->getCntryInfo();
    if (strcmp(myCntryInfo->cid, cntryInfo->cid) == 0)
        showCountryDetails();
    else
        showOneCntryDetails(cntryInfo);
}

void CountryMain::showOneCntryDetails(CCntryBaseInfo *cntryInfo)
{
    innerExit();
    
    if (m_cntryDetails == NULL) {
        m_cntryDetails = new CountryDetails(this, menu_selector(CountryMain::cbOneCntryDetails));
        addChild(m_cntryDetails, 100);
        m_cntryDetails->release();
    }
    
    m_cntryDetails->showOneCntryDetails(cntryInfo);
    
    m_cntryDetails->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_cntryDetails->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

void CountryMain::cbOneCntryDetails(cocos2d::CCObject *pObj)
{
    CCMenuItem* menuItem = (CCMenuItem*)pObj;
    
    switch (menuItem->getTag()) {
        case enCDBtn_Back:
        {
            if (m_curScreen == enCM_CntryRank)
                // 返回countryRank
                showCountryRank();
            else if (m_curScreen == enCM_CntryList)
                // 返回countryList
                showCountryList();
            else if (m_curScreen == enCM_SearchCntry)
                // 返回countrySearch;
                showSearchCountry();
        }
            break;
         
        case enCDBtn_Apply:
        {
            //判断是否已经绑定用户，如果没有，那么提示去绑定
            if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
            {
                Dialog2 *dialog = new Dialog2(this,
                                              callfuncO_selector(CountryMain::dl2CallBack),
                                              CGameData::Inst()->getLanguageValue("cntry_authened"),
                                              CGameData::Inst()->getLanguageValue("cntry_authened_info"),
                                              enDl2TwoBtn);
                addChild(dialog,100);
                dialog->setTag(111);
                dialog->setPosition(ccp(320, 480));
                dialog->release();
            }
            else
            {
                applyCountry();
            }
        }
            
        default:
            break;
    }
}

void CountryMain::applyCountry()
{
    CCntryBaseInfo* cntryInfo;
    if (m_curScreen == enCM_CntryList) {
        cntryInfo = CGameData::Inst()->getCntryList()->getObjectAtIndex(m_listSel);
    }
    else if (m_curScreen == enCM_SearchCntry) {
        cntryInfo = CGameData::Inst()->getSchCntryInfo();
        if (!cntryInfo) {
            return;
        }
    }
    
    if (CGameData::Inst()->applyCountryRequest(cntryInfo->cid)) {
        m_curReqType = enCReqType_Apply;
        scheduleUpdate();
    }
}

void CountryMain::showCountryRank()
{
    innerExit();
    
    m_curScreen = enCM_CntryRank;
    
    MainScene::Inst()->showBottomPromptInfo("cntry_country_rank4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_country_rank"));
    
    showAllCntryInfo();
    innerEnter();
}

void CountryMain::showSetDeclaration()
{
    innerExit();
    
    m_curScreen = enCM_SetDeclaratin;
    
    MainScene::Inst()->showBottomPromptInfo("cntry_change_declaration4_1");
 //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_change_declaration"));
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::setDeclaration));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSetDeclarationDialog();
    
    innerEnter();
}

void CountryMain::setDeclaration()
{
    if (m_cntryDialog->setDeclaration()) {
        m_curReqType = enCReqType_SetDeclaration;
        scheduleUpdate();
    }else{
        m_cntryDialog->showFbdwords();
    }
}


void CountryMain::showCountryManage()
{
    innerExit();
    
    m_curScreen = enCM_CntryManage;
   
    MainScene::Inst()->showBottomPromptInfo("cntry_manage4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_manage"));
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    if (cntryInfo->status == 3) {
        memberManageMenu();
    }
    else if (cntryInfo->status == 2) {
        viceCmndManageMenu();
    }
    else if (cntryInfo->status == 1) {
        commandManageMenu();
    }
    
    innerEnter();
}

void CountryMain::memberManageMenu()
{
    const char* menuName[] = {
        CGameData::Inst()->getLanguageValue("cntry_cardName"),
        CGameData::Inst()->getLanguageValue("cntry_quit_country"),};
    newMenuBtn(menuName, 2);
}

void CountryMain::viceCmndManageMenu()
{
    const char* menuName[] = {
        CGameData::Inst()->getLanguageValue("cntry_apply"),
        CGameData::Inst()->getLanguageValue("cntry_change_declaration"),
        CGameData::Inst()->getLanguageValue("cntry_mail_all"),
        CGameData::Inst()->getLanguageValue("cntry_cardName"),
        CGameData::Inst()->getLanguageValue("cntry_quit_country")};
    newMenuBtn(menuName, 5);
}

void CountryMain::commandManageMenu()
{
    char buf[100];
    if (CGameData::Inst()->getCntryInfo()->cntry_delete_time > 0)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_dismis_country_cancel"));
    else
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_dismis_country"));
    const char* menuName[] = {
        CGameData::Inst()->getLanguageValue("cntry_up"),
        CGameData::Inst()->getLanguageValue("cntry_promotion_0"),
        CGameData::Inst()->getLanguageValue("cntry_apply"),
        CGameData::Inst()->getLanguageValue("cntry_change_declaration"),
        CGameData::Inst()->getLanguageValue("cntry_deliver_p"),
        CGameData::Inst()->getLanguageValue("cntry_mail_all"),
        CGameData::Inst()->getLanguageValue("cntry_cardName"),
        buf, };
    newMenuBtn(menuName, 8);
}

void CountryMain::showMailAll()
{
    CUserInfo* userInfo = CGameData::Inst()->getUsrInfo();
    CommonInfo* commonInfo = CGameData::Inst()->getCommonInfo();
    if (commonInfo->mail_all_coin == 0)
        showMailAllController();
    else if (userInfo->gold >= commonInfo->mail_all_coin)
        showMailAllCoinTips();
    else
        showAlertNoGold();
}

void CountryMain::showMailAllCoinTips()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_mail_all_gold"),
                 CGameData::Inst()->getCommonInfo()->mail_all_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbMailAllCoinTips),
                               CGameData::Inst()->getLanguageValue("cntry_mail_all"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbMailAllCoinTips(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true)
        showMailAllController();
    else {
        // 取消
    }
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::showMailAllNoCoinTips()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_mail_all_shop"),
                 CGameData::Inst()->getUsrInfo()->coin,
                 CGameData::Inst()->getCommonInfo()->mail_all_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbMailAllNoCoinTips),
                               CGameData::Inst()->getLanguageValue("arenaTip8"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbMailAllNoCoinTips(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
    
    if(pRet->bOk == true)
        MainScene::Inst()->goToShop();
    else {
        // 取消
    }
}

void CountryMain::showAlertNoGold()
{
    if (m_dlgRecover == NULL)
    {
        char buf[100];
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cntry_mail_all_no_gold"),
                 CGameData::Inst()->getCommonInfo()->mail_all_coin);
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbAlertNoGold),
                                   buf,
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbAlertNoGold()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
}

void CountryMain::showMailAllController()
{
    m_bMailAll = true;
    OcProxy::Inst()->showMailViewController(CGameData::Inst()->getLanguageValue("cntry_mail_all_name"));
    schedule(schedule_selector(CountryMain::updateMail));
}

void CountryMain::showCountryDonate()
{
    innerExit();
    
//    char buf[100];
//    snprintf(buf, 99, "cntry_country_donate4_%d", int(CCRANDOM_0_1() * 2) + 1);
    

  
    MainScene::Inst()->showBottomPromptInfo("cntry_country_donate4_");
  //    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_country_donate"));

    m_curScreen = enCM_CntryDonate;

    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::donateCoin));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showDonateCoinDialog();
    
    innerEnter();
}

void CountryMain::donateCoin()
{
    m_curReqType = enCReqType_DonateCoin;
    if (m_cntryDialog->donateCoin()) {
        scheduleUpdate();
    }
}

void CountryMain::showWelfareList()
{
    innerExit();
    
//    char buf[100];
//    snprintf(buf, 99, "cntry_exchange_p4_%d", int(CCRANDOM_0_1() * 2) + 1);
    
    
    MainScene::Inst()->showBottomPromptInfo("cntry_exchange_p4_");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_exchange_p"));
    
    m_curScreen = enCM_GetWelfare;
    
    m_listWelfare = new CListLayer(this, callfuncO_selector(CountryMain::cbWelfareList),
                                   4, 145.0, 660.0);
    addChild(m_listWelfare, -1);
    m_listWelfare->release();
    
    CCMutableArray<CWelfareInfo*>* allWelfareInfo = CGameData::Inst()->getWelfareInfoList();
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    if (m_welfareInfoArr == NULL)
        m_welfareInfoArr = new CCMutableArray<CWelfareInfo*>();
    m_welfareInfoArr->removeAllObjects();
    
    if (allWelfareInfo && allWelfareInfo->count() > 0)
    {
        for (int i = 0; i < allWelfareInfo->count(); i++) {
            CWelfareInfo* welfareInfo = allWelfareInfo->getObjectAtIndex(i);
            
            if (welfareInfo->needLv > cntryInfo->lv)
                continue;
            
            m_welfareInfoArr->addObject(welfareInfo);
            
            CMenuBtn* menuBtn = new CMenuBtn(welfareInfo);
            m_listWelfare->push_back(menuBtn);
            menuBtn->release();
        }
    }
    
    innerEnter();
}


void CountryMain::cbWelfareList(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    CListLayerRet* pRet = (CListLayerRet*)pObj;
    m_listSel = pRet->iBtnSel;
    

    
    CWelfareInfo* welfareInfo = m_welfareInfoArr->getObjectAtIndex(m_listSel);
    CCountryInfo* contryInfo = CGameData::Inst()->getCntryInfo();
    
    if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
    {
        innerExit();
        
        MainScene::Inst()->showFullPackDlg(true);
    }
    else
    {
        long lastWelfareTime = CGameData::Inst()->getLastWelfareTime(welfareInfo->wid);
        if (lastWelfareTime > 0)
        {
            long retainTime = lastWelfareTime + welfareInfo->cd_time * 3600 - CGameData::Inst()->getCurrentTime();
            if (retainTime > 0)
            {
                showInCdTimeTip();
                return;
            }
        }
        
        if (contryInfo->user_supply >= welfareInfo->needSupply)
            showExchangeTip();
    }
}

void CountryMain::showInCdTimeTip()
{
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbInCdTimeTip),
                                   CGameData::Inst()->getLanguageValue("cntry_welfare_cd_tips"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbInCdTimeTip()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
}

void CountryMain::showExchangeTip()
{
    m_dlgTip = new Dialog2(this,
                           callfuncO_selector(CountryMain::cbExchangeTip),
                           CGameData::Inst()->getLanguageValue("cntry_hint"),
                           CGameData::Inst()->getLanguageValue("cntry_exchange_p0"),
                           enDl2TwoBtn);
    addChild(m_dlgTip, 100);
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->release();
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbExchangeTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true){
        // 兑换福利
        exchangeWelfare();
    }
    else {
        // 取消
        
    }
    
    if (m_dlgTip)
    {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::exchangeWelfare()
{
    CWelfareInfo* welfareInfo = m_welfareInfoArr->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->exchangeWelfareRequest(welfareInfo->wid))
    {
        m_curReqType = enCReqType_ExchagneWelfare;
        scheduleUpdate();
    }
}

void CountryMain::showGacha()
{
    innerExit();
    
    bool bGachaPlus = (CGameData::Inst()->getGachaRet()->plus>0) ? true : false;

    if (m_gachaLayer == NULL) {
        m_gachaLayer = new GachaGet(UnusualGachaType,
                                    CGameData::Inst()->getGachaRet()->ucid,
                                    this,
                                    callfuncO_selector(CountryMain::cbGacha),
                                    bGachaPlus);
        addChild(m_gachaLayer,200);
        m_gachaLayer->release();
    }
    
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
}

void CountryMain::cbGacha()
{
    if (m_gachaLayer)
    {
        m_gachaLayer->removeFromParentAndCleanup(true);
        m_gachaLayer = NULL;
        
        showWelfareList();
                
        CUserCardInfo* cardInfo = CGameData::Inst()->getUsrCardInfByUcid(CGameData::Inst()->getGachaRet()->ucid);
        m_showCard->show(cardInfo);
    }
}

void CountryMain::getAllCards()
{
    if (CGameData::Inst()->allCardRequest()) {
        m_curReqType = enCReqType_AllCards;
        scheduleUpdate();
    }
}

void CountryMain::showSupplyAssignMain()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_deliver_p"));
    
    m_curScreen = enCM_SupplyAssignMain;
    
    const char* menuName[] = {
        CGameData::Inst()->getLanguageValue("cntry_putongfenpei"),
        CGameData::Inst()->getLanguageValue("cntry_jifengfenpei"),};
    newMenuBtn(menuName, 2);
    
    innerEnter();
}

void CountryMain::showSupplyAssignList()
{
    innerExit();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
 //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_putongfenpei"));
    
    m_curScreen = enCM_SupplyAssignList;
            
    if (m_memberList == NULL) {
        m_memberList = new PagingListLayer(this, callfuncO_selector(CountryMain::cbSupplyAssignList));
        addChild(m_memberList);
        m_memberList->release();
    }
    
    showRangeBtn();
    
    CGameData::Inst()->changeMemberListRange(false);
    m_memberList->showList(enMemberListType_assign,m_pLocal,m_listSel);
    m_pLocal = 0;
    m_listSel = 0;
    showMListRangeTitle();
            
    innerEnter();
}

void CountryMain::cbSupplyAssignList(cocos2d::CCObject *pObj)
{
    PagingListRet* pRet = (PagingListRet*)pObj;
    m_listSel = pRet->iBtnSel;
    m_pLocal = pRet->pLocal;

    if (m_listSel == 0)
        showSupplyAssignAllDialog();
    else if (m_listSel == 1)
        showSupplyAssignOutDialog();
    else if (2==m_listSel)
        showSupplyAssignWeekDialog();
    else
        showSupplyAssignOneDilog();
}

void CountryMain::showSupplyAssignOutDialog()
{
    innerExit();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
//    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_deliver_p"));
    
    m_curScreen = enCM_SupplyAssignDilog;
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::assignOutSupply));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSupplyAssignOutDialog();
    
    innerEnter();
}

void CountryMain::showSupplyAssignWeekDialog()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_deliver_p"));
    
    m_curScreen = enCM_SupplyAssignDilog;
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::assignWeekSupply));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSupplyAssignWeekDialog();
    
    innerEnter();
}

void CountryMain::showSupplyAssignOneDilog()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_deliver_p"));
    
    m_curScreen = enCM_SupplyAssignDilog;
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::assignOneSupply));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel - 3);
    m_cntryDialog->showSupplyAssignOneDialog(memberInfo);
    
    innerEnter();
}

void CountryMain::assignOneSupply()
{
    if (m_cntryDialog->assignOneSupply())
    {
        m_curReqType = enCReqType_AssignOneSupply;
        scheduleUpdate();
    }
}

void CountryMain::showSupplyAssignAllDialog()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_share_p4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_deliver_p"));
    
    m_curScreen = enCM_SupplyAssignDilog;
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::assignAllSupply));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showSupplyAssignAllDialog();
    
    innerEnter();
}

void CountryMain::assignAllSupply()
{
    if (m_cntryDialog->assignAllSupply())
    {
        m_curReqType = enCReqType_AssignAllSupply;
        scheduleUpdate();
    }
}

void CountryMain::assignOutSupply()
{
    if (m_cntryDialog->assignOutSupply())
    {
        m_curReqType = enCReqType_AssignOutSupply;
        scheduleUpdate();
    }
}

void CountryMain::assignWeekSupply()
{
    if (m_cntryDialog->assignWeekSupply())
    {
        m_curReqType = enCReqType_AssignWeekSupply;
        scheduleUpdate();
    }
}


void CountryMain::showOrgFightList()
{
    innerExit(); // 国家保卫战
    
//    char buf[100];
//    snprintf(buf, 99, "cntry_country_fight4_%d", int(CCRANDOM_0_1() * 2) + 1);
    
    
    m_bIsLS = false;
    MainScene::Inst()->showBottomPromptInfo("cntry_country_fight4_");
//    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_country_fight"));

    m_curScreen = enCM_OrgFightList;
    
    CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
    
    for (int i = 0; i < pCntryDgInfArr->count(); i++)
    {
        CCntryDgInfo* cntryInfo = pCntryDgInfArr->getObjectAtIndex(i);
        
        CMenuBtn* menuBtn = new CMenuBtn(cntryInfo);
        m_list->push_back(menuBtn);
        menuBtn->setTag(i);
        menuBtn->release();
        
        // 开启 button
        CCMenuItem* item = NULL;
        if (strcmp(cntryInfo->dg_type, "1") == 0)
        {
            // 疾不显示按钮
        }
        else if (strcmp(cntryInfo->dg_type, "3") == 0){//同盟练手场
            if (CGameData::Inst()->getCntryInfo()->status == 1
                || CGameData::Inst()->getCntryInfo()->status == 2) {
                if (cntryInfo->bIsFinish) {
                    CCSprite* sp7 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                    CCSprite* sp8 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                    item = CCMenuItemImage::itemFromNormalSprite(sp7,sp8,this, menu_selector(CountryMain::cbOrgFightList));
                    item->setTag(kCntryKaiQi);
                    item->setAnchorPoint(CCPointZero);
                    item->setPosition(CCPointMake(140, -18));
                    item->setScale(0.7);

                    TextNode* text9 = NULL;
                    if (cntryInfo->closed_time - CGameData::Inst()->getCurrentTime() <= 0) {
                         text9 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_payopen"), sp7->getContentSize(), CCTextAlignmentCenter, 28);
                    }else{
                        text9 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_freeopen"), sp7->getContentSize(), CCTextAlignmentCenter, 28);
                    }
                    
                    text9->setColor(ccWHITE);
                    text9->setShadowColor(ccBLACK);
                    text9->setPosition(CCPointMake(sp7->getContentSize().width/2, sp7->getContentSize().height/2+3));
                    item->addChild(text9);
                    CCMenu* menu0 = CCMenu::menuWithItems(item, NULL);
                    menu0 -> setTag(kCntryFightListItemMenu);
                    menu0 -> setPosition(CCPointMake(0, 0));
                    menuBtn -> addChild(menu0);
                }
            }
            else{
                if (cntryInfo->bIsFinish){
                    TextNode* text4 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_open_"), kBtnTitleHeight);
                    text4 -> setColor(ccWHITE);
                    text4 -> setShadowColor(ccBLACK);
                    text4 -> setTag(kCntryKaiQi_);
                    text4 -> setPosition(CCPointMake(150, 0));
                    text4 -> setAnchorPoint(CCPointZero);
                    text4 -> setScale(0.7);
                    menuBtn -> addChild(text4);
                }
                
            }
            
        }
        else if (CGameData::Inst()->getCntryInfo()->status == 1
                 || CGameData::Inst()->getCntryInfo()->status == 2){
            if (cntryInfo->bIsFinish){
                CCSprite* sp5 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                CCSprite* sp6 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                item = CCMenuItemImage::itemFromNormalSprite(sp5, sp6, this, menu_selector(CountryMain::cbOrgFightList));
                item -> setTag(kCntryKaiQi);
                item -> setAnchorPoint(CCPointZero);
                item -> setPosition(CCPointMake(140, -18));
                item -> setScale(0.7);
                TextNode* text3 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_open"), sp5->getContentSize(), CCTextAlignmentCenter, kBtnTitleHeight);
                text3 -> setColor(ccWHITE);
                text3 -> setShadowColor(ccBLACK);
                text3 -> setPosition(CCPointMake(sp5->getContentSize().width/2, sp5->getContentSize().height/2+3));
                item -> addChild(text3);
                
                // ccmenu
                CCMenu* menu0 = CCMenu::menuWithItems(item, NULL);
                menu0 -> setTag(kCntryFightListItemMenu);
                menu0 -> setPosition(CCPointMake(0, 0));
                menuBtn -> addChild(menu0);
            }
        }
        else{
            if (cntryInfo->bIsFinish){
                TextNode* text4 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_open_"), kBtnTitleHeight);
                text4 -> setColor(ccWHITE);
                text4 -> setShadowColor(ccBLACK);
                text4 -> setTag(kCntryKaiQi_);
                text4 -> setPosition(CCPointMake(150, 0));
                text4 -> setAnchorPoint(CCPointZero);
                text4 -> setScale(0.7);
                menuBtn -> addChild(text4);
            }
        }
    }

    
    //recoverPower  menuitem
    CCSprite* recoverNormal = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite* recoverSelected = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    CCMenuItem  *  resetMenuItem = CCMenuItemImage::itemFromNormalSprite(recoverNormal,
                                                                         recoverSelected,
                                                                         this,
                                                                         menu_selector(CountryMain::btnRecoverPower));
    resetMenuItem->setAnchorPoint(CCPointZero);
    

    TextNode* tnRecover = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_recover"),
                                                   recoverNormal->getContentSize(),
                                                   CCTextAlignmentCenter,
                                                   24);
    tnRecover->setShadowColor(ccBLACK);
    tnRecover->setPosition(CCPointMake(recoverNormal->getContentSize().width * 0.5,
                                       recoverNormal->getContentSize().height * 0.5 - 28));
    resetMenuItem->addChild(tnRecover);
    
    CCMenu   *    resetMenu  = CCMenu::menuWithItem(resetMenuItem);
    resetMenu->setAnchorPoint(CCPointZero);
    resetMenu->setPosition(CCPointMake(1000,  960 -227));
    
    resetMenu->setTag(kCntryRecoverPowerMenu);
    
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(640 - recoverNormal->getContentSize().width, 960 - 227));
    resetMenu->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    
    this->addChild(resetMenu, 100);
  
    innerEnter();
}

void CountryMain::recoverActionPoint()
{
    if (CGameData::Inst()->recoverPowerRequest())
    {
        m_curReqType = enCReqType_ReqRecoverPower;
        scheduleUpdate();
    }
}



void CountryMain::cbOrgFightList(CCObject* pObj)
{
    CCMenuItem* menuItem = (CCMenuItem*)pObj;
    int int1 = menuItem -> getTag();
    int int3 = menuItem->getParent()->getParent()->getTag();
    
    CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
    CCntryDgInfo* Cntry0 = pCntryDgInfArr->getObjectAtIndex(int3);
    m_fightitem = Cntry0->dg_id;
    m_fightname = Cntry0->dg_name;
    m_lEnemyCid = Cntry0->cid;
    m_fighttype = Cntry0->dg_type;
    if (int1 == kCntryKaiQi)    // 点击 开启 按钮
    {
        bool bShowSureDlg = true;
        long lCurTime = CGameData::Inst()->getCurrentTime();
        if (m_fighttype.compare("0") == 0)
        {
            if (Cntry0->bIsFinish
                && (Cntry0->end_time + Cntry0->cold_time * 60 - lCurTime > 0))
                bShowSureDlg = false;
        }
        else if (m_fighttype.compare("2") == 0)
        {
            if (lCurTime < Cntry0->start_time
                || (lCurTime > Cntry0->start_time + Cntry0->last_time * 3600))
                bShowSureDlg = false;
        }
        else if (m_fighttype.compare("3") == 0)
        {
            if (Cntry0->bIsFinish == false) {
                bShowSureDlg = false;
            }else
                m_bIsLS = true;
        }
        
        if (bShowSureDlg)
            showSureOpenOrgFight();
        else{
            showNotOpenOrgFight();
        }
    }
}




void CountryMain::showSureOpenOrgFight()
{
    char buf[100];
    snprintf(buf, 99,
             CGameData::Inst()->getLanguageValue("jiFengLuanWu_sure"),
             m_fightname.c_str());
    m_dlgTip = new Dialog2(this,
                           callfuncO_selector(CountryMain::cbSureOpenOrgFight),
                           CGameData::Inst()->getLanguageValue("cntry_hint"),
                           buf,
                           enDl2TwoBtn);
    addChild(m_dlgTip, 100);
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->release();
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));

}

void CountryMain::cbSureOpenOrgFight(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true){
        // 开启保卫战
        FightOpen();
    }
    else {
        // 取消
        m_bIsLS = false;
    }
    
    if (m_dlgTip)
    {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::showNotOpenOrgFight()
{
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbNotOpenOrgFight),
                                   CGameData::Inst()->getLanguageValue("jiFengLuanWu_not"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbNotOpenOrgFight()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
}

void CountryMain::showAcPointNotEnoughTip()
{
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_notEnough"),
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbAcPointNotEnoughTip),
                               CGameData::Inst()->getLanguageValue("actionPoint_notEnough_title"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbAcPointNotEnoughTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }

    if(pRet->bOk == true)
    {
        if (CGameData::Inst()->getUsrInfo()->coin >= CGameData::Inst()->getCommonInfo()->recover_action_coin)
            recoverActionPoint();
        else
            showNoCoinForRecover();
    }
    else {
        reqCntryDgCfg();
    }
}



void CountryMain::FightOpen()
{

    
//    if (CGameData::Inst()->cntryDgOpenReq(m_fightitem,false));
//    {
//        m_curReqType = enCReqType_CntryOpen;
//        scheduleUpdate();
//    }
    
    if (CGameData::Inst()->cntryDgOpenReq(m_fightitem,m_bIsLS));
    {
        m_curReqType = enCReqType_CntryOpen;
        scheduleUpdate();
    }

}
void CountryMain::FightRank()
{
    if (CGameData::Inst()->cntryDgRankReq(m_fightitem, m_fighttype));
    {
        m_curReqType = enCReqType_CntryRank;
        scheduleUpdate();
    }
}

void CountryMain::enterFightCfm()
{
    innerExit();
    
    OrgFightCfm* pFightCfm = new OrgFightCfm(this, callfuncO_selector(CountryMain::cbFightCfm), m_fightitem);
    addChild(pFightCfm, 0, kOrgFightCfmTag);
    pFightCfm->release();
}

void CountryMain::cbFightCfm(CCObject* pObj)
{
    OrgFightCfmRet* pCfmRet = (OrgFightCfmRet*)pObj;
    
    if (pCfmRet->bOk) {
        if (CGameData::Inst()->getLocalAcPoint() > 0)
        {
            char buf[16];
            snprintf(buf, 15, "%ld", CGameData::Inst()->getCurrentTime());
            m_strDgRtyId = buf;
            this->FightJoin();
        }
        else
            showAcPointNotEnoughTip();
    }
    else
    {
        m_bBackToFightRank = false;
        
        innerExit();
        
        CCMutableArray<CCntryDgInfo* > * pCntryDgInfArr = CGameData::Inst()->getCntryDgInfArr();
        CCntryDgInfo* pDgInf = pCntryDgInfArr->getObjectAtIndex(m_fightSel);
        if (!pDgInf->bIsFinish)
        {
            showTeamEdit();
        }
        else
        {
          //  showOrgFightList();
            reqCntryDgCfg();
        }
    }
    
    removeChildByTag(kOrgFightCfmTag, true);
}



void CountryMain::FightJoin()
{
//    if (CGameData::Inst()->cntryDgJoinReq(m_fightitem, m_strDgRtyId,false));
//    {
//        m_curReqType = enCReqType_CntryJoin;
//        scheduleUpdate();
//    }
    if (CGameData::Inst()->cntryDgJoinReq(m_fightitem, m_strDgRtyId,m_bIsLS));
    {
        m_curReqType = enCReqType_CntryJoin;
        scheduleUpdate();
    }

    
}

void CountryMain::enterOrgFight()
{
    innerExit();
    
    CGameData::Inst()->releaseUserCardsSprite();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CGameData::Inst()->addBigImageByRGBA4444("fightAni.plist");
    CGameData::Inst()->addBigImageByRGBA4444("fightAni_2.plist");
    CGameData::Inst()->addBigImageByRGBA4444("guangquan.plist");
    CGameData::Inst()->addBigImageByRGBA4444("orgfightres.plist");
        
    if (OrgSaveData::Inst()->hasValidSavedData())
    {
        m_fightitem = string(OrgSaveData::Inst()->getFightId());
        m_fightname = string(OrgSaveData::Inst()->getFightName());
        m_fighttype = string(OrgSaveData::Inst()->getFightType());
        m_lEnemyCid = OrgSaveData::Inst()->getEnemyCid();
    }
    else {
        OrgSaveData::Inst()->setEnemyCid(m_lEnemyCid);
        OrgSaveData::Inst()->setFightId(m_fightitem.c_str());
        OrgSaveData::Inst()->setFightName(m_fightname.c_str());
        OrgSaveData::Inst()->setFightType(m_fighttype.c_str());
    }
    
    if(m_orgFightMngr == NULL){
        m_orgFightMngr = OrgFightMngr::Instance();
        addChild(m_orgFightMngr, 99);
        m_orgFightMngr->release();
//        m_orgFightMngr->init(this,
//                             callfuncO_selector(CountryMain::cbFight),
//                             m_fightitem,
//                             m_lEnemyCid);
        m_orgFightMngr->init(this,
                             callfuncO_selector(CountryMain::cbFight),
                             m_fightitem,
                             m_lEnemyCid,m_bIsLS);
    }

    m_orgFightMngr->startNewFight();
    
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
    
    MainScene::Inst()->switchBgMusic(enBgMusicOrgFight);
}

void CountryMain::cbFight(CCObject* pObj)
{
    OrgFightRet* pRet = (OrgFightRet*)pObj;
    m_bCashin = pRet->bCashin;
    
    removeChild(m_orgFightMngr, true);
    m_orgFightMngr = NULL;
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("guangquan.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("orgfightres.plist");
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    if (m_bCashin || CGameData::Inst()->chkLogin()) // 不是读取进度,直接退出战场
    {
        MainScene::Inst()->showCntryInfo();
        MainScene::Inst()->showUsrInfPanel(true);
    }

    schedule(schedule_selector(CountryMain::updateLoadUserCardsSprite));
   // CGameData::Inst()->loadUserCardsSprite();
}

void CountryMain::updateLoadUserCardsSprite(ccTime dt)
{
    bool bLoadCardEnd = true;
    bLoadCardEnd = CGameData::Inst()->loadUserCardsStep(10);
    if(!bLoadCardEnd)
    {
        if(m_commDlg == NULL)
        {
            m_commDlg = new CommDlg(this, callfuncO_selector(CountryMain::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
    }
    else
    {
        if(m_commDlg != NULL)
        {
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        unschedule(schedule_selector(CountryMain::updateLoadUserCardsSprite));
        
        if(m_bCashin){
            OrgFightCashIn* cashin = new OrgFightCashIn(this, callfunc_selector(CountryMain::cbCashin));
            addChild(cashin, 99, kOrgCashinTag);
            cashin->release();
            cashin->show(m_fightname.c_str());
        }
        else{
            cbCashin();
        }
    }
}

void CountryMain::cbCashin()
{
    CCNode* node = getChildByTag(kOrgCashinTag);
    if(node)
        removeChild(node, true);
    
    OrgSaveData::Inst()->destorySavedFile();
    
#ifdef TalkData
    if(CGameData::Inst()->getCntryDgEndInf())//shikun
    {
        if (CGameData::Inst()->getCntryDgEndInf() && CGameData::Inst()->getCntryDgEndInf()->whokillwho == 1)
        {
            char msg[60];
            sprintf(msg, "%s&&%s&&%s TM" , m_fightitem.c_str() , m_fightname.c_str() , m_fighttype.c_str());
            
            
            TDCCMission::onCompleted(msg);
        }
        else
        {
            char msg[60];
            sprintf(msg, "%s&&%s&&%s TM" , m_fightitem.c_str() , m_fightname.c_str() , m_fighttype.c_str());
            
            
            TDCCMission::onFailed(msg , "fail");
            
        }
    }
#else
    
#endif
    
    
    
    
    
    if (!CGameData::Inst()->chkLogin())
    {
        MainScene::Inst()->reLogin();
    }
    else
    {
        m_bBackToFightRank = true;
        OrgSaveData::Inst()->destorySavedData();

        MainScene::Inst()->showBottomPanel(true);
        reqCntryDgCfg();
        
        MainScene::Inst()->switchBgMusic(enBgMusicOrgEnter);
    }
}

void CountryMain::showAcPointFullTip()
{    
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbAcPointFullTip),
                                   CGameData::Inst()->getLanguageValue("actionPoint_full"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    
}

void CountryMain::cbAcPointFullTip()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
}

void CountryMain::showRecoverAcPointTip()
{    
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_recover"),
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbRecoverAcPointTip),
                               CGameData::Inst()->getLanguageValue("actionPoint_recover_title"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbRecoverAcPointTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk == true)
        recoverActionPoint();
    else {
        // 取消
    }
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

// 提示捐献成功
void CountryMain::showDonateSuccess()
{
    innerExit();
    
    if (m_dlgRecover == NULL)
    {
        m_dlgRecover = new Dialog1(this,
                                   callfuncO_selector(CountryMain::cbGotoCountryMain),
                                   CGameData::Inst()->getLanguageValue("cntry_donate_ok"),
                                   false);
        addChild(m_dlgRecover, 100);
        m_dlgRecover->release();
    }
    
    m_dlgRecover->setPosition(CCPointMake(320, 480));
    m_dlgRecover->setScale(0.0);
    m_dlgRecover->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbGotoCountryMain()
{
    if (m_dlgRecover)
    {
        m_dlgRecover->removeFromParentAndCleanup(true);
        m_dlgRecover = NULL;
    }
    
    showMainOrNot();
}

void CountryMain::showNoCoinForRecover()
{    
    if(m_dlgTip == NULL){
        char buf[100];
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("actionPoint_coin_notEnough"),
                 CGameData::Inst()->getUsrInfo()->coin,
                 CGameData::Inst()->getCommonInfo()->recover_action_coin);
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbNoCoinForRecover),
                               CGameData::Inst()->getLanguageValue("arenaTip8"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}


void CountryMain::cbNoCoinForRecover(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
    
    if(pRet->bOk == true)
        MainScene::Inst()->goToShop();
    else {
        // 取消
    }
}

void CountryMain::showQuitCountryTip()
{
    if (m_dlgTip == NULL)
    {
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbShowQuitCntryTip),
                               CGameData::Inst()->getLanguageValue("cntry_hint"),
                               CGameData::Inst()->getLanguageValue("cntry_hintcontent1"),
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }

    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbShowQuitCntryTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        // 退出公会
        quitCountry();
    }
    else {
        // 取消
        showCountryManage();
    }
    
    if (m_dlgTip)
    {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::quitCountry()
{
    if (CGameData::Inst()->quitCountryRequest()) {
        m_curReqType = enCReqType_QuitCntry;
        scheduleUpdate();
    }
}

void CountryMain::showDeleteCountryTip()
{
    if (m_dlgTip == NULL)
    {
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(CountryMain::cbShowDeleteCntryTip),
                               CGameData::Inst()->getLanguageValue("cntry_hint"),
                               CGameData::Inst()->getLanguageValue("cntry_hintcontent2"),
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }

    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CountryMain::cbShowDeleteCntryTip(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        // 解散公会
        deleteCountry();
    }
    else {
        // 取消
        
    }
    
    if (m_dlgTip)
    {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}

void CountryMain::deleteCountry()
{
    if (CGameData::Inst()->deleteCountryRequest()) {
        m_curReqType = enCReqType_DeleteCntry;
        scheduleUpdate();
    }
}

void CountryMain::unDeleteCountry()
{
    if (CGameData::Inst()->unDeleteCountryRequest()) {
        m_curReqType = enCReqType_UnDeleteCntry;
        scheduleUpdate();
    }
}

void CountryMain::showMemberApplyList()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_apply4_1");
 //    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_apply"));
    
    m_curScreen = enCM_MemberApply;
    
    CCMutableArray<CFriendInf*>* memberApplyArr = CGameData::Inst()->getMemberApplyList();
    
    if (memberApplyArr && memberApplyArr->count() > 0) {
        for (int i = 0; i < memberApplyArr->count(); i++) {
            CFriendInf *fInfo = memberApplyArr->getObjectAtIndex(i);
            CMenuBtn *btn = new CMenuBtn(fInfo, enFriendRangePro, true);
            m_list->push_back(btn);
            btn->release();
            
            CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button_yes.png");
            CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button_yes.png");
            CCMenuItem *itemAccpet = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2, this, menu_selector(CountryMain::cbMemberApplyList));
            itemAccpet->setTag(i*10+2);
            
            spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button_no.png");
            spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button_no.png");
            CCMenuItem *itemRefuse = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CountryMain::cbMemberApplyList));
            itemRefuse->setPosition(ccp(80, 0));
            itemRefuse->setTag(i*10+3);
            
            itemAccpet->setAnchorPoint(ccp(0, 0));
            itemRefuse->setAnchorPoint(ccp(0, 0));
            CCMenu *menu = CCMenu::menuWithItems(itemAccpet,itemRefuse,NULL);
            btn->addChild(menu);
            menu->setPosition(ccp(120, -40));
        }
    }
    else
    {
        if (m_dlgNoTip == NULL) {
            m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(CountryMain::cbNoDlg));
            addChild(m_dlgNoTip);
            m_dlgNoTip->release();
            m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("cntry_no_request"));
            m_dlgNoTip->setPosition(CCPoint(960, 400));
            CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
            m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
    }
    
    innerEnter();
}

void CountryMain::cbMemberApplyList(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getMemberApplyList();
    CCMenuItemImage *temItem = (CCMenuItemImage *)pObj;
    int tag = temItem->getTag();
    m_listSel = tag / 10;
    CFriendInf *friendInfo = array->getObjectAtIndex(m_listSel);
    if(tag%10==2)
    {
        // 接受请求
        showFriendInfoLayer(memLayerType_accept, friendInfo);
    }
    else if(tag %10 == 3)
    {
        // 拒绝请求
        showFriendInfoLayer(memLayerType_refuse, friendInfo);
    }
}

void CountryMain::showFriendInfoLayer(MemberInfoLayerType memInfoLayerType, CFriendInf *friendInfo)
{
    innerExit();
    
    if (m_memInfoLayer == NULL) {
        m_memInfoLayer = new MemberInfoLayer(this, menu_selector(CountryMain::cbMemberInfoLayer));
        addChild(m_memInfoLayer);
        m_memInfoLayer->release();
    }
    
    m_memInfoLayer->showFriendInfoLayer(memInfoLayerType, friendInfo);
    
    m_memInfoLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_memInfoLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

void CountryMain::cbMemberInfoLayer(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    MemberInfoLayerType memLayerType = m_memInfoLayer->getMemLayerType();
    if (mItem->getTag() == enMemLayBtn_OK) {
        if (memLayerType == memLayerType_accept)
            acceptMember();
        else if (memLayerType == memLayerType_refuse)
            refuseMember();
        else if (memLayerType == memLayerType_applyFriend)
            applyFriend();
        else if (memLayerType == memLayerType_delete)
            deleteMember();
        else if (memLayerType == memLayerType_setViceleader)
            setViceleader();
        else if (memLayerType == memLayerType_deleteViceleader)
            deleteViceleader();
        else if (memLayerType == memLayerType_giveUpLeader)
            giveUpLeader();
        else if (memLayerType == memLayerType_applyCountry)
            applyCountry();
    }
    else if (mItem->getTag() == enMemLayBtn_Cancel) {
        if (memLayerType == memLayerType_accept || memLayerType == memLayerType_refuse)
            showMemberApplyList();
        else if (memLayerType == memLayerType_applyFriend || memLayerType == memLayerType_delete
                 || memLayerType == memLayerType_setViceleader || memLayerType == memLayerType_deleteViceleader
                 || memLayerType == memLayerType_giveUpLeader)
            showMemberList();
        else if (memLayerType == memLayerType_applyCountry)
            showSearchCountry();
    }

}

void CountryMain::acceptMember()
{
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getMemberApplyList();
    CFriendInf *friendInfo = array->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->acceptMemberRequest(friendInfo->fid)) {
        m_curReqType = enCReqType_AcceptMember;
        scheduleUpdate();
    }
}

void CountryMain::refuseMember()
{
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getMemberApplyList();
    CFriendInf *friendInfo = array->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->refuseMemberRequest(friendInfo->fid)) {
        m_curReqType = enCReqType_RefuseMember;
        scheduleUpdate();
    }
}

void CountryMain::applyFriend()
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->addFriendRequest(memberInfo->mid)) {
        m_curReqType = enCReqType_ApplyFriend;
        scheduleUpdate();
    }
}

void CountryMain::deleteMember()
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->deleteMemberRequest(memberInfo->mid)) {
        m_curReqType = enCReqType_DeleteMember;
        scheduleUpdate();
    }
}

void CountryMain::setViceleader()
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->setViceLeaderRequest(memberInfo->mid)) {
        m_curReqType = enCReqType_SetViceleader;
        scheduleUpdate();
    }
}

void CountryMain::deleteViceleader()
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->deleteViceLeaderRequest(memberInfo->mid)) {
        m_curReqType = enCReqType_DeleteViceleader;
        scheduleUpdate();
    }
}

void CountryMain::giveUpLeader()
{
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    if (CGameData::Inst()->giveUpLeaderRequest(memberInfo->mid)) {
        m_curReqType = enCReqType_GiveUpLeader;
        scheduleUpdate();
    }
}

void CountryMain::sendMail()
{
    if (m_bMailAll)
    {
        if (CGameData::Inst()->sendMailAllRequest(m_mailText.c_str())) {
            m_curReqType = enCReqType_SendMail;
            scheduleUpdate();
        }
    }
    else
    {
        CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
        if (CGameData::Inst()->sendMailRequest(memberInfo->mid, m_mailText.c_str())) {
            m_curReqType = enCReqType_SendMail;
            scheduleUpdate();
        }
    }
}

void CountryMain::showMemberManage()
{
     innerExit();
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_member_manage"));
    
    m_curScreen = enCM_MemberManage;
    
    
    if (m_memberManage == NULL)
    {
        m_memberManage = new MemberManageLayer(this, callfuncO_selector(CountryMain::cbMemberManage));
        
        addChild(m_memberManage);
        m_memberManage->release();
    }
    
    innerEnter();
}

void CountryMain::cbMemberManage()
{
    
}

void CountryMain::showMemberList()
{
    innerExit();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_member_list4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_member_list"));
    
    m_curScreen = enCM_MemberList;

    if (m_memberList == NULL) {
        m_memberList = new PagingListLayer(this, callfuncO_selector(CountryMain::cbMemberList));
        addChild(m_memberList);
        m_memberList->release();
    }
    
    showRangeBtn();
    
    CGameData::Inst()->changeMemberListRange(false);
    m_memberList->showList(enMemberListType_normal);
    showMListRangeTitle();
    
    innerEnter();
}

void CountryMain::cbMemberList(cocos2d::CCObject *pObj)
{    
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    m_listSel = pRet->iBtnSel;
    
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    CUserInfo* userInfo = CGameData::Inst()->getUsrInfo();
    
    if (strcmp(memberInfo->mid, userInfo->uid) == 0)
        // 此项是自己
        m_showCard->show(&memberInfo->card_info, false);
    else
        showCfmLayer();
}

void CountryMain::showMemberInfoLayer(MemberInfoLayerType memInfoLayerType, CMember *memberInfo)
{
    innerExit();
    
    if (m_memInfoLayer == NULL) {
        m_memInfoLayer = new MemberInfoLayer(this, menu_selector(CountryMain::cbMemberInfoLayer));
        addChild(m_memInfoLayer);
        m_memInfoLayer->release();
    }
    
    m_memInfoLayer->showMemberInfoLayer(memInfoLayerType, memberInfo);
    
    m_memInfoLayer->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_memInfoLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

void CountryMain::getMemberList()
{
    if (CGameData::Inst()->getMemberListRequest()) {
        m_curReqType = enCReqType_MemberList;
        
        scheduleUpdate();
    }
}

void CountryMain::getJifengRecord()
{
    if (CGameData::Inst()->getJiFengRecordReq()) {
        m_curReqType = enCReqType_GetJiFengRecord;
        
        scheduleUpdate();
    }
}

void CountryMain::getRewardMemberList()
{
    if (CGameData::Inst()->rewardMemberListRequest(m_rewardTaskId))
    {
        m_curReqType = enCReqType_RewardMemberList;
        
        scheduleUpdate();
    }
}

void CountryMain::showRangeBtn()
{
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(sp1,
                                                             sp2,
                                                             this,
                                                             menu_selector(CountryMain::changeListRange));
    item->setAnchorPoint(ccp(0, 0));
    
    TextNode* lbRangeTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_title"),
                                                      26);
    lbRangeTitle->setPosition(ccp(53, 10));
    lbRangeTitle->setShadowColor(ccc3(0, 0, 0));
    item->addChild(lbRangeTitle);
    
    m_rangeMenu = CCMenu::menuWithItem(item);
    addChild(m_rangeMenu);
    
    m_rangeTitle = TextNode::textWithString("",
                                            CCSizeMake(150,80),
                                            CCTextAlignmentCenter,
                                            25);
    m_spTitle->addChild(m_rangeTitle);
    m_rangeTitle->setPosition(ccp(335, 15));
}

void CountryMain::changeListRange()
{
    if (m_curScreen == enCM_MemberList || m_curScreen == enCM_SupplyAssignList)
    {
        CGameData::Inst()->changeMemberListRange();
        m_memberList->changeListRange();
        showMListRangeTitle();
    }
    else if (m_curScreen == enCM_JiFengBonusPanel){
        m_pJiFengBonusPanel->toggleSort();
    }
}

void CountryMain::showMListRangeTitle()
{
    switch (CGameData::Inst()->getMemberRangeType()) {
        case enMemberRangeStatus:
            m_rangeTitle->setString(CGameData::Inst()->getLanguageValue("cntry_rank_status"));
            break;
        case enMemberRangeLv:
            m_rangeTitle->setString(CGameData::Inst()->getLanguageValue("cntry_rank_level"));
            break;
            
        case enMemberRangeCbt:
            m_rangeTitle->setString(CGameData::Inst()->getLanguageValue("cntry_rank_contribution"));
            break;
            
        case enMemberRangeLT:
            m_rangeTitle->setString(CGameData::Inst()->getLanguageValue("cntry_rank_login"));
            break;
            
        default:
            break;
    }
}

void CountryMain::showCountryDetails()
{
    innerExit();
    
    if (m_cntryDetails == NULL) {
        m_cntryDetails = new CountryDetails(this, menu_selector(CountryMain::cbCountryDetails));
        addChild(m_cntryDetails, 100);
        m_cntryDetails->release();
    }
    
    m_cntryDetails->showCountryDetails();
    
    m_cntryDetails->setPosition(CCPointMake(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_cntryDetails->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
}

void CountryMain::cbCountryDetails(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    CCMenuItem* menuItem = (CCMenuItem*)pObj;
    
    switch (menuItem->getTag()) {
        case enCDBtn_Back:
            if (m_curScreen == enCM_CntryRank)
                // 返回CountryRank
                showCountryRank();
            else
                // 返回countryMain
                showMainOrNot();
            break;
            
        case enCDBtn_Manage:
            // 管理
            showCountryManage();
            break;
            
        case enCDBtn_Quit:
            // 退出国家
            showQuitCountryTip();
            break;
        
        case enCDBtn_LvUp:
            // 国家升级
            showCntryLevelUp();
            break;
            
        case enCDBtn_BuffUp:
            // 增益升级
            showBuffLevelUpList();
            break;
            
        default:
            break;
    }
    
    if (m_cntryDetails)
    {
        m_cntryDetails->removeFromParentAndCleanup(true);
        m_cntryDetails = NULL;
    }
}

void CountryMain::showBuffLevelUpList()
{
    innerExit();
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_promotion4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_promotion"));
    
    m_curScreen = enCM_BuffLvUpList;
    
    for (int i = enBuffType_Stamina; i < enBuffType_Max; i++) {
        CMenuBtn *btn = new CMenuBtn((enBuffTypeDef)i);
        m_list->push_back(btn);
        btn->release();
        
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *itemLevelUp = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                        spMenu2,
                                                                        this,
                                                                        menu_selector(CountryMain::cbBuffLevelUpList));
        itemLevelUp->setTag(i);
        itemLevelUp->setAnchorPoint(ccp(0, 0));
        
        TextNode* tnLevelUp = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_level_up"),
                                                       spMenu1->getContentSize(),
                                                       CCTextAlignmentCenter,
                                                       40);
        tnLevelUp->setShadowColor(ccBLACK);
        tnLevelUp->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                           spMenu1->getContentSize().height * 0.5));
        itemLevelUp->addChild(tnLevelUp);
        
        //如果等级 超过10，那么不让点击 升级 按钮
        if (cntryInfo->buffInfo[i].lv >= 10)
        {
            itemLevelUp->setIsEnabled(false);
            
            CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            spShadow->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                              spMenu1->getContentSize().height * 0.5));
            itemLevelUp->addChild(spShadow);
        }

        CCMenu *menu = CCMenu::menuWithItem(itemLevelUp);
        btn->addChild(menu);
        menu->setPosition(ccp(120, -42));
    }

    innerEnter();
}

void CountryMain::cbBuffLevelUpList(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_listSel = mItem->getTag();
    showBuffLevelUpDialog();
}

void CountryMain::showBuffLevelUpDialog()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_promotion4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_promotion"));
    
    m_curScreen = enCM_BuffLvUpDialog;

    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::levelUpBuff));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showBuffLevelUpDialog((enBuffTypeDef)m_listSel);
        
    innerEnter();
}

void CountryMain::levelUpBuff()
{
    if (CGameData::Inst()->levelUpBuffRequest(m_listSel)) {
        m_curReqType = enCReqType_LevelUpBuff;
        scheduleUpdate();
    }
}

void CountryMain::showCntryLevelUp()
{
    innerExit();
   
    MainScene::Inst()->showBottomPromptInfo("cntry_up4_1");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_up"));
    
    m_curScreen = enCM_CntryLevelUp;
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::levelUpCntry));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showCntryLevelUpDialog();
    
    innerEnter();
}

void CountryMain::levelUpCntry()
{
    if (CGameData::Inst()->levelUpCountryRequest()) {
        m_curReqType = enCReqType_LevelUpCntry;
        scheduleUpdate();
    }
}

void CountryMain::showCntryCurrentInfo()
{
    innerExit();
    
    MainScene::Inst()->showBottomPromptInfo("cntry_recent_info4_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(CountryMain::cbCntryCurrentInfo));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->showCntryCurrentInfo();
}

void CountryMain::cbCntryCurrentInfo()
{
    m_cntryDialog->removeFromParentAndCleanup(true);
    m_cntryDialog = NULL;
    
    showMainOrNot();
}

void CountryMain::getCntryCurrentInfo()
{
    if (CGameData::Inst()->getCntryCurrentInfo())
    {
        m_curReqType = enCReqType_CntryCurrentInfo;
        scheduleUpdate();
    }
}

void CountryMain::reqCntryDgCfg()
{
    if (CGameData::Inst()->cntryDgCfgReq()) {
        m_curReqType = enCReqType_CntryDgCfg;
        scheduleUpdate();
        innerExit();
    }
}

void CountryMain::getMemberApplyList()
{
    if (CGameData::Inst()->getMemberApplyListRequest()) {
        m_curReqType = enCReqType_Applications;
        scheduleUpdate();
    }
}

void CountryMain::getAllCntryByNewTime()
{
    if (CGameData::Inst()->getAllCountriesRequest(2)) {
        m_curReqType = enCReqType_AllCntryByNewTime;
        scheduleUpdate();
    }
}

void CountryMain::getAllCntryByOldTime()
{
    if (CGameData::Inst()->getAllCountriesRequest(1)) {
        m_curReqType = enCReqType_AllCntryByOldTime;
        scheduleUpdate();
    }
}

void CountryMain::getAllCntryByLv()
{
    if (CGameData::Inst()->getAllCountriesRequest(3)) {
        m_curReqType = enCReqType_AllCntryByLv;
        scheduleUpdate();
    }
}

void CountryMain::updateMail(ccTime dt)
{
    if (OcProxy::Inst()->getIsHasSentMail()) {
        unschedule(schedule_selector(CountryMain::updateMail));
        
        if (strlen(OcProxy::Inst()->getMailText()) != 0) {
            m_mailText = OcProxy::Inst()->getMailText();
            sendMail();
        }
        OcProxy::Inst()->deleteMailViewController();
    }
}

void CountryMain::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(CountryMain::cbCommDlg),
                                    enCommDlgTp_connecting);
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
        switch (m_curReqType) {
            case enCReqType_Create:
            case enCReqType_SetDeclaration:
            case enCReqType_DeleteCntry:
            case enCReqType_UnDeleteCntry:
                showMainOrNot();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_QuitCntry:
                showMain();
                MainScene::Inst()->showUserInfo();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            
            case enCReqType_AllCntryByLv:
            case enCReqType_AllCntryByNewTime:
            case enCReqType_AllCntryByOldTime:
            {
                m_listCntry->list_reset();
                if (CGameData::Inst()->getCntryInfo()->status == 0)
                    showCountryList();
                else
                    showCountryRank();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
            }
                break;
                
            case enCReqType_Apply:
                if (m_curScreen == enCM_CntryList)
                    showCountryList();
                else if (m_curScreen == enCM_SearchCntry)
                    showSearchCountry();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            
            case enCReqType_Applications:
                showMemberApplyList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            
            case enCReqType_AcceptMember:
            case enCReqType_RefuseMember:
                MainScene::Inst()->showTips();
                getMemberApplyList();
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_MemberList:
                if (m_bShowMemberList)
                    showMemberList();
                else
                    showSupplyAssignList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_GetJiFengRecord:
                showBonusPanel();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_MemberManage:
            {
                showMemberManage();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
            }
                break;
            
            case enCReqType_ApplyFriend:
                showMemberList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_DeleteMember:
                CCUserDefault::sharedUserDefault()->setIntegerForKey("currpage", m_listSel/20);
                CCUserDefault::sharedUserDefault()->flush();
                getMemberList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_SetViceleader:
                getMemberList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_DeleteViceleader:
                getMemberList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_GiveUpLeader:
                showMainOrNot();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_SendMail:
            
                break;
                
            case enCReqType_Search:
                showOneCntryDetails(CGameData::Inst()->getSchCntryInfo());
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_DonateCoin:
                showDonateSuccess();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                                
            case enCReqType_LevelUpBuff:
                showBuffLevelUpList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_LevelUpCntry:
                showCountryDetails();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_ExchagneWelfare:
                getAllCards();
                break;
                
            case enCReqType_AllCards:{
                long time = 0;
                int m_iSel = 0;
                CCMutableArray<CWelfareInfo*>* allWelfareInfo = CGameData::Inst()->getWelfareInfoList();
                if (allWelfareInfo && allWelfareInfo->count() > 0)
                {
                    for (int i = 0; i < allWelfareInfo->count(); i++) {
                        CWelfareInfo* welfareInfo = allWelfareInfo->getObjectAtIndex(i);
                        long lastWelfareTime = CGameData::Inst()->getLastWelfareTime(welfareInfo->wid);
                        if (lastWelfareTime > 0)
                        {
                            long retainTime = welfareInfo->cd_time * 3600 + lastWelfareTime - CGameData::Inst()->getCurrentTime();
                            if (retainTime > 0) {
                                if (time==0) {
                                    time = retainTime;
                                    m_iSel = i;
                                }else{
                                    if (time > retainTime) {
                                        time = retainTime;
                                        m_iSel = i;
                                    }
                                }
                            }
                        }
                    }
                }
                if (time > 0) {
                    char buf[100];
                    CWelfareInfo* welfareInfo = allWelfareInfo->getObjectAtIndex(m_iSel);
                    snprintf(buf, 99,CGameData::Inst()->getLanguageValue("time_to_getgift"), welfareInfo->description);
                    OcProxy::Inst()->setPushInfo(buf, time, kPushInfGetGift,true);
                }
                
                showGacha();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            }
            case enCReqType_CntryDgCfg:
                if (m_bBackToFightRank) {
                    FightRank();
                    
                }
                else{
                    showOrgFightList();
                    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                }
               
                break;
                
            case enCReqType_GuildItemShop:
                BaoKuShow();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_RewardTaskList:
                showPostReward();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_RewardTopMember:
                showRewardTopMembers();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_CntryOpen:
                reqCntryDgCfg();
                break;
            
            case enCReqType_CntryRank:
                enterFightCfm();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_CntryJoin:
                enterOrgFight();
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                
                
#ifdef TalkData
                char msg[60];
                sprintf(msg, "%s&&%s&&%s TM" , m_fightitem.c_str() , m_fightname.c_str() , m_fighttype.c_str());
                
                
                TDCCMission::onBegin(msg);
#else
                
#endif
                


                

                break;
                
            case enCReqType_AssignOneSupply:
            case enCReqType_AssignAllSupply:
            case enCReqType_AssignOutSupply:
            case enCReqType_AssignWeekSupply:
                showSupplyAssignList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_CntryCurrentInfo:
                showCntryCurrentInfo();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
                //处理 recoverPower
            case  enCReqType_ReqRecoverPower:
                showOrgFightList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_UpdateCntryInfo:
                showMainOrNot();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_SetNameCard:
                MainScene::Inst()->updateCntryNameCard();
                showCountryManage();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enCReqType_RewardMemberList:
                showRewardMemberList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;

            default:
                break;
                
        }
    }
    else {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(CountryMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            
            if (m_curReqType == enCReqType_MemberManage)
            {
                innerExit();
            }
        }
    }
}

void CountryMain::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true){
        switch (m_curReqType) {
            case enCReqType_Create:
                createCountry();
                break;
            
            case enCReqType_Search:
                searchCountry();
                break;
                
            case enCReqType_SetDeclaration:
                setDeclaration();
                break;
                
            case enCReqType_DeleteCntry:
                deleteCountry();
                break;
            
            case enCReqType_UnDeleteCntry:
                unDeleteCountry();
                break;
                
            case enCReqType_QuitCntry:
                quitCountry();
                break;
                
            case enCReqType_AllCntryByLv:
                getAllCntryByLv();
                break;

            case enCReqType_AllCntryByNewTime:
                getAllCntryByNewTime();
                break;

            case enCReqType_AllCntryByOldTime:
                getAllCntryByOldTime();
                break;

            case enCReqType_Apply:
                applyCountry();
                break;
            
            case enCReqType_Applications:
                getMemberApplyList();
                break;
                
            case enCReqType_AcceptMember:
                acceptMember();
                break;
                
            case enCReqType_RefuseMember:
                refuseMember();
                break;
                
            case enCReqType_MemberList:
                getMemberList();
                break;
                
            case enCReqType_GetJiFengRecord:
                getJifengRecord();
                break;
                
            case enCReqType_MemberManage:
                cbChatRoom();
                break;
                
            case enCReqType_ApplyFriend:
                applyFriend();
                break;
            
            case enCReqType_DeleteMember:
                deleteMember();
                break;
                
            case enCReqType_SetViceleader:
                setViceleader();
                break;
                
            case enCReqType_DeleteViceleader:
                deleteViceleader();
                break;
                
            case enCReqType_GiveUpLeader:
                giveUpLeader();
                break;
                
            case enCReqType_SendMail:
                sendMail();
                break;
                
            case enCReqType_DonateCoin:
                donateCoin();
                break;
                
            case enCReqType_LevelUpBuff:
                levelUpBuff();
                break;
                
            case enCReqType_LevelUpCntry:
                levelUpCntry();
                break;
                
            case enCReqType_ExchagneWelfare:
                exchangeWelfare();
                break;
                
            case enCReqType_AllCards:
                getAllCards();
                break;
                
            case enCReqType_CntryDgCfg:
                reqCntryDgCfg();
                break;
            case enCReqType_CntryOpen:
                FightOpen();
                break;
            case enCReqType_CntryRank:
                FightRank();
                break;
                
            case enCReqType_CntryJoin:
                FightJoin();
                break;
                
            case enCReqType_AssignOneSupply:
                assignOneSupply();
                break;

            case enCReqType_CntryCurrentInfo:
                getCntryCurrentInfo();
                break;
                
            case enCReqType_AssignAllSupply:
                assignAllSupply();
                break;

            case enCReqType_AssignOutSupply:
                assignOutSupply();
                break;

            case enCReqType_ReqRecoverPower:
                recoverActionPoint();
                break;
                
            case enCReqType_UpdateCntryInfo:
                updateCntryInfo();
                break;
            
            case enCReqType_SetNameCard:
                setNameCard();
                break;
                
            case enCReqType_RewardTaskList:
                reqRewardTaskList();
                break;
                
            case enCReqType_RewardTopMember:
                reqRewardTopMembers();
                break;
                
            case enCReqType_RewardMemberList:
                getRewardMemberList();
                break;
                
            case enCReqType_GuildItemShop:
                reqGuildItemShopCfg();
                break;
            case enCReqType_AssignWeekSupply:
            {
                assignWeekSupply();
                break;
            }
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();

        if (CGameData::Inst()->getCntryInfo()->status != m_preStatus)
            showMainOrNot();
        else if (m_curReqType == enCReqType_CntryDgCfg
                 || m_curReqType == enCReqType_MemberManage)
            showMainOrNot();
        else if (m_curReqType == enCReqType_CntryJoin
                 || m_curReqType == enCReqType_CntryRank)
            reqCntryDgCfg();
    }
}

void CountryMain::cbNoDlg(cocos2d::CCObject *pObj)
{
    if (m_dlgNoTip) {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
    
    showMainOrNot();
}

void CountryMain::showCfmLayer()
{
    m_curScreen = enCM_CfmLayer;
    
    if (m_cfmLayer == NULL) {
        m_cfmLayer = new CConfirmLayer(this, callfuncO_selector(CountryMain::cbCfm));
        addChild(m_cfmLayer, 102);
        m_cfmLayer->release();
    }
    
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    enCConfirmLayerType cfmLayerType;
    if (cntryInfo->status == 1) {
        if (memberInfo->status == 2 && memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_LViceleader;
        else if (memberInfo->status == 2 && !memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_LVStranger;
        else if (memberInfo->status == 3 && memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_LMember;
        else if (memberInfo->status == 3 && !memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_LMStranger;
    }
    else if (cntryInfo->status == 2) {
        if (memberInfo->status == 3 && memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_VLMember;
        else if (memberInfo->status == 3 && !memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_VLMStranger;
        else if (memberInfo->bFriend)   // memberInfo->status = 1, 2
            cfmLayerType = enCConfirmLayer_MMember;
        else if (!memberInfo->bFriend)  // memberInfo->status = 1, 2
            cfmLayerType = enCConfirMLayer_MMStranger;
    }
    else if (cntryInfo->status == 3) {
        if (memberInfo->bFriend)
            cfmLayerType = enCConfirmLayer_MMember;
        else if (!memberInfo->bFriend)
            cfmLayerType = enCConfirMLayer_MMStranger;
    }
    
    m_cfmLayer->show(&memberInfo->card_info, memberInfo->name, cfmLayerType);
    
    if (m_lcCover == NULL) {
        m_lcCover = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 196), 640, 960);
        addChild(m_lcCover, 101);
    }
}

void CountryMain::cbCfm(cocos2d::CCObject *pObj)
{
    CCfmLayerRet* pRet = (CCfmLayerRet*) pObj;
    
    int iBtnSel = pRet->iBtnSel;
    enCConfirmLayerType cfmType = m_cfmLayer->getCfmLayerType();
    CMember* memberInfo = CGameData::Inst()->getMemberList()->getObjectAtIndex(m_listSel);
    
    switch (iBtnSel) {
        case 0:
        {
            if (memberInfo->bFriend) {
                // 发邮件
                m_bMailAll = false;
                OcProxy::Inst()->showMailViewController(memberInfo->name);
                schedule(schedule_selector(CountryMain::updateMail));
            }
            else
                // 申请好友
                showMemberInfoLayer(memLayerType_applyFriend, memberInfo);            
        }
            break;
        
        case 1:
        {
            // 武将详情
            m_showCard->show(&memberInfo->card_info, false);
        }
            break;
            
        case 2:
        {
            if (cfmType == enCConfirmLayer_LMember || cfmType == enCConfirmLayer_LMStranger
                || cfmType == enCConfirmLayer_VLMember || cfmType == enCConfirmLayer_VLMStranger)
                // 删除成员
                showMemberInfoLayer(memLayerType_delete, memberInfo);
            else if (cfmType == enCConfirmLayer_LViceleader || cfmType == enCConfirmLayer_LVStranger)
                // 解职
                showMemberInfoLayer(memLayerType_deleteViceleader, memberInfo);
            else
                // 返回
                cancelCfmToMemberList();
        }
            break;
            
        case 3:
        {
            if (cfmType == enCConfirmLayer_LMember || cfmType == enCConfirmLayer_LMStranger)
                // 任命
                showMemberInfoLayer(memLayerType_setViceleader, memberInfo);
            else if (cfmType == enCConfirmLayer_LViceleader || cfmType == enCConfirmLayer_LVStranger)
                // 让贤
                showMemberInfoLayer(memLayerType_giveUpLeader, memberInfo);
            else
                // 返回
                cancelCfmToMemberList();

        }
            break;
        
        case 4:
        // 返回
        cancelCfmToMemberList();
            break;
            
        default:
            break;
    }
}

void CountryMain::cancelCfmToMemberList()
{
    m_cfmLayer->hide();
    m_lcCover->removeFromParentAndCleanup(true);
    m_lcCover = NULL;
    m_curScreen = enCM_MemberList;
}

void CountryMain::newMenuBtn(const char **menuName, int num)
{
    CMenuBtn* btn;
    for (int i = 0; i < num; i++) {
        btn = new CMenuBtn("ma_button.png",
                           menuName[i],
                           CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->release();
    }
}

void CountryMain::newBtn(CCMenuItem *&menuItem, const char *label,
                          const CCPoint& position, int tag, const char* image)
{
    CCSprite* spMenu1 = NULL;
    CCSprite* spMenu2 = NULL;
    
    spMenu1 = CCSprite::spriteWithFile(image);
    if (spMenu1)
    {
        spMenu2 = CCSprite::spriteWithFile(image);
    }
    else
    {
        spMenu1 = CCSprite::spriteWithSpriteFrameName(image);
        spMenu2 = CCSprite::spriteWithSpriteFrameName(image);
    }
    
    menuItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                     spMenu2,
                                                     this,
                                                     menu_selector(CountryMain::btnCallback));
    menuItem->setTag(tag);
    menuItem->setAnchorPoint(CCPointZero);
    menuItem->setPosition(CCPointMake(position.x - spMenu1->getContentSize().width * 0.5,
                                      position.y));
    
    if (strlen(label) > 0)
    {
        TextNode* tempText = TextNode::textWithString(label,
                                                      spMenu1->getContentSize(),
                                                      CCTextAlignmentCenter,
                                                      35);
        tempText->setShadowColor(ccBLACK);
        tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                          spMenu1->getContentSize().height * 0.5 + 2));
        menuItem->addChild(tempText);
    }
}

void CountryMain::showBonusPanel()
{
    innerExit();
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_jifengfenpei"));
    
    m_curScreen = enCM_JiFengBonusPanel;

    if (m_pJiFengBonusPanel == NULL) {
        m_pJiFengBonusPanel = new BonusPanel();
        addChild(m_pJiFengBonusPanel);
    }
    
    showRangeBtn();

    innerEnter();
}


#pragma mark -
#pragma mark 悬赏


void CountryMain::cbBaoKuLayer(CCObject* pObj)
{
    BaoKuRet* ret = (BaoKuRet*)pObj;
    
    switch (ret->iBtnSel)
    {
        case en_cbBaoKuJuanXian:
            showCountryDonate();
            break;
            
        default:
            break;
    }
}

void CountryMain::cbRewardRelease(CCObject* pObj)
{
    RewardReleaseRet* ret = (RewardReleaseRet*)pObj;
    
    switch (ret->iBtnSel)
    {
        case en_fabuXuanShang_end:
            reqRewardTaskList();
            break;
            
        case en_ChooseItemFromFabu:
            m_curScreen = enCM_FabuXuanZe;
            break;
            
        case en_fabuFromChooseItem:
            m_curScreen = enCM_FabuXuanShang;
            break;
            
        case en_toBaoKuFromCFabu:
            m_curScreen = enCM_FabuXuanShang;
            reqGuildItemShopCfg();
            break;
            
        default:
            break;
    }
}

void CountryMain::cbFromPostRwd(CCObject* pObj)
{
    PostRewardRet* ret = (PostRewardRet*)pObj;
    switch (ret->iBtnSel)
    {
        case en_haveNoRewardTask:
            showMainOrNot();
            break;
            
        case en_refreshPostReward:
            reqRewardTaskList();
            break;
            
        case en_cbXuanShangBaoKu:
            reqGuildItemShopCfg();
            break;
            
//        case en_cbBaoKuJuanXian:
//            showCountryDonate();
//            break;
            
        case en_fabuXuanShang:
            showFabuXuanShang();
            break;
            
//        case en_fabuXuanShang_end:
//            reqRewardTaskList();
//            break;
            
//        case en_ChooseItemFromFabu:
//            m_curScreen = enCM_FabuXuanZe;
//            break;
            
//        case en_fabuFromChooseItem:
//            m_curScreen = enCM_FabuXuanShang;
//            break;
            
//        case en_toBaoKuFromCFabu:
//            m_curScreen = enCM_FabuXuanShang;
//            reqGuildItemShopCfg();
//            break;
            
        case en_rewardMemberList:
            m_rewardTaskId = ret->taskId;
            getRewardMemberList();
            break;
            
        default:
            break;
    }
}

void CountryMain::showFabuXuanShang()
{
    innerExit();
    
    m_curScreen = enCM_FabuXuanShang;
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("reward_title_fabu"));
    
    if (m_FaBuLayer == NULL)
        m_FaBuLayer = new RewardRelease(this, callfuncO_selector(CountryMain::cbRewardRelease));
    addChild(m_FaBuLayer);
    m_FaBuLayer->release();
    
    innerEnter();
}

void CountryMain::showRewardMemberList()
{
    innerExit();
    
    m_curScreen = enCM_RewardMemberList;
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("reward_info_title"));
   
    MainScene::Inst()->showBottomPromptInfo("reward_info_naviText_1");
 //    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    CCMutableArray<CRewardMember*>* memberList = CGameData::Inst()->getRewardMemberList();
    if (memberList && memberList->count() > 0)
    {
        if (m_memberList == NULL) {
            m_memberList = new PagingListLayer(this, callfuncO_selector(CountryMain::cbMemberList));
            addChild(m_memberList);
            m_memberList->release();
        }
        m_memberList->showList(enMemberListType_reward);
    }
    else
    {
        if (m_dlgNoTip == NULL) {
            m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(CountryMain::cbNoRewardMember));
            addChild(m_dlgNoTip);
            m_dlgNoTip->release();
            m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("reward_info_no"));
            m_dlgNoTip->setPosition(CCPoint(960, 400));
            CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
            m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
        }
    }

    innerEnter();
}

void CountryMain::cbNoRewardMember()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
    
    showPostReward();
}

void CountryMain::showRewardTopMembers()
{
    innerExit();
    
    m_curScreen = enCM_RewardTopMembers;
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("reward_top_paihangbang"));
    
    if (m_RwdTopMemLayer == NULL)
        m_RwdTopMemLayer = new RewardTopMembers(this,callfuncO_selector(CountryMain::cbTopMembers));
    addChild(m_RwdTopMemLayer);
    m_RwdTopMemLayer->release();
    
    innerEnter();
}

void CountryMain::cbTopMembers(CCObject* pObj)
{
    showPostReward();
}

void CountryMain::showPostReward()
{
    innerExit();
    
    m_curScreen = enCM_PostReward;
    
    
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(sp1,
                                                             sp2,
                                                             this,
                                                             menu_selector(CountryMain::GoToRewardTopMember));
    item->setAnchorPoint(ccp(0, 0));
    
    TextNode* lbRangeTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_top_member"),
                                                      26);
    lbRangeTitle->setPosition(ccp(53, 8));
    lbRangeTitle->setShadowColor(ccc3(0, 0, 0));
    item->addChild(lbRangeTitle);
    
    m_rangeMenu = CCMenu::menuWithItem(item);
    addChild(m_rangeMenu, 1);
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_post_reward_list"));
    
    MainScene::Inst()->showBottomPromptInfo("reward_title_naviText_1");
 //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_postRwdLayer == NULL)
        m_postRwdLayer = new PostReward(this, callfuncO_selector(CountryMain::cbFromPostRwd));
    addChild(m_postRwdLayer);
    m_postRwdLayer->release();
        
    innerEnter();
}

void CountryMain::GoToRewardTopMember()
{
    reqRewardTopMembers();
}

void CountryMain::reqRewardTaskList()
{
    if (CGameData::Inst()->rewardTaskListRequest())
    {
        m_curReqType = enCReqType_RewardTaskList;
        scheduleUpdate();
    }
}

void CountryMain::reqRewardTopMembers()
{
    if (CGameData::Inst()->rewardTopMemberRequest())
    {
        m_curReqType = enCReqType_RewardTopMember;
        scheduleUpdate();
    }
}

void CountryMain::reqGuildItemShopCfg()
{
    if (CGameData::Inst()->reqGuildItemShop())
    {
        m_curReqType = enCReqType_GuildItemShop;
        scheduleUpdate();
    }
}

void CountryMain::BaoKuShow()
{
    innerExit();
    
    m_curScreen = enCM_BaoKuItem;
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("cntry_baoku_list"));
    
    if (m_BaoKuLayer == NULL)
        m_BaoKuLayer = new BaoKuLayer(this, callfuncO_selector(CountryMain::cbBaoKuLayer));
    addChild(m_BaoKuLayer);
    m_BaoKuLayer->release();
    
    innerEnter();

}

void CountryMain::forceExitFight()
{
    if (m_orgFightMngr) {
        removeChild(m_orgFightMngr, true);
        m_orgFightMngr = NULL;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("guangquan.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("orgfightres.plist");
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}



