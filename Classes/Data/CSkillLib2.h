//
//  CSkillLib2.h
//  AgainstWar
//
//  Created by user on 13-10-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CSkillLib2__
#define __AgainstWar__CSkillLib2__

#include "cocos2d.h"
#include "CGameData.h"
#include "CSkillLib.h"

//--能力突破-技能--

//觉醒技类别
enum enAwakeSkillTp{
    enAwakeSkillTp_null = 0,
    enAwakeSkillTp_valChg_hp,       //1 数值改变(hp)
    enAwakeSkillTp_valChg_atk,      //2 数值改变(attack)
    enAwakeSkillTp_valChg_rcv,      //3 数值改变(recover)
    enAwakeSkillTp_atk,             //4 连成横行6颗珠的对应属性攻击力+10%，但是如果是心珠的话，封印时间额外减少1回合
    enAwakeSkillTp_plus,            //5 强化  20%几率掉落对应属性＋宝珠
    enAwakeSkillTp_auto_rcv,        //6 回合内有消珠的话，回血
    enAwakeSkillTp_armor_fengyin,   //7 封印耐性
    enAwakeSkillTp_fengyin_rcv,     //8 封印回复
    enAwakeSkillTp_zhuanzhu_time,   //9 转珠时间延长
    enAwakeSkillTp_cd_dec,          //10 进入关卡时,队伍全体技能冷却减少
    enAwakeSkillTp_jianshang,       //11 属性伤害减少
    enAwakeSkillTp_anannaixing,     //12 暗暗耐性
    enAwakeSkillTp_wuxiaonai,       //13 无效耐性
    enAwakeSkillTp_posion_nai,      //14 毒耐性
    enAwakeSkillTp_erti_atk,        //15 二体攻击
    enAwakeSkillTp_erci_atk,        //16 有几率再攻击一次（攻击数值和第一次一样，包括群攻）
    enAwakeSkillTp_huihe_dec,       //17 威震天下,进入关卡时震退所有敌将1回合
    enAwakeSkillTp_shensu_miss,     //18 神速 有几率闪避敌将的普通攻击(血条位置显示“MISS”)
    enAwakeSkillTp_xianzhinaixing,  //19 先制攻击耐性
    enAwakeSkillTp_armor_fengyin_all,//20 敌将针对全队的封印技能有几率无效化
    enAwakeSkillTp_delay,           //21 消除X宝珠有几率延迟敌将攻击X回合
};

//觉醒技能
typedef struct
{
    int     iType;          //技能分类
    char    cElement;       //技能属性 参考enElementDef
    char    cGenTp;         //武将类型
    char    cOdds;          //发生几率 cOdds/100
    float   fEffect;        //技能效果
    int     param1;         //参数1
}stAwakeSkill;

enum enAwakeSkillID
{
    enAwakeSkill_NULL = 0,
    enAwakeSkill_1,                   //1 血量强化 血量+100
    enAwakeSkill_2,                   //2 攻击强化 攻击+50
    enAwakeSkill_3,                   //3 回复强化 ＋30
    enAwakeSkill_4,                   //4 连成横行6颗火珠的话，火属性攻击力+10%
    enAwakeSkill_5,                   //5 连成横行6颗水珠的话，水属性攻击力+10%
    enAwakeSkill_6,                   //6 连成横行6颗木珠的话，木属性攻击力+10%
    enAwakeSkill_7,                   //7 连成横行6颗光珠的话，光属性攻击力+10%
    enAwakeSkill_8,                   //8 连成横行6颗暗珠的话，心属性攻击力+10%
    enAwakeSkill_9,                   //9 封印回复 连成横行6颗心属性宝珠的话，封印时间额外减少1回合...(问题)
    enAwakeSkill_10,                  //10 20%几率会掉落水属性+宝珠
    enAwakeSkill_11,                  //11 20%几率会掉落火属性+宝珠
    enAwakeSkill_12,                  //12 20%几率会掉落木属性+宝珠
    enAwakeSkill_13,                  //13 20%几率会掉落光属性+宝珠
    enAwakeSkill_14,                  //14 20%几率会掉落暗属性+宝珠
    enAwakeSkill_15,                  //15 自动回复 回合内有消珠的话，回复血量500
    enAwakeSkill_16,                  //16 封印耐性 敌将针对本武将的封印技能50%几率无效...
    enAwakeSkill_17,                  //17 时间延长 转珠时间延长0.5秒
    enAwakeSkill_18,                  //18 技能冷却提升 进入关卡时，队伍全体的技能冷却减少1回合
    enAwakeSkill_20,                  //19 水伤害减轻:水伤害-2%
    enAwakeSkill_19,                  //20 火伤害减轻:火伤害-2%
    enAwakeSkill_21,                  //21 木伤害减轻:木伤害-2%
    enAwakeSkill_22,                  //22 光伤害减轻:光伤害-2%
    enAwakeSkill_23,                  //23 暗伤害减轻:暗伤害-2%
    enAwakeSkill_24,                  //24 暗暗耐性:敌将释放的烟幕类技能20%几率无效
    enAwakeSkill_25,                  //25 无效块耐性:敌将使宝珠变无效块的技能20%几率无效
    enAwakeSkill_26,                  //26 毒耐性:敌将释放的中毒类技能20%几率无效
    enAwakeSkill_27,                  //27 二体攻击:连成4颗对应属性宝珠的话，本武将会同时攻击2个敌人
    enAwakeSkill_28,                  //28 破坏神:有几率再攻击一次（攻击数值和第一次一样，包括群攻）
    enAwakeSkill_29,                  //29 威震天下:进入关卡时震退所有敌将1回合
    enAwakeSkill_30,                  //30 神速:有几率闪避敌将的普通攻击(血条位置显示“MISS”)
    enAwakeSkill_31,                  //31 烟幕无效:使敌将的烟幕类技能无效
    enAwakeSkill_32,                  //32 西凉铁壁:有几率使敌将的先制攻击类技能无效
    enAwakeSkill_33,                  //33 骠骑雄风:敌将针对全队的封印技能有几率无效化
    enAwakeSkill_34,                  //34 锦面银枪:消除光宝珠有几率延迟敌将攻击1回合
    enAwakeSkill_35,                  //35 仁德之主:回合内有宝珠消珠，回复血量3000
    
    enAwakeSkill_Max
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CSkillLib2
{
    static CSkillLib2* s_pcIf;
    
public:
	//Get the instance pointer
	static CSkillLib2* Ins()
    {
        if(s_pcIf == NULL){
            s_pcIf = new CSkillLib2();
        }
        
        return s_pcIf;
    }
    
    stAwakeSkill* getAwakeSkill(int iID);
    unsigned short getNowAwakeSkillSum();
//    stAwakeSkill ** getAwakeSkillList();
    
    
    
private:
    //forbid constructor being called outside
	CSkillLib2(){};
    
	//forbid copy constructor being called outside
	CSkillLib2(const CSkillLib2& );
};


#endif /* defined(__AgainstWar__CSkillLib2__) */
