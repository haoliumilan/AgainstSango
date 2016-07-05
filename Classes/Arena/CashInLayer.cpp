//
//  CashInLayer.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-24.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CashInLayer.h"
#include "CGamedata.h"
#include "MainScene.h"
#include "TextNode.h"
#include "SaveData.h"
#include "CSkillLib.h"
#include "ActNumTTF.h"

#include "TDCCVirtualCurrency.h"
#define  kTouchNUm    50



//******************************************************************************
// Construct
//******************************************************************************
CashInLayer::CashInLayer(CCObject* target, SEL_CallFunc selector)
{
    showTime = 0;
    m_lvlUp = 0;
    m_pListener = target;
    m_pfnSelector = selector;
    isX2Money = false;
    isX2Exp = false;
    isX2Drop = false;
    
}

//显示所获得的卡片
void CashInLayer::showGetCardInfo()
{
    int* pTrophies = CGameData::Inst()->getTrophies();
    CCSprite* sp;
    CCPoint ptStart(23, 320);
    if (m_endTotalCards>10) {
        ptStart = CCPoint(23, 430);
    }
    float gap = (640 - 600) / 7;
    CCPoint pt;
    
    
    CCMutableArray<GuildItemAward*> * pGuildItemAwardArr = CGameData::Inst()->getGuildItemAwardArr();
    if(m_endTotalCards == 0)
    {
        removeAllChildrenWithCleanup(true);
        (m_pListener->*m_pfnSelector)();
        setIsTouchEnabled(false);
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        showTime = 0;
        return;
    }
    // moveLayer->removeAllChildrenWithCleanup(true);
    showTime=3;
    //获得卡片
    TextNode *lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip1"), CCSizeMake(600, 42), CCTextAlignmentCenter, 40);
    if (m_endTotalCards>10) {
        lbText->setPosition(CCPointMake(320, 382));
    }else{
        lbText->setPosition(CCPointMake(320, 302));
    }
    moveLayer->addChild(lbText, 1);
    
    gap = (640-100*m_endTotalCards)/(m_endTotalCards+1);
    int i =0;
    
    while(pTrophies[i] != 0){
        sp = CGameData::Inst()->getHeadSprite(pTrophies[i]);
        if(sp != NULL){
            if(m_endTotalCards>=5)
            {
                pt.x = ptStart.x  + (i % 5) *123;
                
            }
            else 
                pt.x = ptStart.x + gap + (i % m_endTotalCards) * (gap + 100)-23;
            
            pt.y = ptStart.y -90- (i/5)*110;
            
            sp->setAnchorPoint(CCPointMake(0, 1));
            sp->setPosition(pt);
            moveLayer->addChild(sp);
        }
        i++;
    }
    
    for (int j=0; j<pGuildItemAwardArr->count(); j++)
    {
        GuildItemAward* pGuildItemCard = pGuildItemAwardArr->getObjectAtIndex(j);
       // ItemCardInf* pCardInf = CGameData::Inst()->getItemCardByID(pGuildItemCard->strItemId);
        
        sp = CCSprite::spriteWithFile(pGuildItemCard->strImgName.c_str());
        if(sp != NULL)
        {
            if(m_endTotalCards>=5)
            {
                pt.x = ptStart.x  + (i % 5) *123;
                
            }
            else
                pt.x = ptStart.x + gap + (i % m_endTotalCards) * (gap + 100)-23;
            
            pt.y = ptStart.y -90- (i/5)*110;
            
            sp->setAnchorPoint(CCPointMake(0, 1));
            sp->setPosition(pt);
            moveLayer->addChild(sp);
            
            char buf[16];
            snprintf(buf, 16, "/%ld", pGuildItemCard->lCnt);
            CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
            if(spTmp){
                CCSize sz = spTmp->getContentSize();
                CCLabelAtlas* lbItemCount = CCLabelAtlas::labelWithString(buf,
                                                                          "rewardnum.png",
                                                                          sz.width / 11,
                                                                          sz.height,
                                                                          '/');
                lbItemCount->setAnchorPoint(ccp(0.5, 0));
                lbItemCount->setPosition(CCPointMake(sp->getContentSize().width/2, 0));
                sp->addChild(lbItemCount);
            }
        }
        i++;
    }
}


