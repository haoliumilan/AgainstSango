//
//  CSkillLib.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-5-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CSkillLib.h"

CSkillLib* CSkillLib::s_pcIf = NULL;

#pragma mark -
#pragma mark gManualSkillInfo
//主动技能
static stManualSkill gManualSkillInfo[] = {
    //冷却回合  最大lv  目标个数      技能属性   效果基数   效果   持续回合  技能类别
    {0,         0,      0,              0,  0,      0,      0,      enManSkillTp_null},
    // 吐息
    {10,        6,      1,  enElementFire,  1,      3,      0,      enManSkillTp_atk},    //1 灼热吐息 自身攻击力3倍的火属性全体攻击
    {20,        11,     1,  enElementFire,  1,      5,      0,      enManSkillTp_atk},    //2 火焰吐息 自身攻击力5倍的火属性全体攻击
    {30,        21,     1,  enElementFire,  1,      20,     0,      enManSkillTp_atk},    //3 熔岩吐息 自身攻击力20倍的火属性全体攻击

    {10,        6,      1,  enElementWater, 1,      3,      0,      enManSkillTp_atk},    //4 寒冷吐息 自身攻击力3倍的水属性全体攻击
    {20,        11,     1,  enElementWater, 1,      5,      0,      enManSkillTp_atk},    //5 寒冰吐息 自身攻击力5倍的水属性全体攻击
    {30,        21,     1,  enElementWater, 1,      20,     0,      enManSkillTp_atk},    //6 暴雪吐息 自身攻击力20倍的水属性全体攻击

    {10,        6,      1,  enElementWood,  1,      3,      0,      enManSkillTp_atk},    //7 烈风吐息 自身攻击力3倍的木属性全体攻击
    {20,        11,     1,  enElementWood,  1,      5,      0,      enManSkillTp_atk},    //8 大地吐息 自身攻击力5倍的木属性全体攻击
    {30,        21,     1,  enElementWood,  1,      20,     0,      enManSkillTp_atk},    //9 盖亚吐息 自身攻击力20倍的木属性全体攻击

    {10,        6,      1,  enElementLight, 1,      3,      0,      enManSkillTp_atk},    //10 闪光吐息 自身攻击力3倍的光属性全体攻击
    {20,        11,     1,  enElementLight, 1,      5,      0,      enManSkillTp_atk},    //11 雷电吐息 自身攻击力5倍的光属性全体攻击
    {30,        21,     1,  enElementLight, 1,      20,     0,      enManSkillTp_atk},    //12 神圣吐息 自身攻击力20倍的光属性全体攻击

    {10,        6,      1,  enElementDark,  1,      3,      0,      enManSkillTp_atk},    //13 黑暗吐息 自身攻击力3倍的暗属性全体攻击
    {20,        11,     1,  enElementDark,  1,      5,      0,      enManSkillTp_atk},    //14 混乱吐息 自身攻击力5倍的暗属性全体攻击
    {30,        21,     1,  enElementDark,  1,      20,     0,      enManSkillTp_atk},    //15 地狱吐息 自身攻击力20倍的暗属性全体攻击

    // 炸弹
    {30,        21,     0,  enElementMax,   1,      10,     0,      enManSkillTp_atk},    //16 炸弹 自身攻击力10倍的伤害
    {30,        21,     0,  enElementMax,   1,      15,     0,      enManSkillTp_atk},    //17 能量炸弹 自身攻击力15倍的伤害
    {10,        8,      0,  enElementMax,   1,      10,     0,      enManSkillTp_atk},    //18 迅捷炸弹 自身攻击力10倍的伤害，附加20%的几率使敌人攻击延长2回合
    {30,        16,     0,  enElementMax,   1,      50,     0,      enManSkillTp_atk},    //19 爆裂炸弹 对敌方单体造成自身攻击力50倍的伤害，伤害属性与自身属性相同，会受到敌人的属性和防御的影响（原有的30倍调整为50倍，不修改ID）

    //能量球
    {10,        6,      1,  enElementFire,  1000,   1,      0,      enManSkillTp_atk},    //20 热能球 造成1000点火属性全体伤害
    {25,        16,     1,  enElementFire,  3000,   1,      0,      enManSkillTp_atk},    //21 火焰球 造成3000点火属性全体伤害

    {10,        6,      1,  enElementWater, 1000,   1,      0,      enManSkillTp_atk},    //22 寒冷球 造成1000点水属性全体伤害
    {25,        16,     1,  enElementWater, 3000,   1,      0,      enManSkillTp_atk},    //23 冰霜球 造成3000点水属性全体伤害

    {10,        6,      1,  enElementWood,  1000,   1,      0,      enManSkillTp_atk},    //24 烈风球 造成1000点木属性全体伤害
    {25,        16,     1,  enElementWood,  3000,   1,      0,      enManSkillTp_atk},    //25 大地球 造成3000点木属性全体伤害

    {10,        6,      1,  enElementLight, 1000,   1,      0,      enManSkillTp_atk},    //26 闪光球 造成1000点光属性全体伤害
    {25,        16,     1,  enElementLight, 3000,   1,      0,      enManSkillTp_atk},    //27 雷电球 造成3000点光属性全体伤害

    {10,        6,      1,  enElementDark,  1000,   1,      0,      enManSkillTp_atk},    //28 黑暗球 造成1000点暗属性全体伤害
    {25,        16,     1,  enElementDark,  3000,   1,      0,      enManSkillTp_atk},    //29 混乱球 造成3000点暗属性全体伤害

    //重力
    {25,        11,     1,  enElementMax,   2,      0.15,   0,      enManSkillTp_gravity},    //30 重力术 敌方全体减少15%血量
    {30,        16,     1,  enElementMax,   2,      0.3,    0,      enManSkillTp_gravity},    //31 十亿重力 敌方全体减少30%血量

    //毒
    {15,        6,      1,  enElementMax,   1,      1,      0,      enManSkillTp_poison},    //32 淬毒 使敌方全体中淬毒(100%)
    {20,        6,      1,  enElementMax,   1,      2,      0,      enManSkillTp_poison},    //33 猛毒 使敌方全体中猛毒
    {20,        6,      1,  enElementMax,   1,      5,      0,      enManSkillTp_poison},    //34 蝕骨劇毒 使敌方全体中剧毒

    //延时
    {15,        6,      1,  enElementMax,   4,      3,      3,      enManSkillTp_delay},    //35 威吓 敌人攻击延长3回合
    {25,        6,      1,  enElementMax,   4,      5,      5,      enManSkillTp_delay},    //36 威压 敌人攻击延长5回合

    //破防
    {15,        8,      1,  enElementMax,   5,      0.5,    5,      enManSkillTp_break},    //37 破甲 一定回合内，减少敌人防御力
    {17,        8,      1,  enElementMax,   5,      0.75,   5,      enManSkillTp_break},    //38 魔焰熔金 一定回合内，敌人防御力大幅下降

    //恢复
    {10,        6,      1,  enElementMax,   300,    1,      0,      enManSkillTp_recover},    //39 愈合之光 血量回复300
    {10,        6,      1,  enElementMax,   500,    1,      0,      enManSkillTp_recover},    //40 回复之光 血量回复500
    {10,        6,      1,  enElementMax,   2000,   1,      0,      enManSkillTp_recover},    //41 大回复之光 血量回复2000
    {20,        11,     1,  enElementMax,   0xFFFFFF, 1,      0,      enManSkillTp_recover},    //42 完治之光 血量完全回复
    {10,        6,      1,  enElementMax,   6,      5,      0,      enManSkillTp_recover},    //43 治疗 回复自身回复力5倍的血量
    {20,        11,     1,  enElementMax,   6,      10,     0,      enManSkillTp_recover},    //44 治愈 回复自身回复力10倍的血量

    //加仿
    {20,        6,      1,  enElementMax,   7,      0.5,    3,      enManSkillTp_armor},    //45 防御姿态 在3回合内，所受伤害减少
    {20,        11,     1,  enElementMax,   7,      0.5,    5,      enManSkillTp_armor},    //46 铁壁姿态 在5回合内，所受伤害减少

    //寶石変化系
    {11,        7,      0,  enElementHeart | 0xF0,   enElementFire | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //47 破釜沉舟-火 恢复宝石变成火宝石
    {11,        7,      0,  enElementHeart | 0xF0,   enElementDark | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //48 破釜沉舟-暗 恢复宝石变成暗宝石
    {11,        7,      0,  enElementFire | 0xF0,   enElementHeart | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //49 步步爲營-水 火宝石变成恢复宝石
    {11,        7,      0,  enElementDark | 0xF0,   enElementHeart | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //50 步步爲營-光 暗宝石变成恢复宝石
    {9,         5,      0,  enElementWater | 0xF0,   enElementFire | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //51 偷天換日-火 水宝石变成火宝石
    {9,         5,      0,  enElementWood | 0xF0,   enElementWater | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //52 偷天換日-水 木宝石变成水宝石
    {9,         5,      0,  enElementFire | 0xF0,    enElementWood | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //53 偷天換日-木 火宝石变成木宝石
    {9,         5,      0,  enElementDark | 0xF0,   enElementLight | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //54 偷天換日-光 暗宝石变成光宝石
    {9,         5,      0,  enElementLight | 0xF0,   enElementDark | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //55 偷天換日-暗 光宝石变成暗宝石
    
    {11,        7,      0,  0xFF,      0xFF,    0,  0,  enManSkillTp_stonerand},    //56 背水一戰 所有宝石随机变化
    
    {15,        6,     0,  enElementMax,      enElementMax,  5.0,  0,  enManSkillTp_luanshi},    //57 亂世 在5秒内，可以任意挪动宝石
    {20,        7,     0,  enElementMax,      enElementMax, 10.0,  0,  enManSkillTp_luanshi},    //58 亂世天下 在10秒内，可以任意挪动宝石
    
    {20,        11,     1,  enElementMax,   2,      0.15,    0,      enManSkillTp_null},         //59 刮骨疗伤 血量完全恢复，并且减少敌方全体15%血量
    
    //寶石変化系2
    {11,        7,      0,  enElementHeart | 0xF0,   enElementWater | 0xF0,   0,  0,  enManSkillTp_stonetrans},    //60 破釜沉舟-水 所有回复宝石变化成水宝石
    {11,        7,      0,  enElementHeart | 0xF0,   enElementWood | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //61 破釜沉舟-木 所有回复宝石变化成木宝石
    {11,        7,      0,  enElementHeart | 0xF0,   enElementLight | 0xF0,   0,  0,  enManSkillTp_stonetrans},    //62 破釜沉舟-光 所有回复宝石变化成光宝石
    {11,        7,      0,  enElementWood | 0xF0,    enElementHeart | 0xF0,   0,  0,  enManSkillTp_stonetrans},    //63 步步爲營-火 所有木宝珠变化成回复宝珠
    {11,        7,      0,  enElementWater | 0xF0,   enElementHeart | 0xF0,   0,  0,  enManSkillTp_stonetrans},    //64 步步爲營-木 所有水宝珠变化成回复宝珠
    {11,        7,      0,  enElementLight | 0xF0,   enElementHeart | 0xF0,   0,  0,  enManSkillTp_stonetrans},    //65 步步爲營-暗 所有光宝珠变化成回复宝珠
    
    //猛攻
    {15,        6,      1,  enElementWater,     20000,   1,      0,      enManSkillTp_atk},    //66 猛攻水 对敌方全体造成20000点水属性伤害
    {15,        6,      1,  enElementFire,      20000,   1,      0,      enManSkillTp_atk},    //67 猛攻火 对敌方全体造成20000点火属性伤害
    {15,        6,      1,  enElementWood,      20000,   1,      0,      enManSkillTp_atk},    //68 猛攻木 对敌方全体造成20000点木属性伤害
    {15,        6,      1,  enElementLight,     20000,   1,      0,      enManSkillTp_atk},    //69 猛攻光 对敌方全体造成20000点光属性伤害
    {15,        6,      1,  enElementDark,      20000,   1,      0,      enManSkillTp_atk},    //70 猛攻暗 对敌方全体造成20000点暗属性伤害
    
    //加农炮
    {20,        6,      1,  enElementWater,     30000,   1,      0,      enManSkillTp_atk},    //71 30000水伤害 对敌方全体造成30000点水属性伤害
    {20,        6,      1,  enElementFire,      30000,   1,      0,      enManSkillTp_atk},    //72 30000火伤害 对敌方全体造成30000点火属性伤害
    {20,        6,      1,  enElementWood,      30000,   1,      0,      enManSkillTp_atk},    //73 30000木伤害 对敌方全体造成30000点木属性伤害
    {20,        6,      1,  enElementLight,     30000,   1,      0,      enManSkillTp_atk},    //74 30000光伤害 对敌方全体造成30000点光属性伤害
    {20,        6,      1,  enElementDark,      30000,   1,      0,      enManSkillTp_atk},    //75 30000暗伤害 对敌方全体造成30000点暗属性伤害
    
    //天空龙主动技
    {10,        6,      1,  enElementWater,     1,      5,      0,      enManSkillTp_atk},      //76 冰刃舞 对敌方全体造成自身攻击力5倍的水属性伤害
    {10,        6,      1,  enElementFire,      1,      5,      0,      enManSkillTp_atk},      //77 爆炎击 对敌方全体造成自身攻击力5倍的火属性伤害
    {10,        6,      1,  enElementWood,      1,      5,      0,      enManSkillTp_atk},      //78 岩铁碎 对敌方全体造成自身攻击力5倍的木属性伤害
    {10,        6,      1,  enElementLight,     1,      5,      0,      enManSkillTp_atk},      //79 雷龙破 对敌方全体造成自身攻击力5倍的光属性伤害
    {10,        6,      1,  enElementDark,      1,      5,      0,      enManSkillTp_atk},      //80 噬魂炮 对敌方全体造成自身攻击力5倍的暗属性伤害
    
    //麻痹
    {20,        6,      1,  enElementFire,      1,      2,      2,      enManSkillTp_mabi},         //81 麻痹(水) 2回合内敌方火属性武将回合不减
    {20,        6,      1,  enElementWood,      1,      2,      2,      enManSkillTp_mabi},         //82 麻痹(火) 2回合内敌方木属性武将回合不减
    {20,        6,      1,  enElementWater,     1,      2,      2,      enManSkillTp_mabi},         //83 麻痹(木) 2回合内敌方水属性武将回合不减
    {20,        6,      1,  enElementDark,      1,      2,      2,      enManSkillTp_mabi},         //84 麻痹(光) 2回合内敌方暗属性武将回合不减
    {20,        6,      1,  enElementLight,     1,      2,      2,      enManSkillTp_mabi},         //85 麻痹(暗) 2回合内敌方光属性武将回合不减
    {20,        6,      1,  enElementMax,       1,      2,      2,      enManSkillTp_mabi},         //86 麻痹(全) 2回合内所有敌方武将回合不减
    
    //无敌
    {20,        6,      1,  enElementFire,      1,      1,      1,      enManSkillTp_wudi},         //87 无敌一回合(水) 1回合内敌方火属性攻击无效
    {20,        6,      1,  enElementWood,      1,      1,      1,      enManSkillTp_wudi},         //88 无敌一回合(火) 1回合内敌方木属性攻击无效
    {20,        6,      1,  enElementWater,     1,      1,      1,      enManSkillTp_wudi},         //89 无敌一回合(木) 1回合内敌方水属性攻击无效
    {20,        6,      1,  enElementDark,      1,      1,      1,      enManSkillTp_wudi},         //90 无敌一回合(光) 1回合内敌方暗属性攻击无效
    {20,        6,      1,  enElementLight,     1,      1,      1,      enManSkillTp_wudi},         //91 无敌一回合(暗) 1回合内敌方光属性攻击无效
    {20,        6,      1,  enElementMax,       2,      1,      1,      enManSkillTp_wudi},         //92 无敌一次伤害(全) 放出一个护盾，能够抵消一回合的攻击伤害，抵消后护盾消失，如果没受到攻击，护盾持续生效，不可叠加
    
    {15,        8,      1,  enElementMax,       6,    0.2,      3,   enManSkillTp_recover},         //93 如沐春风 持续3回合，每回合回复HP总和20%
    {19,       10,      1,  enElementMax,       5,      1,      3,     enManSkillTp_break},         //94 火烧连营 3回合内，所有敌人的防御力变为0
    {25,       11,      0,  enElementMax,       1,     20,      0,       enManSkillTp_atk},         //95 巧变 对敌方单体造成自身攻击力20倍的伤害，并回复相当于伤害值的HP
    {30,       11,      1,  enElementZero,       7,     10,      1,     enManSkillTp_fanji},         //96 绝地反击 下一次受到敌方伤害不会使你死亡，并且反击所受伤害的10倍
    {15,        7,      0,  enElementFire << 4 | enElementWood,   enElementDark << 4 | enElementDark,    0,  0,  enManSkillTp_stonetrans},       //97 冥王的召唤 所有火、木宝石变化成暗宝石
    
    //神之怒
    {35,       16,      1,  enElementMax,       2,      0.35,   0,      enManSkillTp_gravity},      //98 对敌方全体造成35%当前HP的伤害，该伤害无视属性和防御
    
    //宝石变化(火，木-> 心)
    {15,        7,      0,  enElementFire << 4 | enElementWood,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //99 所有火、木宝石变化成回复宝石
    
    //痛恨一击
    {15,        9,     0,  enElementMax,  10000,   1,      0,      enManSkillTp_atk},              //100 根据我方HP百分比对敌人单体造成伤害，100%HP时10000点，50%HP时大约20000点，1HP时100000点，受属性加成和敌人防御的影响
    
    //瘟疫
    {20,        6,      1,  enElementMax,   1,      3,      3,      enManSkillTp_sick},    //101 使敌方全体感染瘟疫，瘟疫状态的敌人每回合损失相当于使用技能武将自身攻击力3倍的HP，持续3回合，该伤害无视属性和防御
    
    //30倍攻击力 超级吐息
    {19,        10,     1,  enElementFire,  1,      30,     0,      enManSkillTp_atk},    //102 超级熔岩吐息 自身攻击力30倍的火属性全体攻击
    
    {19,        10,     1,  enElementWater, 1,      30,     0,      enManSkillTp_atk},    //103 超级暴雪吐息 自身攻击力30倍的水属性全体攻击
    
    {19,        10,     1,  enElementWood,  1,      30,     0,      enManSkillTp_atk},    //104 超级盖亚吐息 自身攻击力30倍的木属性全体攻击
    
    {19,        10,     1,  enElementLight, 1,      30,     0,      enManSkillTp_atk},    //105 超级神圣吐息 自身攻击力30倍的光属性全体攻击
    
    {19,        10,     1,  enElementDark,  1,      30,     0,      enManSkillTp_atk},    //106 超级地狱吐息 自身攻击力30倍的暗属性全体攻击

    // 双属性宝石变成双属性宝石
    {13,        6,      0,  enElementFire << 4 | enElementWood,  enElementHeart << 4 | enElementFire,   0,   0,   enManSkillTp_stonetrans},    //107 火->心、木->火 所有火宝珠变化成回复宝珠，所有木宝石变化为火宝珠
    
    {13,        6,      0,  enElementWater << 4 | enElementFire,  enElementHeart << 4 | enElementWater,   0,   0,   enManSkillTp_stonetrans},    //108 水->心、火->水 所有水宝珠变化成回复宝珠，所有火宝石变化成水宝珠
    
    {13,        6,      0,  enElementWood << 4 | enElementWater,  enElementHeart << 4 | enElementWood,   0,   0,   enManSkillTp_stonetrans},    //109 木->心、水->木 所有木宝珠变化成回复宝珠，所有水宝石变化成木宝珠
    
    {13,        6,      0,  enElementLight << 4 | enElementDark,  enElementHeart << 4 | enElementLight,   0,   0,   enManSkillTp_stonetrans},    //110 光->心、暗->光 所有光宝珠变化成回复宝珠，所有暗宝石变化成光宝珠
    
    {13,        6,      0,  enElementDark << 4 | enElementLight,  enElementHeart << 4 | enElementDark,   0,   0,   enManSkillTp_stonetrans},    //111 暗->心、光->暗 所有暗宝珠变化成回复宝珠，所有光宝石变化成暗宝珠
    
    // 双属性攻击 攻击系
    {25,        6,     1,  enElementFire,  10000,      1,     0,      enManSkillTp_atk},    //112 火、心属性伤害 对敌方全体造成10000点火属性伤害和30000点心属性伤害
    
    {25,        6,     1,  enElementWater, 10000,      1,     0,      enManSkillTp_atk},    //113 水、心属性伤害 对敌方全体造成10000点水属性伤害和30000点心属性伤害
    
    {25,        6,     1,  enElementWood,  10000,      1,     0,      enManSkillTp_atk},    //114 木、心属性伤害 对敌方全体造成10000点木属性伤害和30000点心属性伤害
     
    {25,        6,     1,  enElementLight, 10000,      1,     0,      enManSkillTp_atk},    //115 光、心属性伤害 对敌方全体造成10000点光属性伤害和30000点心属性伤害
    
    {25,        6,     1,  enElementDark,  10000,      1,     0,      enManSkillTp_atk},    //116 暗、心属性伤害 对敌方全体造成10000点暗属性伤害和30000点心属性伤害

    // 无敌一次伤害
    {20,        6,      1,  enElementWood,       3,      1,      1,      enManSkillTp_wudi},         //117 无敌一回合1+n(火) 1回合内敌方木属性攻击无效，其他属性伤害减半

    {20,        6,      1,  enElementFire,       3,      1,      1,      enManSkillTp_wudi},         //118 无敌一回合1+n(水) 1回合内敌方火属性攻击无效，其他属性伤害减半

    {20,        6,      1,  enElementWater,      3,      1,      1,      enManSkillTp_wudi},         //119 无敌一回合1+n(木) 1回合内敌方水属性攻击无效，其他属性伤害减半

    {20,        6,      1,  enElementDark,       3,      1,      1,      enManSkillTp_wudi},         //120 无敌一回合1+n(光) 1回合内敌方暗属性攻击无效，其他属性伤害减半

    {20,        6,      1,  enElementLight,      3,      1,      1,      enManSkillTp_wudi},         //121 无敌一回合1+n(暗) 1回合内敌方光属性攻击无效，其他属性伤害减半

    // x属性一回合加强
    {17,        6,      1,  enElementWater,      1,     1.5,     3,      enManSkillTp_Enhance},     //122 水加强三回合(1.5倍) 3回合内，水属性武将的攻击力变为1.5倍
    {17,        6,      1,  enElementFire,       1,     1.5,     3,      enManSkillTp_Enhance},     //123 火加强三回合(1.5倍) 3回合内，火属性武将的攻击力变为1.5倍
    {17,        6,      1,  enElementWood,       1,     1.5,     3,      enManSkillTp_Enhance},     //124 木加强三回合(1.5倍) 3回合内，木属性武将的攻击力变为1.5倍
    {17,        6,      1,  enElementLight,      1,     1.5,     3,      enManSkillTp_Enhance},     //125 光加强三回合(1.5倍) 3回合内，光属性武将的攻击力变为1.5倍
    {17,        6,      1,  enElementDark,       1,     1.5,     3,      enManSkillTp_Enhance},     //126 暗加强三回合(1.5倍) 3回合内，暗属性武将的攻击力变为1.5倍
    
    {25,        6,      1,  enElementWood,       5,      1,      1,      enManSkillTp_wudi},//127 1回合内，将木宝珠转化成无效块，且敌人攻击无效
    
    //对敌方单体造成10000点伤害，且属性必定克制
    {20,       11,      0,  enElementInstead,   10000,    1,     0,      enManSkillTp_atk},    //128 对敌方单体造成10000点伤害，且属性必定是克制他的
    
    {17,        6,      1,  enGeneralTp_God,       1,     2.5,     2,      enManSkillTp_EnWuJiangType},    //129 1回合内，神将类武将的攻击力提升2.5倍；次回合攻击力减弱
    
    //宝石加号效果
    {15,        6,      1,  enElementWater | 0xF0,   1,      1,      0,      enManSkillTp_StnPlus},    //130 所有水宝石获得加号效果
    {15,        6,      1,  enElementFire | 0xF0,    1,      1,      0,      enManSkillTp_StnPlus},    //131 所有火宝石获得加号效果
    {15,        6,      1,  enElementWood | 0xF0,    1,      1,      0,      enManSkillTp_StnPlus},    //132 所有木宝石获得加号效果
    {15,        6,      1,  enElementLight | 0xF0,   1,      1,      0,      enManSkillTp_StnPlus},    //133 所有光宝石获得加号效果
    {15,        6,      1,  enElementDark | 0xF0,    1,      1,      0,      enManSkillTp_StnPlus},    //134 所有暗宝石获得加号效果

    // 相同技能不同名字
    {15,        9,      0,   enElementMax,  10000,   1,      0,      enManSkillTp_atk},                  //135 同100 痛恨一击 根据我方HP百分比对敌人单体造成伤害，100%HP时10000点，50%HP时大约20000点，1HP时100000点，受属性加成和敌人防御的影响
    {17,        6,      1,  enElementWood,       1,     1.5,     1,      enManSkillTp_Enhance},         //136 同124 木加强一回合(1.5倍) 1回合内，木属性武将的攻击力变为1.5倍
    {19,        6,      1,  enElementMax,       5,      1,      3,     enManSkillTp_break},             //137 同94  火烧连营 3回合内，所有敌人的防御力变为0
    {25,        11,     1,  enElementMax,   2,      0.15,   0,      enManSkillTp_gravity},              //138 同30  对敌方全体造成15%当前HP的伤害，该伤害无视属性和防御

    // 攻击力40倍
    {23,        11,     1,  enElementFire,  1,      40,     0,      enManSkillTp_atk},    //139   对敌方全体造成自身攻击力40倍的火属性伤害
    {23,        11,     1,  enElementWater, 1,      40,     0,      enManSkillTp_atk},    //140   对敌方全体造成自身攻击力40倍的水属性伤害
    {23,        11,     1,  enElementWood,  1,      40,     0,      enManSkillTp_atk},    //141   对敌方全体造成自身攻击力40倍的木属性伤害
    {23,        11,     1,  enElementLight, 1,      40,     0,      enManSkillTp_atk},    //142   对敌方全体造成自身攻击力40倍的光属性伤害
    {23,        11,     1,  enElementDark,  1,      40,     0,      enManSkillTp_atk},    //143   对敌方全体造成自身攻击力40倍的暗属性伤害

    {14,        6,      1,  enElementMax,   4,      2,      2,      enManSkillTp_delay},    //144 延长所有敌人攻击冷却2回合

    {20,        6,      1,  enElementMax,   7,      0.25,    3,      enManSkillTp_armor},    //145 3回合内，受到的所有伤害减少75%

    {20,        11,      1,  enElementGod,  9999,   1,      0,      enManSkillTp_atk},    //146 对敌方全体造成9999点固定伤害，该伤害无视属性和防御力

    {15,        8,      1,  enElementMax,   5,      0.5,    5,      enManSkillTp_break},   //147  同37 5回合内，所有敌人的防御力减少50%
    {20,        6,      1,  enElementMax,   1,      2,      0,      enManSkillTp_poison},  //148  同33 使敌方全体中剧毒，中毒的敌人每回合损失相当于使用技能宠物自身攻击力2倍的HP，持续到死亡，该伤害无视属性和防御
    
    {15,        6,     0,  enElementMax,      enElementMax, 6.0,  0,  enManSkillTp_luanshi},    //149 6秒内，可以任意挪动宝珠而不会触发消除效果
    
    {15,        1,     0,  enElementDark,   1,      40,     0,      enManSkillTp_atk},    //150   自身攻击力40倍的暗属性单体攻击

    {20,        6,     2,  enElementWater,   37500,      1,     0,      enManSkillTp_atk},    //151   对火属性敌人造成37500点水属性伤害

    {20,        6,     0,  enElementDark,   2,      1.0 + 50.0 / 1000,     0,      enManSkillTp_atk},       //152   对敌方单体造成随机1~50倍伤害，伤害属性与自身属性相同，会受到敌人的属性和防御的影响

    // -------------------3.4----------------------
    {8,         6,      1,  enElementMax,   6,      3,      0,      enManSkillTp_recover},    //153 回复相当于使用技能宠物自身回复力3倍的HP

    {13,        8,      1,  enElementMax,   1,      1,      3,      enManSkillTp_aoe},    //154 3回合内，所有攻击转化为全体攻击

    {14,        6,      1,  enElementGod,  7777,   1,      0,      enManSkillTp_atk},    //155 对敌方全体造成7777点固定伤害，该伤害无视属性和防御力

    {20,        11,      1,  enElementDark,  7,   3.0,      5,      enManSkillTp_fanji},    //156 5回合内，受到伤害时进行反击，造成3倍于受到伤害的暗属性伤害

    {13,        1,     0,  enElementFire,   1,      30,     0,      enManSkillTp_atk},    //157 对敌方单体造成自身攻击力30倍的火属性伤害
    {13,        1,     0,  enElementWater,   1,      30,     0,      enManSkillTp_atk},    //158 对敌方单体造成自身攻击力30倍的水属性伤害
    {13,        1,     0,  enElementWood,   1,      30,     0,      enManSkillTp_atk},    //159 对敌方单体造成自身攻击力30倍的木属性伤害

    {9,        6,      1,  enElementFire,      25000,   1,      0,      enManSkillTp_atk},    //160 对敌方全体造成25000点火属性伤害
    {9,        6,      1,  enElementWater,     25000,   1,      0,      enManSkillTp_atk},    //161 对敌方全体造成25000点水属性伤害
    {9,        6,      1,  enElementWood,      25000,   1,      0,      enManSkillTp_atk},    //162 对敌方全体造成25000点木属性伤害
    {9,        6,      1,  enElementLight,     25000,   1,      0,      enManSkillTp_atk},    //163 对敌方全体造成25000点光属性伤害
    {9,        6,      1,  enElementDark,      25000,   1,      0,      enManSkillTp_atk},    //164 对敌方全体造成25000点暗属性伤害

    {20,        1,     0,  enElementLight,   1,      30,     0,      enManSkillTp_atk},    //165 对敌方单体造成自身攻击力30倍的光属性伤害
    

    {15,        6,     2,  enElementFire,   35000,      1,     0,      enManSkillTp_atk},    //166   对木属性敌人造成35000点火属性伤害
    {15,        6,     2,  enElementWater,   35000,      1,     0,      enManSkillTp_atk},    //167   对火属性敌人造成35000点水属性伤害
    {15,        6,     2,  enElementWood,   35000,      1,     0,      enManSkillTp_atk},    //168   对水属性敌人造成35000点木属性伤害

    {15,        6,     2,  enElementLight,   35000,      1,     0,      enManSkillTp_atk},    //169   对暗属性敌人造成35000点光属性伤害
    {15,        6,     2,  enElementDark,   35000,      1,     0,      enManSkillTp_atk},    //170   对光属性敌人造成35000点暗属性伤害

    {9,        5,      0,  enElementLight | 0xF0,   enElementFire | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //171 所有光宝珠变化成火宝珠
    {9,        5,      0,  enElementDark | 0xF0,   enElementWater | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //172 所有暗宝珠变化成水宝珠
    {9,        5,      0,  enElementWater | 0xF0,   enElementWood | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //173 所有水宝珠变化成木宝珠
    {9,        5,      0,  enElementWood | 0xF0,   enElementLight | 0xF0,    0,  0,  enManSkillTp_stonetrans},    //174 所有木宝珠变化成光宝珠
    {9,        5,      0,  enElementFire | 0xF0,   enElementDark | 0xF0,    0,  0,  enManSkillTp_stonetrans},     //175 所有火宝珠变化成暗宝珠

    {25,        6,      1,  enElementFire,      0,     0,     1,      enManSkillTp_chgElm},     //176 1回合内，把敌将属性转换为火属性
    {25,        6,      1,  enElementWater,     0,    0,     1,      enManSkillTp_chgElm},     //177 1回合内，把敌将属性转换为水属性
    {25,        6,      1,  enElementWood,      0,     0,     1,      enManSkillTp_chgElm},     //178 1回合内，把敌将属性转换为木属性
    {25,        6,      1,  enElementDark,      0,     0,     1,      enManSkillTp_chgElm},     //179 1回合内，把敌将属性转换为暗属性
    {25,        6,      1,  enElementFire,      0,     0,     1,      enManSkillTp_chgElm},     //180 同176 1回合内，把敌将属性转换为火属性

    // -------------------4.0-----------------------
    {25,       11,      0,  enElementMax,       1,     20,      0,       enManSkillTp_atk},         //181 對敵方單體造成自身攻擊力20倍的傷害，並回復相當於傷害5%的HP，傷害屬性與自身屬性相同，會受到敵人的屬性和防禦的影響

    {30,       11,      0,  enElementMax,       1,     50,      0,       enManSkillTp_atk},         //182 HP变为1，并对敌方一体造成50倍光屬性伤害，受屬性加成和敵人防禦的影響 註：已經一HP後，不可發動

    {15,        1,     0,  enElementLight,     1,     40,      0,       enManSkillTp_atk},         //183 对敌方单体造成自身攻击力40倍的光属性伤害

    {16,        8,     0,  enElementWater << 4 | enElementHeart,   enElementFire << 4 | enElementFire,    0,  0,  enManSkillTp_stonetrans}, //184 所有水宝珠和回复宝珠变化成火宝珠

    {16,        8,     0,  enElementWood << 4 | enElementHeart,   enElementWater << 4 | enElementWater,    0,  0,  enManSkillTp_stonetrans}, //185 所有木宝珠和回复宝珠变化成水宝珠

    {16,        8,     0,  enElementFire << 4 | enElementHeart,   enElementWood << 4 | enElementWood,    0,  0,  enManSkillTp_stonetrans}, //186 所有火宝珠和回复宝珠变化成木宝珠

    {16,        8,     0,  enElementDark << 4 | enElementHeart,   enElementLight << 4 | enElementLight,    0,  0,  enManSkillTp_stonetrans}, //187 所有暗宝珠和回复宝珠变化成光宝珠

    {16,        8,     0,  enElementLight << 4 | enElementHeart,   enElementDark << 4 | enElementDark,    0,  0,  enManSkillTp_stonetrans}, //188 所有光宝珠和回复宝珠变化成暗宝珠

    {30,        14,    0,  enElementMax | 0xF0,   enElementFire | 0xF0,    0,  0,  enManSkillTp_stonetrans}, //189 所有宝珠变化为火宝珠

    {17,        6,     0,  enElementFire,   1,      30,     0,      enManSkillTp_atk},    //190 对敌方单体造成自身攻击力30倍的火属性伤害

    {17,        6,     0,  enElementMax,      enElementMax, 7.0,  0,  enManSkillTp_luanshi},    //191 7秒内，可以任意挪动宝珠而不会触发消除效果

    {20,        6,      1,  enElementLight,  7,   3.0,      3,      enManSkillTp_fanji},    //192 3回合内，必定对攻击的敌人单体反击，造成3倍于受到伤害值的光属性伤害
    
    {14,        6,     0,  enElementWood << 4 | enElementLight,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //193 所有木宝珠和光宝珠变化成回复宝珠

    {14,        6,     0,  enElementFire << 4 | enElementDark,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //194 所有火宝珠和暗宝珠变化成回复宝珠

    {14,        6,     0,  enElementWater << 4 | enElementLight,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //195 所有水宝珠和光宝珠变化成回复宝珠

    {14,        6,     0,  enElementWood << 4 | enElementDark,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //196 所有木宝珠和暗宝珠变化成回复宝珠

    {14,        6,     0,  enElementFire << 4 | enElementLight,   enElementHeart << 4 | enElementHeart,    0,  0,  enManSkillTp_stonetrans}, //197 所有火宝珠和光宝珠变化成回复宝珠

    {17,        6,      1,  enElementHeart,       1,     1.5,     1,      enManSkillTp_Enhance},     //198 1回合内，心属性武将的攻击力变为1.5倍

    {20,        6,      1,  enElementHeart,      1,      1,      1,      enManSkillTp_wudi},         //199 1回合内敌方心属性攻击无效
    
    {30,       11,      0,  enElementMax,       1,     50,      0,       enManSkillTp_atk},         //200 HP变为1，并对敌方一体造成50倍光屬性伤害，受屬性加成和敵人防禦的影響 註：已經一HP後，不可發動

    {14,        6,      1,  enElementGod,  7777,   1,      0,      enManSkillTp_atk},    //201 对敌方全体造成7777点固定伤害，该伤害无视属性和防御力
    
    {13,        6,      0,  enElementLight << 4 | enElementDark,  enElementFire << 4 | enElementHeart,   0,   0,   enManSkillTp_stonetrans},    //202 所有光宝珠变为火宝珠，所有暗宝珠变为回复宝珠
    
    {13,        6,      0,  enElementLight << 4 | enElementDark,  enElementWater << 4 | enElementHeart,   0,   0,   enManSkillTp_stonetrans},    //203 所有光宝珠变为水宝珠，所有暗宝珠变为回复宝珠
    
    {13,        6,      0,  enElementLight << 4 | enElementDark,  enElementWood << 4 | enElementHeart,   0,   0,   enManSkillTp_stonetrans},    //204 所有光宝珠变为木宝珠，所有暗宝珠变为回复宝珠
    
    {23,        6,      0,  enElementMax | 0xF0,   enElementHeart | 0xF0,   0,   0,   enManSkillTp_stonetrans},    //205 所有宝珠变为回复宝珠
    
    {35,       11,      0,  enElementDark,       300000,     1,      0,       enManSkillTp_atk},         //206 HP变为1，并且对敌方单体造成300000点暗属性伤害，会受到敌人的属性和防御的影响。当HP为1时不能发动此技能
    
    {20,        11,      1,  enElementFire,  7,   3.0,      5,      enManSkillTp_fanji},    //207 5回合内，受到伤害时进行反击，造成3倍于受到伤害的火属性伤害
    
    {20,        11,      1,  enElementWater,  7,   3.0,      5,      enManSkillTp_fanji},    //208 5回合内，受到伤害时进行反击，造成3倍于受到伤害的水属性伤害
    
    {20,        11,      1,  enElementWood,  7,   3.0,      5,      enManSkillTp_fanji},    //209 5回合内，受到伤害时进行反击，造成3倍于受到伤害的木属性伤害
    
    {20,        11,      1,  enElementLight,  7,   3.0,      5,      enManSkillTp_fanji},    //210 5回合内，受到伤害时进行反击，造成3倍于受到伤害的光属性伤害
    
    {20,        16,     2,  enElementDark,   150000,      1,     0,      enManSkillTp_atk},    //211   对光属性敌人造成150000点暗属性伤害
    
    {35,       11,      0,  enElementDark,       1,     300,      0,       enManSkillTp_atk},         //212 HP变为1，并且对敌方单体造成自身攻击力300倍的暗属性伤害，会受到敌人的属性和防御的影响。当HP为1时不能发动此技能
    
    {30,        14,      0,  enElementMax | 0xF0,   enElementWood | 0xF0,   0,   0,   enManSkillTp_stonetrans},    //213 所有宝珠变为木宝珠
    
    {15,        6,      0,  enElementLight << 4 | enElementHeart,  enElementWood << 4 | enElementLight,   0,   0,   enManSkillTp_stonetrans},    //214 所有光宝珠变为木宝珠，所有回复宝珠变为光宝珠
    
    {35,       18,      1,  enElementMax,       2,      0.35,   0,      enManSkillTp_gravity},      //215 对敌方全体造成35%当前HP的伤害，该伤害无视属性和防御
    
    
    {16,       5,      1,  enElementLight,       enElementDark,      2,   0,      enManSkillTp_stoneAtk},     //216 直接消除暗宝珠造成敌全体光属性伤害，落下后的宝珠不会自动消除，不计算回合数。伤害=自身攻击力×消除宝珠个数×2
    
    {25,       10,      1,  enElementLight,       enElementOther,      2,   0,      enManSkillTp_stoneAtk},     //217 直接消除光属性以外的宝珠造成敌方全体光属性伤害，落下后的宝珠不会自动消除，不计算回合数.伤害=自身攻击力×消除宝珠个数×2
    {17,        6,      1,  enGeneralTp_mojiang,       1,     3,     1,      enManSkillTp_EnWuJiangType},     //218 1回合内，魔将武将的攻击力变为3倍
    
    {17,        6,      1,  enGeneralTp_XiaoYong,       1,     3,     1,      enManSkillTp_EnWuJiangType},     //219	骁将强化	1回合内，骁勇善战类武将的攻击力提升3倍
    {17,        6,      1,  enGeneralTp_JinShi,       1,     3,     1,      enManSkillTp_EnWuJiangType},     //220	悍将强化	1回合内，金石之坚类武将的攻击力提升3倍
    {17,        6,      1,  enGeneralTp_GongShou,       1,     3,     1,      enManSkillTp_EnWuJiangType},     //221	猛将强化	1回合内，攻守兼备类武将的攻击力提升3倍
    {17,        6,      1,  enGeneralTp_ShengSheng,       1,     3,     1,      enManSkillTp_EnWuJiangType},     //222	谋将强化	1回合内，生生不息类武将的攻击力提升3倍
    {17,        6,      1,  enGeneralTp_GuoShi,       1,     2,     1,      enManSkillTp_EnWuJiangType},     //223	龙将强化	1回合内，国士无双类武将的攻击力提升2倍
    {17,        6,      1,  enGeneralTp_GuoShi,       1,     2.5,     1,      enManSkillTp_EnWuJiangType},     //224	龙将强化II	1回合内，国士无双类武将的攻击力提升2.5倍
    {15,        6,      1,  enElementWood <<4 | enElementLight,   1,      1,      0,      enManSkillTp_StnPlus},     //225	风雷双龙斩	木、光属性宝珠的攻击力提升
    {15,        6,      1,  enElementFire <<4 | enElementDark,   1,      1,      0,      enManSkillTp_StnPlus},    //226	漆黑之炎	火、暗属性宝珠的攻击力提升
    {20,        13,      1,  enGeneralTp_mojiang,       1,     3,     1,      enManSkillTp_chglocal},     //227	救驾	与当前自己的主将交换位置，再次使用时换回原位
    {20,        11,      0,  enElementGod,  15000,   1,      0,      enManSkillTp_atk},     //228	猛烈的冲撞	对敌方单体造成15000点固定伤害
    {30,        16,     0,  enElementFire,   1,      50,     0,      enManSkillTp_atk},     //229	烈焰突刺	对敌方单体造成自身攻击力50倍的火属性伤害
    {20,       6,      0,  enElementMax,       1,     20,      0,       enManSkillTp_atk},     //230	深渊突刺	对敌方单体造成自身攻击力20倍的伤害，并回复相当于伤害10%的HP
    {15,        11,     0,  enElementMax,      enElementMax,  5.0,  0,  enManSkillTp_luanshi},     //231	镜火炎	5秒内，可以任意挪动宝珠
    {13,        6,      0,  enElementWood << 4 | enElementHeart,  enElementWater << 4 | enElementDark,   0,   0,   enManSkillTp_stonetrans},     //232	天象从临-初	所有木宝珠变为水宝珠，所有回复宝珠变为暗宝珠
    {12,        6,      1,  enGeneralTp_XiaoYong,       1,     1.5,     1,      enManSkillTp_EnWuJiangType},     //233	螺旋丸手里剑	1回合内，骁勇善战类武将的攻击力提升1.5倍
    {15,        6,      1,  enElementMax,       4,      1,      2,      enManSkillTp_wudi},     //234	千鸟流	2回合内，所受伤害减半
    {20,        6,      0,  enElementMax,       1,     8,     1,      enManSkillTp_atk},     //235	喝！	对敌方单体造成自身攻击力8倍的伤害，并回复相当于伤害50%的HP
    {35,       11,      0,  enElementWater,       300000,     1,      0,       enManSkillTp_atk},     //236	到我们了！	HP变为1，并对敌方单体造成30万点水属性伤害
    {14,        6,      0,  enElementMax,       enElementFire << 8 | enElementWood<<4 | enElementLight,     1,     0,      enManSkillTp_stonetrans},     //237	无式·热月	所有宝珠变化成火、木、光宝珠
    {14,        6,      0,  enElementMax,       enElementWater << 8 | enElementWood<<4 | enElementDark,     1,     0,      enManSkillTp_stonetrans},     //238	无式·雪月	所有宝珠变化成水、木、暗宝珠
    {14,        6,      0,  enElementMax,       enElementWood << 8 | enElementLight<<4 | enElementDark,     1,     0,      enManSkillTp_stonetrans},     //239	无式·风月	所有宝珠变化成木、光、暗宝珠
    {15,       6,      1,  enElementMax,       2,      0.2,   0,      enManSkillTp_gravity},     //240	神之闪光	敌方全体减少20%的血量
    {14,        6,      0,  enElementMax,       enElementFire << 8 | enElementWater<<4 | enElementDark,     1,     0,      enManSkillTp_stonetrans},     //241	无式·芽月	所有宝珠变化成火、水、暗宝珠
    {11,        6,      1,  enElementMax,   6,      6,      0,      enManSkillTp_recover},     //242	奥义·圣光	回复相当于自身回复力6倍的血量
    {15,        6,      1,  enElementLight << 4 | enElementDark,   1,      1,      0,      enManSkillTp_StnPlus},     //243	奥义·天地神	光、暗属性宝珠的攻击力提升
    {15,        11,     2,  enElementFire,   75000,      1,     0,      enManSkillTp_atk},     //244	真·破木-火	对木属性敌人造成75000点火属性伤害
    
    {15,        1,     0,  enElementLight,   1,      40,     0,      enManSkillTp_atk},     //245 疾驰小阿星	对敌方单体造成自身攻击力40倍的光属性伤害
    
    
    {13,        6,      0,  enElementWood << 4 | enElementHeart,  enElementFire << 4 | enElementWood,  0,   0,   enManSkillTp_stonetrans},    //246 所有木宝珠变为火宝珠，所有回复宝珠变为木宝珠
    {13,        6,      0,  enElementFire << 4 | enElementHeart,  enElementWater << 4 | enElementFire, 0,   0,   enManSkillTp_stonetrans},    //247 所有火宝珠变为水宝珠，所有回复宝珠变为火宝珠
    {13,        6,      0,  enElementWater << 4 | enElementHeart, enElementWood << 4 | enElementWater, 0,   0,   enManSkillTp_stonetrans},    //248 所有水宝珠变为木宝珠，所有回复宝珠变为水宝珠
    {13,        6,      0,  enElementDark << 4 | enElementHeart,  enElementLight << 4 | enElementDark, 0,   0,   enManSkillTp_stonetrans},    //249 所有暗宝珠变为光宝珠，所有回复宝珠变为暗宝珠
    {13,        6,      0,  enElementLight << 4 | enElementHeart, enElementDark << 4 | enElementLight, 0,   0,   enManSkillTp_stonetrans},    //250 所有光宝珠变为暗宝珠，所有回复宝珠变为光宝珠
    {15,        6,      1,  enElementWater <<4 | enElementWood,   1,                                   1,   0,   enManSkillTp_StnPlus},       //251	水、木属性宝珠的攻击力提升
    {17,        6,      1,  enGeneralTp_GongShou,                 1,                                   1.5, 2,   enManSkillTp_EnWuJiangType}, //252	2回合内，攻守兼备类武将的攻击力提升1.5倍
    {15,        8,      0,  enElementGod,                         300,                                 1,   0,   enManSkillTp_atk},           //253	对敌方单体造成300点固定伤害
    {25,        18,     1,  enGeneralTp_mojiang,                  1,                                   3,   1,   enManSkillTp_chglocal},      //254	救驾	与当前自己的主将交换位置，再次使用时换回原位
    {10,        1,      1,  enElementMax,                         1,                                   2,   2,   enManSkillTp_delay},         //255 敌人攻击延长2回合；一回合内，火属性大强化（连成横行6颗火属性宝珠，火属性攻击力提升30%）
    {10,        1,      0,  enElementDark | 0xF0,                 enElementWater | 0xF0,               0,   0,   enManSkillTp_stonetrans},    //256 所有暗宝珠变化成水宝珠；水属性宝珠的攻击力提升
    {10,        1,      1,  enElementMax,                         0xFFFFFF,                            1,   2,   enManSkillTp_recover},       //257 2回合内，血量完全回复；首回合国士无双类武将变为虚弱状态
    
    {20,        6,      0,  enElementMax,                         1,                                   0,   0,   enManSkillTp_null},          //258 随机使用下列主动技能:（36威压、58乱世天下、94火烧连营、34蚀骨剧毒、42青囊秘术）
    {20,        6,      0,  enElementMax,                         1,                                   0,   0,   enManSkillTp_null},          //259 随机使用下列主动技能:（124破军点钢矛、224勇者高阶强化、31鬼王咆哮、132蓄势待发·木、213风卷残云）
    {6,         1,      0,  0xFF,                                 0xFF,                                0,   0,   enManSkillTp_stonerand},     //260 所有宝珠随机变化
    {13,        1,      0,  enElementLight,                       1,                                   30,  0,   enManSkillTp_atk},           //261 对敌方单体造成自身攻击力30倍的光属性伤害
    {13,        1,      0,  enElementDark,                        1,                                   30,  0,   enManSkillTp_atk},           //262 对敌方单体造成自身攻击力30倍的暗属性伤害
    {10,        1,      1,  enElementMax,                         0xFFFFFF,                             1,  0,   enManSkillTp_recover},       //263 血量完全回复;木属性宝珠的攻击力提升（木属性宝珠变+号）
    {11,        4,      0,  enGeneralTp_Null,                     6,                                   1.5, 4,   enManSkillTp_ReCover_WuJiangType},    //264	4回合内，队伍的回复力变为1.5倍
    {11,        4,      0,  enElementFire << 4 | enElementHeart,  enElementWater << 4 | enElementWood, 0,   0,   enManSkillTp_stonetrans},    //265 所有火宝珠变为水宝珠，所有回复宝珠变为木宝珠
    {11,        4,      1,  enElementMax,                         7,                                   0.25,1,   enManSkillTp_armor},         //266 1回合内，受到的所有伤害减少75%
    {11,        4,      1,  enGeneralTp_God,                      1,                                   2.0, 1,   enManSkillTp_EnWuJiangType}, //267 1回合内，神将类武将的攻击力提升2倍
    {11,        4,      1,  enElementMax,                         4,                                   1,   1,   enManSkillTp_delay},         //268 延长所有敌人攻击冷却1回合
    {13,        6,      0,  enElementDark << 4 | enElementWood,   enElementWater << 4 | enElementHeart,0,   0,   enManSkillTp_stonetrans},    //269 所有暗宝珠变为水宝珠，所有木宝珠变为回复宝珠
    {13,        6,      0,  enElementFire << 4 | enElementDark,   enElementWood << 4 | enElementHeart, 0,   0,   enManSkillTp_stonetrans},    //270 所有火宝珠变为木宝珠，所有暗宝珠变为回复宝珠
    {15,        6,      1,  enElementFire <<4 | enElementLight,   1,                                   1,   0,   enManSkillTp_StnPlus},       //271	火、光属性宝珠的攻击力提升
    {20,        11,     1,  enElementFire ,                       2,                  1.0 + 100.0 / 1000,   0,   enManSkillTp_atk},           //272	对敌方全体造成随机1~100倍火属性伤害，会受到敌人的属性和防御的影响
    {17,        6,      0,  enElementDark ,                       1,                                  50,   0,   enManSkillTp_atk},           //273	对敌方一体造成自身攻击力x50倍的暗属性伤害并回复15%伤害的HP。会受到敌人的属性和防御的影响

    {12,        1,      1,  enElementFire,                        1,                                  2,    1,   enManSkillTp_Enhance},       //274 1回合内，火属性武将的攻击力提升2倍
    
    {12,        1,      1,  enCountryTp_shuguo,                 1,                                  2,    1,   enManSkillTp_CountryWujiangType          }, //275	1回合内，蜀国武将的攻击力提升2倍（等国家系统需求出来再做）
    
    {10,        1,      0,  enElementWood | 0xF0,                 enElementFire | 0xF0,               10,   0,   enManSkillTp_stonetrans},    //276 所有木宝珠变化成火宝珠；10秒内，可以任意挪动宝珠
    {12,        1,      1,  enElementDark,                        1,                                  2,    1,   enManSkillTp_Enhance},       //277 1回合内，暗属性武将的攻击力提升2倍
    {15,        1,      0,  enElementMax,                         enElementLight << 4 | enElementDark,1,    0,   enManSkillTp_stonetrans},    //278	所有宝珠变化成光、暗宝珠
    {12,        1,      1,  enElementLight,                       1,                                   2,   1,   enManSkillTp_Enhance},       //279 1回合内，光属性武将的攻击力提升2倍；光属性宝珠的攻击力提升
    {20,        1,      1,  enElementFire,  100000,      1,     0,      enManSkillTp_atk},                                                      //280 造成10万点火属性和5万点光属性全体伤害
    
    
    {12,        1,      0,  enElementMax,  enElementFire<<4 | enElementDark, 1,   0,   enManSkillTp_stonetrans},    //281 所有宝珠变化成火、暗宝珠
    {12,        1,      1,  enElementLight,  1, 1.4,   1,   enManSkillTp_Enhance},    //282 光属性伤害大提高（提高40%等于加4个横排）；10秒内任意移动宝珠

    
    {15,        4,      0,  enElementMax,  enElementWood<<4 | enElementDark, 1,   0,   enManSkillTp_stonetrans},    //283 所有宝珠变化成木、暗宝珠
    
    {20,        1,      1,  enGeneralTp_XiaoYong,    1,        2.0,        1,   enManSkillTp_EnWuJiangType},    //284 一回合内，骁勇善战类武将攻击力提高2倍，使敌方全体中剧毒，中毒的敌人每回合损失相当于使用技能宠物自身攻击力10倍的HP，持续到死亡，该伤害无视属性和防御。

    
    {15,        1,      1,  enElementDark,    0,        0,        1,   enManSkillTp_chgElm},    //285 当回合内，敌将属性变为暗属性；所有宝珠转化为光心
    
//    
//     {20,        6,      1,  enElementMax,      1,      1,      1,      enManSkillTp_wudi},         //286 无敌一回合(水) 1回合内敌方火属性攻击无效
    
    //冷却回合  最大lv    目标个数   技能属性                          效果基数                              效果  持续回合  技能类别
};






#pragma mark -
#pragma mark gAutoSkillInfo
//被动技能
static stAutoSkill gAutoSkillInfo[] = {
    //技能类别                      技能属性          武将类型        发生几率   效果       副技能ID
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},
    //各种力
    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //1 坚甲厉兵 火 我方火属性武将的攻击力提升1.5倍
    {enAutoSkillTp_atk,         enElementWater,   enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //2 坚甲厉兵 水 我方水属性武将的攻击力提升1.5倍
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //3 坚甲厉兵 木 我方木属性武将的攻击力提升1.5倍
    {enAutoSkillTp_atk,         enElementLight,   enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //4 坚甲厉兵 光 我方光属性武将的攻击力提升1.5倍
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //5 坚甲厉兵 暗 我方暗属性武将的攻击力提升1.5倍

    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 100,    2,  enAutoSkill_NULL},      //6 精锐之师 火 我方火属性武将的攻击力提升2倍
    {enAutoSkillTp_atk,         enElementWater,   enGeneralTp_Null, 100,    2,  enAutoSkill_NULL},      //7 精锐之师 水 我方水属性武将的攻击力提升2倍
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 100,    2,  enAutoSkill_NULL},      //8 精锐之师 木 我方木属性武将的攻击力提升2倍
    {enAutoSkillTp_atk,         enElementLight,   enGeneralTp_Null, 100,    2,  enAutoSkill_NULL},      //9 精锐之师 光 我方光属性武将的攻击力提升2倍
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 100,    2,  enAutoSkill_NULL},      //10 精锐之师 暗 我方暗属性武将的攻击力提升2倍
    
    //各种防
    {enAutoSkillTp_armor_attr, enElementFire,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //11 据守 火 火属性伤害减少
    {enAutoSkillTp_armor_attr, enElementWater,   enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //12 据守 水 水属性伤害减少
    {enAutoSkillTp_armor_attr, enElementWood,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //13 据守 木 木属性伤害减少
    {enAutoSkillTp_armor_attr, enElementLight,   enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //14 据守 光 光属性伤害减少
    {enAutoSkillTp_armor_attr, enElementDark,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //15 据守 暗 暗属性伤害减少

    {enAutoSkillTp_armor_attr, enElementFire,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //16 固若金汤 火 火属性伤害减少一半
    {enAutoSkillTp_armor_attr, enElementWater,   enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //17 固若金汤 水 水属性伤害减少一半
    {enAutoSkillTp_armor_attr, enElementWood,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //18 固若金汤 木 木属性伤害减少一半
    {enAutoSkillTp_armor_attr, enElementLight,   enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //19 固若金汤 光 光属性伤害减少一半
    {enAutoSkillTp_armor_attr, enElementDark,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //20 固若金汤 暗 暗属性伤害减少一半

    //全面防
    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.9, enAutoSkill_NULL},    //21 磐石 受到的所有伤害小幅减少
    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.8, enAutoSkill_NULL},    //22 墨守 受到的所有伤害减少

    //加血
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},    //23 黄帝内经 每回合恢复少量血量
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},    //24 神农本草 每回合恢复一定血量

    //追打
    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    0.5,   enAutoSkill_NULL},    //25 业炎 攻击后，追加一次全体攻击
    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    2.0,   enAutoSkill_NULL},    //26 狱炎 攻击后，追加一次强力全体攻击

    //免死
    {enAutoSkillTp_revive,      0,              enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},    //27 龙之魄 当受到致死攻击时，有几率生还
    {enAutoSkillTp_revive,      0,              enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},    //28 真龙之魄 当受到致死攻击时，有几率生还

    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    5.0,  enAutoSkill_NULL},     //29 延滞术 宝石移动的最大时限延长少许
    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    10.0, enAutoSkill_NULL},     //30 时光术 宝石移动的最大时限延长较多
    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    15.0, enAutoSkill_NULL},     //31 永恒术 宝石移动的最大时限延长许多
    
    //加血
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    3.0,  enAutoSkill_NULL},    //32 歌莺舞燕 每回合恢复较多血量
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    5.0,  enAutoSkill_NULL},    //33 鸾歌凤舞 每回合恢复很多血量
    
    //追打
    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    5.0,   enAutoSkill_NULL},    //34 无尽狱炎 攻击后，追加一次超强力全体攻击
    
    //全面防
    {enAutoSkillTp_armor_all, 0,                enGeneralTp_Null, 100,    0.67, enAutoSkill_NULL},    //35 无懈可击 受到的所有伤害大幅减少
    
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_JinShi, 100,    2.0,    enAutoSkill_NULL},       //36 战士之怒 金石之坚类武将的攻击提升2倍
    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_GongShou, 100,    1.5,   enAutoSkill_NULL},     //37 猛将血统 攻守兼备类武将的血量提升1.5倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_ShengSheng, 100,    2.0,    enAutoSkill_NULL},   //38 百花缭乱 生生不息类武将的攻击力提升2倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_JinShi, 100,    2.5,    enAutoSkill_NULL},       //39 龙之血 金石之坚类武将的攻击提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_GuoShi, 100,    2.0, enAutoSkill_NULL},       //40 龙骑士之誓 (国士无双类武将的血量提升2倍)
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_GuoShi, 100,    2.5,    enAutoSkill_NULL},       //41 龙骑士之怒 国士无双类武将的攻击提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_GongShou, 100,    2.5,   enAutoSkill_NULL},     //42 斯卡哈的祈祷 攻守兼备类武将的血量提升2.5倍
    {enAutoSkillTp_valChg_rcv,  enElementMax,       enGeneralTp_GongShou, 100,    2.5,  enAutoSkill_NULL},     //43 不死鸟的加护 (平衡类宠物的回复力变为原来的2.5倍)
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_GongShou, 100,    2.5,  enAutoSkill_NULL},     //44 神狼的咆哮 攻守兼备类武将的攻击提升2.5倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_ShengSheng, 100,    2.5,    enAutoSkill_NULL},   //45 瓦尔基里之魂 生生不息类武将的攻击力提升2.5倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_God, 100,    2.0,   enAutoSkill_NULL},          //46 神之祷言 神类武将的攻击提升2倍
    
    //掉金率
    {enAutoSkillIp_money_exp_drop,            1,             0,               0,    1.5,    enAutoSkill_NULL},         //47 掉金率1.5倍 额外获取少量金钱
    {enAutoSkillIp_money_exp_drop,            1,             0,               0,    2.0,    enAutoSkill_NULL},         //48 掉金率2倍 额外获取大量金钱
    
    //经验
    {enAutoSkillIp_money_exp_drop,            0,             1,               0,    1.2,    enAutoSkill_NULL},         //49 经验1.2倍 获得的经验值变为原来的1.2倍
    {enAutoSkillIp_money_exp_drop,            0,             1,               0,    1.5,    enAutoSkill_NULL},         //50 经验1.5倍 获得的经验值变为原来的1.5倍
    
    //掉将率
    {enAutoSkillIp_money_exp_drop,            0,             0,               1,    1.2,    enAutoSkill_NULL},         //51 武将掉率1.2倍 武将掉率小幅提升
    {enAutoSkillIp_money_exp_drop,            0,             0,               1,    1.5,    enAutoSkill_NULL},         //52 武将掉率1.5倍 武将掉率大幅提升
    
    //对应属性hp加成
    {enAutoSkillTp_valChg_hp,  enElementWater,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //53 水1.5倍 (水之魂) 水属性武将的血量提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementFire,        enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //54 火1.5倍 (火之魂) 火属性武将的血量提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementWood,        enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //55 木1.5倍 (木之魂) 木属性武将的血量提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementLight,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //56 光1.5倍 (光之魂) 光属性武将的血量提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementDark,        enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //57 暗1.5倍 (暗之魂) 暗属性武将的血量提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementWater,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},         //58 水2倍 (葵水之精) 水属性武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,  enElementFire,        enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},         //59 火2倍 (离火之精) 火属性武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,  enElementWood,        enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},         //60 木2倍 (甲木之精) 木属性武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,  enElementLight,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},         //61 光2倍 (圣光之命) 光属性武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,  enElementDark,        enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},         //62 暗2倍 (幽冥之命) 暗属性武将的血量提升2倍
    
    //江南style
    {enAutoSkillTp_sndChg,  enElementMax,         enGeneralTp_Null, 100,    1.0,    enAutoSkill_NULL},         //63 江南style 宝石消除的声音变为江南style
    
    //激运
    {enAutoSkillTp_valChg_hp,  enElementMax,         enGeneralTp_JinShi, 100,    2.0,   enAutoSkill_NULL},       //64 金石之坚类武将的HP变为原本的2倍
    
    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 100,    2.5,    enAutoSkill_NULL},           //65 火属性武将造成的普通攻击伤害变为原本的2.5倍
    {enAutoSkillTp_atk,         enElementWater,    enGeneralTp_Null, 100,    2.5,   enAutoSkill_NULL},          //66 水属性武将造成的普通攻击伤害变为原本的2.5倍
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 100,    2.5,    enAutoSkill_NULL},           //67 木属性武将造成的普通攻击伤害变为原本的2.5倍
    {enAutoSkillTp_atk,         enElementLight,    enGeneralTp_Null, 100,    2.5,   enAutoSkill_NULL},          //68 光属性武将造成的普通攻击伤害变为原本的2.5倍
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 100,    2.5,    enAutoSkill_NULL},           //69 暗属性武将造成的普通攻击伤害变为原本的2.5倍
    
    //50%~80%几率发动,同属性武将造成的普通攻击伤害变为原本的3倍
    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 50,    3,   enAutoSkill_NULL},              //70 鲁莽火 50%几率发动,火属性武将造成的普通攻击伤害变为原本的3倍
    {enAutoSkillTp_atk,         enElementWater,    enGeneralTp_Null, 50,    3,  enAutoSkill_NULL},              //71 鲁莽水 50%几率发动,水属性武将造成的普通攻击伤害变为原本的3倍
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 50,    3,   enAutoSkill_NULL},              //72 鲁莽木 50%几率发动,木属性武将造成的普通攻击伤害变为原本的3倍
    {enAutoSkillTp_atk,         enElementLight,    enGeneralTp_Null, 50,    3,  enAutoSkill_NULL},              //73 鲁莽光 50%几率发动,光属性武将造成的普通攻击伤害变为原本的3倍
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 50,    3,   enAutoSkill_NULL},              //74 鲁莽暗 50%几率发动,暗属性武将造成的普通攻击伤害变为原本的3倍
    
    //30%~50%几率发动,同属性武将造成的普通攻击伤害变为原本的4倍
    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 30,    4,   enAutoSkill_NULL},              //75 无尽强袭火 30%几率发动,火属性武将造成的普通攻击伤害变为原本的4倍
    {enAutoSkillTp_atk,         enElementWater,    enGeneralTp_Null, 30,    4,  enAutoSkill_NULL},              //76 无尽强袭水 30%几率发动,水属性武将造成的普通攻击伤害变为原本的4倍
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 30,    4,   enAutoSkill_NULL},              //77 无尽强袭木 30%几率发动,木属性武将造成的普通攻击伤害变为原本的4倍
    {enAutoSkillTp_atk,         enElementLight,    enGeneralTp_Null, 30,    4,  enAutoSkill_NULL},              //78 无尽强袭光 30%几率发动,光属性武将造成的普通攻击伤害变为原本的4倍
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 30,    4,   enAutoSkill_NULL},              //79 无尽强袭暗 30%几率发动,暗属性武将造成的普通攻击伤害变为原本的4倍
    
    //xx之魂
    {enAutoSkillTp_valChg_atkrcv,  enElementFire,       enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},            //80  火属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWater,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},           //81  水属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWood,       enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},            //82  木属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_all,  enElementFire,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},            //83  火属性武将的攻击力、血量和回复力变为1.5倍
    {enAutoSkillTp_valChg_all,  enElementWater,       enGeneralTp_Null, 100,    1.5,    enAutoSkill_NULL},           //84  水属性武将的攻击力、血量和回复力变为1.5倍
    {enAutoSkillTp_valChg_all,  enElementWood,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},            //85  木属性武将的攻击力、血量和回复力变为1.5倍
    
    //龙神的xx
    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_LongShen, 100,    1.5,   enAutoSkill_NULL},         //86  国士无双和神将类武将的HP提升1.5倍
    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_LongShen, 100,    2.0,   enAutoSkill_NULL},         //87  国士无双和神将类武将的HP提升2倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_LongShen, 100,    1.5,  enAutoSkill_NULL},         //88  国士无双和神将类武将造成的伤害提升1.5倍
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_LongShen, 100,    2.0,  enAutoSkill_NULL},         //89  国士无双和神将类武将造成的伤害提升2倍
    
    //属性反击
    {enAutoSkillTp_fanji,     enElementFire,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_NULL},             //90 火焰反击 受到伤害时，有约25%几率（推测）进行反击，造成10倍于受到伤害的火属性伤害
    {enAutoSkillTp_fanji,     enElementWater,    enGeneralTp_Null, 25,    10.0, enAutoSkill_NULL},            //91 冻结反击 受到伤害时，有约25%几率（推测）进行反击，造成10倍于受到伤害的水属性伤害
    {enAutoSkillTp_fanji,     enElementWood,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_NULL},             //92 岩石反击 受到伤害时，有约25%几率（推测）进行反击，造成10倍于受到伤害的木属性伤害
    {enAutoSkillTp_fanji,     enElementLight,    enGeneralTp_Null, 50,    5.0,  enAutoSkill_NULL},             //93 闪电反击 受到伤害时，有约50%几率（推测）进行反击，造成5倍于受到伤害的光属性伤害
    {enAutoSkillTp_fanji,     enElementDark,    enGeneralTp_Null, 50,    5.0,   enAutoSkill_NULL},              //94 黑暗反击 受到伤害时，有约50%几率（推测）进行反击，造成5倍于受到伤害的暗属性伤害
    
    //Teddy & Zeus
    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},                //95 Teddy 当HP为满时，我方全体造成的普通攻击伤害变为原本的2倍
    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},                //96 Zeus 当HP为满时，我方全体造成的普通攻击伤害变为原本的3倍
    
    //双属性防
    {enAutoSkillTp_armor_attr2, enElementFire << 4 | enElementWood,    enGeneralTp_Null, 100,    0.7,   enAutoSkill_NULL},       //97 火和木属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementFire << 4 | enElementWood,    enGeneralTp_Null, 100,    0.5,   enAutoSkill_NULL},       //98 火和木属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementWater << 4 | enElementFire,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},     //99 水和火属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementWater << 4 | enElementFire,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},     //100 水和火属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementWood << 4 | enElementWater,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},      //101 木和水属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementWood << 4 | enElementWater,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},      //102 木和水属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementLight << 4 | enElementFire,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},    //103 光和火属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementLight << 4 | enElementFire,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},    //104 光和火属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementDark << 4 | enElementWood,    enGeneralTp_Null, 100,    0.7,   enAutoSkill_NULL},        //105 暗和木属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementDark << 4 | enElementWood,    enGeneralTp_Null, 100,    0.5,   enAutoSkill_NULL},        //106 暗和木属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementFire << 4 | enElementDark,    enGeneralTp_Null, 100,    0.7,   enAutoSkill_NULL},       //107 火和暗属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementFire << 4 | enElementDark,    enGeneralTp_Null, 100,    0.5,   enAutoSkill_NULL},       //108 火和暗属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementWater << 4 | enElementDark,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},      //109 水和暗属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementWater << 4 | enElementDark,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},      //110 水和暗属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementWood << 4 | enElementLight,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},     //111 木和光属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementWood << 4 | enElementLight,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},     //112 木和光属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementLight << 4 | enElementWater,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},   //113 光和水属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementLight << 4 | enElementWater,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},   //114 光和水属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementDark << 4 | enElementLight,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},     //115 暗和光属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementDark << 4 | enElementLight,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},     //116 暗和光属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementFire,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},      //117 心和火属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementFire,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},      //118 心和火属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementWater,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},     //119 心和水属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementWater,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},     //120 心和水属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementWood,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},       //121 心和木属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementWood,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},       //122 木和水属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementLight,    enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},    //123 心和光属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementLight,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //124 心和光属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementDark,    enGeneralTp_Null, 100,    0.7,  enAutoSkill_NULL},       //125 心和暗属性敌人造成的伤害减少30%
    {enAutoSkillTp_armor_attr2, enElementHeart << 4 | enElementDark,    enGeneralTp_Null, 100,    0.5,  enAutoSkill_NULL},       //126 心和暗属性敌人造成的伤害减少50%

    //双属性攻击提升
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementFire,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},        //127 心和火属性武将造成的普通攻击伤害变为原本的1.5倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementFire,    enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //128 心和火属性武将造成的普通攻击伤害变为原本的2倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementWater,   enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},       //129 心和水属性武将造成的普通攻击伤害变为原本的1.5倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementWater,   enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},       //130 心和水属性武将造成的普通攻击伤害变为原本的2倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementWood,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},         //131 心和木属性武将造成的普通攻击伤害变为原本的1.5倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementWood,    enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},         //132 心和木属性武将造成的普通攻击伤害变为原本的2倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementLight,   enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},      //133 心和光属性武将造成的普通攻击伤害变为原本的1.5倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementLight,   enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},      //134 心和光属性武将造成的普通攻击伤害变为原本的2倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementDark,    enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},         //135 心和暗属性武将造成的普通攻击伤害变为原本的1.5倍
    {enAutoSkillTp_atk2,        enElementHeart << 4 | enElementDark,    enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},         //136 心和暗属性武将造成的普通攻击伤害变为原本的2倍
    
    //弱点
    {enAutoSkillTp_elemburn,    enElementFire,  enGeneralTp_Null, 100,    3.0,  enAutoSkill_NULL},     //137 回合结束后，火属性武将全体对木属性敌人造成300%的属性伤害，受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    {enAutoSkillTp_elemburn,    enElementWater,  enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},    //138 回合结束后，水属性武将全体对火属性敌人造成300%的属性伤害，受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    {enAutoSkillTp_elemburn,    enElementWood,  enGeneralTp_Null, 100,    3.0,  enAutoSkill_NULL},     //139 回合结束后，木属性武将全体对水属性敌人造成300%的属性伤害，受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    {enAutoSkillTp_elemburn,    enElementLight,  enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},    //140 回合结束后，光属性武将全体对暗属性敌人造成300%的属性伤害，受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    {enAutoSkillTp_elemburn,    enElementDark,  enGeneralTp_Null, 100,    3.0,  enAutoSkill_NULL},     //141 回合结束后，暗属性武将全体对光属性敌人造成300%的属性伤害，受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    
    //神佑
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_JinShi, 100,    2.0,    enAutoSkill_NULL},   //142 金石之坚类武将造成的普通攻击变为原来的2倍，且每回合回复总血量10%的HP
    
    //水火木三防
    {enAutoSkillTp_armor_attr3, enElementMax,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},    //143 火水木三种属性敌人造成的伤害减少50%
    
    //欺压
    {enAutoSkillTp_null, enElementZero,    enGeneralTp_Null, 100,    10.0,  enAutoSkill_NULL},          //144 回合结束后，对战场上最虚弱的（剩余血量百分比，如果全部满血则攻击左起第一个）敌方单位造成1000%的伤害，该伤害不受属性影响，但受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    
    //冥王的邀请
    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},        //145 我方全体造成的普通攻击伤害变为原本的2倍,且每回合回复相当于自身HP50%的HP
    
    //瘟疫离宫
    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},        //146 对瘟疫状态敌人造成的普通攻击伤害变为原来的3倍
    
    //限界突破
    {enAutoSkillTp_revive,      0,           enGeneralTp_Null, 100,    0.7, enAutoSkill_NULL},        //147 龙之魄技能效果+攻击后，回复相当于自身攻击力的HP
    
    //各种有暴击的力
    {enAutoSkillTp_atk,         enElementFire,    enGeneralTp_Null, 30,    2.5, enAutoSkill_NULL},      //148 精锐之师 暴击 火 我方火属性武将的攻击力提升2倍，并且有几率触发2.5倍暴击
    {enAutoSkillTp_atk,         enElementWater,   enGeneralTp_Null, 30,    2.5, enAutoSkill_NULL},      //149 精锐之师 暴击 水 我方水属性武将的攻击力提升2倍，并且有几率触发2.5倍暴击
    {enAutoSkillTp_atk,         enElementWood,    enGeneralTp_Null, 30,    2.5, enAutoSkill_NULL},      //150 精锐之师 暴击 木 我方木属性武将的攻击力提升2倍，并且有几率触发2.5倍暴击
    {enAutoSkillTp_atk,         enElementLight,   enGeneralTp_Null, 30,    2.5, enAutoSkill_NULL},      //151 精锐之师 暴击 光 我方光属性武将的攻击力提升2倍，并且有几率触发2.5倍暴击
    {enAutoSkillTp_atk,         enElementDark,    enGeneralTp_Null, 30,    2.5, enAutoSkill_NULL},      //152 精锐之师 暴击 暗 我方暗属性武将的攻击力提升2倍，并且有几率触发2.5倍暴击

    // 各种属性武将的攻击力和回复力翻倍
    {enAutoSkillTp_valChg_atkrcv,  enElementLight,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},       //153  光属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,  enElementDark,       enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},        //154  暗属性武将的攻击力和回复力变为1.5倍
    
    {enAutoSkillTp_valChg_atkrcv,  enElementFire,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //155  火属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWater,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},       //156  水属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWood,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //157  木属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementLight,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},       //158  光属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementDark,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //159  暗属性武将的攻击力和回复力变为2倍

    // 各种属性武将的HP翻倍
    {enAutoSkillTp_valChg_hp,  enElementFire,        enGeneralTp_Null, 100,    2.5, enAutoSkill_NULL},         //160 火 HP 2.5倍 (火之魂) 火属性武将的血量提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementWater,       enGeneralTp_Null, 100,    2.5, enAutoSkill_NULL},         //161 水 HP 2.5倍 (水之魂) 水属性武将的血量提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementWood,        enGeneralTp_Null, 100,    2.5, enAutoSkill_NULL},         //162 木 HP 2.5倍 (木之魂) 木属性武将的血量提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementLight,       enGeneralTp_Null, 100,    2.5, enAutoSkill_NULL},         //163 光 HP 2.5倍 (光之魂) 光属性武将的血量提升2.5倍
    {enAutoSkillTp_valChg_hp,  enElementDark,        enGeneralTp_Null, 100,    2.5, enAutoSkill_NULL},         //164 暗 HP 2.5倍 (暗之魂) 暗属性武将的血量提升2.5倍

    {enAutoSkillTp_atk,     enElementWater,       enGeneralTp_God, 100,    2.5,   enAutoSkill_NULL},         //165  水系武将和神将类武将造成的伤害提升2.5倍

    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.2, enAutoSkill_NULL},            //166  满血状态下受到伤害减少80%

    {enAutoSkillTp_burn,    enElementInstead,     enGeneralTp_Null, 100,    1.5,    enAutoSkill_NULL},              //167  攻击后，追加一次全体攻击，伤害等同于自身攻击力×1.5且属性必定克制敌将

    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},                //168 Zeus升级版，当HP为满时，我方全体造成的普通攻击伤害变为原本的3倍；当HP为1点时，我方全体造成的普通攻击伤害变为原本的4倍 
    
    // 相同技能不同名字
    {enAutoSkillTp_valChg_atkrcv,   enElementWood,      enGeneralTp_Null, 100,    1.5,  enAutoSkill_NULL},     //169 同82  木属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_recover,         enElementWater,     enGeneralTp_Null, 100,    3.0,  enAutoSkill_NULL},     //170 同32  歌莺舞燕 每回合回复相当于队长宠物自身回复力300%的HP，没有产生消除的话不会触发该效果
    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    10.0, enAutoSkill_NULL},            //171 同30  时光术 宝石移动的最大时限延长较多
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_GuoShi, 100,    2.5,    enAutoSkill_NULL},           //172 同41   龙骑士之怒 龙类宠物造成的普通攻击伤害变为原本的2.5倍
    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    2.0,   enAutoSkill_NULL},              //173 同26   每回合对所有敌人造成相当于队长宠物自身攻击力200%的伤害，该伤害不受属性影响，但受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},             //174 同24   每回合回复相当于队长宠物自身回复力200%的HP，没有产生消除的话不会触发该效果

    {enAutoSkillTp_fanji,     enElementFire,    enGeneralTp_Null, 50,    5.0,   enAutoSkill_NULL},              //175   受到伤害时，有约50%几率进行反击，造成5倍于受到伤害的火属性伤害

    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    3.0, enAutoSkill_NULL},                //176   当HP不足30%时，攻击力变为原本的3倍

    // 双属性防，两种属性防的效果不同
    {enAutoSkillTp_armor_attr2V2, enElementFire  << 4 | enElementDark,    enGeneralTp_Null, 100,    0.4 * 100 + 0.6,    enAutoSkill_NULL},    //177   火属性减伤60%，暗属性减伤40%
    {enAutoSkillTp_armor_attr2V2, enElementWater << 4 | enElementDark,    enGeneralTp_Null, 100,    0.4 * 100 + 0.6,    enAutoSkill_NULL},    //178   水属性减伤60%，暗属性减伤40%
    {enAutoSkillTp_armor_attr2V2, enElementWood  << 4 | enElementDark,   enGeneralTp_Null, 100,    0.4 * 100 + 0.6, enAutoSkill_NULL},    //179   木属性减伤60%， 暗属性减伤40%
    {enAutoSkillTp_armor_attr2V2, enElementLight << 4 | enElementWater,   enGeneralTp_Null, 100,    0.4 * 100 + 0.6,    enAutoSkill_NULL},    //180   光属性减伤60%，水属性减伤40%
    {enAutoSkillTp_armor_attr2V2, enElementDark  << 4 | enElementLight,   enGeneralTp_Null, 100,    0.4 * 100 + 0.6,    enAutoSkill_NULL},    //181   暗属性减伤60%，光属性减伤40%

    // 相同技能不同名字
    {enAutoSkillIp_money_exp_drop,            1,             0,               0,    2.0,    enAutoSkill_NULL},           //182 同48 掉金率2倍
    
    {enAutoSkillTp_atk,         enElementHeart,    enGeneralTp_Null, 100,    2.5,   enAutoSkill_NULL},          //183 心属性武将造成的普通攻击伤害变为原本的2.5倍

    // ---------3.4-------------
    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.8, enAutoSkill_NULL},            //184 同22 墨守 受到的所有伤害减少20%

    {enAutoSkillTp_atk_cmb,    enElementMax,     enGeneralTp_Null,   7,    4.0, enAutoSkill_NULL},           //185 连击达到7 Combos以上时，所有武将的攻击力变为4.0倍

    {enAutoSkillTp_valChg_hp_Elm2,  enElementWater << 4 | enElementWood,  enGeneralTp_Null, 100,    2.0,    enAutoSkill_NULL},        //186 木属性和水属性武将的HP变为2倍

    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_XiaoYong, 100,    2.0,  enAutoSkill_NULL},              //187 骁勇善战类武将的攻击力变为2倍

    {enAutoSkillTp_atk_elm_cnt,    enElementMax,    enGeneralTp_Null,   4,    4.0,  enAutoSkill_NULL},             //188 4种属性同时攻击时，所有武将的攻击力变为4倍

    {enAutoSkillTp_atk_elm_cnt,    enElementMax,    enGeneralTp_Null,   3,    3.0,  enAutoSkill_NULL},             //189 3种属性同时攻击时，所有武将的攻击力变为3.0倍

    {enAutoSkillTp_atk_cmb,    enElementMax,    enGeneralTp_Null,   4,    2.5,  enAutoSkill_NULL},            //190 连击达到4 Combos以上时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_elm_cnt,    enElementMax,    enGeneralTp_Null,   5,    5.0,  enAutoSkill_NULL},            //191 5种属性同时攻击时，所有武将的攻击力变为5倍，6种属性同时攻击时，所有武将的攻击力变为6倍

    {enAutoSkillTp_atk_cmb,    enElementMax,    enGeneralTp_Null,   10,    10,   enAutoSkill_NULL},            //192 连击达到10 Combos以上时，所有武将的攻击力变为10倍

    // 技能相同名字不同
    {enAutoSkillTp_valChg_atkrcv,  enElementFire,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //193 同155  火属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWater,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},       //194 同156  水属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementWood,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //195 同157  木属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementLight,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},       //196 同158  光属性武将的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkrcv,  enElementDark,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //197 同159  暗属性武将的攻击力和回复力变为2倍

    {enAutoSkillTp_valChg_hprcv,   enElementFire,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //198 火属性武将的HP和回复力变为2倍

    {enAutoSkillTp_valChg_hprcv,   enElementWater,       enGeneralTp_Null, 100,    2.0, enAutoSkill_NULL},       //199 水属性武将的HP和回复力变为2倍

    {enAutoSkillTp_valChg_hprcv,   enElementWood,       enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},        //200 木属性武将的HP和回复力变为2倍

    {enAutoSkillTp_atk_cmb,    enElementMax,    enGeneralTp_Null,   3,    2.0,    enAutoSkill_NULL},                   //201 连击达到3Combos以上时，所有武将的攻击力变为2倍

    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    2.0,   enAutoSkill_NULL},                  //202 同26 每回合对所有敌人造成相当于队长武将自身攻击力200%的伤害，该伤害不受属性影响，但受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果

    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.8, enAutoSkill_NULL},                //203 同22 墨守 受到的所有伤害减少20%

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   0,    2.5, enAutoSkill_NULL},            //204 火、水、木同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_fanji,     enElementLight,    enGeneralTp_Null, 50,    5.0,  enAutoSkill_NULL},                 //205 同93 闪电反击 受到伤害时，有约50%几率进行反击，造成5倍于受到伤害的光属性伤害

    {enAutoSkillTp_armor_attr, enElementDark,    enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},                //206 同20 固若金汤 暗 暗属性敌人造成的伤害减少50%

    {enAutoSkillTp_valChg_atkrcv,  enElementMax,   enGeneralTp_GongShou, 100,    1.5,   enAutoSkill_NULL},          //207 攻守兼备类武将的攻击力和回复力变为1.5倍

    {enAutoSkillTp_valChg_all,  enElementMax,     enGeneralTp_GongShou, 100,    1.5,    enAutoSkill_NULL},           //208 攻守兼备类武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_armor_attr,  enElementWood,    enGeneralTp_Null,     100,    0.8,    enAutoSkill_JingRuiCritHuo},    //209 我方火属性武将的攻击力提升2.5倍，且木属性减伤20%

    {enAutoSkillTp_armor_attr,  enElementFire,    enGeneralTp_Null,     100,    0.8,    enAutoSkill_JingRuiCritShui},    //210 我方水属性武将的攻击力提升2.5倍，且火属性减伤20%

    {enAutoSkillTp_armor_attr,  enElementWater,    enGeneralTp_Null,     100,    0.8,    enAutoSkill_JingRuiCritMu},    //211 我方木属性武将的攻击力提升2.5倍，且水属性减伤20%

    {enAutoSkillTp_armor_attr,  enElementDark,    enGeneralTp_Null,     100,    0.8,    enAutoSkill_JingRuiCritGuang},    //212 我方光属性武将的攻击力提升2.5倍，且暗属性减伤20%

    {enAutoSkillTp_armor_attr,  enElementLight,    enGeneralTp_Null,     100,    0.8,    enAutoSkill_JingRuiCritAn},    //213 我方暗属性武将的攻击力提升2.5倍，且光属性减伤20%

    {enAutoSkillTp_burn,    enElementZero,     enGeneralTp_Null, 100,    5.0,   enAutoSkill_JingRuiCritHuo},    //214 我方火属性武将的攻击力提升2.5倍，且每回合对所有敌人造成相当于队长武将自身攻击力300%（上调至500%）的伤害，该伤害不受属性影响，但受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果

    {enAutoSkillTp_revive,      0,              enGeneralTp_Null, 100,    0.5,  enAutoSkill_JingRuiCritShui},    //215 我方水属性武将的攻击力提升2.5倍，且当受到一次攻击使你致死时，如果受到攻击前HP为总HP的70%（改为50%）或以上的话，你将会以1点HP生还

    {enAutoSkillTp_armor_attr3, enElementMax,    enGeneralTp_Null, 100,   0.67, enAutoSkill_JingRuiCritMu},     //216 我方木属性武将的攻击力提升2.5倍，且火水木属性减伤15%（改为33%）

    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    3.0,  enAutoSkill_JingRuiCritGuang},    //217 我方光属性武将的攻击力提升2.5倍，且每回合回复相当于主将自身回复力300%的HP，没有产生消除的话不会触发该效果

    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    3.0,  enAutoSkill_JingRuiCritAn},     //218 我方暗属性武将的攻击力提升2.5倍，且宝石移动的最大时限加3秒
    
    {enAutoSkillTp_valChg_hprcv,   enElementFire,       enGeneralTp_Null, 100,    1.5,  enAutoSkill_JingRuiCritHuo},        //219 我方火属性武将的攻击力提升2.5倍，且火属性武将的HP和回复力提升1.5倍
    
    {enAutoSkillTp_valChg_hprcv,   enElementWater,       enGeneralTp_Null, 100,    1.5, enAutoSkill_JingRuiCritShui},       //220 我方水属性武将的攻击力提升2.5倍，且水属性武将的HP和回复力提升1.5倍
    
    {enAutoSkillTp_valChg_hprcv,   enElementWood,       enGeneralTp_Null, 100,    1.5,  enAutoSkill_JingRuiCritMu},         //221 我方木属性武将的攻击力提升2.5倍，且木属性武将的HP和回复力提升1.5倍

    {enAutoSkillTp_fanji,     enElementLight,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_JingRuiCritGuang},             //222 我方光属性武将的攻击力提升2.5倍，且受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的光属性伤害
   
    {enAutoSkillTp_fanji,     enElementDark,    enGeneralTp_Null, 25,    10.0,   enAutoSkill_JingRuiCritAn},              //223 我方暗属性武将的攻击力提升2.5倍，且受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的暗属性伤害

    {enAutoSkillTp_valChg_rcv,  enElementFire,   enGeneralTp_Null, 100,   2.0,  enAutoSkill_JingRuiCritHuo},     //224 我方火属性武将的攻击力提升2.5倍，且火属性武将的回复力提升2倍

    {enAutoSkillTp_valChg_rcv,  enElementWater,   enGeneralTp_Null, 100,   2.0,  enAutoSkill_JingRuiCritShui},     //225 我方水属性武将的攻击力提升2.5倍，且水属性武将的回复力提升2倍

    {enAutoSkillTp_valChg_rcv,  enElementWood,   enGeneralTp_Null, 100,   2.0,  enAutoSkill_JingRuiCritMu},     //226 我方木属性武将的攻击力提升2.5倍，且木属性武将的回复力提升2倍

    {enAutoSkillTp_valChg_rcv,  enElementLight,   enGeneralTp_Null, 100,   2.0,  enAutoSkill_JingRuiCritGuang},     //227 我方光属性武将的攻击力提升2.5倍，且光属性武将的回复力提升2倍

    {enAutoSkillTp_valChg_rcv,  enElementDark,   enGeneralTp_Null, 100,   2.0,  enAutoSkill_JingRuiCritAn},     //228 我方暗属性武将的攻击力提升2.5倍，且暗属性武将的回复力提升2倍

    {enAutoSkillTp_atk,     enElementMax,     enGeneralTp_GongShou, 100,    1.5,    enAutoSkill_NULL},       //229 攻守兼备类武将的攻击力变为1.5倍

    {enAutoSkillTp_valChg_atkhp,   enElementMax,   enGeneralTp_GongShou, 100,    2.0,    enAutoSkill_NULL},       //230 攻守兼备类武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_atkrcv,  enElementMax,   enGeneralTp_ShengSheng, 100,    1.5,   enAutoSkill_NULL},      //231 生生不息类武将的攻击力和回复力变为1.5倍

    {enAutoSkillTp_valChg_all,  enElementMax,     enGeneralTp_ShengSheng, 100,    1.5,    enAutoSkill_NULL},      //232 生生不息类武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_atk,     enElementMax,     enGeneralTp_XiaoYong, 100,    1.5,    enAutoSkill_NULL},       //233 骁勇善战类武将的攻击力变为1.5倍
    
    {enAutoSkillTp_valChg_atkhp,   enElementMax,   enGeneralTp_XiaoYong, 100,    2.0,    enAutoSkill_NULL},       //234 骁勇善战类武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_atkrcv,  enElementMax,   enGeneralTp_JinShi, 100,    1.5,   enAutoSkill_NULL},      //235 金石之坚类武将的攻击力和回复力变为1.5倍
    
    {enAutoSkillTp_valChg_all,  enElementMax,     enGeneralTp_JinShi, 100,    1.5,    enAutoSkill_NULL},      //236 金石之坚类武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_valChg_atkrcv,  enElementMax,   enGeneralTp_XiaoYong, 100,    1.5,   enAutoSkill_NULL},      //237 骁勇善战类武将的攻击力和回复力变为1.5倍
    
    {enAutoSkillTp_valChg_all,  enElementMax,     enGeneralTp_XiaoYong, 100,    1.5,    enAutoSkill_NULL},      //238 骁勇善战类武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_atk_aoe,     enElementFire,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},      //239 发动火属性群攻时，我方火属性武将的攻击力提升2.5倍

    {enAutoSkillTp_atk_aoe,     enElementWater,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},      //240 发动水属性群攻时，我方水属性武将的攻击力提升2.5倍

    {enAutoSkillTp_atk_aoe,     enElementWood,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},      //241 发动木属性群攻时，我方木属性武将的攻击力提升2.5倍

    {enAutoSkillTp_atk_aoe,     enElementDark,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},      //242 发动暗属性群攻时，我方暗属性武将的攻击力提升2.5倍

    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.2, enAutoSkill_NULL},            //243  当HP不足20%时，受到的所有伤害减少80%

    {enAutoSkillTp_fanji,     enElementFire,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_JingRuiCritHuo},             //244 我方火属性武将的攻击力提升2.5倍，且受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的火属性伤害

    {enAutoSkillTp_fanji,     enElementWater,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_JingRuiCritShui},             //245 我方水属性武将的攻击力提升2.5倍，且受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的水属性伤害

    {enAutoSkillTp_fanji,     enElementWood,    enGeneralTp_Null, 25,    10.0,  enAutoSkill_JingRuiCritMu},             //246 我方木属性武将的攻击力提升2.5倍，且受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的木属性伤害

    {enAutoSkillTp_atk,     enElementFire,     enGeneralTp_GongShou, 100,    2.5,    enAutoSkill_NULL},       //247 我方火属性和攻守兼备类武将的攻击力提升2.5倍
   
    {enAutoSkillTp_atk,     enElementWater,     enGeneralTp_JinShi, 100,    2.5,    enAutoSkill_NULL},       //248 我方水属性和金石之坚类武将的攻击力提升2.5倍
    
    {enAutoSkillTp_atk,     enElementWood,     enGeneralTp_GuoShi, 100,    2.5,    enAutoSkill_NULL},       //249 我方木属性和国士无双类武将的攻击力提升2.5倍
    
    {enAutoSkillTp_atk,     enElementLight,     enGeneralTp_God, 100,    2.5,    enAutoSkill_NULL},       //250 我方光属性和神将类武将的攻击力提升2.5倍
    
    {enAutoSkillTp_atk,     enElementDark,     enGeneralTp_ShengSheng, 100,    2.5,    enAutoSkill_NULL},       //251 我方暗属性和生生不息类武将的攻击力提升2.5倍

    // -------------------4.0-----------------------
    {enAutoSkillTp_delay,       0,              enGeneralTp_Null, 100,    5.0,  enAutoSkill_NULL},     //252 同29 宝石移动的最大时限延长少许(5.0)

    {enAutoSkillTp_atk,     enElementFire,     enGeneralTp_JinShi, 100,    2.0,    enAutoSkill_NULL},       //253 火属性和金石之坚类武将的攻击力变为2倍

    {enAutoSkillTp_atk,     enElementWater,     enGeneralTp_XiaoYong, 100,    2.0,    enAutoSkill_NULL},       //254 水属性和骁勇善战类武将的攻击力变为2倍

    {enAutoSkillTp_atk,     enElementWood,     enGeneralTp_XiaoYong, 100,    2.0,    enAutoSkill_NULL},       //255 木属性和骁勇善战类武将的攻击力变为2倍

    {enAutoSkillTp_atk,     enElementLight,     enGeneralTp_JinShi, 100,    2.0,    enAutoSkill_NULL},       //256 光属性和金石之坚类武将的攻击力变为2倍

    {enAutoSkillTp_atk,     enElementDark,     enGeneralTp_XiaoYong, 100,    2.0,    enAutoSkill_NULL},       //257 暗属性和骁勇善战类武将的攻击力变为2倍

    {enAutoSkillIp_money_exp_drop,            0,             0,               1,    1.5,    enAutoSkill_NULL},        //258 同52 武将掉率大幅提升(1.5)

    {enAutoSkillTp_valChg_atkhp,   enElementFire,   enGeneralTp_Null, 100,    2.0,    enAutoSkill_NULL},       //259 火属性武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_atkhp,   enElementWater,   enGeneralTp_Null, 100,    2.0,    enAutoSkill_NULL},       //260 水属性武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_atkhp,   enElementWood,   enGeneralTp_Null, 100,    2.0,    enAutoSkill_NULL},       //261 木属性武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_all,  enElementLight,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},         //262 光属性武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_valChg_all,  enElementDark,       enGeneralTp_Null, 100,    1.5, enAutoSkill_NULL},          //263 暗属性武将的HP、攻击力和回复力变为1.5倍

    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    5.0, enAutoSkill_NULL},                //264 当HP不足20%时，所有武将的攻击力变为5倍

    {enAutoSkillTp_atk,       enElementDark,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},           //265 自身和暗属性武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_cmb,    enElementMax,    enGeneralTp_Null,   8,    5.0,  enAutoSkill_NULL},            //266 连击达到8 Combos以上时，所有武将的攻击力变为5倍

    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},            //267 同24 每回合回复相当于队长武将自身回复力200%的HP，没有产生消除的话不会触发该效果

    {enAutoSkillTp_valChg_hp,  enElementMax,    enGeneralTp_JinShi, 100,  2.5,   enAutoSkill_NULL},         //268 金石之坚类武将的HP变为2.5倍

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   1,    2.5, enAutoSkill_NULL},            //269 火、光、暗同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   2,    2.5, enAutoSkill_NULL},            //270 水、木、暗同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   3,    2.5, enAutoSkill_NULL},            //271 水、木、光同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   4,    2.5, enAutoSkill_NULL},            //272 火、木、光同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_atk_elm_type,    enElementMax,    enGeneralTp_Null,   5,    2.5, enAutoSkill_NULL},            //273 火、水、暗同时攻击时，所有武将的攻击力变为2.5倍

    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_ShengSheng, 100,    2.0,   enAutoSkill_NULL},     //274 生生不息类武将的HP变为2倍

    {enAutoSkillTp_valChg_hp,  enElementMax,       enGeneralTp_ShengSheng, 100,    2.5,   enAutoSkill_NULL},     //275 生生不息类武将的HP变为2.5倍

    {enAutoSkillTp_valChg_atkhp,   enElementHeart,   enGeneralTp_Null, 100,    2.0,    enAutoSkill_NULL},        //276 心属性武将的HP和攻击力变为2倍

    {enAutoSkillTp_valChg_hp,  enElementHeart,       enGeneralTp_Null, 100,    2.5,   enAutoSkill_NULL},        //277 心属性武将的HP变为2.5倍

    {enAutoSkillTp_atk,         enElementHeart,    enGeneralTp_Null, 100,    2.5,  enAutoSkill_NULL},             //278 心属性武将的攻击力变为2.5倍

    {enAutoSkillTp_burn,    enElementGod,     enGeneralTp_Null, 100,    1000,   enAutoSkill_NULL},              //279 每回合对所有敌人造成1000点固定伤害，该伤害不受属性影响，没有产生消除的话不会触发该效果

    {enAutoSkillTp_atk_cmb,    enElementFire,     enGeneralTp_Null,   6,    1.4, enAutoSkill_JingRuiCritHuo},           //280 我方火属性武将的攻击力提升2.5倍,且连击达到6Combos以上时，我方火属性武将的攻击力提升3.5倍

    {enAutoSkillTp_atk_cmb,    enElementWater,     enGeneralTp_Null,   6,    1.4, enAutoSkill_JingRuiCritShui},           //281 连击达到6Combos以上时，我方水属性武将的攻击力提升3.5倍
    
    {enAutoSkillTp_atk_cmb,    enElementWood,     enGeneralTp_Null,   6,    1.4, enAutoSkill_JingRuiCritMu},           //282 连击达到6Combos以上时，我方木属性武将的攻击力提升3.5倍
    
    {enAutoSkillTp_atk_cmb,    enElementLight,     enGeneralTp_Null,   6,    1.4, enAutoSkill_JingRuiCritGuang},           //283 连击达到6Combos以上时，我方光属性武将的攻击力提升3.5倍
    
    {enAutoSkillTp_atk_cmb,    enElementDark,     enGeneralTp_Null,   6,    1.4, enAutoSkill_JingRuiCritAn},           //284 连击达到6Combos以上时，我方暗属性武将的攻击力提升3.5倍
    
    //欺压2
    {enAutoSkillTp_null, enElementZero,    enGeneralTp_Null, 100,    15.0,  enAutoSkill_NULL},          //285 回合结束后，对战场上最虚弱的（剩余血量百分比，如果全部满血则攻击左起第一个）敌方单位造成1500%的伤害，该伤害不受属性影响，但受到防御力减免，最低伤害为0，没有产生消除的话不会触发该效果
    
    //瘟疫离宫2
    {enAutoSkillTp_atk,     enElementMax,    enGeneralTp_Null, 100,    4.0, enAutoSkill_NULL},        //286 对瘟疫状态敌人造成的普通攻击伤害变为原来的4倍
    
    //限界突破2
    {enAutoSkillTp_revive,      0,           enGeneralTp_Null, 100,    0.5, enAutoSkill_NULL},        //287 龙之魄技能效果+攻击后，回复相当于自身攻击力的HP
    
    {enAutoSkillTp_atk,     enElementMax,       enGeneralTp_XiaoYong, 100,    2.0,  enAutoSkill_NULL},              //288 骁勇善战类武将的攻击力变为2倍
    
    // 各种属性武将的攻击力和回复力翻倍
    {enAutoSkillTp_valChg_atkrcv,   enElementLight,     enGeneralTp_Null,       100,    1.5,  enAutoSkill_NULL},            //289  所有光属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementDark,      enGeneralTp_Null,       100,    1.5,  enAutoSkill_NULL},            //290  所有暗属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_hprcv,    enElementDark,      enGeneralTp_Null,       100,    2.0,  enAutoSkill_NULL},            //291  所有暗属性武将的HP和回复力变为2.0倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    2.0,  enAutoSkill_NULL},            //292  所有魔将攻击力变为2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    2.5,  enAutoSkill_NULL},            //293  所有魔将攻击力变为2.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_mojiang,    100,    1.5,  enAutoSkill_NULL},            //294  所有魔将攻击力,回复力变为1.5倍
    {enAutoSkillTp_valChg_all,      enElementMax,       enGeneralTp_mojiang,    100,    1.5,  enAutoSkill_NULL},            //295  所有魔将攻击力,HP,回复力变为1.5倍
    {enAutoSkillTp_valChg_hp,       enElementMax,       enGeneralTp_mojiang,    100,    2.0,  enAutoSkill_NULL},            //296  所有魔将HP变为2倍
    {enAutoSkillTp_valChg_hprcv,    enElementMax,       enGeneralTp_mojiang,    100,    2.0,  enAutoSkill_NULL},            //297  所有魔将HP,回复力变为2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    2.5,  enAutoSkill_NULL},            //298  HP在50%以上时,所有武将攻击力变为2.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    2.5,  enAutoSkill_NULL},            //299  HP在50%以上时,所有武将攻击力变为2.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_mojiang,    100,    1.5,  enAutoSkill_NULL},            //300  所有魔将攻击力,回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_mojiang,    100,    2.0,  enAutoSkill_NULL},            //301  所有魔将攻击力,回复力变为2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    3.5,  enAutoSkill_NULL},            //302  所有魔将攻击力变为3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    3.5,  enAutoSkill_NULL},            //303  所有魔将攻击力变为3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    3.5,  enAutoSkill_NULL},            //304  HP在80%以下时,所有武将攻击力变为3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    3.5,  enAutoSkill_NULL},            //305  HP在80%以下时,所有武将攻击力变为3.5倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,       6,      3,    enAutoSkill_NULL,
        CAutoSkillAppend().addIfGenIDList("&((224|225)&(365|366))")},                                                       //306  与魔貂蝉，神吕布齐上阵时且水、光、暗同时攻击时,所有武将的攻击力变为3倍
    {enAutoSkillTp_burn,            enElementZero,       enGeneralTp_Null,       100,    2.0,  enAutoSkill_NULL},           //307  每回合正常攻击后,对所有敌人造成队长攻击力200%伤害,该伤害不受属性影响,但受防御力减免
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       3,    2.0,  enAutoSkill_NULL},              //308  连击数达到36Combos以上时,所有武将攻击力变为2倍
    {enAutoSkillTp_armor_all,       enElementMax,       enGeneralTp_Null,       100,    0.8,  enAutoSkill_NULL},            //309  所有伤害减少20%
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    1.5,  enAutoSkill_NULL},            //310  龙系武将攻击力变为1.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    2.5,  enAutoSkill_NULL},            //311  龙系武将攻击力变为2.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    3.5,  enAutoSkill_NULL},            //312  龙系武将攻击力变为3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    3.5,  enAutoSkill_NULL},            //313  龙系武将攻击力变为3.5倍
    {enAutoSkillTp_armor_all,       enElementMax,       enGeneralTp_Null,       100,    0.8,  enAutoSkill_NULL},            //314  所有伤害减少20%
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    3.0,  enAutoSkill_NULL},            //315  HP在30%以下时,所有武将攻击力变为3倍
    {enAutoSkillIp_del_half_time_atk2V2, enElementFire, enGeneralTp_Null,       100,    2.5,  enAutoSkill_NULL},            //316  减少一半挪动宝珠时间,火属性武将攻击力变为2.5倍
    {enAutoSkillTp_valChg_hprcv,    enElementFire,       enGeneralTp_Null,      100,    2.0,  enAutoSkill_NULL},            //317  所有火属性武将HP和回复力变为2倍
    {enAutoSkillTp_valChg_hprcv,    enElementWater,      enGeneralTp_Null,      100,    2.0,  enAutoSkill_NULL},            //318  所有水属性武将HP和回复力变为2倍
    {enAutoSkillTp_valChg_hprcv,    enElementWood,       enGeneralTp_Null,      100,    2.0,  enAutoSkill_NULL},            //319  所有木属性武将HP和回复力变为2倍
    {enAutoSkillTp_valChg_hprcv,    enElementLight,      enGeneralTp_Null,      100,    2.0,  enAutoSkill_NULL},            //320  所有光属性武将HP和回复力变为2倍
    {enAutoSkillTp_atk,             enElementWater,       enGeneralTp_Null,       100,    3.5,  enAutoSkill_NULL,
        CAutoSkillAppend().addIfGenIDList("(751|752)&(753|754)&(755|756)&(757|758)").addEffectReplyHp(2000)},                                    //321  HP满时,所有水属性武将攻击力提升3.5倍;与魔四都督齐上阵时,每回合回复2000HP
    {enAutoSkillTp_atk,             enElementWater,       enGeneralTp_Null,       100,    3.5,  enAutoSkill_NULL,
        CAutoSkillAppend().addIfGenIDList("(751|752)&(753|754)&(755|756)&(757|758)").addEffectReplyHp(2000)},                                    //322  HP满时,所有水属性武将攻击力提升3.5倍;与魔四都督齐上阵时,每回合回复2000HP
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,   20,    2.0,    enAutoSkill_NULL}, //323  魔将类武将攻击力提升2倍;有20%几率使敌将中毒一回合,中毒敌人每回合损失使用技能武将攻击力3倍的伤害,该伤害无视属性与防御'
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,   20,    2.0,    enAutoSkill_NULL}, //324  魔将类武将攻击力提升2倍;有20%几率使敌将中毒一回合,中毒敌人每回合损失使用技能武将攻击力3倍的伤害,该伤害无视属性与防御'
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,   20,    2.0,    enAutoSkill_NULL}, //325  魔将类武将攻击力提升2倍;有20%几率使敌将冻结一回合,冻结敌人每回合损失使用技能武将攻击力的伤害,该伤害无视属性与防御
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,   20,    2.0,    enAutoSkill_NULL}, //326  魔将类武将攻击力提升2倍;有20%几率使敌将冻结一回合,冻结敌人每回合损失使用技能武将攻击力的伤害,该伤害无视属性与防御
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    1.0,  enAutoSkill_NULL},            //327 魔将类武将攻击力有80%提升2倍,20%提升2.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,    100,    1.0,  enAutoSkill_NULL},            //328 魔将类武将攻击力有80%提升2倍,20%提升2.5倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_mojiang,    100,    1.0,  enAutoSkill_NULL},            //329 魔将类武将连击时,提升攻击力(3连2倍,6连3倍,9连4倍)
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_mojiang,    100,    1.0,  enAutoSkill_NULL},            //330 魔将类武将连击时,提升攻击力(3连2倍,6连3倍,9连4倍)
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,    100,    1.0,  enAutoSkill_NULL},            //331  连击增加combo数
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //332   骁勇善战类武将的攻击力提升2倍
    {enAutoSkillTp_valChg_rcv,      enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //333   骁勇善战类武将的回复力提升2倍
    {enAutoSkillTp_valChg_hp,       enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //334   骁勇善战类武将的血量提升2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_God,          100,    3.0f,  enAutoSkill_NULL},            //335   神将类武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_mojiang,      100,    3.0f,  enAutoSkill_NULL},            //336   魔将类武将的攻击力提升3倍
    {enAutoSkillTp_armor_all,       enElementMax,       enGeneralTp_Null,         100,    0.9f,  enAutoSkill_NULL},            //337   受到的所有伤害小幅减少   ?10%
    {enAutoSkillTp_armor_all,       enElementMax,       enGeneralTp_Null,         100,    0.8f,  enAutoSkill_NULL},            //338   受到的所有伤害减少      ?20%
    {enAutoSkillTp_atk,             enElementFire,      enGeneralTp_GuoShi,       100,    2.0f,  enAutoSkill_NULL},            //339   火属性和国士无双类武将的攻击力提升2倍
    {enAutoSkillTp_revive,          enElementMax,       enGeneralTp_Null,         100,    0.5f,  enAutoSkill_NULL},            //340   当受到致死攻击时，有几率生还            ?50%
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //341   骁勇善战类武将的血量和攻击力提升2倍
    {enAutoSkillTp_valChg_all,      enElementMax,       enGeneralTp_XiaoYong,     100,    1.5f,  enAutoSkill_NULL},            //342   骁勇善战类武将的血量、攻击力和回复力提升1.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //343   骁勇善战类武将的攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //344   骁勇善战类武将的攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_hprcv,    enElementMax,       enGeneralTp_XiaoYong,     100,    2.0f,  enAutoSkill_NULL},            //345   骁勇善战类武将的血量和回复力提升2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_XiaoYong,     100,    3.0f,  enAutoSkill_NULL},            //346   骁勇善战类武将的攻击力提升3倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,           4,    3.5f,  enAutoSkill_NULL},            //347   火、木、光同时攻击时，所有武将的攻击力提升3.5倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,           2,    3.5f,  enAutoSkill_NULL},            //348   水、木、暗同时攻击时，所有武将的攻击力提升3.5倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,           7,    3.5f,  enAutoSkill_NULL},            //349   木、光、暗同时攻击时，所有武将的攻击力提升3.5倍
    {enAutoSkillTp_atk_elm_type4,   enElementMax,       enGeneralTp_Null,           0,    5.0f,  enAutoSkill_NULL},            //350   火、水、木、光同时攻击时，所有武将的攻击力提升5倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,           5,    3.5,   enAutoSkill_NULL},            //351   火、水、暗同时攻击时，所有武将的攻击力提升3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_ShengSheng,   100,    3.5f,  enAutoSkill_NULL},            //352   当血量高于80%时，所有生生不息类武将的攻击力提升3.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_XiaoYong,     100,    4.0,   enAutoSkill_NULL},            //353   当血量不足80%时，所有骁勇善战类武将的攻击力提升4倍
    {enAutoSkillTp_atk2, enElementFire << 4 | enElementWood,    enGeneralTp_Null,  100,    2.0,   enAutoSkill_NULL},            //354   火属性和木属性武将的攻击力提升2倍
    {enAutoSkillTp_valChg_hp_Elm2, enElementFire << 4 | enElementLight,   enGeneralTp_Null,  100,    2.0,   enAutoSkill_NULL},            //355   火属性和光属性武将的血量提升2倍
    {enAutoSkillTp_null,            enElementMax,       enGeneralTp_Null,            0,    1.0,   enAutoSkill_NULL},            //356   显示文本
    {enAutoSkillTp_null,            enElementMax,       enGeneralTp_Null,            0,    1.0,   enAutoSkill_NULL},            //357   显示文本2
    {enAutoSkillTp_valChg_atkrcv,             enElementFire,      enGeneralTp_XiaoYong,       100,    2.0f,  enAutoSkill_NULL},            //358   火属性和骁勇善战类武将攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,             enElementWater,     enGeneralTp_GongShou,       100,    2.0f,  enAutoSkill_NULL},            //359   水属性和攻守兼备类武将攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,             enElementWood,      enGeneralTp_ShengSheng,       100,    2.0f,  enAutoSkill_NULL},            //360   木属性和生生不息类武将攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,             enElementLight,      enGeneralTp_JinShi,       100,    2.0f,  enAutoSkill_NULL},            //361   光属性和金石之坚武将攻击力和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,             enElementDark,      enGeneralTp_mojiang,       100,    2.0f,  enAutoSkill_NULL},            //362   暗属性和魔将类武将攻击力和回复力提升2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_JinShi,     100,    3.0f,  enAutoSkill_NULL},            //363   金石之坚类武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GongShou,     100,    3.0f,  enAutoSkill_NULL},            //364   攻守兼备类武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_ShengSheng,     100,    3.0f,  enAutoSkill_NULL},            //365   生生不息类武将的攻击力提升3倍
    {enAutoSkillTp_armor_all,       0,           enGeneralTp_Null, 100,    0.9, enAutoSkill_NULL},                              //366 圣诞快乐 受到的所有伤害小幅减少(10%)
    
    
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_XiaoYong,   100,    2.5f,  enAutoSkill_NULL},               //367   骁勇善战类武将的攻击力提升2.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_GongShou,   100,    1.5f,  enAutoSkill_NULL},               //368   攻守兼备类武将的攻击力变和回复力为1.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_JinShi,     100,    2.0f,  enAutoSkill_NULL},               //369   金石之坚类武将的攻击力提升2.0倍
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_JinShi,     100,    2.0f,  enAutoSkill_NULL},               //370   金石之坚类武将的血量和攻击力提升2倍
    {enAutoSkillTp_valChg_hp,       enElementMax,       enGeneralTp_JinShi,     100,    2.0f,  enAutoSkill_NULL},               //371   金石之坚类武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,       enElementMax,       enGeneralTp_GongShou,   100,    2.0f,  enAutoSkill_NULL},               //372   攻守兼备类武将的血量提升2倍
    {enAutoSkillTp_valChg_hp,       enElementMax,       enGeneralTp_XiaoYong,   100,    2.0f,  enAutoSkill_NULL},               //373   骁勇善战类武将的血量提升2倍
    {enAutoSkillTp_delay,           0,                  enGeneralTp_Null,       100,    3.0,   enAutoSkill_NULL},               //374   宝珠挪动的最大时限延长（3秒）
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    5.0,   enAutoSkill_NULL},               //375   当血量不足10%时，所有武将的攻击力提升5倍
    {enAutoSkillTp_atk,             enElementFire,      enGeneralTp_Null,       100,    3.0,   enAutoSkill_NULL},               //376   当血量不足50%时，所有火属性武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementWater,     enGeneralTp_Null,       100,    5.0,   enAutoSkill_NULL},               //377   当血量不足5%时，所有水属性武将的攻击力提升5倍
    {enAutoSkillTp_atk,             enElementWood,      enGeneralTp_Null,       100,    3.0,   enAutoSkill_NULL},               //378   满血量时，所有木属性武将的攻击力提升3倍
    {enAutoSkillTp_armor_all,       0,                  enGeneralTp_Null,       100,    0.75,  enAutoSkill_NULL},               //379   受到的所有伤害减少25%
    {enAutoSkillTp_armor_all,       0,                  enGeneralTp_Null,       100,    0.25,  enAutoSkill_NULL},               //380   满血量时，受到的所有伤害减少75%
    {enAutoSkillTp_valChg_atkrcv,   enElementLight,     enGeneralTp_Null,       100,    1.5,   enAutoSkill_NULL},               //381   光属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementDark,      enGeneralTp_Null,       100,    1.5,   enAutoSkill_NULL},               //382   暗属性武将的攻击力和回复力变为1.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_XiaoYong,   100,    3.5,   enAutoSkill_NULL},               //383   骁勇善战类武将的攻击力提升3+0.5倍；当消除6颗火宝珠或以上时（需连着），提升3.5+0.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_JinShi,     100,    4,     enAutoSkill_NULL},               //384   血量上限减少40%，金石之坚类武将的攻击力提升4倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    4.0f,  enAutoSkill_NULL},               //385   当血量大于或等于80%时，国士无双类武将的攻击力提升3.5+0.5倍；当血量不足50%时，国士无双类武将的回复力提升1.5+0.5倍
    
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    3.5,   enAutoSkill_NULL},               //386   当血量大于或等于50%时，自身攻击力提升3.5倍；若无副将时，自身属性提升3倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       4,      3.5,   enAutoSkill_NULL},               //387   达到4连击或以上时，自身攻击力提升3.5倍；若无副将时，自身属性提升3倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       4,      2,     enAutoSkill_NULL},               //388   达到4连击或以上时，所有武将的攻击力提升2倍
    
    
    
    {enAutoSkillTp_atk,             enElementFire,      enGeneralTp_Null,       100,    3.5,   enAutoSkill_NULL},               //389   当血量大于或等于80%时，所有火属性武将的攻击力提升3.5倍
    {enAutoSkillTp_atk_elm_type4,   enElementMax,       enGeneralTp_Null,       1,      5.0f,  enAutoSkill_NULL},               //390   水，木，光，暗同时攻击时，所有武将的攻击力提升5倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       100,    1.0f,  enAutoSkill_NULL},               //391   达到3连击或以上时，攻击力上升，最多增至10倍 (由3连击开始1.5倍，之后每+1连击再+0.5倍，最大20连击10倍)
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_God,        100,    3.5f,  enAutoSkill_NULL},               //392   当血量大于或等于80%时，所有神将类武将的攻击力提升3.5倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       6,      4,     enAutoSkill_NULL},               //393   达到6连击或以上时，所有武将的攻击力提升4倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,       8,      2,     enAutoSkill_NULL},               //394   火，水，光同时攻击时，所有武将的攻击力提升2倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,       8,      2.5f,  enAutoSkill_NULL},               //395   火，水，光同时攻击时，所有武将的攻击力提升2.5倍
    {enAutoSkillTp_atk_elm_type,    enElementMax,       enGeneralTp_Null,       8,      3,     enAutoSkill_NULL},               //396   火，水，光同时攻击时，所有武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null,       100,    2,     enAutoSkill_NULL},               //397   当HP大于或等于50%时，所有武将的攻击力提升2倍
    {enAutoSkillTp_revive,          0,                  enGeneralTp_Null,       100,    0.7,   enAutoSkill_NULL},               //398   当受到致死攻击时，有几率生还（70%H,同ID27龙之魄）
    {enAutoSkillTp_recover,         enElementWater,     enGeneralTp_Null,       100,    3.0,   enAutoSkill_NULL},               //399   消除宝珠后，血量大回复（3倍自身回复力,同ID32歌莺舞燕）
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_JinShi,     100,    3.5f,  enAutoSkill_NULL},               //400   当血量不足80%时，所有金石之坚类武将的攻击力提升3.5倍
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_JinShi,     100,    2.0,   enAutoSkill_NULL},               //401   金石之坚类武将的HP和攻击力提升2倍
    {enAutoSkillTp_valChg_hprcv,    enElementMax,       enGeneralTp_JinShi,     100,    2.0,   enAutoSkill_NULL},               //402   金石之坚类武将的HP和回复力提升2倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_JinShi,     100,    2.0,   enAutoSkill_NULL},               //403   金石之坚类武将的攻击力和回复力提升2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_JinShi,     100,    2.5,   enAutoSkill_NULL},               //404   满血量时，所有金石之坚类武将的攻击力提升2.5倍
    {enAutoSkillTp_valChg_all,      enElementMax,       enGeneralTp_JinShi,     100,    1.5,   enAutoSkill_NULL},               //405   金石之坚类武将的HP、攻击力和回复力提升1.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    2.5,   enAutoSkill_NULL},               //406   国士无双类武将的攻击力提升2.5倍
    {enAutoSkillTp_burn,            enElementZero,      enGeneralTp_Null,       100,    2.0,   enAutoSkill_NULL},               //407   攻击后，追加一次强力全体攻击（2倍自身攻击力，同ID26狱炎）
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_XiaoYong,   100,    1.5,   enAutoSkill_NULL},               //408   骁勇善战类武将的HP和攻击力提升1.5倍
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_JinShi,     100,    1.5,   enAutoSkill_NULL},               //409   金石之坚类武将的HP和攻击力提升1.5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_GongShou,   100,    1.5,   enAutoSkill_NULL},               //410   攻守兼备类武将的攻击力和回复力提升1.5倍
    {enAutoSkillTp_fanji,           enElementLight,     enGeneralTp_Null,       50,     5.0,   enAutoSkill_NULL},               //411   受到伤害后，中几率反弹中幅光属性伤害（同ID93勇武之袭）
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_ShengSheng, 100,    2.0f,  enAutoSkill_NULL},               //412   生生不息类武将的攻击力提升2倍
    
    {enAutoSkillTp_atk,             enElementFire,      enGeneralTp_Null,       100,    3,     enAutoSkill_NULL},               //413   火属性武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_Null, 100,    3.0,   enAutoSkill_NULL},               //414   蜀国武将的攻击力提升3倍（等国家系统需求出来再做）
    {enAutoSkillTp_atk,             enElementFire,      enGeneralTp_Null,       100,    4.0,   enAutoSkill_NULL},               //415   满血量时，火属性武将的攻击力提升4倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_God,        100,    3.0f,  enAutoSkill_NULL},               //416   神将类武将的攻击力提升3倍
    {enAutoSkillTp_atk,             enElementDark,      enGeneralTp_Null,       100,    3,     enAutoSkill_NULL,},              //417   暗属性武将的攻击力提升3倍;和魏武帝·曹操(+297)齐上阵,提升4倍;达到6连击时提升4.5倍          CAutoSkillAppend().addIfGenIDList("(772|773)")
    {enAutoSkillTp_atk,         enElementLight,   enGeneralTp_Null, 100,    5.0,  enAutoSkill_NULL},                             //418 消除宝珠后,血量超回复（5倍自身回复力,同ID33）;且当血量大于或等于50%时,光属性武将的攻击力提升3.5倍;满血量时,提升4倍
    {enAutoSkillIp_money_exp_drop,            1,             1,               0,    1.5,    enAutoSkill_NULL},                   //419 战斗经验大幅提升；额外获得少量铜钱（都是1.5倍）
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_mojiang,    100,    2.5,  enAutoSkill_NULL},                 //420 狂野的不死鸟	 魔将的攻击力和回复力变为2.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_ShengSheng,  100,   3.5f,  enAutoSkill_NULL},               //421 朔月的冷艳锋芒	生生不息类武将攻击力提高3.5倍，满血时生生不息类武将攻击力提高4倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_GuoShi,   100,    2.5,   enAutoSkill_NULL},                 //422	暴虐的凶兽	国士无双类宠物的攻击力和回复力变为2.5倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_God,          100,    2.5f,  enAutoSkill_NULL},             //423	伤门▪天诛	神类宠物的攻击力变为2.5倍
    {enAutoSkillTp_atk,             enElementLight,     enGeneralTp_Null, 100,    3.0f,  enAutoSkill_NULL},                     //424	烈日的灼热辉耀	光属性武将攻击力提高3倍，和+297诸葛亮或武侯诸葛孔明齐上阵，提升4倍，满血时提高5倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_GongShou,   100,    2.0,   enAutoSkill_NULL},               //425	锦帆之逆袭	攻守兼备类宠物的攻击力和回复力变为2倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GongShou,     100,    3.0f,  enAutoSkill_NULL},             //426	将军之威猛	攻守兼备类宠物的攻击力变为3倍
    {enAutoSkillTp_atk,             enElementMax,       enGeneralTp_GuoShi,     100,    3.0,   enAutoSkill_NULL},               //427	名将之天威	国士无双类宠物的攻击力变为3倍
    {enAutoSkillTp_valChg_atkrcv,   enElementMax,       enGeneralTp_God,   100,    2.0f,  enAutoSkill_NULL},                    //428	神将之天启	神类宠物的攻击力和回复力变为2倍
    {enAutoSkillTp_valChg_atkhp,    enElementMax,       enGeneralTp_GuoShi,     100,    2.0,   enAutoSkill_NULL},               //429	历战之觉醒	国士无双类宠物的HP和攻击力变为2倍
    {enAutoSkillTp_atk_cmb,         enElementMax,       enGeneralTp_Null,       100,    1.0f,  enAutoSkill_NULL},               //430	九五之尊·原罪	连击数达到4以上时攻击力上升，最多增至4倍 (由4连击开始2.5倍，之后每+1连击再+0.5倍，最大7连击4倍)
    {enAutoSkillIp_money_exp_drop,            1,             0,               1,      1.5,    enAutoSkill_NULL},                //431	金牌满营	武将掉率大幅提升，金钱掉率小幅提升
    {enAutoSkillTp_atk,             enElementMax,      enGeneralTp_Null,       100,    2.5,   enAutoSkill_NULL},                //432	苍龙之血	与各形态关羽（武将ID=191、192、332、333、796、995）齐上阵，攻击力提高2.5倍
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},                            //433	预留
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},                            //434	预留
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},                            //435	预留
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},                            //436	预留
    {enAutoSkillTp_null,            0,             0,               0,      0,    enAutoSkill_NULL},                            //437	预留
    {enAutoSkillTp_atk,            enElementMax,             enGeneralTp_GongShou,               100,      4,    enAutoSkill_NULL},                            //438	倾城的姬君	HP上限减少50%，所有攻守兼备类宠物的攻击力变成4倍
    {enAutoSkillTp_atk,            enElementMax,             enGeneralTp_JinShi,               100,      4,    enAutoSkill_NULL},                            //439	尚武的姬君	HP上限减少50%，所有金石之坚类宠物的攻击力变成4倍
    {enAutoSkillTp_armor_attr,            enElementDark,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //440	防御·暗	暗属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr,            enElementDark,             enGeneralTp_Null,               100,      0.45,    enAutoSkill_NULL},                            //441	防御姿态·暗	暗属性敌人造成的伤害减少55%
    {enAutoSkillTp_armor_attr,            enElementFire,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //442	防御·火	火属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr,            enElementFire,             enGeneralTp_Null,               100,      0.45,    enAutoSkill_NULL},                            //443	防御姿态·火	火属性敌人造成的伤害减少55%
    {enAutoSkillTp_armor_attr,            enElementWater,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //444	防御·水	水属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr,            enElementWater,             enGeneralTp_Null,               100,      0.45,    enAutoSkill_NULL},                            //445	防御姿态·水	 水属性敌人造成的伤害减少55%
    {enAutoSkillTp_recover, enElementWater,     enGeneralTp_Null, 100,    2.0,  enAutoSkill_NULL},                                                                   //446	充能	 每回合回复相当于队长宠物自身回复力200%的HP，没有产生消除的话不会触发该效果
    {enAutoSkillTp_armor_attr,            enElementWood,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //447	防御·木	木属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr,            enElementWood,             enGeneralTp_Null,               100,      0.45,    enAutoSkill_NULL},                            //448	防御姿态·木	木属性敌人造成的伤害减少55%
    {enAutoSkillTp_armor_attr,            enElementLight,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //449	防御·光	光属性敌人造成的伤害减少50%
    {enAutoSkillTp_armor_attr,            enElementLight,             enGeneralTp_Null,               100,      0.45,    enAutoSkill_NULL},                            //450	防御姿态·光	光属性敌人造成的伤害减少55%
    {enAutoSkillTp_atk,            enElementMax,             enGeneralTp_GuoShi,               100,      4.00,    enAutoSkill_NULL},                            //451	奋战	当HP不足80%时，所有国士无双类宠物的攻击力变为4倍
    {enAutoSkillTp_valChg_all,            enElementMax,             enGeneralTp_GuoShi,               100,      1.5,    enAutoSkill_NULL},                            //452	自然恩惠	国士无双类宠物的HP、攻击力和回复力变为1.5倍
    {enAutoSkillTp_armor_all,            0,             enGeneralTp_Null,               100,      0.25,    enAutoSkill_NULL},                            //453	大地之力	当HP为满时，受到的所有伤害减少75%
    {enAutoSkillTp_atk,            enElementWater,             enGeneralTp_JinShi,               100,      2.5,    enAutoSkill_NULL},                            //454	水之波动	水属性和体力类宠物的攻击力变为2.5倍
    {enAutoSkillTp_fanji,            enElementFire,             enGeneralTp_Null,               25,      10.0,    enAutoSkill_NULL},                            //455	火之牙	受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的火属性伤害
    {enAutoSkillTp_fanji,            enElementWater,             enGeneralTp_Null,               25,      10.0,    enAutoSkill_NULL},                            //456	水之尾	受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的水属性伤害
    {enAutoSkillTp_fanji,            enElementWood,             enGeneralTp_Null,               25,      10.0,    enAutoSkill_NULL},                            //457	飞叶快刀	受到伤害时，有约25%几率进行反击，造成10倍于受到伤害的木属性伤害
    {enAutoSkillIp_money_exp_drop,            1,             0,               0,      1.5,    enAutoSkill_NULL},                            //458	预知未来	战斗胜利后获得的金钱变为原本的1.5倍
    {enAutoSkillTp_armor_attr,            enElementDark,             enGeneralTp_Null,               100,      0.5,    enAutoSkill_NULL},                            //459	自我再生	暗属性敌人造成的伤害减少50%
};

#pragma mark -
#pragma mark gManSkillName
static const char* gManSkillName[] = {
    "",
    "chiyan",
    "yanliuxing",
    "lieyanfentian",

    "jiliu",
    "shuiyuezhan",
    "shuilongkuangtao",

    "zhendi",
    "liediren",
    "dadikuangxiao",

    "leiji",
    "tianjianzhan",
    "tianjianshenwei",

    "anyue",
    "luocharen",
    "shenguiluanwu",

    "liannu",
    "chuanyunjian",
    "lianzhujian",
    "lienukuangtao",

    "tujihuo",
    "jixihuo",

    "tujishui",
    "jixishui",

    "tujimu",
    "jiximu",

    "tujiguang",
    "jixiguang",

    "tujian",
    "jixian",

    "luochahou",
    "guiwangpaoxiao",

    "cuidu",
    "mengdu",
    "shigujudu",

    "weixia",
    "weiya",

    "pojia",
    "moyanrongjin",

    "zhiliaoshu",
    "zhiyushu",
    "huichunshu",
    "qingnangmishu",
    "guiyuanshu",
    "mingliaoshu",

    "baguazhen",
    "renwangdun",

    "chenzhouhuo",
    "chenzhouan",
    "weiyingshui",
    "weiyingguang",
    "huanrihuo",
    "huanrishui",
    "huanrimu",
    "huanriguang",
    "huanrian",
    "beishui",
    "luanshi",
    "luanshitianxia",
    
    "guaguliaoshang",
    
    "huifushui",
    "huifumu",
    "huifuguang",
    "muhuifu",
    "shuihuifu",
    "guanghuifu",
    
    "feidanshui",
    "feidanhuo",
    "feidanmu",
    "feidanguang",
    "feidanan",
    
    "cannonshui",
    "cannonhuo",
    "cannonmu",
    "cannonguang",
    "cannonan",
    
    "tiankongshui",
    "tiankonghuo",
    "tiankongmu",
    "tiankongguang",
    "tiankongan",
    
    "mabishui",
    "mabihuo",
    "mabimu",
    "mabiguang",
    "mabian",
    "mabiall",
    
    "wudishui",
    "wudihuo",
    "wudimu",
    "wudiguang",
    "wudian",
    "wudiall",
    
    "rumucf",
    "huoshaoly",
    "qiaobian",
    "juedifj",
    "ghostcall",
    
    "godanger",
    
    //宝石变化(火，木-> 心)
    "huomuxin",
    
    //痛恨一击
    "hateatk",
    
    //瘟疫
    "skillsick",
    
    //30倍攻击力
    "lieyanfentian30",
    
    "shuilongkuangtao30",
    
    "dadikuangxiao30",
    
    "tianjianshenwei30",
    
    "shenguiluanwu30",
    
    // 两种宝石变化成两种宝石
    "ManSillTitle_107",
    "ManSillTitle_108",
    "ManSillTitle_109",
    "ManSillTitle_110",
    "ManSillTitle_111",
    
    // 双属性攻击 攻击系
    "ManSillTitle_112",
    "ManSillTitle_113",
    "ManSillTitle_114",
    "ManSillTitle_115",
    "ManSillTitle_116",
    
    // 无敌按伤害数
    "ManSillTitle_117",
    "ManSillTitle_118",
    "ManSillTitle_119",
    "ManSillTitle_120",
    "ManSillTitle_121",
    
    // x属性一回合加强
    "ManSillTitle_122",
    "ManSillTitle_123",
    "ManSillTitle_124",
    "ManSillTitle_125",
    "ManSillTitle_126",
    
    // 1回合内消除所有宝石等同回复宝石
    "ManSillTitle_127",
    
    //对敌方单体造成10000点伤害，且属性必定克制
    "ManSillTitle_128",
    
    //敌将回合+1, 我方回合-1
    "ManSillTitle_129",
    
    //宝石加号效果
    "ManSillTitle_130",
    "ManSillTitle_131",
    "ManSillTitle_132",
    "ManSillTitle_133",
    "ManSillTitle_134",
    
    // 想通技能不同名字
    "ManSkillTitle_135",
    "ManSkillTitle_136",
    "ManSkillTitle_137",
    "ManSkillTitle_138",
    
    // 攻击力40倍
    "ManSkillTitle_139",
    "ManSkillTitle_140",
    "ManSkillTitle_141",
    "ManSkillTitle_142",
    "ManSkillTitle_143",
    
    // 延长敌将2回合
    "ManSkillTitle_144",
    
    // 减伤害75%
    "ManSkillTitle_145",

    // 固定伤害9999
    "ManSkillTitle_146",
    
    // 想通技能不同名字
    "ManSkillTitle_147",
    "ManSkillTitle_148",
    
    // 6秒移珠子
    "ManSkillTitle_149",

    // 单体暗40
    "ManSkillTitle_150",
    
    // 只对火属性打水伤害
    "ManSkillTitle_151",

    // 1~50倍伤害
    "ManSkillTitle_152",

    // ----------3.4---------------
    "ManSkillTitle_153",  // recover3
    
    "ManSkillTitle_154", // aoe3
    
    "ManSkillTitle_155", // 神圣伤害5555
    
    "ManSkillTitle_156", // 反5击3
    
    "ManSkillTitle_157", // 火单30
    
    "ManSkillTitle_158", // 水单30
    
    "ManSkillTitle_159", // 木单30
    
    "ManSkillTitle_160", // 火全250000
    
    "ManSkillTitle_161", // 水全250000
    
    "ManSkillTitle_162", // 木全250000
    
    "ManSkillTitle_163", // 光全250000
    
    "ManSkillTitle_164", // 暗全250000
    
    "ManSkillTitle_165", // 光单30
    
    "ManSkillTitle_166", // 火打木17500
    
    "ManSkillTitle_167", // 水打火17500
    
    "ManSkillTitle_168", // 木打水17500

    "ManSkillTitle_169", // 光打暗15000

    "ManSkillTitle_170", // 暗打光15000

    "ManSkillTitle_171", // 光变火

    "ManSkillTitle_172", // 暗变水

    "ManSkillTitle_173", // 水变木

    "ManSkillTitle_174", // 木变光

    "ManSkillTitle_175", // 火变暗

    "ManSkillTitle_176", // 怪变火

    "ManSkillTitle_177", // 怪变水

    "ManSkillTitle_178", // 怪变木

    "ManSkillTitle_179", // 怪变暗

    "ManSkillTitle_180", // 同176 怪变火

    // ----------4.0---------------
    "ManSkillTitle_181", // Atk20Rcv5%

    "ManSkillTitle_182", // HP1Atk50

    "ManSkillTitle_183", // 单体光Atk40

    "ManSkillTitle_184", // 水、心-》火

    "ManSkillTitle_185", // 木、心-》水

    "ManSkillTitle_186", // 火、心-》木

    "ManSkillTitle_187", // 暗、心-》光

    "ManSkillTitle_188", // 光、心-》暗

    "ManSkillTitle_189", // all-》火

    "ManSkillTitle_190", // 单体火Atk30

    "ManSkillTitle_191", // luanshi7.0

    "ManSkillTitle_192", // 3回合反击光3

    "ManSkillTitle_193", // 木、光-》心

    "ManSkillTitle_194", // 火、暗-》心

    "ManSkillTitle_195", // 水、光-》心

    "ManSkillTitle_196", // 木、暗-》心

    "ManSkillTitle_197", // 火、光-》心

    "ManSkillTitle_198", // 心1回合Atk15

    "ManSkillTitle_199", // 心无敌
    
    "ManSkillTitle_200", // 太古的诅咒
    
    "ManSkillTitle_201", // 对敌方全体造成固定7777无属性伤害
    
    "ManSkillTitle_202", // 光-火 暗-回复
    
    "ManSkillTitle_203", // 光-水 暗-回复
    
    "ManSkillTitle_204", // 光-木 暗-回复
    
    "ManSkillTitle_205", // all-回复
    
    "ManSkillTitle_206", // hp1 300000暗攻击
    
    "ManSkillTitle_207", // 5回合，反击3倍火
    
    "ManSkillTitle_208", // 5回合，反击3倍水
    
    "ManSkillTitle_209", // 5回合，反击3倍木
    
    "ManSkillTitle_210", // 5回合，反击3倍光
    
    "ManSkillTitle_211", // 150000暗伤害，对光敌人
    
    "ManSkillTitle_212", // hp1 暗300倍
    
    "ManSkillTitle_213", // all-木
    
    "ManSkillTitle_214", // 光-木，回复-光
    
    "ManSkillTitle_215", // 35%hp伤害
    
    "ManSkillTitle_216", // 消除暗宝珠全体光伤害 暗宝珠数量＊攻击＊2

    "ManSkillTitle_217", // 消除不是光的宝珠造成全体光伤害  消除的宝珠数量＊攻击＊2
    
    "ManSkillTitle_218", // 1回合 魔将攻击＊3
    
    "ManSkillTitle_219", // 1回合内，骁勇善战类武将的攻击力提升3倍
    
    "ManSkillTitle_220", // 1回合内，金石之坚类武将的攻击力提升3倍
    
    "ManSkillTitle_221", // 1回合内，攻守兼备类武将的攻击力提升3倍
    
    "ManSkillTitle_222", // 1回合内，生生不息类武将的攻击力提升3倍
    
    "ManSkillTitle_223", // 1回合内，国士无双类武将的攻击力提升2倍
    
    "ManSkillTitle_224", // 1回合内，国士无双类武将的攻击力提升2.5倍
    
    "ManSkillTitle_225", // 木、光属性宝珠的攻击力提升
    
    "ManSkillTitle_226", // 火、暗属性宝珠的攻击力提升
    
    "ManSkillTitle_227", // 与当前自己的主将交换位置，再次使用时换回原位
    
    "ManSkillTitle_228", //对敌方单体造成15000点固定伤害
    
    "ManSkillTitle_229", //对敌方单体造成自身攻击力50倍的火属性伤害
    
    "ManSkillTitle_230", //对敌方单体造成自身攻击力20倍的伤害，并回复相当于伤害10%的HP
    
    "ManSkillTitle_231", //5秒内，可以任意挪动宝珠
    
    "ManSkillTitle_232", //所有木宝珠变为水宝珠，所有回复宝珠变为暗宝珠
    
    "ManSkillTitle_233", //1回合内，骁勇善战类武将的攻击力提升1.5倍
    
    "ManSkillTitle_234", //2回合内，所受伤害减半
    
    "ManSkillTitle_235", //对敌方单体造成自身攻击力8倍的伤害，并回复相当于伤害50%的HP
    
    "ManSkillTitle_236", //HP变为1，并对敌方单体造成30万点水属性伤害
    
    "ManSkillTitle_237", //所有宝珠变化成火、木、光宝珠
    
    "ManSkillTitle_238", //所有宝珠变化成水、木、暗宝珠
    
    "ManSkillTitle_239", //所有宝珠变化成木、光、暗宝珠
    
    "ManSkillTitle_240", //敌方全体减少20%的血量
    
    "ManSkillTitle_241", //所有宝珠变化成火、水、暗宝珠
    
    "ManSkillTitle_242", //回复相当于自身回复力6倍的血量
    
    "ManSkillTitle_243", //光、暗属性宝珠的攻击力提升
    
    "ManSkillTitle_244", //对木属性敌人造成75000点火属性伤害

    "ManSkillTitle_245", //对敌方单体造成自身攻击力40倍的光属性伤害
    
    "ManSkillTitle_246", //所有木宝珠变为火宝珠，所有回复宝珠变为木宝珠
    
    "ManSkillTitle_247", //所有火宝珠变为水宝珠，所有回复宝珠变为火宝珠
    
    "ManSkillTitle_248", //所有水宝珠变为木宝珠，所有回复宝珠变为水宝珠
    
    "ManSkillTitle_249", //所有暗宝珠变为光宝珠，所有回复宝珠变为暗宝珠
    
    "ManSkillTitle_250", //所有光宝珠变为暗宝珠，所有回复宝珠变为光宝珠
    
    "ManSkillTitle_251", //水、木属性宝珠的攻击力提升
    
    "ManSkillTitle_252", //2回合内，攻守兼备类武将的攻击力提升1.5倍
    
    "ManSkillTitle_253", //对敌方单体造成300点固定伤害
    
    "ManSkillTitle_254", //与当前自己的主将交换位置，再次使用时换回原位
    
    "ManSkillTitle_255", //敌人攻击延长2回合；一回合内，火属性大强化（连成横行6颗火属性宝珠，火属性攻击力提升30%）
    
    "ManSkillTitle_256", //所有暗宝珠变化成水宝珠；水属性宝珠的攻击力提升
    
    "ManSkillTitle_257", //2回合内，血量完全回复；首回合国士无双类武将变为虚弱状态
    
    "ManSkillTitle_258", //随机使用下列主动技能:（36威压、58乱世天下、94火烧连营、34蚀骨剧毒、42青囊秘术）
    
    "ManSkillTitle_259", //随机使用下列主动技能:（124破军点钢矛、224勇者高阶强化、31鬼王咆哮、132蓄势待发·木、213风卷残云）
    
    "ManSkillTitle_260", //所有宝珠随机变化
    
    "ManSkillTitle_261",  //  闪光冲击   对敌方单体造成自身攻击力30倍的光属性伤害
    
    "ManSkillTitle_262",  //  暗黑冲击   对敌方单体造成自身攻击力30倍的暗属性伤害
    "ManSkillTitle_263",  //  轮回·不灭   血量完全回复;木属性宝珠的攻击力提升
    "ManSkillTitle_264",  //  神凰鼓舞    4回合内，队伍的回复力变为1.5倍
    "ManSkillTitle_265",  //  双龙的决心   所有火宝珠变为水宝珠，所有回复宝珠变为木宝珠
    "ManSkillTitle_266",  //  太后驾到   1回合内，受到的所有伤害减少75%
    "ManSkillTitle_267",  //  神将解禁   1回合内，神将类武将的攻击力提升2倍
    "ManSkillTitle_268",  //  死亡禁锢   延长所有敌人攻击冷却1回合
    "ManSkillTitle_269",  //  方阵      所有暗宝珠变为水宝珠，所有木宝珠变为回复宝珠
    "ManSkillTitle_270",  //  伪·八阵   所有火宝珠变为木宝珠，所有暗宝珠变为回复宝珠
    "ManSkillTitle_271",  //  炎之雷纹   火、光属性宝珠的攻击力提升
    "ManSkillTitle_272",  //  荆州星占   对敌方全体造成随机1~100倍火属性伤害，会受到敌人的属性和防御的影响
    "ManSkillTitle_273",  //  奇袭·夜   对敌方一体造成自身攻击力x50倍的暗属性伤害并回复15%伤害的HP。会受到敌人的属性和防御的影响
    "ManSkillTitle_274",  //  忠孝两难全	1回合内，火属性武将的攻击力提升2倍
    "ManSkillTitle_275",  //  忠孝两难全	1回合内，蜀国武将的攻击力提升2倍（等国家系统需求出来再做）
    "ManSkillTitle_276",  //  大炎戒·炎帝	所有木宝珠变化成火宝珠；10秒内，可以任意挪动宝珠
    "ManSkillTitle_277",  //  跟得上吗？	1回合内，暗属性武将的攻击力提升2倍
    "ManSkillTitle_278",  //  双龙偃月刀	所有宝珠变化成光、暗宝珠
    "ManSkillTitle_279",  //  崩雷圣龙枪	1回合内，光属性武将的攻击力提升2倍;光属性宝珠的攻击力提升
    "ManSkillTitle_280",  //  名字待定	造成10万点火属性和5万点光属性全体伤害
    "ManSkillTitle_281",  //  双极灭星阵·火暗
    "ManSkillTitle_282",  //  光耀·神罚的智者之羽
    "ManSkillTitle_283",  //  双极灭星阵·木暗
    "ManSkillTitle_284",  //  绝机·子午突袭
    "ManSkillTitle_285",  //  闪华·救赎的苍天之剑
    "ManSkillTitle_286",  //  VIP的守护
    "ManSkillTitle_287",  //  武圣之怒
    "ManSkillTitle_288",  //  预留
    "ManSkillTitle_289",  //  预留
    "ManSkillTitle_290",  //  预留
    "ManSkillTitle_291",  //  预留
    "ManSkillTitle_292",  //  明镜止水
    "ManSkillTitle_293",  //  莲华蔽日
    "ManSkillTitle_294",  //  黑眼神
    "ManSkillTitle_295",  //  音速拳
    "ManSkillTitle_296",  //  祈雨
    "ManSkillTitle_297",  //  十万伏特
    "ManSkillTitle_298",  //  光合作用
    "ManSkillTitle_299",  //  自我暗示
    "ManSkillTitle_300",  //  鲨鱼皮
    "ManSkillTitle_301",  //  闪耀种子
    "ManSkillTitle_302",  //  波导弹
    "ManSkillTitle_303",  //  空间破裂
    "ManSkillTitle_304",  //  火焰漩涡
    "ManSkillTitle_305",  //  高压水泵
    "ManSkillTitle_306",  //  种子机枪
    "ManSkillTitle_307",  //  冥想
    "ManSkillTitle_308",  //  神鸟
 
    
};

#pragma mark -
#pragma mark gManSkillDsp
static const char* gManSkillDsp[] = {
    "",

    "chiyanDsp",
    "yanliuxingDsp",
    "lieyanfentianDsp",
    
    "jiliuDsp",
    "shuiyuezhanDsp",
    "shuilongkuangtaoDsp",
    
    "zhendiDsp",
    "liedirenDsp",
    "dadikuangxiaoDsp",
    
    "leijiDsp",
    "tianjianzhanDsp",
    "tianjianshenweiDsp",
    
    "anyueDsp",
    "luocharenDsp",
    "shenguiluanwuDsp",
    
    "liannuDsp",
    "chuanyunjianDsp",
    "lianzhujianDsp",
    "lienukuangtaoDsp",
    
    "tujihuoDsp",
    "jixihuoDsp",
    
    "tujishuiDsp",
    "jixishuiDsp",
    
    "tujimuDsp",
    "jiximuDsp",
    
    "tujiguangDsp",
    "jixiguangDsp",
    
    "tujianDsp",
    "jixianDsp",
    
    "luochahouDsp",
    "guiwangpaoxiaoDsp",
    
    "cuiduDsp",
    "mengduDsp",
    "shigujuduDsp",
    
    "weixiaDsp",
    "weiyaDsp",
    
    "pojiaDsp",
    "moyanrongjinDsp",
    
    "zhiliaoshuDsp",
    "zhiyushuDsp",
    "huichunshuDsp",
    "qingnangmishuDsp",
    "guiyuanshuDsp",
    "mingliaoshuDsp",
    
    "baguazhenDsp",
    "renwangdunDsp",
    
    "chenzhouhuoDsp",
    "chenzhouanDsp",
    "weiyingshuiDsp",
    "weiyingguangDsp",
    "huanrihuoDsp",
    "huanrishuiDsp",
    "huanrimuDsp",
    "huanriguangDsp",
    "huanrianDsp",
    "beishuiDsp",
    "luanshiDsp",
    "luanshitianxiaDsp",
    
    "guaguliaoshangDsp",
    
    "huifushuiDsp",
    "huifumuDsp",
    "huifuguangDsp",
    "muhuifuDsp",
    "shuihuifuDsp",
    "guanghuifuDsp",
    
    "feidanshuiDsp",
    "feidanhuoDsp",
    "feidanmuDsp",
    "feidanguangDsp",
    "feidananDsp",
    
    "cannonshuiDsp",
    "cannonhuoDsp",
    "cannonmuDsp",
    "cannonguangDsp",
    "cannonanDsp",
    
    "tiankongshuiDsp",
    "tiankonghuoDsp",
    "tiankongmuDsp",
    "tiankongguangDsp",
    "tiankonganDsp",
    
    "mabishuiDsp",
    "mabihuoDsp",
    "mabimuDsp",
    "mabiguangDsp",
    "mabianDsp",
    "mabiallDsp",
    
    "wudishuiDsp",
    "wudihuoDsp",
    "wudimuDsp",
    "wudiguangDsp",
    "wudianDsp",
    "wudiallDsp",
    
    "rumucfDsp",
    "huoshaolyDsp",
    "qiaobianDsp",
    "juedifjDsp",
    "ghostcallDsp",
    
    "godangerDsp",
    
    //宝石变化(火，木-> 心)
    "huomuxinDsp",
    
    //痛恨一击
    "hateatkDsp",
    
    //瘟疫
    "skillsickDsp",
    
    //30倍攻击力
    "lieyanfentian30Dsp",
    
    "shuilongkuangtao30Dsp",
    
    "dadikuangxiao30Dsp",
    
    "tianjianshenwei30Dsp",
    
    "shenguiluanwu30Dsp",
    
    // 两种宝石变化成两种宝石
    "ManSillDsp_107",
    "ManSillDsp_108",
    "ManSillDsp_109",
    "ManSillDsp_110",
    "ManSillDsp_111",
    
    // 双属性攻击 攻击系
    "ManSillDsp_112",
    "ManSillDsp_113",
    "ManSillDsp_114",
    "ManSillDsp_115",
    "ManSillDsp_116",

    // 无敌按伤害数
    "ManSillDsp_117",
    "ManSillDsp_118",
    "ManSillDsp_119",
    "ManSillDsp_120",
    "ManSillDsp_121",

    // x属性一回合加强
    "ManSillDsp_122",
    "ManSillDsp_123",
    "ManSillDsp_124",
    "ManSillDsp_125",
    "ManSillDsp_126",
    
    // 1回合内消除所有宝石等同回复宝石
    "ManSillDsp_127",
    
    //对敌方单体造成10000点伤害，且属性必定克制
    "ManSillDsp_128",
    
    //敌将回合+1, 我方回合-1
    "ManSillDsp_129",
    
    //宝石加号效果
    "ManSillDsp_130",
    "ManSillDsp_131",
    "ManSillDsp_132",
    "ManSillDsp_133",
    "ManSillDsp_134",
    
    // 想通技能不同名字
    "ManSkillDsp_135",
    "ManSkillDsp_136",
    "ManSkillDsp_137",
    "ManSkillDsp_138",
    
    // 攻击力40倍
    "ManSkillDsp_139",
    "ManSkillDsp_140",
    "ManSkillDsp_141",
    "ManSkillDsp_142",
    "ManSkillDsp_143",

    // 延长敌将2回合
    "ManSkillDsp_144",
    
    // 减伤害75%
    "ManSkillDsp_145",
    
    // 固定伤害9999
    "ManSkillDsp_146",
    
    // 想通技能不同名字
    "ManSkillDsp_147",
    "ManSkillDsp_148",

    // 6秒移珠子
    "ManSkillDsp_149",
    
    // 单体暗40
    "ManSkillDsp_150",

    // 只对火属性打水伤害
    "ManSkillDsp_151",

    // 1~50倍伤害
    "ManSkillDsp_152",

    // ----------3.4---------------
    "ManSkillDsp_153",  // recover3

    "ManSkillDsp_154", // aoe3
    
    "ManSkillDsp_155", // 神圣伤害5555

    "ManSkillDsp_156", // 反5击3

    "ManSkillDsp_157", // 火单30

    "ManSkillDsp_158", // 水单30

    "ManSkillDsp_159", // 木单30

    "ManSkillDsp_160", // 火全250000

    "ManSkillDsp_161", // 水全250000

    "ManSkillDsp_162", // 木全250000

    "ManSkillDsp_163", // 光全250000

    "ManSkillDsp_164", // 暗全250000

    "ManSkillDsp_165", // 光单30

    "ManSkillDsp_166", // 火打木17500

    "ManSkillDsp_167", // 水打火17500

    "ManSkillDsp_168", // 木打水17500

    "ManSkillDsp_169", // 光打暗15000
    
    "ManSkillDsp_170", // 暗打光15000

    "ManSkillDsp_171", // 光变火
    
    "ManSkillDsp_172", // 暗变水
    
    "ManSkillDsp_173", // 水变木
    
    "ManSkillDsp_174", // 木变光
    
    "ManSkillDsp_175", // 火变暗

    "ManSkillDsp_176", // 怪变火
    
    "ManSkillDsp_177", // 怪变水
    
    "ManSkillDsp_178", // 怪变木
    
    "ManSkillDsp_179", // 怪变暗
    
    "ManSkillDsp_180", // 同176 怪变火
    
    // ----------4.0---------------
    "ManSkillDsp_181", // Atk20Rcv5%
    
    "ManSkillDsp_182", // HP1Atk50
    
    "ManSkillDsp_183", // 单体光Atk40
    
    "ManSkillDsp_184", // 水、心-》火
    
    "ManSkillDsp_185", // 木、心-》水
    
    "ManSkillDsp_186", // 火、心-》木
    
    "ManSkillDsp_187", // 暗、心-》光
    
    "ManSkillDsp_188", // 光、心-》暗
    
    "ManSkillDsp_189", // all-》火
    
    "ManSkillDsp_190", // 单体火Atk30
    
    "ManSkillDsp_191", // luanshi7.0
    
    "ManSkillDsp_192", // 3回合反击光3
    
    "ManSkillDsp_193", // 木、光-》心
    
    "ManSkillDsp_194", // 火、暗-》心
    
    "ManSkillDsp_195", // 水、光-》心
    
    "ManSkillDsp_196", // 木、暗-》心
    
    "ManSkillDsp_197", // 火、光-》心

    "ManSkillDsp_198", // 心1回合Atk15
    
    "ManSkillDsp_199", // 心无敌
    
    "ManSkillDsp_200", // 太古的诅咒
    
    "ManSkillDsp_201", // 7777固定伤害
    
    "ManSkillDsp_202", // 光-火 暗-回复
    
    "ManSkillDsp_203", // 光-水 暗-回复
    
    "ManSkillDsp_204", // 光-木 暗-回复
    
    "ManSkillDsp_205", // all-回复
    
    "ManSkillDsp_206", //hp1 300000暗攻击
    
    "ManSkillDsp_207", //5回合，反击3倍火
    
    "ManSkillDsp_208", //5回合，反击3倍水
    
    "ManSkillDsp_209", //5回合，反击3倍木
    
    "ManSkillDsp_210", //5回合，反击3倍光
    
    "ManSkillDsp_211", //150000暗伤害，对光敌人
    
    "ManSkillDsp_212", //hp1 暗300倍
    
    "ManSkillDsp_213", //all-木
    
    "ManSkillDsp_214", //光-木，回复-光
    
    "ManSkillDsp_215", //35%hp伤害
    
    "ManSkillDsp_216", //消除暗宝珠全体光伤害 暗宝珠数量＊攻击＊2
    
    "ManSkillDsp_217", //消除不是光的宝珠造成全体光伤害  消除的宝珠数量＊攻击＊2
    
    "ManSkillDsp_218", //1回合 魔将类攻击＊3
    
    "ManSkillDsp_219", // 1回合内，骁勇善战类武将的攻击力提升3倍
    
    "ManSkillDsp_220", // 1回合内，金石之坚类武将的攻击力提升3倍
    
    "ManSkillDsp_221", // 1回合内，攻守兼备类武将的攻击力提升3倍
    
    "ManSkillDsp_222", // 1回合内，生生不息类武将的攻击力提升3倍
    
    "ManSkillDsp_223", // 1回合内，国士无双类武将的攻击力提升2倍
    
    "ManSkillDsp_224", // 1回合内，国士无双类武将的攻击力提升2.5倍
    
    "ManSkillDsp_225", // 木、光属性宝珠的攻击力提升
    
    "ManSkillDsp_226", // 火、暗属性宝珠的攻击力提升
    
    "ManSkillDsp_227", // 与当前自己的主将交换位置，再次使用时换回原位
    
    "ManSkillDsp_228", //对敌方单体造成15000点固定伤害
    
    "ManSkillDsp_229", //对敌方单体造成自身攻击力50倍的火属性伤害
    
    "ManSkillDsp_230", //对敌方单体造成自身攻击力20倍的伤害，并回复相当于伤害10%的HP
    
    "ManSkillDsp_231", //5秒内，可以任意挪动宝珠
    
    "ManSkillDsp_232", //所有木宝珠变为水宝珠，所有回复宝珠变为暗宝珠
    
    "ManSkillDsp_233", //1回合内，骁勇善战类武将的攻击力提升1.5倍
    
    "ManSkillDsp_234", //2回合内，所受伤害减半
    
    "ManSkillDsp_235", //对敌方单体造成自身攻击力8倍的伤害，并回复相当于伤害50%的HP
    
    "ManSkillDsp_236", //HP变为1，并对敌方单体造成30万点水属性伤害
    
    "ManSkillDsp_237", //所有宝珠变化成火、木、光宝珠
    
    "ManSkillDsp_238", //所有宝珠变化成水、木、暗宝珠
    
    "ManSkillDsp_239", //所有宝珠变化成木、光、暗宝珠
    
    "ManSkillDsp_240", //敌方全体减少20%的血量
    
    "ManSkillDsp_241", //所有宝珠变化成火、水、暗宝珠
    
    "ManSkillDsp_242", //回复相当于自身回复力6倍的血量
    
    "ManSkillDsp_243", //光、暗属性宝珠的攻击力提升
    
    "ManSkillDsp_244", //对木属性敌人造成75000点火属性伤害

    "ManSkillDsp_245", //对敌方单体造成自身攻击力40倍的光属性伤害
    
    "ManSkillDsp_246", //所有木宝珠变为火宝珠，所有回复宝珠变为木宝珠
    
    "ManSkillDsp_247", //所有火宝珠变为水宝珠，所有回复宝珠变为火宝珠
    
    "ManSkillDsp_248", //所有水宝珠变为木宝珠，所有回复宝珠变为水宝珠
    
    "ManSkillDsp_249", //所有暗宝珠变为光宝珠，所有回复宝珠变为暗宝珠
    
    "ManSkillDsp_250", //所有光宝珠变为暗宝珠，所有回复宝珠变为光宝珠
    
    "ManSkillDsp_251", //水、木属性宝珠的攻击力提升
    
    "ManSkillDsp_252", //2回合内，攻守兼备类武将的攻击力提升1.5倍
    
    "ManSkillDsp_253", //对敌方单体造成300点固定伤害
    
    "ManSkillDsp_254", //与当前自己的主将交换位置，再次使用时换回原位
    
    "ManSkillDsp_255", //敌人攻击延长2回合；一回合内，火属性大强化（连成横行6颗火属性宝珠，火属性攻击力提升30%）
    
    "ManSkillDsp_256", //所有暗宝珠变化成水宝珠；水属性宝珠的攻击力提升
    
    "ManSkillDsp_257", //2回合内，血量完全回复；首回合国士无双类武将变为虚弱状态
    
    "ManSkillDsp_258", //随机使用下列主动技能:（36威压、58乱世天下、94火烧连营、34蚀骨剧毒、42青囊秘术）
    
    "ManSkillDsp_259", //随机使用下列主动技能:（124破军点钢矛、224勇者高阶强化、31鬼王咆哮、132蓄势待发·木、213风卷残云）
    
    "ManSkillDsp_260", //所有宝珠随机变化
    
    "ManSkillDsp_261",  //  闪光冲击   对敌方单体造成自身攻击力30倍的光属性伤害
    
    "ManSkillDsp_262",  //  暗黑冲击   对敌方单体造成自身攻击力30倍的暗属性伤害
    "ManSkillDsp_263",  //  轮回·不灭   血量完全回复;木属性宝珠的攻击力提升
    "ManSkillDsp_264",  //  神凰鼓舞    4回合内，队伍的回复力变为1.5倍
    "ManSkillDsp_265",  //  双龙的决心   所有火宝珠变为水宝珠，所有回复宝珠变为木宝珠
    "ManSkillDsp_266",  //  太后驾到   1回合内，受到的所有伤害减少75%
    "ManSkillDsp_267",  //  神将解禁   1回合内，神将类武将的攻击力提升2倍
    "ManSkillDsp_268",  //  死亡禁锢   延长所有敌人攻击冷却1回合
    "ManSkillDsp_269",  //  方阵      所有暗宝珠变为水宝珠，所有木宝珠变为回复宝珠
    "ManSkillDsp_270",  //  伪·八阵   所有火宝珠变为木宝珠，所有暗宝珠变为回复宝珠
    "ManSkillDsp_271",  //  炎之雷纹   火、光属性宝珠的攻击力提升
    "ManSkillDsp_272",  //  荆州星占   对敌方全体造成随机1~100倍火属性伤害，会受到敌人的属性和防御的影响
    "ManSkillDsp_273",  //  奇袭·夜   对敌方一体造成自身攻击力x50倍的暗属性伤害并回复15%伤害的HP。会受到敌人的属性和防御的影响
    "ManSkillDsp_274",  //  忠孝两难全	1回合内，火属性武将的攻击力提升2倍
    "ManSkillDsp_275",  //  忠孝两难全	1回合内，蜀国武将的攻击力提升2倍（等国家系统需求出来再做）
    "ManSkillDsp_276",  //  大炎戒·炎帝	所有木宝珠变化成火宝珠；10秒内，可以任意挪动宝珠
    "ManSkillDsp_277",  //  跟得上吗？	1回合内，暗属性武将的攻击力提升2倍
    "ManSkillDsp_278",  //  双龙偃月刀	所有宝珠变化成光、暗宝珠
    "ManSkillDsp_279",  //  崩雷圣龙枪	1回合内，光属性武将的攻击力提升2倍;光属性宝珠的攻击力提升
    "ManSkillDsp_280",  //  名字待定	造成10万点火属性和5万点光属性全体伤害
    "ManSkillDsp_281",  //  所有宝珠变化成火、暗宝珠
    "ManSkillDsp_282",  //  光属性伤害大提高（提高40%等于加4个横排）；10秒内任意移动宝珠
    "ManSkillDsp_283",  //  双极灭星阵·木暗  所有宝珠变化成木、暗宝珠
    "ManSkillDsp_284",  //  一回合内，骁勇善战类武将攻击力提高2倍，使敌方全体中剧毒，中毒的敌人每回合损失相当于使用技能宠物自身攻击力10倍的HP，持续到死亡，该伤害无视属性和防御
    "ManSkillDsp_285",  //  当回合内，敌将属性变为暗属性；所有宝珠转化为光心
    "ManSkillDsp_286",  //  二回合内敌方攻击无效
    "ManSkillDsp_287",  //  一回合提升各形态关羽单体伤害2倍（武将ID=191、192、332、333、796、995）。若当前队伍中有多个关羽，则多个关羽伤害均有加成
    "ManSkillDsp_288",  //
    "ManSkillDsp_289",  //
    "ManSkillDsp_290",  //
    "ManSkillDsp_291",  //
    "ManSkillDsp_292",  //  1回合内，受到的所有伤害减少35%
    "ManSkillDsp_293",  //  3回合内，受到的所有伤害减少35%
    "ManSkillDsp_294",  //  1回合内，骁勇善战类宠物的攻击力变为2倍。
    "ManSkillDsp_295",  //  5回合内，攻守兼备类宠物的攻击力变为1.3倍。
    "ManSkillDsp_296",  //  6回合内，受到的所有伤害减少30%
    "ManSkillDsp_297",  //  1回合内，生生不息类宠物的攻击力变为2倍。
    "ManSkillDsp_298",  //  1回合内，攻守兼备类宠物的攻击力变为2倍。
    "ManSkillDsp_299",  //  1回合内，金石之坚类宠物的攻击力变为2倍。
    "ManSkillDsp_300",  //  1回合内，火属性和暗属性宠物的攻击力变为1.5倍
    "ManSkillDsp_301",  //  1回合内，木属性和光属性宠物的攻击力变为1.5倍
    "ManSkillDsp_302",  //  对敌方单体造成自身攻击力4倍的伤害，并回复相当于伤害100%的HP，伤害属性与自身属性相同，会受到敌人的属性和防御的影响
    "ManSkillDsp_303",  //  对敌方单体造成自身攻击力4.5倍的伤害，并回复相当于伤害100%的HP，伤害属性与自身属性相同，会受到敌人的属性和防御的影响
    "ManSkillDsp_304",  //  当前面板上，所有的火属性宝珠获得加号效果，带有加号的宝珠在消除时会造成额外6%伤害。（全屏30个同色加号宝珠时额外伤害为180%，即造成280%伤害）
    "ManSkillDsp_305",  //  当前面板上，所有的水属性宝珠获得加号效果，带有加号的宝珠在消除时会造成额外6%伤害。（全屏30个同色加号宝珠时额外伤害为180%，即造成280%伤害）
    "ManSkillDsp_306",  //  当前面板上，所有的木属性宝珠获得加号效果，带有加号的宝珠在消除时会造成额外6%伤害。（全屏30个同色加号宝珠时额外伤害为180%，即造成280%伤害）
    "ManSkillDsp_307",  //  6回合内，队伍的回复力变为1.2倍
    "ManSkillDsp_308",  //  5回合内，国士无双类宠物的攻击力变为1.2倍。
    
};

#pragma mark -
#pragma mark gAutoSkillName
static const char* gAutoSkillName[] = {
    "",

    //各种力
    "jianjiahuo",
    "jianjiashui",
    "jianjiamu",
    "jianjiaguang",
    "jianjiaan",

    "jingruihuo",
    "jingruishui",
    "jingruimu",
    "jingruiguang",
    "jingruian",

    //各种防
    "jushouhuo",
    "jushoushui",
    "jushoumu",
    "jushouguang",
    "jushouan",

    "jintanghuo",
    "jintangshui",
    "jintangmu",
    "jintangguang",
    "jintangan",

    //全面防
    "panshi",
    "moshou",

    //加血
    "neijing",
    "bencao",

    //追打
    "yeyan",
    "yuyan",

    //
    "longpo",
    "zhenlongpo",

    "yanzhi",
    "shiguang",
    "yongheng",
    
    "geyingwuyan",
    "luangefengwu",
    "wujinyuyan",
    "wuxiekeji",
    
    "zhanshizhinu",
    "mengjiangxuetong",
    "baihualiaoluan",
    "longzhixue",
    "longqishishi",
    "longqishinu",
    "sikaha",
    "businiao",
    "shenlangpaoxiao",
    "waerjili",
    "shendaoyan",
    
    "money15",
    "money20",
    
    "exp12",
    "exp15",
    
    "carddrop12",
    "carddrop15",
    
    "shuihp15",
    "huohp15",
    "muhp15",
    "guanghp15",
    "anhp15",
    "shuihp20",
    "huohp20",
    "muhp20",
    "guanghp20",
    "anhp20",
    
    "jiangnanstyle",
    
    "jiyun",
    
    "huo25",
    "shui25",
    "mu25",
    "guang25",
    "an25",
    
    "wjqxhuo",
    "wjqxshui",
    "wjqxmu",
    "wjqxguang",
    "wjqxan",
    
    "lumanghuo",
    "lumangshui",
    "lumangmu",
    "lumangguang",
    "lumangan",
    
    "yanhuo",
    "hanbing",
    "senluo",
    "hlyanhuo",
    "hhhanbing",
    "senluowx",
    
    "xiaojiang",
    "lsxiaojiang",
    "kuangye",
    "kuangyezx",
    
    "huofanji",
    "shuifanji",
    "mufanji",
    "guangfanji",
    "anfanji",
    
    "teddy",
    "zeus",
    
    //双属性防
    "armorhuomu30",
    "armorhuomu50",
    "armorshuihuo30",
    "armorshuihuo50",
    "armormushui30",
    "armormushui50",
    "armorguanghuo30",
    "armorguanghuo50",
    "armoranmu30",
    "armoranmu50",
    "armorhuoan30",
    "armorhuoan50",
    "armorshuian30",
    "armorshuian50",
    "armormuguang30",
    "armormuguang50",
    "armorguangshui30",
    "armorguangshui50",
    "armoranguang30",
    "armoranguang50",
    "armorxinhuo30",
    "armorxinhuo50",
    "armorxinshui30",
    "armorxinshui50",
    "armorxinmu30",
    "armorxinmu50",
    "armorxinguang30",
    "armorxinguang50",
    "armorxinan30",
    "armorxinan50",
    
    //双属性攻击提升
    "atkxinhuo15",
    "atkxinhuo20",
    "atkxinshui15",
    "atkxinshui20",
    "atkxinmu15",
    "atkxinmu20",
    "atkxinguang15",
    "atkxinguang20",
    "atkxinan15",
    "atkxinan20",
    
    //弱点
    "ruodianhuo",
    "ruodianshui",
    "ruodianmu",
    "ruodianguang",
    "ruodianan",
    
    //神佑
    "godbless",
    
    //三色防
    "3armor50",
    
    //欺压
    "qiya",
    
    //冥王的邀请
    "ghostinvite",
    
    //瘟疫离宫
    "wenyiligong",
    
    //限界突破
    "xianjietupo",
    
    //各种有暴击的力
    "jingruicrithuo",
    "jingruiscrithui",
    "jingruicritmu",
    "jingruicritguang",
    "jingruicritan",
    
    //各种属性武将的攻击力和回复力翻倍
    "AutoSkillTitle_153",
    "AutoSkillTitle_154",

    "AutoSkillTitle_155",
    "AutoSkillTitle_156",
    "AutoSkillTitle_157",
    "AutoSkillTitle_158",
    "AutoSkillTitle_159",
    
    // 各种属性武将的HP翻倍
    "AutoSkillTitle_160",
    "AutoSkillTitle_161",
    "AutoSkillTitle_162",
    "AutoSkillTitle_163",
    "AutoSkillTitle_164",
    
    // 神将攻击2.5
    "AutoSkillTitle_165",
    
    // 满血防
    "AutoSkillTitle_166",
    
    // 追打克制
    "AutoSkillTitle_167",

    // 宙斯升级版，1血4倍攻
    "AutoSkillTitle_168",
    
    // 相同技能不同名字
    "AutoSkillTitle_169",
    "AutoSkillTitle_170",
    "AutoSkillTitle_171",
    "AutoSkillTitle_172",
    "AutoSkillTitle_173",
    "AutoSkillTitle_174",

    // 反击 火 5倍
    "AutoSkillTitle_175",
    
    // hp不足1成 攻击3倍
    "AutoSkillTitle_176",
    
    // 双属性防，两种属性防的效果不同
    "AutoSkillTitle_177",
    "AutoSkillTitle_178",
    "AutoSkillTitle_179",
    "AutoSkillTitle_180",
    "AutoSkillTitle_181",

    // 相同技能不同名字
    "AutoSkillTitle_182",

    //心属性攻击2.5倍
    "AutoSkillTitle_183",
    
    // ------------3.4----------------
    "AutoSkillTitle_184", // 全防20

    "AutoSkillTitle_185", // 7combo攻3.5

    "AutoSkillTitle_186", // 水木血2

    "AutoSkillTitle_187", // 骁勇攻2
    
    "AutoSkillTitle_188", // 4element攻3

    "AutoSkillTitle_189", // 3element攻25

    "AutoSkillTitle_190", // 5combo攻2.5

    "AutoSkillTitle_191", // 5element攻4
    
    "AutoSkillTitle_192", // 10combo攻7
    
    // 同技能不同名字 对应属性武将攻击回复2倍
    "AutoSkillTitle_193",
    "AutoSkillTitle_194",
    "AutoSkillTitle_195",
    "AutoSkillTitle_196",
    "AutoSkillTitle_197",

    "AutoSkillTitle_198", // 火HP&Rcv2

    "AutoSkillTitle_199", // 水HP&Rcv2

    "AutoSkillTitle_200", // 木HP&Rcv2

    "AutoSkillTitle_201", // 3combo攻2

    "AutoSkillTitle_202", // 同追打攻2
    
    "AutoSkillTitle_203", // 全防20

    "AutoSkillTitle_204", // 水火木攻2

    "AutoSkillTitle_205", // 同93 50反击光5

    "AutoSkillTitle_206", // 同20 暗防50

    "AutoSkillTitle_207", // 攻守AtkRcv15

    "AutoSkillTitle_208", // 攻守AtkRcvHP15

    "AutoSkillTitle_209", // 火Atk25Armor20

    "AutoSkillTitle_210", // 水Atk25Armor20

    "AutoSkillTitle_211", // 木Atk25Armor20

    "AutoSkillTitle_212", // 光Atk25Armor20

    "AutoSkillTitle_213", // 暗Atk25Armor20

    "AutoSkillTitle_214", // 火Atk25burn300

    "AutoSkillTitle_215", // 水Atk25Revive70

    "AutoSkillTitle_216", // 木Atk25Armor15

    "AutoSkillTitle_217", // 光Atk25Recover300

    "AutoSkillTitle_218", // 暗Atk25Delay3

    "AutoSkillTitle_219", // 火Atk25HPRcv15

    "AutoSkillTitle_220", // 水Atk25HPRcv15

    "AutoSkillTitle_221", // 木Atk25HPRcv15

    "AutoSkillTitle_222", // 光Atk25Fan25Ji10

    "AutoSkillTitle_223", // 暗Atk25Fan25Ji10

    "AutoSkillTitle_224", // 火Atk25Rcv2

    "AutoSkillTitle_225", // 水Atk25Rcv2

    "AutoSkillTitle_226", // 木Atk25Rcv2

    "AutoSkillTitle_227", // 光Atk25Rcv2

    "AutoSkillTitle_228", // 暗Atk25Rcv2

    "AutoSkillTitle_229", // 攻守Atk1.5

    "AutoSkillTitle_230", // 攻守AtkHp2.0

    "AutoSkillTitle_231", // 生生AtkRcv1.5

    "AutoSkillTitle_232", // 生生All1.5

    "AutoSkillTitle_233", // 骁勇Atk1.5
    
    "AutoSkillTitle_234", // 骁勇AtkHp2.0
    
    "AutoSkillTitle_235", // 金石AtkRcv1.5
    
    "AutoSkillTitle_236", // 金石All1.5

    "AutoSkillTitle_237", // 骁勇AtkRcv1.5
    
    "AutoSkillTitle_238", // 骁勇All1.5

    "AutoSkillTitle_239", // HuoAOEAtk2.5

    "AutoSkillTitle_240", // ShuiAOEAtk2.5

    "AutoSkillTitle_241", // MuAOEAtk2.5

    "AutoSkillTitle_242", // AnAOEAtk2.5

    "AutoSkillTitle_243", // 20HpArmor80
   
    "AutoSkillTitle_244", // 火Atk25Fan25Ji10
    
    "AutoSkillTitle_245", // 水Atk25Fan25Ji10
    
    "AutoSkillTitle_246", // 木Atk25Fan25Ji10

    "AutoSkillTitle_247", // 火&生生Atk25
    
    "AutoSkillTitle_248", // 水&金石Atk25
    
    "AutoSkillTitle_249", // 木&国士Atk25
    
    "AutoSkillTitle_250", // 光&神Atk25
    
    "AutoSkillTitle_251", // 暗&生生Atk25

    "AutoSkillTitle_252", // 延时5.0

    "AutoSkillTitle_253", // 火&金石Atk2

    "AutoSkillTitle_254", // 水&骁勇Atk2

    "AutoSkillTitle_255", // 木&骁勇Atk2

    "AutoSkillTitle_256", // 光&金石Atk2

    "AutoSkillTitle_257", // 暗&骁勇Atk2

    "AutoSkillTitle_258", // 武将掉率1.5

    "AutoSkillTitle_259", // 火HPAtk2

    "AutoSkillTitle_260", // 水HPAtk2

    "AutoSkillTitle_261", // 木HPAtk2

    "AutoSkillTitle_262", // 光RcvHPAtk15

    "AutoSkillTitle_263", // 暗RcvHPAtk15

    "AutoSkillTitle_264", // HP20Atk4

    "AutoSkillTitle_265", // self&暗Atk25

    "AutoSkillTitle_266", // cmb8Atk5

    "AutoSkillTitle_267", // rcvHP2

    "AutoSkillTitle_268", // 体力HP25

    "AutoSkillTitle_269", // 火光暗ElmAtk25

    "AutoSkillTitle_270", // 水木暗ElmAtk25

    "AutoSkillTitle_271", // 水木光ElmAtk25

    "AutoSkillTitle_272", // 火木光ElmAtk25

    "AutoSkillTitle_273", // 火水暗ElmAtk25

    "AutoSkillTitle_274", // 生生HP2

    "AutoSkillTitle_275", // 生生HP25

    "AutoSkillTitle_276", // 心HPAtk2
    
    "AutoSkillTitle_277", // 心HP25
    
    "AutoSkillTitle_278", // 心Atk25
    
    "AutoSkillTitle_279", // 追打God1k
    
    "AutoSkillTitle_280", // 6combo火攻3.5

    "AutoSkillTitle_281", // 6combo水攻3.5

    "AutoSkillTitle_282", // 6combo木攻3.5

    "AutoSkillTitle_283", // 6combo光攻3.5

    "AutoSkillTitle_284", // 6combo暗攻3.5
    
    "AutoSkillTitle_285", // /欺压2
    "AutoSkillTitle_286", // 瘟疫离宫2
    "AutoSkillTitle_287", // 限界突破2
    "AutoSkillTitle_288", // 骁勇善战类武将攻击力x2
    "AutoSkillTitle_289",
    "AutoSkillTitle_290",
    "AutoSkillTitle_291",
    "AutoSkillTitle_292",
    "AutoSkillTitle_293",
    "AutoSkillTitle_294",
    "AutoSkillTitle_295",
    "AutoSkillTitle_296",
    "AutoSkillTitle_297",
    "AutoSkillTitle_298",
    "AutoSkillTitle_299",
    "AutoSkillTitle_300",
    "AutoSkillTitle_301",
    "AutoSkillTitle_302",
    "AutoSkillTitle_303",
    "AutoSkillTitle_304",
    "AutoSkillTitle_305",
    "AutoSkillTitle_306",
    "AutoSkillTitle_307",
    "AutoSkillTitle_308",
    "AutoSkillTitle_309",
    "AutoSkillTitle_310",
    "AutoSkillTitle_311",
    "AutoSkillTitle_312",
    "AutoSkillTitle_313",
    "AutoSkillTitle_314",
    "AutoSkillTitle_315",
    "AutoSkillTitle_316",
    "AutoSkillTitle_317",
    "AutoSkillTitle_318",
    "AutoSkillTitle_319",
    "AutoSkillTitle_320",
    "AutoSkillTitle_321",
    "AutoSkillTitle_322",
    "AutoSkillTitle_323",
    "AutoSkillTitle_324",
    "AutoSkillTitle_325",
    "AutoSkillTitle_326",
    "AutoSkillTitle_327",
    "AutoSkillTitle_328",
    "AutoSkillTitle_329",
    "AutoSkillTitle_330",
    "AutoSkillTitle_331",
    "AutoSkillTitle_332",
    "AutoSkillTitle_333",
    "AutoSkillTitle_334",
    "AutoSkillTitle_335",
    "AutoSkillTitle_336",
    "AutoSkillTitle_337",
    "AutoSkillTitle_338",
    "AutoSkillTitle_339",
    "AutoSkillTitle_340",
    "AutoSkillTitle_341",
    "AutoSkillTitle_342",
    "AutoSkillTitle_343",
    "AutoSkillTitle_344",
    "AutoSkillTitle_345",
    "AutoSkillTitle_346",
    "AutoSkillTitle_347",
    "AutoSkillTitle_348",
    "AutoSkillTitle_349",
    "AutoSkillTitle_350",
    "AutoSkillTitle_351",
    "AutoSkillTitle_352",
    "AutoSkillTitle_353",
    "AutoSkillTitle_354",
    "AutoSkillTitle_355",
    "AutoSkillTitle_356",
    "AutoSkillTitle_357",
    "AutoSkillTitle_358",
    "AutoSkillTitle_359",
    "AutoSkillTitle_360",
    "AutoSkillTitle_361",
    "AutoSkillTitle_362",
    "AutoSkillTitle_363",
    "AutoSkillTitle_364",
    "AutoSkillTitle_365",
    "AutoSkillTitle_366",
    
    "AutoSkillTitle_367",
    "AutoSkillTitle_368",
    "AutoSkillTitle_369",
    "AutoSkillTitle_370",
    "AutoSkillTitle_371",
    "AutoSkillTitle_372",
    "AutoSkillTitle_373",
    "AutoSkillTitle_374",
    "AutoSkillTitle_375",
    "AutoSkillTitle_376",
    "AutoSkillTitle_377",
    "AutoSkillTitle_378",
    "AutoSkillTitle_379",
    "AutoSkillTitle_380",
    "AutoSkillTitle_381",
    "AutoSkillTitle_382",
    "AutoSkillTitle_383",
    "AutoSkillTitle_384",
    "AutoSkillTitle_385",
    "AutoSkillTitle_386",
    "AutoSkillTitle_387",
    "AutoSkillTitle_388",
    
    "AutoSkillTitle_389",
    "AutoSkillTitle_390",
    "AutoSkillTitle_391",
    "AutoSkillTitle_392",
    "AutoSkillTitle_393",
    "AutoSkillTitle_394",
    "AutoSkillTitle_395",
    "AutoSkillTitle_396",
    "AutoSkillTitle_397",
    "AutoSkillTitle_398",
    "AutoSkillTitle_399",
    "AutoSkillTitle_400",
    "AutoSkillTitle_401",
    "AutoSkillTitle_402",
    "AutoSkillTitle_403",
    "AutoSkillTitle_404",
    "AutoSkillTitle_405",
    "AutoSkillTitle_406",
    "AutoSkillTitle_407",
    "AutoSkillTitle_408",
    "AutoSkillTitle_409",
    "AutoSkillTitle_410",
    "AutoSkillTitle_411",
    "AutoSkillTitle_412",
    "AutoSkillTitle_413",
    "AutoSkillTitle_414",
    "AutoSkillTitle_415",
    "AutoSkillTitle_416",
    "AutoSkillTitle_417",
    "AutoSkillTitle_418",
    "AutoSkillTitle_419",
    "AutoSkillTitle_420",
    "AutoSkillTitle_421",
    "AutoSkillTitle_422",
    "AutoSkillTitle_423",
    "AutoSkillTitle_424",
    "AutoSkillTitle_425",
    "AutoSkillTitle_426",
    "AutoSkillTitle_427",
    "AutoSkillTitle_428",
    "AutoSkillTitle_429",
    "AutoSkillTitle_430",
    "AutoSkillTitle_431",
    "AutoSkillTitle_432",
    "AutoSkillTitle_433",
    "AutoSkillTitle_434",
    "AutoSkillTitle_435",
    "AutoSkillTitle_436",
    "AutoSkillTitle_437",
    "AutoSkillTitle_438",
    "AutoSkillTitle_439",
    "AutoSkillTitle_440",
    "AutoSkillTitle_441",
    "AutoSkillTitle_442",
    "AutoSkillTitle_443",
    "AutoSkillTitle_444",
    "AutoSkillTitle_445",
    "AutoSkillTitle_446",
    "AutoSkillTitle_447",
    "AutoSkillTitle_448",
    "AutoSkillTitle_449",
    "AutoSkillTitle_450",
    "AutoSkillTitle_451",
    "AutoSkillTitle_452",
    "AutoSkillTitle_453",
    "AutoSkillTitle_454",
    "AutoSkillTitle_455",
    "AutoSkillTitle_456",
    "AutoSkillTitle_457",
    "AutoSkillTitle_458",
    "AutoSkillTitle_459",
};

#pragma mark -
#pragma mark gAutoSkillDsp
static const char* gAutoSkillDsp[] = {
    "",

    //各种力
    "jianjiahuoDsp",
    "jianjiashuiDsp",
    "jianjiamuDsp",
    "jianjiaguangDsp",
    "jianjiaanDsp",
    
    "jingruihuoDsp",
    "jingruishuiDsp",
    "jingruimuDsp",
    "jingruiguangDsp",
    "jingruianDsp",
    
    //各种防
    "jushouhuoDsp",
    "jushoushuiDsp",
    "jushoumuDsp",
    "jushouguangDsp",
    "jushouanDsp",
    
    "jintanghuoDsp",
    "jintangshuiDsp",
    "jintangmuDsp",
    "jintangguangDsp",
    "jintanganDsp",
    
    //全面防
    "panshiDsp",
    "moshouDsp",
    
    //加血
    "neijingDsp",
    "bencaoDsp",
    
    //追打
    "yeyanDsp",
    "yuyanDsp",
    
    //
    "longpoDsp",
    "zhenlongpoDsp",
    
    "yanzhiDsp",
    "shiguangDsp",
    "yonghengDsp",
    
    "geyingwuyanDsp",
    "luangefengwuDsp",
    "wujinyuyanDsp",
    "wuxiekejiDsp",
    
    "zhanshizhinuDsp",
    "mengjiangxuetongDsp",
    "baihualiaoluanDsp",
    "longzhixueDsp",
    "longqishishiDsp",
    "longqishinuDsp",
    "sikahaDsp",
    "businiaoDsp",
    "shenlangpaoxiaoDsp",
    "waerjiliDsp",
    "shendaoyanDsp",
    
    "money15Dsp",
    "money20Dsp",
    
    "exp12Dsp",
    "exp15Dsp",
    
    "carddrop12Dsp",
    "carddrop15Dsp",
    
    "shuihp15Dsp",
    "huohp15Dsp",
    "muhp15Dsp",
    "guanghp15Dsp",
    "anhp15Dsp",
    "shuihp20Dsp",
    "huohp20Dsp",
    "muhp20Dsp",
    "guanghp20Dsp",
    "anhp20Dsp",
    
    "jiangnanstyleDsp",
    
    "jiyunDsp",
    
    "huo25Dsp",
    "shui25Dsp",
    "mu25Dsp",
    "guang25Dsp",
    "an25Dsp",

    "wjqxhuoDsp",
    "wjqxshuiDsp",
    "wjqxmuDsp",
    "wjqxguangDsp",
    "wjqxanDsp",

    "lumanghuoDsp",
    "lumangshuiDsp",
    "lumangmuDsp",
    "lumangguangDsp",
    "lumanganDsp",
    
    "yanhuoDsp",
    "hanbingDsp",
    "senluoDsp",
    "hlyanhuoDsp",
    "hhhanbingDsp",
    "senluowxDsp",
    
    "xiaojiangDsp",
    "lsxiaojiangDsp",
    "kuangyeDsp",
    "kuangyezxDsp",
    
    "huofanjiDsp",
    "shuifanjiDsp",
    "mufanjiDsp",
    "guangfanjiDsp",
    "anfanjiDsp",
    
    "teddyDsp",
    "zeusDsp",
    
    //双属性防
    "armorhuomu30Dsp",
    "armorhuomu50Dsp",
    "armorshuihuo30Dsp",
    "armorshuihuo50Dsp",
    "armormushui30Dsp",
    "armormushui50Dsp",
    "armorguanghuo30Dsp",
    "armorguanghuo50Dsp",
    "armoranmu30Dsp",
    "armoranmu50Dsp",
    "armorhuoan30Dsp",
    "armorhuoan50Dsp",
    "armorshuian30Dsp",
    "armorshuian50Dsp",
    "armormuguang30Dsp",
    "armormuguang50Dsp",
    "armorguangshui30Dsp",
    "armorguangshui50Dsp",
    "armoranguang30Dsp",
    "armoranguang50Dsp",
    "armorxinhuo30Dsp",
    "armorxinhuo50Dsp",
    "armorxinshui30Dsp",
    "armorxinshui50Dsp",
    "armorxinmu30Dsp",
    "armorxinmu50Dsp",
    "armorxinguang30Dsp",
    "armorxinguang50Dsp",
    "armorxinan30Dsp",
    "armorxinan50Dsp",
    
    //双属性攻击提升
    "atkxinhuo15Dsp",
    "atkxinhuo20Dsp",
    "atkxinshui15Dsp",
    "atkxinshui20Dsp",
    "atkxinmu15Dsp",
    "atkxinmu20Dsp",
    "atkxinguang15Dsp",
    "atkxinguang20Dsp",
    "atkxinan15Dsp",
    "atkxinan20Dsp",
    
    //弱点
    "ruodianhuoDsp",
    "ruodianshuiDsp",
    "ruodianmuDsp",
    "ruodianguangDsp",
    "ruodiananDsp",
    
    //神佑
    "godblessDsp",
    
    //三色防
    "3armor50Dsp",
    
    //欺压
    "qiyaDsp",
    
    //冥王的邀请
    "ghostinviteDsp",
    
    //瘟疫离宫
    "wenyiligongDsp",
    
    //限界突破
    "xianjietupoDsp",
    
    //各种有暴击的力
    "jingruicrithuoDsp",
    "jingruicritshuiDsp",
    "jingruicritmuDsp",
    "jingruicritguangDsp",
    "jingruicritanDsp",

    //各种属性武将的攻击力和回复力翻倍
    "AutoSkillDsp_153",
    "AutoSkillDsp_154",
    
    "AutoSkillDsp_155",
    "AutoSkillDsp_156",
    "AutoSkillDsp_157",
    "AutoSkillDsp_158",
    "AutoSkillDsp_159",
    
    // 各种属性武将的HP翻倍
    "AutoSkillDsp_160",
    "AutoSkillDsp_161",
    "AutoSkillDsp_162",
    "AutoSkillDsp_163",
    "AutoSkillDsp_164",

    // 神将攻击2.5
    "AutoSkillDsp_165",

    // 满血防
    "AutoSkillDsp_166",
    
    // 追打克制
    "AutoSkillDsp_167",
    
    // 宙斯升级版，1血4倍攻
    "AutoSkillDsp_168",
    
    // 相同技能不同名字
    "AutoSkillDsp_169",
    "AutoSkillDsp_170",
    "AutoSkillDsp_171",
    "AutoSkillDsp_172",
    "AutoSkillDsp_173",
    "AutoSkillDsp_174",
    
    // 反击 火 5倍
    "AutoSkillDsp_175",
    
    // hp不足1成 攻击3倍
    "AutoSkillDsp_176",
    
    // 双属性防，两种属性防的效果不同
    "AutoSkillDsp_177",
    "AutoSkillDsp_178",
    "AutoSkillDsp_179",
    "AutoSkillDsp_180",
    "AutoSkillDsp_181",

    // 相同技能不同名字
    "AutoSkillDsp_182",
    
    //心属性攻击2.5倍
    "AutoSkillDsp_183",
    
    // -------------3.4---------------
    "AutoSkillDsp_184", // 全防20

    "AutoSkillDsp_185", // 7combo攻3.5

    "AutoSkillDsp_186", // 水木血2

    "AutoSkillDsp_187", // 骁勇攻2

    "AutoSkillDsp_188", // 4element攻3

    "AutoSkillDsp_189", // 3element攻25

    "AutoSkillDsp_190", // 5combo攻2.5
    
    "AutoSkillDsp_191", // 5element攻4

    "AutoSkillDsp_192", // 10combo攻7
    
    // 同技能不同名字 对应属性武将攻击回复2倍
    "AutoSkillDsp_193",
    "AutoSkillDsp_194",
    "AutoSkillDsp_195",
    "AutoSkillDsp_196",
    "AutoSkillDsp_197",

    "AutoSkillDsp_198", // 火HP&Rcv2

    "AutoSkillDsp_199", // 水HP&Rcv2

    "AutoSkillDsp_200", // 木HP&Rcv2

    "AutoSkillDsp_201", // 3combo攻2

    "AutoSkillDsp_202", // 同追打攻2

    "AutoSkillDsp_203", // 全防20

    "AutoSkillDsp_204", // 水火木攻2

    "AutoSkillDsp_205", // 同93 50反击光5
    
    "AutoSkillDsp_206", // 同20 暗防50

    "AutoSkillDsp_207", // 攻守AtkRcv15

    "AutoSkillDsp_208", // 攻守AtkRcvHP15

    "AutoSkillDsp_209", // 火Atk25Armor40
    
    "AutoSkillDsp_210", // 水Atk25Armor40

    "AutoSkillDsp_211", // 木Atk25Armor40

    "AutoSkillDsp_212", // 光Atk25Armor40

    "AutoSkillDsp_213", // 暗Atk25Armor40

    "AutoSkillDsp_214", // 火Atk25Burn300

    "AutoSkillDsp_215", // 水Atk25Revive70

    "AutoSkillDsp_216", // 木Atk25Armor325

    "AutoSkillDsp_217", // 光Atk25Recover300

    "AutoSkillDsp_218", // 暗Atk25Delay3
    
    "AutoSkillDsp_219", // 火Atk25HPRcv15
    
    "AutoSkillDsp_220", // 水Atk25HPRcv15
    
    "AutoSkillDsp_221", // 木Atk25HPRcv15
    
    "AutoSkillDsp_222", // 光Atk25Fan25Ji10
    
    "AutoSkillDsp_223", // 暗Atk25Fan25Ji10

    "AutoSkillDsp_224", // 火Atk25Rcv2
    
    "AutoSkillDsp_225", // 水Atk25Rcv2
    
    "AutoSkillDsp_226", // 木Atk25Rcv2
    
    "AutoSkillDsp_227", // 光Atk25Rcv2
    
    "AutoSkillDsp_228", // 暗Atk25Rcv2

    "AutoSkillDsp_229", // 攻守Atk1.5
    
    "AutoSkillDsp_230", // 攻守AtkHp2.0

    "AutoSkillDsp_231", // 生生AtkRcv1.5
    
    "AutoSkillDsp_232", // 生生All1.5
    
    "AutoSkillDsp_233", // 骁勇Atk1.5
    
    "AutoSkillDsp_234", // 骁勇AtkHp2.0

    "AutoSkillDsp_235", // 金石AtkRcv1.5
    
    "AutoSkillDsp_236", // 金石All1.5
    
    "AutoSkillDsp_237", // 骁勇AtkRcv1.5
    
    "AutoSkillDsp_238", // 骁勇All1.5
    
    "AutoSkillDsp_239", // 火AOEAtk2.5
    
    "AutoSkillDsp_240", // 水AOEAtk2.5
    
    "AutoSkillDsp_241", // 木AOEAtk2.5
    
    "AutoSkillDsp_242", // 暗AOEAtk2.5
    
    "AutoSkillDsp_243", // 20HpArmor80
    
    "AutoSkillDsp_244", // 火Atk25Fan25Ji10

    "AutoSkillDsp_245", // 水Atk25Fan25Ji10

    "AutoSkillDsp_246", // 木Atk25Fan25Ji10

    "AutoSkillDsp_247", // 火&生生Atk25

    "AutoSkillDsp_248", // 水&金石Atk25

    "AutoSkillDsp_249", // 木&国士Atk25

    "AutoSkillDsp_250", // 光&神Atk25
    
    "AutoSkillDsp_251", // 暗&生生Atk25
    
    "AutoSkillDsp_252", // 延时5.0
    
    "AutoSkillDsp_253", // 火&金石Atk2
    
    "AutoSkillDsp_254", // 水&骁勇Atk2
    
    "AutoSkillDsp_255", // 木&骁勇Atk2
    
    "AutoSkillDsp_256", // 光&金石Atk2
    
    "AutoSkillDsp_257", // 暗&骁勇Atk2
    
    "AutoSkillDsp_258", // 武将掉率1.5
    
    "AutoSkillDsp_259", // 火HPAtk2
    
    "AutoSkillDsp_260", // 水HPAtk2
    
    "AutoSkillDsp_261", // 木HPAtk2
    
    "AutoSkillDsp_262", // 光RcvHPAtk15
    
    "AutoSkillDsp_263", // 暗RcvHPAtk15
    
    "AutoSkillDsp_264", // HP20Atk4
    
    "AutoSkillDsp_265", // self&暗Atk25
    
    "AutoSkillDsp_266", // cmb8Atk5
    
    "AutoSkillDsp_267", // rcvHP2
    
    "AutoSkillDsp_268", // 体力HP25
    
    "AutoSkillDsp_269", // 火光暗ElmAtk25
    
    "AutoSkillDsp_270", // 水木暗ElmAtk25
    
    "AutoSkillDsp_271", // 水木光ElmAtk25
    
    "AutoSkillDsp_272", // 火木光ElmAtk25
    
    "AutoSkillDsp_273", // 火水暗ElmAtk25
    
    "AutoSkillDsp_274", // 生生HP2
    
    "AutoSkillDsp_275", // 生生HP25

    "AutoSkillDsp_276", // 心HPAtk2

    "AutoSkillDsp_277", // 心HP25

    "AutoSkillDsp_278", // 心Atk25

    "AutoSkillDsp_279", // 追打God1k
    
    "AutoSkillDsp_280", // 6combo火攻3.5
    
    "AutoSkillDsp_281", // 6combo水攻3.5
    
    "AutoSkillDsp_282", // 6combo木攻3.5
    
    "AutoSkillDsp_283", // 6combo光攻3.5
    
    "AutoSkillDsp_284", // 6combo暗攻3.5
    
    "AutoSkillDsp_285", // /欺压2
    "AutoSkillDsp_286", // 瘟疫离宫2
    "AutoSkillDsp_287", // 限界突破2
    "AutoSkillDsp_288", // 骁勇善战类武将攻击力x2
    "AutoSkillDsp_289",
    "AutoSkillDsp_290",
    "AutoSkillDsp_291",
    "AutoSkillDsp_292",
    "AutoSkillDsp_293",
    "AutoSkillDsp_294",
    "AutoSkillDsp_295",
    "AutoSkillDsp_296",
    "AutoSkillDsp_297",
    "AutoSkillDsp_298",
    "AutoSkillDsp_299",
    "AutoSkillDsp_300",
    "AutoSkillDsp_301",
    "AutoSkillDsp_302",
    "AutoSkillDsp_303",
    "AutoSkillDsp_304",
    "AutoSkillDsp_305",
    "AutoSkillDsp_306",
    "AutoSkillDsp_307",
    "AutoSkillDsp_308",
    "AutoSkillDsp_309",
    "AutoSkillDsp_310",
    "AutoSkillDsp_311",
    "AutoSkillDsp_312",
    "AutoSkillDsp_313",
    "AutoSkillDsp_314",
    "AutoSkillDsp_315",
    "AutoSkillDsp_316",
    "AutoSkillDsp_317",
    "AutoSkillDsp_318",
    "AutoSkillDsp_319",
    "AutoSkillDsp_320",
    "AutoSkillDsp_321",
    "AutoSkillDsp_322",
    "AutoSkillDsp_323",
    "AutoSkillDsp_324",
    "AutoSkillDsp_325",
    "AutoSkillDsp_326",
    "AutoSkillDsp_327",
    "AutoSkillDsp_328",
    "AutoSkillDsp_329",
    "AutoSkillDsp_330",
    "AutoSkillDsp_331",
    "AutoSkillDsp_332",
    "AutoSkillDsp_333",
    "AutoSkillDsp_334",
    "AutoSkillDsp_335",
    "AutoSkillDsp_336",
    "AutoSkillDsp_337",
    "AutoSkillDsp_338",
    "AutoSkillDsp_339",
    "AutoSkillDsp_340",
    "AutoSkillDsp_341",
    "AutoSkillDsp_342",
    "AutoSkillDsp_343",
    "AutoSkillDsp_344",
    "AutoSkillDsp_345",
    "AutoSkillDsp_346",
    "AutoSkillDsp_347",
    "AutoSkillDsp_348",
    "AutoSkillDsp_349",
    "AutoSkillDsp_350",
    "AutoSkillDsp_351",
    "AutoSkillDsp_352",
    "AutoSkillDsp_353",
    "AutoSkillDsp_354",
    "AutoSkillDsp_355",
    "AutoSkillDsp_356",
    "AutoSkillDsp_357",
    "AutoSkillDsp_358",
    "AutoSkillDsp_359",
    "AutoSkillDsp_360",
    "AutoSkillDsp_361",
    "AutoSkillDsp_362",
    "AutoSkillDsp_363",
    "AutoSkillDsp_364",
    "AutoSkillDsp_365",
    "AutoSkillDsp_366",
    
    "AutoSkillDsp_367",
    "AutoSkillDsp_368",
    "AutoSkillDsp_369",
    "AutoSkillDsp_370",
    "AutoSkillDsp_371",
    "AutoSkillDsp_372",
    "AutoSkillDsp_373",
    "AutoSkillDsp_374",
    "AutoSkillDsp_375",
    "AutoSkillDsp_376",
    "AutoSkillDsp_377",
    "AutoSkillDsp_378",
    "AutoSkillDsp_379",
    "AutoSkillDsp_380",
    "AutoSkillDsp_381",
    "AutoSkillDsp_382",
    "AutoSkillDsp_383",
    "AutoSkillDsp_384",
    "AutoSkillDsp_385",
    "AutoSkillDsp_386",
    "AutoSkillDsp_387",
    "AutoSkillDsp_388",
    
    "AutoSkillDsp_389",
    "AutoSkillDsp_390",
    "AutoSkillDsp_391",
    "AutoSkillDsp_392",
    "AutoSkillDsp_393",
    "AutoSkillDsp_394",
    "AutoSkillDsp_395",
    "AutoSkillDsp_396",
    "AutoSkillDsp_397",
    "AutoSkillDsp_398",
    "AutoSkillDsp_399",
    "AutoSkillDsp_400",
    "AutoSkillDsp_401",
    "AutoSkillDsp_402",
    "AutoSkillDsp_403",
    "AutoSkillDsp_404",
    "AutoSkillDsp_405",
    "AutoSkillDsp_406",
    "AutoSkillDsp_407",
    "AutoSkillDsp_408",
    "AutoSkillDsp_409",
    "AutoSkillDsp_410",
    "AutoSkillDsp_411",
    "AutoSkillDsp_412",
    "AutoSkillDsp_413",
    "AutoSkillDsp_414",
    "AutoSkillDsp_415",
    "AutoSkillDsp_416",
    "AutoSkillDsp_417",
    "AutoSkillDsp_418",
    "AutoSkillDsp_419",
    "AutoSkillDsp_420",
    "AutoSkillDsp_421",
    "AutoSkillDsp_422",
    "AutoSkillDsp_423",
    "AutoSkillDsp_424",
    "AutoSkillDsp_425",
    "AutoSkillDsp_426",
    "AutoSkillDsp_427",
    "AutoSkillDsp_428",
    "AutoSkillDsp_429",
    "AutoSkillDsp_430",
    "AutoSkillDsp_431",
    "AutoSkillDsp_432",
    "AutoSkillDsp_433",
    "AutoSkillDsp_434",
    "AutoSkillDsp_435",
    "AutoSkillDsp_436",
    "AutoSkillDsp_437",
    "AutoSkillDsp_438",
    "AutoSkillDsp_439",
    "AutoSkillDsp_440",
    "AutoSkillDsp_441",
    "AutoSkillDsp_442",
    "AutoSkillDsp_443",
    "AutoSkillDsp_444",
    "AutoSkillDsp_445",
    "AutoSkillDsp_446",
    "AutoSkillDsp_447",
    "AutoSkillDsp_448",
    "AutoSkillDsp_449",
    "AutoSkillDsp_450",
    "AutoSkillDsp_451",
    "AutoSkillDsp_452",
    "AutoSkillDsp_453",
    "AutoSkillDsp_454",
    "AutoSkillDsp_455",
    "AutoSkillDsp_456",
    "AutoSkillDsp_457",
    "AutoSkillDsp_458",
    "AutoSkillDsp_459",

};

#define AUTO_SKILL_APPEND_GEN_ID_LIST               0x00000001          //武将ID列表
#define AUTO_SKILL_APPEND_REPLY_HP                  0x00000002          //每回合回复HP
//#define AUTO_SKILL_APPEND_POISON                    0x00000004          //中毒
//#define AUTO_SKILL_APPEND_FREEZE                    0x00000008          //冻结

CAutoSkillAppend & CAutoSkillAppend::addIfGenIDList(const char *pchCon)
{
    m_nIfEffectType |= AUTO_SKILL_APPEND_GEN_ID_LIST;
    SAutoSkillAppendCell AppendCell;
    std::string szCell;
    int nSize = (int)std::string(pchCon).size();
    char c,c1,c2;
    for (int i = 0; i < nSize; ++i)
    {
        c = pchCon[i];
        if ('('==c) //一项开始
        {
            szCell.clear();
            AppendCell.vrData.clear();
        }
        else if (')'==c)    //一项结束
        {
            if (!szCell.empty())
            {
                AppendCell.vrData.push_back(std::pair<unsigned short, char>(atoi(szCell.c_str()), c));
                szCell.clear();
            }
            
            for (int j = i+1; j < nSize; ++j)
            {
                if ('|'==pchCon[j] ||
                    '&'==pchCon[j] ||
                    '!'==pchCon[j])
                {
                    i = j;
                    c = pchCon[j];
                    break;
                }
                else if (' ' == pchCon[j]){}
                else {break;}
            }
            if (!AppendCell.vrData.empty())
            {
                m_prData.first.push_back(std::pair<SAutoSkillAppendCell,char>(AppendCell, c));
                AppendCell.vrData.clear();
            }
        }
        else if ('|'==c ||
                 '&'==c ||
                 '!'==c)
        {
            int i1 = i+1;
            for (int j = i1; j < nSize; ++j)
            {
                if ('('==pchCon[j])
                {
                    i1 = j;
                    c1 = pchCon[j];
                    break;
                }
                else if (' ' == pchCon[j]){}
                else {break;}
            }
            
            for (int j = i1+1; j < nSize; ++j)
            {
                if ('('==pchCon[j])
                {
                    c2 = pchCon[j];
                    break;
                }
                else if (' ' == pchCon[j]){}
                else {break;}
            }
            if ('('==c1 && '('==c2 && m_prData.first.empty())
            {
                c1 = ' ';
                c2 = ' ';
                m_prData.second = c;
            }
            
            if (!szCell.empty())
            {
                AppendCell.vrData.push_back(std::pair<unsigned short, char>(atoi(szCell.c_str()), c));
                szCell.clear();
            }
        }
        else if ('0'<=c &&
                 '9'>=c)
        {
            szCell += c;
        }
    }
    return *this;
}

bool CAutoSkillAppend::isAppend()const
{
    return 0!=m_nIfEffectType;
}

CAutoSkillAppend & CAutoSkillAppend::addEffectReplyHp(int nReplyHp)
{
    m_nIfEffectType |= AUTO_SKILL_APPEND_REPLY_HP;
    m_nReplyHp = nReplyHp;
    return *this;
}

bool CAutoSkillAppend::isGenIDList(std::vector<unsigned short> &vrID)
{
    int nSize1 = (int)m_prData.first.size();
    for (int i = 0; i < nSize1; ++i)
    {
        bool b1 = false;
        SAutoSkillAppendCell *pAppendCell = &m_prData.first[i].first;
        int nSize2 = (int)pAppendCell->vrData.size();
        for (int j = 0; j < nSize2; ++j)
        {
            bool b2 = false;
            int nSize3 = (int)vrID.size();
            for (int k = 0; k < nSize3; ++k)
            {
                if (pAppendCell->vrData[j].first==vrID[k])
                {
                    b2 = true;
                    break;
                }
            }
            
            b1 = b2;
            char c = pAppendCell->vrData[j].second;
            if ('|'==c)
            {
                if (b2)break;
            }
            else if ('&'==c)
            {
                if (!b2)break;
            }
            else if ('!'==c)
            {
                b1 = !b2;
            }
        }
        
        char c = m_prData.first[i].second;
        if ('|'==c)
        {
            if (b1)
            {
                if ('!'==m_prData.second)
                {
                    return false;
                }
                return true;
            }
        }
        else if ('&'==c)
        {
            if (!b1)
            {
                if ('!'==m_prData.second)
                {
                    return true;
                }
                return false;
            }
        }
        else if ('!'==c)
        {
            b1 = !b1;
        }
        
        if ((i+1) >= nSize1)
        {
            if ('!'==m_prData.second)
            {
                return !b1;
            }
            return b1;
        }
    }
    return true;
}

bool CAutoSkillAppend::isReplyHp()const
{
    return AUTO_SKILL_APPEND_REPLY_HP==(m_nIfEffectType & AUTO_SKILL_APPEND_REPLY_HP);
}

bool CAutoSkillAppend::isMustGenIDList()const
{
    return '&'==m_prData.second;
}

//CAutoSkillAppend & CAutoSkillAppend::addPoison(char chOdds, char chRoundCount/* = 0*/,float fGenRate/* = 0.0f*/,char chElement/* = enElementMax*/, char chGenTp /*= enGeneralTp_Null*/)
//{
//    m_nIfEffectType |= AUTO_SKILL_APPEND_POISON;
//    m_chPoisonOdds = chOdds;
//    m_chPoisonRoundCount = chRoundCount;
//    m_fPoisonGenRate = fGenRate;
//    m_chPoisonElement = chElement;
//    m_chPoisonGenTp = chGenTp;
//    return *this;
//}
//
//bool CAutoSkillAppend::isPoison()const
//{
//    return isType(m_nIfEffectType,AUTO_SKILL_APPEND_POISON);
//}
//
//CAutoSkillAppend & CAutoSkillAppend::addFreeze(char chOdds, char chRoundCount/* = 0*/,float fGenRate/* = 0.0f*/,char chElement/* = enElementMax*/, char chGenTp /*= enGeneralTp_Null*/)
//{
//    m_nIfEffectType |= AUTO_SKILL_APPEND_FREEZE;
//    m_chFreezeOdds = chOdds;
//    m_chFreezeRoundCount = chRoundCount;
//    m_fFreezeGenRate = fGenRate;
//    m_chFreezeElement = chElement;
//    m_chFreezeGenTp = chGenTp;
//    return *this;
//}
//
//bool CAutoSkillAppend::isFreeze()const
//{
//    return isType(m_nIfEffectType,AUTO_SKILL_APPEND_FREEZE);
//}


stManualSkill* CSkillLib::getManSkill(int iID)
{
    if(iID < enManSkill_Max){
        return &gManualSkillInfo[iID];
    }
    else{
        return NULL;
    }
}

const char* CSkillLib::getManSkillName(int iID)
{
    if(iID < enManSkill_Max){
        if (strlen(CGameData::Inst()->getLanguageValue(gManSkillName[iID])) > 0)
            return CGameData::Inst()->getLanguageValue(gManSkillName[iID]);
        else {
            char buf[100];
            snprintf(buf, 99, "ManSkillTitle_%d", iID);
            return CGameData::Inst()->getLanguageValue(buf);
        }
    }
    else{
        return " ";
    }
}


const char* CSkillLib::getManSkillDsp(int iID)
{
    if(iID < enManSkill_Max){
        if (strlen(CGameData::Inst()->getLanguageValue(gManSkillDsp[iID])) > 0)
            return CGameData::Inst()->getLanguageValue(gManSkillDsp[iID]);
        else {
            char buf[100];
            snprintf(buf, 99, "ManSkillDsp_%d", iID);
            return CGameData::Inst()->getLanguageValue(buf);
        }
    }
    else{
        return " ";
    }
}


stAutoSkill* CSkillLib::getAutoSkill(int iID)
{
    if(iID < enAutoSkill_Max){
        return &gAutoSkillInfo[iID];
    }
    else{
        return NULL;
    }
}


const char* CSkillLib::getAutoSkillName(int iID)
{
    if(iID < enAutoSkill_Max){
        if (strlen(CGameData::Inst()->getLanguageValue(gAutoSkillName[iID])) > 0)
            return CGameData::Inst()->getLanguageValue(gAutoSkillName[iID]);
        else {
            char buf[100];
            snprintf(buf, 99, "AutoSkillTitle_%d", iID);
            return CGameData::Inst()->getLanguageValue(buf);
        }
    }
    else{
        return " ";
    }
}


const char* CSkillLib::getAutoSkillDsp(int iID)
{
    if(iID < enAutoSkill_Max){
        if (strlen(CGameData::Inst()->getLanguageValue(gAutoSkillDsp[iID])) > 0)
            return CGameData::Inst()->getLanguageValue(gAutoSkillDsp[iID]);
        else {
            char buf[100];
            snprintf(buf, 99, "AutoSkillDsp_%d", iID);
            return CGameData::Inst()->getLanguageValue(buf);
        }
    }
    else{
        return " ";
    }
}


int CSkillLib::getHateAtkPt(int iCurHp, int iMaxHp)
{
    if(iCurHp == 1)
        return 100000;
    
    if(iCurHp == iMaxHp)
        return 10000;
    
    float x = iCurHp * 1.0 / iMaxHp;
    x = (x > 0.9)? 0.9:x;
    x = (x < 0)?0:x;
    
    int iRet = 10000 / (x + 0.1);
    
    iRet = (iRet < 10000)? 10000:iRet;
    iRet = (iRet > 100000)? 100000:iRet;

    return iRet;
}






