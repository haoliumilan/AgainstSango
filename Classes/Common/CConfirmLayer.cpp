//
//  CConfirmLayer.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CConfirmLayer.h"
#include "CGamedata.h"
#include "SimpleAudioEngine.h"
#include "CSkillLib.h"
#include "TextNode.h"

#include "Arrow.h"

#define kArenaArrowTag          234

#define kArenaTempBtn          333


using namespace CocosDenshion;

#define enFrLBtn_Width      188
#define enFrLBtn_Height     79

static CCPoint butPosition[3][5] = {
    {CCPointMake(320, 425), CCPointMake(320, 325), CCPointMake(320, 225),},
    {CCPointMake(320, 440), CCPointMake(320, 360), CCPointMake(320, 280), CCPointMake(320, 200),},
    {CCPointMake(200, 425), CCPointMake(440, 425), CCPointMake(200, 325), CCPointMake(440, 325), CCPointMake(320, 225),},
};

//******************************************************************************
// Constructor
//******************************************************************************
CConfirmLayer::CConfirmLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_cfmLayerType = enCConfirmLayer_NORMAL;
    
    m_NewbieFirst = false;
    
    setIsTouchEnabled(true);
    this->setIsVisible(false);
}

bool CConfirmLayer::isSelectedForPos(cocos2d::CCPoint pos, int btnCount)
{
    bool bRet;
    
    for (int i = 0; i < btnCount; i++) {
        bRet = CCRect::CCRectContainsPoint(CCRectMake(butPosition[btnCount-3][i].x - enFrLBtn_Width * 0.5,
                                                      butPosition[btnCount-3][i].y - enFrLBtn_Height * 0.5,
                                                      enFrLBtn_Width,
                                                      enFrLBtn_Height),
                                           pos);
        if (bRet) {
            m_ret.iBtnSel = i;
            return bRet;
        }
    }
    
    return false;
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CConfirmLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    bool bRet;
    if (m_cfmLayerType == enCConfirmLayer_NORMAL || m_cfmLayerType == enCConfirmLayer_LOCK
        || m_cfmLayerType == enCConfirmLayer_UNLOCK || m_cfmLayerType == enCConfirmLayer_MMember
        || m_cfmLayerType == enCConfirMLayer_MMStranger
        || m_cfmLayerType == enCConfirmLayer_EVO
        || m_cfmLayerType == enCConfirmLayer_SEVO
        || m_cfmLayerType == enCConfirmLayer_Wake
        || m_cfmLayerType == enCConfirmLayer_POWUP
        || m_cfmLayerType == enCConfirmLayer_NIEPAN)
        

    {

        
        bRet = isSelectedForPos(pos, 3);
    }
    
    
    else if (m_cfmLayerType == enCConfirmLayer_FRIEND || m_cfmLayerType == enCConfirmLayer_VLMember
             || m_cfmLayerType == enCConfirmLayer_VLMStranger
             || m_cfmLayerType == enCConfirmLayer_POWUPNIEPAN
             || m_cfmLayerType == enCConfirmlayer_maa_arm
             || enCConfirmLayer_nEVO_Wake == m_cfmLayerType
             
             )
        
    {

        
        bRet = isSelectedForPos(pos, 4);
    }
    
    else if (m_cfmLayerType == enCConfirmLayer_LMember || m_cfmLayerType == enCConfirmLayer_LMStranger
             || m_cfmLayerType == enCConfirmLayer_LViceleader || m_cfmLayerType == enCConfirmLayer_LVStranger)
    {

        bRet = isSelectedForPos(pos, 5);

        
    } else if (m_cfmLayerType == enCConfirmLayer_nFiveTeam) {
        for (int i = 0; i < 2; i++) {
            bRet = CCRect::CCRectContainsPoint(CCRectMake(butPosition[0][i].x - enFrLBtn_Width * 0.5,
                                                          butPosition[0][i].y - enFrLBtn_Height * 0.5,
                                                          enFrLBtn_Width,
                                                          enFrLBtn_Height),
                                                          pos);
            if (bRet) {
                m_ret.iBtnSel = i;
                break;
            }
        }
        // 如果点击到5个友军head就查看详情
        if (!bRet) {
            for (int i = 0; i < 5; i++) {
                CCSprite *node = (CCSprite *)getChildByTag(100 + i);
                bRet = CCRect::CCRectContainsPoint(CCRectMake(node->getPosition().x - 108 * 0.5,
                                                              node->getPosition().y - 108 * 0.5,
                                                              108,
                                                              108),
                                                   pos);
                if (bRet) {
                    m_ret.iBtnSel = i+2;
                    break;
                }
            }
        }
    }
    

    if (bRet)
    {
        
        if(CGameData::Inst()->isNewbie() && (CGameData::Inst()->getNewbieStep() == 1) && !m_NewbieFirst)
        {
            
            if(m_ret.iBtnSel == 0 || m_ret.iBtnSel == 2)
            {
                return false;
            }
            
            
            
            CCNode * spBtn = getChildByTag(kArenaTempBtn);
            if(spBtn)
                removeChild(spBtn, true);
            
            spBtn = getChildByTag(kArenaTempBtn + 2);
            if(spBtn)
                removeChild(spBtn, true);
            
            CCNode* pArrow = getChildByTag(kArenaArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
            
            m_NewbieFirst = true;
        }
        
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        CCLog("CConfirmLayer iBtnSel = %d", m_ret.iBtnSel);
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    
    return true;
}

void CConfirmLayer::showBtnNormal()
{
  //  const char* title[] = {"选 择", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_select_"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnLock()
{
   // const char* title[] = {"加 锁", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_lock_"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnUnlock()
{    
   // const char* title[] = {"解 锁", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_unlock_"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnFriend()
{            
  //  const char* title[] = {"发邮件", "删除好友", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("con_delete_friend"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 4);
}

void CConfirmLayer::showBtnLViceleader()     // leader's viceleader
{
  //  const char* title[] = {"发邮件", "武将详情", "解 职", "让 贤", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_dismis_"), CGameData::Inst()->getLanguageValue("con_giveup_"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 5);
}

void CConfirmLayer::showBtnLVStranger()
{
   // const char* title[] = {"申请好友", "武将详情", "解 职", "让 贤", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_apply_friend"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_dismis_"), CGameData::Inst()->getLanguageValue("con_giveup_"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 5);
}

void CConfirmLayer::showBtnLMember()          // leader's member
{    
  //  const char* title[] = {"发邮件", "武将详情", "删除成员", "任 命", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_delete_member"), CGameData::Inst()->getLanguageValue("con_commit_"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 5);
}

void CConfirmLayer::showBtnLMStranger()
{
   // const char* title[] = {"申请好友", "武将详情", "删除成员", "任 命", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_apply_friend"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_delete_member"), CGameData::Inst()->getLanguageValue("con_commit_"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 5);
}

void CConfirmLayer::showBtnVLMember()         // viceleader's member
{
  //  const char* title[] = {"发邮件", "武将详情", "删除成员", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_delete_member"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 4);
}

void CConfirmLayer::showBtnVLMStranger()
{
  //  const char* title[] = {"申请好友", "武将详情", "删除成员", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_apply_friend"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_delete_member"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 4);
}

void CConfirmLayer::showBtnPowUp()
{
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_powup"),
        CGameData::Inst()->getLanguageValue("con_general_info"),
        CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnNiePan()
{
    const char* title[] = {
        "",
        CGameData::Inst()->getLanguageValue("con_general_info"),
        CGameData::Inst()->getLanguageValue("con_back_")};
    
    CCSprite* spBtn;
    
    for (int i = 0; i < 3; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn) {
            if (i == 0) {
                CCSprite* sp = CCSprite::spriteWithFile("title_niepan.png");
                if (sp) {
                    spBtn->addChild(sp);
                    sp->setPosition(CCPointMake(spBtn->getContentSize().width*0.5, spBtn->getContentSize().height*0.5));
                }
            }
            else{
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
            }
            
            spBtn->setPosition(butPosition[0][i]);
            addChild(spBtn);
        }
    }
}

void CConfirmLayer::showBtnPowUpNiePan()
{
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_powup"),
        "",
        CGameData::Inst()->getLanguageValue("con_general_info"),
        CGameData::Inst()->getLanguageValue("con_back_")};

    CCSprite* spBtn;
    
    for (int i = 0; i < 4; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn) {
            if (i == 1) {
                CCSprite* sp = CCSprite::spriteWithFile("title_niepan.png");
                if (sp) {
                    spBtn->addChild(sp);
                    sp->setPosition(CCPointMake(spBtn->getContentSize().width*0.5, spBtn->getContentSize().height*0.5));
                }
            }
            else{
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
            }
            
            spBtn->setPosition(butPosition[1][i]);
            addChild(spBtn);
        }
    }
}

void CConfirmLayer::showBtnMAAarm()
{
    //  const char* title[] = {"发邮件", "拜师", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("maa_request_master"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 4);
}

void CConfirmLayer::showBtnMMember()            // member's member
{
  //  const char* title[] = {"发邮件", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_send_mail"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnMMStranger()
{
  //  const char* title[] = {"申请好友", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_apply_friend"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtn_Evo_Wake()
{
    //  const char* title[] = {"转生", "武将详情", "返 回"};
    const char* title[] = {"" ,CGameData::Inst()->getLanguageValue("evolution_title"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    CCSprite* spBtn;
    
    for (int i = 0; i < 4; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn)
        {
            if (i == 0)
            {
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("wake_btntitle.png");
                if (sp) {
                    spBtn->addChild(sp);
                    sp->setPosition(CCPointMake(spBtn->getContentSize().width*0.5, spBtn->getContentSize().height*0.5));
                }
            }
            else
            {
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
            }
            
            spBtn->setPosition(butPosition[1][i]);
            addChild(spBtn);
        }
    }

}

void CConfirmLayer::showBtnEvo()
{
    //  const char* title[] = {"转生", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("evolution_title"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 3);
}

void CConfirmLayer::showBtnSevo()
{
    //  const char* title[] = {"转生", "武将详情", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("evolution_title"), CGameData::Inst()->getLanguageValue("con_general_info"), CGameData::Inst()->getLanguageValue("con_back_")};
    
    CCSprite* spBtn;
    
    for (int i = 0; i < 3; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn) {
            if (i == 0) {
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("sevo_btntitle.png");
                if (sp) {
                    spBtn->addChild(sp);
                    sp->setPosition(CCPointMake(spBtn->getContentSize().width*0.5, spBtn->getContentSize().height*0.5));
                }
            }
            else{
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
            }
            
            spBtn->setPosition(butPosition[3 - 3][i]);
            addChild(spBtn);
        }
    }
}

void CConfirmLayer::showBtnWake()
{
    //  const char* title[] = {"转生", "武将详情", "返 回"};
    const char* title[] = {
        CGameData::Inst()->getLanguageValue("evolution_title"),
        CGameData::Inst()->getLanguageValue("con_general_info"),
        CGameData::Inst()->getLanguageValue("con_back_")};
    
    CCSprite* spBtn;
    
    for (int i = 0; i < 3; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn) {
            if (i == 0) {
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("wake_btntitle.png");
                if (sp) {
                    spBtn->addChild(sp);
                    sp->setPosition(CCPointMake(spBtn->getContentSize().width*0.5, spBtn->getContentSize().height*0.5));
                }
            }
            else{
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
            }
            
            spBtn->setPosition(butPosition[0][i]);
            addChild(spBtn);
        }
    }
}

void CConfirmLayer::showBtnFiveTeam()
{
    //  const char* title[] = {"确 认", "返 回"};
    const char* title[] = {CGameData::Inst()->getLanguageValue("con_sure_"), CGameData::Inst()->getLanguageValue("con_back_")};
    showSameBtn(title, 2);
}

void CConfirmLayer::showSameBtn(const char **title, int btnCount)
{
    CCSprite* spBtn;
    
    for (int i = 0; i < btnCount; i++) {
        spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        
        TextNode* tnTemp = TextNode::textWithString(title[i],
                                                    spBtn->getContentSize(),
                                                    CCTextAlignmentCenter,
                                                    36);
        tnTemp->setShadowColor(ccBLACK);
        
        //if(CGameData::Inst()->isNewbie() && (CGameData::Inst()->getNewbieStep() == 1))
            

        
        
        tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                        spBtn->getContentSize().height * 0.5));
        spBtn->addChild(tnTemp);
        
        if (btnCount < 3) {
            spBtn->setPosition(butPosition[0][i]);
        } else {
            spBtn->setPosition(butPosition[btnCount - 3][i]);
        }
        addChild(spBtn);
        
        
        if(CGameData::Inst()->isNewbie() && (CGameData::Inst()->getNewbieStep() == 1) && !m_NewbieFirst)//添加蒙版按钮
        {
            if(i == 0 || i == 2)
            {
                
                spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
                
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setColor(ccc3(160, 160, 160));
                
                
                
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);
                
                if (btnCount < 3) {
                    spBtn->setPosition(butPosition[0][i]);
                } else {
                    spBtn->setPosition(butPosition[btnCount - 3][i]);
                }
                
                spBtn->setColor(ccc3(160, 160, 160));
                
                spBtn->setTag(kArenaTempBtn + i );
                
                addChild(spBtn );
                
                
                if(i == 0)
                {
                    Arrow* pArrow = new Arrow(30, 255);
                    
                    pArrow->setPosition(CCPointMake(436, 320));
                    
                    addChild(pArrow, 500, kArenaArrowTag);
                    pArrow->release();
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));
                    
                }
                
            }
        }
    }
}

