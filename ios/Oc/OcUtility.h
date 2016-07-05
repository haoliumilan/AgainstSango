//
//  OcUtility.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_OcUtility_h
#define AgainstWar_OcUtility_h

#include "cocos2d.h"

struct ConfigData;

using namespace cocos2d;


class OcUtility
{
    string m_weiboStr;
    
public:
    ~OcUtility();
    
    static OcUtility* Inst();
    
    bool isIpadDevice();    // 是否是iPad
    void showUsedMemory();  // 显示当前应用消耗的内存
    void deleteCookies();   // 删除cookies
    void deleteCache();     // 删除缓存
    int  getTapjoyStatus();
    void resetTapjoyStatus();
    string AesEncript(const char *str);
    string AesDecript(const char *str);
    const char *getExecutablePath();    // 获取可执行文件路径
    const char* localizedStringStatic(const char* str); // 从语言包（Localizable.strings文件获取的）中获取语言
    void fullScreenshots();  // 分享截图
    
    void goAppStoreEvaluate();//去appStore评价
    
    void goAppStoreUpdate();//去appStore更新
    
    const char* getInviteCodeWeibo();
    
    bool is568HighScreen();
    
    float deltaHeightOf568();
    
    void setPushInfo(char *pushInfo, long time, const char *valueInfo,bool isLocalTime);

    void setTapjoyUserId(const char *uid);
    
    void linkToSafriWithUrlStr(const char *url);

    void showOfferWall();
    
    const char* getBundleId();
    const char* getBundleName();
    
    //从年月日形式的字符串和时区，获得相对1970-1-1的秒数
    long getAbsTimeFromString(const char* szTime, const char* szTimeZone);
    
    //从当前时区的秒数，获得当前年月日
    string getYmdhmsFromTimeIntervalSince1970(long timeSec, const char* szTimeZone);
    
    //get mac address
    const char* getMacAddr();
    
    //get AdId
    const char* getAdId();
    
    // mail
    void showMailViewController(const char* userName);
    bool getIsHasSentMail();
    const char* getMailText();  // must be use before deleteMailViewController
    void deleteMailViewController();
    
    // NSLocalizedString
    const char* getLocalizedString(const char* key);
    
    bool checkIsUseExternal(const char* external);
    
    //获取设备型号
    string getMachineType();
    
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
    void Openurl(const char* _url);
    
    
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
    
    void onOutsideZhuiAlarm(const char *pchTitle, const char *pchMessage, const char *pchOkText);
    
    string getUUID();
    
    void resetLogoutStatus();
    int getLogoutStatus();
    void showPPCenter();
    void PPlogout();
    int getPpLoginStatus(); //pp判断登陆状态
    void resetPpLogin();
    int getPpPurchaseStatus(); //pp判断登陆状态
    void resetPpPurchaseStatus();
    string getPptoken();    //获得pp token
    void showPpLogin();
    int getPpCurrentUserId();
    void buyPp(int price,const char* billno,const char* title,const char* roleid);

private:

    static OcUtility* s_pcIf;
    
    //forbid constructor being called outside
	OcUtility();
    
	//forbid copy constructor being called outside
	OcUtility(const OcUtility& );
	
	//forbid assignment
	OcUtility& operator=(const OcUtility& );
    
    // has release mailcontroller or not
    bool m_bHasReleaseMCtrl;
  
    void setUUIDToKeyChain(string& strUUID);
    string getUUIDFromKeyChain();
};

#endif
