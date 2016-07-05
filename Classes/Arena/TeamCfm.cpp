//
//  TeamCfm.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TeamCfm.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "TextNode.h"
#include "CShowCardInf.h"
#include "Dialog2.h"
#include "Dialog1.h"
#include "TeamCfmCard.h"
#include "CommDlg3.h"
#include "OcProxy.h"
#include "MainScene.h"
#include "CommDlg.h"
#include "RecordHelper.h"

#define kRecordTag_on   55
#define kRecordTag_off  56
#define kBeginFight     70


typedef struct{
    CCPoint pos;
    float   scale;
}stPosScale;

static stPosScale gs_PosScaleDef[] = {
    {CCPointZero, 1.0},

    {CCPointMake(71,  378), 1.0},
    {CCPointMake(179, 378), 0.9},
    {CCPointMake(273, 378), 0.9},
    {CCPointMake(367, 378), 0.9},
    {CCPointMake(461, 378), 0.9},
    {CCPointMake(570, 378), 1.0},
    
    {CCPointMake(71,  186), 0.75},
    {CCPointMake(165, 186), 0.75},
    {CCPointMake(259, 186), 0.75},
    {CCPointMake(353, 186), 0.75},
    {CCPointMake(447, 186), 0.75},
    {CCPointMake(541, 186), 0.75},
};

#define kNeedRentDlgTag 9999
#define kRentCfmDlgTag  9990
#define kCfmBtnTag      8888
#define kRstrctDlgTag   7777
#define kCountryPorpertyIconTag   6666

