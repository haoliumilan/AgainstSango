#include "GachaGet.h"
#include "cocos2d.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "PublishWeiboLayer.h"
#include "Dialog2.h"
#include "CCScrewOut.h"
#include "OcProxy.h"

#define FIX_POS(_pos, _min, _max) \
    if (_pos < _min)        \
    _pos = _min;        \
else if (_pos > _max)   \
    _pos = _max;        \

//------------------------------------------------------------------
//
// AccelerometerTest
//
//------------------------------------------------------------------


GachaGet::GachaGet(void)
: pflag(NOTOUGH),handValue(0),m_isFromCardItem(false)
{
}
GachaGet::GachaGet(int type,char ucidbuf[100],CCObject* listener,SEL_CallFuncO selector,bool isGachaCardPlus,bool isFromCardItem)
: pflag(NOTOUGH),handValue(0.0f)
{
    m_isFromCardItem = isFromCardItem;
    m_isGotUnknowCompGachaHero =  false;
    m_pListener=listener;
    m_pfnSelector=selector;
    gachaType=type;
    strcpy(ucid,ucidbuf);
    m_isGachaCardPlus = isGachaCardPlus;
    m_ret.gachaType = type;
}
GachaGet::~GachaGet(void)
{

}

void GachaGet::onEnter()
{
	CCLayer::onEnter();
    
    switch (gachaType) {
        case CompGachaType:
            CGameData::Inst()->addBigImageByRGBA4444("exchange.plist");
            
            MainScene::Inst()->showUsrInfPanel(true);
            exchangeSuperHero();
            break;
            
        
        case FriendGacha10Type:
        case UnusualGacha10Type:
            
            CGameData::Inst()->addBigImageByRGBA4444("exchange.plist");
            
        case FriendGachaType:
        case UnusualGachaType:
            
            setIsTouchEnabled(true);
            CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
            
            setIsAccelerometerEnabled(true);
            
            pflag = GACHAPIC;
            
            if ((FriendGachaType==gachaType)
                || (FriendGacha10Type==gachaType)){
                m_pBg = CCSprite::spriteWithFile("oldman_bg1.jpg");
                
            }
            else if((UnusualGachaType==gachaType)
                    || (UnusualGacha10Type==gachaType))
            {
                m_pBg = CCSprite::spriteWithFile("oldman_bg2.jpg");
            }
            
            
            m_pBg->setAnchorPoint(CCPointMake(0.5,0.5));
            m_pBg->setPosition(ccp(320, 480));
            m_pHand = CCSprite::spriteWithFile("gachahand.png");
            
            m_pHand->setAnchorPoint(CCPointMake(0.5,0.5));
            m_pHand->setPosition(ccp(320, 150));
            
            
            m_pHandTxt = CCSprite::spriteWithFile("gachatxt.png");
            m_pHandTxt->setAnchorPoint(CCPointMake(0.5,0.5));
            m_pHandTxt->setPosition(ccp(320, 100));
            
            
            addChild(m_pBg);
            addChild(m_pHand);
            addChild(m_pHandTxt);
            break;

        default:
            break;
    }
}

void GachaGet::goCompGacha()
{
    m_ret.isGoCompGacha = true;
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void GachaGet::shareToWeibo()
{
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(GachaGet::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("weibo_authened"),
                                      CGameData::Inst()->getLanguageValue("weibo_authened_info"),
                                      enDl2TwoBtn);
        addChild(dialog,100);
        dialog->setTag(111);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else 
    {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCRenderTexture *target = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
    target->begin();
    MainScene::Inst()->visit();
    target->end();
    target->saveBuffer(kCCImageFormatPNG,"captureImage.png");
     //   CGameData::Inst()->setIsGachaWeibo(true);
        OcProxy::Inst()->setIsGachaWeibo(true);
    MainScene::Inst()->showWeiboPublishInfoWithType(enWeiboTypeGacha);
    }
    
}

void GachaGet::dl2CallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(111);
    if(ret->bOk == true)
    {
        closeSelf();
        MainScene::Inst()->setGachaInfoHide();
        MainScene::Inst()->showBottomPanel(true);
        MainScene::Inst()-> gotoOtherMain();
        
    }else {
        dialog->removeFromParentAndCleanup(true);
    }
}   

