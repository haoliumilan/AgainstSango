//
//  Package.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-25.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_Package_h
#define AgainstWar_Package_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"

class Dialog1;
using namespace cocos2d;

class TextNode;
class CUserCardInfo;
class CScrollBar;
class CShowCardInf;
class CConfirmLayer;
class PackSelBtn;
class Dialog2;
class CommDlg;
class PackCard;
class CScrollViewLayer;

enum enPkLookTpDef {
    enPkLkTp_JustLook,
    enPkLkTp_TeamEditLeader,
    enPkLkTp_TeamEditSub,
    enPkLkTp_PwUpBase,
    enPkLkTp_PwUpFood,
    enPkLkTp_NiePanFood,
    enPkLkTp_Evolution,
    enPkLkTp_Sell,
    enPkLkTp_TeamReset,
    enPkLkTp_Max,
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CardSelRet : public CCObject
{
public:
    stUid   uid[5];
    bool    bNiePan;
    int     type;        //0转生 1觉醒 2 究极进化
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Package : public CCLayer
{
    friend class CScrollViewLayer;
    CScrollBar*     m_pScrollBar;
    CCSprite*       m_spMaxCard;
    TextNode*       m_lbMaxCard;
    
    CShowCardInf*   m_showCard;
    CConfirmLayer*  m_cfmLayer;
    CCLayerColor*   m_lcCover;
    PackSelBtn*     m_packSelBtn;
    Dialog2*        m_niepanDlg;
    Dialog2*        m_sellDlg;
    Dialog2*        m_sellUpThreeStarsDlg;
    CommDlg*        m_commDlg;
    CScrollViewLayer    *m_pScrollViewLayer;
    
    CCSize          m_cardSize;
    int             m_iGap;
    int             m_iColumn;
    
    
    int             m_NewCardID;
    CCPoint         m_ptStart;
    int             m_iRmBtn;
    
    bool            m_bShowDelBtn;
    
    int             m_iSelNo;   // 记录选中的卡片序数
    int             m_iSelCnt;  // 选中卡片的数量
    int             m_iLookTp;
    
    int             m_iBaseCardCType;
    
    bool            m_bTouchMvFlg;
    CCPoint         m_ptStartTouch;
    
    float           m_fUpLimitY;
    
    CCMutableArray<CUserCardInfo*> *m_pUserCardArr;
    CCMutableArray<PackCard *> *m_packCardArr;
    CUserCardInfo*  m_pUsrCardSel;
    int             m_usrCardNum;

    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CardSelRet      m_ret;
    
    int             m_iReqType;
    float           m_lastXm;
    float           m_lastYm;
    CCPoint         last_moveTouch;
    
    int             testCount;
    CCPoint         m_dlgPoint;
    
    int             m_iPreThisPosY;
    //yang--
    CCPoint         arrowpoint;
    
    bool            m_bCmfFourBtn;
    
    // 编队重置
    string          m_iGeneral[5];
    int             m_iTotalCost;
    int             m_iCurCost;
    Dialog1*        m_noMainGeneralDlg;
    
    CCSprite *FiveTeamLayer;
    CCMenu* FiveTeamBGMenu;
    CCMenu* FiveTeamMenu;
    CCSprite* FiveTeam1;
    CCSprite* FiveTeam2;
    CCSprite* FiveTeam3;
    CCSprite* FiveTeam4;
    CCSprite* FiveTeam5;
    
public:
    static int getCardTagBase();
    static int getTickCount();
    Package(int iLookTp, CCObject* target, SEL_CallFuncO selector, int costCanUse = 0);
    // 当调整编队时，costCanUse新添加Card的最大cost；其他时候不必赋值
    
    void FiveTeamMemberSelected(string ucid);
    void checkCostEnough();
    void refreshFiveTeamMember();
    void cbFiveTeamLayerBG(CCObject* sender);
    void cbFiveTeamLayerReset(CCObject* sender);
    void cbFiveTeamLayerSure(CCObject* sender);
    void resetIGeneral();
    void cbMainGeneralDlg();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void update(ccTime dt);
    
    bool goback();
    
    virtual void onExit();

    void rangePackageCardWithType(enCardRangeType type);
    void changePackgeCardRange();
    int  getRangeType();
    
    void setThisPosition(const CCPoint& pos, bool bRedraw = false);
    
    void NtyGoTop(CCObject* obj);
    void NtyGoBottom(CCObject* obj);
    bool isShowScrollViewLayer();
    
    void newbie(CCObject* pObj);
    void newbie2(CCObject* pObj);
    void newbie3(CCObject* pObj);
    
    void newbie4(CCObject* pObj);

    
private:
    void cbCommDlg(CCObject* );
    void cbCfm(CCObject*);
    void cbSellDlg(CCObject*);
    void cbSellUpThreeStarsDlg(CCObject*);
    void initButton();
    void initCapacity(char*);
    void redrawCapacity();
    bool checkBtn(const CCPoint& pt);
    void setAllCardGray(bool bGray);
    void setCardGrayByCid(short cid, bool bGray);
    void inertiaMove(float dt);
    void moveY(float dt);
    void showDlgIsSell();
    void showDlgSellUpThreeStars();
    void shareToWeibo();
    void showDlgNiePan();
    void cbDlgNiePan(CCObject* pObj);
    void ccTouchEndedLookTpBtnSel();
    
    // yang share sanguo
    void shareSanGuo();

    //长按显示详细信息
    void waitShowInfo();
    CCPoint beganPoint;
    bool isLongPress;
    
    void updateScreenWithCardCntChg();
    void thisPositionX(PackCard *pPackCard,bool bGo);
    
    void tickTimeCardText(ccTime dt);
    
};

#endif
