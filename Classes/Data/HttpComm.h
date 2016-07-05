//
//  HttpComm.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_HttpComm_h
#define AgainstWar_HttpComm_h

#include <cocos2d.h>
#include "jsoncpp.h"
#include <list>

#define kReqStatOk          0
#define kReqStatRequesting  0xcccc
#define kReqConnectErr      0xdddd
#define kReqHoldOn          0xeeee
#define kReqCancel          0xff11

#define kReloginRc          110

#define AES_KEY_2           "ve^dw+"
#define SECRET_CODE_5       "rn3"
#define SECRET_CODE_2       "#!(HN"
#define AES_KEY_1           "e^qhim="
#define SECRET_CODE_4       "723)_()"
#define AES_KEY_3           "gsz1^"
#define SECRET_CODE_3       "3gfu!^"
#define SECRET_CODE_1       "e"
#define AES_KEY_5           "#@uam"
#define AES_KEY_6           "q*-&"
#define AES_KEY_4           "&5e(x"

enum enDicId{
    enDicLogin = 0,
    enDicCfg,
    enDicFrd,
    enDicNmlDgEnd,
    enDicSpecialDgEnd,
    enDicSangoDgEnd,
    enDicWeeklyDgEnd,
    enDicAllCard,
    enDicUsrInf,
    enDicFriendApplyList,
    enDicCntryInf,
    enDicCollection,
    enDicCntryList,
    enDicMemberList,
    enDicCard,
    enDicMonster,
    enDicMax
};

#define kChatMaxMsgCnt 100
class MsgObj : public cocos2d::CCObject
{
public:
    long    lTime;
    string  strMsg;
};

struct ConfigData;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class HttpComm {
    static  HttpComm*  s_pcIf;

    Json::Value     languageBagDic ;        // 语言包
    Json::Value     usrInfoDic;
    Json::Value     loginDic;
    Json::Value     cfgDic;
    Json::Value     cardDic;
    Json::Value     monsterDic;
    Json::Value     chargeAwardDic;
    Json::Value     frdDic;
    Json::Value     enterDgDic;
    Json::Value     nmlEndDgDic;
    Json::Value     specialEndDgDic;
    Json::Value     SangoYYEndDgDic;
    Json::Value     weeklyEndDgDic;
    Json::Value     allCardDic;
    Json::Value     friendListDic;
    Json::Value     schFriendDic;
    Json::Value     gachaDic;
    Json::Value     reqListDic;
    Json::Value     shopDic;
    Json::Value     collectionDic;
    Json::Value     newbieDic;
    Json::Value     newbieDgDic;
    string          pwUpDicSuccessType;
    string          niepanResultUcid;
    string          tradeNo;
    Json::Value     trophiesDic;
    Json::Value     pushDic;
    Json::Value     compGachaDic;
    Json::Value     exchangeDic;
    Json::Value     cntryInfoDic;
    Json::Value     cntryListDic;
    Json::Value     memberApplyListDic;
    Json::Value     rcvMailDic;
    Json::Value     memberListDic;
    Json::Value     schCntryDic;
    Json::Value     CntryDgCfgDic;
    Json::Value     CntryDgRankDic;
    Json::Value     CntryJFDgRankDic;
    Json::Value     CntryLWDgRankDic;
    string          cntryCurrentInfo;
    Json::Value     CntryDgEndDic;
    Json::Value     useItemDic;
    Json::Value     giftDic;
    Json::Value     titleListDic;
    Json::Value     jifengRecordDic;
    Json::Value     upload_record_award;
    Json::Value     view_other_record;
    Json::Value     guildItemShopDic;
    Json::Value     rewardTaskDic;
    Json::Value     rewardMemberDic;
    Json::Value     rewardTopMemberDic;
    Json::Value     awardRwdInfDic;
    Json::Value     awardConfigDic;
    Json::Value     awardSignDic;
    Json::Value     awardSignTomorrowDic;
    Json::Value     awardLevelUpDic;
    Json::Value     awardMonthDic;
    Json::Value     awardBonusDic;
    Json::Value     awardHuodongDic;
    Json::Value     awardBuchangDic;
    Json::Value     appendCost;
    Json::Value     guildAward;
    Json::Value     ppBilloNoDic;
    
