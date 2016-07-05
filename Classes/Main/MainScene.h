#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "FightMngr.h"
//#include "CNotice.h"


using namespace cocos2d;

class CUsrInfoPanel;
class CBottomPanel;
class CGeneralLayer;
class CArenaLayer;
class GachaMain;
class OtherMain;
class ShopMain;
class FriendMain;
class CLoginLayer;
class NewbieLayer;
class CommDlg;
class Dialog2;
class CTimerLayer;
class PublishWeiboLayer;
class PublishWeiboLayerRet;
class CashInLayer;
class TextNode;
class CNotice;
class CountryMain;
class FirstLayer;
class RewardMain;
class ParcelPageOne;
class ParcelBackPage;
class ParcelPageTwo;


enum enBackgroundMusic{
    enBgMusicNml,
    enBgMusicBattle,
    enBgMusicBoss,
    enBgMusicOrgFight,
    enBgMusicOrgFail,
    enBgMusicOrgWin,
    enBgMusicOrgEnter,
    enBgMusicQianLiBg,
};

enum enMainReqType{
    enMainReqType_Null,
    enMainReqType_Index,
    enMainReqType_Login,
    enMainReqType_Cfg,
    enMainReqType_Card,
    enMainReqType_Monster,
    enMainReqType_Newbie5,
    enMainReqType_Shop,
    enMainReqType_RcvMail,
    enMainReqType_Unknow,
    enMainReqType_FromSL,
    enMainReqType_ReadMzsm,
    enMainReqType_Newbie4,
};


class ParcelSelRet : public CCObject
{
public:
    int iBtnSel;
    int iType;
    
};


class MainScene : public cocos2d::CCLayer
{ 
    static MainScene* s_pcIf;

    CCMenuItem* item1;
   

    int             m_iNewbieStep;
    int             m_iReqType;
    CommDlg*        m_commDlg;

    CUsrInfoPanel*  m_pUsrInfoPanel;
    CBottomPanel*   m_pBottomPanel;

    CTimerLayer*    m_pTimerLayer;
    NewbieLayer*    m_newbie;
    CLoginLayer*    m_pLoginLayer;
    CUiLayerBase*   m_curLayer;
    CUiLayerBase*   m_nxtLayer;
    FirstLayer*     m_pFirstLayer;
    CArenaLayer*    m_pArenaLayer;
    CGeneralLayer*  m_pGeneralLayer;
    GachaMain*      m_pGachaMain;
    OtherMain*      m_pOtherMain;
    ShopMain*       m_pShopMain;
    CountryMain*    m_pCountryMain;
    FriendMain*     m_pFriendMain;
    RewardMain*     m_pRewardMain;
    
    ParcelPageOne*  m_pParcelPageOne;
    
    
    CNotice*        m_notice; 
    Dialog2*        m_fullPackDlg;
    Dialog2*        m_fullCostDlg;
    Dialog2*        m_foodInTeamDlg;
    Dialog2*        m_foodUpThreeStarsDlg;
    PublishWeiboLayer*   m_publishWeiboLayer;
    bool isGfNoticeShow;
    bool isLINoticeShow;
    bool isIVNoticeShow;
    bool isClaimNoticeShow;
    
    unsigned long   m_tipsTime;
    
    

    int m_reloginWeiboType;   // 账户绑定有效期已过，重新登入微博类型，0 = 默认；1 = 结束战场请求；2 = 结束保卫战请求；3 = 重新登入请求；4 = 战场里请求；
    Dialog2*        m_reloginDlg;
    
    //-------------------------底部的tips公告--------------------------//
    vector<string>m_tipVector;
    vector<string>m_tipOneTimeVector;
    int             m_Itip_index;
    string          m_tips_string;
    
public:
    
    ParcelBackPage*  m_pParcelBackPage;
    ParcelPageTwo* m_pParcelPageTwo;
    
    bool new_CardAdd;
    
    bool new_MyNew;
    
    
    bool new_FirstItem;
    bool new_FirstItem_Second;
    
    bool new_FirstZhuanSheng;
    bool new_FirstZhuanSheng_Second;

    
    bool new_FirstPowerUp;
    
    bool new_FirstPowerUpSeeYuXi;
    
    bool new_FirstPowerUpSecond;

    
    
    CCSprite*       m_spTouchTip; // 点击继续的提示
    
    int             m_other_Gacha;
    
    static MainScene* Inst();
    
    MainScene();
    void titleNoAction();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();

    void nmlStart();
    
    void loginFromSL();
    
    void reLogin();
    void updateReLogin(); // 通过计时器确保一定会调用重新登入接口,因为当重新登入的时候,有可能正在访问网络
    
    bool getBottomPanelIsEnableOrNot();
    
    void showBottomPanel(bool bShow);
    void showUsrInfPanel(bool bShow);
    void cnInviteAwardCallBack();
    void cnCallBack();
    void cnGiftCallBack();
    void cnLendInfoCallBack();
    void dl2CallBack(CCObject *pSender);
    void enableBottomPanel(bool bEnable);
    
    
    void setTitle(int iNo, int iStar, int maxStar, char* name, int category, int subCategory);
    const char* getCategoryChar(int category);
    
