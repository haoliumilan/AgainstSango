//
//  StoneGrid.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_StoneGrid_h
#define CardDemo_StoneGrid_h

#include "cocos2d.h"
#include <list>
#include "CommonDef.h"
#include "CGameState.h"
#include "CGamedata.h"

using namespace cocos2d;

class Arrow;
class MsgBox;

typedef struct _StoneNode{
    CCSprite*   spStone;
    
    int         x;
    int         y;
    
    bool        bAlive;
    bool        bErasable;
    bool        bSearched;
	CEncrypData<int>          iElement;
    int         iPlus; //0: 无加号效果
    int         iBlackCover; //0 没有变黑，1表示黑了
    int         iParam1;   //参数1，可以表示特殊宝珠类型
    int         iParam2;   //参数2，可以表示特殊宝珠的参数
    
    struct _StoneNode *left;						
	struct _StoneNode *top;						
	struct _StoneNode *right;						
	struct _StoneNode *bottom;

    _StoneNode() {
        iPlus = 0;
    };
}StoneNode, *PStoneNode;

typedef std::list< PStoneNode >            StoneNodeList;
typedef std::list< PStoneNode >::iterator  StoneNodeIter;


typedef std::list< CCLabelAtlas* >            ComboLbList;
typedef std::list< CCLabelAtlas* >::iterator  ComboLbIter;

typedef std::list< CCNode* >            TmpObjList;
typedef std::list< CCNode* >::iterator  TmpObjIter;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class StoneGrid : public CCLayer
{
    StoneNode       m_stoneNodeArray[kMaxGridRowCnt][kMaxGridColumnCnt];
    
    StoneNodeList   m_grpList[10];
    int             m_iGrpNo;

    ComboLbList     m_lbComboList;
    
    CCLabelAtlas*   m_curLb;
    CCLabelAtlas*   m_curLb2;

    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    CCSprite*       m_spHold;
    
    CCSprite*       m_spNewbieStart;
    Arrow*          m_pArrow;
    CCSprite*       m_pLongArrow;
    MsgBox*         m_pMsgBox;

    TmpObjList      m_tmpObjLst;
    
    
    int             m_iStartColumn;
    int             m_iStartRow;
    
    int             m_iCurColumn;
    int             m_iCurRow;
    
    int             m_iCombo;
    int             m_curEraseCnt;
    int             m_curEraseElement;
    float           m_fCurErasePlus;
    
    int             m_iMaxStoneNum;
    
    CCPoint         m_curErasePos;
    
    bool            m_bStartFlg;
    bool            m_bFreeFlg;
    bool            m_bInitMatrixFlg;
    
    float           m_fAdditionalDur;
    float           m_fMaxMvDur;
    float           m_fElapse;
    CCSprite*       m_spHand1;
    CCSprite*       m_spHand2;
    CCSprite*       m_spHandProgrs;
    CCPoint         m_ptCurTouch;
    
    int             m_iNewbieStep;
    int             m_iNewbieStep5;
    
    int             m_iSndTag;
    
    char            m_stoneCount;
    char            m_stoneElement[enElementMax];
    bool            m_bUserDid;
    CCRect         newBieLineArr[7];
    int            m_newBie4Through;
    
    //CCLayer
    CCLayer*        m_ArrowLayer;
    
    int             m_iPlusLbIdx;
public:
    StoneGrid(CCObject* target, SEL_CallFunc selector);
    virtual ~StoneGrid();
    
    void        initMatrix(int newbieStep);
    //初始化路线引导
    void        initGuideMatrix(int idx);

    void        saveAllStones();
    
    void        startNewRound();
    int         getCurEraseCnt();
    void        setCurEraseCnt(int cnt){m_curEraseCnt = cnt;};
    int         getCurEraseElement();
    CCPoint&    getCurErasePos();
    float       getCurErasePlus();

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    float       checkStoneGrid();
    
    void        addStoneSpecial(int src, int Type);
    
    void        chgStoneFromEnemy(int src, int dst);
    
    void      chgStoneNumberFromEnemy(int num,int dst,bool plus= false);//随机几颗宝珠变成其它元素的珠子
    
    void         stoneChangeToLight();
    
    void        StoneShowBlack();
    
    void        update(ccTime dt);
    void        chgStone(int src, int dst);
    
    void        chgStone2(int src, int dst);    //宝石变化，变成3个宝石
    
    int         clearStone(int src, int dst);       //宝石清除
    
    bool        haveStone(int src);
    
    bool        haveStone(int src,int dst);         //是否含有宝石
    
    int         getOneBeElement(int nNotElement)const;
    
    void        ignitFreeMode(float fDur);
    void        setAdditionalDur(float fDur);
    float       getAdditionalDur();
    
    void        actAfterMove(float time);
    
    bool        isFreeMode();
    void        enableTouch(bool bTouch);
    
    void        setSoundTag(int tag);
    
    bool        isUserDid();
    
    virtual void onExit();
    
    void        enemyResetStone();
    
    //checkInterrupt
    void CheckInterrupt(CCObject* obj);
    
    bool        canGiveStonePlus(int iElem);
    bool        isAllElem(int iElem);
    void        giveStonePlus(int iElem);
    
    int         getComboNum();
    
    void        setSkillCombo(int iCombo, float fEffect);
    void        setSkillCombo2(int iCombo, float fEffect);
    
    void        showSkillEffect(CCPoint &point,float fEffect,enElementDef emElement = enElementMax);
    
    void    changeCaiDanStone();
    
    void changeMaxDurByTime(float dt);
    
    static int     getOddsAwake10(float oddf, float nums);
    
private:
    CCSprite*   getSpriteByElement(int iElement);
    void    initMatrixNml(bool bErasable, bool bUseSaveData = false);
    void    initMatrixNewbie(int newbieStep, bool bDrop);

    bool    srchGrp();
    bool    chkErasable();
    void    sch1Grp(PStoneNode node, int iElement);
    
    void    onlyDrop();
    
    void    erase(ccTime dt);
    void    drop(bool autoClear = true);        //autoClear 判断是否自动消除
    void    dropDelay(ccTime dt);               //掉落延时
    void    chgState(ccTime dt);                //用于修改stone是否可移动
    void    dropCb(ccTime dt);
    void    clrCombo();
    void    cbClrAllStones(ccTime dt);
    void    cbClrAllStonesDrop(ccTime dt);
    void    cbChgStone(ccTime dt);
    void    cbChgStoneEnemy(ccTime dt);
    void    cbEnemyResetStone();
    
    void    resetLink();
    
    void    setHand(const CCPoint& pos, float fPercent);
    
    void    touchEndProc();
    
    int     randElement();
    
    void    removeArrowLayer();

    void    showEraseAnim(int iElement, const CCPoint& pos);
    CCActionInterval* getActionFromFile(const char* filename, bool bForever = false);
    
    void updateAwakeAtkRatio(int idx);
    
};







#endif
