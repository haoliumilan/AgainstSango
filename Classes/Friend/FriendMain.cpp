//
//  FriendMain.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "FriendMain.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CScrollBar.h"
#include "CSeachFriend.h"
#include "CShowCardInf.h"
#include "MainScene.h"
#include "CommDlg.h"
#include "TitleBar.h"
#include "SimpleAudioEngine.h"
#include "NoFriendLayer.h"
#include "FriendInvitationLayer.h"
#include "Dialog2.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "CConfirmLayer.h"
#include "MailListLayer.h"
#include "MailBoxAgt.h"
#include "TitleItem.h"
#include "TitleList.h"
#include "ui_maa_request_master.h"
#include "ui_maa_receive_apprentice_request.h"
#include "ui_maa_master_manage.h"
#include "ui_maa_receive_apprentice.h"
#include "ui_maa_request_master_request.h"
#include "ui_maa_apprentice_manage.h"
using namespace CocosDenshion;

#define enBtnBack           1
#define enBtnTitleList      2
#define enDlgBindWeibo      3
#define enDlgShareSuccess   4

#define UI_TAB_MAA_REQUEST_MASTER                   100      //拜 师
#define UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST       200      //收徒请求
#define UI_TAB_MAA_MASTER_MANAGE                    300      //师傅管理
#define UI_TAB_MAA_RECEIVE_APPRENTICE               400      //收 徒
#define UI_TAB_MAA_REQUEST_MASTER_REQUEST           500      //拜师请求
#define UI_TAB_MAA_APPRENTICE_MANAGE                600      //徒弟管理

//unsigned int g_unUIMAATABTemp = UI_MAA_REQUEST_MASTER |
//                                UI_MAA_RECEIVE_APPRENTICE_REQUEST |
//                                UI_MAA_MASTER_MANAGE |
//                                UI_MAA_RECEIVE_APPRENTICE |
//                                UI_MAA_REQUEST_MASTER_REQUEST |
//                                UI_MAA_APPRENTICE_MANAGE;
unsigned int g_unUIMAATABTemp = 0;

//******************************************************************************
// Construct
//******************************************************************************
FriendMain::FriendMain()
{

}

//******************************************************************************
// enter
//******************************************************************************
void FriendMain::enter()
{
    m_frRange = enFriendRangePro;
    m_searchFriendBoard = NULL;
    m_friendInfoLayer = NULL;
    m_friendInviationLayer = NULL;
    m_friendArray = NULL;
    m_iReqType = enFrdReqType_Null;
    m_commDlg = NULL;
    m_nofriendLayer = NULL;
    m_inviteList = NULL;
    m_friendList = NULL;
    m_rangeMenu = NULL;
    m_cfmLayer = NULL;
    m_frNumBg = NULL;
    m_rangeLabel = NULL;
    m_lcCover = NULL;
    m_pMailList = NULL;
    m_titleItem = NULL;
    m_pTtlList = NULL;
    m_SortBtnLayer = NULL;
        
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 200);
    m_showCard->release();
    
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    m_list = new CListLayer(this, callfuncO_selector(FriendMain::cbList));
    addChild(m_list, -1);
    m_list->release();
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_backItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                       spMenu2,
                                                       this,
                                                       menu_selector(FriendMain::btnCallback));
    m_backItem->setTag(enBtnBack);
    m_backItem->setAnchorPoint(CCPointZero);
    
    spMenu1 = CCSprite::spriteWithFile("title_btnbg.png");
    spMenu2 = CCSprite::spriteWithFile("title_btnbg.png");
    m_titleItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                              spMenu2,
                                                              this,
                                                              menu_selector(FriendMain::cbTitleBtn));
    m_titleItem->setAnchorPoint(CCPointZero);
    m_titleItem->setPosition(CCPointMake(520, 730));
    
    TextNode* tnTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("title_title"),
                                                       spMenu1->getContentSize(),
                                                       CCTextAlignmentCenter,
                                                       35);
    tnTitle->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 10,
                                     spMenu1->getContentSize().height * 0.5 - 10));
    tnTitle->setShadowColor(ccBLACK);
    m_titleItem->addChild(tnTitle);
    
    CCMenu* menu = CCMenu::menuWithItems(m_backItem, m_titleItem, NULL);
    addChild(menu,30);
    menu->setPosition(CCPointZero);

    showFriendMain();
    
    if (getIsTouchEnabled() == false) {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -127, true);
    }
}


void FriendMain::btnCallback(CCObject *sender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    switch (m_curScreen) {
        case enFS_searchFr:
        {
            showFriendMain();
        }
            break; 
        case enFS_recentFr:
        {
            showFriendMain();
        }
            break;
        case enFS_friendIf:
        {
            showFriendMain();
        }
            break;
                        
        case enFS_inviteFr:
        {   
            Dialog2 *dialog = (Dialog2 *)getChildByTag(enDlgShareSuccess);
            if (!dialog) {
                showFriendMain();
            }
        }
            break;
        case enFS_cmfLayer:
        {
            break;
        }
        case enFS_maa:
        {
            showFriendMain();
            break;
        }
        case enFS_titlelist:
        {
            showFriendMain();
            break;
        }
        default:
            break;
    }
}

