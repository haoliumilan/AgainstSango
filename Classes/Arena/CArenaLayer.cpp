//
//  CArenaLayer.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CArenaLayer.h"
#include "CGamedata.h"
#include "CMenuBtn.h"
#include "CListLayer.h"
#include "CScrollBar.h"
#include "FightMngr.h"
#include "CConfirmLayer.h"
#include "CShowCardInf.h"
#include "CashInLayer.h"
#include "MainScene.h"
#include "AddFriend.h"
#include "Dialog2.h"
#include "CommDlg.h"
#include "NewbieLayer.h"
#include "TitleBar.h"
#include "SimpleAudioEngine.h"
#include "Arrow.h"
#include "SaveData.h"
#include "TeamCfm.h"
#include "CommDlg3.h"
#include "RecordHelper.h"
#include "HttpComm.h"

#include "TDCCMission.h"


using namespace CocosDenshion;

#define kArenaBackBtn           1
#define kArenaSpecialBtn        2
#define kArenaNormalBtn         3
#define kArenaSango             4
#define kArenaViewReocrdBtnOn   21
#define kArenaViewReocrdBtnOff  22
#define kArenaArrowTag          234


#define kNewbie                 99999

static CUserInfo pre_usrInfo;

//******************************************************************************
// Construct
//******************************************************************************
CArenaLayer::CArenaLayer()
{
    m_pHelperArr = NULL;
    m_isAddFloorNameImage = true;
    m_goShopDlg = NULL;
    m_staminaDlg = NULL;
    m_less5Dlg = NULL;
    m_commDlg = NULL;
    m_dialog1 = NULL;
    m_upload_or_not = NULL;
    m_teamCfm = NULL;
    m_szDgType = NULL;
    m_getCfgAlert = NULL;
    m_getFantiAlert = NULL;
    m_SortBtnLayer = NULL;
    
    isJustLeaveFight = false;
    isEnter = false;
    m_iFloorId = 0;
    m_iRoomId = 0;
    m_curLayer = enArenaLayer_NmlFloor;
    m_fightSceneType = 1;
    m_ushLimitMaxStar = 0;
    
    resetIFriend();
    
    m_fPositionMap[enArenaLayer_NmlList] = new CListLayerModel(-2,7,0);
    m_fPositionMap[enArenaLayer_SpecialList] = new CListLayerModel(-2,7,0);
    m_fPositionMap[enArenaLayer_SangoList] = new CListLayerModel(-2,7,0);
    m_fPositionMap[enArenaLayer_HelperList] = new CListLayerModel(-2,7,0);
    m_fPositionMap[enArenaLayer_RoomList] = new CListLayerModel(-2,7,0);
    
}

 CArenaLayer::~CArenaLayer()
{
}

void CArenaLayer::InserItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;

    CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
    CNmlDgInfo* pNmlDgInfo;
    
    if(pNmlDgInfoArr->count() == 0)
        return;
    
    int index = pNmlDgInfoArr->count() -1 - i;
    pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(index);
    CMenuBtn* btn = new CMenuBtn(pNmlDgInfo, index + 1);
    m_listNml->reloadItemByIdx(btn, pRet->iBtnSel);
    btn->release();
}

void CArenaLayer::InserListHelpByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CMenuBtn* btn = new CMenuBtn(m_pHelperArr->getObjectAtIndex(i), m_FriendRange);
    m_listHelper->reloadItemByIdx(btn, pRet->iBtnSel);
    
    if (m_szDgType && strcmp(m_szDgType, "special") == 0
        && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding) {
        int num = 0;
        for (int k = 0; k < 5; k++) {
            if (m_iFriend[k] != -1) {
                num++;
            }
        }
        if (num == 5) { //反过来 选择的灰色cover去掉，没有选中的加灰色cover
            int k = 0;
            for (; k < 5; k++) {
                if (m_iFriend[k] == i) {
                    btn->setHelperNo(false, k+1);
                    break;
                }
            }
            if (k == 5) {
                btn->setHelperNo(true, 0);
            }
        } else { // 选择的加灰色cover
            int k = 0;
            for (; k < 5; k++) {
                if (m_iFriend[k] == i) {
                    btn->setHelperNo(true, k+1);
                    break;
                }
            }
            if (k == 5) {
                btn->setHelperNo(false, 0);
            }
        }
    }
    btn->release();
}

void CArenaLayer::InsertListSangoByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CCMutableArray<CSangoYYDgInfo*> * pSangoDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
    CSangoYYDgInfo* pSangoDgInfo;
    
    if(pSangoDgInfoArr->count() == 0)
        return;
    
    int index = pSangoDgInfoArr->count() -1 - i;
    pSangoDgInfo = pSangoDgInfoArr->getObjectAtIndex(index);
    CMenuBtn* btn = new CMenuBtn(pSangoDgInfo, index + 1);
    m_listSango->reloadItemByIdx(btn, pRet->iBtnSel);
    btn->release();
}

//******************************************************************************
// cbList
//******************************************************************************
void  CArenaLayer::cbList(CCObject* pObj)
{
    //已显示扩充包
    if (MainScene::Inst()->isExtendPackShow())
    {
        return;
    }
    
    //战斗中的话直接返回，避免画面重叠
    if(CGameData::Inst()->isFighting()){
        innerExit();
        return;
    }
    
    // 如果在观看录像的时候 不进去战场
    if (m_view_other_record->getTag() == kArenaViewReocrdBtnOff)
    {
        return;
    }
    
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    
    switch (m_curLayer) {
        case enArenaLayer_NmlFloor:
            m_recordCurLayer = enArenaLayer_NmlFloor;
            m_iFloorSel = pRet->iBtnSel;
            enterNmlFloor();
            break;
            
        case enArenaLayer_NmlRoom:
            m_iRoomSel = pRet->iBtnSel;
            enterNmlRoom();
            break;
            
        case enArenaLayer_SpecialFloor:
            m_recordCurLayer = enArenaLayer_SpecialFloor;
            m_iFloorSel = pRet->iBtnSel;
            enterSpecialFloor();
            break;
            
        case enArenaLayer_SpecialRoom:
            
            m_iRoomSel = pRet->iBtnSel;
            enterProposeLvDialog();
            break;
            
        case enArenaLayer_HelperList:
        {
            m_iCurrentFriend = pRet->iBtnSel;
            
            if (m_szDgType && strcmp(m_szDgType, "special") == 0
                && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding) {
                FiveTeamMemberSelected(m_iCurrentFriend);
            } else {
                m_iFriend[0] = m_iCurrentFriend;
                showCfmLayer();
            }
            
            CGameData::Inst()->resetSelectedHelper();
            CFriendHelpInfo* pHelperInfo = NULL;
            for (int i = 0; i < kHelperMaxCount; i++) {
                if (m_iFriend[i] != -1)
                {
                    pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[i]);
                    CGameData::Inst()->setSelectedHelper(pHelperInfo->fid, i);
                }
                
            }

        }
            break;
            
        case enArenaLayer_SangoRoom:
          //  m_recordCurLayer = enArenaLayer_SangoRoom;
            m_iRoomSel = pRet->iBtnSel;
            enterSangoRoom();
            break;
            
        case enArenaLayer_SangoFloor:
            m_recordCurLayer = enArenaLayer_SangoFloor;
            m_iFloorSel = pRet->iBtnSel;
            enterSangoFloor();
            break;
            
        default:
            break;
    }
    
    if(CGameData::Inst()->isNewbie()){
        CCNode* pArrow = getChildByTag(kArenaArrowTag);
        if(pArrow)
            removeChild(pArrow, true);
    }

    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
}

void CArenaLayer::enterProposeLvDialog()
{
    innerExit();
    
    CSpecialDgInfo* pSpecialDgInfo = getSpecialDgInfo();
    CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
    m_iRoomId = pRoomInfoArr->count() - m_iRoomSel;
    
    CDgRoomInfo* pRoomInf = pRoomInfoArr->getObjectAtIndex(m_iRoomId -1);
    int proposelv = pRoomInf->propose_lv;
    
    if(proposelv != 0 && m_proposeLvDialog == NULL)
    {
        char buf[100];
        snprintf(buf, 99,  CGameData::Inst()->getLanguageValue("arenaTip13"),proposelv);
        m_proposeLvDialog = new Dialog2(this, callfuncO_selector(CArenaLayer::cbProposeLvDlg),CGameData::Inst()->getLanguageValue("arenaTip12"),buf,enDl2TwoBtn);
        addChild(m_proposeLvDialog, 100);
        m_proposeLvDialog->setPosition(CCPointMake(320, 480));
        m_proposeLvDialog->release();
        m_proposeLvDialog->setScale(0.0);
        m_proposeLvDialog->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        MainScene::Inst()->enableBottomPanel(false);
    }
    else
    {
         enterSpecialRoom();
    }
}


//******************************************************************************
// showCfmLayer
//******************************************************************************
void CArenaLayer::showCfmLayer()
{
    CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[0]);
    
    CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
    
    m_curLayer = enArenaLayer_Cfm;
    m_lcCover->setOpacity(196);    
    
    m_cfmLayer->show(pUsrCardInfo, pHelperInfo->name);

}

//******************************************************************************
// showFiveTeamCfmLayer
//******************************************************************************
void CArenaLayer::showFiveTeamCfmLayer()
{
    CCMutableArray<CUserCardInfo *> *pUsrCardInfoArr = new CCMutableArray<CUserCardInfo *>(5);
    for (int i = 0; i < 5; i++) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[i]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        pUsrCardInfoArr->addObject(pUsrCardInfo);
    }
    
    
    m_curLayer = enArenaLayer_Cfm;
    m_lcCover->setOpacity(196);
    if (FiveTeamMenu) {
        FiveTeamMenu->setIsTouchEnabled(false);
    }
    if (FiveTeamBGMenu) {
        FiveTeamBGMenu->setIsTouchEnabled(false);
    }
    
    
    m_cfmLayer->showFiveTeam(pUsrCardInfoArr);
    
}

//******************************************************************************
// showTeamCfm
//******************************************************************************
void CArenaLayer::showTeamCfm()
{
    if(m_teamCfm == NULL){
        m_teamCfm = new TeamCfm(this, callfunc_selector(CArenaLayer::cbTeamCfm),
                                m_iFloorId, m_iRoomId, m_szDgType, m_iFriend);
        addChild(m_teamCfm, 10);
        m_teamCfm->release();
        
        m_teamCfm->setPosition(CCPointMake(640, 0));
        m_teamCfm->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    }
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("arena_teamcfm"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));

    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_teamConfirm");
  
}

//******************************************************************************
// cbTeamCfm
//******************************************************************************
void CArenaLayer::cbTeamCfm()
{
    innerExit();
    
    //确保当前没有战斗进行中才可以扣体力进战场，避免宝石重叠的问题
    if(CGameData::Inst()->isFighting() == false){

        m_lRtyID = CGameData::Inst()->getCurrentTime();
        reqEnterArena(m_lRtyID);
        
        CGameData::Inst()->releaseUserCardsSprite();
    }
    
    m_listHelper->list_reset();
}

//******************************************************************************
// cbFight
//******************************************************************************
void CArenaLayer::cbFight(CCObject* pObj)
{
    CFightRet* pRet = (CFightRet*)pObj;
    
    CGameData::Inst()->setIsFighting(false);
    
    if(pRet->bClear)
    {
        if(CGameData::Inst()->isNewbie()){
            CGameData::Inst()->newbieRequest(0, 3, NULL);
            m_curReqType = enArenaReqType_newbie3;
            scheduleUpdate();
        }
        else{
            char floorid[10];
            sprintf(floorid, "%d", m_iFloorId);
            char roomid[10];
            sprintf(roomid, "%d", m_iRoomId);
            
            if(CGameData::Inst()->dgEndRequest(m_szDgType, floorid, roomid, m_dungeonUid))
            {
                m_curReqType = enArenaReqType_dgend;
                CCLog("cbFight tag");
                scheduleUpdate();
            }
        }
    }
    else{
       //战场失败添加请求判断返体
#ifdef ZhanBaiFanTi
        char floorid[10];
        sprintf(floorid, "%d", m_iFloorId);
        char roomid[10];
        sprintf(roomid, "%d", m_iRoomId);
        
        if(CGameData::Inst()->dgEndForFanTiRequest(m_szDgType, floorid, roomid, m_dungeonUid))
        {
            m_curReqType = enArenaReqType_dgendforfanti;
            
            scheduleUpdate();
        }

    
#else
     SaveData::Inst()->destorySavedFile();
    
     addFloorNameImage();
    
    if (CGameData::Inst()->chkLogin()) {
        SaveData::Inst()->destorySavedData();
        enter();
        goDirectRoom();
        
        MainScene::Inst()->showBottomPanel(true);
        MainScene::Inst()->showUsrInfPanel(true);
    }
    else {
        MainScene::Inst()->reLogin();
    }

    
#endif
 }
    
    m_fightMngr->setIsVisible(false);
    m_fightMngr->exit();
    MainScene::Inst()->switchBgMusic(enBgMusicNml);
}