bool GachaGet::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (pflag==GACHAPIC)
    {
        handScroll();

        switch (gachaType) {
            case FriendGachaType:
            case UnusualGachaType:
                dropEgg();
                break;
                
            case FriendGacha10Type:
            case UnusualGacha10Type:
                screwOut();
                break;
                
            default:
                break;
        }
        setIsTouchEnabled(false);
        
        pflag=NOTOUGH;
    }
    if (pflag==INFPIC) {
        setIsTouchEnabled(false);
        closeSelf();
        pflag=NOTOUGH;
      
    }
    return true;
}

void GachaGet::exchangeSuperHero()
{
    CCLog("ucid %s",ucid);
    gachaStar = CGameData::Inst()->getUsrCardInfByUcid(ucid)->star;
    
    setIsAccelerometerEnabled(false);
    setIsTouchEnabled(false);
        
    CCSprite *sp;
    //背景
    MainScene::Inst()->showUsrInfPanel(true);
    sp = CCSprite::spriteWithFile("show_bg.jpg");
    if(sp != NULL){
        sp->setScale(1/0.8);
        sp->setPosition(ccp(320, 480));
        addChild(sp, 10);
        
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0),
                                          CCFadeIn::actionWithDuration(0.5),
                                          NULL));
    }
    
    CCSprite *cirleOne = CCSprite::spriteWithFile("guanghuan.png");
    addChild(cirleOne,11);
    cirleOne->setScale(2.0);
    cirleOne->setPosition(CCPoint(320, 480));
    cirleOne->setOpacity(0);
    CCSprite *cirleTwo = CCSprite::spriteWithFile("guanghuan.png");
    addChild(cirleTwo,11);
    cirleTwo->setScale(2.0);
    cirleTwo->setOpacity(0);
    cirleTwo->setPosition(CCPoint(320, 480));

    CCSprite *cirleThree = CCSprite::spriteWithFile("guanghuan.png");
    addChild(cirleThree,11);
    cirleThree->setScale(2.0);
    cirleThree->setOpacity(0);
    cirleThree->setPosition(CCPoint(320, 480));

    
    char buf[100];
    //    //令牌
    if (gachaStar > 5) {
        snprintf(buf, 99, "droplv5.png");
    }
    else {
        snprintf(buf, 99, "droplv%d.png", gachaStar);
    }
    
    CGameData::Inst()->setGachaCardName(CGameData::Inst()->getUsrCardInfByUcid(ucid)->name);
    sp = CCSprite::spriteWithFile(buf);
    sp->setPosition(CCPoint(320, 650));
    addChild(sp,21);
    sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0),
                                      CCMoveTo::actionWithDuration(1.0, CCPoint(320, 480)),
                                      CCFadeOut::actionWithDuration(0.4),
                                      NULL));
    
     cirleOne->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),CCFadeIn::actionWithDuration(0.2),CCScaleTo::actionWithDuration(0.6, 0),NULL));
     cirleTwo->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),CCFadeIn::actionWithDuration(0.2),CCScaleTo::actionWithDuration(0.6, 0),NULL));
     cirleThree->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.9),CCFadeIn::actionWithDuration(0.2),CCScaleTo::actionWithDuration(0.6, 0),NULL));
   
    CCLayerColor *whiteLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255,255), 640, 960);
    addChild(whiteLayer,12);
    whiteLayer->setOpacity(0);
    whiteLayer->setPosition(CCPoint(0,0));
