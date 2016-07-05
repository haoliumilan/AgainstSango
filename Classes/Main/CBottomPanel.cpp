//
//  CBottomPanel.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CBottomPanel.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "MailBoxAgt.h"

#define kDiableCoverTag 0xaa

 
CBottomPanel::CBottomPanel(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_bEnable = true;
    mailTips = NULL;
    numberTips = NULL;
    gachaTips = NULL;
    cntryTips = NULL;
    
    m_bMvFlg = false;
    
    
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -900, true);
    
    CCSprite *spPanelBg = CCSprite::spriteWithSpriteFrameName("id_frame.png");
    spPanelBg->setPositionInPixels(ccp(0, 0));
    spPanelBg->setAnchorPoint(ccp(0, 0));
    addChild(spPanelBg);
    
    for (int n=0; n < 6; n++) {
        
        char imageName[50];
        
        CCSprite* spBtn ;
        

        sprintf(imageName, "id_button%d.png",n);
        spBtn = CCSprite::spriteWithSpriteFrameName(imageName);
        
        
        if (spBtn) {
            spBtn->setTag(n+1);
            spBtn->setAnchorPoint(CCPointMake(0, 0));
            spBtn->setPositionInPixels(ccp(5+n*105, 2));
            addChild(spBtn);
        }
        
        
        
        
        if(n == 5)
        {
            sprintf(imageName, "id_button%d_close.png",n);
            spBtn = CCSprite::spriteWithSpriteFrameName(imageName);
            
            
            
            spBtn->setTag(555);
            spBtn->setAnchorPoint(CCPointMake(0, 0));
            spBtn->setPositionInPixels(ccp(5+n*105, 2));
            addChild(spBtn);
        }
        


    }
    
    
    

    
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 0), 640, 145);
	addChild(lc, 100, kDiableCoverTag);
}

bool CBottomPanel::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!m_bEnable) {
        return false;
    }
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if (convertToNodeSpace( touchLocation ).y > 150) {
        return false;
    }
    
    m_bMvFlg = false;
    
    return true;
}

void CBottomPanel::ccTouchMoved(CCTouch* touch, CCEvent* event)
{    
    m_bMvFlg = true;
}

void CBottomPanel::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if (m_bMvFlg)
        return;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    m_ret.iBtnSel = chkTouchedBtn(nodePos);
    if (m_ret.iBtnSel == kInvalidValue) {
        CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyGoBottom);
    }
    else{
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
}

int CBottomPanel::chkTouchedBtn(CCPoint& pt)
{
    int iRet = kInvalidValue;
    
    for (int i = 0; i < 6; i++) {
        CCNode* node = getChildByTag(i+1);
        if (node) {
            CCSize sz = node->getContentSize();
            CCPoint pos = node->getPosition();
            
            bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pos.x,
                                                               pos.y,
                                                               sz.width,
                                                               sz.height),
                                                    pt);
            
            if (bRet) {
                iRet = i;
                break;
            }
        }
    }
    
    return iRet;
}


void CBottomPanel::upDateIcon()
{
    CCSprite* spBtn  = (CCSprite *)getChildByTag(555);
    
    if(spBtn)
    {
        if(CGameData::Inst()->getUsrInfo()->lv >= CGameData::Inst()->getCommonInfo()->permit_guild_lv || CGameData::Inst()->getCntryInfo()->status != 0)
        {
            
            spBtn->setIsVisible(false);

        }
        else
        {
            spBtn->setIsVisible(true);

        }
        
        
    }
    




}

void CBottomPanel::setEnable(bool bEnable)
{
    m_bEnable = bEnable;
    
    CCNode* node = getChildByTag(kDiableCoverTag);
    if(node == NULL)
        return;
    
    CCLayerColor* lc = (CCLayerColor*) node;
    
    if(bEnable)
    {
        lc->setOpacity(0);
    }
    else
    {
        lc->setOpacity(128);
    }
}

void CBottomPanel::showCntryTips(bool clearStatus){
    // country
    int cntryNum = 0;
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    if ((cntryInfo->status == 1) || (cntryInfo->status == 2))
        cntryNum = cntryInfo->member_apply_num + cntryInfo->org_dungeon_num;
    else
        cntryNum = cntryInfo->org_dungeon_num;
    
    if (!clearStatus) {
        if (CGameData::Inst()->haveNewMsg())
            cntryNum++;
    }
    
    
    if (cntryTips != NULL)
    {
        cntryTips->removeFromParentAndCleanup(true);
        cntryTips = NULL;
    }
    
    if (cntryNum > 0)
    {
        cntryTips = OcProxy::Inst()->showTipsAndNum(10);
        cntryTips->setPosition(ccp(85, 80));
        CCNode* ndCntry = getChildByTag(id_country + 1);
        ndCntry->addChild(cntryTips);
    }
}

