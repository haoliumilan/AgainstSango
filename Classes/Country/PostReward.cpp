//
//  PostReward.cpp
//  AgainstWar
//
//  Created by user on 13-7-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "PostReward.h"


#define delatTAGTip  20130902


PostReward::PostReward(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgNoTip = NULL;
    m_commDlg = NULL;
    m_dlgTip = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_listPostReward = new CListLayer(this, callfuncO_selector(PostReward::cbList),CGameData::Inst()->getCntryInfo()->status == 1?1:2,268,592);
    addChild(m_listPostReward, -1);
    m_listPostReward->release();
    m_listPostReward->setReloadFunc(callfuncO_selector(PostReward::insertRewardByIdx));
    m_listPostReward->setTouchBeginFunc(callfuncO_selector(PostReward::cbListBegin));
    m_listPostReward->setTouchEndFunc(callfuncO_selector(PostReward::cbListEnd));
    
    showPostReward();
    
}


PostReward::~PostReward()
{
    
}


void PostReward::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(PostReward::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk)
    {
        switch (m_curReqType)
        {
            case en_reqAcceptReward:
                m_ret.iBtnSel = en_refreshPostReward;
                (m_pListener->*m_pfnSelector)(&m_ret);
                break;
            
            case en_reqRecieveReward:
                m_ret.iBtnSel = en_refreshPostReward;
                (m_pListener->*m_pfnSelector)(&m_ret);
                break;
                
            default:
                break;
                
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(PostReward::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


//******************************************************************************
// cbCommDlg
//******************************************************************************
void PostReward::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true)
    {
        switch (m_curReqType) {
            case en_reqAcceptReward:
                reqAcceptRewardTask();
                break;
                
            case en_reqRecieveReward:
                reqRecieveRewardTask();
                break;
                
            default:
                break;
        }
    }
    else
    {
        CGameData::Inst()->clearReqStat();
    }
}

void PostReward::showAlertNoTaskDlg()
{
    if (m_dlgNoTip == NULL)
    {
        m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(PostReward::cbAlertDlg));
        addChild(m_dlgNoTip);
        m_dlgNoTip->release();
        m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("cntry_xs_havenotask"));
        m_dlgNoTip->setPosition(CCPoint(960, 400));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
        m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
}

void PostReward::cbAlertDlg()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
    
    m_ret.iBtnSel = en_haveNoRewardTask;
    (m_pListener->*m_pfnSelector)(&m_ret);
}


void PostReward::cbList(CCObject* pObj)
{

}

void PostReward::cbListEnd(CCObject* pObj)
{
    //CListLayerRet* ret = (CListLayerRet*)pObj;
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listPostReward->getChildByTag(deltaTag+m_iTouchSel));
    
    if (!menuBtn)
        return;
    
    TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(delatTAGTip);
    if (tips)
        tips->runAction(CCScaleTo::actionWithDuration(0.1,0));
}

void PostReward::cbListBegin(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    
    float y = ret->pos.y - (268*ret->iBtnSel);
    
    y *= -1;
    
    CCPoint pos = ccp(ret->pos.x - 320, y);
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listPostReward->getChildByTag(deltaTag+ret->iBtnSel));
    
    CCSprite* rewardIcon = (CCSprite*)menuBtn->getChildByTag(TAGXSREWARDICON);
    
    CCRect rect = CCRectMake(rewardIcon->getPosition().x - rewardIcon->getContentSize().width * 0.5,
                             rewardIcon->getPosition().y - rewardIcon->getContentSize().height * 0.5,
                             rewardIcon->getContentSize().width,
                             rewardIcon->getContentSize().height);
    
    if (CCRect::CCRectContainsPoint(rect,pos))
    {
        m_iTouchSel = ret->iBtnSel;
        TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(delatTAGTip);
        tips->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
    }
}

