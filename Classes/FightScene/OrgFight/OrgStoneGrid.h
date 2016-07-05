//
//  OrgStoneGrid.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgStoneGrid__
#define __AgainstWar__OrgStoneGrid__

#include "cocos2d.h"
#include <list>
#include "CommonDef.h"
#include "CGameState.h"
#include "CGamedata.h"
#include "StoneGrid.h"

using namespace cocos2d;

//typedef struct _StoneNode{
//    CCSprite*   spStone;
//    
//    bool        bAlive;
//    bool        bErasable;
//    bool        bSearched;
//	int         iElement;
//    
//    struct _StoneNode *left;
//	struct _StoneNode *top;
//	struct _StoneNode *right;
//	struct _StoneNode *bottom;
//    
//}StoneNode, *PStoneNode;

typedef std::list< PStoneNode >            StoneNodeList;
typedef std::list< PStoneNode >::iterator  StoneNodeIter;


typedef std::list< CCLabelAtlas* >            ComboLbList;
typedef std::list< CCLabelAtlas* >::iterator  ComboLbIter;

typedef std::list< CCNode* >            TmpObjList;
typedef std::list< CCNode* >::iterator  TmpObjIter;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgStoneGrid : public CCLayer
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
    
    TmpObjList      m_tmpObjLst;
    
    int             m_iStartColumn;
    int             m_iStartRow;
    
    int             m_iCurColumn;
    int             m_iCurRow;
    
    int             m_iCombo;
    int             m_curEraseCnt;
    int             m_curEraseElement;
    
    CCPoint         m_curErasePos;
    
    bool            m_bStartFlg;
    bool            m_bInitMatrixFlg;
    
    float           m_fAdditionalDur;
    float           m_fMaxMvDur;
    float           m_fElapse;
    CCSprite*       m_spHand1;
    CCSprite*       m_spHand2;
    CCSprite*       m_spHandProgrs;
    CCPoint         m_ptCurTouch;
    
    int             m_iSndTag;

public:
    OrgStoneGrid(CCObject* target, SEL_CallFunc selector);
    virtual ~OrgStoneGrid();
    
    void        initMatrix();
    void        startNewRound();
    int         getCurEraseCnt();
    int         getCurEraseElement();
    CCPoint&    getCurErasePos();
    
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void        update(ccTime dt);

    void        enableTouch(bool bTouch);
    
    void        setSoundTag(int tag);
    
private:
    CCSprite*   getSpriteByElement(int iElement);

    bool    srchGrp();
    bool    chkErasable();
    void    sch1Grp(PStoneNode node, int iElement);
    
    void    erase(ccTime dt);
    void    drop();
    void    dropCb(ccTime dt);
    void    clrCombo();
    void    cbClrAllStones(ccTime dt);
    void    cbClrAllStonesDrop(ccTime dt);
    void    cbChgStone(ccTime dt);

    void    resetLink();
    
    void    setHand(const CCPoint& pos, float fPercent);
    
    void    touchEndProc();
    
    int     randElement();
    
    void    showEraseAnim(int iElement, const CCPoint& pos);
    CCActionInterval* getActionFromFile(const char* filename);

};

#endif /* defined(__AgainstWar__OrgStoneGrid__) */
