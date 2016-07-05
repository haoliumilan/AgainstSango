//
//  OcProxy.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "OcProxy.h"
#include "UserDeafault.h"
#include "OcUtility.h"
#include "CPayMM.h"
#include "Sanguoshare.h"
#include "SinaApiMM.h"
#include "TencentApiMM.h"
#include "TextNode.h"
#include "RecordData.h"

OcProxy* OcProxy::s_pcIf = NULL;

OcProxy::OcProxy()
{
    device_token = "";
}

OcProxy::~OcProxy()
{

}

OcProxy* OcProxy::Inst()
{
    if(s_pcIf == NULL){
        s_pcIf = new OcProxy();
    }
    
    return s_pcIf;
}

void OcProxy::setDeviceToken(const char* deviceToken)
{
    device_token = string(deviceToken);
}

string OcProxy::getDeviceToken()
{
    return device_token;
}

void OcProxy::setTapjoyUserId(const char* userId)
{
    OcUtility::Inst()->setTapjoyUserId(userId);
}

void OcProxy::showWebWithType(enWebviewLoadType type, const char *htmlstr,
                              cocos2d::CCRect rect, bool bGoToBrowser)
{
    CCRect newRect;
    if(isIpadDevice())
    {
        newRect = CCRect(rect.origin.x + 64, rect.origin.y + 32, rect.size.width, rect.size.height);
    }
    else
    {
        int a = 0;
        if(OcProxy::Inst()->is568HighScreen())
            a = 15;
        
        newRect = CCRect(rect.origin.x/2, rect.origin.y/2+deltaHeightOf568() - a , rect.size.width/2, rect.size.height/2);
    }
    
    CommonWebView::Inst()->showWebViewWithFrameAndHtml(newRect, htmlstr,
                                                       type, bGoToBrowser);
}

void OcProxy::showHtmlStr(const char *string,bool bBottomAlign)
{
    CommonWebView::Inst()->showHtmlStr(string, bBottomAlign);
}

void OcProxy::setFrame(CCRect rect)
{
    CCRect newRect;
    if(isIpadDevice())
    {
        newRect = CCRect(rect.origin.x + 64, rect.origin.y + 32, rect.size.width, rect.size.height);
    }
    else
    {
        newRect = CCRect(rect.origin.x/2, rect.origin.y/2+deltaHeightOf568(), rect.size.width/2, rect.size.height/2);
    }
    
    CommonWebView::Inst()->setFrame(newRect);
}

bool OcProxy::getIsLoadSuccess()
{
    return CommonWebView::Inst()->getIsLoadSuccess();
}
void OcProxy::removeWebview()
{
    CommonWebView::Inst()->removeWebview();
}

void OcProxy::showMailViewController(const char* userName)
{
    CCLog("OcProxy showMailViewController");
    OcUtility::Inst()->showMailViewController(userName);
}

bool OcProxy::getIsHasSentMail()
{
    return OcUtility::Inst()->getIsHasSentMail();
}

const char* OcProxy::getMailText()
{
    return OcUtility::Inst()->getMailText();
}

void OcProxy::deleteMailViewController()
{
    OcUtility::Inst()->deleteMailViewController();
}

int OcProxy::getPpPurchaseStatus()
{
    return OcUtility::Inst()->getPpPurchaseStatus();
}
void OcProxy::resetPpPurchaseStatus()
{
    OcUtility::Inst()->resetPpPurchaseStatus();
}

int OcProxy::getPpLoginStatus()
{
    return OcUtility::Inst()->getPpLoginStatus();
}

void OcProxy::buyPp(int price,const char* billno,const char* title,const char* roleid)
{
#if defined(alipay_falseprice)
    OcUtility::Inst()->buyPp(1,billno,title,roleid);
#else
    OcUtility::Inst()->buyPp(price,billno,title,roleid);
#endif
}

void OcProxy::showPpLogin()
{
    OcUtility::Inst()->showPpLogin();
}

void OcProxy::PPlogout()
{
    OcUtility::Inst()->PPlogout();
}


