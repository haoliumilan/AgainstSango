//
//  HttpComm.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CommonDef.h"
#include "HttpComm.h"
#include "curl.h"
#include "md5.h"
#include "OcProxy.h"
#include "ZBase64.h"
#include "RecordHelper.h"
#include "SaveData.h"

using namespace cocos2d;

enum enReqType{
    enReqNull,                  //0
    //1：  新浪登录请求
    enReqSinaLogin,             //1
    //2：  腾讯登录请求
    enReqTencetLogin,           //2
    //3：  登录游戏服务器
    enReqLogin,                 //3
    //4：  绑定
    enReqBind,                  //4
    //5：  获取配置
    enReqCfg,                   //5
    //6：  请求援军
    enReqFrd,                   //6
    //7：  进战场
    enReqEnterDg,               //7
    //8：  结束普通战场
    enReqEndDgNml,              //8
    //9：  结束活动战场
    enReqEndDgSpecial,          //9
    //10：结束周活动战场
    enReqEndDgWeekly,           //10
    //11：获得所有武将
    enReqGetAllCard,            //11
    //12：提交编队
    enReqCommitTeam,            //12
    //13：强化
    enReqPwUp,                  //13
    //14：转生
    enReqEvo,                   //14
    //15：超转生
    enReqSEvo,                  //15
    //16：卖卡
    enReqSell,                  //16
    //17：添加好友
    enReqAddFriend,             //17
    //18：抽良奖
    enReqFreeGacha,             //18
    //19：抽神将
    enReqChargeGacha,           //19
    //20：10连抽神将
    enReqMultiChargeGacha,      //20
    //21：10连抽良将
    enReqMultiFreeGacha,        //21
    //22：获取好友列表
    enReqFriendList,            //22
    //23：接收好友申请
    enReqAccept,                //23
    //24：拒绝好友申请
    enReqRefuse,                //24
    //25：搜索好友
    enReqSchFriend,             //25
    //26：删除好友
    enReqDelFriend,             //26
    //27：获取好友申请列表
    enReqReqList,               //27
    //28：背包扩容
    enReqPackExt,               //28
    //29：获取商店信息
    enReqShop,                  //29
    //30：图鉴
    enReqCollection,            //30
    //31：恢复体力
    enReqRecoverStamina,        //31
    //32：付费
    enReqPay,                   //32
    //33：新手引导
    enReqNewbie,                //33
    //34：复活
    enReqRevive,                //34
    //35：发微博奖励
    enReqWeiboBack,             //35
    //36：检查邀请码
    enReqCheckInviteCode,       //36
    //37：取语言包
    enReqLanguageBag,           //37
    //38：扩充好友上限
    enReqFrdExtend,             //38
    //39：获取点将台配置
    enReqCompgachaConfig,       //39
    //40：点将台换卡
    enReqExchangeCard,          //40
    //41：创建同盟
    enReqCountryCreate,         //41
    //42：任命副盟主
    enReqSetViceLeader,         //42
    //43：撤销副盟主
    enReqDeleteViceLeader,      //43
    //44：获取同盟成员申请列表
    enReqMemberApplyList,       //44
    //45：获取同盟成员列表
    enReqMemberList,            //45
    //46：同意同盟成员加入申请
    enReqAcceptMember,          //46
    //47：拒绝同盟成员加入申请
    enReqRefuseMember,          //47
    //48：删除同盟成员
    enReqDeleteMember,          //48
    //49：同盟盟主让贤
    enReqGiveUpLeader,          //49
    //50：修改同盟宣言
    enReqSetDeclaration,        //50
    //51：解散同盟
    enReqDeleteCountry,         //51
    //52：升级同盟等级
    enReqLevelUpCountry,        //52
    //53：升级同盟buff等级
    enReqLevelUpBuff,           //53
    //54：向一个同盟成员分配战利品
    enReqAssignOneSupply,       //54
    //55：向所有同盟成员平均分配战利品
    enReqAssignAllSupply,       //55
    //56：查找同盟
    enReqSearchCountry,         //56
    //57：获取同盟排名
    enReqGetAllCountries,       //57
    //58：删除邮件
    enReqDeleteMail,            //58
    //59：发送邮件
    enReqSendMail,              //59
    //60：获取邮件列表
    enReqRcvMail,               //60
    //61：退出同盟
    enReqQuitCountry,           //61
    //62：申请加入同盟
    enReqApplyCountry,          //62
    //63：捐献元宝
    enReqDonateCoin,            //63
    //64：兑换福利
    enReqExchangeWelfare,       //64
    //65：恢复行动力
    enReqRecoverPower,          //65
    //66：获取同盟保卫战配置
    enReqCntryDgCfg,            //66
    //67：获取同盟保卫战伤害排行
    enReqCntryDgRank,           //67
    //68：开启同盟保卫战
    enReqCntryDgOpen,           //68
    //69：获取同盟近况
    enReqCntryCurrentInfo,      //69
    //70：进入同盟保卫战
    enReqCntryDgJoin,           //70
    //71：结束同盟保卫战
    enReqCntryDgEnd,            //71
    //72：获取info
    enReqInfo,                  //72
    //73：获取武将配置
    enReqCard,                  //73
    //74：获取敌将配置
    enReqMonster,               //74
    //75：发送同盟邮件
    enReqSendMailAll,           //75
    //76：向所有同盟成员按贡献比例分配战利品
    enReqAssignOutSupply,       //76
    //77：取消解散同盟
    enReqUnDeleteCountry,       //77
    //78：修改同盟名片
    enReqSetNameCard,           //78
    //79：使用道具卡
    enReqUseItem,               //79
    //80：兑换礼品码
    enReqGetGift,               //80
    //81：聊天室通信
    enReqSendMsg,               //81
    //82：聊天室禁言
    enReqSetUserVoice,          //82
    //83：疾风战场排名
    enReqJiFengRank,            //83
    //84：乱舞战场排名
    enReqLuanWuRank,            //84
    //85：聊天室屏蔽某人发言
    enReqChangePingbi,          //85
    //86：使用修改同盟名片道具卡
    enReqUseItemRename,         //86
    //87：武将觉醒
    enReqWake,                  //87
    //88：称号信息列表
    enReqTitleList,             //88
    //89：修改称号
    enReqSetTitleName,          //89
    //90：武将涅槃
    enReqNiePan,                //90
    //91：充值奖励
    enReqChargeAward,           //91
    //92：疾风乱舞战场记录信息，用于分配元宝
    enReqGetJiFengRecord,       //92
    //93：疾风乱舞战场排行
    enReqJiLuanWRank,           //93
    //94：向某一个同盟成员分配同盟元宝
    enReqAssignGuildCoin,       //94
    //95：删除所有邮件
    enReqDeleteMailAll,         //95
    //96：通关三国演义战场
    enReqEndDgSangoYY,          //96
    //97：发布悬赏
    enReqPostReward,            //97
    //98：悬赏宝库配置
    enReqGuildItemShop,         //98
    //99：购买用于悬赏奖励的道具
    enReqBuyGuildItem,          //99
    //100：悬赏信息列表
    enReqRewardTaskList,        //100
    //101：接受悬赏任务
    enReqAcceptRewardTask,      //101
    //102：获得悬赏奖励
    enReqRecieveReward,         //102
    //103：获取参与某个悬赏任务的成员列表
    enReqRewardMemberList,      //103
    //104：上传战斗录像
    enReqUploadVideo,           //104
    //105：通关录像信息
    enReqViewOtherRecord,       //105
    //106：悬赏排行榜
    enReqRewardTopMember,       //106
    //107：领取系统奖励
    enReqGetAwardBonus,         //107
    //108:   xxxx
    enReqMAAarm,                //108
    //109:   xxxx
    enReqMAAmra,                //109
    //110：捐献上古彩石
    enReqUseStone,              //110
    //111：获取订单号（用于支付宝）
    enReqTradeNo,               //111
    //112：确认协议
    enReqReadMzsm,              //112
    //113：购买统御力月卡
    enReqAppendCsot,            //113
    //114：按周贡献分配战利品
    enReqAssignWeekSupply,      //114
    //115：购买武将卡（android功能，iOS没有）
    enReqBuyCard,               //115
    //116：更新新手引导状态
    enReqNewChange,             //116
    //117：新手引导送武将（战败、强化）
    enReqNewGiveWuJiang,        //117
    //118：设置推送信息
    enReqPushConfig,            //118
    //119: 合成碎片
    enReqUseCompound,           //119
    //120: 获取pp助手订单号
    enReqPPBilloNo,            //220
    
};

static string g_strNewbieDg = "{ \"steps_info\": [\
{\"monsters\": [{\"mid\": \"3_monster\", \"init_attack_turn\" : 3}]},\
{\"monsters\": [{\"mid\": \"1_monster\", \"init_attack_turn\" : 3}]},\
{\"monsters\": [{\"mid\": \"5_monster\", \"drop_cid\": \"49_card\", \"drop_lv\": 1, \"init_attack_turn\" : 3},\
                {\"mid\": \"5_monster\", \"init_attack_turn\" : 3},\
                {\"mid\": \"5_monster\", \"init_attack_turn\" : 3}]},\
{\"monsters\": [{\"mid\": \"1_monster\", \"init_attack_turn\" : 3},\
                {\"mid\": \"5_monster\", \"init_attack_turn\" : 3}]},\
{\"monsters\": [{\"mid\": \"4_monster\", \"init_attack_turn\" : 1}]}\
]}";
//新手引导敌将配置
HttpComm* HttpComm::s_pcIf = NULL;

static char gRxBuff[1024 * 1024];
static bool gRxBuffReady = false;

struct thread_url_para
{
    string url;
    string postBody;
};

struct writeData
{
	char* response;
	size_t offset;
};

static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

//******************************************************************************
// communication thread
//******************************************************************************
extern "C" void* connectWithReqTypeThread(void* param)
{
    CURL *curl;
	CURLcode res;
    
    struct writeData wd;
    wd.response = gRxBuff;
    wd.offset = 0;
    
    thread_url_para* para = (thread_url_para*)param;
    string url = para->url;
    string postBody = para->postBody;
    
    CCLOG("Req: %s%s", url.c_str(), postBody.c_str());
    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpComm::procResponseStaticFunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wd);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_ENCODING,"gzip");
    
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());
    
    res = curl_easy_perform(curl);
    
    long lStatusCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &lStatusCode);
    HttpComm::Inst()->setStatusCode(lStatusCode);
    
    CCLOG("curl_easy_perform = %d, statuscode = %d", res, lStatusCode);
    if((res != CURLE_OK) || (lStatusCode != 200)){
        char szErr[100];
        snprintf(szErr, 99, "err: %d.%d.%ld", HttpComm::Inst()->getReqType(), res, lStatusCode);
        HttpComm::Inst()->setLocalErrStr(szErr);
    }
    
    if(lStatusCode == 502){
        HttpComm::Inst()->setReqStat(502);
    }
    else{
        if(res != CURLE_OK)
        {
            HttpComm::Inst()->setReqStat(kReqConnectErr);
        }
        else
        {
            gRxBuffReady = true;
        }
    }
    
	curl_easy_cleanup(curl);
    
    if (para) {
        delete para;
    }
    
    return 0;
}

//******************************************************************************
// 聊天室监视线程
//******************************************************************************
#define kChatRxBufSize 512 * 1024

static bool g_bChatRxMonitorThreadRunning = false;

extern "C" void* ChatRxMonitorThread(void* param)
{
    char * pRxBuff = (char *) malloc(kChatRxBufSize);
    
    struct writeData wd;
    CURL *curl;
	CURLcode res;
    char timeBuf[32];

    while (g_bChatRxMonitorThreadRunning) {
        memset(pRxBuff, 0, kChatRxBufSize);
        
        string url(netServerURL);
        url += "api/?";
        
        snprintf(timeBuf, 31, "time=%ld&", HttpComm::Inst()->getChatRxLastMsgTime());
        
        string postBody("");
        postBody.append("method=guild.get_chat_record&");
        postBody.append(timeBuf);
        postBody.append(HttpComm::Inst()->getSameRequestForm());
        
        CCLOG("Req: %s%s", url.c_str(), postBody.c_str());
        
        wd.response = pRxBuff;
        wd.offset = 0;
        
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpComm::ChatRxStaticFunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wd);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
        
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());
        
        res = curl_easy_perform(curl);
        
        if(res == CURLE_OK)
        {
            HttpComm::Inst()->ChatRx_parsedata(pRxBuff);
        }
        
        curl_easy_cleanup(curl);
        
        sleep(2);
    }
    
    free(pRxBuff);
    
    return 0;
}

size_t HttpComm::ChatRxStaticFunc(void *buffer, size_t size , size_t nmemb,void *pf)
{
    struct writeData *p = (struct writeData*)pf;
    size_t sz = size * nmemb;
    
    if (p->offset + sz < kChatRxBufSize) {
        memcpy((p->response+p->offset),(char*)buffer,sz);
        p->offset += sz;
        *(p->response+p->offset) = 0;
    }
    
	return(sz);
}

void HttpComm::ChatRx_parsedata(void *buffer)
{
    string response((char*)buffer);

	Json::Reader reader;
	Json::Value dict;
    
    char buf[1024];

    int rc = 0;
	if(reader.parse(response, dict, false)){
        //printf("ChatRx = %s", dict.toStyledString().c_str());

        rc = dict["rc"].asInt();
        if(rc == 401){
            reqSessionid();
        }
        else if(rc == 0){
            if (dict.isMember("chat_info_lst")) {
                string strMyUid = dict["user_info"]["uid"].asString();
                
                Json::Value arrMsg = dict["chat_info_lst"];
                for (int i = 0; i < arrMsg.size(); i++) {
                    MsgObj msgObj;
                    
                    string strName = arrMsg[i]["name"].asString();
                    string strTxt = arrMsg[i]["chat_msg"].asString();
                    string strUid = arrMsg[i]["uid"].asString();
                    int iRole = arrMsg[i]["guild_role"].asInt();
                    
                    switch (iRole) {
                        case 1:
                            snprintf(buf,
                                     1023,
                                     CGameData::Inst()->getLanguageValue("chat_ceo"),
                                     strName.c_str(),
                                     strTxt.c_str());
                            break;
                            
                        case 2:
                            snprintf(buf,
                                     1023,
                                     CGameData::Inst()->getLanguageValue("chat_vp"),
                                     strName.c_str(),
                                     strTxt.c_str());
                            break;
                            
                        default:
                            if (strUid.compare(strMyUid) == 0) {
                                snprintf(buf,
                                         1023,
                                         CGameData::Inst()->getLanguageValue("chat_self"),
                                         strName.c_str(),
                                         strTxt.c_str());
                            }
                            else{
                                snprintf(buf,
                                         1023,
                                         CGameData::Inst()->getLanguageValue("chat_mem"),
                                         strName.c_str(),
                                         strTxt.c_str());
                            }
                            break;
                    }
                    
                    msgObj.strMsg = buf;
                    msgObj.lTime = arrMsg[i]["datetime"].asInt();
                    setChatRxLastMsgTime(msgObj.lTime);
                    
                    saveMsgObj(msgObj);
                }
            }
        }

        if(dict.isMember("server_now")){
            long serverNow = dict["server_now"].asInt();
            setServerNow(serverNow);
        }
	}
}

