//
//  CUsrInfoPanel.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CUsrInfoPanel.h"
#include "ActNumTTF.h"
#include "TextNode.h"
#include "SaveData.h"
#include "OrgSaveData.h"
#include "OcProxy.h"

#define TAG_USRINFO 211

static CCPoint LEDPositon[] = {
    CCPointMake(40, 35),
    CCPointMake(80, 35),
    CCPointMake(120, 34),
    CCPointMake(159, 33),
    CCPointMake(199, 32),
    CCPointMake(239, 31),
    CCPointMake(279, 31),
    
    CCPointMake(318, 30),
    
    CCPointMake(357, 31),
    CCPointMake(397, 31),
    CCPointMake(437, 32),
    CCPointMake(476, 33),
    CCPointMake(516, 34),
    CCPointMake(555, 35),
    CCPointMake(594, 35),
};

//******************************************************************************
// Constructor
//******************************************************************************
CUsrInfoPanel::CUsrInfoPanel()
{
    m_bShowCntryInfo = false;
    isJustLogin = true;
    m_spUserElem = NULL;
    m_spGoldOrSupply = NULL;
    m_spCntryInfoBg = NULL;
    m_spStaminaBuff = NULL;
    m_iCountry = 0;
    u_count = 0;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,-1, true);
    
    showUserInfo();
    
    crtExpLayer();
    
    crtCountryDetailLayer();
}

void CUsrInfoPanel::cleanCurrentInfo()
{    
    if (m_spGoldOrSupply) {
        m_spGoldOrSupply->removeFromParentAndCleanup(true);
        m_spGoldOrSupply = NULL;
    }
    
    if (m_bShowCntryInfo) {
        m_spCntryInfoBg->removeFromParentAndCleanup(true);
        m_spCntryInfoBg = NULL;

        m_bShowCntryInfo = false;
        
        m_spGoldOrSupply = CCSprite::spriteWithSpriteFrameName("id_coin.png");
        m_spGoldOrSupply->setPosition(CCPointMake(392, 848));
        addChild(m_spGoldOrSupply);
        
        m_lbGoldOrSupply->setNumber(m_pUsrInf->gold, 0.3);
        m_spUserElem->setIsVisible(true);
        if(m_pUsrInf->username)
            m_lbName->setString(m_pUsrInf->username);
    }
    else
    {
        m_lbGoldOrSupply->setNumber(m_pCntryInfo->user_supply, 0.3);
    }
}

bool CUsrInfoPanel::getIsShowCntryInfo()
{
    return m_bShowCntryInfo;
}

void CUsrInfoPanel::updateCntryNameCard()
{
    m_lbName->setString(m_pCntryInfo->nameCard);
}