void FriendMain::cbList(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    switch (m_curScreen) {
        //好友主界面进入二级界面
        case enFS_main:
        {
//            switch (pRet->iBtnSel)
//            {
//                case 0:
//                {
//                    enterFriendList();
//                    break;
//                }
////                case 1:
////                {
////                    if (m_teaStuType == enTeaStuType_CanStudent)
////                        enterGetTeacher();
////                    else
////                        enterTeaStuManger();
////                    break;
////                }
////                    
////                    
////                case 2:
////                {
////                    enterTeaStuRequst();
////                    break;
////                }
//                case 1:
//                {
//                    enterSearchID();
//                    break;
//                }
//                case 2:
//                {
//                    enterFriendInfo();
//                    break;
//                }
//                case 3:
//                {
//                    if (CGameData::Inst()->getIsUseInviteCode())
//                        enterFriendInvitation();
//                    else
//                        enterMailList();
//                    break;
//                }
//                case 4:
//                {
////                    if (CUIAppreticeRequestMaster *pAppreticeRequestMaster = (CUIAppreticeRequestMaster*)getChildByTag(UI_TAB_MAA_APPRENTICE_REQUEST_MASTER))
////                    {
////                        innerExit();
////                        pAppreticeRequestMaster->show();
////                        m_curScreen = enFS_maa_arm;
////                    }
//                    break;
//                }
//                case 5:
//                {
////                    if (CUIMasterReceiveApprentice *pMasterReceiveApprentice = (CUIMasterReceiveApprentice*)getChildByTag(UI_TAB_MAA_MASTER_RECEIVE_APPRENTICE))
////                    {
////                        innerExit();
////                        pMasterReceiveApprentice->show();
////                    }
//                    break;
//                }
//                case 6:
//                {
//                    enterMailList();
//                    break;
//                }
//                default:
//                {
//                    break;
//                }
//            }
            
            if (0==pRet->iBtnSel)
            {
                enterFriendList();
            }
            else if (1==pRet->iBtnSel)
            {
                enterSearchID();
            }
            else if (2==pRet->iBtnSel)
            {
                enterFriendInfo();
            }
            else if (3==pRet->iBtnSel)
            {
                if (CGameData::Inst()->getIsUseInviteCode())
                    enterFriendInvitation();
                else
                    enterMailList();
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_REQUEST_MASTER))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_REQUEST_MASTER))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_MASTER_MANAGE))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_MASTER_MANAGE))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_RECEIVE_APPRENTICE))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_REQUEST_MASTER_REQUEST))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_REQUEST_MASTER_REQUEST))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else if (pRet->iBtnSel==m_list->getMenuBtnIndex(UI_TAB_MAA_APPRENTICE_MANAGE))
            {
                if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_APPRENTICE_MANAGE))
                {
                    innerExit();
                    m_curScreen = enFS_maa;
                    pMAABasic->setBackMenuItemFunc(menu_selector(FriendMain::btnCallback));
                    pMAABasic->show();
                }
            }
            else
            {
                enterMailList();
            }
        }
            break;
        case enFS_recentFr:
        {
            m_listTag = pRet->iBtnSel;
            CFriendInf* tempFriendInfo = m_friendArray->getObjectAtIndex(m_listTag);
            m_curScreen = enFS_cmfLayer;
            m_cfmLayer->show(&tempFriendInfo->card_info, tempFriendInfo->name, enCConfirmLayer_FRIEND);
            m_lcCover = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 196), 640, 960);
            addChild(m_lcCover, 101);
        }
            break;
        case enFS_friendIf:
        {
            CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getInvitersInf();
            CFriendInf* tempFriendInfo = array->getObjectAtIndex(pRet->iBtnSel);
            m_showCard->show(&tempFriendInfo->card_info, false);
            MainScene::Inst()->showBottomPanel(false);
            switch (pRet->iBtnSel) {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
  
}

void FriendMain::enterFriendInvitation() {
    innerExit();

    m_curScreen = enFS_inviteFr;

    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_invite"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_friendInvitate");
   // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    if (m_friendInviationLayer == NULL) {
        m_friendInviationLayer = new FriendInvitationLayer(this, callfunc_selector(FriendMain::shareSanGuo));
        addChild(m_friendInviationLayer);
        m_friendInviationLayer->release();
    }
    CUserInfo *userInfo = CGameData::Inst()->getUsrInfo();
    m_friendInviationLayer->showInvitationCode(userInfo->invite_code);
    m_friendInviationLayer->setPosition(CCPointMake(-640, 0));
    m_friendInviationLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)), 0.8f));
}

void FriendMain::enterFriendInfo()
{
    // 向服务器获取好友请求信息，然后进入好友请求列表
    innerExit();
    m_curScreen = enFS_friendIf;

    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_shenqing"));

    if(CGameData::Inst()->reqlistRequest())
    {
        m_iReqType = enFrdReqType_InviteList;
        scheduleUpdate();
    }
}

void FriendMain::cbNofriend()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    showFriendMain();
}

void FriendMain::enterMailList()
{
    innerExit();
    
    m_curScreen = enFM_mailList;
    
    if(m_pMailList == NULL){
        m_pMailList = new MailListLayer(this, callfunc_selector(FriendMain::cbMailList));
        addChild(m_pMailList);
        m_pMailList->release();
    }
}

void FriendMain::cbMailList()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    showFriendMain();
}

void FriendMain::enterFriendList()
{
    innerExit();
    m_curScreen = enFS_recentFr;
 
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_myfrd"));

    if(CGameData::Inst()->friendListRequest()){
        m_iReqType = enFrdReqType_FriendList;
        scheduleUpdate();
    }
    else{
        showFriendList();
    }
}

void FriendMain::enterGetTeacher()
{
    CCLog("FriendMain::enterGetTeacher()");
}

void FriendMain::enterTeaStuManger()
{
    CCLog("FriendMain::enterTeaStuManger()");
}

void FriendMain::enterTeaStuRequst()
{
    CCLog("FriendMain::enterTeaStuRequst");
}

void FriendMain::enterSearchID()
{
    innerExit();

    m_curScreen = enFS_searchFr;

    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_searchfrd"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_searchFriendBoard == NULL)
    {
        m_searchFriendBoard = new CSeachFriend(this, callfunc_selector(FriendMain::cbSearchFriend));
        addChild(m_searchFriendBoard);
        m_searchFriendBoard->release();
    }
    
    m_searchFriendBoard->enter();
}