//等级提升对应显示的数据
void CashInLayer::showLvlUpInfo()
{
    showTime=1;
    char buf[200];
    float tempY = 250;
    CCSprite *firstSuc = CCSprite::spriteWithSpriteFrameName("dengjitisheng.png");
    firstSuc->setPosition(CCPoint(320, 350));
    firstSuc->setScale(0.3);
    moveLayer->addChild(firstSuc);
 
    //cost值增加后 提示
    if(CGameData::Inst()->getUsrInfo()->cost - m_preCost > 0)
    {
        int delta = CGameData::Inst()->getUsrInfo()->cost - m_preCost;
        sprintf(buf,"%s%d%s%d",CGameData::Inst()->getLanguageValue("cashInLayerTip2"),delta,CGameData::Inst()->getLanguageValue("cashInLayerTip3"),CGameData::Inst()->getUsrInfo()->cost);    // 您的统御力增加了 x 上升到了 xx
        TextNode *label1 = TextNode::textWithString(buf,CCSize(600,40), CCTextAlignmentLeft,30);
        label1->setAnchorPoint(ccp(0, 0.5));
        label1->setPosition(ccp(100, tempY));
        moveLayer->addChild(label1);
        tempY-=40;
    }
    
    
    TextNode *label2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip4"),CCSize(600,40), 
                                                     CCTextAlignmentLeft, 30);
    label2->setAnchorPoint(ccp(0, 0.5));
    label2->setPosition(ccp(100, tempY));
    moveLayer->addChild(label2);
    tempY-=40;
    //体力值增加提示
    if(CGameData::Inst()->getUsrInfo()->max_stamina - m_preStamina > 0)
    {
        int delta = CGameData::Inst()->getUsrInfo()->max_stamina - m_preStamina;
        sprintf(buf, "%s%d%s%d",CGameData::Inst()->getLanguageValue("cashInLayerTip5"),delta,CGameData::Inst()->getLanguageValue("cashInLayerTip3"),CGameData::Inst()->getUsrInfo()->max_stamina);
        TextNode *label3 = TextNode::textWithString(buf,CCSize(600,40), 
                                                         CCTextAlignmentLeft, 30);
        label3->setAnchorPoint(ccp(0, 0.5));
        label3->setPosition(ccp(100, tempY));
        moveLayer->addChild(label3);
        tempY -=80;

    }
    //等级提升送元宝提示
    if(CGameData::Inst()->isNewbie() == false)
    {
        if(CGameData::Inst()->getLvlUpCoin()>0 )
        {
            TextNode *label = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip6"),CCSize(400,80),CCTextAlignmentLeft, 30);
            label->setAnchorPoint(ccp(0, 0.5));
            label->setPosition(ccp(100, tempY));
            moveLayer->addChild(label);
            
            
            
#ifdef TalkData
            TDCCVirtualCurrency::onReward(CGameData::Inst()->getLvlUpCoin(), "getLvlUpCoin");
#else
            
#endif
            
            
            
            char buf[50];
            sprintf(buf, "%d%s",CGameData::Inst()->getLvlUpCoin(),CGameData::Inst()->getLanguageValue("arenaTip5"));
            TextNode *labelY = TextNode::textWithString(buf,CCSize(300,80),CCTextAlignmentLeft, 40);
            labelY->setColor(ccc3(210, 36, 2));
            labelY->setAnchorPoint(ccp(0, 0.5));
            labelY->setPosition(ccp(410, tempY));
            moveLayer->addChild(labelY);
            
            
        }
    }
    CCFiniteTimeAction*action= CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.0),NULL);
    firstSuc->runAction(action);
    
    
}

