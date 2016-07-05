//
//  BonusItem.cpp
//  test
//
//  Created by mac on 13-6-5.
//
//

#include "BonusItem.h"
#include "TextNode.h"
#include "CGameData.h"


BonusItem::BonusItem(int iNo,CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_iNo = iNo;
    m_bShow = false;
}

void BonusItem::show(JiFengRecordInf* pInf)
{
    if (m_bShow)
        return;
    
    m_inf = *pInf;
    
    //背景框
    if (m_iNo % 2 == 0) {
        CCSprite* spBg = CCSprite::spriteWithFile("bonusitmbg.png");
        if (spBg) {
            addChild(spBg, -1);
            spBg->setAnchorPoint(CCPointZero);
            spBg->setPosition(CCPointZero);
        }
    }
    
    //成员名字
    CCLabelTTF* lbName = CCLabelTTF::labelWithString(pInf->strName.c_str(), CCSizeMake(180, 26), CCTextAlignmentLeft, "STHeitiSC-Medium", 22);
    addChild(lbName);
    lbName->setAnchorPoint(CCPointZero);
    lbName->setPosition(ccp(6,18));
    lbName->setColor(ccc3(94, 13, 11));
    
    char buf[32];
    //近五次伤害
    snprintf(buf, sizeof(buf), "%ld", pInf->lRecentScore);
    CCLabelTTF* lbRecentScore = CCLabelTTF::labelWithString(buf, "STHeitiSC-Medium", 22);
    addChild(lbRecentScore);
    lbRecentScore->setAnchorPoint(CCPointZero);
    lbRecentScore->setPosition(ccp(206, 16));
    lbRecentScore->setColor(ccc3(27, 54, 105));
    
    //上次伤害
    snprintf(buf, sizeof(buf), "%ld", pInf->lLastScore);
    CCLabelTTF* lbLastScore = CCLabelTTF::labelWithString(buf, "STHeitiSC-Medium", 22);
    addChild(lbLastScore);
    lbLastScore->setAnchorPoint(CCPointZero);
    lbLastScore->setPosition(ccp(340, 16));
    lbLastScore->setColor(ccBLACK);
    
    //分配按钮
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    
    if (spMenu1 && spMenu2) {
        CCMenuItem* mi = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(BonusItem::cbBtn) );
        mi->setAnchorPoint(CCPointZero);
        mi->setPositionInPixels(ccp(431, -3));
        
        CCMenu* menu = CCMenu::menuWithItems(mi, NULL);
        addChild(menu);
        menu->setPosition(CCPointZero);
        
        TextNode* txtFenPei = TextNode::textWithString("分配", 36);
        txtFenPei->setPosition(CCPointMake(38, 22));
        txtFenPei->setAnchorPoint(CCPointZero);
        txtFenPei->setColor(ccWHITE);
        txtFenPei->setShadowColor(ccBLACK);
        mi->addChild(txtFenPei);
        
        mi->setScale(0.8);
        mi->setTag(m_iNo);
    }
    
    m_bShow = true;
}

void BonusItem::hide()
{
    if (!m_bShow)
        return;
    
    removeAllChildrenWithCleanup(true);
    m_bShow = false;
}

void BonusItem::cbBtn(CCObject* pObj)
{
    (m_pListener->*m_pfnSelector)(&m_inf);
}


