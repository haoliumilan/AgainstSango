//
//  CMenuBtn.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CMenuBtn.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "SaveData.h"
#include "TitleBar.h"
#include "CCSlider.h"
#include "OcProxy.h"
#include "TipsPopInfo.h"
#include "CCScale9Sprite.h"
#include "TextNode.h"

#define TAGBUYNUM  1307191
#define TAGTOTALPRICETEXTNUM  1307192
#define TAGTOTALPRICETEXTNUM2 1307193

#define TAGXUANSHANGNUM       13072301
#define TAGXUANSHANGKUCUN     13072302




//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(const char* img, const char* name, CCTextAlignment align)
{
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName(img);
    addChild(sp);
    
    CCSprite *labelStr = CCSprite::spriteWithSpriteFrameName(name);
    
    if(labelStr)
        addChild(labelStr);
    else{
        TextNode* lbText = TextNode::textWithString(name, CCSizeMake(520, 100), align, 42);
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
}


//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(CNmlDgInfo* pNmlDgInfo, int indexFloor)
{
    char buf[100];
    TextNode* lbText;

//    //frame
//    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");
//    addChild(spFrame);
    
    //frame
    CCSprite* spFrame;
    if (pNmlDgInfo->bg_type == 1)
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");
    else if (pNmlDgInfo->bg_type == 2)
        spFrame = CCSprite::spriteWithFile("ma_button_skill2.png");
    else if (pNmlDgInfo->bg_type == 3)
        spFrame = CCSprite::spriteWithFile("ma_button_skill4.png");
    else if (pNmlDgInfo->bg_type == 4)
        spFrame = CCSprite::spriteWithFile("ma_button_skill5.png");
    else
        spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");
    addChild(spFrame);
    
    
    //floor name
    snprintf(buf, 99, "floor_%d", indexFloor);
//    CCSprite* spText = CCSprite::spriteWithSpriteFrameName(CGameData::Inst()->getLanguageValue(buf));
//    if (spText) {
//        spText->setPosition(CCPointMake(-173 - 85, 5));
//        spText->setAnchorPoint(CCPointMake(0.0, 0.5));
//        addChild(spText, 2);
//    }
//    else
    {
        lbText = TextNode::textWithString(
                                         // CGameData::Inst()->getLanguageValue(buf),
                                          pNmlDgInfo->name,
                                          CCSizeMake(520, 100),
                                          CCTextAlignmentLeft,
                                          42);
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    
    long remmaintime = pNmlDgInfo->effect_endtime - CGameData::Inst()->getCurrentTime();
    
    if(((CGameData::Inst()->getCurrentTime() >= pNmlDgInfo->effect_starttime)) && (remmaintime > 0)){
        //effect name
        lbText = TextNode::textWithString(pNmlDgInfo->effectname, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
        lbText->setAnchorPoint(CCPointMake(1, 0));
        lbText->setPosition(CCPointMake(245, 0));
        lbText->setColor(ccGREEN);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);

        int hour = remmaintime / 3600;
        int minute = remmaintime / 60;
        int day = remmaintime / (3600*24);
        
        if(hour >=24)
        {
            snprintf(buf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("left"), day, CGameData::Inst()->getLanguageValue("left_day"));
        }
        else if(hour > 0&&hour<24)
            snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
        else
            snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
        
        //remain time
        lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, 0));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    
    CCSprite* spClear = NULL;
    if(pNmlDgInfo->cStatus == 2){
        spClear = CCSprite::spriteWithSpriteFrameName("ma_clear.png");
    }
    else if(pNmlDgInfo->cStatus == 0){
        spClear = CCSprite::spriteWithSpriteFrameName("ma_new.png");
        
        CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                       CCFadeIn::actionWithDuration(0.5),
                                                       NULL);
        spClear->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(spClear){
        spClear->setPosition(CCPointMake(-200, 40));
        addChild(spClear, 10);
    }
}


//******************************************************************************
// showRoomBtn
//******************************************************************************
void CMenuBtn::showRoomBtn(CDgRoomInfo *pRoomInf, int indexFloor, int indexRoom, char bgType)
{
    char buf[100];
    TextNode *lbText;

    //frame

    CCSprite* spFrame;
    if (bgType == 1)
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");
    else if (bgType == 2)
        spFrame = CCSprite::spriteWithFile("ma_button_skill2.png");
    else if (bgType == 3)
        spFrame = CCSprite::spriteWithFile("ma_button_skill4.png");
    else if (bgType == 4)
        spFrame = CCSprite::spriteWithFile("ma_button_skill5.png");
    else
        spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");

    addChild(spFrame);    
    
    //room name
    if (indexFloor == 0 && indexRoom == 0) {
        // 特殊战场的room
        lbText = TextNode::textWithString(pRoomInf->name, CCSizeMake(520, 100), CCTextAlignmentLeft, 42);
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    else {
        // 普通战场的room
        lbText = TextNode::textWithString(pRoomInf->name,//CGameData::Inst()->getLanguageValue(buf),
                                          CCSizeMake(520, 100),
                                          CCTextAlignmentLeft,
                                          42);
        lbText->setPosition(CCPointMake(0, 5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    //stamina
    snprintf(buf, 99, "%s:%d", CGameData::Inst()->getLanguageValue("strength"), pRoomInf->stamina);
    lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
    lbText->setAnchorPoint(CCPointMake(1, 0));
    lbText->setPosition(CCPointMake(245, 0));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);

    //step
    snprintf(buf, 99, "%s:%d", CGameData::Inst()->getLanguageValue("bureau"), pRoomInf->steps);
    lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
    lbText->setAnchorPoint(CCPointMake(1, 1));
    lbText->setPosition(CCPointMake(245, 0));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);

    CCSprite* spClear = NULL;
    if(pRoomInf->cStatus == 2){
        spClear = CCSprite::spriteWithSpriteFrameName("ma_clear.png");
    }
    else if(pRoomInf->cStatus == 0){
        spClear = CCSprite::spriteWithSpriteFrameName("ma_new.png");
        
        CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                          CCFadeIn::actionWithDuration(0.5),
                                                                          NULL);
        spClear->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(spClear){
        spClear->setPosition(CCPointMake(-200, 40));
        addChild(spClear, 10);
    }
}

//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(CDgRoomInfo* pRoomInf, char bgType)
{
    showRoomBtn(pRoomInf, 0, 0, bgType);
}

//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(CDgRoomInfo* pRoomInf, int indexFloor, int indexRoom, char bgType)
{
    showRoomBtn(pRoomInf, indexFloor, indexRoom, bgType);
}

//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(CSpecialDgInfo* pSpecialDgInfo)
{
    //frame
    CCSprite* spFrame;
    if (pSpecialDgInfo->bg_type == 1)
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");
    else if (pSpecialDgInfo->bg_type == 2)
        spFrame = CCSprite::spriteWithFile("ma_button_skill2.png");
    else if (pSpecialDgInfo->bg_type == 3)
        spFrame = CCSprite::spriteWithFile("ma_button_skill4.png");
    else if (pSpecialDgInfo->bg_type == 4)
        spFrame = CCSprite::spriteWithFile("ma_button_skill5.png");
    else
        spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");
    addChild(spFrame);
    
    //floor name
    TextNode* lbText = TextNode::textWithString(pSpecialDgInfo->name, CCSizeMake(520, 100), CCTextAlignmentLeft, 42);
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);

    long remmaintime = 0;
    if (pSpecialDgInfo->endtime - pSpecialDgInfo->starttime < 60 * 60 * 24 * 365 * 3)
        remmaintime = pSpecialDgInfo->endtime - CGameData::Inst()->getCurrentTime();
    
    if((CGameData::Inst()->getCurrentTime() >= pSpecialDgInfo->starttime) && (remmaintime > 0)){
        //effect name
        if(pSpecialDgInfo->effectname){
            lbText = TextNode::textWithString(pSpecialDgInfo->effectname, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
            lbText->setAnchorPoint(CCPointMake(1, 0));
            lbText->setPosition(CCPointMake(245, 0));
            lbText->setColor(ccGREEN);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 2);
        }
    
        int hour = remmaintime / 3600;
        int minute = remmaintime / 60;
        int day = remmaintime / (3600*24);

        char buf[100];
        if(hour >=24)
        {
            snprintf(buf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("left"), day, CGameData::Inst()->getLanguageValue("left_day"));
        }
        else  if(hour > 0&&hour<24)
            snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
        else
            snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
        
        //remain time
        lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, 0));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    else if (CGameData::Inst()->getCurrentTime() < pSpecialDgInfo->starttime && pSpecialDgInfo->is_auto)
    {
        CCLayerColor* clayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 190), spFrame->getContentSize().width-8, spFrame->getContentSize().height-10);
        addChild(clayer,100,TAGCOLOCLAYER);
        clayer->setPosition(ccp(-spFrame->getContentSize().width/2, -spFrame->getContentSize().height/2 + 10));
        
        //effect name
        if(pSpecialDgInfo->effectname){
            lbText = TextNode::textWithString(pSpecialDgInfo->effectname, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
            lbText->setAnchorPoint(CCPointMake(1, 0));
            lbText->setPosition(CCPointMake(245, 0));
            lbText->setColor(ccGREEN);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 2);
        }
        
        long waitime = pSpecialDgInfo->starttime - CGameData::Inst()->getCurrentTime();
        
        int hour = waitime / 3600;
        int minute = waitime / 60;
        int day = waitime / (3600*24);
        
        char buf[100];
        if(hour >=24)
        {
            snprintf(buf, 99, "%d%s", day, CGameData::Inst()->getLanguageValue("left_day_open"));
        }
        else  if(hour > 0&&hour<24)
            snprintf(buf, 99, "%d%s",  hour, CGameData::Inst()->getLanguageValue("left_hour_open"));
        else
            snprintf(buf, 99, "%d%s",  minute, CGameData::Inst()->getLanguageValue("left_minute_open"));
        
        //wait time
        lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 24);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, 0));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    
    CCSprite* spClear = NULL;
    if(pSpecialDgInfo->cStatus == 2){
        spClear = CCSprite::spriteWithSpriteFrameName("ma_clear.png");
    }
    else if(pSpecialDgInfo->cStatus == 0
            &&(!(CGameData::Inst()->getCurrentTime() < pSpecialDgInfo->starttime && pSpecialDgInfo->is_auto))
            )
    {
        spClear = CCSprite::spriteWithSpriteFrameName("ma_new.png");
        
        CCActionInterval* action = (CCActionInterval*)CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                          CCFadeIn::actionWithDuration(0.5),
                                                                          NULL);
        spClear->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(spClear){
        spClear->setPosition(CCPointMake(-200, 40));
        addChild(spClear, 10);
    }
}

//******************************************************************************
// Constructor
//******************************************************************************
CMenuBtn::CMenuBtn(CFriendHelpInfo* pHelperInfo,enFriendRange curType, bool hasGrayCover, int helperNo)
{
    char buf[100];
    
    CCardBaseInfo *card = CGameData::Inst()->getCardBaseInfByCid(pHelperInfo->leadercard_cid);

    //frame
    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //country
    CCSprite* spCountry = NULL;
    switch (pHelperInfo->user_elem) {
        case 1:
        case 2:
        case 3:
            snprintf(buf, 99, "stone%ds.png", pHelperInfo->user_elem);
            spCountry = CCSprite::spriteWithSpriteFrameName(buf);
            break;
            
        default:
            break;
    }
    
    if(spCountry){
        spCountry->setPosition(CCPointMake(-164, 18));
        addChild(spCountry);
    }
    
    //head bg
    CCSprite* spHeadBg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    CCPoint pt(-spFrame->getContentSize().width * 0.5, 0);
    if(spHeadBg){
        pt.x += spHeadBg->getContentSize().width * 0.5;
        pt.x -= 2;
        spHeadBg->setPosition(pt);
        addChild(spHeadBg);
    }
    
    //head
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(pHelperInfo->leadercard_cid);
    if (spHead) {
        spHead->setPosition(pt);
        addChild(spHead);
        
        //究极金边
        CCSprite* sp1 = NULL;
        if (card->bIsEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (card->bIsWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            spHead->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, spHead->getContentSize().height-4));
        }
    }
    
    //leader card lv bg
    CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
    if(spLvBg){
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
    }
    
    //helper name
    CCLabelTTF* lbHelperName = CCLabelTTF::labelWithString(pHelperInfo->name,
                                                           CCSizeMake(240, 26),
                                                           CCTextAlignmentLeft,
                                                           "STHeitiSC-Medium",
                                                           24);
    lbHelperName->setPosition(CCPointMake(-25, 18));
    lbHelperName->setColor(ccBLACK);
    addChild(lbHelperName);
    