void HttpComm::startMonitorChatRx()
{
    m_bNewChatRxCame = true;
    g_bChatRxMonitorThreadRunning = true;
    pthread_t id;
    
    pthread_create(&id,NULL,ChatRxMonitorThread, NULL);
}

void HttpComm::stopMonitorChatRx()
{
    g_bChatRxMonitorThreadRunning = false;
}

bool HttpComm::isNewChatRxCame()
{
    return m_bNewChatRxCame;
}

string HttpComm::getChatRxString()
{
    string strRet("");
    if (m_lstChatRx.size() == 0) {
        return strRet;
    }
    
    if(OcProxy::Inst()->isIpadDevice())
        strRet.append(CGameData::Inst()->getLanguageValue("chat_head_pad"));
    else
        strRet.append(CGameData::Inst()->getLanguageValue("chat_head"));
    
    long lPreRcvTime = 0;
    
    for (list<MsgObj>::iterator it = m_lstChatRx.begin(); it != m_lstChatRx.end(); ++it)
    {
        MsgObj msgObj = *it;
        
        long lRcvTime = msgObj.lTime;
        
        //两条消息间隔大于5分钟插入一行日期时间
        if ((lPreRcvTime > 0) && (lRcvTime - lPreRcvTime > 5 * 60)) {
            string strYmdhms = OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(lRcvTime, kOcTimeZone);
            
            char buf[256];
            snprintf(buf,
                     255,
                     CGameData::Inst()->getLanguageValue("chat_div"),
                     strYmdhms.c_str());
            
            strRet.append(buf);
        }
        
        lPreRcvTime = lRcvTime;
        
        strRet.append(msgObj.strMsg);
    }
    
    strRet.append(CGameData::Inst()->getLanguageValue("chat_tail"));

    m_bNewChatRxCame = false;
    return strRet;
}

long HttpComm::getChatRxLastMsgTime()
{
    return m_lChatRxLastMsgTime;
}

void HttpComm::setChatRxLastMsgTime(long lTime)
{
    m_lChatRxLastMsgTime = lTime;
    CGameData::Inst()->setLocalChatTime(lTime);
}

void HttpComm::saveMsgObj(MsgObj msgobj)
{
    pthread_mutex_lock(&g_mutex);

    m_lstChatRx.push_back(msgobj);
    
    if (m_lstChatRx.size() > kChatMaxMsgCnt) {
        m_lstChatRx.pop_front();
    }
    
    m_bNewChatRxCame = true;
    
    pthread_mutex_unlock(&g_mutex);
}

void HttpComm::pushErrMsgToChatLst()
{
    if (m_strServErrMsg.empty()) {
        return;
    }
    
    MsgObj lastmsg = m_lstChatRx.back();
    
    MsgObj errmsg;
    char buf[512];
    snprintf(buf,
             511,
             CGameData::Inst()->getLanguageValue("chat_err"),
             m_strServErrMsg.c_str());
    errmsg.strMsg = buf;
    errmsg.lTime = lastmsg.lTime;
    
    saveMsgObj(errmsg);
}

//******************************************************************************
// Constructor
//******************************************************************************
HttpComm::HttpComm()
{
    m_fLangBagVer = 0.0;
    m_iMaxFrNum = 0;
    m_bChangeStamina = false;
    m_bRightInviteCode = false;
    m_lServerNow = 0;
    m_bNewbie = false;
    m_isFriendListChange = true;
    m_iReqType = 0;
    
    languageBagDic.clear();
    
    usrInfoDic.clear();
    loginDic.clear();
    cfgDic.clear();
    frdDic.clear();
    allCardDic.clear();
    enterDgDic.clear();
    useCompoundDic.clear();

    nmlEndDgDic.clear();
    specialEndDgDic.clear();
    SangoYYEndDgDic.clear();
    weeklyEndDgDic.clear();
    friendListDic.clear();
    schFriendDic.clear();
    gachaDic.clear();
    reqListDic.clear();
    shopDic.clear();
    collectionDic.clear();
    newbieDic.clear();
    pwUpDicSuccessType.clear();
    niepanResultUcid.clear();
    tradeNo.clear();
    trophiesDic.clear();
    compGachaDic.clear();
    exchangeDic.clear();
    cntryInfoDic.clear();
    memberApplyListDic.clear();

    cntryListDic.clear();
    memberListDic.clear();
    schCntryDic.clear();
    cntryCurrentInfo.clear();
    CntryDgCfgDic.clear();
    CntryDgRankDic.clear();
    CntryJFDgRankDic.clear();
    CntryLWDgRankDic.clear();
    CntryDgEndDic.clear();
    titleListDic.clear();
    jifengRecordDic.clear();
    upload_record_award.clear();
    view_other_record.clear();
    rewardTaskDic.clear();
    rewardMemberDic.clear();
    rewardTopMemberDic.clear();
    awardRwdInfDic.clear();
    awardConfigDic.clear();
    awardSignDic.clear();
    awardSignTomorrowDic.clear();
    awardLevelUpDic.clear();
    awardMonthDic.clear();
    awardBonusDic.clear();
    awardHuodongDic.clear();
    awardBuchangDic.clear();
    appendCost.clear();
    
    Json::Reader reader(Json::Features::strictMode());
    Json::Value value;
    
    reader.parse(g_strNewbieDg, newbieDgDic, false);
    
    m_strAppUrl.clear();
    m_strServErrMsg.clear();
    m_strLocalErrMsg.clear();
    m_strSessionid.clear();
    m_strUserid.clear();
    m_strNotice.clear();
    m_strMd.clear();
    m_strMdnTime.clear();
    m_strFmd.clear();
    
    m_lstSpcFlrClosed.clear();
    
    m_strPlatform = OcProxy::Inst()->getValueForKey("platform");
    m_strOpenId = OcProxy::Inst()->getValueForKey("openid");
    m_strToken = OcProxy::Inst()->getValueForKey("access_token");

    memset(m_alDicTime, 0, sizeof(long) * enDicMax);
    
    m_iReqStat = 0;
    
    m_lCfgVer = 0;
    m_lCardVer = 0;
    m_lMonsterVer = 0;
    m_bNeedToUpdateCfg = true;
    m_bNeedToUpdateCard = true;
    m_bNeedToUpdateMonster = true;
    
    m_lstChatRx.clear();
    m_bNewChatRxCame = false;
    m_lChatRxLastMsgTime = 0;
    m_lGuildCoin = 0;
    
    m_strUUID = OcProxy::Inst()->getUUID();
    CCLog("uuid = %s", m_strUUID.c_str());    
}

//******************************************************************************
// adaptiphone
//******************************************************************************
void HttpComm::adaptiphone()
{
    if(m_strMd.empty()){
        const char* filepath = OcProxy::Inst()->getExecutablePath();
        char *pchBuf = NULL;
        int  nLen = 0;
        
        FILE *pF   = fopen(filepath, "rb");
        fseek(pF, 0, SEEK_END); //文件指针移到文件尾
        nLen = ftell(pF);  //得到当前指针位置, 即是文件的长度
        rewind(pF);
        
        pchBuf = (char*) malloc(sizeof(char)*nLen+1);
        if(!pchBuf)
        {
            perror("内存不够!\n");
            return;
        }
        
        //读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
        nLen = fread(pchBuf, sizeof(char), nLen, pF);
        
        if(nLen > 0){
            MD5 md5;
            md5.update(pchBuf, nLen);
            m_strMd = md5.toString();
        }
        
        fclose(pF);  //关闭文件
        free(pchBuf); //释放空间
    }
    
    m_strMdnTime  = OcProxy::Inst()->AesEncript(m_strMd.c_str());
    
    replacePlus(m_strMdnTime);
}

//******************************************************************************
// setReqStat
//******************************************************************************
void HttpComm::setReqStat(int iReqStat)
{
    if(iReqStat == 502)
        m_strServErrMsg = OcProxy::Inst()->localizedStringStatic("appCtrlTip1");
    
    m_iReqStat = iReqStat;
}

//******************************************************************************
// setStatusCode
//******************************************************************************
void HttpComm::setStatusCode(long lStatusCode)
{
    m_lStatusCode = lStatusCode;
}

//******************************************************************************
// setServerNow
//******************************************************************************
void HttpComm::setServerNow(long t)
{
    pthread_mutex_lock(&g_mutex);

    m_lServerNow = t;
    time_t lt =time(NULL);
    m_lTimeInterval = m_lServerNow - lt;
    
    pthread_mutex_unlock(&g_mutex);
}

//******************************************************************************
// getServerNow
//******************************************************************************
long HttpComm::getServerNow()
{
    return m_lServerNow;
}

//******************************************************************************
// getTimeStamp
//******************************************************************************
long HttpComm::getTimeStamp()
{
    time_t t = time(NULL);
    return m_lTimeInterval+t;
}

//******************************************************************************
// getTimeStampStr
//******************************************************************************
string HttpComm::getTimeStampStr()
{
    char buf[100];
    
    snprintf(buf, 99, "%ld", getTimeStamp());
    
    string strTime = buf;
    
    return strTime;
}

//******************************************************************************
// setPostValue
//******************************************************************************
string HttpComm::setPostValue(string key,string value)
{
    string data("");
	data.append(key).append("=").append(value).append("&");
	return(data);
}

//******************************************************************************
// createFakeMd5Str
//******************************************************************************
void HttpComm::createFmd()
{
    const char* preBuf = m_strMd.c_str();
    char randBuf[23];
    
    for(int i =0 ;i<22;i++){
        char tempByte = preBuf[i];
        if((tempByte<='z'&&tempByte>='a') || (tempByte<='Z' &&tempByte>='A') || (tempByte <='9' && tempByte>='0'))
        {
            if((tempByte<='Z' && tempByte >='U' )||( tempByte<='z' && tempByte >='u')
               ||(tempByte<='9' && tempByte>= '5'))
            {
                tempByte -= rand()%5+1;
            }else
                tempByte +=rand()%5+1;
        }
        randBuf[i] = tempByte;
        
    }
    randBuf[22] = '\0';
    
    m_strFmd = randBuf;
}

//******************************************************************************
// transToMdStr
//******************************************************************************
string HttpComm::transToMdStr(string value)
{
    MD5 md5;
	md5.update(value);
	string afterMD5 = md5.toString();
	return afterMD5.substr(0,10);
}

//******************************************************************************
// randString
//******************************************************************************
string HttpComm::randString()
{
    char table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u','v','w','x','y','z'};
    
    char outBuf[33];
    int  iTableLen = sizeof(table);
    
    for(int i = 0; i < 32; i++){
        int j = rand() % iTableLen;
        
        outBuf[i] = table[j];
    }
    outBuf[32] = 0;
    
    string str(outBuf);
    
    return str;
}

//******************************************************************************
// getArgValue
//******************************************************************************
string HttpComm::getArgValue()
{
    string timestamp("");
    long tt = getTimeStamp();
    char value[16] = {0};
    sprintf(value,"%ld",tt);
    
    string argValue("");
    argValue.append(value).append(SECRET_CODE_1).append(SECRET_CODE_2)
    .append(SECRET_CODE_3).append(SECRET_CODE_4).append(SECRET_CODE_5);
    
    string tmp = transToMdStr(argValue);
    
    return tmp.substr(0,10);
}

bool HttpComm::loadLanguageBagFromLocal()
{
    //    string fileName1= "/data/data/cn.mobage.g13000279/enReqLanguageBag.txt";
    string languageBagFilePath = CCFileUtils::getWriteablePath();
    languageBagFilePath += "languageBag.txt";
    
	FILE* fp1 = fopen(languageBagFilePath.c_str(),"r");
    if(fp1 != NULL)
    {
//        int size =0 ;
        
        fseek(fp1,0, SEEK_END );
        const int size = ftell(fp1);
        fseek(fp1,0, SEEK_SET );
        char buffer[size+2];
        fread(buffer, size, 1, fp1);
        buffer[size+1] = 0 ;
        string response((char*)buffer);
        Json::Reader reader;
        if(!reader.parse(response, languageBagDic, false))
        {
            return false;
        }
        fclose(fp1);
    }
    else
    {
        return false ;
    }
    
    CCLog("loadLanguageBagFromLocal");
    return true ;
}

void HttpComm::saveLanguageBagToLocal(void *buffer, size_t size, size_t nmemb)
{
    //    string fileName1 = "/data/data/cn.mobage.g13000279/enReqLanguageBag.txt";
    string languageBagFilePath = CCFileUtils::getWriteablePath();
    languageBagFilePath += "languageBag.txt";
    
	FILE* fp1 = fopen(languageBagFilePath.c_str(),"w+");
	if(fp1 !=NULL)
    {
        fwrite(buffer, size, nmemb, fp1);
        fclose(fp1);
        
        OcProxy::Inst()->setFloatValueForKey(m_fLangBagVer, "language_version");
        CCLog("saveLanguageBagToLocal");
	}
    else
        CCLog("GameData::saveLanguageBagToLocal null:%s", languageBagFilePath.c_str());
}

//******************************************************************************
// setIsUseInviteCode
//******************************************************************************
void HttpComm::setIsUseInviteCode(bool bUseInviteCode)
{
    m_bUseInviteCode = bUseInviteCode;
}

//******************************************************************************
// getIsUseInviteCode
//******************************************************************************
bool HttpComm::getIsUseInviteCode()
{
    return m_bUseInviteCode;
}

