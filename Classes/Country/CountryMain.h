//
//  CountryMain.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-24.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CountryMain__
#define __AgainstWar__CountryMain__

#include <iostream>
#include "cocos2d.h"
#include "CGameData.h"
#include "MemberInfoLayer.h"
#include "ItemCardLayer.h"
#include "OrgTeamEdit.h"
#include "ChatRoom.h"
#include "MemberManageLayer.h"
#include "PostReward.h"
#include "BaoKuLayer.h"
#include "RewardRelease.h"
#include "RewardTopMembers.h"

using namespace cocos2d;


#define kCntryRecoverPowerMenu      10001


enum enCountryMainDef {
    enCM_Null = 0,
    enCM_Main,              // user is not one of the country
    enCM_CreateCntry,        
    enCM_JoinCntry,
    enCM_CntryList,
    enCM_SearchCntry,
    enCM_CntryMain,         // user is ont of country
    enCM_CntryRank,
    enCM_SetDeclaratin,
    enCM_CntryManage,
    enCM_CntryDonate,
    enCM_OrgFightList,
    enCM_MemberApply,
    enCM_MemberList,
    enCM_CfmLayer,
    enCM_BuffLvUpList,
    enCM_BuffLvUpDialog,
    enCM_CntryLevelUp,
    enCM_SupplyAssignMain,
    enCM_SupplyAssignList,
    enCM_SupplyAssignDilog,
    enCM_GetWelfare,
    enCM_CntryHelp,
    enCM_SetNameCard,
    enCM_TeamEdit,
    enCM_ChatRoom,
    enCM_MemberManage,
    enCM_JiFengBonusPanel,
    enCM_PostReward,
    enCM_BaoKuItem,
    enCM_FabuXuanShang,
    enCM_FabuXuanZe,
    enCM_RewardMemberList,  // 悬赏成员详情列表
    enCM_RewardTopMembers,
};

enum enCountryReqType {
    enCReqType_Null = 0,
    enCReqType_Create,
    enCReqType_SetDeclaration,
    enCReqType_DeleteCntry,
    enCReqType_UnDeleteCntry,
    enCReqType_QuitCntry,
    enCReqType_Search,
    enCReqType_AllCntryByNewTime,
    enCReqType_AllCntryByOldTime,
    enCReqType_AllCntryByLv,
    enCReqType_Apply,
    enCReqType_Applications,
    enCReqType_AcceptMember,
    enCReqType_RefuseMember,
    enCReqType_MemberList,
    enCReqType_ApplyFriend,
    enCReqType_DeleteMember,
    enCReqType_SetViceleader,
    enCReqType_DeleteViceleader,
    enCReqType_GiveUpLeader,
    enCReqType_SendMail,
    enCReqType_DonateCoin,
    enCReqType_LevelUpBuff,
    enCReqType_LevelUpCntry,
    enCReqType_ExchagneWelfare,
    enCReqType_AllCards,
    enCReqType_CntryDgCfg,
    enCReqType_CntryOpen,
    enCReqType_CntryJoin,
    enCReqType_CntryRank,
    enCReqType_AssignOneSupply, // 分配给一个人
    enCReqType_AssignAllSupply, // 平均分配所有人
    enCReqType_AssignOutSupply, // 按贡献分配所有人
    enCReqType_CntryCurrentInfo,
    enCReqType_ReqRecoverPower,
    enCReqType_UpdateCntryInfo,
    enCReqType_SetNameCard,
    enCReqType_MemberManage,
    enCReqType_GetJiFengRecord,
    enCReqType_GuildItemShop,
    enCReqType_RewardTaskList,
    enCReqType_RewardMemberList,
    enCReqType_RewardTopMember,
    enCReqType_AssignWeekSupply,    //按周贡献分配
};

class TitleBar;
class CListLayer;
class CountryCreate;
class CountryDialog;
class Dialog2;
class PagingListLayer;
class CommDlg;
class CountryDetails;
class OrgFightMngr;
class OrgFightRet;
class OrgFightCfmRet;
class NoFriendLayer;
class CConfirmLayer;
class CShowCardInf;
class GachaGet;
class Dialog1;
class RaidersLayer;
class BonusPanel;