//    if(strlen(pHelperInfo->title_name) > 0)
//    {
//        TextNode* tnTitleName = TextNode::textWithString(pHelperInfo->title_name,
//                                                         CCSizeMake(240, 26),
//                                                         CCTextAlignmentLeft,
//                                                         24);
//        tnTitleName->setPosition(CCPointMake(-25, 18));
//        tnTitleName->setColor(ccc3(25,110,150));
//        tnTitleName->setOpacity(0);
//        addChild(tnTitleName);
//        
//        CCActionInterval* action1 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            NULL);
//        lbHelperName->runAction(CCRepeatForever::actionWithAction(action1));
//        
//        CCActionInterval* action2 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            NULL);
//        tnTitleName->runActionInside(CCRepeatForever::actionWithAction(action2));
//    }

    
    //friend type
    CCSprite* spFriendType;
    if(strncmp(pHelperInfo->type, "friend", 6) == 0) {
        spFriendType = CCSprite::spriteWithSpriteFrameName("text_friend.png");
    }
    else {
        spFriendType = CCSprite::spriteWithSpriteFrameName("text_stranger.png");
    }
    spFriendType->setPosition(CCPointMake(-130, -20));
    addChild(spFriendType, 1);
    
    int curCountry = card->cou_name;
    CCSprite* nCountryIcon =CCSprite::spriteWithFile("wu.png");
    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
    if (curType == enFriendRangeCou){
        switch (curCountry) {
            case 0://标志为无
                //  nCountryIcon = CCSprite::spriteWithFile("wu.png"); //国家属性图标
                break;
            case 1://标志为汉
                nCountryIcon = CCSprite::spriteWithFile("hanchao.png"); //国家属性图标
                
                break;
            case 2://标志为魏
                nCountryIcon = CCSprite::spriteWithFile("weiguo.png"); //国家属性图标
                
                break;
            case 3://标志为蜀
                nCountryIcon = CCSprite::spriteWithFile("shuguo.png"); //国家属性图标
                
                break;
            case 4://标志为吴
                nCountryIcon = CCSprite::spriteWithFile("wuguo.png"); //国家属性图标
                
                break;
            case 5://标志为群
                nCountryIcon = CCSprite::spriteWithFile("qunying.png"); //国家属性图标
                
                break;
            case 6://标志为晋
                nCountryIcon = CCSprite::spriteWithFile("jindai.png"); //国家属性图标
                break;
            default:
                break;
        }
        nCountryIcon->setScale(0.8);
        nCountryIcon->setAnchorPoint(CCPointMake(0.5, 0.5));
        nCountryIcon->setPosition(pt);
        addChild(nCountryIcon,1);
        
        if(pHelperInfo->plus>0)
        {
            CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
            sprintf(buf, "/%d",pHelperInfo->plus);
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* plusNum = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
            plusNum->setAnchorPoint(CCPointMake(0.5, 0.5));
            plusNum->setPosition(pt);
            addChild(plusNum, 2);
            plusNum->setOpacity(0);
            
            nCountryIcon->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),NULL)));
            plusNum->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),NULL)));
        }

        
    }else{

    
    //friend lv and others
    //计算攻击力 恢复力 hp
    
    long recoverbase = card->recover;
    float fRecoverGrowth = card->recover_growth;
    int curRecover = recoverbase + (pHelperInfo->leadercardlv - 1) * fRecoverGrowth + pHelperInfo->pl_recover * RECPLUSGROWTH;
    
    long hpbase = card->hp;
    float fHpGrowth = card->hp_growth;
    int curHp = hpbase + (pHelperInfo->leadercardlv - 1) * fHpGrowth + pHelperInfo->pl_hp * HPPLUSGROWTH;
    
    long attackbase = card->attack;
    float fAtkGrowth = card->attack_growth;
    int curAtk = attackbase + (pHelperInfo->leadercardlv - 1) * fAtkGrowth + pHelperInfo->pl_attack * ATKPLUSGROWTH;
    
   
    
    if(curType == enFriendRangePro)
    {
        if (pHelperInfo->leadercardlv == card->max_lv) {
            sprintf(buf, "()*+");
        }
        else {
            sprintf(buf, ",-%d", pHelperInfo->leadercardlv);
        }
        
    }else if(curType == enFriendRangeAtk){
        
        sprintf(buf, "%d",curAtk);     
        
    }else if(curType == enFriendRangeHp){
        
        sprintf(buf, "%d",curHp);     
        
    }else if(curType == enFriendRangeRec){
    
        sprintf(buf, "%d",curRecover);     

    }
    
    CCLabelAtlas* lbLeaderLv;
//    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
        lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
        lbLeaderLv->setPosition(pt);
        addChild(lbLeaderLv, 1);
        
    }
    
    if(pHelperInfo->plus>0)
    {
        CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
        sprintf(buf, "/%d",pHelperInfo->plus);
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* plusNum = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
        plusNum->setAnchorPoint(CCPointMake(0.5, 0.5));
        plusNum->setPosition(pt);
        addChild(plusNum, 2);
        plusNum->setOpacity(0);
        
        lbLeaderLv->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),NULL)));
        plusNum->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),NULL)));
    }
 }
    // helper lv
    sprintf(buf, "-./%d", pHelperInfo->lv);
    spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbFriendLv->setAnchorPoint(CCPointMake(1, 0.5));
        pt.x = 260;
        pt.y = 18;
        lbFriendLv->setPosition(pt);
        addChild(lbFriendLv, 1);
    }
    
    //gacha point
    sprintf(buf, "%d:;<", pHelperInfo->gacha_pt);
    spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbGachaPoint = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbGachaPoint->setAnchorPoint(CCPointMake(1, 0.5));
        pt.x = 100;
        pt.y = -20;
        lbGachaPoint->setPosition(pt);
        addChild(lbGachaPoint, 1);
    }
    // FiveTeam
    if (getChildByTag(101)) {
        removeChildByTag(101, true);
    }
    if (hasGrayCover) {
        CCLayerColor* grayLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 116);
        grayLayer->setPosition(ccp(-320, -58));
        grayLayer->setAnchorPoint(CCPointZero);
        grayLayer->setTag(101);
        addChild(grayLayer);
    }
    if (helperNo) {
        if (getChildByTag(100)) {
            removeChildByTag(100, true);
        }
        CCSprite* numberTips = OcProxy::Inst()->showTipsAndNum(helperNo);
        numberTips->setPosition(ccp(-240, 0));
        numberTips->setTag(100);
        addChild(numberTips);
    } else {
        if (getChildByTag(100)) {
            removeChildByTag(100, true);
        }
    }
        
    
}
void CMenuBtn::setHelperNo(bool hasGrayCover, int helperNo)
{
    // FiveTeam
    if (getChildByTag(101)) {
        removeChildByTag(101, true);
    }
    if (hasGrayCover) {
        CCLayerColor* grayLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 116);
        grayLayer->setPosition(ccp(-320, -58));
        grayLayer->setAnchorPoint(CCPointZero);
        grayLayer->setTag(101);
        addChild(grayLayer);
    }
    if (helperNo) {
        if (getChildByTag(100)) {
            removeChildByTag(100, true);
        }
        CCSprite* numberTips = OcProxy::Inst()->showTipsAndNum(helperNo);
        numberTips->setPosition(ccp(-240, 0));
        numberTips->setTag(100);
        addChild(numberTips);
    } else {
        if (getChildByTag(100)) {
            removeChildByTag(100, true);
        }
    }
}

CFriendInf * CMenuBtn::getSchUsrInfo(){
    return m_pSchUsrInf;
}

CMenuBtn::CMenuBtn(CFriendInf* pSchUsrInf, enFriendRange curType, bool bFriendApplyBtn)
{    
    m_pSchUsrInf = pSchUsrInf;
    char buf[100];
    
    CCardBaseInfo* cardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(pSchUsrInf->leadercard_cid);

    //frame
    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //head bg
    CCSprite* spHeadBg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    CCPoint pt(-spFrame->getContentSize().width * 0.5, 0);
    if(spHeadBg){
        spHeadBg->setPosition(CCPointMake(pt.x + spHeadBg->getContentSize().width * 0.5 - 2, pt.y));
        addChild(spHeadBg);
    }

    //head
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(pSchUsrInf->leadercard_cid);

    if(spHead){
        pt.x += spHead->getContentSize().width * 0.5;
        pt.x -= 2;
        spHead->setPosition(pt);
        addChild(spHead);
        
        //究极金边
        CCSprite* sp1 = NULL;
        if (cardBaseInfo->bIsEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (cardBaseInfo->bIsWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            spHead->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, spHead->getContentSize().height-4));
        }
    
        // leader card lv or other bg
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
        
        
        int curCountry = cardBaseInfo->cou_name;
        CCSprite* nCountryIcon =CCSprite::spriteWithFile("wu.png");
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        if (curType == enFriendRangeCou){
            switch (curCountry) {
                case 0://标志为无
                    //  nCountryIcon = CCSprite::spriteWithFile("wu.png"); //国家属性图标
                    break;
                case 1://标志为汉
                    nCountryIcon = CCSprite::spriteWithFile("hanchao.png"); //国家属性图标
                    
                    break;
                case 2://标志为魏
                    nCountryIcon = CCSprite::spriteWithFile("weiguo.png"); //国家属性图标
                    
                    break;
                case 3://标志为蜀
                    nCountryIcon = CCSprite::spriteWithFile("shuguo.png"); //国家属性图标
                    
                    break;
                case 4://标志为吴
                    nCountryIcon = CCSprite::spriteWithFile("wuguo.png"); //国家属性图标
                    
                    break;
                case 5://标志为群
                    nCountryIcon = CCSprite::spriteWithFile("qunying.png"); //国家属性图标
                    
                    break;
                case 6://标志为晋
                    nCountryIcon = CCSprite::spriteWithFile("jindai.png"); //国家属性图标
                    break;
                default:
                    break;
            }
            nCountryIcon->setScale(0.8);
            nCountryIcon->setAnchorPoint(CCPointMake(0.5, 0.5));
            nCountryIcon->setPosition(pt);
            addChild(nCountryIcon,1);
            
            if(pSchUsrInf->card_info.plus>0)
            {
                CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
                sprintf(buf, "/%d",pSchUsrInf->card_info.plus);
                CCSize sz = spTmp->getContentSize();
                CCLabelAtlas* plusNum = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
                plusNum->setAnchorPoint(CCPointMake(0.5, 0.5));
                plusNum->setPosition(pt);
                addChild(plusNum, 2);
                plusNum->setOpacity(0);
                
                nCountryIcon->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),NULL)));
                plusNum->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),NULL)));
            }
            
            
        }else{
        
        
        // leader card lv or other
        if(curType == enFriendRangePro) {
            if (pSchUsrInf->leadercardlv == cardBaseInfo->max_lv) {
                sprintf(buf, "()*+");
            }
            else {
                sprintf(buf, ",-%d", pSchUsrInf->leadercardlv);
            }
        }
        else if(curType == enFriendRangeAtk)
//            sprintf(buf, "%ld",pSchUsrInf->card_info.attack);
            sprintf(buf, "%ld",pSchUsrInf->card_info.attack.get());
        else if(curType == enFriendRangeHp)
//            sprintf(buf, "%ld",pSchUsrInf->card_info.hp);
            sprintf(buf, "%ld",pSchUsrInf->card_info.hp.get());
        else if(curType == enFriendRangeRec)
            sprintf(buf, "%ld",pSchUsrInf->card_info.recover);
        else if(curType == enFriendRangeLT)
        {
            if (pSchUsrInf->leadercardlv == cardBaseInfo->max_lv) {
                sprintf(buf, "()*+");
            }
            else {
                sprintf(buf, ",-%d", pSchUsrInf->leadercardlv);
            }
        }
        
       // CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        CCLabelAtlas* lbLeaderLv;
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');

            lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
            lbLeaderLv->setPosition(pt);
            addChild(lbLeaderLv, 1);
        }
        
        if(pSchUsrInf->card_info.plus>0)
        {
            CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
            sprintf(buf, "/%d",pSchUsrInf->card_info.plus);
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* plusNum = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
            plusNum->setAnchorPoint(CCPointMake(0.5, 0.5));
            plusNum->setPosition(pt);
            addChild(plusNum, 2);
            plusNum->setOpacity(0);
            
            lbLeaderLv->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),NULL)));
            plusNum->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),CCFadeIn::actionWithDuration(0.5),CCDelayTime::actionWithDuration(2.0),CCFadeOut::actionWithDuration(0.5),NULL)));
        }
        
    }
    }
    //helper name
    CCLabelTTF* lbHelperName = CCLabelTTF::labelWithString(pSchUsrInf->name,
                                                           CCSizeMake(240, 26),
                                                           CCTextAlignmentLeft,
                                                           "STHeitiSC-Medium",
                                                           24);
    lbHelperName->setPosition(CCPointMake(-25, 18));
    lbHelperName->setColor(ccBLACK);
    addChild(lbHelperName);
    
    if(strlen(pSchUsrInf->title_name) > 0)
    {
        TextNode* tnTitleName = TextNode::textWithString(pSchUsrInf->title_name,
                                                         CCSizeMake(240, 26),
                                                         CCTextAlignmentLeft,
                                                         24);
        tnTitleName->setPosition(CCPointMake(-25, 18));
        tnTitleName->setColor(ccc3(25,110,150));
        tnTitleName->setOpacity(0);
        addChild(tnTitleName);
        
        CCActionInterval* action1 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                            CCFadeOut::actionWithDuration(0.5),
                                                                            CCDelayTime::actionWithDuration(2.0),
                                                                            CCFadeIn::actionWithDuration(0.5),
                                                                            NULL);
        lbHelperName->runAction(CCRepeatForever::actionWithAction(action1));
        
        CCActionInterval* action2 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                            CCFadeIn::actionWithDuration(0.5),
                                                                            CCDelayTime::actionWithDuration(2.0),
                                                                            CCFadeOut::actionWithDuration(0.5),
                                                                            NULL);
        tnTitleName->runActionInside(CCRepeatForever::actionWithAction(action2));
    }
    
    char buf2[50];
    //country
    CCSprite* spCountry = NULL;
    switch (pSchUsrInf->user_elem) {
        case 1:
        case 2:
        case 3:
            snprintf(buf2, 49, "stone%ds.png", pSchUsrInf->user_elem);
            spCountry = CCSprite::spriteWithSpriteFrameName(buf2);
            break;
            
        default:
            break;
    }
    
    if(spCountry){
        spCountry->setPosition(CCPointMake(-164, 18));
        addChild(spCountry);
    }

    
    //friend lv
    sprintf(buf, "-./%d", pSchUsrInf->lv);
    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbFriendLv->setAnchorPoint(CCPointMake(0, 0.5));
        if (bFriendApplyBtn) {
            pt.x = -170;
            pt.y = -20;
        }
        else {
            pt.x = 140;
            pt.y = 15;
        }
        lbFriendLv->setPosition(pt);
        addChild(lbFriendLv, 1);
    }

//login time
    if (bFriendApplyBtn == false) {
        if(pSchUsrInf->login_time > 0){
            unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
            long lTimeOffset = ulCurTime - pSchUsrInf->login_time;
            
            if(lTimeOffset < 0)
                lTimeOffset = 0;
            
            int hour = lTimeOffset / 3600;
            int minute = lTimeOffset / 60;
            int day = lTimeOffset / (3600*24);
            
            if(day > 0){
                sprintf(buf, "+,-./%d>?", day);
            }
            else if(hour > 0){
                sprintf(buf, "+,-./%d<=?", hour);
            }
            else {
                sprintf(buf, "+,-./%d:;?", minute);
            }
            
            CCSprite* spTmp = CCSprite::spriteWithFile("timenum_login.png");
            if(spTmp){
                CCSize sz = spTmp->getContentSize();
                CCLabelAtlas* lbLoginTime = CCLabelAtlas::labelWithString(buf, "timenum_login.png", sz.width / 21, sz.height, '+');
                lbLoginTime->setAnchorPoint(CCPointMake(0, 0.5));
                lbLoginTime->setPosition(CCPointMake(-145, -20));
                addChild(lbLoginTime, 1);
            }        
        }    
    }
}