//******************************************************************************
// Construct
//******************************************************************************
TeamCfm::TeamCfm(CCObject* target, SEL_CallFunc selector, int iFloorId, int iRoomId, const char* szDgType, int *iFriend)
{
    m_pListener = target;
    m_pfnSelector = selector;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_bNeedSpecialCards = false;
    m_pCardHold = NULL;
    m_lCardsCoin = 0;
    m_iNeedRentCnt = 0;
    m_iCardCnt = 6;
    m_strNeedCardsName = "";
    m_iSpecDgTimes = 0;
    m_commDlg = NULL;
    m_dialog2 = NULL;
    m_dialogWarning = NULL;
    m_iReqType = en_req_team_none;
    m_menuitem_record = NULL;
    m_text_sure_btn = NULL;
    pCIcon = NULL;
    m_IconIndex = kInvalidValue;
    
    if (CGameData::Inst()->isNewbie() == false)
    {
        m_floorid = iFloorId;
        m_roomid = iRoomId;
        string type = szDgType;
        strcpy(m_type, type.c_str());
    }
    
    m_teamLayer = CCLayer::node();
    addChild(m_teamLayer,100);
    
    const char* floorname = NULL;
    const char* roomname = NULL;
    string floordsp = "";
    
    // 由于special里 floor_id 不是012345排序的
    int room_index  = 0;
    
    if (CGameData::Inst()->isNewbie() == false)
    {
        if (strcmp(szDgType, "normal") == 0)
        {
            CCMutableArray<CNmlDgInfo*> * pNmlDgInfoArr = CGameData::Inst()->getNmlDgInfo();
            CNmlDgInfo* pNmlDgInfo;
            
            if ((iFloorId -1 >= 0) && (iFloorId -1 < pNmlDgInfoArr->count())) {
                pNmlDgInfo = pNmlDgInfoArr->getObjectAtIndex(iFloorId -1);
                floorname = pNmlDgInfo->name;
                floordsp = pNmlDgInfo->dungeon_dsp;
                m_dgRstrct = pNmlDgInfo->dgRstrct;
                
                CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pNmlDgInfo->m_pRoomArr;
                
                if ((iRoomId -1 >= 0) && (iRoomId -1 < pRoomInfoArr->count())) {
                    CDgRoomInfo* pRoomInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
                    roomname = pRoomInf->name;
                }
            }
            m_floorindex = iFloorId - 1;
            room_index  = iRoomId - 1;
        }
        else if ((strcmp(szDgType, "special") == 0) || (strcmp(szDgType, "weekly") == 0))
        {
            CCMutableArray<CSpecialDgInfo*> * pSpecialDgInfoArr = CGameData::Inst()->getSpecialDgInfo();
            
            for (int i = 0; i < pSpecialDgInfoArr->count(); i++) {
                CSpecialDgInfo* pSpecialDgInfo = pSpecialDgInfoArr->getObjectAtIndex(i);
                if (pSpecialDgInfo->iFloorId == iFloorId) {
                    floorname = pSpecialDgInfo->name;
                    floordsp = pSpecialDgInfo->dungeon_dsp;
                    
                    CCMutableArray<CDgRoomInfo*> * pRoomInfoArr = pSpecialDgInfo->m_pRoomArr;
                    
                    if ((iRoomId -1 >= 0) && (iRoomId -1 < pRoomInfoArr->count())) {
                        CDgRoomInfo* pRoomInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
                        roomname = pRoomInf->name;
                        m_floorindex = i;
                        room_index = iRoomId-1;
                    }
                    
                    if (pSpecialDgInfo->cType == 1) {
                        for (int i = 0; i < 6; i++) {
                            m_arrMyNeedCards[i] = pSpecialDgInfo->strNeedCards[i];
                            m_arrDgNeedCards[i] = pSpecialDgInfo->strNeedCards[i];
                            m_arrDgHelpCards[i] = pSpecialDgInfo->strHelpCards[i];
                            
                            if (!m_arrDgNeedCards[i].empty()) {
                                if (m_bNeedSpecialCards == false) {
                                    m_bNeedSpecialCards = true;
                                    
                                    m_iSpecDgTimes = CGameData::Inst()->getSpecialDgTimes(iFloorId);
                                    
                                    if (m_iSpecDgTimes <= 0)
                                        m_lCardsCoin = pSpecialDgInfo->cards_gold1;
                                    else if(m_iSpecDgTimes == 1)
                                        m_lCardsCoin = pSpecialDgInfo->cards_gold2;
                                    else if(m_iSpecDgTimes == 2)
                                        m_lCardsCoin = pSpecialDgInfo->cards_gold3;
                                    else
                                        m_lCardsCoin = pSpecialDgInfo->cards_coin;
                                }
                            }
                        }
                    }
                    
                    m_dgRstrct = pSpecialDgInfo->dgRstrct;
                    break;
                }
            }
        }
        else if (strcmp(szDgType, "sgyy") == 0)
        {
            CCMutableArray<CSangoYYDgInfo*> * pSangoYYDgInfoArr = CGameData::Inst()->getSangoYYDgInfo();

            if ((iFloorId -1 >= 0) && (iFloorId -1 < pSangoYYDgInfoArr->count()))
            {
                CSangoYYDgInfo* pSangoYYDgInfo = pSangoYYDgInfoArr->getObjectAtIndex(iFloorId -1);
                floorname = pSangoYYDgInfo->name;
                floordsp = pSangoYYDgInfo->dungeon_dsp;
                m_dgRstrct = pSangoYYDgInfo->dgRstrct;
                
                CCMutableArray<cSangoYYRoom*> * pRoomInfoArr = pSangoYYDgInfo->m_pRoomArr;
                
                if ((iRoomId -1 >= 0) && (iRoomId -1 < pRoomInfoArr->count())) {
                    cSangoYYRoom* pRoomInf = pRoomInfoArr->getObjectAtIndex(iRoomId -1);
                    roomname = pRoomInf->name;
                    m_floorindex = 0;
                }
            }
            m_floorindex = iFloorId-1;
            room_index = iRoomId-1;
        }
    }

    char buf[100];
    CCPoint ptBg(320, 400);
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    if (cntryInfo->buffInfo[enBuffType_Attack].effect > 0)
    {
        CCSprite* spBuffAttack = CCSprite::spriteWithSpriteFrameName("buff_small_attack.png");
        addChild(spBuffAttack);
        spBuffAttack->setPosition(CCPointMake(360, 518));
        
        float effect = cntryInfo->buffInfo[enBuffType_Attack].effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%.1f%%", effect);
        else
            snprintf(buf, 99, "%d%%", int(effect));
        TextNode* tnBuffAttack = TextNode::textWithString(buf, 18);
        addChild(tnBuffAttack);
        tnBuffAttack->setColor(ccBLACK);
        tnBuffAttack->setAnchorPoint(CCPointMake(0, 0.5));
        tnBuffAttack->setPosition(CCPointMake(375, 512));
    }
    
    if (cntryInfo->buffInfo[enBuffType_Recover].effect > 0)
    {
        CCSprite* spBuffRecover = CCSprite::spriteWithSpriteFrameName("buff_small_recover.png");
        addChild(spBuffRecover);
        spBuffRecover->setPosition(CCPointMake(450, 518));
        
        float effect = cntryInfo->buffInfo[enBuffType_Recover].effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%.1f%%", effect);
        else
            snprintf(buf, 99, "%d%%", int(effect));
        TextNode* tnBuffRecover = TextNode::textWithString(buf, 18);
        addChild(tnBuffRecover);
        tnBuffRecover->setColor(ccBLACK);
        tnBuffRecover->setAnchorPoint(CCPointMake(0, 0.5));
        tnBuffRecover->setPosition(CCPointMake(465, 512));
    }

    if (cntryInfo->buffInfo[enBuffType_Hp].effect > 0)
    {
        CCSprite* spBuffHp = CCSprite::spriteWithSpriteFrameName("buff_small_hp.png");
        addChild(spBuffHp);
        spBuffHp->setPosition(CCPointMake(540, 518));
        
        float effect = cntryInfo->buffInfo[enBuffType_Hp].effect * 100;
        if (effect > int(effect))
            snprintf(buf, 99, "%.1f%%", effect);
        else
            snprintf(buf, 99, "%d%%", int(effect));
        TextNode* tnBuffHp = TextNode::textWithString(buf, 18);
        addChild(tnBuffHp);
        tnBuffHp->setColor(ccBLACK);
        tnBuffHp->setAnchorPoint(CCPointMake(0, 0.5));
        tnBuffHp->setPosition(CCPointMake(555, 512));
    }
    
    showFightTeam(CGameData::Inst()->getCurTeamIdx());
    
    

    
 //   CCMenu* menu = CCMenu::node();
    
    bool record_video = CGameData::Inst()->CheckRecordVideo(szDgType, m_floorindex, room_index);
    
    if (CGameData::Inst()->isNewbie() == false && record_video)
    {
        int login_type = CGameData::Inst()->getLoginType();
        CCSprite* spMenu1 = NULL;
        CCSprite* spMenu2 = NULL;
        int record_on_off = 0;
        if ((login_type == enLoginReqType_SinaLogin || login_type == enLoginReqType_TencentLogin) && RecordHelper::Inst()->GetRecordSwitch())
        {
            spMenu1 = CCSprite::spriteWithFile("record_video_on.png");
            spMenu2 = CCSprite::spriteWithFile("record_video_on.png");
            record_on_off = kRecordTag_on;
        }
        else
        {
            spMenu1 = CCSprite::spriteWithFile("record_video_off.png");
            spMenu2 = CCSprite::spriteWithFile("record_video_off.png");
            record_on_off = kRecordTag_off;
        }
        m_menuitem_record = (CCMenuItemImage*)CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(TeamCfm::menuCallBack));
        m_menuitem_record->setAnchorPoint(CCPointZero);
        m_menuitem_record->setPosition(CCPointMake(534, 726));
        m_menuitem_record->setTag(record_on_off);
        CCMenu* menu = CCMenu::menuWithItem(m_menuitem_record);
        addChild(menu);
        menu->setAnchorPoint(CCPointZero);
        menu->setPosition(CCPointZero);
    }
    
    // 确认挑战
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem* sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(TeamCfm::menuCallBack));
    m_text_sure_btn = TextNode::textWithString("sssss", CCSizeMake(188, 30), CCTextAlignmentCenter, 36);
    m_text_sure_btn->setColor(ccWHITE);
    m_text_sure_btn->setShadowColor(ccBLACK);
    m_text_sure_btn->setPosition(ccp(spMenu1->getContentSize().width/2, spMenu1->getContentSize().height/2));
    sureItem->addChild(m_text_sure_btn);
    sureItem->setAnchorPoint(ccp(0, 0));
    sureItem->setPosition(CCPointMake(0, 0));
    sureItem->setTag(kBeginFight);
    CCMenu* menu = CCMenu::menuWithItem(sureItem);
    
    // 按钮的字
    if (record_video && RecordHelper::Inst()->GetRecordSwitch())
    {
        m_text_sure_btn->setString(CGameData::Inst()->getLanguageValue("arena_fightcfm_record"));
        m_text_sure_btn->setScale(0.8);
    }
    else
    {
        m_text_sure_btn->setString(CGameData::Inst()->getLanguageValue("arena_fightcfm"));
        m_text_sure_btn->setScale(1);
    }
    
    menu->setPosition(ccp(226, 148));
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu, 0, kCfmBtnTag);
    
    
    
    
    
    
    
    //floor name
    if(CGameData::Inst()->isNewbie()){
        snprintf(buf, 99, "～%s", CGameData::Inst()->getLanguageValue("arenaTip3"));
    }else{
        if((floorname) && (*floorname != 0))
            snprintf(buf, 99, "～%s", floorname);
        else
            buf[0] = 0;
    }
    
    if (buf[0] != 0){
        TextNode* floorNameLabel = TextNode::textWithString(buf, CCSizeMake(600, 42), CCTextAlignmentLeft, 36);
        floorNameLabel->setPosition(CCPointMake(60, 690));
        floorNameLabel->setAnchorPoint(ccp(0, 0.5));
        addChild(floorNameLabel, 1);
    }

    //room name
    if (CGameData::Inst()->isNewbie()) {
        snprintf(buf, 99, "");
    }
    else {
        if((roomname) && (*roomname != 0))
            snprintf(buf, 99, "%s～", roomname);
        else
            buf[0] = 0;
    }
    
    if (buf[0] != 0){
        TextNode* roomNameLabel = TextNode::textWithString(buf, CCSizeMake(600, 42), CCTextAlignmentRight, 36);
        roomNameLabel->setPosition(CCPointMake(580, 650));
        roomNameLabel->setAnchorPoint(ccp(1.0, 0.5));
        roomNameLabel->setColor(ccWHITE);
        addChild(roomNameLabel, 1);
    }
    
    
    
    CAvailDiamond* availDiamond = CGameData::Inst()->getCurAvailDiamond();
    if(availDiamond->cnt < enElementMax)
    {
        const char* eleName[] = {
            "recover",
            "eleWaterTip",
            "eleFireTip",
            "eleWoodTip",
            "eleLightTip",
            "eleDarkTip"
        };
        
        for (int i = 0; i < availDiamond->cnt; i++) {
            if(availDiamond->buf[i] < enElementMax){
                eleName[availDiamond->buf[i]] = NULL;
            }
        }
        
        bool bSpecial = false;
        string str(CGameData::Inst()->getLanguageValue("nothing"));
        for (int i = 0; i < enElementMax-2; i++) {
            if(eleName[i] != NULL){
                if(bSpecial){
                    str.append("、");
                }
                else{
                    bSpecial = true;
                }
                str.append(CGameData::Inst()->getLanguageValue(eleName[i]));
            }
        }
        str.append(CGameData::Inst()->getLanguageValue("stone"));
        
        if(bSpecial){
            TextNode* noStoneLabel = TextNode::textWithString(str.c_str(), CCSizeMake(600, 42), CCTextAlignmentCenter, 36);
            noStoneLabel->setPosition(CCPointMake(320, 596));
            noStoneLabel->setColor(ccWHITE);
            addChild(noStoneLabel, 1);
        }
    }
    
    
    if (floordsp != "")
    {
        TextNode* dspLabel = TextNode::textWithString(floordsp.c_str(), CCSizeMake(600, 42), CCTextAlignmentCenter, 36);
        dspLabel->setPosition(CCPointMake(320, 596));
        dspLabel->setColor(ccWHITE);
        addChild(dspLabel, 1);
    }
    
    //武将详情
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 101);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    setTips();
}