void OcProxy::resetPpLogin()
{
    OcUtility::Inst()->resetPpLogin();
}

string OcProxy::getPptoken()
{
    return m_Pptoken;
}

void OcProxy::showPPCenter()
{
    OcUtility::Inst()->showPPCenter();
}

int OcProxy::getPpCurrentUserId()
{
    return m_PPCurrentUserId;
}

void OcProxy::savePpData()
{
    m_PPCurrentUserId = OcUtility::Inst()->getPpCurrentUserId();
    m_Pptoken = OcUtility::Inst()->getPptoken();
}

void OcProxy::resetLogoutStatus()
{
    OcUtility::Inst()->resetLogoutStatus();
}
int OcProxy::getLogoutStatus()
{
    return OcUtility::Inst()->getLogoutStatus();
}

#pragma mark -
#pragma mark UserDefault
void OcProxy::setIsLock(const char *ucid, bool isLock)
{
    UserDefault::Inst()->setIsLock(ucid, isLock);
}

bool OcProxy::getIsLock(const char *ucid)
{
    return UserDefault::Inst()->getIsLock(ucid);
}

void OcProxy::setBoolValueForKey(bool bValue, const char *key)
{
    UserDefault::Inst()->setBoolValueForKey(bValue, key);
}

bool OcProxy::getBoolValueForKey(const char *key)
{
    return UserDefault::Inst()->getBoolValueForKey(key);
}

void OcProxy::setIntValueForKey(int bValue, const char *key)
{
    UserDefault::Inst()->setIntValueForKey(bValue, key);
}

int OcProxy::getIntValueForKey(const char *key)
{
    return UserDefault::Inst()->getIntValueForKey(key);
}

void OcProxy::setFloatValueForKey(float fValue, const char* key)
{
    UserDefault::Inst()->setFloatValueForKey(fValue, key);
}

float OcProxy::getFloatValueForKey(const char* key)
{
    return UserDefault::Inst()->getFloatValueForKey(key);
}

void OcProxy::setValueForKey(const char* value, const char* key)
{
    UserDefault::Inst()->setValueForKey(value, key);
}

const char* OcProxy::getValueForKey(const char* key)
{
    return UserDefault::Inst()->getValueForKey(key);
}

void OcProxy::removeObjectForKey(const char *key)
{
    UserDefault::Inst()->removeValueForKey(key);
}

void OcProxy::synchronizeDefaultData()
{
    UserDefault::Inst()->synchronizeDefaultData();
}

#pragma mark -
#pragma mark OcUtility
bool OcProxy::isIpadDevice()
{
    return OcUtility::Inst()->isIpadDevice();
}

void OcProxy::showUsedMemory()
{
    OcUtility::Inst()->showUsedMemory();
}

void OcProxy::deleteCookies()   // 删除cookies
{
    OcUtility::Inst()->deleteCookies();
}

void OcProxy::deleteCache() // 删除cache
{
    OcUtility::Inst()->deleteCache();
}

const char* OcProxy::localizedStringStatic(const char* str) // 从语言包（Localizable.strings文件获取的）中获取语言
{
    return OcUtility::Inst()->localizedStringStatic(str);
}

//UUID
string OcProxy::getUUID()
{
    return OcUtility::Inst()->getUUID();
}

#pragma mark -
#pragma mark File path
//File path
const char* OcProxy::getExecutablePath()
{
    return OcUtility::Inst()->getExecutablePath();
}

const char* OcProxy::getDocumentPath()
{
    return CCFileUtils::getWriteablePath().c_str();
}

//void OcProxy::WriteToFile(const char* filepathname, char* buf, int size)
//{
//    
//}

//获取设备型号
string OcProxy::getMachineType()
{
    return OcUtility::Inst()->getMachineType();
}

#pragma mark -
#pragma mark payment
//payment
void OcProxy::loadStore()
{
    CPayMM::Inst()->loadStore();
}

