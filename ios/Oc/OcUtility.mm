//
//  OcUtility.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "OcUtility.h"
#include "sys/utsname.h"
#include <mach/mach.h>
#include "TapJoyCenter.h"
#include "NSData+Encryp.h"
#include "GTMBase64.h"
#include "HttpComm.h"
#include "TencentApiMM.h"
#include "GoAppStoreAlert.h"
#include "CGamedata.h"
#include "AppController.h"

#include <sys/socket.h>
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>

#import "MailViewController.h"
#import "UIDevice+ProcessesAdditions.h"
#import "ScreenRecordHelper.h"
#include "RecordScreen.h"
#import <AdSupport/AdSupport.h>
#import "SFHFKeychainUtils.h"
#import "RootViewController.h"

OcUtility* OcUtility::s_pcIf = NULL;
static GoAppStoreAlert *s_goAppStoreAlert = NULL;
static UINavigationController *s_naviController = nil;
static MailViewController *s_mailController = nil;

OcUtility::OcUtility()
{
    m_bHasReleaseMCtrl = true;
}

OcUtility::~OcUtility()
{

}

OcUtility* OcUtility::Inst()
{
    if(s_pcIf == NULL){
        s_pcIf = new OcUtility();
    }
    
    return s_pcIf;
}

bool OcUtility::isIpadDevice()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    NSString *deviceModel = [NSString stringWithFormat:@"%@",[[UIDevice currentDevice] model]];
    if([[deviceModel substringToIndex:4] isEqualToString:@"iPad"]){
        return true;
    }else
        return false;
}

//  获取游戏当前内存
void OcUtility::showUsedMemory()
{
//    struct task_basic_info info;
//    mach_msg_type_number_t size = sizeof(info);
//    kern_return_t kerr = task_info(mach_task_self(),
//                                   TASK_BASIC_INFO,
//                                   (task_info_t)&info,
//                                   &size);
//    if( kerr == KERN_SUCCESS ) {
//        NSLog(@"Memory in use : %fMb", info.resident_size/1024.0/1024.0);
//    } else {
//        NSLog(@"Error with task_info(): %s", mach_error_string(kerr));
//    }
    
}

//删除cookies
void OcUtility::deleteCookies()
{
    NSHTTPCookieStorage *storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie *cookie in [storage cookies]) {
        [storage deleteCookie:cookie];
    }
}

// 删除缓存
void OcUtility::deleteCache()
{
    [[NSURLCache sharedURLCache] removeAllCachedResponses];
}

int  OcUtility::getTapjoyStatus()
{
    return [[TapJoyCenter shareInstance] tapJoyRepStatus];
}

void OcUtility::resetTapjoyStatus()
{
    [[TapJoyCenter shareInstance] resetTapjoyStatus];
}


string OcUtility::AesEncript(const char *str)
{

    NSString *oriStr = [NSString stringWithUTF8String:str];
    long timeStamp = HttpComm::Inst()->getTimeStamp();
    NSString *timeStr = [[NSString stringWithFormat:@"%ld",timeStamp] substringFromIndex:4];
    oriStr = [[oriStr substringToIndex:9] stringByAppendingString:timeStr];
    NSString *key = [NSString stringWithFormat:@"%s%s%s%s%s%s",AES_KEY_1,AES_KEY_2,AES_KEY_3,AES_KEY_4,AES_KEY_5,AES_KEY_6];
    NSData *plain = [oriStr dataUsingEncoding:NSUTF8StringEncoding];
    NSData *cipher = [plain AES256EncryptWithKey:key];
    NSString *strmdBase = [GTMBase64 stringByEncodingData:cipher];
    NSString *secretStr = [strmdBase substringToIndex:[strmdBase length]-2];
    string cSecretStr = [secretStr UTF8String];
    
    return cSecretStr;
}


string OcUtility::AesDecript(const char *str)
{
    //decode
    NSString *oriStr = [NSString stringWithUTF8String:str];
    NSString *key = [NSString stringWithFormat:@"%s%s%s%s%s%s",AES_KEY_1,AES_KEY_2,AES_KEY_3,AES_KEY_4,AES_KEY_5,AES_KEY_6];
    NSData *decode = [GTMBase64 decodeString:oriStr];
    NSData *plainData = [decode AES256DecryptWithKey:key];
    NSString *aString = [[NSString alloc] initWithData:plainData encoding:NSUTF8StringEncoding];

    return  [aString UTF8String];
}