void FriendMain::showFriendMain()
{
    innerExit();
    
    // 好友界面的目录
    CMenuBtn* btn;
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_myfrd"),
                       CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();

//    if (m_teaStuType == enTeaStuType_CanStudent)
//        btn = new CMenuBtn("ma_button.png",
//                           CGameData::Inst()->getLanguageValue("teastu_getTeacher"),
//                           CCTextAlignmentCenter);
//    else
//        btn = new CMenuBtn("ma_button.png",
//                           CGameData::Inst()->getLanguageValue("teastu_manager"),
//                           CCTextAlignmentCenter);
//    m_list->push_back(btn);
//    btn->release();
//    
//    btn = new CMenuBtn("ma_button.png",
//                       CGameData::Inst()->getLanguageValue("teastu_request"),
//                       CCTextAlignmentCenter);
//    m_list->push_back(btn);
//    btn->release();

    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_searchfrd"),
                       CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_shenqing"),
                       CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
    m_frInviteBtn = btn;
    
    if (CGameData::Inst()->getIsUseInviteCode()) {
        btn = new CMenuBtn("ma_button.png",
                           CGameData::Inst()->getLanguageValue("title_invite"),
                           CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->release();
        
        CCActionInterval *action = (CCActionInterval*)CCSequence::actions(CCFadeIn::actionWithDuration(1.0),
                                                                          CCFadeOut::actionWithDuration(1.0),
                                                                          NULL);
        
        CCSprite *spDiamond = CCSprite::spriteWithSpriteFrameName("id_gold.png");
        btn->addChild(spDiamond);
        spDiamond->setScale(0.5);
        spDiamond->setFlipX(180);
        spDiamond->setPosition(CCPointMake(290 - spDiamond->getContentSize().width - 50, 0));
        spDiamond->runAction(CCRepeatForever::actionWithAction(action));
        
        action = (CCActionInterval*)CCSequence::actions(CCFadeIn::actionWithDuration(1.0),
                                                        CCFadeOut::actionWithDuration(1.0),
                                                        NULL);
        spDiamond = CCSprite::spriteWithSpriteFrameName("id_gold.png");
        btn->addChild(spDiamond);
        spDiamond->setScale(0.5);
        spDiamond->setPosition(CCPointMake(spDiamond->getContentSize().width - 290 + 50, 0));
        spDiamond->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    //师徒系统
    if (UI_MAA_REQUEST_MASTER==(g_unUIMAATABTemp&UI_MAA_REQUEST_MASTER))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_REQUEST_MASTER))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_REQUEST_MASTER))
        {
            addChild(CUIMAARequestMaster::create(this),0,UI_TAB_MAA_REQUEST_MASTER);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_request_master"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_REQUEST_MASTER);
        btn->release();
    }
    
    if (UI_MAA_RECEIVE_APPRENTICE_REQUEST==(g_unUIMAATABTemp&UI_MAA_RECEIVE_APPRENTICE_REQUEST))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST))
        {
            addChild(CUIMAAReceiveApprenticeRequest::create(this),0,UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_receive_apprentice_request"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_RECEIVE_APPRENTICE_REQUEST);
        btn->release();
    }
    
    if (UI_MAA_MASTER_MANAGE==(g_unUIMAATABTemp&UI_MAA_MASTER_MANAGE))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_MASTER_MANAGE))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_MASTER_MANAGE))
        {
            addChild(CUIMAAMasterManage::create(this),0,UI_TAB_MAA_MASTER_MANAGE);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_master_manage"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_MASTER_MANAGE);
        btn->release();
    }
    
    if (UI_MAA_RECEIVE_APPRENTICE==(g_unUIMAATABTemp&UI_MAA_RECEIVE_APPRENTICE))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_RECEIVE_APPRENTICE))
        {
            addChild(CUIMAAReceiveApprentice::create(this),0,UI_TAB_MAA_RECEIVE_APPRENTICE);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_receive_apprentice"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_RECEIVE_APPRENTICE);
        btn->release();
    }
    
    if (UI_MAA_REQUEST_MASTER_REQUEST==(g_unUIMAATABTemp&UI_MAA_REQUEST_MASTER_REQUEST))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_REQUEST_MASTER_REQUEST))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_REQUEST_MASTER_REQUEST))
        {
            addChild(CUIMAARequestMasterRequest::create(this),0,UI_TAB_MAA_REQUEST_MASTER_REQUEST);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_request_master_request"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_REQUEST_MASTER_REQUEST);
        btn->release();
    }
    
    if (UI_MAA_APPRENTICE_MANAGE==(g_unUIMAATABTemp&UI_MAA_APPRENTICE_MANAGE))
    {
        if (CUIMAABasic *pMAABasic = (CUIMAABasic*)getChildByTag(UI_TAB_MAA_APPRENTICE_MANAGE))
        {
            pMAABasic->hide();
        }
        else if (!getChildByTag(UI_TAB_MAA_APPRENTICE_MANAGE))
        {
            addChild(CUIMAAApprenticeManage::create(this),0,UI_TAB_MAA_APPRENTICE_MANAGE);
        }
        
        btn = new CMenuBtn("ma_button.png",CGameData::Inst()->getLanguageValue("maa_apprentice_manage"),CCTextAlignmentCenter);
        m_list->push_back(btn);
        btn->setUITab2(UI_TAB_MAA_APPRENTICE_MANAGE);
        btn->release();
    }
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_mail"),
                       CCTextAlignmentCenter);
    m_list->push_back(btn);
    btn->release();
    m_frMailBtn = btn;
    
    innerEnter();
}

void FriendMain::showFriendInviteList()
{
    // 好友邀请列表
    innerExit();

    m_inviteList = new CListLayer(this,callfuncO_selector(FriendMain::cbList));
    addChild(m_inviteList,-2);
    m_inviteList->release();

    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
   
    MainScene::Inst()->showBottomPromptInfo("naviText_friendInfo");
   //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getInvitersInf();
    MainScene::Inst()->showTips();
    
    // yang--
    if(!array||array->count() == 0)
    {
        m_nofriendLayer =  new NoFriendLayer(this,callfuncO_selector(FriendMain::cbNofriend));
        addChild(m_nofriendLayer);
        m_nofriendLayer->release();
        m_nofriendLayer->setInfoLabel(CGameData::Inst()->getLanguageValue("FriendTip5"));
        m_nofriendLayer->setPosition(CCPoint(1000, 400));
        m_nofriendLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400)), 0.8f));
        
    }
    else{
        for(int i = 0;i< array->count();i++)
        {
            CFriendInf *fInfo = array->getObjectAtIndex(i);
            CMenuBtn *btn = new CMenuBtn(fInfo, enFriendRangePro, true);
            m_inviteList->push_back(btn);
            btn->release();
            
            CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button_yes.png");
            CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button_yes.png");
            CCMenuItem *itemAccpet = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2, this, menu_selector(FriendMain::acceptInvite));
            itemAccpet->setTag(i*10+1);
            
            spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button_no.png");
            spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button_no.png");
            CCMenuItem *itemRefuse = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FriendMain::acceptInvite));
            itemRefuse->setPosition(ccp(80, 0));
            itemRefuse->setTag(i*10+2);
            
            itemAccpet->setAnchorPoint(ccp(0, 0));
            itemRefuse->setAnchorPoint(ccp(0, 0));
            CCMenu *menu = CCMenu::menuWithItems(itemAccpet,itemRefuse,NULL);
            btn->addChild(menu);
            menu->setPosition(ccp(120, -40));
        }

        m_inviteList->setPosition(CCPointMake(640, 0));
        m_inviteList->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    }

}

