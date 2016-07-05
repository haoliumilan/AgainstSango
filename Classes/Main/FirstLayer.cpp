//
//  FirstLayer.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-8-1.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "FirstLayer.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "OcProxy.h"

#include "Arrow.h"
#include "MainScene.h"

using namespace CocosDenshion;

#define enFirstBtnNotice        101
#define enFirstBtnBag           102
#define enFirstBtnGacha         103
#define enFirstBtnMail          104
#define enFirstBtnVideo         105
#define enFirstBtnOther         106
#define enFirstBtnNormal        107
#define enFirstBtnSpecial       108
#define enFirstBtnCntry         109
#define enFirstBtnReward        110


FirstLayer::FirstLayer()
{
    awardTips = NULL;
    
    
    
}

FirstLayer::~FirstLayer()
{

}

void FirstLayer::onEnter()
{
    CCLayer::onEnter();
    
}

void FirstLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}

void FirstLayer::innerEnter()
{

}

void FirstLayer::innerExit()
{
    if(awardTips != NULL)
    {
        awardTips->removeFromParentAndCleanup(true);
        awardTips= NULL;
    }
}

void FirstLayer::showAwardTips(){
    

    int defaultLabel = 0;
    if(awardTips != NULL)
    {
        awardTips->removeFromParentAndCleanup(true);
        awardTips= NULL;
    }
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
    if (signNum+levelUpNum+bonusNum+monthsNum+sysNum > 0) {
        awardTips = OcProxy::Inst()->showTipsAndNum(signNum+levelUpNum+bonusNum+monthsNum+sysNum);
        awardTips->setPosition(ccp(188, 415));
//        addChild(awardTips,998);
        addChild(awardTips);
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
    
}

void FirstLayer::enter()
{
    m_dlgAlert = NULL;
    
   
    MainScene::Inst()->showBottomPromptInfo("first_naviText_1");
    // MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    
        
    CCSprite* spFire = CCSprite::spriteWithFile("first_fire_2.png");
    spFire->setPosition(CCPointMake(0, 140));
    spFire->setAnchorPoint(CCPointZero);
    addChild(spFire);
    
    CCParticleSystemQuad * emitter = CCParticleSystemQuad::particleWithFile("homeFire.plist");;
    emitter->setPosition(ccp(320, 300));
    this->addChild(emitter);
    emitter->setIsAutoRemoveOnFinish(true);

    CCSprite* spWall = CCSprite::spriteWithFile("first_fire_1.png");
    spWall->setPosition(CCPointMake(0, 146));
    spWall->setAnchorPoint(CCPointZero);
    addChild(spWall);
    
    showBianDui();
    
    CCMenuItem* mItemNotice;
    newBtn(mItemNotice, CCPointMake(108, 440), enFirstBtnNotice, "first_button_notice.png");

    CCMenuItem* mItemBag;
    newBtn(mItemBag, CCPointMake(320, 440), enFirstBtnBag, "first_button_bag.png");

    CCMenuItem* mItemGacha;
    newBtn(mItemGacha, CCPointMake(532, 440), enFirstBtnGacha, "first_button_gacha.png");

    CCMenuItem* mItemMailOrReward;
//    if (CGameData::Inst()->getCommonInfo()->get_reward_open)
        newBtn(mItemMailOrReward, CCPointMake(108, 350), enFirstBtnReward, "first_button_reward.png");
//    else
//        newBtn(mItemMailOrReward, CCPointMake(108, 350), enFirstBtnMail, "first_button_mail.png");
    
    CCMenuItem* mItemVideo;
    newBtn(mItemVideo, CCPointMake(320, 350), enFirstBtnVideo, "first_button_parcel.png");
//    newBtn(mItemVideo, CCPointMake(320, 350), enFirstBtnVideo, "first_button_video.png");

    
    CCMenuItem* mItemOther;
    newBtn(mItemOther, CCPointMake(532, 350), enFirstBtnOther, "first_button_other.png");

    CCMenuItem* mItemNormal;
    newBtn(mItemNormal, CCPointMake(108, 150), enFirstBtnNormal, "first_button_normal.png");
    
    CCMenuItem* mItemSpecial;
    if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_special_lv)
    {
        newBtn(mItemSpecial, CCPointMake(320, 150), enFirstBtnSpecial, "first_button_special.png");
        
    }
    else
    newBtn(mItemSpecial, CCPointMake(320, 150), enFirstBtnSpecial, "first_button_special_close.png");
    
    CCMenuItem* mItemCntry;
    if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_guild_lv || CGameData::Inst()->getCntryInfo()->status != 0)
    {
        newBtn(mItemCntry, CCPointMake(532, 150), enFirstBtnCntry, "first_button_cntry.png");
    }
    else
    newBtn(mItemCntry, CCPointMake(532, 150), enFirstBtnCntry, "first_button_cntry_close.png");
    
    CCMenu* menu = CCMenu::menuWithItems(mItemNotice, mItemBag, mItemGacha, mItemMailOrReward, mItemVideo,
                                         mItemOther, mItemNormal, mItemSpecial, mItemCntry, NULL);
    addChild(menu);
    menu->setPosition(CCPointZero);
    
    showAwardTips();
}

