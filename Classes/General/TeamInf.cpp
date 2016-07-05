//
//  TeamInf.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-28.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TeamInf.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "Arrow.h"
#include "MsgBox.h"
#include "SimpleAudioEngine.h"
#include "Dialog2.h"
#include "MainScene.h"
#include "CShowCardInf.h"
#include "CommDlg.h"
#include "TextNode.h"
#include "OcProxy.h"

#define kTeamInfArrowTag    777
#define kNewbieEndTag 82
#define UI_TAB_APPEND_COST_SHOW     7000
#define UI_TAB_SHOW_CARD            8000

#define UI_TAB_SHOW_COST_CARD       5001
#define UI_TAB_SHOW_COST_USER       5002
#define UI_TAB_SHOW_FRAME           5003


using namespace CocosDenshion;

CCPoint g_pt[8] = {
    CCPointMake(80, 416),
    CCPointMake(374, 416),
    CCPointMake(80, 374),
    CCPointMake(374, 374),
    CCPointMake(80, 332),
    CCPointMake(374, 332),
    CCPointMake(80, 290),
    CCPointMake(374, 290),
};

static const char* gStrMsg[] = {
    "teamInf_prompt1",
    "teamInf_prompt2",
    "teamInf_prompt_new2",
};

//******************************************************************************
// Constructor
//******************************************************************************
TeamInf::TeamInf(CCObject* target, SEL_CallFuncO selector,bool bUseScrollView/* = false*/)
{
    if (!bUseScrollView)
    {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    }
    
    m_bUseScrollView = bUseScrollView;
    m_iNewbieMsgIdx = 0;
    m_commDlg = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    m_resetDlg = NULL;
    isLongPress = false;
    m_ret.iSel = kInvalidValue;
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 1000,UI_TAB_SHOW_CARD);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    m_msgbox = NULL;
    if (!bUseScrollView)
    {
        showTeamInfoList(CGameData::Inst()->getCurTeamIdx());
    }
}

