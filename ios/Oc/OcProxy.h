//
//  OcProxy.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_OcProxy_h
#define AgainstWar_OcProxy_h

#include "cocos2d.h"
#include "CommonWebView.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CommonPublishView.h"


using namespace cocos2d;

class OcProxy
{    
public:
    ~OcProxy();
    
    static OcProxy* Inst();
  

public:
    static OcProxy* s_pcIf;
    
    void setDeviceToken(const char* deviceToken);
    string getDeviceToken();
    
    //UI层封装
    //tapjoy
    void setTapjoyUserId(const char *);
    
    //show_web_all
    void showWebWithType(enWebviewLoadType type, const char * htmlstr,
                         CCRect rect, bool bGoToBrowser = false);
    
    void showHtmlStr(const char *string,bool bBottomAlign = false);
    
    void setFrame(CCRect rect);
    
    void removeWebview();
    bool getIsLoadSuccess();
    
    // mail
    void showMailViewController(const char* userName);
    bool getIsHasSentMail();
    const char* getMailText();
    void deleteMailViewController();
    
    // UserDefault
    void setIsLock(const char *ucid, bool isLock);
    bool getIsLock(const char *ucid);
    
    void setBoolValueForKey(bool bValue, const char* key);
    bool getBoolValueForKey(const char* key);
    
    void setIntValueForKey(int bValue, const char* key);
    int getIntValueForKey(const char* key);

    void setFloatValueForKey(float fValue, const char* key);
    float getFloatValueForKey(const char* key);

    void setValueForKey(const char* value, const char* key);
    const char* getValueForKey(const char* key);
    
    void removeObjectForKey(const char* key);
    
    void synchronizeDefaultData();
    
    // OcUtility
    bool isIpadDevice();
    void showUsedMemory();  // 显示当前应用消耗的内存
    void deleteCookies();   // 删除cookies
    void deleteCache();     // 删除缓存
    const char* localizedStringStatic(const char* str); // 从语言包（Localizable.strings文件获取的）中获取语言

    //File path
    const char* getExecutablePath();
    const char* getDocumentPath();
    //void WriteToFile(const char* filepathname, char* buf, int size);
    
    //获取设备型号
    string getMachineType();
    
    //payment
    void loadStore();
    void purchase(int idx);
    int  getStoreStatus();
    void resetStoreStatus();
    int  getPurchaseStatus();
    void resetPurchaseStatus();
    int  getTapjoyStatus();
    void resetTapjoyStatus();
    void finishPayment();
    const char* getReceipt();
    const char* getTransId();
    void addProduct(const char* szIapId,bool isAlipay = false);
    void rmvProduct(const char* szIapId);
    int  getProductCnt();
    const char* getIapIdAtIdx(int idx);
    const char* getPriceAtIdx(int idx);
    const char* getTitleAtIdx(int idx);
    void purAlipay(string tradeNo,string title,int price,bool isTruePrice);
    int getIntPriceAtIdx(int idx);
    //微博
    void showPublishView(enWeiboType weiboType);
    void removePublishView();
    void setPublishViewHidden(bool isHiden);
    const char *getWeiboContent();
    
    void clearOauthData();
    
    //加密
    string AesEncript(const char *str);
    string AesDecript(const char *str);
    //腾讯登陆
    void loginbyTencent();
    int getTencentLoginStatus();
    const char *getTencentOpenId();
    const char *getTencentAccessToken();
    
    //sina login
    void loginBySina();
    int  getSinaLoginStatus();
    const char* getSinaOpenId();
    const char* getSinaAccessToken();
    
    void showHelpInfoWebView(const char * htmlStr);