//过关之后的数据显示
void CashInLayer::showTongguanInfo()
{
    showTime = 2;
    CCSprite *firstSuc = CCSprite::spriteWithSpriteFrameName("chucitongguan.png");
    firstSuc->setPosition(CCPoint(320, 350));
    firstSuc->setScale(0.3);
    // addChild(firstSuc);
    TextNode *label = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip6"),CCSize(400,80),CCTextAlignmentLeft, 30);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccp(100, 245));
    moveLayer->addChild(label);
    
    
    

    
    char buf[50];
    int dungeonClearCoin = 0;
    dungeonClearCoin = SaveData::Inst()->getDungeonClearCoin();
    
#ifdef TalkData
    TDCCVirtualCurrency::onReward(dungeonClearCoin, "dungeonClearCoin");

#else
    
#endif
    
    
    

    
    
    sprintf(buf, "%d%s", dungeonClearCoin, CGameData::Inst()->getLanguageValue("arenaTip5"));
    TextNode *labelY = TextNode::textWithString(buf,CCSize(300,80),CCTextAlignmentLeft, 50);
    labelY->setColor(ccc3(210, 36, 2));
    labelY->setAnchorPoint(ccp(0, 0.5));
    labelY->setPosition(ccp(410, 250));
    moveLayer->addChild(labelY);
    
    CCFiniteTimeAction*action= CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.0),NULL);
    firstSuc->runAction(action);
    
    moveLayer->addChild(firstSuc);
        
    CGameData::Inst()->maybeGotoAppstoreWeb();

}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CashInLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
 
    CCPoint touchLocation = touch->locationInView(touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );

    CCPoint ptStart(23, 320);
    if (m_endTotalCards>10) {
        ptStart = CCPoint(23, 430);
    }
    float gap = (640-100*m_endTotalCards)/(m_endTotalCards+1);

    CCPoint pt;
    m_iTouch = 0;
    
    for (int i = 0 ; i < m_endTotalCards; i++) {
        bool bRet = false;
        if(m_endTotalCards>=5)
            bRet = CCRect::CCRectContainsPoint(CCRectMake(ptStart.x  + (i % 5) *123, ptStart.y -90- (i/5)*110 - 110, 123, 110), pos);
        else
            bRet = CCRect::CCRectContainsPoint(CCRectMake(ptStart.x + gap + (i % m_endTotalCards) * (gap + 100)-23, ptStart.y -90- (i/5)*110 - 110, 123, 110), pos);
        if (bRet) {
            m_iTouch += kTouchNUm+i;
            break;
        }

    }
    int *pTrophies =  CGameData::Inst()->getTrophies();
    int cardNum = 0;
    while (pTrophies[cardNum] != 0) {
        cardNum++;
    }
    if (m_iTouch - kTouchNUm >= 0 && m_iTouch - kTouchNUm - cardNum >= 0) {
        if (m_ParcelTipsLayer != NULL) {
            m_ParcelTipsLayer->removeFromParentAndCleanup(true);
            m_ParcelTipsLayer = NULL;
        }
           CCMutableArray<GuildItemAward*> * pGuildItemAwardArr = CGameData::Inst()->getGuildItemAwardArr();
        if (pGuildItemAwardArr->count() > 0 && (m_iTouch - kTouchNUm - cardNum)<pGuildItemAwardArr->count()) {
            m_ParcelTipsLayer = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
            m_ParcelTipsLayer->setPosition(ccp(220, 832+(m_ParcelTipsLayer->getContentSize().height/2)));
            addChild(m_ParcelTipsLayer, 1000);
            m_ParcelTipsLayer->setScale(0);
            
            GuildItemAward* pGuildItemCard = pGuildItemAwardArr->getObjectAtIndex(m_iTouch - kTouchNUm - cardNum);
            ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(pGuildItemCard->strItemId);
            char buf[512] = {0};
            sprintf(buf, "%sx%ld",tempBaseInfo->strItemName.c_str(),pGuildItemCard->lCnt);
            TextNode*   parcelTipsText = TextNode::textWithString(buf, CCSize(310, 300), CCTextAlignmentLeft, 25);
            parcelTipsText->setPosition(ccp(180, 54));
            m_ParcelTipsLayer->addChild(parcelTipsText, 10);
            m_ParcelTipsLayer->setPosition(ccp(320 , 180 + 120));
            m_ParcelTipsLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
        }
        
    }
    return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void CashInLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
   
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void CashInLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_iTouch - kTouchNUm >= 0) {
        if (m_ParcelTipsLayer != NULL) {
            m_ParcelTipsLayer->removeFromParentAndCleanup(true);
            m_ParcelTipsLayer = NULL;
        }else{
        int *pTrophies = CGameData::Inst()->getTrophies();
        if (pTrophies) {
        CCardBaseInfo* baseinfo  = CGameData::Inst()->getCardBaseInfByCid(pTrophies[m_iTouch - kTouchNUm]);
        m_cshowCardinfo->show(baseinfo);
        m_cshowCardinfo->setIsDropImage(true);
       }
    }
        m_iTouch = 0;
        return;
    }
    
    if(showTime ==1)
    {
        bool bFirstPass = false;
        int curFloorStatus = SaveData::Inst()->getCurFloorStatus();
        const char* dgType = SaveData::Inst()->getDgType();
        int iCurFloor = SaveData::Inst()->getFloorId();
        bFirstPass = CGameData::Inst()->isFirstPassBySaveData(curFloorStatus, dgType, iCurFloor);
        
        if(bFirstPass)
        {
            moveLayer->removeAllChildrenWithCleanup(true);
            showTongguanInfo();
        }
        else 
        {
            moveLayer->removeAllChildrenWithCleanup(true);
            showGetCardInfo();
            if(isX2Drop)
            {
                runDropX2AnimWithDelay(0);
            }
        }
        
    }else if(showTime == 2){
        moveLayer->removeAllChildrenWithCleanup(true);
        showGetCardInfo();
        if(isX2Drop)
        {
            runDropX2AnimWithDelay(0);
        }
    }else {
        if (CGameData::Inst()->isNewbie()) {
            MainScene::Inst()->removeTouchTip();
        }
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("moneyX2.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("expX2.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("dropX2.plist");
        removeAllChildrenWithCleanup(true);
        
        (m_pListener->*m_pfnSelector)();
        setIsTouchEnabled(false);
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        showTime = 0;
        
    }
    
}