//******************************************************************************
// setIsUseChristmasUI
//******************************************************************************
void HttpComm::setIsUseChristmasUI(bool bUseChristmasUI)
{
    m_bUseChristmasUI = bUseChristmasUI;
    m_bUseChristmasUI = false; //关闭m_bUseChristmas UI
}

//******************************************************************************
// getIsUseChristmasUI
//******************************************************************************
bool HttpComm::getIsUseChristmasUI()
{
    return m_bUseChristmasUI;
}

#pragma mark -
#pragma mark  public get functions
//******************************************************************************
// get functions
//******************************************************************************
long HttpComm::getDicTime(int DicId)
{
    if((DicId < 0) || (DicId >= enDicMax))
        return 0;
    
    return m_alDicTime[DicId];
}

bool HttpComm::isNewbie()
{
    return m_bNewbie;
}

bool HttpComm::isOcAcntAvail()
{
    return m_bOCAccount;
}

bool HttpComm::isQQAcntAvail()
{
    return m_bQQAccount;
}

bool HttpComm::isSinaAcntAvail()
{
    return m_bSinaAccount;
}
bool HttpComm::isFbAcntAvail()
{
    return m_bFBAccount;
}

bool HttpComm::isRightInviteCode()
{
    return m_bRightInviteCode;
}

int HttpComm::getNewbieStep()
{
    return m_iNewbieStep;
}

const char* HttpComm::getAppUrl()
{
    return m_strAppUrl.c_str();
}

int HttpComm::getMaintenance()
{
    return m_iMaintenance;
}

float HttpComm::getServVer()
{
    return m_fServVer;
}

bool HttpComm::isChangeStamina(){
    return m_bChangeStamina;
}

int HttpComm::getLoginType()
{
    int iRet = enLoginReqType_Null;
    
    if (!m_strPlatform.compare(kPltfmOc)){
        iRet = enLoginReqType_OcLogin;
    }
    else if(!m_strPlatform.compare(kPltfmTencent)){
        iRet = enLoginReqType_TencentLogin;
    }
    else if(!m_strPlatform.compare(kPltfmSina)){
        iRet = enLoginReqType_SinaLogin;
    }
#if defined(kPltfmPp)
    else if(!m_strPlatform.compare(kPltfmPp)){
        iRet = enLoginReqType_Pp;
    }
#endif
    else{
        iRet = enLoginReqType_Null;
    }
    
    return iRet;
}

string HttpComm::getNoticeInviteAward()
{
    
    return loginDic["invite/d_award"].asString();
}

string HttpComm::getNoticeStr()
{
    return loginDic["notice"].asString();
}

string HttpComm::getNoticeGiftStr()
{
    return loginDic["login_award"].asString();
}

string HttpComm::getLendInfo()
{
    return loginDic["lend_info"].asString();
}

string HttpComm::getErrMsg()
{
    return m_strServErrMsg;
}

void HttpComm::setLocalErrStr(char* szErr)
{
    m_strLocalErrMsg = szErr;
}

string HttpComm::getLocalErrStr()
{
    return m_strLocalErrMsg;
}

string HttpComm::getWeiboBackStr()
{
    return m_strWeiboBack;
}

int HttpComm::getReqStatus()
{
    if(gRxBuffReady){
        CCLOG("gRxBuff size = %d", strlen(gRxBuff));
        
        procResponse(gRxBuff,1,strlen(gRxBuff));
        gRxBuffReady = false;
    }
    
    if(m_iReqStat == 401){
        m_strLocalErrMsg = m_strServErrMsg;
        
        m_iReqStat = kReqConnectErr;

        reqSessionid();
        
        if (m_iReqStat != 403)
            m_iReqStat = kReqConnectErr;
    }
    
    switch (m_iReqType) {
        case enReqSinaLogin:
            if(OcProxy::Inst()->getSinaLoginStatus() == 2){
                m_strOpenId = OcProxy::Inst()->getSinaOpenId();
                m_strToken = OcProxy::Inst()->getSinaAccessToken();
                m_iReqStat = kReqStatOk;
                m_iReqType = enReqNull;
                if(strcmp(OcProxy::Inst()->getValueForKey("platform"), "oc") == 0)
                {
                    bindRequest(m_strOpenId, m_strToken, kPltfmSina);
                    
                    

                }
            }
            else if(OcProxy::Inst()->getSinaLoginStatus() == -1)
            {
                m_iReqStat = kReqCancel;
                m_strPlatform = kPltfmOc;
                m_iReqType = enReqNull;
            }
            else
                m_iReqStat = kReqStatRequesting;
            break;
        case enReqTencetLogin:
            if(OcProxy::Inst()->getTencentLoginStatus() == 2)
            {
                m_strOpenId = OcProxy::Inst()->getTencentOpenId();
                m_strToken = OcProxy::Inst()->getTencentAccessToken();
                m_iReqStat = kReqStatOk;
                m_iReqType = enReqNull;
                if(strcmp(OcProxy::Inst()->getValueForKey("platform"), "oc") == 0)
                {
                    bindRequest(m_strOpenId, m_strToken, kPltfmTencent);
                }
                
            }else if(OcProxy::Inst()->getTencentLoginStatus()== -1){
                m_iReqStat = kReqCancel;
                m_strPlatform = kPltfmOc;
                m_iReqType = enReqNull;
            }
            else
                m_iReqStat = kReqStatRequesting;
        default:
            break;
    }
    
    return m_iReqStat;
}

Json::Value  HttpComm::getPPBilloNoDic()
{
    return ppBilloNoDic;
}

Json::Value HttpComm::getLoginDic()
{
    return loginDic;
}

Json::Value HttpComm::getCfgDic()
{
    return cfgDic;
}

Json::Value HttpComm::getCardDic()
{
    return cardDic;
}

Json::Value HttpComm::getMonsterDic()
{
    return monsterDic;
}

Json::Value HttpComm::getChargeAwardDic()
{
    return chargeAwardDic;
}

Json::Value HttpComm::getFrdDic()
{
    return frdDic;
}

Json::Value HttpComm::getUseCompoundDic()
{
    return useCompoundDic;
}


Json::Value HttpComm::getEnterDgDic()
{
    if(m_bNewbie)
        return newbieDgDic;
    else
        return enterDgDic;
}

Json::Value HttpComm::getNmlEndDgDic()
{
    return nmlEndDgDic;
}

Json::Value HttpComm::getSpecialEndDgDic()
{
    return specialEndDgDic;
}

Json::Value HttpComm::getSangoYYEndDgDic()
{
    return SangoYYEndDgDic;
}

string HttpComm::getCntryCurrentInfoURL()
{
    return cntryCurrentInfo;
}

Json::Value HttpComm::getWeeklyEndDgDic()
{
    return weeklyEndDgDic;
}

Json::Value HttpComm::getUsrInfoDic()
{
    return usrInfoDic;
}

Json::Value HttpComm::getCntryInfoDic()
{
    return cntryInfoDic;
}

Json::Value HttpComm::getAllCardDic()
{
    return allCardDic;
}

Json::Value HttpComm::getFriendListDic()
{
    return friendListDic;
}

Json::Value HttpComm::getMemberListDic()
{
    return memberListDic;
}

Json::Value HttpComm::getSchCntryDic()
{
    return schCntryDic;
}

Json::Value HttpComm::getCntryListDic()
{
    return cntryListDic;
}

Json::Value HttpComm::getMemberApplyListDic()
{
    return memberApplyListDic;
}

Json::Value HttpComm::getSchFriendDic()
{
    return schFriendDic;
}

Json::Value HttpComm::getGachaDic()
{
    return gachaDic;
}

Json::Value HttpComm::getReqListDic()
{
    return reqListDic;
}

Json::Value HttpComm::getShopDic()
{
    return shopDic;
}

Json::Value HttpComm::getCollectionDic()
{
    return collectionDic;
}

Json::Value HttpComm::getNewbieDic()
{
    return newbieDic;
}

string HttpComm::getPwUpDicSuccessType()
{
    return pwUpDicSuccessType;
}

string HttpComm::getNiePanResultUcid()
{
    return niepanResultUcid;
}
string HttpComm::getTradeNo()
{
    return tradeNo;
}

Json::Value HttpComm::getTrophiesDic()
{
    return trophiesDic;
}

Json::Value HttpComm::getCompGachaDic()
{
    return compGachaDic;
}

Json::Value HttpComm::getExchangeDic()
{
    return exchangeDic;
}


Json::Value HttpComm::getRcvMailDic()
{
    return rcvMailDic;
}

Json::Value HttpComm::getCntryDgCfgDic()
{
    return CntryDgCfgDic;
}

Json::Value HttpComm::getCntrySpecialDgRankDic(string dgType)
{
    if (dgType == "1")
        return CntryJFDgRankDic;
    else
        return CntryLWDgRankDic;
}

Json::Value HttpComm::getCntryDgRankDic()
{
    return CntryDgRankDic;
}

Json::Value HttpComm::getCntryDgEndDic()
{
    return CntryDgEndDic;
}

Json::Value HttpComm::getUseItemDic()
{
    return useItemDic;
}

Json::Value HttpComm::getGiftDic()
{
    return giftDic;
}

Json::Value HttpComm::getTitleListDic()
{
    return titleListDic;
}

Json::Value HttpComm::getJiFengRecordDic()
{
    return jifengRecordDic;
}

Json::Value HttpComm::GetUploadedAward()
{
    return upload_record_award;
}

Json::Value HttpComm::GetOtherRecordUrlVid()
{
    return view_other_record;
}

Json::Value HttpComm::getGuildItemShopDic()
{
    return guildItemShopDic;
}

Json::Value HttpComm::getRewarTaskDic()
{
    return rewardTaskDic;
}

Json::Value HttpComm::getRewardMemberDic()
{
    return rewardMemberDic;
}

Json::Value HttpComm::getRewardTopMembersDic()
{
    return rewardTopMemberDic;
}

Json::Value HttpComm::getAwardInfDic()
{
    return awardRwdInfDic;
}

Json::Value HttpComm::getAwardSignDic()
{
    return awardSignDic;
}

Json::Value HttpComm::getAwardSignTomorrowDic()
{
    return awardSignTomorrowDic;
}

Json::Value HttpComm::getAwardLevelUpDic()
{
    return awardLevelUpDic;
}

Json::Value HttpComm::getAwardMonthDic()
{
    return awardMonthDic;
}

Json::Value HttpComm::getAwardBonusDic()
{
    return awardBonusDic;
}

Json::Value HttpComm::getAwardHuodongDic()
{
    return awardHuodongDic;
}

Json::Value HttpComm::getAwardBuchangDic()
{
    return awardBuchangDic;
}

Json::Value HttpComm::getAwardConfigDic()
{
    return awardConfigDic;
}

Json::Value HttpComm::getAppendCost()
{
    return appendCost;
}

Json::Value HttpComm::getGuildAward()
{
    return guildAward;
}

void HttpComm::clearGuildAward()
{
    guildAward.clear();
}

void HttpComm::clearReqStatus()
{
    m_iReqStat = kReqStatOk;
    m_iReqType = enReqNull;
}

bool HttpComm::isUpdateLanguageBag()
{
    float oldLanguageBagVersion = OcProxy::Inst()->getFloatValueForKey("language_version");
    //if (m_fLangBagVer > oldLanguageBagVersion) {
    if (m_fLangBagVer != oldLanguageBagVersion) {
        return true;
    }
    else {
        return false;
    }
}

const char* HttpComm::getLanguageValue(const char *languageKey)
{
    if (languageBagDic.isNull()) {
        return "";
    }
    
    if (!languageBagDic.isMember(languageKey))
    {
        return "";
    }
    else
    {
        return languageBagDic[languageKey].asCString();
    }
}

string HttpComm::getMD5String()
{
    if (m_strMd.empty()) {
        adaptiphone();
    }
    
    return m_strMd;
}

string HttpComm::getSessionId()
{
    return m_strSessionid;
}

void HttpComm::setSessionId(const char *sessionId)
{
    if (sessionId)
        m_strSessionid = string(sessionId);
}

string HttpComm::getUserId()
{
    return m_strUserid;
}

void HttpComm::setUserId(const char *userId)
{
    if (userId)
        m_strUserid = string(userId);
}

bool HttpComm::isSpcFlrClosed(int floorid)
{
    bool bRet = false;
    
    for (list<int>::iterator it = m_lstSpcFlrClosed.begin(); it != m_lstSpcFlrClosed.end(); ++it)
    {
        int iClosedFlrId = *it;
        
        if (iClosedFlrId == floorid) {
            bRet = true;
            break;
        }
    }
    
    return bRet;
}

bool HttpComm::isNeedToUpdateCfg()
{
    return m_bNeedToUpdateCfg;
}

bool HttpComm::isNeedToUpdateCard()
{
    return m_bNeedToUpdateCard;
}

bool HttpComm::isNeedToUpdateMonster()
{
    return m_bNeedToUpdateMonster;
}

#pragma mark -
#pragma mark login
//******************************************************************************
// login
//******************************************************************************
void HttpComm::loginBySina()
{
    m_iReqType = enReqSinaLogin;
    m_iReqStat = kReqStatRequesting;
    m_strPlatform = kPltfmSina;
    
    OcProxy::Inst()->loginBySina();
}


void HttpComm::loginByTencent()
{
    m_iReqType = enReqTencetLogin;
    m_iReqStat = kReqStatRequesting;
    m_strPlatform = kPltfmTencent;
    
    OcProxy::Inst()->loginbyTencent();
}

