//
//  CGeneralLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CGeneralLayer_h
#define AgainstWar_CGeneralLayer_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "ItemCardLayer.h"
#include "Package.h"

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

enum enPackScreenDef{
    enPkSc_Null,
    enPkSc_TeamInf,
    enPkSc_JunDaiSel,
    enPkSc_PwUpBaseSel,
    enPkSc_PwUpBase,
    enPkSc_PwUpFood,
    enPkSc_PwUpOver,
    enPkSc_EvoSel,
    enPkSc_Evo,
    enPkSc_EvoOver,
    enPkSc_Sell,
    enPkSc_PackInf,
};

enum enPackReqType{
    enPackReqType_Null,
    enPackReqType_PwUp,
    enPackReqType_PwUpAllCards,
    enPackReqType_Evolution,
    enPackReqType_SEVO,
    enPackReqType_Wake,
    enPackReqType_EvoAllCards,
    enPackReqType_NewbieInf,
    enPackReqType_NewbiePwUp,
    enPackReqType_TeamChg,
    enPackReqType_NiePan,
    enPackReqType_GiveWuJiang,
    enPackReqType_GiveWuJiangAllCards,
    enPackReqType_card_update,
    
    
    
    enPackReqType_GiveWuJiang_ZhuanSheng,
    enPackReqType_GiveWuJiangAllCards_ZhuanSheng,
    enPackReqType_card_update_ZhuanSheng,
};


class TextNode;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CGeneralLayer : public CUiLayerBase
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
    
    
    CGeneralLayer();
    
    
    void newbie(CCObject* pObj);
    
    void newbie2(CCObject* pObj);

    
    bool newBool;
    
    void newbieEnd(CCObject* pObj);
    
    virtual void enter();
    virtual void exit();
    virtual void reset();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
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

#endif
