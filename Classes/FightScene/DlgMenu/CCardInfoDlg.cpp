//
//  CCardInfoDlg.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CCardInfoDlg.h"
#include "CSkillLib.h"
#include "TextNode.h"

CCardInfoDlg::CCardInfoDlg()
{
    m_spYanChiIcon = NULL;
    
    m_spDlg = CCSprite::spriteWithSpriteFrameName("fr_frame.png");
    m_spDlg->setScale(2.0);
    addChild(m_spDlg);
    m_spDlg->setAnchorPoint(ccp(0, 0.5));
    
//    m_spDlg->setScaleX(13.0);
//    
//    m_spDlg->setAnchorPoint(ccp(0, 0.5));
//    
//    float ff = m_spDlg->getContentSize().width;
    
    CCSize sz = m_spDlg->getContentSize();
    sz.width = sz.width * (2 * 0.75);
    sz.height = sz.height * 2 / 5;
    
    m_lbName = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cardInfoDlgTip1"), sz, CCTextAlignmentLeft, 20);
    addChild(m_lbName, 10);
    
    m_lbElem = TextNode::textWithString(" ", sz, CCTextAlignmentLeft, 20);
    addChild(m_lbElem, 10);
    
    m_lbAwake = TextNode::textWithString(" ", 20);
    addChild(m_lbAwake, 10);
    m_lbAwake->setAnchorPoint(ccp(0, 0.5));
    
    m_lbSkill = TextNode::textWithString(" ", sz, CCTextAlignmentLeft, 20);
    addChild(m_lbSkill, 10);
    
    m_lbRemainCD = TextNode::textWithString(" ", sz, CCTextAlignmentLeft, 20);
    addChild(m_lbRemainCD, 10);
}



void CCardInfoDlg::setCardInfo(Card* card)
{
    m_spDlg->setScaleX(2.0f);
    
    const char* elem[enElementMax] ={
        CGameData::Inst()->getLanguageValue("eleHeartTip"),
        CGameData::Inst()->getLanguageValue("eleWaterTip"),
        CGameData::Inst()->getLanguageValue("eleFireTip"),
        CGameData::Inst()->getLanguageValue("eleWoodTip"),
        CGameData::Inst()->getLanguageValue("eleLightTip"),
        CGameData::Inst()->getLanguageValue("eleDarkTip"),
        CGameData::Inst()->getLanguageValue("eleWuXiaoTip")
    };
    
    char buf[100];
    sprintf(buf, "%s：%s", CGameData::Inst()->getLanguageValue("propTip"), elem[card->getElement()]);
    m_lbElem->setString(buf);
    
    
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        if (m_lbAwake->getChildByTag(20131105+i))
        {
            m_lbAwake->removeChildByTag(20131105+i, true);
        }
    }
    
    //sprintf(buf, "%s：%d",CGameData::Inst()->getLanguageValue("AwakeSkill_Title"), card->getAttack());
    m_lbAwake->setString(CGameData::Inst()->getLanguageValue("AwakeSkill_Title"));
    
    float offx = m_lbAwake->getContentSize().width+14;
    int cout = 0;
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        if (card->getAwakeSkillArr(i) == enAwakeSkill_NULL)break;
        
        char buf[100];
        snprintf(buf, 99, "awakeskill_%d.png", card->getAwakeSkillArr(i));
        if (CCSprite* spIcon = CCSprite::spriteWithFile(buf))
        {
            if (card->getAwakeSkillNum() <= i)spIcon->setOpacity(120);
            CCLayer *pIconLayer = CCLayer::node();
            pIconLayer->addChild(spIcon);
            m_lbAwake->addChild(pIconLayer,10, 20131105+i);
            pIconLayer->setAnchorPoint(ccp(0, 0.5));
            pIconLayer->setPosition(ccp(offx,0));
            offx += spIcon->getContentSize().width +2;
            cout++;
            
            if (i<card->getAwakeSkillNum())
            {
                if (EN_CARD_STATE_FNEGYIN==card->getState())
                {
                    if (CCSprite* pLock = CCSprite::spriteWithSpriteFrameName("cardlock.png"))
                    {
                        pLock->setScale(0.25f);
                        pIconLayer->addChild(pLock);
                    }
                }
            }
        }
    }
    
    if (cout>6 && cout <9)
    {
        m_spDlg->setScaleX(3.0f);
    }
    else if (cout > 4 && cout < 7)
    {
        m_spDlg->setScaleX(2.5f);
    }
    
    switch (cout)
    {
        case 8:
            m_spDlg->setScaleX(3.0f);
            break;
            
        case 7:
            m_spDlg->setScaleX(2.8f);
            break;
            
        case 6:
            m_spDlg->setScaleX(2.6f);
            break;
            
        case 5:
            m_spDlg->setScaleX(2.4f);
            break;
            
        default:
            break;
    }
    
    m_lbName->setString(card->getName());
}