//******************************************************************************
// getDgStepInfo
//******************************************************************************
CUserCardInfo* TeamCfm::getHelperCardInfo(CFriendHelpInfo *helperInfo)
{
    CFriendHelpInfo* pInfo = helperInfo;
    unsigned short cid = pInfo->leadercard_cid;
    unsigned short lv = pInfo->leadercardlv;
    unsigned short sklv = pInfo->sk_lv;
    
    CCardBaseInfo *card = CGameData::Inst()->getCardBaseInfByCid(pInfo->leadercard_cid);
    CUserCardInfo m_helperCardInfo;
    m_helperCardInfo.cid = cid;
    if (card) {
        m_helperCardInfo.category = card->category;
        m_helperCardInfo.sk_lv = sklv;
        m_helperCardInfo.star = card->star;
        m_helperCardInfo.skid = card->skid;
        m_helperCardInfo.ctype = card->ctype;
        m_helperCardInfo.cost = card->cost;
        m_helperCardInfo.leader_skid = card->leader_skid;
        m_helperCardInfo.max_lv = card->max_lv;
        m_helperCardInfo.ugp_max_star = card->ugp_max_star;
        m_helperCardInfo.sub_category = card->sub_category;
        m_helperCardInfo.cou_name = card->cou_name;
        
        
        long recoverbase = card->recover;
        float fRecoverGrowth = card->recover_growth;
        m_helperCardInfo.recover = recoverbase + (lv - 1) * fRecoverGrowth + helperInfo->pl_recover * RECPLUSGROWTH;
        
        long hpbase = card->hp;
        float fHpGrowth = card->hp_growth;
        m_helperCardInfo.hp = hpbase + (lv - 1) * fHpGrowth + helperInfo->pl_hp * HPPLUSGROWTH;
        
        long attackbase = card->attack;
        float fAtkGrowth = card->attack_growth;
        m_helperCardInfo.attack = attackbase + (lv - 1) * fAtkGrowth + helperInfo->pl_attack * ATKPLUSGROWTH;
        
        m_helperCardInfo.bIsEluTgt = card->bIsEluTgt;
        m_helperCardInfo.bIsWkTgt = card->bIsWkTgt;
        
        strncpy(m_helperCardInfo.name, card->name, 99);
    }
    
    m_helperCardInfo.i_AwakeNum = helperInfo->i_awakeNum;
    m_helperCardInfo.lv = lv;
    
    m_helperCardInfo.pl_attack = helperInfo->pl_attack;
    m_helperCardInfo.pl_hp = helperInfo->pl_hp;
    m_helperCardInfo.pl_recover = helperInfo->pl_recover;
    m_helperCardInfo.plus = helperInfo->plus;
    
    CUserCardInfo* temp = new CUserCardInfo();
    *temp = m_helperCardInfo;
    temp->autorelease();
    return temp;
}

void TeamCfm::showFightTeam(int teamIdx)
{

    for (int i = 0; i < 6; i++)
    {
        m_arrMyNeedCards[i] = m_arrDgNeedCards[i];
    }
    
    CGameData::Inst()->setCurTeamIdx(teamIdx);
    
    m_teamLayer->removeAllChildrenWithCleanup(true);
    
    CCNode* node = getChildByTag(kCfmBtnTag);
    if (node) {
        node->setPosition(ccp(226, 148));
    }
    
    CCPoint ptBg(320, 400);
    
    CCSprite* sp = CCSprite::spriteWithFile("team_confirmation.png"); //team_cfm.png
    if(sp)
    {
        sp->setPosition(ptBg);
        m_teamLayer->addChild(sp);
        sp->getTexture()->setAntiAliasTexParameters();
    }
    
    CUserCardInfo* pUsrCardInfo;
    stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
    
    short  countryPorpertyArr[6] = {-1,-1,-1,-1,-1,-1};
    
    //自军武将
    for(int i = 0; i < 5; i++)
    {
        if (i != CGameData::Inst()->getCurTeamIdx())
        {
            CCSprite* teambutton = CCSprite::spriteWithFile("teamfightchooseno.png");
            m_teamLayer->addChild(teambutton);
            teambutton->setPosition(ccp(95+OFFXTEMAINFO*i,535));
            
            char charteamname[30] = {0};
            sprintf(charteamname, "teamname%d.png",i+1);
            CCSprite* teamname = CCSprite::spriteWithFile(charteamname);
            teambutton->addChild(teamname);
            teamname->setPosition(ccp(60,20));
            
            ccColor3B tempR = {162,28,0};
            teamname->setColor(tempR);
        }
        
        pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[i].uid);
        
        if (pUsrCardInfo == NULL)
            continue;
        countryPorpertyArr[i] = pUsrCardInfo->cou_name;
        if (pTeamInf[i].uid[0] != 0) {
            TeamCfmCard* pCard = new TeamCfmCard(pUsrCardInfo);
            if(pCard != NULL){
                m_teamLayer->addChild(pCard, 10, i+1);
                pCard->setPosition(gs_PosScaleDef[i+1].pos);
                pCard->setScale(gs_PosScaleDef[i+1].scale);
                
                pCard->m_iCardType = enCfmCard_Own;
            }
            pCard->release();
        }
        
        if(i == 0){
            int ldskid = pUsrCardInfo->leader_skid;
            
            const char* cstr;
            if(ldskid == 0)
                cstr = CGameData::Inst()->getLanguageValue("nothing");
            else
                cstr = CSkillLib::Ins()->getAutoSkillName(ldskid);
            
            m_txtSkName = TextNode::textWithString(cstr, CCSizeMake(300, 30), CCTextAlignmentLeft, 24);
            m_txtSkName->setPosition(CCPointMake(136, ptBg.y + 80));
            m_txtSkName->setAnchorPoint(CCPointZero);
            m_txtSkName->setColor(ccBLACK);
            m_teamLayer->addChild(m_txtSkName, 1);
            
           m_txtSkDsp = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(ldskid),24);
            m_txtSkDsp->setPosition(CCPointMake(32, ptBg.y + 46));
            m_txtSkDsp->setAnchorPoint(CCPointZero);
            m_txtSkDsp->setColor(ccBLACK);
            m_txtSkDsp->moveInLimitedWidth(0, 580, false);
            m_teamLayer->addChild(m_txtSkDsp, 1);
        }
    }
    
    CCSprite* curteambutton = CCSprite::spriteWithFile("teamfightchooseyes.png");
    m_teamLayer->addChild(curteambutton);
    curteambutton->setPosition(ccp(95+114*(CGameData::Inst()->getCurTeamIdx()),535));
    curteambutton->getTexture()->setAntiAliasTexParameters();
    
    char charteamname[30] = {0};
    sprintf(charteamname, "teamname%d.png",CGameData::Inst()->getCurTeamIdx()+1);
    CCSprite* teamname = CCSprite::spriteWithFile(charteamname);
    curteambutton->addChild(teamname);
    teamname->setPosition(ccp(60,20));
    
    //友军武将
    if (CGameData::Inst()->getSelectedHelperCount() == 5) {
        pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(teamIdx);
    } else {
        pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
    }
    
    TeamCfmCard* pHelperCard = new TeamCfmCard(pUsrCardInfo);
    if(pHelperCard != NULL){
        m_teamLayer->addChild(pHelperCard, 10, 6);
        pHelperCard->setPosition(gs_PosScaleDef[6].pos);
        
        pHelperCard->m_ucid = "";
        pHelperCard->m_iCardType = enCfmCard_Helper;
    }
    pHelperCard->release();
    
    m_txtHelperSkName = TextNode::textWithString("", CCSizeMake(300, 30), CCTextAlignmentRight, 24);
    m_txtHelperSkName->setPosition(CCPointMake(510, ptBg.y - 155));
    m_txtHelperSkName->setAnchorPoint(CCPointMake(1, 0));
    m_txtHelperSkName->setColor(ccBLACK);
    m_teamLayer->addChild(m_txtHelperSkName, 1);
    
    m_txtHelperSkDsp = TextNode::textWithString("", 24);
    m_txtHelperSkDsp->setPosition(CCPointMake(32, ptBg.y - 120));
    m_txtHelperSkDsp->setAnchorPoint(CCPointMake(0, 0));
    m_txtHelperSkDsp->setColor(ccBLACK);
    m_teamLayer->addChild(m_txtHelperSkDsp, 1);
    
    CFriendHelpInfo* pHelperInf = CGameData::Inst()->getSelectedHelperInfo(0);
    if(strcmp(pHelperInf->type, "friend") == 0){
        int ldskid = pUsrCardInfo->leader_skid;
        
        const char* cstr;
        if(ldskid == 0)
            cstr = CGameData::Inst()->getLanguageValue("nothing");
        else
            cstr = CSkillLib::Ins()->getAutoSkillName(ldskid);
        
        m_txtHelperSkName->setString(cstr);
        m_txtHelperSkDsp->setString(CSkillLib::Ins()->getAutoSkillDsp(ldskid));
        countryPorpertyArr[5] = pUsrCardInfo->cou_name;
    }
    else{
        m_txtHelperSkDsp->setString(CGameData::Inst()->getLanguageValue("arena_othercant"));
        m_txtHelperSkDsp->setColor(ccRED);
        countryPorpertyArr[5] = pUsrCardInfo->cou_name;
    }
    
    m_txtHelperSkDsp->moveInLimitedWidth(0, 580, true);
    
   
    
    CGameData::Inst()->clrPreCmtTeamInfo();
    //检查是否需要租卡
    m_iRentCardsStat = enRentCards_Null;
    if (m_bNeedSpecialCards)
    {
        //检查是否需要租卡
        chkNeedCardsStat();
        
        //如果要求的主将卡在队员位置，交换到主将位置
        chkLeaderPos();
    }
    //判断所选的武将是否为同一个国家
    
    if (pCIcon) {
        removeChild(pCIcon, true);
        pCIcon = NULL;
    }
    CUserInfo* userInfo = CGameData::Inst()->getUsrInfo();
    short    compareShort = userInfo->user_elem + 1;
    bool     isSame = true;
    for (int i = 0; i < 6; i++) {
        if (countryPorpertyArr[i] == -1) {
            continue;
        }
        if (compareShort != countryPorpertyArr[i]) {
            isSame = false;
            break;
        }
    }
    
    if (compareShort != 0 && isSame) {
        switch (compareShort) {
            case 2:{
                pCIcon = CCSprite::spriteWithFile("weiguo.png");
                pCIcon->setTag(kCountryPorpertyIconTag+compareShort);
                pCIcon->setAnchorPoint(ccp(0, 0));
                pCIcon->setPosition(ccp(10, 160));
                addChild(pCIcon, 0, kCountryPorpertyIconTag+2);
            }
                break;
            case 3:{
                pCIcon = CCSprite::spriteWithFile("shuguo.png");
                pCIcon->setTag(kCountryPorpertyIconTag+compareShort);
                pCIcon->setAnchorPoint(ccp(0, 0));
                pCIcon->setPosition(ccp(10, 160));
                addChild(pCIcon, 0, kCountryPorpertyIconTag+3);
            }
                break;
            case 4:{
                pCIcon = CCSprite::spriteWithFile("wuguo.png");
                pCIcon->setTag(kCountryPorpertyIconTag+compareShort);
                pCIcon->setAnchorPoint(ccp(0, 0));
                pCIcon->setPosition(ccp(10, 160));
                addChild(pCIcon, 0, kCountryPorpertyIconTag+4);
            }
                break;
                
            default:
                break;
        }
       
    }
    
}