void TeamInf::waitShowInfo()
{
    CCLog("hahaha");
    isLongPress = true;
    CCPoint pos = convertToNodeSpace(beganPoint);
    
    CCSize size;
    CCPoint ptCard;
    bool bRet = false;
    unschedule(schedule_selector(TeamInf::waitShowInfo));
    
    CCMutableArray<CUserCardInfo*> * pUserCardArr = CGameData::Inst()->getUserCards();

    for(int i = 0; i < 5; i++){
        if(m_spCard[i] == NULL){
            continue;
        }
        if(m_spCard[i]->getTag() == 333)
            continue;

        size   = m_spCard[i]->getContentSize();
        ptCard = m_spCard[i]->getPosition();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(ptCard.x - size.width * 0.5, 
                                                      ptCard.y - size.height * 0.5, 
                                                      size.width, size.height),
                                           pos);
        
        if(bRet){
//            CCMutableArray<CUserCardInfo*> * pUserCardArr = CGameData::Inst()->getUserCards(true);
            CUserCardInfo* pUsrCardInfo;
            stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
            for(int j = 0; j < pUserCardArr->count(); j++){
                pUsrCardInfo = pUserCardArr->getObjectAtIndex(j);
                if((strncmp(pTeamInf[i].uid, pUsrCardInfo->ucid, 99)) == 0)
                    break;
            }
            m_showCard->show(pUsrCardInfo);
           // stUid* pTeamInf = CGameData::Inst()->getTeamInfo();
           // CCardBaseInfo *cardInfo = CGameData::Inst()->getCardBaseInfByCid(pTeamInf)
            break;
        }
    }
}
//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool TeamInf::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    m_bTouchMvFlg = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    beganPoint = touchLocation;
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCSize size;
    CCPoint ptCard;
    bool bRet = false;

    if(m_msgbox){
        removeChild(m_msgbox, true);
        m_msgbox = NULL;
    }
    else {
        schedule(schedule_selector(TeamInf::waitShowInfo),0.3);
    }
    
    if(m_iNewbieMsgIdx == 1){
        m_iNewbieMsgIdx = 2;
        
        m_msgbox = new MsgBox("ma_prompt.png", getTeamInfoPrompt(), ccc3(63, 23, 13));
        addChild(m_msgbox, 10);
        m_msgbox->release();
        m_msgbox->setPosition(CCPointMake(320, 320));
        m_msgbox->setScale(0.0);
        m_msgbox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    }
    else if(m_iNewbieMsgIdx == 2){
        MainScene::Inst()->removeTouchTip();
        stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
        if(CGameData::Inst()->newbieRequest(0, 5, pTeamInf[4].uid))//新手引导结束
            scheduleUpdate();
        return false;
    }
    

    float startx = STARTX_SUB - (OFFX_SUB/2);
    if ((!CGameData::Inst()->isNewbie()) && (pos.y > 670 && pos.y < 728 && pos.x > startx && pos.x < (startx + OFFX_SUB*5)))
    {
        int temp = (pos.x - startx)/OFFX_SUB;
        m_ret.iSel = temp+5;
        bRet = true;
        return bRet;
    }
    
    for(int i = 0; i < 5; i++)
    {
        if(m_spCard[i] == NULL)
            continue;
        
        if(CGameData::Inst()->isNewbie())
        {
            stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
            CUserCardInfo*  pUsrCardInfo1 = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[4].uid);
            
            if(pUsrCardInfo1 && pUsrCardInfo1->cid == 86)
            {
                i = 4;
            }
            else if(pTeamInf[4].uid[0] == 0)
            {
                i = 4;
            }
            else {
                return false;
            }
        }
        
        size   = m_spCard[i]->getContentSize();
        ptCard = m_spCard[i]->getPosition();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(ptCard.x - size.width * 0.5, 
                                                      ptCard.y - size.height * 0.5, 
                                                      size.width, size.height),
                                           pos);
        
        if(bRet){
            m_ret.iSel = i;

            break;
        }
    }
    
    
    
    
    ////show tips
    CCPoint screenPos = touchLocation;
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        screenPos = ccpMult(screenPos, 2);
    if(sz.width < 1000)
        screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    screenPos.x -= MainScene::Inst()->getPosition().x;
    screenPos.y -= MainScene::Inst()->getPosition().y;
    
    if ((330<screenPos.x&&600>screenPos.x) && (440<screenPos.y&&480>screenPos.y))
    {
        removeChildByTag(UI_TAB_APPEND_COST_SHOW,true);
        CUserInfo* pUserInfo = CGameData::Inst()->getUsrInfo();
        if (pUserInfo && CGameData::Inst()->isAppendCost())
        {
            unsigned int unCurTime = (unsigned int)time(NULL);
            unsigned long unTime = 0;
            if (unCurTime<pUserInfo->hours_endTime)
            {
                unTime = pUserInfo->hours_endTime;
            }
            if (unCurTime<pUserInfo->month_endTime)
            {
                if (unCurTime<unTime)
                {
                    unTime = MIN(pUserInfo->month_endTime,unTime);
                }
                else
                {
                    unTime = pUserInfo->month_endTime;
                }
            }
            
            if (unCurTime<unTime)
            {
                unsigned int time = unTime - unCurTime;
                char cBuf[256];
                //天
                unsigned int unDay = time/(24*60*60);
                //时
                unsigned int unHour = (time - unDay*24*60*60)/(60*60);
                //分
                unsigned int unMinute = (time - unDay*24*60*60 - unHour*60*60)/60;
                //秒
                unsigned int unSec = time - unDay*24*60*60 - unHour*60*60 - unMinute*60;
                if (0<unDay)
                {
                    snprintf(cBuf,sizeof(cBuf),CGameData::Inst()->getLanguageValue("title_cost_show_day"),unDay,unHour,unMinute);
                }
                else if (0<unHour)
                {
                    snprintf(cBuf,sizeof(cBuf),CGameData::Inst()->getLanguageValue("title_cost_show_hour"),unHour,unMinute);
                }
                else if (0<unMinute)
                {
                    snprintf(cBuf,sizeof(cBuf),CGameData::Inst()->getLanguageValue("title_cost_show_minute"),unMinute);
                }
                else if (0<unSec)
                {
                    snprintf(cBuf,sizeof(cBuf),CGameData::Inst()->getLanguageValue("title_cost_show_sec"),unSec);
                }
                
                if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png"))
                {
                    pSprite->setScaleX(0.56f);
                    pSprite->setScaleY(0.46f);
                    
                    CCNode *pNode = CCLayer::node();
                    pNode->setAnchorPoint(ccp(0.0f,0.0f));
                    
                    pNode->addChild(pSprite);
                    addChild(pNode,999,UI_TAB_APPEND_COST_SHOW);
                    pNode->setPosition(CCPointMake(506, 510));
                    
                    TextNode *pText = TextNode::textWithString(cBuf, CCSizeMake(288, 18), CCTextAlignmentCenter, 18);
                    pText->setColor(ccWHITE);
                    pText->setShadowColor(ccBLACK);
                    pNode->addChild(pText);
                    pNode->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
                }
            }
        }
    }
    
    return bRet;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void TeamInf::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    m_bTouchMvFlg = true;
    unschedule(schedule_selector(TeamInf::waitShowInfo));
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void TeamInf::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(TeamInf::waitShowInfo));
    if(isLongPress)
    {
        isLongPress = false;
        return;   
    }
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, beganPoint) > kListBoxMinMovY)
        return;
    
    if(m_ret.iSel != kInvalidValue){
        (m_pListener->*m_pfnSelector)(&m_ret);
        
    }
    
    if (CCNode *pNode = getChildByTag(UI_TAB_APPEND_COST_SHOW))
    {
        pNode->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
    }
}