//******************************************************************************
// cbCfm
//******************************************************************************
void CArenaLayer::cbCfm(CCObject* pObj)
{
    CCfmLayerRet* pRet = (CCfmLayerRet*) pObj;
    
    CGameData::Inst()->getFriendHelpArr();
    CUserCardInfo* pUsrCardInfo;
    if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding) {
        switch (pRet->iBtnSel) {
            case 0:
                m_curLayer = enArenaLayer_HelperList;
                m_cfmLayer->hide();
                m_lcCover->setOpacity(0);
                
                innerExit();
                
                showTeamCfm();
                break;
   
            case 1:
                m_curLayer = enArenaLayer_HelperList;
                m_cfmLayer->hide();
                m_lcCover->setOpacity(0);
                if (FiveTeamBGMenu) {
                    FiveTeamBGMenu->setIsTouchEnabled(true);
                }
                if (FiveTeamMenu) {
                    FiveTeamMenu->setIsTouchEnabled(true);
                }
                
                MainScene::Inst()->showBottomPanel(true);
                MainScene::Inst()->showUsrInfPanel(true);
                
                if(CGameData::Inst()->isNewbie()){
                    m_spTitle->setPosition(CCPointMake(210, 766));
                    MainScene::Inst()->enableBottomPanel(false);
                }
                else {
                    m_spTitle->setPosition(CCPointMake(320, 766));
                    
                    m_miBack->setPosition(CCPointMake(1, 731));
                }
                break;
            default:
                CFriendHelpInfo *helperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[pRet->iBtnSel-2]);
                pUsrCardInfo = getHelperCardInfo(helperInfo);
                m_curLayer = enArenaLayer_ShowInf;
                m_showCard->show(pUsrCardInfo, false);
                MainScene::Inst()->showBottomPanel(false);
                break;
        }
    } else {
        switch (pRet->iBtnSel) {
            case 0:
                m_curLayer = enArenaLayer_HelperList;
                m_cfmLayer->hide();
                m_lcCover->setOpacity(0);
                
                innerExit();
                
                showTeamCfm();
                break;
                
            case 1:
            {
                CFriendHelpInfo *helperInfo = m_pHelperArr->getObjectAtIndex(m_iCurrentFriend);
                pUsrCardInfo = getHelperCardInfo(helperInfo);
                m_curLayer = enArenaLayer_ShowInf;
                m_showCard->show(pUsrCardInfo, false);
                MainScene::Inst()->showBottomPanel(false);
            }
                break;
                
            case 2:
            default:
                m_curLayer = enArenaLayer_HelperList;
                m_cfmLayer->hide();
                m_lcCover->setOpacity(0);
                
                MainScene::Inst()->showBottomPanel(true);
                MainScene::Inst()->showUsrInfPanel(true);
                
                if(CGameData::Inst()->isNewbie()){
                    m_spTitle->setPosition(CCPointMake(210, 766));
                    MainScene::Inst()->enableBottomPanel(false);
                }
                else {
                    m_spTitle->setPosition(CCPointMake(320, 766));
                    
                    m_miBack->setPosition(CCPointMake(1, 731));
                }
                break;
        }
    }
}

//******************************************************************************
// addFloorNameImage
//******************************************************************************
void CArenaLayer::addFloorNameImage()
{
    if (!m_isAddFloorNameImage) {
        m_isAddFloorNameImage = true;
 //       CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("floor.plist");
        CCLog("addFloorNameImage");
    }
}

//******************************************************************************
// removeFloorNameImage
//******************************************************************************
void CArenaLayer::removeFloorNameImage()
{
    if (m_isAddFloorNameImage) {
        m_isAddFloorNameImage = false;
 //       CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("floor.plist");
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }
}

//******************************************************************************
// enter
//******************************************************************************
void CArenaLayer::enter()
{
    if (CGameData::Inst()->isFighting())
        return;
    
    //------------------------------------start---------------------------------//
    isEnter = true;
    m_SortBtnLayer = NULL;
    reset();
    
    memset(m_roomName, 0, sizeof(m_roomName));
    memset(m_floorName, 0, sizeof(m_floorName));
    
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    addChild(m_spTitle,1);
    
    CListLayerModel* model = NULL;
    model = m_fPositionMap[enArenaLayer_NmlList];
    m_listNml = new CListLayer(this, callfuncO_selector(CArenaLayer::cbList));
    m_listNml->setPreTopShow(model->getTopShow());
    m_listNml->setPreBottomShow(model->getBottonShow());
    addChild(m_listNml, -1);
    m_listNml->release();
    m_listNml->setReloadFunc(callfuncO_selector(CArenaLayer::InserItemByIdx));
    
    model = m_fPositionMap[enArenaLayer_SpecialList];
    m_listSpecial = new CListLayer(this, callfuncO_selector(CArenaLayer::cbList));
    m_listSpecial->setPreTopShow(model->getTopShow());
    m_listSpecial->setPreBottomShow(model->getBottonShow());
    addChild(m_listSpecial, -1);
    m_listSpecial->release();
    
    model = m_fPositionMap[enArenaLayer_SangoList];
    m_listSango = new CListLayer(this, callfuncO_selector(CArenaLayer::cbList));
    m_listSango->setPreTopShow(model->getTopShow());
    m_listSango->setPreBottomShow(model->getBottonShow());
    addChild(m_listSango);
    m_listSango->release();
    m_listSango->setReloadFunc(callfuncO_selector(CArenaLayer::InsertListSangoByIdx));
    
    model = m_fPositionMap[enArenaLayer_RoomList];
    m_listRoom = new CListLayer(this, callfuncO_selector(CArenaLayer::cbList));
    m_listRoom->setPreTopShow(model->getTopShow());
    m_listRoom->setPreBottomShow(model->getBottonShow());
    addChild(m_listRoom, -1);
    m_listRoom->release();
    
    model = m_fPositionMap[enArenaLayer_HelperList];
    m_listHelper = new CListLayer(this, callfuncO_selector(CArenaLayer::cbList), 5);
    m_listHelper->setPreTopShow(model->getTopShow());
    m_listHelper->setPreBottomShow(model->getBottonShow());
    addChild(m_listHelper, -1);
    m_listHelper->release();
    m_listHelper->setReloadFunc(callfuncO_selector(CArenaLayer::InserListHelpByIdx));
    
    m_cfmLayer = new CConfirmLayer(this, callfuncO_selector(CArenaLayer::cbCfm));
    addChild(m_cfmLayer, 10);
    m_cfmLayer->release();
    
    m_lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 0), 640, 960);
	addChild(m_lcCover, 9);
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::btnCallback) );
    m_miBack->setTag(kArenaBackBtn);
    m_miBack->setAnchorPoint(CCPointZero);
    
    spMenu1 = CCSprite::spriteWithFile("view_other_record_on.png");
    spMenu2 = CCSprite::spriteWithFile("view_other_record_on.png");
    m_view_other_record = (CCMenuItemImage*)CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::btnCallback) );
    m_view_other_record->setTag(kArenaViewReocrdBtnOff);
    m_view_other_record->setAnchorPoint(CCPointZero);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("id_normal.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("id_normal.png");
    m_miNormal = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::btnCallback) );
    m_miNormal->setTag(kArenaNormalBtn);
    m_miNormal->setAnchorPoint(CCPointZero);
    m_miNormal->setIsVisible(false);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("id_activity.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("id_activity.png");
    m_miSpecial = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::btnCallback) );
    m_miSpecial->setTag(kArenaSpecialBtn);
    m_miSpecial->setAnchorPoint(CCPointZero);
    
    //    spMenu1 = CCSprite::spriteWithSpriteFrameName("sgyyNew.png");
    //    spMenu2 = CCSprite::spriteWithSpriteFrameName("sgyyNew.png");
    
    
    
    
    spMenu1 = CCSprite::spriteWithFile("sgyyNew.png");
    spMenu2 = CCSprite::spriteWithFile("sgyyNew.png");
    
    m_miSango = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::btnCallback) );
    m_miSango->setTag(kArenaSango);
    m_miSango->setAnchorPoint(CCPointZero);
    
    newSpecialRoomLogo = CCSprite::spriteWithSpriteFrameName("ma_new.png");
    CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                      CCFadeIn::actionWithDuration(0.5),
                                                                      NULL);
    newSpecialRoomLogo->runAction(CCRepeatForever::actionWithAction(action));
    m_miSpecial->addChild(newSpecialRoomLogo);
    newSpecialRoomLogo->setPosition(CCPoint(30, 60));
    
    CCMenu* menu = CCMenu::menuWithItems( m_miBack, m_view_other_record, m_miSpecial, m_miNormal, m_miSango, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_view_other_record -> setPosition(CCPointMake(-1000, -1000));
    m_miSpecial->setPosition(CCPointMake(-1000, -1000));
    m_miNormal->setPosition(CCPointMake(-1000, -1000));
    m_miSango->setPosition(CCPointMake(-1000, -1000));
    
    m_fightMngr = new FightMngr(this, callfuncO_selector(CArenaLayer::cbFight));
    addChild(m_fightMngr, 99);
    m_fightMngr->release();
    
    m_cashin = new CashInLayer(this, callfunc_selector(CArenaLayer::cbCashin));
    addChild(m_cashin);
    m_cashin->release();
    
    m_addFriend = new AddFriend(this, callfuncO_selector(CArenaLayer::cbAddFriend));
    addChild(m_addFriend);
    m_addFriend->release();
    
    
    CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(sp1, sp2, this, menu_selector(CArenaLayer::cbRangeBtn));
    m_rangeMenu = CCMenu::menuWithItem(item);
    item->setAnchorPoint(ccp(0, 0));
    addChild(m_rangeMenu);
    
    rangeLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_property"),CCSizeMake(150,80),CCTextAlignmentCenter  , 25);
    m_spTitle->addChild(rangeLabel);
    rangeLabel->setPosition(ccp(335, 15));
    rangeLabel->setIsVisible(false);
    
    m_rangeMenu->setPosition(ccp(1000, 766));
    
    m_lbRangeTitle = TextNode::textWithString("", 26);
    m_lbRangeTitle->setPosition(ccp(53, 10));
    
    m_lbRangeTitleBg = TextNode::textWithString("", 26);
    item->addChild(m_lbRangeTitleBg);
    m_lbRangeTitleBg->setPosition(ccp(53+2, 10-2));
    m_lbRangeTitleBg->setColor(ccc3(0, 0, 0));
    
    item->addChild(m_lbRangeTitle);
    
    m_lbRangeTitle->setString(CGameData::Inst()->getLanguageValue("range_title"));
    m_lbRangeTitleBg->setString(CGameData::Inst()->getLanguageValue("range_title"));
    m_frRange = enFriendRangePro;
    
    //------------------------------------end-----------------------------------//
    
    addFloorNameImage();
    
    m_curReqType = enArenaReqType_Null;
    
    if (SaveData::Inst()->hasValidSavedData()) {
        enterValidArena();
    }
    else {
        int totalCost = CGameData::Inst()->getCurTeamCost();
        m_miNormal->setIsVisible(false);
        if (CGameData::Inst()->isNewbie()) {
            m_miSpecial->setIsVisible(false);
        }else {
            m_miSpecial->setIsVisible(true);
        }
        m_miSango->setIsVisible(false);
        if(CGameData::Inst()->getIsNewSpecialRoomExist())
        {
            newSpecialRoomLogo->setIsVisible(true);
        }else {
            newSpecialRoomLogo->setIsVisible(false);
        }
        
        if (CGameData::Inst()->hasSameUcidInTeamInfo()) {
            MainScene::Inst()->showTeamBugDlg();
            return;
        }
        
        if(totalCost > CGameData::Inst()->getUsrInfo()->cost)
        {
            MainScene::Inst()->showFullCostDlg();
        
            return;
        }

        if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
        {
            MainScene::Inst()->showFullPackDlg();
        
        }
        else
        {
            enterNmlList();
        }
        
        if(CGameData::Inst()->isNewbie() && (CGameData::Inst()->getNewbieStep() == 1)){
            Arrow* pArrow = new Arrow(30, 255);
            
            pArrow->setPosition(CCPointMake(486, 670));
            
            addChild(pArrow, 500, kArenaArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
        }
        // printf("totoalCost%d cost %d\n",totalCost,CGameData::Inst()->getUsrInfo()->cost);
    }
    if (getIsTouchEnabled() == false) {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -127, true);
    }
}