const char* OcUtility::getExecutablePath()
{
    return [[[NSBundle mainBundle] executablePath] UTF8String];
}

const char* OcUtility::localizedStringStatic(const char *str)
{
    return [NSLocalizedString([NSString stringWithUTF8String:str], NULL) UTF8String];
}


//截图
void OcUtility::fullScreenshots(){
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                         NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString* path = [documentsDirectory stringByAppendingPathComponent:
                      [NSString stringWithUTF8String:"captureImage.png"]];
    
    UIImage* image = [UIImage imageWithContentsOfFile:path];
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    if (!window) {
        window = [[UIApplication sharedApplication].windows objectAtIndex:0];
    }
    CGRect rect = CGRectMake(0, 0, 200, 200);
    CGImageRef subImageRef = CGImageCreateWithImageInRect(image.CGImage, rect);
    CGRect smallBounds = CGRectMake(0, 0, CGImageGetWidth(subImageRef), CGImageGetHeight(subImageRef)/2);
    
    UIGraphicsBeginImageContext(smallBounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextDrawImage(context, smallBounds, subImageRef);
    UIImage* smallImage = [UIImage imageWithCGImage:subImageRef];
    UIGraphicsEndImageContext();
    
    UIImageWriteToSavedPhotosAlbum(smallImage, nil, nil, nil);
}

//判断是否我iphone5 568高度的类似机型
bool OcUtility::is568HighScreen()
{
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    if (!window) {
        window = [[UIApplication sharedApplication].windows objectAtIndex:0];
    }
    CGRect rect = [window bounds];
    if(rect.size.height == 568)
        return true;
    else
        return false;
}

//获得iphone5多出来的高度
float OcUtility::deltaHeightOf568()
{
    UIWindow* window = [UIApplication sharedApplication].keyWindow;
    if (!window) {
        window = [[UIApplication sharedApplication].windows objectAtIndex:0];
    }
    CGRect rect = [window bounds];
    if(rect.size.height == 568)
        return 59;
    else
        return 0;
}

void OcUtility::goAppStoreEvaluate()
{
    if(s_goAppStoreAlert == NULL)
    {
        s_goAppStoreAlert = [[GoAppStoreAlert alloc] init];
    }
    [s_goAppStoreAlert goToAppStoreEvaluate];
}


void OcUtility::goAppStoreUpdate()
{
    if(s_goAppStoreAlert == NULL)
    {
        s_goAppStoreAlert =  [[GoAppStoreAlert alloc] init];
    }
    [s_goAppStoreAlert goToAppStoreUpdate];
}


const char* OcUtility::getInviteCodeWeibo()
{
    CUserInfo *userInfo = CGameData::Inst()->getUsrInfo();
    NSString *inviteCode = [NSString stringWithUTF8String:userInfo->invite_code];
    NSString *tempString  = [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_5];
    NSString *weiboStr  =  [tempString stringByReplacingOccurrencesOfString:@"&&" withString:inviteCode];
    NSString *repAppUrlStr = [NSString stringWithFormat:@"%s",CGameData::Inst()->getCommonInfo()->app_url];
    NSString *repAndrodUrlStr = [NSString stringWithFormat:@"%s",CGameData::Inst()->getCommonInfo()->androidsg_download];
    weiboStr = [NSString stringWithString:[weiboStr stringByReplacingOccurrencesOfString:@"<<ios>>" withString: repAppUrlStr]];
    weiboStr = [NSString stringWithString:[weiboStr stringByReplacingOccurrencesOfString:@"<<android>>" withString: repAndrodUrlStr]];
    
    m_weiboStr = [weiboStr UTF8String];
    return m_weiboStr.c_str();
}

void OcUtility::setPushInfo(char *pushInfo, long time, const char *valueInfo,bool isLocalTime)
{
    
//    [[UIApplication sharedApplication] cancelAllLocalNotifications];//取消所有的通知
    
    //通过对照对应的userInfo的值来辨别各个notification，从而来取消notification和重新加载notification
    NSString *valureStr = [NSString stringWithUTF8String:valueInfo];
    for (UILocalNotification *notification in [[UIApplication sharedApplication] scheduledLocalNotifications])
    {
        NSString *str = [notification.userInfo objectForKey:@"Identifier"];
        if ([str isEqualToString:valureStr] == YES)
        {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
    
    //------通知；
    UILocalNotification *notification=[[UILocalNotification alloc] init];
    notification.applicationIconBadgeNumber=0; //应用程序的右上角小数字
    if (notification!=nil) {//判断系统是否支持本地通知
        NSDate *pushDate = [NSDate dateWithTimeIntervalSince1970:time];
        if (isLocalTime) {
            pushDate = [NSDate dateWithTimeIntervalSinceNow:time];
        }
        notification.fireDate = pushDate;
        notification.repeatInterval = 0;//循环通知的周期
        notification.timeZone=[NSTimeZone defaultTimeZone];
        NSString *pushStr = [NSString stringWithUTF8String:pushInfo];
        notification.alertBody = [NSString stringWithFormat:@"%@",pushStr];
        notification.applicationIconBadgeNumber = 0; //应用程序的右上角小数字
        notification.soundName= UILocalNotificationDefaultSoundName;//本地化通知的声音
        //notification.alertAction = NSLocalizedString(@"进入战斗", nil);  //弹出的提示框按钮
        
        //添加额外的信息用来判断notification
        NSDictionary *infoDict = [NSDictionary dictionaryWithObject:valureStr forKey:@"Identifier"];
        notification.userInfo = infoDict;
        
        [[UIApplication sharedApplication]scheduleLocalNotification:notification];
        [notification release];
    }
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
     (UIRemoteNotificationType )(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound |UIRemoteNotificationTypeAlert)];

}

//设置tapJoy的userId
void OcUtility::setTapjoyUserId(const char *uid)
{
    NSString *uidStr = [NSString stringWithUTF8String:uid];
    [[TapJoyCenter shareInstance] setUserId:uidStr];
}

void OcUtility::linkToSafriWithUrlStr(const char *url)
{
    NSString *urlStr = [NSString stringWithUTF8String:url];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlStr]];
}