void FriendMain::showFriendList()
{
    innerExit();
    
    char buf[100];
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getFriendList();
    

    //yang--: add(array&&)
    if (array&&array->count() > 0) {
        // 好友列表
        if (m_friendList == NULL)
        {
            m_friendList = new CListLayer(this, callfuncO_selector(FriendMain::cbList));
            addChild(m_friendList,-2);
            m_friendList->release();
            m_friendList->setReloadFunc(callfuncO_selector(FriendMain::InserItemByIdx));
        }
        
        m_cfmLayer = new CConfirmLayer(this, callfuncO_selector(FriendMain::cbCfm));
        addChild(m_cfmLayer, 102);
        m_cfmLayer->release();
        
        CCSprite *sp1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
        CCSprite *sp2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
        
        CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(sp1, sp2, this, menu_selector(FriendMain::cbRangeBtn));
        m_rangeMenu = CCMenu::menuWithItem(item);
        item->setAnchorPoint(ccp(0, 0));
        addChild(m_rangeMenu);
        
        m_rangeLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_property"),CCSizeMake(150,80),CCTextAlignmentCenter, 25);
        m_spTitle->addChild(m_rangeLabel);
        m_rangeLabel->setPosition(ccp(335, 15));
        
        
        TextNode* lbRangeTitle = TextNode::textWithString("", 26);
        lbRangeTitle->setPosition(ccp(53, 10));
        lbRangeTitle->setShadowColor(ccc3(0, 0, 0));
        item->addChild(lbRangeTitle);
        lbRangeTitle->setString(CGameData::Inst()->getLanguageValue("range_title"));
        
        m_frNumBg = CCSprite::spriteWithSpriteFrameName("fr_another.png");
        addChild(m_frNumBg, 1);
        m_frNumBg->setPosition(CCPointMake(535, 167));
        
        sprintf(buf, "%d/%d",array->count(),CGameData::Inst()->getUsrInfo()->maxFr_num);
        TextNode* frInfoLabel = TextNode::textWithString(buf, 24);
        frInfoLabel->setPosition(ccp(120, 18));
        m_frNumBg->addChild(frInfoLabel);
        
        m_rangeMenu->setPosition(CCPoint(-1000, 731));
        m_rangeMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(580-55, 766-35)),0.8f));
        
    }
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    
  //  snprintf(buf, 99, "naviText_friendList_%d", int(CCRANDOM_0_1() * 2) + 1);
    
 
   
    MainScene::Inst()->showBottomPromptInfo("naviText_friendList_");
   //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    //yang--: add(!array||)+(/* */)
    /*
    if(array == NULL)
        return;
    printf("array %d",array->count());
    */
    if(!array||array->count() == 0)
    {
        m_nofriendLayer =  new NoFriendLayer(this,callfuncO_selector(FriendMain::cbNofriend));
        addChild(m_nofriendLayer);
        m_nofriendLayer->release();
        m_nofriendLayer->setInfoLabel(CGameData::Inst()->getLanguageValue("FriendTip6"));
        m_nofriendLayer->setPosition(CCPoint(1000, 400));
        m_nofriendLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400)), 0.8f));
    }else{
        m_frRange = CGameData::Inst()->getDefaultFrRangeType();
        updateFriendRange();
        
        m_friendList->updatePosition();
        m_friendList->setPosition(ccp(640,m_friendList->getPosition().y));
        m_friendList->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,m_friendList->getPosition().y)),0.8f));
    }
}

void FriendMain::InserItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    CFriendInf *fInfo = m_friendArray->getObjectAtIndex(pRet->iBtnSel);
//    CMenuBtn *btn = new CMenuBtn(fInfo,enFriendRangePro);
      CMenuBtn *btn = new CMenuBtn(fInfo,(enFriendRange)m_frRange);
    m_friendList->reloadItemByIdx(btn, pRet->iBtnSel);
    btn->release();
}


