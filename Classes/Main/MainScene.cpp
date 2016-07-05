#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "CGameState.h"
#include "CGamedata.h"
#include "CUsrInfoPanel.h"
#include "CBottomPanel.h"
#include "CGeneralLayer.h"
#include "CArenaLayer.h"
#include "GachaMain.h"
#include "OtherMain.h"
#include "ShopMain.h"
#include "CLoginLayer.h"
#include "NewbieLayer.h"
#include "Dialog1.h"
#include "CommDlg.h"
#include "MsgBox.h"
#include "Arrow.h"
#include "CTipsDlg.h"
#include "DeadDlg.h"
#include "TitleBar.h"
#include "CashInLayer.h"
#include "Dialog2.h"
#include "CTimerLayer.h"
#include "PublishWeiboLayer.h"
#include "TextNode.h"
#include "SaveData.h"
#include "OcProxy.h"
#include "CNotice.h"
#include "OcProxy.h"
#include "MailBoxAgt.h"
#include "OrgSaveData.h"
#include "FriendMain.h"
#include "CountryMain.h"
#include "RecordHelper.h"
#include "FirstLayer.h"
#include "RewardMain.h"

#include "ParcelPageOne.h"
#include "ParcelBackPage.h"

#include "TDGAUtility.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define kMainSceneTitle     0xEE
#define kWelcomeDlgTag      0x911
#define kGoGachaDlgTag      0x119
#define kGoGachaMsgTag      0x120
#define kGoGachaArrowTag    0x456
#define kSnowEmmiterTag     0x1111
#define kDlgNewTitleTag     0x1112

#define UI_TAB_DIGLOG       -99908

MainScene* MainScene::s_pcIf = NULL;

//******************************************************************************
// Constructor
//******************************************************************************
MainScene::MainScene()
{
    m_nxtLayer = NULL;
    m_commDlg = NULL;
    m_fullPackDlg = NULL;
    m_fullCostDlg = NULL;
    m_foodInTeamDlg = NULL;
    m_foodUpThreeStarsDlg = NULL;
    m_lbBottomPanel = NULL;
    m_lbBottomPanel_gonggao = NULL;//add1.15
    m_notice = NULL;
    m_pLoginLayer = NULL;
    m_spTouchTip = NULL;
    isGfNoticeShow = false;
    isIVNoticeShow = false;
    isLINoticeShow = false;
    isClaimNoticeShow = true;
    m_reloginWeiboType = 0;
    m_reloginDlg = NULL;
    m_tips_string = "";
    m_Itip_index = 0;
   
   
    m_pParcelBackPage = NULL;
    m_pParcelPageOne = NULL;
    m_pParcelPageTwo = NULL;

    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(winsz.width > 1000){
        this->setPosition(CCPointMake(128, 64));
        this->setAnchorPoint(CCPointZero);
        this->setScale(2.0);
        
        CCLayerColor* lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255),768,32);
        lcBk->setPosition(CCPointMake(-64, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 64, 1024);
        lcBk->setPosition(CCPointMake(-64, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 64, 1024);
        lcBk->setPosition(CCPointMake(640, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 768,32);
        lcBk->setPosition(CCPointMake(-64, 960));
        addChild(lcBk, 200);
    }
    
    else if(winsz.width == 768){
        this->setPosition(CCPointMake(64, 32));
        
        CCLayerColor* lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255),768,32);
        lcBk->setPosition(CCPointMake(-64, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 64, 1024);
        lcBk->setPosition(CCPointMake(-64, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 64, 1024);
        lcBk->setPosition(CCPointMake(640, -32));
        addChild(lcBk, 200);
        
        lcBk = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 768,32);
        lcBk->setPosition(CCPointMake(-64, 960));
        addChild(lcBk, 200);
    }
    else if(winsz.width == 320){
        this->setAnchorPoint(CCPointZero);
        this->setScale(0.5);
    }
    
    //user info
    m_pUsrInfoPanel = new CUsrInfoPanel();
    m_pUsrInfoPanel->setPosition(CCPointMake(0, 170));
    addChild(m_pUsrInfoPanel, 100);
    showUsrInfPanel(false);
    
    //bottom panel
    m_pBottomPanel = new CBottomPanel(this, callfuncO_selector(MainScene::cbBottomPanel));
    m_pBottomPanel->setPosition(CCPointMake(0, -157));
    addChild(m_pBottomPanel, 100);
    showBottomPanel(false);
    
    
    schedule(schedule_selector(MainScene::updateBottomPanel), 1.00f);
    schedule(schedule_selector(MainScene::waitPPLogout));
    
    
    
    
    //timer layer
    m_pTimerLayer = new CTimerLayer();
    addChild(m_pTimerLayer,2000);
    m_pTimerLayer->release();
    
    //first layer
    m_pFirstLayer = new FirstLayer();
    addChild(m_pFirstLayer);
    m_pFirstLayer->release();
    
    //arena layer
    m_pArenaLayer = new CArenaLayer();
    addChild(m_pArenaLayer);
    m_pArenaLayer->release();
    
    //general layer
    m_pGeneralLayer = new CGeneralLayer();
    addChild(m_pGeneralLayer);
    m_pGeneralLayer->release();
    
    //gacha
    m_pGachaMain = new GachaMain();
    addChild(m_pGachaMain);
    m_pGachaMain->release();
    
    //other
    m_pOtherMain = new OtherMain();
    addChild(m_pOtherMain);
    m_pOtherMain->release();
    
    //shop
    m_pShopMain = new ShopMain();
    addChild(m_pShopMain);
    m_pShopMain->release();
    
    // country
    m_pCountryMain = new CountryMain();
    addChild(m_pCountryMain);
    m_pCountryMain->release();
    
    // friend
    m_pFriendMain = new FriendMain();
    addChild(m_pFriendMain);
    m_pFriendMain->release();
    
    //rewardMain
    m_pRewardMain = new RewardMain();
    addChild(m_pRewardMain);
    m_pRewardMain->release();
    
    m_publishWeiboLayer = new PublishWeiboLayer(this, callfuncO_selector(MainScene::cbPublishWeibo));
    addChild(m_publishWeiboLayer);
    m_publishWeiboLayer->release();
    m_publishWeiboLayer->setScale(0);

    if(OcProxy::Inst()->is568HighScreen())
    {
        this->setPosition(ccp(0, 58 + 30));
        
        CCSprite* sp = CCSprite::spriteWithFile("i5top.png");
        if (sp) {
            addChild(sp, 9999);
            sp->setAnchorPoint(ccp(0,0));
            sp->setPosition(ccp(0, 960));
        }
        
        sp = CCSprite::spriteWithFile("i5bottom.png");
        if (sp) {
            addChild(sp, 9999);
            sp->setAnchorPoint(ccp(0,1));
            sp->setPosition(ccp(0, 0));
        }
    }
    
    
  
    OcProxy::Inst()->weixininit(this);
}



void MainScene::updateBottomPanel()
{
    if(m_pBottomPanel)
    {
        m_pBottomPanel->upDateIcon();
    }
}

//******************************************************************************
// instance
//******************************************************************************
MainScene* MainScene::Inst(){
    if(s_pcIf == NULL){
        s_pcIf = new MainScene();
        s_pcIf->init();
    }
    
    return s_pcIf;
}


//******************************************************************************
// init
//******************************************************************************
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
    
	return true;
}

//******************************************************************************
// onEnter
//******************************************************************************
void MainScene::onEnter()
{
    CCLayer::onEnter();
    
    new_CardAdd = false;
    
    new_MyNew = false;
    
    new_FirstItem = false;
    new_FirstItem_Second = false;
    
    new_FirstPowerUp = false;
    new_FirstPowerUpSeeYuXi = false;
    new_FirstPowerUpSecond = false;
    
    
    new_FirstZhuanSheng = false;
    new_FirstZhuanSheng_Second = false;
    
    m_dlgAlert = NULL;
    
    //background
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("id_background.jpg");
    sp->setScale(2.0);
    sp->setPosition(CCPointMake(320, 480));
    addChild(sp, -100);
    
    if (SaveData::Inst()->hasValidSavedData())
    {
#if defined(kPltfmPp)
          loginFromSL();
#else
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        enableBottomPanel(true);
        m_curLayer = m_pArenaLayer;
        m_curLayer->enter();
        
#endif
       
       
    }
    else if (OrgSaveData::Inst()->hasValidSavedData())
    {
        m_curLayer = m_pCountryMain;
        m_curLayer->enter();
    }
    else
    {
        //login layer
        m_pLoginLayer = new CLoginLayer();
        addChild(m_pLoginLayer);
        m_pLoginLayer->release();
        
        m_curLayer = m_pLoginLayer;
        m_curLayer->enter();
        
        switchBgMusic(enBgMusicNml);
    }
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(MainScene::showNewFinishTitle),
                                                           kNotifyNewTitle,
                                                           NULL);
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(MainScene::showCntryTips),
                                                           kNotifyUpdateChatInf,
                                                           NULL);
}

//******************************************************************************
// onExit
//******************************************************************************
void MainScene::onExit()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyNewTitle);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyUpdateChatInf);
    
    CCLayer::onExit();
}

//******************************************************************************
// nmlStart
//******************************************************************************
void MainScene::nmlStart()
{
    if(CGameData::Inst()->loginRequest())
    {
        m_iReqType = enMainReqType_Login;
        scheduleUpdate();
    }
    
    showBottomPanel(false);
}

void MainScene::loginFromSL()
{
    CGameData::Inst()->clearSessionId();
    if(CGameData::Inst()->loginRequest())
    {
        m_iReqType = enMainReqType_FromSL;
        scheduleUpdate();
    }
}

//******************************************************************************
// reqNewbieGacha
//******************************************************************************
void MainScene::reqNewbieGacha()
{
    CCNode* goGachaDlg = getChildByTag(kGoGachaDlgTag);
    if(goGachaDlg)
        removeChild(goGachaDlg, true);

    if(CGameData::Inst()->newbieRequest(0, 4, NULL))
    {
        m_iReqType = enMainReqType_Newbie4;
        scheduleUpdate();
    }
}