void CUsrInfoPanel::showCntryInfo()
{
    m_bShowCntryInfo = true;
    
    m_spCntryInfoBg = CCSprite::spriteWithFile("cntryInfo_bg.png");
    m_spCntryInfoBg->setPosition(CCPointMake(225, 960 - 88));
    addChild(m_spCntryInfoBg, 1);
        
    //粮饷
    if (m_spGoldOrSupply == NULL) {
        m_spGoldOrSupply = CCSprite::spriteWithSpriteFrameName("icon_supply.png");
        m_spGoldOrSupply->setPosition(CCPointMake(392, 852));
        addChild(m_spGoldOrSupply);
    }
    
    //贡献值
    m_lbContribution = ActNumTTF::initNumber(CCSizeMake(500, 30),
                                             CCTextAlignmentLeft,
                                             24,
                                             ccc3(248,175,0));
    m_lbContribution->setPosition(CCPointMake(340, 62));
    m_spCntryInfoBg->addChild(m_lbContribution);
    m_lbContribution->setNumber(m_pCntryInfo->user_contribution, 0.3);
    
    // 行动点数
    m_lbAcPointValue = ActNumTTF::initNumber(CCSizeMake(100, 30),
                                             CCTextAlignmentLeft,
                                             24,
                                             ccc3(64,170,252));
    m_lbAcPointValue->setPosition(CCPointMake(150, 18));
    m_spCntryInfoBg->addChild(m_lbAcPointValue);
    
    // 冷却时间
    m_lbAcPointTime = TextNode::textWithString("",//"冷却 10:00:00",
                                               CCSizeMake(200, 30),
                                               CCTextAlignmentRight,
                                               18);
    m_lbAcPointTime->setAnchorPoint(CCPointMake(1, 0.5));
    m_lbAcPointTime->setPosition(CCPointMake(330, 18));
    m_spCntryInfoBg->addChild(m_lbAcPointTime);
    
    // 身份Icon
    int userStatus = 0;
    if (userStatus != m_pCntryInfo->status)
    {
        userStatus = m_pCntryInfo->status;
        if (userStatus <= 3 && userStatus > 0)
        {
            char buf[100];
            if (userStatus == 1)
                snprintf(buf, 99, "status_leader.png");
            else if (userStatus == 2)
                snprintf(buf, 99, "status_vleader.png");
            else if (userStatus == 3)
                snprintf(buf, 99, "status_member.png");
            
            CCSprite* spUserStatus = CCSprite::spriteWithFile(buf);
            spUserStatus->setPosition(CCPointMake(18, 100));
            m_spCntryInfoBg->addChild(spUserStatus);
            
            m_spUserElem->setIsVisible(false);
        }
    }
    
    // 同盟名片
    if(m_pCntryInfo->nameCard)
        m_lbName->setString(m_pCntryInfo->nameCard);
    
    if(m_pUsrInf->actionPoint == m_pUsrInf->max_acPoint)
        m_lbAcPointTime->setIsVisible(false);
    else {
        m_lbAcPointTime->setIsVisible(true);
    }
    
    m_lbAcPointValue->setMaxNum(m_pUsrInf->max_acPoint);
    m_lbAcPointValue->setNumber(m_pUsrInf->actionPoint, 0.3);
}

