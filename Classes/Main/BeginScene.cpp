

#include "BeginScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "TextNode.h"
#include "CommDlg.h"
#include "CGamedata.h"
#include "OcProxy.h"
#include "HttpComm.h"
#include "SaveData.h"
#include "OrgSaveData.h"
#include "Dialog2.h"
#include "Dialog1.h"
#include "CommonDef.h"

using namespace CocosDenshion;

#define LoadingIndexMax 51

CCScene* BeginScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	BeginScene *layer = BeginScene::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BeginScene::init()
{
    m_iLoadIdx = 0;
    
    srand(time(0));
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

    m_updateLanguageBagState = enUpdateLanguageBagNull;
    m_commDlg = NULL;
    m_bChkInfPass = false;
    m_dlgSaveData = NULL;
    m_getFantiAlert1 = NULL;//返体
    m_isFanti = false;
  
    CGameData::Inst()->getMD5Str();
    
	return true;
}

bool BeginScene::chkInfo()
{
    if (SaveData::Inst()->hasValidSavedData()
        || OrgSaveData::Inst()->hasValidSavedData())
        return true;
    
    bool bRet = false;
    
    HttpComm::Inst()->reqInfo();
    
    int iReqStat = HttpComm::Inst()->getReqStatus();
    
    if(iReqStat == 0){
#if defined(kPltfmPp)
        bRet = true; //pp助手不用检测更新
#else
        float localVer = atof(kLocalAppVer);
        localVer += 0.0001f;
        float serverVer = HttpComm::Inst()->getServVer();
        
        if(localVer < serverVer){
            OcProxy::Inst()->goAppStoreUpdate();
        }
        else{
            bRet = true;
        }
#endif
    }
    else{
        if (!m_commDlg) {
            m_commDlg = new CommDlg(this, callfuncO_selector(BeginScene::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
    
    return bRet;
}

void BeginScene::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_isFanti) {
        
    }else{
    setIsTouchEnabled(false);
    removeChild(m_lbText, true);
    
    m_bChkInfPass = chkInfo();
    
    if(m_bChkInfPass){
        startToLoadResAndLang();
    }
  }
}

void BeginScene::startToLoadResAndLang()
{
    m_prgrsBar->setPercentage(0);
    m_prgrsBar->setIsVisible(true);
    m_spPrgrsFr->setIsVisible(true);
    
    if (CGameData::Inst()->isUpdateLanguageBag()) {
        updateLanguageBag();
    }
    else {
        if (!CGameData::Inst()->loadLanguageBag()) {
            updateLanguageBag();
        }
    }
    
    scheduleUpdate();
}

void BeginScene::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if (m_isFanti) {
        if (comState == kReqStatRequesting){
            if(m_commDlg == NULL)
            {
                m_commDlg = new CommDlg(this, callfuncO_selector(BeginScene::cbCommDlg), enCommDlgTp_connecting);
                addChild(m_commDlg, 1000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
            }
             return;
        }
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
      }

         unscheduleUpdate();
        if (comState == kReqStatOk) {
            
            if (CGameData::Inst()->getErrMsg() == NULL){
                SaveData::Inst()->destorySavedData();
                SaveData::Inst()->destorySavedFile();
                m_isFanti = false;
                showClickLabel();
                 return;
            }
  
            if (m_getFantiAlert1 == NULL)
            {
                m_getFantiAlert1 = new Dialog1(this,
                                            callfuncO_selector(BeginScene::cbGetNFantiAlert),
                                            CGameData::Inst()->getErrMsg(),false);
                m_getFantiAlert1->setPosition(CCPointMake(320, 480));
                addChild(m_getFantiAlert1);
                m_getFantiAlert1->release();
                m_getFantiAlert1->setScale(0.0);
                m_getFantiAlert1->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));

            }
          
         } else{
            m_commDlg = new CommDlg(this, callfuncO_selector(BeginScene::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        
        return;
    }
    
    if (m_updateLanguageBagState != enUpdateLanguageBagOver
        && m_iLoadIdx < LoadingIndexMax - 1) {
        // 进度条动画过程中，如果没有获得语言包
        if (comState == kReqStatOk) {
            m_updateLanguageBagState = enUpdateLanguageBagOver;
        }
        else if (comState == kReqStatRequesting) {
            
        }
        else {
            updateLanguageBag();
        }
    }
    
    m_iLoadIdx++;
    
    if (m_iLoadIdx <= LoadingIndexMax) {
        loadResource(m_iLoadIdx);
        m_prgrsBar->setPercentage(m_iLoadIdx * 100.0 / LoadingIndexMax);
    }
    
    if (m_iLoadIdx >= LoadingIndexMax) {
        if(m_updateLanguageBagState == enUpdateLanguageBagOver
           || comState == kReqStatOk) {
            unscheduleUpdate();
            goToMainScene();
        }
        else {
            if(comState == kReqStatRequesting){
                showConnectingDlg();
            }
            else {
                unscheduleUpdate();
                showRetryDlg();
            }
        }
    }
}


void BeginScene::cbGetNFantiAlert()//返体
{
    SaveData::Inst()->destorySavedData();
    SaveData::Inst()->destorySavedFile();
    m_isFanti = false;
    if (m_getFantiAlert1) {
        m_getFantiAlert1->removeFromParentAndCleanup(true);
        m_getFantiAlert1 = NULL;
    }
  
    showClickLabel();
}

void BeginScene::showConnectingDlg() {
    if (!m_commDlg) {
        m_commDlg = new CommDlg(this, callfuncO_selector(BeginScene::cbCommDlg), enCommDlgTp_connecting);
        addChild(m_commDlg, 1000);
        m_commDlg->setPosition(CCPointMake(320, 480));
        m_commDlg->release();
    }
}

void BeginScene::showRetryDlg() {
    if (m_commDlg) {
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    m_commDlg = new CommDlg(this, callfuncO_selector(BeginScene::cbCommDlg), enCommDlgTp_retry);
    addChild(m_commDlg, 1000);
    m_commDlg->setPosition(CCPointMake(320, 480));
    m_commDlg->release();
    m_commDlg->setScale(0.0);
    m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void BeginScene::cbCommDlg() {
    if (m_commDlg) {
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    if (m_isFanti) {
        int    m_iFloorId = SaveData::Inst()->getFloorId();
        int   m_iRoomId = SaveData::Inst()->getRoomId();
        const char*  m_szDgType = SaveData::Inst()->getDgType();
        char m_dungeonUid[32];
        snprintf(m_dungeonUid, 31, "%s", SaveData::Inst()->getDungeonUid());
        
        char floorid[10];
        sprintf(floorid, "%d", m_iFloorId);
        char roomid[10];
        sprintf(roomid, "%d", m_iRoomId);
        if(CGameData::Inst()->dgEndForFanTiRequest(m_szDgType, floorid, roomid, m_dungeonUid))
        {
           // m_isFanti = true;
            scheduleUpdate();
        }
        return;
    }
    if(m_bChkInfPass){
        if(m_updateLanguageBagState == enUpdateLanguageBagNull){
            startToLoadResAndLang();
        }
        else if(m_updateLanguageBagState == enUpdateLanguageBagStart){
            updateLanguageBag();
            scheduleUpdate();
        }
    }
    else{
        m_bChkInfPass = chkInfo();
        
        if(m_bChkInfPass){
            startToLoadResAndLang();
        }
    }
}

void BeginScene::goToMainScene() {
    CCScene *scene = CCScene::node();
    scene->addChild(MainScene::Inst());
    CCDirector::sharedDirector()->replaceScene(scene);
}

void BeginScene::loadResource(int idx)
{
    //load sound
    char buf[100];
    
    switch (idx) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            sprintf(buf, "combo%d%s", idx, kSeSuffix);
            SimpleAudioEngine::sharedEngine()->preloadEffect(buf);
            break;
            
        case 15:
            SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("battle.mp3");
            break;
            
        case 16:
            SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("bossbattle.mp3");
            break;
            
        case 17:
            SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("normal_newyear.mp3");
            break;
            
        case 18:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(water));
            break;
            
        case 19:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(fire));
            break;
            
        case 20:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(wood));
            break;
            
        case 21:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(light));
            break;
            
        case 22:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(dark));
            break;
            
        case 23:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(stonemv));
            break;
            
        case 24:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(skillavail));
            break;
            
        case 25:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(jumpnum));
            break;
            
        case 26:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(hpup));
            break;
            
        case 27:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(hpdown));
            break;
            
        case 28:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(useskill));
            break;
            
        case 29:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(stonechg));
            break;
            
        case 30:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(openbox));
            break;
            
        case 31:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(boxdrop));
            break;
            
        case 32:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(btnback));
            break;
            
        case 33:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(btnok));
            break;
            
        case 34:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(fail));
            break;
            
        case 35:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(success));
            break;
            
        case 36:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(timeout));
            break;
            
        case 37:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(upgrade));
            break;
            
        case 38:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(addExp));
            break;
            
        case 39:
            SimpleAudioEngine::sharedEngine()->preloadEffect(kSndEffect(clock));
            
            SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
            SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0);
            break;
            
        case 40:
            break;
            
        case 41:
            break;
            
        case 42:
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fs2.plist");
            break;
            
        case 43:
            
            break;
            
        case 44:
//            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("floor.plist");
            break;
            
        case 45:
//            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("title.plist");
            break;
            
        case 46:
            if (CGameData::Inst()->getIsUseChristmasUI())
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_2_2.plist");
            else
                CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_2_1.plist");
            break;
            
        case 47:
            break;
            
        case 48:
            break;
            
        case 49:
            break;
            
        case 50:
            break;
            
        case 51:
            break;
            
        default:
            break;
    }
}


void BeginScene::updateLanguageBag() {
    CGameData::Inst()->requestALanguageBag();
    m_updateLanguageBagState = enUpdateLanguageBagStart;
}

void BeginScene::onEnter() {
    CCLayer::onEnter();
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
    
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    if(winsz.width > 1000){
        this->setPosition(CCPointMake(128, 64));
        this->setAnchorPoint(CCPointZero);
        this->setScale(2.0);
    }
    
    else if(winsz.width == 768){
        this->setPosition(CCPointMake(64, 32));
    }
    else if(winsz.width == 320){
        this->setAnchorPoint(CCPointZero);
        this->setScale(0.5);
    }
    
    CCSprite* beginpic = CCSprite::spriteWithFile("beginpic.jpg");
    beginpic->setPositionInPixels(CCPointMake(320, 480));
    addChild(beginpic);
    
    CCPoint pt(320, 30);
        
    m_spPrgrsFr = CCSprite::spriteWithFile("loading_fr.png");
    pt.x -= (m_spPrgrsFr->getContentSize().width * 0.5);
    addChild(m_spPrgrsFr, 1);
    m_spPrgrsFr->setIsVisible(false);
    m_spPrgrsFr->setAnchorPoint(CCPointZero);
    m_spPrgrsFr->setPosition(pt);
    
    pt.x++;
    pt.y++;
    m_prgrsBar = CCProgressTimer::progressWithFile("loading_bar.png");
    m_prgrsBar->setType(kCCProgressTimerTypeHorizontalBarLR);
    addChild(m_prgrsBar);
    m_prgrsBar->setAnchorPoint(CCPointZero);
    m_prgrsBar->setIsVisible(false);
    m_prgrsBar->setPosition(pt);
    
    
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
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui.plist");
    
    //debug info
    string strBundleId = CGameData::Inst()->getBundleId();
    if(!(strBundleId == "com.oneclick.sango" || strBundleId == "com.oneclick.sango.pp")){
        CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 300);
        lc->setPosition(ccp(0, 660));
        addChild(lc, 100);
        
        string strBundleName = CGameData::Inst()->getBundleName();
        CCLabelTTF* lb = CCLabelTTF::labelWithString(strBundleName.c_str(), CCSizeMake(600, 30), CCTextAlignmentLeft, "Arial", 30);
        lb->setAnchorPoint(ccp(0, 0.5));
        lb->setPosition(ccp(50, 900));
        addChild(lb, 200);
        
        lb = CCLabelTTF::labelWithString(netServerURL, CCSizeMake(600, 30), CCTextAlignmentLeft, "Arial", 30);
        lb->setAnchorPoint(ccp(0, 0.5));
        lb->setPosition(ccp(50, 870));
        addChild(lb, 200);
        
        char buf[100];
        snprintf(buf, 99, "bundle id: %s", strBundleId.c_str());
        lb = CCLabelTTF::labelWithString(buf, CCSizeMake(600, 30), CCTextAlignmentLeft, "Arial", 30);
        lb->setAnchorPoint(ccp(0, 0.5));
        lb->setPosition(ccp(50, 840));
        addChild(lb, 200);
    }
    
    if(showSaveDataDlg() == false)
        showClickLabel();
}

