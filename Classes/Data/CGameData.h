//
//  C 
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CGameData_h
#define AgainstWar_CGameData_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "jsoncpp.h"
#include <list>
#include "encryp_data.h"

using namespace cocos2d;

#define kReqStatOk          0
#define kReqStatRequesting  0xcccc
#define kReqConnectErr      0xdddd  // 56797 in decimal
#define kReqHoldOn          0xeeee
#define kReqCancel          0xff11

#define ATKPLUSGROWTH     5
#define HPPLUSGROWTH      10
#define RECPLUSGROWTH     3

#define LANGUAGE(x) CGameData::Inst()->getLanguageValue(#x)

#define MAX_BUFF_ITEMS  10

#define ENEMYSKILLMAX     8
#define ENEMYBUFFMAX      8

#define kMaxMemCntOfTeam    6
#define kMaxTeamCnt         6
#define kTmpTeamIdx         5

#define MaxCombineCnt       11

#define MaxAwakeSkillCnt    8

#define kHelperMaxCount             5       // 权限制战场带的好友的数量是5

enum en_cbAboutPostReward
{
    en_cbXuanShangBaoKu = 0,
    en_cbBaoKuJuanXian,
    en_fabuXuanShang,
    en_fabuXuanShang_end,
    en_ChooseItemFromFabu,
    en_fabuFromChooseItem,
    en_rewardMemberList,
    en_refreshPostReward,
    en_haveNoRewardTask,
    en_toBaoKuFromCFabu,
};

enum enSmallDicId{
    enSmallDicUserDungeons = 0,
    enSmallDicUserCards,
    enSmallDicAllBaseCards,
    enSmallDicAllDungeons,
    enSmallDicAllMonsters,
    enSmallDicAllCardLevels,
    enSmallDicCntryConfig,
    enSmallDicMax
};

enum enWeiboType {
    enWeiboTypeTujian = 1,
    enWeiboTypeGacha,
    enWeiboTypeTeam,
    enWeiboTypeGeneral,
    enWeiboTypeCompGacha,
    enWeiboTypeTenCompGacha,
};

enum enElementDef{
    enElementHeart = 0,
    enElementWater,
    enElementFire,
    enElementWood,
    enElementLight,
    enElementDark,
    enElementInvalid,
    enElementBomb,  //暂时无用，占个坑
    enElementMax,
    enElementZero,  // 无属性，主要用于技能，不受属性关系影响，相关效果和火属性一致
    enElementInstead,   // 相克属性，主要用于技能，为克制敌人怪物的属性
    enElementGod,   // 神圣属性，主要用于技能，不仅无视属性关系，而且无视防御
    enElementRecover,   // 用于敌怪的buff效果，只有消除心属性宝石回复的多于武将最大血量的等数值攻击力的属性
    enElementOther,     // 剔除对应某一宝石的其他所有宝石
};

enum enSpecialStone
{
    enSpecialStone_Null = 0,
    enSpecialStoneBomb
};

enum enTeaStuTypeDef {  // 师徒关系
    enTeaStuType_CanStudent = 0,    // 等级小于出师等级、没有师父
    enTeaStuType_BeStudent,     // 等级小于出师等级、有师父
    enTeaStuType_NotTeaAndStu,  // 等级大于等于出师等级、小于收徒等级
    enTeaStuType_CanTeacher,    // 等级大于等于收徒等级
};

enum enLinDef{
    enLinZ,
    enLinW,
    enLinJ,
    enLinMax
};

enum enBuffTypeDef {
    enBuffType_Stamina = 0,
    enBuffType_Attack,
    enBuffType_Recover,
    enBuffType_Hp,
    enBuffType_Max,
};

enum {
    ItemInAppStore = 0,
    ItemNotInAppStore ,
    ItemEnd,
    
    PaymentPurchased, 
    PaymentFailed,     
    PaymentRestored,
    PaymentTimeOut,
    PaymentAliSuccess,
};

typedef enum {
    enTimeNoChange,
    enDayChanged,
    enHourChanged,
} enTimeCompareResult;

class CMailSenderInf;
struct ConfigData;

class CAttackInfo: public CCObject
{
public:
    CAttackInfo()
    {
        clear();
    }
    bool    bAtkOvr;
    
    int     iAtkCategory;
    int     iAtkSubCategory;
    
    int     iElement;			//火，木，水，光，暗
//	int     iAttackPt;
    CEncrypData<int>            iAttackPt;
	int     iTargetCnt;         //0:攻击单个目标   1: 全体攻击  2:攻击属性被克制的 3:攻击固定的单个目标，无法被改变（用于反击的主动技能）
    
    int     iManSkillID;
    int     iAutoSkillID;
    int     skillParam1;
    int     skillParam2;
    int     skillParam3;
    
    bool    changeStone; //是否是转换珠子的技能
    
    int     iEnemyIdx;  //敌人攻击时使用，发动攻击的敌人的索引
    
    char    chRoundCount;       //回合数
    int     nEffectType;        //效果类型
    int     nEffectAttack;      //效果攻击
    CCPoint srcPt;
    
    bool bShowSkillEffect;
    bool    fristAtkOver;
    //int
    
    
    
    //攻击或技能发动需要消耗的时间(秒)
//    float   m_fAtkDur;
    
    void clear(){
        bShowSkillEffect = false;
//        m_fAtkDur = 0.2f;
        bAtkOvr = false;
        iElement = 0;
        iAttackPt = 0;
        iTargetCnt = 0;
        iManSkillID = 0;
        iAutoSkillID = 0;
        chRoundCount = 0;
        nEffectType = 0;
        nEffectAttack = 0;
        iEnemyIdx = kInvalidValue;
        srcPt = CCPointMake(0, 0);
        
        iAtkCategory = 99;
        iAtkSubCategory = 99;
        fristAtkOver = false;
        changeStone = false;
    }
};


class CUiLayerBase: public CCLayer
{
public:
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void reset(){}
};

//卡片排序
enum enCardRangeType {
    enCardRangeTypeProp,    // 默认属性
    enCardRangeTypeAtta,    //按照攻击力排列
    enCardRangeTypeHp,      //按照血量排列
    enCardRangeTypeRec,     //按照恢复力
    enCardRangeTypeGet,     //按照入手排列
    enCardRangeTypeLock,    //按照加锁
    enCardRangeTypePlus,    //+值排序
    enCardRangeTypeCost,    //统御力
    enCardRangeTypeCategory,//种类
    enCardRangeTypeCountryPorterty,//国家类别
    encardRangeTypeMax,
};

//好友排序
enum enFriendRange
{
    enFriendRangePro = 1,
    enFriendRangeHp,
    enFriendRangeAtk,
    enFriendRangeRec,
    enFriendRangeLT,
    enFriendRangeCou,
    };

// 国家成员排序
enum enMemberRange
{
    enMemberRangeStatus = 0,    // 职位
    enMemberRangeLv,            // 等级
    enMemberRangeCbt,           // 贡献值
    enMemberRangeLT,            // 登录时间
    enMemberRangeMax,
};

enum {
    enPayforzhaocaijinbao = 0,
    enPayforcaiyungungun,
    enPayformengjiangwushuang,
    enPayforluanshishenjiang,
    enPayfor_max,
    
};


// 特殊Iap
enum enSpecialIap
{
    enSpecialIap_6RMBGift_00 = 0,      // 6元大礼包
    enSpecialIap_30RMBGift_12,         // 30元大礼包
    enSpecialIap_198RMBGift_13,        // 198元大礼包
    enSpecialIap_30RMBMonth_11,        // 30元月卡（招财进宝）
    enSpecialIap_198RMBMonth_09,       // 198元月卡（财源滚滚）
    enSpecialIap_max,
};
enum enYueBao
{
    enYueBao_zhaocaijinbao = 0,
    enYueBao_caiyungungun,
    enYueBao_max,
};

struct c_sango_Tips {  //add1.14
    string  m_content="";
    int m_interVal;
    int m_iType;
    unsigned long  m_sfirstTime;
    unsigned long  m_sendTime;
   // list<string> l_cardArr;
};

struct c_sango_Tips_oneTime {
    string m_content = "";
    int  m_itype;
};


class CUserInfo: public CCObject
{
public:
    
    list <c_sango_Tips>  m_lsango_tips_1; //公告Tips显示（在一定时间内重复出现的）
    list<c_sango_Tips_oneTime> m_lsango_tips_2;//公告Tips只显示一次
    
    char                username[100];
    char                uid[100];
    char                invite_code[100];
    bool                first_charge;
    
    
    long                free_cd_time;
    
    bool                isMzsm;
        
    unsigned long       gacha_pt;
    unsigned long       coin;
    unsigned long long  gold;
    unsigned char       user_elem;
    unsigned long       stamina_upd_time;   //TimeIntervalSince1970
    unsigned short      stamina;
    unsigned short      max_stamina;
    unsigned long       acPoint_upd_time;   //TimeIntervalSince1970
    unsigned short      actionPoint;
    unsigned short      max_acPoint;
    unsigned short      max_card_num;
    unsigned short      cost; //(=原来cost+附加统御力)
    unsigned long       exp;
    unsigned long       this_lv_now_exp;
    unsigned long       next_lv_need_exp;
    unsigned short      lv;
    unsigned long       login_time;         //TimeIntervalSince1970
    
