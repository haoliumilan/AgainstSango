//
//  TeamCfm.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_TeamCfm_h
#define AgainstWar_TeamCfm_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGameData.h"

using namespace cocos2d;


class CShowCardInf;
class TextNode;
class TeamCfmCard;
class CommDlg;
class Dialog2;

enum enRentCardsStat{
    enRentCards_Null,   //不需要租卡
    enRentCards_Need,   //需要租卡
    enRentCards_Rent,   //已经租到
};

enum enReqType
{
    en_req_team_none,
    en_req_bind_sina,
    en_req_bind_qq,
};

#define OFFXTEMAINFO  114.0f

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TeamCfm : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    CShowCardInf*   m_showCard;
    int             m_iCurSel;
    int             m_iCardCnt;
    
    bool            m_bMvFlg;
    CCPoint         m_ptTouchBegan;
    TeamCfmCard*    m_pCardHold;
    
    TextNode*       m_txtSkName;
    TextNode*       m_txtSkDsp;
    TextNode*       m_txtHelperSkName;
    TextNode*       m_txtHelperSkDsp;
    TextNode*       m_text_sure_btn;
    
    int             m_iRentCardsStat;
    bool            m_bNeedSpecialCards;
    long            m_lCardsCoin;
    long            m_lNeedPayTotal;
    int             m_iNeedRentCnt;
    int             m_iSpecDgTimes;
    string          m_strNeedCardsName;
    string          m_arrDgNeedCards[6];
    string          m_arrDgHelpCards[6];
    string          m_arrMyNeedCards[6];
    
    CCLayer*        m_teamLayer;
    
    stDgRestrict    m_dgRstrct;
    string          m_strDgRstrct;
    
    CommDlg*        m_commDlg;
    enReqType       m_iReqType;
    Dialog2*        m_dialog2;
    Dialog2*        m_dialogWarning;
    
    int             m_floorid;
    int             m_floorindex;
    int             m_roomid;
    char            m_type[16];
    
    CCMenuItemImage* m_menuitem_record;
    
    CCSprite*        pCIcon;
    CCSprite*       m_countryPorpertyTip;
    TextNode*       m_tcountrydetail;
    int             m_IconIndex;
    
    int             m_iFriend[kHelperMaxCount];
    
public:
    TeamCfm(CCObject* target, SEL_CallFunc selector, int iFloorId, int iRoomId, const char* szDgType, int *iFriend);
    CUserCardInfo* getHelperCardInfo(CFriendHelpInfo *helperInfo);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void onEnter();
    virtual void onExit();
    
private:

    void setTipsDetail();
    void setTips();
    
    
    void showFightTeam(int teamIdx);
    
    void menuCallBack(CCObject *pSender);
    void CallBackBind(CCObject* _obj);
    void checkCurSel(CCPoint& pos);
    
    void chkNeedCardsStat();
    bool lookupCidInString(int cid, string& str);
    
    void chkLeaderPos();
    void updateAutoSkill(ccTime dt);
    void update(ccTime ct);
    void ShowAlertMenory();
    void BeginRecord(CCObject* obj);
    void cbCommDlg(CCObject* pObj);
    
    void showNeedRentDlg();
    void cbNeedRentDlg(CCObject *pSender);
    
    void rentCards();
    
    bool isTeamPass();
    void showRentCardsNotPlacedDlg();
    void cbRentCardsNotPlacedDlg();
    
    void writeToTmpTeam();
    
    void notifyAppWillResignActive(CCObject* obj);
    
    void chkDgRstrct();
    void showRstrctDlg();
    void cbRstrctDlg(CCObject *pSender);
    
    void cbUpto10(CCObject* obj);
    
    
};

#endif