//    whiteLayer->setScale(4.0);
    whiteLayer->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.4),CCFadeIn::actionWithDuration(0.6),CCFadeOut::actionWithDuration(0.2),NULL));
    
    //新产品
    unsigned short newCardId = CGameData::Inst()->getUsrCardInfByUcid(ucid)->cid;
    sp = CGameData::Inst()->getBodySprite(newCardId);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(ccp(320, 480-80));
        addChild(sp, 10);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.6),
                                          CCFadeIn::actionWithDuration(1.8),
                                          NULL));
    }
    
    //判断集换的卡牌星级
    if(m_isGotUnknowCompGachaHero)
    {
        snprintf(buf, 99, "chaojipin.png");
        sp = CCSprite::spriteWithFile(buf);
    }
    else
    {
        if (CGameData::Inst()->getUsrCardInfByUcid(ucid)->star == 3) {
                snprintf(buf, 99, "liangpin.png");
        }
        else if (CGameData::Inst()->getUsrCardInfByUcid(ucid)->star == 4) {
            
                snprintf(buf, 99, "zhenpin.png");
        }
        else if(CGameData::Inst()->getUsrCardInfByUcid(ucid)->star>=5){
            
                snprintf(buf, 99, "jipin.png");
        }
        sp = CCSprite::spriteWithFile(buf);
    }

        if (sp != NULL) {
            sp->setPosition(CCPointMake(320, 480 + 250));
            addChild(sp, 40);
            sp->setScale(0.0f);
            sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.4),
                                              CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5f, 1.0f), 0.8f),
                                              NULL));
        }
    
    CCSprite *lightBall = CCSprite::spriteWithSpriteFrameName("goldlight0001.png");
    lightBall->setPosition(ccp(320, 480));
    lightBall->setOpacity(0);
    lightBall->setScale(1.5);
    addChild(lightBall,20);
    CCAnimation *lightBallAnim ;
    lightBallAnim = CCAnimation::animation();
    CCActionInterval *lightBallActionInt;
    for(int i= 0;i<14;i++)
    {
        char frameName[30];
        sprintf(frameName, "goldlight00%02d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        lightBallAnim->addFrame(frame);
    }
    lightBallActionInt = CCAnimate::actionWithDuration(1.0, lightBallAnim, false);
    lightBall->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.7),
                                             CCFadeIn::actionWithDuration(0.2),
                                             lightBallActionInt,CCFadeOut::actionWithDuration(0.2),
                                          NULL));    
    CCSprite *starLight = CCSprite::spriteWithSpriteFrameName("star0001.png");
    starLight->setPosition(ccp(320, 480));
    starLight->setOpacity(0);
    starLight->setScale(1.5);
    addChild(starLight,22);
    CCAnimation *starLightAnim ;
    starLightAnim = CCAnimation::animation();
    CCActionInterval *starLightActionInt;
    for(int i= 1;i<15;i++)
    {
        char frameName[30];
        sprintf(frameName, "star00%02d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
       starLightAnim->addFrame(frame);
    }
    starLightActionInt = CCAnimate::actionWithDuration(2.0, starLightAnim, false);
    starLight->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(3.0),
                                             CCFadeIn::actionWithDuration(0.2),
                                            starLightActionInt,
                                             NULL));   
    
    schedule(schedule_selector(GachaGet::showUsrInf), 2.0);
    schedule(schedule_selector(GachaGet::infCouldBeShow), 3.9);
    
}
void GachaGet::handScroll()
{
    setIsAccelerometerEnabled(false);
    setIsTouchEnabled(false);

    m_pHandTxt->runAction(CCFadeOut::actionWithDuration(0.2));
    
    m_pHand->runAction(CCSequence::actions(CCRepeat::actionWithAction(CCSequence::actions(
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -10)),
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -30)),
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -50)),
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 50)),
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 30)),
                                                                      CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 10)),
                                                                      NULL),3),CCFadeOut::actionWithDuration(0.5),NULL));
}
void  GachaGet::infCouldBeShow()
{
    unschedule(schedule_selector(GachaGet::infCouldBeShow));
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    pflag=INFPIC;
    if(!CGameData::Inst()->isNewbie()&&gachaStar>=3)
    {
        CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
        CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
        CCMenuItem *shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, 
                                                                      tempSprite2, this, menu_selector(GachaGet::shareSanGuo));
        
        tempSprite1 = CCSprite::spriteWithFile("button_dianjiang.png");
        tempSprite2 = CCSprite::spriteWithFile("button_dianjiang.png");
        CCMenuItem *goCompGachaBtn = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2,this, menu_selector(GachaGet::goCompGacha));
        goCompGachaBtn ->setAnchorPoint(CCPointZero);
        shareBtn->setAnchorPoint(CCPointZero);
        
        if(gachaType == CompGachaType || m_isFromCardItem)
            goCompGachaBtn->setIsVisible(false);
        else
            goCompGachaBtn->setIsVisible(CGameData::Inst()->getGachaRet()->is_compGachaMaterial&&CGameData::Inst()->getCommonInfo()->compgacha_fg);
        CCMenu *shareMenu = CCMenu::menuWithItems(shareBtn,goCompGachaBtn,NULL);
        addChild(shareMenu,30);
        shareMenu->setPosition(CCPointZero);  
        shareBtn->setPosition(ccp(170, 100));
        goCompGachaBtn->setPosition(CCPointMake(170, 220));
        //分享开关
        if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
            shareMenu ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
            
        }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
            shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
            
        }else {
            shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        }
    }
    
    //append item
    std::string szTemp;
    CGameData::Inst()->getGuildAwardText(szTemp);
    if (!szTemp.empty())
    {
        if (CCSprite *pSprite = CCSprite::spriteWithFile("append_item_text_box.png"))
        {
            CCLayer *pLayer = CCLayer::node();
            pLayer->addChild(pSprite);
            pLayer->setPosition(ccp(320,50));
            addChild(pLayer,10);
            
            TextNode *pText = TextNode::textWithString(szTemp.c_str(), 22);
            pText->setAnchorPoint(ccp(0.5,0.5));
            pText->setColor(ccc3(255,211,65));
            pText->setShadowColor(ccBLACK);
            pLayer->addChild(pText);
        }
    }
    
}