//******************************************************************************
// checkCurSel
//******************************************************************************
void TeamCfm::checkCurSel(CCPoint& pos)
{
    m_iCurSel = kInvalidValue;
    
    for(int i = 1; i <= m_iCardCnt; i++){
        CCNode* node = m_teamLayer->getChildByTag(i);
        
        if(node){
            CCPoint ptCard = gs_PosScaleDef[i].pos;
            CCSize szCard = CCSizeMake(108, 108);
            
            szCard.width *= node->getScale();
            szCard.height *= node->getScale();
            
            bool bRet = CCRect::CCRectContainsPoint(CCRectMake(ptCard.x - szCard.width * 0.5,
                                                               ptCard.y - szCard.height * 0.5,
                                                               szCard.width,
                                                               szCard.height),
                                                               pos);
            
            if(bRet){
                m_iCurSel = i;
                break;
            }
        }
    }
}

void TeamCfm::setTips()
{
    m_countryPorpertyTip = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    m_countryPorpertyTip->setPosition(ccp(240, 220+(m_countryPorpertyTip->getContentSize().height/2)));
    addChild(m_countryPorpertyTip,101);
    m_tcountrydetail = TextNode::textWithString("", CCSize(310, 300), CCTextAlignmentLeft, 21);
    m_tcountrydetail->setPosition(ccp(180, 54));
    m_countryPorpertyTip->addChild(m_tcountrydetail, 10);
    
    m_countryPorpertyTip->setScale(0);
 
}

void TeamCfm::setTipsDetail()
{
    switch (m_IconIndex) {
        case 2:
        {
            char country_detail_char[256] = {0};
            snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_weiguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        case 3:
        {
            char country_detail_char[256] = {0};
            snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_shuguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        case 4:{
            char country_detail_char[256] = {0};
            
            snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_wuguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        default:
            break;
    }
  //  m_IconIndex = kInvalidValue;
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool TeamCfm::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptTouchBegan = convertToNodeSpace( touchLocation );
    
    if ((!CGameData::Inst()->isNewbie())
        && (m_ptTouchBegan.y > 512 && m_ptTouchBegan.y < 560 && m_ptTouchBegan.x > 42 && m_ptTouchBegan.x < 607)
        )
    {
        int tempi = (int)((m_ptTouchBegan.x - 42)/OFFXTEMAINFO);
        showFightTeam(tempi);
    }

    checkCurSel(m_ptTouchBegan);
    
    if (pCIcon) {
        CCPoint pt = pCIcon->getPosition();
        CCSize sz = pCIcon->getContentSize();
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x ,
                                                           pt.y ,
                                                           sz.width,
                                                           sz.height),
                                                m_ptTouchBegan);
        if (bRet) {
            m_IconIndex = pCIcon->getTag()- kCountryPorpertyIconTag;
            setTipsDetail();
            m_countryPorpertyTip->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
            return true;
        }
        
    }

    
    if (m_iCurSel == kInvalidValue) {
        return false;
    }
    else{
        m_bMvFlg = false;
        return true;
    }
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void TeamCfm::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (m_IconIndex != kInvalidValue) {
        return;
    }
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    if (ccpDistance(pos, m_ptTouchBegan) > kListBoxMinMovY) {
        m_bMvFlg = true;
        
        //租卡之前不允许移动卡片
        if (m_iRentCardsStat != enRentCards_Rent) {
            return;
        }
        
        if (m_pCardHold == NULL)
            m_pCardHold = (TeamCfmCard*)m_teamLayer->getChildByTag(m_iCurSel);
        
        if (m_pCardHold) {
            if ((m_pCardHold->m_bLeader) || (m_iCurSel == 6)){
                m_pCardHold = NULL;
            }
            else{
                m_teamLayer->reorderChild(m_pCardHold, 12);

                TeamCfmCard* pLead1 = (TeamCfmCard*) m_teamLayer->getChildByTag(1);
                m_pCardHold->setPosition(ccp(pos.x, pos.y + 50));
                for (int i = 1; i <= m_iCardCnt; i++) {
                    if (ccpDistance(gs_PosScaleDef[i].pos, ccp(pos.x, pos.y + 50)) < 45) {
                        //主将有限制时，不可替换
                        if ((i == 1) && (pLead1) && (pLead1->m_bLeader)){
                            break;
                        }
                        
                        //援军位置不可替换
                        if (i == 6){
                            break;
                        }
                        
                        if (i != m_pCardHold->getTag()) {
                            TeamCfmCard* pCard = (TeamCfmCard*) m_teamLayer->getChildByTag(i);
                            
                            if (pCard) {
                                pCard->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.1, gs_PosScaleDef[m_pCardHold->getTag()].pos),
                                                                     CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[m_pCardHold->getTag()].scale),
                                                                     NULL));
                                pCard->setTag(m_pCardHold->getTag());
                            }
                            
                            m_pCardHold->setTag(i);
                        }

                        break;
                    }
                }
            }
        }
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void TeamCfm::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_IconIndex != kInvalidValue) {
      m_countryPorpertyTip->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
        m_IconIndex = kInvalidValue;
    }
    if (m_bMvFlg){
        if (m_pCardHold) {
            m_pCardHold->setPosition(gs_PosScaleDef[m_pCardHold->getTag()].pos);
            m_pCardHold->setScale(gs_PosScaleDef[m_pCardHold->getTag()].scale);
            m_teamLayer->reorderChild(m_pCardHold, 10);
            m_pCardHold = NULL;
        }
        
        return;
    }
    
    CUserCardInfo* pUsrCardInfo = NULL;
    
    TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(m_iCurSel);
    if (pCard) {
        switch (pCard->m_iCardType) {
            case enCfmCard_Own:
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)(pCard->m_ucid.c_str()));
                if(pUsrCardInfo)
                    m_showCard->show(pUsrCardInfo);
                break;
                
            case enCfmCard_Helper:
                pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
                if(pUsrCardInfo)
                    m_showCard->show(pUsrCardInfo, false);
                break;
                
            case enCfmCard_Rent:
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pCard->m_ucid);
                if(pUsrCardInfo)
                    m_showCard->show(pUsrCardInfo, false);
                break;
                
            default:
                break;
        }
    }
}