    Json::Value     useCompoundDic;

    
    
    int             m_iReqStat;
    int             m_iReqType;
    long            m_lStatusCode;
    long            m_lServerNow;
    
    long            m_lTimeInterval;
    
    bool            m_bNewbie;
    int             m_iNewbieStep;
    bool            m_bQQAccount;
    bool            m_bSinaAccount;
    bool            m_bOCAccount;
    bool            m_bFBAccount;
    int             m_iMaintenance;
    float           m_fServVer;
    int             m_iMaxFrNum;
    
    bool            m_bChangeStamina;
    bool            m_bRightInviteCode;
    bool            m_isFriendListChange;
    
    float           m_fLangBagVer;    // 语言包最新版本号
    bool            m_bUseInviteCode;   // 是否使用邀请码功能
    bool            m_bUseChristmasUI;  // 是否适用圣诞节UI
    
    long            m_lCfgVer;
    long            m_lCardVer;
    long            m_lMonsterVer;
    
    char            m_languageValue[255]; // 通过key从语言包中获取的文字
    
    string          m_strAppUrl;
    string          m_strSessionid;
    string          m_strUserid;
    string          m_strServErrMsg;
    string          m_strLocalErrMsg;
    string          m_strNotice;
    string          m_strWeiboBack;
    string          m_strMd;
    string          m_strMdnTime;
    string          m_strFmd;
    
    string          m_strPlatform;
    string          m_strOpenId;
    string          m_strToken;
    
    long            m_alDicTime[enDicMax];
    
    list<int>       m_lstSpcFlrClosed;
    
    bool            m_bNeedToUpdateCfg;
    bool            m_bNeedToUpdateCard;
    bool            m_bNeedToUpdateMonster;
    
    list<MsgObj>    m_lstChatRx;
    bool            m_bNewChatRxCame;
    long            m_lChatRxLastMsgTime;
    
    long            m_lGuildCoin;
    
    string          m_strUUID;
public:
	//Get the instance pointer
	static HttpComm* Inst(){
        if(s_pcIf == 0){
            s_pcIf = new HttpComm();
        }
        
        return s_pcIf;
    }
    
    void setReqStat(int iReqStat);
    void setStatusCode(long lStatusCode);
    void clearReqStatus();
    
    ///////////// login ////////////////////////////////////////////////////////
    void loginBySina();
    void loginByTencent();
    
    ///////////// get function /////////////////////////////////////////////////
    long getDicTime(int DicId);
    bool isNewbie();
    bool isOcAcntAvail();
    bool isQQAcntAvail();
    bool isSinaAcntAvail();
    bool isFbAcntAvail();
    bool isRightInviteCode();
    int  getNewbieStep();
    int getMaintenance();
    float getServVer();
    const char* getAppUrl();
    bool isChangeStamina();
    int  getLoginType();
    string getNoticeInviteAward();
    string getNoticeStr();
    string getNoticeGiftStr();
    string getLendInfo();
    string getErrMsg();
    void   setLocalErrStr(char* szErr);
    string getLocalErrStr();
    string getWeiboBackStr();

    int getReqStatus();
    void setIsUseInviteCode(bool bUseInviteCode);
    bool getIsUseInviteCode();
    void setIsUseChristmasUI(bool bUseChristmasUI);
    bool getIsUseChristmasUI();
    long getServerNow();
    
    Json::Value getLoginDic();
    Json::Value getCfgDic();
    Json::Value getCardDic();
    Json::Value getMonsterDic();
    Json::Value getChargeAwardDic();
    Json::Value getFrdDic();
    Json::Value getEnterDgDic();
    