//显示tapjoy的积分墙
void OcUtility::showOfferWall()
{
    [[TapJoyCenter shareInstance] showOfferWall];
}

const char* OcUtility::getBundleId()
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    const char* bid = [[infoDict objectForKey:@"CFBundleIdentifier"] UTF8String];
    
    return bid;
}

const char* OcUtility::getBundleName()
{
    NSDictionary* infoDict = [[NSBundle mainBundle] infoDictionary];
    const char* bname = [[infoDict objectForKey:@"CFBundleDisplayName"] UTF8String];
    
    return bname;
}

//从年月日形式的字符串和时区，获得相对1970-1-1的秒数
long OcUtility::getAbsTimeFromString(const char* szTime, const char* szTimeZone)
{
    NSString* strTimeZone = [NSString stringWithUTF8String:szTimeZone];
    
    NSTimeZone* localzone = [NSTimeZone timeZoneWithName:strTimeZone];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    [dateFormatter setTimeZone:localzone];
    
    NSString* strDate = [NSString stringWithUTF8String:szTime];
    NSDate* date =[dateFormatter dateFromString:strDate];
    
    long lDateTime = [date timeIntervalSince1970];
    
    return lDateTime;
}

//从当前时区的秒数，获得当前年月日
string OcUtility::getYmdhmsFromTimeIntervalSince1970(long timeSec, const char* szTimeZone)
{
    NSDate* date=[NSDate dateWithTimeIntervalSince1970:timeSec];
    
    NSString* strTimeZone = [NSString stringWithUTF8String:szTimeZone];
    NSTimeZone* localzone = [NSTimeZone timeZoneWithName:strTimeZone];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    [dateFormatter setTimeZone:localzone];

    NSString *  locationString=[dateFormatter stringFromDate:date];
    [dateFormatter release];
    return [locationString UTF8String];
}

