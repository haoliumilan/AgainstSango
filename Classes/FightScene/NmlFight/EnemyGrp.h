//
//  EnemyGrp.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_EnemyGrp_h
#define CardDemo_EnemyGrp_h

#include "cocos2d.h"
#include "Enemy.h"
#include "CommonDef.h"
#include <list>
#include "SaveData.h"
#include "CSkillLib.h"

using namespace cocos2d;

typedef struct
{
    CCSprite*   spBuffIcon;
    bool        bBuffEffect;
}stEnemyGrpBuff;

class CDgStepInfo;

class EnemyGrp : public CCLayer
{
    int             m_iSelected;
    Enemy*          m_enemy[kMaxEnemyCnt];
    
    int             m_iPoisonPt;
    int             m_iFreezePt;
    
    bool            m_bSick;
    int             m_iSickPt;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    CCSprite*       m_spAim;
    
    CCSprite*       m_spBox;
    
    int             m_iGoldBox;
    int             m_iGoldBoxDropIdx;

    CAttackInfo     m_atkInfo;
    
    int             m_iCurEnemy;
    
    int             m_iEnemyCnt;
    int             m_iHitCnt;

    CCPoint         m_ptQueue[kMaxEnemyCnt];
    stEnemyGrpBuff m_EnemyGrpBuff[enEnemyBuff_max];
    
    int             m_iComboDunNum;

public:
    //  从服务器获取战场信息
    EnemyGrp(CDgStepInfo*, CCObject* target, SEL_CallFuncO selector);
    //  从本地备份文件获取战场信息
    EnemyGrp(SaveData_EnemyGrp*, CCObject* target, SEL_CallFuncO selector);
    virtual ~EnemyGrp();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    int beAttacked(CAttackInfo* attackinfo, ccTime dt);
    
    void attack();
    
    const char* getSkillName(int skillID);
    const char* getSkillDetail(int skillID);
    const char* getSkillNameSon(int skillID);
    
    void cbEnemy(CCObject* obj);
    void rmBuffTip(CCObject* obj,int tag);
    void EnemyAttack(ccTime dt);
    void skillStart();
    
    void skillStart2();         //123 124敌将技使用
    
    void AttackAgain(ccTime dt);
    
    void AtkDirectFromEnemy(Enemy* pEnemy ,CAttackInfo atkInfo);
    
    bool getIsEnemyGrpBuffByIdx(int idx);
    bool beDefenFromBuf(CAttackInfo atkinfo, int iEnemyIdx);
    float geRatioFromBuf(CAttackInfo atkinfo, int iEnemyIdx);
    
    bool isAllEnemyDead();
    
    virtual void onExit();
    
    void openBox();
    
    CCPoint* queryAtkPos(CAttackInfo*);
    
    bool isSbDelayed();
    bool isArmorBroken();
    bool isPoison();
    bool isMabi();
    bool isSick();
    bool isChangeElement();
    
    int  getPoisonPt();
    void setPoisonPt(int nPoisonPt);
    int  getSickPt();
    void setFreezePt(int nFreezePt);
    
    
    bool isBuffEffect(enEnemyBuff buffType);
    
    bool haveGoldBox();
    
    int collectCard();
    int collectGold();
    int collectExp();
    bool isActionRunning();
    bool hasDoctor(int curEnemyIdx);
    
    bool haveEnemyWithElemInstead(int iElem);
    bool haveEnemyWithElem(int iElem);
    bool haveEnemyWithNotElem(int iElem);
    int getEnemyCurHp(int idx);
    int getEnemyCurRound(int idx);
    int getEnemyCount();
    int getEnemyCurState(int idx);
    
    Enemy* getEnemyByIdx(int idx);
    void showSkillTip(Enemy* enemy, int idx);
    
    void showAtkTip(const char* tip);
    void ChangeShuxinghudun();
    void ChangeShuxinghudun2();
    
    const CCPoint& useCanFei();  // 返回释放残废buff的怪的位置
    int getComboDunNum();
    void setComboDunNum(int num);
    
    //得到存活的数量
    int getAliveCount();
    
    const char* getBufName(int buffType);
    
    void ResetEnemyLianFanState();//新增
    void chageStoneAndAtk(CAttackInfo* pAtkInfo);
    
    void changeEnemy_huihe(int num);
    
    void recoverEnemyBlood(Enemy* pEnemy);
    void updateEnemyBuff();
private:
    void shoot(const CCPoint& src, const CCPoint& dst, float dt);
    
    int  lookupElemOpposite(CAttackInfo*  attackinfo);
    int  lookupElemOpposite(CAttackInfo*  attackinfo,int nNotIndex);
    int  lookupElemOppositeBig(CAttackInfo*  attackinfo,int nBigIndex);
    int  lookupWeakest();
    
    int attackEnemy(int iEnemyIdx, CAttackInfo* attackinfo, ccTime dt);
    
    void dropbox(const CCPoint& pt, float dt);
    void dropbox(int iEnemyIdx);
    
    bool chkBuff(CAttackInfo*  attackinfo);
    
    void initEnemyPos(CDgStepInfo* pStepInfo);
    void initEnemyPos(SaveData_EnemyGrp* pStepInfo);
    
    void chkEnemyState();
    
    void chkAim();
    
    void CheckFirstAtk(ccTime dt);
    
    //void updateEnemyBuff();
    
    void showBuffTip(enEnemyBuff buffType, int index);
    void showBuffAni(enEnemyBuff buffType, int index);
    void showBuffIcon(enEnemyBuff buffType, int index);
    const char* getBufIcon(int buffType);
    
};

#endif
