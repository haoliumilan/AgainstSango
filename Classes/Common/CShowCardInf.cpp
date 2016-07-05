//
//  CShowCardInf.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CShowCardInf.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "MainScene.h"
#include "TextNode.h"
#include "TitleBar.h"
#include "OcProxy.h"
#include "PackCard.h"
#include "TipsAwakeSkill.h"
//******************************************************************************
// show
//******************************************************************************
#define TAB_SPRITE_BODY     404
#define TAB_SPRITE_INFO_BOX 405

CShowCardInf::CShowCardInf()
{
    b_showAwakeInfo     = false;
    m_pfnSelector       = NULL;
    m_pListener         = NULL;
    m_bCanBeTouched     = false;
    m_bShowBtmAfterHide = true;
    m_bForbShowRangeBtn = false;
    m_source            = NULL;
    m_bUseScrollView = false;
    m_pUsrCardInfo = NULL;
    m_bisDropImage = false;
}

void CShowCardInf::show(CUserCardInfo* pUsrCardInfo, bool bSelf)
{
    if(pUsrCardInfo == NULL)
        return;
     m_bisDropImage = false;
    removeAllChildrenWithCleanup(true);
    this->setIsVisible(true);
    
    m_pUsrCardInfo = pUsrCardInfo;
    
    
    
    TextNode* text;
    //background
    if (!m_bUseScrollView)
    {
        setIsTouchEnabled(false);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,kCCMenuTouchPriority-1, true);
        
        //设置背景
        CCSprite* spBg = CCSprite::spriteWithFile("show_bg.jpg");
        spBg->setScale(1.0/0.8);
        spBg->setPosition(CCPointMake(320, 480));
        addChild(spBg, -10);
        
        //设置火炬
        showTorchParticle();
    }
    
    //info box
    CCSprite* spInf = CCSprite::spriteWithFile("pa_information.png");
    spInf->setPosition(CCPointMake(320, 198));
    addChild(spInf, -9,TAB_SPRITE_INFO_BOX);
    
    if (!m_bUseScrollView)
    {
        //设置返回button
        CCSprite *backBtn = CCSprite::spriteWithSpriteFrameName("id_back.png");
        backBtn->setPosition(ccp(54,766));
        addChild(backBtn);
        
        //设置 source button  来源 详情
        m_source = (TitleBar*) CCSprite::spriteWithSpriteFrameName("id_Reset.png");
        m_source -> setPosition(CCPointMake(580, 766));
        m_source -> setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
        m_source -> SetTitlePosition(CCPointMake(12, 15));
        m_source -> setTag(kLaiYuan);
        addChild(m_source);
    }
    
    if (!pUsrCardInfo->find_me.empty())
    {
        // source figure
        CCSprite* sp_source = CCSprite::spriteWithFile("pa_information1.png");
        addChild(sp_source, 10, kSource);
        sp_source -> setPosition(CCPointMake(320, 124));
        sp_source -> setScale(0);
        
        const char* ch0 = pUsrCardInfo->find_me.c_str();
        TextNode* text0 = TextNode::textWithString(ch0, CCSizeMake(520, 110), CCTextAlignmentCenter, 24);
        text0->setPosition(CCPointMake(294, 82));
        text0->setShadowColor(ccBLACK);
        sp_source->addChild(text0);
    }
    
    if (m_source && pUsrCardInfo->find_me.empty())
    {
        m_source -> setScale(0);
    }

    char buf[100];
    //card sprite
    CCSprite* spCard = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
    if (spCard) {
        //        spCard->setPosition(CCPointMake(98 - 5, 280));
        spCard->setAnchorPoint(CCPointMake(0.0f, 1.0f));
        spCard->setPosition(CCPointMake(40, 334));
        
        addChild(spCard);
        
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
    }
    
    //body sprite
    CCSprite* spBody = CGameData::Inst()->getBodySprite(pUsrCardInfo->cid);
    spBody->setAnchorPoint(CCPointMake(0.5, 0));
    spBody->setPosition(CCPointMake(320, 400));
    addChild(spBody,0,TAB_SPRITE_BODY);
    
    //添加该武将的国家属性
    CCSprite* spCountryIcon =NULL;
    switch (pUsrCardInfo->cou_name) {
        case 0://标志为无
            spCountryIcon = CCSprite::spriteWithFile("wu.png"); //国家属性图标
            break;
        case 1://标志为汉
            spCountryIcon = CCSprite::spriteWithFile("hanchao.png"); //国家属性图标
         
            break;
        case 2://标志为魏
            spCountryIcon = CCSprite::spriteWithFile("weiguo.png"); //国家属性图标
          
            break;
        case 3://标志为蜀
            spCountryIcon = CCSprite::spriteWithFile("shuguo.png"); //国家属性图标
           
            break;
        case 4://标志为吴
            spCountryIcon = CCSprite::spriteWithFile("wuguo.png"); //国家属性图标
          
            break;
        case 5://标志为群
            spCountryIcon = CCSprite::spriteWithFile("qunying.png"); //国家属性图标
           
            break;
        case 6://标志为晋
            spCountryIcon = CCSprite::spriteWithFile("jindai.png"); //国家属性图标
            break;
            
        default:
            break;
    }
    spCountryIcon->setAnchorPoint(CCPointMake(0.5, 0));
    spCountryIcon->setPosition(CCPointMake(52, 378));
    addChild(spCountryIcon,0);
    
    if (!m_bUseScrollView)
    {
        
        //title
        MainScene::Inst()->setTitle(pUsrCardInfo->cid, pUsrCardInfo->star,pUsrCardInfo->ugp_max_star, pUsrCardInfo->name, pUsrCardInfo->category, pUsrCardInfo->sub_category);
    }
    
    
    //获得觉醒技能的加成数值
    
    int iAwakeHp = 0;
    int iAwakeRec = 0;
    int iAwakeAtk = 0;
    
    int         m_iAwakeSkillArr[MaxAwakeSkillCnt];
    
    for (int i=0; i<MaxAwakeSkillCnt; i++)
        m_iAwakeSkillArr[i] = enAwakeSkill_NULL;
    
    int m_iAwakeNum = pUsrCardInfo->i_AwakeNum;
    
  
    
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        m_iAwakeSkillArr[i] = pUsrCardInfo->AwakeSkillArr[i];
        
        stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(m_iAwakeSkillArr[i]);
        if (!pAwakeSkill)continue;
        if (i < m_iAwakeNum)
        {
            switch (pAwakeSkill->iType)
            {
                case enAwakeSkillTp_valChg_hp:
                    iAwakeHp += (int)pAwakeSkill->fEffect;
                    break;
                    
                case enAwakeSkillTp_valChg_atk:
                    iAwakeAtk += (int)pAwakeSkill->fEffect;
                    break;
                    
                case enAwakeSkillTp_valChg_rcv:
                    iAwakeRec += (int)pAwakeSkill->fEffect;
                    break;
                    
                default:
                    break;
            }
        }
    }
    

   
    CCardBaseInfo *card_base = CGameData::Inst()->getCardBaseInfByCid(pUsrCardInfo->cid);
    
    //绘制觉醒技能小图标
    int iy = 716;
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        if (card_base->AwakeSkillArr[i] <= 0)
        {
            break;
        }
        
        snprintf(buf, 99, "awakeskill_%d.png", card_base->AwakeSkillArr[i]);
        if (CCSprite* spicon = CCSprite::spriteWithFile(buf))
        {
//            spicon->setPosition(ccp(592, iy));
            
            spicon->setPosition(ccp(617, iy));
            
            iy -= 40;
            addChild(spicon);
            
            if (pUsrCardInfo->i_AwakeNum <= i)
            {
                spicon->setOpacity(120);
            }
        }
        else
        {
            if((iAwakeHp > 0 || iAwakeRec > 0 || iAwakeAtk > 0) && card_base->AwakeSkillArr[i] <= 3)
            {
                if((pUsrCardInfo->pl_hp  > 0 || pUsrCardInfo->pl_recover > 0 || pUsrCardInfo->pl_attack > 0))
                {

                    
                    CCActionInterval* action4 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 0.5 + 2.0 + 0.5),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),

                                                                                       CCDelayTime::actionWithDuration(0.5 + 0.9),

                                                                                       
                                                                                       NULL);
                    
                    spicon->runAction(CCRepeatForever::actionWithAction(action4));
                }
                else
                {
                    CCActionInterval* action4 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                                       CCDelayTime::actionWithDuration(0.3),
                                                                                       CCFadeIn::actionWithDuration(0.1),
                                                                                       
                                                                                       CCDelayTime::actionWithDuration(0.5 + 0.9),

                                                                                       
                                                                                       NULL);
                    
                    spicon->runAction(CCRepeatForever::actionWithAction(action4));
                }
                
            }

        }
    }
    
    
    