CMenuBtn::CMenuBtn(const char * price, const char * iap_id, bool isTapjoy)
{
    CCSprite* spFrame = NULL;
    CCPoint pt;
    CCSprite* sp = NULL;

    //元宝图
    if(strncmp(iap_id, kProductId00, 99) == 0
       || strncmp(iap_id, kProductId09, 99) == 0
       || strncmp(iap_id, kProductId11, 99) == 0
       || strncmp(iap_id, kProductId12, 99) == 0
       || strncmp(iap_id, kProductId13, 99) == 0)
    {
        
    }
        else
    {
        spFrame = CCSprite::spriteWithFile("ma_charge_bg.png");
        spFrame->setPosition(CCPointMake(-6, 0));
        addChild(spFrame);
        
        sp = CCSprite::spriteWithSpriteFrameName("id_gold.png");
    }
    
    pt.x = -243;
    pt.y = 0;
    if(sp){
        sp->setPosition(pt);
        addChild(sp);
    }
    
    if(isTapjoy)
    {
        CCSprite *freeSp = CCSprite::spriteWithFile("ma_free.png");
        freeSp->setPosition(ccp(-272, 23));
        addChild(freeSp);
        
        if(spFrame){
            removeChild(spFrame, true);
        }
    }
    
    if(!isTapjoy)
    {
        //price
        CCLabelTTF* lbRmb = CCLabelTTF::labelWithString(price, "Arial", 26);
        pt.x = 115;
        pt.y = 15;
        lbRmb->setPosition(pt);
        lbRmb->setColor(ccWHITE);
        lbRmb->setAnchorPoint(ccp(1, 0.5));
        addChild(lbRmb, 11);
        
        CCLabelTTF* lbRmbBk = CCLabelTTF::labelWithString(price, "Arial", 26);
        pt.x = 117;
        pt.y = 13;
        lbRmbBk->setPosition(pt);
        lbRmbBk->setColor(ccBLACK);
        lbRmbBk->setAnchorPoint(ccp(1, 0.5));
        addChild(lbRmbBk);
        
        
        TextNode* lbText = TextNode::textWithString("", CCSizeMake(520, 100), CCTextAlignmentLeft, 42);
        
        lbText->setPosition(CCPointMake(200, 40));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText);
        
       
        //元宝数量,优惠等
        CCSprite* spCoinCnt = NULL;
        CCSprite* spDiscount = NULL;
        CCSprite* spFanHuan = NULL;
        CCSprite* spCoinName = NULL;
        CCSprite* spHead = NULL;
        if(strncmp(iap_id, kProductId00, 99) == 0
           || strncmp(iap_id, kProductId09, 99) == 0
           || strncmp(iap_id, kProductId11, 99) == 0
           || strncmp(iap_id, kProductId12, 99) == 0
           || strncmp(iap_id, kProductId13, 99) == 0)
        {
            if ( strncmp(iap_id, kProductId12, 99) == 0
                || strncmp(iap_id, kProductId13, 99) == 0
                || strncmp(iap_id, kProductId00, 99) == 0) {
                spCoinCnt = CCSprite::spriteWithFile("ma_gold_bg.png");
                spDiscount = CCSprite::spriteWithFile("ma_discount_hot.png");
                spHead = CCSprite::spriteWithFile("ma_pack.png");
                
            }
            if (strncmp(iap_id, kProductId09, 99) == 0
                || strncmp(iap_id, kProductId11, 99) == 0) {
                spCoinCnt = CCSprite::spriteWithFile("ma_purple_shop.png");
                spFanHuan = CCSprite::spriteWithFile("ma_fanhuan30.png");
                spDiscount = CCSprite::spriteWithFile("ma_over_time.png");
                spHead = CCSprite::spriteWithFile("ma_pack.png");
                
            }
            
           
            if(spCoinCnt){
                addChild(spCoinCnt);
                spCoinCnt->setPosition(ccp(-3, 0));
                spCoinCnt = NULL;
                
                if(spFrame){
                    removeChild(spFrame, true);
                }
            }
            
            if(spHead){
                spHead->setPosition(CCPointMake(-243, 0));
                addChild(spHead);
            }

            
            if (strncmp(iap_id, kProductId00, 99) == 0){
                spCoinName = CCSprite::spriteWithFile("ma_gold_30.png");
            }
            else if (strncmp(iap_id, kProductId12, 99) == 0){
                spCoinName = CCSprite::spriteWithFile("ma_gold_mengjiang.png");
            }
            else if (strncmp(iap_id, kProductId13, 99) == 0){
                spCoinName = CCSprite::spriteWithFile("ma_gold_shenjiang.png");
            }
            else if (strncmp(iap_id, kProductId11, 99) == 0){
                spCoinName = CCSprite::spriteWithFile("ma_zhaocaijinbao_pag.png");
                if (lbText) {
                    removeChild(lbText, true);
                }
                char buf[200];

                
                unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
                long lTimeOffset =CGameData::Inst()->getUsrInfo()->bSYuebao[enYueBao_zhaocaijinbao] - ulCurTime ;
                
                if(lTimeOffset < 0)
                    lTimeOffset = 0;
                
                
                int day = lTimeOffset / (3600*24);
                if (lTimeOffset - 3600 *24*day > 0) {
                    day += 1;
                }

                sprintf(buf, CGameData::Inst()->getLanguageValue("shopTip31"),day);
                lbText = TextNode::textWithString(buf, CCSizeMake(200, 40), CCTextAlignmentCenter, 22);
                lbText->setAnchorPoint(CCPointMake(0.5, 0.5));
                lbText->setPosition(ccp(208, 42));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText,11);

            }
        
            else if (strncmp(iap_id, kProductId09, 99) == 0){
                spCoinName = CCSprite::spriteWithFile("ma_caiyungungun_pag.png");
                if (lbText) {
                    removeChild(lbText, true);
                }
                char buf[200];
                
   
                
                
                unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
                long lTimeOffset =CGameData::Inst()->getUsrInfo()->bSYuebao[enYueBao_caiyungungun] - ulCurTime  ;
                if(lTimeOffset < 0)
                    lTimeOffset = 0;
               
                int day = lTimeOffset / (3600*24);
                if (lTimeOffset - 3600 *24*day > 0) {
                    day += 1;
                }
                
                sprintf(buf, CGameData::Inst()->getLanguageValue("shopTip31"),day);
                lbText = TextNode::textWithString(buf, CCSizeMake(200, 40), CCTextAlignmentCenter, 22);
                lbText->setAnchorPoint(CCPointMake(0.5, 0.5));
                lbText->setPosition(ccp(208, 42));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText,11);
            }
            if(spCoinName){
                addChild(spCoinName);
                spCoinName->setPosition(ccp(-110, 2));
                 spCoinCnt = NULL;
            }
            
            if(spDiscount){
                addChild(spDiscount, 10);
                spDiscount->setPosition(ccp(200, 40));
                spDiscount = NULL;
            }
            if (spFanHuan) {
                addChild(spFanHuan);
                spFanHuan->setPosition(ccp(46, -16));
                spFanHuan = NULL;
            }
            
          
            
            
            if(lbRmb)
                removeChild(lbRmb, true);
            
            if(lbRmbBk)
                removeChild(lbRmbBk, true);
            
            //price
            lbRmb = CCLabelTTF::labelWithString(price, CCSizeMake(200, 36), CCTextAlignmentRight, "Arial", 36);
            lbRmb->setColor(ccWHITE);
            addChild(lbRmb, 11);
            
            lbRmbBk = CCLabelTTF::labelWithString(price, CCSizeMake(200, 36), CCTextAlignmentRight, "Arial", 36);
            lbRmbBk->setColor(ccBLACK);
            addChild(lbRmbBk);
            
            lbRmb->setAnchorPoint(CCPointMake(1, 0.5));
            lbRmb->setPosition(ccp(100, 16));
            
            lbRmbBk->setAnchorPoint(CCPointMake(1, 0.5));
            lbRmbBk->setPosition(ccp(102, 14));
        }
        else if(strncmp(iap_id, kProductId01, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_6.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_0.png");
        }
        else if(strncmp(iap_id, kProductId02, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_32.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_6.png");
        }
        else if(strncmp(iap_id, kProductId03, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_75.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_10.png");
        }
        else if(strncmp(iap_id, kProductId04, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_230.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_16.png");
        }
        else if(strncmp(iap_id, kProductId05, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_395.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_20.png");
        }
        else if(strncmp(iap_id, kProductId06, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_810.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_25.png");
        }
        else if(strncmp(iap_id, kProductId08, 99) == 0){
            spCoinCnt = CCSprite::spriteWithFile("ma_gold_300.png");
            spDiscount = CCSprite::spriteWithFile("ma_discount_18.png");
        }
      
        if(spCoinCnt){
            addChild(spCoinCnt);
            spCoinCnt->setPosition(ccp(-136, 0));
        }
        
        if(spDiscount){
            addChild(spDiscount);
            spDiscount->setPosition(ccp(16, -16));
        }
        
        CCSprite* spValue = NULL;
        CCSprite* spPop = NULL;
        //超值
        if(CGameData::Inst()->isIapValuePack(iap_id)){
            spValue = CCSprite::spriteWithFile("ma_value.png");
            if(spValue){
                addChild(spValue, 100);
                spValue->setPosition(ccp(-262, 18));
            }
        }
        
        //热销
        else if(CGameData::Inst()->isIapBestSell(iap_id)){
            spPop = CCSprite::spriteWithFile("ma_popular.png");
            if(spPop){
                addChild(spPop, 100);
                spPop->setPosition(ccp(-262, 18));
            }
        }
        
        if(spValue || spPop){
            CCSprite* spStar = CCSprite::spriteWithFile("ma_star.png");
            if(spStar){
                addChild(spStar, 110);
                spStar->setPosition(CCPointMake(-240, 50));
                spStar->setScale(1.2);
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeOut::actionWithDuration(0.7),
                                                                                   CCFadeIn::actionWithDuration(1.0),
                                                                                   NULL);
                spStar->runAction(CCRepeatForever::actionWithAction(action));
            }
            
            spStar = CCSprite::spriteWithFile("ma_star.png");
            if(spStar){
                addChild(spStar, 110);
                spStar->setPosition(CCPointMake(-268, 20));
                spStar->setScale(0.8);
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeOut::actionWithDuration(0.7),
                                                                                   CCFadeIn::actionWithDuration(0.8),
                                                                                   NULL);
                spStar->runAction(CCRepeatForever::actionWithAction(action));
            }
            
            spStar = CCSprite::spriteWithFile("ma_star.png");
            if(spStar){
                addChild(spStar, 110);
                spStar->setPosition(CCPointMake(-296, 17));
                spStar->setScale(1.8);
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeOut::actionWithDuration(0.7),
                                                                                   CCFadeIn::actionWithDuration(0.6),
                                                                                   NULL);
                spStar->runAction(CCRepeatForever::actionWithAction(action));
            }
        }
    }
    
    bool isFirstCharge = CGameData::Inst()->getUsrInfo()->first_charge;

    if(isFirstCharge
       && (isTapjoy == false)
       && (strncmp(iap_id, kProductId00, 99) != 0)
       && (strncmp(iap_id, kProductId09, 99) != 0)
       && (strncmp(iap_id, kProductId11, 99) != 0)
       && (strncmp(iap_id, kProductId12, 99) != 0)
       && (strncmp(iap_id, kProductId13, 99) != 0))
    {
        sp = CCSprite::spriteWithFile("first_charge.png");
        pt.x = 200;
        pt.y = 38;
        if(sp){
            sp->setPosition(pt);
            addChild(sp, 10);
        }
    }
    
    
    
}

CMenuBtn::CMenuBtn(CRwdTopMemberInfo* rwdTopMemberInf, int idx)
{
    char buf[100];
    
    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //head
    if (idx > 0 && idx <= 3)
        snprintf(buf, 99, "cntry_rank%d.png", idx);
    else if (idx <= 10)
        snprintf(buf, 99, "cntry_rank4.png");
    else
        snprintf(buf, 99, "cntry_rank5.png");
    
    CCSprite* spHead = CCSprite::spriteWithFile(buf);
    
    if(spHead){
        float spHeadPX = spHead->getContentSize().width * 0.5 - spFrame->getContentSize().width * 0.5;
        spHead->setPosition(CCPointMake(spHeadPX, 8));
        addChild(spHead, 1);
        
        if (idx > 3)
        {
            sprintf(buf, "%d", idx);
            CCSprite* spTmp = CCSprite::spriteWithFile("rank_num.png");
            CCLabelAtlas* lbRank;
            if(spTmp){
                CCSize sz = spTmp->getContentSize();
                lbRank = CCLabelAtlas::labelWithString(buf,
                                                       "rank_num.png",
                                                       sz.width / 10,
                                                       sz.height,
                                                       '0');
                lbRank->setAnchorPoint(CCPointMake(0.5, 0.5));
                lbRank->setPosition(CCPointMake(spHead->getContentSize().width * 0.5,
                                                spHead->getContentSize().height * 0.5 + 10));
                spHead->addChild(lbRank);
                
                if (idx > 999)
                    lbRank->setScale(0.75);
            }
        }
        
        // leader card lv or other bg
        CCPoint pt(spHead->getPosition().x, 0);
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
        
        // leader card lv or other
        sprintf(buf, ",-%d", rwdTopMemberInf->lv);
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        CCLabelAtlas* lbLeaderLv;
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
            
            lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
            lbLeaderLv->setPosition(pt);
            addChild(lbLeaderLv);
        }
    }
    
    TextNode* userName = TextNode::textWithString(rwdTopMemberInf->name.c_str(),24);
    userName->setAnchorPoint(ccp(0, 0.5));
    userName->setPosition(CCPointMake(-168,19));
    userName->setColor(ccWHITE);
    addChild(userName, 1);
    
    //获得贡献值
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("reward_top_gongxian"), rwdTopMemberInf->contribution);
    TextNode* tContribution = TextNode::textWithString(buf,21);
    tContribution->setAnchorPoint(ccp(0, 0.5));
    tContribution->setPosition(CCPointMake(-172,-22));
    tContribution->setColor(ccc3(136, 12, 0));
    addChild(tContribution, 1);
    
    // 所属公会
    TextNode* lbLearderName = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_top_gonghui"),24);
    lbLearderName->setAnchorPoint(ccp(0.5, 0.5));
    lbLearderName->setPosition(CCPointMake(180,18));
    lbLearderName->setColor(ccBLACK);
    addChild(lbLearderName, 1);
    
    snprintf(buf, 99, "%s", rwdTopMemberInf->guild_name.c_str());
    TextNode* LeaderName = TextNode::textWithString(buf,24);
    LeaderName->setAnchorPoint(ccp(0.5, 0.5));
    LeaderName->setPosition(CCPointMake(176,-18));
    LeaderName->setColor(ccBLACK);
    addChild(LeaderName, 1);
}


CMenuBtn::CMenuBtn(CCntryBaseInfo* cntryInfo)
{
    char buf[100];
    
    //frame
    CCountryInfo* myCntryInfo = CGameData::Inst()->getCntryInfo();
    
    CCSprite* spFrame;
    if (strcmp(cntryInfo->cid, myCntryInfo->cid) == 0)
        spFrame = CCSprite::spriteWithFile("rank_myself_bg.png");
    else
        spFrame = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //head
    if (cntryInfo->rank > 0 && cntryInfo->rank <= 3)
        snprintf(buf, 99, "cntry_rank%d.png", cntryInfo->rank);
    else if (cntryInfo->rank <= 10)
        snprintf(buf, 99, "cntry_rank4.png");
    else
        snprintf(buf, 99, "cntry_rank5.png");
    
    CCSprite* spHead = CCSprite::spriteWithFile(buf);
    
    if(spHead){
        float spHeadPX = spHead->getContentSize().width * 0.5 - spFrame->getContentSize().width * 0.5;
        spHead->setPosition(CCPointMake(spHeadPX, 8));
        addChild(spHead, 1);
        
        if (cntryInfo->rank > 3)
        {
            sprintf(buf, "%d", cntryInfo->rank);
            CCSprite* spTmp = CCSprite::spriteWithFile("rank_num.png");
            CCLabelAtlas* lbRank;
            if(spTmp){
                CCSize sz = spTmp->getContentSize();
                lbRank = CCLabelAtlas::labelWithString(buf,
                                                        "rank_num.png",
                                                        sz.width / 10,
                                                        sz.height,
                                                        '0');
                lbRank->setAnchorPoint(CCPointMake(0.5, 0.5));
                lbRank->setPosition(CCPointMake(spHead->getContentSize().width * 0.5,
                                                spHead->getContentSize().height * 0.5 + 10));
                spHead->addChild(lbRank);
                
                if (cntryInfo->rank > 999)
                    lbRank->setScale(0.75);
            }
        }
        
        // leader card lv or other bg
        CCPoint pt(spHead->getPosition().x, 0);
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
        
        // leader card lv or other
        sprintf(buf, ",-%d", cntryInfo->lv);
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        CCLabelAtlas* lbLeaderLv;
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
            
            lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
            lbLeaderLv->setPosition(pt);
            addChild(lbLeaderLv);
        }
    }
    
    //country name