//******************************************************************************
// show
//******************************************************************************
void CashInLayer::show(const char* floorname, const char* roomname, int coin,
                       int exp,int prelV, int preCost, int preStamina, double expEffect)
{
    m_ParcelTipsLayer = NULL;
    m_sCount = 0;
    m_stopCount = 0;
    m_desCoin = coin;
    m_desExp = exp;
    m_tempExp = 0;
    m_tempCoin = 0;
    m_preCost = preCost;
    m_preStamina = preStamina;
    //removeAllChildrenWithCleanup(true);
    m_lvlUp = CGameData::Inst()->getUsrInfo()->lv- prelV;
    exp_effect = expEffect;
    
    m_desCoin = coin;
    m_desExp = exp;

    //是否钱翻倍
    int my_skid = 0;
    int helper_skid = 0;
    stAutoSkill* pAutoSkill1 = NULL;
    stAutoSkill* pAutoSkill2 = NULL;
    isX2Money = false;
    isX2Exp = false;
    isX2Drop = false;
    
    for (int i = 0; i < 5; i++) {
        my_skid = SaveData::Inst()->getCardInfo(i*6, true)->auto_skill_id;
        helper_skid = SaveData::Inst()->getCardInfo(5 + i*6, true)->auto_skill_id;
        pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(helper_skid);
        pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(my_skid);
        
        if ((pAutoSkill1->iType == enAutoSkillIp_money_exp_drop
             && pAutoSkill1->cElement > 0)
            || (pAutoSkill2->iType == enAutoSkillIp_money_exp_drop
                && pAutoSkill2->cElement > 0)) {
            isX2Money = true;
        }
        
        if ((pAutoSkill1->iType == enAutoSkillIp_money_exp_drop
             && pAutoSkill1->cGenTp > 0)
            ||(pAutoSkill2->iType == enAutoSkillIp_money_exp_drop
               && pAutoSkill2->cGenTp > 0)) {
            isX2Exp = true;
        }
        
        if ((pAutoSkill1->iType == enAutoSkillIp_money_exp_drop
             && pAutoSkill1->cOdds > 0)
            || (pAutoSkill2->iType == enAutoSkillIp_money_exp_drop
                && pAutoSkill2->cOdds > 0)) {
            isX2Drop = true;
        }
        
        if (SaveData::Inst()->getDgSkillType() != enSpecialDgSkillType_quanduiwuxianding
            && i == 0)
        {   // 如果不是全队无限定战场，不用判断其他编队
            break;
        }
    }
    
    setIsTouchEnabled(false);
    
    char buf[100];
    
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_reward"));
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle);
    
    //cover
    CCLayerColor* cover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 100), 640, 960);
    addChild(cover, -100);
    
    //下半部移动部分
    moveLayer = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 0), 640, 380);
    moveLayer->setPosition(ccp(1000,0));
    
    addChild(moveLayer);
    
    
    m_cshowCardinfo = new CShowCardInf();
    addChild(m_cshowCardinfo,1200);
    m_cshowCardinfo->release();
    m_cshowCardinfo->setIsVisible(false);
    
    
    
    
    int* pTrophies = CGameData::Inst()->getTrophies();
    m_endTotalCards = 0;
    while(pTrophies[m_endTotalCards] != 0){
        m_endTotalCards++;
    }
    
    CCMutableArray<GuildItemAward*> * pGuildItemAwardArr = CGameData::Inst()->getGuildItemAwardArr();
    m_endTotalCards += pGuildItemAwardArr->count();
    
    //floor name
    floorNameLabel = TextNode::textWithString(floorname, CCSizeMake(600, 42), CCTextAlignmentLeft, 36);
    floorNameLabel->setPosition(CCPointMake(100, 730-20));
    floorNameLabel->setAnchorPoint(ccp(0, 0.5));
    addChild(floorNameLabel, 1);
    
    if (m_endTotalCards>10) {
        floorNameLabel->setPosition(CCPointMake(-300, 730-20));
    }else{
        floorNameLabel->setPosition(CCPointMake(-300, 730-40));
    }
    
    //room name
    roomNameLabel = TextNode::textWithString(roomname, CCSizeMake(600, 42), CCTextAlignmentRight, 36);
    roomNameLabel->setPosition(CCPointMake(520, 652-20));
    roomNameLabel->setAnchorPoint(ccp(1.0, 0.5));
    roomNameLabel->setColor(ccWHITE);
    addChild(roomNameLabel, 1);
    if (m_endTotalCards>10) {
        roomNameLabel->setPosition(CCPointMake(1200, 652));
    }else{
        roomNameLabel->setPosition(CCPointMake(1200, 632));
    }
    //roomNameLabel->setPosition(ccp(1200, 632));

    //coin
    snprintf(buf, 99, "%d", coin);
    coinNameLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip10"),CCSizeMake(600, 42), CCTextAlignmentLeft, 30);
    coinNameLabel->setPosition(CCPointMake(100, 572));
    coinNameLabel->setAnchorPoint(ccp(0.0, 0.5));
    coinNameLabel->setColor(ccWHITE);
    addChild(coinNameLabel, 1);
    if (m_endTotalCards>10) {
        coinNameLabel->setPosition(CCPointMake(-300, 592));
    }else{
        coinNameLabel->setPosition(CCPointMake(-300, 572));
    }
    //coinNameLabel->setPosition(ccp(-300, 572));

    coinLabel = ActNumTTF::initNumber(CCSizeMake(200, 42), CCTextAlignmentRight, 30, ccc3(255,255,255));
    if (m_endTotalCards>10) {
        coinLabel->setPosition(CCPointMake(420, 592));
    }else{
        coinLabel->setPosition(CCPointMake(420, 572));
    }
    addChild(coinLabel, 1);

    //exp
    snprintf(buf, 99, "%d", exp);
    expNameLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("gainExpTip"), CCSizeMake(600, 42), CCTextAlignmentLeft, 30);
    expNameLabel->setPosition(CCPointMake(100, 522));
    expNameLabel->setAnchorPoint(ccp(0.0, 0.5));
    expNameLabel->setColor(ccWHITE);
    addChild(expNameLabel, 1);
    //expNameLabel->setPosition(ccp(-500, 522));
    if (m_endTotalCards>10) {
        expNameLabel->setPosition(CCPointMake(-500, 542));
    }else{
        expNameLabel->setPosition(CCPointMake(-500, 522));
    }
    
    // 获得经验倍数
    if (exp_effect > 1) {
        sprintf(buf, "CashInLayer_ExpEffect_%d.png",int(exp_effect*10));  // 2, 2.5, 3 -> 20, 25, 30
        spExpEffect = CCSprite::spriteWithFile(buf);
        if (m_endTotalCards>10) {
            spExpEffect->setPosition(CCPointMake(280, 542));
        }else{
            spExpEffect->setPosition(CCPointMake(280, 522));
        }
        spExpEffect->setScale(2);
        spExpEffect->setOpacity(0);
        addChild(spExpEffect, 1);
        isX2Exp = true;   // 添加闪光效果
    }

    //获得经验
    //sprintf(buf,"%ld",CGameData::Inst()->getUsrInfo()->exp - preUserInfo->exp);
    sprintf(buf, "%d",m_desExp);
    expLabel = ActNumTTF::initNumber(CCSizeMake(200, 42), CCTextAlignmentRight, 30, ccc3(255,255,255));
    if (m_endTotalCards>10) {
        expLabel->setPosition(CCPointMake(420, 542));
    }else{
        expLabel->setPosition(CCPointMake(420, 522));
    }
    addChild(expLabel, 1);
    
    //next lv
  //  snprintf(buf, 99, "%d", exp);
    upExpNameLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cashInLayerTip11"), CCSizeMake(600, 42), CCTextAlignmentLeft, 30);
    upExpNameLabel->setPosition(CCPointMake(100, 472));
    upExpNameLabel->setAnchorPoint(ccp(0.0, 0.5));
    upExpNameLabel->setColor(ccWHITE);
    addChild(upExpNameLabel, 1);
    
    if (m_endTotalCards>10) {
        upExpNameLabel->setPosition(CCPointMake(-700, 492));
    }else{
        upExpNameLabel->setPosition(CCPointMake(-700, 472));
    }
    //upExpNameLabel->setPosition(ccp(-700, 472));

    //下次升级经验
    upExpLabel = ActNumTTF::initNumber(CCSizeMake(200, 42), CCTextAlignmentRight, 30, ccc3(255,255,255));
    if (m_endTotalCards>10) {
        upExpLabel->setPosition(CCPointMake(420, 492));
    }else{
        upExpLabel->setPosition(CCPointMake(420, 472));
    }
    addChild(upExpLabel, 1);
    
    
    //获得粮饷
    if (CGameData::Inst()->getNmlDgEndGuildAward() > 0) {
        lbGuildAward = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntrydgend_wealth"), CCSizeMake(600, 42), CCTextAlignmentLeft, 30);
        //lbGuildAward->setPosition(CCPointMake(-700, 422));
        if (m_endTotalCards>10) {
            lbGuildAward->setPosition(CCPointMake(-700, 442));
        }else{
            lbGuildAward->setPosition(CCPointMake(-700, 422));
        }
        lbGuildAward->setAnchorPoint(ccp(0.0, 0.5));
        lbGuildAward->setColor(ccWHITE);
        addChild(lbGuildAward, 1);
        
        numGuildAward = ActNumTTF::initNumber(CCSizeMake(200, 42), CCTextAlignmentRight, 30, ccc3(255,255,255));
        if (m_endTotalCards>10) {
            numGuildAward->setPosition(CCPointMake(420, 442));
        }else{
            numGuildAward->setPosition(CCPointMake(420, 422));
        }
        addChild(numGuildAward, 1);
    }
    else{
        lbGuildAward = NULL;
        numGuildAward = NULL;
    }
    
    //金钱翻倍的动画加载
    CCAnimation* animation;
    CCSpriteFrame* frame;
    if(isX2Money)
    {
        animation = CCAnimation::animation();
        CGameData::Inst()->addBigImageByRGBA4444("moneyX2.plist");
        for(int i=1;i<=12;i++)
        {
            char frameName[30];
            sprintf(frameName, "X200%02d.png", i);
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
            animation->addFrame(frame);
        }
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
        X2Action = CCAnimate::actionWithDuration(1.0, animation, false);
        X2Action->retain();
    }
    
    if(isX2Exp)
    {
        CGameData::Inst()->addBigImageByRGBA4444("expX2.plist");

        animation = CCAnimation::animation();
        //经验动画
        for(int i=1;i<=12;i++)
        {
            char frameName[30];
            sprintf(frameName, "explight00%02d.png", i);
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
            animation->addFrame(frame);
        }
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
        ExpX2Action = CCAnimate::actionWithDuration(1.0, animation, false);
        ExpX2Action->retain();
    }
    
    if(isX2Drop)
    {
        CGameData::Inst()->addBigImageByRGBA4444("dropX2.plist");
        
        animation = CCAnimation::animation();
        //经验动画
        for(int i=1;i<=14;i++)
        {
            char frameName[30];
            sprintf(frameName, "luck00%02d.png", i);
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
            animation->addFrame(frame);
        }
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
        DropX2Action = CCAnimate::actionWithDuration(1.5, animation, false);
        DropX2Action->retain();
    }
    
    
    if(m_lvlUp>0)
    {
        showLvlUpInfo();
    }else {
        bool bFirstPass = false;
        int curFloorStatus = SaveData::Inst()->getCurFloorStatus();
        const char* dgType = SaveData::Inst()->getDgType();
        int iCurFloor = SaveData::Inst()->getFloorId();
        bFirstPass = CGameData::Inst()->isFirstPassBySaveData(curFloorStatus, dgType, iCurFloor);
        
        if(bFirstPass)
        {
            showTongguanInfo();
        }else {
            int* pTrophies = CGameData::Inst()->getTrophies();
            int count = 0;
            CCPoint ptStart(0, 320);
            CCPoint pt;
            while(pTrophies[count] != 0){
                count++;
            }
            
            count += CGameData::Inst()->getGuildItemAwardArr()->count();
            
            if(count !=0)
            {
                showGetCardInfo();
               
                showTime = 3;
            }
        }
        
    }

    schedule(schedule_selector(CashInLayer::OnNumAnimTimer), 1.0);
}