//    snprintf(buf, 99, "%ld", pUsrCardInfo->hp);
    snprintf(buf, 99, "%ld", pUsrCardInfo->hp.get() + iAwakeHp);
    TextNode* tnHp = TextNode::textWithString(buf, CCSizeMake(172, 30), CCTextAlignmentRight, 24);
    tnHp->setPosition(CCPointMake(155, 302));
    tnHp->setAnchorPoint(CCPointZero);
    tnHp->setColor(ccc3(78, 24, 0));
    tnHp->setShadowColor(ccGRAY);
    addChild(tnHp, 1);
    
    //    snprintf(buf, 99, "%lu", pUsrCardInfo->attack);
    snprintf(buf, 99, "%lu", pUsrCardInfo->attack.get() + iAwakeAtk);
    TextNode* textAt = TextNode::textWithString(buf, CCSizeMake(172, 30), CCTextAlignmentRight, 24);
    textAt->setPosition(CCPointMake(155, 265));
    textAt->setAnchorPoint(CCPointZero);
    textAt->setColor(ccc3(78, 24, 0));
    textAt->setShadowColor(ccGRAY);
    addChild(textAt, 1);
    
    snprintf(buf, 99, "%ld", pUsrCardInfo->recover + iAwakeRec);
    TextNode* textRec = TextNode::textWithString(buf, CCSizeMake(172, 30), CCTextAlignmentRight, 24);
    textRec->setPosition(CCPointMake(155, 228));
    textRec->setAnchorPoint(CCPointZero);
    textRec->setColor(ccc3(78, 24, 0));
    textRec->setShadowColor(ccGRAY);
    addChild(textRec, 1);
    
    
    


    if((iAwakeHp > 0 || iAwakeRec > 0 || iAwakeAtk > 0) && (pUsrCardInfo->pl_hp  > 0 || pUsrCardInfo->pl_recover > 0 || pUsrCardInfo->pl_attack > 0))
    {
        if(pUsrCardInfo->pl_hp  > 0 && iAwakeHp > 0)
        {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_hp);
            
            TextNode* txPlusHp = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
            txPlusHp->setPosition(CCPointMake(155 + 30, 302));
            txPlusHp->setAnchorPoint(CCPointZero);
            txPlusHp->setColor(ccc3(255, 72, 0));
            txPlusHp->setShadowColor(ccGRAY);
            addChild(txPlusHp);
            
            CCSprite* spPlusHp = CCSprite::spriteWithSpriteFrameName("plus_10.png");
            spPlusHp->setPosition(CCPointMake(154 + 160, 317));
            addChild(spPlusHp);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(6.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.5 ),
                                                                               NULL);
            
            CCActionInterval* action4 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 0.5 + 2.0 + 0.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            txPlusHp->setOpacity(0);
            spPlusHp->setOpacity(0);
            tnHp->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusHp->runAction(CCRepeatForever::actionWithAction(action3));
            
            
            
            
            snprintf(buf, 99, "+%d", iAwakeHp);
            
            TextNode* txAwakeHp = TextNode::textWithString(buf,
                                                                CCSizeMake(100, 30),
                                                                CCTextAlignmentRight,
                                                                24);
            txAwakeHp->setPosition(CCPointMake(155 + 30 + 30, 302));
            txAwakeHp->setAnchorPoint(CCPointZero);
            txAwakeHp->setColor(ccc3(20, 125, 6));
            txAwakeHp->setShadowColor(ccGRAY);
            addChild(txAwakeHp);
            
            
            
            
            txAwakeHp->setOpacity(0);
            
            txAwakeHp->runActionInside(CCRepeatForever::actionWithAction(action4));

        }
        else if (iAwakeHp > 0) {
            snprintf(buf, 99, "+%d", iAwakeHp);
            
            TextNode* txPlusHp = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
            txPlusHp->setPosition(CCPointMake(155 + 30 + 30, 302));
            txPlusHp->setAnchorPoint(CCPointZero);
            txPlusHp->setColor(ccc3(20, 125, 6));
            txPlusHp->setShadowColor(ccGRAY);
            addChild(txPlusHp);
            
            
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusHp->setOpacity(0);
            
            tnHp->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
        else if (pUsrCardInfo->pl_hp > 0) {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_hp);
            
            TextNode* txPlusHp = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
            txPlusHp->setPosition(CCPointMake(155 + 30, 302));
            txPlusHp->setAnchorPoint(CCPointZero);
            txPlusHp->setColor(ccc3(255, 72, 0));
            txPlusHp->setShadowColor(ccGRAY);
            addChild(txPlusHp);
            
            CCSprite* spPlusHp = CCSprite::spriteWithSpriteFrameName("plus_10.png");
            spPlusHp->setPosition(CCPointMake(154 + 160, 317));
            addChild(spPlusHp);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusHp->setOpacity(0);
            spPlusHp->setOpacity(0);
            tnHp->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusHp->runAction(CCRepeatForever::actionWithAction(action3));
        }
        
        
        
        if(pUsrCardInfo->pl_attack  > 0 && iAwakeAtk > 0)
        {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_attack);
            
            TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                              CCSizeMake(100, 30),
                                                              CCTextAlignmentRight,
                                                              24);
            txPlusAttack->setPosition(CCPointMake(155 + 30, 265));
            txPlusAttack->setAnchorPoint(CCPointZero);
            txPlusAttack->setColor(ccc3(255, 72, 0));
            txPlusAttack->setShadowColor(ccGRAY);
            addChild(txPlusAttack);
            
            CCSprite* spPlusAttack = CCSprite::spriteWithSpriteFrameName("plus_5.png");
            spPlusAttack->setPosition(CCPointMake(154 + 160, 280));
            addChild(spPlusAttack);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(6.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.5 ),
                                                                               NULL);
            
            CCActionInterval* action4 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 0.5 + 2.0 + 0.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);

            
            txPlusAttack->setOpacity(0);
            spPlusAttack->setOpacity(0);
            textAt->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusAttack->runAction(CCRepeatForever::actionWithAction(action3));
            
            
            snprintf(buf, 99, "+%d", iAwakeAtk);
            
            TextNode* txAwakeAtk = TextNode::textWithString(buf,
                                                           CCSizeMake(100, 30),
                                                           CCTextAlignmentRight,
                                                           24);
            txAwakeAtk->setPosition(CCPointMake(155 + 30 + 30, 265));
            txAwakeAtk->setAnchorPoint(CCPointZero);
            txAwakeAtk->setColor(ccc3(20, 125, 6));
            txAwakeAtk->setShadowColor(ccGRAY);
            addChild(txAwakeAtk);
            
            
            
            
            txAwakeAtk->setOpacity(0);
            
            txAwakeAtk->runActionInside(CCRepeatForever::actionWithAction(action4));
        }
        else if (iAwakeAtk > 0) {
            snprintf(buf, 99, "+%d", iAwakeAtk);
            
            TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                              CCSizeMake(100, 30),
                                                              CCTextAlignmentRight,
                                                              24);
            txPlusAttack->setPosition(CCPointMake(155 + 30 + 30, 265));
            txPlusAttack->setAnchorPoint(CCPointZero);
            txPlusAttack->setColor(ccc3(20, 125, 6));
            txPlusAttack->setShadowColor(ccGRAY);
            addChild(txPlusAttack);
            
            
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusAttack->setOpacity(0);
            
            textAt->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
        else if (pUsrCardInfo->pl_attack > 0) {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_attack);
            
            TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                              CCSizeMake(100, 30),
                                                              CCTextAlignmentRight,
                                                              24);
            txPlusAttack->setPosition(CCPointMake(155 + 30, 265));
            txPlusAttack->setAnchorPoint(CCPointZero);
            txPlusAttack->setColor(ccc3(255, 72, 0));
            txPlusAttack->setShadowColor(ccGRAY);
            addChild(txPlusAttack);
            
            CCSprite* spPlusAttack = CCSprite::spriteWithSpriteFrameName("plus_5.png");
            spPlusAttack->setPosition(CCPointMake(154 + 160, 280));
            addChild(spPlusAttack);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusAttack->setOpacity(0);
            spPlusAttack->setOpacity(0);
            textAt->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusAttack->runAction(CCRepeatForever::actionWithAction(action3));
        }

        if(pUsrCardInfo->pl_recover > 0 && iAwakeRec > 0)
        {
            
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_recover);
            
            TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                               CCSizeMake(100, 30),
                                                               CCTextAlignmentRight,
                                                               24);
            txPlusRecover->setPosition(CCPointMake(155 + 30, 228));
            txPlusRecover->setAnchorPoint(CCPointZero);
            txPlusRecover->setColor(ccc3(255, 72, 0));
            txPlusRecover->setShadowColor(ccGRAY);
            addChild(txPlusRecover);
            
            CCSprite* spPlusRecover = CCSprite::spriteWithSpriteFrameName("plus_3.png");
            spPlusRecover->setPosition(CCPointMake(154 + 160, 243));
            addChild(spPlusRecover);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(6.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.5 ),
                                                                               NULL);
            
            CCActionInterval* action4 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 0.5 + 2.0 + 0.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               
                                                                               NULL);
            
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusRecover->setOpacity(0);
            spPlusRecover->setOpacity(0);
            textRec->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusRecover->runAction(CCRepeatForever::actionWithAction(action3));
            
            
            
            
            snprintf(buf, 99, "+%d", iAwakeRec);
            
            TextNode* txAwakeRecover = TextNode::textWithString(buf,
                                                                CCSizeMake(100, 30),
                                                                CCTextAlignmentRight,
                                                                24);
            txAwakeRecover->setPosition(CCPointMake(155 + 30 + 30, 228));
            txAwakeRecover->setAnchorPoint(CCPointZero);
            txAwakeRecover->setColor(ccc3(20, 125, 6));
            txAwakeRecover->setShadowColor(ccGRAY);
            addChild(txAwakeRecover);
            
            
            
            
            txAwakeRecover->setOpacity(0);
            
            txAwakeRecover->runActionInside(CCRepeatForever::actionWithAction(action4));
            
            
            
            
        }
        else if (iAwakeRec > 0) {
            snprintf(buf, 99, "+%d", iAwakeRec);
            
            TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                               CCSizeMake(100, 30),
                                                               CCTextAlignmentRight,
                                                               24);
            txPlusRecover->setPosition(CCPointMake(155 + 30 + 30, 228));
            txPlusRecover->setAnchorPoint(CCPointZero);
            txPlusRecover->setColor(ccc3(20, 125, 6));
            txPlusRecover->setShadowColor(ccGRAY);
            addChild(txPlusRecover);
            
            
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusRecover->setOpacity(0);
            
            textRec->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
        else if(pUsrCardInfo->pl_recover > 0)
        {
            
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_recover);
            
            TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                               CCSizeMake(100, 30),
                                                               CCTextAlignmentRight,
                                                               24);
            txPlusRecover->setPosition(CCPointMake(155 + 30, 228));
            txPlusRecover->setAnchorPoint(CCPointZero);
            txPlusRecover->setColor(ccc3(255, 72, 0));
            txPlusRecover->setShadowColor(ccGRAY);
            addChild(txPlusRecover);
            
            CCSprite* spPlusRecover = CCSprite::spriteWithSpriteFrameName("plus_3.png");
            spPlusRecover->setPosition(CCPointMake(154 + 160, 243));
            addChild(spPlusRecover);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0 + 3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0 + 3.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5 ),
                                                                               NULL);
            
            
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusRecover->setOpacity(0);
            spPlusRecover->setOpacity(0);
            textRec->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusRecover->runAction(CCRepeatForever::actionWithAction(action3));
            
            
            
        }

    }
    else if((iAwakeHp > 0 || iAwakeRec > 0 || iAwakeAtk > 0))
    {
        if (iAwakeHp > 0) {
            snprintf(buf, 99, "+%d", iAwakeHp);
            
            TextNode* txPlusHp = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
            txPlusHp->setPosition(CCPointMake(155 + 30 + 30, 302));
            txPlusHp->setAnchorPoint(CCPointZero);
            txPlusHp->setColor(ccc3(20, 125, 6));
            txPlusHp->setShadowColor(ccGRAY);
            addChild(txPlusHp);
            

            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusHp->setOpacity(0);
            
            tnHp->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
        
        
        
        if (iAwakeAtk > 0) {
            snprintf(buf, 99, "+%d", iAwakeAtk);
            
            TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                              CCSizeMake(100, 30),
                                                              CCTextAlignmentRight,
                                                              24);
            txPlusAttack->setPosition(CCPointMake(155 + 30 + 30, 265));
            txPlusAttack->setAnchorPoint(CCPointZero);
            txPlusAttack->setColor(ccc3(20, 125, 6));
            txPlusAttack->setShadowColor(ccGRAY);
            addChild(txPlusAttack);
            

            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusAttack->setOpacity(0);
            
            textAt->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
        
        
        
        if (iAwakeRec > 0) {
            snprintf(buf, 99, "+%d", iAwakeRec);
            
            TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                               CCSizeMake(100, 30),
                                                               CCTextAlignmentRight,
                                                               24);
            txPlusRecover->setPosition(CCPointMake(155 + 30 + 30, 228));
            txPlusRecover->setAnchorPoint(CCPointZero);
            txPlusRecover->setColor(ccc3(20, 125, 6));
            txPlusRecover->setShadowColor(ccGRAY);
            addChild(txPlusRecover);
            

            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            
            
            txPlusRecover->setOpacity(0);
            
            textRec->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
            
        }
    }
    else if((pUsrCardInfo->pl_hp  > 0 || pUsrCardInfo->pl_recover > 0 || pUsrCardInfo->pl_attack > 0))
    {
        
        
        if (pUsrCardInfo->pl_hp > 0) {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_hp);
            
            TextNode* txPlusHp = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
            txPlusHp->setPosition(CCPointMake(155 + 30, 302));
            txPlusHp->setAnchorPoint(CCPointZero);
            txPlusHp->setColor(ccc3(255, 72, 0));
            txPlusHp->setShadowColor(ccGRAY);
            addChild(txPlusHp);
            
            CCSprite* spPlusHp = CCSprite::spriteWithSpriteFrameName("plus_10.png");
            spPlusHp->setPosition(CCPointMake(154 + 160, 317));
            addChild(spPlusHp);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusHp->setOpacity(0);
            spPlusHp->setOpacity(0);
            tnHp->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusHp->runAction(CCRepeatForever::actionWithAction(action3));
        }
        
        
        
        if (pUsrCardInfo->pl_attack > 0) {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_attack);
            
            TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                              CCSizeMake(100, 30),
                                                              CCTextAlignmentRight,
                                                              24);
            txPlusAttack->setPosition(CCPointMake(155 + 30, 265));
            txPlusAttack->setAnchorPoint(CCPointZero);
            txPlusAttack->setColor(ccc3(255, 72, 0));
            txPlusAttack->setShadowColor(ccGRAY);
            addChild(txPlusAttack);
            
            CCSprite* spPlusAttack = CCSprite::spriteWithSpriteFrameName("plus_5.png");
            spPlusAttack->setPosition(CCPointMake(154 + 160, 280));
            addChild(spPlusAttack);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusAttack->setOpacity(0);
            spPlusAttack->setOpacity(0);
            textAt->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusAttack->runAction(CCRepeatForever::actionWithAction(action3));
        }
        
        
        
        if (pUsrCardInfo->pl_recover > 0) {
            snprintf(buf, 99, "+%d", pUsrCardInfo->pl_recover);
            
            TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                               CCSizeMake(100, 30),
                                                               CCTextAlignmentRight,
                                                               24);
            txPlusRecover->setPosition(CCPointMake(155 + 30, 228));
            txPlusRecover->setAnchorPoint(CCPointZero);
            txPlusRecover->setColor(ccc3(255, 72, 0));
            txPlusRecover->setShadowColor(ccGRAY);
            addChild(txPlusRecover);
            
            CCSprite* spPlusRecover = CCSprite::spriteWithSpriteFrameName("plus_3.png");
            spPlusRecover->setPosition(CCPointMake(154 + 160, 243));
            addChild(spPlusRecover);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(3.0),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(2.0),
                                                                               CCFadeOut::actionWithDuration(0.5),
                                                                               CCDelayTime::actionWithDuration(0.5),
                                                                               NULL);
            CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
            
            txPlusRecover->setOpacity(0);
            spPlusRecover->setOpacity(0);
            textRec->runActionInside(CCRepeatForever::actionWithAction(action1));
            txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
            spPlusRecover->runAction(CCRepeatForever::actionWithAction(action3));
        }
        
    }



    //lv
    if(pUsrCardInfo->lv == pUsrCardInfo->max_lv){
        snprintf(buf, 99, "Lv:%s", CGameData::Inst()->getLanguageValue("max"));
    }
    else{
        snprintf(buf, 99, "Lv:%d", pUsrCardInfo->lv);
    }
    text = TextNode::textWithString(buf, CCSizeMake(120, 30), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(350, 302));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccc3(78, 24, 0));
    text->setShadowColor(ccGRAY);
    addChild(text, 1);

    //max lv
    snprintf(buf, 99, "%sLv:%hu", CGameData::Inst()->getLanguageValue("max"), pUsrCardInfo->max_lv);
    text = TextNode::textWithString(buf, CCSizeMake(250, 30), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(350, 265));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccc3(78, 24, 0));
    text->setShadowColor(ccGRAY);
    addChild(text, 1);
    
    //exp
    if(bSelf == true && pUsrCardInfo->lv != pUsrCardInfo->max_lv){
        unsigned long ulCurExpBase = CGameData::Inst()->getExpByType(pUsrCardInfo->exp_type, pUsrCardInfo->lv);
        unsigned long ulNextLvExp = CGameData::Inst()->getExpByType(pUsrCardInfo->exp_type, pUsrCardInfo->lv+1);
        unsigned long ulNeedExp = ulNextLvExp - pUsrCardInfo->exp;

        float per = (pUsrCardInfo->exp-ulCurExpBase)*1.0/(ulNextLvExp-ulCurExpBase);
         
        sprintf(buf, "%s:%ld", CGameData::Inst()->getLanguageValue("exp_need"), ulNeedExp);

        text = TextNode::textWithString(buf, CCSizeMake(300, 30), CCTextAlignmentLeft, 24);
        text->setPosition(CCPointMake(350, 228));
        text->setAnchorPoint(CCPointZero);
        text->setColor(ccc3(78, 24, 0));
        text->setShadowColor(ccGRAY);
        addChild(text, 1);
    
        CCSprite *totalExp = CCSprite::spriteWithFile("pa_information_bar.png");
        totalExp->setPosition(CCPointMake(340, 223));
        totalExp->setAnchorPoint(ccp(0,0.5));
        totalExp->setScaleX(1.0);
        addChild(totalExp);
        
        CCSprite *myExp = CCSprite::spriteWithFile("pa_information_bar2.png");
        myExp->setPosition(CCPointMake(350, 223));
        myExp->setScaleX(per);
        myExp->setAnchorPoint(ccp(0, 0.5));
        addChild(myExp);
    }