class CountryMain : public CUiLayerBase
{
public:
    CountryMain();
    CountryMain(CCObject* target, SEL_CallFunc selector);
    ~CountryMain();
    
    void init();
    
    virtual void enter();
    virtual void exit();
    virtual void onEnter();
    virtual void onExit();
    
    void enterArenaEnd();   // 账户绑定有效期已过，重新登入后，直接向服务器发结束战场请求
    
    void showWelfareList();      // 兑换福利
    
    void showMainOrNot();       // 国家主界面
    
    void forceExitFight();

private:
    CCObject*           m_pListener;
    SEL_CallFunc        m_pfnSelector;
    
    PostReward*         m_postRwdLayer; //悬赏页
    BaoKuLayer*         m_BaoKuLayer;   //宝库页
    RewardRelease*      m_FaBuLayer;    //发布页
    RewardTopMembers*   m_RwdTopMemLayer; //排序页
    
    
    
    CCSprite*           m_spCntryBg;    // 同盟的背景
    TitleBar*           m_spTitle;
    CCMenuItem*         m_backItem;
    CListLayer*         m_list;
    CListLayer*         m_listCntry;    // 同盟列表
    CCMenu*             m_menuCntryMain;
    CountryCreate*      m_cntryCreate;
    CountryDialog*      m_cntryDialog;   // 国家搜索、修改宣言、捐献元宝、捐献粮饷
    Dialog2*            m_dlgTip; // 提示框(退出公会，解散工会, 兑换福利提示, 回复行动力，发同盟邮件)
    CCSprite*           m_cntryDeclaration; // 国家宣言
    CountryDetails*     m_cntryDetails;
    NoFriendLayer*      m_dlgNoTip; // 没有成员请求提示框
    MemberInfoLayer*    m_memInfoLayer; // 成员信息提示框（接受，拒绝）
    PagingListLayer*    m_memberList;   // 成员列表
    MemberManageLayer*  m_memberManage;
    CConfirmLayer*      m_cfmLayer;     // cfm菜单
    CCLayerColor*       m_lcCover;      // cfm bg
    CShowCardInf*       m_showCard;     // 武将详情
    CListLayer*         m_listWelfare;  // 福利列表
    GachaGet*           m_gachaLayer;    // 求将
    CCMenu*             m_rangeMenu;    // 成员列表排序button
    TextNode*           m_rangeTitle;   // 成员列表排序方式
    Dialog1*            m_dlgRecover;   // 战斗力已满提示
    //TextNode*           m_tnDeclaration;    // 滚动的公告
    CCLabelTTF*           m_tnDeclaration;    // 滚动的公告
    CCLayerColor*       m_lcCntryProBg;         // 国家信息的暗色半透明背景
    RaidersLayer*       m_rlCntryHelp;  // 国家的帮助
    TextNode*           m_tnDeleteTime; // 解散国家倒计时
    int                 m_iDeleteTime;
    
    CCMenu*             m_memberManageBtn; //聊天成员管理按钮
    
    enCountryMainDef    m_curScreen;
    enCountryReqType    m_curReqType;
    CommDlg*            m_commDlg;
    
    OrgTeamEdit*        m_teamEdit;
    
    OrgFightMngr*       m_orgFightMngr;
    
    bool                m_bCashin;
    
    int                 m_fightSel;
    
    int                 m_listSel;     // pagingList select index
    string              m_fightitem;    // 保卫战id
    string              m_fightname;    // 保卫战名称
    string              m_fighttype;    // 保卫战类型
    string              m_mailText;
    bool                m_bShowMemberList;  // 判断获取memberList后，显示memberList or supplyAssignList
    CCMutableArray<CWelfareInfo*>* m_welfareInfoArr;
    
    string              m_strDgRtyId;

    long                m_lEnemyCid;
    
    bool                m_bMailAll;     // 是否是发同盟邮件
    
    int                 m_preStatus;    // 记录玩家同盟中的身份
    
    bool                m_bBackToFightRank;
    
    BonusPanel*         m_pJiFengBonusPanel;
    string              m_rewardTaskId; // 选中的悬赏Id
    
    int                 m_pLocal;       //记录分配粮饷后的y轴坐标
    