#pragma mark -
#pragma mark sync request
//******************************************************************************
// sync request
//******************************************************************************
bool HttpComm::reqInfo()
{
    string url(netServerURL);
	url +="info/";
    
	bool result;
    CURL *curl;
	CURLcode res;
    
    //reset value
    m_strAppUrl = "";
	m_bOCAccount = false;
	m_bQQAccount = false;
	m_bSinaAccount = false;
	m_bFBAccount = false;
	m_lServerNow = 0;
    m_fServVer = 0.0f;
    m_iMaintenance = 0;
    m_fLangBagVer = 0.0f;
    m_bUseInviteCode = false;
    m_bUseChristmasUI = false;
    
	curl = curl_easy_init();
    
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reqInfoStaticFunc);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    
    string postBody("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    postBody.append(setPostValue("device_platform",kPltfmAndroid));
#else
    postBody.append(setPostValue("device_platform", kPltfmIos));
#endif
    
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());
	
    CCLOG("Req: %s", url.c_str());
	res = curl_easy_perform(curl);
    result = (res == CURLE_OK);
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &m_lStatusCode);
    
    if((res != CURLE_OK) || (m_lStatusCode != 200)){
        char szErr[100];
        snprintf(szErr, 99, "err: %d.%d.%ld", m_iReqType, res, m_lStatusCode);
        setLocalErrStr(szErr);
    }
    
    if(m_lStatusCode == 502){
        m_iReqStat = 502;
        m_strServErrMsg = OcProxy::Inst()->localizedStringStatic("appCtrlTip1");
    }
    else if(res != CURLE_OK){
        m_iReqStat = kReqConnectErr;
    }
    
	curl_easy_cleanup(curl);
    
	return(result);
}



size_t HttpComm::reqInfoStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf)
{
    return(static_cast<HttpComm*>(pf)->reqInfo_parsedata(buffer,size,nmemb));
}


size_t HttpComm::reqInfo_parsedata(void *buffer, size_t size, size_t nmemb)
{
    
    string parseData((char *) buffer);
	Json::Value root;
	Json::Reader reader;
    
	bool parsedSuccess = reader.parse(parseData, root, false);
    
	if ( !parsedSuccess ){
	   	m_iReqStat = kReqConnectErr;
        
        if (parseData.find("python") != string::npos) {
            char szErr[100];
            snprintf(szErr, 99, "err: %d.py", m_iReqType);
            setLocalErrStr(szErr);
        }
	    return 0;
	}
    
    CCLog("reqInfo = %s", parseData.c_str());
    
	m_strAppUrl = root["app_url"].asString();
    
	m_bOCAccount = root["oc_account"].asBool();
	m_bQQAccount = root["qq_account"].asBool();
	m_bSinaAccount = root["sina_account"].asBool();
	m_bFBAccount = root["fb_account"].asBool();
    
	m_lServerNow = root["server_now"].asInt();
    
    m_fServVer = atof(root["version"].asString().c_str());
    m_iMaintenance = root["maintenance"].asInt();
    
    m_fLangBagVer = atof(root["language_version"].asString().c_str());
    m_bUseInviteCode = root["open_invite"].asBool();
    m_bUseChristmasUI = root["christmas"].asBool();
    m_bUseChristmasUI = false; //关闭Christmas UI
    
    m_lCfgVer = root["sup_value"].asInt();
    m_lCardVer = root["cup_value"].asInt();
    m_lMonsterVer = root["mup_value"].asInt();
    
	setServerNow(m_lServerNow);
    
    m_iReqStat = 0;
    
	return size*nmemb;
}

//******************************************************************************
// reqSessionid
//******************************************************************************
void HttpComm::reqSessionid()
{
    m_strUserid.clear();
    reqSessionidWithInviteCode("");
}

void HttpComm::reqSessionidWithInviteCode(string strInviteCode)
{
    if (m_lServerNow == 0) {
        reqInfo();
    }
    
    if (m_lServerNow == 0) {
        return;
    }
    
    string url(netServerURL);
	url +="index/";
    
    CURL *curl;
	CURLcode res;
    
    curl = curl_easy_init();
    
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reqSessionidStaticFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
    
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    
	string postBody = getIndexPostBody(strInviteCode);
    
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());
    
    CCLOG("Req: %s%s", url.c_str(), postBody.c_str());
	res = curl_easy_perform(curl);
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &m_lStatusCode);
    
    if((res != CURLE_OK) || (m_lStatusCode != 200)){
        char szErr[100];
        snprintf(szErr, 99, "err: %d.%d.%ld", m_iReqType, res, m_lStatusCode);
        setLocalErrStr(szErr);
    }
    
    if(m_lStatusCode == 502){
        m_iReqStat = 502;
        m_strServErrMsg = OcProxy::Inst()->localizedStringStatic("appCtrlTip1");
    }
    else if(res != CURLE_OK){
        m_iReqStat = kReqConnectErr;
    }
    
	curl_easy_cleanup(curl);
}

string HttpComm::getIndexPostBody(string inviteCode)
{
    string postBody("");
    
    if(!inviteCode.empty()){
		postBody.append(setPostValue("invite_code",inviteCode));
	}
    
    if(m_strPlatform.empty()){
        m_strPlatform = kPltfmOc;
    }
    
#if defined(kPltfmPp)
    postBody.append(setPostValue("platform", kPltfmPp));
    m_strPlatform = kPltfmPp;
    CCLog("userid  %d ",OcProxy::Inst()->getPpCurrentUserId());
    char t[256];
    string s;
    sprintf(t, "%d", OcProxy::Inst()->getPpCurrentUserId());
    s=t;
    postBody.append(setPostValue("openid",s));
    postBody.append(setPostValue("access_token",OcProxy::Inst()->getPptoken()));
#else
    
    postBody.append(setPostValue("platform", m_strPlatform));
    
    if(!m_strOpenId.empty()){
        postBody.append(setPostValue("openid",m_strOpenId));
        postBody.append(setPostValue("access_token",m_strToken));
    }
    
#endif
    
    char tmp[16]={0};
    sprintf(tmp,"%ld",getTimeStamp());
    string timestamp(tmp);
    
    postBody.append(setPostValue("timestamp",timestamp));
    postBody.append(setPostValue("arg",getArgValue()));
    postBody.append(setPostValue("sig",randString()));
    postBody.append(setPostValue("version",kLocalAppVer));
    
    const char* szMacAddr = OcProxy::Inst()->getMacAddr();
    if (szMacAddr) {
        postBody.append(setPostValue("macaddr",szMacAddr));
    }
    
    if (!m_strUUID.empty()) {
        postBody.append(setPostValue("uuid", m_strUUID));
    }
    
    const char* cAdId = OcProxy::Inst()->getAdId();
    if (cAdId)
        postBody.append(setPostValue("mac_adid", cAdId));
    
    postBody.append(setPostValue("device_token", OcProxy::Inst()->getDeviceToken()));
    
    
    postBody.append(setPostValue("channel_mark", channel_mark));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    postBody.append(setPostValue("device_platform",kPltfmAndroid));
#else
    postBody.append(setPostValue("device_platform", kPltfmIos));
#endif
    
    
    return(postBody);
}

size_t HttpComm::reqSessionidStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf)
{
    return(static_cast<HttpComm*>(pf)->reqSessionid_parsedata(buffer,size,nmemb));
}

size_t HttpComm::reqSessionid_parsedata(void *buffer, size_t size, size_t nmemb)
{
    string openid ;
	string access_token;
    string parseData((char *)buffer);

    
	Json::Value root;
	Json::Reader reader;
    
    bool parsedSuccess = reader.parse(parseData, root, false);
    
	if ( !parsedSuccess ){
	   	m_iReqStat = kReqConnectErr;
        
        if (parseData.find("python") != string::npos) {
            char szErr[100];
            snprintf(szErr, 99, "err: %d.py", m_iReqType);
            setLocalErrStr(szErr);
        }
	    return 0;
	}
    
#if defined (kDeveloping)
    CCLog("reqSessionid = %s", root.toStyledString().c_str());
#endif
    
    if(!root["server_now"].isNull()){
        m_lServerNow = root["server_now"].asInt();
        setServerNow(m_lServerNow);
    }

    int rc = root["rc"].asInt();
    
    if(rc == 0){
        m_bNewbie = root["newbie"].asBool();
        m_iNewbieStep = root["newbie_step"].asInt();
        m_strSessionid = root["sessionid"].asString();
        
        if (!root["oc_openid"].isNull()) {
        	m_strOpenId = root["oc_openid"].asString();
            m_strToken = root["oc_access_token"].asString();
        }
        
        m_strUserid = root["pid"].asString();
        
        if(string(OcProxy::Inst()->getValueForKey("access_token")).compare("") == 0){
            OcProxy::Inst()->setValueForKey(m_strPlatform.c_str(), "platform");
            OcProxy::Inst()->setValueForKey(m_strOpenId.c_str(), "openid");
            OcProxy::Inst()->setValueForKey(m_strToken.c_str(), "access_token");
            OcProxy::Inst()->synchronizeDefaultData();
        }
    }
    else{
        m_strServErrMsg = root["msg"].asString();
    }
    
    m_iReqStat = rc;
    
    return (size * nmemb);
}

//******************************************************************************
// checkInviteCodeReq
//******************************************************************************
bool HttpComm::checkInviteCodeReq(string inviteCode)
{
    m_bRightInviteCode = false;
    
    string url(netServerURL);
	url +="invite_code/?";
    
    url.append(setPostValue("invite_code", inviteCode));
    
	bool result;
    CURL *curl;
	CURLcode res;
    
	curl = curl_easy_init();
    
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, chkInvCodeStaticFunc);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    
    string postBody("");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    postBody.append(setPostValue("device_platform",kPltfmAndroid));
#else
    postBody.append(setPostValue("device_platform", kPltfmIos));
#endif
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());
	
    CCLOG("Req: %s", url.c_str());
	res = curl_easy_perform(curl);
    result = (res == CURLE_OK);
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &m_lStatusCode);
    
    if((res != CURLE_OK) || (m_lStatusCode != 200)){
        char szErr[100];
        snprintf(szErr, 99, "err: %d.%d.%ld", m_iReqType, res, m_lStatusCode);
        setLocalErrStr(szErr);
    }
    
    if(m_lStatusCode == 502){
        m_iReqStat = 502;
        m_strServErrMsg = OcProxy::Inst()->localizedStringStatic("appCtrlTip1");
    }
    else if(res != CURLE_OK){
        m_iReqStat = kReqConnectErr;
    }
    
	curl_easy_cleanup(curl);
    
    return true;
}

size_t HttpComm::chkInvCodeStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf)
{
    return(static_cast<HttpComm*>(pf)->chkInvCode_parsedata(buffer,size,nmemb));
}

size_t HttpComm::chkInvCode_parsedata(void *buffer, size_t size, size_t nmemb)
{
    
    string parseData((char *)buffer);
	Json::Value root;
	Json::Reader reader;
    
	bool parsedSuccess = reader.parse(parseData, root, false);
    
	if ( !parsedSuccess ){
	   	m_iReqStat = kReqConnectErr;
        
        if (parseData.find("python") != string::npos) {
            char szErr[100];
            snprintf(szErr, 99, "err: %d.py", m_iReqType);
            setLocalErrStr(szErr);
        }
	    return 0;
	}
    
    m_bRightInviteCode = root["status"].asBool();
    
    m_iReqStat = 0;
    
	return size*nmemb;
}

#pragma mark -
#pragma mark Asynchronous response process
bool HttpComm::AsyncConnect(string url,size_t (*p)(void*, size_t, size_t, void*),string postBody)
{
    thread_url_para* param = new thread_url_para;
    param->url = url;
    param->postBody = postBody;
    pthread_t id;
    
    m_iReqStat = kReqStatRequesting;
    
    memset(gRxBuff, 0, sizeof(gRxBuff));
    gRxBuffReady = false;
    
    pthread_create(&id,NULL,connectWithReqTypeThread,(void*)param);
    
	return(true);
}

size_t HttpComm::procResponseStaticFunc(void *buffer, size_t size, size_t nmemb, void *pf)
{
    struct writeData *p = (struct writeData*)pf;
    size_t sz = size * nmemb;
    
	memcpy((p->response+p->offset),(char*)buffer,sz);
	p->offset += sz;
	return(sz);
}

size_t HttpComm::procResponse(void *buffer, size_t size, size_t nmemb)
{
//    string decodeStr;
////    if(getIsApiReq())
////         decodeStr = getDecodeStr((char *)buffer);
////    else
//        decodeStr =  string((char *) buffer);
//    
//    string response(decodeStr.c_str());
    
    string response((char *) buffer);
    
	Json::Reader reader;
	Json::Value dict;
    
    char szErr[100];
    int rc = 0;
	if(!reader.parse(response, dict, false)){
	    rc = kReqConnectErr;
        
        if (response.find("python") != string::npos) {
            snprintf(szErr, 99, "err: %d.py", m_iReqType);
            setLocalErrStr(szErr);
        }
        else{
            snprintf(szErr, 99, "err: %d.cant parse", m_iReqType);
            setLocalErrStr(szErr);
        }
        
        outputLogFile(szErr, strlen(szErr));
        outputLogFile((char *) buffer, size*nmemb);
	}
    else{
#if defined (kDeveloping)
        printf("%s", dict.toStyledString().c_str());
#endif

        if (m_iReqType == enReqLanguageBag) {
            languageBagDic = dict;
            saveLanguageBagToLocal(buffer, size, nmemb);
            m_iReqStat = kReqStatOk;
            
            return size * nmemb;
        }
        else{
            rc = dict["rc"].asInt();
        }
    }
    
    //取配置比较花时间,不更新server now
    if((rc == 0) && ((m_iReqType == enReqCard) || (m_iReqType == enReqMonster))){
    }
    else{
        if(!dict["server_now"].isNull() && size*nmemb < 50000){
            long serverNow = dict["server_now"].asInt();
            setServerNow(serverNow);
        }
    }
    
    m_strServErrMsg = dict["msg"].asString().c_str();
    
    //验证服务器
    if(rc == 0){
        if (m_iReqType == 111) {
            saveDict(&dict);
            m_iReqStat = kReqStatOk;
        }else{
            if(chkTmd(&dict))
            {
                saveDict(&dict);
                m_iReqStat = kReqStatOk;
            }
            else{
                m_iReqStat = kReqConnectErr;
                
                snprintf(szErr, 99, " %d.0.200.911", m_iReqType);
                m_strServErrMsg.append(szErr);
            }
        }
    }
    else
    {
        m_iReqStat = rc;

        if(rc == 101)
        {
            OcProxy::Inst()->goAppStoreUpdate();
        }
        
        if (m_iReqType != enReqSendMsg) {
            snprintf(szErr, 99, " %d.0.200.%d", m_iReqType, rc);
            m_strServErrMsg.append(szErr);
        }
    }
    
    //更新 user info
    if(m_iReqStat != kReqConnectErr){
        if(!dict["user_info"].isNull()){
            if(!usrInfoDic.empty()){
                usrInfoDic.clear();
            }
            
            usrInfoDic = dict["user_info"];
            m_alDicTime[enDicUsrInf] = getTimeStamp();
            
            m_bNewbie = usrInfoDic["newbie"].asBool();
            m_iNewbieStep = usrInfoDic["newbie_step"].asInt();
            
            m_bChangeStamina = true;
        }
        
        //guildAward.clear();
        if (dict.isMember("guild_award"))
        {
            guildAward = dict["guild_award"];
        }
        
        
        if (dict.isMember("guild_info")) {
            if (!dict["guild_info"].isNull()) {
                if (!cntryInfoDic.empty()) {
                    cntryInfoDic.clear();
                }
                
                cntryInfoDic = dict["guild_info"];
                m_alDicTime[enDicCntryInf] = getTimeStamp();
                
                Json::Value cntryBaseDic = cntryInfoDic["guild_base"];
                
                int last_chat_time = cntryBaseDic["last_chat_time"].asInt();
                if (last_chat_time > CGameData::Inst()->getLocalChatTime()) {
                    CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyUpdateChatInf);
                }
                
                
            }
        }
        
        string strNewTitle = usrInfoDic["new_title_name"].asString();
        if (!strNewTitle.empty()) {
            CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyNewTitle);
        }
        
        if(!dict["user_info"].isNull()){
            CCNotificationCenter::sharedNotifCenter()->postNotification(kNotifyUpdateUsrInf);
        }
    }
	return size*nmemb;
}


