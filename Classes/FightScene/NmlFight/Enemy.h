//
//  Enemy.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_Enemy_h
#define CardDemo_Enemy_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "CMonsterBlood.h"
#include "SaveData.h"
#include "EnemySkill.h"

#include "EnemyGrp.h"

using namespace cocos2d;

class CMonsterBlood;
class CDgMonsterInfo;

enum EN_ENEMY_STATE
{
    EN_ENEMY_STATE_NORMAL = 0,
    EN_ENEMY_STATE_DEAD,
    EN_ENEMY_STATE_NEAR_DIE,
    
};

enum EN_ENEMY_BUFFID
{
    EN_ENEMY_NONE = 0,
    EN_ENEMY_POJIA,
    EN_ENEMY_MABI,
    EN_ENEMY_DELAY,
    EN_ENEMY_CHGELM,
    EN_ENEMY_SICK,      // 5
    EN_ENEMY_POISON,
    EN_ENEMY_HUJIA,
    EN_ENEMY_XUSHIDAIFA,
    EN_ENEMY_JINU,
    EN_ENEMY_KUANGBAO,  // 10
    EN_ENEMY_GONGJIZITAI,
    EN_ENEMY_BAONU,
    EN_ENEMY_SHASHOUJIAN,
    EN_ENEMY_HAOYIN,
    EN_ENEMY_HAOYIN2,   // 15
    EN_ENEMY_MUTENGJIA,
    EN_ENEMY_JINGANGHUTI,
    EN_ENEMY_JINU2,
    EN_ENEMY_BAOZOU,
    EN_ENEMY_XINGYUNYIJI,   // 20
    EN_ENEMY_SHIJIANNINGJIE,
    EN_ENEMY_DEFEN_FIRE,
    EN_ENEMY_DEFEN_WATER,
    EN_ENEMY_DEFEN_WOOD,
    EN_ENEMY_DEFEN_LIGHT,   // 25
    EN_ENEMY_DEFEN_AN,
    EN_ENEMY_JINU3,
    EN_ENEMY_FREEZE,
    EN_ENEMY_BUFF118,
    EN_ENEMY_BUFF122,       // 30
    EN_ENEMY_BUFF133,
    EN_ENEMY_BUFF137,
    EN_ENEMY_BUFF142,
    EN_ENEMY_BUFF145,
    EN_ENEMY_BUFF146,       // 35
    EN_ENEMY_BUFF149,
    EN_ENEMY_BUFF157,
    EN_ENEMY_BUFF159,
    EN_ENEMY_BUFF202,
    EN_ENEMY_BUFF232,       // 40
    EN_ENEMY_BUFF247,
    EN_ENEMY_BUFF249,
    EN_ENEMY_BUFF252,
    EN_ENEMY_XUSHIDAIFA238,
    EN_ENEMY_XUSHIDAIFA250, // 45
    EN_ENEMY_BUFF287,//lvgenquan
    EN_ENEMY_XUSHIDAIFA294,//lvgenquan
    EN_ENEMY_500THOUNSAND_LIMIT,
    EN_ENEMY_BUFF303,
    EN_ENEMY_BUFF306,       // 50
    EN_ENEMY_XUSHIDAIFA314,
    EN_ENEMY_XUSHIDAIFA316,
    EN_ENEMY_BUFF447,
    EN_ENEMY_XUSHIDAIFA508,
    EN_ENEMY_BUFFMAX,       // 54
    
    
};

typedef struct
{
    CCSprite*   spBuff;
    CEncrypData<short>       cd;
    short       buffID;
    bool        showCd;
}stBuffItem;

class Enemy : public CCLayer
{
    int     m_index;    // 每一回合中怪的序号位置
    short   m_scid;
    CEncrypData<int>    m_iBaseHP;      //基础HP
    float   m_iAtkRatio;
    CEncrypData<int>      m_iAttackPt;    //基础atk
    CEncrypData<int>     m_iRoundMax;
    CEncrypData<int>      m_iArmor;
    CEncrypData<int>      m_iArmorMax;
    int     m_iElement;
    int     m_iSecondElement;
    
    int     m_iState;
    
    int     m_iDefaultElm;   // 默认敌将属性
    
    int     m_iBuffIdArr[ENEMYBUFFMAX];
    
    CEncrypData<int>     m_iHP;              //当前
    
    
    CEncrypData<int>     m_iCurRound;
    
    int     m_iExp;
    int     m_iDrop_Gold;
    int     m_iDropStar;
    bool    m_bDrop_plus;
    
    bool    m_bWudi;
    
    int     m_iDoSpecilAtk;
    int     m_iCurAtkType;
    int     m_iActTimes;
    
    bool    m_isLianFan2;  //新增 用来判断连返2的buf使敌将攻击力最多翻一倍
    int     m_iEnemyBuf_78;
    int     m_iEnemyBuf_80;
    
    
    CCSprite*       m_spRound;
    CCLabelAtlas*   m_lbRound;