//    TextNode* lbCntryName = TextNode::textWithString(cntryInfo->name,
//                                                      CCSizeMake(240, 26),
//                                                      CCTextAlignmentLeft,
//                                                      24);
    
    CCLabelTTF* lbCntryName = CCLabelTTF::labelWithString(cntryInfo->name, CCSizeMake(240, 26),CCTextAlignmentLeft, FONTSTHEITISCM, 24);
    
    lbCntryName->setPosition(CCPointMake(-45, 18));
  //  lbCntryName->setShadowColor(ccBLACK);
    lbCntryName->setColor(ccWHITE);
    addChild(lbCntryName);
    
    // leader name:   
    snprintf(buf, 99, "%s: ",CGameData::Inst()->getLanguageValue("cntry_general_alone"));
    CCLabelTTF* lbLearderName = CCLabelTTF::labelWithString(buf,
                                                            CCSizeMake(240, 26),
                                                            CCTextAlignmentLeft,
                                                            "STHeitiSC-Medium",
                                                            24);
    lbLearderName->setPosition(CCPointMake(-45, -20));
    lbLearderName->setColor(ccc3(0, 115, 175));
    addChild(lbLearderName);
    
    // :leader name
    snprintf(buf, 99, "%s", cntryInfo->leader_name);
    CCLabelTTF* LeaderName = CCLabelTTF::labelWithString(buf,
                                                         CCSizeMake(200, 26),
                                                         CCTextAlignmentLeft,
                                                         "STHeitiSC-Medium",
                                                         24);
    LeaderName->setPosition(CCPointMake(0, -20));
    LeaderName->setColor(ccc3(0, 115, 175));
    addChild(LeaderName);

    
}

CMenuBtn::CMenuBtn(CMember* memberInfo,bool isManage)
{
    char buf[100];
    
    //frame
    CCSprite* spFrame = CCSprite::spriteWithFile("bg_memberList.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    // head bg
    CCSprite* spHeadBg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    CCPoint pt(-spFrame->getContentSize().width * 0.5, 0);
    if(spHeadBg){
        spHeadBg->setPosition(CCPointMake(pt.x + spHeadBg->getContentSize().width * 0.5 + 18, pt.y));
        addChild(spHeadBg);
    }
    
    //head
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(memberInfo->card_info.cid);
    if(spHead){
        pt.x += spHead->getContentSize().width * 0.5;
        pt.x -= 2;
        spHead->setPosition(spHeadBg->getPosition());
        addChild(spHead);
        
        // leader card lv or other bg
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
        
        // leader card lv or other
        sprintf(buf, ",-%d", memberInfo->card_info.lv);
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        CCLabelAtlas* lbLeaderLv;
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
            
            lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
            lbLeaderLv->setPosition(pt);
            addChild(lbLeaderLv, 1);
        }
    }
        
    //member name
//    TextNode* lbCntryName = TextNode::textWithString(memberInfo->name,
//                                                     CCSizeMake(210, 26),
//                                                     CCTextAlignmentLeft,
//                                                     24);
    
    CCLabelTTF* lbMemberName = CCLabelTTF::labelWithString(memberInfo->name,
                                                           CCSizeMake(210, 26),
                                                           CCTextAlignmentLeft,
                                                           FONTSTHEITISCM,
                                                           25);
    
    lbMemberName->setAnchorPoint(CCPointMake(0, 0.5));
    lbMemberName->setPosition(CCPointMake(-150, 32));
    lbMemberName->setColor(ccBLACK);
    addChild(lbMemberName);
    
//    if(strlen(memberInfo->title_name) > 0)
//    {
//        TextNode* tnTitleName = TextNode::textWithString(memberInfo->title_name,
//                                                         CCSizeMake(210, 26),
//                                                         CCTextAlignmentLeft,
//                                                         25);
//        tnTitleName->setAnchorPoint(CCPointMake(0, 0.5));
//        tnTitleName->setPosition(CCPointMake(-150, 32));
//        tnTitleName->setColor(ccc3(25,110,150));
//        tnTitleName->setOpacity(0);
//        addChild(tnTitleName);
//        
//        CCActionInterval* action1 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            NULL);
//        lbMemberName->runAction(CCRepeatForever::actionWithAction(action1));
//        
//        CCActionInterval* action2 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            NULL);
//        tnTitleName->runActionInside(CCRepeatForever::actionWithAction(action2));
//    }

    // status
    if (memberInfo->status == 1)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_king"));
    else if (memberInfo->status == 2)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_vice_alone"));
    else// memberInfo->status == 3
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_member_alone"));
    TextNode* lbStatus = TextNode::textWithString(buf,
                                                  CCSizeMake(240, 26),
                                                  CCTextAlignmentRight,
                                                  24);
    lbStatus->setPosition(CCPointMake(-10, 32));
    addChild(lbStatus);

    // user lv
    sprintf(buf, "-./%d", memberInfo->lv);
    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbFriendLv->setAnchorPoint(CCPointMake(1, 0.5));
        lbFriendLv->setPosition(CCPointMake(260, 30));
        addChild(lbFriendLv, 1);
    }
    
    // login time
    if(memberInfo->login_time > 0){
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        long lTimeOffset = ulCurTime - memberInfo->login_time;
        
        if(lTimeOffset < 0)
            lTimeOffset = 0;
        
        int hour = lTimeOffset / 3600;
        int minute = lTimeOffset / 60;
        int day = lTimeOffset / (3600*24);
        
        if(day > 0){
            sprintf(buf, "+,-./%d>?", day);
        }
        else if(hour > 0){
            sprintf(buf, "+,-./%d<=?", hour);
        }
        else {
            sprintf(buf, "+,-./%d:;?", minute);
        }
        
        CCSprite* spTmp = CCSprite::spriteWithFile("timenum_login.png");
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* lbLoginTime = CCLabelAtlas::labelWithString(buf, "timenum_login.png", sz.width / 21, sz.height, '+');
            lbLoginTime->setAnchorPoint(CCPointMake(0, 0.5));
            lbLoginTime->setPosition(CCPointMake(-150, -10));
            addChild(lbLoginTime, 1);
        }
    }
    
    // contribution
    sprintf(buf, ",-./%ld", memberInfo->contibution);
    spTmp = CCSprite::spriteWithFile("contribution_num.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "contribution_num.png", sz.width / 14, sz.height, ',');
        lbFriendLv->setAnchorPoint(CCPointMake(0, 0.5));
        lbFriendLv->setPosition(CCPointMake(-150, -40));
        addChild(lbFriendLv, 1);
    }
}

CMenuBtn::CMenuBtn(CRewardMember* memberInfo)
{
    char buf[100];
    
    //frame
    CCSprite* spFrame = CCSprite::spriteWithFile("bg_memberList.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    // head bg
    CCSprite* spHeadBg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    CCPoint pt(-spFrame->getContentSize().width * 0.5, 0);
    if(spHeadBg){
        spHeadBg->setPosition(CCPointMake(pt.x + spHeadBg->getContentSize().width * 0.5 + 18, pt.y));
        addChild(spHeadBg);
    }
    
    //head
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(memberInfo->card_info.cid);
    if(spHead){
        pt.x += spHead->getContentSize().width * 0.5;
        pt.x -= 2;
        spHead->setPosition(spHeadBg->getPosition());
        addChild(spHead);
        
        // leader card lv or other bg
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        pt = spHead->getPosition();
        pt.y -= spHead->getContentSize().height * 0.5;
        pt.y += 8;
        spLvBg->setPosition(pt);
        addChild(spLvBg);
        
        // leader card lv or other
        sprintf(buf, ",-%d", memberInfo->card_info.lv);
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        CCLabelAtlas* lbLeaderLv;
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLeaderLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
            
            lbLeaderLv->setAnchorPoint(CCPointMake(0.5, 0.5));
            lbLeaderLv->setPosition(pt);
            addChild(lbLeaderLv, 1);
        }
    }
    
    //member name    
    CCLabelTTF* lbMemberName = CCLabelTTF::labelWithString(memberInfo->name,
                                                           CCSizeMake(210, 26),
                                                           CCTextAlignmentLeft,
                                                           FONTSTHEITISCM,
                                                           25);
    
    lbMemberName->setAnchorPoint(CCPointMake(0, 0.5));
    lbMemberName->setPosition(CCPointMake(-150, 32));
    lbMemberName->setColor(ccBLACK);
    addChild(lbMemberName);
    
//    if(strlen(memberInfo->title_name) > 0)
//    {
//        TextNode* tnTitleName = TextNode::textWithString(memberInfo->title_name,
//                                                         CCSizeMake(210, 26),
//                                                         CCTextAlignmentLeft,
//                                                         25);
//        tnTitleName->setAnchorPoint(CCPointMake(0, 0.5));
//        tnTitleName->setPosition(CCPointMake(-150, 32));
//        tnTitleName->setColor(ccc3(25,110,150));
//        tnTitleName->setOpacity(0);
//        addChild(tnTitleName);
//        
//        CCActionInterval* action1 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            NULL);
//        lbMemberName->runAction(CCRepeatForever::actionWithAction(action1));
//        
//        CCActionInterval* action2 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeIn::actionWithDuration(0.5),
//                                                                            CCDelayTime::actionWithDuration(2.0),
//                                                                            CCFadeOut::actionWithDuration(0.5),
//                                                                            NULL);
//        tnTitleName->runActionInside(CCRepeatForever::actionWithAction(action2));
//    }
    
    // status
    if (memberInfo->status == 1)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_king"));
    else if (memberInfo->status == 2)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_vice_alone"));
    else// memberInfo->status == 3
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_member_alone"));
    TextNode* lbStatus = TextNode::textWithString(buf,
                                                  CCSizeMake(240, 26),
                                                  CCTextAlignmentRight,
                                                  24);
    lbStatus->setPosition(CCPointMake(-10, 32));
    addChild(lbStatus);
    
    // user lv
    sprintf(buf, "-./%d", memberInfo->lv);
    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbFriendLv->setAnchorPoint(CCPointMake(1, 0.5));
        lbFriendLv->setPosition(CCPointMake(260, 30));
        addChild(lbFriendLv, 1);
    }
        
    // contribution
    TextNode* lbContribution = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_contribution_done"),
                                                        24);
    lbContribution->setColor(ccc3(100, 30, 0));
    lbContribution->setAnchorPoint(CCPointMake(0, 0.5));
    lbContribution->setPosition(CCPointMake(-150, -10));
    addChild(lbContribution);
    
    snprintf(buf, 99, "%ld/%ld", memberInfo->done_contribution,
             memberInfo->target_contribution);
    lbContribution = TextNode::textWithString(buf, 24);
    lbContribution->setColor(ccc3(100, 30, 0));
    lbContribution->setAnchorPoint(CCPointMake(0, 0.5));
    lbContribution->setPosition(CCPointMake(-150, -40));
    addChild(lbContribution);
    
    // 已领取 or 已完成
    CCSprite* spTips = NULL;
    if (memberInfo->bRecieveReward)
        spTips = CCSprite::spriteWithFile("reward_recieve.png");
    else if (memberInfo->bFinishReward)
        spTips = CCSprite::spriteWithFile("reward_done.png");
    
    if (spTips)
    {
        spTips->setPosition(CCPointMake(210, -20));
        addChild(spTips);
    }
}

CMenuBtn::CMenuBtn()
{
    
}



void CMenuBtn::showOrgLianShouChang(CCntryDgInfo * pCntryDgInfo)
{
    //同盟练手场
    CCSprite* spFrame = NULL;
    spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");
    addChild(spFrame);
    
    // title
    TextNode* lbText = TextNode::textWithString(pCntryDgInfo->dg_name,
                                                CCSizeMake(520, 100),
                                                CCTextAlignmentLeft,
                                                42);
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
     lbText->setPosition(CCPointMake(0, 15));
    // stars
    int int_num_star = pCntryDgInfo -> star;
    for (int i=0; i<int_num_star; i++)
    {
        CCSprite* sp_star = CCSprite::spriteWithSpriteFrameName("id_star.png");
        sp_star -> setAnchorPoint(CCPointZero);
        sp_star -> setPosition(CCPointMake(47+i*29, 97));
        spFrame   -> addChild(sp_star);
    }
    
    char buf1[30];
    snprintf(buf1, 29, "%s:", CGameData::Inst()->getLanguageValue("cntry_open_costStone"));
    TextNode* text1 = TextNode::textWithString(buf1, CCSizeMake(250, 30), CCTextAlignmentLeft, 20);
    text1 -> setAnchorPoint(CCPointZero);
    text1 -> setPosition(CCPointMake(44, 25));
    text1 -> setColor(ccYELLOW);
    spFrame -> addChild(text1, 0, kCntryMoneyTitle);
    
    snprintf(buf1, 29, "%ld/%ld",
             pCntryDgInfo->gulid_coins,
             CGameData::Inst()->getCntryInfo()->cntry_coin);
    TextNode* textM = TextNode::textWithString(buf1, CCSizeMake(400, 30), CCTextAlignmentLeft, 20);
    textM -> setAnchorPoint(CCPointZero);
    textM -> setPosition(CCPointMake(220, 25));
    textM -> setColor(ccWHITE);
    spFrame -> addChild(textM, 0, kCntryMoney);
    
    long remmaintime = 0;
    if (pCntryDgInfo->bIsFinish ){
        if (CGameData::Inst()->getCntryInfo()->status == 1
            || CGameData::Inst()->getCntryInfo()->status == 2) {
            remmaintime = pCntryDgInfo->closed_time - CGameData::Inst()->getCurrentTime();
            snprintf(buf1, 29, "%s",
                     CGameData::Inst()->getLanguageValue("left"));
        }
    }
    else{
        spFrame->removeChildByTag(kCntryMoney, true);
        spFrame->removeChildByTag(kCntryMoneyTitle, true);
       remmaintime = pCntryDgInfo->closed_time - CGameData::Inst()->getCurrentTime();
        snprintf(buf1, 29, "%s", CGameData::Inst()->getLanguageValue("left"));
    }
    
    if (remmaintime > 0) {
        int hour = remmaintime / 3600;
        int minute = remmaintime / 60;
        int day = remmaintime / (3600*24);
        
        char buf[100];
        if(hour >=24)
        {
            snprintf(buf, 99, "%s%d%s",
                     buf1, day,
                     CGameData::Inst()->getLanguageValue("left_day"));
        }
        else if(hour > 0&&hour<24)
            snprintf(buf, 99, "%s%d%s",
                     buf1, hour,
                     CGameData::Inst()->getLanguageValue("left_hour"));
        else
            snprintf(buf, 99, "%s%d%s",
                     buf1, minute,
                     CGameData::Inst()->getLanguageValue("left_minute"));
        
        //remain time
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 20);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, -6));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);

    }
    
    
}