void PostReward::showPostReward()
{
    showPostRewardList();
    
    if (CGameData::Inst()->getCntryInfo()->status == 1)
    {
        CCSprite* bottomBar = CCSprite::spriteWithFile("fr_substrate.png");
        bottomBar->setPosition(ccp(320, 188));
        addChild(bottomBar,0,13071701);
        
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCMenuItem*  xuanShangBaoKuMM = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                              spMenu2,
                                                                              this,
                                                                              menu_selector(PostReward::xuanShangBaoKu));
        xuanShangBaoKuMM->setAnchorPoint(CCPointZero);
        xuanShangBaoKuMM->setPosition(ccp(15, 12));
        //name
        TextNode* BaokuText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_baoku"),
                                                       36);
        BaokuText->setPosition(CCPointMake(110,bottomBar->getContentSize().height/2));
        BaokuText->setColor(ccWHITE);
        BaokuText->setShadowColor(ccBLACK);
        bottomBar->addChild(BaokuText, 1);
        
        spMenu1 = CCSprite::spriteWithFile("fr_button_green.png");
        spMenu2 = CCSprite::spriteWithFile("fr_button_green.png");
        CCMenuItem*  xuanShangFaBu = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                           spMenu2,
                                                                           this,
                                                                           menu_selector(PostReward::faBuXuanShang));
        xuanShangFaBu->setAnchorPoint(CCPointZero);
        xuanShangFaBu->setPosition(ccp(434, 12));
        //name
        TextNode* FaBuText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_fabu"),
                                                      36);
        FaBuText->setPosition(CCPointMake(530,bottomBar->getContentSize().height/2));
        FaBuText->setColor(ccWHITE);
        FaBuText->setShadowColor(ccBLACK);
        bottomBar->addChild(FaBuText, 1);
        
        CCMenu* menus = CCMenu::menuWithItems(xuanShangBaoKuMM,xuanShangFaBu,NULL);
        menus->setPosition(CCPointZero);
        menus->setiPriority(-150);
        bottomBar->addChild(menus);
    }
}

void PostReward::xuanShangBaoKu(CCObject* pObj)
{
    m_ret.iBtnSel = en_cbXuanShangBaoKu;
    (m_pListener->*m_pfnSelector)(&m_ret);
}


void PostReward::faBuXuanShang(CCObject* pObj)
{
    m_ret.iBtnSel = en_fabuXuanShang;
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void PostReward::showPostRewardList()
{
    CMenuBtn* newMenuBtn;
    CCMutableArray<CPostRewardBaseInfo*>* pRewardTaskList = CGameData::Inst()->getRewardTaskList();
    
    if (pRewardTaskList->count() <= 0)
    {
        showAlertNoTaskDlg();
        return;
    }
    
    for (int i = 0; i < pRewardTaskList->count(); i++)
    {
        if (i >= m_listPostReward->getPreTopShow()
            && i <= m_listPostReward->getPreBottomShow())
        {
            CPostRewardBaseInfo* rewardInfo = pRewardTaskList->getObjectAtIndex(i);
            newMenuBtn = new CMenuBtn(rewardInfo);
            
            if (rewardInfo->status > 0)
            {
                CCSprite* spJoin = CCSprite::spriteWithFile("reward_join.png");
                newMenuBtn->addChild(spJoin);
                spJoin->setPosition(CCPointMake(250, 85));
            }
            
            if (rewardInfo->status == 1 || rewardInfo->status == 2) // 已参加、已完成
            {
                CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
                CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");                
                CCMenuItem *itemInfo = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                             spMenu2,
                                                                             this,
                                                                             menu_selector(PostReward::cbShowRewardInfo));
                itemInfo->setTag(i*10 + 2);
                itemInfo->setAnchorPoint(ccp(0, 0));
                itemInfo->setPosition(CCPointMake(0, 50));
                itemInfo->setScale(0.8);
                
                TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_xiangqing"),
                                                            30);
                lbText->setAnchorPoint(ccp(0.5, 0.5));
                lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                                spMenu1->getContentSize().height * 0.5));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                itemInfo->addChild(lbText, 1);

                spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
                spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
                CCMenuItem *itemRecieve = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                                spMenu2,
                                                                                this,
                                                                                menu_selector(PostReward::cbRecieveReward));
                itemRecieve->setTag(i*10 + 3);
                itemRecieve->setAnchorPoint(ccp(0, 0));
                itemRecieve->setPosition(CCPointMake(0, -10));
                itemRecieve->setScale(0.8);
                
                lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_lingqu"),
                                                  30);
                lbText->setAnchorPoint(ccp(0.5, 0.5));
                lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                                spMenu1->getContentSize().height * 0.5));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                itemRecieve->addChild(lbText, 1);
                
                if (rewardInfo->status == 1)
                {
                    CCSprite* spBlack = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
                    spBlack->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                                     spMenu1->getContentSize().height * 0.5));
                    itemRecieve->addChild(spBlack, 1);
                }

                CCMenu *menu = CCMenu::menuWithItems(itemInfo, itemRecieve, NULL);
                newMenuBtn->addChild(menu,100);
                menu->setPosition(ccp(124, -106));
            }
            else if (rewardInfo->status == 3
                     || rewardInfo->CurJoinPerson >= rewardInfo->MaxJoinPerson
                     || rewardInfo->applyDeadLine <= CGameData::Inst()->getCurrentTime())   // 已领取、未参加（人数已满、时间已过）
            {
                CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                CCMenuItem *itemInfo = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                             spMenu2,
                                                                             this,
                                                                             menu_selector(PostReward::cbShowRewardInfo));
                itemInfo->setTag(i*10 + 2);
                itemInfo->setAnchorPoint(ccp(0, 0));
                
                TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_xiangqing"),
                                                            28);
                lbText->setAnchorPoint(ccp(0.5, 0.5));
                lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                                spMenu1->getContentSize().height * 0.5 + 2));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                itemInfo->addChild(lbText, 1);
                
                CCMenu *menu = CCMenu::menuWithItem(itemInfo);
                newMenuBtn->addChild(menu,100);
                menu->setPosition(ccp(124, -106));
            }
            else   // 未参加（可以参加）
            {
                CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                
                CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                             spMenu2,
                                                                             this,
                                                                             menu_selector(PostReward::cbJoinPostReward));
                itemShow->setTag(i*10+1);
                itemShow->setAnchorPoint(ccp(0, 0));
                
                char buf[100];
                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_post_reward_canjia"));
                
                TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
                lbText->setAnchorPoint(ccp(0.5, 0.5));
                lbText->setPosition(CCPointMake(196,-64));
                lbText->setColor(ccWHITE);
                lbText->setShadowColor(ccBLACK);
                newMenuBtn->addChild(lbText, 200);
                
                
                CCMenu *menu = CCMenu::menuWithItem(itemShow);
                newMenuBtn->addChild(menu,100);
                menu->setPosition(ccp(124, -106));
            }
            
            TipsPopInfo* tips = TipsPopInfo::tipWithContent(rewardInfo->strItemDsp.c_str());
            
            //            const char* chr = "物色遇袭:可以得到10个社呢么";
            //            TipsPopInfo* tips = TipsPopInfo::tipWithContent(chr);
            
            newMenuBtn->addChild(tips,10,delatTAGTip);
            tips->setPosition(ccp(50, 10));
            tips->setScale(0);        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_listPostReward->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
        
    }
    
    if (CGameData::Inst()->getCntryInfo()->status == 1)
    m_listPostReward->increaseUpLimitY(-220);
}

