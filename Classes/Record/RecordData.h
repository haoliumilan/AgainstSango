//
//  RecordData.h
//  AgainstWar
//
//  Created by caojiye on 13-7-24.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_RecordData_h
#define AgainstWar_RecordData_h

/////////////////////////////////////////////////////////////////
struct PreviousVideo
{
    bool             valid;
    unsigned long    time0;
    char             fight_name[32];
    char             video_url[64];
};
/////////////////////////////////////////////////////////////////
enum enUploadState
{
    en_upload_state_unuploaded = 1,    // 保存在本地
    en_upload_state_reviewing,         // 保存在本地
    en_upload_state_reviewed,          // 保存在本地
    en_upload_state_ok,
    en_upload_state_uploading,         // 保存在 inst 里
    en_upload_state_upload_fail,       // 保存在 inst 里
};
/////////////////////////////////////////////////////////////////
enum enLocalType
{
    en_local_type_current = 1,
    en_local_type_previous,
};
/////////////////////////////////////////////////////////////////
struct ConfigData
{
    short            max_combo;
    float            average_combo;
    unsigned long    max_damage;
    short            total_rounds;     // 本场回合数
    short            group_general;    // 携带队伍总统帅值
    unsigned long    group_attack;     // 携带队伍攻击力
    unsigned long    group_hp;         // 携带队伍HP
    short            skill_rounds;     // 使用技能次数
    short            rank;             // level ?
    char             fight_id[16];
    char             fight_type[16];
    char             fight_name[32];
    short            room_id;
    short            floor_id;
    bool             has_uploaded;     // deprecated
    bool             has_get_award;    // deprecated
    bool             has_pass_review;  // deprecated
    bool             has_shared;       // deprecated
    char             video_url[64];
    char             img_url[128];
    char             vid[16];
    unsigned long    time0;
    char             game_id[16];
    short            upload_state;
};
/////////////////////////////////////////////////////////////////


#endif