void FriendMain::updateFriendRange()
{
    if (m_curScreen == enFS_cmfLayer) {
        return;
    }
    
    CCMutableArray<CFriendInf*> *initialArray = CCMutableArray<CFriendInf *>::arrayWithArray(CGameData::Inst()->getFriendList());
    m_friendList->clearAll();
    if(m_friendArray)
        m_friendArray->removeAllObjects();
    else {
        m_friendArray =  new CCMutableArray<CFriendInf *>();
    }
    
    if(m_curScreen == enFS_recentFr)
    {
        //按照属性排序
        if(m_frRange == enFriendRangePro)
        {
            for(int j=0;j<=5;j++)
            {
                for(int i=0;i<initialArray->count();i++)
                {
                    CUserCardInfo *info =
                    &initialArray->getObjectAtIndex(i)->card_info;
                    CFriendInf *tempFrInfp = initialArray->getObjectAtIndex(i);
                    if(info->ctype == j)
                    {
                        m_friendArray->addObject(tempFrInfp);
                    }
                }
            }
        }
        //按照血量排序
        if(m_frRange == enFriendRangeHp)
        {
            
            for(int i=0;i<initialArray->count()-1;i++)
            {
                for (int j=0;j< initialArray->count()-1-i; j++) {
                    int hp1 = initialArray->getObjectAtIndex(j)->card_info.hp ;
                    int hp2 = initialArray->getObjectAtIndex(j+1)->card_info.hp;
                    if(hp1 < hp2)
                    {
                        CFriendInf *tempFr1 = initialArray->getObjectAtIndex(j);
                        CFriendInf *tempFr2 = initialArray->getObjectAtIndex(j+1);
                        initialArray->replaceObjectAtIndex(j, tempFr2);
                        initialArray->replaceObjectAtIndex(j+1, tempFr1);
                    }
                }
            }
            
            for(int i=0;i<initialArray->count();i++)
            {
                m_friendArray->addObject(initialArray->getObjectAtIndex(i));
            }
        }
        //按照攻击力排序
        if(m_frRange == enFriendRangeAtk)
        {
            
            for(int i=0;i<initialArray->count()-1;i++)
            {
                for (int j=0;j< initialArray->count()-1-i; j++) {
                    int attack1 = initialArray->getObjectAtIndex(j)->card_info.attack ;
                    int attack2 = initialArray->getObjectAtIndex(j+1)->card_info.attack;
                    if(attack1 < attack2)
                    {
                        CFriendInf *tempFr1 = initialArray->getObjectAtIndex(j);
                        CFriendInf *tempFr2 = initialArray->getObjectAtIndex(j+1);
                        initialArray->replaceObjectAtIndex(j, tempFr2);
                        initialArray->replaceObjectAtIndex(j+1, tempFr1);
                    }
                }
            }
            
            for(int i=0;i<initialArray->count();i++)
            {
                m_friendArray->addObject(initialArray->getObjectAtIndex(i));
            }
        }
        
        //按照回复力排序
        if(m_frRange == enFriendRangeRec)
        {
            
            for(int i=0;i<initialArray->count()-1;i++)
            {
                for (int j=0;j< initialArray->count()-1-i; j++) {
                    int rec1 = initialArray->getObjectAtIndex(j)->card_info.recover ;
                    int rec2 = initialArray->getObjectAtIndex(j+1)->card_info.recover;
                    if(rec1 < rec2)
                    {
                        CFriendInf *tempFr1 = initialArray->getObjectAtIndex(j);
                        CFriendInf *tempFr2 = initialArray->getObjectAtIndex(j+1);
                        initialArray->replaceObjectAtIndex(j, tempFr2);
                        initialArray->replaceObjectAtIndex(j+1, tempFr1);
                    }
                }
            }
            
            for(int i=0;i<initialArray->count();i++)
            {
                m_friendArray->addObject(initialArray->getObjectAtIndex(i));
            }
        }
        
        //按照登陆时间排序
        if(m_frRange == enFriendRangeLT)
        {
            
            for(int i=0;i<initialArray->count()-1;i++)
            {
                for (int j=0;j< initialArray->count()-1-i; j++) {
                    int lt1 = initialArray->getObjectAtIndex(j)->login_time ;
                    int lt2 = initialArray->getObjectAtIndex(j+1)->login_time;
                    if(lt1 < lt2)
                    {
                        CFriendInf *tempFr1 = initialArray->getObjectAtIndex(j);
                        CFriendInf *tempFr2 = initialArray->getObjectAtIndex(j+1);
                        initialArray->replaceObjectAtIndex(j, tempFr2);
                        initialArray->replaceObjectAtIndex(j+1, tempFr1);
                    }
                }
            }
            
            for(int i=0;i<initialArray->count();i++)
            {
                m_friendArray->addObject(initialArray->getObjectAtIndex(i));
            }
        }
        //按照武将国家属性排序
        if (m_frRange == enFriendRangeCou) {
            
            int nCountryPorperty1 = 0,nCountryPorperty2 = 0;
            const int nConstCount = initialArray->count();
            for (int i = 0; i < nConstCount-1; ++i)
            {
                for (int j = 0; j < nConstCount-1-i; ++j)
                {
                    nCountryPorperty1 = initialArray->getObjectAtIndex(j)->card_info.cou_name;
                    nCountryPorperty2 = initialArray->getObjectAtIndex(j+1)->card_info.cou_name;
                    
                    if (nCountryPorperty1 > nCountryPorperty2)
                    {
                        CFriendInf *tempFr1 = initialArray->getObjectAtIndex(j);
                        CFriendInf *tempFr2 = initialArray->getObjectAtIndex(j+1);
                        initialArray->replaceObjectAtIndex(j, tempFr2);
                        initialArray->replaceObjectAtIndex(j+1, tempFr1);
                        
                    }
                }
            }
            for(int i=0;i<initialArray->count();i++)
            {
                m_friendArray->addObject(initialArray->getObjectAtIndex(i));
            }
        }
        
        //重排好友
        for(int i=0;i<m_friendArray->count();i++)
        {
//            CFriendInf *fInfo = m_friendArray->getObjectAtIndex(i);
            CMenuBtn *btn ;//= new CMenuBtn(fInfo,enFriendRangePro);
            
            if (i <= m_friendList->getPreBottomShow()
                &&i >= m_friendList->getPreTopShow())
                
            {
                CFriendInf *fInfo = m_friendArray->getObjectAtIndex(i);
//                btn = new CMenuBtn(fInfo,enFriendRangePro);
                 btn = new CMenuBtn(fInfo,(enFriendRange)m_frRange);
                
            }
            else
            {
                btn = NULL;
            }
            
            m_friendList->push_back(btn);
            
            if (btn != NULL)
            {
                btn->release();
            }
        }
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
    m_rangeLabel->setString(buf);
    CGameData::Inst()->setDefaultFrRangeType((enFriendRange)m_frRange);
}

void FriendMain::cbRangeBtn(CCObject* sender)
{
    
    const char* szStringTbl[] = {
        "sortbtn_prop",
        "sortbtn_hp",
        "sortbtn_atk",
        "sortbtn_rcv",
        "sortbtn_lt",
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
        
        for (int i = 0; i < 6; i++) {
            CCSprite* spMenu1 = CCSprite::spriteWithFile("sortbtn.png");
            CCSprite* spMenu2 = CCSprite::spriteWithFile("sortbtn.png");
            CCMenuItem* pMI = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(FriendMain::onSortBtnTouch) );
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
                                                          CCCallFunc::actionWithTarget(this, callfunc_selector(FriendMain::onSortBtnLayerExit)),
                                                             NULL));
          
        }
    }
}

//******************************************************************************
// onSortBtnLayerExit
//******************************************************************************
void FriendMain::onSortBtnLayerExit()
{
    if (m_SortBtnLayer) {
        removeChild(m_SortBtnLayer, true);
        m_SortBtnLayer = NULL;
    }
}

//******************************************************************************
// onSortBtnTouch
//******************************************************************************
void FriendMain::onSortBtnTouch(CCObject* sender)
{
    CCMenuItem* mi = (CCMenuItem*) sender;
    m_frRange = mi->getTag() + 1;

    if(m_frRange > enFriendRangeCou)
        m_frRange = enFriendRangePro;
    
    updateFriendRange();
    
    CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
    CCSize szBtn(0,0);
    if (spTemp)
        szBtn = spTemp->getContentSize();
    
    CCPoint ptBase(632 - szBtn.width, 720 - szBtn.height);
    m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(640-ptBase.x, 0)), 0.3),
                                                  CCCallFunc::actionWithTarget(this, callfunc_selector(FriendMain::onSortBtnLayerExit)),
                                                  NULL));
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool FriendMain::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_SortBtnLayer) {
        CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
        CCSize szBtn(0,0);
        if (spTemp)
            szBtn = spTemp->getContentSize();
        
        m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(szBtn.width + 8, 0)), 0.3),
                                                      CCCallFunc::actionWithTarget(this, callfunc_selector(FriendMain::onSortBtnLayerExit)),
                                                      NULL));
    }
    
    return false;
}

