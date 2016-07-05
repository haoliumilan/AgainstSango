//
//  MyVideoLayer.cpp
//  AgainstWar
//
//  Created by caojiye on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
#include "MyVideoLayer.h"
#include "RecordHelper.h"
#include "Dialog2.h"
#include "CommDlg.h"
#include "CGameData.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "RecordVideoItem.h"
#include "MainScene.h"

//////////////////////////////////////////////////////////////////////////////
MyVideoLayer* MyVideoLayer::Create(CCObject* target, SEL_CallFuncO selector)
{
    MyVideoLayer* rt = new MyVideoLayer(target, selector);
    return rt;
}

MyVideoLayer::MyVideoLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_list_layer = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    m_item_pListener = NULL;
    m_item_pfnSelector = NULL;
    m_dialog2 = NULL;
}

void MyVideoLayer::Refresh()
{
    removeAllChildrenWithCleanup(true);
    CCSprite* spLogo = CCSprite::spriteWithFile("appplay.png");
    spLogo->setPosition(CCPointMake(320, 180));
    addChild(spLogo, 1);
    
    if (RecordHelper::Inst()->CheckRecordExist())
    {
        RecordHelper::Inst()->CheckPassReview();
        SetList();
    }
    else
    {
        m_dialog2 = new Dialog2(this, callfuncO_selector(MyVideoLayer::cbNoLocalRecord), CGameData::Inst()->getLanguageValue("cntry_hint"), CGameData::Inst()->getLanguageValue("record_local_none"), enDl2OneBtn);
        addChild(m_dialog2, 500);
        m_dialog2->setPosition(CCPointMake(320, 480));
        m_dialog2->setScale(0.0);
        m_dialog2->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        m_dialog2->release();
    }
}

void MyVideoLayer::RefreshStrategy()
{
    removeAllChildrenWithCleanup(true);
    CCSprite* spLogo = CCSprite::spriteWithFile("appplay.png");
    spLogo->setPosition(CCPointMake(320, 180));
    addChild(spLogo, 1);
    
    //...
    
    m_dialog2 = new Dialog2(this, callfuncO_selector(MyVideoLayer::cbNoLocalRecord), CGameData::Inst()->getLanguageValue("cntry_hint"), CGameData::Inst()->getLanguageValue("ont_video_strategy"), enDl2OneBtn);
    addChild(m_dialog2, 500);
    m_dialog2->setPosition(CCPointMake(320, 480));
    m_dialog2->setScale(0.0);
    m_dialog2->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    m_dialog2->release();
}

void MyVideoLayer::SetList()
{
    m_list_layer = new CListLayer(this, callfuncO_selector(MyVideoLayer::cbList), 3.8, 150, 654);
    addChild(m_list_layer);
    m_list_layer->release();
    m_list_layer->setPosition(CCPointMake(530, 0));
    m_list_layer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)), 0.8f));
    
    int list_item = 0;
    int max_list_item = RecordHelper::Inst()->GetMaxVideoNumber();
    int i = 0;
    while (1)
    {
        ConfigData* data = RecordHelper::Inst()->GetCurrentVideoAtIndex(i);
        if (data == NULL)
            break;
        RecordVideoItem* item = RecordVideoItem::CreateWith(this, callfuncO_selector(MyVideoLayer::cbListItem), *data);
        m_list_layer->push_back(item);
        i++;
        list_item++;
    }
    
    i = 0;
    while (1)
    {
        ConfigData* data = RecordHelper::Inst()->GetPreviousVideoAtIndex(i);
        if (data == NULL)
            break;
        RecordVideoItem* item = RecordVideoItem::CreateWith(this, callfuncO_selector(MyVideoLayer::cbListItem), *data);
        m_list_layer->push_back(item);
        i++;
        list_item++;
        if (list_item >= max_list_item)
        {
            RecordHelper::Inst()->RemovePreviousVideoBehindIndex(i);
            break;
        }
    }
    
    // 视频的数量
    CCSprite* frNumBg = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    addChild(frNumBg, 1);
    frNumBg->setPosition(CCPointMake(535, 167));
    
    char buf[8];
    sprintf(buf, "%d/%d", list_item, max_list_item);
    TextNode* frInfoLabel = TextNode::textWithString(buf, 24);
    frInfoLabel->setPosition(ccp(120, 18));
    frNumBg->addChild(frInfoLabel);
}

void MyVideoLayer::cbListItem(CCObject* obj)
{
    RecordVideoItemRet* ret = (RecordVideoItemRet*)obj;
    int value = ret->rtValue;
    if (value == en_rtValue_none)
    {
        
    }
    else if (value == en_rtValue_uploading_wait)
    {
        m_dialog2 = new Dialog2(this, callfuncO_selector(MyVideoLayer::cbRemoveDialog2),
                                CGameData::Inst()->getLanguageValue("cntry_hint"),
                                CGameData::Inst()->getLanguageValue("record_text_uploading_failed"),
                                enDl2OneBtn);
        addChild(m_dialog2, 500);
        m_dialog2->setPosition(CCPointMake(320, 480));
        m_dialog2->setScale(0.0);
        m_dialog2->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        m_dialog2->release();
    }
    else if (value == en_rtValue_show_commdlg)
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MyVideoLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
    }
    else if (value == en_rtValue_remove_commdlg)
    {
        if(m_commDlg != NULL)
        {
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
            
            if(CGameData::Inst()->isNewbie() == false)
                MainScene::Inst()->enableBottomPanel(true);
        }
    }
    else if (value == en_rtValue_retry_commdlg)
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MyVideoLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            m_item_pListener = ret->m_pListener;
            m_item_pfnSelector = ret->m_pfnSelector;
        }
    }
    else if (value == en_rtValue_show_got_award)
    {
        string award = RecordHelper::Inst()->GetUploadedAward();
        
        m_dialog2 = new Dialog2(this, callfuncO_selector(MyVideoLayer::cbRemoveDialog2AndRefresh), CGameData::Inst()->getLanguageValue("cntry_hint"), award.c_str(), enDl2OneBtn);
        addChild(m_dialog2, 500);
        m_dialog2->setPosition(CCPointMake(320, 480));
        m_dialog2->setScale(0.0);
        m_dialog2->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        m_dialog2->release();
    }
}

void MyVideoLayer::cbList(CCObject* obj)
{
    // 点击第几个list  这个cpp里什么都不用做
    
    CListLayerRet* pRet = (CListLayerRet*)obj;
    int list = pRet->iBtnSel;
    CCLog("UploadVideo.list  = %d", list);
}



void MyVideoLayer::cbNoLocalRecord()
{
    m_dialog2->removeFromParentAndCleanup(true);
    m_dialog2 = NULL;
    (m_pListener->*m_pfnSelector)(NULL);
}

void MyVideoLayer::cbRemoveDialog2()
{
    m_dialog2->removeFromParentAndCleanup(true);
    m_dialog2 = NULL;
}

void MyVideoLayer::cbRemoveDialog2AndRefresh()
{
    Refresh();   // 包含了 remove dlg2
}

void MyVideoLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true)
    {
        (m_item_pListener->*m_item_pfnSelector)(NULL);
    }
    else
    {
        CGameData::Inst()->clearReqStat();
    }
    
    MainScene::Inst()->enableBottomPanel(true);
}


