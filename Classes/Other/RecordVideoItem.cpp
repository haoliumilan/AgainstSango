//
//  RecordVideoItem.cpp
//  AgainstWar
//
//  Created by caojiye on 13-7-24.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

////////////////////////////////////////////////////////////
#include "RecordVideoItem.h"
#include "TextNode.h"
#include "CGameData.h"
#include "OcProxy.h"
#include "RecordHelper.h"
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
RecordVideoItem* RecordVideoItem::CreateWith(CCObject* target, SEL_CallFuncO selector, ConfigData data)
{
    RecordVideoItem* rt = new RecordVideoItem(target, selector, data);
    rt->autorelease();
    return rt;
}
////////////////////////////////////////////////////////////
RecordVideoItem::RecordVideoItem(CCObject* target, SEL_CallFuncO selector, ConfigData data)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_en_upload_state = data.upload_state;
    m_configdata = data;
    
    m_text_button = TextNode::textWithString("alloc", 36);
    m_text_upload_progress = TextNode::textWithString("alloc", 26);
    
    SetInitialValue();
    SetState(m_en_upload_state);
    if (m_en_upload_state == en_upload_state_uploading)
    {
        schedule(schedule_selector(RecordVideoItem::updateUploading));
    }
}

void RecordVideoItem::SetInitialValue()
{
    CCSprite* bkg = CCSprite::spriteWithFile("record_other_main.png");
    addChild(bkg);
    
    // title
    TextNode* title = TextNode::textWithString(m_configdata.fight_name, 26);
    bkg->addChild(title);
    title->setAnchorPoint(CCPointZero);
    title->setPosition(CCPointMake(50, 92));
    title->setColor(kTitleColor);
    
    // record time
    string time = OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(m_configdata.time0);
    char char_time[11];
    snprintf(char_time, 11, "%s", time.c_str());
    char buf[32];
    snprintf(buf, 32, "%s:%s", CGameData::Inst()->getLanguageValue("record_time"), char_time);
    TextNode* record_time = TextNode::textWithString(buf, 26);
    bkg->addChild(record_time);
    record_time->setAnchorPoint(CCPointZero);
    record_time->setPosition(CCPointMake(50, 55));
    record_time->setColor(ccBLACK);
    
    // upload
    char char_upload_progress[16];
    snprintf(char_upload_progress, 16, "%s:", CGameData::Inst()->getLanguageValue("record_upload_progress"));
    TextNode* text_upload = TextNode::textWithString(char_upload_progress, 26);
    bkg->addChild(text_upload);
    text_upload->setAnchorPoint(CCPointZero);
    text_upload->setPosition(CCPointMake(50, 20));
    text_upload->setColor(ccBLACK);
    
    // got award
    char char_got_award[16];
    snprintf(char_got_award, 16, "%s", CGameData::Inst()->getLanguageValue("record_get_award_done"));
    bkg->addChild(m_text_upload_progress);
    m_text_upload_progress->setColor(kTitleColor);
    m_text_upload_progress->setAnchorPoint(CCPointZero);
    m_text_upload_progress->setPosition(CCPointMake(170, 20));
    
    // button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem *menu_item = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(RecordVideoItem::cbButton));
    m_text_button->setColor(ccWHITE);
    m_text_button->setShadowColor(ccBLACK);
    m_text_button->setPosition(ccp(spMenu1->getContentSize().width/2, spMenu1->getContentSize().height/2));
    menu_item->addChild(m_text_button);
    menu_item->setPosition(CCPointMake(370, 32));
    CCMenu* menu = CCMenu::menuWithItem(menu_item);
    menu_item->setAnchorPoint(CCPointZero);
    bkg->addChild(menu);
    menu->setPosition(CCPointZero);
}

void RecordVideoItem::SetState(int state)
{
    m_en_upload_state = state;
    
    if (state == en_upload_state_ok)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_previous_view_record"));
        m_text_upload_progress->setString(CGameData::Inst()->getLanguageValue("record_get_award_done"));
        m_text_upload_progress->setColor(kTitleColor);
    }
    else if (state == en_upload_state_reviewed)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_get_award"));
        m_text_upload_progress->setString(CGameData::Inst()->getLanguageValue("record_upload_done"));
        m_text_upload_progress->setColor(ccGREEN);
    }
    else if (state == en_upload_state_reviewing)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_reupload"));
        m_text_button->getParent()->setScale(0);
        m_text_upload_progress->setString(CGameData::Inst()->getLanguageValue("record_upload_reviewing"));
        m_text_upload_progress->setColor(kTitleColor);
    }
    else if (state == en_upload_state_unuploaded)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_upload_now"));
        m_text_upload_progress->setString(CGameData::Inst()->getLanguageValue("record_upload_no"));
        m_text_upload_progress->setColor(kTitleColor);
    }
    else if (state == en_upload_state_uploading)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_upload_ing"));
        m_text_upload_progress->setString("0%");
        m_text_upload_progress->setColor(kTitleColor);
    }
    else if (state == en_upload_state_upload_fail)
    {
        m_text_button->setString(CGameData::Inst()->getLanguageValue("record_reupload"));
        m_text_upload_progress->setString(CGameData::Inst()->getLanguageValue("record_upload_failed"));
        m_text_upload_progress->setColor(kTitleColor);
    }
}