    bool                m_bIsLS;       //发送的请求是否是同盟练手场里的
private:
    void cbList(CCObject* pObj);
    void btnRecoverPower(CCObject* pObj);
    void btnCallback(CCObject* pObj);
    
    
    void innerEnter();
    void innerExit();

    void showMain();            // 国家主界面 user is not one of the country
    void showCountryMain();     // 国家主界面 user is one of the country

    void cbTeamEdit();
    void showTeamEdit();
    
    void showChatRoom();
    void cbChatRoom();
    
    void dl2CallBack(CCObject *sender);
    void showCreateCntryTip();  // 创建国家的提示
    void showCreateCountry();   // 创建国家
    
    void showJoinCountry();     // 加入国家
    void showSearchCountry();   // 搜索国家
    
    void showCountryList();     // 国家列表
    void showAllCntryInfo();
    void insertItemByIdx(CCObject* pObj);
    void cbCountryList(CCObject* pObj);
    void showOneCntryDetails(CCntryBaseInfo* cntryInfo);
    void cbOneCntryDetails(CCObject* pObj);
    
    void showSetDeclaration();  // 修改宣言
    
    void showCountryManage();   // 内政管理
    void memberManageMenu();    // 会员的内政menu
    void commandManageMenu();   // 会长的内政menu
    void viceCmndManageMenu();  // 副会长的内政menu
    
    void showMailAll();         // 发同盟邮件
    void showMailAllCoinTips();  // 发同盟邮件需要元宝提示
    void cbMailAllCoinTips(CCObject* pObj);
    void showMailAllNoCoinTips();   // 发同盟邮件元宝不足提示
    void cbMailAllNoCoinTips(CCObject* pObj);
    void showMailAllController();
    void showAlertNoGold();
    void cbAlertNoGold();
    
    void showCountryDonate();   // 国家捐献    
    
    void cbItemCardLayer(CCObject *pSender);
    void showItemCard();
    
    void cbWelfareList(CCObject* pObj);
    void showExchangeTip();
    void cbExchangeTip(CCObject* pObj);
    void showInCdTimeTip();    // ÊèêÁ§∫Á¶èÂà©ËøòÂú®cd‰∏≠ÔºåÊó†Ê≥ïÂÖëÊç¢
    void cbInCdTimeTip();
    void showGacha();
    void cbGacha();
    
    void showSupplyAssignMain();
    void showSupplyAssignList();    // 福利分配
    void cbSupplyAssignList(CCObject* pObj);
    void showSupplyAssignOneDilog();
    void showSupplyAssignAllDialog();
    void showSupplyAssignOutDialog();
    void showSupplyAssignWeekDialog();
    
  
    void showOrgFightList();    // 国家保卫战Lists
    void cbOrgFightList(CCObject* pObj);
    void showSureOpenOrgFight();    // 确认开启某个保卫战
    void cbSureOpenOrgFight(CCObject* pObj);
    void showNotOpenOrgFight();     // 提示保卫战不在有效时间无法开启
    void cbNotOpenOrgFight();
    void showAcPointNotEnoughTip();    // 提示行动点数不足
    void cbAcPointNotEnoughTip(CCObject* pObj);
    void enterOrgFight();       // 进入国家保卫战
    void FightOpen();           // 点击开启按钮 （开启保卫战）
    void FightRank();           // 点击伤害排行按钮
    void FightJoin();           // 点击参与按钮
    void enterFightCfm();
    
    
    void showAcPointFullTip();  // 提示行动力点数已满
    void cbAcPointFullTip();
    void showRecoverAcPointTip();   // 提示回复行动点数需要的元宝
    void cbRecoverAcPointTip(CCObject* pObj);
    void showNoCoinForRecover();    // 提示回复行动点数元宝不够
    void cbNoCoinForRecover(CCObject* pObj);
    void showDonateSuccess();         // 提示捐献成功
    void cbGotoCountryMain();
    
    void showQuitCountryTip();  // 退出公会提示（member，viceLeader）
    void cbShowQuitCntryTip(CCObject* pObj);
    
    void showDeleteCountryTip();// 解散公会提示 (leader)
    void cbShowDeleteCntryTip(CCObject* pObj);
    