void FirstLayer::exit()
{
    innerExit();
    
    removeAllChildrenWithCleanup(true);
}

void FirstLayer::newBtn(CCMenuItem *&menuItem, const CCPoint& position, int tag, const char* image)
{
    CCSprite* spMenu1 = NULL;
    CCSprite* spMenu2 = NULL;
    
    spMenu1 = CCSprite::spriteWithFile(image);
    if (spMenu1) {
        spMenu2 = CCSprite::spriteWithFile(image);
    }
    else{
        spMenu1 = CCSprite::spriteWithSpriteFrameName(image);
        spMenu2 = CCSprite::spriteWithSpriteFrameName(image);
    }

    menuItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                     spMenu2,
                                                     this,
                                                     menu_selector(FirstLayer::btnCallback));
    menuItem->setTag(tag);
    menuItem->setAnchorPoint(CCPointZero);
    menuItem->setPosition(CCPointMake(position.x - spMenu1->getContentSize().width * 0.5,
                                      position.y));
    
}

void FirstLayer::btnCallback(cocos2d::CCObject *pObj)
{
    

    
    
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    
    if(MainScene::Inst()->getChildByTag(98997))
    {
        CCMenuItem* mItem = (CCMenuItem*)pObj;
        
        if(mItem->getTag() == enFirstBtnReward)
        {
            MainScene::Inst()->setLabel(1);
            
             MainScene::Inst()->eneterReward();
            
        }
        
        return;
    }
    
    
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    switch (mItem->getTag()) {
        case enFirstBtnNotice:
            MainScene::Inst()->showOnlyNotice();
            break;

        case enFirstBtnBag:
            MainScene::Inst()->enterBag();
            break;

        case enFirstBtnGacha:
            MainScene::Inst()->enterGachaMain();
            break;

        case enFirstBtnMail:
            MainScene::Inst()->enterMailList();
            break;
            
        case enFirstBtnReward:
            //if (CGameData::Inst()->getCommonInfo()->get_reward_open)
                MainScene::Inst()->eneterReward();
//            else
            //showAlertCanotAward();
            break;

        case enFirstBtnVideo://去合成界面
//            MainScene::Inst()->enterMyVideo();
            
            MainScene::Inst()->enterParcel();

            break;

        case enFirstBtnOther:
            MainScene::Inst()->enterOtherMain();
            break;

        case enFirstBtnNormal:
            MainScene::Inst()->enterNormalArenal();
            break;

        case enFirstBtnSpecial:
            
            if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_special_lv)
            {
                MainScene::Inst()->enterSpecialArenal();
            }
            else
            {
                if (m_dlgAlert == NULL)
                {
                    
                    char buf[100];
                    snprintf(buf, 99, "%s%d%s",
                             CGameData::Inst()->getLanguageValue("open_permit_special_lv1"),
                             CGameData::Inst()->getCommonInfo()->permit_special_lv,
                             CGameData::Inst()->getLanguageValue("open_permit_special_lv2"));
                    
                    
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(FirstLayer::cbAlertDlg),
                                             buf,
                                             false);
                    addChild(m_dlgAlert, 10000);
                    m_dlgAlert->release();
                }
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));

            }
            break;

        case enFirstBtnCntry:
            
            if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_guild_lv || CGameData::Inst()->getCntryInfo()->status != 0)
            {
                MainScene::Inst()->enterCntryMain();
            }
            else
            {
                if (m_dlgAlert == NULL)
                {
                    
                    char buf[100];
                    snprintf(buf, 99, "%s%d%s",
                             CGameData::Inst()->getLanguageValue("open_permit_guild_lv1"),
                             CGameData::Inst()->getCommonInfo()->permit_guild_lv,
                             CGameData::Inst()->getLanguageValue("open_permit_guild_lv2"));
                    
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(FirstLayer::cbAlertDlg),
                                             buf,
                                             false);
                    addChild(m_dlgAlert, 10000);
                    m_dlgAlert->release();
                }
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            }

            
            
            break;

        default:
            break;
    }
}