//******************************************************************************
// exit
//******************************************************************************
void CArenaLayer::exit()
{
    if(this->getChildrenCount() == 0)
    {
        return;
    }
    
    innerExit();
    m_miSpecial->setIsVisible(true);
    m_miNormal->setIsVisible(false);
    m_miSango->setIsVisible(false);
    
    if (m_getCfgAlert)
    {
        m_getCfgAlert->removeFromParentAndCleanup(true);
        m_getCfgAlert = NULL;
    }
    
    if(m_fightMngr)
    {
        m_fightMngr->setIsVisible(false);
        m_fightMngr->exit();
    }
    
    if(m_pHelperArr)
    {
        delete m_pHelperArr;
        m_pHelperArr = NULL;
    }
    
    removeAllChildrenWithCleanup(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("guangquan.plist");

    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    setIsTouchEnabled(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}


//******************************************************************************
// innerExit
//******************************************************************************
void CArenaLayer::innerExit()
{
    if(!isEnter)
    {
        CListLayerModel* model = NULL;
        model = m_fPositionMap[enArenaLayer_NmlList];
        model->setTopShow(m_listNml->getPreTopShow());
        model->setBottonShow(m_listNml->getPreBottomShow());
        model->setY(m_listNml->getPosition().y);
  
        model = m_fPositionMap[enArenaLayer_SpecialList];
        model->setTopShow(m_listSpecial->getPreTopShow());
        model->setBottonShow(m_listSpecial->getPreBottomShow());
        model->setY(m_listSpecial->getPosition().y);
        
        model = m_fPositionMap[enArenaLayer_SangoList];
        model->setTopShow(m_listSango->getPreTopShow());
        model->setBottonShow(m_listSango->getPreBottomShow());
        model->setY(m_listSango->getPosition().y);
        
        model = m_fPositionMap[enArenaLayer_HelperList];
        model->setTopShow(m_listHelper->getPreTopShow());
        model->setBottonShow(m_listHelper->getPreBottomShow());
        model->setY(m_listHelper->getPosition().y);
        
        model = m_fPositionMap[enArenaLayer_RoomList];
        model->setTopShow(m_listRoom->getPreTopShow());
        model->setBottonShow(m_listRoom->getPreBottomShow());
        model->setY(m_listRoom->getPosition().y);
    }
    isEnter = false;
    m_rangeMenu->stopAllActions();
    m_spTitle->stopAllActions();
    m_miBack->stopAllActions();
    m_view_other_record->stopAllActions();
    ResetViewOtherRecordBtn(false);
    m_miSpecial->stopAllActions();
    m_miNormal->stopAllActions();
    m_miSango->stopAllActions();
    
    m_lbRangeTitle->setIsVisible(false);
    m_lbRangeTitleBg->setIsVisible(false);
    rangeLabel->setIsVisible(false);

    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_view_other_record->setPosition(CCPointMake(-1000, -1000));
    m_miSpecial->setPosition(CCPointMake(-1000, -1000));
    m_miNormal->setPosition(CCPointMake(-1000, -1000));
    m_miSango->setPosition(CCPointMake(-1000, -1000));
    m_rangeMenu->setPosition(CCPointMake(-1000, -1000));
    m_cfmLayer->hide();
    m_lcCover->setOpacity(0);
    

    m_listNml->clearAll();
    m_listNml->removeTBobserver();
    
    m_listSpecial->clearAll();
    m_listRoom->clearAll();
    m_listHelper->clearAll();
    m_listSango->clearAll();
    
    if(m_teamCfm){
        removeChild(m_teamCfm, true);
        m_teamCfm = NULL;
    }
    if (m_szDgType && strcmp(m_szDgType, "special") == 0 && FiveTeamLayer) {
        removeChild(FiveTeamLayer, true);
        FiveTeamLayer = NULL;
    }
    if (m_less5Dlg) {
        m_less5Dlg->removeFromParentAndCleanup(true);
        m_less5Dlg = NULL;
    }
    if (m_getCfgAlert) {
        m_getCfgAlert->removeFromParentAndCleanup(true);
        m_getCfgAlert = NULL;
    }
    if (m_dialog1) {
        m_dialog1->removeFromParentAndCleanup(true);
        m_dialog1 = NULL;
    }
    
    if (m_SortBtnLayer) {
        m_SortBtnLayer->stopAllActions();
        removeChild(m_SortBtnLayer, true);
        m_SortBtnLayer = NULL;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// getDgStepInfo
//******************************************************************************
CUserCardInfo*   CArenaLayer::getHelperCardInfo(CFriendHelpInfo *helperInfo)
{
    CFriendHelpInfo* pInfo = helperInfo;
    unsigned short cid = pInfo->leadercard_cid;
    unsigned short lv = pInfo->leadercardlv;
    unsigned short sklv = pInfo->sk_lv;
    
    m_helperCardInfo.i_AwakeNum = helperInfo->i_awakeNum;
    m_helperCardInfo.lv = lv;
    
    m_helperCardInfo.exp = 0;
    m_helperCardInfo.base_exp = 0;
    m_helperCardInfo.upd_time = 0;
    
    m_helperCardInfo.pl_attack = helperInfo->pl_attack;
    m_helperCardInfo.pl_hp = helperInfo->pl_hp;
    m_helperCardInfo.pl_recover = helperInfo->pl_recover;
    m_helperCardInfo.plus = helperInfo->plus;
    
    CCardBaseInfo *card = CGameData::Inst()->getCardBaseInfByCid(pInfo->leadercard_cid);
    m_helperCardInfo.cid = cid;
    if (card) {
        m_helperCardInfo.category = card->category;
        m_helperCardInfo.cou_name = card->cou_name;
        m_helperCardInfo.exp_type = card->exp_type;
        m_helperCardInfo.sk_lv = sklv;
        m_helperCardInfo.star = card->star;
        m_helperCardInfo.skid = card->skid;
        m_helperCardInfo.ctype = card->ctype;
        m_helperCardInfo.stype = card->stype;
        m_helperCardInfo.cost = card->cost;
        m_helperCardInfo.leader_skid = card->leader_skid;
        m_helperCardInfo.max_lv = card->max_lv;
        m_helperCardInfo.ugp_max_star = card->ugp_max_star;
        m_helperCardInfo.sub_category = card->sub_category;
        m_helperCardInfo.upg_gold = card->upg_gold;
        m_helperCardInfo.sell_gold = card->sell_gold;
        
        memset(m_helperCardInfo.AwakeSkillArr, 0, sizeof(m_helperCardInfo.AwakeSkillArr));
        for (int i = 0; i < MaxAwakeSkillCnt; i++)
        {
            m_helperCardInfo.AwakeSkillArr[i] = card->AwakeSkillArr[i];
        }
        
        m_helperCardInfo.find_me = card->find_me;
        
        long recoverbase = card->recover;
        float fRecoverGrowth = card->recover_growth;
        m_helperCardInfo.recover = recoverbase + (lv - 1) * fRecoverGrowth + helperInfo->pl_recover * RECPLUSGROWTH;
        
        long hpbase = card->hp;
        float fHpGrowth = card->hp_growth;
        m_helperCardInfo.hp = hpbase + (lv - 1) * fHpGrowth + helperInfo->pl_hp * HPPLUSGROWTH;
        
        long attackbase = card->attack;
        float fAtkGrowth = card->attack_growth;
        m_helperCardInfo.attack = attackbase + (lv - 1) * fAtkGrowth + helperInfo->pl_attack * ATKPLUSGROWTH;
        
        // 觉醒、涅槃、转生神马的就不写了，这块太坑了
        m_helperCardInfo.bIsEluTgt = false;
        m_helperCardInfo.bIsWkTgt = false;
        m_helperCardInfo.ugp_target = 0;
        m_helperCardInfo.upg_flg = 0;
        memset(m_helperCardInfo.ugp_need, 0, sizeof(m_helperCardInfo.ugp_need));
        
        strncpy(m_helperCardInfo.name, card->name, 99);
    }

    CUserCardInfo* temp = new CUserCardInfo();
    *temp = m_helperCardInfo;
    temp->autorelease();
    return temp;
}
//******************************************************************************
// enterHelperList
//******************************************************************************
void CArenaLayer::enterHelperList()
{
    resetIFriend();
    m_FriendRange = enFriendRangePro;
    rangeLabel->setIsVisible(true);
    m_lbRangeTitle->setIsVisible(true);
    m_lbRangeTitleBg->setIsVisible(true);
    m_rangeMenu->setPosition(CCPoint(-1000, 731));
    m_rangeMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(580-55, 766-35)),0.8f));
    
    if(m_curLayer != enArenaLayer_HelperList)
        m_preLayer = m_curLayer;
    
    m_curLayer = enArenaLayer_HelperList;
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("helperTip"));
    
    m_listHelper->clearAll();
    
    if(m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding){
        m_listHelper->setCellShowCount(4);
    } else {
        m_listHelper->setCellShowCount(5);
    }
    changeHelperListRange(true);

    CListLayerModel* model = m_fPositionMap[enArenaLayer_HelperList];
    m_listHelper->setPosition(CCPointMake(640,model->getY()));
    m_listHelper->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,model->getY())),0.8f));
    
    if(CGameData::Inst()->isNewbie()){
        CGameData::Inst()->setDiamondAllAvail();
        
        m_spTitle->setPosition(CCPointMake(-210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    }
    else {
        m_spTitle->setPosition(CCPointMake(-210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        
        m_miBack->setPosition(CCPointMake(-529, 731));
        m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    }
    

   // snprintf(buf, 99, "naviText_room_%d", int(CCRANDOM_0_1() * 3) + 1);
   
    MainScene::Inst()->showBottomPromptInfo("naviText_room_");
    //
    FiveTeamLayer = CCSprite::spriteWithFile("fr_substrate.png");
    if(FiveTeamLayer != NULL && m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding){
        FiveTeamLayer->setPosition(CCPointMake(320, 190));
        addChild(FiveTeamLayer);
        
        char buf[100];
        CCSprite *spMenu1 = CCSprite::spriteWithFile("fr_substrate.png");
        CCSprite *spMenu2 = CCSprite::spriteWithFile("fr_substrate.png");
        CCMenuItem *menuBG = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::cbFiveTeamLayerBG));
        menuBG->setAnchorPoint(CCPointZero);
        menuBG->setPosition(CCPointMake(0,0));
        
        spMenu1 = CCSprite::spriteWithFile("fr_button2.png");
        spMenu2 = CCSprite::spriteWithFile("fr_button2.png");
        CCMenuItem *reset = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::cbFiveTeamLayerReset));
        reset->setAnchorPoint(CCPointZero);
        reset->setPosition(CCPointMake(5, 8));
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("helperFiveTeamResetTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(10, 20));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        reset->addChild(lbText, 1);
        
        spMenu1 = CCSprite::spriteWithFile("fr_button2.png");
        spMenu2 = CCSprite::spriteWithFile("fr_button2.png");
        CCMenuItem *sure = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::cbFiveTeamLayerSure));
        sure->setAnchorPoint(CCPointZero);
        sure->setPosition(CCPointMake(515, 8));
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("helperFiveTeamSureTip"));
        lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(10, 20));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        sure->addChild(lbText, 1);
        
        
        FiveTeamBGMenu = CCMenu::menuWithItems(menuBG, NULL);
        FiveTeamMenu = CCMenu::menuWithItems(reset, sure, NULL);
        FiveTeamLayer->addChild(FiveTeamBGMenu, 0);
        FiveTeamLayer->addChild(FiveTeamMenu, 1);
        FiveTeamMenu->setPosition(CCPointZero);
        FiveTeamBGMenu->setPosition(CCPointZero);
    
        refreshFiveTeamMember();
    }
}


void CArenaLayer::FiveTeamMemberSelected(int number)
{
    bool haveSelected = false;
    for (int i = 0; i < 5; i++) {
        if (m_iFriend[i] == number) {  // 去除已选中标记
            haveSelected = true;
            m_iFriend[i] = -1;
            m_listHelper->setNumber(number, 0, false);
        }
    }
    if (!haveSelected) { // 添加已选中标记
        for (int i = 0; i < 5; i++) {
            if (m_iFriend[i] == -1) {
                m_iFriend[i] = number;
                m_listHelper->setNumber(number, i+1, true);
                break;
            }
        }
        int num = 0;
        for (int i = 0; i < 5; i++) {
            if (m_iFriend[i] != -1) {
                num++;
            }
        }
        if (num == 5) { // 已经选满5个 -> 选中的cell正常显示，没有选中的变灰
            //////////////
            for (int k = 0; k < m_listHelper->getItemCount(); k++) {
                bool isDone = false;
                for (int i = 0; i < 5; i++) {
                    if (m_iFriend[i] == k) {
                        isDone = true;
                        m_listHelper->setNumber(k, i+1, false);
                        break;
                    }
                }
                if (!isDone) {
                    m_listHelper->setNumber(k, 0, true);
                }
            }
        }
    } else {
        int num = 0;
        for (int i = 0; i < 5; i++) {
            if (m_iFriend[i] != -1) {
                num++;
            }
        }
        if (num == 4) { // 由已选满变为没有 -> 选中的cell变灰，没有选中的正常显示
            //////////////
            for (int k = 0; k < m_listHelper->getItemCount(); k++) {
                bool isDone = false;
                for (int i = 0; i < 5; i++) {
                    if (m_iFriend[i] == k) {
                        isDone = true;
                        m_listHelper->setNumber(k, i+1, true);
                        break;
                    }
                }
                if (!isDone) {
                    m_listHelper->setNumber(k, 0, false);
                }
            }
        }
    }
    refreshFiveTeamMember();
    printf("%i_%i_%i_%i_%i", m_iFriend[0], m_iFriend[1], m_iFriend[2], m_iFriend[3], m_iFriend[4]);
}

void CArenaLayer::refreshFiveTeamMember()
{
    char buf[100];
    
    if (FiveTeam1) {
        FiveTeamLayer->removeChild(FiveTeam1, true);
    }
    if (m_iFriend[0] != -1) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[0]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam1 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam1 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam1->setAnchorPoint(CCPointZero);
    FiveTeam1->setScale(0.7);
    FiveTeam1->setPosition(CCPointMake(128 + 77*0, 14));
    FiveTeamLayer->addChild(FiveTeam1, 10);
    
    if (FiveTeam2) {
        FiveTeamLayer->removeChild(FiveTeam2, true);
    }
    if (m_iFriend[1] != -1) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[1]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam2 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam2 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam2->setAnchorPoint(CCPointZero);
    FiveTeam2->setScale(0.7);
    FiveTeam2->setPosition(CCPointMake(128 + 77*1, 14));
    FiveTeamLayer->addChild(FiveTeam2, 10);
    
    if (FiveTeam3) {
        FiveTeamLayer->removeChild(FiveTeam3, true);
    }
    if (m_iFriend[2] != -1) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[2]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam3 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam3 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam3->setAnchorPoint(CCPointZero);
    FiveTeam3->setScale(0.7);
    FiveTeam3->setPosition(CCPointMake(128 + 77*2, 14));
    FiveTeamLayer->addChild(FiveTeam3, 10);
    
    if (FiveTeam4) {
        FiveTeamLayer->removeChild(FiveTeam4, true);
    }
    if (m_iFriend[3] != -1) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[3]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam4 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam4 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam4->setAnchorPoint(CCPointZero);
    FiveTeam4->setScale(0.7);
    FiveTeam4->setPosition(CCPointMake(128 + 77*3, 14));
    FiveTeamLayer->addChild(FiveTeam4, 10);
    
    if (FiveTeam5) {
        FiveTeamLayer->removeChild(FiveTeam5, true);
    }
    if (m_iFriend[4] != -1) {
        CFriendHelpInfo* pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[4]);
        CUserCardInfo* pUsrCardInfo = getHelperCardInfo(pHelperInfo);
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam5 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam5 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam5->setAnchorPoint(CCPointZero);
    FiveTeam5->setScale(0.7);
    FiveTeam5->setPosition(CCPointMake(128 + 77*4, 14));
    FiveTeamLayer->addChild(FiveTeam5, 10);
}

void CArenaLayer::cbFiveTeamLayerBG(CCObject* sender)
{
    printf("bg");
}

void CArenaLayer::cbFiveTeamLayerReset(CCObject* sender)
{
    resetIFriend();
    for (int k = 0; k < m_listHelper->getItemCount(); k++) {
        m_listHelper->setNumber(k, 0, false);
    }
    refreshFiveTeamMember();
}

void CArenaLayer::cbFiveTeamLayerSure(CCObject* sender)
{
    int num = 0;
    for (int i = 0; i < 5; i++) {
        if (m_iFriend[i] != -1) {
            num++;
        }
    }
    if (num == 5) {
        showFiveTeamCfmLayer();
    } else {

        if (m_less5Dlg == NULL)
        {
            m_less5Dlg = new Dialog1(this,
                                    callfuncO_selector(CArenaLayer::cbLess5Dlg),
                                    CGameData::Inst()->getLanguageValue("friend5"),
                                    false);
            addChild(m_less5Dlg, 100);
            m_less5Dlg->release();
        }
        
        m_less5Dlg->setPosition(CCPointMake(320, 480 - getPosition().y));
        m_less5Dlg->setScale(0.0);
        m_less5Dlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
}


void CArenaLayer::cbLess5Dlg()
{
    if (m_less5Dlg)
    {
        m_less5Dlg->removeFromParentAndCleanup(true);
        m_less5Dlg = NULL;
    }
}


void CArenaLayer::cbRangeBtn(CCObject* sender)
{
    
    const char* szStringTbl[] = {
        "sortbtn_prop",
        "sortbtn_hp",
        "sortbtn_atk",
        "sortbtn_rcv",
        "sortbtn_countryportery",
    };
    
    CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
    CCSize szBtn(0,0);
    if (spTemp)
        szBtn = spTemp->getContentSize();
    
    CCPoint ptBase(632 - szBtn.width, 720 - szBtn.height);
    
    if (m_SortBtnLayer == NULL) {
        m_SortBtnLayer = CCLayer::node();
        addChild(m_SortBtnLayer);
        
        for (int i = 0; i < 5; i++) {
            CCSprite* spMenu1 = CCSprite::spriteWithFile("sortbtn.png");
            CCSprite* spMenu2 = CCSprite::spriteWithFile("sortbtn.png");
            CCMenuItem* pMI = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CArenaLayer::onSortBtnTouch) );
            pMI->setTag(i);
            pMI->setAnchorPoint(CCPointZero);
            
            TextNode* txt = TextNode::textWithString(CGameData::Inst()->getLanguageValue(szStringTbl[i]),24);
            txt->setPosition(CCPointMake(szBtn.width * 0.5 + 6, szBtn.height * 0.5));
            txt->setColor(ccWHITE);
            txt->setShadowColor(ccBLACK);
            pMI->addChild(txt);
            
            CCMenu* menu = CCMenu::menuWithItems(pMI, NULL);
            m_SortBtnLayer->addChild(menu, 30);
            menu->setPosition(CCPointZero);
            
            pMI->setPosition(ccp(ptBase.x, ptBase.y - (szBtn.height + 4) * i));
        }
        
        m_SortBtnLayer->setPosition(ccp(640-ptBase.x, 0));
        m_SortBtnLayer->runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(0.2, CCPointZero), 0.3));
    }
    else if(m_SortBtnLayer->numberOfRunningActions() == 0){
        if (m_SortBtnLayer->getPosition().x > 0) {
            m_SortBtnLayer->runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(0.2, CCPointZero), 0.3));
        }
        else{
            m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(640-ptBase.x, 0)), 0.3),
                                                          CCCallFunc::actionWithTarget(this, callfunc_selector(CArenaLayer::onSortBtnLayerExit)),
                                                          NULL));
        }
    }
}