bool HttpComm::getIsApiReq()
{
    bool bIsApi = false;
    switch (m_iReqType) {
            //api
        case enReqLogin:
        case enReqCfg:
        case enReqCard:
        case enReqMonster:
        case enReqFrd:
        case enReqEnterDg:
        case enReqEndDgNml:
        case enReqEndDgSpecial:
        case enReqEndDgSangoYY:
        case enReqEndDgWeekly:
        case enReqGetAllCard:
        case enReqCommitTeam:
        case enReqPwUp:
        case enReqEvo:
        case enReqSEvo:
        case enReqWake:
        case enReqSell:
        case enReqAddFriend:
        case enReqFreeGacha:
        case enReqChargeGacha:
        case enReqMultiChargeGacha:
        case enReqMultiFreeGacha:
        case enReqFriendList:
        case enReqAccept:
        case enReqRefuse:
        case enReqSchFriend:
        case enReqDelFriend:
        case enReqReqList:
        case enReqPackExt:
        case enReqShop:
        case enReqCollection:
        case enReqRecoverStamina:
        case enReqRevive:
        case enReqWeiboBack:
        case enReqFrdExtend:
        case enReqCompgachaConfig:
        case enReqExchangeCard:
            
        case enReqCountryCreate:
        case enReqSetViceLeader:
        case enReqDeleteViceLeader:
        case enReqMemberApplyList:
        case enReqMemberList:
        case enReqAcceptMember:
        case enReqRefuseMember:
        case enReqDeleteMember:
        case enReqGiveUpLeader:
        case enReqSetDeclaration:
        case enReqDeleteCountry:
        case enReqUnDeleteCountry:
        case enReqSetNameCard:
        case enReqLevelUpCountry:
        case enReqLevelUpBuff:
        case enReqAssignOneSupply:
        case enReqAssignAllSupply:
        case enReqAssignOutSupply:
        case enReqSearchCountry:
        case enReqGetAllCountries:
        case enReqDeleteMail:
        case enReqDeleteMailAll:
        case enReqSendMail:
        case enReqSendMailAll:
        case enReqRcvMail:
        case enReqQuitCountry:
        case enReqApplyCountry:
        case enReqDonateCoin:
        case enReqExchangeWelfare:
        case enReqRecoverPower:
        case enReqCntryDgCfg:
        case enReqCntryDgRank:
        case enReqCntryDgOpen:
        case enReqCntryCurrentInfo:
        case enReqCntryDgJoin:
        case enReqCntryDgEnd:
        case enReqUseItem:
        case enReqGetGift:
        case enReqSendMsg:
        case enReqSetUserVoice:
        case enReqJiFengRank:
        case enReqLuanWuRank:
        case enReqChangePingbi:
        case enReqUseItemRename:
        case enReqTitleList:
        case enReqSetTitleName:
        case enReqNiePan:
        case enReqGetJiFengRecord:
        case enReqJiLuanWRank:
        case enReqAssignGuildCoin:
        case enReqPostReward:
        case enReqGuildItemShop:
        case enReqBuyGuildItem:
        case enReqRewardTaskList:
        case enReqAcceptRewardTask:
        case enReqRecieveReward:
        case enReqRewardMemberList:
        case enReqRewardTopMember:
        case enReqGetAwardBonus:
        case enReqUseStone:
        case enReqUseCompound:
        case enReqTradeNo:
        case enReqReadMzsm:
        case enReqAssignWeekSupply:
        case enReqBuyCard:
            
        case enReqNewChange:
        case enReqNewGiveWuJiang:
        case enReqPushConfig:
            bIsApi = true;
            break;
            
            //not api
        case enReqBind:
        case enReqPay:
        case enReqNewbie:
        case enReqLanguageBag:
        case enReqChargeAward:
        default:
            bIsApi = false;
            break;
    }
    return bIsApi;
}

bool HttpComm::chkTmd(Json::Value *resp)
{
    bool bRet = false;
    
    bool bIsApi = getIsApiReq();
    
    if(bIsApi == false){
        bRet = true;
        return bRet;
    }
    
    string strTmd = (*resp)["cag"].asString();
    
    if(!strTmd.empty()){
        const char* pBuf = strTmd.c_str();
        
        if(strlen(pBuf) > 7){
            int i = 7;
            int sum = 0;
            
            while (pBuf[i] != 0) {
                if((pBuf[i] >= '0') && (pBuf[i] <= '9')){
                    sum += (pBuf[i] - '0');
                }
                else if((pBuf[i] >= 'a') && (pBuf[i] <= 'f')){
                    sum += (pBuf[i] - 'a' + 10);
                }
                else if((pBuf[i] >= 'A') && (pBuf[i] <= 'F')){
                    sum += (pBuf[i] - 'A' + 10);
                }
                
                i++;
            }
            
            if(sum % 4 == 0){
                bRet = true;
            }
            else{
                char buf[200];
                snprintf(buf, 199, "%d%s%s%s%s%s",
                         (*resp)["server_now"].asInt(),
                         SECRET_CODE_1,
                         SECRET_CODE_2,
                         SECRET_CODE_3,
                         SECRET_CODE_4,
                         SECRET_CODE_5);
                
                string strArgValue = buf;
                
                string strArgValueMd = transToMdStr(strArgValue);
                
                if(strTmd.substr(0, 7).compare(strArgValueMd.substr(0, 7)) == 0){
                    bRet = true;
                }
                else{
                }
            }
        }
    }
    
    return bRet;
}

void HttpComm::saveDict(Json::Value *dict)
{
    switch (m_iReqType) {
        case enReqPPBilloNo:
            ppBilloNoDic = *dict;
            break;
        case enReqInfo:
            {
                long lVer = (*dict)["sup_value"].asInt();
                if (lVer > m_lCfgVer) {
                    m_lCfgVer = lVer;
                    m_bNeedToUpdateCfg = true;
                }
                
                lVer = (*dict)["cup_value"].asInt();
                if (lVer > m_lCardVer) {
                    m_lCardVer = lVer;
                    m_bNeedToUpdateCard = true;
                }
                
                lVer = (*dict)["mup_value"].asInt();
                if (lVer > m_lMonsterVer) {
                    m_lMonsterVer = lVer;
                    m_bNeedToUpdateMonster = true;
                }
            }
            break;
            
        case enReqLogin:
        	loginDic = *dict;
            m_alDicTime[enDicLogin] = getTimeStamp();
            
            if (dict->isMember("award_info"))
            {
                awardRwdInfDic = (*dict)["award_info"];
                
                if (!awardRwdInfDic.empty())
                {
                    if (awardRwdInfDic.isMember("continuous_bonus_record"))
                        awardSignDic = awardRwdInfDic["continuous_bonus_record"];
                    
                    if (awardRwdInfDic.isMember("continuous_tomorrow_bonus"))
                        awardSignTomorrowDic = awardRwdInfDic["continuous_tomorrow_bonus"];
                    
                    if (awardRwdInfDic.isMember("upgrade_bonus_record"))
                        awardLevelUpDic = awardRwdInfDic["upgrade_bonus_record"];
                    
                    if (awardRwdInfDic.isMember("month_bonus_record"))
                        awardMonthDic = awardRwdInfDic["month_bonus_record"];
                    
                    if (awardRwdInfDic.isMember("bonus_record"))
                        awardBonusDic = awardRwdInfDic["bonus_record"];
                    
                    if (awardRwdInfDic.isMember("system_bonus_record"))
                        awardHuodongDic = awardRwdInfDic["system_bonus_record"];
                    
                    if (awardRwdInfDic.isMember("system_compensates_record"))
                        awardBuchangDic = awardRwdInfDic["system_compensates_record"];  
                }
            }
            
            break;
            
        case enReqCfg:
            cfgDic = *dict;
            m_alDicTime[enDicCfg] = getTimeStamp();
            m_lstSpcFlrClosed.clear();
            m_bNeedToUpdateCfg = false;
            
            if (cfgDic.isMember("award_config"))
                awardConfigDic = cfgDic["award_config"];
            
            break;
            
        case enReqCard:
            cardDic = *dict;
            m_alDicTime[enDicCard] = getTimeStamp();
            m_bNeedToUpdateCard = false;
            break;
            
        case enReqMonster:
            monsterDic = *dict;
            m_alDicTime[enDicMonster] = getTimeStamp();
            m_bNeedToUpdateMonster = false;
            break;
            
        case enReqChargeAward:
            chargeAwardDic = *dict;
            break;
            
        case enReqFrd:
            frdDic = *dict;
            m_alDicTime[enDicFrd] = getTimeStamp();
            break;
            
        case enReqEnterDg:
            enterDgDic = *dict;
            
            {
                CommonInfo* pComInf = CGameData::Inst()->getCommonInfo();
                if ((*dict)["is_select_revive"].asBool()) {
                    pComInf->is_select_revive = true;
                }else
                {
                    pComInf->is_select_revive = false;
                }
            }
            
            break;
            
        case enReqUseCompound:
            
            useCompoundDic = *dict;
            
            break;
        case enReqEndDgNml:
            nmlEndDgDic = (*dict)["dungeon_info"]["normal_current"];
            m_alDicTime[enDicNmlDgEnd] = getTimeStamp();
            trophiesDic = *dict;
            break;
            
        case enReqEndDgSangoYY:
            SangoYYEndDgDic = (*dict)["dungeon_info"]["sgyy_current"];
            m_alDicTime[enDicSangoDgEnd] = getTimeStamp();
            
            trophiesDic = *dict;
            break;
            
        case enReqEndDgSpecial:
            specialEndDgDic = (*dict)["dungeon_info"];
            m_alDicTime[enDicSpecialDgEnd] = getTimeStamp();
            trophiesDic = *dict;
            
            {
                if (dict->isMember("limit_dungeon")) {
                    bool bLimitDg = (*dict)["limit_dungeon"].asBool();
                    if(bLimitDg == false){
                        int iFlrId = atoi((*dict)["dungeon_info"]["floor_id"].asString().c_str());
                        m_lstSpcFlrClosed.push_back(iFlrId);
                    }
                }
            }
            break;
            
        case enReqEndDgWeekly:
            weeklyEndDgDic = (*dict)["dungeon_info"];
            m_alDicTime[enDicWeeklyDgEnd] = getTimeStamp();
            trophiesDic = *dict;
            break;
            
        case enReqGetAllCard:
            allCardDic = (*dict)["user_cards"];
            m_alDicTime[enDicAllCard] = getTimeStamp();
            break;
            
        case enReqFriendList:
            friendListDic = *dict;
            m_isFriendListChange = true;
            break;
            
        case enReqCompgachaConfig:
            compGachaDic = (*dict)["compgacha_info"];
            break;
        case enReqExchangeCard:
            exchangeDic = (*dict)["new_card"];
            break;
        case enReqSchFriend:
            schFriendDic = (*dict)["friend_info"];
            break;
            
        case enReqReqList:
            reqListDic = (*dict)["friend_requests"];
            m_alDicTime[enDicFriendApplyList] = getTimeStamp();
            break;
            
        case enReqShop:
            shopDic = (*dict)["shop_info"];
            break;
            
        case enReqCollection:
            collectionDic = (*dict)["collection"];
            m_alDicTime[enDicCollection] = getTimeStamp();
            break;
            
        case enReqNewbie:
            newbieDic = *dict;
            break;
            
        case enReqPwUp:
            pwUpDicSuccessType = (*dict)["success_type"].asString();
            break;
            
        case enReqFreeGacha:
        case enReqChargeGacha:
        case enReqMultiFreeGacha:
        case enReqMultiChargeGacha:
            
            
            if((*dict).isMember("new_cards"))
                gachaDic = (*dict)["new_cards"];
            else
                gachaDic = (*dict)["new_card"];
            
            break;
            
        case enReqEvo:
        case enReqSEvo:
        case enReqWake:
            break;
        case enReqWeiboBack:
            m_strWeiboBack = (*dict)["msg"].asString();
            break;
            
        case enReqGetAllCountries:
            cntryListDic = (*dict)["getsortedguild"];
            m_alDicTime[enDicCntryList] = getTimeStamp();
            break;
        
        case enReqMemberApplyList:
            memberApplyListDic = (*dict)["getapplyguild"];
            break;
            
        case enReqRcvMail:
            rcvMailDic = (*dict)["mail"];
            break;
            
        case enReqMemberList:
            memberListDic = (*dict)["getallguildmemberinfo"];
            m_alDicTime[enDicMemberList] = getTimeStamp();
            break;
        case enReqTradeNo:
            tradeNo = (*dict)["order_id"].asString();
            break;
        case enReqSearchCountry:
            schCntryDic = (*dict)["findguildinfo"];
            break;
            
        case enReqCntryDgCfg:
            CntryDgCfgDic = *dict;
            //printf("CntryDgCfgDic: %s", CntryDgCfgDic.toStyledString().c_str());
            break;
            
        case enReqCntryDgRank:
        case enReqJiFengRank:
        case enReqLuanWuRank:
            CntryDgRankDic = *dict;
            //CCLOG("CntryDgRankDic: %s", CntryDgRankDic.toStyledString().c_str());
            break;
            
        case enReqCntryDgEnd:
            CntryDgEndDic = *dict;
            break;
            
        case enReqUseItem:
        case enReqUseItemRename:
            useItemDic = *dict;
            break;
            
        case enReqGetGift:
            giftDic = (*dict)["data"];
            break;
            
        case enReqExchangeWelfare:
            gachaDic = (*dict)["new_card"];
            break;
            
        case enReqCntryCurrentInfo:
            cntryCurrentInfo = (*dict)["get_guild_notice"].asString();
            break;
            
        case enReqTitleList:
            titleListDic = *dict;
            break;
            
        case enReqGetJiFengRecord:
            jifengRecordDic = *dict;
            break;
            
        case enReqNiePan:
            niepanResultUcid = (*dict)["new_card_info"]["ucid"].asString();
            break;
            
        case enReqJiLuanWRank:
            CntryJFDgRankDic = (*dict)["jf"];
            CntryLWDgRankDic = (*dict)["lw"];
            break;
            
        case enReqUploadVideo:
            upload_record_award = *dict;
            break;
            
        case enReqPostReward:
    
            break;
            
        case enReqNewChange:
            
            break;
            
        case enReqNewGiveWuJiang:
            
            break;
            
        case enReqGuildItemShop:
            guildItemShopDic = *dict;
            break;
            
        case enReqRewardTaskList:
            rewardTaskDic = (*dict)["task_list"];
            break;
            
        case enReqRewardMemberList:
            rewardMemberDic = (*dict)["member"];
            break;
            
        case enReqRewardTopMember:
            rewardTopMemberDic = (*dict)["top"];
            break;
            
        case enReqViewOtherRecord:
            view_other_record = *dict;
            break;
            
        case enReqAppendCsot:
        {
            appendCost = *dict;
            break;
        }
        case enReqGetAwardBonus:
            if (dict->isMember("continuous_bonus_record"))
                awardSignDic = (*dict)["continuous_bonus_record"];
            
            if (dict->isMember("continuous_tomorrow_bonus"))
                awardSignTomorrowDic = (*dict)["continuous_tomorrow_bonus"];
            
            if (dict->isMember("upgrade_bonus_record"))
                awardLevelUpDic = (*dict)["upgrade_bonus_record"];
            
            if (dict->isMember("month_bonus_record"))
                awardMonthDic = (*dict)["month_bonus_record"];
            
            if (dict->isMember("bonus_record"))
                awardBonusDic = (*dict)["bonus_record"];
            
            if (dict->isMember("system_bonus_record"))
                awardHuodongDic = (*dict)["system_bonus_record"];
            
            if (dict->isMember("system_compensates_record"))
                awardBuchangDic = (*dict)["system_compensates_record"];
            
            break;
            
        case enReqBuyCard:
            
            break;
            
        default:
            break;
    }
    
    if((*dict)["max_num"].asInt()!=0){
        m_iMaxFrNum = (*dict)["max_num"].asInt();
    }
    
    if((*dict)["guild_coin"].asInt()!=0){
        m_lGuildCoin = (*dict)["guild_coin"].asInt();
    }
}