    Json::Value getUseCompoundDic();
    Json::Value getNmlEndDgDic();
    Json::Value getSpecialEndDgDic();
    Json::Value getSangoYYEndDgDic();
    Json::Value getWeeklyEndDgDic();
    Json::Value getUsrInfoDic();
    Json::Value getAllCardDic();
    Json::Value getFriendListDic();
    Json::Value getSchFriendDic();
    Json::Value getGachaDic();
    Json::Value getReqListDic();
    Json::Value getShopDic();
    Json::Value getCollectionDic();
    Json::Value getNewbieDic();
    string      getPwUpDicSuccessType();
    string      getNiePanResultUcid();
    string      getTradeNo();
    Json::Value getTrophiesDic();
    Json::Value getCompGachaDic();
    Json::Value getExchangeDic();
    Json::Value getCntryInfoDic();
    Json::Value getCntryListDic();
    Json::Value getMemberApplyListDic();
    Json::Value getRcvMailDic();
    Json::Value getMemberListDic();
    Json::Value getSchCntryDic();
    Json::Value getCntryDgCfgDic();
    Json::Value getCntrySpecialDgRankDic(string dgType);
    Json::Value getCntryDgRankDic();
    string      getCntryCurrentInfoURL();
    Json::Value getCntryDgEndDic();
    Json::Value getUseItemDic();
    Json::Value getGiftDic();
    Json::Value getTitleListDic();
    Json::Value getJiFengRecordDic();
    Json::Value GetUploadedAward();
    Json::Value GetOtherRecordUrlVid();
    Json::Value getGuildItemShopDic();
    Json::Value getRewarTaskDic();
    Json::Value getRewardMemberDic();
    Json::Value getRewardTopMembersDic();
    Json::Value getAwardInfDic();
    Json::Value getAwardSignDic();
    Json::Value getAwardSignTomorrowDic();
    Json::Value getAwardLevelUpDic();
    Json::Value getAwardMonthDic();
    Json::Value getAwardBonusDic();
    Json::Value getAwardHuodongDic();
    Json::Value getAwardBuchangDic();
    Json::Value getAwardConfigDic();
    Json::Value getAppendCost();
    Json::Value getGuildAward();
    Json::Value getPPBilloNoDic();

    void clearGuildAward();
    ///////////// sync request /////////////////////////////////////////////////
    //request info
    bool reqInfo();
    static size_t reqInfoStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf);
    size_t reqInfo_parsedata(void *buffer, size_t size, size_t nmemb);
    
    //request index
    void reqSessionid();
    void reqSessionidWithInviteCode(string strInviteCode);
    static size_t reqSessionidStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf);
    size_t reqSessionid_parsedata(void *buffer, size_t size, size_t nmemb);
    
    //checkInviteCode
    bool checkInviteCodeReq(string inviteCode);
    static size_t chkInvCodeStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf);
    size_t chkInvCode_parsedata(void *buffer, size_t size, size_t nmemb);
    
    ///////////// async request ////////////////////////////////////////////////

    //异步通信 响应处理
    size_t procResponse(void *buffer, size_t size, size_t nmemb);
    static size_t procResponseStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf);

    //request language bag
    bool reqLanguageBag();
    bool isUpdateLanguageBag(); // 根据服务器获取的最新语言包版本号判断是否更新语言包
    const char *getLanguageValue(const char *languageKey);
    
    string getSameRequestForm();
    bool   checkSessionId();
    
   
    void ReqInfoAsync();
    void loginRequest();
    void cfgRequest();
    void reqCard();
    void reqMonster();
    void reqChargeAward();
    void frdRequest();
    void getAllCardReq();
    
    void commitTeamReq(string new_deck);
    void powerUpReq(string base_ucid, string foods);
    void niepanReq(string base_ucid, string foods);
    void evolutionReq(string base_ucid, string foods);
    void sevoReq(string base_ucid, string foods);
    void wakeReq(string base_ucid, string foods);
    void sellReq(string sell_ucid);
    void addFriendReq(string fid);
    void freeGachaReq();