//******************************************************************************
// show
//******************************************************************************
void CConfirmLayer::show(CUserCardInfo* pUsrCard, const char* frdname, enCConfirmLayerType type,int nOPIndex)
{
    m_ret.iConfirmType = type;
    m_nOPIndex = nOPIndex;
    removeAllChildrenWithCleanup(true);
    if (enCConfirmlayer_maa_arm==type)
    {
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority, true);
    }
    else
    {
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    }
    this->setIsVisible(true);
    m_cfmLayerType = type;
    CCLog("m_cfmLayerType = %d, type = %d", m_cfmLayerType, type);
    //button
    switch (type) {
        case enCConfirmLayer_NORMAL:
            showBtnNormal();
            break;
            
        case enCConfirmLayer_LOCK:
            showBtnLock();
            break;
            
        case enCConfirmLayer_UNLOCK:
            showBtnUnlock();
            break;
            
        case enCConfirmLayer_FRIEND:
            showBtnFriend();
            break;
            
        case enCConfirmLayer_LViceleader:
            showBtnLViceleader();
            break;
            
        case enCConfirmLayer_LVStranger:
            showBtnLVStranger();
            break;
            
        case enCConfirmLayer_LMember:
            showBtnLMember();
            break;
            
        case enCConfirmLayer_LMStranger:
            showBtnLMStranger();
            break;
            
        case enCConfirmLayer_VLMember:
            showBtnVLMember();
            break;
        
        case enCConfirmLayer_VLMStranger:
            showBtnVLMStranger();
            break;
            
        case enCConfirmLayer_MMember:
            showBtnMMember();
            break;
            
        case enCConfirMLayer_MMStranger:
            showBtnMMStranger();
            break;
            
        case enCConfirmLayer_nEVO_Wake:
            showBtn_Evo_Wake();
            break;
            
        case enCConfirmLayer_EVO:
            showBtnEvo();
            break;
            
        case enCConfirmLayer_SEVO:
            showBtnSevo();
            break;
            
        case enCConfirmLayer_Wake:
            showBtnWake();
            break;
            
        case enCConfirmLayer_POWUP:
            showBtnPowUp();
            break;
            
        case enCConfirmLayer_NIEPAN:
            showBtnNiePan();
            break;

        case enCConfirmLayer_POWUPNIEPAN:
            showBtnPowUpNiePan();
            break;
        case enCConfirmlayer_maa_arm:
            showBtnMAAarm();
            break;
        case enCConfirmLayer_nFiveTeam:
            showBtnFiveTeam();
            break;

        default:
            break;
    }
    
    //card sprite
    char buf[100];

    CCSprite* spCard = CGameData::Inst()->getHeadSprite(pUsrCard->cid);
    if (spCard) {
        spCard->setPosition(CCPointMake(226, 574));
        addChild(spCard);
        
        //究极金边
        CCSprite* sp1 = NULL;
        if (pUsrCard->bIsEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (pUsrCard->bIsWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            spCard->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, spCard->getContentSize().height-4));
        }
    }
    
    CCSprite* spSkill;
    if(pUsrCard->skid > 0)
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill.png");
    else
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill gray.png");
    
    spSkill->setPosition(CCPointMake(198, 512));
    addChild(spSkill);
    
    
    CCSprite* spLSkill;
    if(pUsrCard->leader_skid > 0)
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill.png");
    else
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill gray.png");
    
    spLSkill->setPosition(CCPointMake(252, 512));
    addChild(spLSkill);
    
    //text
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("lv"));
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 596));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    if(pUsrCard->lv == pUsrCard->max_lv){
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("max"));
    }
    else{
        snprintf(buf, 99, "%d", pUsrCard->lv);
    }
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 596));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    if(pUsrCard->skid > 0){
        snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("skill_lv"));
        lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
        lbText->setPosition(CCPointMake(282, 574));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        addChild(lbText);
        
        stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pUsrCard->skid);
        if (pManSkill && pUsrCard->sk_lv == pManSkill->cMaxLv) {
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
        }
        else {
            sprintf(buf, "%d", pUsrCard->sk_lv);
        }
        lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
        lbText->setPosition(CCPointMake(282, 574));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        addChild(lbText);
    }
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("hp"));
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 552));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
//    sprintf(buf, "%ld", pUsrCard->hp);
    sprintf(buf, "%ld", pUsrCard->hp.get());
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 552));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("attack"));
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 530));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
//    sprintf(buf, "%lu", pUsrCard->attack);
    sprintf(buf, "%lu", pUsrCard->attack.get());
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 530));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("recover"));
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, 508));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    sprintf(buf, "%ld", pUsrCard->recover);
    lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentRight, 20);
    lbText->setPosition(CCPointMake(282, 508));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    CCSprite* spStar;
    for(int i = 0; i < pUsrCard->star; i++){
        spStar = CCSprite::spriteWithSpriteFrameName("id_star.png");
        float starBeginPosX = 320 - spStar->getContentSize().width * (pUsrCard->star - 1) / 2;
        spStar->setPosition(CCPointMake(starBeginPosX + spStar->getContentSize().width * i, 640));
        addChild(spStar);
    }
    
    
    lbText = TextNode::textWithString(pUsrCard->name, CCSizeMake(640, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(320, 650 + 30));
    lbText->setColor(ccWHITE);
    addChild(lbText);
    
    if(frdname != NULL){
        lbText = TextNode::textWithString(frdname, CCSizeMake(640, 36), CCTextAlignmentCenter, 36);
        lbText->setPosition(CCPointMake(320, 700 + 30));
        lbText->setColor(ccWHITE);
        addChild(lbText);
    }
    
    //action
    CCPoint pt = this->getPosition();
    this->setPosition(CCPointMake(640, pt.y));
    this->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, pt.y)), 0.8f));
}