//******************************************************************************
// resetTeamInfo
//******************************************************************************
void TeamInf::resetTeamInfo()
{
    CGameData::Inst()->resetTeam(kMaxTeamCnt);
}

//******************************************************************************
// showResetDlg
//******************************************************************************
void TeamInf::showResetDlg() {
    if(m_resetDlg == NULL){
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        const char* title = CGameData::Inst()->getLanguageValue("alert");
        char buf[100];
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("teamInf_reset_alert"));
        
        m_resetDlg = new Dialog2(this, callfuncO_selector(TeamInf::cbResetTeamInfo), title, buf, enDl2TwoBtn);
        addChild(m_resetDlg, 100);
        m_resetDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_resetDlg->release();
    }
}

//******************************************************************************
// cbResetTeamInfo
//******************************************************************************
void TeamInf::cbResetTeamInfo(CCObject* pObj) {
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        resetTeamInfo();
        removeAllChildrenWithCleanup(true);
        showTeamInfoList(CGameData::Inst()->getCurTeamIdx());
        m_showCard = new CShowCardInf();
        addChild(m_showCard, 1000,UI_TAB_SHOW_CARD);
        m_showCard->release();
    }
    else {
        removeChild(m_resetDlg, true);
    }
    m_resetDlg = NULL;
}