//******************************************************************************
// newbieFightEnd
//******************************************************************************
void MainScene::newbieFightEnd()
{
    const char* dsp = CGameData::Inst()->getLanguageValue("newBieTip_new1");
    
    Dialog1* goGachaDlg = new Dialog1(this, callfuncO_selector(MainScene::cbGoGachaDlg), dsp,false);
    addChild(goGachaDlg, 500, kGoGachaDlgTag);
    goGachaDlg->release();
    goGachaDlg->setPosition(CCPointMake(320, 480));
    goGachaDlg->setScale(0.0);
    goGachaDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

//******************************************************************************
// newbieFightEnd
//******************************************************************************
void MainScene::newbieGachaEnd()
{
    CGachaRet* pGachaRet = CGameData::Inst()->getGachaRet();
    CUserCardInfo* pCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
    
    char buf[200];
    snprintf(buf, 199, CGameData::Inst()->getLanguageValue("newbie_gacha_end"),
             pCardInfo->star, pCardInfo->name);
    
    Dialog1* goGachaDlg = new Dialog1(this, callfuncO_selector(MainScene::cbNewbieGachaEnd), buf, false);
    addChild(goGachaDlg, 500, kGoGachaDlgTag);
    goGachaDlg->release();
    goGachaDlg->setPosition(CCPointMake(320, 480));
    goGachaDlg->setScale(0.0);
    goGachaDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

//******************************************************************************
// cbNewbieGachaEnd
//******************************************************************************
void MainScene::cbNewbieGachaEnd(cocos2d::CCObject *pObj)
{
    CCNode* goGachaDlg = getChildByTag(kGoGachaDlgTag);
    if(goGachaDlg)
        removeChild(goGachaDlg, true);

    goToGeneral();
}

//******************************************************************************
// cbGoGachaDlg
//******************************************************************************
void MainScene::cbGoGachaDlg(CCObject* pObj)
{
    if (CGameData::Inst()->getCommonInfo()->bOpen_cards_shop)
    {
        reqNewbieGacha();
    }
    else
    {
        CCNode* goGachaDlg = getChildByTag(kGoGachaDlgTag);
        if(goGachaDlg)
            removeChild(goGachaDlg, true);
        
        const char* dsp = CGameData::Inst()->getLanguageValue("shopTip_mainMenu");
        
        MsgBox* goGachaMsg = new MsgBox("ma_prompt.png", dsp, ccc3(63, 23, 13));
        addChild(goGachaMsg, 500, kGoGachaMsgTag);
        goGachaMsg->release();
        goGachaMsg->setPosition(CCPointMake(320, 480));
        goGachaMsg->setScale(0.0);
        goGachaMsg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        enableBottomPanel(true);
        
        Arrow* pArrow = new Arrow(30, 255);
        pArrow->setPosition(CCPointMake(268, 110));
        addChild(pArrow, 500, kGoGachaArrowTag);
        pArrow->release();
        pArrow->setRotation(-90);
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
}



void MainScene::goToShop()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    if(m_curLayer != m_pShopMain){
        m_curLayer->exit();
        m_curLayer = m_pShopMain;
        m_pShopMain->enter();
        m_pShopMain->beginCoinShop();
    }
}

void MainScene::cbAlipay(int payType)
{
    if(m_curLayer != m_pShopMain){
        m_curLayer->exit();
        m_curLayer = m_pShopMain;
        m_pShopMain->enter();
        m_pShopMain->beginCoinShop();
    }
    m_pShopMain->showAlipayOver(payType);
    
}

void MainScene::gotoOtherMain()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    m_curLayer->exit();
    m_curLayer = m_pOtherMain;
    m_curLayer->enter();
    showBottomPanel(true);
}
void MainScene::refreshArena()
{    
    if((m_curLayer == m_pArenaLayer)
       &&(CGameData::Inst()->isFighting() == false)){
        m_curLayer->exit();
        m_curLayer->enter();
    }
    
}


void MainScene::GoToHomePage()
{
    m_nxtLayer = m_pFirstLayer;
    
    m_curLayer->exit();
    
    OcProxy::Inst()->showUsedMemory();
    
    if (m_fullPackDlg) {
        m_fullPackDlg->removeFromParentAndCleanup(true);
        m_fullPackDlg = NULL;
    }
    
    if (m_fullCostDlg) {
        m_fullCostDlg->removeFromParentAndCleanup(true);
        m_fullCostDlg = NULL;
    }
    
    if (m_foodInTeamDlg) {
        m_foodInTeamDlg->removeFromParentAndCleanup(true);
        m_foodInTeamDlg = NULL;
    }
    
    if (m_foodUpThreeStarsDlg) {
        m_foodUpThreeStarsDlg->removeFromParentAndCleanup(true);
        m_foodUpThreeStarsDlg = NULL;
    }
    
    if(CGameData::Inst()->getReqStat() == kReqStatOk){
        enterNxtLayer();
    }
    else {
        if(m_iReqType == enMainReqType_Null){
            m_iReqType = enMainReqType_Unknow;
            scheduleUpdate();
        }
    }
}

void MainScene::GoToYuanbaoPage()
{
    m_nxtLayer = m_pShopMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    
    m_pShopMain->beginCoinShop();
    
}

void MainScene::exitArenaLayer()
{
    if (m_pArenaLayer && m_nxtLayer==m_pArenaLayer)
    {
        m_pArenaLayer->exit();
    }
}

//******************************************************************************
// cbBottomPanel
//******************************************************************************
void MainScene::cbBottomPanel(CCObject* pObj)
{
    if (CGeneralLayer* pGeneralLayer = dynamic_cast<CGeneralLayer*>(m_curLayer))
    {
        if (onCurTeamCostCall(pGeneralLayer->getTeamInfoScrollViewLayer()))
        {
            return;
        }
    }
    
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    CBottomPanelRet* pRet = (CBottomPanelRet*) pObj;
    
    
    if(MainScene::Inst()->getChildByTag(98998))//判断是否绘制了引导的箭头
    {
        
        if(pRet->iBtnSel == id_generals && MainScene::Inst()->new_CardAdd)
        {
            if(MainScene::Inst()->new_CardAdd)
            {
                m_nxtLayer = m_pGeneralLayer;
                
                
                //引导
                Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98998);
                if(temp)
                    temp->removeFromParentAndCleanup(true);
                
                
                
                Arrow* pArrow = new Arrow(30, 255);
                
                pArrow->setPosition(CCPointMake(115 + 300, 210));
                
                
                
                MainScene::Inst()->addChild(pArrow, 50000, 98997);
                
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));
                
                
                MainScene::Inst()->enableBottomPanel(false);
            }
        }
        
        else if(pRet->iBtnSel == id_first && !MainScene::Inst()->new_CardAdd)
        {
            
            if(MainScene::Inst()->new_CardAdd)
            {
               return;
            }
            else
            {
                m_nxtLayer = m_pFirstLayer;
                
                
                //引导
                Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98998);
                if(temp)
                    temp->removeFromParentAndCleanup(true);
                
                
                
                Arrow* pArrow = new Arrow(30, 255);
                
                pArrow->setPosition(CCPointMake(205, 395));
                
                
                
                MainScene::Inst()->addChild(pArrow, 50000, 98997);
                
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));
                
                
                MainScene::Inst()->enableBottomPanel(false);
            }
            
            

            
            
        }
        else
            return;

    }
    else
    {
        
        
        switch (pRet->iBtnSel) {
            case id_first:
                m_nxtLayer = m_pFirstLayer;
                break;
            case id_generals:
                m_nxtLayer = m_pGeneralLayer;
                break;
            case id_gacha:
                m_nxtLayer = m_pGachaMain;
                break;
            case id_shop:
                m_nxtLayer = m_pShopMain;
                break;
            case id_social:
                m_nxtLayer = m_pFriendMain;
                break;
            case id_country:
                
                if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_guild_lv || CGameData::Inst()->getCntryInfo()->status != 0)
                {
                    m_nxtLayer = m_pCountryMain;
                }
                else
                {
                    if (m_dlgAlert == NULL && m_pFirstLayer->m_dlgAlert == NULL)
                    {
                        char buf[100];
                        snprintf(buf, 99, "%s%d%s",
                                 CGameData::Inst()->getLanguageValue("open_permit_guild_lv1"),
                                 CGameData::Inst()->getCommonInfo()->permit_guild_lv,
                                 CGameData::Inst()->getLanguageValue("open_permit_guild_lv2"));
                        
                        
                        m_dlgAlert = new Dialog1(this,
                                                 callfuncO_selector(MainScene::cbAlertDlg),
                                                 buf,
                                                 false);
                        addChild(m_dlgAlert, 10000);
                        m_dlgAlert->release();
                        
                        
                        m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                        m_dlgAlert->setScale(0.0);
                        m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                    }
                    
                    
                    return;
                }
                
                
                break;
            default:
                m_nxtLayer = m_pFirstLayer;
                break;
        }
    }
    
    
    
    
    if(CGameData::Inst()->chkLogin() == false){
        if(m_nxtLayer == m_pGachaMain){
            CCNode* goGachaMsg = getChildByTag(kGoGachaMsgTag);
            if(goGachaMsg)
                removeChild(goGachaMsg, true);
            
            CCNode* pArrow = getChildByTag(kGoGachaArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
        }
        else {
            return;
        }
    }

//    if(m_curLayer == m_nxtLayer)
//        return;
    
    m_curLayer->exit();

    OcProxy::Inst()->showUsedMemory();
        
    if (m_fullPackDlg) {
        m_fullPackDlg->removeFromParentAndCleanup(true);
        m_fullPackDlg = NULL;
    }
    
    if (m_fullCostDlg) {
        m_fullCostDlg->removeFromParentAndCleanup(true);
        m_fullCostDlg = NULL;
    }
    
    if (m_foodInTeamDlg) {
        m_foodInTeamDlg->removeFromParentAndCleanup(true);
        m_foodInTeamDlg = NULL;
    }
    
    if (m_foodUpThreeStarsDlg) {
        m_foodUpThreeStarsDlg->removeFromParentAndCleanup(true);
        m_foodUpThreeStarsDlg = NULL;
    }
        
    if(CGameData::Inst()->getReqStat() == kReqStatOk){
        enterNxtLayer();
    }
    else {
        if(m_iReqType == enMainReqType_Null){
            m_iReqType = enMainReqType_Unknow;
            scheduleUpdate();
        }
    }
}


void MainScene::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
}


