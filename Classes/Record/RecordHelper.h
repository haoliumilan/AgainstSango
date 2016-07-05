//
//  RecordHelper.h
//  AgainstWar
//
//  Created by caojiye on 13-7-3.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RecordHelper__
#define __AgainstWar__RecordHelper__

#include "cocos2d.h"
#include "RecordData.h"

using namespace cocos2d;
////////////////////////////////////////////////////
struct RecordConfig
{
    bool    record_state;
};
////////////////////////////////////////////////////
class RecordHelper : public CCObject
{
    static RecordHelper* inst_recorder_helper;
    
    unsigned long m_record_time0;
    
    bool          m_is_recording;
    
    bool          m_is_pause_record;
    
    ConfigData    m_config_data;
    
    short         m_max_video_number;
    
    bool          m_record_switch;
    
    int           m_floor_index;
    
public:
    static RecordHelper* Inst();
    
    void InitNewGameUid();
    
    bool GetRecordSwitch();
    void SetRecordSwitch(bool _b);
    
    ConfigData* GetCurrentVideoAtIndex(int _index);
    ConfigData* GetPreviousVideoAtIndex(int _index);
    void RemovePreviousVideoBehindIndex(int _index);
    
    float GetUploadProgress();
    
    void SetRecording();
    bool CheckRecording();
    bool CheckRecording(bool is_recording);
    
    void SetUploading(unsigned long time0);
    void SetUploading(unsigned long time0, bool uploading);
    void UploadLastReplay();
    void SetUploadSuccess(unsigned long time0);
    int GetUploadState(unsigned long time0);
    bool CheckIsUploading();
    void CheckPassReview();
    
    bool CheckUploadedUpto10();
    
    void SetPauseRecord(bool state);
    bool CheckIsPausing();
    
    void SetFightScene(int floor_id, int room_id, const char* fight_scene, int floor_index);
    bool EndFighting();
    void SaveLastCurrentReplay(ConfigData data);
    void SavePreviousReplay(ConfigData data);
    
    void SaveToAlbum();
    
    bool CheckRecordExist();
    
    int GetMaxVideoNumber();
    
    // 服务器
    void GetUploadAward(ConfigData data);
    string GetUploadedAward();
    bool ReqOtherVideoRecord(const char* dungeon_id, const char* type);
    string GetOtherRecordUrlVid();
    
    
    // 分享视频
    void ShareVideo(const char* url);
    
    void ViewRecord(const char* url);
    
private:
    
    RecordHelper();
    
    void InitMaxVideoNumber();
    
    void SaveLastReplayData(ConfigData data, int _type);
    
    // local data
    void SaveRecordConfig(RecordConfig conf);
    RecordConfig GetRecordConfig();
    
};
////////////////////////////////////////////////////



#endif /* defined(__AgainstWar__RecordHelper__) */