void FirstLayer::showAlertCanotAward()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(FirstLayer::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("rwd_first_canot_lingj"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void FirstLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
}


void FirstLayer::showBianDui()
{
    CCSprite* spBianDuiBg = CCSprite::spriteWithFile("first_biandui_bg.png");
    spBianDuiBg->setPosition(CCPointMake(320, 670));
    addChild(spBianDuiBg);

    CUserCardInfo* pUsrCardInfo;
    
    stUid* pTeamInf = CGameData::Inst()->getTeamInfByIdx(CGameData::Inst()->getCurTeamIdx());
        
    //leader card
    pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[0].uid);
    showCardHead(pUsrCardInfo, 84);
        
    //副将卡片
    float posX = 241;
    for(int i = 1; i < 5; i++){
        if (pTeamInf[i].uid == 0)
            pUsrCardInfo = NULL;
        else
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[i].uid);
        
        showCardHead(pUsrCardInfo, posX);
        posX += 108;
    }

}

void FirstLayer::showCardHead(CUserCardInfo *pUsrCardInfo, float posX)
{
    TextNode* lbText;
    CCSprite* spCard;
    CCSprite* spCard2;
    char buf[100];
    CCMenuItem* mItem;
    CCMenu* menu;

    if(pUsrCardInfo == NULL){
        spCard = CCSprite::spriteWithSpriteFrameName("fr_box.png");
        if(spCard != NULL){
            spCard2 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
            mItem = CCMenuItemImage::itemFromNormalSprite(spCard,
                                                          spCard2,
                                                          this,
                                                          menu_selector(FirstLayer::cbShowCardHead));
            mItem->setAnchorPoint(CCPointZero);
            mItem->setPosition(CCPointMake(posX - spCard->getContentSize().width * 0.5,
                                           670 - spCard->getContentSize().width * 0.5));
            
            menu = CCMenu::menuWithItem(mItem);
            addChild(menu);
            menu->setPosition(CCPointZero);
        }
    }
    else
    {
        if (pUsrCardInfo != NULL) {
            spCard = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
            if(spCard != NULL){
                spCard2 = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
                mItem = CCMenuItemImage::itemFromNormalSprite(spCard,
                                                              spCard2,
                                                              this,
                                                              menu_selector(FirstLayer::cbShowCardHead));
                mItem->setAnchorPoint(CCPointZero);
                mItem->setPosition(CCPointMake(posX - spCard->getContentSize().width * 0.5,
                                               670 - spCard->getContentSize().width * 0.5));
                
                menu = CCMenu::menuWithItem(mItem);
                addChild(menu);
                menu->setPosition(CCPointZero);
                
                //究极金边
                CCSprite* sp1 = NULL;
                if (pUsrCardInfo->bIsEluTgt) {
                    sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
                }
                else if (pUsrCardInfo->bIsWkTgt) {
                    sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
                }
                
                if (sp1) {
                    spCard->addChild(sp1);
                    sp1->setAnchorPoint(CCPointMake(0, 1));
                    sp1->setPosition(ccp(4, spCard->getContentSize().height-4));
                }
                
                //究极金边
                CCSprite* sp2 = NULL;
                if (pUsrCardInfo->bIsEluTgt) {
                    sp2 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
                }
                else if (pUsrCardInfo->bIsWkTgt) {
                    sp2 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
                }
                
                if (sp2) {
                    spCard2->addChild(sp2);
                    sp2->setAnchorPoint(CCPointMake(0, 1));
                    sp2->setPosition(ccp(4, spCard2->getContentSize().height-4));
                }
                
            }
            
            if (pUsrCardInfo->plus > 0) {
                CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
                
                if (spTmp) {
                    CCSize sz = spTmp->getContentSize();
                    snprintf(buf, 99, "/%d", pUsrCardInfo->plus);
                    CCLabelAtlas* lbPlus = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
                    lbPlus->setAnchorPoint(CCPointMake(1, 0));
                    lbPlus->setPosition(CCPointMake(95, 10));
                    spCard->addChild(lbPlus);
                    
                    CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                                       CCFadeOut::actionWithDuration(0.2),
                                                                                       CCFadeIn::actionWithDuration(0.2),
                                                                                       NULL);
                    lbPlus->runAction(CCRepeatForever::actionWithAction(action1));
                }
            }
            
            if (pUsrCardInfo->lv < pUsrCardInfo->max_lv) {
                snprintf(buf, 99, "Lv.%d", pUsrCardInfo->lv);
            }
            else {
                snprintf(buf, 99, "Lv.%s", CGameData::Inst()->getLanguageValue("max"));
            }
            lbText = TextNode::textWithString(buf, CCSizeMake(110, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(posX, 610));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
            
            snprintf(buf, 99, "%s:%d", CGameData::Inst()->getLanguageValue("cost"), pUsrCardInfo->cost);
            lbText = TextNode::textWithString(buf, CCSizeMake(130, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(posX, 590));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }
}

void FirstLayer::cbShowCardHead(cocos2d::CCObject *pObj)
{
    
    if(MainScene::Inst()->getChildByTag(98997))
    {
        return;
    }
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    MainScene::Inst()->enterTeamInfo();
}





