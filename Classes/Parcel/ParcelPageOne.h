//
//  ParcelPageOne.h
//  AgainstWar
//
//  Created by AllenSK on 14-2-11.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ParcelPageOne__
#define __AgainstWar__ParcelPageOne__

#include "cocos2d.h"
#include "CGamedata.h"
#include "ItemCardLayer.h"
#include "Package.h"

#include "ParcelPageTwo.h"

using namespace cocos2d;

class CListLayer;
class CScrollBar;
class TeamInf;
class CTeamInfoScrollViewLayer;
class PwUpBase;
class Evolution;
class EvoOverShow;
class CommDlg;
class TitleBar;

enum enPackScreenDef_ParcelPageOne{
    enPkSc_Null_ParcelPageOne,
    enPkSc_TeamInf_ParcelPageOne,
    enPkSc_JunDaiSel_ParcelPageOne,
    enPkSc_PwUpBaseSel_ParcelPageOne,
    enPkSc_PwUpBase_ParcelPageOne,
    enPkSc_PwUpFood_ParcelPageOne,
    enPkSc_PwUpOver_ParcelPageOne,
    enPkSc_EvoSel_ParcelPageOne,
    enPkSc_Evo_ParcelPageOne,
    enPkSc_EvoOver_ParcelPageOne,
    enPkSc_Sell_ParcelPageOne,
    enPkSc_PackInf_ParcelPageOne
};

enum enPackReqType_ParcelPageOne{
    enPackReqType_Null_ParcelPageOne,
    enPackReqType_PwUp_ParcelPageOne,
    enPackReqType_PwUpAllCards_ParcelPageOne,
    enPackReqType_Evolution_ParcelPageOne,
    enPackReqType_SEVO_ParcelPageOne,
    enPackReqType_Wake_ParcelPageOne,
    enPackReqType_EvoAllCards_ParcelPageOne,
    enPackReqType_NewbieInf_ParcelPageOne,
    enPackReqType_NewbiePwUp_ParcelPageOne,
    enPackReqType_TeamChg_ParcelPageOne,
    enPackReqType_NiePan_ParcelPageOne,
    enPackReqType_GiveWuJiang_ParcelPageOne,
    enPackReqType_GiveWuJiangAllCards_ParcelPageOne,
    enPackReqType_card_update_ParcelPageOne,
    
    
    
    enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne,
    enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne,
    enPackReqType_card_update_ZhuanSheng_ParcelPageOne,
};


class TextNode;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ParcelPageOne : public CUiLayerBase
{
    TitleBar*       m_spTitle;
    CCMenuItem*     m_miBack;
    CCMenuItem*     m_JunDaiItem;
    
    CListLayer*     m_listMain;
    
    Package*        m_package;
    TeamInf*        m_teamInf;
    CTeamInfoScrollViewLayer    *m_pTeamInfoScrollViewLayer;
    
    PwUpBase*       m_pwUpBase;
    Evolution*      m_evolution;
    EvoOverShow*    m_evoOvrShow;
    
    CommDlg*        m_commDlg;
    
    bool            m_bFromHome;
    
    int             m_curScreen;
    
    int             m_iTeamMemberSel;
    
    int             m_iReqType;
    stUid           m_teamInfoUId[5];
    int             m_PreCurTeamIdx;
    
    CCSprite* m_pwUpCardSp;
    CCParticleStar *m_pwUpStars;
    CCSprite *m_pwUpStoneSp;
    CCSprite *m_pwUpTypeSp;
    CCSprite *m_pwUpLightSp;
    CCAnimate *m_pwUpAction;
    CCMenuItem *m_rangeBtn;
    TextNode *rangeLabel;
    TextNode *m_lbRangeTitle;
    TextNode *m_lbRangeTitleBg;
    
    CCLayer*        m_SortBtnLayer;
    
    int            m_iEvoType;      // 进化类型
    
    int iLookY;
    float arraypackageY[enPkLkTp_Max];
    
    bool            m_bNiePan;
    
    short           m_NiePanBaseCid;    // 涅槃前武将的cid
    
public:
    
    float fDownLimitY;
    int                 m_iPreThisPosY;
    CCSize          m_cardSize;
    int             m_iGap;
    int             m_iColumn;
    CCPoint         m_ptStart;
    float           m_fUpLimitY;
//    CScrollBar*     m_pScrollBar;
    float m_lastYm;
    
    
    

    void NtyGoItem();
    void NtyGoChild();
    
    bool m_choiceChild;
    
    
    void moveY(float dt);
    void setThisPosition(const CCPoint& pt, bool bRedraw = false);
    void inertiaMove(float dt);
    
    bool            m_bTouchMvFlg;
    
    CCPoint         m_ptStartNode;
    CCPoint         m_ptStartGl;

    CCPoint         last_moveTouch;

    virtual void visit();
    

    
    void cbTwoBack();
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    ParcelPageOne(CCObject* target, SEL_CallFuncO selector);
    
    
    void newbie(CCObject* pObj);
    
    void newbie2(CCObject* pObj);
    
    
    bool newBool;
    
    void newbieEnd(CCObject* pObj);
    
    virtual void enter();
    virtual void exit();
    virtual void reset();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void innerEnter();
    void innerExit();
    
    void cbList(CCObject*);
    
    void update(ccTime dt);
    
    void enterPwUpSel();
    void enterPwUpBase();
    void enterPwUpFood();
    void enterNiePanFood();
    void enterPwUpOver();
    void showPwUpEffect();
    void showNiePanEffect();
    void showPwUpLightEffect();
    void hidePwUpLightEffect();
    void showPwUpStone();
    void hidePwUpStone();
    void showPwUpType();
    void showPwUpStars();
    void hidePwUpStars();
    
    void enterEvoSel();
    void enterEvo(int type); //0转生 1 觉醒 2超转生
    void enterEvoOver();
    
    void enterCardSell();
    void enterTeamInf();
    
    void beginPwUp();
    void beginNiePan();
    void setRangeBtnIsVisble(bool isShow);
    void setRangeTypeString();
    
    void enterSuiShenJunDai();
    
    void setIsFromHome(bool isFromHome);
    CCLayer * getTeamInfoScrollViewLayer();
    
private:
    void enterPackage(int iLookTp);
    bool isCommitTeamInfo();
    void saveTeamInfo();
    
    ItemCardLayer* m_itemCardLayer;
    void cbItemCardLayer(CCObject* pSender);
    
    void btnCallback(CCObject* sender);
    void jdTitleBtn(CCObject* sender);
    void showItemCard();
    void showChildCard();
    
    void cbCommDlg(CCObject* );
    void cbTeamInf(CCObject* );
    void cbPackage(CCObject* );
    void cbPwUpBase(CCObject* );
    void cbEvolution(CCObject* obj);
    void cbEvoOver();
    void cbChangePackRange();
    void dl2CallBack(CCObject *);
    void initNewbieArrow();
    void shareToWeibo();
    // yang share sanguo
    void shareSanGuo();
    
    CCActionInterval* getActionFromPlist(const char* filename);
    CCActionInterval* getActionFromPlist(const char* filename, bool isReverse);
    CCActionInterval* getActionFromFile(const char* filename);
    
    void onSortBtnLayerExit();
    void onSortBtnTouch(CCObject* sender);
};

#endif /* defined(__AgainstWar__ParcelPageOne__) */