//    //cost
    snprintf(buf, 99, "%hu", pUsrCardInfo->cost);
    if (pUsrCardInfo->cost > 99) {
        text = TextNode::textWithString(buf, CCSizeMake(120, 24), CCTextAlignmentLeft, 18);
        text->setPosition(CCPointMake(545, 306));
    }else{
        text = TextNode::textWithString(buf, CCSizeMake(120, 24), CCTextAlignmentLeft, 24);
        text->setPosition(CCPointMake(552, 306));
    }
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccWHITE);
    addChild(text, 1);

    //skill description
    int skid = pUsrCardInfo->skid;
    int sklv = pUsrCardInfo->sk_lv;
    int ldskid = pUsrCardInfo->leader_skid;
    int turn = 0;

    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(skid);
    
    if(pManSkill != NULL){
        turn = pManSkill->cTurn - (sklv -1);
    }

    const char* cstr;
    if(skid == 0)
        cstr = CGameData::Inst()->getLanguageValue("nothing");
    else
        cstr = CSkillLib::Ins()->getManSkillName(skid);
    
    text = TextNode::textWithString(cstr, CCSizeMake(300, 30), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(128, 182));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccc3(64,170,252));
    addChild(text, 1);

    if(skid > 0){  
        
        if(sklv == pManSkill->cMaxLv){
            snprintf(buf, 99, "Lv:%s %s:%d", CGameData::Inst()->getLanguageValue("max"), CGameData::Inst()->getLanguageValue("round"), turn);
        }
        else{
            snprintf(buf, 99, "Lv:%d %s:%d", sklv, CGameData::Inst()->getLanguageValue("round"), turn);
        }
       // snprintf(buf, 99, "Lv.%d 回合:%d", sklv, turn);
        text = TextNode::textWithString(buf, CCSizeMake(200, 25), CCTextAlignmentRight, 24);
        text->setPosition(CCPointMake(572, 182));
        text->setAnchorPoint(CCPointMake(1, 0));
        addChild(text, 1);
        
//        text = TextNode::textWithString(CSkillLib::Ins()->getManSkillDsp(skid), CCSizeMake(500, 50), CCTextAlignmentLeft, 24);
        text = TextNode::textWithString(CSkillLib::Ins()->getManSkillDsp(skid), CCSizeMake(500, 0), CCTextAlignmentLeft, 24);
        text->setPosition(CCPointMake(80, 177));
        text->setAnchorPoint(ccp(0, 1));
        text->setColor(ccc3(78, 24, 0));
        text->setShadowColor(ccGRAY);
        text->moveInLimitedHeight(28,2);
        addChild(text, 1);
    }

    if(ldskid == 0)
        cstr = CGameData::Inst()->getLanguageValue("nothing");
    else
        cstr = CSkillLib::Ins()->getAutoSkillName(ldskid);
    text = TextNode::textWithString(cstr, CCSizeMake(200, 25), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(153, 88));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccGREEN);
    addChild(text, 1);
    