void FriendMain::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(FriendMain::cbCommDlg), enCommDlgTp_connecting);
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
        switch (m_iReqType) {
            case enFrdReqType_Search:
                showFriendInfoLayer(friInfo_addFriend,CGameData::Inst()->getSchUsrInf());
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_InviteList:
                showFriendInviteList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_FriendList:
                CGameData::Inst()->setFrndLstReqTime(CGameData::Inst()->getCurrentTime());
                showFriendList();
                CGameData::Inst()->setNeedToReqHelper(true);
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_Delete:
                //删除好友
                CGameData::Inst()->setFrndLstReqTime(0);
                enterFriendList();
                CGameData::Inst()->setNeedToReqHelper(true);
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_Accept:
                //接受好友
                CGameData::Inst()->setFrndLstReqTime(0);
                enterFriendInfo();
                CGameData::Inst()->setNeedToReqHelper(true);
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_Refuse:
                //拒绝好友
                enterFriendInfo();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_AddFr:
                //添加好友
                CGameData::Inst()->setNeedToReqHelper(true);
                showFriendMain();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_SendMail:
                showFriendMain();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_TitleList:
                enterTtlList();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enFrdReqType_setTitleName:
                enterTtlList();
                chgTitleName();
                 MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            default:
                break;
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(FriendMain::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void FriendMain::acceptInvite(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    innerExit();
    
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getInvitersInf();
    CCMenuItemImage *temItem = (CCMenuItemImage *)pSender;
    int tag = temItem->getTag();
    CFriendInf *friendInfo = array->getObjectAtIndex(tag/10);
    m_acceptFriendTag = tag;

    if(tag%10==1)
    {
        showFriendInfoLayer(friInfo_accept, friendInfo);
    }
    else if(tag %10 == 2)
    {
        showFriendInfoLayer(friInfo_refuse, friendInfo);
    }

}

void FriendMain::cbSearchFriend()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    if(m_searchFriendBoard->requestSearchFriend())
    {
        m_iReqType = enFrdReqType_Search;
        scheduleUpdate();
    }
}

void FriendMain::reqDeleteFriend()
{
    CCMutableArray<CFriendInf*> *array = m_friendArray;
    CFriendInf *friendInfo = array->getObjectAtIndex(m_listTag);
    char *id = friendInfo->fid;
    m_iReqType = enFrdReqType_Delete;
    if(CGameData::Inst()->delFriendRequest(id))
    {
        scheduleUpdate();
    }
}

void FriendMain::reqAcceptInvite()
{
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getInvitersInf();
    int tag = m_acceptFriendTag;
    CFriendInf *friendInfo = array->getObjectAtIndex(tag/10);
    if(CGameData::Inst()->acceptRequest(friendInfo->fid))
    {
        m_iReqType = enFrdReqType_Accept;
        scheduleUpdate();
    }

}

void FriendMain::reqRefuseFriend()
{
    CCMutableArray<CFriendInf*> *array = CGameData::Inst()->getInvitersInf();
    int tag = m_acceptFriendTag;
    CFriendInf *friendInfo = array->getObjectAtIndex(tag/10);
    if(CGameData::Inst()->refuseRequest(friendInfo->fid))
    {
        m_iReqType = enFrdReqType_Refuse;
        scheduleUpdate();
    }
}

void FriendMain::reqAddFriend()
{
    if(m_friendInfoLayer->addFriend())
    {
        m_iReqType = enFrdReqType_AddFr;
        scheduleUpdate();
    }
}

void FriendMain::cbInviteFriend() {
    // 发新浪微博, 邀请码    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this, callfuncO_selector(FriendMain::dlCallBack), CGameData::Inst()->getLanguageValue("weibo_authened"), CGameData::Inst()->getLanguageValue("weibo_authened_info"), enDl2TwoBtn);   
        addChild(dialog, 100);
        dialog->setTag(enDlgBindWeibo);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else {
        OcProxy::Inst()->sendWeibo(OcProxy::Inst()->getInviteCodeWeibo(), false);
        finishShareInviteCode();
    }
}

void FriendMain::finishShareInviteCode() {
    char buf[200];
    CUserInfo *userInfo = CGameData::Inst()->getUsrInfo();
    if (CGameData::Inst()->getLoginType() == enLoginReqType_SinaLogin) {
        snprintf(buf, 199, "%s%s%s", CGameData::Inst()->getLanguageValue("weibo_shareInviteCode_sina_1"), userInfo->username, CGameData::Inst()->getLanguageValue("weibo_shareInviteCode_sina_2"));
    }
    else if (CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin) {
        snprintf(buf, 199, "%s%s%s", CGameData::Inst()->getLanguageValue("weibo_shareInviteCode_qq_1"), userInfo->username, CGameData::Inst()->getLanguageValue("weibo_shareInviteCode_qq_2"));
    }
    
    m_friendInviationLayer->enableShareBtn(false);
    Dialog2 *finishDlg = new Dialog2(this, callfuncO_selector(FriendMain::cbFinishShareInviteCode), CGameData::Inst()->getLanguageValue("weibo_shareInviteCode_success"), buf, enDl2OneBtn);
    addChild(finishDlg, 100);
    finishDlg->setPosition(CCPointMake(320, 480));
    finishDlg->setTag(enDlgShareSuccess);
    finishDlg->release();
    finishDlg->setScale(0.0);
    finishDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    MainScene::Inst()->enableBottomPanel(false);
}

void FriendMain::cbFinishShareInviteCode() {
    m_friendInviationLayer->enableShareBtn(true);
    Dialog2 *dialog = (Dialog2 *)getChildByTag(enDlgShareSuccess);
    dialog->removeFromParentAndCleanup(true);
    dialog = NULL;
    CGameData::Inst()->clearReqStat();
    MainScene::Inst()->enableBottomPanel(true);
    
    showFriendMain();
}

void FriendMain::dlCallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(enDlgBindWeibo);
    dialog->removeFromParentAndCleanup(true);
    if(ret->bOk == true)
    {
        MainScene::Inst()-> gotoOtherMain();
    }
}   