void TeamCfm::notifyAppWillResignActive(CCObject* obj)
{
    if (m_bMvFlg){
        if (m_pCardHold) {
            m_pCardHold->setPosition(gs_PosScaleDef[m_pCardHold->getTag()].pos);
            m_pCardHold->setScale(gs_PosScaleDef[m_pCardHold->getTag()].scale);
            m_teamLayer->reorderChild(m_pCardHold, 10);
            m_pCardHold = NULL;
        }
    }
}

//******************************************************************************
// menuCallBack
//******************************************************************************
void TeamCfm::menuCallBack(CCObject *pSender)
{
    if (MainScene::Inst()->onCurArenaLimitMaxStarCall(this))
    {
        return;
    }
    
    bool bRentOk = false;
    
    switch (m_iRentCardsStat) {
        case enRentCards_Null:
            if (pSender == m_menuitem_record)
            {
                // 是否绑定
                int login_type = CGameData::Inst()->getLoginType();
                if (login_type == enLoginReqType_SinaLogin || login_type == enLoginReqType_TencentLogin)
                {
                    CCMenuItemImage* item_img  = (CCMenuItemImage*)pSender;
                    if (item_img->getTag() == kRecordTag_on)
                    {
                        item_img->setTag(kRecordTag_off);
                        RecordHelper::Inst()->SetRecordSwitch(false);
                        item_img->setNormalImage(CCSprite::spriteWithFile("record_video_off.png"));
                        item_img->setSelectedImage(CCSprite::spriteWithFile("record_video_off.png"));
                        m_text_sure_btn->setString(CGameData::Inst()->getLanguageValue("arena_fightcfm"));
                        m_text_sure_btn->setScale(1);
                    }
                    else if (item_img->getTag() == kRecordTag_off)
                    {
                        item_img->setTag(kRecordTag_on);
                        RecordHelper::Inst()->SetRecordSwitch(true);
                        item_img->setNormalImage(CCSprite::spriteWithFile("record_video_on.png"));
                        item_img->setSelectedImage(CCSprite::spriteWithFile("record_video_on.png"));
                        m_text_sure_btn->setString(CGameData::Inst()->getLanguageValue("arena_fightcfm_record"));
                        m_text_sure_btn->setScale(0.8);
                    }
                }
                else
                {
                    // 去绑定用户
                    Dialog3* dlg3 = Dialog3::CreateBind(this, callfuncO_selector(TeamCfm::CallBackBind));
                    addChild(dlg3, 100);
                    dlg3->setPosition(CCPointMake(320, 480));
                }
            }
            else if (kBeginFight == ((CCNode*)pSender)->getTag())
            {
                if (CGameData::Inst()->isNewbie()==false && m_menuitem_record!=NULL && m_menuitem_record->getTag()==kRecordTag_on)
                {
                    if (RecordHelper::Inst()->CheckUploadedUpto10())  // 检查本地为上传视频达10个
                    {
                        m_dialog2 = new Dialog2(this, callfuncO_selector(TeamCfm::cbUpto10), CGameData::Inst()->getLanguageValue("cntry_hint"), CGameData::Inst()->getLanguageValue("record_local_not_upload_content"),enDl2TwoBtn);
                        addChild(m_dialog2, 100);
                        m_dialog2->setPosition(CCPointMake(320, 480));
                        m_dialog2->setScale(0.0);
                        m_dialog2->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                        m_dialog2->release();
                    }
                    else
                    {
                        ShowAlertMenory();
                    }
                }
                else
                {
                    if (!MainScene::Inst()->onCurTeamCostCall(this))
                    {
                        CGameData::Inst()->setUseTmpTeam(false);
                        bRentOk = true;
                    }
                }
            }
            break;
            
        case enRentCards_Need:
            showNeedRentDlg();
            break;
            
        case enRentCards_Rent:
            if (isTeamPass()) {
                writeToTmpTeam();
                CGameData::Inst()->setUseTmpTeam(true);
                CGameData::Inst()->setCostCoin(m_lNeedPayTotal);
                bRentOk = true;
            }
            else{
                showRentCardsNotPlacedDlg();
            }
            break;
            
        default:
            break;
    }
    
    if (bRentOk) {
        chkDgRstrct();
        
        if (m_strDgRstrct.empty()) {
            (m_pListener->*m_pfnSelector)();
        }
        else{
            showRstrctDlg();
        }
    }
}

void TeamCfm::CallBackBind(CCObject* _obj)
{
    Dialog3Ret* ret = (Dialog3Ret*) _obj;
    int index = ret->bSel;
    if (index == 1)
    {
        // 从othermain里的代码抄过来的
        OcProxy::Inst()->deleteCookies();
        if(CGameData::Inst()->loginBySinaRequest())
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_iReqType = en_req_bind_sina;
            
            MainScene::Inst()->showBottomPromptInfo("naviText_weibo_");
         //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
            scheduleUpdate();
        }
    }
    else if (index == 2)
    {
        OcProxy::Inst()->deleteCookies();
        if(CGameData::Inst()->loginByTencentRequest())
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_iReqType = en_req_bind_qq;
        
            MainScene::Inst()->showBottomPromptInfo("naviText_weibo_");
         //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
            scheduleUpdate();
        }
        
    }
}



