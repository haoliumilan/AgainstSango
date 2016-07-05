//
//  CSkillLib2.cpp
//  AgainstWar
//
//  Created by user on 13-10-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "CSkillLib2.h"

CSkillLib2* CSkillLib2::s_pcIf = NULL;

static stAwakeSkill gAwakeSkillInfo[] =
{
      //技能分类  技能属性  武将类型    发生几率   技能效果   参数1
    {0, 0, 0, 0, 0, 0},
    {enAwakeSkillTp_valChg_hp, enElementMax, enGeneralTp_Null, 100, 100, 0},       //1 血量强化 血量+100
    {enAwakeSkillTp_valChg_atk, enElementMax, enGeneralTp_Null, 100, 50, 0},       //2 攻击强化 攻击+50
    {enAwakeSkillTp_valChg_rcv, enElementMax, enGeneralTp_Null, 100, 30, 0},       //3 回复强化 回复+30
    {enAwakeSkillTp_atk, enElementFire, enGeneralTp_Null, 100, 0.1f, 0},           //4 连成横行6颗火珠的话，火属性攻击力+10%
    {enAwakeSkillTp_atk, enElementWater, enGeneralTp_Null, 100, 0.1f, 0},          //5 连成横行6颗水珠的话，水属性攻击力+10%
    {enAwakeSkillTp_atk, enElementWood, enGeneralTp_Null, 100, 0.1f, 0},           //6 连成横行6颗木珠的话，木属性攻击力+10%
    {enAwakeSkillTp_atk, enElementLight, enGeneralTp_Null, 100, 0.1f, 0},          //7 连成横行6颗光珠的话，光属性攻击力+10%
    {enAwakeSkillTp_atk, enElementDark, enGeneralTp_Null, 100, 0.1f, 0},           //8 连成横行6颗暗珠的话，心属性攻击力+10%
    {enAwakeSkillTp_plus, enElementHeart, enGeneralTp_Null, 20, 0, 0},             //9 封印回复 连成横行6颗心属性宝珠的话，封印时间额外减少1回合
    {enAwakeSkillTp_plus, enElementWater, enGeneralTp_Null, 20, 0, 0},             //10 20%几率会掉落水属性+宝珠
    {enAwakeSkillTp_plus, enElementFire, enGeneralTp_Null, 20, 0, 0},              //11 20%几率会掉落火属性+宝珠
    {enAwakeSkillTp_plus, enElementWood, enGeneralTp_Null, 20, 0, 0},              //12 20%几率会掉落木属性+宝珠
    {enAwakeSkillTp_plus, enElementLight, enGeneralTp_Null, 20, 0, 0},             //13 20%几率会掉落光属性+宝珠
    {enAwakeSkillTp_plus, enElementWood, enGeneralTp_Null, 20, 0, 0},              //14 20%几率会掉落暗属性+宝珠
    
    {enAwakeSkillTp_auto_rcv, enElementWater, enGeneralTp_Null, 100, 500, 0},      //15 自动回复 回合内有消珠的话，回复血量500
    {enAwakeSkillTp_armor_fengyin, enElementMax, enGeneralTp_Null, 50, 0, 0},      //16 封印耐性 敌将针对本武将的封印技能50%几率无效
    {enAwakeSkillTp_zhuanzhu_time, enElementMax, enGeneralTp_Null, 100, 0.5f, 0},  //17 时间延长 转珠时间延长0.5秒
    {enAwakeSkillTp_cd_dec, enElementMax, enGeneralTp_Null, 100, 1, 0},            //18 技能冷却提升 进入关卡时，队伍全体的技能冷却减少1回合
    
    {enAwakeSkillTp_jianshang, enElementWater,enGeneralTp_Null, 100, 0.02f, 0},    //19 水伤害减轻:水伤害-2%
    {enAwakeSkillTp_jianshang, enElementFire, enGeneralTp_Null, 100, 0.02f, 0},    //20 火伤害减轻:火伤害-2%
    {enAwakeSkillTp_jianshang, enElementWood, enGeneralTp_Null, 100, 0.02f, 0},    //21 木伤害减轻:木伤害-2%
    {enAwakeSkillTp_jianshang, enElementLight, enGeneralTp_Null, 100, 0.02f, 0},   //22 光伤害减轻:光伤害-2%
    {enAwakeSkillTp_jianshang, enElementDark, enGeneralTp_Null, 100, 0.02f, 0},    //23 暗伤害减轻:暗伤害-2%
    {enAwakeSkillTp_anannaixing, enElementMax, enGeneralTp_Null, 20, 0.0f, 0},     //24 烟幕耐性:敌将释放的烟幕类技能20%几率无效
    {enAwakeSkillTp_wuxiaonai, enElementMax, enGeneralTp_Null, 20, 0.0f, 0},       //25 无效块耐性:有20%率使敌将的无效块类技能无效
    {enAwakeSkillTp_posion_nai, enElementMax, enGeneralTp_Null, 0, 0.0f, 0},       //26 毒耐性:敌将释放的中毒类技能20%几率无效
    {enAwakeSkillTp_erti_atk, enElementMax, enGeneralTp_Null, 0, 0.0f, 0},         //27 二体攻击:连成4颗对应属性宝珠的话，本武将会同时攻击2个敌人
    {enAwakeSkillTp_erci_atk, enElementMax, enGeneralTp_Null, 15, 0.0f, 0},        //28 破坏神:有几率再攻击一次（攻击数值和第一次一样，包括群攻）
    {enAwakeSkillTp_huihe_dec, enElementMax, enGeneralTp_Null, 0, 0.0f, 0},        //29 威震天下:进入关卡时震退所有敌将1回合
    {enAwakeSkillTp_shensu_miss, enElementMax, enGeneralTp_Null, 15, 0.0f, 0},     //30 神速:有几率闪避敌将的普通攻击(血条位置显示“MISS”)
    {enAwakeSkillTp_anannaixing, enElementMax, enGeneralTp_Null, 100, 0.0f, 0},    //31 烟幕无效:使敌将的烟幕类技能无效
    {enAwakeSkillTp_xianzhinaixing, enElementMax, enGeneralTp_Null, 25, 0.0f, 0},  //32 西凉铁壁:有25%率使敌将的先制攻击类技能无效
    {enAwakeSkillTp_armor_fengyin_all, enElementMax, enGeneralTp_Null, 30, 0, 0},  //33 骠骑雄风:敌将针对全队的封印技能有30%率无效化
    {enAwakeSkillTp_delay, enElementLight, enGeneralTp_Null, 15, 1, 0},            //34 锦面银枪:消除光宝珠有15%率延迟敌将攻击1回合
    {enAwakeSkillTp_auto_rcv, enElementMax, enGeneralTp_Null, 100, 3000, 0},      //35 仁德之主 回合内有宝珠消珠，回复血量3000
    
    
};

stAwakeSkill* CSkillLib2::getAwakeSkill(int iID)
{
    if(iID < enAwakeSkill_Max){
        return &gAwakeSkillInfo[iID];
    }
    else{
        return NULL;
    }
}

unsigned short CSkillLib2::getNowAwakeSkillSum()
{
    return sizeof(gAwakeSkillInfo)/sizeof(gAwakeSkillInfo[0]);
}

//stAwakeSkill ** CSkillLib2::getAwakeSkillList()
//{
//    return gAwakeSkillInfo;
//}





