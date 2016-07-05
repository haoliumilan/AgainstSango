//
//  EnemySkill.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-15.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__EnemySkill__
#define __AgainstWar__EnemySkill__



#define LIANHUAN_HITCNT 2
#define LIANDA_HITCNT 2
#define NANMANRUQIN_HITCNT 7
#define WUSHUANG_HITCNT 5
#define SHASHOUJIAN_HITCNT 10
#define LUANSHITIANXIA_HITCNT 10
#define LIANJI5_HITCNT 5
#define LIANJI82_HITCNT 2
#define LIANJI83_HITCNT 6
#define LIANJI73_HITCNT 3
#define LIANJI454_HITCNT 4
#define LIANJI753_HITCNT 3
#define LIANJI119_HITCNT 7
#define LIANJI125_HITCNT 6
#define LIANJI132_HITCNT 5
#define LIANJI141_HITCNT 7
#define LIANJI143_HITCNT 5
#define LIANJI164_HITCNT 4
#define LIANJI166_HITCNT 3
#define LIANJI194_HITCNT 3
#define LIANJI208_HITCNT 6
#define LIANJI210_HITCNT 10
#define LIANJI215_HITCNT 3
#define LIANJI218_HITCNT 2
#define LIANJI220_HITCNT 2
#define LIANJI222_HITCNT 2
#define LIANJI228_HITCNT 3
#define LIANJI239_HIYCNT 7
#define LIANJI246_HIYCNT 2
#define LIANJI248_HIYCNT 2
#define LIANJI267_HIYCNT 4
#define LIANJI268_HIYCNT 2
#define LIANJI269_HIYCNT 2
#define LIANJI275_HIYCNT 3
#define LIANJI276_HIYCNT 2
#define LIANJI285_HIYCNT 2
#define LIANJI292_HIYCNT 2
#define LIANJI345_HIYCNT 7
#define LIANJI350_HIYCNT 2
#define LIANJI351_HIYCNT 10
#define LIANJI307_HIYCNT 5
#define LIANJI309_HIYCNT 2
#define LIANJI312_HIYCNT 7
#define LIANJI329_HIYCNT 12
#define LIANJI354_HIYCNT 2
#define LIANJI364_HIYCNT 3
#define LIANJI370_HIYCNT 2
#define LIANJI380_HIYCNT 2
#define LIANJI382_HIYCNT 4
#define LIANJI388_HIYCNT 2
#define LIANJI389_HIYCNT 4
#define LIANJI392_HIYCNT 2
#define LIANJI399_HIYCNT 3
#define LIANJI403_HIYCNT 2
#define LIANJI407_HIYCNT 2
#define LIANJI418_HIYCNT 4
#define LIANJI423_HIYCNT 4
#define LIANJI429_HIYCNT 7
#define LIANJI431_HIYCNT 3
#define LIANJI433_HIYCNT 4
#define LIANJI448_HIYCNT 2
#define LIANJI451_HIYCNT 2
#define LIANJI454_HIYCNT 2
#define LIANJI466_HIYCNT 8
#define LIANJI467_HIYCNT 2
#define LIANJI474_HIYCNT 2
#define LIANJI481_HIYCNT 2
#define LIANJI483_HIYCNT 2
#define LIANJI485_HIYCNT 4
#define LIANJI489_HIYCNT 5
#define LIANJI491_HIYCNT 5
#define LIANJI495_HIYCNT 4
#define LIANJI497_HIYCNT 4
#define LIANJI499_HIYCNT 3
#define LIANJI513_HIYCNT 5
#define LIANJI530_HIYCNT 3


#include "cocos2d.h"

using namespace cocos2d;

class Enemy;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class EnemySkill : public CCNode
{
protected:
    Enemy*  m_pEnemy;
    int     m_nOccurOdds;
    int     m_iSkId;
    
    //技能发动需要消耗的时间(秒)
    float   m_fSkillDur;
    short   m_occurTimes = 999;
    short   m_runCounts = 0;                   //记录行动次数(包括:成功,不成功)
    
    unsigned int        m_unDialogueSum;         //子说话总数
    unsigned int        m_unCurDialogue;         //当前子说话
    
    
    bool    m_ChangeStone;//是否是转换珠子的技能
    
public:
    EnemySkill():m_runCounts(0),m_unDialogueSum(0),m_unCurDialogue(0),m_ChangeStone(false){}
    
    int getSkillID(){return m_iSkId;};
    
    bool getChangeStone(){return m_ChangeStone;};//是否是转换珠子的技能
    
    //发动技能
    virtual void action() = 0;
    
    //技能触发几率
    virtual int getOccurOdds(){return m_nOccurOdds;}
    
    //记录行动次
    void recordRunCount(){m_runCounts++;}
    //得到行动次
    unsigned int getRunCount()const {return m_runCounts;}
    unsigned int getDialogueSum()const {return m_unDialogueSum;}
    unsigned int getCurDialogue()const {
        //m_unCurDialogue = (1+m_runCounts)%(m_unDialogueSum+1);
        return (m_runCounts-1)%(m_unDialogueSum+1);
    }
    
    
    //技能剩余释放次数
    virtual int get_occurTimes(){return m_occurTimes;}
    virtual void set_occurTimes(short occurtimes){m_occurTimes = occurtimes;};
    virtual short get_runCounts(){return m_runCounts;}
    virtual void set_runCounts(short runCounts){m_runCounts = runCounts;};
    
