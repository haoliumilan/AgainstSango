//
//  AddFriend.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "AddFriend.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "SaveData.h"

#define kOkBtnTag       123
#define kCancelBtnTag   456
//******************************************************************************
// Construct
//******************************************************************************
AddFriend::AddFriend(CCObject* target, SEL_CallFuncO selector):m_bFriend(false)
{
    m_pListener = target;
    m_pfnSelector = selector;
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool AddFriend::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* nodeOk = getChildByTag(kOkBtnTag);
    CCNode* nodeCancel = getChildByTag(kCancelBtnTag);
    
    CCPoint pt;
    CCSize sz;
    bool bRet;
    
    if(nodeOk){
        pt = nodeOk->getPosition();
        sz = nodeOk->getContentSize();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                      pt.y - sz.height * 0.5, 
                                                      sz.width, 
                                                      sz.height), 
                                           pos);
        
        if(bRet){
            if(m_bFriend)
                m_ret.fid[0] = 0;
            
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    
    if((!bRet) && (nodeCancel)){
        pt = nodeCancel->getPosition();
        sz = nodeCancel->getContentSize();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                      pt.y - sz.height * 0.5, 
                                                      sz.width, 
                                                      sz.height), 
                                           pos);
        
        if(bRet){
            m_ret.fid[0] = 0;
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    
    
    return true;
}

//******************************************************************************
// show
//******************************************************************************
void AddFriend::show()
{
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    //取得数据
    SaveData_Helper* helpInfo = SaveData::Inst()->getHelperInfoForAddFriend();
    strncpy(m_ret.fid, helpInfo->helper_uid, 99);
    m_bFriend = (strcmp(helpInfo->helper_type, "friend") == 0) ? true : false;
    
    ProcFriendInfBarFromSaveData();
    
    if(m_bFriend)
        procAsFriend();
    else
        procAsOther();
    
    //窗体
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    sp->setScale(2.0);
    addChild(sp, -1);
}

//******************************************************************************
// ProcFriendInfBarFromSaveData
//******************************************************************************
void AddFriend::ProcFriendInfBarFromSaveData()
{
    int index = SaveData::Inst()->getHelperIndexForAddFriend();
    SaveData_Helper* helpInfo = SaveData::Inst()->getHelperInfo(index);
    SaveData_Card* pCardInf = SaveData::Inst()->getCardInfo(index * 6 + 5);
    //frame
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    if(sp){
        sp->setPosition(CCPointMake(0, 80));
        addChild(sp);
    }
    
    char buf[100];
    //head
    CCPoint pt(-sp->getContentSize().width * 0.5, 80);
    CCSprite* spHead = CGameData::Inst()->getHeadSprite(pCardInf->cid);
    pt.x += spHead->getContentSize().width * 0.5;
    pt.x -= 2;
    spHead->setPosition(pt);
    addChild(spHead);
    
    //leader card lv
    if(pCardInf->lv == pCardInf->lv_max){
        snprintf(buf, 99,"Lv.%s", CGameData::Inst()->getLanguageValue("max"));
    }
    else{
        sprintf(buf, "Lv.%ld", pCardInf->lv);
    }
    TextNode* lbLeaderLv = TextNode::textWithString(buf,
                                                    CCSizeMake(100, 30),
                                                    CCTextAlignmentCenter,
                                                    24);
    pt = spHead->getPosition();
    pt.y -= spHead->getContentSize().height * 0.5;
    pt.y += 8;
    lbLeaderLv->setPosition(pt);
    addChild(lbLeaderLv, 1);
    
    //helper name
    TextNode* lbHelperName = TextNode::textWithString(helpInfo->helper_name,
                                                      CCSizeMake(240, 26),
                                                      CCTextAlignmentLeft,
                                                      24);
    lbHelperName->setPosition(CCPointMake(-25, 98));
    lbHelperName->setColor(ccBLACK);
    addChild(lbHelperName);
    
    //country
    CCSprite* spCountry = NULL;
    switch (helpInfo->helper_country) {
        case 1:
        case 2:
        case 3:
            snprintf(buf, 99, "stone%ds.png", helpInfo->helper_country);
            spCountry = CCSprite::spriteWithSpriteFrameName(buf);
            break;
            
        default:
            break;
    }
    
    if(spCountry){
        spCountry->setPosition(CCPointMake(-164, 98));
        addChild(spCountry);
    }
    
    //friend lv
    sprintf(buf, "%s:%d",
            CGameData::Inst()->getLanguageValue("level"),
            helpInfo->helper_lv);
    TextNode* lbLv = TextNode::textWithString(buf,
                                              CCSizeMake(150, 30),
                                              CCTextAlignmentRight,
                                              30);
    pt.x = 195;
    pt.y = 102;
    lbLv->setPosition(pt);
    lbLv->setColor(ccWHITE);
    addChild(lbLv, 1);
    
    //援军点数结算
    TextNode* lbText;
    
    snprintf(buf, 99, "%s%d%s",
             CGameData::Inst()->getLanguageValue("addFriendTip1"),
             SaveData::Inst()->getAllGachaPoint(),
             CGameData::Inst()->getLanguageValue("addFriendTip2"));
    
    lbText = TextNode::textWithString(buf, CCSizeMake(600, 40), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, -40));
    lbText->setColor(kInfoColor);
    addChild(lbText, 1);
    
    
    snprintf(buf, 99, "%s%hu",
             CGameData::Inst()->getLanguageValue("addFriendTip3"),
             SaveData::Inst()->getGachaPtAll());
    lbText = TextNode::textWithString(buf, CCSizeMake(600, 40), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, -90));
    lbText->setColor(kInfoColor);
    addChild(lbText, 1);
}

