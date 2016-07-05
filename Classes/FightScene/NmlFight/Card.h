//
//  Card.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_Card_h
#define CardDemo_Card_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "CJumpNum.h"
#include "TextNode.h"
#include "encryp_data.h"
#include "CSkillLib2.h"

using namespace cocos2d;


#define FENGYINPNGTAG 130401
#define FENGYINDEFENSETAG  130409


enum EN_CARD_STATES
{
  EN_CARD_STATE_NORMAL = 0,
  EN_CARD_STATE_FNEGYIN,
};

class Card : public CCLayer
{
    int         m_state;
    
    CEncrypData<int>         m_iHP;
    int         m_iRecoverPt;
    CEncrypData<int>         m_iAttackPt;
    int         m_iElement;
    int         m_iSubElement;
    
    int         m_iComboBase;
    int         m_iSubComoBase;
    
    int         m_iManSkillID;
    int         m_iAutoSkillID;
    int         m_iSkillCDMax;
    CEncrypData<int>  m_iCurSkillCD;
    
    
    int         m_iSklCdDwn;
    int         m_iCategory;
    int         m_iSubCategory;
    char        m_iName[100];
    int         m_iAwakeSkillArr[MaxAwakeSkillCnt];     //总存在突破技能
    int         m_iAwakeNum;                            //已能力突破数量
    
    short       m_countryPorperty; //国家属性
    //int         m_atk_num;
    bool        m_isTouchOffAwakeSkill_28;
    
    int         m_idx;
    unsigned short  m_cid;
    short         m_FengYinCD;
    int         m_plus;
    
    list<enElementDef> m_listSkillElement;
    
    CJumpNum*   m_numAtkVal;
    CJumpNum*   m_mininumAtkVal;
    
    CAttackInfo m_SubAtkInfo;
    CAttackInfo m_AtkInfo;
    CAttackInfo m_skillAtkInfo;
    
    CCSprite*   m_spCard;
    
    CCLayerColor*   m_lcCardBk;

public:
    Card(int idx);
    
    bool bisTouchOffAwakeSkill_28(); //用于能力突破28
    void setisTouchOffAwakeSkill_28(bool bl);
    
    short getCountryPorperty();//获取卡片的国家属性
    
    int getAwakeSkillNum();
    int getAwakeSkillArr(int idx);
    int getCanUseNumByAwakeId(int idx);
    
    short getFengYinCD(){return m_FengYinCD;};
    void setFengYinCD(short cd);
    
    void setCardIndex(int cardIndex);       //设置card位置
    
    void resetCardPos();        //重置card的位置
    
    void setCardState(int state, int param1 = 0);
    int getState(){return m_state;};
    
    void fengYinUpdate(int cd);
    
    void initCardFromSaveData();    //  用本地保存的数据初始化
    
    void setJumpNumber(CJumpNum* jumpNum,bool isMainElem = true);
    
    virtual void onExit();
    
    const CCPoint& getCardPos();
    
    void reset();
    
    void stoneEraseNotify(int iCnt, float fStonePlus, bool bAOE = false, bool isMain = true);
    void comboNotify();
    
    void setManSkillId(int manSkillId) {m_iManSkillID = manSkillId;};
    
    const char *getName();
    int  getSubElement();
    int  getElement();
    int  getRecover();
    int  getBaseHp();
    int  getAttack();
    int  getManSkId();
    int  getAutoSkId();
    int  getCategory();
    int  getSubCategory();
    
    int carryAutoSkId();
    
    void decHpFactor(float fFactor);
    void setHpFactor(float fFactor);
    
    void decRecoverFactor(float fFactor);
    void setRecoverFactor(float fFactor);
    
    void setSubSpecialAtk(int type , int cout,int nAttack);
    void setSpecialAtk(int type , int cout,int nAttack);
    CAttackInfo&  getAttackInfo();
    CAttackInfo& getSubAttackInfo();
    
    void attack();
    
    const CCSize& getContentSize();
    
    void updateCd(int iCd);
    int  getCD();
    int  getCDMax();
    void decCD();
    void skillDecCD(int cd);
    void resetSkillCD();

    CAttackInfo&  useManSkill();
    
    void useAutoSkill(float fEffect, int iBaoJi = 0);
    
    void drawSkillX3(float fEffect, int iBaoJi = 0);

    
    void showSkillAnimation();
    void subUseAutoSkill(float fEffect);
    
    void useAwakeSkill(int nID){}
    
    void delUnusedObj();

    void initSkillAtk();
    
    void yanChiSkillCD(bool bYanChi);
    
    void yanChiFromSaveData();  // 进度保存进入有延迟buff的战场（技能最大CD bug）
    
    void setNoCD();
    
    void resetSklCdDwn();
    
    void showSkillElementList();
    void resetSkillElementList();
    void showSkillElement(enElementDef elem);
    
    int getBaseRecover();  // recoverPt可能被被动技能翻倍了，从savedata中获取card的真实恢复力
    unsigned short getCardID();
    unsigned short getCardPlus();
    void awakeUpdate(bool bAdd,bool bInit = true);
    
    
};


#endif