    void switchBgMusic(int idx);
    void loginFinished();
    void newbieLogin();
    void newbieFightEnd();
    void goToGeneral();
    
    void loadingCardRes(ccTime dt);
    
     void waitPPLogout(ccTime dt);

    void update(ccTime dt);
    void showTips();
    void showCntryTips(bool clearStatus = false);
    void showAwardTips();           //显示领奖提示
    void showNoticeInviteAward();
    void showNotice();
    void showGiftNotice();
    void showLendInfoNotice();
    void goToShop();
    void cbAlipay(int payType);                //支付宝付款成功回调
    void refreshArena();
    
    void showFullPackDlg(bool bRemoveLayer = false);
    void showFullCostDlg();
    void showTeamBugDlg();
    void showFoodInTeam();
    void showFoodUpThreeStars();
    void showWeiboPublishInfoWithType(int type);
    void gotoOtherMain();
    
    
    void showTips_gonggao(CUserInfo * pUsrInf,bool isFReq = false);//add1.14
    void showBottomPromptGongGaoInfo(const char *promptInfo);//add1.15
    void scrollBottomPromptGongGaoInfo();//add1.15
    void hideBottomPromptGongGao();//add1.15
    
    void saveArenaShowTips_gonggao();
    void afterArenaShowTips_gonggao();
    
    
    
    
    void showBottomPromptInfo(const char *promptInfo);
    void scrollBottomPromptInfo();
    void resetBottomPromptInfo();

    void setGenRangeBtnIsVisble(bool isShow);
    
    void setGachaInfoHide();
    
    void showSnow();
    void stopSnow();
    
    void updateCntryNameCard();
    void showCntryInfo();
    void showUserInfo();
    
    void GoToHomePage();
    
    void exitArenaLayer();
    
    void GoToYuanbaoPage();
    
    void forceExitFight();
    
    void updateBottomPanel();
    
   
    void cbParCelLayer(CCObject *pSender);
    void cbTwoBack(CCObject *pSender);

    
private:
    int  currenBackMusic;
    TextNode *m_lbBottomPanel;
    
    TextNode *m_lbBottomPanel_gonggao;//add1.15
    void setBottomPromptInfoString(const char* str);
    
    
    void cbBottomPanel(CCObject*);
    void enterNxtLayer();
    
    void cbWelcomeDlg(CCObject* );
    void cbCommDlg(CCObject* );
    void cbGoGachaDlg(CCObject* );
    void cbPublishWeibo(PublishWeiboLayerRet *ret);
    void newbieProc();
    void setTapjoyUserId();
    
    void reqNewbieGacha(); // 新手引导求将请求
    void newbieGachaEnd();  // 新手引导求将结束，去编队提示
    void cbNewbieGachaEnd(CCObject* pObj);
    
    void finishLoginAllRequest(); // 完成login、config、friendlist、receivemial等一些列登陆请求
    
    void showNewFinishTitle(CCObject* obj); // show tips which finish new title
    void cbNewFinishTitle();
    void showStars();
    
    void InitRecordVideoAndRelated();
    
    void cbShowOnlyNotice(CCObject* pObj);

    Dialog1*                m_dlgAlert;
    void cbAlertDlg();
    void Re_showTips(ccTime dt);
    
  
    
public:
    int  getBackMusic();
    bool getIsJustLeave();
    void setIsJustLeave(bool agu);
    void showTouchTip();    // 显示点击继续的提示
    void removeTouchTip();  // 去掉点击继续的提示
    void switchAccount();   // 切换账号
    void reloginWeibo(int type);    // 重新登入绑定账号，对应游戏中服务器返回403 重新登入微博类型type:0 = 默认；1 = 结束战场请求；2 = 结束保卫战请求；3 = 重新登入请求；4 = 战场里请求；
    
    // 首页调用接口
    void showOnlyNotice();  // 从首页进入公告
    void enterNormalArenal();   // 普通战场
    void enterSpecialArenal();  // 特殊战场
    void enterCntryMain();      // 国家主界面
    void enterOtherMain();      // 其他主界面
    void enterGachaMain();      // 求将主界面
    void enterMailList();       // 邮件列表
    void enterBag();            // 随身军袋
    void enterMyVideo();        // 我的录像
    void enterParcel();         // 碎片合成
    void enterTeamInfo();       // 武将编队
    void eneterReward();        //活动领取
    
    bool onCurTeamCostCall(CCLayer *pBaseLayer);
    void okDiglog();
    
    bool onCurArenaLimitMaxStarCall(CCLayer *pBaseLayer);
    void okArenaDiglog();
    unsigned short getArenaLimitMaxStar()const;
    
    bool isExtendPackShow();
    
    void setLabel(int label);       //设置领奖跳转标签
    
    void isRelogin();               //是否重新登录
    void cbDl2Relogin(CCObject* pObject);//重新登录callback
    
};

#endif // __MAIN_SCENE_H__