//******************************************************************************
// 检查是否需要租卡
//******************************************************************************
void TeamCfm::chkNeedCardsStat()
{
    m_strNeedCardsName = "";
    
    TeamCfmCard* pLead1 = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
    TeamCfmCard* pLead2 = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
    
    if(lookupCidInString(pLead1->m_cid, m_arrMyNeedCards[0])){
        pLead1->m_bLeader = true;
        m_arrMyNeedCards[0] = "";
    }
    
    if(lookupCidInString(pLead1->m_cid, m_arrMyNeedCards[5])){
        if (pLead1->m_bLeader == false) {
            pLead1->m_bLeader = true;
            m_arrMyNeedCards[5] = "";
        }
    }
    
    if(lookupCidInString(pLead2->m_cid, m_arrMyNeedCards[0])){
        pLead2->m_bLeader = true;
        m_arrMyNeedCards[0] = "";
    }
    
    if(lookupCidInString(pLead2->m_cid, m_arrMyNeedCards[5])){
        if (pLead2->m_bLeader == false) {
            pLead2->m_bLeader = true;
            m_arrMyNeedCards[5] = "";
        }
    }
    
    int iStar = 0;
    int iLevel = 0;
    
    int idx= 0;
    if (!m_arrMyNeedCards[0].empty())
    {
        for (int i=1; i<=6; i++)
        {
            TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i);
            if (pCard != NULL)
            {
                if (lookupCidInString(pCard->m_cid, m_arrMyNeedCards[0]) )
                {
                    if (pCard->m_star > iStar)
                    {
                        idx = i;
                        iStar = pCard->m_star;
                    }
                    else if (pCard->m_star == iStar && pCard->m_level > iLevel)
                    {
                        idx = i;
                        iLevel = pCard->m_level;
                    }
                }
            }
        }
        
        if (idx != 0)
        {
            TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(idx);
            pCard->m_bLeader = true;
            m_arrMyNeedCards[0] = "";
        }
    }
    
    iStar = 0;
    idx= 0;
    
    if (!m_arrMyNeedCards[5].empty())
    {
        for (int i=1; i<=6; i++)
        {
            TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i);
            if (pCard != NULL)
            {
                if (lookupCidInString(pCard->m_cid, m_arrMyNeedCards[5]))
                {
                    if (pCard->m_star > iStar)
                    {
                        idx = i;
                        iStar = pCard->m_star;
                    }
                    else if (pCard->m_star == iStar && pCard->m_level > iLevel)
                    {
                        idx = i;
                        iLevel = pCard->m_level;
                    }
                }
            }
        }
        
        if (idx != 0)
        {
            TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(idx);
            pCard->m_bLeader = true;
            m_arrMyNeedCards[5] = "";
        }
    }
    
    for (int j = 0; j < 6; j++)
    {
        if (!m_arrMyNeedCards[j].empty())
        {
            for (int i = 1; i <= 6; i++)
            {
                TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i);
                
                if (NULL != pCard)
                {
                    if (lookupCidInString(pCard->m_cid, m_arrMyNeedCards[j]))
                    {
                        m_arrMyNeedCards[j] = "";
                        
                        if ((j == 0) || (j == 5)) {
                            pCard->m_bLeader = true;
                        }
                        break;
                    }
                }
            }
        }
    }
    
    m_iNeedRentCnt = 0;
    for (int j = 0; j < 6; j++) {
        if (!m_arrMyNeedCards[j].empty()) {
            m_iRentCardsStat = enRentCards_Need;
            
            m_iNeedRentCnt++;
            
            string::size_type pos(0);
            pos = m_arrDgHelpCards[j].find(",");
            string strCid = m_arrDgHelpCards[j].substr(0, pos);
            CCardBaseInfo* pCardInf = CGameData::Inst()->getCardBaseInfByCid(atoi(strCid.c_str()));
            
            if (!m_strNeedCardsName.empty()) {
                m_strNeedCardsName.append(",");
            }
            
            m_strNeedCardsName.append(pCardInf->name);
        }
    }
    
    m_lNeedPayTotal = m_iNeedRentCnt * m_lCardsCoin;
}

//******************************************************************************
// lookupCidInString
//******************************************************************************
bool TeamCfm::lookupCidInString(int cid, string& str)
{
    bool bRet = false;
    
    char buf[16];
    const char* pos = str.c_str();
    
    int i = 0;
    while (1) {
        if ((*pos == ',') || (*pos == 0)) {
            buf[i] = 0;
            
            if (atoi(buf) == cid) {
                bRet = true;
                break;
            }
            
            if (*pos == 0)
                break;
            
            i = 0;
            pos++;
            continue;
        }
        
        buf[i] = *pos;
        
        i++;
        pos++;
    }
    
    return bRet;
}

//******************************************************************************
// 如果要求的主将卡在队员位置，交换到主将位置
//******************************************************************************
void TeamCfm::chkLeaderPos()
{
    TeamCfmCard* pLead_1 = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
    TeamCfmCard* pLead_2 = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
    
    if ((pLead_1) && (pLead_1->m_bLeader)
        && (pLead_2) && (pLead_2->m_bLeader))
    {
        return;
    }
    
    bool bSwapLeader = false;
    
    for (int i = 2; i <= 5; i++)
    {
        TeamCfmCard* pLead1 = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
        TeamCfmCard* pLead2 = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
        
        TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i);
        
        if ((pCard) && (pLead1) && (pLead2)){
            if (pCard->m_bLeader) {
                if (pLead1->m_bLeader == false) {
                    pCard->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                         CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[1].pos),
                                                         CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[1].scale),
                                                         NULL));
                    pCard->setTag(1);
                    
                    pLead1->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                         CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[i].pos),
                                                         CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[i].scale),
                                                         NULL));
                    pLead1->setTag(i);
                    
                    bSwapLeader = true;
                    
                    stUid* pCurTeam = CGameData::Inst()->getCurTeamInf();
                    for (int k = 0; k < kMaxMemCntOfTeam; k++)
                        CGameData::Inst()->setPreCmtTeamInfo(k, pCurTeam[k].uid, pCurTeam[k].cost);
                    
                    CGameData::Inst()->setPreCmtTeamInfo(0, pCurTeam[i-1].uid, pCurTeam[i-1].cost);
                    CGameData::Inst()->setPreCmtTeamInfo(i-1, pCurTeam[0].uid, pCurTeam[0].cost);
                }
                else if (pLead2->m_bLeader == false) {
                    pCard->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                         CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[6].pos),
                                                         CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[6].scale),
                                                         NULL));
                    pCard->setTag(6);
                    
                    pLead2->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                          CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[i].pos),
                                                          CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[i].scale),
                                                          NULL));
                    pLead2->setTag(i);
                    
                    bSwapLeader = true;
                }
            }
        }
    }
    
    if (bSwapLeader) {
        schedule(schedule_selector(TeamCfm::updateAutoSkill), 1.0);
    }
}

//******************************************************************************
// updateAutoSkill
//******************************************************************************
void TeamCfm::updateAutoSkill(ccTime dt)
{
    unschedule(schedule_selector(TeamCfm::updateAutoSkill));
    
    TeamCfmCard* pLead1 = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
    TeamCfmCard* pLead2 = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
    
    CUserCardInfo* pUsrCardInfo = NULL;
    
    if (pLead1) {
        switch (pLead1->m_iCardType) {
            case enCfmCard_Own:
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)(pLead1->m_ucid.c_str()));
                break;

            case enCfmCard_Rent:
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pLead1->m_ucid);
                break;
                
            default:
                break;
        }

        if (pUsrCardInfo) {
            if (pUsrCardInfo->leader_skid == 0) {
                m_txtSkName->setString(CGameData::Inst()->getLanguageValue("nothing"));
                m_txtSkDsp->setString("");
            }
            else{
                m_txtSkName->setString(CSkillLib::Ins()->getAutoSkillName(pUsrCardInfo->leader_skid));
                m_txtSkDsp->setString(CSkillLib::Ins()->getAutoSkillDsp(pUsrCardInfo->leader_skid));
            }
        }
    }
    
    if (pLead2) {
        switch (pLead2->m_iCardType) {
            case enCfmCard_Helper:
                {
                    CFriendHelpInfo* pHelperInf = CGameData::Inst()->getSelectedHelperInfo(0);
                    if(strcmp(pHelperInf->type, "friend") == 0){
                        pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
                    }
                    else{
                        pUsrCardInfo = NULL;
                    }
                }
                break;
                
            case enCfmCard_Rent:
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pLead2->m_ucid);
                break;
                
            default:
                break;
        }
        
        if (pUsrCardInfo) {
            if (pUsrCardInfo->leader_skid == 0) {
                m_txtHelperSkName->setString(CGameData::Inst()->getLanguageValue("nothing"));
                m_txtHelperSkDsp->setString("");
            }
            else{
                m_txtHelperSkName->setString(CSkillLib::Ins()->getAutoSkillName(pUsrCardInfo->leader_skid));
                m_txtHelperSkDsp->setString(CSkillLib::Ins()->getAutoSkillDsp(pUsrCardInfo->leader_skid));
                m_txtHelperSkDsp->setColor(ccBLACK);
            }
        }
    }
}

