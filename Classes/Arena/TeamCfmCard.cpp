//
//  TeamCfmCard.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-2-25.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "TeamCfmCard.h"
#include "CGameData.h"


TeamCfmCard::TeamCfmCard(CUserCardInfo* pUsrCardInfo, bool bRent)
{
    if (pUsrCardInfo == NULL) {
        return;
    }
    
    m_ucid = pUsrCardInfo->ucid;
    m_cid = pUsrCardInfo->cid;
    m_bLeader = false;
    m_star = pUsrCardInfo->star;
    m_level = pUsrCardInfo->lv;
    
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(m_cid);
    if (spHead) {
        addChild(spHead);

        //究极金边
        CCSprite* sp1 = NULL;
        if (pUsrCardInfo->bIsEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (pUsrCardInfo->bIsWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            spHead->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, spHead->getContentSize().height-4));
        }
        
        char buf[100];
        
        //lv text
        if(pUsrCardInfo->lv == pUsrCardInfo->max_lv){
            snprintf(buf, 99, "()*+");
        }
        else{
            snprintf(buf, 99, ",-%d", pUsrCardInfo->lv);
        }
        
        CCLabelAtlas* lbLv;
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            lbLv = CCLabelAtlas::labelWithString(buf, "lvnum.png", sz.width / 18, sz.height, '(');
            lbLv->setAnchorPoint(CCPointMake(0.5, 0));
            lbLv->setPosition(CCPointMake(0, -54));
            addChild(lbLv);
        }
        
        spTmp = CCSprite::spriteWithFile("plusnum.png");
        if (pUsrCardInfo->plus > 0) {
            snprintf(buf, 99, "/%d", pUsrCardInfo->plus);
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* lbPlus = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
            lbPlus->setAnchorPoint(CCPointMake(0.5, 0));
            lbPlus->setPosition(CCPointMake(0, -54));
            addChild(lbPlus);
            lbPlus->setIsVisible(false);
            
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.5),
                                                                               CCHide::action(),
                                                                               CCDelayTime::actionWithDuration(1.5),
                                                                               CCShow::action(),
                                                                               NULL);
            CCActionInterval* action2 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.5),
                                                                               CCShow::action(),
                                                                               CCDelayTime::actionWithDuration(1.5),
                                                                               CCHide::action(),
                                                                               NULL);
            
            lbLv->runAction(CCRepeatForever::actionWithAction(action1));
            lbPlus->runAction(CCRepeatForever::actionWithAction(action2));
        }
    }
    
    CCSprite* spBg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    if (spBg) {
        addChild(spBg, -1);
    }
}