void  GachaGet::showUsrInf()
{
    unschedule(schedule_selector(GachaGet::showUsrInf));
    MainScene::Inst()->showUsrInfPanel(true);

    if (CGameData::Inst()->isNewbie()) {
        MainScene::Inst()->showTouchTip();
    }
}

void GachaGet::closeSelf()
{
    (m_pListener->*m_pfnSelector)(&m_ret);
}

//******************************************************************************
// showTorchParticle
//****************************************************************
void GachaGet::showTorchParticle() {
    CCParticleTorch *pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(50, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch, 15);
    
    pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(590, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch, 15);
}

void GachaGet::onExit()
{
    if (CGameData::Inst()->isNewbie()) {
        MainScene::Inst()->removeTouchTip();
    }
    
    if(CompGachaType == gachaType)
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("exchange.plist");
    }
    
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

void GachaGet::didAccelerate(CCAcceleration* pAccelerationValue)
{
    if (handValue!=0) {
        float a=handValue- pAccelerationValue->z;
        if (a<0.0f) {
            a=-a;
        }
        if (a>0.5f) {

            handValue=0.0f;
            
            if (pflag==GACHAPIC){
                handScroll();
                
                switch (gachaType) {
                    case FriendGachaType:
                    case UnusualGachaType:
                        dropEgg();
                        break;
                        
                    case FriendGacha10Type:
                    case UnusualGacha10Type:
                        screwOut();
                        break;
                        
                    default:
                        break;
                }
                
                pflag=NOTOUGH;
            }
        }
    }
    
    handValue = pAccelerationValue->z;
}