void FriendMain::cbFriendInfoLayer(CCObject *pSender)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    CFriendInfoLayerRet *ret = (CFriendInfoLayerRet *)pSender;
    switch (ret->iBtnSel) {
        case enFILayerBtnOk:
        {
            if(m_friendInfoLayer->getLayerType() == friInfo_addFriend)
            {
                reqAddFriend();
            }
            else if (m_friendInfoLayer->getLayerType() == friInfo_delete)
            {
                reqDeleteFriend();
            }
            else if (m_friendInfoLayer->getLayerType() == friInfo_accept)
            {
                reqAcceptInvite();
            }
            else if (m_friendInfoLayer->getLayerType() == friInfo_refuse)
            {
                reqRefuseFriend();
            }
        }
            break;
        case enFILayerBtnCancel:
        {
            if(m_friendInfoLayer->getLayerType() == friInfo_addFriend)
            {
                enterSearchID();
            }
            else if(m_friendInfoLayer->getLayerType() == friInfo_delete)
            {
                showFriendList();
            }
            else if(m_friendInfoLayer->getLayerType() == friInfo_accept)
            {
                showFriendInviteList();
            }
            else if(m_friendInfoLayer->getLayerType() == friInfo_refuse)
            {
                showFriendInviteList();
            }
        }
            break;

        default:
            break;
    }
}

//******************************************************************************
// showFriendInfoLayer
//******************************************************************************
void FriendMain::showFriendInfoLayer(FriendInfoLayerType type, CFriendInf *friendInfo)
{
    innerExit();
    
    if (m_friendInfoLayer == NULL)
    {
        m_friendInfoLayer = new CFriendInfoLayer(this, callfuncO_selector(FriendMain::cbFriendInfoLayer));
        addChild(m_friendInfoLayer);
        m_friendInfoLayer->release();
    }
    
    m_friendInfoLayer->enterWithType(type, friendInfo);
}

//******************************************************************************
// exit
//******************************************************************************
void FriendMain::exit()
{
    innerExit();
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    setIsTouchEnabled(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

//******************************************************************************
// innerEnter
//******************************************************************************
void FriendMain::innerEnter()
{
    m_curScreen = enFS_main;

    CCActionInterval* tempAction;
        
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_social"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));

    m_backItem->stopAllActions();
    m_backItem->setPosition(CCPointMake(-1000, -1000));

    m_titleItem->setIsVisible(true);
    m_titleItem->setPosition(CCPointMake(800, 730));
    tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(425, 730));
    m_titleItem->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    
    m_list->setPosition(CCPointMake(640, 0));
    tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero);
    m_list->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_social_1");
  //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
    showTips();
}

//******************************************************************************
// innerExit
//******************************************************************************
void FriendMain::innerExit()
{
    m_spTitle->stopAllActions();
    m_spTitle->setPosition(CCPointMake(-1000, -1000));

    m_backItem->stopAllActions();
    m_backItem->setPosition(CCPointMake(-1000, -1000));

    m_list->stopAllActions();
    m_list->setPosition(CCPointMake(-1000, -1000));
    m_list->clearAll();
    
    m_titleItem->stopAllActions();
    m_titleItem->setIsVisible(false);
    
    if (m_pTtlList) {
        m_pTtlList->clearAll();
        m_pTtlList->setIsVisible(false);
    }
    
    if (m_friendInviationLayer) {
        removeChild(m_friendInviationLayer, true);
        m_friendInviationLayer = NULL;
    }
    
    if (m_nofriendLayer) {
        removeChild(m_nofriendLayer, true);
        m_nofriendLayer = NULL;
    }
    
    if (m_friendList) {
        m_friendList->clearAll();
    }
    
    if (m_cfmLayer) {
        removeChild(m_cfmLayer, true);
        m_cfmLayer = NULL;
    }
    
    if (m_rangeMenu) {
        removeChild(m_rangeMenu, true);
        m_rangeMenu = NULL;
    }
    
    if (m_frNumBg) {
        removeChild(m_frNumBg, true);
        m_frNumBg = NULL;
    }
    
    if (m_rangeLabel) {
        m_spTitle->removeChild(m_rangeLabel, true);
        m_rangeLabel = NULL;
    }
    
    if (m_inviteList) {
        removeChild(m_inviteList, true);
        m_inviteList = NULL;
    }
    
    if(m_searchFriendBoard!=NULL) {
        m_searchFriendBoard->removeFromParentAndCleanup(true);
        m_searchFriendBoard = NULL;
    }
    
    if(m_friendInfoLayer!=NULL) {
        m_friendInfoLayer->removeFromParentAndCleanup(true);
        m_friendInfoLayer = NULL;
    }
    
    if (m_lcCover != NULL) {
        m_lcCover->removeFromParentAndCleanup(true);
        m_lcCover = NULL;
    }
    
    if (m_pMailList) {
        m_pMailList->removeFromParentAndCleanup(true);
        m_pMailList = NULL;
    }
    
    if (m_SortBtnLayer) {
        m_SortBtnLayer->stopAllActions();
        removeChild(m_SortBtnLayer, true);
        m_SortBtnLayer = NULL;
    }

}

//******************************************************************************
// updateMail
//******************************************************************************
void FriendMain::updateMail(ccTime dt)
{
    if (OcProxy::Inst()->getIsHasSentMail())
    {
        unschedule(schedule_selector(FriendMain::updateMail));
        if (strlen(OcProxy::Inst()->getMailText()) != 0) {
            m_mailText = OcProxy::Inst()->getMailText();
            
            CFriendInf* tempFriendInfo = m_friendArray->getObjectAtIndex(m_listTag);
            if(CGameData::Inst()->sendMailRequest(tempFriendInfo->fid, m_mailText.c_str())){
                m_iReqType = enFrdReqType_SendMail;
                scheduleUpdate();
                innerExit();
            }
        }
        OcProxy::Inst()->deleteMailViewController();
        OcProxy::Inst()->showUsedMemory();
    }
}