//******************************************************************************
// onSortBtnLayerExit
//******************************************************************************
void CArenaLayer::onSortBtnLayerExit()
{
    if (m_SortBtnLayer) {
        removeChild(m_SortBtnLayer, true);
        m_SortBtnLayer = NULL;
    }
}

//******************************************************************************
// onSortBtnTouch
//******************************************************************************
void CArenaLayer::onSortBtnTouch(CCObject* sender)
{
    CCMenuItem* mi = (CCMenuItem*) sender;
    m_frRange = mi->getTag() + 1;
    if (m_frRange == enFriendRangeLT) { // 跳过登录时间排序
        m_frRange ++;
    }
    if(m_frRange > enFriendRangeCou)
        m_frRange = enFriendRangePro;
    
    changeHelperListRange(false);
    
    CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
    CCSize szBtn(0,0);
    if (spTemp)
        szBtn = spTemp->getContentSize();
    
    CCPoint ptBase(632 - szBtn.width, 720 - szBtn.height);
    m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(640-ptBase.x, 0)), 0.3),
                                                  CCCallFunc::actionWithTarget(this, callfunc_selector(CArenaLayer::onSortBtnLayerExit)),
                                                  NULL));

}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CArenaLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_SortBtnLayer) {
        CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
        CCSize szBtn(0,0);
        if (spTemp)
            szBtn = spTemp->getContentSize();
        
        m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(szBtn.width + 8, 0)), 0.3),
                                                      CCCallFunc::actionWithTarget(this, callfunc_selector(CArenaLayer::onSortBtnLayerExit)),
                                                      NULL));
    }
    
    return false;
}



void CArenaLayer::createHelperList(enFriendRange type)
{
    
    m_FriendRange = type;
    
    
    for(int i=0;i<m_pHelperArr->count();i++)
    {
        CMenuBtn *btn;
        
        if ((i <= m_listHelper->getPreBottomShow())
            &&(i >= m_listHelper->getPreTopShow())
            )
        {
            CFriendHelpInfo *fInfo = m_pHelperArr->getObjectAtIndex(i);
            btn = new CMenuBtn(fInfo,type);
            //
            int num = 0;
            for (int k = 0; k < 5; k++) {
                if (m_iFriend[k] != -1) {
                    num++;
                }
            }
            if (num == 5) { //反过来 选择的灰色cover去掉，没有选中的加灰色cover
                int k = 0;
                for (; k < 5; k++) {
                    if (m_iFriend[k] == i) {
                        btn->setHelperNo(false, k+1);
                        break;
                    }
                }
                if (k == 5) {
                    btn->setHelperNo(true, 0);
                }
            } else { // 选择的加灰色cover
                int k = 0;
                for (; k < 5; k++) {
                    if (m_iFriend[k] == i) {
                        btn->setHelperNo(true, k+1);
                        break;
                    }
                }
                if (k == 5) {
                    btn->setHelperNo(false, 0);
                }
            }

        }
        else
        {
            btn = NULL;
        }
        
        m_listHelper->push_back(btn);
        
        if (btn != NULL)
        {
            btn->release();
        }
    }
}

void CArenaLayer::changeHelperListRange(bool isUpdate)
{
    if(m_curLayer == enArenaLayer_Cfm){
        return;
    }
    
    CCMutableArray<CFriendHelpInfo*> *initialArray = CCMutableArray<CFriendHelpInfo *>::arrayWithArray(CGameData::Inst()->getFriendHelpArr(isUpdate));
    //
    string fidArr[5];
    int num = 0;
    for (int i = 0; i < 5; i++) {
        if (m_iFriend[i] != -1) {
            num++;
        }
    }
    if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
        for (int i = 0; i < 5; i++) {
            if (m_iFriend[i] == -1) {
                continue;
            }
            fidArr[i] = m_pHelperArr->getObjectAtIndex(m_iFriend[i])->fid;
        }
    }
    resetIFriend();
    //
    m_listHelper->clearAll();
    if(m_pHelperArr)
        m_pHelperArr->removeAllObjects();
    else {
        m_pHelperArr =  new CCMutableArray<CFriendHelpInfo *>();
    }
    //按照属性排序
    if(m_frRange == enFriendRangePro)
    {
        for(int j=0;j<=5;j++)
        {
            for(int i=0;i<initialArray->count();i++)
            {
                CUserCardInfo *info = getHelperCardInfo( initialArray->getObjectAtIndex(i));
                CFriendHelpInfo *tempFrInfp = initialArray->getObjectAtIndex(i);
                if(info->ctype == j)
                {
                    m_pHelperArr->addObject(tempFrInfp);
                }
            }
        }
        int index = 0;
        for(int i=0;i<m_pHelperArr->count();i++)
        {
            if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
                for (int k = 0 ; k < 5; k++) {
                    string fId = m_pHelperArr->getObjectAtIndex(i)->fid;
                    if (fId == fidArr[k]) {
                        m_iFriend[k] = index;
                        break;
                    }
                }
            }
            index++;
        }
        createHelperList(enFriendRangePro);
    }
    //按照血量排序
    if(m_frRange == enFriendRangeHp)
    {
        for(int i=0;i<initialArray->count()-1;i++)
        {
            for (int j=0;j< initialArray->count()-1-i; j++) {
                int hp1 = getHelperCardInfo(initialArray->getObjectAtIndex(j))->hp;
                int hp2 = getHelperCardInfo(initialArray->getObjectAtIndex(j+1))->hp;
                if(hp1 < hp2)
                {
                    CFriendHelpInfo *tempFr1 = initialArray->getObjectAtIndex(j);
                    CFriendHelpInfo *tempFr2 = initialArray->getObjectAtIndex(j+1);
                    initialArray->replaceObjectAtIndex(j, tempFr2);
                    initialArray->replaceObjectAtIndex(j+1, tempFr1);
                }
            }
        }
        for(int i=0;i<initialArray->count();i++)
        {
            m_pHelperArr->addObject(initialArray->getObjectAtIndex(i));
        }
        int index = 0;
        for(int i=0;i<m_pHelperArr->count();i++)
        {
            if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
                for (int k = 0 ; k < 5; k++) {
                    string fId = m_pHelperArr->getObjectAtIndex(i)->fid;
                    if (fId == fidArr[k]) {
                        m_iFriend[k] = index;
                        break;
                    }
                }
            }
            index++;
        }
        createHelperList(enFriendRangeHp);
    }
    //按照攻击力排序
    if(m_frRange == enFriendRangeAtk)
    {
        for(int i=0;i<initialArray->count()-1;i++)
        {
            for (int j=0;j< initialArray->count()-1-i; j++) {
                int attack1 = getHelperCardInfo(initialArray->getObjectAtIndex(j))->attack;
                int attack2 = getHelperCardInfo(initialArray->getObjectAtIndex(j+1))->attack;
                if(attack1 < attack2)
                {
                    CFriendHelpInfo *tempFr1 = initialArray->getObjectAtIndex(j);
                    CFriendHelpInfo *tempFr2 = initialArray->getObjectAtIndex(j+1);
                    initialArray->replaceObjectAtIndex(j, tempFr2);
                    initialArray->replaceObjectAtIndex(j+1, tempFr1);
                }
            }
        }
        for(int i=0;i<initialArray->count();i++)
        {
            m_pHelperArr->addObject(initialArray->getObjectAtIndex(i));
        }
        int index = 0;
        for(int i=0;i<m_pHelperArr->count();i++)
        {
            if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
                for (int k = 0 ; k < 5; k++) {
                    string fId = m_pHelperArr->getObjectAtIndex(i)->fid;
                    if (fId == fidArr[k]) {
                        m_iFriend[k] = index;
                        break;
                    }
                }
            }
            index++;
        }
        createHelperList(enFriendRangeAtk);
    }
    //按照武将国家属性排序
    if (m_frRange == enFriendRangeCou) {
        
        int nCountryPorperty1 = 0,nCountryPorperty2 = 0;
        const int nConstCount = initialArray->count();
        for (int i = 0; i < nConstCount-1; ++i)
        {
            for (int j = 0; j < nConstCount-1-i; ++j)
            {
                nCountryPorperty1 = getHelperCardInfo(initialArray->getObjectAtIndex(j))->cou_name;
                nCountryPorperty2 = getHelperCardInfo(initialArray->getObjectAtIndex(j+1))->cou_name;
                
                if (nCountryPorperty1 > nCountryPorperty2)
                {
                    CFriendHelpInfo* tempFr1 = initialArray->getObjectAtIndex(j);
                    CFriendHelpInfo* tempFr2 = initialArray->getObjectAtIndex(j+1);
                    initialArray->replaceObjectAtIndex(j,tempFr2);
                    initialArray->replaceObjectAtIndex(j+1,tempFr1);
         
                    }
                }
            }
      
        
        for (int i = 0; i < nConstCount; ++i)
        {
            m_pHelperArr->addObject(initialArray->getObjectAtIndex(i));
       }
        int index = 0;
        for(int i=0;i<m_pHelperArr->count();i++)
        {
            if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
                for (int k = 0 ; k < 5; k++) {
                    string fId = m_pHelperArr->getObjectAtIndex(i)->fid;
                    if (fId == fidArr[k]) {
                        m_iFriend[k] = index;
                        break;
                    }
                }
            }
            index++;
        }
        createHelperList(enFriendRangeCou);
        
    }
    
    
    
    //按照回复力排序
    if(m_frRange == enFriendRangeRec)
    {
        for(int i=0;i<initialArray->count()-1;i++)
        {
            for (int j=0;j< initialArray->count()-1-i; j++) {
                int rec1 = getHelperCardInfo(initialArray->getObjectAtIndex(j) )->recover;
                int rec2 = getHelperCardInfo(initialArray->getObjectAtIndex(j+1) )->recover;
                if(rec1 < rec2)
                {
                    CFriendHelpInfo *tempFr1 = initialArray->getObjectAtIndex(j);
                    CFriendHelpInfo *tempFr2 = initialArray->getObjectAtIndex(j+1);
                    initialArray->replaceObjectAtIndex(j, tempFr2);
                    initialArray->replaceObjectAtIndex(j+1, tempFr1);
                }
            }
        }
        for(int i=0;i<initialArray->count();i++)
        {
            m_pHelperArr->addObject(initialArray->getObjectAtIndex(i));
        }
        int index = 0;
        for(int i=0;i<m_pHelperArr->count();i++)
        {
            if (m_szDgType && strcmp(m_szDgType, "special") == 0 && getSpecialDgInfo()->skillType == enSpecialDgSkillType_quanduiwuxianding && num) {
                for (int k = 0 ; k < 5; k++) {
                    string fId = m_pHelperArr->getObjectAtIndex(i)->fid;
                    if (fId == fidArr[k]) {
                        m_iFriend[k] = index;
                        break;
                    }
                }
            }
            index++;
        }
        //重排好友
//        for(int i=0;i<m_pHelperArr->count();i++)
//        {
//            CFriendHelpInfo *fInfo = m_pHelperArr->getObjectAtIndex(i);
//            CMenuBtn *btn = new CMenuBtn(fInfo,enFriendRangeRec); 
//            m_listHelper->push_back(btn);
//            btn->release();
//        }
        createHelperList(enFriendRangeRec);
    }
    
    //文字
    char buf[100];
    switch (m_frRange) {
        case enFriendRangeAtk:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_attack"));
            break;
        case enFriendRangeLT:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_lt"));
            break;
        case enFriendRangeHp:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_hp"));
            break;
        case enFriendRangePro:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_property"));
            break;
        case enFriendRangeRec:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_recover"));
            break;
        case enFriendRangeCou:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_countryporte"));
            break;
        default:
            break;
    }
    rangeLabel->setString(buf);
}
//******************************************************************************
// update
//******************************************************************************
void CArenaLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    bool bLoadCardEnd = true;
    if (m_curReqType == enArenaReqType_dgend)
         bLoadCardEnd = CGameData::Inst()->loadUserCardsStep(10);
    
    if(comState == kReqStatRequesting || (!bLoadCardEnd))
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(CArenaLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        
        if(CGameData::Inst()->isNewbie() == false)
            MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();

    if(comState == kReqStatOk){
        switch (m_curReqType)
        {
            case enArenaReqType_getfightcfg:
                enterSpecialList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enArenaReqType_helper:
                //援军更新之后，好友列表也要重新取
                CGameData::Inst()->setFrndLstReqTime(0);
                
                enterHelperList();
                CGameData::Inst()->setNeedToReqHelper(false);
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enArenaReqType_newbie2:
                CGameData::Inst()->initAfterLogin();
                enterHelperList();
                CGameData::Inst()->setNeedToReqHelper(false);
               //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enArenaReqType_dungeon:
                CGameData::Inst()->clrPreCmtTeamInfo();
                CGameData::Inst()->saveAllTeamInfo();
                enterArena();
                
                char msg[40];
                sprintf(msg, "%s&&%d&&%d", m_szDgType , m_iFloorId , m_iRoomId);
                
                
                
#ifdef TalkData
                TDCCMission::onBegin(msg);
#else
                
#endif
                
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enArenaReqType_dgend:
                SaveData::Inst()->destorySavedFile();
                addFloorNameImage();
                
                if(CGameData::Inst()->allCardRequest()){
                    m_curReqType = enArenaReqType_allcards;
                    scheduleUpdate();
                }
                
                
                
                char msg1[40];
                sprintf(msg1, "%s&&%d&&%d", m_szDgType , m_iFloorId , m_iRoomId);
                
#ifdef TalkData
                TDCCMission::onCompleted(msg1);
#else
                
#endif
                
                MainScene::Inst()->saveArenaShowTips_gonggao();
             //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16

                break;
            case enArenaReqType_dgendforfanti://返体
                if (CGameData::Inst()->getErrMsg() == NULL) { //没有返体效果
                   
                    SaveData::Inst()->destorySavedFile();
                    
                    addFloorNameImage();
                    
                    if (CGameData::Inst()->chkLogin()) {
                        SaveData::Inst()->destorySavedData();
                        exit();
                        enter();
                        goDirectRoom();
                        
                        MainScene::Inst()->showBottomPanel(true);
                        MainScene::Inst()->showUsrInfPanel(true);
                        MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                    }
                    else {
                        MainScene::Inst()->reLogin();
                    }
                    return;
                    
                    
                }else{//有返体效果
                    
                    if (m_getFantiAlert == NULL)
                    {
                        m_getFantiAlert = new Dialog1(this,
                                                      callfuncO_selector(CArenaLayer::cbGetFantiAlert),
                                                      CGameData::Inst()->getErrMsg(),
                                                      false);
                        addChild(m_getFantiAlert, 100);
                        m_getFantiAlert->release();
                    }
                    
                    m_getFantiAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                    m_getFantiAlert->setScale(0.0);
                    m_getFantiAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                    
                }
                
               
                
                break;

            case enArenaReqType_newbie3:
            case enArenaReqType_allcards:
                MainScene::Inst()->showUsrInfPanel(true);
                MainScene::Inst()->showBottomPanel(false);
                
                showCashinLayer();
                
               
                if(CGameData::Inst()->getUsrInfo()->add_card == 0)//开关
                {
                    for (int i = 0; i < CGameData::Inst()->getUserCards()->count(); i++)
                    {
                        CUserCardInfo* userCard = CGameData::Inst()->getUserCards()->getObjectAtIndex(i);
                        
                        
                        
                        if(userCard->plus != 0)
                        {
                            MainScene::Inst()->new_CardAdd = true;
                            
                            
                            if(CGameData::Inst()->newChangeRequest("add_card"))
                            {
                                
                                
                                
                                m_curReqType = enArenaReqType_cardAdd;
                                scheduleUpdate();
                            }
                            
                            break;
                        }
                        
                        
                    }
                }
                
                

                
                break;
            case enArenaReqType_cardAdd:
                
                break;
            case enArenaReqType_addfriend:
                MainScene::Inst()->showBottomPanel(true);
            
                if(CGameData::Inst()->isNewbie()){
                    innerExit();
                    MainScene::Inst()->newbieFightEnd();
                    MainScene::Inst()->enableBottomPanel(false);
                }
                else{
                    if (CGameData::Inst()->chkLogin()) {
                        enter();
                        goDirectRoom();
                        MainScene::Inst()->afterArenaShowTips_gonggao();
                        // MainScene::Inst()->showBottomPanel(true);
                    }
                    else {
                        MainScene::Inst()->reLogin();
                    }
                }
                break;
                
            case enArenaReqType_stamina:
                if(m_curLayer == enArenaLayer_NmlRoom){
                    enterNmlRoom();
                }
                else if(m_curLayer == enArenaLayer_SpecialRoom){
                    enterSpecialRoom();
                }
                else if (m_curLayer == enArenaLayer_SangoRoom)
                {
                    enterSangoRoom();
                }
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            case enArenaReqType_req_view_other_record_vid:
                RequestUrl();
                break;
                
            default:
                break;
        }
    }
    else {
        if (403 == comState)
        {
            if (m_curReqType == enArenaReqType_dgend)
            {
                CGameData::Inst()->setIsFighting(false);
                CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni.plist");
                CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni_2.plist");
                CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("guangquan.plist");
                
                CCTextureCache::sharedTextureCache()->removeUnusedTextures();
                MainScene::Inst()->switchBgMusic(enBgMusicNml);
                
                MainScene::Inst()->reloginWeibo(1);
            }
            else
            {
                MainScene::Inst()->reloginWeibo(3);
            }
            return;
        }
        
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(CArenaLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            
            if((m_curReqType == enArenaReqType_dgend)
               ||(m_curReqType == enArenaReqType_dungeon) || m_curReqType == enArenaReqType_dgendforfanti)
            {
                m_commDlg->setNoCancel();
            }
        }
    }
}