void PostReward::insertRewardByIdx(CCObject *pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;

    CMenuBtn* newMenuBtn = new CMenuBtn(CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(i));    
    m_listPostReward->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    TipsPopInfo* tips = TipsPopInfo::tipWithContent(CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(i)->strItemDsp.c_str());
    newMenuBtn->addChild(tips,10,delatTAGTip);
    tips->setPosition(ccp(50, 10));
    tips->setScale(0);
    
    
    CPostRewardBaseInfo* rewardInfo = CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(pRet->iBtnSel);
    
    if (rewardInfo->status > 0)
    {
        CCSprite* spJoin = CCSprite::spriteWithFile("reward_join.png");
        newMenuBtn->addChild(spJoin);
        spJoin->setPosition(CCPointMake(250, 85));
    }
    
    if (rewardInfo->status == 1 || rewardInfo->status == 2) // 已参加、已完成
    {
        CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCMenuItem *itemInfo = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                     spMenu2,
                                                                     this,
                                                                     menu_selector(PostReward::cbShowRewardInfo));
        itemInfo->setTag(i*10 + 2);
        itemInfo->setAnchorPoint(ccp(0, 0));
        itemInfo->setPosition(CCPointMake(0, 50));
        itemInfo->setScale(0.8);
        
        TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_xiangqing"),
                                                    30);
        lbText->setAnchorPoint(ccp(0.5, 0.5));
        lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                        spMenu1->getContentSize().height * 0.5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        itemInfo->addChild(lbText, 1);
        
        spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        CCMenuItem *itemRecieve = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                        spMenu2,
                                                                        this,
                                                                        menu_selector(PostReward::cbRecieveReward));
        itemRecieve->setTag(i*10 + 3);
        itemRecieve->setAnchorPoint(ccp(0, 0));
        itemRecieve->setPosition(CCPointMake(0, -10));
        itemRecieve->setScale(0.8);
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_lingqu"),
                                          30);
        lbText->setAnchorPoint(ccp(0.5, 0.5));
        lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                        spMenu1->getContentSize().height * 0.5));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        itemRecieve->addChild(lbText, 1);
        
        if (rewardInfo->status == 1)
        {
            CCSprite* spBlack = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
            spBlack->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5,
                                             spMenu1->getContentSize().height * 0.5));
            itemRecieve->addChild(spBlack, 1);
        }
        
        CCMenu *menu = CCMenu::menuWithItems(itemInfo, itemRecieve, NULL);
        newMenuBtn->addChild(menu,100);
        menu->setPosition(ccp(124, -106));
    }
    else if (rewardInfo->status == 3
             || rewardInfo->CurJoinPerson >= rewardInfo->MaxJoinPerson
             || rewardInfo->applyDeadLine <= CGameData::Inst()->getCurrentTime())   // 已领取、未参加（人数已满、时间已过）
    {
        CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCMenuItem *itemInfo = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                     spMenu2,
                                                                     this,
                                                                     menu_selector(PostReward::cbShowRewardInfo));
        itemInfo->setTag(i*10 + 2);
        itemInfo->setAnchorPoint(ccp(0, 0));
        
        TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("reward_btn_xiangqing"),
                                                    28);
        lbText->setAnchorPoint(ccp(0.5, 0.5));
        lbText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                        spMenu1->getContentSize().height * 0.5 + 2));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        itemInfo->addChild(lbText, 1);
        
        CCMenu *menu = CCMenu::menuWithItem(itemInfo);
        newMenuBtn->addChild(menu,100);
        menu->setPosition(ccp(124, -106));
    }
    else   // 未参加（可以参加）
    {
        CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        
        CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                     spMenu2,
                                                                     this,
                                                                     menu_selector(PostReward::cbJoinPostReward));
        itemShow->setTag(i*10+1);
        itemShow->setAnchorPoint(ccp(0, 0));
        
        char buf[100];
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_post_reward_canjia"));
        TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
        lbText->setAnchorPoint(ccp(0.5, 0.5));
        lbText->setPosition(CCPointMake(196,-64));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        newMenuBtn->addChild(lbText, 200);
        
        CCMenu *menu = CCMenu::menuWithItem(itemShow);
        newMenuBtn->addChild(menu,100);
        menu->setPosition(ccp(124, -106));
    }
    