//    text = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(ldskid), CCSizeMake(500, 50), CCTextAlignmentLeft, 24);
    text = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(ldskid), CCSizeMake(500, 0), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(80, 83));
    text->setAnchorPoint(ccp(0, 1));
    text->setColor(ccc3(78, 24, 0));
    text->setShadowColor(ccGRAY);
    text->moveInLimitedHeight(28,2);
    addChild(text, 1);

    if (!m_bUseScrollView)
    {
        //action
        CCLayerColor* lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 640, 960);
        addChild(lcCover, 10);
        //lcCover->runAction(CCFadeOut::actionWithDuration(0.3));
        lcCover->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.3),
                                               CCDelayTime::actionWithDuration(0.3),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(CShowCardInf::ccCanBeTouched)),
                                               NULL));
    }
    else
    {
        m_bCanBeTouched = true;
    }
    
    MainScene::Inst()->setGenRangeBtnIsVisble(false);
    MainScene::Inst()->showBottomPanel(false);

}

void CShowCardInf::show(CCardBaseInfo* pUsrCardInfo, int plHp, int plAttack, int plRecover)
{
    if(pUsrCardInfo == NULL)
        return;
    m_bisDropImage = false;
    removeAllChildrenWithCleanup(true);
    this->setIsVisible(true);
    
    TextNode* text;
    if (!m_bUseScrollView)
    {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,kCCMenuTouchPriority-1, true);
        //设置背景
        CCSprite* spBg = CCSprite::spriteWithFile("show_bg.jpg");
        spBg->setScale(1/0.8);
        spBg->setPosition(CCPointMake(320, 480));
        addChild(spBg, -10);
        
        //设置火炬
        showTorchParticle();
    }

    //info box
    CCSprite* spInf = CCSprite::spriteWithFile("pa_information.png");
    spInf->setPosition(CCPointMake(320, 198));
    addChild(spInf, -9,TAB_SPRITE_INFO_BOX);
    
    if (!m_bUseScrollView)
    {
        //返回button
        CCSprite *backBtn = CCSprite::spriteWithSpriteFrameName("id_back.png");
        backBtn->setPosition(ccp(54,766));
        addChild(backBtn);
        
        // source button  来源 详情
        m_source = (TitleBar*) CCSprite::spriteWithSpriteFrameName("id_Reset.png");
        m_source -> setPosition(CCPointMake(580, 766));
        m_source -> setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
        m_source -> SetTitlePosition(CCPointMake(12, 15));
        m_source -> setTag(kLaiYuan);
        addChild(m_source);
    }
    
    if (!pUsrCardInfo->find_me.empty())
    {
        // source figure
        CCSprite* sp_source = CCSprite::spriteWithFile("pa_information1.png");
        addChild(sp_source, 10, kSource);
        sp_source -> setPosition(CCPointMake(320, 124));
        sp_source -> setScale(0);
        
        const char* ch0 = pUsrCardInfo->find_me.c_str();
        TextNode* text0 = TextNode::textWithString(ch0, CCSizeMake(520, 110), CCTextAlignmentCenter, 24);
        text0 -> setPosition(CCPointMake(294, 82));
        text0->setShadowColor(ccBLACK);
        sp_source -> addChild(text0);
    }
    
    if (m_source && pUsrCardInfo->find_me.empty())
    {
        m_source -> setScale(0);
    }
    
    char buf[100];
    //card sprite
    CCSprite* spCard = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
    if (spCard) {
//        spCard->setPosition(CCPointMake(98 - 5, 280));
        spCard->setAnchorPoint(CCPointMake(0.0f, 1.0f));
        spCard->setPosition(CCPointMake(40, 334));
        addChild(spCard);
        
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
    }

    //body sprite
    CCSprite* spBody = CGameData::Inst()->getBodySprite(pUsrCardInfo->cid);
    spBody->setAnchorPoint(CCPointMake(0.5, 0));
    spBody->setPosition(CCPointMake(320, 400));
    addChild(spBody,0,TAB_SPRITE_BODY);
    
    //添加该武将的国家属性
    CCSprite* spCountryIcon =NULL;
    switch (pUsrCardInfo->cou_name) {
        case 0://标志为无
            spCountryIcon = CCSprite::spriteWithFile("wu.png"); //国家属性图标
            break;
        case 1://标志为汉
            spCountryIcon = CCSprite::spriteWithFile("hanchao.png"); //国家属性图标
            
            break;
        case 2://标志为魏
            spCountryIcon = CCSprite::spriteWithFile("weiguo.png"); //国家属性图标
            
            break;
        case 3://标志为蜀
            spCountryIcon = CCSprite::spriteWithFile("shuguo.png"); //国家属性图标
            
            break;
        case 4://标志为吴
            spCountryIcon = CCSprite::spriteWithFile("wuguo.png"); //国家属性图标
            
            break;
        case 5://标志为群
            spCountryIcon = CCSprite::spriteWithFile("qunying.png"); //国家属性图标
            
            break;
        case 6://标志为晋
            spCountryIcon = CCSprite::spriteWithFile("jindai.png"); //国家属性图标
            break;
            
        default:
            break;
    }
    spCountryIcon->setAnchorPoint(CCPointMake(0.5, 0));
    spCountryIcon->setPosition(CCPointMake(52, 378));
    addChild(spCountryIcon,0);
    
    if (!m_bUseScrollView)
    {
        //title
        MainScene::Inst()->setTitle(pUsrCardInfo->cid, pUsrCardInfo->star,pUsrCardInfo->ugp_max_star, pUsrCardInfo->name, pUsrCardInfo->category, pUsrCardInfo->sub_category);
    }
    
    snprintf(buf, 99, "%ld", pUsrCardInfo->hp + plHp * HPPLUSGROWTH);
    text = TextNode::textWithString(buf, CCSizeMake(168, 30), CCTextAlignmentRight, 24);
    text->setPosition(CCPointMake(155, 302));
    text->setAnchorPoint(CCPointZero);
    text->setColor(kShowInfoColor);
    text->setShadowColor(ccGRAY);
    addChild(text, 1);
    
    if (plHp > 0) {
        snprintf(buf, 99, "+%d", plHp);
        
        TextNode* txPlusHp = TextNode::textWithString(buf,
                                                      CCSizeMake(100, 30),
                                                      CCTextAlignmentRight,
                                                      24);
        txPlusHp->setPosition(CCPointMake(155 + 30, 302));
        txPlusHp->setAnchorPoint(CCPointZero);
        txPlusHp->setColor(ccc3(255, 72, 0));
        txPlusHp->setShadowColor(ccGRAY);
        addChild(txPlusHp);
        
        CCSprite* spPlusHp = CCSprite::spriteWithSpriteFrameName("plus_10.png");
        spPlusHp->setPosition(CCPointMake(154 + 160, 317));
        addChild(spPlusHp);
        
        CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(3.0),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
        
        txPlusHp->setOpacity(0);
        spPlusHp->setOpacity(0);
        text->runActionInside(CCRepeatForever::actionWithAction(action1));
        txPlusHp->runActionInside(CCRepeatForever::actionWithAction(action2));
        spPlusHp->runAction(CCRepeatForever::actionWithAction(action3));
    }

    //攻击
    snprintf(buf, 99, "%lu", pUsrCardInfo->attack + plAttack * ATKPLUSGROWTH);
    text = TextNode::textWithString(buf, CCSizeMake(168, 30), CCTextAlignmentRight, 24);
    text->setPosition(CCPointMake(155, 265));
    text->setAnchorPoint(CCPointZero);
    text->setColor(kShowInfoColor);
    text->setShadowColor(ccGRAY);
    addChild(text, 1);
    
    if (plAttack > 0) {
        snprintf(buf, 99, "+%d", plAttack);
        
        TextNode* txPlusAttack = TextNode::textWithString(buf,
                                                          CCSizeMake(100, 30),
                                                          CCTextAlignmentRight,
                                                          24);
        txPlusAttack->setPosition(CCPointMake(155 + 30, 265));
        txPlusAttack->setAnchorPoint(CCPointZero);
        txPlusAttack->setColor(ccc3(255, 72, 0));
        txPlusAttack->setShadowColor(ccGRAY);
        addChild(txPlusAttack);
        
        CCSprite* spPlusAttack = CCSprite::spriteWithSpriteFrameName("plus_5.png");
        spPlusAttack->setPosition(CCPointMake(154 + 160, 280));
        addChild(spPlusAttack);
        
        CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(3.0),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
        
        txPlusAttack->setOpacity(0);
        spPlusAttack->setOpacity(0);
        text->runActionInside(CCRepeatForever::actionWithAction(action1));
        txPlusAttack->runActionInside(CCRepeatForever::actionWithAction(action2));
        spPlusAttack->runAction(CCRepeatForever::actionWithAction(action3));
    }

    //回复
    snprintf(buf, 99, "%ld", pUsrCardInfo->recover + plRecover * RECPLUSGROWTH);
    text = TextNode::textWithString(buf, CCSizeMake(168, 30), CCTextAlignmentRight, 24);
    text->setPosition(CCPointMake(155, 228));
    text->setAnchorPoint(CCPointZero);
    text->setColor(kShowInfoColor);
    text->setShadowColor(ccGRAY);
    addChild(text, 1);
    
    if (plRecover > 0) {
        snprintf(buf, 99, "+%d", plRecover);
        
        TextNode* txPlusRecover = TextNode::textWithString(buf,
                                                           CCSizeMake(100, 30),
                                                           CCTextAlignmentRight,
                                                           24);
        txPlusRecover->setPosition(CCPointMake(155 + 30, 228));
        txPlusRecover->setAnchorPoint(CCPointZero);
        txPlusRecover->setColor(ccc3(255, 72, 0));
        txPlusRecover->setShadowColor(ccGRAY);
        addChild(txPlusRecover);
        
        CCSprite* spPlusRecover = CCSprite::spriteWithSpriteFrameName("plus_3.png");
        spPlusRecover->setPosition(CCPointMake(154 + 160, 243));
        addChild(spPlusRecover);
        
        CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(3.0),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                                                           CCFadeIn::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(2.0),
                                                                           CCFadeOut::actionWithDuration(0.5),
                                                                           CCDelayTime::actionWithDuration(0.5),
                                                                           NULL);
        CCActionInterval* action3 = (CCActionInterval*)action2->copyWithZone(NULL);
        
        txPlusRecover->setOpacity(0);
        spPlusRecover->setOpacity(0);
        text->runActionInside(CCRepeatForever::actionWithAction(action1));
        txPlusRecover->runActionInside(CCRepeatForever::actionWithAction(action2));
        spPlusRecover->runAction(CCRepeatForever::actionWithAction(action3));
    }

    //max lv
    snprintf(buf, 99, "%sLv:%hu", CGameData::Inst()->getLanguageValue("max"), pUsrCardInfo->max_lv);
    text = TextNode::textWithString(buf, CCSizeMake(250, 30), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(350, 302));
    text->setAnchorPoint(CCPointZero);
    text->setColor(kShowInfoColor);
    text->setShadowColor(ccGRAY);
    addChild(text, 1);

    //cost
    snprintf(buf, 99, "%hu", pUsrCardInfo->cost);
    if (pUsrCardInfo->cost > 99) {
        text = TextNode::textWithString(buf, CCSizeMake(120, 24), CCTextAlignmentLeft, 18);
        text->setPosition(CCPointMake(545, 306));
    }else{
        text = TextNode::textWithString(buf, CCSizeMake(120, 24), CCTextAlignmentLeft, 24);
        text->setPosition(CCPointMake(552, 306));
    }
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccWHITE);
    addChild(text, 1);

    //skill description
    int skid = pUsrCardInfo->skid;
    int sklv = 1;
    int ldskid = pUsrCardInfo->leader_skid;
    int turn = 0;
    
    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(skid);
    if(pManSkill != NULL){
        turn = pManSkill->cTurn - (sklv -1);
    }

    const char* cstr;
    if(skid == 0)
        cstr = CGameData::Inst()->getLanguageValue("nothing");
    else
        cstr = CSkillLib::Ins()->getManSkillName(skid);

    text = TextNode::textWithString(cstr, CCSizeMake(300, 30), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(128, 182));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccc3(64,170,252));
    addChild(text, 1);
    
    if(skid > 0){
        snprintf(buf, 99, "Lv:1 %s:%d", CGameData::Inst()->getLanguageValue("round"), turn);

        text = TextNode::textWithString(buf, CCSizeMake(200, 25), CCTextAlignmentRight, 24);
        text->setPosition(CCPointMake(572, 182));
        text->setAnchorPoint(CCPointMake(1, 0));
        text->setColor(ccWHITE);
        addChild(text, 1);

        text = TextNode::textWithString(CSkillLib::Ins()->getManSkillDsp(skid), CCSizeMake(500, 0), CCTextAlignmentLeft, 24);
        text->setPosition(CCPointMake(80, 177));
        text->setAnchorPoint(ccp(0, 1));
        text->setColor(ccc3(78, 24, 0));
        text->setShadowColor(ccGRAY);
        text->moveInLimitedHeight(28,2);
        addChild(text, 1);
    }
    

    if(ldskid == 0)
        cstr = CGameData::Inst()->getLanguageValue("nothing");
    else
        cstr = CSkillLib::Ins()->getAutoSkillName(ldskid);
    text = TextNode::textWithString(cstr, CCSizeMake(200, 25), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(153, 88));
    text->setAnchorPoint(CCPointZero);
    text->setColor(ccGREEN);
    addChild(text, 1);

    text = TextNode::textWithString(CSkillLib::Ins()->getAutoSkillDsp(ldskid), CCSizeMake(500, 0), CCTextAlignmentLeft, 24);
    text->setPosition(CCPointMake(80, 83));
    text->setAnchorPoint(ccp(0, 1));
    text->setColor(kShowInfoColor);
    text->setShadowColor(ccGRAY);
    text->moveInLimitedHeight(28,2);
    addChild(text, 1);
    
    int iy = 716;
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        if (pUsrCardInfo->AwakeSkillArr[i] <= 0)
        {
            break;
        }
        snprintf(buf, 99, "awakeskill_%d.png", pUsrCardInfo->AwakeSkillArr[i]);
        if (CCSprite* spicon = CCSprite::spriteWithFile(buf))
        {
            spicon->setPosition(ccp(592, iy));
            iy -= 40;
            addChild(spicon);
        }
    }

    
    if (!m_bUseScrollView)
    {
        //action
        CCLayerColor* lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 640, 960);
        addChild(lcCover, 10);
        //lcCover->runAction(CCFadeOut::actionWithDuration(0.3));
        MainScene::Inst()->showBottomPanel(false);
        lcCover->runAction(CCSequence::actions(CCFadeOut::actionWithDuration(0.3),
                                               CCDelayTime::actionWithDuration(0.3),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(CShowCardInf::ccCanBeTouched)),
                                               NULL));
    }
    else
    {
        m_bCanBeTouched = true;
    }
    
    MainScene::Inst()->setGenRangeBtnIsVisble(false);
    MainScene::Inst()->showBottomPanel(false);

}