void BeginScene::showClickLabel()
{
    CCPoint pt(320, 30);
    
    m_lbText = TextNode::textWithString(OcProxy::Inst()->localizedStringStatic("touchFightTip"), CCSizeMake(600, 100), CCTextAlignmentCenter, 39);
    m_lbText->setPosition(pt);
    m_lbText->setAnchorPoint(CCPointMake(0.5, 0.5));
    m_lbText->setColor(ccc3(233, 183, 72));
    addChild(m_lbText);
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCScaleTo::actionWithDuration(0.5 ,  1.15),
                                                                       CCScaleTo::actionWithDuration(0.5 ,  1.0),
                                                                       NULL);
    m_lbText->runAction(CCRepeatForever::actionWithAction(action));
}

bool BeginScene::showSaveDataDlg()
{
    int logintype=CGameData::Inst()->getLoginType();
    if (logintype == enLoginReqType_Null)
    {   // 没有登录，玩毛线sl
        SaveData::Inst()->destorySavedData();
        SaveData::Inst()->destorySavedFile();
        OrgSaveData::Inst()->destorySavedData();
        OrgSaveData::Inst()->destorySavedFile();
        return false;
    }
    
    if (SaveData::Inst()->hasValidSavedData()
        || OrgSaveData::Inst()->hasValidSavedData()) {
        if (SaveData::Inst()->hasValidSavedData())
        {
            if (strcmp(SaveData::Inst()->getMD5Str(), CGameData::Inst()->getMD5Str()) == 0) {
                SaveData::Inst()->checkIsUseExternal();
                
                m_dlgSaveData = new Dialog2(this,
                                            callfuncO_selector(BeginScene::cbSaveDataDlg),
                                            OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip3"),
                                            OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip4"),
                                            enDl2TwoBtn);
                m_dlgSaveData->setPosition(CCPointMake(320, 480));
                addChild(m_dlgSaveData);
                m_dlgSaveData->release();
                m_dlgSaveData->setScale(0);
                m_dlgSaveData->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                
                return true;
            }
            else
            {
                SaveData::Inst()->destorySavedData();
                SaveData::Inst()->destorySavedFile();
                return false;
            }
        }
        else if (OrgSaveData::Inst()->hasValidSavedData())
        {
            if (strcmp(OrgSaveData::Inst()->getMD5Str(), CGameData::Inst()->getMD5Str()) == 0) {
                OrgSaveData::Inst()->checkIsUseExternal();
                
                m_dlgSaveData = new Dialog2(this,
                                            callfuncO_selector(BeginScene::cbSaveDataDlg),
                                            OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip3"),
                                            OcProxy::Inst()->getLocalizedString("selectLoginOrFightTip4"),
                                            enDl2TwoBtn);
                m_dlgSaveData->setPosition(CCPointMake(320, 480));
                addChild(m_dlgSaveData);
                m_dlgSaveData->release();
                m_dlgSaveData->setScale(0);
                m_dlgSaveData->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                
                return true;
            }
            else
            {
                OrgSaveData::Inst()->destorySavedData();
                OrgSaveData::Inst()->destorySavedFile();
                return false;
            }
        }
    }
    
    return false;
}