//******************************************************************************
// enterNxtLayer
//******************************************************************************
void MainScene::enterNxtLayer()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(m_nxtLayer == m_pArenaLayer
       //|| (m_nxtLayer == m_pGachaMain)
       )
    {
        if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count()){
            showFullPackDlg();
            return;
        }
    }

    m_curLayer = m_nxtLayer;
    m_curLayer->enter();
}
//
//显示系统公告，当有公告的时候
//@pragma  isFReq  第一次调用的时候为true
//
void MainScene::showTips_gonggao(CUserInfo * pUsrInf,bool isFReq)
{
    if (isFReq) {
        if (pUsrInf->m_lsango_tips_1.size() > 0) {
                vector<c_sango_Tips> tipTime_GongGao;
                tipTime_GongGao.clear();
                list <c_sango_Tips>::iterator ii;
                for (ii  = pUsrInf->m_lsango_tips_1.begin(); ii != pUsrInf->m_lsango_tips_1.end(); ++ii) {
                    
                    c_sango_Tips sip = *ii;
                    if ( CGameData::Inst()->getCurrentTime() >= sip.m_sfirstTime &&   sip.m_sendTime >= CGameData::Inst()->getCurrentTime()) {
                        tipTime_GongGao.push_back(sip);
                    }
                }
                if (tipTime_GongGao.size() > 0) {
                    m_tipVector.clear();
                    for (vector<c_sango_Tips>::iterator it = tipTime_GongGao.begin(); it != tipTime_GongGao.end(); ++it) {
                        m_tipVector.push_back((*it).m_content);
                    }
                    schedule(schedule_selector(MainScene::Re_showTips), tipTime_GongGao.at(0).m_interVal * 60);
                }
        }
        m_tipOneTimeVector.clear();
        if (pUsrInf->m_lsango_tips_2.size() > 0) {
            list<c_sango_Tips_oneTime>::iterator it;
            for (it = pUsrInf->m_lsango_tips_2.begin(); it != pUsrInf->m_lsango_tips_2.end(); ++it) {
                c_sango_Tips_oneTime sangoTips_onetime = *it;
                m_tipOneTimeVector.push_back(sangoTips_onetime.m_content);
            }
        }
        if (m_tipVector.size() > 0) {
            showBottomPromptGongGaoInfo(m_tipVector.at(0).c_str());
        }else{
            if (m_tipOneTimeVector.size() > 0) {
                showBottomPromptGongGaoInfo(m_tipOneTimeVector.at(0).c_str());
            }
        }

    }else{
        m_tipOneTimeVector.clear();
        if (pUsrInf->m_lsango_tips_2.size() > 0) {
            list<c_sango_Tips_oneTime>::iterator it;
            for (it = pUsrInf->m_lsango_tips_2.begin(); it != pUsrInf->m_lsango_tips_2.end(); ++it) {
                c_sango_Tips_oneTime sangoTips_onetime = *it;
                m_tipOneTimeVector.push_back(sangoTips_onetime.m_content);
            }
        }
        if (m_tipVector.size() > 0) {

        }else{
            if (m_tipOneTimeVector.size() > 0) {
                showBottomPromptGongGaoInfo(m_tipOneTimeVector.at(0).c_str());
            }
        }

    }
    
}

void MainScene::saveArenaShowTips_gonggao()
{
    m_tipOneTimeVector.clear();
    CUserInfo * pUsrInf = CGameData::Inst()->getUsrInfo();
    if (pUsrInf->m_lsango_tips_2.size() > 0) {
        list<c_sango_Tips_oneTime>::iterator it;
        for (it = pUsrInf->m_lsango_tips_2.begin(); it != pUsrInf->m_lsango_tips_2.end(); ++it) {
            c_sango_Tips_oneTime sangoTips_onetime = *it;
            m_tipOneTimeVector.push_back(sangoTips_onetime.m_content);
        }
    }
}

void MainScene::afterArenaShowTips_gonggao()
{
    if (m_tipVector.size() > 0) {
        
    }else{
        if (m_tipOneTimeVector.size() > 0) {
            showBottomPromptGongGaoInfo(m_tipOneTimeVector.at(0).c_str());
        }
    }

}

void MainScene::Re_showTips(ccTime dt)
{
   unschedule(schedule_selector(MainScene::Re_showTips));
    if (m_tipOneTimeVector.size()>0) {
        schedule(schedule_selector(MainScene::Re_showTips),1.0f);
        return;
    }
    CUserInfo*pUsrInf = CGameData::Inst()->getUsrInfo();
    vector<c_sango_Tips> tipTime_GongGao;
    tipTime_GongGao.clear();
    list <c_sango_Tips>::iterator ii;
    for (ii  = pUsrInf->m_lsango_tips_1.begin(); ii != pUsrInf->m_lsango_tips_1.end(); ++ii) {
        
        c_sango_Tips sip = *ii;
        if ( CGameData::Inst()->getCurrentTime() >= sip.m_sfirstTime &&   sip.m_sendTime >= CGameData::Inst()->getCurrentTime()) {
            tipTime_GongGao.push_back(sip);
        }
    }
    if (tipTime_GongGao.size() > 0) {
        m_tipVector.clear();
        for (vector<c_sango_Tips>::iterator it = tipTime_GongGao.begin(); it != tipTime_GongGao.end(); ++it) {
            m_tipVector.push_back((*it).m_content);
        }
        showBottomPromptGongGaoInfo(m_tipVector.at(0).c_str());
        schedule(schedule_selector(MainScene::Re_showTips), tipTime_GongGao.at(0).m_interVal * 60);
    }

    
}


void MainScene::showBottomPromptGongGaoInfo(const char *promptInfo)//add1.15
{
    if (promptInfo == 0) {
        m_lbBottomPanel_gonggao->stopAllActions();
        m_lbBottomPanel_gonggao->setPosition(CCPointMake(2 + 640, 110));
        return;
    }
    if (!m_lbBottomPanel) {
        m_lbBottomPanel = TextNode::textWithString("", 22);
        m_pBottomPanel->addChild(m_lbBottomPanel, 100);
        m_lbBottomPanel->setAnchorPoint(CCPointMake(0, 0));
    }
    m_lbBottomPanel->setIsVisible(false);
    if (!m_lbBottomPanel_gonggao) {
        m_lbBottomPanel_gonggao = TextNode::textWithString(promptInfo, 22);
        m_pBottomPanel->addChild(m_lbBottomPanel_gonggao, 100);
        m_lbBottomPanel_gonggao->setAnchorPoint(CCPointMake(0, 0));
    }
    else {
        m_lbBottomPanel_gonggao->setString(promptInfo);
    }
    m_lbBottomPanel_gonggao->setIsVisible(true);
    m_lbBottomPanel_gonggao->stopAllActions();
    m_lbBottomPanel_gonggao->setPosition(CCPointMake(2 + 640, 110));
    
    m_lbBottomPanel_gonggao->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.2f, CCPointMake(2, 110)),
                                                   CCDelayTime::actionWithDuration(1.0f),
                                                   CCCallFunc::actionWithTarget(this, callfunc_selector(MainScene::scrollBottomPromptGongGaoInfo)),
                                                   NULL));


}


void MainScene::scrollBottomPromptGongGaoInfo()//add1.15
{
    float lbBottomPanelX = m_lbBottomPanel_gonggao->getContentSize().width;
    m_lbBottomPanel_gonggao->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(lbBottomPanelX / 30, CCPointMake(-lbBottomPanelX, 110)),
                                                   CCCallFunc::actionWithTarget(this, callfunc_selector(MainScene::hideBottomPromptGongGao)),
                                                   NULL));
}

void MainScene::hideBottomPromptGongGao()//add1.15
{
    if (m_tipVector.size() > 0 ) {
        if (m_tipVector.size() - 1 > m_Itip_index) {
            m_Itip_index++;
            showBottomPromptGongGaoInfo(m_tipVector.at(m_Itip_index).c_str());
        }
        else if (m_Itip_index < m_tipVector.size() + m_tipOneTimeVector.size()-1)
        {
            m_Itip_index++;
            showBottomPromptGongGaoInfo(m_tipOneTimeVector.at(m_Itip_index - m_tipVector.size()).c_str());
        }else{
            m_Itip_index = 0;
            m_tipVector.clear();
            m_tipOneTimeVector.clear();
            m_lbBottomPanel_gonggao->setIsVisible(false);
            if (m_lbBottomPanel) {
                m_lbBottomPanel->setIsVisible(true);
                showBottomPromptInfo(m_tips_string.c_str());
            }
        }
                return;
    }
    if (m_tipOneTimeVector.size()>0) {
        if (m_Itip_index < m_tipOneTimeVector.size()-1) {
            m_Itip_index++;
            showBottomPromptGongGaoInfo(m_tipOneTimeVector.at(m_Itip_index).c_str());
            return;
        }else{
            m_Itip_index = 0;
           
            m_tipVector.clear();
            m_tipOneTimeVector.clear();
           
        }

    }
    m_lbBottomPanel_gonggao->setIsVisible(false);
    if (m_lbBottomPanel) {
        m_lbBottomPanel->setIsVisible(true);
        showBottomPromptInfo(m_tips_string.c_str());
    }
   
}

void MainScene::setBottomPromptInfoString(const char* str)
{
    m_tips_string = string(str);
}

//******************************************************************************
// showBottomPromptInfo
//******************************************************************************
void MainScene::showBottomPromptInfo(const char *promptInfo) {
    if (promptInfo == 0) {
        m_lbBottomPanel->stopAllActions();
        m_lbBottomPanel->setPosition(CCPointMake(2 + 640, 110));
        return;
    }
    setBottomPromptInfoString(promptInfo);
    const char* s_promptInfo = NULL;
    if (strlen(CGameData::Inst()->getLanguageValue(promptInfo)) != 0) {
        string ss(promptInfo);
        if (ss.compare("navi_cost_month_card") == 0) {
            char cBuf[256];
            sprintf(cBuf,CGameData::Inst()->getLanguageValue("navi_cost_month_card"),CGameData::Inst()->getCommonInfo()->month_item_cost);
             s_promptInfo = cBuf;
        }
        else{
        s_promptInfo = CGameData::Inst()->getLanguageValue(promptInfo);
        }
    }else{
        int count = 0;
        char buf[100];
        do {
            count++;
            memset(buf, 0, sizeof(buf));
            snprintf(buf, 99, "%s%d", promptInfo,count);
            
        } while (strlen(CGameData::Inst()->getLanguageValue(buf)) != 0);
        memset(buf, 0, sizeof(buf));
        snprintf(buf, 99, "%s%d",promptInfo ,int(CCRANDOM_0_1() * (count-1)) + 1);
        s_promptInfo = CGameData::Inst()->getLanguageValue(buf);
    }

    if (!m_lbBottomPanel) {
        m_lbBottomPanel = TextNode::textWithString(s_promptInfo, 22);
        m_pBottomPanel->addChild(m_lbBottomPanel, 100);
        m_lbBottomPanel->setAnchorPoint(CCPointMake(0, 0));
    }
    else {
        m_lbBottomPanel->setString(s_promptInfo);
    }
    
    m_lbBottomPanel->stopAllActions();
    m_lbBottomPanel->setPosition(CCPointMake(2 + 640, 110));
    
    m_lbBottomPanel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.2f, CCPointMake(2, 110)),
                                                   CCDelayTime::actionWithDuration(1.0f),
                                                   CCCallFunc::actionWithTarget(this, callfunc_selector(MainScene::scrollBottomPromptInfo)),
                                                   NULL));
}

//******************************************************************************
// scrollBottomPromptInfo
//******************************************************************************
void MainScene::scrollBottomPromptInfo() {
    float lbBottomPanelX = m_lbBottomPanel->getContentSize().width;
    m_lbBottomPanel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(lbBottomPanelX / 30, CCPointMake(-lbBottomPanelX, 110)),
                                                   CCCallFunc::actionWithTarget(this, callfunc_selector(MainScene::resetBottomPromptInfo)),
                                                   NULL));
}
//******************************************************************************
// resetBottomPromptInfo
//******************************************************************************
void MainScene::resetBottomPromptInfo() {
    m_lbBottomPanel->setPosition(CCPointMake(2 + 640, 110));
    
    m_lbBottomPanel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(2, 110)),
                                                   CCDelayTime::actionWithDuration(1.0f),
                                                   CCCallFunc::actionWithTarget(this, callfunc_selector(MainScene::scrollBottomPromptInfo)),
                                                   NULL));
}