void CShowCardInf::setUseScrollView(bool b)
{
    m_bUseScrollView = b;
}

CCNode * CShowCardInf::getChildBody()
{
    return getChildByTag(TAB_SPRITE_BODY);
}

CCNode * CShowCardInf::getChildInfoBox()
{
    return getChildByTag(TAB_SPRITE_INFO_BOX);
}

void CShowCardInf::loadBody(unsigned short ushCID)
{
    if (!getChildByTag(TAB_SPRITE_BODY))
    {
        CCSprite* spBody = CGameData::Inst()->getBodySprite(ushCID);
        spBody->setAnchorPoint(CCPointMake(0.5, 0));
        spBody->setPosition(CCPointMake(320, 400));
        addChild(spBody,0,TAB_SPRITE_BODY);
    }
}

void CShowCardInf::ccCanBeTouched()
{
    m_bCanBeTouched = true;
}

void CShowCardInf::drawAwakeSkillTip(CCPoint pos, int awakeSkillId, int idx)
{
    char buf[100];
    snprintf(buf, 99, "awakeskill_%d.png", awakeSkillId);
    
    char bufName[100];
    snprintf(bufName, 99, "AwakeSkill_Name_%d", awakeSkillId);
    
    char bufDsp[100];
    snprintf(bufDsp, 99, "AwakeSkill_Dsp_%d", awakeSkillId);
    
    char bufIdx[100];
    snprintf(bufIdx, 99, CGameData::Inst()->getLanguageValue("AwakeSkill_index"), idx);
    
    TipsAwakeSkill* tipAwakeSkill = TipsAwakeSkill::tipWithContent(buf, CGameData::Inst()->getLanguageValue(bufName), CGameData::Inst()->getLanguageValue(bufDsp), bufIdx);
    addChild(tipAwakeSkill, 100, 20131029);
    tipAwakeSkill->setPosition(pos);
    b_showAwakeInfo = true;
}

