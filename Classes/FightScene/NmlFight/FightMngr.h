//
//  FightMngr.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_FightMngr_h
#define CardDemo_FightMngr_h

#include "cocos2d.h"
#include "Player.h"
#include "EnemyGrp.h"
#include "FightPauseMenu.h"
#include "CListLayer.h"
#include "Dialog1.h"
#include "CScrollBar.h"
#include "Dialog2.h"
#include "CGamedata.h"

using namespace cocos2d;



class CTipsDlg;
class DeadDlg;
class CommDlg;
class TitleBar;
class TextNode;
class DeadReviveDlg;

enum {
    enFightMngrReqType_Null=0,
    enFightMngrReqType_Revive,
    enFightMngrReqType_PaySuccess,
    enFightMngrReqType_GetShopInfo,
    enFightMngrReqType_GetTradeNo,
    enFightMngrReqType_RefreshAlipay,
    
    
    enFightMngrReqType_GiveWuJiang,
    enFightMngrReqType_GiveWuJiangAllCards,
    
    enFightMngrReqType__PayppBillNo,
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CFightRet : public CCObject
{
public:
    bool    bClear;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class FightMngr : public CCNode
{
    int             m_iReqType;
    int             m_iCurStep;
    int             m_iMaxStep;
    int             m_iDropItemCnt;
    int             m_iDropGoldTotal;
    int             m_iCollectStep;
    int             fightSceneType;
        
    long            m_lReviveRty;
    bool            m_isUseAwakeSkill29;
    
    long            payArr[enPayfor_max];
    
    long            payTag;
    
    CCSprite*       m_spGold;
    CJumpNum        *m_lbGold;
    
    CCSprite*       m_spBox;
    CCLabelAtlas*   m_lbBox;
    CCMenuItem*     m_menu;
    CCMenuItem*     m_btnCaiDan;    // caidan button（normal->caidan）
    CCMenuItem*     m_btnNormal;    // normal button (caidan->normal)
    CCSprite*       m_spTriangle;
    CCSprite*       m_spFightScene;
    
    Player*         m_player;
    EnemyGrp*       m_enemyGrp;
    
    
    
    CCLayerColor*   m_lcCover;
    
    CTipsDlg*       m_tips;

    TextNode*       m_lbShowStep;
    CCSprite*       m_spShowStep;

    CFightRet       m_ret;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    FightPauseMenu *pauseMenu;
    DeadDlg*        m_deadDlg;
    DeadReviveDlg*  m_deadReviveDlg;
    CommDlg*        m_commDlg;
    Dialog2*        m_checkRevDlg;
    CListLayer*     m_listCoin;
    TitleBar*       m_spTitle;
    CCMenuItem*     m_miBack;
    CCParticleVictoryStar* m_starEffect;
    
    Dialog2*   m_fanhuanDlg;
    
    int             m_iIndex;
public:
    
    int             m_iSL_type;
    
    FightMngr(CCObject* target, SEL_CallFuncO selector);
    ~FightMngr();
    
    Player* getPlayer(){return m_player;};
    EnemyGrp* getEnemyGrp(){return  m_enemyGrp;};
    
    void startNewFight(int type, enSpecialDgSkillType skillType, bool cntryBuff = true);
    
    int  getCoin();
    
    bool haveEnemyWithNotElem(int iElem);
    bool haveEnemyWithElem(int iElem);
    bool haveEnemyWithElemInstead(int iElem);
    
    bool isEnemyBuffChenMo();
    bool isEnemyBuffYanChi();
    bool enemyGrpIsLive();
    
    bool isEnemySick();
    bool isEnemyPoisned();
    bool isEnemyDelayed();
    bool isEnemyMabi();
    bool isEnemyArmorBroken();
    bool isEnemyChangeElement();
    
    void removeBuffEffect(int idx);
    void addBuffEffect(int idx);
    
    void AtkPlayerDirect(CAttackInfo atkInfo);
    
    void saveCurFightInfo(bool isStoneMove = false);    // 保存每一回合的战场状态信息
    
    int getCurStep();
    
    void showAlipayOver(int payType);
    
    void forceExitFight();
    
    void initPlayerRoundVars();
    
    void newbie(CCObject* pObj);
    
    void changeStoneAndAtk(CAttackInfo* pAtkInfo);
    
    
    void exit();
    
    void waitPPBilloNo(ccTime dt);
    void waitPPPurchaseCb(ccTime dt);

private:
    void checkReviceCb(CCObject *sender);
    void newEnemyGrp();
    void startStep();
    void startStepFromSaveData();
    void cbStartStep();

    void attackDetail(CAttackInfo* atkinfo);
    void shoot(const CCPoint& src, const CCPoint& dst);
    
    void StopRecordScreen(bool saveFile);
    void startRecordScreen(ccTime dt);
    void CheckInterrupt(CCObject* pSender);
    
    void cbPlayer(CCObject*);
    void cbEnemyGrp(CCObject*);

    void cbStartFight();
    
    void cbCollect(ccTime dt);
    
    void fightEnd();
    
    void updateInfBar(int gold, int item);
    
    void menuCallback(CCObject* sender);
    
    void cbCaiDan(CCObject* sender);
    
    void cbTipsDlg();
    
    void fmCallback(CCObject* sender);
    
    void ddCallback(CCObject* sender);
    void ddReviveCallback(CCObject* sender);
    void ddFreeReviveCallback(CCObject* sender);
    
    void PlayerEndEnemyBegin(ccTime dt);
    
    void showEnd();
    
    void showEndAction(bool bClear);
    
    void update(ccTime dt);
    
    void cbCommDlg(CCObject* );
    
    void removeGabage();
    
    void hideFightScene();
    void showFightScene();
    
    void cbPlayerDeadDelay(ccTime dt);

    void beginCoinShop();
    
    void enterCoinList();
    
    void closeCoinList();
    
    void coinBtnAdd(const char *iap_id,const char *price,int index);
    
    void payBtnCB(CCObject *pSender);
    
    void showPayResult(int result);
    
    void payForCoin(int index);
    
    void waitPurchaseCb(ccTime dt);
    
    void dialog1Callback(CCObject* sender);
    
    void btnCallback(CCObject* sender);
    
    void showDeathDlg();
    
    void showStarEffect();
    
    void updateEnemyGrpBuff();
    void setEnemyBuffEffect();
    void cbEnemyBuffCanFei(ccTime dt);
    
    void cbHeadTips();
    void cbEndTips();
    
    void cbList(CCObject *pObj);
    
    void getShopInfo();
    
    bool countAwakeSkill25();
    
    void showGiftBage(CCObject* pSender);  //用于战场中购买礼包时，点击礼包说明框时的调用
    
};

#endif