//******************************************************************************
// showBottomPanel
//******************************************************************************
void MainScene::showBottomPanel(bool bShow)
{    
    m_pBottomPanel->setEnable(bShow);
    
    if((bShow) && (CGameData::Inst()->isFighting() == false)){
        m_pBottomPanel->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 0)));
    }
    else {
        m_pBottomPanel->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, -157)));
    }
}

bool MainScene::getBottomPanelIsEnableOrNot()
{
    return m_pBottomPanel->getIsEnable();
}


//******************************************************************************
// enableBottomPanel
//******************************************************************************
void MainScene::enableBottomPanel(bool bEnable)
{
    m_pBottomPanel->setEnable(bEnable);
}



//******************************************************************************
// showNoticeInviteAward
//******************************************************************************
void MainScene::showNoticeInviteAward() {
    
    m_notice->enterWithType(enNoticeTypeInviteAward);
}

//showNotice
void MainScene::showNotice()
{
    isClaimNoticeShow = CGameData::Inst()->getUsrInfo()->isMzsm;
    //notice 
    m_notice = new CNotice(this,callfuncO_selector(MainScene::cnCallBack));
    addChild(m_notice,2000);
    m_notice->release();
    m_notice->setAnchorPoint(ccp(0.5, 0.5));
    m_notice->setPosition(ccp(0, 0));
    
    enableBottomPanel(false);
    if (!isClaimNoticeShow)
        m_notice->enterWithType(enNoticeTypeClaim);
    else
        m_notice->enterWithType(enNoticeTypeSystem);
}

void MainScene::showGiftNotice()
{
    enableBottomPanel(false);
    m_notice->enterWithType(enNoticeTypeGift);
}

void MainScene::showLendInfoNotice()
{
    enableBottomPanel(false);
    m_notice->enterWithType(enNoticeTypeLendInfo);
}

void MainScene::cnCallBack()
{
    
//    if(OcProxy::Inst()->getIsLoadSuccess() == false)
//        return;
    if (!isClaimNoticeShow) {
        if(CGameData::Inst()->readMzsm())
        {
            m_iReqType = enMainReqType_ReadMzsm;
            scheduleUpdate();
        }
    }else{
        if(strlen(CGameData::Inst()->getNoticeGiftString())!=0 && isGfNoticeShow == false)
        {
            showGiftNotice();
            isGfNoticeShow = true;
        }else {
            if(strlen(CGameData::Inst()->getLendInfo())!=0&&isLINoticeShow == false){
                showLendInfoNotice();
                isLINoticeShow = true;
            }else {
                
                if (strlen(CGameData::Inst()->getNoticeInviteAward())!=0 && isIVNoticeShow == false)
                {
                    showNoticeInviteAward();
                    isIVNoticeShow = true;
                }
                else {
                    OcProxy::Inst()->removeWebview();
                    removeChild(m_notice, true);
                    m_notice = NULL;
                    enableBottomPanel(true);
                    
                    
                    
                    
                    //强制领取奖励
                    
                    
                    
                    int defaultLabel = 0;
                    
                    int lv = CGameData::Inst()->getUsrInfo()->lv;
                    int signNum = CGameData::Inst()->getRwdSignInfList()->count()-1;    //获得的奖励总条数-1：总有一个明日可领取
                    int levelUpNum = 0;
                    for(int i=0;i<CGameData::Inst()->getRwdLevelUpArr()->count();++i){
                        if(CGameData::Inst()->getRwdLevelUpArr()->getObjectAtIndex(i)->lv <= lv){
                            levelUpNum += 1;
                        }else
                            break;
                    }
                    
                    int monthsNum = 0;
                    
                    int bonusNum = 0;
                    for(int i=0;i<CGameData::Inst()->getRwdBonusInfArr()->count();++i){
                        if(CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(i)->iDays >= CGameData::Inst()->getRwdBonusInfArr()->getObjectAtIndex(i)->rewardDays){
                            bonusNum += 1;
                        }else
                            break;
                    }
                    int sysNum = 0;
                    for(int i=0;i<CGameData::Inst()->getRwdHuodongInfArr()->count();++i){
                        if(!CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->bCanGet&&(CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->loginCounts>=CGameData::Inst()->getRwdHuodongInfArr()->getObjectAtIndex(i)->awardDays)){
                            sysNum += 1;
                        }else
                            break;
                    }
                    
                    if (signNum> 0 ) {
                        defaultLabel = 0;
                    }else if(levelUpNum>0){
                        defaultLabel = 1;
                    }else if(monthsNum>0){
                        defaultLabel = 2;
                    }else if(bonusNum>0){
                        defaultLabel = 3;
                    }else if(sysNum>0){
                        defaultLabel = 4;
                    }
                    MainScene::Inst()->setLabel(defaultLabel);
                    if (signNum+levelUpNum+bonusNum+monthsNum+sysNum > 0) {
                        
                        if(m_pFirstLayer)
                            m_pFirstLayer->exit();
                        
                        m_curLayer = m_pRewardMain;
                        m_curLayer->enter();

                        
                    }
                    else{
                        m_curLayer = m_pFirstLayer;
                        m_curLayer->enter();
                    }
                    
                    
#ifdef TalkData
                    TDGAUtility::Inst()->createAccount(CGameData::Inst()->getUsrInfo()->uid);
                    TDGAUtility::Inst()->shareAccount()->setAccountName(CGameData::Inst()->getUsrInfo()->username);
                    TDGAUtility::Inst()->shareAccount()->setLevel(CGameData::Inst()->getUsrInfo()->lv);
                    
                    
                    
                    
                    if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
                        
                        TDGAUtility::Inst()->shareAccount()->setAccountType(TDCCAccount::kAccountTencentWeibo);
                    }
                    else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
                        TDGAUtility::Inst()->shareAccount()->setAccountType(TDCCAccount::kAccountSianWeibo);
                        
                    }
                    else
                    {
                        TDGAUtility::Inst()->shareAccount()->setAccountType(TDCCAccount::kAccountAnonymous);
                    }
#else
                    
#endif
                    
                    

                    
                    
                    
//                    m_curLayer = m_pFirstLayer;
//                    m_curLayer->enter();
                    
#if defined (apppay)
                    if(CGameData::Inst()->getStoreStatus() != 2)
#endif
                        CGameData::Inst()->loadStore();
                }
            }
        }
    }
}

 

//******************************************************************************
// showUsrInfPanel
//******************************************************************************
void MainScene::showUsrInfPanel(bool bShow)
{
    if((bShow) && (CGameData::Inst()->isFighting() == false)){
        m_pUsrInfoPanel->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 0)));
    }
    else {
        m_pUsrInfoPanel->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 170)));
    }
}


//******************************************************************************
// showUsrInfPanel
//******************************************************************************
void MainScene::setTitle(int iNo, int iStar, int max_Star, char* name, int category, int subCategory)
{
    
    if(iNo == 0){
        CCNode* node = this->getChildByTag(kMainSceneTitle);
        
        while (node != NULL) {
            removeChild(node, true);
            node = this->getChildByTag(kMainSceneTitle);
        }
        
        return;
    }
    
    
    //card No.
    char buf[100];
    snprintf(buf, 99,  "%d", iNo);
    
   
    TitleBarNoAction *m_spTitle = TitleBarNoAction::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(320, 766));
    m_spTitle->setTag(kMainSceneTitle);
    addChild(m_spTitle);
    
    TextNode* lbText = TextNode::textWithString("No", CCSizeMake(310, 20), CCTextAlignmentCenter, 24);
    lbText->setPosition(CCPointMake(185, 780));
    lbText->setColor(ccYELLOW);
    lbText->setShadowColor(ccBLACK);
    lbText->setTag(kMainSceneTitle);
    addChild(lbText, 1);
    
    lbText = TextNode::textWithString(buf, CCSizeMake(310, 20), CCTextAlignmentCenter, 24);
    lbText->setPosition(CCPointMake(185, 760));
    lbText->setColor(ccYELLOW);
    lbText->setShadowColor(ccBLACK);
    lbText->setTag(kMainSceneTitle);
    addChild(lbText, 1);
    
    
    //card name
   // lbText = TextNode::textWithString(name, CCSizeMake(310, 40), CCTextAlignmentCenter, 42);
    lbText = TextNode::textWithString(name, 42);
    lbText->setAnchorPoint(ccp(0.5, 0.5));
    
    if (lbText->getContentSize().width > 238)
        lbText->setPosition(CCPointMake(342, 770));
    else
        lbText->setPosition(CCPointMake(322, 770));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    lbText->setTag(kMainSceneTitle);
    addChild(lbText, 1);
    
    //star
    CCSprite* spStar;
    // 根据iNo 找到对应的升级的最大星级数
    //    CCardBaseInfo* cardBaseInfo;
    //    int currentNo = iNo;
    //    int maxStar = iStar;
    //    while (1) {
    //        cardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(currentNo);
    //        if (cardBaseInfo->ugp_target) {
    //            currentNo = cardBaseInfo->ugp_target;
    //            cardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(cardBaseInfo->ugp_target);
    //            if (maxStar < cardBaseInfo->star)
    //                maxStar = cardBaseInfo->star;
    //        } else {
    //            break;
    //        }
    //    }
    
//    CCardBaseInfo* cardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(iNo);
    int maxStar = max_Star;
    if (maxStar < iStar) {
        maxStar = iStar;
    }
    for(int i = 0; i < maxStar; i++){
        if (i < iStar) {
            spStar = CCSprite::spriteWithSpriteFrameName("id_star.png");
            float starBeginPosX = 320 - spStar->getContentSize().width * (maxStar - 1) / 2;
            spStar->setPosition(CCPointMake(starBeginPosX + spStar->getContentSize().width * i, 715));
            spStar->setTag(kMainSceneTitle);
            addChild(spStar);
        } else {
            spStar = CCSprite::spriteWithSpriteFrameName("id_star_2.png");
            float starBeginPosX = 320 - spStar->getContentSize().width * (maxStar - 1) / 2;
            spStar->setPosition(CCPointMake(starBeginPosX + spStar->getContentSize().width * i, 715));
            spStar->setTag(kMainSceneTitle);
            addChild(spStar);
        }
    }
    
    //card category
    const char* categoryChar = getCategoryChar(category);
    
    lbText = TextNode::textWithString(categoryChar, CCSizeMake(310, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(180, 710));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    lbText->setTag(kMainSceneTitle);
    addChild(lbText, 1);
    
    //card subcategory
    if (subCategory != enGeneralTp_Null &&
        enGeneralTp_Invalidbn != subCategory)
    {
        categoryChar = getCategoryChar(subCategory);
        
        lbText = TextNode::textWithString(categoryChar, CCSizeMake(310, 20), CCTextAlignmentLeft, 20);
        lbText->setPosition(CCPointMake(180, 680));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        lbText->setTag(kMainSceneTitle);
        addChild(lbText, 1);
    }
}