    unsigned short      leadercard_cid;
    unsigned short      leadercardlv;
    unsigned short      maxFr_num;
    unsigned short      friend_extend_num;
    
    unsigned short      friend_request_num;     // 好友请求数量
    unsigned long       last_mail_time;         // 上一封邮件的时间
    
    unsigned long       can_contribute_coin;    //可以捐献的元宝数
    
    
    
    //限时卡
    unsigned short      hours_append_cost;
    unsigned long       hours_endTime;
    //月卡
    unsigned short      month_append_cost;
    unsigned long       month_endTime;
    
    //引导
    int add_866_card;//太极果
    int add_award;//第一次领奖
    int add_card;//武将加
    int add_lv_5;//第一次到五级
    int card_update;//强化
    int card_upgrade;//转生
    int dungeon_fail;//失败次数
    int guild_item;//随身军袋
    
    //tapjoy
    int tjPoints;
    int tjCoins;
    int tjPoints_per_coin;
    int tjMax_coins;
    bool max_fg;
    
    //push
    bool push_special_dunegon;
    bool push_double_drop;
    bool push_guild_charge_cold;
    int  push_stamina_num;
   
    
    string              title_name;
    string              new_title_name;
    
    bool                bSpecialIap[enSpecialIap_max];  // 礼包开关
    
    unsigned long               bSYuebao[enYueBao_max]; //月包剩余时间
};





//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//好友信息
class CFriendHelpInfo: public CCObject
{
public:
    char            title_name[100];
    char            name[100];
    char            fid[100];
    char            type[16];
    unsigned char   gacha_pt;
    unsigned char   user_elem;
    unsigned short  lv;
    
    
    
    unsigned short  leadercard_cid;
    unsigned short  leadercardlv;
    unsigned short  sk_lv;
    unsigned short  pl_hp;
    unsigned short  pl_attack;
    unsigned short  pl_recover;
    unsigned short  plus = 0;
    
    int             i_awakeNum;
    
    string          leader_ucid;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//用户卡片信息
class CUserCardInfo: public CCObject
{
public:
    char            ucid[100];
    char            name[100];  //
    
    short           cou_name;  //国家属性
    
    unsigned short  cid;        //
    unsigned short  category;
    unsigned short  sub_category;
    unsigned short  ugp_target;
    unsigned short  ugp_max_star;     // 最大upgrade后的星数
    char            exp_type;
    unsigned long   exp;
    unsigned long   base_exp;
    unsigned short  lv;
    unsigned short  sk_lv;
    unsigned long   upd_time;
    unsigned long   upg_gold;
    
    unsigned char   star;
    CEncrypData<long> hp;
    unsigned short  skid;
    unsigned char   ctype;
    unsigned char   stype; //sub副属性
    CEncrypData<long> attack;
    unsigned short  cost;
    unsigned short  leader_skid;
    unsigned long   sell_gold;
    long            recover;
    unsigned short  max_lv;
    unsigned short  ugp_need[5];
    char            upg_flg;    //  0: 不可进化, 1: 可进化 材料不全,  2: 材料齐全
    bool            bChecked;   //  在判断转生素材的时候，用来记录素材是否被选中
    unsigned short  pl_hp;
    unsigned short  pl_attack;
    unsigned short  pl_recover;
    unsigned short  plus = 0;
    
    
    
    
    int             AwakeSkillArr[MaxAwakeSkillCnt];        //能力突破
    int             i_AwakeNum;                             //已能力突破数量
    
    string          find_me;    //入手地信息
    
    //究极进化
    bool            bIsEluTgt;  //是否为究极进化产物
    char            elu_flg;    //  0: 不可进化, 1: 可进化 材料不全,  2: 材料齐全
    unsigned short  elu_target;
    unsigned short  elu_need[5];
    
    //觉醒
    bool            bIsWkTgt;  //是否为觉醒产物
    char            wk_flg;    //  0: 不可觉醒, 1: 可觉醒 材料不全,  2: 材料齐全
    unsigned short  wk_target;
    unsigned short  wk_need[5];
    
    //涅槃
    bool            bIsNPTgt;       //是否可以涅槃
    unsigned short  np_need_num;    //需要素材的数量
    list<short>     np_list_need;   //需要素材
    list<short>     np_list_show;   //推荐素材
    list<short>     np_list_target; //目标武将
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//卡片基础信息
class CCardBaseInfo: public CCObject
{
public:
    char            name[100];
    
    short           cou_name;  //国家属性
    
    unsigned short  cid;
    unsigned short  category;
    unsigned short  sub_category;
    int             AwakeSkillArr[MaxAwakeSkillCnt];            //能力突破
    unsigned short  ugp_target;
    unsigned short  ugp_max_star;   // 最大upgrade后的星数
    char            exp_type;
    unsigned long   base_exp;
    unsigned long   upg_gold;
    unsigned char   star;
    long            hp;
    float           hp_growth;
    unsigned short  skid;
    unsigned char   ctype;
    unsigned char   stype;
    long            attack;
    float           attack_growth;
    unsigned short  cost;
    unsigned short  leader_skid;
    unsigned long   sell_gold;
    long            recover;
    float           recover_growth;
    unsigned short  max_lv;
    unsigned short  ugp_need[5];
    
    string          find_me;    //入手地信息
    
    //究极进化
    bool            bIsEluTgt;  //是否为究极进化产物
    unsigned short  elu_target;
    unsigned short  elu_need[5];
    
    //觉醒
    bool            bIsWkTgt;  //是否为觉醒产物
    unsigned short  wk_target;
    unsigned short  wk_need[5];
    
    //涅槃
    bool            bIsNPTgt;       //是否可以涅槃
    unsigned short  np_need_num;    //需要素材的数量
    list<short>     np_list_need;   //需要素材
    list<short>     np_list_show;   //推荐素材
    list<short>     np_list_target; //目标武将
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//战场room信息
class CDgRoomInfo: public CCObject
{
public:
    CDgRoomInfo(){
        m_pHeadTipsArr = NULL;
        m_pEndTipsArr = NULL;
    }
    
    ~CDgRoomInfo(){
        if (m_pHeadTipsArr) {
            m_pHeadTipsArr->removeAllObjects();
            delete m_pHeadTipsArr;
        }
        
        if (m_pEndTipsArr) {
            m_pEndTipsArr->removeAllObjects();
            delete m_pEndTipsArr;
        }
    }
    
    CCMutableArray<CCString*> *m_pHeadTipsArr;
    CCMutableArray<CCString*> *m_pEndTipsArr;
    
    char            name[100];
    unsigned short  stamina;
    unsigned short  steps;
    unsigned short  propose_lv; //建议等级
    char            cStatus;
    short           video;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//战场限制信息
typedef struct
{
    unsigned short  cards_cost;
    unsigned short  cards_ctype[kMaxMemCntOfTeam];
    unsigned short  max_cost;
    unsigned short  only_ctype[kMaxMemCntOfTeam];
}stDgRestrict;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//普通战场信息
class CNmlDgInfo: public CCObject
{
public:
    CNmlDgInfo(){
        m_pRoomArr = new CCMutableArray<CDgRoomInfo*>();
        m_unLimitMaxStar = 0;
    }
    
    ~CNmlDgInfo(){
        m_pRoomArr->removeAllObjects();
        delete m_pRoomArr;
    }
    
    CCMutableArray<CDgRoomInfo*> *m_pRoomArr;
    
    char            name[100];
    char            effectname[100];
    unsigned long   effect_starttime;
    unsigned long   effect_endtime;
    unsigned long   effect_remaintime;
    char            cStatus;
    int             dungeon_type;
    unsigned short  m_unLimitMaxStar;   //限制最大星
    
    long            minlevel;
    long            maxlevel;
    
    string          dungeon_dsp;
    
    bool            buff_void;                  //是否可以使用同盟buff
    
    char            bg_type;    // 特殊战场菜单选项的背景图片类型，0 = 默认黄色；3 ＝ 带框黄色；
    
    stDgRestrict    dgRstrct;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//特殊战场信息
enum enSpecialDgSkillType{
    enSpecialDgSkillType_null = 0,
    enSpecialDgSkillType_qianlizoudanqi,        // 千里走单骑
    enSpecialDgSkillType_quanduiwuxianding,     // 全队无限定
    enSpecialDgSkillType_max,
};


class CSpecialDgInfo: public CCObject
{
public:
    CSpecialDgInfo(){
        m_pRoomArr = new CCMutableArray<CDgRoomInfo*>();
        m_unLimitMaxStar = 0;
        skillType = enSpecialDgSkillType_null;
    }
    
    ~CSpecialDgInfo(){
        m_pRoomArr->removeAllObjects();
        delete m_pRoomArr;
    }
    
    CCMutableArray<CDgRoomInfo*> *m_pRoomArr;
    
    char            name[100];
    char            effectname[100];
    
    bool            is_auto;
    unsigned long   starttime;
    unsigned long   endtime;
    unsigned long   remaintime;
    int             dungeon_type;
    