    virtual bool canExecute(){return SaveData::Inst()->getrand() < getOccurOdds();}
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//同归于尽
class EsTongGuiYuJin : public EnemySkill
{
    float   m_BaseAtkPt;
    int     m_isAtkNum = 1;
    
public:
    EsTongGuiYuJin(int iSkId, Enemy* pEnemy);
    
    virtual bool canExecute();
    virtual void action();
    
    void shootAtk(CCObject* pSender);
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//连环
class EsLianHuan : public EnemySkill
{
    int     m_BaseAtkPt;
    
    float   m_fAtkFactor;
    int     m_iHitCnt;
    
public:
    EsLianHuan(int iSkId, Enemy* pEnemy);
    
    virtual void action();
    virtual int getOccurOdds();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//连打
class EsLianDa : public EnemySkill
{
    int     m_BaseAtkPt;
    float   m_fAtkFactor;
    int     m_iHitCnt;
    int     m_iMaxHitCnt;
    
public:
    EsLianDa(int iSkId, Enemy* pEnemy);
    
    virtual void action();
    virtual int getOccurOdds();
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//无双
class EsWuShuang : public EnemySkill
{
    int     m_BaseAtkPt;
    float   m_fAtkFactor;
    int     m_iHitCnt;
    
public:
    EsWuShuang(int iSkId, Enemy* pEnemy);
    
    virtual void action();
    virtual int getOccurOdds();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//重击
class EsZhongJi : public EnemySkill
{
    float   m_fAtkFactor;
    
public:
    EsZhongJi(int iSkId, Enemy* pEnemy);
    
    void action();
    bool canExecute();
};

//比例减MC血
class EsScaleAtk : public EnemySkill
{
public:
    EsScaleAtk(int iSkId, Enemy* pEnemy);
    
    void action();
    bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//强袭
class EsQiangXi : public EnemySkill
{
    float   m_fAtkFactor;
    
public:
    EsQiangXi(int iSkId, Enemy* pEnemy);
    
    void action();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//碎骨
class EsSuiGu : public EnemySkill
{
    float   m_fAtkFactor;
    
public:
    EsSuiGu(int iSkId, Enemy* pEnemy);
    
    void action();
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//封印
class EsFengYin : public EnemySkill
{
    float   m_fAtkFactor;
    
public:
    EsFengYin(int iSkId, Enemy* pEnemy);
    
    void action();

    int get_occurTimes(){return m_occurTimes;};
    void set_occurTimes(short occurtimes);
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//治愈
class EsZhiYu : public EnemySkill
{
public:
    EsZhiYu(int iSkId, Enemy* pEnemy);
    
    void action();
    
    int get_occurTimes(){return m_occurTimes;};
    void set_occurTimes(short occurtimes);
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//复活
class EsFuHuo : public EnemySkill
{
    
public:
    EsFuHuo(int iSkId, Enemy* pEnemy);
    
    void action();
    
    int get_occurTimes(){return m_occurTimes;};
    void set_occurTimes(short occurtimes);
    
    virtual bool canExecute();
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//逆转
class EsNiZhuan : public EnemySkill
{
    float   m_fAtkFactor;
    int     m_lastCD;               //1或2回合
public:
    EsNiZhuan(int iSkId, Enemy* pEnemy);
    
    void action();
    virtual bool canExecute();
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//护甲
class EsHuJia : public EnemySkill
{
    
public:
    EsHuJia(int iSkId, Enemy* pEnemy);
    
    void action();
    
    virtual bool canExecute();
};

class EsBuffOnly : public EnemySkill
{
    
public:
    EsBuffOnly(int iSkId, Enemy* pEnemy);
    void action();
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//蓄势待发
class EsXuShiDaiFa : public EnemySkill
{
    
public:
    EsXuShiDaiFa(int iSkId, Enemy* pEnemy);
    
    void action();
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//杀手锏
class EsShaShowJian : public EnemySkill
{
    float   m_fAtkFactor;
    int     m_iHitCnt;
    int     m_BaseAtkPt;
    
public:
    EsShaShowJian(int iSkId, Enemy* pEnemy);
    
    void action();
    void attack();
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//激怒狂暴
class EsJiNuKuangBao : public EnemySkill
{
    
public:
    EsJiNuKuangBao(int iSkId, Enemy* pEnemy);
    
    void action();
    
    virtual bool canExecute();
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//对话
class EsDuiHua : public EnemySkill
{
    
public:
    EsDuiHua(int iSkId, Enemy* pEnemy);
    
    void action();
    
    virtual bool canExecute();
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class EnemySkillPool {
    static EnemySkillPool* s_pcIf;
    
public:
	//Get the instance pointer
	static EnemySkillPool* Inst(){
        if(s_pcIf == NULL){
            s_pcIf = new EnemySkillPool();
        }
        
        return s_pcIf;
    }
    
    EnemySkill* createEnemySkill(int skid, Enemy* pEnemy);

private:
    //forbid constructor being called outside
	EnemySkillPool(){};
    
	//forbid copy constructor being called outside
	EnemySkillPool(const EnemySkillPool& );
	
	//forbid assignment
	EnemySkillPool& operator=(const EnemySkillPool& );
    
};

#endif /* defined(__AgainstWar__EnemySkill__) */
