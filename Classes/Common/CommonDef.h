//
//  CommonDef.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CommonDef_h
#define CardDemo_CommonDef_h

#define SCREEN_WIDTH  CCDirector::sharedDirector()->getWinSize().width
#define SCREEN_HEIGHT  CCDirector::sharedDirector()->getWinSize().height

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 移植或发布需要注意修改的项目
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#define kDeveloping             // 开发中，可以看到注释，如果提交把这个宏注释掉
#define kLocalAppVer    "5.00"
#define kMaxCardNum 1020         // 图鉴开放的最大编号

//#define netServerURL        "http://sangoioscn.negaplay.com/"

//#define netServerURL          "http://42.121.15.153:9192/"

#define netServerURL          "http://42.121.15.153:9193/"

//#define netServerURL          "http://42.121.15.153:9194/"

//#define netServerURL          "http://42.121.15.153:8003/"


//#define TalkData //talkdata开关

#define ZhanBaiFanTi   //战败返体


//pp助手版本的时候请将apppay关掉
#define apppay        //苹果商店收费
//#define alipay          //支付宝收费

//#define alipay_falseprice   //支付宝假价格

#define channel_mark         "00000000"      //官方苹果
//#define channel_mark         "10100001"      //官方越狱
//#define channel_mark         "30100001"      //91越狱
//#define channel_mark         "30200001"      //同步推越狱

//#define channel_mark         "30300001"       //PP助手


#define kProductId00    "com.oneclick.sango.coinaa" // 6元大礼包
#define kProductId01    "com.oneclick.sango.coin01"
#define kProductId02    "com.oneclick.sango.coin02"
#define kProductId03    "com.oneclick.sango.coin03"
#define kProductId04    "com.oneclick.sango.coin04"
#define kProductId05    "com.oneclick.sango.coin05"
#define kProductId06    "com.oneclick.sango.coin06"
#define kProductId08    "com.oneclick.sango.coin08"
#define kProductId09    "com.oneclick.sango.coin09" // 198元财源滚滚月卡大礼包
#define kProductId11    "com.oneclick.sango.coin11" // 30元招财进宝月卡大礼包
#define kProductId12    "com.oneclick.sango.coin12" // 30元大礼包
#define kProductId13    "com.oneclick.sango.coin13" // 198元大礼包


#define kOcTimeZone     "Asia/Shanghai"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#define FONTSTHEITISCM  "Helvetica-Bold"//"STHeitiSC-Medium"//

#define kRandLstLen 128                 // 进度保存文件中保存随机数的数组大小
#define kMaxCardCntInBattle 6
#define kMaxEnemyCnt        6

#define kStoneMvDur1        1.5
#define kStoneMvDur2        5.0

#define kUiActionDur1       0.5
#define kUiActionDur2       0.2

#define kTagToBeDel         0xcc

#define kBtnTitleHeight     42

#define kMenuBtnHeight      116
#define kMenuBtnWidth       570

#define kTitleColor   ccc3(122,37,8)
#define kInfoColor    ccc3(63,23,13)
#define kShowInfoColor  ccc3(78, 24, 0)

#define kListBoxMinMovY 10.0

#define kMaxGridRowCnt      5
#define kMaxGridColumnCnt   6

#define DialogType_caiyungungun  0x333   //财运滚滚
#define DialogType_zhaocaijinbao  0x444  //招财进宝
#define DialogType_mengjiangwushuang  0x555 //猛将无双
#define DialogType_luanshishenjiang   0x566 //乱世神将


enum enLoginReqType{
    enLoginReqType_Null,
    enLoginReqType_TencentLogin,
    enLoginReqType_SinaLogin,
    enLoginReqType_Pp,
    enLoginReqType_OcLogin,
    enLoginReqType_Cfg
};

enum enWebviewLoadType {
    
    enWebviewLoadTypeRequest,
    enWebviewLoadTypeHtmlStr
    
};

#define kPltfmOc        "oc"
#define kPltfmTencent   "qq"
#define kPltfmSina      "sina"
#define kPltfmIos       "ios"
#define kPltfmAndroid   "android"
//#define kPltfmPp        "pp"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define kSeSuffix       ".wav"
#else
#define kSeSuffix       ".caf"
#endif

#define kSndEffect(x) #x kSeSuffix


#define kInvalidValue   0xFFFF

//点击title bar 和 bottom panel时，背包和列表滚动到顶部或底部
#define kNotifyGoTop    "NotifyGoTop"
#define kNotifyGoBottom "NotifyGoBottom"



//碎片系统通知

#define kNotifyItem    "kNotifyItem"
#define kNotifyChild "kNotifyChild"

#define kNotifyShowBtn    "kNotifyShowBtn"
#define kNotifyHideBtn    "kNotifyHideBtn"





//获得新称号的通知
#define kNotifyNewTitle "NotifyNewTitle"

//更新玩家信息的通知
#define kNotifyUpdateUsrInf "NotifyUpdateUsrInf"

//更新同盟聊天信息的通知
#define kNotifyUpdateChatInf "NotifyUpdateChatInf"

//推送
#define kPushInfDg      "PushInfDg"
#define kPushInfStamina "PushInfStamina"

#define kPushInfStamina2 "PushInfStamina2"   //按体力推送消息

#define kPushInfGetGift "PushInfoGetGift"   //宝物冷却完毕推送

#define kPushInfSevenDay "PushInfSevenDay"  //7天未登陆


#endif