//******************************************************************************
// showNeedRentDlg
//******************************************************************************
void TeamCfm::showNeedRentDlg()
{
    char buf[200];
    
    if (m_iSpecDgTimes <= 0) {
        if (m_lNeedPayTotal == 0) {
            snprintf(buf,
                     199,
                     CGameData::Inst()->getLanguageValue("TeamCfm_NeedRent"),
                     m_strNeedCardsName.c_str());
        }
        else{
            snprintf(buf,
                     199,
                     CGameData::Inst()->getLanguageValue("TeamCfm_NeedRentGold"),
                     m_strNeedCardsName.c_str(),
                     m_lNeedPayTotal);
        }
    }
    else if(m_iSpecDgTimes < 3){
        snprintf(buf,
                 199,
                 CGameData::Inst()->getLanguageValue("TeamCfm_NeedRentGold"),
                 m_strNeedCardsName.c_str(),
                 m_lNeedPayTotal);
    }
    else{
        snprintf(buf,
                 199,
                 CGameData::Inst()->getLanguageValue("TeamCfm_NeedRentCoin"),
                 m_strNeedCardsName.c_str(),
                 m_lNeedPayTotal);
    }
    
    Dialog2* pDlg = new Dialog2(this,
                                callfuncO_selector(TeamCfm::cbNeedRentDlg),
                                CGameData::Inst()->getLanguageValue("alert"),
                                buf,
                                enDl2TwoBtn);
    pDlg->setPosition(CCPointMake(320, 480));
    addChild(pDlg, 100, kNeedRentDlgTag);
    pDlg->release();

    pDlg->setScale(0);
    pDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// dl2CallBack
//******************************************************************************
void TeamCfm::cbNeedRentDlg(CCObject *pSender)
{
    Dialog2Ret *ret = (Dialog2Ret *)pSender;
    
    if (ret->bOk) {
        rentCards();
        
        if(m_iCardCnt > 6){
            CCNode* node = getChildByTag(kCfmBtnTag);
            if (node) {
                node->setPosition(ccp(430, 148));
            }
        }
    }
    
    removeChildByTag(kNeedRentDlgTag, true);
}

//******************************************************************************
// rentCards
//******************************************************************************
void TeamCfm::rentCards()
{
    int     iBlankIdx = 1;
    bool    bLeaderChg = false;
    
    for (int i = 0; i < 6; i++) {
        if (m_arrMyNeedCards[i].empty())
            continue;
        
        while (iBlankIdx < 12) {
            if (m_teamLayer->getChildByTag(iBlankIdx) == NULL) {
                break;
            }
            else{
                iBlankIdx++;
            }
        }
        

        CUserCardInfo* pUsrCardInf = CGameData::Inst()->getUsrCardInfByRentStr(m_arrDgHelpCards[i]);
        
        if (pUsrCardInf == NULL)
            continue;
        
        TeamCfmCard* pCard = new TeamCfmCard(pUsrCardInf);
        if(pCard){
            m_teamLayer->addChild(pCard, 0, iBlankIdx);
            
            pCard->setPosition(gs_PosScaleDef[iBlankIdx].pos);
            pCard->setScale(gs_PosScaleDef[iBlankIdx].scale);
            
            pCard->m_ucid = m_arrDgHelpCards[i];
            pCard->m_iCardType = enCfmCard_Rent;
            
            if ((i == 0) || (i == 5)) {
                TeamCfmCard* pLead = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
                
                if (pLead->m_bLeader)
                    pLead = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
                
                if (pLead->m_bLeader == false) {
                    if (pLead->getTag() == 6) {
                        pCard->setPosition(gs_PosScaleDef[pLead->getTag()].pos);
                        pCard->setScale(gs_PosScaleDef[pLead->getTag()].scale);
                        pCard->m_bLeader = true;
                        
                        m_teamLayer->removeChildByTag(6, true);
                        pCard->setTag(6);
                        iBlankIdx--;
                    }
                    else{
                        pCard->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[pLead->getTag()].pos),
                                                             CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[pLead->getTag()].scale),
                                                             NULL));
                        pCard->setTag(pLead->getTag());
                        pCard->m_bLeader = true;
                        
                        pLead->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.3, gs_PosScaleDef[iBlankIdx].pos),
                                                             CCScaleTo::actionWithDuration(0.1, gs_PosScaleDef[iBlankIdx].scale),
                                                             NULL));
                        pLead->setTag(iBlankIdx);
                    }
                }
                
                bLeaderChg = true;
            }
        }
        
        pCard->release();
    }
    
    
    if (iBlankIdx > 6) {
        m_iCardCnt = iBlankIdx;
    }
    else{
        m_iCardCnt = 6;
    }
    
    m_iRentCardsStat = enRentCards_Rent;
    
    if (bLeaderChg) {
        schedule(schedule_selector(TeamCfm::updateAutoSkill), 0.4);
    }
}