//******************************************************************************
// Touch
//******************************************************************************
bool CShowCardInf::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView(touch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    if (m_bUseScrollView)return true;
    
    CCPoint screenPos = touchLocation;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        screenPos = ccpMult(screenPos, 2);
    if(sz.width < 1000)
        screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    screenPos.x -= MainScene::Inst()->getPosition().x;
    screenPos.y -= MainScene::Inst()->getPosition().y;
    
    if (m_pUsrCardInfo != NULL)
    {
        if (CCardBaseInfo *card_base = CGameData::Inst()->getCardBaseInfByCid(m_pUsrCardInfo->cid))
        {
            for (int i=0; i<MaxAwakeSkillCnt; i++)
            {
                if (card_base != NULL && card_base->AwakeSkillArr[i] <= 0)
                {
                     break;
                }
                if (screenPos.x > 510 && screenPos.y < 736 && screenPos.y > 736 - ((i+1)*40))
                {
                    drawAwakeSkillTip(ccp(320, 650-(i*40)), card_base->AwakeSkillArr[i], i+1);
                    return true;
                }
            }
        }
    }

    
    
    if(m_bCanBeTouched){
        if (m_source && m_source->getScale() != 0)
        {
            CCSize  size_source  = m_source->getContentSize();
            CCPoint point_source = m_source->getPosition();
            CCRect  rect_source  = CCRectMake(point_source.x - size_source.width/2, point_source.y - size_source.height/2, size_source.width, size_source.height);
            CCPoint touch_location = CCDirector::sharedDirector()->convertToGL(touch->locationInView(touch->view()));
            touch_location = convertToNodeSpace(touch_location);

            bool b0 = CCRect::CCRectContainsPoint(rect_source, touch_location);
            if (b0)
            {
                ShowGeneralSource();
                return true;
            }
        }
        hide();
        if(m_pfnSelector&&m_pListener)
        {
            (m_pListener->*m_pfnSelector)();
            m_pfnSelector=NULL;
            m_pListener=NULL;
        }
    }
    return true;

}

