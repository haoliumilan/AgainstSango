//
//  SaveData.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-10-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_SaveData_h
#define AgainstWar_SaveData_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "CommonDef.h"

using namespace cocos2d;


typedef struct
{
    long minMubiaoGX;
   // long MaxMubiaoGX;
    
    int ideadLine;
    int iPerson;
    int m_iBaozhengCoin;
    
} str_mubiaoGx;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef struct{
    short   cid;
    long    base_hp;
    long    cur_hp;
    long    drop_exp;
    long    base_armor;
    long    cur_armor;
    long    drop_gold;
    long    attack;
    short   state;
    short   drop_star;
    char    drop_plus;
    short   element;
    short   element_default;
    short   shSecondElement;
    short   round_max;
    short   cur_round;
//    short   skill_id;
//    short   buff_id;
    short   buffArr[ENEMYBUFFMAX];
    
    short   skidArr[ENEMYSKILLMAX];
    short   skill_occurTimes[ENEMYSKILLMAX];
    short   runCount[ENEMYSKILLMAX];
    
    short   buffId[MAX_BUFF_ITEMS];
    short   buffCd[MAX_BUFF_ITEMS];
    short   buffShow[MAX_BUFF_ITEMS];
    long    enemyHurt; //新增
    int     iSkillUseLastID;
    
}SaveData_Enemy;

typedef struct{
    long            enemycnt;
    long            goldbox;
    
    SaveData_Enemy  enemies[6];
}SaveData_EnemyGrp;

typedef struct{
    char                dungeon_uid[32];
    short               fightScene_type;
    long                stepcnt;
    char                dgtype[32];
    int                 aiming_index;    // 有哪个怪被瞄准，kInvalidValue表示没有
    
    char                poison_cd;      // 没中毒是0，中毒是1
    int                 poison_point;   // 毒的伤害值
//    char                delay_cd;       // 没延时是0，延时是1
    int                 sick_point;     // 瘟疫伤害值
    
    SaveData_EnemyGrp   enemygrps[50];
    
    unsigned char       m_cRandLst[kRandLstLen];
    unsigned char       m_cRandIdx;
    
    unsigned char       m_cDgSkillType;      // 技术战场类型，1: 千里走单骑，2：全队伍限定
    
    int                 m_iMoment;      // 0: 回合开始  1:触摸结束
    
    str_ReviveMinusCoin revive_minus_coin_arr[3];   // 复活选项
    short               is_select_revive;//0:战斗失败延续以前的6元宝复活方式 1:战斗失败给出三个复活选项
    
    int                 is_iFreeRevive;  //0: 没有使用免费复活  1:已经使用免费复活
}SaveData_StepInf;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef struct{
    short   cid;
    unsigned char   star;
    long    hp;
    long    recover;
    long    attack;
    short   element;
    short   sub_element;
    short   category;
    short   sub_category;
    char    isEluTgt;
    char    isWkTgt;
    int     plus;
    long    lv;
    long    lv_max;
    
    short   m_couPorperty;//国家属性
    
    short   man_skill_id;
    short   auto_skill_id;
    long    skill_cd_max;
    long    cur_skill_cd;
    
    char    card_name[32];
    
    int     state;
    short   FengYinCD;
    
    int     AwakeSkillArr[MaxAwakeSkillCnt];
    int     AwakeNum;
    
}SaveData_Card;

typedef struct{
    char    helper_uid[32];
    char    helper_type[32];
    char    helper_name[32];
    char    helper_country;
    short   helper_lv;
    short   gacha_pt_add;   // 对应援军可以可以增加的援军点数
    
}SaveData_Helper;