//******************************************************************************
// cbCfm
//******************************************************************************
void FriendMain::cbCfm(CCObject* pObj)
{
    CCfmLayerRet* pRet = (CCfmLayerRet*) pObj;
    
    switch (pRet->iBtnSel) {
        case 0:
        {
            CFriendInf* tempFriendInfo = m_friendArray->getObjectAtIndex(m_listTag);
            OcProxy::Inst()->showMailViewController(tempFriendInfo->name);
            schedule(schedule_selector(FriendMain::updateMail));
        }

            break;
        
        case 1:
        {
            CCMutableArray<CFriendInf*> *array = m_friendArray;
            CFriendInf *friendInfo = array->getObjectAtIndex(m_listTag);
            showFriendInfoLayer(friInfo_delete, friendInfo);
            m_curScreen = enFS_recentFr;
        }
            
            break;
    
        case 2:
        {
            CFriendInf* tempFriendInfo = m_friendArray->getObjectAtIndex(m_listTag);
            m_showCard->show(&tempFriendInfo->card_info, false);
            MainScene::Inst()->showBottomPanel(false);
        }

            break;
            
        case 3:
        default:
        {
            m_curScreen = enFS_recentFr;
            
            m_cfmLayer->hide();
            m_lcCover->removeFromParentAndCleanup(true);
            m_lcCover = NULL;
        }
            
            break;
            
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void FriendMain::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        switch (m_iReqType) {
            case enFrdReqType_FriendList:
            {
                enterFriendList();
            }
                break;
            case enFrdReqType_InviteList:
            {
                enterFriendInfo();
            }
                break;
            case enFrdReqType_Accept:
            {
                MainScene::Inst()->enableBottomPanel(true);
                reqAcceptInvite();
            }
                break;
            case enFrdReqType_Refuse:
            {
                MainScene::Inst()->enableBottomPanel(true);
                reqRefuseFriend();
            }
                break;
            case enFrdReqType_Delete:
            {
                MainScene::Inst()->enableBottomPanel(true);
                reqDeleteFriend();
            }
                break;
            case enFrdReqType_Search:
            {
                if(m_searchFriendBoard->requestSearchFriend())
                {
                    scheduleUpdate();
                }
            }
                break;
            case enFrdReqType_AddFr:
            {
                reqAddFriend();
            }
                break;
                
            case enFrdReqType_SendMail:
            {
                CFriendInf* tempFriendInfo = m_friendArray->getObjectAtIndex(m_listTag);
                if(CGameData::Inst()->sendMailRequest(tempFriendInfo->fid, m_mailText.c_str())){
                    scheduleUpdate();
                }
            }
                break;
                
            case enFrdReqType_TitleList:
                if(CGameData::Inst()->reqTitleList())
                {
                    scheduleUpdate();
                }
                break;
                
            case enFrdReqType_setTitleName:
                if(CGameData::Inst()->reqSetTitleName(m_strCurTtlId))
                {
                    scheduleUpdate();
                }
                break;
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        
        switch (m_iReqType) {
            case enFrdReqType_FriendList:
            {
                showFriendMain();
            }
                break;
            case enFrdReqType_InviteList:
            {
                showFriendMain();
            }
                break;
            case enFrdReqType_Accept:
            {
                enterFriendInfo();
            }
                break;
            case enFrdReqType_Refuse:
            {
                enterFriendInfo();
            }
                break;
            case enFrdReqType_Delete:
            {
                enterFriendList();
            }
                break;
            case enFrdReqType_Search:
            {
                enterSearchID();
            }
                break;
            case enFrdReqType_AddFr:
            {
                showFriendMain();
            }
                break;
            
            case enFrdReqType_SendMail:
            {
                showFriendMain();
            }
                break;
                
            case enFrdReqType_TitleList:
                showFriendMain();
                break;
                
            default:
                break;
        }
        
        MainScene::Inst()->showBottomPanel(true);
    }    
}


void FriendMain::showTips()
{
    int number = CGameData::Inst()->getUsrInfo()->friend_request_num;
        
    if (number > 0)
    {
        CCSprite* numberTips = OcProxy::Inst()->showTipsAndNum(number);
        numberTips->setPosition(ccp(191, 5));
        m_frInviteBtn->addChild(numberTips);
    }
    
    int newMailNum = MailBoxAgt::Inst()->getNewMailCnt();
    if (CGameData::Inst()->getUsrInfo()->last_mail_time > MailBoxAgt::Inst()->getLastRcvTime())
        newMailNum++;
    
    if (newMailNum > 0)
    {
        CCSprite* newMailNumTips = OcProxy::Inst()->showTipsAndNum(newMailNum);
        newMailNumTips->setPosition(ccp(191, 5));
        m_frMailBtn->addChild(newMailNumTips);
    }
        
}

//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void FriendMain::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(FriendMain::cbInviteFriend),1);
    
}

//******************************************************************************
// cbTitleBtn
//******************************************************************************
void FriendMain::cbTitleBtn(CCObject* sender)
{
    if(CGameData::Inst()->reqTitleList())
    {
        innerExit();
        m_iReqType = enFrdReqType_TitleList;
        scheduleUpdate();
    }
}

//******************************************************************************
// enterTtlList
//******************************************************************************
void FriendMain::enterTtlList()
{
    m_curScreen = enFS_titlelist;
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_titlelst"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));

    m_backItem->setPosition(CCPointMake(-529, 731));
    m_backItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));

    if (m_pTtlList == NULL) {
        m_pTtlList = new TitleList(CCRectMake(0, 150, 640, 570));
        addChild(m_pTtlList);
    }
    
    m_pTtlList->clearAll();
    m_pTtlList->setIsVisible(true);
    
    CCMutableArray<TitleInf* > * pTtlInfArr = CGameData::Inst()->getTitleInfArr();
    for (int i = 0; i < pTtlInfArr->count(); i++) {
        TitleInf* pTtlInf = pTtlInfArr->getObjectAtIndex(i);
        TitleItem* pTtlItm = new TitleItem(pTtlInf);

        m_pTtlList->push_back(pTtlItm);
        pTtlItm->release();
        pTtlItm->setUseCb(this, callfuncO_selector(FriendMain::cbEquipTtl));
    }
    
    m_pTtlList->setPosition(CCPointMake(640, 0));
    m_pTtlList->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
}

//******************************************************************************
// cbEquipTtl
//******************************************************************************
void FriendMain::cbEquipTtl(CCObject* pObj)
{
    m_strCurTtlId = ((TitleItem*)pObj)->getTitleId();

    if(CGameData::Inst()->reqSetTitleName(m_strCurTtlId))
    {
        innerExit();
        m_iReqType = enFrdReqType_setTitleName;
        scheduleUpdate();
    }
}

//******************************************************************************
// chgTitleName
//******************************************************************************
void FriendMain::chgTitleName()
{
    TitleItem* pTtlItm = m_pTtlList->getEquipedItem();
    if (pTtlItm) {
        pTtlItm->setEquipSt(enTtlNotEquiped);
    }
    
    m_pTtlList->equipById(m_strCurTtlId);
    
}

//******************************************************************************
// chgTitleName
//******************************************************************************
void FriendMain::enterMailListFromFirst()
{
    enterMailList();
}