void CShowCardInf::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (b_showAwakeInfo)
    {
        if (getChildByTag(20131029))
            removeChildByTag(20131029, true);
        
        b_showAwakeInfo = false;
    }
}

//******************************************************************************
//  ShowGeneralSource()
//******************************************************************************
void CShowCardInf::ShowGeneralSource()
{
    if (!m_source)return;
    if (m_source->getTag() == kLaiYuan)
    {
        m_source -> setTitleString(CGameData::Inst()->getLanguageValue("xiangqing_alone"));
        m_source -> SetTitlePosition(CCPointMake(12, 15));
        m_source -> setTag(kXiangQing);
        getChildByTag(kSource)->setScale(1);
    }else{
        m_source -> setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
        m_source -> SetTitlePosition(CCPointMake(12, 15));
        m_source -> setTag(kLaiYuan);
        getChildByTag(kSource)->setScale(0);
    }
    
}

//******************************************************************************
// hide
//******************************************************************************
void CShowCardInf::hide()
{
    if(!m_bForbShowRangeBtn)
        MainScene::Inst()->setGenRangeBtnIsVisble(true);
    m_bCanBeTouched = false;
    if (m_source)
    {
//        getChildByTag(kSource)->removeAllChildrenWithCleanup(true);
//        removeChildByTag(kSource, true);
//        m_source      -> setTag(kLaiYuan);
        m_source      =  NULL;
    }

    this->setIsVisible(false);
    
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    setIsTouchEnabled(false);
    
    if(!MainScene::Inst()->new_FirstZhuanSheng){
        if (m_bisDropImage) {
           MainScene::Inst()->showBottomPanel(false);
        }else
        MainScene::Inst()->showBottomPanel(m_bShowBtmAfterHide);
    }
    
    if(MainScene::Inst()->new_FirstZhuanSheng && m_bShowBtmAfterHide)
    {
        MainScene::Inst()->new_FirstZhuanSheng_Second = true;
    }
    
    
    if(CGameData::Inst()->isNewbie())
        MainScene::Inst()->enableBottomPanel(false);
    

    MainScene::Inst()->setTitle(0, 0, 0, 0 ,0, 0);
    m_bForbShowRangeBtn = false;
    
    printf("hide[CShowCardInf] cur child count [%d]\n",getChildrenCount());
}