const char* MainScene::getCategoryChar(int category)
{
    switch (category)
    {
        case enGeneralTp_GuoShi:     //国士无双(龙)
            return CGameData::Inst()->getLanguageValue("mainSceneTip1");
            break;
            
        case enGeneralTp_GongShou:   //攻守兼备(平衡系)
            return CGameData::Inst()->getLanguageValue("mainSceneTip2");
            break;
            
        case enGeneralTp_ShengSheng: //生生不息(回复系)
            return CGameData::Inst()->getLanguageValue("mainSceneTip3");
            break;
            
        case enGeneralTp_XiaoYong:   //骁勇善战(攻击)
            return CGameData::Inst()->getLanguageValue("mainSceneTip4");
            break;
            
        case enGeneralTp_JinShi:     //金石之坚(体力系)
            return CGameData::Inst()->getLanguageValue("mainSceneTip5");
            break;
            
        case enGeneralTp_God:        //神将(神)
            return CGameData::Inst()->getLanguageValue("mainSceneTip6");
            break;
            
        case enGeneralTp_ZhuanSheng: //转生道具(进化用)
            return CGameData::Inst()->getLanguageValue("mainSceneTip7");
            break;
            
        case enGeneralTp_QiangHua:   //强化合成(强化合成用)
            return CGameData::Inst()->getLanguageValue("mainSceneTip8");
            break;
            
        case enGeneralTp_mojiang:    //魔将
            return CGameData::Inst()->getLanguageValue("card_category_9");
            break;
            
        case enGeneralTp_LongShen:    //龙&神
            return CGameData::Inst()->getLanguageValue("card_category_10");
            break;
            

            
        default:
            return "";
            break;
    }
}

//******************************************************************************
// switchBgMusic
//******************************************************************************
void MainScene::switchBgMusic(int idx)
{
    currenBackMusic = idx;
    static int iCur = kInvalidValue;
    
    if(iCur == idx)
        return;
    
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

    switch (idx) {
        case enBgMusicBoss:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bossbattle.mp3", true);
            break;
             
        case enBgMusicBattle:
            if(CGameData::Inst()->isMusicOn == true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("battle.mp3", true);
            break;
             
        case enBgMusicNml:
            if(CGameData::Inst()->isMusicOn ==  true) {
                if (CGameData::Inst()->getIsUseChristmasUI()) {
                    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("normal_newyear.mp3", true);
                }
                else {
                    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("normal.mp3", true);
                }
            }
            break;
            
        case enBgMusicOrgFight:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OrgFight.mp3", true);
            break;
            
        case enBgMusicOrgFail:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OrgFail.mp3", false);
            break;
            
        case enBgMusicOrgWin:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OrgWin.mp3", false);
            break;
        
        case enBgMusicOrgEnter:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OrgEnter.mp3", true);
            break;
            
        case enBgMusicQianLiBg:
            if(CGameData::Inst()->isMusicOn== true)
                SimpleAudioEngine::sharedEngine()->playBackgroundMusic("QianLiBg.mp3", true);
            break;
            
        default:
            if(CGameData::Inst()->isMusicOn ==  true) {
                if (CGameData::Inst()->getIsUseChristmasUI()) {
                    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("normal_newyear.mp3", true);
                }
                else {
                    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("normal.mp3", true);
                }
            }
            break;
    }
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0);
    iCur = idx;
}

int MainScene::getBackMusic()
{
    return currenBackMusic;
}

//******************************************************************************
// loginFinished
//******************************************************************************
void MainScene::loginFinished()
{
    if(m_pLoginLayer != NULL){
        removeChild(m_pLoginLayer, true);
        if(m_curLayer == m_pLoginLayer)
            m_curLayer = NULL;
        m_pLoginLayer = NULL;
    }
    
    if (SaveData::Inst()->hasValidSavedData()
        && strcmp(SaveData::Inst()->getUserId(), CGameData::Inst()->getUserId()) != 0)
    {   // 进度保存文件中的pid和登入返回的pid不同
        SaveData::Inst()->destorySavedData();
        SaveData::Inst()->destorySavedFile();
        
        m_reloginWeiboType = 0;
    }
    else if (OrgSaveData::Inst()->hasValidSavedData()
             && strcmp(OrgSaveData::Inst()->getUserId(), CGameData::Inst()->getUserId()) != 0)
    {
        OrgSaveData::Inst()->destorySavedData();
        OrgSaveData::Inst()->destorySavedFile();
        
        m_reloginWeiboType = 0;
    }
    
    if(CGameData::Inst()->isNewbie()){
        m_iNewbieStep = CGameData::Inst()->getNewbieStep();
        
        if(CGameData::Inst()->cfgRequest()){
            scheduleUpdate();
            m_iReqType = enMainReqType_Cfg;
        }
    }
    else{
        if (m_reloginWeiboType == 1) {
            m_reloginWeiboType = 0;
            m_curLayer = m_pArenaLayer;
            m_pArenaLayer->enterArenaEnd();
        }
        else if (m_reloginWeiboType == 2)
        {
            m_reloginWeiboType = 0;
            m_curLayer = m_pCountryMain;
            m_pCountryMain->enterArenaEnd();
        }
        else if (m_reloginWeiboType == 4)
        {
            m_reloginWeiboType = 0;
            m_curLayer = m_pArenaLayer;
        }
        else {
            if (SaveData::Inst()->hasValidSavedData())
            {
                loginFromSL();
            }
            else
            {
                nmlStart();
            }
        }
    }
    
    // showSnow();
};

//******************************************************************************
// newbieLogin
//******************************************************************************
void MainScene::newbieLogin()
{   
    removeChild(m_newbie, true);
    m_newbie = NULL;
    
    Dialog1* welcomeDlg = new Dialog1(this, callfuncO_selector(MainScene::cbWelcomeDlg), NULL, true);
    addChild(welcomeDlg, 500, kWelcomeDlgTag);
    welcomeDlg->release();
    welcomeDlg->setPosition(CCPointMake(320, 480));
    welcomeDlg->setScale(0.0);
    welcomeDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

//设置tapjoy userId
void MainScene::setTapjoyUserId()
{
    OcProxy::Inst()->setTapjoyUserId(CGameData::Inst()->getUsrInfo()->uid);
}


//******************************************************************************
// cbWelcomeDlg
//******************************************************************************
void MainScene::cbWelcomeDlg(CCObject* pObj)
{
    CCNode* welcomeDlg = getChildByTag(kWelcomeDlgTag);
    if(welcomeDlg) {
        removeChild(welcomeDlg, true);
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }
    
    m_curLayer = m_pArenaLayer;
    m_curLayer->enter();
    
    m_pUsrInfoPanel->showData();
    showUsrInfPanel(true);
    showBottomPanel(true);
    enableBottomPanel(false);
}


//******************************************************************************
// cbCommDlg
//******************************************************************************
void MainScene::cbCommDlg(CCObject* pObj)
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
            case enMainReqType_FromSL:
                CGameData::Inst()->clearSessionId();
                if(CGameData::Inst()->loginRequest())
                {
                    scheduleUpdate();
                }
                break;

            case enMainReqType_Login:
                if(CGameData::Inst()->loginRequest())
                {
                    scheduleUpdate();
                }
                break;
                
                //获得配置信息
            case enMainReqType_Cfg:
                if(CGameData::Inst()->cfgRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enMainReqType_Card:
                if(CGameData::Inst()->reqCard()){
                    scheduleUpdate();
                }
                break;
                
            case enMainReqType_Monster:
                if(CGameData::Inst()->reqMonster()){
                    scheduleUpdate();
                }
                break;
                
            case enMainReqType_Shop:
                if(CGameData::Inst()->getShopRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enMainReqType_RcvMail:
                if(CGameData::Inst()->rcvMailRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enMainReqType_Newbie4:
                reqNewbieGacha();
                break;
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
    }
}

//读取用户卡片纹理
void MainScene::loadingCardRes(ccTime dt)
{
    if (!CGameData::Inst()->loadUserCardsStep(10))
    {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MainScene::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 5000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }

    
    unschedule(schedule_selector(MainScene::loadingCardRes));
   
    m_pBottomPanel->showTips();
    showAwardTips();
    showNotice();
    
    
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
    
    m_pUsrInfoPanel->showData();
    showUsrInfPanel(true);
    showBottomPanel(true);
    showTips_gonggao(CGameData::Inst()->getUsrInfo(),true);
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(false);
    }
}

//******************************************************************************
// update
//******************************************************************************
void MainScene::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    //等待编队请求的回复
    if(m_iReqType == enMainReqType_Unknow){
        if(comState == kReqStatOk){
            unscheduleUpdate();
            m_iReqType = enMainReqType_Null;
            enterNxtLayer();
            return;
        }
        else {
            return;
        }
    }
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MainScene::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 5000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }

    unscheduleUpdate();

    if(comState == kReqStatOk){
        switch (m_iReqType)
        {
            case enMainReqType_FromSL:
                if(m_commDlg != NULL){
                    removeChild(m_commDlg, true);
                    m_commDlg = NULL;
                }
                enableBottomPanel(true);
                m_curLayer = m_pArenaLayer;
                m_curLayer->enter();
                break;
                
                //登录成功
            case enMainReqType_Login:
                m_iReqType = enMainReqType_Null;
                if(CGameData::Inst()->cfgRequest()){
                    scheduleUpdate();
                    m_iReqType = enMainReqType_Cfg;
                }
               
                break;

                //获得配置信息
            case enMainReqType_Cfg:
                m_iReqType = enMainReqType_Null;
                if(CGameData::Inst()->reqCard()){
                    scheduleUpdate();
                    m_iReqType = enMainReqType_Card;
                }
                
                CGameData::Inst()->setUpdateCom(true);
                break;
                
            case enMainReqType_Card:
                m_iReqType = enMainReqType_Null;
                if(CGameData::Inst()->reqMonster()){
                    scheduleUpdate();
                    m_iReqType = enMainReqType_Monster;
                }
                break;
                
            case enMainReqType_Monster:
                SaveData::Inst()->destorySavedData();
                OrgSaveData::Inst()->destorySavedData();
                
                m_iReqType = enMainReqType_Null;
                if(CGameData::Inst()->isNewbie()){
                    if(m_commDlg != NULL){
                        removeChild(m_commDlg, true);
                        m_commDlg = NULL;
                    }
                    newbieProc();
                }
                else{
                    //获得配置后，初始化编队、邮箱等
                    CGameData::Inst()->initAfterLogin();
                    
                    //注册推送
                    CCMutableArray<PushInfo *> *pushInfoArr= CGameData::Inst()->getPushInfo();
                    if(pushInfoArr->count()!=0)
                    {
                        PushInfo *pushInfo1 = pushInfoArr->getObjectAtIndex(0);
                        //  CCLog("pushtime %ld",pushInfo1->pushTime);
                        OcProxy::Inst()->setPushInfo(pushInfo1->msg, pushInfo1->pushTime, kPushInfDg);
                    }

                    if(CGameData::Inst()->getShopRequest()){
                        scheduleUpdate();
                        m_iReqType = enMainReqType_Shop;
                    }
                }
                break;
                
            case enMainReqType_Shop:
                m_iReqType = enMainReqType_Null;
                
                if(CGameData::Inst()->rcvMailRequest()){
                    scheduleUpdate();
                    m_iReqType = enMainReqType_RcvMail;
                }
                else
                {
                    finishLoginAllRequest();
                }
                break;
                
            case enMainReqType_RcvMail:
                m_iReqType = enMainReqType_Null;
                CGameData::Inst()->setRcvMailReqTime();
                MailBoxAgt::Inst()->updateMailBox();
                
                finishLoginAllRequest();
                
                break;
            case enMainReqType_ReadMzsm:
                m_iReqType = enMainReqType_Null;
                if(m_commDlg != NULL){
                    removeChild(m_commDlg, true);
                    m_commDlg = NULL;
                }
                isClaimNoticeShow = true;
                enableBottomPanel(false);
                m_notice->enterWithType(enNoticeTypeSystem);
                break;
                
            case enMainReqType_Newbie4:
                if(m_commDlg != NULL){
                    removeChild(m_commDlg, true);
                    m_commDlg = NULL;
                }
                newbieGachaEnd();
                break;
                
            default:
                break;
        }
        
        if(CGameData::Inst()->isNewbie())
            enableBottomPanel(false);
    }
    else {
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        if (comState == 403)
        {
            reloginWeibo(3);
            return;
        }
        
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MainScene::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 5000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            
            m_commDlg->setNoCancel();
        }
    }
}