void CUsrInfoPanel::showUserInfo()
{
    CCSprite* sp;

    //user information frame
    sp = CCSprite::spriteWithSpriteFrameName("id_information.png");
    sp->setPosition(CCPointMake(320, 960 - 85));
    addChild(sp);
    sp->setTag(TAG_USRINFO);
    
//    if (CGameData::Inst()->getIsUseChristmasUI()) {
//        for (int i = 0; i < 15; i++) {
//            CCSprite* spLEDRed = CCSprite::spriteWithSpriteFrameName("id_ledred.png");
//            spLEDRed->setPosition(LEDPositon[i]);
//            sp->addChild(spLEDRed);
//            
//            CCSprite* spLEDYellow = CCSprite::spriteWithSpriteFrameName("id_ledyellow.png");
//            spLEDYellow->setPosition(LEDPositon[i]);
//            sp->addChild(spLEDYellow);
//            
//            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
//                                                                               CCHide::action(),
//                                                                               CCDelayTime::actionWithDuration(0.5),
//                                                                               CCShow::action(),
//                                                                               NULL);
//            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
//                                                                               CCShow::action(),
//                                                                               CCDelayTime::actionWithDuration(0.5),
//                                                                               CCHide::action(),
//                                                                               NULL);
//            if (i % 2 == 0) {
//                spLEDRed->runAction(CCRepeatForever::actionWithAction(action1));
//                spLEDYellow->setIsVisible(false);
//                spLEDYellow->runAction(CCRepeatForever::actionWithAction(action2));
//            }
//            else {
//                spLEDRed->setIsVisible(false);
//                spLEDRed->runAction(CCRepeatForever::actionWithAction(action2));
//                spLEDYellow->runAction(CCRepeatForever::actionWithAction(action1));
//            }
//            
//        }
//    }
    
    //元宝
    sp = CCSprite::spriteWithSpriteFrameName("id_gold.png");
    sp->setPosition(CCPointMake(480, 914));
    sp->setScale(0.6);
    addChild(sp);
    
    //铜钱
    if (m_spGoldOrSupply == NULL) {
        m_spGoldOrSupply = CCSprite::spriteWithSpriteFrameName("id_coin.png");
        m_spGoldOrSupply->setPosition(CCPointMake(392, 848));
        addChild(m_spGoldOrSupply);
    }
    
    //经验条
    m_spExpBar = CCSprite::spriteWithSpriteFrameName("id_exp bar.png");
    m_spExpBar->setAnchorPoint(CCPointMake(0, 0));
    m_spExpBar->setPosition(CCPointMake(90, 893));
    addChild(m_spExpBar);
    
    //体力条
    m_spStaminaBar = CCSprite::spriteWithSpriteFrameName("id_physical bar.png");
    m_spStaminaBar->setAnchorPoint(CCPointMake(0, 0));
    m_spStaminaBar->setPosition(CCPointMake(23, 839));
    addChild(m_spStaminaBar);
    
    m_lbName = CCLabelTTF::labelWithString("", CCSizeMake(240, 30), CCTextAlignmentLeft, "STHeitiSC-Medium", 24);
    m_lbName->setPosition(CCPointMake(172, 932));
    m_lbName->setColor(ccBLACK);
    addChild(m_lbName);
    
    m_txtTitleName = TextNode::textWithString("",CCSizeMake(240, 30), CCTextAlignmentLeft, 24);
    m_txtTitleName->setPosition(CCPointMake(172, 932));
    m_txtTitleName->setColor(ccc3(25,110,150));
    addChild(m_txtTitleName);
    
    char levelStr[50];
    sprintf(levelStr, "%s:",CGameData::Inst()->getLanguageValue("level"));
    TextNode* lbLv = TextNode::textWithString(levelStr, CCSizeMake(115, 30), CCTextAlignmentLeft, 24);
    lbLv->setPosition(CCPointMake(370, 924));
    lbLv->setColor(ccc3(248,175,0));
    addChild(lbLv);
    
    m_lbLv = ActNumTTF::initNumber(CCSizeMake(115, 30), CCTextAlignmentRight, 24, ccc3(248,175,0));
    m_lbLv->setPosition(CCPointMake(370, 924));
    addChild(m_lbLv);
    
    m_lbCoin = ActNumTTF::initNumber(CCSizeMake(120, 30), CCTextAlignmentRight, 24, ccc3(139,231,0));
    m_lbCoin->setPosition(CCPointMake(558, 914));
    addChild(m_lbCoin);
    
    m_lbGoldOrSupply = ActNumTTF::initNumber(CCSizeMake(205, 30), CCTextAlignmentRight, 24, ccc3(248,175,0));
    m_lbGoldOrSupply->setPosition(CCPointMake(514, 852));
    addChild(m_lbGoldOrSupply);
    
    m_lbStaminaTime = TextNode::textWithString("", CCSizeMake(140, 20), CCTextAlignmentLeft, 20);
    m_lbStaminaTime->setPosition(CCPointMake(210, 858));
    addChild(m_lbStaminaTime);
    
    m_lbStaminaValue = ActNumTTF::initNumber(CCSizeMake(200, 20), CCTextAlignmentRight, 20, ccc3(64,170,252));
    m_lbStaminaValue->setPosition(CCPointMake(248, 858));
    addChild(m_lbStaminaValue);
    
    m_strCdTip = CGameData::Inst()->getLanguageValue("cdTip");
}