#pragma mark -
#pragma mark Asynchronous request
//******************************************************************************
// Asynchronous request
//******************************************************************************

string HttpComm::getSameRequestForm()
{
    adaptiphone();
    createFmd();
    
    string postbody("");
    
    postbody.append(setPostValue("pid", m_strUserid));
    postbody.append(setPostValue("sessionid", m_strSessionid));
    postbody.append(setPostValue("version", string(kLocalAppVer)));
    postbody.append(setPostValue("org", m_strMdnTime));
    postbody.append(setPostValue("orf", m_strFmd));
    postbody.append(setPostValue("timestamp", getTimeStampStr()));
    postbody.append(setPostValue("arg", getArgValue()));
    postbody.append(setPostValue("sig", randString()));
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
    postbody.append(setPostValue("device_platform",kPltfmAndroid));
    #else
    postbody.append(setPostValue("device_platform", kPltfmIos));
    #endif
    
    
    const char* szMacAddr = OcProxy::Inst()->getMacAddr();
    if (szMacAddr)
    {
        postbody.append(setPostValue("macaddr",szMacAddr));
    }
    
    if (!m_strUUID.empty()) {
        postbody.append(setPostValue("uuid", m_strUUID));
    }
    
    return postbody;
}

bool HttpComm::checkSessionId()
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return false;
    
    if (m_lServerNow == 0) {
        reqInfo();
    }
    
    if (m_lServerNow == 0) {
        return false;
    }
    
    return true;
}

// reqLanguageBag
bool HttpComm::reqLanguageBag()
{
    m_iReqType = enReqLanguageBag;
    
    string url(netServerURL);
    url +="language_version/";
    
    string postBody("");
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
    
    return true;
}

void HttpComm::ReqInfoAsync()
{
    m_iReqType = enReqInfo;
    
    string url(netServerURL);
    url +="info/?";

    AsyncConnect(url, procResponseStaticFunc, "");
}