void OcProxy::purchase(int idx)
{
    CPayMM::Inst()->purchase(idx);
}
void OcProxy::purAlipay(string tradeNo,string title,int price,bool isTruePrice)
{
    CPayMM::Inst()->purAlipay(tradeNo.c_str(),title.c_str(),price,isTruePrice);
}
int  OcProxy::getStoreStatus()
{
    return CPayMM::Inst()->getStoreStatus();
}

void OcProxy::resetStoreStatus()
{
    CPayMM::Inst()->resetStoreStatus();
}

int  OcProxy::getPurchaseStatus()
{
    return CPayMM::Inst()->getPurchaseStatus();
}

void OcProxy::resetPurchaseStatus()
{
    CPayMM::Inst()->resetPurchaseStatus();
}

int   OcProxy::getTapjoyStatus()
{
    return OcUtility::Inst()->getTapjoyStatus();
}

void OcProxy::resetTapjoyStatus()
{
    OcUtility::Inst()->resetTapjoyStatus();
    
}

void OcProxy::finishPayment()
{
    CPayMM::Inst()->finishPayment();
}

const char* OcProxy::getReceipt()
{
    return CPayMM::Inst()->getReceipt();
}

const char* OcProxy::getTransId()
{
    return CPayMM::Inst()->getTransId();
}

void OcProxy::addProduct(const char* szIapId,bool isAlipay)
{
    CPayMM::Inst()->addProduct(szIapId,isAlipay);
}

void OcProxy::rmvProduct(const char* szIapId)
{
    CPayMM::Inst()->rmvProduct(szIapId);
}

int  OcProxy::getProductCnt()
{
    return CPayMM::Inst()->getProductCnt();
}

const char* OcProxy::getIapIdAtIdx(int idx)
{
    return CPayMM::Inst()->getIapIdAtIdx(idx);
}

const char* OcProxy::getPriceAtIdx(int idx)
{
    return CPayMM::Inst()->getPriceAtIdx(idx);
}

const char* OcProxy::getTitleAtIdx(int idx)
{
    return CPayMM::Inst()->getTitleAtIdx(idx);
}

int OcProxy::getIntPriceAtIdx(int idx)
{
    return CPayMM::Inst()->getIntPriceAtIdx(idx);
}

#pragma mark -
#pragma mark -weixin


void OcProxy:: weixininit(CCObject* target,bool isFight){

    theshareSanguo->shareSanGuoInit(target,isFight);

}

void OcProxy:: sharetoSanguo(CCObject* target, SEL_CallFunc selector,int type){

    theshareSanguo->shareSanGuoCallback(target, selector,type);

}

int OcProxy:: returnWeiBoOrWeixinType(){

    return theshareSanguo->retunShareType();

}

void OcProxy:: setweixinback( int weixinbacki ){

    theshareSanguo->weixincallback(weixinbacki);

}
void OcProxy:: setPayBack( int rCode ){
    
    theshareSanguo->alipaycallback(rCode);
    
}

//微博
void OcProxy::showPublishView(enWeiboType type)
{
    CommonPublishView::Inst()->showPublishView(type);
}

void OcProxy::setPublishViewHidden(bool isHiden)
{
    CommonPublishView::Inst()->setPublishViewHiden(isHiden);
}

void OcProxy::removePublishView()
{
    CommonPublishView::Inst()->removePublishView();
}

const char* OcProxy::getWeiboContent()
{
    return CommonPublishView::Inst()->getWeiboContent();
}

void OcProxy::clearOauthData()
{
    TencentApiMM::Inst()->clearOauthData();
}

//加密
string OcProxy::AesEncript(const char *str)
{
    return OcUtility::Inst()->AesEncript(str);
}

string OcProxy::AesDecript(const char *str)
{
    return OcUtility::Inst()->AesDecript(str);
}

#pragma mark -
#pragma mark tencent login

void OcProxy::loginbyTencent()
{
    TencentApiMM::Inst()->login();
}

int OcProxy::getTencentLoginStatus()
{
    return TencentApiMM::Inst()->getLoginStatus();
}