void CUsrInfoPanel::updateAcPoint()
{
    long  tempTime = CGameData::Inst()->getCurrentTime();
    unsigned short acPointRecoverTime;
    if (CGameData::Inst()->getCommonInfo()->action_recover_time == 0)
        acPointRecoverTime = MAX(SaveData::Inst()->getAcPointRecoverTime(),
                                 OrgSaveData::Inst()->getAcPointRecoverTime());
    else
        acPointRecoverTime = CGameData::Inst()->getCommonInfo()->action_recover_time * 60;
    
    if (acPointRecoverTime == 0) {
        return;
    }
        
    char timeStr[50];
    long deltaTime=0;
    
    deltaTime = tempTime-baseTime;
    int tempCountDown ;
    tempCountDown = m_acPointCountDown-deltaTime;
    if(tempCountDown <= 0)
    {
        //显示体力值
        int sm = deltaTime/(60*acPointRecoverTime);//几个十小时
        int acPointValueAdd;
        //剩余的时间 如果比countDown大 +1 否则不加
        if((deltaTime%(60*acPointRecoverTime))>=m_acPointCountDown)
        {
            acPointValueAdd = sm+1;
        }
        else
            acPointValueAdd = sm;
        //基础体力 加计算出的部分
        m_tempAcPoint = m_baseAcPoint + acPointValueAdd;
        if(m_tempAcPoint > m_pUsrInf->max_acPoint)
            m_tempAcPoint = m_pUsrInf->max_acPoint;
        CGameData::Inst()->setLocalAcPoint(m_tempAcPoint);
        tempCountDown =60*acPointRecoverTime+(int)tempCountDown%(60*acPointRecoverTime);

        if (m_bShowCntryInfo) {
            m_lbAcPointValue->setMaxNum(m_pUsrInf->max_acPoint);
            m_lbAcPointValue->setNumber(m_tempAcPoint, 0.3);
            
            if (m_tempAcPoint == m_pUsrInf->max_acPoint)
                m_lbAcPointTime->setIsVisible(false);
        }
    }
    
    if (m_bShowCntryInfo) {
        sprintf(timeStr,"%s  %s", m_strCdTip.c_str(),
                CGameData::Inst()->getHourCountDownTimeStr(tempCountDown));
        m_lbAcPointTime->setString(timeStr);
    }
    
    //update append cost
//    if (CUserInfo *pUserInfo = CGameData::Inst()->getUsrInfo())
//    {
//        time(NU)
//        
//        if (0<pUserInfo->hours_endTime && 0<pUserInfo->hours_append_cost)
//        {
//            
//        }
//        if (0<pUserInfo->month_endTime)--pUserInfo->month_endTime;
//    }
}

void CUsrInfoPanel::updateTime(ccTime dt)
{
    long  tempTime = CGameData::Inst()->getCurrentTime();
    unsigned short staminaRecoverTime;
        
    if (CGameData::Inst()->getCommonInfo()->stamina_recover_time == 0) {
        staminaRecoverTime = MAX(SaveData::Inst()->getStaminaRecoverTime(),
                                 OrgSaveData::Inst()->getStaminaRecoverTime());
    }
    else {
        staminaRecoverTime = CGameData::Inst()->getCommonInfo()->stamina_recover_time * 60;
        
        CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
        staminaRecoverTime = staminaRecoverTime * (1 - cntryInfo->buffInfo[enBuffType_Stamina].effect);
    }
    
    if (staminaRecoverTime == 0) {
        return;
    }

    char timeStr[50];
    long deltaTime=0;
    
    deltaTime = tempTime-baseTime;
    int tempCountDown ;
    tempCountDown = countDown-deltaTime;
    if(tempCountDown <= 0)
    {
        //显示体力值
        int sm = deltaTime/staminaRecoverTime;//几个十分钟
        int staminaValueAdd;
        //剩余的时间 如果比countDown大 +1 否则不加
        if((deltaTime%staminaRecoverTime)>=countDown)
        {
            staminaValueAdd = sm+1;
        }
        else
            staminaValueAdd = sm;
        //基础体力 加计算出的部分
        tempStamina = baseStamina + staminaValueAdd;
        if(tempStamina >m_pUsrInf->max_stamina)
            tempStamina = m_pUsrInf->max_stamina;
        CGameData::Inst()->setLocalStamina(tempStamina);
        m_lbStaminaValue->setMaxNum(m_pUsrInf->max_stamina);
        m_lbStaminaValue->setNumber(tempStamina, 0.3);
        tempCountDown = staminaRecoverTime+(int)tempCountDown%staminaRecoverTime;
        float fPercent;        
        fPercent = tempStamina * 1.0 / m_pUsrInf->max_stamina;
        m_spStaminaBar->setScaleX(fPercent);
        if(tempStamina==m_pUsrInf->max_stamina)
        {
            m_lbStaminaTime->setIsVisible(false);
        }
    }
    sprintf(timeStr,"%s  %s", m_strCdTip.c_str(),
            CGameData::Inst()->getCountDownTimeStr(tempCountDown));
    m_lbStaminaTime->setString(timeStr);
    updateAcPoint();
}