//******************************************************************************
// dropEgg
//******************************************************************************
void GachaGet::dropEgg()
{
    gachaStar = CGameData::Inst()->getUsrCardInfByUcid(ucid)->star;
    CCSprite *plusIcon =NULL;
    char buf[100];
    //    //令牌
    if (gachaStar > 5) {
        snprintf(buf, 99, "droplv5.png");
    }
    else {
        snprintf(buf, 99, "droplv%d.png", gachaStar);
    }
    CGameData::Inst()->setGachaCardName(CGameData::Inst()->getUsrCardInfByUcid(ucid)->name);
    CCSprite* sp = CCSprite::spriteWithFile(buf);
    
    //+1
    plusIcon = NULL;
    if(m_isGachaCardPlus)
    {
        plusIcon = CCSprite::spriteWithSpriteFrameName("icon_plus.png");
        sp->addChild(plusIcon);
        plusIcon->setPosition(ccp(60, 20));
        //sp->setPosition(ccp(30, 50));
    }
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(ccp(320, 480-150));
        addChild(sp, 2);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.2),
                                          CCFadeIn::actionWithDuration(0.1),
                                          CCScaleBy::actionWithDuration(0.1 ,  3.0),
                                          CCScaleBy::actionWithDuration(0.05 ,  1.3),
                                          CCScaleBy::actionWithDuration(0.05 ,  0.7),
                                          CCScaleBy::actionWithDuration(0.05 ,  1.2),
                                          CCScaleBy::actionWithDuration(0.05 ,  0.9),
                                          CCDelayTime::actionWithDuration(1.0),
                                          CCFadeOut::actionWithDuration(0.5),
                                          NULL));
        
        //+1
        if(plusIcon)
        {
            plusIcon->setOpacity(0);
            plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.2),
                                              CCFadeIn::actionWithDuration(0.1),
                                              CCDelayTime::actionWithDuration(1.3),
                                              CCFadeOut::actionWithDuration(0.5),
                                              NULL));
        }
    }
    
    
    
    //背景
    sp = CCSprite::spriteWithFile("show_bg.jpg");
    if(sp != NULL){
        sp->setScale(1/0.8);
        sp->setPosition(ccp(320, 480));
        addChild(sp, 10);
       

        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(3.5),
                                          CCFadeIn::actionWithDuration(0.5),
                                          NULL));
    }
    sp = CCSprite::spriteWithFile(buf);
    if(sp != NULL){
        sp->setPosition(ccp(320, 480-20));
        addChild(sp, 10);
        //+1
        
        if(plusIcon)
        {
            plusIcon = CCSprite::spriteWithSpriteFrameName("icon_plus.png");
            sp->addChild(plusIcon);
            plusIcon->setPosition(ccp(60, 20));
        }
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(3.5),
                                          CCFadeIn::actionWithDuration(0.5),
                                          CCDelayTime::actionWithDuration(1.0),
                                          CCFadeOut::actionWithDuration(0.5),
                                          NULL));
        //+1
        if(plusIcon)
        {
            plusIcon->setOpacity(0);
            plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(3.5),
                                              CCFadeIn::actionWithDuration(0.5),
                                              CCDelayTime::actionWithDuration(1.0),
                                              CCFadeOut::actionWithDuration(0.5),
                                              NULL));
        }
    }
    
    //光效
    sp = CCSprite::spriteWithFile("evolutionlight.png");
    if(sp != NULL){
        sp->setScale(4.0);
        sp->setPosition(ccp(320, 480));
        addChild(sp, 10);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(4.5),
                                          CCFadeIn::actionWithDuration(1.0),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(GachaGet::showTorchParticle)),
                                          CCFadeOut::actionWithDuration(0.5),
                                          NULL));
    }
    
    
    //新产品
    unsigned short newCardId = CGameData::Inst()->getUsrCardInfByUcid(ucid)->cid;
    sp = CGameData::Inst()->getBodySprite(newCardId);
    if(sp != NULL){
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(ccp(320, 480-80));
        addChild(sp, 10);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(5.5),
                                          CCFadeIn::actionWithDuration(1.5),
                                          NULL));
    }
    
    if (gachaStar >= 3) {
        if (gachaStar == 3) {
            snprintf(buf, 99, "liangpin.png");
        }
        else if (gachaStar == 4) {
            snprintf(buf, 99, "zhenpin.png");
        }
        else {
            snprintf(buf, 99, "jipin.png");
        }
        
        sp = CCSprite::spriteWithFile(buf);
        
        if (sp != NULL) {
            sp->setPosition(CCPointMake(320, 480 + 250));
            addChild(sp, 10);
            sp->setScale(0.0f);
            sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(6.0f),
                                              CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5f, 1.0f), 0.8f),
                                              NULL));
        }
    }
    setIsTouchEnabled(true);
    
    schedule(schedule_selector(GachaGet::showUsrInf), 3.5);
    schedule(schedule_selector(GachaGet::infCouldBeShow), 5.9);
}