void CMenuBtn::showOrgLuanWu(CCntryDgInfo *pCntryDgInfo)
{
    // 乱舞
    CCSprite* spFrame = NULL;
    long curTime = CGameData::Inst()->getCurrentTime();
    
    long remmaintime = 0;
    char tipsBuf[100];
    char buf[100];
    bool bShowNeedSupply = false;
    if (pCntryDgInfo->start_time > curTime)
    {   // 特殊保卫战还有XXXX时间开始
        spFrame = CCSprite::spriteWithFile("ma_button_skill3.png");
        
        remmaintime = pCntryDgInfo->start_time - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_begin"));
        
        bShowNeedSupply = true;
    }
    else if (pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 < curTime)
    {   // 特殊保卫战持续时间已经结束, 下次保卫战还未开启
        spFrame = CCSprite::spriteWithFile("ma_button_skill3.png");
        
        TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("jiFengLuanWu_over"),
                                                    CCSizeMake(490, 30),
                                                    CCTextAlignmentRight,
                                                    20);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, -6));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
        
        bShowNeedSupply = true;
    }
    else if (pCntryDgInfo->dungeon_tms > pCntryDgInfo->dungeon_tms_config
             && !pCntryDgInfo->bIsFinish)
    {   // 特殊保卫战还在开启期间，但还没打完，还剩XXXX时间关闭
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");
        
        remmaintime = pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_end"));
        
        bShowNeedSupply = false;
    }
    else
    {   // 特殊保卫战还在开启期间，但已经打完，或者还没有开启战场，还剩XXXX时间关闭
        
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");
        
        remmaintime = pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_end"));
        
        bShowNeedSupply = true;
    }
    
    addChild(spFrame);
    
    // title
    TextNode* lbText = TextNode::textWithString(
                                                pCntryDgInfo->dg_name,
                                                CCSizeMake(520, 100),
                                                CCTextAlignmentLeft,
                                                42);
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    if (pCntryDgInfo->contribute > 0 && bShowNeedSupply){
        lbText->setPosition(CCPointMake(0, 15));
        
        char buf1[30];
        snprintf(buf1, 29, "%s:", CGameData::Inst()->getLanguageValue("cntry_open_money"));
        TextNode* text1 = TextNode::textWithString(buf1, CCSizeMake(250, 30), CCTextAlignmentLeft, 20);
        text1 -> setAnchorPoint(CCPointZero);
        text1 -> setPosition(CCPointMake(44, 25));
        text1 -> setColor(ccYELLOW);
        spFrame -> addChild(text1, 0, kCntryMoneyTitle);

        snprintf(buf, 99, "%ld/%ld",
                 CGameData::Inst()->getCntryInfo()->cntry_contribution,
                 pCntryDgInfo->contribute);
        TextNode* textM = TextNode::textWithString(buf, CCSizeMake(400, 30), CCTextAlignmentLeft, 20);
        textM -> setAnchorPoint(CCPointZero);
        textM -> setPosition(CCPointMake(220, 25));
        textM -> setColor(ccWHITE);
        spFrame -> addChild(textM, 0, kCntryMoney);
    }
    else{
        lbText->setPosition(CCPointMake(0, 5));
    }
        
    // stars
    int int_num_star = pCntryDgInfo -> star;
    for (int i=0; i<int_num_star; i++)
    {
        CCSprite* sp_star = CCSprite::spriteWithSpriteFrameName("id_star.png");
        sp_star -> setAnchorPoint(CCPointZero);
        sp_star -> setPosition(CCPointMake(47+i*29, 97));
        spFrame   -> addChild(sp_star);
    }
    
    
    
}

void CMenuBtn::showOrgJiFeng(CCntryDgInfo *pCntryDgInfo)
{
    // 疾风
    CCSprite* spFrame = NULL;
    long curTime = CGameData::Inst()->getCurrentTime();
    
    long remmaintime = 0;
    char tipsBuf[100];
    if (pCntryDgInfo->start_time > curTime)
    {   // 特殊保卫战还有XXXX时间开始
        spFrame = CCSprite::spriteWithFile("ma_button_skill3.png");
        
        remmaintime = pCntryDgInfo->start_time - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_begin"));
    }
    else if (pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 < curTime)
    {   // 特殊保卫战已经结束, 下次保卫战还未开启
        spFrame = CCSprite::spriteWithFile("ma_button_skill3.png");

        TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("jiFengLuanWu_over"),
                                                    CCSizeMake(490, 30),
                                                    CCTextAlignmentRight,
                                                    20);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, -6));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
    else if (pCntryDgInfo->dungeon_tms < pCntryDgInfo->dungeon_tms_config
             || !pCntryDgInfo->bIsFinish)
    {   // 特殊保卫战还在开启期间，但还没打完，还剩XXXX时间关闭
        spFrame = CCSprite::spriteWithFile("ma_button_skill.png");

        remmaintime = pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_end"));
    }
    else
    {
        spFrame = CCSprite::spriteWithFile("ma_button_skill3.png");

        remmaintime = pCntryDgInfo->start_time + pCntryDgInfo->last_time * 3600 - curTime;
        snprintf(tipsBuf, 99, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_end"));
    }
    
    addChild(spFrame);
    
    // title
    TextNode* lbText = TextNode::textWithString(
                                                pCntryDgInfo->dg_name,
                                                CCSizeMake(520, 100),
                                                CCTextAlignmentLeft,
                                                42);
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
        
    // stars
    int int_num_star = pCntryDgInfo -> star;
    for (int i=0; i<int_num_star; i++)
    {
        CCSprite* sp_star = CCSprite::spriteWithSpriteFrameName("id_star.png");
        sp_star -> setAnchorPoint(CCPointZero);
        sp_star -> setPosition(CCPointMake(47+i*29, 97));
        spFrame   -> addChild(sp_star);
    }
    
    // left time
    if (remmaintime > 0) {
        int hour = remmaintime / 3600;
        int minute = remmaintime / 60;
        int day = remmaintime / (3600*24);
        
        char buf[100];
        if(hour >=24)
        {
            snprintf(buf, 99, "%s%d%s%s",
                     CGameData::Inst()->getLanguageValue("left"),
                     day,
                     CGameData::Inst()->getLanguageValue("left_day"),
                     tipsBuf);
        }
        else if(hour > 0&&hour<24)
            snprintf(buf, 99, "%s%d%s%s",
                     CGameData::Inst()->getLanguageValue("left"),
                     hour,
                     CGameData::Inst()->getLanguageValue("left_hour"),
                     tipsBuf);
        else
            snprintf(buf, 99, "%s%d%s%s",
                     CGameData::Inst()->getLanguageValue("left"),
                     minute,
                     CGameData::Inst()->getLanguageValue("left_minute"),
                     tipsBuf);
        
        //remain time
        TextNode* lbText = TextNode::textWithString(buf,
                                                    CCSizeMake(490, 30),
                                                    CCTextAlignmentRight,
                                                    20);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, -6));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
}

void CMenuBtn::showOrgNormal(CCntryDgInfo *pCntryDgInfo)
{
    // frame
    CCSprite* spFrame = NULL;
    spFrame = CCSprite::spriteWithSpriteFrameName("ma_button.png");
    addChild(spFrame);
    
    // title
    TextNode* lbText = TextNode::textWithString(pCntryDgInfo->dg_name,
                                                CCSizeMake(520, 100),
                                                CCTextAlignmentLeft,
                                                42);
    lbText->setPosition(CCPointMake(0, 5));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    if (pCntryDgInfo->bIsFinish ){
        lbText->setPosition(CCPointMake(0, 15));
    }
    else{
        lbText->setPosition(CCPointMake(0, 5));
    }
    
    // stars
    int int_num_star = pCntryDgInfo -> star;
    for (int i=0; i<int_num_star; i++)
    {
        CCSprite* sp_star = CCSprite::spriteWithSpriteFrameName("id_star.png");
        sp_star -> setAnchorPoint(CCPointZero);
        sp_star -> setPosition(CCPointMake(47+i*29, 97));
        spFrame   -> addChild(sp_star);
    }

    char buf1[30];
    snprintf(buf1, 29, "%s:", CGameData::Inst()->getLanguageValue("cntry_open_money"));
    TextNode* text1 = TextNode::textWithString(buf1, CCSizeMake(250, 30), CCTextAlignmentLeft, 20);
    text1 -> setAnchorPoint(CCPointZero);
    text1 -> setPosition(CCPointMake(44, 25));
    text1 -> setColor(ccYELLOW);
    spFrame -> addChild(text1, 0, kCntryMoneyTitle);
    
    snprintf(buf1, 29, "%ld/%ld",
             CGameData::Inst()->getCntryInfo()->cntry_contribution,
             pCntryDgInfo->contribute);
    TextNode* textM = TextNode::textWithString(buf1, CCSizeMake(400, 30), CCTextAlignmentLeft, 20);
    textM -> setAnchorPoint(CCPointZero);
    textM -> setPosition(CCPointMake(220, 25));
    textM -> setColor(ccWHITE);
    spFrame -> addChild(textM, 0, kCntryMoney);
    
    long remmaintime = 0;
    if (pCntryDgInfo->bIsFinish ){
        remmaintime = pCntryDgInfo->end_time + pCntryDgInfo->cold_time * 60 - CGameData::Inst()->getCurrentTime();
        snprintf(buf1, 29, "%s",
                 CGameData::Inst()->getLanguageValue("jiFengLuanWu_cd"));
    }
    else{
        spFrame->removeChildByTag(kCntryMoney, true);
        spFrame->removeChildByTag(kCntryMoneyTitle, true);
        
        remmaintime = pCntryDgInfo->open_time + pCntryDgInfo->last_time * 3600 - CGameData::Inst()->getCurrentTime();
        
        snprintf(buf1, 29, "%s", CGameData::Inst()->getLanguageValue("left"));
    }
    
    if (remmaintime > 0) {
        int hour = remmaintime / 3600;
        int minute = remmaintime / 60;
        int day = remmaintime / (3600*24);
        
        char buf[100];
        if(hour >=24)
        {
            snprintf(buf, 99, "%s%d%s",
                     buf1, day,
                     CGameData::Inst()->getLanguageValue("left_day"));
        }
        else if(hour > 0&&hour<24)
            snprintf(buf, 99, "%s%d%s",
                     buf1, hour,
                     CGameData::Inst()->getLanguageValue("left_hour"));
        else
            snprintf(buf, 99, "%s%d%s",
                     buf1, minute,
                     CGameData::Inst()->getLanguageValue("left_minute"));
        
        //remain time
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(490, 30), CCTextAlignmentRight, 20);
        lbText->setAnchorPoint(CCPointMake(1, 1));
        lbText->setPosition(CCPointMake(245, -6));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 2);
    }
}

CMenuBtn::CMenuBtn(CCntryDgInfo* pCntryDgInfo)
{

    if (strcmp(pCntryDgInfo->dg_type, "1") == 0)
        showOrgJiFeng(pCntryDgInfo);
    else if (strcmp(pCntryDgInfo->dg_type, "2") == 0)
        showOrgLuanWu(pCntryDgInfo);
    else if (strcmp(pCntryDgInfo->dg_type, "3") == 0)//新增2.12
        showOrgLianShouChang(pCntryDgInfo);
    else
        showOrgNormal(pCntryDgInfo);
}

CMenuBtn::CMenuBtn(enBuffTypeDef buffType)   // buff升级列表项
{
    char buf[100];
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    //frame
    CCSprite* spFrame = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //head
    if (buffType == enBuffType_Stamina)
        snprintf(buf, 99, "buff_action.png");
    else if (buffType == enBuffType_Attack)
        snprintf(buf, 99, "buff_attack.png");
    else if (buffType == enBuffType_Recover)
        snprintf(buf, 99, "buff_recover.png");
    else if (buffType == enBuffType_Hp)
        snprintf(buf, 99, "buff_hp.png");
    
    CCPoint pt(-spFrame->getContentSize().width * 0.5, 0);
    CCSprite* spHead = CCSprite::spriteWithFile(buf);
    
    if(spHead){
        pt.x += spHead->getContentSize().width * 0.5;
        pt.x -= 2;
        spHead->setPosition(pt);
        addChild(spHead);
    }

    // name
    if (buffType == enBuffType_Stamina)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_stamina_promotion"));
    else if (buffType == enBuffType_Attack)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_attack_promotion"));
    else if (buffType == enBuffType_Recover)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_recover_promotion"));
    else if (buffType == enBuffType_Hp)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_hp_promotion"));

    TextNode* lbHelperName = TextNode::textWithString(buf,
                                                      CCSizeMake(240, 26),
                                                      CCTextAlignmentLeft,
                                                      24);
    lbHelperName->setPosition(CCPointMake(-45, 18));
    lbHelperName->setColor(ccBLACK);
    addChild(lbHelperName);
    
    // lv
    sprintf(buf, "-./%d", cntryInfo->buffInfo[buffType].lv);
    CCSprite* spTmp = CCSprite::spriteWithFile("lvnum_friend.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbFriendLv = CCLabelAtlas::labelWithString(buf, "lvnum_friend.png", sz.width / 16, sz.height, '-');
        lbFriendLv->setAnchorPoint(CCPointMake(1, 0.5));
        lbFriendLv->setPosition(CCPointMake(100, 18));
        addChild(lbFriendLv, 1);
    }
    
    // need
    //如果 cntryInfo->buffInfo[buffType].lv >=10的时候，提示 已满级
    if (cntryInfo->buffInfo[buffType].lv >= 10)
    {
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_buffLv_max"));
        TextNode* lbNeedLevelUp = TextNode::textWithString(buf, 20);
        lbNeedLevelUp->setColor(ccc3(100, 30, 0));
        lbNeedLevelUp->setPosition(CCPointMake(-45, -20));
        addChild(lbNeedLevelUp);
    }
    else
    {
        snprintf(buf, 99, "%s%d%s、%ld%s",CGameData::Inst()->getLanguageValue("cntry_levelup_need"),  cntryInfo->buffInfo[buffType].coin, CGameData::Inst()->getLanguageValue("cntry_gold_alone"), cntryInfo->buffInfo[buffType].exp, CGameData::Inst()->getLanguageValue("cntry_exp_alone"));
        TextNode* lbNeedLevelUp = TextNode::textWithString(buf,
                                                           CCSizeMake(300, 60),
                                                           CCTextAlignmentLeft,
                                                           18);
        lbNeedLevelUp->setColor(ccc3(100, 30, 0));
        lbNeedLevelUp->setAnchorPoint(CCPointMake(0, 0.5));
        lbNeedLevelUp->setPosition(CCPointMake(-180, -20));
        addChild(lbNeedLevelUp);
    }

}