//******************************************************************************
// isRentCardsPlaced
//******************************************************************************
bool TeamCfm::isTeamPass()
{
    bool bRet = true;
    
    string arrDgNeedCards[6];
    for (int i = 0; i < 6; i++) {
        arrDgNeedCards[i] = m_arrDgNeedCards[i];
    }
    
    
    for (int j = 0; j < 6; j++) {
        if (!arrDgNeedCards[j].empty()) {
            for (int i = 1; i <= 6; i++) {
                TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i);
                
                if (pCard) {
                    bool bFind = lookupCidInString(pCard->m_cid, arrDgNeedCards[j]);
                    if (bFind) {
                        arrDgNeedCards[j] = "";
                        break;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < 6; i++) {
        if(!arrDgNeedCards[i].empty()){
            bRet = false;
            break;
        }
    }
    
    return bRet;
}

//******************************************************************************
// showRentCardsNotPlacedDlg
//******************************************************************************
void TeamCfm::showRentCardsNotPlacedDlg()
{
    Dialog1* dlg = new Dialog1(this,
                               callfuncO_selector(TeamCfm::cbRentCardsNotPlacedDlg),
                               CGameData::Inst()->getLanguageValue("TeamCfm_RentCardPlace"),
                               false);
    
    addChild(dlg, 100, kRentCfmDlgTag);
    dlg->release();
    dlg->setPosition(CCPointMake(320, 480));
    dlg->setScale(0.0);
    dlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

//******************************************************************************
// cbRentCardsNotPlacedDlg
//******************************************************************************
void TeamCfm::cbRentCardsNotPlacedDlg()
{
    removeChildByTag(kRentCfmDlgTag, true);
}

//******************************************************************************
// writeToTmpTeam
//******************************************************************************
void TeamCfm::writeToTmpTeam()
{
    for (int i = 1; i <= 6; i++) {
        TeamCfmCard* pCard = (TeamCfmCard*) m_teamLayer->getChildByTag(i);
        if (pCard) {
            CGameData::Inst()->setTeamInfo(kTmpTeamIdx, i-1, (char*)(pCard->m_ucid.c_str()));
        }
        else{
            CGameData::Inst()->setTeamInfo(kTmpTeamIdx, i-1, (char*)"");
        }
    }
}

//******************************************************************************
// chkDgRstrct
//******************************************************************************
void TeamCfm::chkDgRstrct()
{
    char buf[256];
    
    const char* elem[enElementMax] ={
        CGameData::Inst()->getLanguageValue("eleHeartTip"),
        CGameData::Inst()->getLanguageValue("eleWaterTip"),
        CGameData::Inst()->getLanguageValue("eleFireTip"),
        CGameData::Inst()->getLanguageValue("eleWoodTip"),
        CGameData::Inst()->getLanguageValue("eleLightTip"),
        CGameData::Inst()->getLanguageValue("eleDarkTip"),
        CGameData::Inst()->getLanguageValue("eleWuXiaoTip"),
        CGameData::Inst()->getLanguageValue("eleBombTip"),
    };
    
    m_strDgRstrct.clear();
    
    if (CGameData::Inst()->isNewbie()) {
        return;
    }
    
    do {
        //队伍总统御力限制
        if (m_dgRstrct.cards_cost > 0){
            int iTotalCost = 0;
            for (int i = 0; i < kMaxMemCntOfTeam; i++) {
                CUserCardInfo* pUsrCardInfo = NULL;
                
                TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i+1);
                if (pCard) {
                    switch (pCard->m_iCardType) {
                        case enCfmCard_Own:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)(pCard->m_ucid.c_str()));
                            break;
                            
                        case enCfmCard_Helper:
                            pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
                            break;
                            
                        case enCfmCard_Rent:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pCard->m_ucid);
                            break;
                            
                        default:
                            break;
                    }
                }
                
                if(pUsrCardInfo){
                    iTotalCost += pUsrCardInfo->cost;
                }
            }
            
            if (iTotalCost > m_dgRstrct.cards_cost) {
                snprintf(buf, sizeof(buf), CGameData::Inst()->getLanguageValue("dgrestrict_cost"), m_dgRstrct.cards_cost);
                m_strDgRstrct = buf;
                break;
            }
        }
        
        //单个武将统御力限制
        if (m_dgRstrct.max_cost > 0){
            for (int i = 0; i < kMaxMemCntOfTeam; i++) {
                CUserCardInfo* pUsrCardInfo = NULL;
                
                TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i+1);
                if (pCard) {
                    switch (pCard->m_iCardType) {
                        case enCfmCard_Own:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)(pCard->m_ucid.c_str()));
                            break;
                            
                        case enCfmCard_Helper:
                            pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
                            break;
                            
                        case enCfmCard_Rent:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pCard->m_ucid);
                            break;
                            
                        default:
                            break;
                    }
                }
                
                if((pUsrCardInfo) && (pUsrCardInfo->cost > m_dgRstrct.max_cost)){
                    snprintf(buf, sizeof(buf), CGameData::Inst()->getLanguageValue("dgrestrict_singlecost"), m_dgRstrct.max_cost);
                    m_strDgRstrct = buf;
                    break;
                }
            }
            
            if (!m_strDgRstrct.empty()) {
                break;
            }
        }
        
        //战场武将属性限制
        {
            unsigned short tmpCtypeArr[kMaxMemCntOfTeam];
            
            for (int i = 0; i < kMaxMemCntOfTeam; i++) {
                CUserCardInfo* pUsrCardInfo = NULL;
                
                TeamCfmCard* pCard = (TeamCfmCard*)m_teamLayer->getChildByTag(i+1);
                if (pCard) {
                    switch (pCard->m_iCardType) {
                        case enCfmCard_Own:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid((char*)(pCard->m_ucid.c_str()));
                            break;
                            
                        case enCfmCard_Helper:
                            pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(0);
                            break;
                            
                        case enCfmCard_Rent:
                            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(pCard->m_ucid);
                            break;
                            
                        default:
                            break;
                    }
                }
                
                if(pUsrCardInfo)
                    tmpCtypeArr[i] = pUsrCardInfo->ctype;
                else
                    tmpCtypeArr[i] = kInvalidValue;
            }
            
            string strElems("");
            
            //只能携带指定属性的武将
            if (m_dgRstrct.only_ctype[0] != kInvalidValue) {
                for (int i = 0; i < kMaxMemCntOfTeam; i++) {
                    if (tmpCtypeArr[i] == kInvalidValue) {
                        continue;
                    }
                    
                    bool bFound = false;
                    for (int j = 0; j < kMaxMemCntOfTeam; j++) {
                        if (m_dgRstrct.only_ctype[j] == kInvalidValue) {
                            break;
                        }
                        
                        if (tmpCtypeArr[i] == m_dgRstrct.only_ctype[j]) {
                            bFound = true;
                            break;
                        }
                    }
                    
                    if (!bFound) {
                        for (int k = 0; k < kMaxMemCntOfTeam; k++) {
                            if (m_dgRstrct.only_ctype[k] == kInvalidValue) {
                                break;
                            }
                            
                            if (!strElems.empty())
                                strElems.append(",");

                            strElems.append(elem[m_dgRstrct.only_ctype[k]]);
                        }
                        
                        break;
                    }
                }
            }
            
            if(!strElems.empty()){
                snprintf(buf, sizeof(buf), CGameData::Inst()->getLanguageValue("dgrestrict_onlyelem"), strElems.c_str());
                m_strDgRstrct = buf;
                break;
            }
            
            //必须携带指定属性的武将
            if (m_dgRstrct.cards_ctype[0] != kInvalidValue) {
                for (int i = 0; i < kMaxMemCntOfTeam; i++) {
                    if (m_dgRstrct.cards_ctype[i] == kInvalidValue) {
                        break;
                    }
                    
                    bool bFound = false;
                    for (int j = 0; j < kMaxMemCntOfTeam; j++) {
                        if (tmpCtypeArr[j] == m_dgRstrct.cards_ctype[i]) {
                            tmpCtypeArr[j] = kInvalidValue;
                            bFound = true;
                            break;
                        }
                    }
                    
                    if (!bFound) {
                        if (m_dgRstrct.cards_ctype[i] <= enElementDark) {
                            if (!strElems.empty()) {
                                if (strElems.find(elem[m_dgRstrct.cards_ctype[i]]) == string::npos) {
                                    strElems.append(",");
                                    strElems.append(elem[m_dgRstrct.cards_ctype[i]]);
                                }
                            }
                            else{
                                strElems.append(elem[m_dgRstrct.cards_ctype[i]]);
                            }
                        }
                    }
                }
            }
            
            
            if(!strElems.empty()){
                snprintf(buf, sizeof(buf), CGameData::Inst()->getLanguageValue("dgrestrict_elem"), strElems.c_str());
                m_strDgRstrct = buf;
            }
        }
    } while (0);
}

//******************************************************************************
// showRstrctDlg
//******************************************************************************
void TeamCfm::showRstrctDlg()
{
    Dialog1* dlg = new Dialog1(this,
                               callfuncO_selector(TeamCfm::cbRstrctDlg),
                               m_strDgRstrct.c_str(),
                               false);
    
    addChild(dlg, 100, kRstrctDlgTag);
    dlg->release();
    dlg->setPosition(CCPointMake(320, 480));
    dlg->setScale(0.0);
    dlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

//******************************************************************************
// cbRstrctDlg
//******************************************************************************
void TeamCfm::cbRstrctDlg(CCObject *pSender)
{
    removeChildByTag(kRstrctDlgTag, true);
}

//******************************************************************************
// onEnter
//******************************************************************************
void TeamCfm::onEnter()
{
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(TeamCfm::notifyAppWillResignActive),  // 处理的消息的回调函数
                                                           "CHECK_TOUCH_MOVE_INTERRUPT",                    // 感兴趣的消息名称
                                                           NULL);
}

//******************************************************************************
// onExit
//******************************************************************************
void TeamCfm::onExit()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this,"CHECK_TOUCH_MOVE_INTERRUPT");
    
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

//******************************************************************************
// update
//******************************************************************************
void TeamCfm::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if (comState == kReqHoldOn) {
        return;
    }
    if (comState == kReqCancel) {
        
        if(m_commDlg != NULL){
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
            MainScene::Inst()->enableBottomPanel(true);
        }
        
        unscheduleUpdate();
        MainScene::Inst()->enableBottomPanel(true);
        CGameData::Inst()->clearReqStat();
        return;
    }
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(TeamCfm::cbCommDlg), enCommDlgTp_connecting);
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
            case en_req_bind_sina:
            case en_req_bind_qq:
              //  ShowAlertMenory();
                break;
                
            default:
                break;
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(TeamCfm::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void TeamCfm::cbCommDlg(CCObject* pObj)
{
    MainScene::Inst()->enableBottomPanel(true);
    m_commDlg->removeFromParentAndCleanup(true);
    m_commDlg = NULL;
    CGameData::Inst()->clearReqStat();
}

void TeamCfm::ShowAlertMenory()
{
    //Dialog2* men_warning;
    m_dialogWarning = new Dialog2(this, callfuncO_selector(TeamCfm::BeginRecord), CGameData::Inst()->getLanguageValue("cntry_hint"), CGameData::Inst()->getLanguageValue("arena_record_mem_warning"), enDl2TwoBtn);
    addChild(m_dialogWarning, 500);
    m_dialogWarning->setPosition(CCPointMake(320, 480));
    m_dialogWarning->setScale(0.0);
    m_dialogWarning->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    m_dialogWarning->release();
}

void TeamCfm::BeginRecord(CCObject* obj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) obj;
    bool bOk = pRet->bOk;
    if (bOk)
    {
        MainScene::Inst()->enableBottomPanel(true);
        // 开始录制的 东西 写在这里
        RecordHelper::Inst()->SetRecording();
        RecordHelper::Inst()->SetFightScene(m_floorid , m_roomid, m_type, m_floorindex);
        CCLog("begin record");
        CGameData::Inst()->setUseTmpTeam(false);
        (m_pListener->*m_pfnSelector)();
    }
    else
    {
        m_dialogWarning->removeFromParentAndCleanup(true);
        m_dialogWarning=NULL;
    }

    
}

void TeamCfm::cbUpto10(CCObject* obj)
{
    Dialog2Ret* ret = (Dialog2Ret*)obj;
    
    if (ret->bOk)
    {
        ShowAlertMenory();
    }

    m_dialog2->removeFromParentAndCleanup(true);
    m_dialog2 = NULL;
}