//    CCMenu *menu = CCMenu::menuWithItem(itemShow);
//    newMenuBtn->addChild(menu);
//    menu->setPosition(ccp(124, -106));
}

void PostReward::cbShowRewardInfo(cocos2d::CCObject *pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_listSel = mItem->getTag() / 10;
    
    CPostRewardBaseInfo* rewardInfo = CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(m_listSel);
    m_ret.taskId = rewardInfo->name;
    m_ret.iBtnSel = en_rewardMemberList;
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void PostReward::cbRecieveReward(cocos2d::CCObject *pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_listSel = mItem->getTag() / 10;
    reqRecieveRewardTask();
}

void PostReward::reqRecieveRewardTask()
{
    CPostRewardBaseInfo* rewardInfo = CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(m_listSel);
    if (rewardInfo->status == 1)    // 还没完成，不能领奖励
        return;
    
    if (CGameData::Inst()->recieveRewardRequest(rewardInfo->name))
    {
        m_curReqType = en_reqRecieveReward;
        scheduleUpdate();
    }
}

void PostReward::cbJoinPostReward(cocos2d::CCObject *pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    m_listSel = mItem->getTag() / 10;
    showJoinPostRewardTips(CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(m_listSel)->ensureYuanBao);
}

void PostReward::showJoinPostRewardTips(long lCoin)
{
    if(m_dlgTip == NULL)
    {
        char buf[200];
        snprintf(buf, 199, CGameData::Inst()->getLanguageValue("cntry_xs_querenzhifu"), lCoin);
        
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(PostReward::cbJoinPostRewardTips),
                               CGameData::Inst()->getLanguageValue("sureTip"),
                               buf,
                               enDl2TwoBtn);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480 - this->getPosition().y));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void PostReward::reqAcceptRewardTask()
{
    if (CGameData::Inst()->acceptRewardTaskRequest(CGameData::Inst()->getRewardTaskList()->getObjectAtIndex(m_listSel)->name))
    {
        m_curReqType = en_reqAcceptReward;
        scheduleUpdate();
    }
}

void PostReward::cbJoinPostRewardTips(CCObject *pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    if(pRet->bOk)
        reqAcceptRewardTask();
    
    if (m_dlgTip)
    {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
}