CMenuBtn::CMenuBtn(CWelfareInfo* welfareInfo)      // 福利列表项
{
    char buf[200];
    // frame
    CCSprite* spFrame = CCSprite::spriteWithFile("bg_memberList.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    // head
    snprintf(buf, 199, "welfareIcon_%d.png", welfareInfo->imageId);
    CCSprite* spHead = CCSprite::spriteWithFile(buf);
    spHead->setPosition(CCPointMake(-220, 0));
    addChild(spHead);
    
    // need contribution
    snprintf(buf, 199, "%s：%ld", CGameData::Inst()->getLanguageValue("cntry_need_gold"), welfareInfo->needSupply);
    TextNode* tnContribution = TextNode::textWithString(buf, 24);
    tnContribution->setColor(ccBLACK);
    tnContribution->setAnchorPoint(CCPointMake(0, 0.5));
    tnContribution->setPosition(CCPointMake(-148, 32));
    addChild(tnContribution);
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    if (cntryInfo->user_supply < welfareInfo->needSupply) {
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                           CCFadeTo::actionWithDuration(0.2, 255),
                                                                           NULL);
        tnContribution->setColor(ccc3(100, 30, 0));
        tnContribution->setShadowColor(ccBLACK);
        tnContribution->runActionInside(CCRepeatForever::actionWithAction(action));
    }
    
    // cd time
    long lastWelfareTime = CGameData::Inst()->getLastWelfareTime(welfareInfo->wid);
    if (lastWelfareTime > 0)
    {
        long retainTime = welfareInfo->cd_time * 3600 - CGameData::Inst()->getCurrentTime() + lastWelfareTime;
        
        if(retainTime > 0) {
            int hour = retainTime / 3600;
            int minute = retainTime / 60;
            int day = retainTime / (3600*24);
            
            char buf[100];
            if(hour >=24)
            {
                snprintf(buf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("cntry_welfare_cd"), day, CGameData::Inst()->getLanguageValue("left_day"));
            }
            else if(hour > 0&&hour<24)
                snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("cntry_welfare_cd"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
            else
                snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("cntry_welfare_cd"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
            
            //remain time
            TextNode* lbText = TextNode::textWithString(buf, 20);
            lbText->setAnchorPoint(CCPointMake(0, 0.5));
            lbText->setPosition(CCPointMake(130, 32));
            lbText->setColor(ccRED);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 2);
        }
    }
    
    // decription
    snprintf(buf, 199, "%s", welfareInfo->description);
    TextNode* tnDecription = TextNode::textWithString(welfareInfo->description,
                                                      CCSizeMake(450, 60),
                                                      CCTextAlignmentLeft,
                                                      24);
    tnDecription->setColor(ccc3(100, 30, 0));
    tnDecription->setAnchorPoint(CCPointMake(0, 0.5));
    tnDecription->setPosition(CCPointMake(-160, -20));
    addChild(tnDecription);

}


CMenuBtn::CMenuBtn(CPostRewardBaseInfo* pPostRewardInfo)
{
    char buf[100];
    
    CCSprite* spFrame = CCSprite::spriteWithFile("postrewardbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    if (pPostRewardInfo->status == 3
        || (pPostRewardInfo->CurJoinPerson >= pPostRewardInfo->MaxJoinPerson && pPostRewardInfo->status == 0)
        || pPostRewardInfo->applyDeadLine <= CGameData::Inst()->getCurrentTime()
        )
    {
        CCLayerColor* clayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 190), spFrame->getContentSize().width-8, spFrame->getContentSize().height-8);
        addChild(clayer,50,TAGCOLOCLAYER);
        clayer->setPosition(ccp(-spFrame->getContentSize().width/2 + 6, -spFrame->getContentSize().height/2 + 4));
    }
    
    //reward_icon
    CCSprite* rewardIcon = CCSprite::spriteWithFile(pPostRewardInfo->strImgName.c_str());
    addChild(rewardIcon,1,TAGXSREWARDICON);
    rewardIcon->setPosition(ccp(-214, 18));
    
    snprintf(buf, 99, "/%d", pPostRewardInfo->itemCount);
    CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbItemCount = CCLabelAtlas::labelWithString(buf,
                                                                  "rewardnum.png",
                                                                  sz.width / 11,
                                                                  sz.height,
                                                                  '/');
        lbItemCount->setAnchorPoint(ccp(0.5, 0));
        lbItemCount->setPosition(CCPointMake(rewardIcon->getContentSize().width/2, -2));
        rewardIcon->addChild(lbItemCount, 1);
    }
    
    //name
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_xs_xuanshangname"), pPostRewardInfo->contribution);
    TextNode* nameText = TextNode::textWithString(buf, 28);
    nameText->setAnchorPoint(ccp(0, 0.5));
    nameText->setPosition(CCPointMake(-260,92));
    nameText->setColor(ccc3(156, 16, 0));
    addChild(nameText, 1);
    
    //报名 deadline
    TextNode* baomingText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_baomingqixian"), 25);
    baomingText->setAnchorPoint(ccp(0, 0.5));
    baomingText->setPosition(CCPointMake(-150,44));
    baomingText->setColor(ccBLACK);
    addChild(baomingText, 1);

    snprintf(buf, 99, "%s",
             OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(pPostRewardInfo->applyDeadLine).c_str());
    TextNode* baomingText2 = TextNode::textWithString(buf, 22);
    baomingText2->setAnchorPoint(ccp(0, 0.5));
    baomingText2->setPosition(CCPointMake(-20,44));
    baomingText2->setColor(ccBLACK);
    addChild(baomingText2, 1);

    //保证金
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_xuyaobaozhengjin"), pPostRewardInfo->ensureYuanBao);
    TextNode* baozhengText = TextNode::textWithString(buf, 25);
    baozhengText->setAnchorPoint(ccp(0, 0.5));
    baozhengText->setPosition(CCPointMake(-150, 9));
    baozhengText->setColor(ccBLACK);
    addChild(baozhengText, 1);
    
    //参与人数
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_canyurenshu"), pPostRewardInfo->CurJoinPerson,
             pPostRewardInfo->MaxJoinPerson);
    TextNode* renshuText = TextNode::textWithString(buf, 25);
    renshuText->setAnchorPoint(ccp(0, 0.5));
    renshuText->setPosition(CCPointMake(-150, -26));
    renshuText->setColor(ccBLACK);
    addChild(renshuText, 1);
    
    //完成期限
    TextNode* wanchengText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_wanchengqixian"),27);
    wanchengText->setAnchorPoint(ccp(0, 0.5));
    wanchengText->setPosition(CCPointMake(-260,-66));
    wanchengText->setColor(ccc3(72, 142, 0));
    addChild(wanchengText, 1);
    
    snprintf(buf, 99, "%s",
             OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(pPostRewardInfo->completeDeadLine).c_str());
    TextNode* wanchengText2 = TextNode::textWithString(buf, 27);
    wanchengText2->setAnchorPoint(ccp(0, 0.5));
    wanchengText2->setPosition(CCPointMake(-260,-98));
    wanchengText2->setColor(ccc3(72, 142, 0));
    addChild(wanchengText2, 1);

}

CMenuBtn::CMenuBtn(GuildShopItemInf* ShopItemInf)
{
    m_GShopItemInf = *ShopItemInf;
    
    CCSprite* spFrame = CCSprite::spriteWithFile("postrewardbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    CCSprite* rewardIcon = CCSprite::spriteWithFile(ShopItemInf->strImgName.c_str());
    addChild(rewardIcon,1,TAGXSREWARDICON);
    rewardIcon->setPosition(ccp(-214, 18));
    
    //name
    TextNode* nameText = TextNode::textWithString(ShopItemInf->strItemName.c_str(),28);
    nameText->setAnchorPoint(ccp(0.5, 0.5));
    nameText->setPosition(CCPointMake(0,92));
    nameText->setColor(ccc3(156, 16, 0));
    addChild(nameText, 1);
    
    float leftPosX = -142;
    float topPosY = 50;
    float offy = 42;
    float fontSize = 23;
    
    char buf[100];
    
    //库存
    TextNode* kuCunText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_baoku_kucun"),fontSize);
    kuCunText->setAnchorPoint(ccp(0, 0.5));
    kuCunText->setPosition(CCPointMake(leftPosX,topPosY));
    kuCunText->setColor(ccBLACK);
    addChild(kuCunText, 1);
    
    snprintf(buf, 99, "%ld", CGameData::Inst()->getGuildItemInStoreCnt(ShopItemInf->strItemId));
    TextNode* kuCunTextNum = TextNode::textWithString(buf,fontSize);
    kuCunTextNum->setAnchorPoint(ccp(0, 0.5));
    kuCunTextNum->setPosition(CCPointMake(leftPosX + kuCunText->getContentSize().width+10,topPosY - offy*0));
    kuCunTextNum->setColor(ccc3(162, 17, 0));
    addChild(kuCunTextNum, 1);
    
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_baoku_levelxianzhi"),ShopItemInf->lLv);
    //购买级别限制
    TextNode* xianZhiText = TextNode::textWithString(buf,fontSize);
    xianZhiText->setAnchorPoint(ccp(0, 0.5));
    xianZhiText->setPosition(CCPointMake(leftPosX+180,topPosY));
    xianZhiText->setColor(ccBLACK);
    addChild(xianZhiText, 1);
    
    //价格
    if (ShopItemInf->lCoin != 0)
    {
        snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_baoku_jiageyuanbao"),ShopItemInf->lCoin);
    }
    else
    {
        snprintf(buf, 99, CGameData::Inst()->getLanguageValue("cntry_baoku_jiagebt"),ShopItemInf->lCntrbt);
    }
    
    TextNode* priceText = TextNode::textWithString(buf,fontSize);
    priceText->setAnchorPoint(ccp(0, 0.5));
    priceText->setPosition(CCPointMake(leftPosX,topPosY - offy*1));
    priceText->setColor(ccBLACK);
    addChild(priceText, 1);
    
    //总价
    TextNode* totalPriceText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_baoku_zongjia"),fontSize);
    totalPriceText->setAnchorPoint(ccp(0, 0.5));
    totalPriceText->setPosition(CCPointMake(leftPosX,topPosY - offy*2));
    totalPriceText->setColor(ccBLACK);
    addChild(totalPriceText, 1);
    
    
    TextNode* totalPriceTextNum = TextNode::textWithString("0",fontSize);
    totalPriceTextNum->setAnchorPoint(ccp(0, 0.5));
    totalPriceTextNum->setPosition(CCPointMake(leftPosX + totalPriceText->getContentSize().width+10,topPosY - offy*2));
    totalPriceTextNum->setColor(ccc3(66, 142, 0));
    addChild(totalPriceTextNum, 1,TAGTOTALPRICETEXTNUM);
    
    if (ShopItemInf->lCoin != 0)
        snprintf(buf, 99, "/%ld",CGameData::Inst()->getCntryInfo()->cntry_coin);
    else
        snprintf(buf, 99, "/%ld",CGameData::Inst()->getCntryInfo()->cntry_contribution);
    
    TextNode* totalPriceTextNum2 = TextNode::textWithString(buf,fontSize);
    totalPriceTextNum2->setAnchorPoint(ccp(0, 0.5));
    totalPriceTextNum2->setPosition(CCPointMake(totalPriceTextNum->getPosition().x+totalPriceTextNum->getContentSize().width,topPosY - offy*2));
    totalPriceTextNum2->setColor(ccc3(66, 142, 0));
    addChild(totalPriceTextNum2, 1,TAGTOTALPRICETEXTNUM2);
    
    //购买数量
    
    leftPosX -= 48;
    
    m_buyNum = 0;
    TextNode* buyNumText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_baoku_goumaishuliang"),fontSize);
    buyNumText->setAnchorPoint(ccp(0, 0.5));
    buyNumText->setPosition(CCPointMake(leftPosX ,topPosY - offy*3));
    buyNumText->setColor(ccBLACK);
    addChild(buyNumText, 1);
}

GuildShopItemInf* CMenuBtn::getGShopItemInf()
{
    return &m_GShopItemInf;
}
short CMenuBtn::getBuyNum()
{
    return m_buyNum;
}

void CMenuBtn::baokuPriceChange(int num)
{
    m_buyNum = num;//(short)g_slider->getValue();
    
    char buf[100];
    
    TextNode* totalPriceTextNum = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM);
    TextNode* totalPriceTextNum2 = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM2);
    
    if (m_GShopItemInf.lCoin != 0)
    {
        long price = (long)(m_GShopItemInf.lCoin*m_buyNum);
        snprintf(buf, 99, "%ld",price);
        
        if (price>CGameData::Inst()->getCntryInfo()->cntry_coin)
            totalPriceTextNum->setColor(ccc3(156, 16, 0));
        else
            totalPriceTextNum->setColor(ccc3(66, 142, 0));
    }
    else
    {
        long price = (long)(m_GShopItemInf.lCntrbt*m_buyNum);
        snprintf(buf, 99, "%ld",price);
        
        if (price>CGameData::Inst()->getCntryInfo()->cntry_contribution)
            totalPriceTextNum->setColor(ccc3(156, 16, 0));
        else
            totalPriceTextNum->setColor(ccc3(66, 142, 0));
    }
    
    totalPriceTextNum->setString(buf);
    totalPriceTextNum2->setPosition(ccp(totalPriceTextNum->getPosition().x+totalPriceTextNum->getContentSize().width,totalPriceTextNum2->getPosition().y));

}

//void CMenuBtn::CliderChange(CCObject* pObj)
//{
//    CCSlider *g_slider=(CCSlider*)(pObj);
//    m_buyNum = (short)g_slider->getValue();
//    
//    char buf[50];
//    snprintf(buf, 49, "%d",m_buyNum);
//    
//    
//    TextNode* buyNumTextNum = (TextNode*)getChildByTag(TAGBUYNUM);
//    buyNumTextNum->setString(buf);
//    
//    TextNode* totalPriceTextNum = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM);
//    TextNode* totalPriceTextNum2 = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM2);
//    
//    if (m_GShopItemInf.lCoin != 0)
//    {
//        long price = (long)(m_GShopItemInf.lCoin*m_buyNum);
//        snprintf(buf, 99, "%ld",price);
//        
//        if (price>CGameData::Inst()->getCntryInfo()->cntry_coin)
//            totalPriceTextNum->setColor(ccc3(156, 16, 0));
//        else
//            totalPriceTextNum->setColor(ccc3(66, 142, 0));
//    }
//    else
//    {
//        long price = (long)(m_GShopItemInf.lCntrbt*m_buyNum);
//        snprintf(buf, 99, "%ld",price);
//        
//        if (price>CGameData::Inst()->getCntryInfo()->cntry_contribution)
//            totalPriceTextNum->setColor(ccc3(156, 16, 0));
//        else
//            totalPriceTextNum->setColor(ccc3(66, 142, 0));
//    }
//    
//    totalPriceTextNum->setString(buf);
//    totalPriceTextNum2->setPosition(ccp(totalPriceTextNum->getPosition().x+totalPriceTextNum->getContentSize().width,totalPriceTextNum2->getPosition().y));
//
//}

void CMenuBtn::refreshMenu()
{
    char buf[50];
    snprintf(buf, 49, "%ld",m_buyNum);
    
    TextNode* buyNumTextNum = (TextNode*)getChildByTag(TAGBUYNUM);
    buyNumTextNum->setString(buf);
    
    TextNode* totalPriceTextNum = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM);
    TextNode* totalPriceTextNum2 = (TextNode*)getChildByTag(TAGTOTALPRICETEXTNUM2);
    
    if (m_GShopItemInf.lCoin != 0)
    {
        long price = (long)(m_GShopItemInf.lCoin*m_buyNum);
        snprintf(buf, 99, "%ld",price);
        
        if (price>CGameData::Inst()->getCntryInfo()->cntry_coin)
            totalPriceTextNum->setColor(ccc3(156, 16, 0));
        else
            totalPriceTextNum->setColor(ccc3(66, 142, 0));
        
        snprintf(buf, 99, "/%ld",CGameData::Inst()->getCntryInfo()->cntry_coin);
        totalPriceTextNum2->setString(buf);
    }
    else
    {
        long price = (long)(m_GShopItemInf.lCntrbt*m_buyNum);
        snprintf(buf, 99, "%ld",price);
        
        if (price>CGameData::Inst()->getCntryInfo()->cntry_contribution)
            totalPriceTextNum->setColor(ccc3(156, 16, 0));
        else
            totalPriceTextNum->setColor(ccc3(66, 142, 0));
        
        snprintf(buf, 99, "/%ld",CGameData::Inst()->getCntryInfo()->cntry_contribution);
        totalPriceTextNum2->setString(buf);
    }
    
    totalPriceTextNum->setString(buf);
    totalPriceTextNum2->setPosition(ccp(totalPriceTextNum->getPosition().x+totalPriceTextNum->getContentSize().width,totalPriceTextNum2->getPosition().y));
}