const char*  OcProxy::getTencentOpenId()
{
    return TencentApiMM::Inst()->getOpenId();
}

const char*  OcProxy::getTencentAccessToken()
{
    return TencentApiMM::Inst()->getAccessToken();
}

#pragma mark -
#pragma mark sina login
void OcProxy::loginBySina()
{
    SinaApiMM::Inst()->Login();
}

int  OcProxy::getSinaLoginStatus()
{
    return SinaApiMM::Inst()->getLoginStatus();
}

const char* OcProxy::getSinaOpenId()
{
    return SinaApiMM::Inst()->getOpenId();
}

const char* OcProxy::getSinaAccessToken()
{
    return SinaApiMM::Inst()->getAccessToken();
}


void OcProxy::showHelpInfoWebView(const char * htmlStr)
{
    CCRect rect;
    if(OcUtility::Inst()->isIpadDevice())
    {
        rect = CCRect(65, 300, 640, 520);
        
    }else{
        rect = CCRect(0,130+deltaHeightOf568(), 320, 260);
    }
    CommonWebView::Inst()->showWebViewWithFrameAndHtml(rect, htmlStr, enWebviewLoadTypeRequest);
}

void OcProxy::goAppStoreUpdate()
{
    OcUtility::Inst()->goAppStoreUpdate();
}

void OcProxy::goAppStoreEvaluate()
{
    OcUtility::Inst()->goAppStoreEvaluate();
}

void OcProxy::sendWeibo(const char *str, bool isImageExist)
{
   if(strcmp(getValueForKey("platform"), "sina") == 0)
   {
       SinaApiMM::Inst()->setIsGachaWeibo(m_isGachaWeibo);
       SinaApiMM::Inst()->sendWeibo(str, isImageExist);
       
   }else if(strcmp(getValueForKey("platform"), "qq") == 0){
       
       TencentApiMM::Inst()->setIsGachaWeibo(m_isGachaWeibo);
       TencentApiMM::Inst()->sendWeibo(str, isImageExist);
   }
    m_isGachaWeibo = false;
}

void OcProxy::setIsGachaWeibo(bool isOk)
{
    m_isGachaWeibo = isOk;
}

const char * OcProxy::getInviteCodeWeibo()
{
    return OcUtility::Inst()->getInviteCodeWeibo();
}

bool OcProxy::is568HighScreen()
{
    return OcUtility::Inst()->is568HighScreen();
}

float OcProxy::deltaHeightOf568()
{
    return OcUtility::Inst()->deltaHeightOf568();
}

void OcProxy::setPushInfo(char *pushInfo, long  time, const char *valueInfo,bool isLocalTime)
{
    OcUtility::Inst()->setPushInfo(pushInfo, time, valueInfo,isLocalTime);
}

void OcProxy::linkToSafriWithUrlStr(const char *url)
{
    OcUtility::Inst()->linkToSafriWithUrlStr(url);
}

void OcProxy::showOfferWall()
{
    OcUtility::Inst()->showOfferWall();
}


const char* OcProxy::getBundleId()
{
    return OcUtility::Inst()->getBundleId();
}

const char* OcProxy::getBundleName()
{
    return OcUtility::Inst()->getBundleName();
}

//从年月日形式的字符串和时区，获得相对1970-1-1的秒数
long OcProxy::getAbsTimeFromString(const char* szTime, const char* szTimeZone)
{
    return OcUtility::Inst()->getAbsTimeFromString(szTime, szTimeZone);
}

//从当前时区的秒数，获得当前年月日
string OcProxy::getYmdhmsFromTimeIntervalSince1970(long timeSec)
{
    return getYmdhmsFromTimeIntervalSince1970(timeSec, kOcTimeZone);
}

string OcProxy::getYmdhmsFromTimeIntervalSince1970(long timeSec, const char* szTimeZone)
{
    return OcUtility::Inst()->getYmdhmsFromTimeIntervalSince1970(timeSec, szTimeZone);
}

const char* OcProxy::getMacAddr()
{
    return OcUtility::Inst()->getMacAddr();
}

