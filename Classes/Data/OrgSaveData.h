//
//  OrgSaveData.h
//  AgainstWar
//
//  Created by Liu Hao on 13-2-19.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgSaveData__
#define __AgainstWar__OrgSaveData__

#include <iostream>
#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef struct{
    short   cid;
    long    hp;
    long    attack;
    short   element;
        
    char    card_name[32];
}OrgSaveData_Card;

typedef struct{
    char    fight_type[32];
    char    fight_id[32];
    char    fight_name[32];
    short   enemy_cid;
    short   combo_index;
    short   maxCombo;
    
    bool     m_bisTMLS;
    long    cur_gold;
        
    short   stamina_recover_time;
    short   acPoint_recover_time;
    
    char    stonegrid[5][6];
    
    OrgSaveData_Card card_info[5];
    
    char    md5_str[64];    // 保存应用的md5
    
    char    session_id[64];      // 保存sessionId
    
    char    user_id[64];    // pid
    
    char    external_list[300];    // 外挂列表
    
    char                bUseChristmasUI;    // 是否适用圣诞UI
    
    unsigned char       m_cRandLst[kRandLstLen];
    unsigned char       m_cRandIdx;
    
}SaveData_OrgFightInf;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgSaveData {
    static  OrgSaveData*  s_pcIf;
    
    SaveData_OrgFightInf  m_OrgFightInf;
    
    char                m_filePath[1000];
    char                m_tempFilePath[1000];
    
    bool                m_bValid;   // 是否保存备份文件
    bool                m_bUseSaveData; //是否需要适用备份数据
    
  

    
public:
	//Get the instance pointer
	static OrgSaveData* Inst(){
        if(s_pcIf == NULL){
            s_pcIf = new OrgSaveData();
            s_pcIf->init();
        }
        
        return s_pcIf;
    }
    
    //init
    void init();
    
    //delete file
    void destorySavedFile();
    //clear memory
    void destorySavedData();
    
    //save to file
    void flush();
    
    //有否有效的保存的数据
    bool hasValidSavedData();
    
    // 是否使用备份数据
    bool getIsUseSaveData();
    void setIsUseSaveData(bool isUseSaveData);
    
    //****************************************
    //orgFight info process
    //****************************************
    void initOrgFightInfo();
    
    void setStone(int iRow, int iColumn, char elem);
    char getStone(int iRow, int iColumn);
    
    short getComboIndex();
    void setComboIndex(short comboIndex);
    
    short getMaxCombo();
    void setMaxCombo(short comboCount);
    
    void setIsTongMengLianShouChang(bool bisTMLS);
    bool getIsTongMengLianShouChang();
    
    
    const char* getFightId();
    void setFightId(const char* fightId);

    const char* getFightType();
    void setFightType(const char* fightType);

    const char* getFightName();
    void setFightName(const char* fightName);
    
    short getEnemyCid();
    void setEnemyCid(short enemyCid);
    
    OrgSaveData_Card* getCardInfo(int idx);
    
    const char* getMD5Str();
    
    const char* getSessionId();
    
    const char* getUserId();
    
    short getStaminaRecoverTime();
    
    short getAcPointRecoverTime();
    
    void checkIsUseExternal();
    
    bool getIsUseChristmasUI();
    
    unsigned char getrand();
    
private:
    //forbid constructor being called outside
	OrgSaveData():m_bValid(false), m_bUseSaveData(false){};
    
	//forbid copy constructor being called outside
	OrgSaveData(const OrgSaveData& );
	
	//forbid assignment
	OrgSaveData& operator=(const OrgSaveData& );
    
};



#endif /* defined(__AgainstWar__OrgSaveData__) */