void CArenaLayer::goDirectRoom()
{
    if (m_recordCurLayer == enArenaLayer_NmlFloor)
    {
        enterNmlFloor();
        return;
    }
    
    if (m_recordCurLayer == enArenaLayer_SpecialFloor)
    {
        enterSpecialList();
        
        int indexSel = getSeciaSelByFloorID(m_iFloorId);
        
        if (indexSel >= 0)
        {
            m_iFloorSel = indexSel;
            enterSpecialFloor();
        }
        return;
    }
    
    if (m_recordCurLayer == enArenaLayer_SangoFloor)
    {
        enterSangoList();
        enterSangoFloor();
        return;
    }
}

//******************************************************************************
// enterNmlFloor
//******************************************************************************
void CArenaLayer::enterNmlFloor()
{
    innerExit();
    
    CUserInfo usrinfo = *(CGameData::Inst()->getUsrInfo());
    
    if(CGameData::Inst()->isNewbie()){
        m_szDgType = NULL;
        CGameData::Inst()->newbieRequest(0, 2, NULL);
        m_curReqType = enArenaReqType_newbie2;//新手 请求三个援军
        scheduleUpdate();
    }
    else
    {
        CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
        CNmlDgInfo* pNmlDgInfo;
        
        m_iFloorId = pNmlDgInfoArr->count() - m_iFloorSel;
        
        pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(m_iFloorId -1);
        
//        CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
//        m_iFloorId = (pNmlDgInfoArr->count() - m_iFloorSel) - 1;
//        CNmlDgInfo* pNmlDgInfo = NULL;
//        int index = 0;
//        for(int i = 0; i < pNmlDgInfoArr->count(); i++)
//        {
//            pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(i);
//            
//            if(pNmlDgInfo == NULL)
//                continue;
//            
//            if (CGameData::Inst()->getUsrInfo()->lv < pNmlDgInfo->minlevel || CGameData::Inst()->getUsrInfo()->lv > pNmlDgInfo->maxlevel)
//                continue;
//            
//            if (index == m_iFloorId)
//                break;
//            
//            index++;        
//        }
        
        
        m_fightSceneType = pNmlDgInfo->dungeon_type;
        m_ushLimitMaxStar = pNmlDgInfo->m_unLimitMaxStar;
        m_spTitle->setTitleSprite(pNmlDgInfo->name);
        
        CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
        CMenuBtn* btn;
        CDgRoomInfo* pRoomInfo;
        for(int i = pRoomInfoArr->count() -1; i >= 0; i--){
            pRoomInfo = pRoomInfoArr->getObjectAtIndex(i);
            
            if(pRoomInfo == NULL)
                continue;
            
            //wuliang
            btn = new CMenuBtn(pRoomInfo, m_iFloorId, i + 1, pNmlDgInfo->bg_type);
            
            m_listRoom->push_back(btn);
            btn->release();
        }
        
        m_spTitle->setPosition(CCPointMake(-210, 766));
        m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        
        m_miBack->setPosition(CCPointMake(-529, 731));
        m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
        
        if (pRoomInfo->video)
        {
            m_view_other_record->setPosition(CCPointMake(530+640, 730));
            m_view_other_record->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(530, 730)), 0.8f));
        }
    }
      
    m_listRoom->setPosition(CCPointMake(640, 0));
    m_listRoom->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_curLayer = enArenaLayer_NmlRoom;
    
//    char buf[100];
//    snprintf(buf, 99, "naviText_floor_%d", int(CCRANDOM_0_1() * 2) + 1);
   
    MainScene::Inst()->showBottomPromptInfo("naviText_floor_");

}

//******************************************************************************
// enterNmlRoom
//******************************************************************************
void CArenaLayer::enterNmlRoom()
{
    innerExit();
    
    CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
    CNmlDgInfo* pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(m_iFloorId -1);
    snprintf(m_floorName, 199, "%s", pNmlDgInfo->name);
    
    buff_void = pNmlDgInfo->buff_void;

    CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
    m_iRoomId = pRoomInfoArr->count() - m_iRoomSel;
    CDgRoomInfo* pRoomInf = pRoomInfoArr->getObjectAtIndex(m_iRoomId -1);
    snprintf(m_roomName, 199, "%s", pRoomInf->name);
        
    if(CGameData::Inst()->getLocalStamina() < pRoomInf->stamina){
        if(m_staminaDlg == NULL){
            m_staminaDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbStaminaDlg), CGameData::Inst()->getLanguageValue("arenaTip1"), CGameData::Inst()->getLanguageValue("arenaTip2"),enDl2TwoBtn);
            addChild(m_staminaDlg, 100);
            m_staminaDlg->setPosition(CCPointMake(320, 480));
            m_staminaDlg->release();
            m_staminaDlg->setScale(0.0);
            m_staminaDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            MainScene::Inst()->enableBottomPanel(false);
        }
    }
    else{
        m_szDgType = "normal";
        
        if(CGameData::Inst()->needToReqHelper()){
            if(CGameData::Inst()->frdRequest()){
                m_curReqType = enArenaReqType_helper;
                scheduleUpdate();
            }
        }
        else{
            enterHelperList();
        }
        
        CGameData::Inst()->setDiamondAllAvail();
    }
//    char buf[100];
//    snprintf(buf, 99, "naviText_room_%d", int(CCRANDOM_0_1() * 3) + 1);
    

    
    MainScene::Inst()->showBottomPromptInfo("naviText_room_");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// btnCallback
//******************************************************************************
void CArenaLayer::btnCallback(CCObject* sender)
{
    if (MainScene::Inst()->isExtendPackShow())return;
    
    CCMenuItem* pMI = (CCMenuItem*) sender;
    if (pMI->getTag() == kArenaViewReocrdBtnOff || pMI->getTag() == kArenaViewReocrdBtnOn)
    {
        SetViewOtherRecord();
    }
    else if(pMI->getTag() == kArenaBackBtn)
    {
        if(m_curLayer == enArenaLayer_NmlRoom){
            enterNmlList();
            m_miSpecial->setIsVisible(true);
            m_miNormal->setIsVisible(false);
            m_miSango->setIsVisible(false);
        }
        else if(m_curLayer == enArenaLayer_SpecialRoom)
        {
            enterSpecialList();
            m_miSpecial->setIsVisible(false);
            m_miNormal->setIsVisible(false);
            m_miSango->setIsVisible(true);
        }
        else if (m_curLayer == enArenaLayer_SangoRoom)
        {
            enterSangoList();
            m_miSpecial->setIsVisible(false);
            m_miNormal->setIsVisible(true);
            m_miSango->setIsVisible(false);
        }
        else if(m_curLayer == enArenaLayer_HelperList)
        {
            if(m_teamCfm){
                removeChild(m_teamCfm, true);
                m_teamCfm = NULL;
                
                enterHelperList();
            }
            else{
                if(m_preLayer == enArenaLayer_NmlRoom)
                {
                    enterNmlFloor();
                }
                else if (m_preLayer == enArenaLayer_SpecialRoom)
                {
                    enterSpecialFloor();
                }
                else if (m_preLayer == enArenaLayer_SangoRoom)
                {
                    enterSangoFloor();
                }
            }
        }
        else if(m_curLayer == enArenaLayer_Cfm)
        {
            return;
        }
        else if(m_curLayer == enArenaLayer_ShowInf)
        {
            m_curLayer = enArenaLayer_Cfm;
            m_showCard->hide();
        }
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnback));
    }
    // 顺序 normal -> special -> sango
    else
    {
        if(pMI->getTag() == kArenaSpecialBtn)
        {
            m_miSpecial->setIsVisible(false);
            m_miNormal->setIsVisible(false);
            m_miSango->setIsVisible(true);
            MCurLayer();
        }
        else if(pMI->getTag() == kArenaNormalBtn)
        {
            m_miSpecial->setIsVisible(true);
            m_miNormal->setIsVisible(false);
            m_miSango->setIsVisible(false);
            MCurLayer();
        }
        else if (pMI->getTag() == kArenaSango)
        {
            // mark
            m_miSpecial->setIsVisible(false);
            m_miNormal->setIsVisible(true);
            m_miSango->setIsVisible(false);
            MCurLayer();
        }
    }
}

//******************************************************************************
// enterSpecialArena
//******************************************************************************
void CArenaLayer::enterSpecialArena()
{
    m_miSpecial->setIsVisible(false);
    m_miNormal->setIsVisible(false);
    m_miSango->setIsVisible(true);
    MCurLayer();
}

//******************************************************************************
// m_curLayer  -- by cjy
//******************************************************************************
void CArenaLayer::MCurLayer()
{
    if (m_curLayer == enArenaLayer_NmlFloor)
    {
        enterSpecialList();
    }
    else if (m_curLayer == enArenaLayer_SpecialFloor)
    {
        if (CGameData::Inst()->CheckSangoYYExist())
            enterSangoList();
        else
            enterNmlList();
    }
    else if (m_curLayer == enArenaLayer_SangoFloor)
    {
        enterNmlList();
    }
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
}
//******************************************************************************
// reqEnterArena
//******************************************************************************
void CArenaLayer::reqEnterArena(long lRty)
{
    if(CGameData::Inst()->isNewbie()){
        enterArena();
    }
    else{
        const char* helperId[kHelperMaxCount];
        const char* helperTp[kHelperMaxCount];
        CFriendHelpInfo* pHelperInfo = NULL;
        for (int i = 0; i < kHelperMaxCount; i++) {
            if (m_iFriend[i] >= 0)
            {
                pHelperInfo = m_pHelperArr->getObjectAtIndex(m_iFriend[i]);
                helperId[i] = pHelperInfo->fid;
                helperTp[i] = pHelperInfo->type;
            }
            else
            {
                helperId[i] = "";
                helperTp[i] = "";
            }
        }

        char floorid[10];
        sprintf(floorid, "%d", m_iFloorId);
        
        char roomid[10];
        sprintf(roomid, "%d", m_iRoomId);

        if(CGameData::Inst()->enterDgRequest(m_szDgType, floorid, roomid,
                                             pHelperInfo->leader_ucid.c_str(),
                                             helperId, helperTp, lRty)){
            m_curReqType = enArenaReqType_dungeon;
            scheduleUpdate();
        }
    }
}


