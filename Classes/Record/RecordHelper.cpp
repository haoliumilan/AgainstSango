//
//  RecordHelper.cpp
//  AgainstWar
//
//  Created by caojiye on 13-7-3.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
////////////////////////////////////////////////
#include "RecordHelper.h"
#include "OcProxy.h"
#include "SaveData.h"
#include "OrgSaveData.h"
#include "CGameData.h"
////////////////////////////////////////////////
RecordHelper* RecordHelper::inst_recorder_helper = NULL;
////////////////////////////////////////////////
RecordHelper* RecordHelper::Inst()
{
    if(inst_recorder_helper == NULL)
    {
        inst_recorder_helper = new RecordHelper();
    }
    return inst_recorder_helper;
}

RecordHelper::RecordHelper()
{
    m_record_time0 = 0;
    m_is_recording = false;
    m_is_pause_record = false;
    m_floor_index = 0;
    RecordConfig conf = GetRecordConfig();
    m_record_switch = conf.record_state;
    InitMaxVideoNumber();
}

void RecordHelper::InitMaxVideoNumber()
{
    m_max_video_number = CGameData::Inst()->getCommonInfo()->max_video_num;
}

void RecordHelper::InitNewGameUid()  // this is public
{
    const char* game_uid = CGameData::Inst()->getUsrInfo()->uid;
    OcProxy::Inst()->InitWithGameUid(game_uid);
}

bool RecordHelper::GetRecordSwitch()
{
    return m_record_switch;
}

void RecordHelper::SetRecordSwitch(bool _b)
{
    m_record_switch = _b;
    RecordConfig conf;
    conf.record_state = _b;
    SaveRecordConfig(conf);
}

ConfigData* RecordHelper::GetCurrentVideoAtIndex(int _index)
{
    ConfigData* rt = OcProxy::Inst()->GetVideoAtIndex(_index, en_local_type_current);
    return rt;
}

ConfigData* RecordHelper::GetPreviousVideoAtIndex(int _index)
{
    ConfigData* rt = OcProxy::Inst()->GetVideoAtIndex(_index, en_local_type_previous);
    return rt;
}

void RecordHelper::RemovePreviousVideoBehindIndex(int _index)
{
    CCLog("index = %d", _index);
    OcProxy::Inst()->RemovePreviousVideoBehindIndex(_index);
}

float RecordHelper::GetUploadProgress()
{
    float rt = OcProxy::Inst()->GetUploadVideoProgress();
    return rt;
}

void RecordHelper::SetRecording()
{
    //m_record_time0 = time(0);
    
    m_record_time0 = (long)CGameData::Inst()->getCurrentTime();
    
    char time0[16];
    snprintf(time0, 16, "%ld", m_record_time0);
    OcProxy::Inst()->SetPathRecording(time0);
    m_is_recording = true;
}

bool RecordHelper::CheckRecording()
{
    return m_is_recording;
}

bool RecordHelper::CheckRecording(bool is_recording)
{
    bool rt = m_is_recording;
    m_is_recording = is_recording;
    return rt;
}

void RecordHelper::SetUploading(unsigned long time0)
{
    SetUploading(time0, true);
}

void RecordHelper::SetUploading(unsigned long time0, bool uploading)
{
    if (RecordHelper::Inst()->CheckIsUploading() && uploading)
        return;
    char buf[16];
    snprintf(buf, 16, "%lu", time0);
    OcProxy::Inst()->SetUploading(buf, uploading);
}

void RecordHelper::UploadLastReplay()
{
    SetUploading(m_config_data.time0, true);
}

void RecordHelper::SetUploadSuccess(unsigned long time0)
{
    char buf[16];
    snprintf(buf, 16, "%lu", time0);
    OcProxy::Inst()->SetUploadSuccess(buf);
}

int RecordHelper::GetUploadState(unsigned long time0)
{
    char buf[16];
    snprintf(buf, 16, "%lu", time0);
    int rt = OcProxy::Inst()->GetUploadState(buf);
    return rt;
}

bool RecordHelper::CheckIsUploading()
{
    return OcProxy::Inst()->CheckUploading();
}

void RecordHelper::CheckPassReview()
{
    OcProxy::Inst()->RequestVideoInfo();
}

bool RecordHelper::CheckUploadedUpto10()
{
    return OcProxy::Inst()->CheckUploadedUpto10();
}

void RecordHelper::SetPauseRecord(bool state)
{
    m_is_pause_record = state;
    
    if (state)
    {
        OcProxy::Inst()->pauseRecordScreen();
    }
    else
    {
        OcProxy::Inst()->resumeRecordScreen();
    }
}

bool RecordHelper::CheckIsPausing()
{
    return m_is_pause_record;
}