//******************************************************************************
// showAcPoint
//******************************************************************************
void CUsrInfoPanel::showAcPoint()
{
    if (m_bShowCntryInfo) {
        if(m_pUsrInf->actionPoint == m_pUsrInf->max_acPoint)
            m_lbAcPointTime->setIsVisible(false);
        else {
            m_lbAcPointTime->setIsVisible(true);
        }
        
        m_lbAcPointValue->setMaxNum(m_pUsrInf->max_acPoint);
        m_lbAcPointValue->setNumber(m_pUsrInf->actionPoint, 0.3);
    }
    
    m_tempAcPoint = m_pUsrInf->actionPoint;
    m_baseAcPoint = m_pUsrInf->actionPoint;
}

//******************************************************************************
// showData
//******************************************************************************
void CUsrInfoPanel::showData()
{
    m_pUsrInf = CGameData::Inst()->getUsrInfo();
    m_pCntryInfo = CGameData::Inst()->getCntryInfo();
    
    if(m_pUsrInf == NULL)
        return;
    
    char buf[100];
    countDown = CGameData::Inst()->getCountDownTime();
    baseTime = CGameData::Inst()->getCurrentTime();
    m_acPointCountDown = CGameData::Inst()->getAPCountDownTime();

    if(m_pUsrInf->stamina == m_pUsrInf->max_stamina)
        m_lbStaminaTime->setIsVisible(false);
    else {
        m_lbStaminaTime->setIsVisible(true);
    }

    if(!m_bShowCntryInfo){
        m_lbName->setString(m_pUsrInf->username);
        
        if(m_pUsrInf->title_name.empty() == false){
            m_txtTitleName->setString(m_pUsrInf->title_name.c_str());
            m_lbName->stopAllActions();
            m_lbName->setOpacity(255);//刷新前复位透明度，避免显示异常
            CCActionInterval* action1 = (CCActionInterval *)CCSequence::actions(CCDelayTime::actionWithDuration(4.0),
                                                                                CCFadeOut::actionWithDuration(0.5),
                                                                                CCDelayTime::actionWithDuration(4.0),
                                                                                CCFadeIn::actionWithDuration(0.5),
                                                                                NULL);
            m_lbName->runAction(CCRepeatForever::actionWithAction(action1));
            
            m_txtTitleName->stopAllActionsInside();
            
            CCActionInterval* action2 = (CCActionInterval *)CCSequence::actions(CCHide::action(),
                                                                                CCDelayTime::actionWithDuration(4.0),
                                                                                CCShow::action(),
                                                                                CCFadeIn::actionWithDuration(0.5),
                                                                                CCDelayTime::actionWithDuration(4.0),
                                                                                CCFadeOut::actionWithDuration(0.5),
                                                                                NULL);
            m_txtTitleName->runActionInside(CCRepeatForever::actionWithAction(action2));
        }
    }
    
    if((m_spUserElem == NULL) || (m_iCountry != m_pUsrInf->user_elem)){
        if(m_spUserElem){
            removeChild(m_spUserElem, true);
            m_spUserElem = NULL;
        }
        
        switch (m_pUsrInf->user_elem) {
            case 1:
            case 2:
            case 3:
                m_iCountry = m_pUsrInf->user_elem;
                snprintf(buf, 99, "stone%ds.png", m_iCountry);
                m_spUserElem = CCSprite::spriteWithSpriteFrameName(buf);
                break;
                
            default:
                break;
        }
        
        if(m_spUserElem){
            m_spUserElem->setPosition(CCPointMake(36, 932));
            addChild(m_spUserElem);
        }
    }
    
    if (m_pCntryInfo->buffInfo[enBuffType_Stamina].effect > 0) {
        if (m_spStaminaBuff == NULL)
        {
            m_spStaminaBuff = CCSprite::spriteWithSpriteFrameName("buff_small_stamina.png");
            addChild(m_spStaminaBuff);
            m_spStaminaBuff->setPosition(CCPointMake(110, 858));
        }
    }
    else {
        if (m_spStaminaBuff)
        {
            m_spStaminaBuff->removeFromParentAndCleanup(true);
            m_spStaminaBuff = NULL;
        }
    }
    
    if(isJustLogin == true)
    {
        schedule(schedule_selector(CUsrInfoPanel::updateTime), 1);
        isJustLogin = false;
    }

    float fPercent = 0.0;
    fPercent = m_pUsrInf->stamina * 1.0 / m_pUsrInf->max_stamina;
    if (fPercent >= 0) {
        m_spStaminaBar->runAction(CCScaleTo::actionWithDuration(0.3, fPercent, 1));
    }
    
    fPercent = 0.0;
    fPercent = m_pUsrInf->this_lv_now_exp * 1.0 / (m_pUsrInf->this_lv_now_exp + m_pUsrInf->next_lv_need_exp);
    if(fPercent < m_spExpBar->getScaleX()){
        m_spExpBar->setScaleX(0.0);
    }
    if (fPercent > 0) {
        m_spExpBar->runAction(CCScaleTo::actionWithDuration(0.3, fPercent, 1));
    }

    m_lbLv->setNumber(m_pUsrInf->lv, 0.3);
    if (m_pUsrInf->lv >= 1000) {
        m_lbLv->setPosition(ccp(m_lbLv->getPosition().x + 2, m_lbLv->getPosition().y));
        m_lbLv->setScaleX(0.8);
    }
    
    if (m_bShowCntryInfo) {
        m_lbGoldOrSupply->setNumber(m_pCntryInfo->user_supply, 0.3);
        
        m_lbContribution->setNumber(m_pCntryInfo->user_contribution, 0.3);
    }
    else
        m_lbGoldOrSupply->setNumber(m_pUsrInf->gold, 0.3);
    
    m_lbCoin->setNumber(m_pUsrInf->coin, 0.3);
    
    m_lbStaminaValue->setMaxNum(m_pUsrInf->max_stamina);
    m_lbStaminaValue->setNumber(m_pUsrInf->stamina, 0.3);
    
    tempStamina = m_pUsrInf->stamina;
    baseStamina = m_pUsrInf->stamina;
    
    showAcPoint();
    
    //注册推送
    if (m_pUsrInf->stamina < m_pUsrInf->max_stamina) {
        long stamina_time;
        unsigned short staminaRecoverTime;
        staminaRecoverTime = CGameData::Inst()->getCommonInfo()->stamina_recover_time * 60;
        if (staminaRecoverTime == 0) {
            return;
        }
        stamina_time = m_pUsrInf->stamina_upd_time + (m_pUsrInf->max_stamina - tempStamina -1) * staminaRecoverTime;
        OcProxy::Inst()->setPushInfo((char*)(CGameData::Inst()->getLanguageValue("pushinf_stamina")),
                                     stamina_time,
                                     kPushInfStamina);
    }
    
    if (m_pUsrInf->stamina < CGameData::Inst()->intPowerRemind && CGameData::Inst()->intPowerRemind < m_pUsrInf->max_stamina) {
        char buf[100];
        snprintf(buf, 99,CGameData::Inst()->getLanguageValue("pushinf_stamina2"), CGameData::Inst()->intPowerRemind);
        long stamina_time;
        unsigned short staminaRecoverTime;
        staminaRecoverTime = CGameData::Inst()->getCommonInfo()->stamina_recover_time * 60;
        if (staminaRecoverTime == 0) {
            return;
        }
        stamina_time = m_pUsrInf->stamina_upd_time + (CGameData::Inst()->intPowerRemind - tempStamina -1) * staminaRecoverTime;
        OcProxy::Inst()->setPushInfo(buf,
                                     stamina_time,
                                     kPushInfStamina2);
    }
    
    OcProxy::Inst()->setPushInfo((char *)CGameData::Inst()->getLanguageValue("pushinf_sevenday"), 7*24*60*60, kPushInfSevenDay,true);
}