void CShowCardInf::setIsDropImage(bool bl)
{
    m_bisDropImage = bl;
}

//******************************************************************************
// showTorchParticle
//****************************************************************
void CShowCardInf::showTorchParticle() {
    CCParticleTorch *pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(50, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch);
    
    pTorch = CCParticleTorch::node();
    pTorch->setPosition(CCPointMake(590, 492));
    pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
	addChild(pTorch);
}

//******************************************************************************
// onExit
//******************************************************************************
void CShowCardInf::onExit()
{
    removeAllChildrenWithCleanup(true);
    m_pfnSelector =  NULL;
    m_pListener   =  NULL;
    m_source      =  NULL;
    
    CCLayer::onExit();
}

//******************************************************************************
// setOnExitCB
//******************************************************************************
void CShowCardInf::setOnExitCB(CCObject* listener,SEL_CallFunc m_pSelector)
{
    m_pListener=listener;
    m_pfnSelector=m_pSelector;
}

//******************************************************************************
// setShowBtmAfterHide
//******************************************************************************
void CShowCardInf::setShowBtmAfterHide(bool bShow)
{
    m_bShowBtmAfterHide = bShow;
}

//******************************************************************************
// setForbShowRangeBtn//禁止显示排序按钮
//******************************************************************************
void CShowCardInf::setForbShowRangeBtn(bool isSet)
{
    m_bForbShowRangeBtn  = isSet;
}