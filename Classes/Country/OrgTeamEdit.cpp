//
//  OrgTeamEdit.cpp
//  AgainstWar
//
//  Created by liang jun on 13-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgTeamEdit.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "TextNode.h"
#include "CShowCardInf.h"
#include "Dialog2.h"
#include "Dialog1.h"
#include "TeamCfmCard.h"
#include "MainScene.h"

typedef struct{
    CCPoint pos;
    float   scale;
}stPosScale2;

static stPosScale2 gs_PosScaleDef2[] = {
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

#define kCfmBtnTag1      8888

//******************************************************************************
// Construct
//******************************************************************************
OrgTeamEdit::OrgTeamEdit(CCObject* target, SEL_CallFunc selector, char* dgName)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);

    m_pCardHold = NULL;
    m_iCardCnt = 6;
    m_teamLayer = CCLayer::node();
    addChild(m_teamLayer,100);
    
    showFightTeam(CGameData::Inst()->getCurTeamIdx());
    
    //按钮
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(OrgTeamEdit::menuCallBack));
    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("org_teamcfm"),
                                                   CCSizeMake(188, 30), CCTextAlignmentCenter, 36);
    itemLabel->setColor(ccWHITE);
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setAnchorPoint(CCPointZero);
    itemLabel->setPosition(ccp(0, 16));
    sureItem->addChild(itemLabel);
    
    sureItem->setAnchorPoint(ccp(0, 0));
    CCMenu *menu = CCMenu::menuWithItem(sureItem);
    menu->setPosition(ccp(226, 148));
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu, 0, kCfmBtnTag1);
    
    //武将详情
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 101);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    if (dgName[0] != 0)
    {
      TextNode* roomNameLabel = TextNode::textWithString(dgName, 46);
      roomNameLabel->setPosition(CCPointMake(320, 650));
      roomNameLabel->setAnchorPoint(ccp(0.5, 0.5));
      roomNameLabel->setColor(ccWHITE);
      addChild(roomNameLabel, 1);
    }
}


void OrgTeamEdit::showFightTeam(int teamIdx)
{
    CGameData::Inst()->setCurTeamIdx(teamIdx);
    
    m_teamLayer->removeAllChildrenWithCleanup(true);
    
    CCNode* node = getChildByTag(kCfmBtnTag1);
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
        
        if (pTeamInf[i].uid[0] != 0) {
            TeamCfmCard* pCard = new TeamCfmCard(pUsrCardInfo);
            if(pCard != NULL){
                m_teamLayer->addChild(pCard, 10, i+1);
                pCard->setPosition(gs_PosScaleDef2[i+1].pos);
                pCard->setScale(gs_PosScaleDef2[i+1].scale);
                
                pCard->m_iCardType = enCfmCard_Own;
            }
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
            
            m_txtSkDsp = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(ldskid), 24);
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
    
    
    
    TextNode* m_txtHelperSkName = TextNode::textWithString(CGameData::Inst()->getLanguageValue("nothing"), CCSizeMake(300, 30), CCTextAlignmentRight, 24);
    m_txtHelperSkName->setPosition(CCPointMake(510, ptBg.y - 155));
    m_txtHelperSkName->setAnchorPoint(CCPointMake(1, 0));
    m_txtHelperSkName->setColor(ccBLACK);
    m_teamLayer->addChild(m_txtHelperSkName, 1);
    
    TextNode* m_txtHelperSkDsp = TextNode::textWithString(CGameData::Inst()->getLanguageValue("org_othercant"), 24);
    m_txtHelperSkDsp->setPosition(CCPointMake(640 - 32, ptBg.y - 120));
    m_txtHelperSkDsp->setAnchorPoint(CCPointMake(1, 0));
    m_txtHelperSkDsp->setColor(ccBLACK);
    m_teamLayer->addChild(m_txtHelperSkDsp, 1);
    m_txtHelperSkDsp->setColor(ccRED);
    
}

//******************************************************************************
// checkCurSel
//******************************************************************************
void OrgTeamEdit::checkCurSel(CCPoint& pos)
{
    m_iCurSel = kInvalidValue;
    
    if ((!CGameData::Inst()->isNewbie()) && (pos.y > 512 && pos.y < 560 && pos.x > 42 && pos.x < 607)
        )
    {
        int tempi = (int)((pos.x - 42)/OFFXTEMAINFO);
        showFightTeam(tempi);
        return;
    }
    
    for(int i = 1; i <= m_iCardCnt; i++){
        CCNode* node = m_teamLayer->getChildByTag(i);
        
        if(node){
            CCPoint ptCard = gs_PosScaleDef2[i].pos;
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

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool OrgTeamEdit::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptTouchBegan = convertToNodeSpace( touchLocation );
    
    checkCurSel(m_ptTouchBegan);
    
    if (m_iCurSel == kInvalidValue)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void OrgTeamEdit::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void OrgTeamEdit::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
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

//******************************************************************************
// menuCallBack
//******************************************************************************
void OrgTeamEdit::menuCallBack(CCObject *pSender)
{
    if (!MainScene::Inst()->onCurTeamCostCall(this))
    {
        (m_pListener->*m_pfnSelector)();
    }
}

//******************************************************************************
// lookupCidInString
//******************************************************************************
bool OrgTeamEdit::lookupCidInString(int cid, string& str)
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
// updateAutoSkill
//******************************************************************************
void OrgTeamEdit::updateAutoSkill(ccTime dt)
{
    unschedule(schedule_selector(OrgTeamEdit::updateAutoSkill));
    
    TeamCfmCard* pLead1 = (TeamCfmCard*)m_teamLayer->getChildByTag(1);
    TeamCfmCard* pLead2 = (TeamCfmCard*)m_teamLayer->getChildByTag(6);
    
    CUserCardInfo* pUsrCardInfo = NULL;
    
    if (pLead1)
    {
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
    }
}

//******************************************************************************
// onEnter
//******************************************************************************
void OrgTeamEdit::onEnter()
{
    CCLayer::onEnter();
}

//******************************************************************************
// onExit
//******************************************************************************
void OrgTeamEdit::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