//******************************************************************************
// show
//******************************************************************************
void CConfirmLayer::showFiveTeam(CCMutableArray<CUserCardInfo *> *pUsrCardArr, enCConfirmLayerType type,int nOPIndex)
{
    m_ret.iConfirmType = enCConfirmLayer_nFiveTeam;
    m_nOPIndex = nOPIndex;
    removeAllChildrenWithCleanup(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    this->setIsVisible(true);
    m_cfmLayerType = enCConfirmLayer_nFiveTeam;
    CCLog("m_cfmLayerType = %d, type = %d", m_cfmLayerType, enCConfirmLayer_nFiveTeam);
    //button
    showBtnFiveTeam();
    
    //card sprite
    char buf[100];
    
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("helperSureTip"));
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(300, 60), CCTextAlignmentCenter, 42);
    lbText->setPosition(CCPointMake(170, 700));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccc3(255, 180, 0));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText);
    
    for (int i = 0; i < pUsrCardArr->count(); i++) {
        CUserCardInfo *pUsrCard = pUsrCardArr->getObjectAtIndex(i);
        
        CCSprite* spCard = CGameData::Inst()->getHeadSprite(pUsrCard->cid);
        if (spCard) {
            spCard->setPosition(CCPointMake(100+110*i, 618));
            spCard->setTag(100+i);
            addChild(spCard);
            
            //究极金边
            CCSprite* sp1 = NULL;
            if (pUsrCard->bIsEluTgt) {
                sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
            }
            else if (pUsrCard->bIsWkTgt) {
                sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
            }
            
            if (sp1) {
                spCard->addChild(sp1);
                sp1->setAnchorPoint(CCPointMake(0, 1));
                sp1->setPosition(ccp(4, spCard->getContentSize().height-4));
            }
        }
        
        CCSprite* spLvBg = CCSprite::spriteWithSpriteFrameName("fr_lv.png");
        if(spLvBg){
            CCPoint pt = spCard->getPosition();
            pt.y -= spCard->getContentSize().height * 0.5;
            pt.y += 8;
            spLvBg->setPosition(pt);
            addChild(spLvBg);
        }
        
        if(pUsrCard->lv == pUsrCard->max_lv){
            snprintf(buf, 99, "Lv.%s", CGameData::Inst()->getLanguageValue("max"));
        }
        else{
            snprintf(buf, 99, "Lv.%d", pUsrCard->lv);
        }
        lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentCenter, 20);
        lbText->setPosition(CCPointMake(18+110*i, 560));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText);
        
        if(pUsrCard->skid > 0){
            stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pUsrCard->skid);
            if (pManSkill && pUsrCard->sk_lv == pManSkill->cMaxLv) {
                sprintf(buf, "SLv.%s", CGameData::Inst()->getLanguageValue("max"));
            }
            else {
                sprintf(buf, "SLv.%d", pUsrCard->sk_lv);
            }
            lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(18+110*i, 530));
            lbText->setAnchorPoint(CCPointZero);
            lbText->setColor(ccc3(255, 180, 0));
            lbText->setShadowColor(ccBLACK);
            addChild(lbText);
        }
        
        snprintf(buf, 99, "+%d", pUsrCard->plus);
        lbText = TextNode::textWithString(buf, CCSizeMake(172, 20), CCTextAlignmentCenter, 20);
        lbText->setPosition(CCPointMake(18+110*i, 500));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccc3(0, 223, 105));
        lbText->setShadowColor(ccBLACK);
        addChild(lbText);
    }


    //action
    CCPoint pt = this->getPosition();
    this->setPosition(CCPointMake(640, pt.y));
    this->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, pt.y)), 0.8f));
}


enCConfirmLayerType CConfirmLayer::getCfmLayerType()
{
    return m_cfmLayerType;
}

//******************************************************************************
// hide
//******************************************************************************
void CConfirmLayer::hide()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    this->setIsVisible(false);
}

//******************************************************************************
// onExit
//******************************************************************************
void CConfirmLayer::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}