//    void chargeGachaReq(bool bSuiTang, string strItem);
//    
//    void chargeGachaReq_Other(int OtherGacha, string strItem);

    void chargeGachaReq_Other(int OtherGacha, string strItem,string cStr , bool free);
    void chargeGachaReq(bool bSuiTang, string strItem,string cStr, bool free);
    
    void multiChargeGachaReq(string strItem,string cStr);
    
 //   void multiChargeGachaReq(string strItem);
    void multiFreeGachaReq();
    void compGachaConfigReq();
    void exchangeCardReq(string time, string number);
    void friendListReq();
    
    void newTypeChangeReq(string str);
    void newGiveWuJiangReq(string str);
    
    void acceptRequest(string fid);
    void refuseRequest(string fid);
    void delFriendRequest(string fid);
    void schFriendRequest(string fid);
    void reqListRequest();
    void packExtendRequest();
    void shopRequest();
    void readMzsm();
    void collectionRequest();
    void recoverStaminaRequest();
    void payRequest(string strData, string iapid);
    void newbieRequest(int country, int iStep, string strUcid);
    void newbieRequestPwUp(int iStep, string strBaseUcid, string strCostUcids);
    void reviveRequest(string rty,string strReviveCostId="");
    
    void reqEnterDungeon(string dg_type, string flrid, string rid, string costcoin,
                         string hprid, string hp_ucid,string hptp, string rty,  string new_deck,
                         string helper_deck);
    void dgEndRequest(string dg_type, string flrid, string rid, string dgUid, string score);
    void dgEndForFanTiRequest(string dg_type, string flrid, string rid, string dgUid, string score);
    
    // country
    void createCountryRequest(string name, string declaraion);
    void setViceLeaderRequest(string uid);
    void deleteViceLeaderRequest(string uid);
    void changeMemberPingbi(string uid,bool pingbi);
    void setUserVoiceOnOff(string uid,bool voiceOn);
    void getMemberApplyListRequest();
    void getMemberListRequest();
    void acceptMemberRequest(string uid);
    void refuseMemberRequest(string uid);
    void deleteMemberRequest(string uid);
    void giveUpLeaderRequest(string uid);
    void setDeclarationRequest(string declaration);
    void deleteCountryRequest();
    void unDeleteCountryRequest();
    void setNameCardRequest(string name);
    void levelUpCountryRequest();
    void levelUpBuffRequest(int buffType);
    void assignOneSupplyRequest(string assignDic);
    void assignAllSupplyRequest(long supply);
    void assignOutSupplyRequest(long supply);
    void assignWeekSupplyRequest(long supply);
    void buyCard(short sCid);
    void searchCountryRequest(string cidOrName);
    void getAllCountriesRequest(int type); //1：按时间前10个（最初创建的10个）， 2：按时间后10个（最新创建的10个） ， 3：按等级和经验
    void deleteMail(string  uid,long  timestamp);
    void DeleteMailAll();
    void sendMailRequest(string uid, string msg);
    void sendMailAllRequest(string msg);
    void rcvMailRequest(long lTime);
    void quitCountryRequest();
    void applyCountryRequest(string cid);
    void donateCoinRequest(int coin);
    void exchangeWelfareRequest(string wid);
    void recoverPowerRequest();
    void cntryDgCfgReq();
    void cntryDgRankReq(string dg_id);
    void cntryJiFengRankReq(string dg_id);
    void cntryLuanWuRankReq(string dg_id);
    void cntryJiFLuanWRankReq(string dg_id);
    void cntryDgOpenReq(string dg_id,bool bIsTongMEnglianShou);
    void getCntryCurrentInfo();
    void cntryDgJoinReq(string dg_id, string rtyid, string new_deck,bool bIsTongMEnglianShou);
    void cntryDgEndReq(string dg_id, string combo,bool bIsTongMEnglianShou);
    void useItemReq(string strItem, long lCnt);
    void useStoneReq(string strItem, long lCnt);
    
    
    void useCompoundReq(string type, string newID);
    
    //void useItemAppendCsot(const char *pcItem, long lCnt);
    
    void useItemCardRename(string strItem, string newName);
    void getGiftReq(string strCDKey);
    void titleListReq();
    void setTitleNameReq(string title_id);
    void postRewardReq(long lContribution, int iDeadline, int iPerson,
                       int iCoin, string itemId, int itemCount);
    void rewardTaskListReq();
    void acceptRewardTaskReq(string taskId);
    void recieveRewardReq(string taskId);
    void rewardMemberListReq(string taskId);
    void rewardTopMemberReq();
    
    //聊天室
    static size_t ChatRxStaticFunc(void *buffer, size_t size , size_t nmemb,void *pf);
    void ChatRx_parsedata(void *buffer);
    void sendMsg(string strMsg);
    void startMonitorChatRx();
    void stopMonitorChatRx();
    bool isNewChatRxCame();
    string getChatRxString();
    long getChatRxLastMsgTime();
    void setChatRxLastMsgTime(long lTime);
    void saveMsgObj(MsgObj msgobj);
    void pushErrMsgToChatLst();
    
    //战利品分配
    void getJiFengRecordReq();
    void assignGuildCoin(string strUid, int iCoin);
    void getOrderNo(string productId,string uid);
    //悬赏
    void reqGuildItemShop();
    void reqBuyGuildItem(string strItemId, long lNum);
    
    //领奖
    void reqGetAwardBonus(string bonus, string award_key,string loginDays = "");
    
    //bind account
    static size_t reqAccountBindStaticFunc(void *buffer, size_t size , size_t nmemb,void *pf);
    size_t reqAccountBind_parsedata(void *buffer, size_t size , size_t nmemb);
    void bindRequest(string newOpenid, string newAcessToken ,string newPlatform);
    
    bool loadLanguageBagFromLocal();   // 从本地加载语言包
    
    void weiboBackReq();
    long getTimeStamp();
    string getTimeStampStr();

    bool getIsFriendListChange();
    void setIsFriendListChange(bool isChange);
    
    void clearSessionId();  // 清楚sessionId
    void clearAllData();   // 清除所有数据，用于切换账号
    void frExtend();
    void askCostMonthCard();
    
    string getDecodeStr(void * buffer);
    
    bool getIsApiReq();
    
    string getMD5String();  // 获取MD5 string
    
    string getSessionId();  // 获取SessionId
    void setSessionId(const char* sessionId);
    
    string getUserId();     // 获取Pid
    void setUserId(const char* userId);
    
    bool isSpcFlrClosed(int floorid);
    
    int getReqType(){return m_iReqType;};
    
    bool isNeedToUpdateCfg();
    bool isNeedToUpdateCard();
    bool isNeedToUpdateMonster();
    
    long getGuildCoin(){return m_lGuildCoin;};
    
    // upload record
    void GetUploadAward(ConfigData data);
    
    // 查看他人视频
    void ReqOtherRecord(const char* dungeon_id, const char* type);
    
    //视频服务器试验
    void ReqVideoTest();
    
    //发送请求附加统御力数据
    void sendAppendCostRequest();
    // 推送设置 请求后台保存 对应状态
    void setPushConfig(bool special_dunegon, bool double_drop, bool guild_charge_cold, int push_stamina_num);
    
//    //拜师
//    void sendMAAarm();
    
     void reqPPBilloNo(const char* item_id);
    

    
private:
    //forbid constructor being called outside
	HttpComm();
    
	//forbid copy constructor being called outside
	HttpComm(const HttpComm& );
	
	//forbid assignment
	HttpComm& operator=(const HttpComm& );
    
    void adaptiphone();
    void createFmd();
    string transToMdStr(string value);
    string randString();
    string setPostValue(string key,string value);
    void setServerNow(long t);
 
    string getArgValue();
    
    void saveLanguageBagToLocal(void *buffer, size_t size, size_t nmemb);     // 把语言包保存在本地
    
    string getIndexPostBody(string inviteCode);
    
    void saveDict(Json::Value *dict);
    
    //异步请求启动
    bool AsyncConnect(string url,size_t (*p)(void*, size_t, size_t, void*),string postBody);
    
    //验证服务器
    bool chkTmd(Json::Value *resp);
    
    //替换'+'
    void replacePlus(string& str);
    
    void outputLogFile(char* buf, int nLen);
    
    
    string IntToString(unsigned long _int);
};

#endif