GuildItemInStore* CMenuBtn::getGuildItemInf()
{
    return m_pGuildItemInf;
}

short CMenuBtn::getRewardNum()
{
    return m_rewardNum;
}

CMenuBtn::CMenuBtn(GuildItemInStore* GuildItemInf, int total)
{
    m_pGuildItemInf = GuildItemInf;
    CCSprite* spFrame = CCSprite::spriteWithFile("postrewardbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    CCSprite* rewardIcon = CCSprite::spriteWithFile(GuildItemInf->strImgName.c_str());
    addChild(rewardIcon,1,TAGXSREWARDICON);
    rewardIcon->setPosition(ccp(-214, 18));
    
    //name
    TextNode* nameText = TextNode::textWithString(GuildItemInf->strItemName.c_str(),28);
    nameText->setAnchorPoint(ccp(0.5, 0.5));
    nameText->setPosition(CCPointMake(0,92));
    nameText->setColor(ccc3(156, 16, 0));
    addChild(nameText, 1);
    
    float leftPosX = -132;
    float topPosY = 40;
    float offy = 50;
    float fontSize = 25;
    
    char buf[100];
    
    //库存
    TextNode* kuCunText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_baoku_kucun"),fontSize);
    kuCunText->setAnchorPoint(ccp(0, 0.5));
    kuCunText->setPosition(CCPointMake(leftPosX,topPosY));
    kuCunText->setColor(ccBLACK);
    addChild(kuCunText, 1);
    
    snprintf(buf,99,"%ld",GuildItemInf->lCnt);
    TextNode* kuCunTextNum = TextNode::textWithString(buf,fontSize);
    kuCunTextNum->setAnchorPoint(ccp(0, 0.5));
    kuCunTextNum->setPosition(CCPointMake(leftPosX+kuCunText->getContentSize().width+10,topPosY));
    kuCunTextNum->setColor(ccc3(66, 142, 0));
    addChild(kuCunTextNum,1,TAGXUANSHANGKUCUN);
    
    //最大数量
    int maxNum = 0;
    
    maxNum = total/GuildItemInf->lMinCondition;
    
//    switch (GuildItemInf->lLv)
//    {
//        case 1:
//        case 2:
//            maxNum = total / 500;
//            break;
//            
//        case 3:
//        case 4:
//        case 5:
//        case 6:
//            maxNum = total / 1000;
//            break;
//            
//        case 7:
//        case 8:
//        case 9:
//            maxNum = total / 2000;
//            break;
//            
//        case 10:
//            maxNum = total / 3000;
//            break;
//            
//        default:
//            break;
//    }
    
    TextNode* maxText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_fabu_maxnum"),fontSize);
    maxText->setAnchorPoint(ccp(0, 0.5));
    maxText->setPosition(CCPointMake(leftPosX,topPosY - offy*1 + 10));
    maxText->setColor(ccBLACK);
    addChild(maxText, 1);
    
    snprintf(buf,99,"%d",maxNum);
    TextNode* maxTextNum = TextNode::textWithString(buf,fontSize);
    maxTextNum->setAnchorPoint(ccp(0, 0.5));
    maxTextNum->setPosition(CCPointMake(leftPosX+maxText->getContentSize().width+10,topPosY - offy*1 + 10));
    maxTextNum->setColor(ccc3(66, 142, 0));
    addChild(maxTextNum,1);
    
    
    //宝物悬赏数量
    m_rewardNum = 0;
    
    leftPosX -= 120;
    
    TextNode* buyNumText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_baoku_xuanshangnum"),fontSize);
    buyNumText->setAnchorPoint(ccp(0, 0.5));
    buyNumText->setPosition(CCPointMake(leftPosX ,topPosY - offy*2 - 10));
    buyNumText->setColor(ccBLACK);
    addChild(buyNumText, 1);
    
    CCSprite* m_numBG1 = CCSprite::spriteWithFile("fr_reward_bar.png");
    addChild(m_numBG1,0,TAGXUANSHANGBAR);
    m_numBG1->setAnchorPoint(ccp(0,0.5));
    m_numBG1->setPosition(ccp(-90,topPosY - offy*2 - 10));
    
    TipsPopInfo* tips = TipsPopInfo::tipWithContent(GuildItemInf->strItemDsp.c_str());
    addChild(tips,10,TAGXUANSHANGTIP);
    tips->setPosition(ccp(50, 10));
    tips->setScale(0);
    
    if (maxNum == 0)
    {
        CCLayerColor* clayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 190), spFrame->getContentSize().width-8, spFrame->getContentSize().height-8);
        addChild(clayer,100,TAGCOLOCLAYER);
        clayer->setPosition(ccp(-spFrame->getContentSize().width/2 + 6, -spFrame->getContentSize().height/2 + 4));
    }
}

void CMenuBtn::chooseUpdate(int num)
{
    m_rewardNum = (long)num;
    
    char buf[50];
    snprintf(buf, 49, "%ld",m_rewardNum);
    
//    TextNode* buyNumTextNum = (TextNode*)getChildByTag(TAGXUANSHANGNUM);
//    buyNumTextNum->setString(buf);
    
    TextNode* kucunTextNum = (TextNode*)getChildByTag(TAGXUANSHANGKUCUN);
    
    if (m_rewardNum > m_pGuildItemInf->lCnt)
        kucunTextNum->setColor(ccc3(156, 16, 0));
    else
        kucunTextNum->setColor(ccc3(66, 142, 0));
}

//签到奖品信息
CMenuBtn::CMenuBtn(RwdSignInf* pRwdSignInf,bool isLast)
{
    CCSprite* spFrame = CCSprite::spriteWithFile("rwdbtnbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    
    float posX = -258;
    if (pRwdSignInf->WujiangCardList.size() > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "card");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        if (pRwdSignInf->WujiangCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("wujiangicon.png", pRwdSignInf->WujiangCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdWuJiangCard>::iterator it = pRwdSignInf->WujiangCardList.begin(); it != pRwdSignInf->WujiangCardList.end(); ++it)
            {
                RwdWuJiangCard ItmInf = *it;
                
                int id = atoi(ItmInf.strCardId.c_str());
                CCSprite* card;// = CGameData::Inst()->getHeadSprite(id);
                if (isShow) {
                    card = CGameData::Inst()->getHeadSprite(id);
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    
                    // 等级
                    char buf[16];
                    snprintf(buf, 16, ",-%hd", ItmInf.lv);
                    CCSprite* lv = CCSprite::spriteWithFile("lvnum.png");
                    if(lv){
                        CCSize sz = lv->getContentSize();
                        CCLabelAtlas* lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
                        lbLv->setAnchorPoint(CCPointMake(0.5, 0));
                        lbLv->setPosition(CCPointMake(card->getContentSize().width/2, 0));
                        card->addChild(lbLv);
                    }
                }else{
                    card = CCSprite::spriteWithFile("rwdwenhao.png");
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                }
                
                posX += card->getContentSize().width+2;
                
            }
        }
    }
    
    if (pRwdSignInf->ItemCardList.size() > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "guild_item");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        if (pRwdSignInf->ItemCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("itemcard12.png", pRwdSignInf->ItemCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdItemCard>::iterator it = pRwdSignInf->ItemCardList.begin(); it != pRwdSignInf->ItemCardList.end(); ++it)
            {
                RwdItemCard ItemInf = *it;
                ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
                CCSprite* card = RewardHelperCardIcon(pItemCard->strResName.c_str(), ItemInf.num,isShow);
                if (card)
                {
                    addChild(card, 1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    posX += card->getContentSize().width+2;
                }
            }
        }
    }
    
    if (pRwdSignInf->lCoin > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "coin");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard6.png", pRwdSignInf->lCoin,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdSignInf->lGold > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "gold");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard5.png", pRwdSignInf->lGold,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdSignInf->lWealth > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "wealth");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard3.png", pRwdSignInf->lWealth,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdSignInf->lGacha_pt > 0)
    {
        list<string>::iterator it = find(pRwdSignInf->hideInf.begin(), pRwdSignInf->hideInf.end(), "gacha_pt");
        bool isShow = true;
        if (it != pRwdSignInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("rwdyuanjundian.png", pRwdSignInf->lGacha_pt,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    
    
    //天数
    /*char buf[50];
    snprintf(buf, 49, CGameData::Inst()->getLanguageValue("rwd_qiandao_tianshu"),pRwdSignInf->iDays);
    
    TextNode* dayText = TextNode::textWithString(buf,36);
    dayText->setPosition(CCPointMake(190,32));
    dayText->setColor(kTitleColor);
    addChild(dayText, 1);*/
    const char* spriteName;
    switch (isLast ? 0:pRwdSignInf->iDays) {
        case 1:
            spriteName = "rwd1.png";
            break;
        case 2:
            spriteName = "rwd2.png";
            break;
        case 3:
            spriteName = "rwd3.png";
            break;
        case 4:
            spriteName = "rwd4.png";
            break;
        case 5:
            spriteName = "rwd5.png";
            break;
        case 6:
            spriteName = "rwd6.png";
            break;
        case 7:
            spriteName = "rwd7.png";
            break;
        case 0:
            spriteName = "rwd8.png";
            break;
            
        default:
            spriteName = "rwd7.png";
            break;
    }
    CCSprite* iconDay = CCSprite::spriteWithFile(spriteName);
    iconDay->setPosition(CCPointMake(190, 32));
    addChild(iconDay,1);
    
    if (isLast)
    {
        CCSprite* coverBg = CCSprite::spriteWithFile("cannotgetbg.png");
        coverBg->setPosition(CCPointMake(2, 0));
        addChild(coverBg,998);
    }
    
}

CCSprite* CMenuBtn::RewardHelperCardIcon(const char* card_name, int card_number,bool isShow)
{
    
    CCSprite* rt;
    if (!isShow) {
        rt = CCSprite::spriteWithFile("rwdwenhao.png");
    }else{
        rt = CCSprite::spriteWithFile(card_name);
    }
    if (rt&&isShow)
    {
        char buf[16];
        snprintf(buf, 16, "/%d", card_number);
        CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* lbItemCount = CCLabelAtlas::labelWithString(buf,
                                                                      "rewardnum.png",
                                                                      sz.width / 11,
                                                                      sz.height,
                                                                      '/');
            lbItemCount->setAnchorPoint(ccp(0.5, 0));
            lbItemCount->setPosition(CCPointMake(rt->getContentSize().width/2, 0));
            if (card_number>=10000) {
                lbItemCount->setScale(0.8f);
            }
            rt->addChild(lbItemCount);
        }
    }
    return rt;
}

//升级奖励信息
CMenuBtn::CMenuBtn(RwdLevelUpInf* pRwdLevelUpInf)
{
    CCSprite* spFrame = CCSprite::spriteWithFile("rwdbtnbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    bool isShow = true;
    if (CGameData::Inst()->getUsrInfo()->lv < pRwdLevelUpInf->showLv){
        isShow = false;
    }
    
    //reward_icon
    float posX = -258;
    if (pRwdLevelUpInf->WujiangCardList.size() > 0)
    {
        if (pRwdLevelUpInf->WujiangCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("wujiangicon.png", pRwdLevelUpInf->WujiangCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdWuJiangCard>::iterator it = pRwdLevelUpInf->WujiangCardList.begin(); it != pRwdLevelUpInf->WujiangCardList.end(); ++it)
            {
                RwdWuJiangCard ItmInf = *it;
                
                int id = atoi(ItmInf.strCardId.c_str());
                CCSprite* card;// = CGameData::Inst()->getHeadSprite(id);
                if (isShow) {
                    card = CGameData::Inst()->getHeadSprite(id);
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    
                    // 等级
                    char buf[16];
                    snprintf(buf, 16, ",-%hd", ItmInf.lv);
                    CCSprite* lv = CCSprite::spriteWithFile("lvnum.png");
                    if(lv){
                        CCSize sz = lv->getContentSize();
                        CCLabelAtlas* lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
                        lbLv->setAnchorPoint(CCPointMake(0.5, 0));
                        lbLv->setPosition(CCPointMake(card->getContentSize().width/2, 0));
                        card->addChild(lbLv);
                    }
                }else{
                    card = CCSprite::spriteWithFile("rwdwenhao.png");
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                }
                
                
                posX += card->getContentSize().width+2;
            }
        }
    }

    
    if (pRwdLevelUpInf->ItemCardList.size() > 0)
    {
        if (pRwdLevelUpInf->ItemCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("itemcard12.png", pRwdLevelUpInf->ItemCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdItemCard>::iterator it = pRwdLevelUpInf->ItemCardList.begin(); it != pRwdLevelUpInf->ItemCardList.end(); ++it)
            {
                RwdItemCard ItemInf = *it;
                ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
                CCSprite* card = RewardHelperCardIcon(pItemCard->strResName.c_str(), ItemInf.num,isShow);
                if (card)
                {
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    posX += card->getContentSize().width+2;
                }
            }
        }
    }
    
    if (pRwdLevelUpInf->lCoin > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard6.png", pRwdLevelUpInf->lCoin,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdLevelUpInf->lGold > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard5.png", pRwdLevelUpInf->lGold,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdLevelUpInf->lWealth > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard3.png", pRwdLevelUpInf->lWealth,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdLevelUpInf->lGacha_pt > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("rwdyuanjundian.png", pRwdLevelUpInf->lGacha_pt,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    
    
    //级别
    char buf[50];
    snprintf(buf, 40, "%d级",pRwdLevelUpInf->lv);
    
    TextNode* levelText = TextNode::textWithString(buf,30);
    levelText->setAnchorPoint(ccp(0.5, 0.5));
    levelText->setPosition(CCPointMake(190,30));
    levelText->setColor(ccRED);
    addChild(levelText, 1);
    
    if (CGameData::Inst()->getUsrInfo()->lv < pRwdLevelUpInf->lv)
    {
        CCSprite* coverBg = CCSprite::spriteWithFile("cannotgetbg.png");
        coverBg->setPosition(CCPointMake(2, 0));
        addChild(coverBg,998);
    }
}

//累积天数奖励
CMenuBtn::CMenuBtn(RwdBonusInf* pRwdBonusInf,int i)
{
    CCSprite* spFrame = CCSprite::spriteWithFile("rwdbtnbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    float posX = -258;
    if (pRwdBonusInf->WujiangCardList.size() > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "card");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        if (pRwdBonusInf->WujiangCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("wujiangicon.png", pRwdBonusInf->WujiangCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdWuJiangCard>::iterator it = pRwdBonusInf->WujiangCardList.begin(); it != pRwdBonusInf->WujiangCardList.end(); ++it)
            {
                RwdWuJiangCard ItmInf = *it;
                
                int id = atoi(ItmInf.strCardId.c_str());
                CCSprite* card;// = CGameData::Inst()->getHeadSprite(id);
                if (isShow) {
                    card = CGameData::Inst()->getHeadSprite(id);
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    
                    // 等级
                    char buf[16];
                    snprintf(buf, 16, ",-%hd", ItmInf.lv);
                    CCSprite* lv = CCSprite::spriteWithFile("lvnum.png");
                    if(lv){
                        CCSize sz = lv->getContentSize();
                        CCLabelAtlas* lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
                        lbLv->setAnchorPoint(CCPointMake(0.5, 0));
                        lbLv->setPosition(CCPointMake(card->getContentSize().width/2, 0));
                        card->addChild(lbLv);
                    }
                }else{
                    card = CCSprite::spriteWithFile("rwdwenhao.png");
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                }
                
                posX += card->getContentSize().width+2;
            }
        }
    }
    
    
    if (pRwdBonusInf->ItemCardList.size() > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "guild_item");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        if (pRwdBonusInf->ItemCardList.size()>1) {
            CCSprite* card;
            if (isShow) {
                card = RewardHelperCardIcon("itemcard12.png", pRwdBonusInf->ItemCardList.size());
            }else{
                card = CCSprite::spriteWithFile("rwdwenhao.png");
            }
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdItemCard>::iterator it = pRwdBonusInf->ItemCardList.begin(); it != pRwdBonusInf->ItemCardList.end(); ++it)
            {
                RwdItemCard ItemInf = *it;
                ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
                CCSprite* card = RewardHelperCardIcon(pItemCard->strResName.c_str(), ItemInf.num,isShow);
                if (card)
                {
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, 0));
                    posX += card->getContentSize().width+2;
                }
            }
        }
    }
    
    if (pRwdBonusInf->lCoin > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "coin");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard6.png", pRwdBonusInf->lCoin,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
        
    }
    
    if (pRwdBonusInf->lGold > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "gold");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard5.png", pRwdBonusInf->lGold,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdBonusInf->lWealth > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "wealth");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard3.png", pRwdBonusInf->lWealth,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdBonusInf->lGacha_pt > 0)
    {
        list<string>::iterator it = find(pRwdBonusInf->hideInf.begin(), pRwdBonusInf->hideInf.end(), "gacha_pt");
        bool isShow = true;
        if (it != pRwdBonusInf->hideInf.end()){
            isShow = false;
        }
        CCSprite* cardIcon = RewardHelperCardIcon("rwdyuanjundian.png", pRwdBonusInf->lGacha_pt,isShow);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    //天数
    char buf[50];
    
    if (i == 0) {
        snprintf(buf, 49, "%d/%d天",pRwdBonusInf->iDays,pRwdBonusInf->rewardDays);
        TextNode* dayText = TextNode::textWithString(buf,32);
        dayText->setAnchorPoint(ccp(0.5, 0.5));
        dayText->setPosition(CCPointMake(180,36));
        dayText->setColor(ccRED);
        addChild(dayText, 1);
        if (pRwdBonusInf->iDays < pRwdBonusInf->rewardDays)
        {
            CCSprite* coverBg = CCSprite::spriteWithFile("cannotgetbg.png");
            coverBg->setPosition(CCPointMake(2, 0));
            addChild(coverBg,998);
        }
    }else{
        if (pRwdBonusInf->iDays < pRwdBonusInf->rewardDays)
        {
            CCSprite* coverBg = CCSprite::spriteWithFile("cannotgetbg.png");
            coverBg->setPosition(CCPointMake(2, 0));
            addChild(coverBg,998);
            snprintf(buf, 49, "%d天",pRwdBonusInf->rewardDays);
            TextNode* dayText = TextNode::textWithString(buf,32);
            dayText->setAnchorPoint(ccp(0.5, 0.5));
            dayText->setPosition(CCPointMake(180,36));
            dayText->setColor(ccRED);
            addChild(dayText, 1);
        }else{
            snprintf(buf, 49, "%d/%d天",pRwdBonusInf->iDays,pRwdBonusInf->rewardDays);
            TextNode* dayText = TextNode::textWithString(buf,32);
            dayText->setAnchorPoint(ccp(0.5, 0.5));
            dayText->setPosition(CCPointMake(180,36));
            dayText->setColor(ccRED);
            addChild(dayText, 1);
        }
    }
}