const char* OcUtility::getAdId()
{
    NSString *adId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return [adId  UTF8String];
}

const char* OcUtility::getMacAddr()
{
    int                    mib[6];
	size_t                len;
	char                *buf;
	unsigned char        *ptr;
	struct if_msghdr    *ifm;
	struct sockaddr_dl    *sdl;
	
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	
	if ((mib[5] = if_nametoindex("en0")) == 0) {
//		NSLog(@"Error: if_nametoindex error/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
//		NSLog(@"Error: sysctl, take 1/n");
		return NULL;
	}
	
	if ((buf = (char*)malloc(len)) == NULL) {
//		NSLog(@"Could not allocate memory. error!/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
//		NSLog(@"Error: sysctl, take 2");
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);

	NSString *outstring = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x",
                           *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	free(buf);
    
	return [[outstring uppercaseString] UTF8String];
}

void OcUtility::showMailViewController(const char* userName)
{
    if (m_bHasReleaseMCtrl) {
        NSString* strUserName = [NSString stringWithUTF8String:userName];
        s_mailController = [[MailViewController alloc] initWithUserName:strUserName];
        [s_mailController setModalTransitionStyle:UIModalTransitionStyleCoverVertical];
        
        s_naviController = [[UINavigationController alloc] initWithRootViewController:s_mailController];
        
        AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
        [appCtr.viewController presentModalViewController:s_naviController animated:YES];
        
        [s_mailController release];
        [s_naviController release];
        
        m_bHasReleaseMCtrl = false;
    }
}

bool OcUtility::getIsHasSentMail()
{
    if (!m_bHasReleaseMCtrl)
        return [s_mailController bHasSent];
    else
        return true;
}

const char* OcUtility::getMailText()
{
    if (!m_bHasReleaseMCtrl)
        return [[s_mailController strMail] UTF8String];
    else
        return "";
}

void OcUtility::deleteMailViewController()
{
    if (!m_bHasReleaseMCtrl) {
        AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
        [appCtr.viewController dismissModalViewControllerAnimated:YES];

        m_bHasReleaseMCtrl = true;
    }
}

int OcUtility::getPpPurchaseStatus()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    return [appCtr.viewController getPpPurchaseStatus];
}
void OcUtility::resetPpPurchaseStatus()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController resetPpPurchaseStatus];
}


int OcUtility::getPpLoginStatus()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    return [appCtr.viewController getPpLoginStatus];
}

void OcUtility::PPlogout()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController PPlogout];
    
}

void OcUtility::resetPpLogin()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController resetPpLoginStatus];
}

int OcUtility::getPpCurrentUserId()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    return [appCtr.viewController getPpCurrentUserId];
}

string OcUtility::getPptoken()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    NSString* token = [appCtr.viewController getPptoken];
    string ctokenStr = [token UTF8String];
    return ctokenStr;
}

void OcUtility::showPpLogin()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController loginPressDown];
}

void OcUtility::showPPCenter()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController showPPCenter];
}

void OcUtility::buyPp(int price,const char* billno,const char* title,const char* roleid)
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    NSString* billNo_pp = [NSString stringWithUTF8String:billno];
    NSString* title_pp = [NSString stringWithUTF8String:title];
    NSString* roleid_pp = [NSString stringWithUTF8String:roleid];
    [appCtr.viewController setPrice:price setBillNo:billNo_pp setTitle:title_pp setRoleid:roleid_pp];
}

void OcUtility::resetLogoutStatus()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    [appCtr.viewController resetLogoutStatus];
}
int  OcUtility::getLogoutStatus()
{
    AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
    return [appCtr.viewController getLogoutStatus];
}

// NSLocalizedString
const char* OcUtility::getLocalizedString(const char *key)
{
    NSString* strKey = [NSString stringWithUTF8String:key];
    return [NSLocalizedString(strKey, nil) UTF8String];
}
#import <Foundation/Foundation.h>
bool OcUtility::checkIsUseExternal(const char* external)
{
    NSArray* processes = [[UIDevice currentDevice] runningProcesses];
    NSString* strExternal = [NSString stringWithUTF8String:external];
    for (NSDictionary * dict in processes){
        if ([strExternal isEqualToString:[dict objectForKey:@"ProcessName"]])
        {
            //[[UIDevice currentDevice] OutsideZhuiAlarm];
            
            //[self action:@selector(btnClicked:) forControlEvents:UIControlEventTouchUpInside];
            //[button2 addTarget:self action:@selector(btnClicked:) forControlEvents:UIControlEventTouchUpInside];
            return true;
        }
            
//        NSLog(@"%@ - %@", [dict objectForKey:@"ProcessID"], [dict objectForKey:@"ProcessName"]);
    }
    
    return false;
}