//******************************************************************************
// screwOut
//******************************************************************************
void GachaGet::screwOut()
{
    CCMutableArray<CGachaRet *> * pGachaRetArr = CGameData::Inst()->getMultiGachaRet();
    
    if(pGachaRetArr->count() == 0){
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
        pflag=INFPIC;
        return;
    }

    CGachaRet * pGachaRet;
    CUserCardInfo* pUsrCardInfo;
    CCSprite* sp;
    float dt = 2.0;
    
    CCLayerColor* pLc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255, 0), 640, 960);
    addChild(pLc, 100);
    pLc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                       CCFadeIn::actionWithDuration(0.01),
                                       CCFadeOut::actionWithDuration(0.5),
                                       NULL));
    
    
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    int i;
    animation = CCAnimation::animation();
    CCSprite* spGoldlight = CCSprite::spriteWithSpriteFrameName("blank.png");
    spGoldlight->setScale(2.0);
    if(spGoldlight){
        addChild(spGoldlight, 200);
        spGoldlight->setPositionInPixels(CCPointMake(320, 560));
        
        for(i = 1; i < 100; i++){
            sprintf(frameName, "goldlight00%02d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
            if(frame)
                animation->addFrame(frame);
            else
                break;
        }
        action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
        
        spGoldlight->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                   action,
                                                   CCFadeOut::actionWithDuration(0.1),
                                                   NULL));
    }
    
    
    
    animation = CCAnimation::animation();
    CCSprite* spStar = CCSprite::spriteWithSpriteFrameName("blank.png");
    spStar->setScale(2.0);
    if(spStar){
        addChild(spStar, 200);
        spStar->setPositionInPixels(CCPointMake(320, 560));
        
        for(i = 1; i < 100; i++){
            sprintf(frameName, "star00%02d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
            if(frame)
                animation->addFrame(frame);
            else
                break;
        }
        action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
        
        spStar->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                              action,
                                              CCFadeOut::actionWithDuration(0.1),
                                              NULL));
    }
    
    CCSprite *plusIcon = NULL;

    for (int i = 0; i < pGachaRetArr->count(); i++) {
        pGachaRet = pGachaRetArr->getObjectAtIndex(i);
        if(pGachaRet){
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
            
            if(pUsrCardInfo){
                char buf[100];
                
                if (pUsrCardInfo->star > 5) {
                    snprintf(buf, 99, "droplv5.png");
                }
                else {
                    snprintf(buf, 99, "droplv%d.png", pUsrCardInfo->star);
                }
                
                sp = CCSprite::spriteWithFile(buf);
                
                plusIcon = NULL;
                if(pGachaRet->plus>0)
                {
                    plusIcon= CCSprite::spriteWithSpriteFrameName("icon_plus.png");
                    sp->addChild(plusIcon);
                    plusIcon->setPosition(ccp(60, 20));
                    plusIcon->setOpacity(0);
                }
                //sp->setPosition(ccp(30, 50));
                if(sp){
                    sp->setPositionInPixels(ccp(360, 560));
                    addChild(sp, 100 - i);
                    
                    sp->setOpacity(0);
                    
                    dt += 0.2;
                    
                    if(i == pGachaRetArr->count()-1){
                        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                          CCFadeIn::actionWithDuration(0.1),
                                                          CCSpawn::actions(CCScrewOut::actionWithDuration(3),
                                                                           CCScaleTo::actionWithDuration(3, 2.0),
                                                                           NULL),
                                                          CCCallFunc::actionWithTarget(this, callfunc_selector(GachaGet::cbScrewOut)),
                                                          
                                                          NULL));
                        if(plusIcon)
                            plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                                CCFadeIn::actionWithDuration(0.1),
                                                                CCDelayTime::actionWithDuration(3.0),
                                                                NULL));
                    }
                    else{
                        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                          CCFadeIn::actionWithDuration(0.1),
                                                          CCSpawn::actions(CCScrewOut::actionWithDuration(3),
                                                                           CCScaleTo::actionWithDuration(3, 2.0),
                                                                           NULL),
                                                          
                                                          NULL));
                        if(plusIcon)
                            plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                                CCFadeIn::actionWithDuration(0.1),
                                                                CCDelayTime::actionWithDuration(3.0),
                                                                NULL));
                    }
                }
            }
        }
        
        
        
        
    }
    setIsTouchEnabled(true);
}


//******************************************************************************
// cbScrewOut
//******************************************************************************
void GachaGet::cbScrewOut()
{
    removeAllChildrenWithCleanup(true);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("exchange.plist");
    
    closeSelf();
}

//******************************************************************************
// setIsGotUnknowCompGachaHero
//******************************************************************************
void GachaGet::setIsGotUnknowCompGachaHero(bool isOk)
{
    m_isGotUnknowCompGachaHero = isOk;
}


//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void GachaGet::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(GachaGet::shareToWeibo));

    
}