    CCSprite*       m_spEnemy;
    CCSprite*       m_spBullet;
    
    CCLabelAtlas*   m_lbPoison;
    CCLabelAtlas*   m_lbSick;
    
    stBuffItem      m_buffArr[MAX_BUFF_ITEMS];
        
    CMonsterBlood*  m_blood;
    CMonsterBlood   *m_pSecondBlood;
    
    CCSprite*       m_spDropItem;

    CCSprite*       m_spDropMoney;
    

    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CCPoint         m_pos;
    
    int            m_iSkillUseLastID;
    CCMutableArray<EnemySkill*> *m_skillArr;
    int            m_actSkillID;
    CAttackInfo    m_atkInfo;
    
    void           buffSelfInit();
    
public:
    void                buffCheckAfterGrp();
    void                elementChange();
    void                elementChange2();          //随机切换属性
    int                 getEnemyActTimes(){return m_iActTimes;};
    int                 getEnemyIndex(){return m_index;};
    int                 getCurAtkType(){return m_iCurAtkType;};
    
    CCMutableArray<EnemySkill*>* getSkill(){return m_skillArr;};
    void skillStart();
    CAttackInfo getAtkInfo(){return m_atkInfo;};
    const CAttackInfo* getAtkInfoEx()const{return &m_atkInfo;};
    void setAtkInfo(CAttackInfo atkinfo){m_atkInfo = atkinfo;};
 
    
    Enemy(CDgMonsterInfo*, CCPoint&, CCObject* target, SEL_CallFuncO selector, int index);
    Enemy(SaveData_Enemy*, CCPoint&, CCObject* target, SEL_CallFuncO selector, int index);
    virtual ~Enemy();
    
    int getSkillUseLastID();
    void AddBuffs();
    
    void init(CCPoint&);
    
    const CCSize& getEnemySize();
    
    virtual void onExit();
    
    bool isAlive();
    
    void beAttacked(CAttackInfo* attackinfo, float dt);
    
    void clearBuffWithEnemyRelive();
    void reLive();
    void realDie();
    void checkDie(float dt);
    bool hasDoctorNotMe();
    void chkActionFromDie();
    
    void setEnemyState(int state);
    
    void chkActBeAttack();
    void chkRoundSkill();
    
    const CCPoint& getEnemyPos();
    
    int  attack();
    
    int  getCanExecuteSkillIdx();
    void startAttack();
    void cbStartAttack(ccTime time);
    void sentAtkInfo();
    
    void AttackAgain();

    int  getElement();
    int  getWidth();
    int  getHeight();
    int  getArmor();
    int  getCurHp();
    int  getCurState();
    int  getBaseHp();
    int  getDropGold();
    int  getExp();
    int  getCurRound();
    int  getAtkPt();
    int  getBuffId(int idx);
    bool isDoctor();
    int getCurFirstHP();
    int getCurSecondHP();
    int getBaseFirstHP();
    int getBaseSecondHP();
    int changeCurHPAction(int nHP,float fTime,enElementDef emElement = enElementInvalid);
    void fadeoutHP(float fTime);
    void fadeinHP(float fTime);
    
    void showDefenFromBuff(int idx);
    
    void setWudi(bool bWudi);
    void sickHpDown(int sickPt);
    void setPoison();
    void bePoisoned(int poisonPt);
    void beFreeze(int FreezePt);
    void changeElement(int element, int iRound, int idx);
    void breakArmor(int iArmor, int iRound, int idx);
    bool isWudi();
    bool isBuffByIdx(int idx);
    bool isDecEachRound(int idx);
    void setBuffByIdx(int iName, int cd, int idx);
    int  collectDropItem();
    void moneyDrop(int dt);
    
    bool isActionRunning();
    
    void clearBuffByIdx(int idx);
    
    //存活的是否只有自己
    bool isAliveOnlyMe();
    
    void saveEnemyBuff();
    
    bool b_UseBuffLianfan2();//新增
    void setLianFan(bool st);//新增
    int     m_currentAtk; //新增
   
   
    int getcurrentRoundAtk();
    void setcurrentRoundAtk(int cAtx);
    
    void changeEnemyCount(int num);
    void setCurRound(int round){m_iCurRound = round;}
    CCLabelAtlas* getLbRound(){return m_lbRound;}
    void enemyBuff_78();
    void enemyBuff_80();
    void enemyBuff_83();
private:
    void runAnim(int iElem, float dt);

    void updateBuffPos();
    void decBuffCD(bool isEachRound);
    void clearBuffWithEnemyDie(float dt);
    void removeObj();
    bool CheckGrpBuff(int idx);
    void ChkGrpBuffEffect(CAttackInfo* attackinfo);
    bool bUseBuffLianfan();
    int getIndexByBufID(int bufId);
    void checkOtherJinGangHuti();
    int CheckComboAtk();
    void endBloodMove(CCObject *pObj, void *pVoid);
    
    
   
};

#endif