bool CUsrInfoPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->locationInView(pTouch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    CCNode* node = getChildByTag(TAG_USRINFO);
    
    if (this->getPositionInPixels().y > 100) {
        return false;
    }
    
    if(node == NULL)
        return false;
    
    CCPoint pt = node->getPosition();
    CCSize sz = node->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
   

    
    CCPoint pt_UserElem = m_spUserElem->getPosition();
    CCSize sz_UserElem = m_spUserElem->getContentSize();
    
    CCSize  sz_name = m_lbName->getContentSize();
    bool bRet2 = CCRect::CCRectContainsPoint(CCRectMake(pt_UserElem.x - sz_UserElem.width * 0.5,
                                                        pt_UserElem.y - sz_UserElem.height * 0.5,
                                                        sz_name.width+sz_UserElem.width,
                                                        sz_name.height),
                                             pos);
    
    if (bRet2) {
        setCountryDetail();
        showCountryDetail(pos);
        return true;
    }else{
        if(bRet)
        {
            setExpInfo();
            showExpInfo(pos);
            return true;
            
        }else {
            return false;
        }

    }
    
    
//    if(bRet)
//    {
//        
//        setExpInfo();
//        showExpInfo(pos);
//        
//        return true;
//        
//    }else {
//        return false;
//    }
    
}