//******************************************************************************
// cbCashin
//******************************************************************************
void CArenaLayer::showCashinLayer()
{
    int curLvl = CGameData::Inst()->getUsrInfo()->lv;
    int curYuanbapo = CGameData::Inst()->getUsrInfo()->coin;
    CCLog("cur lvl %d yuanbao %d",curLvl,curYuanbapo);
    
    if(CGameData::Inst()->isNewbie()){
        snprintf(m_floorName, 199, "%s", CGameData::Inst()->getLanguageValue("arenaTip3"));
        snprintf(m_roomName, 199, "%s", "");
    }
    
    if (RecordHelper::Inst()->CheckRecording(false))
    {
        m_upload_or_not = Dialog3::CreateUpload(this, callfuncO_selector(CArenaLayer::cbUpload));
        addChild(m_upload_or_not);
        m_upload_or_not->setPosition(ccp(320, 480));
        m_upload_or_not->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
    else if (CGameData::Inst()->isNewbie()) {
        m_cashin->show(m_floorName, m_roomName, 64, 40, preLV, preCost, preStamina, CGameData::Inst()->getDgEndExpEffect());
    }
    else
    {
        long lGold = CGameData::Inst()->getDgEndGold();
        long lExp = CGameData::Inst()->getDgEndExp();
        double fExpEffect = CGameData::Inst()->getDgEndExpEffect();
        m_cashin->show(m_floorName, m_roomName, lGold, lExp, preLV, preCost, preStamina, fExpEffect);
    }
}

//******************************************************************************
// cbCashin
//******************************************************************************
void CArenaLayer::cbCashin()
{

    
    if(MainScene::Inst()->new_CardAdd)
    {
        
        //引导
        
        
        
        SaveData::Inst()->destorySavedData();
        
        //        MainScene::Inst()->showTouchTip();
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(CArenaLayer::newbie1),
                                            CGameData::Inst()->getLanguageValue("WuJiangJia_new23"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 245));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, 98999);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
        
        
    }else if(!CGameData::Inst()->isNewbie()
       
       && CGameData::Inst()->getUsrInfo()->add_lv_5 == 0
       
       && CGameData::Inst()->getUsrInfo()->lv >= 5
       
       )
    {
        //引导
        
        
        
        SaveData::Inst()->destorySavedData();
        
//        MainScene::Inst()->showTouchTip();
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(CArenaLayer::newbie),
                                            CGameData::Inst()->getLanguageValue("lingJiang_new5"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 245));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, 98999);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
        
        MainScene::Inst()->new_MyNew = true;
        
        //    if(CGameData::Inst()->isNewbie()){
        //        innerExit();
        //        MainScene::Inst()->newbieFightEnd();
        //        MainScene::Inst()->enableBottomPanel(false);
        //    }
        //    else{
        //        if (CGameData::Inst()->chkLogin()) {
        //            SaveData::Inst()->destorySavedData();
        //            enter();
    }
    else
    {
        m_addFriend->show();
        m_addFriend->setPosition(ccp(320, 480));
        m_addFriend->setScale(0.0);
        m_addFriend->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(kUiActionDur1, 1.0),0.8f));
        
    }
    
    
    
    isJustLeaveFight = true;
}


//******************************************************************************
// newbie
//******************************************************************************
void CArenaLayer::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    Arrow* pArrow = new Arrow(30, 255);
    
    pArrow->setPosition(CCPointMake(121, 45));
    

    
    MainScene::Inst()->addChild(pArrow, 50000,  98998);
    
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));
    
    MainScene::Inst()->removeTouchTip();
    
    MainScene::Inst()->showBottomPanel(true);
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    
    
    
}


void CArenaLayer::newbie1(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    Arrow* pArrow = new Arrow(30, 255);
    
    pArrow->setPosition(CCPointMake(121 + 100, 45));
    
    
    
    MainScene::Inst()->addChild(pArrow, 50000,  98998);
    
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));
    
    MainScene::Inst()->removeTouchTip();
    
    MainScene::Inst()->showBottomPanel(true);
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    
    
    
}


//******************************************************************************
// cbCashin
//******************************************************************************
void CArenaLayer::cbAddFriend(CCObject* pObj)
{
    m_addFriend->hide();

    AddFriendRet* pRet = (AddFriendRet*)pObj;
    
    if(pRet->fid[0] == 0){
        MainScene::Inst()->showBottomPanel(true);
       
        
        if(CGameData::Inst()->isNewbie()){
            innerExit();
            MainScene::Inst()->newbieFightEnd();
            MainScene::Inst()->enableBottomPanel(false);
        }
        else{
            if (CGameData::Inst()->chkLogin()) {
                SaveData::Inst()->destorySavedData();
                exit();
                enter();
                goDirectRoom();
                 MainScene::Inst()->afterArenaShowTips_gonggao();
            }
            else {
                MainScene::Inst()->reLogin();
            }
        }
    }
    else{
        snprintf(m_helperUid, 31, "%s", pRet->fid);
        if(CGameData::Inst()->addFriendRequest(m_helperUid)){
            m_curReqType = enArenaReqType_addfriend;
            scheduleUpdate();
        }
    }
}


//******************************************************************************
// cbStaminaDlg
//******************************************************************************
void CArenaLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        switch (m_curReqType) {
            case enArenaReqType_helper:
                if(CGameData::Inst()->frdRequest()){
                    m_curReqType = enArenaReqType_helper;
                    scheduleUpdate();
                }
                break;
                
            case enArenaReqType_dungeon:
                reqEnterArena(m_lRtyID);
                break;
                
            case enArenaReqType_getfightcfg:
                if(CGameData::Inst()->cfgRequest())
                {
                    m_curReqType = enArenaReqType_getfightcfg;
                    scheduleUpdate();
                }
                break;
                
            case enArenaReqType_dgend:
            {
                char floorid[10];
                sprintf(floorid, "%d", m_iFloorId);
                char roomid[10];
                sprintf(roomid, "%d", m_iRoomId);
                
                if(CGameData::Inst()->dgEndRequest(m_szDgType, floorid, roomid, m_dungeonUid)){
                    CGameData::Inst()->resetLoadCardStep();
                    scheduleUpdate();
                }
            }   
                break;
                
            case enArenaReqType_allcards:
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enArenaReqType_addfriend:
                if(CGameData::Inst()->addFriendRequest(m_helperUid)){
                    scheduleUpdate();
                }
                
                break;
            case enArenaReqType_cardAdd:
                if(CGameData::Inst()->newChangeRequest("add_card"))
                {
                    
                    
                    
                    m_curReqType = enArenaReqType_cardAdd;
                    scheduleUpdate();
                }
                break;
                
            case enArenaReqType_stamina:
                if(CGameData::Inst()->recoverStaminaRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enArenaReqType_newbie2:
                CGameData::Inst()->newbieRequest(0, 2, NULL);
                scheduleUpdate();  
                break;
                
            case enArenaReqType_newbie3:
                CGameData::Inst()->newbieRequest(0, 3, NULL);
                scheduleUpdate(); 
                break;
            case enArenaReqType_dgendforfanti: //返体
                char floorid[10];
                sprintf(floorid, "%d", m_iFloorId);
                char roomid[10];
                sprintf(roomid, "%d", m_iRoomId);
                
                if(CGameData::Inst()->dgEndForFanTiRequest(m_szDgType, floorid, roomid, m_dungeonUid))
                {
                    m_curReqType = enArenaReqType_dgendforfanti;
                    CCLog("cbFight tag");
                    scheduleUpdate();
                }
                break;
            default:
                break;
        }
       
    }
    else {
        CGameData::Inst()->clearReqStat();
        
        if (!CGameData::Inst()->chkLogin())
        {
            SaveData::Inst()->destorySavedFile();
            MainScene::Inst()->reLogin();
        }
        else
        {
            switch (m_curReqType) {
                case enArenaReqType_helper:
                case enArenaReqType_dungeon:
                case enArenaReqType_dgend:
                case enArenaReqType_stamina:
                    if (m_curReqType == enArenaReqType_dgend) {
                        SaveData::Inst()->destorySavedFile();
                        SaveData::Inst()->destorySavedData();
                    }
                    if((m_curLayer == enArenaLayer_NmlRoom)
                       ||(m_preLayer == enArenaLayer_NmlRoom)){
                        enterNmlFloor();
                    }
                    else if((m_curLayer == enArenaLayer_SpecialRoom)
                            ||(m_preLayer == enArenaLayer_SpecialRoom)){
                        enterSpecialFloor();
                    }
                    else if (m_curLayer == enArenaLayer_SangoRoom)
                    {
                        enterSangoRoom();
                    }
                    MainScene::Inst()->showUsrInfPanel(true);
                    MainScene::Inst()->showBottomPanel(true);
                    break;
                    
                case enArenaReqType_allcards:
                    MainScene::Inst()->showUsrInfPanel(true);
                    MainScene::Inst()->showBottomPanel(false);
                    
                    showCashinLayer();
                    break;
                    
                case enArenaReqType_addfriend:
                    if (CGameData::Inst()->chkLogin()) {
                        exit();
                        enter();
                        goDirectRoom();
                        MainScene::Inst()->showBottomPanel(true);
                    }
                    else {
                        MainScene::Inst()->reLogin();
                    }
                    break;
                    
                case enArenaReqType_newbie2:
                case enArenaReqType_newbie3:
                    enterNmlList();
                    break;
                
                default:
                    break;
            }
        }
                
        if(CGameData::Inst()->isNewbie() == false)
            MainScene::Inst()->enableBottomPanel(true);
    }    
}

//******************************************************************************
// cbHasSameCardDlg
//******************************************************************************
void CArenaLayer::cbHasSameCardDlg(cocos2d::CCObject* pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    bool bOk = pRet->bOk;
    
    removeChild(m_staminaDlg, true);
    m_staminaDlg = NULL;
    MainScene::Inst()->enableBottomPanel(true);
    
    if(bOk == true){
        MainScene::Inst()->enterTeamInfo();
    }
    else {
        if(m_curLayer == enArenaLayer_NmlRoom){
            enterNmlFloor();
        }
        else if(m_curLayer == enArenaLayer_SpecialRoom){
            enterSpecialFloor();
        }
        else if (m_curLayer == enArenaLayer_SangoRoom)
        {
            enterSangoFloor();
        }
    }
}

void  CArenaLayer::cbProposeLvDlg(CCObject* pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    bool bOk = pRet->bOk;
    
    removeChild(m_proposeLvDialog, true);
    m_proposeLvDialog = NULL;
    MainScene::Inst()->enableBottomPanel(true);
    if(bOk == true)
    {
        enterSpecialRoom();
    }
    else
    {
        enterSpecialFloor();
    }
}

//******************************************************************************
// cbStaminaDlg
//******************************************************************************
void CArenaLayer::cbStaminaDlg(CCObject* pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    bool bOk = pRet->bOk;
    
    removeChild(m_staminaDlg, true);
    m_staminaDlg = NULL;
    MainScene::Inst()->enableBottomPanel(true);
    
    if(bOk == true){
        CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
        CommonInfo* pComInf = CGameData::Inst()->getCommonInfo();
        MainScene::Inst()->showTips_gonggao(pUsrInf);//add1.16
        
        if(pUsrInf->coin >= pComInf->recover_stamina_coin){
            if(CGameData::Inst()->recoverStaminaRequest()){
                m_curReqType = enArenaReqType_stamina;
                scheduleUpdate();
            }
        }
        else {
            //购买元宝
            char buf[100];
            snprintf(buf, 99, "%s%lu%s， %s%d%s，%s",CGameData::Inst()->getLanguageValue("arenaTip4"), pUsrInf->coin, CGameData::Inst()->getLanguageValue("arenaTip5"),CGameData::Inst()->getLanguageValue("arenaTip6"),
                     pComInf->recover_stamina_coin,CGameData::Inst()->getLanguageValue("arenaTip5"), CGameData::Inst()->getLanguageValue("arenaTip7"));
            m_goShopDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbGoShop), CGameData::Inst()->getLanguageValue("arenaTip8"),buf,enDl2TwoBtn);
            addChild(m_goShopDlg, 100);
            m_goShopDlg->setPosition(CCPointMake(320, 480));
            m_goShopDlg->release();
            m_goShopDlg->setScale(0.0);
            m_goShopDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            MainScene::Inst()->enableBottomPanel(false);
        }
    }
    else {
        if(m_curLayer == enArenaLayer_NmlRoom){
            enterNmlFloor();
        }
        else if(m_curLayer == enArenaLayer_SpecialRoom){
            enterSpecialFloor();
        }
        else if (m_curLayer == enArenaLayer_SangoRoom)
        {
            enterSangoFloor();
        }
    }
    
    
}


//******************************************************************************
// cbGoShop
//******************************************************************************
void CArenaLayer::cbGoShop(CCObject* pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    bool bOk = pRet->bOk;
    
    removeChild(m_goShopDlg, true);
    m_goShopDlg = NULL;
    MainScene::Inst()->enableBottomPanel(true);
    
    if(bOk == true){
        MainScene::Inst()->goToShop();
    }
    else {
        if(m_curLayer == enArenaLayer_NmlRoom){
            enterNmlFloor();
        }
        else if(m_curLayer == enArenaLayer_SpecialRoom){
            enterSpecialFloor();
        }
        else if (m_curLayer == enArenaLayer_SangoRoom)
        {
            enterSangoFloor();
        }
    }
}