void RecordVideoItem::cbButton()
{
    int state = m_en_upload_state;
    if (state == en_upload_state_ok)
    {
        ViewRecord();
    }
    else if (state == en_upload_state_reviewed)
    {
        GetAward();
    }
    else if (state == en_upload_state_reviewing)
    {
        // 审核中 不做任何操作
    }
    else if (state == en_upload_state_unuploaded)
    {
        StartUploadVideo();
    }
    else if (state == en_upload_state_uploading)
    {
        CancelUploading();
    }
    else if (state == en_upload_state_upload_fail)
    {
        StartUploadVideo();
    }
}

void RecordVideoItem::StartUploadVideo()
{
    CCLog("start upload video");
    if (RecordHelper::Inst()->CheckIsUploading())
    {
        m_ret.rtValue = en_rtValue_uploading_wait;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else
    {
        RecordHelper::Inst()->SetUploading(m_configdata.time0);
        char name[16];
        snprintf(name, 16, "%lu", m_configdata.time0);
        
        SetState(en_upload_state_uploading);
        
        schedule(schedule_selector(RecordVideoItem::updateUploading));
    }
}

void RecordVideoItem::UploadSuccess()
{
    CCLog("upload success");
    SetState(en_upload_state_reviewing);
    RecordHelper::Inst()->SetUploadSuccess(m_configdata.time0);
}

void RecordVideoItem::UploadFail()
{
    CCLog("upload fail or cancel upload");
    SetState(en_upload_state_upload_fail);
    RecordHelper::Inst()->SetUploading(m_configdata.time0, false);
}

void RecordVideoItem::CancelUploading()
{
    unschedule(schedule_selector(RecordVideoItem::updateUploading));
    UploadFail();
}

void RecordVideoItem::GetAward()
{
    CCLog("getaward");
    RecordHelper::Inst()->ShareVideo(m_configdata.video_url);
    RecordHelper::Inst()->GetUploadAward(m_configdata);
    schedule(schedule_selector(RecordVideoItem::UpdateGetAward));
}

void RecordVideoItem::ViewRecord()
{
    RecordHelper::Inst()->ViewRecord(m_configdata.video_url);
}

// ------------------- update ------------------------
void RecordVideoItem::updateUploading(ccTime dt)
{
    static int fake_progress = 0;
    int progress = RecordHelper::Inst()->GetUploadProgress();
    if (progress <= 100 && progress >= 0)
    {
        if (progress < 10)
        {
            fake_progress = progress;
        }
        
        if (fake_progress < progress)
        {
            fake_progress += 2;
            if (fake_progress >= 100)
            {
                fake_progress = 100;
                unschedule(schedule_selector(RecordVideoItem::updateUploading));
                fake_progress = 0;
                UploadSuccess();
                return;
            }
        }
        else if (progress==10 && fake_progress<90)  // 假数据
        {
            int ran = arc4random()%100;
            if (ran > 97)
            {
                fake_progress++;
            }
        }
        char buf[8];
        snprintf(buf, 8, "%d%%", fake_progress);
        m_text_upload_progress->setString(buf);
    }
    else // if (progress < 0)
    {
        fake_progress = 0;
        unschedule(schedule_selector(RecordVideoItem::updateUploading));
        UploadFail();
    }
}

void RecordVideoItem::UpdateGetAward(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting)
    {
        m_ret.rtValue = en_rtValue_show_commdlg;
        (m_pListener->*m_pfnSelector)(&m_ret);
        return;
    }
    
    m_ret.rtValue = en_rtValue_remove_commdlg;
    (m_pListener->*m_pfnSelector)(&m_ret);
    
    unschedule(schedule_selector(RecordVideoItem::UpdateGetAward));
    
    if(comState == kReqStatOk)
    {
        string award = RecordHelper::Inst()->GetUploadedAward();
        
        // 放视频到 pre_record 里
        RecordHelper::Inst()->SavePreviousReplay(m_configdata);
        
        // 弹出领取奖励的提示框
        m_ret.rtValue = en_rtValue_show_got_award;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else {
        m_ret.m_pListener = this;
        m_ret.m_pfnSelector = callfuncO_selector(RecordVideoItem::GetAward);
        m_ret.rtValue = en_rtValue_retry_commdlg;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
}











