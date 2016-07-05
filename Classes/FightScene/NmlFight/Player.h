//
//  Player.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_Player_h
#define CardDemo_Player_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "Card.h"
#include "StoneGrid.h"
#include "CPlayerBlood.h"
#include "CJumpNum.h"
#include "CCardInfoDlg.h"
#include "CSkillDlg.h"

#include "FeatureAnim.h"
#include "encryp_data.h"

using namespace cocos2d;

class MsgBox;
class Arrow;
class TextNode;
class ShiningFrame;
class CAutoSkillAppend;
class EnemyGrp;
class Enemy;

#define kMaxNewbieCoverNum  7
#define kPlayerBuffCnt  10

enum enWudiTypeDef {
    enWudiType_Normal = 1, // 按回合对指定属性无敌
    enWudiType_Hurt,        // 按伤害对指定属性无敌，当被伤害的时候，无敌的有效回合数才会减
    enWudiType_OneAndAll,   // 按回合对指定属性无敌，同时对其他属性伤害减半
    enWudiType_HarfHurt,    // 按回合对所有伤害减半
    enWudiType_NoHurt,      // 按回合对所有伤害无敌
    enWUdiType_Max,
};

typedef struct tagBuffGrp
{
    int     iCD;
    int     iElem;
    int     iType;
    float   fEffect;
    float   fFactor;
    CCSprite*       sp;
    TextNode*       lb;
    
    tagBuffGrp()
    {
        iCD = iElem = iType = 0;
        fEffect = fFactor = 0.0f;
        sp = NULL;
        lb = NULL;
    }
}stBuffGrp;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Player : public CCLayer
{
    int         m_iRcvAttack;   // 用于以多余的血作为攻击力
    CEncrypData<int>         m_iPreBaseHp;
    CEncrypData<int>         m_iBaseHP;
    int         m_iBaseRecoverPt;
    
    CEncrypData<int>    m_iHP;
    CEncrypData<int>         m_iPrevHp;
    int         m_iRecoverPt;
    int         m_iComboBaseRcvPt;
    int         m_iRoundStartHp;
    int         m_iRoundEndHp;
    
    int         m_iTouchedCard;
    
    bool        m_bErased;
    
    // *********** 各种带buffIcon的武将技能
    // 减伤害
    stBuffGrp m_stArmorBuffGrp;
    // 回血
    stBuffGrp m_stRcvBuffGrp;
    // 回复力提升
    stBuffGrp m_stRcvWuJiangBuffGrp;
    // 无敌
    struct tagWudiBuffGrp: stBuffGrp
    {
        enWudiTypeDef    enType;        // 无敌类型
        
        tagWudiBuffGrp()
        {
            enType = enWudiType_Normal;
        }
    }m_stWudiBuffGrp;
    // 用于对应属性武将攻击力增加
    stBuffGrp m_stEnhanceBuffGrp;
    // 横向消 6 个珠子 ，属性加强
    stBuffGrp m_stEnhanceH6BuffGrp;
    //用于武将类型攻击增加
    stBuffGrp m_stEnWujiangBuffGrp;
    //用于国家武将类型的增加
    stBuffGrp m_stCountryWujiangBuffGrp;
    // 反击
    struct tagFanJiBuffGrp: stBuffGrp
    {
        bool        bMianSiFanJi;
        int         iJueDiCardIdx;
        
        tagFanJiBuffGrp()
        {
            bMianSiFanJi = false;
            iJueDiCardIdx = 0;
        }
    }m_stFanJiBuffGrp;
    // 普通攻击变为aoe
    stBuffGrp m_stAOEBuffGrp;
    // 虚弱状态
    struct tagSickBuffGrp: stBuffGrp
    {
        bool            bIsManSkill_257;
        
        tagSickBuffGrp()
        {
            bIsManSkill_257 = false;
        }
    }m_stSickBuffGrp;
    
    // *********** 各种带buffIcon的武将技能
    
    unsigned long   m_ulCombo;
    
    float       m_fAwakeAtk[6];
    
    Card*       m_card[kMaxCardCntInBattle];
    StoneGrid*  m_stoneGrid;
    
    CPlayerBlood*   m_blood;
    CJumpNum*       m_bloodNum;
    
    CCardInfoDlg*   m_cardInfoDlg;
    CSkillDlg*      m_skillDlg;
    
    TextNode*       m_tlabel;
    
    
    CAttackInfo     m_AtkInfo;
    
    int             m_iCurAttackCard;
    bool            m_bIsMainAtk;
    
    
    bool           m_tempBoolNew;
    
    MsgBox*         m_pMsgBox;
    int             m_iNewbieMsgIdx;
    Arrow*          m_pArrow;
    CCSprite*       m_newbieCover[kMaxNewbieCoverNum];
    int             m_iNewbieCovIdx;
    int             m_iNewbieStep;
    int             m_iNewbieCombStepIdx;
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    bool            m_bTouchEnable;
    
    int             m_iCurManSkillID;
    int             m_iSndTag;
    
    int             m_StoneEraseCnt[enElementMax];
    bool            m_bElementAttck[enElementMax];    // 对应属性武将是否攻击
    
    CCSprite*       m_buffArr[kPlayerBuffCnt];
    
    CCMutableArray<CAttackInfo*> *m_EnemyAtkArr;
    
    bool            m_bSkillCDYanChi;
    
    bool            m_bCanFei;
    
    ShiningFrame*   m_pSkillHighlight;
    
    // record video
    float           m_record_wait_time;
    
    FeatureAnim*    m_featureAnim;
    
    int             m_iAwakeCout[enAwakeSkill_Max];
    
    bool            m_bIsManSkill_129;  //  专门为129号武将技添加一个表示，技能效果比较特殊
    
    
    bool            m_bIsOver6Fire;
    int             m_i258_259ManSkillID;
    
    
    // enEnemyBuff_73/enEnemyBuff_76
    bool            m_bHasHeartPlus;
    bool            m_bHasWaterPlus;
    bool            m_bHasFirePlus;
    bool            m_bHasWoodPlus;
    bool            m_bHasLightPlus;
    bool            m_bHasDarkPlus;
    
    int             m_iAwakeSkill_33;// 0:重新计算并保存和返回    1:返回true    2:返回false
    
public:
    
    void resetAwakeAtk();
    float getAwakeValueById(int idx);
    void updateAwakeAtk(float f, int idx);
    
    int  getNumByAwakeId(int idx);
    void updateNumByAwakeId();
    
    
    
    Player(CCObject* target,SEL_CallFuncO selector);
    
    void changeTeamInfo();  // 换编队，坑爹的全队伍限定战场
    void initCard();
    void resetCard();
    void initJumpNum(int index, bool bMainElem);
    void initBuffFromSaveData();        // 如果是s&l,加各种buff
    void resetBuff();   // 全队伍限定战场，进入下一回合，所有buff效果都取消
    
    void initMatrix(int newbieStep);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    bool isManSkillCanUse(int manSkillID);
    
    const CCPoint& getPosition();
    
    
    
    int getCurCombo();
    Card* getCardByIdx(int idx);
    Card * getCardCID(unsigned short unCID)const;
    
    void beAttacked(CAttackInfo*);
    void beDirAtk(CAttackInfo*);
    
    void startNewRound(bool first);
    void startFromMove();
    
    bool isDead();
    
    void revive();
    void reviveByOption(int order);
    
    virtual void onExit();
    
    void saveCurPlayerInfo();
    
    void addBlood(int iBlood);
    
    void setBlood(int nHp,int BaseHp);
    
    void hideBloodNum();
    
    void enemyAtkOvr();
    
    void yanChiSkillCD(bool bYanChi);
    
    bool canFei(bool bCanFei);
    void canFeiAnimate(int iCanFeiBlood);
    
    void beHitFromEnemyBuf(int bufIdx);
    //   void beWenYiHuanJing();
    void removeBuffEffect(int idx);
    void addBuffEffect(int idx);
    
    void setCardsNoCd();
    
    void fengYinAutoSkill(int idx);
    void unFengYinAutoSkill(int idx);
    
    StoneGrid*  getStoneGrid(){return m_stoneGrid;};
    
    void updateRoundStartHp();
    void resetSkillCD();
    
    // record video
    void StartUpdate();
    void StopUpdate();
    void update(ccTime dt);
    float getRecordWaitTime(){return m_record_wait_time;};
    void setRecordWaitTime(float time){m_record_wait_time = time;};
    
    void RecordControl(bool pause, int time);
    
    void changeCaiDanStone();       // 切换彩蛋珠子
    
    int getCurHp();
    int getBaseHP();
    
    
    int getArmorCD(){return m_stArmorBuffGrp.iCD;}
    
    int getEnhanceCD(){return m_stEnhanceBuffGrp.iCD;}
    //
    int getEnhanceH6CD(){return m_stEnhanceH6BuffGrp.iCD;}
    float getEnhanceH6Factor(){return this->m_stEnhanceH6BuffGrp.fFactor;}
    
    int getEnWujiangCD(){return m_stEnWujiangBuffGrp.iCD;}
    
    void setArmorCD(int armorCD){this->m_stArmorBuffGrp.iCD = armorCD;}
    
    void setEnhanceCD(int enhanceCD){this->m_stEnhanceBuffGrp.iCD = enhanceCD;}
    // 横向 6消 属性加强
    void setEnhanceH6CD(int enhanceH6CD){this->m_stEnhanceH6BuffGrp.iCD = enhanceH6CD;}
    void setEnhanceH6Elem(int enhanceH6Elem){this->m_stEnhanceH6BuffGrp.iElem = enhanceH6Elem;}
    void setEnhanceH6Factor(float enhanceH6Factor){this->m_stEnhanceH6BuffGrp.fFactor = enhanceH6Factor;}
    
    void setEnWujiangCD(int enWujiangCD){this->m_stEnWujiangBuffGrp.iCD = enWujiangCD;}
    
    void enemyDecBuffCD();
    
    void fengYinUpdate(int cd);
    
    int getBaseRecoverPt()const;
    void setBaseRecoverPt(int nBaseRecoverPt);
    int getRealBaseRecoverPt();
    
    int getCurManSkillID() {return m_iCurManSkillID;};
    // CAttackInfo getAttackInfo() {return m_AtkInfo;};
    void changeZhuJiang(int index1, int index2);
    bool getElementHasPlus(int element);
    void doAutoSkillBeforeBeingAttacked(CAttackInfo* atkinfo);
    
    //  int countAwakeSkill_29();
    void setAnimal_awakeskill_30();
    bool haveAwakeSkill_33(); // 是否觉醒技33 有觉醒的
    bool haveAwakeSkill_32();
    
    void delayPlaySkillCD(int d_cd);
private:
    void updateBlood(bool bUpdate = false);
    
    void cbEndChange(CCObject* obj,int tag);
    
    void cbStoneGrid();
    void chkComboEffect();
    void cbSkillDlg(CCObject*);
    void cbSkillDlg2();
    
    void useSkill(ccTime dt);
    
    void shoot(const CCPoint& src, const CCPoint& dst, int iElem);
    
    void attack(ccTime dt);
    
    int  containsTouchLocation(CCTouch *pTouch);
    
    void doAwakeSkillWhileStarting();
    void doActForAwake();
    
    bool cardHasAtkByIdx(int idx);
    void doAutoSkillWhileStarting();
    bool doAutoSkillBeforeAttacking();
    void doOneAutoAtkSkillBeforeAtk(int iSkillCardId, int i,
                                    float &fEffect, bool &bCard0Jmp , int skillID);
    
    
    void doOneSubAutoAtkSkillBeforeAtk(int iSkillCardId, int i,
                                       float &fEffect, bool &bCard0Jmp , int skillID);
    
    
    
    void doOneAutoRcvSkillBeforeAtk(int iSkillCardId, bool &bRet,
                                    int &iRecovTotal, bool &bCard0Jmp);
    void doAwakeSkillBeforeAtk(int &iRecovTotal);
    
    void doCountrySkillBeforeAtk(int &iRecovToal);
    
    bool doAutoSkillAfterAttacking();
    //  void doAutoSkillBeforeBeingAttacked(CAttackInfo* atkinfo);
    void doAutoSkillAfterEnemyAtkOvr();
    
    
    void doAwakeSkillBeforeBeingAttacked(CAttackInfo* atkinfo);
    
    
    const char* getPlayerTip();
    
    int  chkAutoSkillAvail(int skid);
    
    void decBuffCD();
    
    void showHpDownNum(int iAtkPt);
    
    void addAOEBuff();
    void addArmorBuff();
    void addWudiBuff();
    void addRcvBuff();
    
    void addRcvWuJiangBuff();
    
    void addJdfjBuff();
    void addEnhanceBuff();
    void addEnWujiangBuff();        //武将类型攻击增强buff
    void addSickBuff();             // 虚弱
    void addEnhanceBuffH6();
    
    void removeBuff(CCSprite* sp);
    void addBuff(CCSprite* sp);
    void redrawBuff();
    
    void updateSkillHighlight();
    
    int  getAttackElemCnt();
    
    bool bHasAOEOfElement(enElementDef element);
    
    void attackOver();
    void makeRcvAttack();
    
    bool beAllGoGenIDList(CAutoSkillAppend *pAutoSkillAppend);
    
    
    void addCountryWujiangBuff();
    
};

#endif