//******************************************************************************
// enterArena
//******************************************************************************
void CArenaLayer::enterArena()
{
    //MainScene::Inst()->stopSnow();
    
    removeFloorNameImage();

    CGameData::Inst()->setIsFighting(true);
    
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
    
    CGameData::Inst()->addBigImageByRGBA4444("fightAni.plist");
    CGameData::Inst()->addBigImageByRGBA4444("fightAni_2.plist");
    CGameData::Inst()->addBigImageByRGBA4444("guangquan.plist");
    
    m_fightMngr->setIsVisible(true);
    
    snprintf(m_dungeonUid, 31, "%s", CGameData::Inst()->getDungeonUid());
    
    CUserInfo *cuUsrInfo = CGameData::Inst()->getUsrInfo();
    preLV = cuUsrInfo->lv;
    preCost = cuUsrInfo->cost;
    preStamina = cuUsrInfo->max_stamina;
    int curFloorStatus = CGameData::Inst()->getCurFlrStatus();
    
    SaveData::Inst()->setDgType(m_szDgType);
    SaveData::Inst()->setFloorId(m_iFloorId);
    SaveData::Inst()->setCurFloorStatus(curFloorStatus);
    SaveData::Inst()->setRoomId(m_iRoomId);
    SaveData::Inst()->setFloorName(m_floorName);
    SaveData::Inst()->setRoomName(m_roomName);
    SaveData::Inst()->setFightSceneType(m_fightSceneType);
    SaveData::Inst()->setDungeonUid(m_dungeonUid);
    
    if (m_szDgType == NULL)
    {
        m_fightMngr->startNewFight(m_fightSceneType, enSpecialDgSkillType_null, buff_void);
    }
    else
    {
        if (strcmp(m_szDgType, "normal") == 0
            || strcmp(m_szDgType, "sgyy") == 0)
        {
            m_fightMngr->startNewFight(m_fightSceneType, enSpecialDgSkillType_null, buff_void);
        }
        else
        {
            CSpecialDgInfo* pSpecialDgInfo = getSpecialDgInfo();
            
            m_fightMngr->startNewFight(m_fightSceneType, pSpecialDgInfo->skillType, buff_void);
        }
    }
    
    if (SaveData::Inst()->getDgSkillType() == 1) {
        MainScene::Inst()->switchBgMusic(enBgMusicQianLiBg);
    }
    else{
        MainScene::Inst()->switchBgMusic(enBgMusicBattle);
    }
    
    CGameData::Inst()->setNeedToReqHelper(true);
}

//******************************************************************************
// enterArenaEnd
//******************************************************************************
void CArenaLayer::enterArenaEnd()
{
    char floorid[10];
    sprintf(floorid, "%d", m_iFloorId);
    char roomid[10];
    sprintf(roomid, "%d", m_iRoomId);
    
    if(CGameData::Inst()->dgEndRequest(m_szDgType, floorid, roomid, m_dungeonUid)){
        m_curReqType = enArenaReqType_dgend;
        scheduleUpdate();
    }

    m_fightMngr->setIsVisible(false);
    m_fightMngr->exit();
    
   // CGameData::Inst()->loadUserCardsSprite();
}

//******************************************************************************
// enterValidArena
//******************************************************************************
void CArenaLayer::enterValidArena()
{    
    CGameData::Inst()->setIsFighting(true);

    removeFloorNameImage();
    
    MainScene::Inst()->showBottomPanel(false);
    MainScene::Inst()->showUsrInfPanel(false);
    
    CGameData::Inst()->addBigImageByRGBA4444("fightAni.plist");
    CGameData::Inst()->addBigImageByRGBA4444("fightAni_2.plist");
    CGameData::Inst()->addBigImageByRGBA4444("guangquan.plist");
    
    preLV = SaveData::Inst()->getCurLv();
    preCost = SaveData::Inst()->getCurCost();
    preStamina = SaveData::Inst()->getCurStatina();
    
    m_iFloorId = SaveData::Inst()->getFloorId();
    m_iRoomId = SaveData::Inst()->getRoomId();
    m_szDgType = SaveData::Inst()->getDgType();
    snprintf(m_floorName, 199, "%s", SaveData::Inst()->getFloorName());
    snprintf(m_roomName, 199, "%s", SaveData::Inst()->getRoomName());
    m_fightSceneType = SaveData::Inst()->getFightSceneType();
    snprintf(m_dungeonUid, 31, "%s", SaveData::Inst()->getDungeonUid());
    
    m_fightMngr->setPosition(CCPointZero);
    
    m_fightMngr->startNewFight(m_fightSceneType, enSpecialDgSkillType_null, buff_void);
    
    if (SaveData::Inst()->getDgSkillType() == 1) {
        MainScene::Inst()->switchBgMusic(enBgMusicQianLiBg);
    }
    else{
        MainScene::Inst()->switchBgMusic(enBgMusicBattle);
    }
}


//******************************************************************************
// enterNmlList
//******************************************************************************
void CArenaLayer::enterNmlList()
{
    innerExit();
    
    do {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_zhanchang"));
        m_listNml->clearAll();
        m_listNml->addTBobserver();
        CMenuBtn* btn;
        
        if(CGameData::Inst()->isNewbie()){
            btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("arenaTip3"), CCTextAlignmentLeft);
            m_listNml->push_back(btn);
            btn->release();
        }
        else
        {
            CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
            CNmlDgInfo* pNmlDgInfo;
            
            if(pNmlDgInfoArr->count() == 0)
                break;
            
            for(int i = pNmlDgInfoArr->count() -1; i >= 0; i--)
            {
                pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(i);
                
                if(pNmlDgInfo == NULL)
                    continue;
                
//                if (CGameData::Inst()->getUsrInfo()->lv < pNmlDgInfo->minlevel || CGameData::Inst()->getUsrInfo()->lv > pNmlDgInfo->maxlevel)
//                    continue;
                
                int cout  = pNmlDgInfoArr->count() - 1 - i;
                if ((cout <= m_listNml->getPreBottomShow())
                    &&(cout >= m_listNml->getPreTopShow())
                    )
                {
                    btn = new CMenuBtn(pNmlDgInfo, i + 1);
                }
                else
                {
                    btn = NULL;
                }
                
                m_listNml->push_back(btn);
                
                if (btn != NULL)
                {
                    btn->release();
                }
            }
            if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_special_lv)
            {
                m_miSpecial->setPosition(CCPointMake(648, 729));
                m_miSpecial->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(458, 729)),0.8f));
            }
            
        }
    } while (0);

    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_special_lv)
    {
        m_miSpecial->setPosition(CCPointMake(648, 729));
        m_miSpecial->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(458, 729)),0.8f));
    }
    
    CListLayerModel* model = m_fPositionMap[enArenaLayer_NmlList];
    m_listNml->setPosition(ccp(640,model->getY()));
    m_listNml->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,model->getY())),0.8f));
    
    m_curLayer = enArenaLayer_NmlFloor;
    m_preLayer = enArenaLayer_NmlFloor;
    
//    char buf[100];
//    snprintf(buf, 99, "naviText_arena_%d", int(CCRANDOM_0_1() * 2) + 1);
    

   
    MainScene::Inst()->showBottomPromptInfo("naviText_arena_");
   //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

void CArenaLayer::showGetConfigAlert()
{
    if (m_getCfgAlert == NULL)
    {
        m_getCfgAlert = new Dialog1(this,
                                callfuncO_selector(CArenaLayer::cbGetConfigAlert),
                                CGameData::Inst()->getLanguageValue("fight_config_update_alert"),
                                false);
        addChild(m_getCfgAlert, 100);
        m_getCfgAlert->release();
    }
    
    m_getCfgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_getCfgAlert->setScale(0.0);
    m_getCfgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// enterSpecialList
//******************************************************************************
void CArenaLayer::enterSpecialList()
{
    CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
    CSpecialDgInfo* pSpecialDgInfo;
    for (int z=0; z<pSpecialDgInfoArr->count(); z++)
    {
        pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(z);
        if(pSpecialDgInfo == NULL)
            continue;
        
        if (pSpecialDgInfo->is_auto && CGameData::Inst()->getCurrentTime() > pSpecialDgInfo->endtime)
        {
            showGetConfigAlert();
            return;
        }
    }
    
    innerExit();
    
    do
    {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_event"));
        m_listSpecial->clearAll();

        if(pSpecialDgInfoArr->count() == 0)
            break;
        
        CMenuBtn* btn;
        long curDate = CGameData::Inst()->getCurrentTime();
        
        for(int i = 0; i < pSpecialDgInfoArr->count(); i++)
        {
            pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
            
            if(pSpecialDgInfo == NULL)
                continue;
            
            if((curDate > pSpecialDgInfo->endtime)
               || (curDate < pSpecialDgInfo->starttime))
            {
                pSpecialDgInfo->remaintime = 0;
                
                if (!pSpecialDgInfo->is_auto)
                continue;
            }
            else
            {
                pSpecialDgInfo->remaintime = pSpecialDgInfo->endtime - curDate;
            }
            
            if (CGameData::Inst()->getUsrInfo()->lv < pSpecialDgInfo->minlevel || CGameData::Inst()->getUsrInfo()->lv > pSpecialDgInfo->maxlevel)
                continue;
            
            btn = new CMenuBtn(pSpecialDgInfo);
            m_listSpecial->push_back(btn);
            btn->release();
        }
        
    }while (0);
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    if (CGameData::Inst()->CheckSangoYYExist())
    {
        if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_sgyy_lv)
        {
            m_miSango->setPosition(CCPointMake(648, 729));
            m_miSango->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(458, 729)),0.8f));
        }
        else
        {
            
        }
        
    }
    else
    {
        m_miNormal->setPosition(CCPointMake(648, 729));
        m_miNormal->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(458, 729)),0.8f));
        m_miNormal->setIsVisible(true);
    }
    
    m_listSpecial->setPosition(CCPointMake(640, 0));
    m_listSpecial->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_curLayer = enArenaLayer_SpecialFloor;
    m_preLayer = enArenaLayer_SpecialFloor;
    
   
    MainScene::Inst()->showBottomPromptInfo("naviText_specialArena");
  //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}


//******************************************************************************
// enterSpecialFloor
//******************************************************************************
void CArenaLayer::enterSpecialFloor()
{
    CSpecialDgInfo* pSpecialDgInfo = getSpecialDgInfo();
    if (pSpecialDgInfo->is_auto && CGameData::Inst()->getCurrentTime() < pSpecialDgInfo->starttime)
        return;
    
    innerExit();
    
    m_iFloorId = pSpecialDgInfo->iFloorId;

    m_spTitle->setTitleString(pSpecialDgInfo->name);
    m_fightSceneType = pSpecialDgInfo->dungeon_type;
    m_ushLimitMaxStar = pSpecialDgInfo->m_unLimitMaxStar;
    CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
    
    CMenuBtn* btn;
    CDgRoomInfo* pRoomInfo;
    for(int i = pRoomInfoArr->count() -1; i >= 0; i--){
        pRoomInfo = pRoomInfoArr->getObjectAtIndex(i);
        
        if(pRoomInfo == NULL)
            continue;
        
        btn = new CMenuBtn(pRoomInfo, pSpecialDgInfo->bg_type);
        m_listRoom->push_back(btn);
        btn->release();
    }
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if (pRoomInfo->video)
    {
        m_view_other_record->setPosition(CCPointMake(530+640, 730));
        m_view_other_record->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(530, 730)), 0.8f));
    }
    
    m_listRoom->setPosition(CCPointMake(640, 0));
    m_listRoom->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_curLayer = enArenaLayer_SpecialRoom;
    
    MainScene::Inst()->showBottomPromptInfo("naviText_specialArena");

}

//******************************************************************************
// enterSangolList
//******************************************************************************
void CArenaLayer::enterSangoList()
{
    innerExit();   
    do {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_sango"));
        m_listSango->clearAll();
        CMenuBtn* btn;
        
        
        CCMutableArray<CSangoYYDgInfo*> * pSangoDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
        CSangoYYDgInfo* pSangoDgInfo;
        
        if(pSangoDgInfoArr->count() == 0)
            break;
        
        
        for(int i = pSangoDgInfoArr->count() -1; i >= 0; i--){
            pSangoDgInfo = pSangoDgInfoArr->getObjectAtIndex(i);
            
            if(pSangoDgInfo == NULL)
                continue;
            
            int cout  = pSangoDgInfoArr->count() - 1 - i;
            
            if ((cout <= m_listSango->getPreBottomShow())
                &&(cout >= m_listSango->getPreTopShow())
                )
            {
                btn = new CMenuBtn(pSangoDgInfo, i + 1);
            }
            else
            {
                btn = NULL;
            }
            
            m_listSango->push_back(btn);
            
            if (btn != NULL)
            {
                btn->release();
            }
        }
    }while (false);
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    m_miNormal->setPosition(CCPointMake(648, 729));
    m_miNormal->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(458, 729)),0.8f));
    
    CListLayerModel* model = m_fPositionMap[enArenaLayer_SangoList];
    m_listSango->setPosition(CCPointMake(640,model->getY()));
    m_listSango->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,model->getY())),0.8f));
    
    m_curLayer = enArenaLayer_SangoFloor;
    m_preLayer = enArenaLayer_SangoFloor;
    
    MainScene::Inst()->showBottomPromptInfo("naviText_sangoArena");
  
}

//******************************************************************************
// enterSangoFloor
//******************************************************************************
void CArenaLayer::enterSangoFloor()
{
    innerExit(); 
    CDgRoomInfo* pRoomInfo;
    do {
        CCMutableArray<CSangoYYDgInfo*> * pSangoDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
        CSangoYYDgInfo* pNmlDgInfo;
        
        m_iFloorId = pSangoDgInfoArr->count() - m_iFloorSel;
        
        pNmlDgInfo = pSangoDgInfoArr->getObjectAtIndex(m_iFloorId -1);
        m_fightSceneType = pNmlDgInfo->dungeon_type;
        m_ushLimitMaxStar = pNmlDgInfo->m_unLimitMaxStar;
        m_spTitle->setTitleSprite(pNmlDgInfo->name);
        
        CCMutableArray<cSangoYYRoom*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
        CMenuBtn* btn;
        for(int i = pRoomInfoArr->count() -1; i >= 0; i--){
            pRoomInfo = pRoomInfoArr->getObjectAtIndex(i);
            
            if(pRoomInfo == NULL)
                continue;
            
            btn = new CMenuBtn(pRoomInfo, m_iFloorId, i + 1);
            
            m_listRoom->push_back(btn);
            btn->release();
        }
    }while (0);
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if (pRoomInfo->video)
    {
        m_view_other_record->setPosition(CCPointMake(530+640, 730));
        m_view_other_record->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(530, 730)), 0.8f));
    }
    
    m_listRoom->setPosition(CCPointMake(640, 0));
    m_listRoom->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    m_curLayer = enArenaLayer_SangoRoom;