    void showMemberApplyList(); // 会员申请
    void cbMemberApplyList(CCObject* pObj);
    void showFriendInfoLayer(MemberInfoLayerType memInfoLayerType, CFriendInf *friendInfo);
    void cbMemberInfoLayer(CCObject* pObj);
    
    void showMemberManage();//会员管理
    void showMemberList();  // 会员列表
    void cbMemberManage();
    void cbMemberList(CCObject* pObj);
    void showMemberInfoLayer(MemberInfoLayerType memInfoLayerType, CMember *memberInfo);
    void showRangeBtn();
    void changeListRange();
    void showMListRangeTitle();
    
    void showCountryRank();     // 查看排名
    
    void showCountryDetails();  // 国家详情
    void cbCountryDetails(CCObject* bObj);
    
    void showBuffLevelUpList();     // 增益升级
    void cbBuffLevelUpList(CCObject* pObj);
    void showBuffLevelUpDialog();
    
    void showCntryLevelUp();    // 国家升级
    
    void showCntryCurrentInfo();    // 国家近况
    void cbCntryCurrentInfo();
    
    void showDeclaration();    // 滚动的公告
    void scrollDeclaration();
    
    void showCntryPro();        // 国家首页显示属性部分
    
    void showCntryHelp();       // 国家帮助界面
    void showNotCntryHelp();    // 提示不再显示国家帮助
    void cbNotCntryHelp();
    
    void showSetNameCard();     // 修改同盟名片
    void showSetNameCardDlg();  
    void cbSetNameCardDlg();
    void showNameCardCoinTips(); // 修改同盟名片消耗元宝提示（如果不消耗元宝，则不提示）
    void cbNameCardCoinTips(CCObject* pObj);
    void showSureNameCardTips(); // 确定修改同盟名片内容（如果不消耗元宝，则不提示）
    void cbSureNameCardTips(CCObject* pObj);
    
    // request
    void createCountry();
    void setDeclaration();
    void quitCountry();
    void deleteCountry();
    void unDeleteCountry();
    void getMemberApplyList();
    void getAllCntryByNewTime();
    void getAllCntryByOldTime();
    void getAllCntryByLv();
    void applyCountry();
    void acceptMember();
    void refuseMember();
    void getMemberList();
    void applyFriend();
    void deleteMember();
    void setViceleader();
    void deleteViceleader();
    void giveUpLeader();
    void sendMail();
    void searchCountry();
    void donateCoin();
    void levelUpBuff();
    void levelUpCntry();
    void exchangeWelfare();
    void getAllCards();
    void reqCntryDgCfg();
    void assignOneSupply();
    void assignAllSupply();
    void assignOutSupply();
    void assignWeekSupply();
    void getCntryCurrentInfo();
    void recoverActionPoint();
    void updateCntryInfo();
    void setNameCard();
    void getJifengRecord();
    void getRewardMemberList();
    
    void updateDeleteTime(ccTime dt);
    void updateMail(ccTime dt);
    void update(ccTime dt);
    void cbCommDlg(CCObject* );
    void cbNoDlg(CCObject* );
    void showCfmLayer();
    void cbCfm(CCObject* pObj);
    void cancelCfmToMemberList();
    
    void updateLoadUserCardsSprite(ccTime dt);
    
    void newMenuBtn(const char* menuName[], int num);
    void newBtn(CCMenuItem *&menuItem, const char *label, const CCPoint& position,
                int tag, const char *image);
    
    void cbFightCfm(CCObject*);
    void cbFight(CCObject*);
    void cbCashin();
    
    void showBonusPanel();
    
    //悬赏
    void showRewardTopMembers();
    void showPostReward();
    void cbRewardRelease(CCObject* pObj);
    void cbFromPostRwd(CCObject* pObj);
    void cbBaoKuLayer(CCObject* pObj);
    void reqGuildItemShopCfg();
    void BaoKuShow();
    void reqRewardTaskList();
    void showFabuXuanShang();
    void showRewardMemberList();
    void cbNoRewardMember();
    void GoToRewardTopMember();
    void reqRewardTopMembers();
    void cbTopMembers(CCObject* pObj);
};

#endif /* defined(__AgainstWar__CountryMain__) */