    string          dungeon_dsp;
    
    bool            buff_void;          //是否可以使用同盟buff
    
    char            cType;  //1: special  2: weekly
    int             iFloorId;
    char            cStatus;
    
    char            availDiamondCnt;
    char            availDiamond[enElementMax];
    
    bool            bottom_dungeon;
    
    long            minlevel;
    long            maxlevel;
    
    long            cards_coin;         //借将需要消耗的元宝
    long            cards_gold1;        //第1次借将需要消耗的铜钱
    long            cards_gold2;        //第2次借将需要消耗的铜钱
    long            cards_gold3;        //第3次借将需要消耗的铜钱
    
    string          strNeedCards[6];    //第一个元素为必须的主将卡，主将无限制则置空
                                        //每个元素的格式为 cid,cid,...
    
    string          strHelpCards[6];    //第一个元素为必须的主将卡，主将无限制则置空
                                        //每个元素的格式为 cid,lv
    
    char            bg_type;    // 特殊战场菜单选项的背景图片类型，0 = 默认黄色；1 = 绿色；2 = 蓝色；
    unsigned short  m_unLimitMaxStar;   //限制最大星
    
    stDgRestrict    dgRstrct;
    
    enSpecialDgSkillType    skillType;  // 技术战场类型

};
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//三国演义
class cSangoYYRoom: public CDgRoomInfo
{
    
};
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CSangoYYDgInfo : public CNmlDgInfo
{
public:
    CSangoYYDgInfo(){
        m_pRoomArr = new CCMutableArray<cSangoYYRoom*>();
    }
    
    ~CSangoYYDgInfo(){
        m_pRoomArr->removeAllObjects();
        delete m_pRoomArr;
    }
    
    int       floor_id;
    CCMutableArray<cSangoYYRoom*> *m_pRoomArr;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//战场怪物信息
class CDgMonsterInfo: public CCObject
{
public:
    unsigned short  cid;
    unsigned short  drop_cid;
    unsigned short  drop_lv;
    bool            drop_plus;  // 掉落的武将是否有+1
    
    unsigned long   attack;
    CEncrypData<unsigned short>  turn;
    unsigned long   defense;
    unsigned long   drop_exp;
    unsigned long   drop_gold;
    CEncrypData<unsigned long>      hp;
    unsigned char   ctype;
    unsigned char   mtype;
    CEncrypData<unsigned short>  init_attack_turn;
    //unsigned short  skid;
    unsigned short  skidArr[ENEMYSKILLMAX];
    
    //unsigned short  buff_id;
    unsigned short  buffArr[ENEMYBUFFMAX];
};

class CDgStepInfo: public CCObject
{
public:
    CDgStepInfo(){
        m_pMonsterArr = new CCMutableArray<CDgMonsterInfo*>();
    }
    
    ~CDgStepInfo(){
        m_pMonsterArr->removeAllObjects();
        delete m_pMonsterArr;
    }
    
    CCMutableArray<CDgMonsterInfo*> *m_pMonsterArr;

    unsigned long   gold_box;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//战场进度
class CDgStatus: public CCObject
{
public:
    unsigned short  curFloorId;
    unsigned short  curRoomId;
    unsigned short  status;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//好友信息
class CFriendInf: public CCObject
{
public:
    char            title_name[100];
    char            name[100];
    char            fid[100];
    unsigned char   user_elem;
    unsigned short  lv;
    
    unsigned short  leadercard_cid;
    unsigned short  leadercardlv;
    unsigned short  sk_lv;
    unsigned short  max_friend_num;
    unsigned short  now_friend_num;
    
    unsigned long   login_time;
    
    CUserCardInfo   card_info;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//拜 师信息
class CMAARequestMasterInfo: public CCObject
{
public:
    char            title_name[100];
    char            name[100];
    char            fid[100];
    unsigned char   user_elem;
    unsigned short  lv;
    
    unsigned short  leadercard_cid;
    unsigned short  leadercardlv;
    unsigned short  sk_lv;
    unsigned short  max_friend_num;
    unsigned short  now_friend_num;
    
    unsigned long   login_time;
    
    CUserCardInfo   card_info;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//gacha 结果
class CGachaRet: public CCObject
{
public:
    char            ucid[100];
    char            is_first;
    bool            is_compGachaMaterial;
    int             pl_recover = 0;
    int             pl_hp = 0;
    int             pl_attack = 0;
    int             plus;
};

struct str_TaskCntryBtn
{
    int minLlv;
    int maxLlv;
    int contribution;
};

struct str_ReviveMinusCoin
{
    char item_id[100];
    char item_dsp[100];
    int  item_coin;
};

//common信息
class CommonInfo: public CCObject
{
  public:
    CommonInfo():month_item_cost(0),month_cost_coin(0),is_select_revive(false),revive_select_tag(-1){}
    unsigned short card_extend_coin;
    unsigned short card_extend_num;
    unsigned short dungeon_clear_coin;
    unsigned short gacha_cost_coin;
    unsigned short gacha_cost_pt;
    unsigned short max_gacha_point ;
    unsigned short recover_stamina_coin ;
    
    unsigned short permit_guild_lv ;//同盟等级需求
    unsigned short permit_special_lv ;//活动等级需求
    unsigned short permit_sgyy_lv ; //技术等级需求
    
    
    unsigned short recover_action_coin;
    unsigned short action_recover_time;
    unsigned short revive_coin;
    vector<str_ReviveMinusCoin>   vec_revive_minus_coin;     // 复活选项
    bool is_select_revive;
    short revive_select_tag;
    
    unsigned short stamina_recover_time;
    unsigned short mail_all_coin;       // 发同盟邮件需要的元宝数
    char            aboutus[100];
    char            war_help_url[100];
    char            gacha_notice[100];
    char            cntry_notice[100];  // 国家的帮助
    char            niepan_notice[100]; // 涅槃的帮助
    char            voice_icon[100];
    bool            qq_voice_fg;
    bool            sina_voice_fg;
    bool            weixin_voice_fg;
    char            voice_1[500];
    char            voice_2[500];
    char            voice_3[500];
    char            voice_4[500];
    char            voice_5[500];
    char            voice_6[500];
    char            voice_7[500];
    char            voice_8[500];
    char            voice_9[500];
    char            app_url[100];
    char            compgacha_note_url[100];
    char            praise[100];
    bool            compgacha_fg;
    bool            tapjoy_fg;
    bool            bUseJiangNanStyle;  // 是否使用江南style的技能
    char            bbsurl[100];
    char            charge_url[200];    //是否使用charge_url
    unsigned short  charge_type;
    char            androidsg_download[100];
    unsigned short max_friend_extend_num;
    short          max_video_num;
    unsigned short friend_extend_num;
    unsigned short friend_extend_coin;
    unsigned short cntry_create_coin;   // 创建国家需要的元宝数
    
    bool           get_reward_open;   //是否开启奖励活动
    bool           m_bIsShowVideoStrategy;      //是否显示视频攻略
    
    unsigned short multi_gacha_cnt; //多练抽次数
    unsigned short bg_change;       //战斗场景 0:随机， 1:旧， 2: 新
    bool           bChrisBg;        // 圣诞 战斗场景 "special_bg" = shengdan
    
    unsigned short  evolution_lv;   //超进化需要的等级限制
    string          evolution_notice;   //超进化规则
    string          wake_notice;   //觉醒规则
    
    unsigned short delete_guild_cache_time; // 解散国家的缓冲时间
    
    long skill_lv_up_starttime;//强化按钮显示技能升级双倍开始时间
    long skill_lv_up_endtime;//强化按钮显示技能升级双倍开始时间
    
    long modify_name_gold;    // 修改同盟名片需要的铜钱
    
    bool bOpen_suitang_gacha;   // 是否开启隋唐求将

    
    bool bOpen_hero_1_gacha;   // 是否开启其他求将
    bool bOpen_hero_2_gacha;   // 是否开启其他求将
    bool bOpen_hero_3_gacha;   // 是否开启其他求将

//    int bOpen_other_gacha;   // 是否开启其他求将

    bool bShow_charge_award;    //是否开启周年商店活动
    
    string weixin_share_type;   //微信分享类型，好友or朋友圈
  
    list<string> list_external;     // 外挂列表
    
    bool bShowCDKey;                // 是否显示礼品兑换界面
    
    unsigned short month_item_cost;     //月附加统御力增加值
    unsigned short month_cost_coin;     //月附加统御力元宝数
    
    list<str_TaskCntryBtn> list_taskCntrybution; //默认发布悬赏贡献值
    long task_coin_default;
    long task_deadline_default;
    long task_person_default;
    
    string            mzsm_url;  //免责声明url
    bool bOpen_cards_shop;      // 是否开放武将商店
    
    unsigned short  m_splan_coin; //分期返还元宝，每天给多少元宝
    unsigned short  m_splan_days;  //分期返还元宝的天数
    
    bool              bOpen_month_charge;   // 月卡礼包开关
    
    int  free_revive_lv; //免费复活等级
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//碎片信息

class ParcelNeedElementInfo: public CCObject
{
public:
    
    string            objId;
    int             num;
    
};
class ParcelChildInfo: public CCObject
{

public:
    
    long coin;
    
    CCMutableArray<ParcelNeedElementInfo*> * m_ParcelNeedElementInfo;
    
    long start_time;
    long end_time;
    long gold;
    int lv;
    int skill_lv;
    int rate;
    string            targetId;
    
    string toID;
    
    long wealth;
    
    bool longTime;
    
};


class ParcelItemInfo: public CCObject
{

public:
    
    long coin;
    
    CCMutableArray<ParcelNeedElementInfo*> * m_ParcelNeedElementInfo;
    
    long start_time;
    long end_time;
    long gold;
    int rate;
    string            targetId;
    
    string toID;
    long wealth;
    
    bool longTime;

    
};







//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//可用的宝石信息
class CAvailDiamond: public CCObject
{
public:
    char            cnt;
    char            buf[enElementMax];
    
    void clear(){
        cnt = 0;
    }
    
    void set(char cCnt, char* pBuf){
        cnt = cCnt;
        
        if(cnt < enElementMax){
            memcpy(buf, pBuf, cnt);
        }
        else{
            setAllAvail();
        }
    }
    
    
    void setAllAvail()
    {
        cnt = enElementMax-2;  //-2的意义是初始关卡不包含无效图块和炸弹图块
        
        for(int i = 0; i < enElementMax; i++)
            buf[i] = i;
    }
};

//push信息

class PushInfo:public CCObject
{
    public: 
        char msg[1000];
        long pushTime;
        
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//team info

typedef struct{
    char    uid[100];
    int     cost;
}stUid;

typedef struct{
    stUid   mem[kMaxMemCntOfTeam];
}stTeam;


class CompGachaCardInfo :public CCObject{
  public:
    unsigned short cid;
    bool isOwe;
    bool isNew;
};
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//compGacha info
class CompGachaInfo :public CCObject{
public:
    CCMutableArray<CompGachaCardInfo *>  *material;
    char    startTime[100];
    char    endTime[100];
    int     claimTimes;
    unsigned short    targetCid;
    bool    isHide;
    unsigned long rest_exchange_time;
    int     position;
    bool    isTop;
    CompGachaInfo()
    {
        material = new CCMutableArray<CompGachaCardInfo *>();
    }
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//exchangeCardInfo
class ExchangeCardInfo :public CCObject{
public:
    char  cid[50];
    unsigned short exp;
    bool isFirst;
    unsigned short lv;
    unsigned short sk_lv;
    char uCid[100];
    long long upd_time;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//会员信息
class CMember: public CCObject
{
public:
    char            title_name[100];
    char            name[100];
    char            mid[100];
    unsigned short  status;             // user‘s status in the country, 1 = leader, 2 = viceleader, 3 = member
    unsigned char   user_elem;
    unsigned short  lv;
    bool            bFriend;            // member is user's friend or not
    
    bool            bePingbi;           //member pingbi from me
    bool            VoiceOn;            // member can voice
    unsigned long   contibution;
    unsigned long   login_time;         //TimeIntervalSince1970
    unsigned long   supply;             // 粮饷
    
    unsigned short  encounter;          // 遭遇战次数
    unsigned short  power;              // 行动力点数
    
    CUserCardInfo   card_info;          // 成员的主将
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//悬赏进度信息
class CRewardMember: public CMember
{
public:
    bool            bRecieveReward;     // 是否已经领取奖励
    long            done_contribution;  // 已经完成的贡献值
    long            target_contribution;    // 完成悬赏的目标贡献值
    bool            bFinishReward;      // 是否完成悬赏
};


//商店活动订单信息
class ChargeAward : public CCObject
{
public:
    ChargeAward();
    ~ChargeAward();
    
    int* m_iAwadrIds;
    int  m_iAwadrNum;
    int* m_iOidIds;
    int  m_iOidNum;
    int m_iOidTotalNum;
    
    int charge_sum;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//buff信息
class CBuffInfo : public CCObject
{
public:
    unsigned short lv;      // buff 等级
    float effect;           // buff 效果
    unsigned short coin;    // buff 升级需要的元宝
    unsigned long  exp;     // buff 升级需要的经验
};

//战场掉落卡片信息
class GuildItemAward : public CCObject
{
public:
    string strItemId;
    string strImgName;
    long   lCnt;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//公会信息
class GuildItemInStore: public CCObject
{
public:
    string          strItemId;
    string          strImgName;
    string          strItemName;
    string          strItemDsp;
    long            lMinCondition;
    long            lMaxCondition;
    long            lLv;
    long            lCnt;
};

class CCountryInfo: public CCObject
{
public:
    char            name[100];
    char            nameCard[100];
    char            cid[100];
    char            declaration[200];
    unsigned short  status;             // user‘s status in the country, 1 = leader, 2 = viceleader, 3 = member
    char            leader_name[100];
    unsigned short  member_cur_num;
    unsigned short  member_max_num;
    unsigned short  member_apply_num; // 申请加入国家的成员数量
    unsigned short  org_dungeon_num;    // 可进入的保卫战数量
    
    unsigned short  lv;
    CBuffInfo       buffInfo[enBuffType_Max];
    unsigned long   cur_exp;
    unsigned long   need_exp;
    unsigned long   cntry_supply;
    unsigned long   user_supply;
    unsigned long   cntry_contribution;
    unsigned long   user_contribution;
    unsigned long   cntry_coin;
    unsigned long   cntry_delete_time;  // 解散同盟持续时间
    unsigned long   last_chat_time;
    Json::Value     charge_wealth_record;   //记录每一项福利上一次兑换的时间
    unsigned long   can_contribute_coin;        //可以捐献的元宝数
    
    CCMutableArray<GuildItemInStore*> *m_pGuildItemInStore;
    
    CCountryInfo(){
        m_pGuildItemInStore = NULL;
    }
    
    ~CCountryInfo(){
        if (m_pGuildItemInStore) {
            m_pGuildItemInStore->removeAllObjects();
            delete m_pGuildItemInStore;
        }
    }
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//公会基础信息
class CCntryBaseInfo: public CCObject
{
public:
    char            name[100];
    char            cid[100];
    char            declaration[200];
    char            leader_name[100];
    char            member_num[100];
    bool            bApply;
    
    unsigned short  lv;
    unsigned short  rank;   // 按等级排名的名次
    unsigned long   gexp_total;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//悬赏成员排行信息
class CRwdTopMemberInfo: public CCObject
{
public:
    string              name;
    string              guild_name;
    long                contribution;
    unsigned short      lv;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//悬赏基础信息
class CPostRewardBaseInfo: public CCObject
{
public:
    string          name;
    long            applyDeadLine;      // 接受悬赏任务的截止时间
    long            completeDeadLine;   // 完成悬赏任务的截止时间
    long            ensureYuanBao;      // 保证金元宝
    short           MaxJoinPerson;      // 悬赏任务允许接受的最大人数
    short           CurJoinPerson;      // 悬赏任务当前接受的人数
    long            contribution;       // 悬赏任务设置的目标贡献值
    char            status;             // 奖励的状态：0.没参加；1.已参加；2.已完成；3.已领取；
    
    string          itemId;         // 奖励道具Id
    short           itemCount;      // 奖励道具数量
    string          strImgName;     // 奖励道具的Icon
    string          strItemDsp;     // 奖励道具的描述
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//福利信息
class CWelfareInfo: public CCObject
{
public:
    char            wid[100];
    char            description[200];
    unsigned short  imageId;        // 福利项对应的图片Id
    unsigned short  needLv;
    unsigned short  cd_time;        // ÂÖëÊç¢Á¶èÂà©ÁöÑÂÜ∑Âç¥Êó∂Èó¥
    unsigned long   needSupply;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//保卫战配置信息
class CCntryDgInfo: public CCObject
{
public:
    char            dg_type[100];   //  ”0“: 正常保卫战；”1“：疾风；”2“：乱舞；“3”：同盟练习场
    char            dg_name[100];
    char            dg_id[100];
    char            star;
    long            cid;
    long            contribute;
    
    long            gulid_coins; //需要消耗的上古彩石（同盟练手场）
    long            closed_time;
    
    long            curBossHp;
    long            start_time;     //同盟战开启时间
    long            totalBossHp;
    double          last_time;      //hour
    long            remain_time;    //sec
    long            open_time;      //sec from 1970.1.1
    long            user_power;     //消耗的个人行动力
    bool            bIsFinish;      // true for the latter one
    
    long            end_time;       //上次结束时间
    long            cold_time;      //下次开启前的冷却时间
    
    int             dungeon_tms;
    int             dungeon_tms_config;     // 用于“疾风”、“乱舞”
    unsigned short  m_unLimitMaxStar;       //限制最大星
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//保卫战伤害排名信息
class CCntryDgRankInfo: public CCObject
{
public:
    long            lDamage;
    string          strUid;
    string          strName;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//特殊保卫战同盟成绩排名信息
class CSpecialDgRankInfo : public CCObject
{
public:
    string          strCid;     // 同盟ID
    string          strName;    // 同盟名字
    string          strTime;    // 同盟通关用的时间
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//同盟道具卡信息
#define kItemPackLimitNum   9

class ItemCardInf: public CCObject
{
public:
    string          strItemId;
    string          strResName;
    string          strType;
    string          strItemName;
    string          strItemDsp;
    long            lCnt;
    long            lNeedLv;    //使用该卡所需等级
    long            lNeedCbu;   //使用该卡所需个人贡献
//    unsigned short  cost_up_time;  //使用后的有效时候(小时)(统御力卡)
//    unsigned short  cost_up_num;   //使用后提高的值(统御力卡)
    
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//使用道具卡获得的奖励
class UseItemAward: public CCObject
{
public:
    long            coin;
    long            exp;
    long            gold;
    long            stamina;
    long            wealth;
    long            ucontribution;
    long            gcontribution;
    long            cost_up;
    long            cost_time;
    list<string>    strUcidLst;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//保卫战结果信息
class CntryDgEndInf: public CCObject
{
public:
    long    contribution;
    long    gold;
    long    playAtk;
    long    wealth;
    long    whokillwho; //0: boss被其他人打死, 1: 我打死boss, 2: boss打死我
    
    list<ItemCardInf> lstItem;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//兑换cdkey结果
class GiftInfo: public CCObject
{
public:
    char            name[100];
    long            coin;
    long            gold;
    long            gacha_pt;
    long            wealth;
    list<string>    strUcidList;
    std::vector<std::pair<std::string,unsigned int> >            m_vrGuildItem;
    
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//强化成功类型
enum enPwUpSucType{
    enPwupSucType_Null,
    enPwupSucType_Normal,
    enPwupSucType_Big,
    enPwupSucType_Super
};

enum enReqWeiboType{
    enReqWeiboTypeSina =0,
    enReqWeiboTypeQQ
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//称号列表
class TitleQuestInf: public CCObject
{
public:
    string  strQuest;
    bool    bPass;
};

class TitleInf: public CCObject
{
public:
    int                 iEquipSt;   //0: 未获得, 1:获得未装备, 2:装备中
    string              strTitleName;
    string              strTitleId;
    string              strTitleDsp;
    string              strTitleDspEx;
    list<TitleQuestInf> TtlQstLst;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//战利品分配列表
class JiFengRecordInf: public CCObject
{
public:
    long    lRecentScore;
    long    lLastScore;
    string  strUid;
    string  strName;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//悬赏用商品信息
class GuildShopItemInf: public CCObject
{
public:
    long    lCoin;      //消耗同盟元宝数
    long    lCntrbt;    //消耗贡献值
    long    lLv;        //购买所需同盟等级
    string  strItemId;
    string  strItemName;
    string  strImgName;
    string  strItemDsp;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//签到活动信息
struct RwdWuJiangCard
{
    string strCardId;
    short  lv;
};

struct RwdItemCard
{
    string strCardId;
    short  num;
};


class RwdAwardInfo : public CCObject
{
public:
    long  lCoin;
    long  lGold;
    long  lGacha_pt;
    long  lWealth;
    bool  isAward;                      //是否已领奖，true：已领  false：未领
    string content;                     //奖励描述
    list<RwdWuJiangCard> WujiangCardList;
    list<RwdItemCard> ItemCardList;
    string awardDsp;                    //奖励内容
    bool   needShare;                   //需要分享
};

class RwdBuchangInf : public RwdAwardInfo
{
public:
    string strDate;
    string strContent;
    bool bCanGet;
};

class RwdHuodongInf : public RwdAwardInfo
{
public:
    string strDate;
    string strContent;
    bool bCanGet;
    string  awardType;           //"2"活动  "3"补偿
    int awardDays;               //领奖的次数
    int loginCounts;          //连续登录的天数
};

class RwdBonusInf : public RwdAwardInfo
{
public:
    int rewardDays;                 //可以领奖的总天数
    int iDays;
    list<string> hideInf;
};

class RwdSignInf : public RwdAwardInfo
{
public:
    string strDate;
    int   iDays;
    list<string> hideInf;
};

class RwdLevelUpInf : public RwdAwardInfo
{
public:
    short lv;
    string showDate;
    int showLv;
};

class RwdMonthInf : public RwdAwardInfo
{
public:
    short    day;
    string   strDate;
    int      lDays;             //月登录天数
    
    bool     hide;              //是否绘制问号
    
    
    
};

class CRwdContinuBonusRecord
{
public:
    CCMutableArray<ItemCardInf*> * m_ItemCardArr;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//拜师
class CMAAarm : public CCObject
{
public:
    
    
};

class CMAAarmContainers : public CCMutableArray<CMAAarm*>
{
public:
//    void getSort()
//    {
//        //CCMutableArrayIterator it = m_array.begin();//,end = m_array.end();
//    }
};

//收徒
class CMAAmra : public CCObject
{
    
};

//师傅
class CMAAmaster : public CCObject
{
};

//徒弟
class CMAAapprentice : public CCObject
{
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//附加统御力卡-使用后
class CAppendCostCard : public CCObject
{
public:
    std::string     szUCID;             //唯一ID
    std::string     szItemID;           //item id
    long            lUseTime;           //可用时间(秒)
    unsigned short  ushAppendCost;      //附加的统御力
    std::string     szIcon;             //图片
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 武将的商品状态
enum enShopCardStatus {
    enShopCardStatus_default = 0,
    enShopCardStatus_hot,
    enShopCardStatus_new,
    enShopCardStatus_sale,
    enShopCardStatus_max
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 武将商店信息
class CShopCardInfo : public CCObject
{
public:
    short       sCid;                   // 武将id
    int         iPrice;                 // 武将价格
    enShopCardStatus status;            // 武将的商品状态
    int         iStartLv;               // 武将初始等级
    
    char        cStar;                  // 星级
    char        cElement;               // 属性
};

#define kMaxTrophies    100
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CGameData {
    static CGameData* s_pcIf;
    
    CCMutableArray<CFriendHelpInfo*> *m_pFriendHelpArr;
    
    CCMutableArray<CUserCardInfo*> *m_pUserCardArr;
    
    CCMutableArray<CShopCardInfo*> *m_pShopCardArr;
        
    CCMutableArray<CNmlDgInfo*> *m_pNmlDgArr;
    CCMutableArray<CSpecialDgInfo*> *m_pSpecialDgArr;
    CCMutableArray<CSangoYYDgInfo*>* m_pSangoYYArr;
    
    
    CCMutableArray<CDgStepInfo*> *m_pDgStepArr;
    
    CCMutableArray<CFriendInf*> *m_pFriendInfArr;
    
    CCMutableArray<CFriendInf*> *m_pFriendApplyInfArr;
    
    CCMutableArray<CCntryBaseInfo*> *m_pCntryInfArr;
    
    CCMutableArray<CRwdTopMemberInfo*> *m_pRwdTopMemberInfArr;
    
    CCMutableArray<CFriendInf*> *m_pMemberApplyInfArr;
    
    CCMutableArray<CMember*> *m_pMemberInfArr;
    
    CCMutableArray<PushInfo*> *m_pushInfoArr;
    
    CCMutableArray<CompGachaInfo *> *m_compGachaInfoArr;
    
    CCMutableArray<CGachaRet *> *m_GachaRetArr;

    
    
    CCMutableArray<ParcelChildInfo *> *m_ParcelChildInfoArr;

    
    CCMutableArray<ParcelItemInfo *> *m_ParcelItemInfoArr;

    
    
    CCMutableArray<CWelfareInfo*>* m_pWelfareInfoArr;
    
    CCMutableArray<CCntryDgInfo* > * m_pCntryDgInfArr;
        
    CCMutableArray<ItemCardInf* > * m_pItmTtlInf;
    CCMutableArray<ItemCardInf* > * m_pItmPkInf;
    
    CCMutableArray<TitleInf*> * m_pTitleInfArr;
    
    CCMutableArray<JiFengRecordInf*> * m_pJiFengRecordArr;
    
    CCMutableArray<GuildShopItemInf*>* m_pGuildShopItemInfArr;
    
    CCMutableArray<CPostRewardBaseInfo*>* m_pRewardTaskArr;
    
    CCMutableArray<CRewardMember*>* m_pRewardMemberArr;
    
    CCMutableArray<CMAARequestMasterInfo*> * m_pMAARequestMasterInfoArr;
    
    //签到活动
    CCMutableArray<RwdSignInf*> * m_pRwdSignInfArr;  //签到奖励
    CCMutableArray<RwdLevelUpInf*> * m_pRwdLevelUpInfArr;  //升级奖励
    CCMutableArray<RwdMonthInf*> * m_pRwdMonthInArr;  // 月份奖励
    CCMutableArray<RwdBonusInf*> * m_pRwdBonusInfArr; //累积奖励
    CCMutableArray<RwdHuodongInf*> * m_pRwdHuodongInfArr; //活动奖励
    CCMutableArray<RwdBuchangInf*> * m_pRwdBuchangInfArr; //补偿
    
    //签到活动end
    
    ExchangeCardInfo *m_exchangeCardInfo;
    int             m_pTrophies[kMaxTrophies];
    
    CCMutableArray<GuildItemAward*> * m_pGuildItemAwardAff; //战场掉落卡片
    
    CUserInfo       m_usrInfo;
    CCountryInfo    m_cntryInfo;
        
    CommonInfo      m_commonInfo;
    
    int             m_iSelectedHelper[kHelperMaxCount];
    CUserCardInfo   m_helperCardInfo;
    CUserCardInfo   m_usrCardInf;
    CCardBaseInfo   m_baseCardInf;
    
    CFriendInf      m_schUsrInf;
    CCntryBaseInfo  m_schCntryInfo;
    CBuffInfo       m_buffInfo;
    
    CGachaRet       m_gachaRet;
    
    bool            m_bUseTmpTeam;
    int             m_iCurTeamIdx;
    stTeam          m_TeamInf[kMaxTeamCnt];
    stTeam          m_preCmtTeam;
    stUid           m_CombineQueue[MaxCombineCnt];
    
    int             m_iCidBeforeEvo;
    int             m_iCidAfterEvo;
    
    int             m_iMaxFirendNum;
    int             m_iCurFirendNum;
    
    char            m_cCollection[kMaxCardNum+1];
    bool            m_bNeedToReqCollection;
    bool            m_bNeedToReqHelper;

    bool           isPlayedGame;
    long             enterBackTime;
    int             comeBackTime;

    CAvailDiamond   m_curAvailDiamond;

    int             localStamina;
    int             localAcPoint;
    
    int             m_iCurNmlFloor;
    int             m_iCurNmlRoom;
    
    int             m_iCurSangoFloor;
    int             m_iCurSangoRoom;
    
    int             m_iCurFloor;
    int             m_iCurRoom;
    int             m_iCurFlrStat;
    char            m_curDgType[20];
    
    enCardRangeType    m_defaultRangeType;
    enFriendRange    m_defaultFrRangeType;
    bool            isNewSpecialRoomExist;
    
    bool            m_bFighting;
    char            compGachaTime[100];
    int             compGachaNumber;
    char            gachaCardName[100];

    bool            m_bUseChristmasUI;  // 是否使用圣诞节UI
    bool            m_bUseInviteCode;
    
    int             m_productIdx;
    bool            m_isFirstShowCompGachaHelp;     //compGacha显示帮助的开关
    
    long            dateRecord;
    
    bool            m_bUpdateCom;
    
    Json::Value     m_newSpecialDgStatusDic;
    Json::Value     m_newWeeklyDgStatusDic;
    
    Json::Value     m_allUserDugeonsDic;    // 存放user的所有dungeon信息，来至login
    Json::Value     m_allUserCardsDic;  // 存放user的所有card信息，来至login/allCard
    Json::Value     m_allBaseCardsDic;  // 存放所有card基础信息，来至config
    Json::Value     m_allBaseDungeonsDic;   // 存放所有dungeon基础信息，来至config
    Json::Value     m_allMonstersDic;   // 存放所有monster信息，来至config
    Json::Value     m_allCardLevelsDic; // 存放所有card_level(不同等级升级所需经验)信息，来至config
    Json::Value     m_cntryConfigDic;   // cntryConfig,来至config
    
    long            m_allSmalDicTime[enSmallDicMax];
    
    CCLayer*        m_lyUserCards;
    int             m_iLoadCardsStep;
    
    long            m_lFrndLstReqTime;
    long            m_lRcvMailReqTime;
    
    enMemberRange   m_memberRangeType;  // 成员排序类型
    
    string          m_strCostCoin;
    
    char            m_nameCard[100];
    
    long            m_hourReqTime;  // 记录进行了整点通信的时间

    string            m_selectedTaskId;   // 查看的悬赏任务ID
public:
	//Get the instance pointer
	static CGameData* Inst();
    
    bool           isMusicOn;
    bool           isSoundOn;
    bool           isSkillOn;
    
    
    
    //提醒开关
    bool           isActivityRemind;    //活动战场开启提醒
    bool           isTwoUpRemind;       //战场2倍掉率提醒
    bool           isCntryCdRemind;     //同盟宝物兑换CD提醒
    int            intPowerRemind;      //体力指定值提醒

    
    void setEnterBackTime(int time);
    int getEnterBackTime();
    
    void setComeBackTime(int time);
    int getComeBackTime();
    
    time_t getCurrentTime();
    
    //request
    void requestALanguageBag(); // 从后台更新语言包 -----异步
    bool loadLanguageBag();     // 加载本地语言包, 语言包为空返回false，否则返回true
    bool isUpdateLanguageBag();   // 根据最新的语言包版本号判断时候要更新语言包
    void setLanguageBagVersion(float languageBagVersion);   // 设置最近的语言包版本号
    bool newbieRequest(int country, int step, char* ucid);
    bool newbieRequestPwUp();
    void requestIndex(const char* inviteCode = 0);
    bool reqInfoAsync();
    bool isInviteCodeRight();
    bool OCAccountYes();
    bool QQAccountYes();
    bool SinaAccountYes();
    bool FacebookAccountYes();
    int  getLoginType();
    bool loginByTencentRequest();
    bool loginBySinaRequest();
    bool loginRequest();
    bool cfgRequest();
    bool reqCard();
    bool reqMonster();
    
    
   bool frdRequest();  //新增
    bool frdRequest(long curTime);
    
    bool reqChargeAward();
    ChargeAward* getChargeAward();
    bool frdExtendRequest();
    bool askCostMonthCard();
    
    bool reqBuyCard(short cid);
    
    bool enterDgRequest(const char* dg_type, char* flrid, char* roomid, const char* hp_ucid,
                        const char* hprid[kHelperMaxCount], const char* helpertp[kHelperMaxCount],
                        long lRty);
    bool dgEndRequest(const char* dg_type, char* flrid, char* roomid, char* dgUid);
    
    bool dgEndForFanTiRequest(const char *dg_type, char *flrid, char *roomid, char *dgUid);
    
    bool allCardRequest();//新增
    
    bool allCardRequest(long curTime);
    
    
    bool commitTeamRequest();
    bool checkInviteCodeRequest(const char* inviteCode);
    bool powerUpRequest();
    bool niepanRequest();
    bool evolutionRequest();
    bool sevoRequest();
    bool wakeRequest();
    bool sellRequest();
    bool freeGachaRequest();
//    bool chargeGachaRequest(bool bSuiTang = false, string strItem = "");
//    
//    bool chargeGachaRequest_Other(int OtherGacha = 0 , string strItem = "");

    bool chargeGachaRequest_Other(int OtherGacha = 0, string strItem = "",long curtime = 0 , bool free = true);
    bool chargeGachaRequest(bool bSuiTang = false, string strItem = "",long curtime = 0 , bool free = true);
    
    bool multiChargeGachaRequest(string strItem = "",long curtime = 0);
    
  //  bool multiChargeGachaRequest(string strItem = "");
    bool multiFreeGachaRequest();
    bool compGachaConfigRequest();
    bool exchangeCardRequest();
    void setCompGachaNumber(int number);
    bool packExtendRequest();
    bool recoverStaminaRequest();
    bool reviveRequest(long lRty,string strReviveCostId="");
    int getLvlUpCoin();
    long getDgEndGold();
    long getDgEndExp();
    double getDgEndExpEffect();

    bool needToReqCollection();
    void setIsNeedToReqCollection(bool isNeed);
    bool needToReqHelper();
    void setNeedToReqHelper(bool bNeed);
    
    // 公会
    bool createCountryRequest(const char* name, const char* declaration);
    bool setUserVoiceOnOff(const char *uid,bool voiceOn);
    bool changeMemberPingbi(const char* uid,bool pingbi);
    bool setViceLeaderRequest(const char* uid);
    bool deleteViceLeaderRequest(const char* uid);
    bool getMemberApplyListRequest();
    bool getMemberListRequest();
    bool acceptMemberRequest(const char* uid);
    bool refuseMemberRequest(const char* uid);
    bool deleteMemberRequest(const char* uid);
    bool giveUpLeaderRequest(const char* uid);
    bool setDeclarationRequest(const char* declaration);
    bool deleteCountryRequest();
    bool unDeleteCountryRequest();
    bool setNameCardRequest();
    bool useItemCardRename(string strItem);    //setNamePayItemCard
    bool levelUpCountryRequest();
    bool levelUpBuffRequest(int buffType);
    bool assignOneSupplyRequest(const char* mid, long supply);
    bool assignAllSupplyRequest(long supply);
    bool assignOutSupplyRequest(long supply);
    bool assignWeekSupplyRequest(long supply);
    bool searchCountryRequest(const char* cidOrName);
    bool getAllCountriesRequest(int type);
    bool deleteMailRequest(const char* uid, long timeStamp);
    bool deleteMailAllRequest();
    bool sendMailRequest(const char* uid, const char* msg);
    bool sendMailAllRequest(const char* msg);
    bool rcvMailRequest();
    bool quitCountryRequest();
    bool applyCountryRequest(const char* cid);
    bool donateCoinRequest(int coin);
    bool exchangeWelfareRequest(const char* wid);
    bool recoverPowerRequest();
    bool cntryDgCfgReq();
    bool cntryDgRankReq(string dg_id, string dg_type);
    bool cntryDgOpenReq(string dg_id ,bool bIsTongMEnglianShou );
    bool getCntryCurrentInfo();
    bool cntryDgJoinReq(string dg_id, string rtyid ,bool bIsTongMEnglianShou );
    bool cntryDgEndReq(string dg_id, string combo ,bool bIsTongMEnglianShou );
    bool useItemReq(string strItem, long lCnt);
    bool useStoneReq(string strItem, long lCnt);
    
    bool useCompoundReq(string type, string newID);
    
    
    bool getGiftReq(string strCDKey);
    bool useItemAppendCsot(const char *pcItemID, long lCnt);
    
    //聊天室
    bool sendMsg(string strMsg);
    string replaceString(string in_str, string src, string dest);
    
    void startMonitorChatRx();
    void stopMonitorChatRx();
    bool isNewChatRxCame();
    string getChatRxString();
    void pushErrMsgToChatLst();
    bool haveNewMsg();
    
    //称号系统
    bool reqTitleList();
    bool reqSetTitleName(string title_id);
    
    //战利品分配
    bool getJiFengRecordReq();
    bool assignGuildCoin(string strUid, int iCoin);
    
    bool getOrderNo(string productId,string uid);
    //悬赏
    bool reqGuildItemShop();
    bool reqBuyGuildItem(string strItemId, long lNum);
    CCMutableArray<GuildShopItemInf*>* getGuildShopItemInfArr();
    bool postRewardRequest(long lContribution, int iDeadline, int iPerson,
                           int iCoin, string itemId, int itemCount);
    long getGuildItemInStoreCnt(string strItemId);
    
    //领奖
    bool reqGetAwardBonus(string bonus, string award_key,string loginDays = "");
    
    // 发布悬赏
    // lContribution 目标贡献值；iDeadline 悬赏有效时间；iPerson 悬赏可以参加的最大人数；
    // iCoin 悬赏领取需要抵押的元宝数；itemId 悬赏完成可领取的道具卡ID；itemId 道具卡数量；
    bool rewardTaskListRequest();   // 获取悬赏列表
    bool acceptRewardTaskRequest(string taskId);   // 接受悬赏任务 taskId 悬赏项ID
    bool recieveRewardRequest(string taskId);      // 领取任务奖励 taskId 悬赏项ID
    bool rewardMemberListRequest(string taskId);   // 获取某一个悬赏项参加成员详情 taskId 悬赏项ID
    bool rewardTopMemberRequest();      //获取悬赏排名
    
    CCMutableArray<CRwdTopMemberInfo*>* getRwdTopMembers();
    CCountryInfo* getCntryInfo();
    CCMutableArray<CCntryBaseInfo*> *getCntryList();
    CCMutableArray<CFriendInf*> *getMemberApplyList();
    CCMutableArray<CMember*> *getMemberList();
    void changeMemberListRange(bool bChangeRange = true);   // bChangeRange 是否改变排序方式
    CCntryBaseInfo* getSchCntryInfo();
    CBuffInfo* getBuffInfo(enBuffTypeDef buffType, int buffLv);
    CCMutableArray<CWelfareInfo*> *getWelfareInfoList();
    const char* getCntryCurrentInfoURL();
    
    CCMutableArray<CPostRewardBaseInfo*>* getRewardTaskList();  // 悬赏任务列表
    CCMutableArray<CRewardMember*>* getRewardMemberList();      // 某一项悬赏任务参与成员的详情
    
    //签到活动
    CCMutableArray<RwdSignInf*> * getRwdSignInfList();
    CCMutableArray<RwdLevelUpInf*> * getRwdLevelUpArr();
    CCMutableArray<RwdMonthInf*> * getRwdMonthInfoArr();
    int getRwdMonthDay();   // 奖励到第几天
    CCMutableArray<RwdBonusInf*> * getRwdBonusInfArr();
    CCMutableArray<RwdHuodongInf*> * getRwdHuodongInfArr();
    CCMutableArray<RwdBuchangInf*> * getRwdBuchangInfArr();
    //获得活动奖励信息
    RwdHuodongInf* getRwdAwardInfo(Json::Value dic,RwdHuodongInf* pHuodongInf);
    
    //签到活动end

    //请求加好友
    bool addFriendRequest(const char* fid);
    
    //请求自己的好友列表
    bool friendListRequest();
    CCMutableArray<CFriendInf*> * getFriendList();
    
    //接收好友请求
    bool acceptRequest(const char* fid);
    
    
    //更改新手状态
    bool newChangeRequest(const char* str);
    
    //新手引导送武将
    bool newGiveWuJiangRequest(string str);

    
    //拒绝好友请求
    bool refuseRequest(const char* fid);
    
    //通过ID搜索好友
    bool schFriendRequest(const char* fid);
    CFriendInf* getSchUsrInf();
    
    //请求删除好友
    bool delFriendRequest(const char* fid);
    
    //请求获得申请加自己的用户列表
    bool reqlistRequest();
    
    //发送微博的请求
    bool reqWeiboBackRequest();
    CCMutableArray<CFriendInf*> * getInvitersInf();
    
    //获得自己的好友数量
    int getMaxFriendNum();
    int getCurFriendNum();
    
    //请求图鉴
    bool collectionRequest();
    char* getCollection();

    int  getReqStat();
    void clearReqStat();

    //拜师-师傅-列表
    CCMutableArray<CMAARequestMasterInfo*> * getCMAARequestMasterList();
    
    //附加统御力卡
    bool isAppendCost();
    void getGuildAwardText(std::string &szText);
    
    bool sendAppendCostRequest();       //发送请求附加统御力数据
    
    
    bool getUseCompoundResult();
    
    CCMutableArray<CFriendHelpInfo*> * getFriendHelpArr(bool bUpdate = false);
    CCMutableArray<CNmlDgInfo*> * getNmlDgInfo();
    CCMutableArray<CSpecialDgInfo*> * getSpecialDgInfo();
    CCMutableArray<CSangoYYDgInfo*> * getSangoYYDgInfo();
    bool CheckSangoYYExist();
    CCMutableArray<CDgStepInfo*> * getDgStepInfo(bool bUpdate = false);
    const char* getDungeonUid();
    CCMutableArray<CUserCardInfo*> * getUserCards();
    CCMutableArray<CShopCardInfo*> * getShopCards();
    void sortShopCards();       // 武将商店排序，先按星级，从大到小，然后按属性，然后按价格
    CCMutableArray<PushInfo*> * getPushInfo();
    const char* getNiePanResultUcid();
    
    const char* getTradeNo();
    
    CommonInfo *getCommonInfo();
    unsigned short getStaminaRecoverTime();
    unsigned short getActionPointRecoverTime();
    bool getIsUseJiangNanStyle();
    bool isShopHuoDong();
    
    
    
    CCMutableArray<ParcelChildInfo *> * getParcelChildInfoAry();
    CCMutableArray<ParcelItemInfo *> * getParcelItemInfoAry();
    
    
    CUserInfo*  getUsrInfo();
    
    CGachaRet*  getGachaRet();
    
    CCMutableArray<CGachaRet *> * getMultiGachaRet();
    
    int getDefaultCntryBu(int lv);

    
    int  getPwUpSucType();
    
    void resetSelectedHelper();
    void setSelectedHelper(char* fid, int index);
    int getSelectedHelperCount();
    CUserCardInfo* getHelperCardInfo(int index);
    CFriendHelpInfo* getSelectedHelperInfo(int index);
    
    CUserCardInfo* getUsrCardInfByUcid(const char* ucid);
    CCardBaseInfo* getCardBaseInfByCid(unsigned short cid);
    CUserCardInfo* getUsrCardInfByMailInf(CMailSenderInf* pSenderInf);
    CUserCardInfo* getUsrCardInfByRentStr(string& str);

    // 编队操作
    void saveAllTeamInfo();
    void setCurTeamIdx(int idx);
    int getCurTeamIdx();
    bool getIsSameCardInTeam();
    stUid* getTeamInfByIdx(int idx);
    stUid* getCurTeamInf();
    int getCurTeamCost();
    void setTeamInfo(int iTeamIdx, int iMemIdx, char *ucid);
    void updateTeamInfo(); // 如果进化目标在编队中，进化完成后要更新编队
    bool isOneOfTeam(char* ucid);
    unsigned short getCurCostSum();
    unsigned short getMaxCostSum();
    unsigned short getTeamMaxStar();
    unsigned short getFriendMaxStar();  //友军武将最大星
    
    //是否在指定队伍中
    bool isInTeamForIdx(char* ucid, int idx);
    void resetTeam(int iTeamIdx);
    bool hasSameUcidInTeamInfo(); // 检查编队中的武将ucid是否有重复
    void setUseTmpTeam(bool bUse);
    bool getIsUseTmpTeam();
    stUid* getTmpTeamInf();
    void setPreCmtTeamInfo(int iMemIdx, char *ucid, int cost);
    void clrPreCmtTeamInfo();
    void setCostCoin(long lCostCoin);
    
    int*  getTrophies();
    CCMutableArray<GuildItemAward*> * getGuildItemAwardArr();
    
    const char* getErrMsg();

    bool    getIsNiePanFoodInPackage();
    bool    getIsNiePanFoodByCid(short cid);
    const char* getUcidFromCombineByCid(short cid); // cid是否在combine数组里，返回对应的ucid
    void    setCombineQueue(int idx, char* uid);
    char*   getCombineCard(int idx);
    void    clearCombineQueue();
    int     getCombineCardCnt();
    
    unsigned long getExpByType(char type, int lv);
    
    void    setCidBeforeEvo(int cid);
    int     getCidBeforeEvo();
    void    setCidAfterEvo(int cid);
    int     getCidAfterEvo();
    
    void   setSoundValue(bool value);
    void   setMusicValue(bool value);
    void   setSkillValue(bool value);
    
    
    void setCntryCdRemindValue(bool value);
    void setActivityRemindValue(bool value);
    void setTwoupRemindValue(bool value);
    void setPowerRemindValue(int value);
    void RemindReq();

    //shop
    void loadStore();
    bool getShopRequest();
    bool sendPaySuccessInfo();
    void payForCoin(int index);
    void setPayResponse(void (*payResponse)(int,const char *,int));
    void finishPayment();
    int  getPurchaseStatus();
    void resetPurchaseStatus();
    int  getTapjoyStatus();
    void resetTapjoyStatus();
    int  getProductCnt();
    const char* getIapIdAtIdx(int idx);
    const char* getPriceAtIdx(int idx);
    const char* getTitleAtIdx(int idx);
    bool isValidTransId();
    
    //获得当前时间
    char* getCountDownTimeStr(long time);
    char* getHourCountDownTimeStr(long time);
    int getAPCountDownTime();
    int getCountDownTime();;
    
    enTimeCompareResult timeCompare();

    //新手
    bool isNewbie();
    int  getNewbieStep();
    
    bool chkLogin();
    
    //邀请码奖励
    const char* getNoticeInviteAward();
    
    //公告
    const char* getNoticeString();
    //礼物公告
    const char* getNoticeGiftString();
    //帮助履历
    const char* getLendInfo();

    //有效的宝石信息
    CAvailDiamond* getCurAvailDiamond();
    void setCurAvailDiamond(char cCnt, char* pBuf);
    void setDiamondAllAvail();

    //本地行动力
    int getLocalAcPoint();
    void setLocalAcPoint(int acPoint);
    
    //本地体力值
    int getLocalStamina();
    void setLocalStamina(int stamina);
    
    //Appstore评价
    void maybeGotoAppstoreWeb();
    
    
    //获取设备对应url
    const char* getUrlDepDevice(const char *url);
    
    //保存之前的cardInfo
    CUserCardInfo preUpCardInfo;

    void setDefaultRangeType(enCardRangeType type);
    enCardRangeType getDefaultRangeType();
    //是否有新的特殊战场任务
    bool getIsNewSpecialRoomExist();
    
    // 读取游戏进度保存状态下，判断是否初次通关
    bool isFirstPassBySaveData(int curFloorStatus, const char* dg_type, int iCurFloor);
    
    //获得compaGacha时间
    const char  *getCompGachaTime();
    const char *getWeiboBackStr();
    ExchangeCardInfo *getExchangeCardInfo();
    bool isPaying();
    int  getStoreStatus();
    
    void setIsFighting(bool bFighting);
    bool isFighting();
    
    bool getIsLock(const char* ucid);
    void setIsLock(const char* ucid, bool isLock);
    
    const char *getLanguageValue(const char *languageKey); // 根据key获取语言包中对应的语言
    void clearUserDefaultData();                      // 清除本地保存的数据，用于切换账号
    void clearLoginDefaultData();                       // 清除本地保存的登入数据，用于重新绑定账号
    void getCardHeadFileName(unsigned short ushCardID,char *pchBuf,unsigned int unLength);
    CCSprite* getHeadSprite(unsigned short cardId);     // 根据cid获得对应的head 精灵
    const char *getBodyFileName(unsigned short cardId);     // 根据cid获得对应的body文件名字
    CCSprite * getBodySprite(unsigned short cardId);
    
    void setCurFlrStatus(int istat);
    int  getCurFlrStatus();
    
    CCMutableArray<CompGachaInfo *> *getCompGachaInfo();
    void clearSessionId();

    void updateCoin();
    
    bool isIapBestSell(const char* iap_id);
    bool isIapValuePack(const char* iap_id);

    // 圣诞节开关
    void setIsUseChristmasUI(bool bUseChristmasUI);
    bool getIsUseChristmasUI();
    
    //好友排序
    enFriendRange  getDefaultFrRangeType();
    void      setDefaultFrRangeType(enFriendRange type);
    
    // 是否使用邀请码功能
    void setIsUseInviteCode(bool bUseInviteCode);
    bool getIsUseInviteCode();
    
    //compGacha显示帮助的开关
    bool getIsFirstShowCompGachaHelp();
    void setIsFirstShowCompGachaHelp(bool isFirst);
    
    // 是否已经进入过帮助
    bool isEnterHelp();
    void setIsEnterHelp();
    
    // 是否能录制视频
    bool CheckRecordVideo(const char* dgType, int floorId, int roomId);
    
    // 是否已经打开过国家帮助说明
    bool getHasShowCntryNotice();
    void setHasShowCntryNotice();
    
    //设置是否刷新common info
    void setUpdateCom(bool bUpdate);
    
    //获得app的更新地址
    
    const char *getAppUrl();
    
    void setGachaCardName(const char *name);
    
    const char *getGachaCardName();
    
    void loadUserCardsSprite();
    void releaseUserCardsSprite();
    bool loadUserCardsStep(int iCount);
    void resetLoadCardStep();
    
    const char* getBundleId();
    const char* getBundleName();
    
    const char* getMD5Str();
    
    const char* getSessionId();
    void setSessionId(const char* sessionId);
    
    const char* getUserId();
    void setUserId(const char* userId);
    
    void setFrndLstReqTime(long lTime);
    void setRcvMailReqTime();
    
    CCMutableArray<CCntryDgInfo* > * getCntryDgInfArr();
    void sortCntryDgInfArr();
    
    enMemberRange getMemberRangeType();
    
    string getCntryDgRankStr();
    
    string getCntryBossLastKillName();
    CntryDgEndInf* getCntryDgEndInf();
    
    long getNmlDgEndGuildAward();
    
    void initAfterLogin();
        
    string getCurDgType();
    int getCurFloor();
    int getCurRoom();
    
    bool needToUpdateCfg();
    bool needToUpdateCard();
    bool needToUpdateMonster();
    
    int getSpecialDgTimes(int iFloorId);
    
    CCMutableArray<CCntryDgRankInfo* > * getCntryDgRankInfArr();
    CCMutableArray<CSpecialDgRankInfo* > * getSpecialDgRankInfArr(string dgType);
    int getMySpecialDgRank(string dgType);
    CSpecialDgRankInfo* getMySpecialDgRankInf(string dgType);// dgType 1 : 疾风; 2 : 乱舞

    long getMyDamageFromRankInf();
    
    int  getDgFloorStatus(const char* dg_type, int floorid);

    unsigned long getLastWelfareTime(const char* welfareId);

    CCMutableArray<ItemCardInf* > * getItemCardTotalInf();
    CCMutableArray<ItemCardInf* > * getItemCardPkInf();
    GiftInfo *getGiftInfo();
    
    ItemCardInf* getItemCardByID(string strItemId);

    void setNameCard(const char* name);
    const char* getNameCard();
    
    UseItemAward* getUseItemAward();

    CCMutableArray<TitleInf* > * getTitleInfArr();
    
    CCMutableArray<JiFengRecordInf* > * getJiFengRecordArr();
    
    long getGuildCoin();
    
    void checkIsUseExternal();
    
    // 上传视频
    bool GetUploadAward(ConfigData data);
    string GetUploadedAward();
    string GetFightName(int floor_id, int room_index, const char* type, int floor_index);
    
    // 查看其他人的视频
    bool ReqOtherRecord(const char* dungeon_id, const char* type);
    string GetOtherRecordUrlVid();
    
    //阅读免责声明
    bool readMzsm();
    
    void setLocalChatTime(long time);
    
    long getLocalChatTime();
    
    string getFirstXChars(string strInput, uint16_t uiCnt);
    
    void addBigImageByRGBA4444(const char* fileName);
    
    string getPPBilloNo();//从服务器获取pp助手订单号
    bool   reqPPBilloNo(const char* item_id);
    
private:
    //forbid constructor being called outside
	CGameData();
    
	//forbid copy constructor being called outside
	CGameData(const CGameData& );
	
	//forbid assignment
	CGameData& operator=(const CGameData& );
        
    unsigned long getEchoTime(string date); // 获取格式化时间
    
    long getSmallDicTime(enSmallDicId smallDicId);
    
    void chkSpecDgNeedCards(CSpecialDgInfo* , Json::Value);
    
    void chkEvoMaterial();
    void chkSuperEvoMaterial();
    void chkWakeMaterial();
    
    void savePayTrans();
    void rmvPayTrans();
    
};

#endif