//******************************************************************************
// updateTeamInfoList
//******************************************************************************
void TeamInf::showTeamInfoList(int pageIdx)
{
    removeAllChildrenWithCleanup(true);
    
    CGameData::Inst()->setCurTeamIdx(pageIdx);
    
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 1000,UI_TAB_SHOW_CARD);
    m_showCard->release();
    
    
    CCSprite* sp;
    char buf[100];
    
    int cost_total = 0;
    int max_cost = 0;
    int leader_sk_id = 0;
    
    int attack[8] = {0,0,0,0,0,0,0,0};
    memset(m_cost, 0, sizeof(m_cost));
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    max_cost = pUsrInf->cost;
    
    show_user_cost = pUsrInf->cost;
    show_hours_append_cost = 0;
    show_hours_endTime = 0;
    show_month_append_cost = 0;
    show_month_endTime = 0;
    
    unsigned long ulCurTime = (unsigned long)time(NULL);
    if (ulCurTime<pUsrInf->hours_endTime)
    {
        show_hours_append_cost = pUsrInf->hours_append_cost;
        show_hours_endTime = pUsrInf->hours_endTime;
    }
    if (ulCurTime<pUsrInf->month_endTime)
    {
        show_month_append_cost = pUsrInf->month_append_cost;
        show_month_endTime = pUsrInf->month_endTime;
    }
    
    //leader 背景框
    sp = CCSprite::spriteWithFile("mainleaderframe.png");   //pa_chief.png
    if(sp != NULL){
        addChild(sp);
        sp->setPosition(CCPointMake(84, 612));
    }
    
    
    for (int i=0; i<kMaxTeamCnt-1; i++)
    {
        sp = CCSprite::spriteWithFile("teamchoosenot.png");
        if(sp != NULL)
        {
            addChild(sp);
            sp->setPosition(CCPointMake(555 - 76*i, 689));
            
            char textname[20] = {0};
            sprintf(textname, "teamname%d.png",5-i);
            CCSprite* textSP = CCSprite::spriteWithFile(textname);
            sp->addChild(textSP);
            textSP->setPosition(ccp(69,28));
            textSP->setScale(0.8);
        }
        

    }
    
    //副将背景框
    sp = CCSprite::spriteWithFile("subleaderframe.png"); //pa_lieutenant.png
    if(sp != NULL){
        addChild(sp,0,UI_TAB_SHOW_FRAME);
        sp->setPosition(CCPointMake(401, 591));
    }
    
    sp = CCSprite::spriteWithFile("teamchooseyes.png"); //pa_lieutenant.png
    if(sp != NULL)
    {
        addChild(sp);
        sp->setPosition(CCPointMake(251+76*pageIdx, 689));
        
        char textname[20] = {0};
        sprintf(textname, "teamname%d.png",pageIdx+1);
        CCSprite* textSP = CCSprite::spriteWithFile(textname);
        sp->addChild(textSP);
        textSP->setPosition(ccp(64,25));
    }
    
    //数值背景框
    sp = CCSprite::spriteWithFile("teamparamframe.png"); //pa_parameter.png
    if(sp != NULL){
        addChild(sp);
        sp->setPosition(CCPointMake(320, 326));
    }
    
    CUserCardInfo* pUsrCardInfo;
    TextNode* lbText;
    stUid* pTeamInf = CGameData::Inst()->getTeamInfByIdx(pageIdx);//getCurTeamInf();
    
    if(CGameData::Inst()->getTeamInfByIdx(0)[0].uid[0] == 0){
        memset(m_spCard, 0, 5);
        return;
    }
    
    if(pTeamInf[0].uid[0] == 0){
        memset(m_spCard, 0, 5);
        return;
    }
    
    //leader card
    pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[0].uid);
    if (pUsrCardInfo != NULL) {
        m_spCard[0] = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
        if(m_spCard[0] != NULL){
            addChild(m_spCard[0]);
            m_spCard[0]->setPosition(CCPointMake(84, 608));
            
            //究极金边
            CCSprite* sp1 = NULL;
            if (pUsrCardInfo->bIsEluTgt) {
                sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
            }
            else if (pUsrCardInfo->bIsWkTgt) {
                sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
            }
            
            if (sp1) {
                m_spCard[0]->addChild(sp1);
                sp1->setAnchorPoint(CCPointMake(0, 1));
                sp1->setPosition(ccp(4, m_spCard[0]->getContentSize().height-4));
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
                m_spCard[0]->addChild(lbPlus);
                
                CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                                   CCFadeOut::actionWithDuration(0.2),
                                                                                   CCFadeIn::actionWithDuration(0.2),
                                                                                   NULL);
                lbPlus->runAction(CCRepeatForever::actionWithAction(action1));
            }
        }
        
        //awake
        for (int j=0; j<MaxAwakeSkillCnt; j++)
        {
            if (j<pUsrCardInfo->i_AwakeNum)
            {
                if (stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(pUsrCardInfo->AwakeSkillArr[j]))
                {
                    switch (pAwakeSkill->iType)
                    {
                        case enAwakeSkillTp_valChg_hp:
                        {
                            attack[1] += (int)pAwakeSkill->fEffect;
                            break;
                        }
                        case enAwakeSkillTp_valChg_atk:
                        {
                            attack[pUsrCardInfo->ctype + 2] += (int)pAwakeSkill->fEffect;
                            break;
                        }
                        case enAwakeSkillTp_valChg_rcv:
                        {
                            attack[0] += (int)pAwakeSkill->fEffect;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
            }
        }
        
        m_cost[0] = pUsrCardInfo->cost;
        cost_total += pUsrCardInfo->cost;
        attack[pUsrCardInfo->ctype + 2] += pUsrCardInfo->attack;
        if (pUsrCardInfo->stype != enElementInvalid)
        {
            if (pUsrCardInfo->stype == pUsrCardInfo->ctype)
                attack[pUsrCardInfo->stype + 2] += (int)(pUsrCardInfo->attack*0.1);
            else
                attack[pUsrCardInfo->stype + 2] += (int)(pUsrCardInfo->attack*0.3333);
        }
        attack[1] += pUsrCardInfo->hp;
        attack[0] += pUsrCardInfo->recover;
        leader_sk_id = pUsrCardInfo->leader_skid;
        
        if (pUsrCardInfo->lv < pUsrCardInfo->max_lv) {
            snprintf(buf, 99, "Lv.%d", pUsrCardInfo->lv);
        }
        else {
            snprintf(buf, 99, "Lv.%s", CGameData::Inst()->getLanguageValue("max"));
        }
        lbText = TextNode::textWithString(buf, CCSizeMake(110, 20), CCTextAlignmentCenter, 20);
        lbText->setPosition(CCPointMake(84, 548));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        snprintf(buf, 99, "%s:%d", CGameData::Inst()->getLanguageValue("cost"), pUsrCardInfo->cost);
        lbText = TextNode::textWithString(buf, CCSizeMake(130, 20), CCTextAlignmentCenter, 20);
        lbText->setPosition(CCPointMake(84, 528));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
    
    //副将卡片
    CCPoint pt(241, 608);
    for(int i = 1; i < 5; i++){
        if(pTeamInf[i].uid[0] == 0){
            m_spCard[i] = CCSprite::spriteWithSpriteFrameName("fr_box.png");
            m_spCard[i]->setTag(333);
            if(m_spCard[i] != NULL){
                addChild(m_spCard[i]);
                m_spCard[i]->setPosition(pt);
            }
        }
        else
        {
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[i].uid);
            if (pUsrCardInfo != NULL) {
                m_spCard[i] = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
                if(m_spCard[i] != NULL){
                    addChild(m_spCard[i]);
                    m_spCard[i]->setPosition(pt);
                    
                    //究极金边
                    CCSprite* sp1 = NULL;
                    if (pUsrCardInfo->bIsEluTgt) {
                        sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
                    }
                    else if (pUsrCardInfo->bIsWkTgt) {
                        sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
                    }
                    
                    if (sp1) {
                        m_spCard[i]->addChild(sp1);
                        sp1->setAnchorPoint(CCPointMake(0, 1));
                        sp1->setPosition(ccp(4, m_spCard[i]->getContentSize().height-4));
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
                        m_spCard[i]->addChild(lbPlus);
                        
                        CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                                           CCFadeOut::actionWithDuration(0.2),
                                                                                           CCFadeIn::actionWithDuration(0.2),
                                                                                           NULL);
                        lbPlus->runAction(CCRepeatForever::actionWithAction(action1));
                    }
                }
                
                
                //awake
                for (int j=0; j<MaxAwakeSkillCnt; j++)
                {
                    if (j<pUsrCardInfo->i_AwakeNum)
                    {
                        if (stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(pUsrCardInfo->AwakeSkillArr[j]))
                        {
                            switch (pAwakeSkill->iType)
                            {
                                case enAwakeSkillTp_valChg_hp:
                                {
                                    attack[1] += (int)pAwakeSkill->fEffect;
                                    break;
                                }
                                case enAwakeSkillTp_valChg_atk:
                                {
                                    attack[pUsrCardInfo->ctype + 2] += (int)pAwakeSkill->fEffect;
                                    break;
                                }
                                case enAwakeSkillTp_valChg_rcv:
                                {
                                    attack[0] += (int)pAwakeSkill->fEffect;
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                
                
                
                m_cost[i] = pUsrCardInfo->cost;
                cost_total += pUsrCardInfo->cost;
                attack[pUsrCardInfo->ctype + 2] += pUsrCardInfo->attack;
                if (pUsrCardInfo->stype != enElementInvalid)
                {
                  if (pUsrCardInfo->stype == pUsrCardInfo->ctype)
                      attack[pUsrCardInfo->stype + 2] += (int)(pUsrCardInfo->attack*0.1);
                    else
                       attack[pUsrCardInfo->stype + 2] += (int)(pUsrCardInfo->attack*0.3333);
                }
                
                
                attack[1] += pUsrCardInfo->hp;
                attack[0] += pUsrCardInfo->recover;
                
                if (pUsrCardInfo->lv < pUsrCardInfo->max_lv) {
                    snprintf(buf, 99, "Lv.%d", pUsrCardInfo->lv);
                }
                else {
                    snprintf(buf, 99, "Lv.%s", CGameData::Inst()->getLanguageValue("max"));
                }
                TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(110, 20), CCTextAlignmentCenter, 20);
                lbText->setPosition(CCPointMake(pt.x, 548));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText, 1);
                
                snprintf(buf, 99, "%s:%d", CGameData::Inst()->getLanguageValue("cost"), pUsrCardInfo->cost);
                lbText = TextNode::textWithString(buf, CCSizeMake(130, 20), CCTextAlignmentCenter, 20);
                lbText->setPosition(CCPointMake(pt.x, 528));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                addChild(lbText, 1);
            }
        }
        pt.x += 108;
    }
    
    //总回复力不小于0
    attack[0] = (attack[0] < 0)? 0 : attack[0];
    
    
    
    
    
//    //...
//    //总cost
//    snprintf(buf, 99, "%d/%d", cost_total, max_cost);
//    TextNode *text = TextNode::textWithString(buf, CCSizeMake(264, 24), CCTextAlignmentRight, 24);
//    text->setPosition(CCPointMake(455, 460));
//    text->setColor(ccWHITE);
//    addChild(text);
//    
//    if (cost_total > max_cost) {
//        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
//                                                                           CCFadeTo::actionWithDuration(0.2, 255),
//                                                                           NULL);
//        text->setColor(ccRED);
//        text->setShadowColor(ccBLACK);
//        text->runActionInside(CCRepeatForever::actionWithAction(action));
//    }
//    //...

//    //...
//    snprintf(buf,99,"%d/",cost_total);
//    TextNode *pText1 = TextNode::textWithString(buf, CCSizeMake(144, 24), CCTextAlignmentRight, 24);
//    pText1->setPosition(CCPointMake(470, 460));
//    pText1->setColor(ccWHITE);
//    pText1->setShadowColor(ccBLACK);
//    addChild(pText1);
//    if (cost_total > max_cost) {
//        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.5, 128),
//                                                                           CCFadeTo::actionWithDuration(0.5, 255),
//                                                                           NULL);
//        pText1->setColor(ccRED);
//        pText1->setShadowColor(ccBLACK);
//        pText1->runActionInside(CCRepeatForever::actionWithAction(action));
//    }
//
//    snprintf(buf,99,"%d",max_cost);
//    TextNode *pText2 = TextNode::textWithString(buf, CCSizeMake(144, 24), CCTextAlignmentLeft, 24);
//    pText2->setPosition(CCPointMake(612, 460));
//    pText2->setColor(ccWHITE);
//    pText2->setShadowColor(ccBLACK);
//    addChild(pText2);
//    
//    if (0<CGameData::Inst()->isAppendCost())
//    {
//        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.5, 128),
//                                                                           CCFadeTo::actionWithDuration(0.5, 255),
//                                                                           NULL);
//        pText2->setColor(ccGREEN);
//        pText2->setShadowColor(ccBLACK);
//        pText2->runActionInside(CCRepeatForever::actionWithAction(action));
//    }
//    //....
//    removeChildByTag(UI_TAB_SHOW_COST_CARD,true);
//    removeChildByTag(UI_TAB_SHOW_COST_USER,true);
    //updateCostText(0);
    
    
    updateCost();
    unschedule(schedule_selector(TeamInf::updateCostText));
    if ((0<show_hours_endTime && 0<show_hours_append_cost)||
        (0<show_month_endTime && 0<show_month_append_cost))
    {
        updateCostText(0);
        schedule(schedule_selector(TeamInf::updateCostText),1.0f);
    }
    else
    {
        updateCostText(0);
    }
    
    //recover, hp, attack
    for(int i = 0; i < 8; i++){
        //if (0==i)continue;
        snprintf(buf, 99, "%d", attack[i]);
        lbText = TextNode::textWithString(buf, CCSizeMake(222, 24), CCTextAlignmentRight, 24);
        lbText->setAnchorPoint(CCPointMake(0, 0.5));
        lbText->setColor(ccBLACK);
        addChild(lbText, 1);
        
        if (i < 2) {
            lbText->setPosition(g_pt[i]);
        }
        else if(i == 2){
            lbText->setPosition(g_pt[7]);
        }
        else{
            lbText->setPosition(g_pt[i-1]);
        }
    }
    
    //leader skill
    if(leader_sk_id == 0){
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("nothing"),
                                             CCSizeMake(430, 24),
                                             CCTextAlignmentLeft,
                                             24);
    }
    else{
        const char* skill_name = CSkillLib::Ins()->getAutoSkillName(leader_sk_id);
        lbText = TextNode::textWithString(skill_name, CCSizeMake(430, 24), CCTextAlignmentLeft, 24);
    }
    lbText->setPosition(CCPointMake(150, 248));
    lbText->setAnchorPoint(CCPointMake(0, 0.5)); 
    lbText->setColor(ccWHITE);
    addChild(lbText, 1);
    
    if(leader_sk_id != 0){
//        const char* skill_dsp = CSkillLib::Ins()->getAutoSkillDsp(leader_sk_id);
//        lbText = TextNode::textWithString(skill_dsp, CCSizeMake(550, 50), CCTextAlignmentLeft, 24);
//        lbText->setPosition(CCPointMake(48, 200));
//        lbText->setAnchorPoint(CCPointMake(0, 0.5));
//        lbText->setColor(ccBLACK);
//        addChild(lbText, 1);
        
        // 竖屏滚动
        lbText = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(leader_sk_id), CCSizeMake(540, 0), CCTextAlignmentLeft, 24);
        lbText->setPosition(CCPointMake(54, 225));
        lbText->setAnchorPoint(ccp(0, 1));
        lbText->setColor(ccBLACK);
        lbText->moveInLimitedHeight(24,2);
        addChild(lbText, 1);
        
        // 横屏滚动
//        lbText = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(385), 24);
//        lbText->setPosition(CCPointMake(60, 185));
//        lbText->setAnchorPoint(CCPointZero);
//        lbText->setColor(ccBLACK);
//        lbText->moveInLimitedWidth(0, 540, false);
//        addChild(lbText, 1);
    }
    
    //分享开关
    CCMenuItem* m_shareBtn = NULL;
    if (!m_bUseScrollView)
    {
        CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
        CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("shareWeibo.png");
        m_shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(TeamInf::shareSanGuo));
        m_shareBtn->setAnchorPoint(CCPointZero);
        CCMenu *shareMenu = CCMenu::menuWithItem(m_shareBtn);
        addChild(shareMenu,10);
        shareMenu->setPosition(CCPointZero);
        
        m_shareBtn->setScale(0.5);
        m_shareBtn->setPosition(ccp(320, 740));
        
        if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
            shareMenu ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
            
        }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
            shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
            
        }else {
            shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
        }
    }
    
    //m_shareBtn->setIsVisible(false);
    if(CGameData::Inst()->isNewbie()){
        if (m_shareBtn)m_shareBtn->setIsVisible(false);
        
        stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
        CUserCardInfo*  pUsrCardInfo1 = CGameData::Inst()->getUsrCardInfByUcid(pTeamInf[4].uid);
        
        if(pUsrCardInfo1 && pUsrCardInfo1->cid == 86)
        {
            
            
            Arrow* pArrow = new Arrow(30, 255);
            pArrow->setPosition(CCPointMake(566, 570));
            pArrow->setRotation(90);
            addChild(pArrow, 500, kTeamInfArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
            
            //msgbox
            m_iNewbieMsgIdx = 0;
            m_msgbox = new MsgBox("ma_prompt.png", getTeamInfoPrompt(), ccc3(63, 23, 13));
            addChild(m_msgbox, 10);
            m_msgbox->release();
            m_msgbox->setPosition(CCPointMake(320, 320));
            m_msgbox->setScale(0.0);
            m_msgbox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        }
        else if(pTeamInf[4].uid[0] == 0)
        {
            Arrow* pArrow = new Arrow(30, 255);
            pArrow->setPosition(CCPointMake(566, 570));
            pArrow->setRotation(90);
            addChild(pArrow, 500, kTeamInfArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
            
            //msgbox
            m_iNewbieMsgIdx = 0;
            m_msgbox = new MsgBox("ma_prompt.png", getTeamInfoPrompt(), ccc3(63, 23, 13));
            addChild(m_msgbox, 10);
            m_msgbox->release();
            m_msgbox->setPosition(CCPointMake(320, 320));
            m_msgbox->setScale(0.0);
            m_msgbox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));

        }
        else {
            m_iNewbieMsgIdx = 1;
            m_msgbox = new MsgBox("ma_prompt.png", getTeamInfoPrompt(), ccc3(63, 23, 13));
            addChild(m_msgbox, 10);
            m_msgbox->release();
            m_msgbox->setPosition(CCPointMake(320, 320));
            m_msgbox->setScale(0.0);
            m_msgbox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
            MainScene::Inst()->showTouchTip();
        }
    }
    
    
    
    
    //...
