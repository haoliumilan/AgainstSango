//
//  CSkillDlg.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CSkillDlg.h"
#include "CSkillLib.h"
#include "CommonDef.h"
#include "TextNode.h"
#include "CGamedata.h"

CSkillDlg::CSkillDlg(CCObject* target, SEL_CallFuncO selector):m_bShow(false)
{
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -128, true);
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_spDlg = CCSprite::spriteWithSpriteFrameName("fightDlg.png");
    m_spDlg->setScale(2.0);
    addChild(m_spDlg);
    
    CCSize sz = m_spDlg->getContentSize();
    sz.width *= 2;
    sz.height *= 2;
    
    m_lbName = TextNode::textWithString( CGameData::Inst()->getLanguageValue("skillDlgTip1"), CCSizeMake(sz.width-20, sz.height/5), CCTextAlignmentCenter, 30);
    addChild(m_lbName, 10);
    
    m_lbDsp = TextNode::textWithString(CGameData::Inst()->getLanguageValue("skillDlgTip2"), CCSizeMake(sz.width-40, sz.height/5), CCTextAlignmentCenter, 25);
    addChild(m_lbDsp, 10);
    
    m_lbCD = TextNode::textWithString("", CCSizeMake(sz.width-20, sz.height/5), CCTextAlignmentCenter, 25);
    addChild(m_lbCD, 10);
    
    m_lbUseCfm = TextNode::textWithString(CGameData::Inst()->getLanguageValue("skillDlgTip3"), CCSizeMake(sz.width-20, sz.height/5), CCTextAlignmentCenter, 25);
    addChild(m_lbUseCfm, 10);
    
    int delta = 30;
    m_lbName->setPosition(CCPointMake(0, 2* sz.height/5-delta));
    m_lbDsp->setPosition(CCPointMake(0,2* sz.height/5-35-20-delta));
    m_lbCD->setPosition(CCPointMake(0, 2* sz.height/5-70-30-delta));
    m_lbUseCfm->setPosition(CCPointMake(0, 2* sz.height/5-105-30-delta));
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_miOk = CCMenuItemImage::itemFromNormalSprite(spMenu1,spMenu2, this, menu_selector(CSkillDlg::menuCallback));
    m_miOk->setTag(11);
    
    spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    m_miCancel = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(CSkillDlg::menuCallback));
    m_miCancel->setTag(12);
    m_miOk->setAnchorPoint(ccp(0, 0));
    m_miCancel->setAnchorPoint(ccp(0, 0));
    
    
    
    TextNode *sureLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
    sureLabel->setColor(ccc3(255, 255, 255));
    sureLabel->setShadowColor(ccBLACK);
    sureLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    m_miOk->addChild(sureLabel);
    
    
    TextNode *cancelLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), kBtnTitleHeight);
    cancelLabel->setColor(ccc3(255, 255, 255));
    cancelLabel->setShadowColor(ccBLACK);
    cancelLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                         spMenu1->getContentSize().height * 0.5 + 2));
    m_miCancel->addChild(cancelLabel);
      
    
    CCPoint  point = m_miOk->getPosition();
    point.x+=190;
    m_miCancel->setPosition(point);
    CCMenu *menu = CCMenu::menuWithItems(m_miOk,m_miCancel,NULL);
    addChild(menu);
    menu->setAnchorPoint(CCPoint(0, 0));
    menu->setPosition(CCPoint(-170, -110-35));
}

void CSkillDlg::menuCallback(CCObject *pSender)
{
    CCMenuItemImage *item = (CCMenuItemImage *)pSender;
    if(item->getTag() == 11)
    {
        m_pSkillDlgRet.bUse = true;

    }else {
     
        m_pSkillDlgRet.bUse = false;

    }
    
    m_miOk->setIsEnabled(false);
    m_miCancel->setIsEnabled(false);
//    if(iLocation == 1)
//    else
    
    (m_pListener->*m_pfnSelector)(&m_pSkillDlgRet);
}
void CSkillDlg::setSkillID(int iID)
{
    const char* pStr = CSkillLib::Ins()->getManSkillName(iID);
    if(pStr != NULL)
        m_lbName->setString(pStr);
    
    pStr = CSkillLib::Ins()->getManSkillDsp(iID);
    if(pStr != NULL)
        m_lbDsp->setString(pStr);
}


void CSkillDlg::setCD(int iCD)
{
    char buf[100];
    sprintf(buf, "%d%s", iCD,CGameData::Inst()->getLanguageValue("skillDlgTip4"));
    m_lbCD->setString(buf);
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CSkillDlg::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(m_bShow == false)
        return false;
    
//    int iLocation =  containsTouchLocation(touch);
//    
//    if(iLocation == 0)
//        return true;
//    
//    
//    if(iLocation == 1)
//        m_pSkillDlgRet.bUse = true;
//    else
//        m_pSkillDlgRet.bUse = false;
//    
//    (m_pListener->*m_pfnSelector)(&m_pSkillDlgRet);
    
    return true;
}


//******************************************************************************
// containsTouchLocation
// 0: 没点中按鈕, 1: 確定,  2:取消
//******************************************************************************
int CSkillDlg::containsTouchLocation(CCTouch *touch)
{

    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );

    CCSize  szDlg = m_spDlg->getContentSize();
    CCPoint ptDlg = m_spDlg->getPosition();
    
    CCSize  szBtn(szDlg.width * 0.25, szDlg.height * 0.25);
    CCPoint ptOk(ptDlg.x - szDlg.width * 0.25, ptDlg.y - szDlg.height * 0.25);
    CCPoint ptCancel(ptDlg.x + szDlg.width * 0.25, ptDlg.y - szDlg.height * 0.25);
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(ptOk.x - szBtn.width * 0.5, 
                                                      ptOk.y - szBtn.height * 0.5, 
                                                      szBtn.width, szBtn.height),
                                           nodePos);
    
    if(bRet)
        return 1;
    
    
    bRet = CCRect::CCRectContainsPoint(CCRectMake(ptCancel.x - szBtn.width * 0.5, 
                                                       ptCancel.y - szBtn.height * 0.5, 
                                                       szBtn.width, szBtn.height),
                                            nodePos);
    
    if(bRet)
        return 2;
    
    return 0;
}

//******************************************************************************
// show
//******************************************************************************
void CSkillDlg::show(bool bShow)
{
    m_bShow = bShow;
    
    if(bShow){
        this->setScale(0.1);
        this->setIsVisible(true);
        this->runAction(CCScaleTo::actionWithDuration(0.3, 1.0));
        
        m_miOk->setIsEnabled(true);
        m_miCancel->setIsEnabled(true);
    }
    else{
        this->setScale(1.0);
        this->runAction(CCScaleTo::actionWithDuration(0.3, 0.0));
    }
}


//******************************************************************************
// onExit
//******************************************************************************
void CSkillDlg::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}