typedef struct{
    short   cur_lv;
    short   cur_cost;
    short   cur_max_statina;
    short   floor_id;
    char    floor_name[32];
    short   room_id;
    char    room_name[32];
    short   cur_step;
    short   cur_itemcnt;
    long    cur_hp;
    long    base_hp;
    long    cur_dggold;
    
    SaveData_Helper helper_info[kHelperMaxCount];
    
    short   gacha_pt_all;   // 当前拥有的援军点数
    
    short   stamina_recover_time;
    short   acPoint_recover_time;
    short   revive_coin;
    long    cur_coin;
    
    char    stone_count;
    char    stone_element[enElementMax];
    char    stonegrid[5][6];
    char    stonegridplus[5][6];
    char    stonegridblackcover[5][6];
    int     stonegridparam1[5][6];
    int     stonegridparam2[5][6];
    
    short   wudi_cd;
    short   wudi_element;   // 无敌针对的属性
    char    wudi_type;      // 无敌类型
    
    short   sheild_cd;
    float   armor_effect;    // 加防的效果
    
    short   recover_cd;     // 如沐春风
    float   recover_effect;
    
    short   recoverWuJiang_cd;  //武将类型的回复力改变
    float   recoverWuJiang_effect;
    
    
    // 反击
    short   fanji_cd;
    float   fanji_effect;
    short   fanji_element;
    char    fanji_miansi;
    
    short   aoe_cd;         // aoe
    
    SaveData_Card card_info[30];

    long    revive_time;    // 记录发送复活的时间戳，用于复活接口的参数
 //   long    l_requestGacha_time; //记录求神将（一抽，十连抽等）时的时间戳，用于请求扣除元宝的参数
    
    char    bUseChristmasUI;    // 是否适用圣诞UI
    char    bUseChristmasFightScene;    // 是否使用圣诞战斗场景
    
    short   fightScene_change;  // 战斗场景的大类型
    
    char    md5_str[64];    // 保存应用的md5
    
    char    session_id[64];      // 保存sessionId
    
    char    user_id[64];        // fid

    char    cur_floor_status;   // 当前关卡状态，用于判断首次通关
    
    short   dungeon_clear_coin; // 初次通关送的元宝数量
    
    short   beCanFei;
    
    char    external_list[300];    // 外挂列表
    
    unsigned long   ulTouchCnt;     //战斗转珠次数
    unsigned long   ulCurTouchCnt;  //当前局战斗转珠次数
    unsigned long   ulMaxCombo;     //最大combo数
    unsigned long   ulTotalCombo;   //combo总数
    unsigned long   ulMaxDamage;    //单次转珠的最大伤害
    unsigned long   ulTotalDamage;  //输出的总伤害
    unsigned long   ulCurDamage;    //当前伤害值
    unsigned long   ulUseSkillCnt;  //本场使用技能次数
    
    
    
    int         iEnhanceCD;
    int         iEnhanceElem;
    float       fEnhanceFactor;
    
    int         iEnWujiangCD;
    int         iEnWujiangType;       //用于武将类型攻击增加：主将技
    float       fEnWujiangFactor;
    
    int         iCountryWujiangCD;
    int         iCountryWujiangType;
    float       fCountryWujiangFactor;
    
    
    bool    isReplaced;      //主将切换过
    int     position;        //切换后主将的位置(再次使用位置换回)
    
    bool    isManSkill_129;

}SaveData_PlayerInf;



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class SaveData {
    static  SaveData*  s_pcIf;

    SaveData_StepInf    m_StepInf;
    SaveData_PlayerInf  m_PlayerInf;
    
    char                m_filePath[1000];
    char                m_tempFilePath[1000];
    
    bool                m_bValid;   // 是否保存备份文件
    bool                m_bUseSaveData; //是否需要适用备份数据
    
    
public:
	//Get the instance pointer
	static SaveData* Inst(){
        if(s_pcIf == NULL){
            s_pcIf = new SaveData();
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
    //enemy info process
    //****************************************
    void initStepInfo(unsigned char cSkillType);
    
    void setEnemyData(SaveData_Enemy* pEnemyInf, int iStep, int idx);
    
    void setEnemySkillOccTimes(short occTimes, short skillId,int iStep, int idx);
    void setEnemyRunCount(short occTimes, short skillId,int iStep, int idx);
    void setEnemyElement(int elem, int iStep, int idx);
    void setEnemyCurState(int curState, int iStep, int idx);
    void setEnemyCurHp(int curHp, int iStep, int idx);
    void setEnemyCurRound(int curRound, int iStep, int idx);
    void setEnemyCurArmor(long curArmor, int iStep, int idx);
    void setEnemyLastSkillUse(int skillId, int iStep, int idx);
//    void setEnemyMabiCd(short mabiCd, int iStep, int idx);
//    void setEnemyArmorBreakCd(short armorBreakCd, int iStep, int idx);
//    void setEnemySickCd(short sickCd, int iStep, int idx);
    
    void setEnemyBeAttrack(int beAtkNum,int iStep,int idx); //新增  敌将当前回合受到的总伤害
    
    void setEnemyBuffbuffId(int index, short buffId, int iStep,int idx);
    void setEnemyBuffbuffCd(int index, short buffCd, int iStep,int idx);
    void setEnemyBuffbuffShow(int index, short buffShow, int iStep,int idx);
    
    SaveData_EnemyGrp* getEnemyGroupData(int iStep);
    int  getStepCountMax();
    
    void setDungeonUid(const char* dungeonUid);
    const char* getDungeonUid();
    
    void setFightSceneType(short fightSceneType);
    short getFightSceneType();
    
    void setDgType(const char* dgType);
    const char* getDgType();
    
    void setAimingIndex(int index);
    int  getAimingIndex();
    
    void setPoisonCd(bool isPoisoned);
    bool getPoisonCd();
    
    void setPoisonPoint(int poisonPoint);
    int getPoisonPoint();
    
    void setSickPoint(int sickPoint);
    int getSickPoint();
    
//    void setDelayCd(bool isDelay);
//    bool getDelayCd();
    
    //****************************************
    //player info process
    //****************************************
    void initPlayerInfo(bool cntryBuff=true);

    bool getIsUseInviteCode();
    
    bool getIsUseChristmasUI();
    
    bool isReplace();
    
    int getPosition();
    
    void setReplace(bool isReplace);
    
    bool isManSkill_129();      //
    
    void setManSkill_129(bool isManSkill_129);     //
    
    void setPosition(int position);
    
    int getHelperIndexForAddFriend();
    SaveData_Helper* getHelperInfo(int index);
    SaveData_Helper* getHelperInfoForAddFriend();
    SaveData_Card*  getHelperCardForAddFried();
    int getAllGachaPoint();
        
    short getGachaPtAll();

    short getStaminaRecoverTime();
    
    short getAcPointRecoverTime();
    
    short getReviveCoin();
    
    short getWudiCd();
    void setWudiCd(short wudiCd);
    
    short getWudiElement();
    void setWudiElement(short wudiElement);
    
    char getWudiType();
    void setWudiType(char wudiType);
    
    bool getIsFanjiMiansi();
    void setIsFanjiMiansi(bool bMiansi);
    
    short getFanjiCd();
    void setFanjiCd(short fanjiCd);
    
    short getFanjiElement();
    void setFanjiElement(short fanjiElement);
    
    float getFanjiEffect();
    void setFanjiEffect(float fanjiEffect);
    
    short getAOECd();
    void setAOECd(short aoeCd);
    
    short getSheildCd();
    void setSheildCd(short sheildCd);
    
    float getArmorEffect();
    void setArmorEffect(float armorEffect);
    
    short getRecoverCd();
    void setRecoverCd(short recoverCd);
    
    float getRecoverEffect();
    void setRecoverEffect(float recoverEffect);
    
    short getRecoverWuHiangCd();
    void setRecoverWuJiangCd(short recoverCd);
    
    float getRecoverWuJiangEffect();
    void setRecoverWuJiangEffect(float recoverEffect);
    
    
   
    
    long getCurCoin();
    void setCurCoin(long curCoin);
    
    short getHelperSkillId();   // 获取好友的被动技能Id，用于结算画面的属性翻倍技能
    short getLeaderSkillId();   // 获取主将的被动技能Id，用于结算画面的属性翻倍技能
    
    void setStone(int iRow, int iColumn, char elem);
    char getStone(int iRow, int iColumn);
    
    void setStonePlus(int iRow, int iColumn, char plus);
    char getStonePlus(int iRow, int iColumn);
    
    void setStoneBlackCover(int iRow, int iColumn, char blackCover);
    char getStoneBlackCover(int iRow, int iColumn);
    
    int getstoneParam1(int iRow, int iColumn);
    void setstoneParam1(int iRow, int iColumn, int param1);
    int getstoneParam2(int iRow, int iColumn);
    void setstoneParam2(int iRow, int iColumn, int param2);
    
    
    void setCardInfo(int idx,SaveData_Card card);
    SaveData_Card* getCardInfo(int idx, bool bCrash = false);
    
    SaveData_Card getCardInfo2(int idx);
    
    void setSkillCD(int idx, int cd);
    
    void setFengYinCD(int idx, short cd);
    void setState(int idx, int state);
    
    short getCurLv();
    
    short getCurCost();
    
    short getCurStatina();
    
    short getDungeonClearCoin();
    
    char getCurFloorStatus();
    void setCurFloorStatus(char curFloorStatus);
    
    void setFloorId(int floor_id);
    int  getFloorId();
    
    void setFloorName(const char* floorName);
    const char* getFloorName();
    
    void setRoomId(int room_id);
    int  getRoomId();
    
    void setRoomName(const char* roomName);
    const char* getRoomName();
    
    void setCurStep(int iStep);
    int  getCurStep();
    int getCardCurIndex(int index);
    
    void setCurItemCnt(int iItemCnt);
    int  getCurItemCnt();
        
    void setCurDgGold(int iGold);
    int  getCurDgGold();
    
    void setPlayerCurHp(long hp);
    long getPlayerCurHp();
    
    short getPlayerCanfei();
    void setPlayerCanfei(bool canfei);
    void setPlayerBaseHp(long baseHp);
    long getPlayerBaseHp();
    
    void setStoneCount(char stoneCount);
    char getStoneCount();
    
    void setStoneElement(int idx, char stoneElement);
    char getStoneElement(int idx);
    
    void setReviveTime(long reviveTime);
    long getReviveTime();
    
  //  void setReqCachaTime(long l_reqGachaTime);//新建
  //  long getReqGachaTime();
    
   
    
    
    void setFightSceneChange(short fightSceneChange);
    short getFightSceneChange();
    
    void setIsUseChristmasFightScene(bool isUseChristmasFightScene);
    bool getIsUseChristmasFightScene();
    
    const char* getMD5Str();
    
    const char* getSessionId();
    
    const char* getUserId();
    
    unsigned char getrand();
    
    void setiMoment(int moment);
    int getiMoment();
    
    unsigned char getDgSkillType();
        
    void checkIsUseExternal();
    
    str_mubiaoGx getMubiaoGongXianBylv(int lv);
    
    /* 录视频相关 */
    void OneRoundOvrNotify();   //一次攻击结束
    void setCurCombo(unsigned long ulCombo);  //提交本次combo值
    void IncCurDamage(unsigned long ulDamage); //累加本次伤害值
    void IncUseSkillCnt();
    unsigned long getMaxCombo();
    unsigned long getTotalCombo();
    float getAverageCombo();
    unsigned long getMaxDamage();
    unsigned long getTotalDamage();
    unsigned long getAverageDamage();
    unsigned long getTouchCnt();    //本场转珠次数
    unsigned long getCurTouchCnt(); //本场当前局转珠次数
    void setCurTouchCnt(unsigned long cnt);
    unsigned long getUseSkillCnt(); //本场使用技能次数
    unsigned long getMaxHp();
    unsigned long GetGroupAttack();
    
    int getPlayerEnhanceCD();
    int getPlayerEnhanceElem();
    float getPlayerEnhanceFactor();
    int getPlayerEnWujiangCD();
    int getPlayerEnWujiangType();
    float getPlayerEnWujiangFactor();
    
    
    void setPlayerEnhanceCD(int cd);
    void setPlayerEnhanceElem(int elem);
    void setPlayerEnhanceFactor(float factor);
    void setPlayerEnWujiangCD(int cd);
    void setPlayerEnWujiangType(int type);
    void setPlayerEnWujiangFactor(float factor);
    
    
    
    int getPlayerCountryWujiangCD();
    int getPlayerCountryWujiangType();
    float getPlayerCountryWujiangFactor();
    
    void setPlayerCountryWujiangCD(int cd);
    void setPlayerCountryWujiangType(int type);
    void setPlayerCountryWujiangFactor(float factor);

    void setSelectRevive(short is_sel);
    short getSelectRevive();
    
    void setFreeRevive(int isFreeRevive);
    int  getFreeRevive();
    
    str_ReviveMinusCoin* getReviveMinusCoinArr();
private:
    //forbid constructor being called outside
	SaveData():m_bValid(false), m_bUseSaveData(false){};
    
	//forbid copy constructor being called outside
	SaveData(const SaveData& );
	
	//forbid assignment
	SaveData& operator=(const SaveData& );
    
};









#endif