//******************************************************************************
// finishLoginAllRequest
//******************************************************************************
void MainScene::finishLoginAllRequest()
{
    CGameData::Inst()->checkIsUseExternal();
    
    //设置tapjoyUserId
    setTapjoyUserId();
    
    // 设置需要初始化的数据
    InitRecordVideoAndRelated();
    
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

//    m_pBottomPanel->showTips();
//    showNotice();
//    
//    m_pUsrInfoPanel->showData();
//    showUsrInfPanel(true);
//    showBottomPanel(true);
    
//    if(m_commDlg != NULL){
//        removeChild(m_commDlg, true);
//        m_commDlg = NULL;
//        MainScene::Inst()->enableBottomPanel(false);
//    }
    
    //CGameData::Inst()->loadUserCardsSprite();
    CGameData::Inst()->releaseUserCardsSprite();
    schedule(schedule_selector(MainScene::loadingCardRes));
}

//******************************************************************************
// newbieProc
//******************************************************************************
void MainScene::newbieProc()
{
    switch (m_iNewbieStep) {
        case 0://选初始武将
            m_newbie = new NewbieLayer();
            addChild(m_newbie);
            m_newbie->release();
            
            m_newbie->show();
            break;
            
        case 1: //选将结束，显示欢迎对话框，获取配置信息
        case 2: //获取帮助列表
            newbieLogin();
            break;
            
        case 3: //新手战斗结束，去抽奖
            m_curLayer = m_pArenaLayer;
            m_pUsrInfoPanel->showData();
            showUsrInfPanel(true);
            showBottomPanel(true);
            newbieFightEnd();
            break;
            
        case 4: //编队
        case 5: //强化
            goToGeneral();
            m_pUsrInfoPanel->showData();
            showUsrInfPanel(true);
            showBottomPanel(true);
            enableBottomPanel(false);
            break;
            
        default:
            break;
    }
}



//******************************************************************************
// reLogin
//******************************************************************************
void MainScene::reLogin()
{
    if (m_curLayer){
        m_curLayer->exit(); 
    }
    
    CGameData::Inst()->clearSessionId();
    if(CGameData::Inst()->loginRequest())
    {
       // unschedule(schedule_selector(MainScene::updateReLogin));
        m_iReqType = enMainReqType_Login;
        scheduleUpdate();
    }
    
//    schedule(schedule_selector(MainScene::updateReLogin));
//    enableBottomPanel(false);
}

//******************************************************************************
// updateRelogin
//******************************************************************************
void MainScene::updateReLogin() {
    if(CGameData::Inst()->loginRequest())
    {
        unschedule(schedule_selector(MainScene::updateReLogin));
        m_iReqType = enMainReqType_Login;
        scheduleUpdate();
    }
}

//******************************************************************************
// showTips
//******************************************************************************
void MainScene::showTips()
{
    m_pBottomPanel->showTips();
}

void MainScene::showCntryTips(bool clearStatus){
    m_pBottomPanel->showCntryTips(clearStatus);
}

void MainScene::showAwardTips()
{
    m_pFirstLayer->showAwardTips();
}

//******************************************************************************
// dl2CallBack
//******************************************************************************
void MainScene::dl2CallBack(CCObject *pSender)
{
    enableBottomPanel(true);
    Dialog2Ret *ret = (Dialog2Ret *)pSender ;
    if(ret->bSel == 111)
    {
        if (m_curLayer != m_pGeneralLayer) {
            m_curLayer->exit();
            m_curLayer = m_pGeneralLayer;
            m_pGeneralLayer->enter();
        }
        m_pGeneralLayer->enterTeamInf();
        m_fullCostDlg->removeFromParentAndCleanup(true);
        m_fullCostDlg= NULL;
        
    }
    else if (ret->bSel == kShowFoodInTeam) {
        m_curLayer = m_pGeneralLayer;
        m_pGeneralLayer->enterTeamInf();
        m_foodInTeamDlg->removeFromParentAndCleanup(true);
        m_foodInTeamDlg = NULL;
    }
    else if (ret->bSel == kShowFoodUpThreeStarsOk) {
        m_curLayer = m_pGeneralLayer;
        m_pGeneralLayer->beginPwUp();
        m_foodUpThreeStarsDlg->removeFromParentAndCleanup(true);
        m_foodUpThreeStarsDlg = NULL;
    }
    else if (ret->bSel == kShowFoodUpThreeStarsCancel) {
        m_curLayer = m_pGeneralLayer;
        m_foodUpThreeStarsDlg->removeFromParentAndCleanup(true);
        m_foodUpThreeStarsDlg = NULL;
    }
    else
    {

        if(ret->bSel == 11)
        {
            if (m_curLayer==m_pArenaLayer)
            {
                 m_pArenaLayer->exit();
            }
            m_curLayer = m_pShopMain;
            m_pShopMain->enter();
            m_pShopMain->beginBoxEnlarge ();
           
        }else if(ret->bSel == 12){
            m_curLayer = m_pGeneralLayer;
            m_pGeneralLayer->enter();
            m_pGeneralLayer->enterPwUpSel();

        }else if(ret->bSel == 13){
            m_curLayer = m_pGeneralLayer;
            m_pGeneralLayer->enter();
            m_pGeneralLayer->enterCardSell();

        }
        
        m_fullPackDlg->removeFromParentAndCleanup(true);
        m_fullPackDlg= NULL;
    }
}