void CashInLayer::OnNumAnimTimer(ccTime dt)
{
    bool bAction = false;
    
    //floor name, room name
    if((floorNameLabel)
       && (floorNameLabel->numberOfRunningActions() == 0)
       && (floorNameLabel->getPosition().x < 0)){
        bAction = true;
        if (m_endTotalCards>10) {
            floorNameLabel->runAction(CCMoveTo::actionWithDuration(0.4,ccp(100, 730-20)));
            roomNameLabel->runAction(CCMoveTo::actionWithDuration(0.4, ccp(520, 652)));
        }else{
            floorNameLabel->runAction(CCMoveTo::actionWithDuration(0.4,ccp(100, 730-40)));
            roomNameLabel->runAction(CCMoveTo::actionWithDuration(0.4, ccp(520, 632)));
        }
        //floorNameLabel->runAction(CCMoveTo::actionWithDuration(0.4,ccp(100, 730-40)));
        //roomNameLabel->runAction(CCMoveTo::actionWithDuration(0.4, ccp(520, 652-20)));
    }
    
    
    if (bAction)
        return;
    //获得金钱
    if((coinNameLabel)
       && (coinNameLabel->numberOfRunningActions() == 0)
       && (coinNameLabel->getPosition().x < 0)){
        bAction = true;
        if (m_endTotalCards>10) {
            coinNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 592)),
                                                         CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbCoinNameAct)),
                                                         NULL));
        }else{
            coinNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 572)),
                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbCoinNameAct)),
                                                 NULL));
        }
    }
    
    if (bAction)
        return;
    //获得经验
    if((expNameLabel)
       && (expNameLabel->numberOfRunningActions() == 0)
       && (expNameLabel->getPosition().x < 0)){
        bAction = true;
        if (m_endTotalCards>10) {
            expNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 542)),
                                                        CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbExpNameAct)),
                                                        NULL));
        }else{
            expNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 522)),
                                                     CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbExpNameAct)),
                                                     NULL));
        }
    }
    
    
    if (bAction)
        return;
    //获得经验 的 倍数
    if((spExpEffect && exp_effect > 1)
       && (spExpEffect->numberOfRunningActions() == 0)
       && !spExpEffect->getOpacity()){
        bAction = true;
        spExpEffect->runAction(CCSequence::actions(CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.3, 0.5), CCFadeIn::actionWithDuration(0.3)),
                                                   CCScaleTo::actionWithDuration(0.01, 1.1),
                                                   CCScaleTo::actionWithDuration(0.05, 0.9),
                                                   NULL));
    }
    
    if (bAction)
        return;
    //下次升级
    if((upExpNameLabel)
       && (upExpNameLabel->numberOfRunningActions() == 0)
       && (upExpNameLabel->getPosition().x < 0)){
        bAction = true;
        if (m_endTotalCards>10) {
            upExpNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 492)),
                                                          CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbUpExpNameAct)),
                                                          NULL));
        }else{
            upExpNameLabel->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 472)),
                                                    CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbUpExpNameAct)),
                                                    NULL));
        }
    }
    
    if (bAction)
        return;
    //获得粮饷
    if((lbGuildAward)
       && (lbGuildAward->numberOfRunningActions() == 0)
       && (lbGuildAward->getPosition().x < 0)){
        bAction = true;
        if (m_endTotalCards>10) {
            lbGuildAward->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 442)),
                                                        CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbGuildAwardAct)),
                                                        NULL));
        }else{
            lbGuildAward->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4,ccp(100, 422)),
                                                      CCCallFunc::actionWithTarget(this, callfunc_selector(CashInLayer::cbGuildAwardAct)),
                                                      NULL));
        }
    }
    
    if (bAction)
        return;
    //获得武将等
    if((moveLayer)
       && (moveLayer->numberOfRunningActions() == 0)
       && (moveLayer->getPosition().x > 640)){
        bAction = true;
        
        moveLayer->runAction(CCMoveTo::actionWithDuration(0.4,ccp(0, 0)));
        
        if(showTime == 3)
        {
            if(isX2Drop)
            {
                runDropX2AnimWithDelay(0.5);
            }
        }
    }
    
    if (!bAction) {
        unschedule(schedule_selector(CashInLayer::OnNumAnimTimer));
        
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
        
        if (CGameData::Inst()->isNewbie()) {
            MainScene::Inst()->showTouchTip();
        }
    }
}