//    char buf[100];
//    snprintf(buf, 99, "naviText_floor_%d", int(CCRANDOM_0_1() * 2) + 1);  // ����烘�����ヨ� ��loor涓���崇郴�芥病��
    

    
    MainScene::Inst()->showBottomPromptInfo("naviText_floor_");
 
}
//******************************************************************************
// enterSango Room
//******************************************************************************
void CArenaLayer::enterSangoRoom()
{
    innerExit();
    
    CCMutableArray<CSangoYYDgInfo*> * pSangoDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();
    CSangoYYDgInfo* pSangoDgInfo = pSangoDgInfoArr->getObjectAtIndex(m_iFloorId -1);
    snprintf(m_floorName, 199, "%s", pSangoDgInfo->name);
    
    buff_void = pSangoDgInfo->buff_void;
    
    CCMutableArray<cSangoYYRoom*> * pRoomInfoArr = pSangoDgInfo->m_pRoomArr;
    m_iRoomId = pRoomInfoArr->count() - m_iRoomSel;
    cSangoYYRoom* pRoomInf = pRoomInfoArr->getObjectAtIndex(m_iRoomId -1);
    snprintf(m_roomName, 199, "%s", pRoomInf->name);
    
    if(CGameData::Inst()->getLocalStamina() < pRoomInf->stamina){
        if(m_staminaDlg == NULL){
            m_staminaDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbStaminaDlg), CGameData::Inst()->getLanguageValue("arenaTip1"), CGameData::Inst()->getLanguageValue("arenaTip2"),enDl2TwoBtn);
            addChild(m_staminaDlg, 100);
            m_staminaDlg->setPosition(CCPointMake(320, 480));
            m_staminaDlg->release();
            m_staminaDlg->setScale(0.0);
            m_staminaDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            MainScene::Inst()->enableBottomPanel(false);
        }
    }
    else{
        m_szDgType = "sgyy";
        
        if(CGameData::Inst()->needToReqHelper()){
            if(CGameData::Inst()->frdRequest()){
                m_curReqType = enArenaReqType_helper;
                scheduleUpdate();
            }
        }
        else{
            enterHelperList();
        }
        
        CGameData::Inst()->setDiamondAllAvail();
    }
//    char buf[100];
//    snprintf(buf, 99, "naviText_room_%d", int(CCRANDOM_0_1() * 3) + 1);
    

   
    MainScene::Inst()->showBottomPromptInfo("naviText_room_");
  
}

int CArenaLayer::getSeciaSelByFloorID(int ifloorid)
{
    int index = 0;
    CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
    CSpecialDgInfo* pSpecialDgInfo = NULL;
    for (int i = 0; i<pSpecialDgInfoArr->count(); i++)
    {
        pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
        if (pSpecialDgInfo == NULL)
            continue;
        
        if(pSpecialDgInfo->remaintime == 0 && !pSpecialDgInfo->is_auto)
            continue;
        
        if (CGameData::Inst()->getUsrInfo()->lv < pSpecialDgInfo->minlevel || CGameData::Inst()->getUsrInfo()->lv > pSpecialDgInfo->maxlevel)
            continue;
        
        if (pSpecialDgInfo->iFloorId == ifloorid)
            return pSpecialDgInfo->remaintime == 0 ? -1 : index;
        
        
        index++;
    }

    return -1;
}

//******************************************************************************
// getSpecialDgInfo
//******************************************************************************
CSpecialDgInfo* CArenaLayer::getSpecialDgInfo()
{
    CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
    CSpecialDgInfo* pSpecialDgInfo = NULL;//pSpecialDgInfoArr->getObjectAtIndex(m_iFloorSel);
    int index = 0;
    for(int i = 0; i < pSpecialDgInfoArr->count(); i++){
        pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
        
        if(pSpecialDgInfo == NULL)
            continue;
        
        if(pSpecialDgInfo->remaintime == 0 && !pSpecialDgInfo->is_auto)
            continue;
        
        if (CGameData::Inst()->getUsrInfo()->lv < pSpecialDgInfo->minlevel || CGameData::Inst()->getUsrInfo()->lv > pSpecialDgInfo->maxlevel)
            continue;
        
        if (index == m_iFloorSel)
            break;
        
        index++;        
    }
    
    return pSpecialDgInfo;
}



//******************************************************************************
// enterSpecialRoom
//******************************************************************************
void CArenaLayer::enterSpecialRoom()
{
    innerExit();
    
    CSpecialDgInfo* pSpecialDgInfo = getSpecialDgInfo();
    CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
    
    m_iFloorId = pSpecialDgInfo->iFloorId;
    sprintf(m_floorName, pSpecialDgInfo->name,199);
    m_iRoomId = pRoomInfoArr->count() - m_iRoomSel;
    
    CDgRoomInfo* pRoomInf = pRoomInfoArr->getObjectAtIndex(m_iRoomId -1);
    sprintf(m_roomName,  pRoomInf->name,199);
    
    buff_void = pSpecialDgInfo->buff_void;
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    m_spTitle->setTitleString(pRoomInf->name);
      
    if(CGameData::Inst()->getLocalStamina() < pRoomInf->stamina||pUsrInf->max_stamina<pRoomInf->stamina){
        char buf[50];
        if(pUsrInf->max_stamina < pRoomInf->stamina)
        {
            sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("arenaTip10"));
            if(m_staminaDlg == NULL){
                m_staminaDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbStaminaDlg), buf, CGameData::Inst()->getLanguageValue("arenaTip11"),enDl2OneBtn);
                addChild(m_staminaDlg, 100);
                m_staminaDlg->setPosition(CCPointMake(320, 480));
                m_staminaDlg->release();
                m_staminaDlg->setScale(0.0);
                m_staminaDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                MainScene::Inst()->enableBottomPanel(false);
            }
        }
        else 
        {
            sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("arenaTip1"));
            if(m_staminaDlg == NULL){
                m_staminaDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbStaminaDlg),
                                           buf, CGameData::Inst()->getLanguageValue("arenaTip2"),
                                           enDl2TwoBtn);
                addChild(m_staminaDlg, 100);
                m_staminaDlg->setPosition(CCPointMake(320, 480));
                m_staminaDlg->release();
                m_staminaDlg->setScale(0.0);
                m_staminaDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                MainScene::Inst()->enableBottomPanel(false);
            }
        }
    }
    else{
        
        if (pSpecialDgInfo->skillType == enSpecialDgSkillType_quanduiwuxianding
            && CGameData::Inst()->getIsSameCardInTeam())
        {
            if(m_staminaDlg == NULL){
                m_staminaDlg = new Dialog2(this, callfuncO_selector(CArenaLayer::cbHasSameCardDlg),
                                           CGameData::Inst()->getLanguageValue("quanxianding_1"),
                                           CGameData::Inst()->getLanguageValue("quanxianding_2"),
                                           enDl2TwoBtn);
                addChild(m_staminaDlg, 100);
                m_staminaDlg->setPosition(CCPointMake(320, 480));
                m_staminaDlg->release();
                m_staminaDlg->setScale(0.0);
                m_staminaDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                MainScene::Inst()->enableBottomPanel(false);
            }
        }
        else
        {
            if(pSpecialDgInfo->cType == 1){
                m_szDgType = "special";
            }
            else if(pSpecialDgInfo->cType == 2){
                m_szDgType = "weekly";
            }
            
            if(CGameData::Inst()->needToReqHelper()){
                if(CGameData::Inst()->frdRequest()){
                    m_curReqType = enArenaReqType_helper;
                    scheduleUpdate();
                }
            }
            else{
                enterHelperList();
            }
            
            CGameData::Inst()->setCurAvailDiamond(pSpecialDgInfo->availDiamondCnt, pSpecialDgInfo->availDiamond);
        }
    }
//    char buf[100];
//    snprintf(buf, 99, "naviText_room_%d", int(CCRANDOM_0_1() * 3) + 1);
    

    
    MainScene::Inst()->showBottomPromptInfo("naviText_room_");
 
}


bool CArenaLayer::getIsJustLeaveFight()
{
    return isJustLeaveFight;
}

void CArenaLayer::setIsJustLeaveFight(bool agu)
{
    isJustLeaveFight = agu;
}

void CArenaLayer::reset()
{
    CListLayerModel* model = NULL;
    model = m_fPositionMap[enArenaLayer_NmlList];
    model->setTopShow(-2);
    model->setBottonShow(7);
    model->setY(0);
    
    model = m_fPositionMap[enArenaLayer_SpecialList];
    model->setTopShow(-2);
    model->setBottonShow(7);
    model->setY(0);
    
    model = m_fPositionMap[enArenaLayer_SangoList];
    model->setTopShow(-2);
    model->setBottonShow(7);
    model->setY(0);
    
    model = m_fPositionMap[enArenaLayer_HelperList];
    model->setTopShow(-2);
    model->setBottonShow(7);
    model->setY(0);
    
    model = m_fPositionMap[enArenaLayer_RoomList];
    model->setTopShow(-2);
    model->setBottonShow(7);
    model->setY(0);
}

void CArenaLayer::cbUpload(CCObject* obj)
{
    Dialog3Ret* ret = (Dialog3Ret*)obj;
    int index = ret->bSel;
    
    if (index == 1)
    {
        // 上传并分享
        RecordHelper::Inst()->UploadLastReplay();
    }
    else if (index == 2)
    {
        // 保存至本地
        RecordHelper::Inst()->SaveToAlbum();
    }
    else if (index == 3)
    {
        // 暂不分享  神马都不做
    }
    
    long lGold = CGameData::Inst()->getDgEndGold();    
    long lExp = CGameData::Inst()->getDgEndExp();
    double fExpEffect = CGameData::Inst()->getDgEndExpEffect();
    
    m_cashin->show(m_floorName, m_roomName, lGold, lExp, preLV, preCost, preStamina, fExpEffect);
}

void CArenaLayer::SetViewOtherRecord()
{
    ResetViewOtherRecordBtn();
    if (1 || m_curLayer == enArenaLayer_NmlRoom)
    {
        CCArray* children = m_listRoom->getChildren();
        int count = children->count();
        for (int i=0; i<count; i++)
        {
            CCObject* obj = children->objectAtIndex(i);
            if (dynamic_cast<CMenuBtn*>(obj))
            {
                CMenuBtn* btn = (CMenuBtn*)obj;
                if (m_view_other_record->getTag() == kArenaViewReocrdBtnOn)
                {
                    btn->RemoveBtnViewOtherRecord();
                }
                else
                {
                    btn->AddBtnViewOtherRecord(this, menu_selector(CArenaLayer::cbViewOtherRecord), count - i);
                }
            }
        }
    }
    else if (m_curLayer == enArenaLayer_SpecialRoom)
    {
        CCLog("special");
    }
    else if (m_curLayer == enArenaLayer_SangoRoom)
    {
        
    }
}

void CArenaLayer::ResetViewOtherRecordBtn(bool on_off)
{
    if (on_off == true)
    {
        m_view_other_record->setNormalImage(CCSprite::spriteWithFile("view_other_record_off.png"));
        m_view_other_record->setSelectedImage(CCSprite::spriteWithFile("view_other_record_off.png"));
        m_view_other_record->setTag(kArenaViewReocrdBtnOff);
    }
    else
    {
        m_view_other_record->setNormalImage(CCSprite::spriteWithFile("view_other_record_on.png"));
        m_view_other_record->setSelectedImage(CCSprite::spriteWithFile("view_other_record_on.png"));
        m_view_other_record->setTag(kArenaViewReocrdBtnOn);
    }
}

void CArenaLayer::ResetViewOtherRecordBtn()
{
    int tag = m_view_other_record->getTag();
    if (tag == kArenaViewReocrdBtnOn)
    {
        ResetViewOtherRecordBtn(true);
    }
    else
    {
        ResetViewOtherRecordBtn(false);
    }
}

void CArenaLayer::cbViewOtherRecord(CCObject* obj)
{
    CCMenuItemSprite* ret = (CCMenuItemSprite*)obj;
    int tag = ret->getTag();
    CCLog("cb = %d   %d  %d", tag, m_iFloorId, m_iFloorSel);
    char dungeon_id[8];
    snprintf(dungeon_id, 8, "%d_%d", m_iFloorId, tag);
    string type;
    if (m_curLayer == enArenaLayer_NmlRoom)
    {
        type = "normal";
    }
    else if (m_curLayer == enArenaLayer_SpecialRoom)
    {
        type = "special";
    }
    else if (m_curLayer == enArenaLayer_SangoRoom)
    {
        type = "sgyy";
    }
    if (RecordHelper::Inst()->ReqOtherVideoRecord(dungeon_id, type.c_str()))
    {
        m_curReqType = enArenaReqType_req_view_other_record_vid;
        scheduleUpdate();
    }
}

void CArenaLayer::RequestUrl()
{
    string url = RecordHelper::Inst()->GetOtherRecordUrlVid();
    
    if (url.length()<5)
    {
        if (m_dialog1 == NULL)
        {
            m_dialog1 = new Dialog1(this,
                                     callfuncO_selector(CArenaLayer::CallBackRemoveD2),
                                     CGameData::Inst()->getLanguageValue("record_no_other_record"),
                                     false);
            addChild(m_dialog1, 100);
            m_dialog1->release();
        }
        
        m_dialog1->setPosition(CCPointMake(320, 480 - getPosition().y));
        m_dialog1->setScale(0.0);
        m_dialog1->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
    else
    {
        RecordHelper::Inst()->ViewRecord(url.c_str());
    }
}

void CArenaLayer::CallBackRemoveD2()
{
    if (m_dialog1)
    {
        m_dialog1->removeFromParentAndCleanup(true);
        m_dialog1 = NULL;
    }
}

void CArenaLayer::cbGetConfigAlert()
{
    if (m_getCfgAlert)
    {
        m_getCfgAlert->removeFromParentAndCleanup(true);
        m_getCfgAlert = NULL;
    }
    
    if(CGameData::Inst()->cfgRequest())
    {
        m_curReqType = enArenaReqType_getfightcfg;
        scheduleUpdate();
    }
    
}

void CArenaLayer::cbGetFantiAlert()//返体
{
    if (m_getFantiAlert) {
        m_getFantiAlert->removeFromParentAndCleanup(true);
        m_getFantiAlert = NULL;
    }
    SaveData::Inst()->destorySavedFile();
    
    addFloorNameImage();
    
    if (CGameData::Inst()->chkLogin()) {
        SaveData::Inst()->destorySavedData();
        exit();
        enter();
        goDirectRoom();
        
        MainScene::Inst()->showBottomPanel(true);
        MainScene::Inst()->showUsrInfPanel(true);
        MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    else {
        MainScene::Inst()->reLogin();
    }

}




void CArenaLayer::forceExitFight()
{
    m_fightMngr->setIsVisible(false);
    m_fightMngr->forceExitFight();
    m_fightMngr->exit();

    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fightAni_2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("guangquan.plist");
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CArenaLayer::resetIFriend()
{
    for (int i = 0; i < kHelperMaxCount; i++)
    {
        m_iFriend[i] = -1;
    }
}




