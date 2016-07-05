//
//  CArenaLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CArenaLayer_h
#define AgainstWar_CArenaLayer_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "FriendMain.h"
#include "Dialog1.h"
#include "map.h"

using namespace cocos2d;

class CListLayer;
struct CListLayerModel;
class CScrollBar;
class FightMngr;
class CConfirmLayer;
class CShowCardInf;
class CashInLayer;
class AddFriend;
class Dialog2;
class CommDlg;
class TitleBar;
class CUserInfo;
class TeamCfm;
class Dialog3;

enum enArenaLayerDef{
    enArenaLayer_NmlFloor,
    enArenaLayer_NmlRoom,
    enArenaLayer_SpecialFloor,
    enArenaLayer_SpecialRoom,
    enArenaLayer_SangoFloor,
    enArenaLayer_SangoRoom,
    enArenaLayer_Cfm,
    enArenaLayer_ShowInf,
    enArenaLayer_NmlList,
    enArenaLayer_SpecialList,
    enArenaLayer_SangoList,
    enArenaLayer_HelperList,
    enArenaLayer_RoomList
    
};

enum enArenaReqType{
    enArenaReqType_Null,
    enArenaReqType_helper,
    enArenaReqType_dungeon,
    enArenaReqType_dgend,
    enArenaReqType_dgendforfanti,//返体
    enArenaReqType_allcards,
    enArenaReqType_addfriend,
    enArenaReqType_stamina,
    enArenaReqType_newbie2,
    enArenaReqType_newbie3,
    enArenaReqType_upload_video,
    enArenaReqType_req_view_other_record_vid,
    enArenaReqType_getfightcfg,
    