//    printf("使用[TeamInf::showTeamInfoList]开始\n");
//    OcProxy::Inst()->showUsedMemory();
    
    
}

bool TeamInf::isShowCardInfo()
{
    //    if (CCNode *pNode = getChildByTag(UI_TAB_SHOW_CARD))
    //    {
    //        return pNode->getIsVisible();
    //    }
    if (CShowCardInf *pShowCardInfo = dynamic_cast<CShowCardInf*>(getChildByTag(UI_TAB_SHOW_CARD)))
    {
        return pShowCardInfo->getChildBody();
    }
    return false;
}

void TeamInf::shareToWeibo()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(TeamInf::dl2CallBack),
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
        MainScene::Inst()->showWeiboPublishInfoWithType(3);
    }
}

//******************************************************************************
// dl2CallBack
//******************************************************************************
void TeamInf::dl2CallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(111);
    if(ret->bOk == true)
    {
        MainScene::Inst()-> gotoOtherMain();
    }else {
        dialog->removeFromParentAndCleanup(true);
    }
}   

//******************************************************************************
// update
//******************************************************************************
void TeamInf::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(TeamInf::cbCommDlg), enCommDlgTp_connecting);
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
    
    if(comState == kReqStatOk){
        if(CGameData::Inst()->isNewbie()){
            m_ret.iSel = kInvalidValue;
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
        else{

        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(TeamInf::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void TeamInf::cbCommDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
        if(CGameData::Inst()->newbieRequest(0, 5, pTeamInf[4].uid))
            scheduleUpdate();
    }
    else {
        CGameData::Inst()->clearReqStat();
    }    
}

//******************************************************************************
// getTeamInfoPrompt
//******************************************************************************
const char* TeamInf::getTeamInfoPrompt() {
    return CGameData::Inst()->getLanguageValue(gStrMsg[m_iNewbieMsgIdx]);
}

//******************************************************************************
// hide
//******************************************************************************
void TeamInf::hide()
{
    //setIsVisible(false);
    unschedule(schedule_selector(TeamInf::updateCostText));
//    removeChildByTag(UI_TAB_SHOW_COST_CARD,true);
//    removeChildByTag(UI_TAB_SHOW_COST_USER,true);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    printf("hide[CScrollViewLayer] cur child count [%d]\n",getChildrenCount());
}

//******************************************************************************
// onExit
//******************************************************************************
void TeamInf::onExit()
{
//    removeAllChildrenWithCleanup(true);
//    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    hide();
    CCLayer::onExit();
}

//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void TeamInf::shareSanGuo()
{
    if (!CGameData::Inst()->isNewbie())
    {
        int totalCost = CGameData::Inst()->getCurTeamCost();
        if(totalCost <= CGameData::Inst()->getUsrInfo()->cost)
            OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(TeamInf::shareToWeibo));
    }
}

//******************************************************************************
// updateCost
//******************************************************************************
void TeamInf::updateCost()
{
    std::map<std::string,int>   mapTemp;
    for (int i = 0; i < kMaxTeamCnt; i++)
    {
        if (stUid *pstUid = CGameData::Inst()->getTeamInfByIdx(i))
        {
            for (int j = 0; j < kMaxMemCntOfTeam; ++j)
            {
                if (""!=std::string(pstUid[j].uid))
                {
                    std::map<std::string,int>::iterator it = mapTemp.find(pstUid[j].uid);
                    if (it == mapTemp.end())
                    {
                        mapTemp.insert(std::pair<std::string, int>(pstUid[j].uid,pstUid[j].cost));
                    }
                    else
                    {
                        if (it->second<pstUid[j].cost)
                        {
                            mapTemp[pstUid[j].uid] = pstUid[j].cost;
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < kMaxTeamCnt; ++i)
    {
        if (stUid *pstUid = CGameData::Inst()->getTeamInfByIdx(i))
        {
            for (int j = 0; j < kMaxMemCntOfTeam; ++j)
            {
                std::map<std::string,int>::iterator it = mapTemp.find(pstUid[j].uid);
                if (it != mapTemp.end())
                {
                    pstUid[j].cost = it->second;
                }
            }
        }
    }
}

//******************************************************************************
// updateCostText
//******************************************************************************
void TeamInf::updateCostText(ccTime dt)
{
    if (CUserInfo* pUserInfo = CGameData::Inst()->getUsrInfo())
    {
//        if (pUserInfo->cost!=pUserInfo->show_user_cost)        //1
//        {
//            removeChildByTag(UI_TAB_SHOW_COST_CARD,true);
//            removeChildByTag(UI_TAB_SHOW_COST_USER,true);
//        }
        
        unsigned long ulCurTime = (unsigned long)time(NULL);
        if (0<show_hours_endTime && 0<show_hours_append_cost && ulCurTime>=show_hours_endTime)
        {
            show_user_cost -= show_hours_append_cost;
            show_hours_append_cost = 0;
            show_hours_endTime = 0;
            
            removeChildByTag(UI_TAB_SHOW_COST_CARD,true);
            removeChildByTag(UI_TAB_SHOW_COST_USER,true);
        }
        
        if (0<show_month_endTime && 0<show_month_append_cost && ulCurTime>=show_month_endTime)
        {
            show_user_cost -= show_month_append_cost;
            show_month_append_cost = 0;
            show_month_endTime = 0;
            
            removeChildByTag(UI_TAB_SHOW_COST_CARD,true);
            removeChildByTag(UI_TAB_SHOW_COST_USER,true);
        }
        
        if (getChildByTag(UI_TAB_SHOW_FRAME) && !getChildByTag(UI_TAB_SHOW_COST_CARD) && !getChildByTag(UI_TAB_SHOW_COST_USER))
        {
            int nCurTeamCost = CGameData::Inst()->getCurTeamCost();
            char cBuf[256];
            
            //cur team cost
            snprintf(cBuf,sizeof(cBuf),"%d/",nCurTeamCost);
            TextNode *pText = TextNode::textWithString(cBuf, CCSizeMake(144, 24), CCTextAlignmentRight, 24);
            pText->setPosition(CCPointMake(470, 460));
            pText->setColor(ccWHITE);
            pText->setShadowColor(ccBLACK);
            addChild(pText,0,UI_TAB_SHOW_COST_CARD);
            if(nCurTeamCost >show_user_cost)
            {
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.5, 128),
                                                                                   CCFadeTo::actionWithDuration(0.5, 255),
                                                                                   NULL);
                pText->setColor(ccRED);
                pText->setShadowColor(ccBLACK);
                pText->runActionInside(CCRepeatForever::actionWithAction(action));
            }
            
            //user cost
            snprintf(cBuf,sizeof(cBuf),"%d",show_user_cost);
            pText = TextNode::textWithString(cBuf, CCSizeMake(144, 24), CCTextAlignmentLeft, 24);
            pText->setPosition(CCPointMake(612, 460));
            pText->setColor(ccWHITE);
            pText->setShadowColor(ccBLACK);
            addChild(pText,0,UI_TAB_SHOW_COST_USER);
            
            unsigned long ulCurTime = (unsigned long)time(NULL);
            if ((0<show_hours_append_cost && ulCurTime<show_hours_endTime) ||
                (0<show_month_append_cost && ulCurTime<show_month_endTime))
            {
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.5, 128),
                                                                                   CCFadeTo::actionWithDuration(0.5, 255),
                                                                                   NULL);
                pText->setColor(ccGREEN);
                pText->setShadowColor(ccBLACK);
                pText->runActionInside(CCRepeatForever::actionWithAction(action));
            }
        }
    }
}