//******************************************************************************
// hide
//******************************************************************************
void AddFriend::procAsFriend()
{
    int delta = 30;
    //ok
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(sp){
        sp->setPosition(CCPointMake(0, -150-delta));
        addChild(sp, 10, kOkBtnTag);
    }
    
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                CCSizeMake(100, 30),
                                                CCTextAlignmentCenter,
                                                kBtnTitleHeight );
    lbText->setPosition(CCPointMake(0, -148-delta));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 21);
    
    //title
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("addFriendTip4"),
                                      CCSizeMake(500, 40),
                                      CCTextAlignmentCenter,
                                      55);
    lbText->setPosition(CCPointMake(0, 170));
    lbText->setColor(kTitleColor);
    addChild(lbText, 1);
}

//******************************************************************************
// hide
//******************************************************************************
void AddFriend::procAsOther()
{
    //ok
    int delta = 30,deltaX = 30;
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(sp){
        sp->setPosition(CCPointMake(-153+deltaX, -150-delta));
        addChild(sp, 10, kOkBtnTag);
    }
    
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
    lbText->setPosition(CCPointMake(-153+deltaX-3, -150-delta+2));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 21);

    //cancel
    sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(sp){
        sp->setPosition(CCPointMake(153-deltaX, -150-delta));
        addChild(sp, 10, kCancelBtnTag);
    }
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
    lbText->setPosition(CCPointMake(153-deltaX-3, -150-delta+2));
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 21);
    
    //title
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("addFriendTip4"), CCSizeMake(500, 40), CCTextAlignmentCenter, 55);
    lbText->setPosition(CCPointMake(0, 170));
    lbText->setColor(kTitleColor);
    addChild(lbText, 1);

    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("addFriendTip5"), CCSizeMake(600, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, 0));
    lbText->setColor(kInfoColor);
    addChild(lbText, 1);
    

}

//******************************************************************************
// hide
//******************************************************************************
void AddFriend::hide()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    setIsTouchEnabled(false);
    
    removeAllChildrenWithCleanup(true);
}

//******************************************************************************
// onExit
//******************************************************************************
void AddFriend::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}