void CashInLayer::cbCoinNameAct()
{
    if(isX2Money == true)
    {
        runMoneyX2Anim();
    }
    
    if(coinLabel){
        coinLabel->setNumber(m_desCoin, 0.5);
    }
}

void CashInLayer::cbExpNameAct()
{
    if(isX2Exp == true)
    {
        runExpX2Anim();
    }
    
    if(expLabel){
        expLabel->setNumber(m_desExp, 0.5);
    }
}

void CashInLayer::cbUpExpNameAct()
{
    if(upExpLabel){
        upExpLabel->setNumber(CGameData::Inst()->getUsrInfo()->next_lv_need_exp, 0.5);
    }
}

void CashInLayer::cbGuildAwardAct()
{
    if(numGuildAward){
        numGuildAward->setNumber(CGameData::Inst()->getNmlDgEndGuildAward(), 0.5);
    }
}

void CashInLayer::runMoneyX2Anim()
{
    CCSprite *moneySp  = CCSprite::spriteWithSpriteFrameName("X20001.png");
    addChild(moneySp);
    moneySp->setPosition(ccp(470, 572));
    moneySp->runAction(X2Action);
    X2Action->release();
}

void CashInLayer::runExpX2Anim()
{
    CCSprite *expSp = CCSprite::spriteWithSpriteFrameName("explight0001.png");
    addChild(expSp);
    expSp->setPosition(ccp(470, 500));
    expSp->runAction(ExpX2Action);
    ExpX2Action->release();
}

void CashInLayer::runDropX2AnimWithDelay(float dt)
{
    CCSprite *expSp = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(expSp);
    expSp->setPosition(ccp(470, 300));
    
    expSp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),DropX2Action,NULL));
    DropX2Action->release();
}

//******************************************************************************
// onExit
//******************************************************************************
void CashInLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}