const char* OcProxy::getAdId()
{
    return OcUtility::Inst()->getAdId();
}

// show tips and num
CCSprite* OcProxy::showTipsAndNum(int num)
{
    CCSprite* numberTips = CCSprite::spriteWithSpriteFrameName("id_tips.png");
    
    char numberStr[50];
    if(num < 10)
        sprintf(numberStr, "%d", num);
    else
        sprintf(numberStr, "N");
    
    TextNode *numberLabel = TextNode::textWithString(numberStr, 25);
    numberLabel->setPosition(ccp(18, 18));
    numberTips->addChild(numberLabel);
    numberLabel->setColor(ccc3(38, 81, 0));
    
    return numberTips;
}

// NSLocalizedString
const char* OcProxy::getLocalizedString(const char* key)
{
    return OcUtility::Inst()->getLocalizedString(key);
}

bool OcProxy::checkIsUseExternal(const char *external)
{
    if (OcUtility::Inst()->checkIsUseExternal(external))
    {
        OcUtility::Inst()->onOutsideZhuiAlarm(CGameData::Inst()->getLanguageValue("OutsideZhuiAlarm_title"),
                                              CGameData::Inst()->getLanguageValue("OutsideZhuiAlarm_message"),
                                              CGameData::Inst()->getLanguageValue("OutsideZhuiAlarm_oktext"));
        return true;
    }
    return false;
}


#pragma mark -
#pragma mark 录像


void OcProxy::startRecordScreen()
{
    OcUtility::Inst()->startRecordScreen();
}

void OcProxy::stopRecordScreen()
{
    OcUtility::Inst()->stopRecordScreen();
}

void OcProxy::cancelRecordScreen()
{
    OcUtility::Inst()->cancelRecordScreen();
}

float OcProxy::GetUploadVideoProgress()
{
    float rt = OcUtility::Inst()->GetUploadVideoProgress();
    return rt;
}

void OcProxy::pauseRecordScreen()
{
    OcUtility::Inst()->pauseRecordScreen();
}

void OcProxy::resumeRecordScreen()
{
    OcUtility::Inst()->resumeRecordScreen();
}

void OcProxy::SaveToAlbum()
{
    OcUtility::Inst()->SaveToAlbum();
}

void OcProxy::RequestVideoInfo()
{
    OcUtility::Inst()->RequestVideoInfo();
}

void OcProxy::OpenUrl(const char* _url)
{
    OcUtility::Inst()->Openurl(_url);
}

void OcProxy::InitWithGameUid(const char* _uid)
{
    OcUtility::Inst()->InitWithGameUid(_uid);
}

void OcProxy::SetPathRecording(const char* _url)
{
    OcUtility::Inst()->SetPathRecording(_url);
}

void OcProxy::SaveLastReplayData(ConfigData data, int _type)
{
    OcUtility::Inst()->SaveLastReplayData(data, _type);
}

bool OcProxy::CheckRecordExist()
{
    return OcUtility::Inst()->CheckRecordExist();
}

ConfigData* OcProxy::GetVideoAtIndex(int index, int type)
{
    return OcUtility::Inst()->GetVideoAtIndex(index, type);
}

void OcProxy::SetUploading(const char* time0, bool uploading)
{
    OcUtility::Inst()->SetUploading(time0, uploading);
}

void OcProxy::SetUploadSuccess(const char* time0)
{
    OcUtility::Inst()->SetUploadSuccess(time0);
}

int OcProxy::GetUploadState(const char* time0)
{
    return OcUtility::Inst()->GetUploadState(time0);
}

bool OcProxy::CheckUploading()
{
    return OcUtility::Inst()->CheckUploading();
}

bool OcProxy::CheckUploadedUpto10()
{
    return OcUtility::Inst()->CheckUploadedUpto10();
}

void OcProxy::RemovePreviousVideoBehindIndex(int _index)
{
    OcUtility::Inst()->RemovePreviousVideoBehindIndex(_index);
}