//系统活动奖励
CMenuBtn::CMenuBtn(RwdHuodongInf* pRwdHuodongInf)
{
    CCScale9Sprite* spFrame = CCScale9Sprite::create("huodongbg.png");
    spFrame->setPreferredSize(CCSizeMake(600, 400));
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    float posX = -258;
    float posY = 0;
    int count = 0;              //当前数量
    int totalCounts = 0;        //总数量
    
    if (pRwdHuodongInf->WujiangCardList.size()>0)
        totalCounts+=1;
    if (pRwdHuodongInf->ItemCardList.size() > 0)
        totalCounts+=1;
    if (pRwdHuodongInf->lCoin > 0)
        totalCounts+=1;
    if (pRwdHuodongInf->lGold > 0)
        totalCounts+=1;
    if (pRwdHuodongInf->lGacha_pt > 0)
        totalCounts+=1;
    if (pRwdHuodongInf->lWealth > 0)
        totalCounts+=1;
    
    if (totalCounts<=3) {
        posY = -50;
    }
    
    if (pRwdHuodongInf->WujiangCardList.size() > 0)
    {
        count+=1;
        if (pRwdHuodongInf->WujiangCardList.size()>1) {
            CCSprite* card = RewardHelperCardIcon("wujiangicon.png", pRwdHuodongInf->WujiangCardList.size());
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, posY));
            
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdWuJiangCard>::iterator it = pRwdHuodongInf->WujiangCardList.begin(); it != pRwdHuodongInf->WujiangCardList.end(); ++it)
            {
                RwdWuJiangCard ItmInf = *it;
                
                int id = atoi(ItmInf.strCardId.c_str());
                CCSprite* card = CGameData::Inst()->getHeadSprite(id);
                addChild(card,1);
                card->setAnchorPoint(ccp(0, 0.5));
                card->setPosition(ccp(posX, posY));
                
                // 等级
                char buf[16];
                snprintf(buf, 16, ",-%hd", ItmInf.lv);
                CCSprite* lv = CCSprite::spriteWithFile("lvnum.png");
                if(lv){
                    CCSize sz = lv->getContentSize();
                    CCLabelAtlas* lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
                    lbLv->setAnchorPoint(CCPointMake(0.5, 0));
                    lbLv->setPosition(CCPointMake(card->getContentSize().width/2, 0));
                    card->addChild(lbLv);
                }
                
                posX += card->getContentSize().width+2;
            }
        }
    }
    
    
    if (pRwdHuodongInf->ItemCardList.size() > 0)
    {
        count+=1;
        if (pRwdHuodongInf->ItemCardList.size()>1) {
            CCSprite* card = RewardHelperCardIcon("itemcard12.png", pRwdHuodongInf->ItemCardList.size());
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, posY));
            
            posX += card->getContentSize().width+2;
        }else{
            for (list<RwdItemCard>::iterator it = pRwdHuodongInf->ItemCardList.begin(); it != pRwdHuodongInf->ItemCardList.end(); ++it)
            {
                RwdItemCard ItemInf = *it;
                ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
                CCSprite* card = RewardHelperCardIcon(pItemCard->strResName.c_str(), ItemInf.num);
                if (card)
                {
                    addChild(card,1);
                    card->setAnchorPoint(ccp(0, 0.5));
                    card->setPosition(ccp(posX, posY));
                    posX += card->getContentSize().width+2;
                }
            }
        }
    }
    
    if (pRwdHuodongInf->lCoin > 0)
    {
        count+=1;
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard6.png", pRwdHuodongInf->lCoin);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, posY));
        posX += cardIcon->getContentSize().width+2;
        
        if (count==3) {
            posX = -258;
            posY = -cardIcon->getContentSize().height-2;
        }
    }
    
    if (pRwdHuodongInf->lGold > 0)
    {
        count+=1;
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard5.png", pRwdHuodongInf->lGold);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, posY));
        posX += cardIcon->getContentSize().width+2;
        
        if (count==3) {
            posX = -258;
            posY = -cardIcon->getContentSize().height-2;
        }
    }
    
    if (pRwdHuodongInf->lWealth > 0)
    {
        count+=1;
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard3.png", pRwdHuodongInf->lWealth);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, posY));
        posX += cardIcon->getContentSize().width+2;
        
        if (count==3) {
            posX = -258;
            posY = -cardIcon->getContentSize().height-2;
        }
    }
    
    if (pRwdHuodongInf->lGacha_pt > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("rwdyuanjundian.png", pRwdHuodongInf->lGacha_pt);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, posY));
        posX += cardIcon->getContentSize().width+2;
    }
    
    //天数
    char buf[999];
    snprintf(buf, 999, "%s",CGameData::Inst()->getLanguageValue("activity_time"));
    TextNode* dayText = TextNode::textWithString(buf,CCSizeMake(150, 200),CCTextAlignmentLeft,20);
    dayText->setAnchorPoint(ccp(0, 0));
    ccColor3B black = {16,18,16};
    dayText->setColor(black);
    dayText->setPosition(CCPointMake(-248,20));
    addChild(dayText, 1);
    
    snprintf(buf, 999, "%s\n%s",pRwdHuodongInf->strDate.c_str(),pRwdHuodongInf->strContent.c_str());
    dayText = TextNode::textWithString(buf,CCSizeMake(400, 200),CCTextAlignmentLeft,20);
    
    float fWidthCount = 400/20;
    float flLen = strlen(pRwdHuodongInf->strContent.c_str())/3;
    int line = flLen/fWidthCount+1;
    
    dayText->setAnchorPoint(ccp(0, 1));
    ccColor3B red = {125,0,3};
    dayText->setColor(red);
    dayText->setPosition(CCPointMake(-150,221-24*(line-1)));
    addChild(dayText, 1);
    /*char buf[1000];
    snprintf(buf, 999, CGameData::Inst()->getLanguageValue("activity_time_content"),pRwdHuodongInf->strDate.c_str(),pRwdHuodongInf->strContent.c_str());
    TextNode* dayText = TextNode::textWithString(buf,CCSizeMake(450, 200),CCTextAlignmentLeft,20);
    dayText->setAnchorPoint(ccp(0, 0));
    ccColor3B red = {125,0,3};
    dayText->setColor(red);
    dayText->setPosition(CCPointMake(-248,0));
    addChild(dayText, 1);*/
    
    if (pRwdHuodongInf->bCanGet||(pRwdHuodongInf->loginCounts<pRwdHuodongInf->awardDays))
    {
        CCScale9Sprite* coverBg = CCScale9Sprite::create("cannotgetbg.png");
        coverBg->setPreferredSize(CCSizeMake(600, 400));
        coverBg->setPosition(CCPointMake(2, 0));
        addChild(coverBg,998);
    }
}

//系统补偿奖励
CMenuBtn::CMenuBtn(RwdBuchangInf* pRwdBuchangInf)
{
    CCSprite* spFrame = CCSprite::spriteWithFile("rwdbtnbg.png");
    spFrame->setPosition(CCPointMake(2, 0));
    addChild(spFrame);
    
    //reward_icon
    float posX = -258;
    if (pRwdBuchangInf->WujiangCardList.size() > 0)
    {
        for (list<RwdWuJiangCard>::iterator it = pRwdBuchangInf->WujiangCardList.begin(); it != pRwdBuchangInf->WujiangCardList.end(); ++it)
        {
            RwdWuJiangCard ItmInf = *it;
            
            int id = atoi(ItmInf.strCardId.c_str());
            CCSprite* card = CGameData::Inst()->getHeadSprite(id);
            addChild(card,1);
            card->setAnchorPoint(ccp(0, 0.5));
            card->setPosition(ccp(posX, 0));
            
            // 等级
            char buf[16];
            snprintf(buf, 16, ",-%hd", ItmInf.lv);
            CCSprite* lv = CCSprite::spriteWithFile("lvnum.png");
            if(lv){
                CCSize sz = lv->getContentSize();
                CCLabelAtlas* lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
                lbLv->setAnchorPoint(CCPointMake(0.5, 0));
                lbLv->setPosition(CCPointMake(card->getContentSize().width/2, 0));
                card->addChild(lbLv);
            }
            
            posX += card->getContentSize().width+2;
        }
    }
    
    
    if (pRwdBuchangInf->ItemCardList.size() > 0)
    {
        
        for (list<RwdItemCard>::iterator it = pRwdBuchangInf->ItemCardList.begin(); it != pRwdBuchangInf->ItemCardList.end(); ++it)
        {
            RwdItemCard ItemInf = *it;
            ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
            CCSprite* card = RewardHelperCardIcon(pItemCard->strResName.c_str(), ItemInf.num);
            if (card)
            {
                addChild(card,1);
                card->setAnchorPoint(ccp(0, 0.5));
                card->setPosition(ccp(posX, 0));
                posX += card->getContentSize().width+2;
            }
        }
    }
    
    if (pRwdBuchangInf->lCoin > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard6.png", pRwdBuchangInf->lCoin);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdBuchangInf->lGold > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard5.png", pRwdBuchangInf->lGold);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdBuchangInf->lWealth > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("itemcard3.png", pRwdBuchangInf->lWealth);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    if (pRwdBuchangInf->lGacha_pt > 0)
    {
        CCSprite* cardIcon = RewardHelperCardIcon("rwdyuanjundian.png", pRwdBuchangInf->lGacha_pt);
        addChild(cardIcon,1);
        cardIcon->setAnchorPoint(ccp(0, 0.5));
        cardIcon->setPosition(ccp(posX, 0));
        posX += cardIcon->getContentSize().width+2;
    }
    
    //天数
    char buf[50];
    snprintf(buf, 11, "%s",pRwdBuchangInf->strDate.c_str());
    TextNode* dayText = TextNode::textWithString(buf,26);
    dayText->setAnchorPoint(ccp(0.5, 0.5));
    dayText->setPosition(CCPointMake(180,36));
    dayText->setColor(ccc3(143, 35, 2));
    addChild(dayText, 1);
    
    if (!pRwdBuchangInf->bCanGet)
    {
        CCSprite* spFrame = CCSprite::spriteWithFile("cannotgetbg.png");
        spFrame->setPosition(CCPointMake(2, 0));
        addChild(spFrame,10);
    }
}



//******************************************************************************
// onExit
//******************************************************************************
void CMenuBtn::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

//******************************************************************************
// addvierecord
//******************************************************************************
void CMenuBtn::AddBtnViewOtherRecord(CCObject* target, SEL_CallFuncO selector, int _tag)
{
    CCSprite* sp1 = CCSprite::spriteWithFile("btn_bkd_purple.png");
    CCSprite* sp2 = CCSprite::spriteWithFile("btn_bkd_purple.png");
    CCMenuItem* item = CCMenuItemImage::itemFromNormalSprite(sp1, sp2, target, selector);
    item->setAnchorPoint(CCPointZero);
    item->setPosition(CCPointMake(100, -34));
    item->setTag(_tag);
    
    TextNode* text = TextNode::textWithString(CGameData::Inst()->getLanguageValue("record_view_other_record"), 28);
    text->setPosition(CCPointMake(sp1->getContentSize().width/2, sp1->getContentSize().height/2));
    item->addChild(text);
    
    
    CCMenu* menu_view_other_record = CCMenu::menuWithItem(item);
    menu_view_other_record->setPosition(CCPointZero);
    menu_view_other_record->setAnchorPoint(CCPointZero);
    menu_view_other_record->setTag(kMenuViewOtherRecord);
    addChild(menu_view_other_record, 50);
}

void CMenuBtn::RemoveBtnViewOtherRecord()
{
    removeChildByTag(kMenuViewOtherRecord, true);
}