//******************************************************************************
// showFullPackDlg
//******************************************************************************
void MainScene::showFullPackDlg(bool bRemoveLayer)
{
    if (bRemoveLayer)
        m_curLayer->exit();
    
    enableBottomPanel(false);
    if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
    {
        if(m_fullPackDlg == NULL)        
        {
            char buf[100];
           
            sprintf(buf, "%s%d%s", CGameData::Inst()->getLanguageValue("mainSceneTip9"),
                    CGameData::Inst()->getUsrInfo()->max_card_num,
                    CGameData::Inst()->getLanguageValue("mainSceneTip10"));
            m_fullPackDlg = new Dialog2(this,
                                        callfuncO_selector(MainScene::dl2CallBack),
                                        CGameData::Inst()->getLanguageValue("mainSceneTip11"),
                                        buf,
                                        enDl2ThreeBtn);
            m_fullPackDlg->setPosition(CCPointMake(320, 480));
            addChild(m_fullPackDlg);
            m_fullPackDlg->release();
        }
        m_fullPackDlg->setScale(0);
        m_fullPackDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
}

//******************************************************************************
// showFullCostDlg
//******************************************************************************
void MainScene::showFullCostDlg()
{
    enableBottomPanel(false);

    if(m_fullCostDlg == NULL)        
    {
        char buf[100];
        sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip12"));
        m_fullCostDlg = new Dialog2(this, callfuncO_selector(MainScene::dl2CallBack),
                                    CGameData::Inst()->getLanguageValue("mainSceneTip13"),
                                    buf, enDl2FullCostBtn);
        m_fullCostDlg->setPosition(CCPointMake(320, 480));
        addChild(m_fullCostDlg);
        m_fullCostDlg->release();
    }
    m_fullCostDlg->setScale(0);
    m_fullCostDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// showTeamBugDlg
//******************************************************************************
void MainScene::showTeamBugDlg()
{
    enableBottomPanel(false);
    
    if(m_fullCostDlg == NULL)
    {
        char buf[100];
        sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip18"));
        m_fullCostDlg = new Dialog2(this, callfuncO_selector(MainScene::dl2CallBack), CGameData::Inst()->getLanguageValue("mainSceneTip17"), buf, enDl2FullCostBtn);
        m_fullCostDlg->setPosition(CCPointMake(320, 480));
        addChild(m_fullCostDlg);
        m_fullCostDlg->release();
    }
    m_fullCostDlg->setScale(0);
    m_fullCostDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// showFoodInTeam
//******************************************************************************
void MainScene::showFoodInTeam() {
    enableBottomPanel(false);
    if (m_foodInTeamDlg == NULL) {
        char buf[100];
        sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip14"));
        m_foodInTeamDlg = new Dialog2(this,
                                      callfuncO_selector(MainScene::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("mainSceneTip15"),
                                      buf, enDl2FoodInTeam);
        m_foodInTeamDlg->setPosition(CCPointMake(320, 480));
        addChild(m_foodInTeamDlg);
        m_foodInTeamDlg->release();
    }
    m_foodInTeamDlg->setScale(0.0f);
    m_foodInTeamDlg->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
}


//******************************************************************************
// goToGeneral
//******************************************************************************
void MainScene::goToGeneral()
{
    if(m_curLayer != m_pGeneralLayer){
        if(m_curLayer != NULL)
            m_curLayer->exit();
        
        m_curLayer = m_pGeneralLayer;
        m_curLayer->enter();
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
}

void MainScene::showFoodUpThreeStars() {
    enableBottomPanel(false);
    if (m_foodUpThreeStarsDlg == NULL) {
        char buf[100];
        sprintf(buf, "%s",CGameData::Inst()->getLanguageValue("mainSceneTip16"));
        m_foodUpThreeStarsDlg = new Dialog2(this, callfuncO_selector(MainScene::dl2CallBack),CGameData::Inst()->getLanguageValue("alert"), buf, enDl2FoodUpThreeStars);
        m_foodUpThreeStarsDlg->setPosition(CCPointMake(320, 480));
        addChild(m_foodUpThreeStarsDlg);
        m_foodUpThreeStarsDlg->release();
    }
    m_foodUpThreeStarsDlg->setScale(0.0f);
    m_foodUpThreeStarsDlg->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
}

void MainScene::showWeiboPublishInfoWithType(int type)
{
    m_publishWeiboLayer->setScale(1);
    m_publishWeiboLayer->showPublishViewWithType((enWeiboType)type);
}

void MainScene::cbPublishWeibo(PublishWeiboLayerRet *ret)
{
    if(ret->iSel == 11 || ret->iSel == 13)
    {
        m_publishWeiboLayer->setScale(0);
        m_publishWeiboLayer->removePublishView();
        
    }else {
        
    }
}

void MainScene::setGenRangeBtnIsVisble(bool isShow)
{
    if (m_curLayer == m_pGeneralLayer) {
        m_pGeneralLayer->setRangeBtnIsVisble(isShow);
    }
}

void MainScene::setGachaInfoHide()
{
    m_pGachaMain->hideGachaInfo();
}

bool  MainScene::getIsJustLeave()
{
    return m_pArenaLayer->getIsJustLeaveFight();
}

void MainScene::setIsJustLeave(bool agu)
{
    return m_pArenaLayer->setIsJustLeaveFight(agu);
}

void MainScene::showTouchTip()
{
    if (m_spTouchTip == NULL) {
        m_spTouchTip = CCSprite::spriteWithFile("click_to_continue.jpg");
        m_spTouchTip->setPosition(CCPointMake(320, 65));
        addChild(m_spTouchTip, 500);
        m_spTouchTip->setOpacity(0);
        
        m_spTouchTip->runAction(CCFadeIn::actionWithDuration(0.5));
    }
}

void MainScene::removeTouchTip()
{
    if (m_spTouchTip) {
        removeChild(m_spTouchTip, true);
        m_spTouchTip = NULL;
    }
}

void MainScene::reloginWeibo(int type)
{
    m_reloginWeiboType = type;
    
    m_pBottomPanel->setPosition(CCPointMake(0, -157));
    m_pUsrInfoPanel->setPosition(CCPointMake(0, 170));
    
    if (m_pLoginLayer == NULL) {
        m_pLoginLayer = new CLoginLayer();
        addChild(m_pLoginLayer);
        m_pLoginLayer->release();
    }
    m_curLayer = m_pLoginLayer;
    m_curLayer->enter();
}

void MainScene::isRelogin(){
    m_reloginDlg = new Dialog2(this, callfuncO_selector(MainScene::cbDl2Relogin), CGameData::Inst()->getLanguageValue("alert"), CGameData::Inst()->getLanguageValue("otherMainTip9"), enDl2TwoBtn);
    m_reloginDlg->setPosition(CCPointMake(320, 480));
    addChild(m_reloginDlg);
    m_reloginDlg->release();
    m_reloginDlg->setScale(0.0f);
    m_reloginDlg->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
}

void MainScene::cbDl2Relogin(CCObject* pObject)
{
    Dialog2Ret *ret = (Dialog2Ret *)pObject;
    if (ret -> bOk == true)
    {
        MainScene::Inst()->reLogin();
        if (m_reloginDlg) {
            m_reloginDlg->removeFromParentAndCleanup(true);
            m_reloginDlg = NULL;
        }
    }
    else{
        if (m_reloginDlg) {
            m_reloginDlg->removeFromParentAndCleanup(true);
            m_reloginDlg = NULL;
        }
    }
}

void MainScene::switchAccount()
{
    OcProxy::Inst()->clearOauthData();
    CGameData::Inst()->clearUserDefaultData();
    m_pBottomPanel->clearTips();
    m_pBottomPanel->setPosition(CCPointMake(0, -157));
    m_pUsrInfoPanel->setPosition(CCPointMake(0, 170));

    isGfNoticeShow = false;
    isIVNoticeShow = false;
    isLINoticeShow = false;
    isClaimNoticeShow = true;
    
    if (m_curLayer == m_pOtherMain) {
        m_pOtherMain->exit();
    }
    
    if (m_pLoginLayer == NULL) {
        m_pLoginLayer = new CLoginLayer();
        addChild(m_pLoginLayer);
        m_pLoginLayer->release();
    }
    m_curLayer = m_pLoginLayer;
    m_curLayer->enter();
    
    //重置列表位置等
    m_pArenaLayer->reset();
    m_pGeneralLayer->reset();
}

//******************************************************************************
// showSnow
//******************************************************************************
void MainScene::showSnow()
{
    if(CGameData::Inst()->getIsUseChristmasUI() != true)
        return;
    
    CCNode* node = getChildByTag(kSnowEmmiterTag);
    if(node)
        return;

    CCSize winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    int large = 1;
    if((winSize.width == 768) || (winSize.width == 320)){
        large = 2;
    }

    CCParticleSnow* emitter = CCParticleSnow::node();
    emitter->retain();
	addChild(emitter, 100000, kSnowEmmiterTag);
	
	emitter->setPosition( CCPointMake(320, 960) );
    emitter->setPosVar(CCPointMake(320 * large, 0));
	emitter->setLife(4);
	emitter->setLifeVar(1);
	
	// gravity
	emitter->setGravity(CCPointMake(0,-10 * large));
    
	// speed of particles
	emitter->setSpeed(80 * large);
	emitter->setSpeedVar(20 * large);
	
    // size of particles
    emitter->setStartSize(10.0f * large);
    emitter->setStartSizeVar(5.0f * large);
    emitter->setEndSize(kParticleStartSizeEqualToEndSize);
	
	ccColor4F startColor = emitter->getStartColor();
	startColor.r = 0.9f;
	startColor.g = 0.9f;
	startColor.b = 0.9f;
	emitter->setStartColor(startColor);
	
	ccColor4F startColorVar = emitter->getStartColorVar();
	startColorVar.b = 0.1f;
	emitter->setStartColorVar(startColorVar);
	
	emitter->setEmissionRate(emitter->getTotalParticles()/emitter->getLife());
	
	emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_snow.png") );
}

//******************************************************************************
// stopSnow
//******************************************************************************
void MainScene::stopSnow()
{
    CCNode* node = getChildByTag(kSnowEmmiterTag);
    if(node){
        removeChild(node, true);
    }
}

//******************************************************************************
// showCntryInfo
//******************************************************************************
void MainScene::updateCntryNameCard()
{
    m_pUsrInfoPanel->updateCntryNameCard();
}

//******************************************************************************
// showCntryInfo
//******************************************************************************
void MainScene::showCntryInfo()
{
    if (!m_pUsrInfoPanel->getIsShowCntryInfo()) {
        m_pUsrInfoPanel->cleanCurrentInfo();
        m_pUsrInfoPanel->showCntryInfo();
    }
}

//******************************************************************************
// showUserInfo
//******************************************************************************
void MainScene::showUserInfo()
{
    if (m_pUsrInfoPanel->getIsShowCntryInfo()) {
        m_pUsrInfoPanel->cleanCurrentInfo();
    }
}

//******************************************************************************
// showNewFinishTitle
//******************************************************************************
void MainScene::showNewFinishTitle(CCObject* obj)
{
    CCSprite* spTitleBg = (CCSprite*)getChildByTag(kDlgNewTitleTag);
    if (spTitleBg != NULL)
        return;
    
    spTitleBg = CCSprite::spriteWithFile("titledspbg1.png");
    if (spTitleBg == NULL)
        return;
    
    float fHeight = spTitleBg->getContentSize().height;
    
    CCSprite* spTitleBg2 = CCSprite::spriteWithFile("titledspbg2.png");
    if(spTitleBg2){
        spTitleBg->addChild(spTitleBg2);
        spTitleBg2->setAnchorPoint(ccp(0, 1));
        spTitleBg2->setPosition(ccp(0,1));
        
        fHeight += spTitleBg2->getContentSize().height;
        
        CCSprite* spTitleBg3 = CCSprite::spriteWithFile("titledspbg3.png");
        if(spTitleBg3){
            spTitleBg->addChild(spTitleBg3);
            spTitleBg3->setAnchorPoint(ccp(0, 1));
            spTitleBg3->setPosition(ccp(0,2-spTitleBg2->getContentSizeInPixels().height));
            
            fHeight += spTitleBg3->getContentSize().height;
        }
    }

    
    addChild(spTitleBg, 100, kDlgNewTitleTag);
    
    
    CCSprite* spCup = CCSprite::spriteWithFile("title_cup.png");
    if (spCup) {
        spCup->setPosition(CCPointMake(120,-35));
        spTitleBg->addChild(spCup);
    }
    
    TextNode* tnDsp = TextNode::textWithString(CGameData::Inst()->getLanguageValue("title_newtitle"), 28);
    tnDsp->setColor(ccYELLOW);
    tnDsp->setShadowColor(ccBLACK);
    tnDsp->setPosition(CCPointMake(330,-11));
    spTitleBg->addChild(tnDsp);
    
    TextNode* titlename = TextNode::textWithString(CGameData::Inst()->getUsrInfo()->new_title_name.c_str(), 36);
    titlename->setColor(ccc3(68, 181, 6));
    titlename->setShadowColor(ccBLACK);
    titlename->setPosition(CCPointMake(330,-60));
    spTitleBg->addChild(titlename);
    
    spTitleBg->setAnchorPoint(ccp(0.5, 1));
    spTitleBg->setPosition(CCPointMake(320,
                                       960 + fHeight));
    
    CCMoveBy* moveIn = CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -fHeight));
    CCMoveBy* moveOut = CCMoveBy::actionWithDuration(0.5, CCPointMake(0, fHeight));
    CCCallFunc* cbStars = CCCallFunc::actionWithTarget(this,
                                                       callfunc_selector(MainScene::showStars));
    CCCallFunc* cbRemove = CCCallFunc::actionWithTarget(this,
                                                        callfunc_selector(MainScene::cbNewFinishTitle));
    
    spTitleBg->runAction(CCSequence::actions(moveIn,
                                             cbStars,
                                             CCDelayTime::actionWithDuration(5.0),
                                             moveOut,
                                             cbRemove,
                                             NULL));
    
}

//******************************************************************************
// cbNewFinishTitle
//******************************************************************************
void MainScene::cbNewFinishTitle()
{    
    CCSprite* spTitleBg = (CCSprite*)getChildByTag(kDlgNewTitleTag);
    if (spTitleBg)
    {
        spTitleBg->removeFromParentAndCleanup(true);
        spTitleBg = NULL;
    }
}

//******************************************************************************
// showStars
//******************************************************************************
void MainScene::showStars()
{
    CCSprite* spTitleBg = (CCSprite*)getChildByTag(kDlgNewTitleTag);
    if (spTitleBg == NULL)
        return;

    CCSprite* spStar;
    for (int i = 0; i < 20; i++) {
        spStar = CCSprite::spriteWithFile("ma_star.png");
        if(spStar){
            spTitleBg->addChild(spStar);
            spStar->setPosition(CCPointMake(spTitleBg->getContentSize().width * CCRANDOM_0_1(),
                                            167 * CCRANDOM_0_1() - 125));
            spStar->setScale(1.0 + CCRANDOM_0_1());
            spStar->setOpacity(0.0);
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(1.0 * CCRANDOM_0_1()),
                                                                               CCFadeIn::actionWithDuration(1.0),
                                                                               CCFadeOut::actionWithDuration(0.7),
                                                                               NULL);
            spStar->runAction(CCRepeatForever::actionWithAction(action));
        }
    }
}

//******************************************************************************
// init record video related
//******************************************************************************
void MainScene::InitRecordVideoAndRelated()
{
    // 切换帐号的时候 需要init的相关数据
    
    // oc部分 把视频的地址变化 
    RecordHelper::Inst()->InitNewGameUid();
    
}

//******************************************************************************
// showOnlyNotice
//******************************************************************************
void MainScene::showOnlyNotice()
{
    m_curLayer->exit();
    
    m_notice = new CNotice(this,callfuncO_selector(MainScene::cbShowOnlyNotice));
    addChild(m_notice,2000);
    m_notice->release();
    m_notice->setAnchorPoint(ccp(0.5, 0.5));
    m_notice->setPosition(ccp(0, 0));
    
    enableBottomPanel(false);
    m_notice->enterWithType(enNoticeTypeSystem);
}

//******************************************************************************
// cbShowOnlyNotice
//******************************************************************************
void MainScene::cbShowOnlyNotice(cocos2d::CCObject *pObj)
{
    OcProxy::Inst()->removeWebview();
    removeChild(m_notice, true);
    m_notice = NULL;
    enableBottomPanel(true);
    
    m_curLayer->enter();
}

//******************************************************************************
// enterNormalArenal
//******************************************************************************
void MainScene::enterNormalArenal()
{
    m_nxtLayer = m_pArenaLayer;
    
    m_curLayer->exit();
    
    enterNxtLayer();
}

//******************************************************************************
// enterSpecialArenal
//******************************************************************************
void MainScene::enterSpecialArenal()
{
    m_nxtLayer = m_pArenaLayer;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    

    if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
        return;
    
    m_pArenaLayer->enterSpecialArena();
}

//******************************************************************************
// enterSpecialArenal
//******************************************************************************
void MainScene::enterCntryMain()
{
    m_nxtLayer = m_pCountryMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
}

//******************************************************************************
// enterOtherMain
//******************************************************************************
void MainScene::enterOtherMain()
{
    m_nxtLayer = m_pOtherMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
}

//******************************************************************************
// enterGachaMain
//******************************************************************************
void MainScene::enterGachaMain()
{
    m_nxtLayer = m_pGachaMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
}

//******************************************************************************
// enterMailList
//******************************************************************************
void MainScene::enterMailList()
{
    m_nxtLayer = m_pFriendMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    
    m_pFriendMain->enterMailListFromFirst();
}

void MainScene::eneterReward()
{
    m_nxtLayer = m_pRewardMain;
    m_curLayer->exit();
    enterNxtLayer();
}

//******************************************************************************
// enterBag
//******************************************************************************
void MainScene::enterBag()
{
    m_nxtLayer = m_pGeneralLayer;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    
    m_pGeneralLayer->enterSuiShenJunDai();
}
//******************************************************************************
// enterParcel
//******************************************************************************
void MainScene::cbParCelLayer(CCObject *pSender)
{
    ParcelSelRet* ret = (ParcelSelRet*)pSender;
    
//    CCLog("%d" , ret->iBtnSel);
    
    m_pParcelBackPage->setPosition(CCPointMake(-10000, -10000));
    
    float yy = m_pParcelPageOne->getPosition().y;
    
    m_pParcelPageOne->setPosition(CCPointMake(-10000, yy));


    if(m_pParcelPageTwo == NULL)
    {
        m_pParcelPageTwo = new ParcelPageTwo(this,callfuncO_selector(MainScene::cbTwoBack));
        addChild(m_pParcelPageTwo);
        
    }

    m_pParcelPageTwo->m_pType = ret->iType;
    m_pParcelPageTwo->m_pNum = ret->iBtnSel;
    
    
    m_pParcelPageTwo->enter();
    m_pParcelPageTwo->setPosition(CCPoint(640, 0));
    CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0));
    m_pParcelPageTwo->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    
    
}
void MainScene::cbTwoBack(CCObject *pSender)
{
    m_pParcelPageTwo->exit();
    
    m_pParcelBackPage->setPosition(CCPointMake(0, 0));
    
    float yy = m_pParcelPageOne->getPosition().y;
    
    m_pParcelPageOne->setPosition(CCPointMake(0, yy));
    
}