    enArenaReqType_cardAdd,
    
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CArenaLayer : public CUiLayerBase
{
    TitleBar*       m_spTitle;
    
    CCMenuItem*     m_miBack;
    CCMenuItem*     m_miSpecial;
    CCMenuItem*     m_miNormal;
    CCMenuItem*     m_miSango;
    CCMenuItemImage*     m_view_other_record;
    CListLayer*     m_listSpecial;
    CListLayer*     m_listNml;
    CListLayer*     m_listSango;
    CListLayer*     m_listRoom;
    CListLayer*     m_listHelper;
    CConfirmLayer*  m_cfmLayer;
    CShowCardInf*   m_showCard;
    AddFriend*      m_addFriend;
    
    CashInLayer*    m_cashin;
    
    Dialog2*        m_proposeLvDialog;
    Dialog2*        m_staminaDlg;
    Dialog2*        m_goShopDlg;
    Dialog1*        m_less5Dlg;
    Dialog1*        m_dialog1;
    CommDlg*        m_commDlg;
    Dialog3*        m_upload_or_not;
    
    TeamCfm*        m_teamCfm;
    
    Dialog1*        m_getCfgAlert;
    Dialog1*        m_getFantiAlert;//返体的提示框
    
    
    int             m_iFloorSel;
    int             m_iRoomSel;
    int             m_iFloorId;
    int             m_iRoomId;
    int             m_iFriend[kHelperMaxCount];
    int             m_iCurrentFriend;
    const char*     m_szDgType;
    char            m_floorName[200];
    char            m_roomName[200];
    
    
    enArenaLayerDef m_curLayer;
    enArenaLayerDef m_preLayer;
    
    enArenaLayerDef m_recordCurLayer;
    
    enArenaReqType  m_curReqType;
    
    FightMngr*      m_fightMngr;
    
    CCLayerColor*   m_lcCover;
    
    int             m_fightSceneType;
    unsigned short  m_ushLimitMaxStar;
    char            m_dungeonUid[32];
    char            m_helperUid[32];
    
    CCSprite        *newSpecialRoomLogo;
    bool            isJustLeaveFight;
    
    long            m_lRtyID;
    int  preLV;
    int  preCost;
    int  preStamina;
    bool m_isAddFloorNameImage;  // 记录是否已经加载floor，room的name大图
    
    TextNode*     m_frInfoLabel;
    TextNode*   m_lbRangeTitle;
    TextNode*   m_lbRangeTitleBg;
    
    CCMenu  *m_rangeMenu;
    TextNode *rangeLabel;
    int  m_frRange;
    CCMutableArray<CFriendHelpInfo*> * m_pHelperArr;
    CUserCardInfo* getHelperCardInfo(CFriendHelpInfo *helperInfo);
    int m_iSelectedHelper;
    CUserCardInfo m_helperCardInfo;
    
    void InserItemByIdx(CCObject* pObj);
    enFriendRange   m_FriendRange;
    
    bool  buff_void;              //是否使用同盟buff
    
    map<int, CListLayerModel*> m_fPositionMap;
    bool isEnter;
    
    CCSprite *FiveTeamLayer;
    CCMenu* FiveTeamBGMenu;
    CCMenu* FiveTeamMenu;
    CCSprite* FiveTeam1;
    CCSprite* FiveTeam2;
    CCSprite* FiveTeam3;
    CCSprite* FiveTeam4;
    CCSprite* FiveTeam5;
    
    CCLayer*        m_SortBtnLayer;
    
public:
    CArenaLayer();
    ~CArenaLayer();

    void update(ccTime dt);
    
    virtual void enter();
    virtual void exit();
    virtual void reset();
    
    bool getIsJustLeaveFight();
    void setIsJustLeaveFight(bool agu);
    
    void enterArenaEnd();   // 账户绑定有效期已过，重新登入后，直接向服务器发结束战场请求
    
    void enterSpecialArena();   // 从FirstLayer直接进入SpecialAnrena
    unsigned short getArenaLimitMaxStar()const{return m_ushLimitMaxStar;}
    
    void forceExitFight();
    
    void newbie(CCObject* pObj);
    
    void newbie1(CCObject* pObj);

    
private:
    void innerExit();
    
    void goDirectRoom();
    
    void showGetConfigAlert();
    
    void enterNmlList();
    void enterNmlFloor();
    void enterNmlRoom();
    void enterSpecialList();
    void enterSpecialFloor();
    void enterSpecialRoom();
    void enterSangoFloor();
    void enterSangoList();
    void enterSangoRoom();
    void enterHelperList();
    void enterArena();
    void enterValidArena();
    
    void enterProposeLvDialog();
    
    void reqEnterArena(long lRty);
    
    void showCfmLayer();
    void showFiveTeamCfmLayer();
    void showCashinLayer();
    void showTeamCfm();
    
    void  cbProposeLvDlg(CCObject*);
    void  cbCommDlg(CCObject*);
    void  cbStaminaDlg(CCObject*);
    void  cbHasSameCardDlg(CCObject*);
    void  cbLess5Dlg();
    void  cbList(CCObject*);
    void  cbFight(CCObject*);
    void  cbCfm(CCObject*);
    void  cbCashin();
    void  cbAddFriend(CCObject*);
    void  cbGoShop(CCObject*);
    void  btnCallback(CCObject* sender);
    void  MCurLayer();
    void  cbRangeBtn(CCObject* sender);
    void  cbTeamCfm();
    void  FiveTeamMemberSelected(int number);
    void  refreshFiveTeamMember();
    void  cbFiveTeamLayerBG(CCObject* sender);
    void  cbFiveTeamLayerReset(CCObject* sender);
    void  cbFiveTeamLayerSure(CCObject* sender);
    void  addFloorNameImage();    // 加载floor的name大图
    void  removeFloorNameImage(); // 释放floor的name大图
    
    void changeHelperListRange(bool isUpdate = false);//修改好友排序// 记录是否要更新义军，每次进入友军列表的时候更新义军
    
    void onSortBtnLayerExit();
    void onSortBtnTouch(CCObject* sender);
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void createHelperList(enFriendRange type);
    void InserListHelpByIdx(CCObject* pObj);
    
    void InsertListSangoByIdx(CCObject* pObj);
    
    CSpecialDgInfo* getSpecialDgInfo();
    
    int getSeciaSelByFloorID(int ifloorid);
    
    void cbUpload(CCObject* obj);
    
    void SetViewOtherRecord();
    void ResetViewOtherRecordBtn(bool on_off);
    void ResetViewOtherRecordBtn();
    void cbViewOtherRecord(CCObject* obj);
    void RequestUrl();
    void CallBackRemoveD2();
    void cbGetConfigAlert();
    
    void cbGetFantiAlert();//返体
    
    void resetIFriend();        // 重置 m_iFriend[]

};

#endif