//3 44
void CUsrInfoPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (ExpLayer) {
        ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
    }
    
    if (m_CounDLayer) {
        m_CounDLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
    }
}

void CUsrInfoPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->locationInView(pTouch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* node = getChildByTag(TAG_USRINFO);
    CCPoint pt = node->getPosition();
    CCSize sz = node->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                       pt.y - sz.height * 0.5,
                                                       sz.width,
                                                       sz.height),
                                            pos);
    
    
    CCPoint pt_UserElem = m_spUserElem->getPosition();
    CCSize sz_UserElem = m_spUserElem->getContentSize();
    
    CCSize  sz_name = m_lbName->getContentSize();
    bool bRet2 = CCRect::CCRectContainsPoint(CCRectMake(pt_UserElem.x - sz_UserElem.width * 0.5,
                                                        pt_UserElem.y - sz_UserElem.height * 0.5,
                                                        sz_name.width+sz_UserElem.width,
                                                        sz_name.height),
                                             pos);
    if (bRet2) {
       
        ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
        setCountryDetail();
        
        showCountryDetail(pos);
       
    }else{
        if (m_CounDLayer) {
            m_CounDLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
        }
        
        if(bRet)
        {
            setExpInfo();
            showExpInfo(pos);
            
        }else {
            ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
        }
        
    }

    
//    if (bRet)
//    {
//        showExpInfo(pos);
//    }
//    else
//    {
//        ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
//    }
}

void CUsrInfoPanel::showExpInfo(CCPoint pos)
{
    float w = ExpLayer->getContentSize().width;
    
    float x = 0.0f;
    if (pos.x < w)
    {
        x = pos.x + 80.0f;
        if (x > 640.0f - w)
        {
            x = 640.0f - w;
        }
    }
    else
    {
        x = pos.x - w - 80.0f;
        if (x < 0.0f)
        {
            x = 0.0f;
        }
    }
    
    ExpLayer->setPosition(ccp(x+(ExpLayer->getContentSize().width/2),ExpLayer->getPosition().y));
    ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
}