void MainScene::enterParcel()        // 碎片合成
{
    
    if(m_pParcelBackPage == NULL)
    {
        m_pParcelBackPage = new ParcelBackPage();
        
        addChild(m_pParcelBackPage);

    }
    m_pParcelBackPage->setPosition(CCPointMake(0, 0));

    m_pParcelBackPage->enter();
    
    

    if(m_pParcelPageOne == NULL)
    {
        m_pParcelPageOne = new ParcelPageOne(this,callfuncO_selector(MainScene::cbParCelLayer));
        addChild(m_pParcelPageOne);

    }

    m_pParcelPageOne->setPosition(CCPointMake(0, 0));

    
    
    
    
    
    m_nxtLayer = m_pParcelPageOne;
    m_curLayer->exit();
    
    OcProxy::Inst()->showUsedMemory();
    
    if(CGameData::Inst()->getReqStat() == kReqStatOk){
        enterNxtLayer();
        
        
    }
    else {
        if(m_iReqType == enMainReqType_Null){
            m_iReqType = enMainReqType_Unknow;
            scheduleUpdate();
        }
    }
    
    

}

//******************************************************************************
// enterMyVideo
//******************************************************************************
void MainScene::enterMyVideo()
{
    m_nxtLayer = m_pOtherMain;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    
    m_pOtherMain->enterMyVideo();
}

//******************************************************************************
// enterTeamInfo
//******************************************************************************
void MainScene::enterTeamInfo()
{
    m_nxtLayer = m_pGeneralLayer;
    
    m_curLayer->exit();
    
    enterNxtLayer();
    
    m_pGeneralLayer->setIsFromHome(true);
    m_pGeneralLayer->enterTeamInf();
}

CCLayer* g_pBaseLayerDialog = NULL;
bool MainScene::onCurTeamCostCall(CCLayer *pBaseLayer)
{
    if (pBaseLayer)
    {
        if (CGameData::Inst()->getCurCostSum() > CGameData::Inst()->getMaxCostSum())
        {
            if (CCNode *pNode = pBaseLayer->getChildByTag(UI_TAB_DIGLOG))
            {
                pNode->setScale(0.0);
                pNode->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            }
            else
            {
                if (CCNode *pNode= new Dialog1(this, callfuncO_selector(MainScene::okDiglog),
                                               CGameData::Inst()->getLanguageValue("team_cost_upper_limit"),false))
                {
                    pBaseLayer->addChild(pNode,1000,UI_TAB_DIGLOG);
                    pNode->release();
                    pNode->setPosition(CCPointMake(320, 480 - getPosition().y));
                    
                    pNode->setScale(0.0);
                    pNode->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                }
            }
            g_pBaseLayerDialog = pBaseLayer;
            return true;
        }
    }

    return false;
}

void MainScene::okDiglog()
{
    if (g_pBaseLayerDialog)
    {
        if (CCNode *pNode = g_pBaseLayerDialog->getChildByTag(UI_TAB_DIGLOG))
        {
            g_pBaseLayerDialog->removeChild(pNode,true);
        }
        g_pBaseLayerDialog = NULL;
    }
}


CCLayer* g_pArenaLimitMaxStarDialog = NULL;
bool MainScene::onCurArenaLimitMaxStarCall(CCLayer *pBaseLayer)
{
    if (pBaseLayer)
    {
        unsigned short unArenaLimitMaxStar = m_pArenaLayer->getArenaLimitMaxStar();
        if (0==unArenaLimitMaxStar)return false;
        unsigned short ushTeamMaxStar = CGameData::Inst()->getTeamMaxStar();
        unsigned short ushFriendMaxStar = CGameData::Inst()->getFriendMaxStar();
        if (m_curLayer==m_pArenaLayer &&
            0!=unArenaLimitMaxStar &&
            unArenaLimitMaxStar<MAX(ushTeamMaxStar,ushFriendMaxStar))
        {
            if (CCNode *pNode = pBaseLayer->getChildByTag(UI_TAB_DIGLOG))
            {
                pNode->setScale(0.0);
                pNode->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            }
            else
            {
                std::string szText;
                if (unArenaLimitMaxStar<ushTeamMaxStar || ushTeamMaxStar>=ushFriendMaxStar)
                {
                    szText = CGameData::Inst()->getLanguageValue("team_star_max_limit");
                }
                else
                {
                    szText = CGameData::Inst()->getLanguageValue("friend_star_max_limit");
                }
                if (std::string::npos!=szText.find("%d"))
                {
                    char chBuf[128];
                    snprintf(chBuf, sizeof(chBuf),szText.c_str(),m_pArenaLayer->getArenaLimitMaxStar());
                    szText = chBuf;
                }
                
                if (CCNode *pNode= new Dialog1(this, callfuncO_selector(MainScene::okArenaDiglog),
                                               szText.c_str(),false))
                {
                    pBaseLayer->addChild(pNode,1000,UI_TAB_DIGLOG);
                    pNode->release();
                    pNode->setPosition(CCPointMake(320, 480 - getPosition().y));
                    
                    pNode->setScale(0.0);
                    pNode->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                }
            }
            g_pArenaLimitMaxStarDialog = pBaseLayer;
            return true;
        }
    }
    return false;
}

void MainScene::okArenaDiglog()
{
    if (g_pArenaLimitMaxStarDialog)
    {
        if (CCNode *pNode = g_pArenaLimitMaxStarDialog->getChildByTag(UI_TAB_DIGLOG))
        {
            g_pArenaLimitMaxStarDialog->removeChild(pNode,true);
        }
        g_pArenaLimitMaxStarDialog = NULL;
    }
}

unsigned short MainScene::getArenaLimitMaxStar()const
{
    //if (CArenaLayer *pArenaLayer = dynamic_cast<CArenaLayer*>(m_curLayer))
    if (m_curLayer==m_pArenaLayer)
    {
        return m_pArenaLayer->getArenaLimitMaxStar();
    }
    return 0;
}

bool MainScene::isExtendPackShow()
{
    return m_fullPackDlg;
}

void MainScene::setLabel(int label){
    m_pRewardMain->setLabel(label);
}

void MainScene::forceExitFight()
{
    if (m_pArenaLayer) {
        m_pArenaLayer->forceExitFight();
    }
    
    if (m_pCountryMain) {
        m_pCountryMain->forceExitFight();
    }
    
    CGameData::Inst()->setIsFighting(false);
    MainScene::Inst()->switchBgMusic(enBgMusicNml);
}

void MainScene::waitPPLogout(ccTime dt)
{
    int status = OcProxy::Inst()->getLogoutStatus();
    if(status == 2)
    {
        MainScene::Inst()->switchAccount();
        OcProxy::Inst()->resetLogoutStatus();
    }
    
}