void RecordHelper::SetFightScene(int floor_id, int room_id, const char* fight_scene, int floor_index)
{
    snprintf(m_config_data.fight_id, 16, "%d_%d", floor_id, room_id);
    snprintf(m_config_data.fight_type, 16, "%s", fight_scene);
    m_config_data.room_id = room_id;
    m_config_data.floor_id = floor_id;
    m_floor_index = floor_index;
}

bool RecordHelper::EndFighting()
{
    if (m_is_recording)
    {
        SaveData* save_data = SaveData::Inst();
        
        m_config_data.max_combo = save_data->getMaxCombo();
        m_config_data.average_combo = save_data->getAverageCombo();
        m_config_data.max_damage = save_data->getMaxDamage();
        m_config_data.total_rounds = save_data->getTouchCnt();
        m_config_data.group_general = CGameData::Inst()->getCurTeamCost();
        m_config_data.group_attack = save_data->GetGroupAttack();
        m_config_data.group_hp = save_data->getMaxHp();
        m_config_data.skill_rounds = save_data->getUseSkillCnt();
        m_config_data.rank = CGameData::Inst()->getUsrInfo()->lv;
        m_config_data.has_uploaded = 0;
        m_config_data.has_get_award = 0;
        m_config_data.has_pass_review = 0;
        m_config_data.has_shared = 0;
        m_config_data.time0 = m_record_time0;
        m_config_data.upload_state = en_upload_state_unuploaded;
        
        snprintf(m_config_data.game_id, 16, "%s", CGameData::Inst()->getUsrInfo()->uid);
        snprintf(m_config_data.vid, 16, "123");
        snprintf(m_config_data.img_url, 128, "image");
        snprintf(m_config_data.video_url, 64, "video");
        string fight_name = CGameData::Inst()->GetFightName(m_config_data.floor_id, m_config_data.room_id, m_config_data.fight_type, m_floor_index);
        snprintf(m_config_data.fight_name, 32, "%s", fight_name.c_str());

        SaveLastCurrentReplay(m_config_data);
    }
    return m_is_recording;
}

void RecordHelper::SaveLastCurrentReplay(ConfigData data)
{
    SaveLastReplayData(data, en_local_type_current);
}

void RecordHelper::SavePreviousReplay(ConfigData data)
{
    SaveLastReplayData(data, en_local_type_previous);
}

void RecordHelper::SaveLastReplayData(ConfigData data, int _type)
{
    OcProxy::Inst()->SaveLastReplayData(data, _type);
}

void RecordHelper::SaveToAlbum()
{
    OcProxy::Inst()->SaveToAlbum();
}

bool RecordHelper::CheckRecordExist()
{
    bool rt = OcProxy::Inst()->CheckRecordExist();
    return rt;
}

int RecordHelper::GetMaxVideoNumber()
{
    return m_max_video_number;
}

// ------------------  服务器 ------------------------
void RecordHelper::GetUploadAward(ConfigData data)
{
    CGameData::Inst()->GetUploadAward(data);
}

string RecordHelper::GetUploadedAward()
{
    string rt = CGameData::Inst()->GetUploadedAward();
    return rt;
}

void RecordHelper::ViewRecord(const char* url)
{
    OcProxy::Inst()->OpenUrl(url);
}

bool RecordHelper::ReqOtherVideoRecord(const char* dungeon_id, const char* type)
{
    bool rt = CGameData::Inst()->ReqOtherRecord(dungeon_id, type);
    return rt;
}

string RecordHelper::GetOtherRecordUrlVid()
{
    string rt = CGameData::Inst()->GetOtherRecordUrlVid();
    return rt;
}

// ---------------------------  分享视频 --------------------------
void RecordHelper::ShareVideo(const char* url)
{
    CCLog("share video");
       
    string content = CGameData::Inst()->getLanguageValue("record_share_content");
    long time0 = time(0);
    string time = OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(time0);
    content = content + time + url;

    OcProxy::Inst()->sendWeibo(content.c_str(), 0);
}
// --------------------------- local data -------------------------
void RecordHelper::SaveRecordConfig(RecordConfig conf)
{
    string path = OcProxy::Inst()->getDocumentPath();
    path  = path+"RecordPath";
    FILE* p_file = fopen(path.c_str(), "wb");
    fwrite(&conf, sizeof(conf), 1, p_file);
    fclose(p_file);
}

RecordConfig RecordHelper::GetRecordConfig()
{
    RecordConfig rt;
    rt.record_state = false;
    string path = OcProxy::Inst()->getDocumentPath();
    path  = path+"RecordPath";
    FILE* p_file = fopen(path.c_str(), "rb+");
    if (p_file)
        fread(&rt, sizeof(rt), 1, p_file);
    fclose(p_file);
    return rt;
}



