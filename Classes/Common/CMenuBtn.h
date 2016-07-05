//
//  CMenuBtn.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CMenuBtn_h
#define AgainstWar_CMenuBtn_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "FriendMain.h"

#define kCntryFightListItemMenu    177
#define kCntryFrame                178
#define kCntryRestTimeTitle        179
#define kCntryRestTime             180
#define kCntryHP                   181
#define kCntryMoneyTitle           182
#define kCntryMoney                183
#define kCntryKaiQi                184
#define kCntryCanYu                185
#define kCntryShangHai             186
#define kCntryKaiQi_               187


#define kMenuViewOtherRecord       321

#define TAGXSREWARDICON       130902

#define TAGXUANSHANGBAR       20130903
#define TAGXUANSHANGTIP       20130904
#define TAGTEXTFEILD          20130905
#define TAGCOLOCLAYER         20130906



using namespace cocos2d;

class CFriendHelpInfo;
class CNmlDgInfo;
class CDgRoomInfo;
class CSpecialDgInfo;
class CFriendInf;
class CCntryDgInfo;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CMenuBtn : public CCLayer
{

public:
    CMenuBtn(const char* img, const char* name, CCTextAlignment align);
    CMenuBtn(CFriendHelpInfo* pHelperInfo,enFriendRange type, bool hasGrayCover = false, int helperNo = 0);
    void setHelperNo(bool hasGrayCover, int helperNo);
    CMenuBtn(CNmlDgInfo* pNmlDgInfo, int indexFloor);
    CMenuBtn(CSpecialDgInfo* pSpecialDgInfo);
    CMenuBtn(CDgRoomInfo* pRoomInf, char bgType);
    CMenuBtn(CDgRoomInfo* pRoomInf, int indexFloor = 0, int indexRoom = 0, char bgType = '0');
    CMenuBtn(CFriendInf* pSchUsrInf, enFriendRange type, bool bFriendApplyBtn = false);
    CMenuBtn(const char * price, const char * iap_id,bool isTapjoy);
    CMenuBtn(CRwdTopMemberInfo* cntryInfo, int idx);  //悬赏排名
    CMenuBtn(CCntryBaseInfo* cntryInfo);    // 国家列表项
    CMenuBtn(CCntryDgInfo* pCntryDgInfo);     // 保卫战列表项
    CMenuBtn();
    CMenuBtn(CMember* memberInfo,bool isManage = false);            // 国家成员列表项
    CMenuBtn(enBuffTypeDef buffType);                               // buff升级列表项
    CMenuBtn(CWelfareInfo* welfareInfo);                            // 福利列表项
    CMenuBtn(CPostRewardBaseInfo* pPostRewardInfo);                 //悬赏列表项
    CMenuBtn(GuildShopItemInf* ShopItemInf);                        //悬赏宝库道具项
    CMenuBtn(CRewardMember* memberInfo);                            // 悬赏成员详情列表项
    CMenuBtn(RwdSignInf* pRwdSignInf,bool isLast=false);            //签到奖品信息项
    CMenuBtn(RwdLevelUpInf* pRwdLevelUpInf);                        //升级奖品信息项
    CMenuBtn(RwdBonusInf* pRwdBonusInf,int i);                            //累积天数奖励项
    CMenuBtn(RwdHuodongInf* pRwdHuodongInf);                        //系统活动奖励项
    CMenuBtn(RwdBuchangInf* pRwdBuchangInf);                        //系统补偿奖励项
    
    void baokuPriceChange(int num);
    //void CliderChange(CCObject* pObj);
    GuildShopItemInf* getGShopItemInf();
    short getBuyNum();
    void refreshMenu();
    
    virtual void onExit();
    CFriendInf* getSchUsrInfo();
    
    CMenuBtn(GuildItemInStore* GuildItemInf, int total); //悬赏库存道具列表项
    void chooseUpdate(int num);
    GuildItemInStore* getGuildItemInf();
    short getRewardNum();
    
    void AddBtnViewOtherRecord(CCObject* target, SEL_CallFuncO selector, int _tag);
    void RemoveBtnViewOtherRecord();
    int getUITab2(){return m_nUITab2;}
    void setUITab2(int nUITab2){m_nUITab2 = nUITab2;}
    
    
private:
    CFriendInf *m_pSchUsrInf;

    double   m_lasthour;
    long     m_opentime;
    
    GuildShopItemInf     m_GShopItemInf;
    long    m_buyNum;
    
    long    m_rewardNum;
    GuildItemInStore*     m_pGuildItemInf;
    int                   m_nUITab2;
    // 选5个援军 使用到的 标示
    CCLayerColor* grayLayer;
    CCSprite* numberTips;
    
private:
    void showRoomBtn(CDgRoomInfo* pRoomInf, int indexFloor, int indexRoom, char bgType);
    
    void showOrgNormal(CCntryDgInfo* pCntryDgInfo);
    void showOrgJiFeng(CCntryDgInfo* pCntryDgInfo);
    void showOrgLuanWu(CCntryDgInfo* pCntryDgInfo);
    
    void showOrgLianShouChang(CCntryDgInfo * pCntryDgInfo);
    
    CCSprite* RewardHelperCardIcon(const char* card_name, int card_number,bool isShow = true);

    //
    
};

#endif