//restUserExpInfo
void CUsrInfoPanel::setExpInfo()
{
    char chrCurLv[20] = {0};
    sprintf(chrCurLv,"%s: %d",CGameData::Inst()->getLanguageValue("level"),  m_pUsrInf->lv);
    curLevel->setString(chrCurLv);
    
    char chrCurLvExp[30] = {0};
    sprintf(chrCurLvExp,"%s %lu", CGameData::Inst()->getLanguageValue("usrinf_curexp"), m_pUsrInf->exp);
    curLvExp->setString(chrCurLvExp);
    
    char chrNextLvExp[30] = {0};
    sprintf(chrNextLvExp,"%s %lu", CGameData::Inst()->getLanguageValue("usrinf_needexp"), m_pUsrInf->next_lv_need_exp);
    nextLvExp->setString(chrNextLvExp);
}

//Create UserExpInfo
void CUsrInfoPanel::crtExpLayer()
{
    ExpLayer = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    ExpLayer->setPosition(ccp(220, 832+(ExpLayer->getContentSize().height/2)));
    addChild(ExpLayer, 100);
    
    curLevel = TextNode::textWithString("", CCSize(400,100), CCTextAlignmentLeft, 23);
    ExpLayer->addChild(curLevel, 10);
    curLevel->setPosition(ccp(219,86));
    
    curLvExp = TextNode::textWithString("", CCSize(400,100), CCTextAlignmentLeft, 23);
    ExpLayer->addChild(curLvExp, 10);
    curLvExp->setPosition(ccp(219,54));
    
    nextLvExp = TextNode::textWithString("", CCSize(400,100), CCTextAlignmentLeft, 23);
    ExpLayer->addChild(nextLvExp, 10);
    nextLvExp->setPosition(ccp(219,22));
    
    ExpLayer->setScale(0);
}


void CUsrInfoPanel::setCountryDetail()
{
    switch (m_pUsrInf->user_elem) {
        case 1:
        {
            char country_detail_char[256] = {0};
            snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_weiguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        case 2:
        {
            char country_detail_char[256] = {0};
             snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_shuguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        case 3:{
            char country_detail_char[256] = {0};
        
             snprintf(country_detail_char, sizeof(country_detail_char), "%s",CGameData::Inst()->getLanguageValue("usrinf_wuguoDetail"));
            m_tcountrydetail->setString(country_detail_char);
        }
            break;
        default:
            break;
    }
}

void CUsrInfoPanel::crtCountryDetailLayer()
{
    m_CounDLayer = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    m_CounDLayer->setPosition(ccp(220, 832+(m_CounDLayer->getContentSize().height/2)));
    addChild(m_CounDLayer, 100);
    
    m_tcountrydetail = TextNode::textWithString("", CCSize(310, 300), CCTextAlignmentLeft, 21);

     m_tcountrydetail->setPosition(ccp(180, 54));
    m_CounDLayer->addChild(m_tcountrydetail, 10);
    
    m_CounDLayer->setScale(0);
}

void CUsrInfoPanel::showCountryDetail(CCPoint pos)
{
    float w = m_CounDLayer->getContentSize().width;
    
    float x = 0.0f;
    if (pos.x < w)
    {
        x = pos.x + 80.0f;
        if (x > 640.0f - w)
        {
            x = 640.0f - w;
        }
    }
    else
    {
        x = pos.x - w - 80.0f;
        if (x < 0.0f)
        {
            x = 0.0f;
        }
    }
    
    m_CounDLayer->setPosition(ccp(x+(m_CounDLayer->getContentSize().width/2),m_CounDLayer->getPosition().y));
    m_CounDLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
}

//******************************************************************************
// onExit
//******************************************************************************
void CUsrInfoPanel::onEnter()
{
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->setPriority(-160, this);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(CUsrInfoPanel::showData),kNotifyUpdateUsrInf,
                                                           NULL);
}
void CUsrInfoPanel::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyUpdateUsrInf);
    
    CCLayer::onExit();
}