void HttpComm::loginRequest()
{    
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqLogin;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.index"));
    postBody.append(getSameRequestForm());
    
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cfgRequest()
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqCfg;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.get_config_ex"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqCard()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCard;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.get_card_config"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqMonster()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqMonster;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.get_monster_config"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqChargeAward()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqChargeAward;
    
    
    string url(netServerURL);
    url +="charge_award_info/?";
    
    
    string postBody("");
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::frdRequest()
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqFrd;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.help_me"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getAllCardReq()
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqGetAllCard;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.get_all"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}


//void HttpComm::frdRequest(string cStr)
//{
//    if (checkSessionId() == false) {
//        return;
//    }
//    
//    m_iReqType = enReqFrd;
//    
//    string url(netServerURL);
//    url +="api/?";
//    
//    string postBody("");
//    postBody.append(setPostValue("method","friend.help_me"));
//    postBody.append(setPostValue("rty", cStr));
//    postBody.append(getSameRequestForm());
//    
//    AsyncConnect(url, procResponseStaticFunc, postBody);
//}
//
//
//
//void HttpComm::getAllCardReq(string cStr)
//{
//    if (checkSessionId() == false) {
//        return;
//    }
//    
//    m_iReqType = enReqGetAllCard;
//    
//    string url(netServerURL);
//    url +="api/?";
//    
//    string postBody("");
//    postBody.append(setPostValue("method","card.get_all"));
//    postBody.append(setPostValue("rty", cStr));
//    postBody.append(getSameRequestForm());
//    
//    AsyncConnect(url, procResponseStaticFunc, postBody);
//}




void HttpComm::commitTeamReq(string new_deck)
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqCommitTeam;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.set_decks"));
    postBody.append(setPostValue("new_deck",new_deck));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::powerUpReq(string base_ucid, string foods)
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqPwUp;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.update"));
    postBody.append(setPostValue("base_ucid",base_ucid));
    postBody.append(setPostValue("cost_ucids",foods));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::niepanReq(string base_ucid, string foods)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqNiePan;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.card_np"));
    postBody.append(setPostValue("base_ucid",base_ucid));
    postBody.append(setPostValue("cost_ucids",foods));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::evolutionReq(string base_ucid, string foods)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqEvo;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.upgrade"));
    postBody.append(setPostValue("base_ucid",base_ucid));
    postBody.append(setPostValue("cost_ucids",foods));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sevoReq(string base_ucid, string foods)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqSEvo;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.evolution"));
    postBody.append(setPostValue("base_ucid",base_ucid));
    postBody.append(setPostValue("cost_ucids",foods));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::wakeReq(string base_ucid, string foods)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqWake;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.wake"));
    postBody.append(setPostValue("base_ucid",base_ucid));
    postBody.append(setPostValue("cost_ucids",foods));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::titleListReq()
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqTitleList;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","sgtitle.get_title_progress"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::setTitleNameReq(string title_id)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqSetTitleName;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","sgtitle.set_title_name"));
    postBody.append(setPostValue("title_id",title_id));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::postRewardReq(long lContribution, int iDeadline, int iPerson,
                             int iCoin, string itemId, int itemCount)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqPostReward;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.add_guild_task"));
    
    char buf[32];
    snprintf(buf, 31, "%ld", lContribution);
    postBody.append(setPostValue("contribution", string(buf)));
    
    snprintf(buf, 31, "%d", iDeadline);
    postBody.append(setPostValue("deadline", string(buf)));
    
    snprintf(buf, 31, "%d", iPerson);
    postBody.append(setPostValue("person", string(buf)));
    
    snprintf(buf, 31, "%d", iCoin);
    postBody.append(setPostValue("coin", string(buf)));
    
    Json::Value rewardDic;
    rewardDic[itemId] = itemCount;
    postBody.append(setPostValue("award", rewardDic.toStyledString()));
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::rewardTaskListReq()
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqRewardTaskList;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.get_task_list"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::rewardTopMemberReq()
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqRewardTopMember;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.task_top_member"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::acceptRewardTaskReq(string taskId)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqAcceptRewardTask;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.accept_guild_task"));
    postBody.append(setPostValue("task_id", taskId));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::recieveRewardReq(string taskId)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqRecieveReward;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.task_award"));
    postBody.append(setPostValue("task_id", taskId));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::rewardMemberListReq(string taskId)
{
    if(m_strSessionid.empty())
        reqSessionid();
    
    if(m_strSessionid.empty())
        return;
    
    m_iReqType = enReqRewardMemberList;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.get_task_member_list"));
    postBody.append(setPostValue("task_id", taskId));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sellReq(string sell_ucid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSell;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.sell"));
    postBody.append(setPostValue("sell_ucids",sell_ucid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::addFriendReq(string fid)
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqAddFriend;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.add_request"));
    postBody.append(setPostValue("fid",fid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::freeGachaReq()
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqFreeGacha;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","gacha.free"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

//void HttpComm::chargeGachaReq(bool bSuiTang, string strItem)
//{
//    if (checkSessionId() == false) {
//        return;
//    }
//    
//    m_iReqType = enReqChargeGacha;
//    
//    string url(netServerURL);
//    url +="api/?";
//    
//    string postBody("");
//    postBody.append(setPostValue("method","gacha.charge"));
//   
//
//    if (bSuiTang)
//        postBody.append(setPostValue("st_gacha", "1"));
//    
//    if (!strItem.empty()) {
//        postBody.append(setPostValue("item", strItem));
//        postBody.append(setPostValue("gacha_item", "1"));
//    }
//    postBody.append(getSameRequestForm());
//    
//    AsyncConnect(url, procResponseStaticFunc, postBody);
//}


//void HttpComm::chargeGachaReq_Other(int OtherGacha, string strItem)
//{
//    if (checkSessionId() == false) {
//        return;
//    }
//    
//    m_iReqType = enReqChargeGacha;
//    
//    string url(netServerURL);
//    url +="api/?";
//    
//    string postBody("");
//    postBody.append(setPostValue("method","gacha.charge"));
//    
//
//
//
//    if (OtherGacha == 1)
//        postBody.append(setPostValue("hero_1_gacha", "1"));
//
//    if (OtherGacha == 2)
//        postBody.append(setPostValue("hero_2_gacha", "1"));
//
//    if (OtherGacha == 3)
//        postBody.append(setPostValue("hero_3_gacha", "1"));
//
//    
//    
//    if (!strItem.empty()) {
//        postBody.append(setPostValue("item", strItem));
//        postBody.append(setPostValue("gacha_item", "1"));
//    }
//    postBody.append(getSameRequestForm());
//    
//    AsyncConnect(url, procResponseStaticFunc, postBody);
//}


void HttpComm::chargeGachaReq_Other(int OtherGacha, string strItem,string cStr, bool free)  //新增
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqChargeGacha;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    
    if(free)
        postBody.append(setPostValue("method","gacha_shop.cd_free"));
    else
    postBody.append(setPostValue("method","gacha.charge"));
    
    if (!cStr.empty()) {
        postBody.append(setPostValue("rty", cStr));
    }
    if (OtherGacha == 1)
        postBody.append(setPostValue("hero_1_gacha", "1"));
    
    if (OtherGacha == 2)
        postBody.append(setPostValue("hero_2_gacha", "1"));
    
    if (OtherGacha == 3)
        postBody.append(setPostValue("hero_3_gacha", "1"));
    
    
    
    if (!strItem.empty()) {
        postBody.append(setPostValue("item", strItem));
        postBody.append(setPostValue("gacha_item", "1"));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::chargeGachaReq(bool bSuiTang, string strItem,string cStr, bool free)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqChargeGacha;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    
    
    if(free)
        postBody.append(setPostValue("method","gacha_shop.cd_free"));
    else
        postBody.append(setPostValue("method","gacha.charge"));
    
    
    if (!cStr.empty()) {
       postBody.append(setPostValue("rty", cStr));
    }
   
    if (bSuiTang)
        postBody.append(setPostValue("st_gacha", "1"));
    
    if (!strItem.empty()) {
        postBody.append(setPostValue("item", strItem));
        postBody.append(setPostValue("gacha_item", "1"));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}


void HttpComm::multiChargeGachaReq(string strItem,string cStr)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqMultiChargeGacha;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","gacha.charge_multi"));
    if (!cStr.empty()) {
        postBody.append(setPostValue("rty", cStr));
    }
    if (!strItem.empty()) {
        postBody.append(setPostValue("item", strItem));
        postBody.append(setPostValue("gacha_multi_item", "1"));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::multiFreeGachaReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqMultiFreeGacha;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","gacha.free_multi"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::compGachaConfigReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCompgachaConfig;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","compgacha.get_compgacha_info"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::exchangeCardReq(string time, string number)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqExchangeCard;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","compgacha.exchange_card"));
    postBody.append(setPostValue("action_key",time));
    postBody.append(setPostValue("action_rule",number));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}




void HttpComm::friendListReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqFriendList;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.get_friend_list"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::newGiveWuJiangReq(string str)//1:强化加武将，2：战败给武将
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqNewGiveWuJiang;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.tutorial_add_cards"));
    
    postBody.append(setPostValue("add_type",str));
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}
void HttpComm::newTypeChangeReq(string str)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqNewChange;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.tutorial_records"));
    postBody.append(setPostValue("data",str));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::acceptRequest(string fid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAccept;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.accept_request"));
    postBody.append(setPostValue("fid",fid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::refuseRequest(string fid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqRefuse;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.refuse_request"));
    postBody.append(setPostValue("fid",fid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::delFriendRequest(string fid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDelFriend;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.del_friend"));
    postBody.append(setPostValue("fid",fid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::schFriendRequest(string fid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSchFriend;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.search_friend"));
    postBody.append(setPostValue("fid",fid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqListRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqReqList;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.get_request_list"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void  HttpComm::frExtend()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqFrdExtend;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","friend.extend"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);

}

void HttpComm::askCostMonthCard()
{
    if (!checkSessionId())return;
    m_iReqType = enReqAppendCsot;
    
    string url(netServerURL);
    url +="api/?";
    string postBody("");
    postBody.append(setPostValue("method", "guild.cost_up"));
    postBody.append(getSameRequestForm());
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::packExtendRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqPackExt;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.extend_num"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::shopRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqShop;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.get_shop"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::readMzsm()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqReadMzsm;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.is_mzsm"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::collectionRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCollection;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","card.get_collection"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::recoverStaminaRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqRecoverStamina;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","dungeon.recover_stamina"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::payRequest(string strData, string iapid)
{
    CCLog("pay request iapid = %s", iapid.c_str());
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqPay;
    
    char buf[10];
    const char* pcIap = iapid.c_str();
    int iLen = strlen(pcIap);
    buf[0] = pcIap[iLen - 2];
    buf[1] = pcIap[iLen - 1];
    buf[2] = 0;
    
    string strIap = buf;
    
    string url(netServerURL);
    url +="charge_result/?";
    
    replacePlus(strData);
    
    string postBody("");
    postBody.append(setPostValue("receipt", strData));
    postBody.append(setPostValue("iap", strIap));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqPPBilloNo(const char* item_id)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqPPBilloNo;
    
    string url(netServerURL);
    url +="generate_billno/?";
    
    string postBody("");
    
    postBody.append(getSameRequestForm());
    postBody.append(setPostValue("item_id", item_id));
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}


void HttpComm::newbieRequest(int country, int iStep, string strUcid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqNewbie;
    
    string url(netServerURL);
    url +="tutorial/?";
    
    string postBody("");
    
    char buf[100];
    snprintf(buf, 99, "%d", iStep);
    string strStep = buf;
    postBody.append(setPostValue("step", strStep));
    
    if (iStep == 1) {
        snprintf(buf, 99, "%d", country);
        string strCountry = buf;
        postBody.append(setPostValue("country", strCountry));
    }
    
    if(!strUcid.empty())
        postBody.append(setPostValue("ucid", strUcid));
    
    snprintf(buf, 99, "%d", true);
    postBody.append(setPostValue("is_tutorial", string(buf)));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::newbieRequestPwUp(int iStep, string strBaseUcid, string strCostUcids)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqNewbie;
    
    string url(netServerURL);
    url +="tutorial/?";
    
    string postBody("");
    
    char buf[100];
    snprintf(buf, 99, "%d", iStep);
    string strStep = buf;
    postBody.append(setPostValue("step", strStep));
    postBody.append(setPostValue("base_ucid", strBaseUcid));
    postBody.append(setPostValue("cost_ucids", strCostUcids));
    
    snprintf(buf, 99, "%d", true);
    postBody.append(setPostValue("is_tutorial", string(buf)));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reviveRequest(string rty,string strReviveCostId)
{
    if (checkSessionId() == false) {
        return;
    }

    m_iReqType = enReqRevive;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","dungeon.revive"));
    postBody.append(setPostValue("rty", rty));
    if (!strReviveCostId.empty()) {
        postBody.append(setPostValue("item_id", strReviveCostId));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqEnterDungeon(string dg_type, string flrid, string rid, string costcoin,
                               string hprid, string hp_ucid, string hptp, string rty,
                               string new_deck, string helper_deck)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqEnterDg;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","dungeon.start"));
    
    postBody.append(setPostValue("helper_deck", helper_deck));
    postBody.append(setPostValue("dungeon_type", dg_type));
    postBody.append(setPostValue("floor_id", flrid));
    postBody.append(setPostValue("room_id", rid));
    postBody.append(setPostValue("helper_id", hprid));
    postBody.append(setPostValue("helper_ucid", hp_ucid));
    postBody.append(setPostValue("help_type", hptp));
    postBody.append(setPostValue("rty", rty));
    if (!new_deck.empty())
        postBody.append(setPostValue("new_deck",new_deck));
    
    if (!costcoin.empty())
        postBody.append(setPostValue("coin",costcoin));

    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::dgEndRequest(string dg_type, string flrid, string rid, string dgUid, string score)
{
    if (checkSessionId() == false)
    {
        m_iReqStat = kReqConnectErr;
        m_lServerNow = 0;
        return;
    }
    
    if(dg_type.compare("normal") == 0)
        m_iReqType = enReqEndDgNml;
    else if (dg_type.compare("special") == 0)
        m_iReqType = enReqEndDgSpecial;
    else if (dg_type.compare("weekly") == 0)
        m_iReqType = enReqEndDgWeekly;
    else if (dg_type.compare("sgyy") == 0) 
    {
        m_iReqType = enReqEndDgSangoYY; 
    }
    else
        return;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","dungeon.end"));
    postBody.append(setPostValue("dungeon_type", dg_type));
    postBody.append(setPostValue("floor_id", flrid));
    postBody.append(setPostValue("room_id", rid));
    postBody.append(setPostValue("dungeon_uid", dgUid));
    postBody.append(setPostValue("score", score));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

/*
 *获取是否有战败返体效果的请求
 */
void HttpComm::dgEndForFanTiRequest(string dg_type, string flrid, string rid, string dgUid, string score)
{
    if (checkSessionId() == false)//添加请求--
    {
        m_iReqStat = kReqConnectErr;
        m_lServerNow = 0;
        return;
    }
    
    if(dg_type.compare("normal") == 0)
        m_iReqType = enReqEndDgNml;
    else if (dg_type.compare("special") == 0)
        m_iReqType = enReqEndDgSpecial;
    else if (dg_type.compare("weekly") == 0)
        m_iReqType = enReqEndDgWeekly;
    else if (dg_type.compare("sgyy") == 0)
    {
        m_iReqType = enReqEndDgSangoYY;
    }
    else
        return;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","dungeon.dungeon_failure")); //添加请求--
    postBody.append(setPostValue("dungeon_type", dg_type));
    postBody.append(setPostValue("floor_id", flrid));
    postBody.append(setPostValue("room_id", rid));
    postBody.append(setPostValue("dungeon_uid", dgUid));
    postBody.append(setPostValue("score", score));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}


void HttpComm::createCountryRequest(string name, string declaraion)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCountryCreate;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.createguild"));
    postBody.append(setPostValue("gname", name));
    if (declaraion.empty() == false) {
        postBody.append(setPostValue("gdeclaration", declaraion));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
    
}

void HttpComm::setViceLeaderRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSetViceLeader;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.setsecondleader"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::changeMemberPingbi(string uid,bool pingbi)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqChangePingbi;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    
    if (pingbi)
    {
        postBody.append(setPostValue("method", "guild.set_user_screen"));
    }
    else
    {
        postBody.append(setPostValue("method", "guild.set_user_unscreen"));
    }
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::setUserVoiceOnOff(string uid,bool voiceOn)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSetUserVoice;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    
    if (voiceOn)
    {
      postBody.append(setPostValue("method", "guild.set_user_voice"));
    }
    else
    {
      postBody.append(setPostValue("method", "guild.set_user_unvoice"));
    }
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::deleteViceLeaderRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDeleteViceLeader;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.deletesecondleader"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getMemberApplyListRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqMemberApplyList;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.getapplyguild"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getMemberListRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqMemberList;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.getallguildmemberinfo"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::acceptMemberRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAcceptMember;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.addmember"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::refuseMemberRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqRefuseMember;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.rejectmember"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::deleteMemberRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDeleteMember;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.deletemember"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::giveUpLeaderRequest(string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGiveUpLeader;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.giveupleader"));
    postBody.append(setPostValue("uid", uid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::setDeclarationRequest(string declaration)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSetDeclaration;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.setguilddeclaration"));
    postBody.append(setPostValue("declaration", declaration));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::deleteCountryRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDeleteCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.deleteguild"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::setNameCardRequest(string name)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSetNameCard;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.setGuildUserName"));
    postBody.append(setPostValue("name", name));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::unDeleteCountryRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUnDeleteCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.undeleteguild"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::levelUpCountryRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqLevelUpCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.upgradeguild"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::levelUpBuffRequest(int buffType)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqLevelUpBuff;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.upgradebuf"));
    char buf[100];
    snprintf(buf, 99, "%d", buffType + 1);
    string strBuffType(buf);
    postBody.append(setPostValue("type", strBuffType)); // "1":"buff_power", "2":"buff_attack", "3":"buff_recover", "4":"buff_hp"
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::assignOneSupplyRequest(string assignDic)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAssignOneSupply;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.assignwealth"));
    postBody.append(setPostValue("assign", assignDic));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::assignAllSupplyRequest(long supply)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAssignAllSupply;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.averageassignwealth"));
    char buf[100];
    snprintf(buf, 99, "%ld", supply);
    string strSupply(buf);
    postBody.append(setPostValue("assign", strSupply));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::assignOutSupplyRequest(long supply)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAssignOutSupply;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.assignWealthBycontribute"));
    char buf[100];
    snprintf(buf, 99, "%ld", supply);
    string strSupply(buf);
    postBody.append(setPostValue("wealth", strSupply));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::buyCard(short sCid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqBuyCard;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "gacha_shop.gacha_cards"));
    
    char buf[100];
    snprintf(buf, 99, "%d_card", sCid);
    postBody.append(setPostValue("cid", string(buf)));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::assignWeekSupplyRequest(long supply)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAssignWeekSupply;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.assignwealth_week_contribution"));
    char buf[100];
    snprintf(buf, 99, "%ld", supply);
    string strSupply(buf);
    postBody.append(setPostValue("wealth", strSupply));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::searchCountryRequest(string cidOrName)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSearchCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.findguildinfo"));
    postBody.append(setPostValue("guildidname", cidOrName));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getAllCountriesRequest(int type)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGetAllCountries;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.getsortedguild"));
    char buf[32];
    snprintf(buf, 31, "%d", type);
    string strType(buf);
    postBody.append(setPostValue("type", strType));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void  HttpComm::deleteMail(string  uid, long  timestamp)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDeleteMail;
    
    char buf[100];
    snprintf(buf, 99, "%ld", timestamp);
    string strTimeStamp(buf);
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.deletemail"));
    postBody.append(setPostValue("uid",uid));
    postBody.append(setPostValue("datetime", strTimeStamp));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::DeleteMailAll()
{
    m_iReqType = enReqDeleteMailAll;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","guild.delete_all_mail"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sendMailRequest(string uid, string msg)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSendMail;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.sendmail"));
    postBody.append(setPostValue("id", uid));
    postBody.append(setPostValue("mail_msg", msg));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sendMailAllRequest(string msg)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSendMailAll;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.sendguildmail"));
    postBody.append(setPostValue("mail_msg", msg));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::rcvMailRequest(long lTime)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqRcvMail;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.receivemail"));
    
    char buf[32];
    snprintf(buf, 31, "%ld", lTime);
    postBody.append(setPostValue("datetime", buf));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::quitCountryRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqQuitCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.quitguild"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::applyCountryRequest(string cid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqApplyCountry;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.applyguild"));
    postBody.append(setPostValue("gid", cid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::donateCoinRequest(int coin)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqDonateCoin;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.contributecoin"));
    char buf[100];
    snprintf(buf, 99, "%d", coin);
    string strCoin(buf);
    postBody.append(setPostValue("contribute", strCoin));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::exchangeWelfareRequest(string wid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqExchangeWelfare;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.chargewealth"));
    postBody.append(setPostValue("id", wid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::recoverPowerRequest()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqRecoverPower;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.recoverpower"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryDgCfgReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCntryDgCfg;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_guild_dungeon_config"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryJiFengRankReq(string dg_id)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqJiFengRank;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_max_quick_dungeon"));
    postBody.append(setPostValue("dungeon_id", dg_id));
    
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryLuanWuRankReq(string dg_id)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqLuanWuRank;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_less_time_dungeon"));
    postBody.append(setPostValue("dungeon_id", dg_id));
    
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryDgRankReq(string dg_id)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCntryDgRank;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_sorted_guildcontribute"));
    postBody.append(setPostValue("dungeon_id", dg_id));
    
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryDgOpenReq(string dg_id,bool bIsTongMEnglianShou)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCntryDgOpen;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    if (bIsTongMEnglianShou)
        postBody.append(setPostValue("method", "guild_ex.opendungeon"));
    else
        postBody.append(setPostValue("method", "guild.opendungeon"));
    
    postBody.append(setPostValue("dungeon_id", dg_id));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}




void HttpComm::getCntryCurrentInfo()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCntryCurrentInfo;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_guild_notice"));
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryDgJoinReq(string dg_id, string rtyid, string new_deck,bool bIsTongMEnglianShou)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqCntryDgJoin;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    if (bIsTongMEnglianShou)
         postBody.append(setPostValue("method", "guild_ex.startdungeon"));
    else
         postBody.append(setPostValue("method", "guild.startdungeon"));
    
    postBody.append(setPostValue("dungeon_id", dg_id));
    postBody.append(setPostValue("enter_rty", rtyid));
    if (!new_deck.empty())
        postBody.append(setPostValue("new_deck",new_deck));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryDgEndReq(string dg_id, string combo,bool bIsTongMEnglianShou)
{
    if (checkSessionId() == false)
    {
        m_iReqStat = kReqConnectErr;
        m_lServerNow = 0;
        return;
    }
    
    m_iReqType = enReqCntryDgEnd;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    if (bIsTongMEnglianShou)
        postBody.append(setPostValue("method", "guild_ex.enddungeon"));
    else
        postBody.append(setPostValue("method", "guild.enddungeon"));
    
    postBody.append(setPostValue("dungeon_id", dg_id));
    
    string aesCombo = OcProxy::Inst()->AesEncript(combo.c_str());
    replacePlus(aesCombo);
    postBody.append(setPostValue("combo", aesCombo));
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::useItemCardRename(string strItem, string newName)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUseItemRename;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.consume_guild_item"));
    postBody.append(setPostValue("item", strItem));
    
    long lCnt = 1;
    char buf[10];
    snprintf(buf, 9, "%ld", lCnt);
    string strCnt = buf;
    postBody.append(setPostValue("num", strCnt));
    postBody.append(setPostValue("new_name", newName));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::useItemReq(string strItem, long lCnt)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUseItem;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.consume_guild_item"));
    postBody.append(setPostValue("item", strItem));
    
    char buf[10];
    snprintf(buf, 9, "%ld", lCnt);
    string strCnt = buf;
    postBody.append(setPostValue("num", strCnt));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}


void HttpComm::useCompoundReq(string type, string newID)
{
    
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUseCompound;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.compound_composite"));
    postBody.append(setPostValue("compound", type));
    
    postBody.append(setPostValue("compound_id", newID));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
    
}
void HttpComm::useStoneReq(string strItem, long lCnt)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUseStone;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.contribute_stone"));
    postBody.append(setPostValue("item", strItem));
    
    char buf[10];
    snprintf(buf, 9, "%ld", lCnt);
    string strCnt = buf;
    postBody.append(setPostValue("num", strCnt));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

//void HttpComm::useItemAppendCsot(const char *pcItem, long lCnt)
//{
//    if (checkSessionId() == false) {
//        return;
//    }
//    
//    m_iReqType = enReqUseItem;
//    
////    string url(netServerURL);
////    url += "api/?";
////    
////    string postBody("");
////    postBody.append(setPostValue("method", "guild.consume_guild_item"));
////    postBody.append(setPostValue("item", pcItem));
////    
////    char buf[10];
////    snprintf(buf, 9, "%ld", lCnt);
////    string strCnt = buf;
////    postBody.append(setPostValue("num", strCnt));
////    postBody.append(getSameRequestForm());
////    
////    AsyncConnect(url, procResponseStaticFunc, postBody);
//}

void HttpComm::getGiftReq(string strCDKey)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGetGift;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_guild_gift"));
    postBody.append(setPostValue("gift_code", strCDKey));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sendMsg(string strMsg)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqSendMsg;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.add_chat_record"));
    postBody.append(setPostValue("msg", strMsg));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::cntryJiFLuanWRankReq(string dg_id)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqJiLuanWRank;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_dungeon_top_record"));
    postBody.append(setPostValue("dungeon_id", dg_id));
    
    if(OcProxy::Inst()->isIpadDevice()){
        postBody.append(setPostValue("device_type","0"));
    }
    else{
        postBody.append(setPostValue("device_type","1"));
    }
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getJiFengRecordReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGetJiFengRecord;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.special_dungeon_record"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqGuildItemShop()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGuildItemShop;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.get_guild_item_shop"));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqBuyGuildItem(string strItemId, long lNum)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqBuyGuildItem;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.buy_guild_task"));
    postBody.append(setPostValue("item", strItemId));
    
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", lNum);
    postBody.append(setPostValue("num", buf));
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::reqGetAwardBonus(string bonus, string award_key,string loginDays)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqGetAwardBonus;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "main.receive_award"));
    postBody.append(setPostValue("bonus", bonus));
    postBody.append(setPostValue("award_key", award_key));
    if (loginDays!=""&&loginDays!="0") {
        postBody.append(setPostValue("login_days", loginDays));
    }
    postBody.append(getSameRequestForm());
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::assignGuildCoin(string strUid, int iCoin)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqAssignGuildCoin;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "guild.assign_guild_coin"));
    
    Json::Value assignDic;
    assignDic[strUid] = iCoin;
    postBody.append(setPostValue("guild_coin", assignDic.toStyledString()));
    postBody.append(setPostValue("uid", strUid));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::getOrderNo(string productId,string uid)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqTradeNo;
    
#if defined (alipay_falseprice)
    string url("http://42.121.15.153:9139/");
#else
    string url("http://ocpay.negaplay.com/");

#endif
    
    url += "aliorder_result/?";
    
    string postBody("");
    postBody.append(setPostValue("product_id", productId));
    postBody.append(setPostValue("app_id", "nzsg01"));
    postBody.append(setPostValue("app_uid", m_strUserid));
    postBody.append(setPostValue("platform", m_strPlatform));
    postBody.append(setPostValue("uid", uid));
    //postBody.append(getSameRequestForm());
    
    if (!m_strUUID.empty()) {
        postBody.append(setPostValue("remark", m_strUUID));
    }
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

size_t HttpComm::reqAccountBindStaticFunc(void *buffer, size_t size , size_t nmemb,void *pf)
{
    return(static_cast<HttpComm*>(pf)->reqAccountBind_parsedata(buffer,size,nmemb));
    
}

size_t HttpComm::reqAccountBind_parsedata(void *buffer, size_t size , size_t nmemb)
{

    string parseData((char *)buffer);

	Json::Value root;
	Json::Reader reader;
    
    bool parsedSuccess = reader.parse(parseData, root, false);
    
	if ( !parsedSuccess ){
	   	m_iReqStat = kReqConnectErr;
        
        if (parseData.find("python") != string::npos) {
            char szErr[100];
            snprintf(szErr, 99, "err: %d.py", m_iReqType);
            setLocalErrStr(szErr);
        }
	    return 0;
	}
    
    int rc = root["rc"].asInt();
    
    if(rc == 0){
        OcProxy::Inst()->setValueForKey(m_strPlatform.c_str(), "platform");
        OcProxy::Inst()->setValueForKey(m_strOpenId.c_str(), "openid");
        OcProxy::Inst()->setValueForKey(m_strToken.c_str(), "access_token");
        OcProxy::Inst()->synchronizeDefaultData();
        m_iReqStat = kReqStatOk;
    }
    else{
        m_strServErrMsg = root["msg"].asString();
        m_iReqStat = rc;
        m_strPlatform = kPltfmOc;
        m_iReqType = 0;

    }

    return (size * nmemb);
}


void  HttpComm::bindRequest(string newOpenid, string newAcessToken ,string newPlatform)
{
    CURL *curl;
	CURLcode res;

    string url(netServerURL);
	url +="account_bind/";
    
    string preOpenid = OcProxy::Inst()->getValueForKey("openid");
    string preAccessToken = OcProxy::Inst()->getValueForKey("access_token");
    
	string postBody("");
    postBody.append(setPostValue("platform",newPlatform));
    postBody.append(setPostValue("openid", newOpenid));
    postBody.append(setPostValue("access_token", newAcessToken));
    postBody.append(setPostValue("bind_openid", preOpenid));
    postBody.append(setPostValue("bind_access_token", preAccessToken));
    postBody.append(getSameRequestForm());
    
    curl = curl_easy_init();
    
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, reqAccountBindStaticFunc);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	
    
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postBody.length());
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postBody.c_str());

    CCLOG("Req: %s%s", url.c_str(), postBody.c_str());
	res = curl_easy_perform(curl);
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &m_lStatusCode);
    
    if((res != CURLE_OK) || (m_lStatusCode != 200)){
        char szErr[100];
        snprintf(szErr, 99, "err: %d.%d.%ld", m_iReqType, res, m_lStatusCode);
        setLocalErrStr(szErr);
    }
    
    if(m_lStatusCode == 502){
        m_iReqStat = 502;
        m_strServErrMsg = OcProxy::Inst()->localizedStringStatic("appCtrlTip1");
    }
    else if(res != CURLE_OK){
        m_iReqStat = kReqConnectErr;
        m_strPlatform = kPltfmOc;
    }
    
	curl_easy_cleanup(curl);
}
void HttpComm::weiboBackReq()
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqWeiboBack;
    
    string url(netServerURL);
    url +="api/?";
    
    string postBody("");
    postBody.append(setPostValue("method","main.weibo_back"));
    
    if (OcProxy::Inst()->returnWeiBoOrWeixinType() == 1) {
        postBody.append(setPostValue("send_type","2"));
        
    }else{
        
        if(getLoginType() == enLoginReqType_SinaLogin)
        {
            postBody.append(setPostValue("send_type","0"));
            
        }else {
            postBody.append(setPostValue("send_type","1"));
        }
        
    }
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
    
    
}

bool HttpComm::getIsFriendListChange()
{
    return m_isFriendListChange;
}

void HttpComm::setIsFriendListChange(bool isChange)
{
    m_isFriendListChange = isChange;
}

string HttpComm::getDecodeStr(void * buffer)
{
    string str((char *)buffer);
    ZBase64 base64;
    string decodeStr = base64.DecodeBase64(str);
    
    return decodeStr;
}

void HttpComm::clearSessionId()
{
    m_strSessionid.clear();
}

// yang---
void HttpComm::clearAllData(){
    
    
    m_strPlatform="";
    m_strOpenId="";
    m_strToken="";
    
    usrInfoDic.clear();
    loginDic.clear();
    cfgDic.clear();
    frdDic.clear();
    allCardDic.clear();
    enterDgDic.clear();
    useCompoundDic.clear();
    nmlEndDgDic.clear();
    specialEndDgDic.clear();
    weeklyEndDgDic.clear();
    SangoYYEndDgDic.clear();
    friendListDic.clear();
    schFriendDic.clear();
    gachaDic.clear();
    reqListDic.clear();
    shopDic.clear();
    collectionDic.clear();
    newbieDic.clear();
    pwUpDicSuccessType.clear();
    niepanResultUcid.clear();
    tradeNo.clear();
    trophiesDic.clear();
    compGachaDic.clear();
    exchangeDic.clear();
    cntryInfoDic.clear();
    memberApplyListDic.clear();
    memberListDic.clear();
    schCntryDic.clear();
    cntryCurrentInfo.clear();

    memset(m_alDicTime, 0, sizeof(long) * enDicMax);
    
    m_lstChatRx.clear();
    m_bNewChatRxCame = false;
    m_lChatRxLastMsgTime = 0;
}

//******************************************************************************
// replacePlus
//******************************************************************************
void HttpComm::replacePlus(string& str)
{
    while(true) {
		string::size_type pos(0);
		if( (pos=str.find("+"))!=string::npos )
			str.replace(pos, 1, "%2b");
		else break;
	}
}

//******************************************************************************
// outputLogFile
//******************************************************************************
void HttpComm::outputLogFile(char* buf, int nLen)
{
    string strPath = CCFileUtils::getWriteablePath().append("errLog.txt");
    
    FILE *pFile = fopen(strPath.c_str(),"a");
    if(pFile){
        string strYmdhms = string("\n\n[").append(OcProxy::Inst()->getYmdhmsFromTimeIntervalSince1970(getTimeStamp(), kOcTimeZone)).append("]\n");
        
        fwrite(strYmdhms.c_str(), strYmdhms.size(), 1, pFile);
        fwrite(buf, nLen, 1, pFile);
        fclose(pFile);
    }
}

void HttpComm::GetUploadAward(ConfigData data)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqUploadVideo;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "dungeon.video_back"));
    
    postBody.append(setPostValue("max_combo", IntToString(data.max_combo)));
    postBody.append(setPostValue("avg_combo", IntToString(data.average_combo)));
    postBody.append(setPostValue("max_hurt", IntToString(data.max_damage)));
    postBody.append(setPostValue("step", IntToString(data.total_rounds)));
    postBody.append(setPostValue("max_cost", IntToString(data.group_general)));
    postBody.append(setPostValue("deck_attack", IntToString(data.group_attack)));
    postBody.append(setPostValue("deck_hp", IntToString(data.group_hp)));
    postBody.append(setPostValue("skid_step", IntToString(data.skill_rounds)));
    postBody.append(setPostValue("dungeon_type", (string)data.fight_type));
    postBody.append(setPostValue("dungeon_id", (string)data.fight_id));
    postBody.append(setPostValue("vid", data.vid));
    postBody.append(setPostValue("video_url", data.video_url));
    postBody.append(setPostValue("img_url", data.img_url));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

string HttpComm::IntToString(unsigned long _int)
{
    char buf[20];
    string rt = "";
    snprintf(buf, 19, "%ld", _int);
    rt = buf;
    return rt;
}

void HttpComm::ReqOtherRecord(const char* dungeon_id, const char* type)
{
    if (checkSessionId() == false) {
        return;
    }
    
    m_iReqType = enReqViewOtherRecord;
    
    string url(netServerURL);
    url += "api/?";
    
    string postBody("");
    postBody.append(setPostValue("method", "dungeon.get_vid_by_dungeon_id"));
    postBody.append(setPostValue("dungeon_id", dungeon_id));
    postBody.append(setPostValue("dungeon_type", type));
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

void HttpComm::sendAppendCostRequest()
{
    if (!checkSessionId())return;
    m_iReqType = enReqAppendCsot;
    
    string url(netServerURL);
    url +="api/?";
    string postBody("");
    postBody.append(setPostValue("method", "guild.cost_up"));
    postBody.append(getSameRequestForm());
    AsyncConnect(url, procResponseStaticFunc, postBody);
}

//void HttpComm::sendMAAarm()
//{
//    if (checkSessionId() == false)
//    {
//        return;
//    }
//    m_iReqType = enReqMAAarm;
//    
//    string url(netServerURL);
//    url += "sgstxt.find_user";
//}


void HttpComm::setPushConfig(bool special_dunegon, bool double_drop, bool guild_charge_cold, int push_stamina_num)
{
    if (checkSessionId() == false) {
        return;
    }
    m_iReqType = enReqPushConfig;
    string url(netServerURL);
    url += "api/?";
    string postBody("");
    postBody.append(setPostValue("method", "main.set_sangopush_config"));
    
    char buf[100];
    snprintf(buf, 99, "%d", special_dunegon);
    postBody.append(setPostValue("special_dunegon", buf));
    snprintf(buf, 99, "%d", double_drop);
    postBody.append(setPostValue("double_drop", buf));
    snprintf(buf, 99, "%d", guild_charge_cold);
    postBody.append(setPostValue("guild_charge_cold", buf));
    snprintf(buf, 99, "%d", push_stamina_num);
    postBody.append(setPostValue("push_stamina_num", buf));
    
    postBody.append(getSameRequestForm());
    
    AsyncConnect(url, procResponseStaticFunc, postBody);
}