void CBottomPanel::showTips()
{
    if (!CGameData::Inst()->chkLogin())
    {
        return;
    }
    
    CUserInfo* userInfo = CGameData::Inst()->getUsrInfo();
    CommonInfo* commonInfo = CGameData::Inst()->getCommonInfo();

    
    // friend
    if (mailTips != NULL)
    {
        mailTips->removeFromParentAndCleanup(true);
        mailTips = NULL;
    }
    
    if(numberTips!=NULL)
    {
        numberTips->removeFromParentAndCleanup(true);
        numberTips= NULL;
    }

    int number = MailBoxAgt::Inst()->getNewMailCnt();
    if (userInfo->last_mail_time > MailBoxAgt::Inst()->getLastRcvTime())
        // 服务器又收到新邮件
        number++;
    if (number > 0)
    {   // 显示邮件的ICON
        mailTips = CCSprite::spriteWithSpriteFrameName("icon_mail.png");
        mailTips->setPosition(ccp(85, 80));
        CCNode* ndFriend = getChildByTag(id_social + 1);
        ndFriend->addChild(mailTips);
        
        CCActionInterval* fadeAction = CCSequence::actionOneTwo(CCFadeOut::actionWithDuration(1.0),
                                                                CCFadeIn::actionWithDuration(1.0));
        mailTips->runAction(CCRepeatForever::actionWithAction(fadeAction));
    }
    else
    {
        number = userInfo->friend_request_num;
        
        if (number > 0) {
            numberTips = OcProxy::Inst()->showTipsAndNum(number);
            numberTips->setPosition(ccp(85, 80));
            CCNode* ndFriend = getChildByTag(id_social + 1);
            ndFriend->addChild(numberTips);
        }
    }
        
    // gacha
    int gacha = userInfo->gacha_pt/commonInfo->gacha_cost_pt;
    int chargeGacha = userInfo->coin/commonInfo->gacha_cost_coin;
    if (CGameData::Inst()->getCommonInfo()->bOpen_cards_shop == false)
    {
        gacha += chargeGacha;
    }
    
    if(gachaTips != NULL)
    {
        gachaTips->removeFromParentAndCleanup(true);
        gachaTips= NULL;
    }
    
    if (gacha > 0)
    {
        gachaTips = OcProxy::Inst()->showTipsAndNum(gacha);
        gachaTips->setPosition(ccp(85, 80));
        CCNode* ndGacha = getChildByTag(id_gacha + 1);
        ndGacha->addChild(gachaTips);
    }
    
    // country
    int cntryNum = 0;
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    if ((cntryInfo->status == 1) || (cntryInfo->status == 2))
        cntryNum = cntryInfo->member_apply_num + cntryInfo->org_dungeon_num;
    else
        cntryNum = cntryInfo->org_dungeon_num;
    
    if (CGameData::Inst()->haveNewMsg())
        cntryNum++;
    
    
    if (cntryTips != NULL)
    {
        cntryTips->removeFromParentAndCleanup(true);
        cntryTips = NULL;
    }
    
    if (cntryNum > 0)
    {
        cntryTips = OcProxy::Inst()->showTipsAndNum(10);
        cntryTips->setPosition(ccp(85, 80));
        CCNode* ndCntry = getChildByTag(id_country + 1);
        ndCntry->addChild(cntryTips);
    }

}

void CBottomPanel::clearTips()
{
    if (mailTips != NULL)
    {
        mailTips->removeFromParentAndCleanup(true);
        mailTips = NULL;
    }
    
    if(numberTips != NULL)
    {
        numberTips->removeFromParentAndCleanup(true);
        numberTips= NULL;
    }
    
    if(gachaTips != NULL)
    {
        gachaTips->removeFromParentAndCleanup(true);
        gachaTips= NULL;
    }

    if(cntryTips != NULL)
    {
        cntryTips->removeFromParentAndCleanup(true);
        cntryTips= NULL;
    }

}