void BeginScene::cbSaveDataDlg(cocos2d::CCObject *pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    bool bOk = pRet->bOk;
    if (bOk)
    {
        if (SaveData::Inst()->hasValidSavedData())
        {
            CGameData::Inst()->setIsUseChristmasUI(SaveData::Inst()->getIsUseChristmasUI());
            CGameData::Inst()->setSessionId(SaveData::Inst()->getSessionId());
            CGameData::Inst()->setUserId(SaveData::Inst()->getUserId());
        }
        else
        {
            CGameData::Inst()->setIsUseChristmasUI(OrgSaveData::Inst()->getIsUseChristmasUI());
            CGameData::Inst()->setSessionId(OrgSaveData::Inst()->getSessionId());
            CGameData::Inst()->setUserId(OrgSaveData::Inst()->getUserId());
        }
        showClickLabel();
    }
    else
    {

        if (SaveData::Inst()->hasValidSavedData()) {//战场结算请求判断返体
#ifdef ZhanBaiFanTi
          int    m_iFloorId = SaveData::Inst()->getFloorId();
          int   m_iRoomId = SaveData::Inst()->getRoomId();
          const char*  m_szDgType = SaveData::Inst()->getDgType();
          char m_dungeonUid[32];
          snprintf(m_dungeonUid, 31, "%s", SaveData::Inst()->getDungeonUid());
            
          char floorid[10];
          sprintf(floorid, "%d", m_iFloorId);
          char roomid[10];
         sprintf(roomid, "%d", m_iRoomId);
   
          if(CGameData::Inst()->dgEndForFanTiRequest(m_szDgType, floorid, roomid, m_dungeonUid))
            {
                m_isFanti = true;
              
                scheduleUpdate();
            }
#else
         SaveData::Inst()->destorySavedData();
         SaveData::Inst()->destorySavedFile();
         showClickLabel();
#endif
            
        }else{
            OrgSaveData::Inst()->destorySavedData();
            OrgSaveData::Inst()->destorySavedFile();
            showClickLabel();
        }
       
    }
    
    if (m_dlgSaveData)
    {
        m_dlgSaveData->removeFromParentAndCleanup(true);
        m_dlgSaveData = NULL;
    }
    
   // showClickLabel();
}

void BeginScene::onExit() {
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    CCLayer::onExit();
}

