//
//  OrgFightMngr.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgFightMngr__
#define __AgainstWar__OrgFightMngr__

#include "cocos2d.h"
#include "encryp_data.h"

using namespace cocos2d;

class OrgStoneGrid;
class CPlayerBlood;
class CCardInfoDlg;
class FightPauseMenu;
class CTipsDlg;
class OrgStoreEnergy;
class CommDlg;

#define kComboArrLen    3

typedef struct{
    char    ucid[100];
    int     xpos;
    int     ypos;
    int     width;
    int     height;
    int     iElem;
}stOrgCardInf;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgFightRet : public CCObject
{
public:
    bool bCashin;

    OrgFightRet():bCashin(false){};
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgFightMngr : public CCLayer
{
    static OrgFightMngr* s_pcIf;
    
    bool            m_bInitFlg;
    bool            m_bFighting;
    bool            m_bIsTongMengLianShou;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    CCLayerColor*   m_lcCover;
    OrgStoneGrid*   m_stoneGrid;
    CPlayerBlood*   m_blood;
    CCardInfoDlg*   m_cardInfoDlg;
    CCSprite*       m_spEnemy;

    CCMenuItem*     m_menu;
    FightPauseMenu* m_menuLayer;
    
    OrgStoreEnergy* m_storeEngy;

    int                         m_iComboIdx;
    OrgFightRet     m_ret;
    CEncrypData<int>            m_iMaxCombo;
    CEncrypData<int>            m_iCurCombo;
    
    
    int             m_iBaseHP;
    int             m_iHP;
    int             m_iCardCnt;
    int             m_iBossHp;
    
    stOrgCardInf    m_OrgCardInfArr[5];
    
    CCPoint         m_playerPos;
    CCPoint         m_EnemyPos;
    
    CommDlg*        m_commDlg;
    string          m_strDgId;
    string          m_strMaxCombo;
    
    long            m_lEnemyCid;
    
    CCSprite*       m_spRound;
    CCLabelAtlas*   m_lbRound;
public:
    //Get the instance pointer
	static OrgFightMngr* Instance(){
        if(s_pcIf == NULL){
            s_pcIf = new OrgFightMngr();
        }
        
        return s_pcIf;
    }
    
    ~OrgFightMngr(){s_pcIf = NULL;}
    
//    void init(CCObject* target, SEL_CallFuncO selector, string dg_id, long lEnemyCid);
    void init(CCObject* target, SEL_CallFuncO selector, string dg_id, long lEnemyCid,bool isTMLS);
    
    void startNewFight();
    void enterArenaEnd();   // 账户绑定有效期已过，重新登入后，直接向服务器发结束战场请求
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
private:
    void cbStartFight();
    void cbStoneGrid();
    
    void placeFightScene();
    void placeCards();
    void placeCardsFromSaveData();
    void placeHpBar();
    void placeEnemy();
    
    bool showCardInf(CCTouch *touch);
    
    void menuCallback(CCObject* sender);
    void cbMenuLayer(CCObject* sender);
    
    void QuitFight();
    
    void shoot(const CCPoint& src, const CCPoint& dst, float dt);
    void attackEnemy(int atkpt, float dt);
    void attackPlayer(float dt);
//    void cbAttackPlayer(ccTime dt);
    
    void procStepAnim();
    void dostep1();
    void dostep2();
    void dostep3();
    void cbStepOvr(ccTime dt);
    
    void animBom(const CCPoint& pos, float fScale, float dt);
    void animHol(const CCPoint& pos, float fScale, float dt);
    void animHalo(const CCPoint& pos, float fScale, float dt);
    void animComm(const char* szFilename, const CCPoint& pos, float fScale, float dt);
    
    void runAtkAnim(int iElem, int iXpos, float dt);
    void runSucAnim(float dt);
    void runFailAnim(float dt);
    
    void reqDgEnd();
    void update(ccTime dt);
    void cbCommDlg(CCObject* );
    void procDgEnd();
    void YouWin();
    void cbYouWin(ccTime dt);
    void YouLose();
    void cbYouLose(ccTime dt);
    void BossDie();
    void cbBossDieDlg(CCObject* pObj);
    
    void saveCurFightInfo();
    
    void PlayerAtkSndEffct();
    void BossAtkSndEffct();
    void BossBlinkSndEffct();
    
    void showJumpCombo(int iCombo);
    void hideJumpCombo();
    void showFinalCombo(int iCombo);
    void hideFinalCombo();
    
    void decEnemyRound(ccTime dt);
private:
    //forbid constructor being called outside
	OrgFightMngr():m_bInitFlg(false),m_bFighting(false){};
    
	//forbid copy constructor being called outside
	OrgFightMngr(const OrgFightMngr& );
	
	//forbid assignment
	OrgFightMngr& operator=(const OrgFightMngr& );
};

#endif /* defined(__AgainstWar__OrgFightMngr__) */
