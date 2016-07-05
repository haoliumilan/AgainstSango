//
//  FriendMain.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_FriendMain_h
#define AgainstWar_FriendMain_h

#include "cocos2d.h"
#include "CGamedata.h"
#include "CFriendInfoLayer.h"

using namespace cocos2d;

class CListLayer;
class CScrollBar;
class FightMngr;
class CConfirmLayer;
class CShowCardInf;
class CashInLayer;
class CSeachFriend;
class CFriendInfoLayer;
class CommDlg;
class TitleBar;
class CMenuBtn;
class NoFriendLayer;
class FriendInvitationLayer;
class TextNode;
class CConfirmLayer;
class MailListLayer;
class TitleList;
class TitleItem;

enum  enFriendScreenDef{
    enFS_recentFr = 1,
    enFS_cmfLayer,
    enFS_searchFr ,
    enFS_friendIf,
    enFS_inviteFr,
    enFM_mailList,
    enFS_main,
    enFS_titlelist,
    enFS_maa,
    
};


enum enFrdReqType{
    enFrdReqType_Null,
    enFrdReqType_FriendList,
    enFrdReqType_InviteList,
    enFrdReqType_Accept,
    enFrdReqType_Refuse,
    enFrdReqType_Delete,
    enFrdReqType_Search,
    enFrdReqType_AddFr,
    enFrdReqType_SendMail,
    enFrdReqType_TitleList,
    enFrdReqType_setTitleName,
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class FriendMain : public CUiLayerBase
{
public:
    FriendMain();
    
    virtual void enter();
    virtual void exit();

    void enterMailListFromFirst();      // 从首页直接到邮件列表
private:
    int             m_iReqType;
    int             m_curScreen;
    CSeachFriend *  m_searchFriendBoard;
    CFriendInfoLayer *m_friendInfoLayer;
    FriendInvitationLayer *m_friendInviationLayer;
    TitleBar *      m_spTitle;
    CCMenuItem*     m_backItem;
    CCMenuItem*     m_titleItem;  // 称号系统按钮
    CListLayer *    m_list;
    CListLayer *    m_friendList;
    CListLayer *    m_inviteList;
    CShowCardInf *  m_showCard;
    CommDlg*        m_commDlg;
    CMenuBtn*       m_frInviteBtn;
    CMenuBtn*       m_frMailBtn;
    NoFriendLayer*  m_nofriendLayer;
    CCSprite *   m_frNumBg;
    
    CCMenu  *m_rangeMenu;
    TextNode *m_rangeLabel;
    int  m_frRange;
    
    int m_acceptFriendTag;
    int m_listTag;
   
    CCMutableArray<CFriendInf*> *m_friendArray;
    CConfirmLayer* m_cfmLayer;
    CCLayerColor* m_lcCover;
    string            m_mailText;
    
    MailListLayer*      m_pMailList;
    
    TitleList*      m_pTtlList;
    string          m_strCurTtlId;
    CCLayer*        m_SortBtnLayer;
    
//    enTeaStuTypeDef     m_teaStuType;
private:
    void innerExit();
    void innerEnter();
    
    void showFriendMain();
    
    void cbList(CCObject* pObj);
    void cbInviteFriend();
    void cbSearchFriend();
    void cbFriendInfoLayer(CCObject *pObj);
    void cbNofriend();
    void btnCallback(CCObject* sender);
    void cbTitleBtn(CCObject* sender);
    void finishShareInviteCode();
    void cbFinishShareInviteCode();
    void enterFriendInvitation(); // 进入邀请好友界面
    void enterFriendInfo();
    void enterFriendList();
    void enterSearchID();
    void enterMailList();
    void cbMailList();
    void update(ccTime dt);
    void acceptInvite(CCObject *sender);
    
    void reqDeleteFriend();
    void reqAcceptInvite();
    void reqRefuseFriend();
    void reqAddFriend();
    void showFriendInviteList();
    void showFriendList();
    
    void cbCommDlg(CCObject* );
    void cbCfm(CCObject*);
    void showTips();
    
    void dlCallBack(CCObject *sender);
    void cbRangeBtn(CCObject* sender);
    void updateFriendRange();
    void onSortBtnLayerExit();
    void onSortBtnTouch(CCObject* sender);
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void shareSanGuo();
    
    void updateMail(ccTime dt);
    
    void InserItemByIdx(CCObject* pObj);
    
    void showFriendInfoLayer(FriendInfoLayerType type,CFriendInf *friendInfo);

    void enterTtlList();
    void cbEquipTtl(CCObject*);
    void chgTitleName();
    
    void enterGetTeacher();
    void enterTeaStuManger();
    void enterTeaStuRequst();
};

#endif