    //appStore去评价
    void goAppStoreEvaluate();
    //appStore去更新
    void goAppStoreUpdate();
    //发布微博
    void sendWeibo(const char *str,bool isImageExist);
    //获取邀请码
    const char *getInviteCodeWeibo();
    //是否iphone5的尺寸
    bool is568HighScreen();
    //iphone5高出来的高度
    float deltaHeightOf568();
    //设置推送消息
    void setPushInfo(char *pushInfo, long time, const char *valueInfo,bool isLocalTime = false);
    //进入浏览器
    void linkToSafriWithUrlStr(const char *url);
    //显示积分墙
    void showOfferWall();
    
    //getBundleId
    const char* getBundleId();
    
    //getBundleName
    const char* getBundleName();
    
    //从年月日形式的字符串和时区，获得相对1970-1-1的秒数
    long getAbsTimeFromString(const char* szTime, const char* szTimeZone);
    
    //从当前时区的秒数，获得当前年月日
    string getYmdhmsFromTimeIntervalSince1970(long timeSec, const char* szTimeZone);
    string getYmdhmsFromTimeIntervalSince1970(long timeSec);
    
    //get mac address
    const char* getMacAddr();
    
    //get AdId
    const char* getAdId();
    
    // show tips and num
    CCSprite* showTipsAndNum(int num);
    
    // NSLocalizedString
    const char* getLocalizedString(const char* key);
    
    bool checkIsUseExternal(const char* external);
    
    // record screen
    void startRecordScreen();
    void stopRecordScreen();
    void cancelRecordScreen();
    void pauseRecordScreen();
    void resumeRecordScreen();
    
    
    float GetUploadVideoProgress();
    void SaveToAlbum();
    
    void RequestVideoInfo();
    
    
    void InitWithGameUid(const char* _uid);
    
    // 查看他人视频
    void OpenUrl(const char* _url);
    
    void SetPathRecording(const char* _url);
    
    void SaveLastReplayData(ConfigData data, int _type);
    
    bool CheckRecordExist();
    
    ConfigData* GetVideoAtIndex(int index, int type);
    
    void SetUploading(const char* time0, bool uploading);
    void SetUploadSuccess(const char* time0);
    int GetUploadState(const char* time0);
    bool CheckUploading();
    
    bool CheckUploadedUpto10();
    
    void RemovePreviousVideoBehindIndex(int _index);
    
    // get UUID
    string getUUID();
    
    void resetLogoutStatus();
    int getLogoutStatus();
    void showPPCenter();
    void savePpData();
    void PPlogout(); //注销
    int getPpLoginStatus();
    string getPptoken();
    void showPpLogin();
    int getPpCurrentUserId();
    void resetPpLogin();
    int getPpPurchaseStatus(); //pp判断登陆状态
    void resetPpPurchaseStatus();
    void buyPp(int price,const char* billno,const char* title,const char* roleid);
    
private:
    //forbid constructor being called outside
	OcProxy();
    
	//forbid copy constructor being called outside
	OcProxy(const OcProxy& );
	
	//forbid assignment
	OcProxy& operator=(const OcProxy& );
    
    bool m_isGachaWeibo = false;
    
    string device_token;
    
    int m_PPCurrentUserId;
    string m_Pptoken;
    

private:
    
#pragma mark -
#pragma mark -weixin
public:
    
    //初始化微信－－纪录主场景>>>>>可以设置主场景一直查找微信相关信息
    void weixininit(CCObject* target,bool isFight = false);
    // 分享按钮启用按钮 弹出微信／微博sheet－－CCObject* target, SEL_CallFunc selector 跳到微博 ；type 纪录微信息法是类型 默认0为常规发送 1为邀请码发送 
    void sharetoSanguo(CCObject* target, SEL_CallFunc selector,int type=0);
    // 返回发送类型 纪录是选择微信发布还是微博发送－－向服务器发送纪录奖励类型  !!!去替代WXtpe 
    int returnWeiBoOrWeixinType();  
    //设置微信回调 －－微信操作后回调返回参数 根据参数查看是否发送成功，
    void setweixinback(int weixinbacki);
    void setPayBack(int rCode);
    void setIsGachaWeibo(bool isOk);
    
    
};

#endif