void OcUtility::onOutsideZhuiAlarm(const char *pchTitle, const char *pchMessage, const char *pchOkText)
{
    [[UIDevice currentDevice] OutsideZhuiAlarm:
     [NSString stringWithUTF8String:pchTitle]:
     [NSString stringWithUTF8String:pchMessage]:
     [NSString stringWithUTF8String:pchOkText]];
}

//获取设备型号
string OcUtility::getMachineType()
{
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *machine = (char*)malloc(size);
    sysctlbyname("hw.machine", machine, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
    free(machine);
    if ([platform isEqualToString:@"iPhone1,1"])    return "iPhone 2G";
    if ([platform isEqualToString:@"iPhone1,2"])    return "iPhone 3G";
    if ([platform isEqualToString:@"iPhone2,1"])    return "iPhone 3GS";
    if ([platform isEqualToString:@"iPhone3,1"])    return "iPhone 4";
    if ([platform isEqualToString:@"iPhone3,2"])    return "iPhone 4";
    if ([platform isEqualToString:@"iPhone3,3"])    return "iPhone 4 (CDMA)";
    if ([platform isEqualToString:@"iPhone4,1"])    return "iPhone 4S";
    if ([platform isEqualToString:@"iPhone5,1"])    return "iPhone 5";
    if ([platform isEqualToString:@"iPhone5,2"])    return "iPhone 5 (GSM+CDMA)";
    
//    if ([platform isEqualToString:@"iPod1,1"])      return @"iPod Touch (1 Gen)";
//    if ([platform isEqualToString:@"iPod2,1"])      return @"iPod Touch (2 Gen)";
    if ([platform isEqualToString:@"iPod3,1"])      return "iTouch3";
    if ([platform isEqualToString:@"iPod4,1"])      return "iTouch4";
//    if ([platform isEqualToString:@"iPod5,1"])      return @"iPod Touch (5 Gen)";
//    
//    if ([platform isEqualToString:@"iPad1,1"])      return @"iPad";
//    if ([platform isEqualToString:@"iPad1,2"])      return @"iPad 3G";
//    if ([platform isEqualToString:@"iPad2,1"])      return @"iPad 2 (WiFi)";
//    if ([platform isEqualToString:@"iPad2,2"])      return @"iPad 2";
//    if ([platform isEqualToString:@"iPad2,3"])      return @"iPad 2 (CDMA)";
//    if ([platform isEqualToString:@"iPad2,4"])      return @"iPad 2";
//    if ([platform isEqualToString:@"iPad2,5"])      return @"iPad Mini (WiFi)";
//    if ([platform isEqualToString:@"iPad2,6"])      return @"iPad Mini";
//    if ([platform isEqualToString:@"iPad2,7"])      return @"iPad Mini (GSM+CDMA)";
//    if ([platform isEqualToString:@"iPad3,1"])      return @"iPad 3 (WiFi)";
//    if ([platform isEqualToString:@"iPad3,2"])      return @"iPad 3 (GSM+CDMA)";
//    if ([platform isEqualToString:@"iPad3,3"])      return @"iPad 3";
//    if ([platform isEqualToString:@"iPad3,4"])      return @"iPad 4 (WiFi)";
//    if ([platform isEqualToString:@"iPad3,5"])      return @"iPad 4";
//    if ([platform isEqualToString:@"iPad3,6"])      return @"iPad 4 (GSM+CDMA)";
//    
//    if ([platform isEqualToString:@"i386"])         return @"Simulator";
//    if ([platform isEqualToString:@"x86_64"])       return @"Simulator";
    return [platform UTF8String];
}


#pragma mark -
#pragma mark 录像

void OcUtility::startRecordScreen()
{
    [[RecordScreen shareInstance] startRecord];
}

void OcUtility::stopRecordScreen()
{
    [[RecordScreen shareInstance] stopRecord];
}

void OcUtility::cancelRecordScreen()
{
    [[RecordScreen shareInstance] cancelRecord];
}

void OcUtility::pauseRecordScreen()
{
    [[RecordScreen shareInstance] pauseRecord];
}

void OcUtility::resumeRecordScreen()
{
    [[RecordScreen shareInstance] resumeRecord];
}

float OcUtility::GetUploadVideoProgress()
{
    float rt =[[ScreenRecordHelper ShareHelper] GetUploadProgress];
    return rt;
}

void OcUtility::SaveToAlbum()
{
    [[ScreenRecordHelper ShareHelper] SaveToAlbum];
}

void OcUtility::RequestVideoInfo()
{
    [[ScreenRecordHelper ShareHelper] RequestVideoInfo];
}

void OcUtility::Openurl(const char* _url)
{
    [[ScreenRecordHelper ShareHelper] OpenUrl:[NSString stringWithUTF8String:_url]];
}

void OcUtility::InitWithGameUid(const char* _uid)
{
    [ScreenRecordHelper ShareWithUid:[NSString stringWithUTF8String:_uid]];
}

void OcUtility::SetPathRecording(const char* _url)
{
    [[ScreenRecordHelper ShareHelper] SetPathRecording:[NSString stringWithUTF8String:_url]];
}

void OcUtility::SaveLastReplayData(ConfigData data, int _type)
{
    [[ScreenRecordHelper ShareHelper] AddNewVideo:data SaveType:_type];
}

bool OcUtility::CheckRecordExist()
{
    return [[ScreenRecordHelper ShareHelper] CheckRecordExist];
}

ConfigData* OcUtility::GetVideoAtIndex(int index, int type)
{
    ConfigData* rt = [[ScreenRecordHelper ShareHelper] GetVideoAtIndex:index Type:type];
    return rt;
}

void OcUtility::SetUploading(const char* time0, bool uploading)
{
    [[ScreenRecordHelper ShareHelper] SetUploading:[NSString stringWithUTF8String:time0] Uploading:uploading];
}

void OcUtility::SetUploadSuccess(const char* time0)
{
    [[ScreenRecordHelper ShareHelper] SetUploadSuccess:[NSString stringWithUTF8String:time0]];
}

int OcUtility::GetUploadState(const char* time0)
{
    return [[ScreenRecordHelper ShareHelper] GetUploadState:[NSString stringWithUTF8String:time0]];
}

bool OcUtility::CheckUploading()
{
    bool rt = [[ScreenRecordHelper ShareHelper] CheckUploading];
    return rt;
}

bool OcUtility::CheckUploadedUpto10()
{
    return [[ScreenRecordHelper ShareHelper] CheckUploadedUpto10];
}

void OcUtility::RemovePreviousVideoBehindIndex(int _index)
{
    [[ScreenRecordHelper ShareHelper] RemovePreviousVideoBehindIndex:_index];
}

string OcUtility::getUUID()
{
    string strUUID = getUUIDFromKeyChain();
    
    if (strUUID.empty()) {
        CFUUIDRef cfuuid = CFUUIDCreate(kCFAllocatorDefault);
        strUUID = [(NSString*)CFBridgingRelease(CFUUIDCreateString(kCFAllocatorDefault, cfuuid)) UTF8String];
        
        setUUIDToKeyChain(strUUID);
    }
    
    return strUUID;
}

void OcUtility::setUUIDToKeyChain(string& strUUID)
{
    [SFHFKeychainUtils storeUsername:@"UUID" andPassword:[NSString stringWithUTF8String:strUUID.c_str()] forServiceName:@"uq" updateExisting:YES error:nil];
}

string OcUtility::getUUIDFromKeyChain()
{
    NSString* strUUID = [SFHFKeychainUtils getPasswordForUsername:@"UUID" andServiceName:@"uq" error:nil];
    
    if (strUUID) {
        return [strUUID UTF8String];
    }
    else{
        return "";
    }
}