void CCardInfoDlg::setCardInfo(int iElem, int iAttack, const char* cardName)
{
    const char* elem[enElementMax] ={
        CGameData::Inst()->getLanguageValue("eleHeartTip"),
        CGameData::Inst()->getLanguageValue("eleWaterTip"),
        CGameData::Inst()->getLanguageValue("eleFireTip"),
        CGameData::Inst()->getLanguageValue("eleWoodTip"),
        CGameData::Inst()->getLanguageValue("eleLightTip"),
        CGameData::Inst()->getLanguageValue("eleDarkTip"),
        CGameData::Inst()->getLanguageValue("eleWuXiaoTip")
    };
    
    char buf[100];
    sprintf(buf, "%s：%s", CGameData::Inst()->getLanguageValue("propTip"), elem[iElem]);
    m_lbElem->setString(buf);
    
    //sprintf(buf, "%s：%d",CGameData::Inst()->getLanguageValue("attackTip"), iAttack);
    m_lbAwake->setString(CGameData::Inst()->getLanguageValue("AwakeSkill_Title"));
    
    m_lbName->setString(cardName);
}

void CCardInfoDlg::setSkill(int skillID, int cd, bool bYanChi)
{
    char buf[100];
    
    const char *skillName = CSkillLib::Ins()->getManSkillName(skillID);
    if(strlen(skillName) == 0) {
        m_lbRemainCD->setString("");
        sprintf(buf, "%s：%s", CGameData::Inst()->getLanguageValue("skillTip"),CGameData::Inst()->getLanguageValue("noTip"));
    
        if (m_spYanChiIcon)
        {
            m_spYanChiIcon->removeFromParentAndCleanup(true);
            m_spYanChiIcon = NULL;
        }
    }
    else
    {
        if(CGameData::Inst()->isNewbie())
        {
            if(cd > 0)
                cd = 1;
        }
        
        sprintf(buf, "%s%d%s",CGameData::Inst()->getLanguageValue("cardInfoDlgTip2"),cd,CGameData::Inst()->getLanguageValue("cardInfoDlgTip3"));
        m_lbRemainCD->setString(buf);
        
        if (bYanChi)
        {
            m_lbRemainCD->setColor(ccRED);
            
            if (m_spYanChiIcon == NULL)
            {
                m_spYanChiIcon = CCSprite::spriteWithSpriteFrameName("icon_yanchi.png");
                float posX = m_lbRemainCD->getPosition().x + 30;
                float posY = m_lbRemainCD->getPosition().y;
                m_spYanChiIcon->setPosition(CCPointMake(posX, posY));
                addChild(m_spYanChiIcon);
            }
        }
        else
        {
            m_lbRemainCD->setColor(ccWHITE);
            
            if (m_spYanChiIcon)
            {
                m_spYanChiIcon->removeFromParentAndCleanup(true);
                m_spYanChiIcon = NULL;
            }
        }
        sprintf(buf, "%s:%s", CGameData::Inst()->getLanguageValue("skillTip"), CSkillLib::Ins()->getManSkillName(skillID));
    }
    m_lbSkill->setString(buf);
}


void CCardInfoDlg::setPosition(const CCPoint& newPosition)
{
    m_spDlg->setScaleX(2.0f);
    m_spDlg->setPosition(ccp(newPosition.x-m_spDlg->getContentSize().width+8, newPosition.y));
    
    CCSize sz = m_spDlg->getContentSize();
    sz.width *= 2;
    sz.height *= 2;
    
    m_lbName->setPosition(CCPointMake(newPosition.x, newPosition.y +2 + 2 * sz.height / 6));
    m_lbElem->setPosition(CCPointMake(newPosition.x, newPosition.y +2+ sz.height / 6));
    m_lbAwake->setPosition(CCPointMake(newPosition.x - 106, newPosition.y+2));
    m_lbSkill->setPosition(CCPointMake(newPosition.x, newPosition.y +2- sz.height / 6));
    m_lbRemainCD->setPosition(CCPointMake(newPosition.x, newPosition.y+2 - 2 * sz.height / 6));
}



void CCardInfoDlg::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}


