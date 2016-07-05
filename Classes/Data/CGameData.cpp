//
//  CGameData.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-15.
//  Copyright (c) 2012Âπ?OneClick Co.,Ltd. All rights reserved.
//

#include "CGamedata.h"
#include "HttpComm.h"
#include "TimeTransform.h"
#include "MainScene.h"
#include "OcProxy.h"
#include "jsoncpp.h"
#include "MailBoxAgt.h"
#include "FbdWords.h"
#include "RecordHelper.h"

#include "TDCCVirtualCurrency.h"

CGameData* CGameData::s_pcIf = NULL;

static char retUrl[200];
static const char* buffTypeArr[4] = {"buff_power", "buff_attack", "buff_recover", "buff_hp"};

#define kLocalTime "klocaltime"

//******************************************************************************
// instance
//******************************************************************************
CGameData* CGameData::Inst(){
    if(s_pcIf == NULL){
        s_pcIf = new CGameData();
    }
    
    return s_pcIf;
}



//******************************************************************************
// CGameData
//******************************************************************************
CGameData::CGameData()
{
    m_iLoadCardsStep = 0;
    m_hourReqTime = 0;
    dateRecord = 0;
    m_iCurNmlFloor = 0;
    m_iCurNmlRoom = 0;
    m_iCurSangoFloor = 0;
    m_iCurSangoRoom = 0;
    m_bFighting = false;
    m_productIdx = 0;
    m_bUpdateCom = false;
    m_commonInfo.stamina_recover_time = 0;
    m_iCurFloor = 0;
    m_iCurRoom = 0;
    m_iCurFlrStat = 0;
    m_lyUserCards = NULL;
    
    m_lFrndLstReqTime = 0;
    m_lRcvMailReqTime = 0;
    m_bUseTmpTeam = false;
    m_iCurTeamIdx = 0;
    
    memset(m_curDgType, 0, sizeof(m_curDgType));

    memset(m_TeamInf, 0, sizeof(m_TeamInf));
    clrPreCmtTeamInfo();
    
    comeBackTime = -1;
    m_pFriendHelpArr = new CCMutableArray<CFriendHelpInfo*>();
    m_pUserCardArr = new CCMutableArray<CUserCardInfo*>();
    m_pShopCardArr = new CCMutableArray<CShopCardInfo*>();
    m_pNmlDgArr = new CCMutableArray<CNmlDgInfo*>();
    m_pSangoYYArr = new CCMutableArray<CSangoYYDgInfo*>();
    m_pWelfareInfoArr = new CCMutableArray<CWelfareInfo*>();
    m_pSpecialDgArr = new CCMutableArray<CSpecialDgInfo*>();
    m_pDgStepArr = new CCMutableArray<CDgStepInfo*>();
    m_pFriendInfArr = new CCMutableArray<CFriendInf*>();
    m_pFriendApplyInfArr = new CCMutableArray<CFriendInf*>();
    m_pMemberInfArr = new CCMutableArray<CMember*>();
    m_pMemberApplyInfArr = new CCMutableArray<CFriendInf*>();
    m_pCntryInfArr = new CCMutableArray<CCntryBaseInfo*>();
    m_pRwdTopMemberInfArr = new CCMutableArray<CRwdTopMemberInfo*>();
    m_pushInfoArr = new CCMutableArray<PushInfo*>();
    m_compGachaInfoArr = new CCMutableArray<CompGachaInfo *>();
    m_exchangeCardInfo = new ExchangeCardInfo();
    m_GachaRetArr = new CCMutableArray<CGachaRet *>();
    
    m_ParcelItemInfoArr = new CCMutableArray<ParcelItemInfo *>();
    m_ParcelChildInfoArr = new CCMutableArray<ParcelChildInfo *>();

    
    
    m_pCntryDgInfArr = new CCMutableArray<CCntryDgInfo*>();
    m_pItmPkInf = new CCMutableArray<ItemCardInf* >();
    m_pItmTtlInf = new CCMutableArray<ItemCardInf* >();
    m_pTitleInfArr = new CCMutableArray<TitleInf*>();
    m_pJiFengRecordArr = new CCMutableArray<JiFengRecordInf*>();
    m_pGuildShopItemInfArr = new CCMutableArray<GuildShopItemInf*>();
    m_pRewardTaskArr = new CCMutableArray<CPostRewardBaseInfo*>();
    m_pRewardMemberArr = new CCMutableArray<CRewardMember*>();
    m_pRwdSignInfArr = new CCMutableArray<RwdSignInf*>();
    m_pRwdLevelUpInfArr = new CCMutableArray<RwdLevelUpInf*>();
    m_pRwdMonthInArr = new CCMutableArray<RwdMonthInf*>();
    m_pRwdBonusInfArr = new CCMutableArray<RwdBonusInf*>();
    m_pRwdHuodongInfArr = new CCMutableArray<RwdHuodongInf*>();
    m_pRwdBuchangInfArr = new CCMutableArray<RwdBuchangInf*>();
    m_pGuildItemAwardAff = new CCMutableArray<GuildItemAward*>();
    m_pMAARequestMasterInfoArr = new CCMutableArray<CMAARequestMasterInfo*>();
    
    m_newSpecialDgStatusDic.clear();
    m_newWeeklyDgStatusDic.clear();
    m_allUserDugeonsDic.clear();
    m_allUserCardsDic.clear();
    m_allBaseCardsDic.clear();
    m_allBaseDungeonsDic.clear();
    m_allMonstersDic.clear();
    m_allCardLevelsDic.clear();
    m_cntryConfigDic.clear();
    memset(m_allSmalDicTime, 0, sizeof(long) * enSmallDicMax);
    
    isPlayedGame = OcProxy::Inst()->getBoolValueForKey("isPlayedGame");
    if (isPlayedGame == false) {
        OcProxy::Inst()->setBoolValueForKey(true, "isPlayedGame");
        OcProxy::Inst()->setBoolValueForKey(true, "Music");
        OcProxy::Inst()->setBoolValueForKey(true, "Sound");
        OcProxy::Inst()->setBoolValueForKey(true, "SkillSound");
        isMusicOn = true;
        isSoundOn = true;
        isSkillOn = true;
        
        isCntryCdRemind = true;
        isActivityRemind = true;
        isTwoUpRemind = true;
        intPowerRemind = 0;
    }
    else {
        isMusicOn = OcProxy::Inst()->getBoolValueForKey("Music");
        isSoundOn = OcProxy::Inst()->getBoolValueForKey("Sound");
        isSkillOn = OcProxy::Inst()->getBoolValueForKey("SkillSound");
        
    }

    m_bNeedToReqCollection = true;
    m_bNeedToReqHelper = true;
    
    m_usrInfo.login_time = 0;
        
    // 默认排序
    m_defaultRangeType = enCardRangeTypeProp;
    m_defaultFrRangeType = enFriendRangePro;
    m_memberRangeType = enMemberRangeStatus;

    m_isFirstShowCompGachaHelp = true;
    
    m_commonInfo.bOpen_month_charge = false;
    
    m_usrInfo.bSpecialIap[enSpecialIap_6RMBGift_00] = false;
    m_usrInfo.bSpecialIap[enSpecialIap_30RMBGift_12] = false;
    m_usrInfo.bSpecialIap[enSpecialIap_198RMBGift_13] = false;
    m_usrInfo.bSpecialIap[enSpecialIap_30RMBMonth_11] = false;
    
    OcProxy::Inst()->deleteCache();
}

//******************************************************************************
// setMusicValue
//******************************************************************************
void CGameData::setMusicValue(bool value)
{
    OcProxy::Inst()->setBoolValueForKey(value, "Music");
    isMusicOn = value;
}

//******************************************************************************
// setSoundValue
//******************************************************************************
void CGameData::setSoundValue(bool value)
{
    OcProxy::Inst()->setBoolValueForKey(value, "Sound");
    isSoundOn = value;
}

//******************************************************************************
// setskill value
//******************************************************************************
void CGameData::setSkillValue(bool value)
{
    OcProxy::Inst()->setBoolValueForKey(value, "SkillSound");
    isSkillOn = value;
}

//******************************************************************************
//setyuxiremindvalue
//******************************************************************************
void CGameData::setCntryCdRemindValue(bool value)
{
    isCntryCdRemind = value;
}

//******************************************************************************
//setactivityremindvalue
//******************************************************************************
void CGameData::setActivityRemindValue(bool value)
{
    isActivityRemind = value;
}

//******************************************************************************
//settwoupremindvalue
//******************************************************************************
void CGameData::setTwoupRemindValue(bool value)
{
    isTwoUpRemind = value;
}

//******************************************************************************
//setpowerremindvalue
//******************************************************************************
void CGameData::setPowerRemindValue(int value)
{
    intPowerRemind = value;
}

//******************************************************************************
//setpowerremindvalue
//******************************************************************************
void CGameData::RemindReq()
{
    HttpComm::Inst()->setPushConfig(isActivityRemind, isTwoUpRemind, isCntryCdRemind, intPowerRemind);
}




//******************************************************************************
// loadLanguageBag
//******************************************************************************
bool CGameData::loadLanguageBag()
{
    return HttpComm::Inst()->loadLanguageBagFromLocal();
}

//******************************************************************************
// isUpdateLanguageBag
//******************************************************************************
bool CGameData::isUpdateLanguageBag() {
    return HttpComm::Inst()->isUpdateLanguageBag();
}

//******************************************************************************
// setLanguageBagVersion
//******************************************************************************
void CGameData::setLanguageBagVersion(float languageBagVersion) {
    //[gGameData setLanguageBagVersion:languageBagVersion];
}

//******************************************************************************
// setCompGachaNumber
//******************************************************************************
void CGameData::setCompGachaNumber(int number)
{
    compGachaNumber = number;
}

//******************************************************************************
// needToReqCollection
//******************************************************************************
bool CGameData::needToReqCollection()
{
    return m_bNeedToReqCollection;
}

void CGameData::setIsNeedToReqCollection(bool isNeed)
{
    m_bNeedToReqCollection = isNeed;
}

//******************************************************************************
// needToReqHelper
//******************************************************************************
bool CGameData::needToReqHelper()
{
    if(m_pFriendHelpArr->count() == 0)
        m_bNeedToReqHelper = true;
    
    return m_bNeedToReqHelper;
}

//******************************************************************************
// setNeedToReqHelper
//******************************************************************************
void CGameData::setNeedToReqHelper(bool bNeed)
{
    m_bNeedToReqHelper = bNeed;
}

//******************************************************************************
// getSelectedHelperCount
//******************************************************************************
int CGameData::getSelectedHelperCount()
{
    int count = 0;
    for (int i = 0; i < kHelperMaxCount; i++) {
        if (m_iSelectedHelper[i] != -1)
        {
            count++;
        }
    }
    
    return count;
}

//******************************************************************************
// resetSelectedHelper
//******************************************************************************
void CGameData::resetSelectedHelper()
{
    for (int i = 0; i < kHelperMaxCount; i++) {
        m_iSelectedHelper[i] = -1;
    }
}

//******************************************************************************
// getSelectedHelperInfo
//******************************************************************************
CFriendHelpInfo* CGameData::getSelectedHelperInfo(int index)
{
    if(m_iSelectedHelper[index] >= m_pFriendHelpArr->count()
       || m_iSelectedHelper[index] == -1)
        return NULL;
    
    CFriendHelpInfo* pInfo = m_pFriendHelpArr->getObjectAtIndex(m_iSelectedHelper[index]);
    
    return pInfo;
}

//******************************************************************************
// setSelectedHelper
//******************************************************************************
void CGameData::setSelectedHelper(char* fid, int index)
{
    if (fid == NULL)
    {   // fid是空的，表示未选中好友
        m_iSelectedHelper[index] = -1;
        return;
    }
    
    CFriendHelpInfo* pInfo;
    
    for(int i = 0; i < m_pFriendHelpArr->count(); i++){
        pInfo = m_pFriendHelpArr->getObjectAtIndex(i);
        
        if(strncmp(pInfo->fid, fid, 99) == 0){
            m_iSelectedHelper[index] = i;
            break;
        }
    }
}

//******************************************************************************
// getIsNiePanFoodInPackage
//******************************************************************************
bool CGameData::getIsNiePanFoodInPackage()
{
    char* baseUcid = getCombineCard(0);
    if (baseUcid[0] == 0)
        return false;
    
    CUserCardInfo* BaseCard = getUsrCardInfByUcid(baseUcid);
    if (BaseCard == NULL)
        return false;

    list<short> listNPNeed = BaseCard->np_list_need;
    
    getUserCards();
    for (int i = 0; i < m_pUserCardArr->count(); i++) {
        CUserCardInfo* userCard = m_pUserCardArr->getObjectAtIndex(i);
        
        for (list<short>::iterator itCid = listNPNeed.begin();
             itCid != listNPNeed.end();
             ++itCid) {
            short tempCid = *itCid;
            if (userCard->cid == tempCid)
                return true;
        }
    }
    
    return false;
}

//******************************************************************************
// getIsNiePanFoodByCid
//******************************************************************************
bool CGameData::getIsNiePanFoodByCid(short cid)
{    
    char* baseUcid = getCombineCard(0);
    if (baseUcid[0] == 0)
        return false;
    
    CUserCardInfo* userCard = getUsrCardInfByUcid(baseUcid);
    if (userCard == NULL)
        return false;
    
    for (list<short>::iterator itCid = userCard->np_list_need.begin();
         itCid != userCard->np_list_need.end();
         ++itCid)
    {
        short tempCid = *itCid;
        if (cid == tempCid)
            return true;
    }
    
    return false;
}

//******************************************************************************
// getUcidFromCombineByCid
//******************************************************************************
const char* CGameData::getUcidFromCombineByCid(short cid)
{
    if (getCombineCardCnt() == 0)
        return NULL;
    
    char* ucid;
    CUserCardInfo* pUsrCard;

    for(int i = 1; i < 6; i++){
        ucid = CGameData::Inst()->getCombineCard(i);
        if(ucid[0] == 0)
            break;

        pUsrCard = CGameData::Inst()->getUsrCardInfByUcid(ucid);
        if (pUsrCard->cid == cid)
            return pUsrCard->ucid;
    }
    
    return NULL;
}

//******************************************************************************
// setCombineQueue
//******************************************************************************
void CGameData::setCombineQueue(int idx, char* uid)
{
    if((idx < 0) || (idx > 10))
        return;
    
    if(uid == 0)
        m_CombineQueue[idx].uid[0] = 0;
    else
        strncpy(m_CombineQueue[idx].uid, uid, 99);
}

//******************************************************************************
// getCombineQueue
//******************************************************************************
char* CGameData::getCombineCard(int idx)
{
    if((idx < 0) || (idx > 10))
        return NULL;
    
    return m_CombineQueue[idx].uid;
}

//******************************************************************************
// clearCombineQueue
//******************************************************************************
void CGameData::clearCombineQueue()
{
//    memset(m_CombineQueue, 0, sizeof(m_CombineQueue));
    for (int i = 0; i < MaxCombineCnt; ++i)
    {
        m_CombineQueue[i].cost = 0;
        memset(m_CombineQueue[i].uid, 0, sizeof(m_CombineQueue[i].uid));
    }
}

//******************************************************************************
// clearCombineQueue
//******************************************************************************
int CGameData::getCombineCardCnt()
{
    int cnt = 0;
    
    for(int i = 1; i < 11; i++)
    {
        if(m_CombineQueue[i].uid[0] == 0){
            cnt = i -1;
            break;
        }
        else{
            cnt = i;
        }
    }
    
    return cnt;
}

//******************************************************************************
// getCardBaseInfByCid
//******************************************************************************
void CGameData::setCidBeforeEvo(int cid)
{
    m_iCidBeforeEvo = cid;
}

//******************************************************************************
// getCardBaseInfByCid
//******************************************************************************
int CGameData::getCidBeforeEvo()
{
    return m_iCidBeforeEvo;
}

//******************************************************************************
// setCidAfterEvo
//******************************************************************************
void CGameData::setCidAfterEvo(int cid)
{
    m_iCidAfterEvo = cid;
}

//******************************************************************************
// getCidAfterEvo
//******************************************************************************
int CGameData::getCidAfterEvo()
{
    return m_iCidAfterEvo;
}

//******************************************************************************
// getMaxFriendNum
//******************************************************************************
int CGameData::getMaxFriendNum()
{
    return m_iMaxFirendNum;
}

//******************************************************************************
// getCurFriendNum
//******************************************************************************
int CGameData::getCurFriendNum()
{
    return m_iCurFirendNum;
}

//******************************************************************************
// getCurAvailDiamond
//******************************************************************************
CAvailDiamond* CGameData::getCurAvailDiamond()
{
    return &m_curAvailDiamond;
}

//******************************************************************************
// setCurAvailDiamond
//******************************************************************************
void CGameData::setCurAvailDiamond(char cCnt, char* pBuf)
{
    m_curAvailDiamond.set(cCnt, pBuf);
}

//******************************************************************************
// setDiamondAllAvail
//******************************************************************************
void CGameData::setDiamondAllAvail()
{
    m_curAvailDiamond.setAllAvail();
}

//******************************************************************************
// updateCoin
//******************************************************************************
void CGameData::updateCoin()
{
    getUsrInfo();

    bool isAlipay = false;
#if defined (alipay)  || defined (kPltfmPp)
    isAlipay = true;
#endif
    
    if(m_usrInfo.bSpecialIap[enSpecialIap_6RMBGift_00])
    {
        OcProxy::Inst()->addProduct(kProductId00, isAlipay);
    }
    else
    {
        OcProxy::Inst()->rmvProduct(kProductId00);
    }
    
    if (m_usrInfo.bSpecialIap[enSpecialIap_30RMBGift_12])
    {
        OcProxy::Inst()->addProduct(kProductId12, isAlipay);
    }
    else
    {
        OcProxy::Inst()->rmvProduct(kProductId12);
    }
    
    if (m_usrInfo.bSpecialIap[enSpecialIap_198RMBGift_13])
    {
        OcProxy::Inst()->addProduct(kProductId13, isAlipay);
    }
    else
    {
        OcProxy::Inst()->rmvProduct(kProductId13);
    }
    
    if (CGameData::Inst()->getCommonInfo()->bOpen_month_charge)
    {
        OcProxy::Inst()->addProduct(kProductId11,isAlipay);
        OcProxy::Inst()->addProduct(kProductId09, isAlipay);
    }
    else
    {
        OcProxy::Inst()->rmvProduct(kProductId11);
         OcProxy::Inst()->rmvProduct(kProductId09);
    }
    
   
    
}

int CGameData::getEnterBackTime()
{
    return enterBackTime;
}
int CGameData::getComeBackTime()
{
    return comeBackTime;
}
void CGameData::setEnterBackTime(int time)
{
    enterBackTime = time;
}
void CGameData::setComeBackTime(int time)
{
    comeBackTime = time;
}

int CGameData::getLocalAcPoint()
{
    return localAcPoint;
}

void CGameData::setLocalAcPoint(int acPoint)
{
    localAcPoint = acPoint;
}

int  CGameData::getLocalStamina()
{
    return localStamina;
}

void CGameData::setLocalStamina(int stamina)
{
    localStamina = stamina;
}

enCardRangeType CGameData::getDefaultRangeType()
{
    return m_defaultRangeType;
}

void CGameData::setDefaultRangeType(enCardRangeType type)
{
    m_defaultRangeType = type;
}

enFriendRange  CGameData::getDefaultFrRangeType()
{
    return m_defaultFrRangeType;
}

void CGameData::setDefaultFrRangeType(enFriendRange type)
{
    m_defaultFrRangeType = type;
}

//******************************************************************************
// setIsUseInviteCode
//******************************************************************************
void CGameData::setIsUseInviteCode(bool bUseInviteCode)
{
    HttpComm::Inst()->setIsUseInviteCode(bUseInviteCode);
}

//******************************************************************************
// getIsUseInviteCode
//******************************************************************************
bool CGameData::getIsUseInviteCode()
{
    return HttpComm::Inst()->getIsUseInviteCode();
}

//******************************************************************************
// getIsFirstShowCompGachaHelp
//******************************************************************************
bool CGameData::getIsFirstShowCompGachaHelp()
{
    return m_isFirstShowCompGachaHelp;
}

//******************************************************************************
// setIsFirstShowCompGachaHelp
//******************************************************************************
void CGameData::setIsFirstShowCompGachaHelp(bool isFirst)
{
    m_isFirstShowCompGachaHelp = isFirst;
}

//******************************************************************************
// getHasShowCntryNotice
//******************************************************************************
bool CGameData::getHasShowCntryNotice()
{
    return OcProxy::Inst()->getBoolValueForKey("isShowCntryNotice");
}

//******************************************************************************
// setHasShowCntryNotice
//******************************************************************************
void CGameData::setHasShowCntryNotice()
{
    OcProxy::Inst()->setBoolValueForKey(true, "isShowCntryNotice");
}

//******************************************************************************
// isEnterHelp
//******************************************************************************
bool CGameData::isEnterHelp()
{
    bool isEnterHelp = OcProxy::Inst()->getBoolValueForKey("isEnterHelp");
    
    return  isEnterHelp;
}

//******************************************************************************
// setIsEnterHelp
//******************************************************************************
//设置已经入帮助过
void CGameData::setIsEnterHelp()
{
    OcProxy::Inst()->setBoolValueForKey(true, "isEnterHelp");
}

//******************************************************************************
// Check record video
//******************************************************************************
// 是否能录制视频
bool CGameData::CheckRecordVideo(const char* dgType, int floorIndex, int roomIndex)
{
    int rt = 0;
    if (dgType == NULL)
        return rt;
    if (strcmp(dgType, "normal") == 0)
    {
        CNmlDgInfo* info = m_pNmlDgArr->getObjectAtIndex(floorIndex);
        CDgRoomInfo* room = info->m_pRoomArr->getObjectAtIndex(roomIndex);
        rt = room->video;
    }
    else if (strcmp(dgType, "special")==0 || strcmp(dgType, "weekly")==0 )
    {
        // 由于special里 floor_id 和 room_id 不是0，1，2，3，4 排序的
        CSpecialDgInfo* info = m_pSpecialDgArr->getObjectAtIndex(floorIndex);
        CDgRoomInfo* room = info->m_pRoomArr->getObjectAtIndex(roomIndex);
        rt = room->video;
    }
    else if (strcmp(dgType, "sgyy") == 0)
    {
        CSangoYYDgInfo* info = m_pSangoYYArr->getObjectAtIndex(floorIndex);
        cSangoYYRoom* room = info->m_pRoomArr->getObjectAtIndex(roomIndex);
        rt = room->video;
    }
    return rt;
}

//******************************************************************************
// setIsFighting
//******************************************************************************
void CGameData::setIsFighting(bool bFighting)
{
    m_bFighting = bFighting;
}


//******************************************************************************
// isFighting
//******************************************************************************
bool CGameData::isFighting()
{
    return m_bFighting;
}

//******************************************************************************
// setCurFlrStatus
//******************************************************************************
void CGameData::setCurFlrStatus(int istat)
{
    m_iCurFlrStat = istat;
}

//******************************************************************************
// getCurFlrStatus
//******************************************************************************
int CGameData::getCurFlrStatus()
{
    return m_iCurFlrStat;
}

//******************************************************************************
// setIsLock
//******************************************************************************
void CGameData::setIsLock(const char *ucid, bool isLock) {
    OcProxy::Inst()->setIsLock(ucid, isLock);
}

//******************************************************************************
// getIsLock
//******************************************************************************
bool CGameData::getIsLock(const char *ucid) {
    return OcProxy::Inst()->getIsLock(ucid);
}

//******************************************************************************
// getCardHeadFileName
//******************************************************************************
void CGameData::getCardHeadFileName(unsigned short ushCardID,char *pchBuf,unsigned int unLength)
{
    if (getIsUseJiangNanStyle() && (246==ushCardID || 247==ushCardID))
    {
        snprintf(pchBuf, unLength, "bk_%d_head.png", ushCardID);
    }
    else
    {
        snprintf(pchBuf, unLength, "%d_head.png", ushCardID);
    }
}

//******************************************************************************
// getHeadSprite
//******************************************************************************
CCSprite* CGameData::getHeadSprite(unsigned short cardId)
{
    CCTexture2DPixelFormat emDefault = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    
    char buf[100];
    getCardHeadFileName(cardId,buf,100);
    CCSprite* sp = CCSprite::spriteWithFile(buf);
    if (!sp)
    {
        getCardHeadFileName(1,buf,100);
        sp = CCSprite::spriteWithFile(buf);
    }
    
    CCTexture2D::setDefaultAlphaPixelFormat(emDefault);
    return sp;
}

//******************************************************************************
// getBodyFileName
//******************************************************************************
const char* CGameData::getBodyFileName(unsigned short cardId) {
    static char buf[100];
    
    if (getIsUseJiangNanStyle()
        && (cardId == 246 || cardId == 247)) {
        snprintf(buf, 99, "bk_%d_body.png", cardId);
    }
    else {
        snprintf(buf, 99, "%d_body.png", cardId);
    }
    if (!CCTextureCache::sharedTextureCache()->addImage(buf))
    {
        snprintf(buf, 99, "%d_body.png", 1);
    }
    return buf;
}

//******************************************************************************
// getBodySprite
//******************************************************************************
CCSprite * CGameData::getBodySprite(unsigned short cardId)
{
    CCTexture2DPixelFormat emDefault = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCSprite *pSprite = CCSprite::spriteWithFile(getBodyFileName(cardId));
    CCTexture2D::setDefaultAlphaPixelFormat(emDefault);
    return pSprite;
}

//******************************************************************************
// getSmallDicTime
//******************************************************************************
long CGameData::getSmallDicTime(enSmallDicId smallDicId)
{
    if((smallDicId < 0) || (smallDicId >= enSmallDicMax))
        return 0;
    
    return m_allSmalDicTime[smallDicId];
}

//******************************************************************************
// getEchoTime
//******************************************************************************
unsigned long CGameData::getEchoTime(string date)
{
    unsigned long ldate = OcProxy::Inst()->getAbsTimeFromString(date.c_str(), kOcTimeZone);
    
    return ldate;
}

//******************************************************************************
// getMemberRangeType
//******************************************************************************
enMemberRange CGameData::getMemberRangeType()
{
    return m_memberRangeType;
}

#pragma mark -
#pragma mark TimeTransform
char *CGameData::getCountDownTimeStr(long  time)
{
    
    return TimeTransform::transFormTime(time);
}

char* CGameData::getHourCountDownTimeStr(long time)
{
    return TimeTransform::transHourFormTime(time);
}

time_t CGameData::getCurrentTime()
{
    return HttpComm::Inst()->getTimeStamp();
}

#pragma mark -
#pragma mark payment
//******************************************************************************
// finishPayment
//******************************************************************************
void CGameData::finishPayment()
{
    OcProxy::Inst()->finishPayment();
    

    
    rmvPayTrans();
}

//******************************************************************************
// getStoreStatus
//******************************************************************************
//0: null, 1: loading, -1: fail, 2: success
int  CGameData::getStoreStatus()
{
    return OcProxy::Inst()->getStoreStatus();
}

//******************************************************************************
// getPurchaseStatus
//******************************************************************************
//0: null, 1: purchasing, -1: fail, 2: success 3:tapJoy
int  CGameData::getPurchaseStatus()
{
    return OcProxy::Inst()->getPurchaseStatus();
}

//******************************************************************************
// resetPurchaseStatus
//******************************************************************************
void CGameData::resetPurchaseStatus()
{
    OcProxy::Inst()->resetPurchaseStatus();
}

//******************************************************************************
// getTapjoyStatus
//******************************************************************************
//0: null, 1: purchasing, -1: fail, 2: success 3:tapJoy
int  CGameData::getTapjoyStatus()
{
    return OcProxy::Inst()->getTapjoyStatus();
}

//******************************************************************************
// resetTapjoyStatus
//******************************************************************************
void CGameData::resetTapjoyStatus()
{
    OcProxy::Inst()->resetTapjoyStatus();
}

//******************************************************************************
// loadStore
//******************************************************************************
void CGameData::loadStore()
{
    //init store
    bool isAlipay = false;
#if defined (alipay) || defined (kPltfmPp)
    isAlipay = true;
#endif
    updateCoin();
    OcProxy::Inst()->addProduct(kProductId06,isAlipay);
    OcProxy::Inst()->addProduct(kProductId05,isAlipay);
    OcProxy::Inst()->addProduct(kProductId08,isAlipay);
    OcProxy::Inst()->addProduct(kProductId04,isAlipay);
    OcProxy::Inst()->addProduct(kProductId03,isAlipay);
    OcProxy::Inst()->addProduct(kProductId02,isAlipay);
    OcProxy::Inst()->addProduct(kProductId01,isAlipay);
#if defined (apppay)
    OcProxy::Inst()->loadStore();
#endif
}

//******************************************************************************
// payForCoin
//******************************************************************************
void CGameData::payForCoin(int index)
{
    m_productIdx = index;
    bool isTruePrice = true;
#if defined (alipay_falseprice)
    isTruePrice = false;
#endif
    
#if defined (alipay)
    OcProxy::Inst()->purAlipay(CGameData::getTradeNo(),OcProxy::Inst()->getTitleAtIdx(index),OcProxy::Inst()->getIntPriceAtIdx(index),isTruePrice);
#elif defined (apppay)
    OcProxy::Inst()->purchase(index);
#endif
}

//******************************************************************************
// getProductCnt
//******************************************************************************
int CGameData::getProductCnt()
{
    return OcProxy::Inst()->getProductCnt();
}

//******************************************************************************
// getIapIdAtIdx
//******************************************************************************
const char* CGameData::getIapIdAtIdx(int idx)
{
    return OcProxy::Inst()->getIapIdAtIdx(idx);
}

//******************************************************************************
// getPriceAtIdx
//******************************************************************************
const char* CGameData::getPriceAtIdx(int idx)
{
    return OcProxy::Inst()->getPriceAtIdx(idx);
}

//******************************************************************************
// getTitleAtIdx
//******************************************************************************
const char* CGameData::getTitleAtIdx(int idx)
{
    return OcProxy::Inst()->getTitleAtIdx(idx);
}

//******************************************************************************
// getShopRequest
//******************************************************************************
bool CGameData::getShopRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->shopRequest();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// readMSSM
//******************************************************************************
bool CGameData::readMzsm()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->readMzsm();
        bRet = true;
    }
    
    return bRet;
}

void CGameData::setLocalChatTime(long time){
    CCUserDefault::sharedUserDefault()->setIntegerForKey(kLocalTime, time);
    CCUserDefault::sharedUserDefault()->flush();
}

long CGameData::getLocalChatTime(){
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(kLocalTime);
}

//******************************************************************************
// sendPaySuccessInfo
//******************************************************************************
bool CGameData::sendPaySuccessInfo()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    savePayTrans();
    
    if(reqstat != kReqStatRequesting){
        string iapid = OcProxy::Inst()->getIapIdAtIdx(m_productIdx);
        

        
        HttpComm::Inst()->payRequest(OcProxy::Inst()->getReceipt(),
                                     iapid);
        
        
        
#ifdef TalkData
        
        TDCCVirtualCurrency::onChargeRequest(OcProxy::Inst()->getReceipt(), iapid.c_str(),OcProxy::Inst()->getIntPriceAtIdx(m_productIdx),"CNY", 1, "ios");
#else
        
#endif

        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// savePayTrans
//******************************************************************************
void CGameData::savePayTrans()
{
    const char* szTransId = OcProxy::Inst()->getTransId();
    
    OcProxy::Inst()->setValueForKey(m_usrInfo.uid, szTransId);
    OcProxy::Inst()->synchronizeDefaultData();
}

//******************************************************************************
// rmvPayTrans
//******************************************************************************
void CGameData::rmvPayTrans()
{
    const char* szTransId = OcProxy::Inst()->getTransId();
    
    OcProxy::Inst()->removeObjectForKey(szTransId);
}

//******************************************************************************
// isValidTransId
//******************************************************************************
bool CGameData::isValidTransId()
{
    bool bRet = true;
    
    const char* szTransId = OcProxy::Inst()->getTransId();
    
    const char* szUid = OcProxy::Inst()->getValueForKey(szTransId);
    
    if ((szUid != NULL) && (*szUid != 0)) {
        string strUid = szUid;
        
        if (strUid.compare(m_usrInfo.uid) != 0) {
            bRet = false;
        }
    }

    return bRet;
}

#pragma mark -
#pragma mark data HttpComm
//******************************************************************************
// getLoginType
//******************************************************************************
int CGameData::getLoginType()
{
    return HttpComm::Inst()->getLoginType();
}

//******************************************************************************
// getLastWelfareTime
//******************************************************************************
unsigned long CGameData::getLastWelfareTime(const char *welfareId)
{
    getCntryInfo();
    unsigned long lastWelfareTime = 0;
    Json::Value chargeWealthRecordDic = m_cntryInfo.charge_wealth_record;
    if (chargeWealthRecordDic.isMember(welfareId))
    {
        lastWelfareTime = chargeWealthRecordDic[welfareId].asInt();
    }
    
    return lastWelfareTime;
}

//******************************************************************************
// setNameCard
//******************************************************************************
void CGameData::setNameCard(const char *name)
{
    if (strlen(name) > 0)
        snprintf(m_nameCard, 99, "%s", name);
}

//******************************************************************************
// getNameCard
//******************************************************************************
const char* CGameData::getNameCard()
{
    return m_nameCard;
}

//******************************************************************************
// getDgFloorStatus
//******************************************************************************
int CGameData::getDgFloorStatus(const char* dg_type, int floorid)
{
    if (dg_type == 0)
        return 0;
    
    int status = 0;
    if(getSmallDicTime(enSmallDicUserDungeons) < HttpComm::Inst()->getDicTime(enDicLogin)) {
        m_allSmalDicTime[enSmallDicUserDungeons] = HttpComm::Inst()->getDicTime(enDicLogin);
        m_allUserDugeonsDic = HttpComm::Inst()->getLoginDic()["user_dungeon_info"];
    }

    if (strcmp(dg_type, "normal") == 0) {
        int iCurFloorId = 1;
        if (!m_allUserDugeonsDic.empty()) {
            Json::Value usrNmlDgInfo = m_allUserDugeonsDic["normal_current"];
            iCurFloorId = atoi(usrNmlDgInfo["floor_id"].asString().c_str());
            status = usrNmlDgInfo["status"].asInt();
        }

        Json::Value nmlDgStatus = HttpComm::Inst()->getNmlEndDgDic();
        if (!nmlDgStatus.empty()) {
            iCurFloorId = atoi(nmlDgStatus["floor_id"].asString().c_str());
            status = nmlDgStatus["status"].asInt();
        }

        if(floorid < iCurFloorId){
            return 2;
        }
        else {
            return status;
        }
    }
    else if (strcmp(dg_type, "sgyy") == 0) {
        int iCurFloorId = 1;
        if (!m_allUserDugeonsDic.empty()) {
            Json::Value usrSgyyDgInfo = m_allUserDugeonsDic["sgyy_current"];
            iCurFloorId = atoi(usrSgyyDgInfo["floor_id"].asString().c_str());
            status = usrSgyyDgInfo["status"].asInt();
        }
        
        Json::Value sgyyDgStatus = HttpComm::Inst()->getSangoYYEndDgDic();
        if (!sgyyDgStatus.empty()) {
            iCurFloorId = atoi(sgyyDgStatus["floor_id"].asString().c_str());
            status = sgyyDgStatus["status"].asInt();
        }
        
        if(floorid < iCurFloorId){
            return 2;
        }
        else {
            return status;
        }
    }

    else if(strcmp(dg_type, "special") == 0)
    {
    	Json::Value specialEndDic = HttpComm::Inst()->getSpecialEndDgDic();

        if(!specialEndDic.empty()){
            int iFloorId = atoi(specialEndDic["floor_id"].asString().c_str());
            if(floorid == iFloorId){
                status = specialEndDic["floor_status"].asInt();
                return status;
            }
        }

        char tmp[30] = {'\0'};
        snprintf(tmp, 29, "%d", floorid);
        string strFloor(tmp);
        
        if (m_allUserDugeonsDic["special"].isMember(strFloor)) {
            if (m_newSpecialDgStatusDic.isMember(strFloor))
                status = m_newSpecialDgStatusDic[strFloor]["floor_status"].asInt();
            else
                status = m_allUserDugeonsDic["special"][strFloor]["status"].asInt();
        }
        else
            status = 0;

        return status;
    }

    else if(strcmp(dg_type, "weekly") == 0){
        Json::Value weeklyEndDic = HttpComm::Inst()->getWeeklyEndDgDic();

        if(!weeklyEndDic.empty()){
            int iFloorId = atoi(weeklyEndDic["floor_id"].asString().c_str());
            if(floorid == iFloorId){
                status = weeklyEndDic["floor_status"].asInt();
                return status;
            }
        }

        char tmp[30] = {'\0'};
        snprintf(tmp, 29, "%d", floorid);
        string strFloor(tmp);

        if(m_allUserDugeonsDic["weekly"].isMember(strFloor)) {
            if (m_newWeeklyDgStatusDic.isMember(strFloor))
                status = m_newWeeklyDgStatusDic[strFloor]["floor_status"].asInt();
            else
                status =  m_allUserDugeonsDic["weekly"][strFloor]["status"].asInt();
        }
        else
            status = 0;

        return status;
    }
    
    return 0;
}

//******************************************************************************
// isFirstPassBySaveData
//******************************************************************************
bool CGameData::isFirstPassBySaveData(int curFloorStatus,
                                      const char *dg_type, int iCurFloor)
{
    if (curFloorStatus == 2)
        return false;
 
    int iNewFloorId = 1;
    if (strcmp(dg_type, "normal") == 0) {
        Json::Value nmlDgStatus = HttpComm::Inst()->getNmlEndDgDic();
        if (!nmlDgStatus.empty()) {
            iNewFloorId = atoi(nmlDgStatus["floor_id"].asString().c_str());
            curFloorStatus = nmlDgStatus["status"].asInt();
        }
        
        if(iCurFloor < iNewFloorId)
            curFloorStatus = 2;
    }
    else if (strcmp(dg_type, "sgyy") == 0)
    {
        Json::Value sgyyDgStatus = HttpComm::Inst()->getSangoYYEndDgDic();
        if (!sgyyDgStatus.empty()) {
            iNewFloorId = atoi(sgyyDgStatus["floor_id"].asString().c_str());
            curFloorStatus = sgyyDgStatus["status"].asInt();
        }
        
        if(iCurFloor < iNewFloorId)
            curFloorStatus = 2;
    }
    else if(strcmp(dg_type, "special") == 0)
    {
    	Json::Value specialEndDic = HttpComm::Inst()->getSpecialEndDgDic();
        
        if(!specialEndDic.empty()){
            int iFloorId = atoi(specialEndDic["floor_id"].asString().c_str());
            if(iCurFloor == iFloorId){
                curFloorStatus = specialEndDic["floor_status"].asInt();
            }
        }
    }
    else if(strcmp(dg_type, "weekly") == 0)
    {
        Json::Value weeklyEndDic = HttpComm::Inst()->getWeeklyEndDgDic();
        
        if(!weeklyEndDic.empty()){
            int iFloorId = atoi(weeklyEndDic["floor_id"].asString().c_str());
            if(iCurFloor == iFloorId){
                curFloorStatus = weeklyEndDic["floor_status"].asInt();
            }
        }
    }
    
    if (curFloorStatus == 2)
        return true;
    else
        return false;
}

//******************************************************************************
// getReqStat
//******************************************************************************
int  CGameData::getReqStat()
{
    return HttpComm::Inst()->getReqStatus();
}

//******************************************************************************
// clearReqStat
//******************************************************************************
void CGameData::clearReqStat()
{
    HttpComm::Inst()->clearReqStatus();
}

//******************************************************************************
// getCollection
//******************************************************************************
char* CGameData::getCollection()
{    
    static long slCollectionObtainTm = 0;
    if (slCollectionObtainTm < HttpComm::Inst()->getDicTime(enDicCollection)) {
        slCollectionObtainTm = HttpComm::Inst()->getDicTime(enDicCollection);
    }
    else {
        return m_cCollection;
    }
    
    memset(m_cCollection, 0, sizeof(m_cCollection));
    
    Json::Value collection =  HttpComm::Inst()->getCollectionDic();

    if (!collection.empty()) {
        Json::Value::Members keyArr = collection.getMemberNames();
        
        int cnt = MIN(kMaxCardNum, keyArr.size());

        for(int i = 0; i < cnt; i++){
            string strKey = keyArr[i];
            int cid = atoi(strKey.c_str());
            m_cCollection[cid] = collection[strKey].asInt();
        }
    }
    
    return m_cCollection;
}

//******************************************************************************
// getFriendArr
//******************************************************************************
CCMutableArray<CFriendHelpInfo*> * CGameData::getFriendHelpArr(bool bUpdate)
{
    if((bUpdate == false) && (m_pFriendHelpArr->count() > 0))
        return m_pFriendHelpArr;
    
    m_pFriendHelpArr->removeAllObjects();
    
    Json::Value frddic = NULL;
    
    if(isNewbie()){
        frddic = HttpComm::Inst()->getNewbieDic();
    }
    else {
        frddic = HttpComm::Inst()->getFrdDic();
    }
    
    if (frddic.empty()) {
        return m_pFriendHelpArr;
    }

    CFriendHelpInfo* frdInfo;
    
    //friend
    Json::Value array = frddic["friend"];
    for (int i =0; i< array.size(); i++) {
        Json::Value frd = array[i];
        
        frdInfo = new CFriendHelpInfo();
        
        if (frd.isMember("title_name"))
            strncpy(frdInfo->title_name, frd["title_name"].asString().c_str(), 99);
        else
            strncpy(frdInfo->title_name, "初出茅庐", 99);
        
        strncpy(frdInfo->name, frd["name"].asString().c_str(), 99);
        strncpy(frdInfo->fid, frd["fid"].asString().c_str(), 99);
        strncpy(frdInfo->type, "friend", 15);

        frdInfo->gacha_pt = frd["gacha_pt"].asInt();
        frdInfo->user_elem = frd["country"].asInt();
        frdInfo->lv = frd["lv"].asInt();
        
        Json::Value leader = frd["leader_card"];
        frdInfo->leadercard_cid = atoi(leader["cid"].asString().c_str());
        frdInfo->leadercardlv = leader["lv"].asInt();
        frdInfo->sk_lv = leader["sk_lv"].asInt();
        
        frdInfo->pl_attack = leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
        frdInfo->pl_hp =leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
        frdInfo->pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
        frdInfo->plus =  frdInfo->pl_attack + frdInfo->pl_hp + frdInfo->pl_recover;
        frdInfo->i_awakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
        
        frdInfo->leader_ucid = frd.isMember("leader_ucid") ? frd["leader_ucid"].asString() : "";
        
        if(frdInfo->leadercard_cid == 0){
            frdInfo->release();
            continue;
        }
        else{
            m_pFriendHelpArr->addObject(frdInfo);
            frdInfo->release();
        }
    }
    
    int iFrCnt = m_pFriendHelpArr->count();
    
    //other
    array = frddic["other"];
    for (int i =0; i< array.size(); i++) {
        Json::Value frd = array[i];
        
        frdInfo = new CFriendHelpInfo();
        
        if (frd.isMember("title_name"))
            strncpy(frdInfo->title_name, frd["title_name"].asString().c_str(), 99);
        else
            strncpy(frdInfo->title_name, "初出茅庐", 99);
        
        strncpy(frdInfo->name, frd["name"].asString().c_str(), 99);
        strncpy(frdInfo->fid, frd["fid"].asString().c_str(), 99);
        strncpy(frdInfo->type, "other", 15);
    
        frdInfo->gacha_pt = frd["gacha_pt"].asInt();
        frdInfo->user_elem = frd["country"].asInt();
        frdInfo->lv = frd["lv"].asInt();
        
        Json::Value leader = frd["leader_card"];
        frdInfo->leadercard_cid = atoi(leader["cid"].asString().c_str());
        frdInfo->leadercardlv = leader["lv"].asInt();
        frdInfo->sk_lv = leader["sk_lv"].asInt();
        
        frdInfo->pl_attack = leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
        frdInfo->pl_hp =leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
        frdInfo->pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
        frdInfo->plus =  frdInfo->pl_attack + frdInfo->pl_hp + frdInfo->pl_recover;
        
        frdInfo->leader_ucid = frd.isMember("leader_ucid") ? frd["leader_ucid"].asString() : "";
        
        if(frdInfo->leadercard_cid == 0){
            frdInfo->release();
            continue;
        }
        else{
            m_pFriendHelpArr->addObject(frdInfo);
            frdInfo->release();
        }
    }
    
    int iOtherCnt = m_pFriendHelpArr->count() - iFrCnt;
    
    while (iOtherCnt > 3) {
        int iRand = rand() % iOtherCnt;
        
        m_pFriendHelpArr->removeObjectAtIndex(iFrCnt + iRand);
        iOtherCnt = m_pFriendHelpArr->count() - iFrCnt;
    }
    
    return m_pFriendHelpArr;
}

//******************************************************************************
// getSchCntryInfo
//******************************************************************************
CCntryBaseInfo* CGameData::getSchCntryInfo()
{
    if (HttpComm::Inst()->getSchCntryDic().empty())
        return NULL;
    
    Json::Value cntryInfoDic = HttpComm::Inst()->getSchCntryDic();
    
    snprintf(m_schCntryInfo.name, 99, "%s", cntryInfoDic["gname"].asString().c_str());
    snprintf(m_schCntryInfo.cid, 99, "%s", cntryInfoDic["gid"].asString().c_str());
    snprintf(m_schCntryInfo.declaration, 199, "%s", cntryInfoDic["gdeclaration"].asString().c_str());
    snprintf(m_schCntryInfo.leader_name, 99, "%s", cntryInfoDic["gleader"].asString().c_str());
    snprintf(m_schCntryInfo.member_num, 99, "%s", cntryInfoDic["gnumber"].asString().c_str());
    
    m_schCntryInfo.bApply = cntryInfoDic["is_applyguild"].asBool();
    
    m_schCntryInfo.lv = cntryInfoDic["glevel"].asInt();
    m_schCntryInfo.gexp_total = cntryInfoDic["gexp_total"].asInt();
    
    
    return &m_schCntryInfo;
}

//******************************************************************************
// getBuffInfo
//******************************************************************************
CBuffInfo* CGameData::getBuffInfo(enBuffTypeDef buffType, int buffLv)
{
    if (getSmallDicTime(enSmallDicCntryConfig) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicCntryConfig] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_cntryConfigDic = HttpComm::Inst()->getCfgDic()["guild_config"];
    }
    
    if (m_cntryConfigDic.empty() || !m_cntryConfigDic.isMember("guild_buff_grade")) {
        return NULL;
    }
    
    Json::Value buffGradeDic = m_cntryConfigDic["guild_buff_grade"];
    if (buffGradeDic.empty()) {
        return NULL;
    }

    if (buffType < enBuffType_Stamina || buffType > enBuffType_Hp)
        return NULL;
    
    char buf[16];
    snprintf(buf, 15, "%d", buffLv);
    
    if (!buffGradeDic.isMember(buffTypeArr[buffType])
        ||buffGradeDic[buffTypeArr[buffType]].empty()
        ||!buffGradeDic[buffTypeArr[buffType]].isMember(buf)
        ||buffGradeDic[buffTypeArr[buffType]][buf].empty())
        return NULL;
    
    Json::Value buffDic = buffGradeDic[buffTypeArr[buffType]][buf];
    m_buffInfo.coin = buffDic["coin"].asInt();
    m_buffInfo.exp = buffDic["exp"].asInt();
    m_buffInfo.effect = buffDic["value"].asDouble();
    m_buffInfo.lv = buffLv;
    
    return &m_buffInfo;
}

//******************************************************************************
// getCntryCurrentInfoURL
//******************************************************************************
const char* CGameData::getCntryCurrentInfoURL()
{
    return HttpComm::Inst()->getCntryCurrentInfoURL().c_str();
}

//******************************************************************************
// getWelfareInfoList
//******************************************************************************
CCMutableArray<CWelfareInfo*>* CGameData::getWelfareInfoList()
{
    static long slCfgObtainTm = 0;
    bool bUpdate = false;
    
    if (slCfgObtainTm < HttpComm::Inst()->getDicTime(enDicCfg)) {
        bUpdate = true;
        slCfgObtainTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    
    if (!bUpdate)
        return m_pWelfareInfoArr;

    if (getSmallDicTime(enSmallDicCntryConfig) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicCntryConfig] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_cntryConfigDic = HttpComm::Inst()->getCfgDic()["guild_config"];
    }
    
    if (m_cntryConfigDic.empty() || !m_cntryConfigDic.isMember("chargewealth_config")) {
        return NULL;
    }
    
    Json::Value allWelfareDic = m_cntryConfigDic["chargewealth_config"];
    
    m_pWelfareInfoArr->removeAllObjects();
    
    for (int i = 0; i < allWelfareDic.size(); i++) {
        Json::Value welfareDic = allWelfareDic[i];
        CWelfareInfo* welfareInfo = new CWelfareInfo();
        
        snprintf(welfareInfo->wid, 99, "%s", welfareDic["id"].asString().c_str());
        snprintf(welfareInfo->description, 199, "%s", welfareDic["description"].asString().c_str());
        welfareInfo->needLv = welfareDic["level"].asInt();
        welfareInfo->needSupply = welfareDic["wealth"].asInt();
        if (welfareDic.isMember("image"))
            welfareInfo->imageId = welfareDic["image"].asInt();
        else
            welfareInfo->imageId = 1;
        welfareInfo->cd_time = welfareDic["cd_time"].asInt();
        
        m_pWelfareInfoArr->addObject(welfareInfo);
        welfareInfo->release();
    }
    
    return m_pWelfareInfoArr;
}

//******************************************************************************
// getCntryInfo
//******************************************************************************
CCountryInfo* CGameData::getCntryInfo()
{
    static long slCntryInfoObtainTm = 0;
    
    if (slCntryInfoObtainTm < HttpComm::Inst()->getDicTime(enDicCntryInf)) {
        slCntryInfoObtainTm = HttpComm::Inst()->getDicTime(enDicCntryInf);
    }
    else
        return &m_cntryInfo;
    
    Json::Value cntryDic = HttpComm::Inst()->getCntryInfoDic();
    if (cntryDic.empty()) {
        m_cntryInfo.status = 0; // 没有加入国家
        return &m_cntryInfo;
    }

    
//    CCLog("card info = %s", cntryDic.toStyledString().c_str());
    
    
    Json::Value cntryBaseDic = cntryDic["guild_base"];
    Json::Value cntryUserDic = cntryDic["guild_user"];

    m_cntryInfo.cntry_delete_time = cntryBaseDic["guild_delete_time"].asInt();
    m_cntryInfo.cur_exp = cntryBaseDic["gexp"].asInt();
    m_cntryInfo.need_exp = cntryBaseDic["guild_lv_exp"].asInt();
    m_cntryInfo.lv = cntryBaseDic["glevel"].asInt();
    m_cntryInfo.cntry_coin = cntryBaseDic["coin"].asInt();
    m_cntryInfo.cntry_contribution = cntryBaseDic["contribute"].asInt();
    m_cntryInfo.cntry_supply = cntryBaseDic["wealth"].asInt();
    m_cntryInfo.member_cur_num = cntryBaseDic["guildmembernum"].asInt();
    m_cntryInfo.member_max_num = cntryBaseDic["guild_member_lv"].asInt();
    m_cntryInfo.last_chat_time = cntryBaseDic["last_chat_time"].asInt();
    
    for (int i = 0; i < 4; i++) {
        m_cntryInfo.buffInfo[i].lv = cntryBaseDic["gbuff"][buffTypeArr[i]].asInt();
        
        CBuffInfo* curBuffInfo = getBuffInfo((enBuffTypeDef)i, m_cntryInfo.buffInfo[i].lv);
        m_cntryInfo.buffInfo[i].effect = (curBuffInfo == NULL) ? 0 : curBuffInfo->effect;
        
        CBuffInfo* nextLvBuffInfo = getBuffInfo((enBuffTypeDef)i, m_cntryInfo.buffInfo[i].lv + 1);
        m_cntryInfo.buffInfo[i].coin = (nextLvBuffInfo == NULL) ? 0 : nextLvBuffInfo->coin;
        m_cntryInfo.buffInfo[i].exp = (nextLvBuffInfo == NULL) ? 0 : nextLvBuffInfo->exp;        
    }
    
    snprintf(m_cntryInfo.cid, 99, "%s", cntryBaseDic["gid"].asString().c_str());
    snprintf(m_cntryInfo.declaration, 199, "%s", cntryBaseDic["gdeclaration"].asString().c_str());
    snprintf(m_cntryInfo.name, 99, "%s", cntryBaseDic["gname"].asString().c_str());
    snprintf(m_cntryInfo.leader_name, 99, "%s", cntryBaseDic["gleadername"].asString().c_str());
    
    m_cntryInfo.status = cntryUserDic["guildrole"].asInt();
    m_cntryInfo.user_contribution = cntryUserDic["contribute"].asInt();
    m_cntryInfo.user_supply = cntryUserDic["wealth"].asInt();
    m_cntryInfo.member_apply_num = cntryUserDic["apply_num"].asInt();
    m_cntryInfo.org_dungeon_num = cntryUserDic["dungeon_num"].asInt();
    m_cntryInfo.charge_wealth_record = cntryUserDic["charge_wealth_record"];
    m_cntryInfo.can_contribute_coin = cntryUserDic["can_contribute_coin"].asInt();
    snprintf(m_cntryInfo.nameCard, 99, "%s", cntryUserDic["name"].asString().c_str());
    
    if (cntryUserDic.isMember("task_items")) {
        if (m_cntryInfo.m_pGuildItemInStore == NULL) {
            m_cntryInfo.m_pGuildItemInStore = new CCMutableArray<GuildItemInStore*>();
        }
        
        m_cntryInfo.m_pGuildItemInStore->removeAllObjects();
        
        Json::Value itmCfgDic = HttpComm::Inst()->getCfgDic()["guild_config"]["guild_task_shop"];
        
        Json::Value::Members itemIdArr = cntryUserDic["task_items"].getMemberNames();
        for (int i = 0; i < itemIdArr.size(); i++) {
            GuildItemInStore* pGuildItemInStore = new GuildItemInStore();
            
            pGuildItemInStore->strItemId = itemIdArr[i];
            pGuildItemInStore->lCnt = cntryUserDic["task_items"][pGuildItemInStore->strItemId].asInt();
            
            if (!itmCfgDic.empty())
            {
                pGuildItemInStore->strImgName = itmCfgDic[pGuildItemInStore->strItemId]["item_img"].asString();
                pGuildItemInStore->strItemName = itmCfgDic[pGuildItemInStore->strItemId]["name"].asString();
                pGuildItemInStore->strItemDsp = itmCfgDic[pGuildItemInStore->strItemId]["item_dsp"].asString();
                pGuildItemInStore->lLv = itmCfgDic[pGuildItemInStore->strItemId]["lv"].asInt();
                
                if (itmCfgDic[pGuildItemInStore->strItemId].isMember("condition"))
                {
                    Json::Value conDic = itmCfgDic[pGuildItemInStore->strItemId]["condition"];
                    
                    for (int z=0; z < conDic.size(); z++)
                    {
                        switch (z) {
                            case 0:
                                pGuildItemInStore->lMinCondition = conDic[z].asInt();
                                break;
                                
                            case 1:
                                pGuildItemInStore->lMaxCondition = conDic[z].asInt();
                                break;
                                
                            default:
                                break;
                        }
                            
                    }
                }
            }
            
            m_cntryInfo.m_pGuildItemInStore->addObject(pGuildItemInStore);
            pGuildItemInStore->release();
        }
    }

    return &m_cntryInfo;
}


//******************************************************************************
// getUsrInfo
//******************************************************************************
CUserInfo* CGameData::getUsrInfo()
{
    static long slUsrInfObtainTm = 0;
    
    if (slUsrInfObtainTm < HttpComm::Inst()->getDicTime(enDicUsrInf)){
        slUsrInfObtainTm = HttpComm::Inst()->getDicTime(enDicUsrInf);
    }
    else
        return &m_usrInfo;
    
    Json::Value usrdic = HttpComm::Inst()->getUsrInfoDic();
    if(usrdic.empty())
        return &m_usrInfo;
    
    
    
//    CCLog("card info = %s", usrdic.toStyledString().c_str());
    
   
    strncpy(m_usrInfo.username, usrdic["username"].asString().c_str(), 99);
    
    m_usrInfo.title_name = usrdic["title_name"].asString();
    m_usrInfo.new_title_name = usrdic["new_title_name"].asString();
    
    m_usrInfo.free_cd_time = usrdic["free_cd_time"].asInt();

    
    
    if(usrdic["uid"].asString().c_str())
        strncpy(m_usrInfo.uid, usrdic["uid"].asString().c_str(), 99);
    
    if(usrdic["invite_code"].asString().c_str())
        strncpy(m_usrInfo.invite_code, usrdic["invite_code"].asString().c_str(), 99);
    
    m_usrInfo.first_charge = usrdic["first_charge"].asBool();
    
    m_usrInfo.bSpecialIap[enSpecialIap_6RMBGift_00] = usrdic["special_first_charge"].asBool();
    m_usrInfo.bSpecialIap[enSpecialIap_30RMBGift_12] = usrdic["special_coin12_charge"].asBool();
    m_usrInfo.bSpecialIap[enSpecialIap_198RMBGift_13] = usrdic["special_coin13_charge"].asBool();
    m_usrInfo.bSpecialIap[enSpecialIap_30RMBMonth_11] = usrdic["month_card_plan_charge"].asBool();


    m_usrInfo.bSpecialIap[enSpecialIap_198RMBMonth_09] = usrdic["month_coin_plan_charge"].asBool();
    
    m_usrInfo.bSYuebao[enYueBao_zhaocaijinbao] = usrdic["month_card_deadline"].asInt();
    m_usrInfo.bSYuebao[enYueBao_caiyungungun] = usrdic["month_coin_deadline"].asInt();
 //   m_usrInfo.bSpecialIap[enSpecialIap_198RMBMonth_09] = usrdic["month_plan_charge"].asBool();
    

    m_usrInfo.gacha_pt = usrdic["gacha_pt"].asInt();
    m_usrInfo.coin = usrdic["coin"].asInt();
    m_usrInfo.gold = usrdic["gold"].asUInt();
    m_usrInfo.user_elem = usrdic["country"].asInt();
    m_usrInfo.stamina_upd_time = usrdic["stamina_upd_time"].asInt();
    m_usrInfo.stamina = usrdic["stamina"].asInt();
    m_usrInfo.can_contribute_coin = usrdic["can_contribute_coin"].asInt();
    if (HttpComm::Inst()->isChangeStamina())
    {
        setLocalStamina(m_usrInfo.stamina);
        //        HttpComm::Inst()->setIsChangeStamina(false);
    }
    m_usrInfo.max_stamina = usrdic["max_stamina"].asInt();
    m_usrInfo.max_card_num = usrdic["max_card_num"].asInt();
    m_usrInfo.acPoint_upd_time = usrdic["power_upd_time"].asInt();
    m_usrInfo.actionPoint = usrdic["guild_power"].asInt();
    setLocalAcPoint(m_usrInfo.actionPoint);
    m_usrInfo.max_acPoint = usrdic["max_power"].asInt();
    m_usrInfo.cost = usrdic["cost"].asInt();
    
    m_usrInfo.exp = usrdic["exp"].asInt();
    m_usrInfo.this_lv_now_exp = usrdic["this_lv_now_exp"].asInt();
    m_usrInfo.next_lv_need_exp = usrdic["next_lv_need_exp"].asInt();
    m_usrInfo.lv = usrdic["lv"].asInt();
    m_usrInfo.max_card_num = usrdic["max_card_num"].asInt();
    m_usrInfo.login_time = usrdic["login_time"].asInt();
    
    m_usrInfo.isMzsm = usrdic.isMember("is_mzsm") ? usrdic["is_mzsm"].asBool() : true;
    
    Json::Value leader = usrdic["leader_card"];
    m_usrInfo.leadercard_cid = atoi(leader["cid"].asString().c_str());
    m_usrInfo.leadercardlv = leader["lv"].asInt();
    m_usrInfo.maxFr_num = usrdic["max_friend_num"].asInt();
    m_usrInfo.friend_extend_num = usrdic["friend_extend_num"].asInt();
    m_usrInfo.friend_request_num = usrdic["friend_request_num"].asInt();
    m_usrInfo.last_mail_time = usrdic["mail_time"].asInt();
    
    Json::Value push_sanguo = usrdic["sango_push"];
    if (!push_sanguo.empty()) {
        m_usrInfo.push_special_dunegon = push_sanguo["special_dunegon"].asBool();
        m_usrInfo.push_double_drop = push_sanguo["double_drop"].asBool();
        m_usrInfo.push_guild_charge_cold = push_sanguo["guild_charge_cold"].asBool();
        m_usrInfo.push_stamina_num = push_sanguo["push_stamina_num"].asInt();
    }
    
    Json::Value tapjoyValue = usrdic["tapjoy"];
    if (!tapjoyValue.empty()) {
        m_usrInfo.tjPoints = tapjoyValue["points"].asInt();
        m_usrInfo.tjCoins = tapjoyValue["coins"].asInt();
        m_usrInfo.tjPoints_per_coin = tapjoyValue["points_per_coin"].asInt();
        m_usrInfo.tjMax_coins = tapjoyValue["max_coins"].asInt();
        m_usrInfo.max_fg = tapjoyValue["max_fg"].asBool();
    }
    
    Json::Value newTemp = usrdic["tutorial_record"];
    if (!newTemp.empty()) {
        m_usrInfo.add_866_card = newTemp["add_866_card"].asInt();//太极果
        m_usrInfo.add_award = newTemp["add_award"].asInt();//第一次领奖
        m_usrInfo.add_card = newTemp["add_card"].asInt();//武将加
        m_usrInfo.add_lv_5 = newTemp["add_lv_5"].asInt();//第一次到五级
        m_usrInfo.card_update = newTemp["card_update"].asInt();//强化
        m_usrInfo.card_upgrade = newTemp["card_upgrade"].asInt();//转生
        m_usrInfo.dungeon_fail = newTemp["dungeon_fail"].asInt();//失败次数
        m_usrInfo.guild_item = newTemp["guild_item"].asInt();//随身军袋
    }
    else
    {
        m_usrInfo.add_866_card = -1;//太极果
        m_usrInfo.add_award = -1;//第一次领奖
        m_usrInfo.add_card = -1;//武将加
        m_usrInfo.add_lv_5 = -1;//第一次到五级
        m_usrInfo.card_update = -1;//强化
        m_usrInfo.card_upgrade = -1;//转生
        m_usrInfo.dungeon_fail = -1;//失败次数
        m_usrInfo.guild_item = -1;//随身军袋

    }

    m_usrInfo.hours_append_cost = 0;
    m_usrInfo.hours_endTime = 0;
    m_usrInfo.month_append_cost = 0;
    m_usrInfo.month_endTime = 0;
    
    m_usrInfo.m_lsango_tips_1.clear();

    if ( usrdic.isMember("sango_tips_1")) {
        Json::Value sango_tips_1 = usrdic["sango_tips_1"]; //add1.14
        if (sango_tips_1.size() > 0) {
            for (int i = 0; i < sango_tips_1.size(); i++){
                c_sango_Tips sangotip ;
              //  memset(&sangotip, 0, sizeof(sangotip));
                sangotip.m_content = sango_tips_1[i]["content"].asCString();
              //  string  yy(sangotip.m_content );
               // cout << "llllll" <<  sangotip.m_content << endl;
                sangotip.m_interVal = sango_tips_1[i]["interval"].asInt();
                sangotip.m_iType = sango_tips_1[i]["type"].asInt();
                string s_first= sango_tips_1[i]["start_time"].asCString();
                sangotip.m_sfirstTime = getEchoTime(s_first);
                string s_end = sango_tips_1[i]["end_time"].asCString();
                sangotip.m_sendTime = getEchoTime(s_end);
                m_usrInfo.m_lsango_tips_1.push_back(sangotip);
            }
               
        }
    }
    m_usrInfo.m_lsango_tips_2.clear();
    if (usrdic.isMember("sango_tips_2")) {
         Json::Value sango_tips_2 = usrdic["sango_tips_2"];//add1.14
        if (sango_tips_2.size() > 0) {
            for (int i = 0; i < sango_tips_2.size(); i++) {
                c_sango_Tips_oneTime sangotip_onetime;
              //  memset(&sangotip_onetime, 0, sizeof(sangotip_onetime));
                sangotip_onetime.m_content = sango_tips_2[i]["content"].asCString();
                sangotip_onetime.m_itype = sango_tips_2[i]["type"].asInt();
                m_usrInfo.m_lsango_tips_2.push_back(sangotip_onetime);
            }
        }
    }
    
    
    if (usrdic.isMember("cost_up"))
    {
        Json::Value temp = usrdic["cost_up"];
        if (temp.isMember("hours"))
        {
            if (temp["hours"].isMember("cost"))
            {
                m_usrInfo.hours_append_cost = temp["hours"]["cost"].asInt();
            }
            if (temp["hours"].isMember("end_time"))
            {
                m_usrInfo.hours_endTime = temp["hours"]["end_time"].asInt();
            }
        }
        if (temp.isMember("month"))
        {
            if (temp["month"].isMember("cost"))
            {
                m_usrInfo.month_append_cost = temp["month"]["cost"].asInt();
            }
            if (temp["month"].isMember("end_time"))
            {
                m_usrInfo.month_endTime = temp["month"]["end_time"].asInt();
            }
        }
    }
    
    return &m_usrInfo;
}

//******************************************************************************
// getNmlDgInfo
//******************************************************************************
CCMutableArray<CNmlDgInfo*> * CGameData::getNmlDgInfo()
{
    static long slCfgObtainTm = 0;
    static long slDgEndObtainTm = 0;
    bool bUpdate = false;
    
    if (slCfgObtainTm < HttpComm::Inst()->getDicTime(enDicCfg)) {
        bUpdate = true;
        slCfgObtainTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    
    if (slDgEndObtainTm < HttpComm::Inst()->getDicTime(enDicNmlDgEnd)) {
        bUpdate = true;
        slDgEndObtainTm = HttpComm::Inst()->getDicTime(enDicNmlDgEnd);
    }
        
    if(bUpdate == false)
        return m_pNmlDgArr;
    
    CNmlDgInfo* pNmlDgInfo;
    CDgRoomInfo* pDgRoomInfo;
    
    m_pNmlDgArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllDungeons) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicAllDungeons] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_allBaseDungeonsDic = HttpComm::Inst()->getCfgDic()["dungeon"];
    }
        
    Json::Value effectDic = m_allBaseDungeonsDic["effect"];
    Json::Value nmlDgDic = m_allBaseDungeonsDic["normal"];
    
    if(getSmallDicTime(enSmallDicUserDungeons) < HttpComm::Inst()->getDicTime(enDicLogin)) {
        m_allSmalDicTime[enSmallDicUserDungeons] = HttpComm::Inst()->getDicTime(enDicLogin);
        m_allUserDugeonsDic = HttpComm::Inst()->getLoginDic()["user_dungeon_info"];
    }
    Json::Value usrNmlDgInfo = m_allUserDugeonsDic["normal_current"];
    
    int iCurFloorId = atoi(usrNmlDgInfo["floor_id"].asString().c_str());
    int iCurRoomId = atoi(usrNmlDgInfo["room_id"].asString().c_str());
    int status = usrNmlDgInfo["status"].asInt();
    int floorid, roomid;
    
    Json::Value nmlDgStatus = HttpComm::Inst()->getNmlEndDgDic();
    if(!nmlDgStatus.empty()){
        iCurFloorId = atoi(nmlDgStatus["floor_id"].asString().c_str());
        iCurRoomId = atoi(nmlDgStatus["room_id"].asString().c_str());
        status = nmlDgStatus["status"].asInt();
    }
    
    if(iCurFloorId > m_iCurNmlFloor){
        m_iCurNmlFloor = iCurFloorId;
        m_iCurNmlRoom = iCurRoomId;
    }
    else if((iCurFloorId == m_iCurNmlFloor) && (iCurRoomId > m_iCurNmlRoom)){
        m_iCurNmlRoom = iCurRoomId;
    }

    Json::Value::Members nmlArr = nmlDgDic.getMemberNames();
    for (int i =0; i< nmlArr.size(); i++) {
        pNmlDgInfo = new CNmlDgInfo();
        floorid = i+1;

        char tmp[32] = {'\0'};
        sprintf(tmp,"%d",floorid);
        std::string strNo(tmp);

        Json::Value oneNml = nmlDgDic[strNo];
        
        
        
        
        //wuliang
        string strBgColor = "0";
        if (oneNml.isMember("bgcolor"))
            strBgColor = oneNml["bgcolor"].asString();
        if (strBgColor == "1")
            pNmlDgInfo->bg_type = 1;
        else if (strBgColor == "2")
            pNmlDgInfo->bg_type = 2;
        else if (strBgColor == "3")
            pNmlDgInfo->bg_type = 3;
        else if (strBgColor == "4")
            pNmlDgInfo->bg_type = 4;
        else
            pNmlDgInfo->bg_type = 0;

        //
        
        strncpy(pNmlDgInfo->name, oneNml["name"].asString().c_str(), 99);
        pNmlDgInfo->dungeon_type = atoi(oneNml["dungeon_type"].asString().c_str());
        
        pNmlDgInfo->minlevel = 0;
        pNmlDgInfo->maxlevel = 9999;
        
        if (oneNml.isMember("lv_range"))
        {
            int p1 = 0, p2 = 1;
            pNmlDgInfo->minlevel = oneNml["lv_range"][p1].asInt();
            pNmlDgInfo->maxlevel = oneNml["lv_range"][p2].asInt();
        }
        
        pNmlDgInfo->dungeon_dsp = oneNml.isMember("dungeon_dsp") ? oneNml["dungeon_dsp"].asString() : "";
        
        pNmlDgInfo->m_unLimitMaxStar = oneNml.isMember("max_star")? oneNml["max_star"].asInt() : 0;
        
        pNmlDgInfo->buff_void = oneNml.isMember("buff_void") ? oneNml["buff_void"].asBool() : true;
        
        string strData = oneNml["effect"]["start_time"].asString();
        pNmlDgInfo->effect_starttime = getEchoTime(strData);
        
        strData = oneNml["effect"]["end_time"].asString();
        pNmlDgInfo->effect_endtime = getEchoTime(strData);
        
        long curDate = getCurrentTime();
        
        if((curDate > pNmlDgInfo->effect_endtime)
           || (curDate < pNmlDgInfo->effect_starttime)){
            pNmlDgInfo->effect_remaintime = 0;
        }
        else {
            pNmlDgInfo->effect_remaintime = pNmlDgInfo->effect_endtime - curDate;
        }
        
        string strNumber = oneNml["effect"]["number"].asString();
        strncpy(pNmlDgInfo->effectname, effectDic[strNumber]["name"].asString().c_str(), 99);

        pNmlDgInfo->cStatus = 2;
        
        float fStaminaFactor  = 1.0;
        Json::Value effect_stamina;
        if(pNmlDgInfo->effect_remaintime > 0){
            effect_stamina = effectDic[strNumber]["type"]["stamina"];
            if(!effect_stamina.empty())
                fStaminaFactor = (float)effect_stamina.asDouble();
        }
        
        //统御力限制
        if (oneNml.isMember("cards_cost")) {
            pNmlDgInfo->dgRstrct.cards_cost = oneNml["cards_cost"].asInt();
        }
        else{
            pNmlDgInfo->dgRstrct.cards_cost = 0;
        }
        
        if (oneNml.isMember("max_cost")) {
            pNmlDgInfo->dgRstrct.max_cost = oneNml["max_cost"].asInt();
        }
        else{
            pNmlDgInfo->dgRstrct.max_cost = 0;
        }
        
        //武将属性限制
        if (oneNml.isMember("cards_ctype")) {
            Json::Value ctypeArr = oneNml["cards_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pNmlDgInfo->dgRstrct.cards_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pNmlDgInfo->dgRstrct.cards_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pNmlDgInfo->dgRstrct.cards_ctype, 0xFF, sizeof(pNmlDgInfo->dgRstrct.cards_ctype));
        }
        
        if (oneNml.isMember("only_ctype")) {
            Json::Value ctypeArr = oneNml["only_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pNmlDgInfo->dgRstrct.only_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pNmlDgInfo->dgRstrct.only_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pNmlDgInfo->dgRstrct.only_ctype, 0xFF, sizeof(pNmlDgInfo->dgRstrct.only_ctype));
        }
        
        //rooms
        Json::Value roomsDic = oneNml["rooms"];
        Json::Value::Members roomsArr = roomsDic.getMemberNames();
        for (int j =0; j< roomsArr.size(); j++) {
            pDgRoomInfo = new CDgRoomInfo();
            roomid = j+1;
            
            char tmp[32] = {'\0'};
            sprintf(tmp,"%d", roomid);
            std::string strNo(tmp);
            
            strncpy(pDgRoomInfo->name,roomsDic[strNo]["name"].asString().c_str(),99);
            pDgRoomInfo->stamina = roomsDic[strNo]["stamina"].asInt();
            pDgRoomInfo->propose_lv = roomsDic[strNo]["propose_lv"].asInt();
            if(!effect_stamina.empty())
                pDgRoomInfo->stamina = ceilf(pDgRoomInfo->stamina * fStaminaFactor);
            
            pDgRoomInfo->steps = roomsDic[strNo]["steps"].asInt();
            pDgRoomInfo->video = roomsDic[strNo]["video"].asInt();
            
            pDgRoomInfo->cStatus = 2;
            
            
            if (roomsDic[strNo].isMember("head_tips")) {
                if (pDgRoomInfo->m_pHeadTipsArr == NULL) {
                    pDgRoomInfo->m_pHeadTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["head_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pDgRoomInfo->m_pHeadTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }
            
            if (roomsDic[strNo].isMember("end_tips")) {
                if (pDgRoomInfo->m_pEndTipsArr == NULL) {
                    pDgRoomInfo->m_pEndTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["end_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pDgRoomInfo->m_pEndTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }
            
            pNmlDgInfo->m_pRoomArr->addObject(pDgRoomInfo);
            pDgRoomInfo->release();
            
            if((floorid == m_iCurNmlFloor) && (roomid == m_iCurNmlRoom)){
                pDgRoomInfo->cStatus = status;
                break;
            }
        }
        
        m_pNmlDgArr->addObject(pNmlDgInfo);
        pNmlDgInfo->release();
        
        if(floorid == m_iCurNmlFloor){
            pNmlDgInfo->cStatus = status;
            break;
        }
    }
        
    return m_pNmlDgArr;
}

//******************************************************************************
// getSangoYYDgInfo
//******************************************************************************
bool CGameData::CheckSangoYYExist()
{
    return getSangoYYDgInfo()->count();
}
CCMutableArray<CSangoYYDgInfo*> * CGameData::getSangoYYDgInfo()
{
    static long slCfgObtainTm = 0;
    static long slDgEndObtainTm = 0;
    bool bUpdate = false;
    
    if (slCfgObtainTm < HttpComm::Inst()->getDicTime(enDicCfg))
    {
        bUpdate = true;
        slCfgObtainTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    
    if (slDgEndObtainTm < HttpComm::Inst()->getDicTime(enDicSangoDgEnd)) {
        bUpdate = true;
        slDgEndObtainTm = HttpComm::Inst()->getDicTime(enDicSangoDgEnd);
    }
    
    if(bUpdate == false)
        return m_pSangoYYArr;
    
    CSangoYYDgInfo* pSangoDgInfo;
    cSangoYYRoom* pSangoDgRoomInfo;
    
    m_pSangoYYArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllDungeons) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicAllDungeons] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_allBaseDungeonsDic = HttpComm::Inst()->getCfgDic()["dungeon"];
    }
    
    Json::Value effectDic = m_allBaseDungeonsDic["effect"];
    Json::Value SangoDgDic = m_allBaseDungeonsDic["sgyy"];
    
    if(getSmallDicTime(enSmallDicUserDungeons) < HttpComm::Inst()->getDicTime(enDicLogin)) {
        m_allSmalDicTime[enSmallDicUserDungeons] = HttpComm::Inst()->getDicTime(enDicLogin);
        m_allUserDugeonsDic = HttpComm::Inst()->getLoginDic()["user_dungeon_info"];
    }
    Json::Value usrSangoDgInfo = m_allUserDugeonsDic["sgyy_current"];
    
    int iCurFloorId = atoi(usrSangoDgInfo["floor_id"].asString().c_str());
    int iCurRoomId = atoi(usrSangoDgInfo["room_id"].asString().c_str());
    int status = usrSangoDgInfo["status"].asInt();
    int floorid, roomid;
    
    Json::Value SangoDgStatus = HttpComm::Inst()->getSangoYYEndDgDic();
    if(!SangoDgStatus.empty()){
        iCurFloorId = atoi(SangoDgStatus["floor_id"].asString().c_str());
        iCurRoomId = atoi(SangoDgStatus["room_id"].asString().c_str());
        status = SangoDgStatus["status"].asInt();
    }
    
    if(iCurFloorId > m_iCurSangoFloor){
        m_iCurSangoFloor = iCurFloorId;
        m_iCurSangoRoom = iCurRoomId;
    }
    else if((iCurFloorId == m_iCurSangoFloor) && (iCurRoomId > m_iCurSangoRoom)){
        m_iCurSangoRoom = iCurRoomId;
    }
    
    Json::Value::Members SangoArr = SangoDgDic.getMemberNames();
    for (int i =0; i< SangoArr.size(); i++) {
        pSangoDgInfo = new CSangoYYDgInfo();
        floorid = i+1;
        
        char tmp[32] = {'\0'};
        sprintf(tmp,"%d",floorid);
        std::string strNo(tmp);
        
        Json::Value oneSango = SangoDgDic[strNo];
        
        strncpy(pSangoDgInfo->name, oneSango["name"].asString().c_str(), 99);
        pSangoDgInfo->dungeon_type = atoi(oneSango["dungeon_type"].asString().c_str());
        pSangoDgInfo->dungeon_dsp = oneSango.isMember("dungeon_dsp") ? oneSango["dungeon_dsp"].asString() : "";
        
        pSangoDgInfo->m_unLimitMaxStar = oneSango.isMember("max_star")? oneSango["max_star"].asInt() : 0;
        
        pSangoDgInfo->buff_void = oneSango.isMember("buff_void") ? oneSango["buff_void"].asBool() : true;
        
        string strData = oneSango["effect"]["start_time"].asString();
        pSangoDgInfo->effect_starttime = getEchoTime(strData);
        
        strData = oneSango["effect"]["end_time"].asString();
        pSangoDgInfo->effect_endtime = getEchoTime(strData);
        
        long curDate = getCurrentTime();
        
        if((curDate > pSangoDgInfo->effect_endtime)
           || (curDate < pSangoDgInfo->effect_starttime)){
            pSangoDgInfo->effect_remaintime = 0;
        }
        else {
            pSangoDgInfo->effect_remaintime = pSangoDgInfo->effect_endtime - curDate;
        }
        
        string strNumber = oneSango["effect"]["number"].asString();
        strncpy(pSangoDgInfo->effectname, effectDic[strNumber]["name"].asString().c_str(), 99);
        
        pSangoDgInfo->cStatus = 2;
        
        float fStaminaFactor  = 1.0;
        Json::Value effect_stamina;
        if(pSangoDgInfo->effect_remaintime > 0){
            effect_stamina = effectDic[strNumber]["type"]["stamina"];
            if(!effect_stamina.empty())
                fStaminaFactor = (float)effect_stamina.asDouble();
        }
        
        //统御力限制
        if (oneSango.isMember("cards_cost")) {
            pSangoDgInfo->dgRstrct.cards_cost = oneSango["cards_cost"].asInt();
        }
        else{
            pSangoDgInfo->dgRstrct.cards_cost = 0;
        }
        
        if (oneSango.isMember("max_cost")) {
            pSangoDgInfo->dgRstrct.max_cost = oneSango["max_cost"].asInt();
        }
        else{
            pSangoDgInfo->dgRstrct.max_cost = 0;
        }
        
        //武将属性限制
        if (oneSango.isMember("cards_ctype")) {
            Json::Value ctypeArr = oneSango["cards_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSangoDgInfo->dgRstrct.cards_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSangoDgInfo->dgRstrct.cards_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSangoDgInfo->dgRstrct.cards_ctype, 0xFF, sizeof(pSangoDgInfo->dgRstrct.cards_ctype));
        }
        
        if (oneSango.isMember("only_ctype")) {
            Json::Value ctypeArr = oneSango["only_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSangoDgInfo->dgRstrct.only_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSangoDgInfo->dgRstrct.only_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSangoDgInfo->dgRstrct.only_ctype, 0xFF, sizeof(pSangoDgInfo->dgRstrct.only_ctype));
        }
        
        //rooms
        Json::Value roomsDic = oneSango["rooms"];
        Json::Value::Members roomsArr = roomsDic.getMemberNames();
        for (int j =0; j< roomsArr.size(); j++) {
            pSangoDgRoomInfo = new cSangoYYRoom();
            roomid = j+1;
            
            char tmp[32] = {'\0'};
            sprintf(tmp,"%d", roomid);
            std::string strNo(tmp);
            
            strncpy(pSangoDgRoomInfo->name,roomsDic[strNo]["name"].asString().c_str(),99);
            pSangoDgRoomInfo->stamina = roomsDic[strNo]["stamina"].asInt();
            pSangoDgRoomInfo->propose_lv = roomsDic[strNo]["propose_lv"].asInt();
            if(!effect_stamina.empty())
                pSangoDgRoomInfo->stamina = ceilf(pSangoDgRoomInfo->stamina * fStaminaFactor);
            
            pSangoDgRoomInfo->steps = roomsDic[strNo]["steps"].asInt();
            pSangoDgRoomInfo->video = roomsDic[strNo]["video"].asInt();
            
            pSangoDgRoomInfo->cStatus = 2;
            
            
            if (roomsDic[strNo].isMember("head_tips")) {
                if (pSangoDgRoomInfo->m_pHeadTipsArr == NULL) {
                    pSangoDgRoomInfo->m_pHeadTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["head_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pSangoDgRoomInfo->m_pHeadTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }
            
            if (roomsDic[strNo].isMember("end_tips")) {
                if (pSangoDgRoomInfo->m_pEndTipsArr == NULL) {
                    pSangoDgRoomInfo->m_pEndTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["end_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pSangoDgRoomInfo->m_pEndTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }
            
            pSangoDgInfo->m_pRoomArr->addObject(pSangoDgRoomInfo);
            pSangoDgRoomInfo->release();
            
            if((floorid == m_iCurSangoFloor) && (roomid == m_iCurSangoRoom)){
                pSangoDgRoomInfo->cStatus = status;
                break;
            }
        }
        
        m_pSangoYYArr->addObject(pSangoDgInfo);
        pSangoDgInfo->release();
        
        if(floorid == m_iCurSangoFloor){
            pSangoDgInfo->cStatus = status;
            break;
        }
    }
    
    return m_pSangoYYArr;
    
}


//******************************************************************************
// getIsNewSpecialRoomExist
//******************************************************************************
bool CGameData::getIsNewSpecialRoomExist()
{
    CCMutableArray<CSpecialDgInfo *> *array = getSpecialDgInfo();
    long curDate = getCurrentTime();
    for(int i =0;i<array->count();i++)
    {
        CSpecialDgInfo* pSpecialDgInfo = array->getObjectAtIndex(i);
        if (pSpecialDgInfo->endtime < curDate
            || pSpecialDgInfo->starttime > curDate)
            continue;
            
        if(pSpecialDgInfo->cStatus == 0)
        {
            return true;
        }
    }
    return false;
}

//******************************************************************************
// getSpecialDgInfo
//******************************************************************************
CCMutableArray<CSpecialDgInfo*> * CGameData::getSpecialDgInfo()
{
    static long slCfgObtainTm = 0;
    static long slSpecialDgEndObtainTm = 0;
    static long slWeeklyDgEndObtainTm = 0;
    static long slHourReqTm = 0;
    bool bUpdate = false;
    
    if (slCfgObtainTm < HttpComm::Inst()->getDicTime(enDicCfg)) {
        bUpdate = true;
        slCfgObtainTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    
    if (slSpecialDgEndObtainTm < HttpComm::Inst()->getDicTime(enDicSpecialDgEnd)) {
        bUpdate = true;
        slSpecialDgEndObtainTm = HttpComm::Inst()->getDicTime(enDicSpecialDgEnd);
    }
    
    if (slWeeklyDgEndObtainTm < HttpComm::Inst()->getDicTime(enDicWeeklyDgEnd)) {
        bUpdate = true;
        slWeeklyDgEndObtainTm = HttpComm::Inst()->getDicTime(enDicWeeklyDgEnd);
    }
    
    if (slHourReqTm < m_hourReqTime)
    {
        bUpdate = true;
        slHourReqTm = m_hourReqTime;
    }
    
    if(bUpdate == false)
        return m_pSpecialDgArr;
    
    CSpecialDgInfo* pSpecialDgInfo;
    CDgRoomInfo* pDgRoomInfo;
    
    m_pSpecialDgArr->removeAllObjects();

    if(getSmallDicTime(enSmallDicAllDungeons) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicAllDungeons] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_allBaseDungeonsDic = HttpComm::Inst()->getCfgDic()["dungeon"];
    }
    
    Json::Value effectDic = m_allBaseDungeonsDic["effect"];
    Json::Value specialDgDic = m_allBaseDungeonsDic["special"];
    Json::Value weeklyDgDic = m_allBaseDungeonsDic["weekly"];
    
    if(getSmallDicTime(enSmallDicUserDungeons) < HttpComm::Inst()->getDicTime(enDicLogin)) {
        m_allSmalDicTime[enSmallDicUserDungeons] = HttpComm::Inst()->getDicTime(enDicLogin);
        m_allUserDugeonsDic = HttpComm::Inst()->getLoginDic()["user_dungeon_info"];
    }
            
//special
    Json::Value specialEndDic = HttpComm::Inst()->getSpecialEndDgDic();
    
    int roomid;
    Json::Value::Members specialArr = specialDgDic.getMemberNames();
    for (int i =0; i< specialArr.size(); i++) {
        pSpecialDgInfo = new CSpecialDgInfo();
        
        string floorid = specialArr[i];
        pSpecialDgInfo->iFloorId = atoi(floorid.c_str());
        
        //战场是否已关(限制次数的战场)
        if (HttpComm::Inst()->isSpcFlrClosed(pSpecialDgInfo->iFloorId)) {
            pSpecialDgInfo->release();
            continue;
        }
        
        pSpecialDgInfo->cType = 1;
        
        if((!specialEndDic.empty())
           && (pSpecialDgInfo->iFloorId == atoi(specialEndDic["floor_id"].asString().c_str())))
        {
            pSpecialDgInfo->cStatus = specialEndDic["floor_status"].asInt();
            m_newSpecialDgStatusDic[floorid]["floor_status"] = specialEndDic["floor_status"];
        }
        else
        {
            if (m_newSpecialDgStatusDic.isMember(floorid)) {
                pSpecialDgInfo->cStatus =  m_newSpecialDgStatusDic[floorid]["floor_status"].asInt();
            }
            else {
                pSpecialDgInfo->cStatus = m_allUserDugeonsDic["special"][floorid]["status"].asInt();
            }
        }
                        
        Json::Value oneSpecial = specialDgDic[floorid];
        
        string strBgColor = "0";
        if (oneSpecial.isMember("bgcolor"))
            strBgColor = oneSpecial["bgcolor"].asString();
        
        if (strBgColor == "1")
            pSpecialDgInfo->bg_type = 1;
        else if (strBgColor == "2")
            pSpecialDgInfo->bg_type = 2;
        else if (strBgColor == "3")
            pSpecialDgInfo->bg_type = 3;
        else if (strBgColor == "4")
            pSpecialDgInfo->bg_type = 4;
        else
            pSpecialDgInfo->bg_type = 0;
        
        strncpy(pSpecialDgInfo->name, oneSpecial["name"].asString().c_str(), 99);
        pSpecialDgInfo->dungeon_type = atoi(oneSpecial["dungeon_type"].asString().c_str());
        pSpecialDgInfo->dungeon_dsp = oneSpecial.isMember("dungeon_dsp") ? oneSpecial["dungeon_dsp"].asString() : "";
        pSpecialDgInfo->m_unLimitMaxStar = oneSpecial.isMember("max_star")? oneSpecial["max_star"].asInt() : 0;
        
        pSpecialDgInfo->buff_void = oneSpecial.isMember("buff_void") ? oneSpecial["buff_void"].asBool() : true;
        
        string strData = oneSpecial["start_time"].asString();
        pSpecialDgInfo->starttime = getEchoTime(strData);
        
        strData = oneSpecial["end_time"].asString();
        pSpecialDgInfo->endtime = getEchoTime(strData);
        
        pSpecialDgInfo->is_auto = oneSpecial.isMember("is_auto") ? oneSpecial["is_auto"].asBool() : false;
        
        string strNumber = oneSpecial["effect"]["number"].asString();
        strncpy(pSpecialDgInfo->effectname, effectDic[strNumber]["name"].asString().c_str(), 99);
        
        pSpecialDgInfo->bottom_dungeon = oneSpecial.isMember("bottom_dungeon") ? oneSpecial["bottom_dungeon"].asBool() : false;
        
        pSpecialDgInfo->minlevel = 0;
        pSpecialDgInfo->maxlevel = 9999;
        
        if (oneSpecial.isMember("lv_range"))
        {
            int p1 = 0, p2 = 1;
            pSpecialDgInfo->minlevel = oneSpecial["lv_range"][p1].asInt();
            pSpecialDgInfo->maxlevel = oneSpecial["lv_range"][p2].asInt();
        }
        
        //diamond type
        Json::Value diamondArr = oneSpecial["diamond_type"];
        if(!diamondArr.empty()){
            pSpecialDgInfo->availDiamondCnt = diamondArr.size();
            for(int m = 0; m < diamondArr.size(); m++){
                pSpecialDgInfo->availDiamond[m] = atoi(diamondArr[m].asString().c_str());
            }
        }
        else{
            pSpecialDgInfo->availDiamondCnt = enElementMax;
        }
        
        float fStaminaFactor  = 1.0;
        Json::Value effect_stamina;;
        if(pSpecialDgInfo->remaintime > 0){
            effect_stamina = effectDic[strNumber]["type"]["stamina"];
            if(!effect_stamina.empty())
                fStaminaFactor = (float)effect_stamina.asDouble();
        }
        
        //限制必须带某武将进入
        chkSpecDgNeedCards(pSpecialDgInfo, oneSpecial);
        
        if (oneSpecial.isMember("all_deck") && oneSpecial["all_deck"].asBool() == true)
        {   // 全队伍限定战场
            pSpecialDgInfo->skillType = enSpecialDgSkillType_quanduiwuxianding;
        }
        else
        {
            for (int i = 0; i < 6; i++) {
                if (!pSpecialDgInfo->strNeedCards[i].empty())
                {   // 千里走单骑
                    pSpecialDgInfo->skillType = enSpecialDgSkillType_qianlizoudanqi;
                }
            }
        }
        
        //统御力限制
        if (oneSpecial.isMember("cards_cost")) {
            pSpecialDgInfo->dgRstrct.cards_cost = oneSpecial["cards_cost"].asInt();
        }
        else{
            pSpecialDgInfo->dgRstrct.cards_cost = 0;
        }
        
        if (oneSpecial.isMember("max_cost")) {
            pSpecialDgInfo->dgRstrct.max_cost = oneSpecial["max_cost"].asInt();
        }
        else{
            pSpecialDgInfo->dgRstrct.max_cost = 0;
        }
        
        //武将属性限制
        if (oneSpecial.isMember("cards_ctype")) {
            Json::Value ctypeArr = oneSpecial["cards_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSpecialDgInfo->dgRstrct.cards_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSpecialDgInfo->dgRstrct.cards_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSpecialDgInfo->dgRstrct.cards_ctype, 0xFF, sizeof(pSpecialDgInfo->dgRstrct.cards_ctype));
        }
        
        if (oneSpecial.isMember("only_ctype")) {
            Json::Value ctypeArr = oneSpecial["only_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSpecialDgInfo->dgRstrct.only_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSpecialDgInfo->dgRstrct.only_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSpecialDgInfo->dgRstrct.only_ctype, 0xFF, sizeof(pSpecialDgInfo->dgRstrct.only_ctype));
        }
        
        //rooms
        Json::Value roomsDic = oneSpecial["rooms"];
        Json::Value::Members roomsArr = roomsDic.getMemberNames();
        for (int j =0; j< roomsArr.size(); j++)
        {
            pDgRoomInfo = new CDgRoomInfo();
            roomid = j+1;
            char tmp[32] = {'\0'};
            sprintf(tmp,"%d",roomid);
            string strNo(tmp);

            strncpy(pDgRoomInfo->name,roomsDic[strNo]["name"].asString().c_str(),99);
            pDgRoomInfo->stamina = roomsDic[strNo]["stamina"].asInt();
            pDgRoomInfo->propose_lv = roomsDic[strNo]["propose_lv"].asInt();
            if(!effect_stamina.empty())
                pDgRoomInfo->stamina = ceilf(pDgRoomInfo->stamina * fStaminaFactor);
            
            pDgRoomInfo->steps = roomsDic[strNo]["steps"].asInt();
            pDgRoomInfo->video = roomsDic[strNo]["video"].asInt();
            
            if((!specialEndDic.empty())
               && (pSpecialDgInfo->iFloorId == specialEndDic["floor_id"].asInt())
               && (roomid == specialEndDic["room_id"].asInt())){
                pDgRoomInfo->cStatus = specialEndDic["room_status"].asInt();

                 m_newSpecialDgStatusDic[floorid][strNo] = specialEndDic["room_status"];
            }
            else{
                if (m_newSpecialDgStatusDic.isMember(floorid)
                    && m_newSpecialDgStatusDic[floorid].isMember(strNo)) {
                    pDgRoomInfo->cStatus =  m_newSpecialDgStatusDic[floorid][strNo].asInt();
                }
                else {
                    pDgRoomInfo->cStatus = m_allUserDugeonsDic["special"][floorid]["rooms"][strNo].asInt();
                }
            }
            
            if (roomsDic[strNo].isMember("head_tips")) {
                if (pDgRoomInfo->m_pHeadTipsArr == NULL) {
                    pDgRoomInfo->m_pHeadTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["head_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pDgRoomInfo->m_pHeadTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }
            
            if (roomsDic[strNo].isMember("end_tips")) {
                if (pDgRoomInfo->m_pEndTipsArr == NULL) {
                    pDgRoomInfo->m_pEndTipsArr = new CCMutableArray<CCString*>();
                }
                
                Json::Value tips = roomsDic[strNo]["end_tips"];
                for (int i = 0; i < tips.size(); i++) {
                    CCString* pStrTip = new CCString(tips[i].asString().c_str());
                    pDgRoomInfo->m_pEndTipsArr->addObject(pStrTip);
                    pStrTip->release();
                }
            }

            pSpecialDgInfo->m_pRoomArr->addObject(pDgRoomInfo);
            pDgRoomInfo->release();
        }
        
        m_pSpecialDgArr->addObject(pSpecialDgInfo);
        pSpecialDgInfo->release();
    }

//weekly
    Json::Value weeklyEndDic = HttpComm::Inst()->getWeeklyEndDgDic();
    Json::Value::Members weekArr = weeklyDgDic.getMemberNames();
    for (int i =0; i< weeklyDgDic.size(); i++) {
        pSpecialDgInfo = new CSpecialDgInfo();
        
        string floorid(weekArr[i]);
        pSpecialDgInfo->iFloorId = atoi(floorid.c_str());
        pSpecialDgInfo->cType = 2;
        
        if((!weeklyEndDic.empty())
           && (pSpecialDgInfo->iFloorId == atoi(weeklyEndDic["floor_id"].asString().c_str()))){
            pSpecialDgInfo->cStatus = weeklyEndDic["floor_status"].asInt();
           
            m_newWeeklyDgStatusDic[floorid]["floor_status"] = weeklyEndDic["floor_status"];
        }
        else{
            if (m_newWeeklyDgStatusDic.isMember(floorid)) {
                pSpecialDgInfo->cStatus = m_newWeeklyDgStatusDic[floorid]["floor_status"].asInt();
            }
            else {
                pSpecialDgInfo->cStatus = m_allUserDugeonsDic["weekly"][floorid]["status"].asInt();
            }
        }

        Json::Value oneWeek = weeklyDgDic[floorid];
        
        pSpecialDgInfo->bg_type = 0;

        pSpecialDgInfo->bottom_dungeon = oneWeek.isMember("bottom_dungeon") ? oneWeek["bottom_dungeon"].asBool() : false;
        
        pSpecialDgInfo->minlevel = 0;
        pSpecialDgInfo->maxlevel = 9999;
        
        if (oneWeek.isMember("lv_range"))
        {
            int p1 = 0, p2 = 1;
            pSpecialDgInfo->minlevel = oneWeek["lv_range"][p1].asInt();
            pSpecialDgInfo->maxlevel = oneWeek["lv_range"][p2].asInt();
        }
        
        strncpy(pSpecialDgInfo->name,oneWeek["name"].asString().c_str(),127);
        pSpecialDgInfo->dungeon_type =atoi(oneWeek["dungeon_type"].asString().c_str());
        pSpecialDgInfo->dungeon_dsp = oneWeek.isMember("dungeon_dsp") ? oneWeek["dungeon_dsp"].asString() : "";
        pSpecialDgInfo->buff_void = oneWeek.isMember("buff_void") ? oneWeek["buff_void"].asBool() : true;
        string strDate = oneWeek["start_time"].asString();
        pSpecialDgInfo->starttime = getEchoTime(strDate);
        pSpecialDgInfo->m_unLimitMaxStar = oneWeek.isMember("max_star")? oneWeek["max_star"].asInt() : 0;
        
        strDate = oneWeek["end_time"].asString();
        pSpecialDgInfo->endtime = getEchoTime(strDate);
        
        Json::Value effect = oneWeek["effect"];
        float fStaminaFactor;
        Json::Value effect_stamina;
        if(!effect.empty())
        {
        	std::string strNumber = oneWeek["effect"]["number"].asString();
        	strcpy(pSpecialDgInfo->effectname,effectDic[strNumber]["name"].asString().c_str());
            CCLog("pSpecialDgInfo->effectname is %s,effectDic[strNumber][name].asString().c_str() is %s",pSpecialDgInfo->effectname,effectDic[strNumber]["name"].asString().c_str());
        	effect_stamina = effectDic[strNumber]["type"]["stamina"];
        	if(!effect_stamina.empty())
        		fStaminaFactor = (float)effect_stamina.asDouble();
        }
        else
        {
            std::string strNumber = "";
        	strcpy(pSpecialDgInfo->effectname,"");
        	fStaminaFactor = 0.0;
        }

        //diamond type
        Json::Value diamondArr = oneWeek["diamond_type"];
        if(!diamondArr.empty()){
            pSpecialDgInfo->availDiamondCnt = diamondArr.size();
            for(int m = 0; m < diamondArr.size(); m++){
                pSpecialDgInfo->availDiamond[m] = atoi(diamondArr[m].asString().c_str());
            }
        }
        else{
            pSpecialDgInfo->availDiamondCnt = enElementMax;
        }
        
        //统御力限制
        if (oneWeek.isMember("cards_cost")) {
            pSpecialDgInfo->dgRstrct.cards_cost = oneWeek["cards_cost"].asInt();
        }
        else{
            pSpecialDgInfo->dgRstrct.cards_cost = 0;
        }
        
        if (oneWeek.isMember("max_cost")) {
            pSpecialDgInfo->dgRstrct.max_cost = oneWeek["max_cost"].asInt();
        }
        else{
            pSpecialDgInfo->dgRstrct.max_cost = 0;
        }
        
        //武将属性限制
        if (oneWeek.isMember("cards_ctype")) {
            Json::Value ctypeArr = oneWeek["cards_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSpecialDgInfo->dgRstrct.cards_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSpecialDgInfo->dgRstrct.cards_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSpecialDgInfo->dgRstrct.cards_ctype, 0xFF, sizeof(pSpecialDgInfo->dgRstrct.cards_ctype));
        }
        
        if (oneWeek.isMember("only_ctype")) {
            Json::Value ctypeArr = oneWeek["only_ctype"];
            
            for (int idx = 0; idx < kMaxMemCntOfTeam; idx++) {
                if (idx < ctypeArr.size()) {
                    pSpecialDgInfo->dgRstrct.only_ctype[idx] = ctypeArr[idx].asInt();
                }
                else{
                    pSpecialDgInfo->dgRstrct.only_ctype[idx] = kInvalidValue;
                }
            }
        }
        else{
            memset(pSpecialDgInfo->dgRstrct.only_ctype, 0xFF, sizeof(pSpecialDgInfo->dgRstrct.only_ctype));
        }
        
        //rooms
        Json::Value roomsDic = oneWeek["rooms"];
        Json::Value::Members roomsArr = roomsDic.getMemberNames();        
        for (int j =0; j< roomsArr.size(); j++) {
            pDgRoomInfo = new CDgRoomInfo();
            roomid = j+1;

            char tmp[32] = {'\0'};
            sprintf(tmp,"%d",roomid);
            std::string strNo(tmp);

            strncpy(pDgRoomInfo->name,roomsDic[strNo]["name"].asString().c_str(),99);
            pDgRoomInfo->stamina = roomsDic[strNo]["stamina"].asInt();
            pDgRoomInfo->propose_lv = roomsDic[strNo]["propose_lv"].asInt();
            if(!effect_stamina.empty())
                pDgRoomInfo->stamina = ceilf(pDgRoomInfo->stamina * fStaminaFactor);
        
            pDgRoomInfo->steps = roomsDic[strNo]["steps"].asInt();
            pDgRoomInfo->video = roomsDic[strNo].isMember("video") ? roomsDic[strNo]["video"].asInt() : 0;

            if((!weeklyEndDic.empty())
               && (pSpecialDgInfo->iFloorId == atoi(weeklyEndDic["floor_id"].asString().c_str()))
               && (roomid == atoi(weeklyEndDic["room_id"].asString().c_str()))){
                pDgRoomInfo->cStatus = weeklyEndDic["room_status"].asInt();
                
                m_newWeeklyDgStatusDic[floorid][strNo] = weeklyEndDic["room_status"];
            }
            else{
                if (m_newWeeklyDgStatusDic.isMember(floorid)
                    &&m_newWeeklyDgStatusDic[floorid].isMember(strNo)) {
                    pDgRoomInfo->cStatus = m_newWeeklyDgStatusDic[floorid][strNo].asInt();
                }
                else {
                    pDgRoomInfo->cStatus = m_allUserDugeonsDic["weekly"][floorid]["rooms"][strNo].asInt();
                }
            }
            
            pSpecialDgInfo->m_pRoomArr->addObject(pDgRoomInfo);
            pDgRoomInfo->release();
        }
        
        m_pSpecialDgArr->addObject(pSpecialDgInfo);
        pSpecialDgInfo->release();
    }    
    
    
    CCMutableArray<CSpecialDgInfo*> *pSpDgInfoArr = NULL;
    
    for (int i=0; i<m_pSpecialDgArr->count(); i++)
    {
        if (m_pSpecialDgArr->getObjectAtIndex(i)->bottom_dungeon)
        {
            if (pSpDgInfoArr == NULL)
                 pSpDgInfoArr = new CCMutableArray<CSpecialDgInfo*>();
            
            CSpecialDgInfo* pSpDgInfo = m_pSpecialDgArr->getObjectAtIndex(i);
            pSpDgInfoArr->addObject(pSpDgInfo);
            m_pSpecialDgArr->removeObjectAtIndex(i);

        }
    }
    
    if (pSpDgInfoArr != NULL)
    {
        for (int i=0; i<pSpDgInfoArr->count(); i++)
        {
            m_pSpecialDgArr->addObject(pSpDgInfoArr->getObjectAtIndex(i));
        }
        
        pSpDgInfoArr->removeAllObjects();
        pSpDgInfoArr = NULL;
    }
    
    
    return m_pSpecialDgArr;
}




void CGameData::chkSpecDgNeedCards(CSpecialDgInfo* pSpecialDgInfo, Json::Value oneSpecial)
{
    //cards_coin
    pSpecialDgInfo->cards_coin = oneSpecial["cards_coin"].asInt();
    pSpecialDgInfo->cards_gold1 = oneSpecial["cards_gold1"].asInt();
    pSpecialDgInfo->cards_gold2 = oneSpecial["cards_gold2"].asInt();
    pSpecialDgInfo->cards_gold3 = oneSpecial["cards_gold3"].asInt();
    
    //leader_cards
    if (oneSpecial.isMember("leader_cards")) {
        Json::Value leader = oneSpecial["leader_cards"];
        
        if (leader.size() > 0) {
            int zero = 0;
            for (int i = 0 ; i < leader[zero].size(); i++) {
                int icid = atoi(leader[zero][i].asString().c_str());
                
                char buf[10];
                if (i == 0)
                    snprintf(buf, 9, "%d", icid);
                else
                    snprintf(buf, 9, ",%d", icid);
                pSpecialDgInfo->strNeedCards[0].append(buf);
            }
        }
        else{
            pSpecialDgInfo->strNeedCards[0] = "";
        }
        
        if (leader.size() > 1) {
            for (int i = 0 ; i < leader[1].size(); i++) {
                int icid = atoi(leader[1][i].asString().c_str());
                
                char buf[10];
                if (i == 0)
                    snprintf(buf, 9, "%d", icid);
                else
                    snprintf(buf, 9, ",%d", icid);
                pSpecialDgInfo->strNeedCards[5].append(buf);
            }
        }
        else{
            pSpecialDgInfo->strNeedCards[5] = "";
        }
    }
    else{
        pSpecialDgInfo->strNeedCards[0] = "";
        pSpecialDgInfo->strNeedCards[5] = "";
    }
    
    //deck_cards
    pSpecialDgInfo->strNeedCards[1] = "";
    pSpecialDgInfo->strNeedCards[2] = "";
    pSpecialDgInfo->strNeedCards[3] = "";
    pSpecialDgInfo->strNeedCards[4] = "";
    
    if (oneSpecial.isMember("deck_cards")) {
        Json::Value deck = oneSpecial["deck_cards"];
        
        int deckCnt = MIN(4, deck.size());
        for (int i = 0; i < deckCnt; i++) {
            for (int j = 0; j < deck[i].size(); j++) {
                int icid = atoi(deck[i][j].asString().c_str());
                
                char buf[10];
                if (j == 0)
                    snprintf(buf, 9, "%d", icid);
                else
                    snprintf(buf, 9, ",%d", icid);
                pSpecialDgInfo->strNeedCards[i+1].append(buf);
            }
        }
    }
    
    //help_cards
    for (int i = 0; i < 6; i++)
        pSpecialDgInfo->strHelpCards[i] = "";

    if (oneSpecial.isMember("help_cards")) {
        Json::Value help = oneSpecial["help_cards"];
        
        int zero = 0;
        int helpCnt = MIN(6, help.size());
        for (int i = 0; i < helpCnt; i++) {
            int icid = atoi(help[i][zero].asString().c_str());
            string strlv = help[i][1].asString();
            
            char buf[10];
            snprintf(buf, 9, "%d,", icid);
            pSpecialDgInfo->strHelpCards[i].append(buf).append(strlv);
        }
    }
}


//******************************************************************************
// getDgStepInfo
//******************************************************************************
CCMutableArray<CDgStepInfo*> * CGameData::getDgStepInfo(bool bUpdate)
{
    if((bUpdate == false) && (m_pDgStepArr->count() > 0))
        return m_pDgStepArr;
    
    CDgStepInfo* pDgStepInfo;
    CDgMonsterInfo* pMonsterInfo;
    
    m_pDgStepArr->removeAllObjects();

    Json::Value stepArr = HttpComm::Inst()->getEnterDgDic()["steps_info"];
    
    if(getSmallDicTime(enSmallDicAllMonsters) < HttpComm::Inst()->getDicTime(enDicMonster)) {
        m_allSmalDicTime[enSmallDicAllMonsters] = HttpComm::Inst()->getDicTime(enDicMonster);
        m_allMonstersDic = HttpComm::Inst()->getMonsterDic();
    }
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }

    for(int i = 0; i < stepArr.size(); i++){
        Json::Value oneStep = stepArr[i];
        
        pDgStepInfo = new CDgStepInfo();
        
        Json::Value goldbox = oneStep["gold_box"];
        if(!goldbox.empty())
        {
        	pDgStepInfo->gold_box = oneStep["gold_box"].asInt();
        }
        else
        {
        	pDgStepInfo->gold_box = 0;
        }
        
        Json::Value monsterArr = oneStep["monsters"];
        
        for(int j = 0; j < monsterArr.size(); j++){
            Json::Value oneMonster = monsterArr[j];
            
            pMonsterInfo = new CDgMonsterInfo();
            
            if (oneMonster.isMember("drop_cid")) {
                pMonsterInfo->drop_cid = atoi(oneMonster["drop_cid"].asString().c_str());
            }
            else {
                pMonsterInfo->drop_cid = 0;
            }

            if (oneMonster.isMember("drop_lv")) {
                pMonsterInfo->drop_lv = oneMonster["drop_lv"].asInt();
            }
            else {
                pMonsterInfo->drop_lv = 0;
            }
            
            pMonsterInfo->drop_plus = false;
            if (oneMonster.isMember("drop_plus_type")) {
                if (oneMonster["drop_plus_type"] != "") {
                    pMonsterInfo->drop_plus = true;
                }
            }
                        
            //string mid = "608_monster";//oneMonster["mid"].asString();//"1_monster";1291_monster "188_monster";//
            string mid = oneMonster["mid"].asString();//"1_monster";1291_monster "188_monster";//
            
            if (m_allMonstersDic.isMember(mid) == false) {
                continue;
            }
            
            Json::Value monsterBase = m_allMonstersDic[mid];
            
            pMonsterInfo->attack = monsterBase["attack"].asInt();
            pMonsterInfo->turn = monsterBase["attack_turn"].asInt();
            pMonsterInfo->defense = monsterBase["defense"].asInt();
            pMonsterInfo->cid = atoi(monsterBase["cid"].asString().c_str());
            pMonsterInfo->drop_exp = monsterBase["drop_exp"].asInt();
            pMonsterInfo->drop_gold = monsterBase["drop_gold"].asInt();
            pMonsterInfo->hp = monsterBase["hp"].asInt();
            pMonsterInfo->ctype = atoi(m_allBaseCardsDic[monsterBase["cid"].asString()]["ctype"].asString().c_str());
            pMonsterInfo->mtype = monsterBase.isMember("mtype") ? monsterBase["mtype"].asInt() : enElementInvalid;
            
            pMonsterInfo->init_attack_turn = oneMonster["init_attack_turn"].asUInt();
            
            for (int i=0; i<ENEMYSKILLMAX; i++)
            {
                pMonsterInfo->skidArr[i] = 0;
            }
            
            Json::Value array;
            array = monsterBase["skidArr"];
            for (int i=0; i<array.size(); i++)
            {
                if (i >= ENEMYSKILLMAX)
                {
                    break;
                }
                pMonsterInfo->skidArr[i] = (short)(array[i].asInt());
            }
          
            for (int i=0; i<ENEMYBUFFMAX; i++)
            {
                pMonsterInfo->buffArr[i] = 0;
            }
            
            Json::Value array2;
            array2 = monsterBase["buffArr"];
            for (int i=0; i<array2.size(); i++)
            {
                if (i >= ENEMYBUFFMAX)
                {
                    break;
                }
                pMonsterInfo->buffArr[i] = (short)(array2[i].asInt());
            }
           
            pDgStepInfo->m_pMonsterArr->addObject(pMonsterInfo);
            pMonsterInfo->release();
        }
        m_pDgStepArr->addObject(pDgStepInfo);
        pDgStepInfo->release();
    }

    return m_pDgStepArr;
}

//******************************************************************************
// getDungeonUid
//******************************************************************************
const char* CGameData::getDungeonUid()
{
    return HttpComm::Inst()->getEnterDgDic()["dungeon_uid"].asString().c_str();
}

//******************************************************************************
// getStaminaRecoverTime
//******************************************************************************
unsigned short CGameData::getStaminaRecoverTime()
{
    return getCommonInfo()->stamina_recover_time;
}

//******************************************************************************
// getActionPointRecoverTime
//******************************************************************************
unsigned short CGameData::getActionPointRecoverTime()
{
    return getCommonInfo()->action_recover_time;
}

//******************************************************************************
// getIsUseJiangNanStyle
//******************************************************************************
bool CGameData::getIsUseJiangNanStyle()
{
    return getCommonInfo()->bUseJiangNanStyle;
}

//******************************************************************************
// setUpdateCom
//******************************************************************************
void CGameData::setUpdateCom(bool bUpdate)
{
    m_bUpdateCom = bUpdate;
}

bool CGameData::isShopHuoDong()
{
    //return false;
    return getCommonInfo()->bShow_charge_award;
}
//******************************************************************************
// getNiePanResultUcid
//******************************************************************************
const char* CGameData::getNiePanResultUcid()
{
    return HttpComm::Inst()->getNiePanResultUcid().c_str();
}
const char* CGameData::getTradeNo()
{
    return HttpComm::Inst()->getTradeNo().c_str();
}

//******************************************************************************
// getCommonInfo
//******************************************************************************
CommonInfo*  CGameData::getCommonInfo()
{
    if(!m_bUpdateCom)
        return &m_commonInfo;
    
    m_bUpdateCom = false;
    
    Json::Value commonInfoDic = HttpComm::Inst()->getCfgDic()["common"];
    
    

//    CCLog("ttt = %s", HttpComm::Inst()->getCfgDic().toStyledString().c_str());
    
    
    m_commonInfo.bShow_charge_award = commonInfoDic["show_charge_award"].asBool();  //是否开启周年商店活动
    m_commonInfo.bOpen_suitang_gacha = commonInfoDic["is_open_st_gacha"].asBool();  // 是否开启隋唐求将
    
    
    
    m_commonInfo.bOpen_hero_1_gacha = commonInfoDic["open_hero_1_gacha"].asBool();  // 是否开启其他求将
    
    m_commonInfo.bOpen_hero_2_gacha = commonInfoDic["open_hero_2_gacha"].asBool();  // 是否开启其他求将
    
    m_commonInfo.bOpen_hero_3_gacha = commonInfoDic["open_hero_3_gacha"].asBool();  // 是否开启其他求将

    
    m_commonInfo.modify_name_gold = commonInfoDic["modify_name_gold"].asInt();//修改同盟名片需要的铜钱
    m_commonInfo.card_extend_coin = commonInfoDic["card_extend_coin"].asInt();//扩充武将栏每次花费的元宝数
    m_commonInfo.card_extend_num = commonInfoDic["card_extend_num"].asInt();//每次扩充武将栏提升的武将栏的数目
    m_commonInfo.dungeon_clear_coin= commonInfoDic["dungeon_clear_coin"].asInt();//初次通关能得到的元宝数
    m_commonInfo.gacha_cost_coin = commonInfoDic["gacha_cost_coin"].asInt();//稀有抽奖花费的元宝点数
    m_commonInfo.gacha_cost_pt = commonInfoDic["gacha_cost_pt"].asInt(); //援军抽奖花费的援军点数
    m_commonInfo.max_gacha_point = commonInfoDic["max_gacha_point"].asInt();//援军点最大数目
    m_commonInfo.recover_stamina_coin = commonInfoDic["recover_stamina_coin"].asInt();//回复体力话费的元宝点数
    
    
    m_commonInfo.permit_guild_lv = commonInfoDic["permit_guild_lv"].asInt();
    m_commonInfo.permit_special_lv = commonInfoDic["permit_special_lv"].asInt();
    m_commonInfo.permit_sgyy_lv = commonInfoDic["permit_sgyy_lv"].asInt();
    
    
    m_commonInfo.revive_coin = commonInfoDic["revive_coin"].asInt();
    m_commonInfo.vec_revive_minus_coin.clear();
    Json::Value item_array = commonInfoDic["revive_minus_coin"];
    Json::Value::Members item_members = item_array.getMemberNames();
    for (int i=0; i<item_members.size(); i++)
    {
        str_ReviveMinusCoin strReviveMinusCoin;
        memcpy(strReviveMinusCoin.item_id,item_members[i].c_str(),sizeof(strReviveMinusCoin.item_id));
        memcpy(strReviveMinusCoin.item_dsp,item_array[item_members[i].c_str()]["dsp"].asCString(),sizeof(strReviveMinusCoin.item_dsp));
        strReviveMinusCoin.item_coin = item_array[item_members[i].c_str()]["coin"].asInt();
        m_commonInfo.vec_revive_minus_coin.push_back(strReviveMinusCoin);
    }
  
    m_commonInfo.stamina_recover_time = commonInfoDic["stamina_recover_time"].asInt();
    m_commonInfo.max_friend_extend_num = commonInfoDic["max_friend_extend_num"].asInt();
    m_commonInfo.max_video_num = commonInfoDic["video_num"].asInt();
    m_commonInfo.friend_extend_num = commonInfoDic["friend_extend_num"].asInt();
    m_commonInfo.friend_extend_coin = commonInfoDic["friend_extend_coin"].asInt();
    
    m_commonInfo.qq_voice_fg = commonInfoDic["qq_voice_fg"].asBool();
    m_commonInfo.sina_voice_fg = commonInfoDic["sina_voice_fg"].asBool();
    m_commonInfo.weixin_voice_fg = commonInfoDic["weixin_voice_fg"].asBool();
    m_commonInfo.tapjoy_fg = commonInfoDic["tapjoy_fg"].asBool();
    m_commonInfo.compgacha_fg = commonInfoDic["compgacha_fg"].asBool();
    m_commonInfo.mail_all_coin = commonInfoDic["guild_mail_coin"].asInt();
    m_commonInfo.delete_guild_cache_time = commonInfoDic["delete_guild_cache_time"].asInt();
    
    m_commonInfo.m_bIsShowVideoStrategy = commonInfoDic["is_show_video_strategy"].asBool();
    
#if defined(kPltfmPp)
    m_commonInfo.qq_voice_fg = false; //将qq sina weixin分享关闭
    m_commonInfo.sina_voice_fg = false;
    m_commonInfo.weixin_voice_fg = false;
    m_commonInfo.tapjoy_fg = false;
#endif
    
    if (commonInfoDic.isMember("month_plan_charge")) {
      m_commonInfo.m_splan_coin =  commonInfoDic["plan_coin"].asInt();
      m_commonInfo.m_splan_days =  commonInfoDic["plan_days"].asInt();
    }else{
        
    }
    
    
    if (commonInfoDic.isMember("open_lv_up_double"))
    {
        m_commonInfo.skill_lv_up_starttime = getEchoTime(commonInfoDic["open_lv_up_double"]["start_time"].asString());
        m_commonInfo.skill_lv_up_endtime = getEchoTime(commonInfoDic["open_lv_up_double"]["end_time"].asString());
    }
    else
    {
        m_commonInfo.skill_lv_up_starttime = -1;
        m_commonInfo.skill_lv_up_endtime = -1;
    }
    
    //多练抽次数，至少为1
    m_commonInfo.multi_gacha_cnt = commonInfoDic["multi_gacha_cnt"].asInt();
    if(m_commonInfo.multi_gacha_cnt == 0)
        m_commonInfo.multi_gacha_cnt = 1;
    
    if (commonInfoDic.isMember("month_item_cost"))
    {
        m_commonInfo.month_item_cost = commonInfoDic["month_item_cost"].asInt();
    }
    if (commonInfoDic.isMember("month_cost_coin"))
    {
        m_commonInfo.month_cost_coin = commonInfoDic["month_cost_coin"].asInt();
    }
    
    
    
    // cntry cofig
    if (getSmallDicTime(enSmallDicCntryConfig) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicCntryConfig] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_cntryConfigDic = HttpComm::Inst()->getCfgDic()["guild_config"];
    }
    m_commonInfo.recover_action_coin = m_cntryConfigDic["recover_power_coin"].asInt();
    m_commonInfo.action_recover_time = m_cntryConfigDic["recover_power_time"].asInt();
    if (m_cntryConfigDic.isMember("create_guild_coin"))
        m_commonInfo.cntry_create_coin = m_cntryConfigDic["create_guild_coin"].asInt();
    else
        m_commonInfo.cntry_create_coin = 200;
    
    m_commonInfo.task_coin_default = m_cntryConfigDic.isMember("task_coin") ? m_cntryConfigDic["task_coin"].asInt() : 10;
    m_commonInfo.task_deadline_default = m_cntryConfigDic.isMember("task_deadline") ? m_cntryConfigDic["task_deadline"].asInt() : 1;
    m_commonInfo.task_person_default = m_cntryConfigDic.isMember("task_person") ? m_cntryConfigDic["task_person"].asInt() : 1;
    
    m_commonInfo.list_taskCntrybution.clear();
    Json::Value array = m_cntryConfigDic["task_contribution"];
    for (int i = 0 ; i < array.size(); i++)
    {
        str_TaskCntryBtn strTaskCnt;
        Json::Value array1 = array[i];
        for (int j=0; j<array1.size(); j++)
        {
            switch (j) {
                case 0:
                    strTaskCnt.minLlv = array1[j].asInt();
                    break;
                    
                case 1:
                    strTaskCnt.maxLlv = array1[j].asInt();
                    break;
                    
                case 2:
                    strTaskCnt.contribution = array1[j].asInt();
                    break;
                    
                default:
                    break;
            }
        }
    
        m_commonInfo.list_taskCntrybution.push_back(strTaskCnt);
    }

    //圣诞场景配置
    m_commonInfo.bChrisBg = false;
    if (commonInfoDic.isMember("special_bg")) {
        if (strcmp(commonInfoDic["special_bg"].asString().c_str(), "shengdan") == 0) {
            m_commonInfo.bChrisBg = true;
        }
    }
    
    // 是否显示礼品兑换界面
    m_commonInfo.bShowCDKey = false;
    if (commonInfoDic.isMember("gifte_open"))
    {
        m_commonInfo.bShowCDKey = commonInfoDic["gifte_open"].asBool();
    }
    
    // 外挂列表
    m_commonInfo.list_external.clear();
    if (commonInfoDic.isMember("wg_process"))
    {
        Json::Value externalDic = commonInfoDic["wg_process"];
        if (externalDic.size() > 0)
        {
            for (int i = 0; i < externalDic.size(); i++)
                m_commonInfo.list_external.push_back(externalDic[i].asString());
        }
    }
    
    //微信分享类型
    m_commonInfo.weixin_share_type = "0";
    if (commonInfoDic.isMember("weixin_share_type"))
    {
        m_commonInfo.weixin_share_type = commonInfoDic["weixin_share_type"].asString();
    }

    //战斗场景配置
    if (commonInfoDic.isMember("bg_change") == false) {
        m_commonInfo.bg_change = 1;
    }
    else{
        const char* szBgChange = commonInfoDic["bg_change"].asCString();
        m_commonInfo.bg_change = atoi(szBgChange);
    }
    
    if (commonInfoDic.isMember("jiangnan") == false) {
        m_commonInfo.bUseJiangNanStyle = false;
    }
    else{
        m_commonInfo.bUseJiangNanStyle = commonInfoDic["jiangnan"].asBool();
    }
    
    snprintf(m_commonInfo.cntry_notice, 99, "%s", commonInfoDic["guild_notice"].asString().c_str());
    snprintf(m_commonInfo.niepan_notice, 99, "%s", commonInfoDic["card_np_notice"].asString().c_str());
    
    if (commonInfoDic.isMember("gacha_notice") == false) {
        m_commonInfo.gacha_notice[0] = 0;
    }
    else{
        strncpy(m_commonInfo.gacha_notice, commonInfoDic["gacha_notice"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("help") == false) {
        m_commonInfo.war_help_url[0] = 0;
    }
    else{
        strncpy(m_commonInfo.war_help_url, commonInfoDic["help"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("praise") == false) {
        m_commonInfo.praise[0] = 0;
    }else{
        strncpy(m_commonInfo.praise, commonInfoDic["praise"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("aboutus") == false) {
        m_commonInfo.aboutus[0] = 0;
    }
    else{
        strncpy(m_commonInfo.aboutus, commonInfoDic["aboutus"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("voice_icon") == false) {
        m_commonInfo.voice_icon[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_icon, commonInfoDic["voice_icon"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("voice_1") == false) {
        m_commonInfo.voice_1[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_1, commonInfoDic["voice_1"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_2") == false) {
        m_commonInfo.voice_2[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_2, commonInfoDic["voice_2"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_3") == false) {
        m_commonInfo.voice_3[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_3, commonInfoDic["voice_3"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_4") == false) {
        m_commonInfo.voice_4[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_4, commonInfoDic["voice_4"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_5") == false) {
        m_commonInfo.voice_5[0] = 0;
    }
    else
    {
        strncpy(m_commonInfo.voice_5, commonInfoDic["voice_5"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_6") == false) {
        m_commonInfo.voice_6[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_6, commonInfoDic["voice_6"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_7") == false) {
        m_commonInfo.voice_7[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_7, commonInfoDic["voice_7"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_8") == false) {
        m_commonInfo.voice_8[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_8, commonInfoDic["voice_8"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("voice_9") == false) {
        m_commonInfo.voice_9[0] = 0;
    }
    else{
        strncpy(m_commonInfo.voice_9, commonInfoDic["voice_9"].asString().c_str(), 499);
    }
    
    if (commonInfoDic.isMember("app_url") == false) {
        m_commonInfo.app_url[0] = 0;
    }
    else{
        strncpy(m_commonInfo.app_url, commonInfoDic["app_url"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("androidsg_download") == false) {
        m_commonInfo.androidsg_download[0] = 0;
    }
    else{
        strncpy(m_commonInfo.androidsg_download, commonInfoDic["androidsg_download"].asString().c_str(), 99);
    }
    
    m_commonInfo.get_reward_open = commonInfoDic.isMember("award_open") ? commonInfoDic["award_open"].asBool() : false;
    
    if (commonInfoDic.isMember("bbs_url") == false) {
        m_commonInfo.bbsurl[0] = 0;
    }
    else{
        strncpy(m_commonInfo.bbsurl, commonInfoDic["bbs_url"].asString().c_str(), 99);
    }
    
    if (commonInfoDic.isMember("charge_url") == false)
    {
        m_commonInfo.charge_url[0] = 0;
    }
    else
    {
        strncpy(m_commonInfo.charge_url, commonInfoDic["charge_url"].asString().c_str(), 199);
    }
    
    if (commonInfoDic.isMember("charge_type") == false)
    {
        m_commonInfo.charge_type = 1;
    }
    else
    {
        m_commonInfo.charge_type = commonInfoDic["charge_type"].asInt();
    }
    
    if (commonInfoDic.isMember("compgacha_note_url") == false) {
        m_commonInfo.compgacha_note_url[0] = 0;
    }
    else{
        strncpy(m_commonInfo.compgacha_note_url, commonInfoDic["compgacha_note_url"].asString().c_str(), 99);
    }
    
    m_commonInfo.evolution_lv = commonInfoDic.isMember("evolution_lv")? commonInfoDic["evolution_lv"].asInt(): 0;
    
    m_commonInfo.evolution_notice = commonInfoDic.isMember("evolution_notice")? commonInfoDic["evolution_notice"].asString(): "";
    
    m_commonInfo.wake_notice = commonInfoDic.isMember("wake_notice")? commonInfoDic["wake_notice"].asString(): "";
    
    m_commonInfo.mzsm_url = commonInfoDic.isMember("mzsm_html")? commonInfoDic["mzsm_html"].asString(): "";

    m_commonInfo.bOpen_cards_shop = commonInfoDic["open_cards_shop"].asBool();
        
    m_commonInfo.bOpen_month_charge = commonInfoDic["month_charge"].asBool();
    
    m_commonInfo.free_revive_lv = commonInfoDic["free_revive_lv"].asInt();
    m_commonInfo.free_revive_lv = 200;
    
    return &m_commonInfo;
}

int CGameData::getDefaultCntryBu(int lv)
{
    for (list<str_TaskCntryBtn>::iterator itEn = m_commonInfo.list_taskCntrybution.begin();
         itEn != m_commonInfo.list_taskCntrybution.end();
         ++itEn)
    {
        str_TaskCntryBtn str_task = *itEn;
        
        if (lv >= str_task.minLlv && lv <= str_task.maxLlv)
            return str_task.contribution;
    }

    return 500;
}

//******************************************************************************
// getHelperCardInfo
//******************************************************************************
CUserCardInfo* CGameData::getHelperCardInfo(int index)
{
    if(m_iSelectedHelper[index] >= m_pFriendHelpArr->count()
       || m_iSelectedHelper[index] == -1)
        return NULL;

    CFriendHelpInfo* pInfo = m_pFriendHelpArr->getObjectAtIndex(m_iSelectedHelper[index]);
    unsigned short cid = pInfo->leadercard_cid;
    unsigned short lv = pInfo->leadercardlv;
    unsigned short sklv = pInfo->sk_lv;
        
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    char tmp[32] = {'\0'};
    sprintf(tmp,"%d",cid);
    string strCid = string(tmp) + "_card";
    Json::Value card = m_allBaseCardsDic[strCid];
    
    m_helperCardInfo.cid = cid;
    strncpy(m_helperCardInfo.name, card["name"].asString().c_str(), 99);

    m_helperCardInfo.category = atoi(card["category"].asString().c_str());
    m_helperCardInfo.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
    m_helperCardInfo.i_AwakeNum = pInfo->i_awakeNum;
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        m_helperCardInfo.AwakeSkillArr[i] = 0;
    }
    
    if (card.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card["awake_skid"];
        for (int i=0; i<AwakeSkillArr.size(); i++)
        {
            if (i >= MaxAwakeSkillCnt)
                break;
            
            m_helperCardInfo.AwakeSkillArr[i] = AwakeSkillArr[i].asInt();
        }
    }
    
    m_helperCardInfo.lv = lv;
    m_helperCardInfo.sk_lv = sklv;
    m_helperCardInfo.star = atoi(card["star"].asString().c_str());
    m_helperCardInfo.ugp_max_star = atoi(card["target_star"].asString().c_str());
    m_helperCardInfo.skid = atoi(card["skid"].asString().c_str());
    m_helperCardInfo.ctype = atoi(card["ctype"].asString().c_str());;
    m_helperCardInfo.stype = card.isMember("stype") ? (unsigned char)atoi(card["stype"].asString().c_str()) : enElementInvalid;
    
    m_helperCardInfo.cost = card["cost"].asInt();
    m_helperCardInfo.leader_skid = atoi(card["leader_skid"].asString().c_str());
    m_helperCardInfo.max_lv = card["max_lv"].asInt();
    
    m_helperCardInfo.cou_name = card.isMember("country") ? card["country"].asInt() : 0;//卡片的国家属性

  //  m_helperCardInfo.cou_name = 4;
    
    
    
    m_helperCardInfo.pl_attack = pInfo->pl_attack;
    m_helperCardInfo.pl_hp = pInfo->pl_hp;
    m_helperCardInfo.pl_recover = pInfo->pl_recover;
    
    m_helperCardInfo.plus = m_helperCardInfo.pl_attack+m_helperCardInfo.pl_hp+ m_helperCardInfo.pl_recover;
    
    long recoverbase = card["recover"].asInt();
    float fRecoverGrowth = (float)card["recover_growth"].asDouble();
    m_helperCardInfo.recover = recoverbase + (m_helperCardInfo.lv - 1) * fRecoverGrowth+m_helperCardInfo.pl_recover*RECPLUSGROWTH;
    
    long hpbase = card["hp"].asInt();
    float fHpGrowth = (float)card["hp_growth"].asDouble();
    m_helperCardInfo.hp = hpbase + (m_helperCardInfo.lv - 1) * fHpGrowth+m_helperCardInfo.pl_hp*HPPLUSGROWTH;
    
    long attackbase = card["attack"].asInt();
    float fAtkGrowth = (float)card["attack_growth"].asDouble();
    m_helperCardInfo.attack = attackbase + (m_helperCardInfo.lv - 1) * fAtkGrowth+m_helperCardInfo.pl_attack*ATKPLUSGROWTH;
    
    if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
        m_helperCardInfo.bIsEluTgt = true;
    }
    else
        m_helperCardInfo.bIsEluTgt = false;
    
    if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
        m_helperCardInfo.bIsWkTgt = true;
    }
    else
        m_helperCardInfo.bIsWkTgt = false;
    
    return &m_helperCardInfo;
}


//******************************************************************************
// getTrophies
//******************************************************************************
int* CGameData::getTrophies()
{
    memset(m_pTrophies, 0, sizeof(m_pTrophies));
    
    if(isNewbie()){
        m_pTrophies[0] = 49;
        return m_pTrophies;
    }
    
    Json::Value tpDic = HttpComm::Inst()->getTrophiesDic();
    if(!tpDic.empty())
    {
        Json::Value cardsarr = tpDic["get_cards"];
        for(int i = 0; i < cardsarr.size(); i++){
            if(i >= kMaxTrophies)
                break;
            
            m_pTrophies[i] = atoi(cardsarr[i]["cid"].asString().c_str());
        }
    }
    
    return m_pTrophies;
}

CCMutableArray<GuildItemAward*> * CGameData::getGuildItemAwardArr()
{
    m_pGuildItemAwardAff->removeAllObjects();
    
    Json::Value tpDic = HttpComm::Inst()->getTrophiesDic();
    if (tpDic.empty())
        return m_pGuildItemAwardAff;
    
    if (!tpDic.isMember("guild_item_award"))
        return m_pGuildItemAwardAff;
    
    
    Json::Value itemArr = tpDic["guild_item_award"];
    Json::Value::Members guildItemNames = itemArr.getMemberNames();
    
    for (int i=0; i<itemArr.size(); i++) {
        GuildItemAward* guildItemAward = new GuildItemAward();
        guildItemAward->strItemId = guildItemNames[i];
        guildItemAward->strImgName = itemArr[guildItemNames[i]]["img"].asString();
        guildItemAward->lCnt = itemArr[guildItemNames[i]]["num"].asInt();
        
        m_pGuildItemAwardAff->addObject(guildItemAward);
        guildItemAward->release();
    }
    
    return m_pGuildItemAwardAff;
}

//******************************************************************************
// getNmlDgEndGuildAward
//******************************************************************************
long CGameData::getNmlDgEndGuildAward()
{
    long lRet = 0;
    
    Json::Value tpDic = HttpComm::Inst()->getTrophiesDic();
    if(!tpDic.empty())
    {
        if (tpDic.isMember("guild_award")) {
            lRet = tpDic["guild_award"].asInt();
        }
    }
    
    return lRet;
}

//******************************************************************************
// getShopCards
//******************************************************************************
CCMutableArray<CShopCardInfo*> * CGameData::getShopCards()
{
    static long slShopCardsUpdateTm = 0;
    if (slShopCardsUpdateTm < HttpComm::Inst()->getDicTime(enDicCfg))
    {
        slShopCardsUpdateTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    else
    {
        return m_pShopCardArr;
    }
    
    m_pShopCardArr->removeAllObjects();
    Json::Value shopCardsDic = HttpComm::Inst()->getCfgDic()["gacha_shop_config"];
    Json::Value::Members memberNameDic = shopCardsDic.getMemberNames();
    Json::Value oneShopCard;
    CShopCardInfo* pShopCardInfo = NULL;
    CCardBaseInfo* pBaseCardInfo = NULL;
    for (int i = 0; i < memberNameDic.size(); i++) {
        oneShopCard = shopCardsDic[memberNameDic[i]];
        
        pShopCardInfo = new CShopCardInfo();
        pShopCardInfo->sCid = atoi(oneShopCard["cid"].asString().c_str());
        pShopCardInfo->iPrice = oneShopCard["price"].asInt();
        pShopCardInfo->status = (enShopCardStatus)atoi(oneShopCard["state"].asString().c_str());        
        pShopCardInfo->iStartLv = oneShopCard.isMember("clv") ? oneShopCard["clv"].asInt() : 1;
        
        pBaseCardInfo = getCardBaseInfByCid(pShopCardInfo->sCid);
        pShopCardInfo->cStar = pBaseCardInfo->star;
        pShopCardInfo->cElement = pBaseCardInfo->ctype;
        pShopCardInfo->iStartLv = MIN(pShopCardInfo->iStartLv, pBaseCardInfo->max_lv);
        
        m_pShopCardArr->addObject(pShopCardInfo);
        pShopCardInfo->release();
    }
    
    sortShopCards();
    
    return m_pShopCardArr;
}

//******************************************************************************
// sortShopCards
//******************************************************************************
void CGameData::sortShopCards()
{
    CShopCardInfo* pCardInfo1 = NULL;
    CShopCardInfo* pCardInfo2 = NULL;
    CCMutableArray<CShopCardInfo*>* pTempArray = CCMutableArray<CShopCardInfo*>::arrayWithArray(m_pShopCardArr);
    int iCardCount = m_pShopCardArr->count();
    char iMaxStar = 1;
    
    // 按价格
    for (int i = 0; i < iCardCount - 1; i++) {
        for (int j = i+1; j < iCardCount; j++) {
            pCardInfo1 = pTempArray->getObjectAtIndex(i);
            pCardInfo2 = pTempArray->getObjectAtIndex(j);
            
            iMaxStar = MAX(pCardInfo1->cStar, iMaxStar);
            iMaxStar = MAX(pCardInfo2->cStar, iMaxStar);
            
            if (pCardInfo1->iPrice < pCardInfo2->iPrice) {
                pCardInfo1->retain();
                pTempArray->replaceObjectAtIndex(i, pCardInfo2);
                pTempArray->replaceObjectAtIndex(j, pCardInfo1);
                pCardInfo1->release();
            }
        }
    }

    // 按属性、星级
    m_pShopCardArr->removeAllObjects();
    for (int i = iMaxStar; i > 0; i--) {
        for (int j = enElementHeart; j <= enElementDark; j++) {
            for (int z = 0; z < iCardCount; z++) {
                pCardInfo1 = pTempArray->getObjectAtIndex(z);
                if (pCardInfo1->cStar == i && pCardInfo1->cElement == j)
                {
                    m_pShopCardArr->addObject(pCardInfo1);
                }
            }
        }
    }
}

//******************************************************************************
// getUserCards
//******************************************************************************
CCMutableArray<CUserCardInfo*> * CGameData::getUserCards()
{
    static long slLoginDicUpdateTm = 0;
    static long slAllCardDicUpdateTm = 0;
    static long slCfgDicUpdateTm = 0;
    
    bool bUpdate = false;
    
    if(HttpComm::Inst()->getDicTime(enDicLogin) > slLoginDicUpdateTm){
        bUpdate = true;
        slLoginDicUpdateTm = HttpComm::Inst()->getDicTime(enDicLogin);
    }
    
    if(HttpComm::Inst()->getDicTime(enDicAllCard) > slAllCardDicUpdateTm){
        bUpdate = true;
        slAllCardDicUpdateTm = HttpComm::Inst()->getDicTime(enDicAllCard);
    }
    
    if(HttpComm::Inst()->getDicTime(enDicCfg) > slCfgDicUpdateTm){
        bUpdate = true;
        slCfgDicUpdateTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    
    if(isNewbie())
        bUpdate = true;
    
    if(bUpdate == false){
        return m_pUserCardArr;
    }
    
    
    //user card info
    if (HttpComm::Inst()->getDicTime(enDicAllCard) == 0) {
        if(chkLogin() == false){
            m_allUserCardsDic = HttpComm::Inst()->getNewbieDic()["user_cards"];
            bUpdate = true;
        }
        else {
            if(getSmallDicTime(enSmallDicUserCards) < HttpComm::Inst()->getDicTime(enDicLogin)) {
                m_allSmalDicTime[enSmallDicUserCards] = HttpComm::Inst()->getDicTime(enDicLogin);
                m_allUserCardsDic = HttpComm::Inst()->getLoginDic()["user_cards"];
            }
        }
    }
    else{
        if(getSmallDicTime(enSmallDicUserCards) < HttpComm::Inst()->getDicTime(enDicAllCard)) {
            m_allSmalDicTime[enSmallDicUserCards] = HttpComm::Inst()->getDicTime(enDicAllCard);
            m_allUserCardsDic = HttpComm::Inst()->getAllCardDic();
        }
    }
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    m_pUserCardArr->removeAllObjects();
    
    CUserCardInfo* usercardinfo;
    
    
    Json::Value::Members array = m_allUserCardsDic.getMemberNames();
    for (int i =0; i< array.size(); i++) {
        usercardinfo = new CUserCardInfo();
        Json::Value user_card = m_allUserCardsDic[array[i]];
        string strcid(user_card["cid"].asString());
        
        if (m_allBaseCardsDic.isMember(strcid) == false) {
            continue;
        }
        
        Json::Value card_baseinfo = m_allBaseCardsDic[strcid];
        
        strncpy(usercardinfo->ucid, array[i].c_str(), 99);
        
        usercardinfo->lv = user_card["lv"].asInt();
        
        usercardinfo->cid = atoi(strcid.c_str());
        usercardinfo->exp = user_card["exp"].asInt();
        usercardinfo->sk_lv = user_card["sk_lv"].asInt();
        usercardinfo->upd_time = user_card["upd_time"].asInt();
        
        strncpy(usercardinfo->name, card_baseinfo["name"].asString().c_str(), 99);
        
        usercardinfo->base_exp = card_baseinfo["base_exp"].asInt();
        usercardinfo->category = atoi(card_baseinfo["category"].asCString());
        usercardinfo->sub_category = card_baseinfo.isMember("sub_category") ? atoi(card_baseinfo["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
        
        usercardinfo->i_AwakeNum = user_card.isMember("awake_index") ? user_card["awake_index"].asInt() : 0;
        
        for (int z=0; z<MaxAwakeSkillCnt; z++)
        {
            usercardinfo->AwakeSkillArr[z] = 0;
        }
        
        if (card_baseinfo.isMember("awake_skid"))
        {
            Json::Value AwakeSkillArr = card_baseinfo["awake_skid"];
            for (int z=0; z<AwakeSkillArr.size(); z++)
            {
                if (z >= MaxAwakeSkillCnt)
                    break;
                
                usercardinfo->AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
            }
        }
        
    
        
        usercardinfo->cou_name = card_baseinfo.isMember("country") ? card_baseinfo["country"].asInt() : 0;
      //  usercardinfo->cou_name = 4;
        
        usercardinfo->ugp_target = atoi(card_baseinfo["upg_target"].asString().c_str());
        usercardinfo->ugp_max_star = atoi(card_baseinfo["target_star"].asString().c_str());
        string tmp = card_baseinfo["exp_type"].asString();
        usercardinfo->exp_type = tmp.at(0);
        
        usercardinfo->upg_gold = (unsigned long)card_baseinfo["upg_gold"].asUInt();
        usercardinfo->star = (unsigned char)atoi(card_baseinfo["star"].asString().c_str());
        usercardinfo->skid = (unsigned short)atoi(card_baseinfo["skid"].asString().c_str());
        usercardinfo->ctype = (unsigned char)atoi(card_baseinfo["ctype"].asString().c_str());
        usercardinfo->stype = card_baseinfo.isMember("stype") ? (unsigned char)atoi(card_baseinfo["stype"].asString().c_str()) : enElementInvalid;
        
        usercardinfo->cost = card_baseinfo["cost"].asUInt();
        usercardinfo->leader_skid = atoi(card_baseinfo["leader_skid"].asString().c_str());
        usercardinfo->max_lv = card_baseinfo["max_lv"].asInt();
        
        usercardinfo->pl_attack = user_card.isMember("pl_attack") ? user_card["pl_attack"].asInt() : 0;
        usercardinfo->pl_hp = user_card.isMember("pl_hp") ? user_card["pl_hp"].asInt() : 0;
        usercardinfo->pl_recover = user_card.isMember("pl_recover") ? user_card["pl_recover"].asInt() : 0;
        
        usercardinfo->plus = usercardinfo->pl_attack+usercardinfo->pl_hp+ usercardinfo->pl_recover;
        
        long recoverbase = card_baseinfo["recover"].asInt();
        float fRecoverGrowth = card_baseinfo["recover_growth"].asDouble();
        usercardinfo->recover = recoverbase + (usercardinfo->lv - 1) * fRecoverGrowth+usercardinfo->pl_recover*RECPLUSGROWTH;
        
        long hpbase = card_baseinfo["hp"].asInt();
        float fHpGrowth = card_baseinfo["hp_growth"].asDouble();
        usercardinfo->hp = hpbase + (usercardinfo->lv - 1) * fHpGrowth+usercardinfo->pl_hp*HPPLUSGROWTH;
        
        long attackbase = card_baseinfo["attack"].asInt();
        float fAtkGrowth = card_baseinfo["attack_growth"].asDouble();
        usercardinfo->attack = attackbase + (usercardinfo->lv - 1) * fAtkGrowth+usercardinfo->pl_attack*ATKPLUSGROWTH;
        
        unsigned long  sell_goldbase = card_baseinfo["sell_gold"].asUInt();
        usercardinfo->sell_gold = sell_goldbase + (usercardinfo->lv - 1) * 10;
        
        Json::Value ugp_needArr = card_baseinfo["upg_need"];
        int j;
        memset(usercardinfo->ugp_need, 0, sizeof(usercardinfo->ugp_need));
        for ( j =0; j< ugp_needArr.size(); j++) {
            usercardinfo->ugp_need[j] = atoi(ugp_needArr[j].asString().c_str());
        }
        
        usercardinfo->bChecked = false;
        
        //入手地信息
        if (card_baseinfo.isMember("find_me")) {
            usercardinfo->find_me = card_baseinfo["find_me"].asString();
        }
        else{
            usercardinfo->find_me = "";
        }
        
        //究极进化
        usercardinfo->elu_target = atoi(card_baseinfo["elu_target"].asString().c_str());
        memset(usercardinfo->elu_need, 0, sizeof(usercardinfo->elu_need));
        
        if (usercardinfo->elu_target > 0) {
            Json::Value elu_needArr = card_baseinfo["elu_need"];
            
            for ( j =0; j< elu_needArr.size(); j++) {
                usercardinfo->elu_need[j] = atoi(elu_needArr[j].asString().c_str());
            }
        }
        
        if (card_baseinfo.isMember("is_elu_target") && (card_baseinfo["is_elu_target"].asInt() == 1)) {
            usercardinfo->bIsEluTgt = true;
        }
        else{
            usercardinfo->bIsEluTgt = false;
        }
        
        //觉醒
        usercardinfo->wk_target = atoi(card_baseinfo["wake_target"].asString().c_str());
        memset(usercardinfo->wk_need, 0, sizeof(usercardinfo->wk_need));
        
        if (usercardinfo->wk_target > 0) {
            Json::Value wk_needArr = card_baseinfo["wake_need"];
            
            for ( j =0; j< wk_needArr.size(); j++) {
                usercardinfo->wk_need[j] = atoi(wk_needArr[j].asString().c_str());
            }
        }
        
        if (card_baseinfo.isMember("is_wake_target") && (card_baseinfo["is_wake_target"].asInt() == 1)) {
            usercardinfo->bIsWkTgt = true;
        }
        else{
            usercardinfo->bIsWkTgt = false;
        }
        
        // 涅槃
        usercardinfo->np_need_num = card_baseinfo["need_num"].asInt();

        usercardinfo->np_list_target.clear();
        Json::Value np_targetArr = card_baseinfo["np_target"];
        if (!np_targetArr.empty())
        {
            for (int i = 0; i < np_targetArr.size(); i++) {
                short cid = np_targetArr[i].asInt();
                usercardinfo->np_list_target.push_back(cid);
            }
        }

        usercardinfo->np_list_need.clear();
        Json::Value np_needArr = card_baseinfo["np_need"];
        if (!np_needArr.empty())
        {
            for (int i = 0; i < np_needArr.size(); i++)
            {
                short cid = np_needArr[i].asInt();
                usercardinfo->np_list_need.push_back(cid);
            }
        }

        usercardinfo->np_list_show.clear();
        Json::Value np_showArr = card_baseinfo["np_show"];
        if (!np_needArr.empty())
        {
            for (int i = 0; i < np_showArr.size(); i++)
            {
                short cid = np_showArr[i].asInt();
                usercardinfo->np_list_show.push_back(cid);
            }
        }

        if (usercardinfo->np_need_num > 0
            && usercardinfo->np_list_target.size() > 0
            && usercardinfo->np_list_need.size() >= usercardinfo->np_need_num
            && usercardinfo->np_list_show.size() >= usercardinfo->np_need_num)
            usercardinfo->bIsNPTgt = true;
        else
            usercardinfo->bIsNPTgt = false;
        
        m_pUserCardArr->addObject(usercardinfo);
        
        usercardinfo->release();
    }
    
    
    //检查进化素材是否凑齐
    chkEvoMaterial();
    
    //检查超进化素材是否凑齐
    chkSuperEvoMaterial();
    
    //检查觉醒素材是否凑齐
    chkWakeMaterial();
    
    return m_pUserCardArr;
}

//******************************************************************************
// chkWakeMaterial
//******************************************************************************
void CGameData::chkWakeMaterial()
{
    CUserCardInfo* usercardinfo;
    
    for(int i = 0; i < m_pUserCardArr->count(); i++){
        usercardinfo = m_pUserCardArr->getObjectAtIndex(i);
        
        if(usercardinfo->wk_target == 0){
            usercardinfo->wk_flg = 0;
            continue;
        }
        
        int k;
        CUserCardInfo* pinfo;
        
        usercardinfo->wk_flg = 2;
        for(int j = 0; j < 5; j++){
            if(usercardinfo->wk_need[j] == 0)
                break;
            
            for(k = 0; k < m_pUserCardArr->count(); k++){
                pinfo = m_pUserCardArr->getObjectAtIndex(k);
                
                if (strcmp(usercardinfo->ucid, pinfo->ucid) == 0) {
                    continue;   // 基础武将不能在素材中
                }
                
                if(pinfo->bChecked)
                    continue;
                
                if(pinfo->cid == usercardinfo->wk_need[j]){
                    pinfo->bChecked = true;
                    break;
                }
            }
            
            if(k == m_pUserCardArr->count()){
                usercardinfo->wk_flg = 1;
                break;
            }
        }
        
        for(k = 0; k < m_pUserCardArr->count(); k++){
            pinfo = m_pUserCardArr->getObjectAtIndex(k);
            pinfo->bChecked = false;
        }
    }
}

//******************************************************************************
// chkEvoMaterial
//******************************************************************************
void CGameData::chkEvoMaterial()
{
    CUserCardInfo* usercardinfo;
    
    for(int i = 0; i < m_pUserCardArr->count(); i++){
        usercardinfo = m_pUserCardArr->getObjectAtIndex(i);
        
        if(usercardinfo->ugp_target == 0){
            usercardinfo->upg_flg = 0;
            continue;
        }
        
        int k;
        CUserCardInfo* pinfo;
        
        usercardinfo->upg_flg = 2;
        for(int j = 0; j < 5; j++){
            if(usercardinfo->ugp_need[j] == 0)
                break;
            
            for(k = 0; k < m_pUserCardArr->count(); k++){
                pinfo = m_pUserCardArr->getObjectAtIndex(k);
                
                if (strcmp(usercardinfo->ucid, pinfo->ucid) == 0) {
                    continue;   // 基础武将不能在素材中
                }
                
                if(pinfo->bChecked)
                    continue;
                
                if(pinfo->cid == usercardinfo->ugp_need[j]){
                    pinfo->bChecked = true;
                    break;
                }
            }
            
            if(k == m_pUserCardArr->count()){
                usercardinfo->upg_flg = 1;
                break;
            }
        }
        
        for(k = 0; k < m_pUserCardArr->count(); k++){
            pinfo = m_pUserCardArr->getObjectAtIndex(k);
            pinfo->bChecked = false;
        }
    }
}

//******************************************************************************
// chkSuperEvoMaterial
//******************************************************************************
void CGameData::chkSuperEvoMaterial()
{
    CUserCardInfo* usercardinfo;
    
    for(int i = 0; i < m_pUserCardArr->count(); i++){
        usercardinfo = m_pUserCardArr->getObjectAtIndex(i);
        
        if(usercardinfo->elu_target == 0){
            usercardinfo->elu_flg = 0;
            continue;
        }
        
        int k;
        CUserCardInfo* pinfo;
        
        usercardinfo->elu_flg = 2;
        for(int j = 0; j < 5; j++){
            if(usercardinfo->elu_need[j] == 0)
                break;
            
            for(k = 0; k < m_pUserCardArr->count(); k++){
                pinfo = m_pUserCardArr->getObjectAtIndex(k);
                
                if (strcmp(usercardinfo->ucid, pinfo->ucid) == 0) {
                    continue;   // 基础武将不能在素材中
                }
                
                if(pinfo->bChecked)
                    continue;
                
                if(pinfo->cid == usercardinfo->elu_need[j]){
                    pinfo->bChecked = true;
                    break;
                }
            }
            
            if(k == m_pUserCardArr->count()){
                usercardinfo->elu_flg = 1;
                break;
            }
        }
        
        for(k = 0; k < m_pUserCardArr->count(); k++){
            pinfo = m_pUserCardArr->getObjectAtIndex(k);
            pinfo->bChecked = false;
        }
    }
}

//******************************************************************************
// getUserId
//******************************************************************************
const char* CGameData::getErrMsg()
{
    string strErrMsg = HttpComm::Inst()->getErrMsg();
    
    if(!strErrMsg.empty())
        return strErrMsg.c_str();
    else
        return NULL;
}

//******************************************************************************
// timeCompare
//******************************************************************************
enTimeCompareResult CGameData::timeCompare()
{
    if (dateRecord == 0) {
        dateRecord = TimeTransform::currentTime();
        return enTimeNoChange;
    }
    
    long curTime = TimeTransform::currentTime();
    
    if(((dateRecord / 3600 + 8) / 24) != ((curTime / 3600 + 8) / 24)){
        dateRecord = curTime;
        return enDayChanged;
    }
    
    if((dateRecord / 3600) != (curTime / 3600)){
        dateRecord = curTime;
        return enHourChanged;
    }
    
    return enTimeNoChange;
}

//******************************************************************************
// getExpByType
//******************************************************************************
unsigned long CGameData::getExpByType(char type, int lv)
{
    if(getSmallDicTime(enSmallDicAllCardLevels) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_allCardLevelsDic = HttpComm::Inst()->getCfgDic()["card_level"]["exp_type"];
    }
    
    if(m_allCardLevelsDic.empty())
        return 0;
    
    char buf[10];
    buf[0] = type;
    buf[1] = 0;
    string strType(buf);
    
    char tmp[32] = {'\0'};
    sprintf(tmp,"%d",lv);
    string strLv(tmp);
    
    unsigned long ulExp = m_allCardLevelsDic[strType][strLv].asUInt();
    
    return ulExp;
}

//******************************************************************************
// getGachaRet
//******************************************************************************
CGachaRet* CGameData::getGachaRet()
{
    getMultiGachaRet();
    
    return (CGachaRet*)m_GachaRetArr->getObjectAtIndex(0);
}


bool CGameData::getUseCompoundResult()
{
    
    
    
    
    Json::Value res = HttpComm::Inst()->getUseCompoundDic();
    
//    CCLog("ttt = %s", res.toStyledString().c_str());
    
    
    Json::Value resCom = HttpComm::Inst()->getUseCompoundDic()["compound"];
    
    Json::Value resItem = resCom["item"];
    
    if(resItem.size() > 0)
        return true;
    
//    CCLog("ttt = %s", resItem.toStyledString().c_str());
//    CCLog("ttt = %d", resItem.size());


    
    
    Json::Value resChild = resCom["new_cards"];
    if(resChild.size() > 0)
        return true;
    
    
//    CCLog("ttt = %s", resChild.toStyledString().c_str());
//    CCLog("ttt = %d", resChild.size());

 
    return  false;
}
//******************************************************************************
// getParcelChildInfoAry
//******************************************************************************
CCMutableArray<ParcelChildInfo *> * CGameData::getParcelChildInfoAry()
{
    m_ParcelChildInfoArr->removeAllObjects();

    
    Json::Value cardInfoDic = HttpComm::Inst()->getCfgDic()["compound_card"];

    Json::Value::Members cardInfoDicNames = cardInfoDic.getMemberNames();
    
    
    CCMutableArray<ParcelChildInfo *> *temp_ParcelChildInfoArr;
    temp_ParcelChildInfoArr = new CCMutableArray<ParcelChildInfo *>();
    
//    CCLog("ttt = %s", cardInfoDic.toStyledString().c_str());
    
    for(int i = 0 ; i < cardInfoDic.size()  ; i ++)
    {

        
        ParcelChildInfo* parcelChildInfo = new ParcelChildInfo();
        parcelChildInfo->coin  = cardInfoDic[cardInfoDicNames[i]]["coin"].asInt();
        
        parcelChildInfo->toID = cardInfoDicNames[i];
        
//        CCLog("%s" , parcelChildInfo->toID.c_str());
        
        Json::Value eleDic = cardInfoDic[cardInfoDicNames[i]]["element"];
        Json::Value::Members eleDicNames = eleDic.getMemberNames();
        
        parcelChildInfo->m_ParcelNeedElementInfo = new CCMutableArray<ParcelNeedElementInfo *>();
        
        for(int j = 0 ; j < eleDic.size() ; j ++)
        {
            ParcelNeedElementInfo* parcelNeedElementInfo = new ParcelNeedElementInfo();
            
            parcelNeedElementInfo->objId = eleDicNames[j];
            parcelNeedElementInfo->num = eleDic[eleDicNames[j]].asInt();;
            
//            CCLog("%s" , parcelNeedElementInfo->objId.c_str());
//            CCLog("%d" , parcelNeedElementInfo->num);
            
            parcelChildInfo->m_ParcelNeedElementInfo->addObject(parcelNeedElementInfo);
            parcelNeedElementInfo->release();
            
        }
        
        
        parcelChildInfo->longTime = false;
        
        if(cardInfoDic[cardInfoDicNames[i]]["start_time"].asString() == "")
        {
            parcelChildInfo->longTime = true;
        }
        else
        {
            parcelChildInfo->start_time = getEchoTime(cardInfoDic[cardInfoDicNames[i]]["start_time"].asString());
            parcelChildInfo->end_time = getEchoTime(cardInfoDic[cardInfoDicNames[i]]["end_time"].asString());
        }

        
        
        parcelChildInfo->gold  = cardInfoDic[cardInfoDicNames[i]]["gold"].asInt();
        parcelChildInfo->lv  = cardInfoDic[cardInfoDicNames[i]]["lv"].asInt();
        parcelChildInfo->skill_lv  = cardInfoDic[cardInfoDicNames[i]]["skill_lv"].asInt();
        
        parcelChildInfo->rate  = cardInfoDic[cardInfoDicNames[i]]["rate"].asInt();
        
        
        parcelChildInfo->targetId  = cardInfoDic[cardInfoDicNames[i]]["target"].asString();
        
//        CCLog("%s" , parcelChildInfo->targetId.c_str());
        
        
        parcelChildInfo->wealth  = cardInfoDic[cardInfoDicNames[i]]["wealth"].asInt();

        
        
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        
        long lTimeOffset = parcelChildInfo->end_time - ulCurTime;
        
        if(lTimeOffset <= 0 && !parcelChildInfo->longTime)
            continue;
        
        
        temp_ParcelChildInfoArr->addObject(parcelChildInfo);
        parcelChildInfo->release();
        
    }
    
    
    
    while (temp_ParcelChildInfoArr->count() > 0)
    {
        int j = 99999;
        int num = 0;
        
        for(int i = 0 ; i < temp_ParcelChildInfoArr->count() ; i ++)
        {
            ParcelChildInfo* parcelChildInfo = temp_ParcelChildInfoArr->getObjectAtIndex(i);
            
            if(atoi(parcelChildInfo->toID.c_str()) < j)
            {
                j = atoi(parcelChildInfo->toID.c_str());
                num = i;
            }
        }
        
        
        ParcelChildInfo* parcelChildInfo1 = temp_ParcelChildInfoArr->getObjectAtIndex(num);
        m_ParcelChildInfoArr->addObject(parcelChildInfo1);
        temp_ParcelChildInfoArr->removeObjectAtIndex(num);
        
        
    }

    
    
    
    return m_ParcelChildInfoArr;
}


//******************************************************************************
// getParcelItemInfoAry
//******************************************************************************
CCMutableArray<ParcelItemInfo *> * CGameData::getParcelItemInfoAry()
{
    m_ParcelItemInfoArr->removeAllObjects();
    
    
    Json::Value itemInfoDic = HttpComm::Inst()->getCfgDic()["compound_item"];
    
    Json::Value::Members itemInfoDicNames = itemInfoDic.getMemberNames();
    
    
    
    CCMutableArray<ParcelItemInfo *> *temp_ParcelItemInfoArr;
    temp_ParcelItemInfoArr = new CCMutableArray<ParcelItemInfo *>();

    
    
    for(int i = 0 ; i < itemInfoDic.size()  ; i ++)
    {
        
        
        ParcelItemInfo* parcelItemInfo = new ParcelItemInfo();
        parcelItemInfo->coin  = itemInfoDic[itemInfoDicNames[i]]["coin"].asInt();
        
        parcelItemInfo->toID = itemInfoDicNames[i];
        
        Json::Value eleDic = itemInfoDic[itemInfoDicNames[i]]["element"];
        Json::Value::Members eleDicNames = eleDic.getMemberNames();
        
        parcelItemInfo->m_ParcelNeedElementInfo = new CCMutableArray<ParcelNeedElementInfo *>();
        
        
        for(int j = 0 ; j < eleDic.size() ; j ++)
        {
            ParcelNeedElementInfo* parcelNeedElementInfo = new ParcelNeedElementInfo();
            
            parcelNeedElementInfo->objId = eleDicNames[j];
            parcelNeedElementInfo->num = eleDic[eleDicNames[j]].asInt();;
            

            
            parcelItemInfo->m_ParcelNeedElementInfo->addObject(parcelNeedElementInfo);
            parcelNeedElementInfo->release();
            
        }
        
        
        parcelItemInfo->longTime = false;
        
//        CCLog("ttt = %s", itemInfoDic[itemInfoDicNames[i]]["start_time"].asString().c_str());
        
        if(itemInfoDic[itemInfoDicNames[i]]["start_time"].asString() == "")
        {
            parcelItemInfo->longTime = true;
        }
        else
        {
            parcelItemInfo->start_time = getEchoTime(itemInfoDic[itemInfoDicNames[i]]["start_time"].asString());
            parcelItemInfo->end_time = getEchoTime(itemInfoDic[itemInfoDicNames[i]]["end_time"].asString());
        }
        

        
        
        parcelItemInfo->gold  = itemInfoDic[itemInfoDicNames[i]]["gold"].asInt();
        
        parcelItemInfo->rate  = itemInfoDic[itemInfoDicNames[i]]["rate"].asInt();
        
        
        parcelItemInfo->targetId  = itemInfoDic[itemInfoDicNames[i]]["target"].asString();
        

        
        parcelItemInfo->wealth  = itemInfoDic[itemInfoDicNames[i]]["wealth"].asInt();
        
        
        
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        
        long lTimeOffset = parcelItemInfo->end_time - ulCurTime;
        
        if(lTimeOffset <= 0 && !parcelItemInfo->longTime)
            continue;

        
        
        temp_ParcelItemInfoArr->addObject(parcelItemInfo);
        parcelItemInfo->release();
        
    }
    
    
    
    while (temp_ParcelItemInfoArr->count() > 0)
    {
        int j = 99999;
        int num = 0;
        
        for(int i = 0 ; i < temp_ParcelItemInfoArr->count() ; i ++)
        {
            ParcelItemInfo* parcelItemInfo = temp_ParcelItemInfoArr->getObjectAtIndex(i);
            
            if(atoi(parcelItemInfo->toID.c_str()) < j)
            {
                j = atoi(parcelItemInfo->toID.c_str());
                num = i;
            }
        }
        
        
        ParcelItemInfo* parcelItemInfo1 = temp_ParcelItemInfoArr->getObjectAtIndex(num);
        m_ParcelItemInfoArr->addObject(parcelItemInfo1);
        temp_ParcelItemInfoArr->removeObjectAtIndex(num);
        
        
    }
    

    
    
    
    return m_ParcelItemInfoArr;
}



//******************************************************************************
// getMultiGachaRet
//******************************************************************************
CCMutableArray<CGachaRet *> * CGameData::getMultiGachaRet()
{
    Json::Value newcardsArr ;
    
    if (!chkLogin())
    {
        Json::Value temp = HttpComm::Inst()->getNewbieDic();
        
        if(temp.isMember("new_cards"))
            newcardsArr = temp["new_cards"];
        else
            newcardsArr = temp["new_card"];

    }
    
    else
        newcardsArr = HttpComm::Inst()->getGachaDic();
    
    if(newcardsArr.empty())
        return NULL;
    
    m_GachaRetArr->removeAllObjects();
    
    Json::Value onecardDic;
    CGachaRet* gacharet;
    
    if(!newcardsArr.empty()){
        
        for(int i = 0; i < newcardsArr.size(); i++){
            onecardDic = newcardsArr[i];            
            if(!onecardDic.empty()){
                gacharet = new CGachaRet();
                                
                strncpy(gacharet->ucid, onecardDic["ucid"].asString().c_str(), 99);
                
                gacharet->is_first = onecardDic["is_first"].asInt();
                gacharet->is_compGachaMaterial = onecardDic["is_compgacha_material"].asBool();
                gacharet->pl_attack =  onecardDic.isMember("pl_attack") ? onecardDic["pl_attack"].asInt() : 0;
                gacharet->pl_hp = onecardDic.isMember("pl_hp") ? onecardDic["pl_hp"].asInt() : 0;
                gacharet->pl_recover = onecardDic.isMember("pl_recover") ? onecardDic["pl_recover"].asInt() : 0;
                gacharet->plus = gacharet->pl_attack+ gacharet->pl_hp+ gacharet->pl_recover;
                m_GachaRetArr->addObject(gacharet);
            }
        }
        
    }

    return m_GachaRetArr;
}

//******************************************************************************
// getUsrCardInfByUcid
//******************************************************************************
CUserCardInfo* CGameData::getUsrCardInfByUcid(const char* ucid)
{
    if (strcmp(ucid, "") == 0) {
        return NULL;
    }
    //user card info
    if (HttpComm::Inst()->getDicTime(enDicAllCard) == 0) {
        if (HttpComm::Inst()->getDicTime(enDicLogin) == 0)
            m_allUserCardsDic = HttpComm::Inst()->getNewbieDic()["user_cards"];
        else
        {
            if(getSmallDicTime(enSmallDicUserCards) < HttpComm::Inst()->getDicTime(enDicLogin)) {
                m_allSmalDicTime[enSmallDicUserCards] = HttpComm::Inst()->getDicTime(enDicLogin);
                m_allUserCardsDic = HttpComm::Inst()->getLoginDic()["user_cards"];
            }
        }
    }
    else{
        if(getSmallDicTime(enSmallDicUserCards) < HttpComm::Inst()->getDicTime(enDicAllCard)) {
            m_allSmalDicTime[enSmallDicUserCards] = HttpComm::Inst()->getDicTime(enDicAllCard);
            m_allUserCardsDic = HttpComm::Inst()->getAllCardDic();
        }
    }
    

    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }

    string strUcid(ucid);
    
    if (m_allUserCardsDic.isMember(ucid) == false) {
        return NULL;
    }
    
    Json::Value user_card = m_allUserCardsDic[ucid];
    
    std::string strcid(user_card["cid"].asString());
    
    if (m_allBaseCardsDic.isMember(strcid) == false) {
        CCLog("!!! undefined card: %s", strcid.c_str());
        return NULL;
    }
    
    Json::Value card_baseinfo = m_allBaseCardsDic[strcid];
    
    strncpy(m_usrCardInf.ucid, ucid, 99);
    
    m_usrCardInf.lv = user_card["lv"].asInt();
    m_usrCardInf.cid = atoi(strcid.c_str());
    m_usrCardInf.exp = user_card["exp"].asInt();
    m_usrCardInf.sk_lv = user_card["sk_lv"].asInt();
    m_usrCardInf.upd_time = user_card["upd_time"].asInt();
    
    strncpy(m_usrCardInf.name, card_baseinfo["name"].asString().c_str(), 99);
    
    m_usrCardInf.base_exp = card_baseinfo["base_exp"].asInt();
    m_usrCardInf.category = atoi(card_baseinfo["category"].asString().c_str());
    m_usrCardInf.sub_category = card_baseinfo.isMember("sub_category") ? atoi(card_baseinfo["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
    m_usrCardInf.i_AwakeNum = user_card.isMember("awake_index") ? user_card["awake_index"].asInt() : 0;
    
    for (int z=0; z<MaxAwakeSkillCnt; z++)
    {
        m_usrCardInf.AwakeSkillArr[z] = 0;
    }
    
    if (card_baseinfo.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card_baseinfo["awake_skid"];
        for (int z=0; z<AwakeSkillArr.size(); z++)
        {
            if (z >= MaxAwakeSkillCnt)
                break;
            
            m_usrCardInf.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
        }
    }
    
    
 
    
    
    m_usrCardInf.cou_name = card_baseinfo.isMember("country") ? card_baseinfo["country"].asInt() : 0;//卡片的国家属性 //卡片的国家属性
    
   // m_usrCardInf.cou_name = 4;
    
    m_usrCardInf.ugp_target = atoi(card_baseinfo["upg_target"].asString().c_str());
    m_usrCardInf.ugp_max_star = atoi(card_baseinfo["target_star"].asString().c_str());
    strncpy(&(m_usrCardInf.exp_type), card_baseinfo["exp_type"].asString().c_str(), 1);
    
    m_usrCardInf.upg_gold = card_baseinfo["upg_gold"].asInt();
    m_usrCardInf.star = atoi(card_baseinfo["star"].asString().c_str());
    m_usrCardInf.skid = atoi(card_baseinfo["skid"].asString().c_str());
    m_usrCardInf.ctype = atoi(card_baseinfo["ctype"].asString().c_str());
    m_usrCardInf.stype = card_baseinfo.isMember("stype") ? (unsigned char)atoi(card_baseinfo["stype"].asString().c_str()) : enElementInvalid;
    
    m_usrCardInf.cost = card_baseinfo["cost"].asInt();
    m_usrCardInf.leader_skid = atoi(card_baseinfo["leader_skid"].asString().c_str());
    m_usrCardInf.max_lv = card_baseinfo["max_lv"].asInt();
    
    //+值
    m_usrCardInf.pl_attack = user_card.isMember("pl_attack") ? user_card["pl_attack"].asInt() : 0;
    m_usrCardInf.pl_hp = user_card.isMember("pl_hp") ? user_card["pl_hp"].asInt() : 0;
    m_usrCardInf.pl_recover = user_card.isMember("pl_recover") ? user_card["pl_recover"].asInt() : 0;
    
    m_usrCardInf.plus = m_usrCardInf.pl_attack + m_usrCardInf.pl_hp + m_usrCardInf.pl_recover;
    
    long recoverbase = card_baseinfo["recover"].asInt();
    float fRecoverGrowth = card_baseinfo["recover_growth"].asDouble();
    m_usrCardInf.recover = recoverbase + (m_usrCardInf.lv - 1) * fRecoverGrowth + m_usrCardInf.pl_recover*RECPLUSGROWTH;
    
    long hpbase = card_baseinfo["hp"].asInt();
    float fHpGrowth = card_baseinfo["hp_growth"].asDouble();
    m_usrCardInf.hp = hpbase + (m_usrCardInf.lv - 1) * fHpGrowth+m_usrCardInf.pl_hp*HPPLUSGROWTH;
    
    long attackbase = card_baseinfo["attack"].asInt();
    float fAtkGrowth = card_baseinfo["attack_growth"].asDouble();
    m_usrCardInf.attack = attackbase + (m_usrCardInf.lv - 1) * fAtkGrowth+m_usrCardInf.pl_attack*ATKPLUSGROWTH;
    
    unsigned long  sell_goldbase = card_baseinfo["sell_gold"].asUInt();
    m_usrCardInf.sell_gold = sell_goldbase + (m_usrCardInf.lv - 1) * 10;
    
    Json::Value ugp_needArr = card_baseinfo["upg_need"];
    int j;
    memset(m_usrCardInf.ugp_need, 0, sizeof(m_usrCardInf.ugp_need));
    for ( j =0; j< ugp_needArr.size(); j++) {
        m_usrCardInf.ugp_need[j] = atoi(ugp_needArr[j].asString().c_str());
    }
    
    //入手地信息
    if (card_baseinfo.isMember("find_me")) {
        m_usrCardInf.find_me = card_baseinfo["find_me"].asString();
    }
    else{
        m_usrCardInf.find_me = "";
    }
    
    //究极进化
    m_usrCardInf.elu_target = atoi(card_baseinfo["elu_target"].asString().c_str());
    memset(m_usrCardInf.elu_need, 0, sizeof(m_usrCardInf.elu_need));
    
    if (m_usrCardInf.elu_target > 0) {
        Json::Value elu_needArr = card_baseinfo["elu_need"];
        
        for ( j =0; j< elu_needArr.size(); j++) {
            m_usrCardInf.elu_need[j] = atoi(elu_needArr[j].asString().c_str());
        }
    }
    
    if (card_baseinfo.isMember("is_elu_target") && (card_baseinfo["is_elu_target"].asInt() == 1)) {
        m_usrCardInf.bIsEluTgt = true;
    }
    else{
        m_usrCardInf.bIsEluTgt = false;
    }
    
    //觉醒
    m_usrCardInf.wk_target = atoi(card_baseinfo["wake_target"].asString().c_str());
    memset(m_usrCardInf.wk_need, 0, sizeof(m_usrCardInf.wk_need));
    
    if (m_usrCardInf.wk_target > 0) {
        Json::Value wk_needArr = card_baseinfo["wake_need"];
        
        for ( j =0; j< wk_needArr.size(); j++) {
            m_usrCardInf.wk_need[j] = atoi(wk_needArr[j].asString().c_str());
        }
    }
    
    if (card_baseinfo.isMember("is_wake_target") && (card_baseinfo["is_wake_target"].asInt() == 1)) {
        m_usrCardInf.bIsWkTgt = true;
    }
    else{
        m_usrCardInf.bIsWkTgt = false;
    }
    
    // 涅槃
    m_usrCardInf.np_need_num = card_baseinfo["need_num"].asInt();
    
    m_usrCardInf.np_list_target.clear();
    Json::Value np_targetArr = card_baseinfo["np_target"];
    if (!np_targetArr.empty())
    {
        for (int i = 0; i < np_targetArr.size(); i++) {
            short cid = np_targetArr[i].asInt();
            m_usrCardInf.np_list_target.push_back(cid);
        }
    }
    
    m_usrCardInf.np_list_need.clear();
    Json::Value np_needArr = card_baseinfo["np_need"];
    if (!np_needArr.empty())
    {
        for (int i = 0; i < np_needArr.size(); i++)
        {
            short cid = np_needArr[i].asInt();
            m_usrCardInf.np_list_need.push_back(cid);
        }
    }
    
    m_usrCardInf.np_list_show.clear();
    Json::Value np_showArr = card_baseinfo["np_show"];
    if (!np_showArr.empty())
    {
        for (int i = 0; i < np_showArr.size(); i++)
        {
            short cid = np_showArr[i].asInt();
            m_usrCardInf.np_list_show.push_back(cid);
        }
    }
    
    if (m_usrCardInf.np_need_num > 0
        && m_usrCardInf.np_list_target.size() > 0
        && m_usrCardInf.np_list_need.size() >= m_usrCardInf.np_need_num
        && m_usrCardInf.np_list_show.size() >= m_usrCardInf.np_need_num)
        m_usrCardInf.bIsNPTgt = true;
    else
        m_usrCardInf.bIsNPTgt = false;


    return &m_usrCardInf;
}

//******************************************************************************
// getCardBaseInfByMailInfo
//******************************************************************************
CUserCardInfo*  CGameData::getUsrCardInfByMailInf(CMailSenderInf* pSenderInf)
{
    stMailSenderInf st = pSenderInf->senderInf;
    
    char tmp[32] = {'\0'};
    sprintf(tmp,"%d",st.lcard_cid);
    string strCid = string(tmp) + "_card";
    if (m_allBaseCardsDic.isMember(strCid) == false) {
        return NULL;
    }
    Json::Value card = m_allBaseCardsDic[strCid];
    strncpy(m_usrCardInf.name, card["name"].asString().c_str(), 99);             // name
    m_usrCardInf.cid             = st.lcard_cid;                                 // icon
    m_usrCardInf.lv              = st.lcard_lv;                                  // level
    m_usrCardInf.max_lv          = card["max_lv"].asInt();                       // max_level
    m_usrCardInf.category        = atoi(card["category"].asString().c_str());    // star at left top
    m_usrCardInf.sub_category    = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
    m_usrCardInf.i_AwakeNum = st.i_awakeNum;
    for (int z=0; z<MaxAwakeSkillCnt; z++)
    {
        m_usrCardInf.AwakeSkillArr[z] = 0;
    }
    
    if (card.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card["awake_skid"];
        for (int z=0; z<AwakeSkillArr.size(); z++)
        {
            if (z >= MaxAwakeSkillCnt)
                break;
            
            m_usrCardInf.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
        }
    }
    
    m_usrCardInf.cou_name      = card.isMember("country") ? card["country"].asInt() : 0;//卡片的国家属性
   // m_usrCardInf.cou_name = 4;

    m_usrCardInf.star            = atoi(card["star"].asString().c_str());        // star
    m_usrCardInf.ugp_max_star    = atoi(card["target_star"].asString().c_str());
    m_usrCardInf.skid            = atoi(card["skid"].asString().c_str());        // skill
    m_usrCardInf.ctype           = atoi(card["ctype"].asString().c_str());       // ----------  what's this?????
    m_usrCardInf.stype           = card.isMember("stype") ? (unsigned char)atoi(card["stype"].asString().c_str()) : enElementInvalid;
    
    m_usrCardInf.sk_lv           = st.lcard_sklv;                                // skill level
    m_usrCardInf.cost            = card["cost"].asInt();                         // tong yu li
    m_usrCardInf.leader_skid     = atoi(card["leader_skid"].asString().c_str()); // leader skill
    m_usrCardInf.pl_attack       = st.pl_attack;                                 // pl attack
    m_usrCardInf.pl_hp           = st.pl_hp;                                     // pl hp
    m_usrCardInf.pl_recover      = st.pl_recover;                                // pl recover
    
    // recover
    long recoverbase = card["recover"].asInt();
    float fRecoverGrowth = card["recover_growth"].asDouble();
    m_usrCardInf.recover = recoverbase + (m_usrCardInf.lv - 1) * fRecoverGrowth+m_usrCardInf.pl_recover*RECPLUSGROWTH;
    
    // hp
    long hpbase = card["hp"].asInt();
    float fHpGrowth = card["hp_growth"].asDouble();
    m_usrCardInf.hp = hpbase + (m_usrCardInf.lv - 1) * fHpGrowth+m_usrCardInf.pl_hp*HPPLUSGROWTH;
    
    // attack
    long attackbase = card["attack"].asInt();
    float fAtkGrowth = card["attack_growth"].asDouble();
    m_usrCardInf.attack = attackbase + (m_usrCardInf.lv - 1) * fAtkGrowth+m_usrCardInf.pl_attack*ATKPLUSGROWTH;
    
    if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
        m_usrCardInf.bIsEluTgt = true;
    }
    else{
        m_usrCardInf.bIsEluTgt = false;
    }
    
    if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
        m_usrCardInf.bIsWkTgt = true;
    }
    else{
        m_usrCardInf.bIsWkTgt = false;
    }

    return  &m_usrCardInf;
}

//******************************************************************************
// getUsrCardInfByRentStr
//******************************************************************************
CUserCardInfo* CGameData::getUsrCardInfByRentStr(string& str)
{
    string::size_type pos(0);
    pos = str.find(",");
    
    if (pos == string::npos) {
        return NULL;
    }
    
    string strCid = str.substr(0, pos);
    strCid.append("_card");
    
    const char* lvPos = str.c_str() + pos + 1;
    int iLv = atoi(lvPos);
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    if (m_allBaseCardsDic.isMember(strCid) == false) {
        CCLog("!!! undefined card: %s", strCid.c_str());
        return NULL;
    }
    
    Json::Value card_baseinfo = m_allBaseCardsDic[strCid];
    
    m_usrCardInf.lv = iLv;
    m_usrCardInf.cid = atoi(strCid.c_str());
    m_usrCardInf.exp = 0;
    m_usrCardInf.sk_lv = 1;
    m_usrCardInf.upd_time = 0;
    
    strncpy(m_usrCardInf.name, card_baseinfo["name"].asString().c_str(), 99);
    
    m_usrCardInf.base_exp = card_baseinfo["base_exp"].asInt();
    m_usrCardInf.category = atoi(card_baseinfo["category"].asString().c_str());
    m_usrCardInf.sub_category    = card_baseinfo.isMember("sub_category") ? atoi(card_baseinfo["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
//    m_usrCardInf.i_AwakeNum = m_usrCardInf.i_AwakeNum = user_card.isMember("awake_index") ? user_card["awake_index"].asInt() : 0;
    
    for (int z=0; z<MaxAwakeSkillCnt; z++)
    {
        m_usrCardInf.AwakeSkillArr[z] = 0;
    }
    
    if (card_baseinfo.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card_baseinfo["awake_skid"];
        for (int z=0; z<AwakeSkillArr.size(); z++)
        {
            if (z >= MaxAwakeSkillCnt)
                break;
            
            m_usrCardInf.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
        }
    }
    

    
    m_usrCardInf.cou_name = card_baseinfo.isMember("country") ? card_baseinfo["country"].asInt() : 0;//卡片的国家属性
 //  m_usrCardInf.cou_name = 4;
    
    m_usrCardInf.ugp_target = atoi(card_baseinfo["upg_target"].asString().c_str());
    m_usrCardInf.ugp_max_star = atoi(card_baseinfo["target_star"].asString().c_str());
    strncpy(&(m_usrCardInf.exp_type), card_baseinfo["exp_type"].asString().c_str(), 1);
    
    m_usrCardInf.upg_gold = card_baseinfo["upg_gold"].asInt();
    m_usrCardInf.star = atoi(card_baseinfo["star"].asString().c_str());
    m_usrCardInf.skid = atoi(card_baseinfo["skid"].asString().c_str());
    m_usrCardInf.ctype = atoi(card_baseinfo["ctype"].asString().c_str());
    m_usrCardInf.stype = card_baseinfo.isMember("stype") ? (unsigned char)atoi(card_baseinfo["stype"].asString().c_str()) : enElementInvalid;
    
    m_usrCardInf.cost = card_baseinfo["cost"].asInt();
    m_usrCardInf.leader_skid = atoi(card_baseinfo["leader_skid"].asString().c_str());
    m_usrCardInf.max_lv = card_baseinfo["max_lv"].asInt();
    
    m_usrCardInf.pl_attack = 0;
    m_usrCardInf.pl_hp = 0;
    m_usrCardInf.pl_recover = 0;
    m_usrCardInf.plus = 0;
    
    long recoverbase = card_baseinfo["recover"].asInt();
    float fRecoverGrowth = card_baseinfo["recover_growth"].asDouble();
    m_usrCardInf.recover = recoverbase + (m_usrCardInf.lv - 1) * fRecoverGrowth+m_usrCardInf.pl_recover*RECPLUSGROWTH;
    
    long hpbase = card_baseinfo["hp"].asInt();
    float fHpGrowth = card_baseinfo["hp_growth"].asDouble();
    m_usrCardInf.hp = hpbase + (m_usrCardInf.lv - 1) * fHpGrowth+m_usrCardInf.pl_hp*HPPLUSGROWTH;
    
    long attackbase = card_baseinfo["attack"].asInt();
    float fAtkGrowth = card_baseinfo["attack_growth"].asDouble();
    m_usrCardInf.attack = attackbase + (m_usrCardInf.lv - 1) * fAtkGrowth+m_usrCardInf.pl_attack*ATKPLUSGROWTH;
    
    unsigned long  sell_goldbase = card_baseinfo["sell_gold"].asUInt();
    m_usrCardInf.sell_gold = sell_goldbase + (m_usrCardInf.lv - 1) * 10;
    
    //入手地信息
    if (card_baseinfo.isMember("find_me")) {
        m_usrCardInf.find_me = card_baseinfo["find_me"].asString();
    }
    else{
        m_usrCardInf.find_me = "";
    }
    
    //究极进化
    m_usrCardInf.elu_target = atoi(card_baseinfo["elu_target"].asString().c_str());
    memset(m_usrCardInf.elu_need, 0, sizeof(m_usrCardInf.elu_need));
    
    if (m_usrCardInf.elu_target > 0) {
        Json::Value elu_needArr = card_baseinfo["elu_need"];
        
        for (int j =0; j< elu_needArr.size(); j++) {
            m_usrCardInf.elu_need[j] = atoi(elu_needArr[j].asString().c_str());
        }
    }
    
    if (card_baseinfo.isMember("is_elu_target") && (card_baseinfo["is_elu_target"].asInt() == 1)) {
        m_usrCardInf.bIsEluTgt = true;
    }
    else{
        m_usrCardInf.bIsEluTgt = false;
    }
    
    //觉醒
    m_usrCardInf.wk_target = atoi(card_baseinfo["wake_target"].asString().c_str());
    memset(m_usrCardInf.wk_need, 0, sizeof(m_usrCardInf.wk_need));
    
    if (m_usrCardInf.wk_target > 0) {
        Json::Value wk_needArr = card_baseinfo["wake_need"];
        
        for (int j =0; j< wk_needArr.size(); j++) {
            m_usrCardInf.wk_need[j] = atoi(wk_needArr[j].asString().c_str());
        }
    }
    
    if (card_baseinfo.isMember("is_wake_target") && (card_baseinfo["is_wake_target"].asInt() == 1)) {
        m_usrCardInf.bIsWkTgt = true;
    }
    else{
        m_usrCardInf.bIsWkTgt = false;
    }
    
    // 涅槃
    m_usrCardInf.np_need_num = card_baseinfo["need_num"].asInt();
    
    m_usrCardInf.np_list_target.clear();
    Json::Value np_targetArr = card_baseinfo["np_target"];
    if (!np_targetArr.empty())
    {
        for (int i = 0; i < np_targetArr.size(); i++) {
            short cid = np_targetArr[i].asInt();
            m_usrCardInf.np_list_target.push_back(cid);
        }
    }
    
    m_usrCardInf.np_list_need.clear();
    Json::Value np_needArr = card_baseinfo["np_need"];
    if (!np_needArr.empty())
    {
        for (int i = 0; i < np_needArr.size(); i++)
        {
            short cid = np_needArr[i].asInt();
            m_usrCardInf.np_list_need.push_back(cid);
        }
    }
    
    m_usrCardInf.np_list_show.clear();
    Json::Value np_showArr = card_baseinfo["np_show"];
    if (!np_showArr.empty())
    {
        for (int i = 0; i < np_showArr.size(); i++)
        {
            short cid = np_showArr[i].asInt();
            m_usrCardInf.np_list_show.push_back(cid);
        }
    }
    
    if (m_usrCardInf.np_need_num > 0
        && m_usrCardInf.np_list_target.size() > 0
        && m_usrCardInf.np_list_need.size() >= m_usrCardInf.np_need_num
        && m_usrCardInf.np_list_show.size() >= m_usrCardInf.np_need_num)
        m_usrCardInf.bIsNPTgt = true;
    else
        m_usrCardInf.bIsNPTgt = false;
    
    return &m_usrCardInf;
}

//******************************************************************************
// getCardBaseInfByCid
//******************************************************************************
CCardBaseInfo* CGameData::getCardBaseInfByCid(unsigned short cid)
{
    if(cid == 0)
        return NULL;
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    char tmp[32] = {'\0'};
    sprintf(tmp,"%d",cid);
    
    string strCid = string(tmp) + "_card";
    
    if (m_allBaseCardsDic.isMember(strCid) == false) {
        return NULL;
    }
    
    Json::Value card = m_allBaseCardsDic[strCid];
    
    strncpy(m_baseCardInf.name, card["name"].asString().c_str(), 99);
    
    m_baseCardInf.cid = cid;
    m_baseCardInf.base_exp = card["base_exp"].asInt();
    m_baseCardInf.category = atoi(card["category"].asString().c_str());
    m_baseCardInf.sub_category    = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
    
    for (int z=0; z<MaxAwakeSkillCnt; z++)
        m_baseCardInf.AwakeSkillArr[z] = 0;
    
    if (card.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card["awake_skid"];
        for (int z=0; z<AwakeSkillArr.size(); z++)
        {
            if (z >= MaxAwakeSkillCnt)
                break;
            
            m_baseCardInf.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
        }
    }
    

    
//    CCLog("card info = %s", card.toStyledString().c_str());
//    CCLog("ugp_target = %s", card["ugp_target"].asString().c_str());
    
    m_baseCardInf.cou_name = card.isMember("country") ? card["country"].asInt() : 0;//卡片的国家属性
   //  m_baseCardInf.cou_name = 4;
    
    m_baseCardInf.ugp_target = atoi(card["upg_target"].asString().c_str());
    m_baseCardInf.ugp_max_star = atoi(card["target_star"].asString().c_str());
    strncpy(&(m_baseCardInf.exp_type), card["exp_type"].asString().c_str(), 1);
    
    m_baseCardInf.upg_gold = card["upg_gold"].asInt();
    m_baseCardInf.star = atoi(card["star"].asString().c_str());
    m_baseCardInf.skid = atoi(card["skid"].asString().c_str());
    m_baseCardInf.ctype = atoi(card["ctype"].asString().c_str());
    m_baseCardInf.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid ;
    
    m_baseCardInf.cost = card["cost"].asInt();
    m_baseCardInf.leader_skid = atoi(card["leader_skid"].asString().c_str());
    m_baseCardInf.max_lv = card["max_lv"].asInt();
    
    m_baseCardInf.recover = card["recover"].asInt();
    m_baseCardInf.hp = card["hp"].asInt();
    m_baseCardInf.attack = card["attack"].asInt();
    m_baseCardInf.sell_gold = card["sell_gold"].asInt();
    
    m_baseCardInf.hp_growth = card["hp_growth"].asDouble();
    m_baseCardInf.attack_growth = card["attack_growth"].asDouble();
    m_baseCardInf.recover_growth = card["recover_growth"].asDouble();
    
    Json::Value ugp_needArr = card["ugp_need"];
    int j;
    memset(m_baseCardInf.ugp_need, 0, sizeof(m_baseCardInf.ugp_need));
    for ( j =0; j< ugp_needArr.size(); j++) {
        m_baseCardInf.ugp_need[j] = ugp_needArr[j].asInt();
    }
    
    //入手地信息
    if (card.isMember("find_me")) {
        m_baseCardInf.find_me = card["find_me"].asString();
    }
    else{
        m_baseCardInf.find_me = "";
    }
    
    //究极进化
    m_baseCardInf.elu_target = atoi(card["elu_target"].asString().c_str());
    memset(m_baseCardInf.elu_need, 0, sizeof(m_baseCardInf.elu_need));
    
    if (m_baseCardInf.elu_target > 0) {
        Json::Value elu_needArr = card["elu_need"];
        
        for ( j =0; j< elu_needArr.size(); j++) {
            m_baseCardInf.elu_need[j] = atoi(elu_needArr[j].asString().c_str());
        }
    }
    
    if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
        m_baseCardInf.bIsEluTgt = true;
    }
    else{
        m_baseCardInf.bIsEluTgt = false;
    }
    
    //觉醒
    m_baseCardInf.wk_target = atoi(card["wake_target"].asString().c_str());
    memset(m_baseCardInf.wk_need, 0, sizeof(m_baseCardInf.wk_need));
    
    if (m_baseCardInf.wk_target > 0) {
        Json::Value wk_needArr = card["wake_need"];
        
        for ( j =0; j< wk_needArr.size(); j++) {
            m_baseCardInf.wk_need[j] = atoi(wk_needArr[j].asString().c_str());
        }
    }
    
    if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
        m_baseCardInf.bIsWkTgt = true;
    }
    else{
        m_baseCardInf.bIsWkTgt = false;
    }
    
    // 涅槃
    m_baseCardInf.np_need_num = card["need_num"].asInt();
    
    m_baseCardInf.np_list_target.clear();
    Json::Value np_targetArr = card["np_target"];
    if (!np_targetArr.empty())
    {
        for (int i = 0; i < np_targetArr.size(); i++) {
            short cid = np_targetArr[i].asInt();
            m_baseCardInf.np_list_target.push_back(cid);
        }
    }
    
    m_baseCardInf.np_list_need.clear();
    Json::Value np_needArr = card["np_need"];
    if (!np_needArr.empty())
    {
        for (int i = 0; i < np_needArr.size(); i++)
        {
            short cid = np_needArr[i].asInt();
            m_baseCardInf.np_list_need.push_back(cid);
        }
    }
    
    m_baseCardInf.np_list_show.clear();
    Json::Value np_showArr = card["np_show"];
    if (!np_showArr.empty())
    {
        for (int i = 0; i < np_showArr.size(); i++)
        {
            short cid = np_showArr[i].asInt();
            m_baseCardInf.np_list_show.push_back(cid);
        }
    }
    
    if (m_baseCardInf.np_need_num > 0
        && m_baseCardInf.np_list_target.size() > 0
        && m_baseCardInf.np_list_need.size() >= m_baseCardInf.np_need_num
        && m_baseCardInf.np_list_show.size() >= m_baseCardInf.np_need_num)
        m_baseCardInf.bIsNPTgt = true;
    else
        m_baseCardInf.bIsNPTgt = false;
    
    return &m_baseCardInf;
}

//******************************************************************************
// getSchUsrInf
//******************************************************************************
CFriendInf* CGameData::getSchUsrInf()
{
    if (HttpComm::Inst()->getSchFriendDic().empty())
        return NULL;
    
    Json::Value friendInfDic = HttpComm::Inst()->getSchFriendDic();
    
    if (friendInfDic.isMember("title_name"))
        strncpy(m_schUsrInf.title_name, friendInfDic["title_name"].asString().c_str(), 99);
    else
        strncpy(m_schUsrInf.title_name, "初出茅庐", 99);
    
    strncpy(m_schUsrInf.name, friendInfDic["name"].asString().c_str(), 99);
    
    strncpy(m_schUsrInf.fid, friendInfDic["fid"].asString().c_str(), 99);
    
    m_schUsrInf.user_elem = friendInfDic["country"].asInt();
    m_schUsrInf.lv = friendInfDic["lv"].asInt();
    
    Json::Value leader = friendInfDic["leader_card"];
    
    string strCid = leader["cid"].asString();
    if(strCid.empty())
        return NULL;

    m_schUsrInf.leadercard_cid = atoi(strCid.c_str());
    m_schUsrInf.leadercardlv = leader["lv"].asInt();
    m_schUsrInf.sk_lv = leader["sk_lv"].asInt();
    m_schUsrInf.max_friend_num = friendInfDic["max_friend_num"].asInt();
    m_schUsrInf.now_friend_num = friendInfDic["now_friend_num"].asInt();
    m_schUsrInf.login_time = 0;
    
    //card info
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }

    if (m_allBaseCardsDic.isMember(strCid) == false) {
        CCLog("!!! undefined card: %s", strCid.c_str());
        return NULL;
    }
    
    Json::Value card = m_allBaseCardsDic[strCid];
    
    strncpy(m_schUsrInf.card_info.name, card["name"].asString().c_str(), 99);
    
    m_schUsrInf.card_info.cid = m_schUsrInf.leadercard_cid;
    m_schUsrInf.card_info.category = atoi(card["category"].asString().c_str());
    m_schUsrInf.card_info.sub_category    = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
    
    m_schUsrInf.card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
    
    for (int z=0; z<MaxAwakeSkillCnt; z++)
        m_schUsrInf.card_info.AwakeSkillArr[z] = 0;
    
    if (card.isMember("awake_skid"))
    {
        Json::Value AwakeSkillArr = card["awake_skid"];
        for (int z=0; z<AwakeSkillArr.size(); z++)
        {
            if (z >= MaxAwakeSkillCnt)
                break;
            
            m_schUsrInf.card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
        }
    }
    
    m_schUsrInf.card_info.lv = m_schUsrInf.leadercardlv;
    m_schUsrInf.card_info.sk_lv = m_schUsrInf.sk_lv;
    m_schUsrInf.card_info.star = atoi(card["star"].asString().c_str());
    m_schUsrInf.card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
    m_schUsrInf.card_info.skid = atoi(card["skid"].asString().c_str());
    m_schUsrInf.card_info.ctype = atoi(card["ctype"].asString().c_str());
    m_schUsrInf.card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid ;
    
    m_schUsrInf.card_info.cost = card["cost"].asInt();
    m_schUsrInf.card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
    m_schUsrInf.card_info.max_lv  = card["max_lv"].asInt();
    
    m_schUsrInf.card_info.pl_attack = leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
    m_schUsrInf.card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
    m_schUsrInf.card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
    
    m_schUsrInf.card_info.plus = m_schUsrInf.card_info.pl_attack+m_schUsrInf.card_info.pl_hp+ m_schUsrInf.card_info.pl_recover;
    
    long recoverbase = card["recover"].asInt();
    float fRecoverGrowth = card["recover_growth"].asDouble();
    m_schUsrInf.card_info.recover = recoverbase + (m_schUsrInf.card_info.lv - 1) * fRecoverGrowth+m_schUsrInf.card_info.pl_recover*RECPLUSGROWTH;
    
    long hpbase = card["hp"].asInt();
    float fHpGrowth = card["hp_growth"].asDouble();
    m_schUsrInf.card_info.hp = hpbase + (m_schUsrInf.card_info.lv - 1) * fHpGrowth+m_schUsrInf.card_info.pl_hp*HPPLUSGROWTH;
    
    long attackbase = card["attack"].asInt();
    float fAtkGrowth = card["attack_growth"].asDouble();
    m_schUsrInf.card_info.attack = attackbase + (m_schUsrInf.card_info.lv - 1) * fAtkGrowth+m_schUsrInf.card_info.pl_attack*ATKPLUSGROWTH;
    
    if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
        m_schUsrInf.card_info.bIsEluTgt = true;
    }
    else{
        m_schUsrInf.card_info.bIsEluTgt = false;
    }
    
    if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
        m_schUsrInf.card_info.bIsWkTgt = true;
    }
    else{
        m_schUsrInf.card_info.bIsWkTgt = false;
    }
    
    return &m_schUsrInf;
}

CCMutableArray<CFriendInf*>* CGameData::getMemberApplyList()
{
    Json::Value reqArr = HttpComm::Inst()->getMemberApplyListDic();
    
    if (reqArr.empty())
        return 0;    
    
    m_pMemberApplyInfArr->removeAllObjects();

    for (int i = 0; i < reqArr.size(); i++) {
        Json::Value memberInfDic = reqArr[i];
        if (memberInfDic.isNull()) {
            break;
        }
        CFriendInf* pMemberInf = new CFriendInf();
        
        if (memberInfDic.isMember("title_name"))
            strncpy(pMemberInf->title_name, memberInfDic["title_name"].asString().c_str(), 99);
        else
            strncpy(pMemberInf->title_name, "初出茅庐", 99);
        
        
        strncpy(pMemberInf->name, memberInfDic["name"].asString().c_str(), 99);
        
        strncpy(pMemberInf->fid, memberInfDic["uid"].asString().c_str(), 99);
        
        pMemberInf->user_elem = memberInfDic["country"].asInt();
        pMemberInf->lv = memberInfDic["lv"].asInt();
        pMemberInf->login_time = 0;
        
        Json::Value leader = memberInfDic["leader_card"];
        if (leader.isNull()) {
            break;
        }
        string strCid = leader["cid"].asString();
        pMemberInf->leadercard_cid = atoi(strCid.c_str());
        pMemberInf->leadercardlv = leader["lv"].asInt();
        pMemberInf->sk_lv = leader["sk_lv"].asInt();
        
        //card info
        if(!strCid.empty()){
            if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
                m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
                m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
            }
            
            if(m_allBaseCardsDic.isMember(strCid) == false){
                CCLog("!!! undefined card: %s", strCid.c_str());
                continue;
            }
            
            Json::Value card = m_allBaseCardsDic[strCid];
            
            strncpy(pMemberInf->card_info.name, card["name"].asString().c_str(), 99);
            
            pMemberInf->card_info.cid = pMemberInf->leadercard_cid;
            pMemberInf->card_info.category = atoi(card["category"].asString().c_str());
            pMemberInf->card_info.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
            
            pMemberInf->card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
            
            for (int z=0; z<MaxAwakeSkillCnt; z++)
                pMemberInf->card_info.AwakeSkillArr[z] = 0;
            
            if (card.isMember("awake_skid"))
            {
                Json::Value AwakeSkillArr = card["awake_skid"];
                for (int z=0; z<AwakeSkillArr.size(); z++)
                {
                    if (z >= MaxAwakeSkillCnt)
                        break;
                    
                    pMemberInf->card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
                }
            }
            
            pMemberInf->card_info.lv = pMemberInf->leadercardlv;
            pMemberInf->card_info.sk_lv = pMemberInf->sk_lv;
            pMemberInf->card_info.star = atoi(card["star"].asString().c_str());
            pMemberInf->card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
            pMemberInf->card_info.skid = atoi(card["skid"].asString().c_str());
            pMemberInf->card_info.ctype = atoi(card["ctype"].asString().c_str());
            pMemberInf->card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid ;
            pMemberInf->card_info.cost = card["cost"].asInt();
            pMemberInf->card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
            pMemberInf->card_info.max_lv = card["max_lv"].asInt();
            
            pMemberInf->card_info.pl_attack = leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
            pMemberInf->card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
            pMemberInf->card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
            
            pMemberInf->card_info.plus = pMemberInf->card_info.pl_attack + pMemberInf->card_info.pl_hp + pMemberInf->card_info.pl_recover;
            
            //+1
            long recoverbase = card["recover"].asInt();
            float fRecoverGrowth = card["recover_growth"].asDouble();
            pMemberInf->card_info.recover = recoverbase + (pMemberInf->card_info.lv - 1) * fRecoverGrowth + pMemberInf->card_info.pl_recover*RECPLUSGROWTH;
            
            long hpbase = card["hp"].asInt();
            float fHpGrowth = card["hp_growth"].asDouble();
            pMemberInf->card_info.hp = hpbase + (pMemberInf->card_info.lv - 1) * fHpGrowth + pMemberInf->card_info.pl_hp*HPPLUSGROWTH;
            
            long attackbase = card["attack"].asInt();
            float fAtkGrowth = card["attack_growth"].asDouble();
            pMemberInf->card_info.attack = attackbase + (pMemberInf->card_info.lv - 1) * fAtkGrowth + pMemberInf->card_info.pl_attack*ATKPLUSGROWTH;
            
        }
        
        m_pMemberApplyInfArr->addObject(pMemberInf);
        pMemberInf->release();

    }

    return m_pMemberApplyInfArr;
}

//******************************************************************************
// getFriendsInf
//******************************************************************************
CCMutableArray<CFriendInf*> * CGameData::getInvitersInf()
{
    Json::Value reqArr;
    static long slFriApplyListObtainTm = 0;
    if (slFriApplyListObtainTm < HttpComm::Inst()->getDicTime(enDicFriendApplyList)) {
        slFriApplyListObtainTm = HttpComm::Inst()->getDicTime(enDicFriendApplyList);
        reqArr = HttpComm::Inst()->getReqListDic();
    }
    else
        return m_pFriendApplyInfArr;
    
    m_pFriendApplyInfArr->removeAllObjects();

    if(reqArr.empty())
        return 0;
    
    for(int i = 0; i < reqArr.size(); i++){
        Json::Value friendInfDic = reqArr[i];
        if (friendInfDic.isNull()) {
            break;
        }
        CFriendInf* pFriendInf = new CFriendInf();
        
        if (friendInfDic.isMember("title_name"))
            strncpy(pFriendInf->title_name, friendInfDic["title_name"].asString().c_str(), 99);
        else
            strncpy(pFriendInf->title_name, "初出茅庐", 99);
        
        strncpy(pFriendInf->name, friendInfDic["name"].asString().c_str(), 99);
        
        strncpy(pFriendInf->fid, friendInfDic["fid"].asString().c_str(), 99);
        
        pFriendInf->user_elem = friendInfDic["country"].asInt();
        pFriendInf->lv = friendInfDic["lv"].asInt();
        pFriendInf->login_time = 0;
        
        Json::Value leader = friendInfDic["leader_card"];
        if (leader.isNull()) {
            break;
        }
        string strCid = leader["cid"].asString();
        pFriendInf->leadercard_cid = atoi(strCid.c_str());
        pFriendInf->leadercardlv = leader["lv"].asInt();
        pFriendInf->sk_lv = leader["sk_lv"].asInt();

        //card info
        if(!strCid.empty()){
            if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
                m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
                m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
            }
            
            if(m_allBaseCardsDic.isMember(strCid) == false){
                CCLog("!!! undefined card: %s", strCid.c_str());
                continue;
            }

            Json::Value card = m_allBaseCardsDic[strCid];
            
            strncpy(pFriendInf->card_info.name, card["name"].asString().c_str(), 99);
            
            pFriendInf->card_info.cid = pFriendInf->leadercard_cid;
            pFriendInf->card_info.category = atoi(card["category"].asString().c_str());
            pFriendInf->card_info.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
            
            pFriendInf->card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
            
            for (int z=0; z<MaxAwakeSkillCnt; z++)
                pFriendInf->card_info.AwakeSkillArr[z] = 0;
            
            if (card.isMember("awake_skid"))
            {
                Json::Value AwakeSkillArr = card["awake_skid"];
                for (int z=0; z<AwakeSkillArr.size(); z++)
                {
                    if (z >= MaxAwakeSkillCnt)
                        break;
                    
                    pFriendInf->card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
                }
            }
            
            pFriendInf->card_info.cou_name = card.isMember("country") ? card["country"].asInt() : 0;
            pFriendInf->card_info.lv = pFriendInf->leadercardlv;
            pFriendInf->card_info.sk_lv = pFriendInf->sk_lv;
            pFriendInf->card_info.star = atoi(card["star"].asString().c_str());
            pFriendInf->card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
            pFriendInf->card_info.skid = atoi(card["skid"].asString().c_str());
            pFriendInf->card_info.ctype = atoi(card["ctype"].asString().c_str());
            pFriendInf->card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid ;
            pFriendInf->card_info.cost = card["cost"].asInt();
            pFriendInf->card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
            pFriendInf->card_info.max_lv = card["max_lv"].asInt();
            
            pFriendInf->card_info.pl_attack = leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
            pFriendInf->card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
            pFriendInf->card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
            
            pFriendInf->card_info.plus = pFriendInf->card_info.pl_attack+pFriendInf->card_info.pl_hp+ pFriendInf->card_info.pl_recover;
            
            //+1
            long recoverbase = card["recover"].asInt();
            float fRecoverGrowth = card["recover_growth"].asDouble();
            pFriendInf->card_info.recover = recoverbase + (pFriendInf->card_info.lv - 1) * fRecoverGrowth+pFriendInf->card_info.pl_recover*RECPLUSGROWTH;
            
            long hpbase = card["hp"].asInt();
            float fHpGrowth = card["hp_growth"].asDouble();
            pFriendInf->card_info.hp = hpbase + (pFriendInf->card_info.lv - 1) * fHpGrowth+pFriendInf->card_info.pl_hp*HPPLUSGROWTH;
            
            long attackbase = card["attack"].asInt();
            float fAtkGrowth = card["attack_growth"].asDouble();
            pFriendInf->card_info.attack = attackbase + (pFriendInf->card_info.lv - 1) * fAtkGrowth+pFriendInf->card_info.pl_attack*ATKPLUSGROWTH;
            
            if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
                pFriendInf->card_info.bIsEluTgt = true;
            }
            else{
                pFriendInf->card_info.bIsEluTgt = false;
            }
            
            if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
                pFriendInf->card_info.bIsWkTgt = true;
            }
            else{
                pFriendInf->card_info.bIsWkTgt = false;
            }
        }
        
        m_pFriendApplyInfArr->addObject(pFriendInf);
        pFriendInf->release();
    }
    
    return m_pFriendApplyInfArr;
}

//******************************************************************************
// changeMemberListRange
//******************************************************************************
void CGameData::changeMemberListRange(bool bChangeRange)
{
    getMemberList();
    
    if (bChangeRange)
    {
        m_memberRangeType = (enMemberRange)(m_memberRangeType + 1);
    }

    m_memberRangeType = (enMemberRange)(m_memberRangeType % enMemberRangeMax);
    if (m_memberRangeType == enMemberRangeStatus)
    {   // 按职位
        for(int i=0;i<m_pMemberInfArr->count()-1;i++)
        {
            for (int j=0;j< m_pMemberInfArr->count()-1-i; j++) {
                int lv1 = m_pMemberInfArr->getObjectAtIndex(j)->lv;
                int lv2 = m_pMemberInfArr->getObjectAtIndex(j+1)->lv;
                if(lv1 < lv2)
                {
                    CMember *tempMember1 = m_pMemberInfArr->getObjectAtIndex(j);
                    tempMember1->retain();
                    CMember *tempMember2 = m_pMemberInfArr->getObjectAtIndex(j+1);
                    m_pMemberInfArr->replaceObjectAtIndex(j, tempMember2);
                    m_pMemberInfArr->replaceObjectAtIndex(j+1, tempMember1);
                    tempMember1->release();
                }
            }
        }
        
        for (int i = 0; i < m_pMemberInfArr->count()-1; i++) {
            for (int j=0;j< m_pMemberInfArr->count()-1-i; j++) {
                int status1 = m_pMemberInfArr->getObjectAtIndex(j)->status;
                int status2 = m_pMemberInfArr->getObjectAtIndex(j+1)->status;
                if(status1 > status2)
                {
                    CMember *tempMember1 = m_pMemberInfArr->getObjectAtIndex(j);
                    tempMember1->retain();
                    CMember *tempMember2 = m_pMemberInfArr->getObjectAtIndex(j+1);
                    m_pMemberInfArr->replaceObjectAtIndex(j, tempMember2);
                    m_pMemberInfArr->replaceObjectAtIndex(j+1, tempMember1);
                    tempMember1->release();
                }
            }
        }
    }
    else if (m_memberRangeType == enMemberRangeLv)
    {   // 等级
        for(int i=0;i<m_pMemberInfArr->count()-1;i++)
        {
            for (int j=0;j< m_pMemberInfArr->count()-1-i; j++) {
                int lv1 = m_pMemberInfArr->getObjectAtIndex(j)->lv;
                int lv2 = m_pMemberInfArr->getObjectAtIndex(j+1)->lv;
                if(lv1 < lv2)
                {
                    CMember *tempMember1 = m_pMemberInfArr->getObjectAtIndex(j);
                    tempMember1->retain();
                    CMember *tempMember2 = m_pMemberInfArr->getObjectAtIndex(j+1);
                    m_pMemberInfArr->replaceObjectAtIndex(j, tempMember2);
                    m_pMemberInfArr->replaceObjectAtIndex(j+1, tempMember1);
                    tempMember1->release();
                }
            }
        }        
    }
    else if (m_memberRangeType == enMemberRangeCbt)
    {   // 贡献
        for(int i=0;i<m_pMemberInfArr->count()-1;i++)
        {
            for (int j=0;j< m_pMemberInfArr->count()-1-i; j++) {
                int contribution1 = m_pMemberInfArr->getObjectAtIndex(j)->contibution ;
                int contribution2 = m_pMemberInfArr->getObjectAtIndex(j+1)->contibution;
                if(contribution1 < contribution2)
                {
                    CMember *tempMember1 = m_pMemberInfArr->getObjectAtIndex(j);
                    tempMember1->retain();
                    CMember *tempMember2 = m_pMemberInfArr->getObjectAtIndex(j+1);
                    m_pMemberInfArr->replaceObjectAtIndex(j, tempMember2);
                    m_pMemberInfArr->replaceObjectAtIndex(j+1, tempMember1);
                    tempMember1->release();
                }
            }
        }
    }
    else if (m_memberRangeType == enMemberRangeLT)
    {   // 登陆时间
        for(int i=0;i<m_pMemberInfArr->count()-1;i++)
        {
            for (int j=0;j< m_pMemberInfArr->count()-1-i; j++) {
                int loginTime1 = m_pMemberInfArr->getObjectAtIndex(j)->login_time ;
                int loginTime2 = m_pMemberInfArr->getObjectAtIndex(j+1)->login_time;
                if(loginTime1 < loginTime2)
                {
                    CMember *tempMember1 = m_pMemberInfArr->getObjectAtIndex(j);
                    CMember *tempMember2 = m_pMemberInfArr->getObjectAtIndex(j+1);
                    tempMember1->retain();
                    m_pMemberInfArr->replaceObjectAtIndex(j, tempMember2);
                    m_pMemberInfArr->replaceObjectAtIndex(j+1, tempMember1);
                    tempMember1->release();
                }
            }
        }
    }    
}

//******************************************************************************
// getMemberList
//******************************************************************************
CCMutableArray<CMember*> *CGameData::getMemberList()
{
    static long slMemberListObtainTm = 0;
    if (slMemberListObtainTm < HttpComm::Inst()->getDicTime(enDicMemberList)) {
        slMemberListObtainTm = HttpComm::Inst()->getDicTime(enDicMemberList);
    }
    else {
        return m_pMemberInfArr;
    }
    
    Json::Value reqArr = HttpComm::Inst()->getMemberListDic();
    if(reqArr.empty())
        return 0;
        
    m_pMemberInfArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    for(int i = 0; i < reqArr.size(); i++){
        Json::Value memberInfoDic = reqArr[i];
        CMember* pMemberInfo = new CMember();
        
        if (memberInfoDic.isMember("title_name"))
            snprintf(pMemberInfo->title_name, 99, "%s", memberInfoDic["title_name"].asString().c_str());
        else
            snprintf(pMemberInfo->title_name, 99, "%s", "初出茅庐");
        
        snprintf(pMemberInfo->name, 99, "%s", memberInfoDic["name"].asString().c_str());
        snprintf(pMemberInfo->mid, 99, "%s", memberInfoDic["cid"].asString().c_str());
        
        pMemberInfo->status = memberInfoDic["guildrole"].asInt();
        pMemberInfo->bePingbi = memberInfoDic["is_screen"].asBool();
        pMemberInfo->VoiceOn = memberInfoDic["voice_status"].asBool();
        pMemberInfo->contibution = memberInfoDic["contribute"].asInt();
        pMemberInfo->bFriend = memberInfoDic["is_friend"].asBool();
        pMemberInfo->user_elem = memberInfoDic["country"].asInt();
        pMemberInfo->lv = memberInfoDic["lv"].asInt();
        pMemberInfo->login_time = memberInfoDic["login_time"].asInt();
        
        Json::Value leader = memberInfoDic["leader_card"];
        string strCid = leader["cid"].asString();
        pMemberInfo->card_info.cid = atoi(strCid.c_str());
        pMemberInfo->card_info.lv = leader["lv"].asInt();
        pMemberInfo->card_info.sk_lv = leader["sk_lv"].asInt();
        
        //card info
        if(m_allBaseCardsDic.isMember(strCid) == false){
            CCLog("!!! undefined card: %s", strCid.c_str());
            continue;
        }
        
        Json::Value card = m_allBaseCardsDic[strCid];
        
        strncpy(pMemberInfo->card_info.name, card["name"].asString().c_str(), 99);
        pMemberInfo->card_info.category = atoi(card["category"].asString().c_str());
        pMemberInfo->card_info.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
        
        pMemberInfo->card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
        
        for (int z=0; z<MaxAwakeSkillCnt; z++)
            pMemberInfo->card_info.AwakeSkillArr[z] = 0;
        
        if (card.isMember("awake_skid"))
        {
            Json::Value AwakeSkillArr = card["awake_skid"];
            for (int z=0; z<AwakeSkillArr.size(); z++)
            {
                if (z >= MaxAwakeSkillCnt)
                    break;
                
                pMemberInfo->card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
            }
        }
        
        pMemberInfo->card_info.cou_name = card.isMember("country") ? card["country"].asInt() : 0;
        pMemberInfo->card_info.star = atoi(card["star"].asString().c_str());
        pMemberInfo->card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
        pMemberInfo->card_info.skid = atoi(card["skid"].asString().c_str());
        pMemberInfo->card_info.ctype = atoi(card["ctype"].asString().c_str());
        pMemberInfo->card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid;
        pMemberInfo->card_info.cost = card["cost"].asInt();
        pMemberInfo->card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
        pMemberInfo->card_info.max_lv = card["max_lv"].asInt();
        
        pMemberInfo->card_info.pl_attack =leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
        pMemberInfo->card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
        pMemberInfo->card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
        
        pMemberInfo->card_info.plus = pMemberInfo->card_info.pl_attack + pMemberInfo->card_info.pl_hp + pMemberInfo->card_info.pl_recover;
        
        long recoverbase = card["recover"].asInt();
        float fRecoverGrowth = card["recover_growth"].asDouble();
        pMemberInfo->card_info.recover = recoverbase + (pMemberInfo->card_info.lv - 1)*fRecoverGrowth + pMemberInfo->card_info.pl_recover*RECPLUSGROWTH;
        
        long hpbase = card["hp"].asInt();
        float fHpGrowth = card["hp_growth"].asDouble();
        pMemberInfo->card_info.hp = hpbase + (pMemberInfo->card_info.lv - 1)*fHpGrowth + pMemberInfo->card_info.pl_hp*HPPLUSGROWTH;
        
        long attackbase = card["attack"].asInt();
        float fAtkGrowth = card["attack_growth"].asDouble();
        pMemberInfo->card_info.attack = attackbase + (pMemberInfo->card_info.lv - 1)*fAtkGrowth + pMemberInfo->card_info.pl_attack*ATKPLUSGROWTH;
        
        m_pMemberInfArr->addObject(pMemberInfo);
        pMemberInfo->release();
    }

    return m_pMemberInfArr;
}

bool sortSign(RwdSignInf *inf1,RwdSignInf *inf2)
{
    if (inf1 && inf2)
    {
        return inf1->iDays < inf2->iDays;
    }
    return false;
}

//签到活动奖励
CCMutableArray<RwdSignInf*> *CGameData::getRwdSignInfList()
{
    m_pRwdSignInfArr->removeAllObjects();
    
    Json::Value signDic = HttpComm::Inst()->getAwardSignDic();
    
    if (!signDic.empty()){
    
    Json::Value::Members memberArr = signDic.getMemberNames();
    for (int i=0; i<signDic.size(); i++)
    {
        RwdSignInf* pSignInfo = new RwdSignInf();
        pSignInfo->strDate = memberArr[i];
        pSignInfo->iDays = signDic[memberArr[i]]["days"].asInt();
        pSignInfo->isAward = signDic[memberArr[i]].isMember("is_award") ? signDic[memberArr[i]]["is_award"].asBool():true;
        
        Json::Value jawardDic = signDic[memberArr[i]]["award"];
        pSignInfo->awardDsp = jawardDic.isMember("award_dsp") ? jawardDic["award_dsp"].asString() : "";
        pSignInfo->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
        pSignInfo->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
        pSignInfo->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
        pSignInfo->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
        
        if (jawardDic.isMember("hide")) {
            Json::Value hideArr = jawardDic["hide"];
            
            for ( int j =0; j< hideArr.size(); j++) {
                pSignInfo->hideInf.push_back(hideArr[j].asString());
            }
        }
        
        if (jawardDic.isMember("card"))
        {
            Json::Value jcardDic = jawardDic["card"];
            Json::Value::Members cardNames = jcardDic.getMemberNames();
            
            for (int j=0; j<jcardDic.size(); j++)
            {
                RwdWuJiangCard stWuJiangCard;
                stWuJiangCard.strCardId = cardNames[j];
                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                pSignInfo->WujiangCardList.push_back(stWuJiangCard);
            }
        }
        
        if (jawardDic.isMember("guild_item"))
        {
            Json::Value jguildItemDic = jawardDic["guild_item"];
            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
            
            for (int j=0; j<jguildItemDic.size(); j++)
            {
                RwdItemCard stItemCard;
                stItemCard.strCardId = guildItemNames[j];
                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                pSignInfo->ItemCardList.push_back(stItemCard);
            }
        }

        m_pRwdSignInfArr->addObject(pSignInfo);
        pSignInfo->release();
    }
    }
    CCMutableArray<RwdSignInf*>::sortArray(m_pRwdSignInfArr,sortSign);
    
    //明日奖励
    signDic = HttpComm::Inst()->getAwardSignTomorrowDic();
    
    if (signDic.empty())
        return m_pRwdSignInfArr;
    
    Json::Value::Members memberArr = signDic.getMemberNames();
    for (int i=0; i<signDic.size(); i++)
    {
        RwdSignInf* pSignInfo = new RwdSignInf();
        pSignInfo->strDate = memberArr[i];
        pSignInfo->iDays = signDic[memberArr[i]]["days"].asInt();
        pSignInfo->isAward = signDic[memberArr[i]].isMember("is_award") ? signDic[memberArr[i]]["is_award"].asBool():true;
        
        Json::Value jawardDic = signDic[memberArr[i]]["award"];
        pSignInfo->awardDsp = jawardDic.isMember("award_dsp") ? jawardDic["award_dsp"].asString() : "";
        pSignInfo->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
        pSignInfo->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
        pSignInfo->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
        pSignInfo->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
        
        if (jawardDic.isMember("hide")) {
            Json::Value hideArr = jawardDic["hide"];
            
            for ( int j =0; j< hideArr.size(); j++) {
                pSignInfo->hideInf.push_back(hideArr[j].asString());
            }
        }
        
        if (jawardDic.isMember("card"))
        {
            Json::Value jcardDic = jawardDic["card"];
            Json::Value::Members cardNames = jcardDic.getMemberNames();
            
            for (int j=0; j<jcardDic.size(); j++)
            {
                RwdWuJiangCard stWuJiangCard;
                stWuJiangCard.strCardId = cardNames[j];
                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                pSignInfo->WujiangCardList.push_back(stWuJiangCard);
            }
        }
        
        if (jawardDic.isMember("guild_item"))
        {
            Json::Value jguildItemDic = jawardDic["guild_item"];
            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
            
            for (int j=0; j<jguildItemDic.size(); j++)
            {
                RwdItemCard stItemCard;
                stItemCard.strCardId = guildItemNames[j];
                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                pSignInfo->ItemCardList.push_back(stItemCard);
            }
        }
        
        m_pRwdSignInfArr->addObject(pSignInfo);
        pSignInfo->release();
    }
    
    
    return m_pRwdSignInfArr;
}

bool sortLv(RwdLevelUpInf *inf1,RwdLevelUpInf *inf2)
{
    if (inf1 && inf2)
    {
        return inf1->lv < inf2->lv;
    }
    return false;
}

//升级活动奖励
CCMutableArray<RwdLevelUpInf*> * CGameData::getRwdLevelUpArr()
{
    m_pRwdLevelUpInfArr->removeAllObjects();
    
    Json::Value levelUpDic = HttpComm::Inst()->getAwardLevelUpDic();
    if (levelUpDic.empty())
        return m_pRwdLevelUpInfArr;
    
    /*bool isOpen = levelUpDic["is_open"].asBool();
    if (!isOpen)
        return m_pRwdLevelUpInfArr;
    
    Json::Value lvDic = levelUpDic["lv"];
    Json::Value::Members memberArr = lvDic.getMemberNames();*/
    Json::Value::Members memberArr = levelUpDic.getMemberNames();
    for (int i=0; i<levelUpDic.size(); i++)
    {
        RwdLevelUpInf* pLevelUpInfo = new RwdLevelUpInf();
        pLevelUpInfo->lv = atoi(memberArr[i].c_str());
        pLevelUpInfo->showLv = levelUpDic[memberArr[i]]["show_lv"].asInt();
        pLevelUpInfo->showDate = levelUpDic[memberArr[i]]["show_date"].asString();
        pLevelUpInfo->isAward = levelUpDic[memberArr[i]].isMember("is_award") ? levelUpDic[memberArr[i]]["is_award"].asBool():true;
        pLevelUpInfo->content = levelUpDic[memberArr[i]].isMember("content") ? levelUpDic[memberArr[i]]["content"].asString():"";
        
        
        Json::Value jawardDic = levelUpDic[memberArr[i]]["award"];
        pLevelUpInfo->awardDsp = jawardDic.isMember("award_dsp") ? jawardDic["award_dsp"].asString() : "";
        
        pLevelUpInfo->needShare = jawardDic.isMember("is_share") ? jawardDic["is_share"].asBool():false;
        
        pLevelUpInfo->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
        pLevelUpInfo->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
        pLevelUpInfo->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
        pLevelUpInfo->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;

        if (jawardDic.isMember("card"))
        {
            Json::Value jcardDic = jawardDic["card"];
            Json::Value::Members cardNames = jcardDic.getMemberNames();
            
            for (int j=0; j<jcardDic.size(); j++)
            {
                RwdWuJiangCard stWuJiangCard;
                stWuJiangCard.strCardId = cardNames[j];
                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                pLevelUpInfo->WujiangCardList.push_back(stWuJiangCard);
            }
        }
        
        if (jawardDic.isMember("guild_item"))
        {
            Json::Value jguildItemDic = jawardDic["guild_item"];
            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
            
            for (int j=0; j<jguildItemDic.size(); j++)
            {
                RwdItemCard stItemCard;
                stItemCard.strCardId = guildItemNames[j];
                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                pLevelUpInfo->ItemCardList.push_back(stItemCard);
            }
        }
        
        m_pRwdLevelUpInfArr->addObject(pLevelUpInfo);
        pLevelUpInfo->release();
    }
    CCMutableArray<RwdLevelUpInf*>::sortArray(m_pRwdLevelUpInfArr,sortLv);
    return m_pRwdLevelUpInfArr;
}

// 月份活动奖励
CCMutableArray<RwdMonthInf*> * CGameData::getRwdMonthInfoArr()
{
    m_pRwdMonthInArr->removeAllObjects();
    
    Json::Value awardInfDic = HttpComm::Inst()->getAwardMonthDic();
    if (awardInfDic.empty())
        return m_pRwdMonthInArr;
    
    Json::Value::Members memberArr = awardInfDic.getMemberNames();
    Json::Value monthDic = awardInfDic[memberArr[0]];
    Json::Value::Members memberArr1 = monthDic.getMemberNames();
    
    for (int i=0; i<monthDic.size(); i++)
    {
        RwdMonthInf* info = new RwdMonthInf();
        info->strDate = memberArr[0];
        info->day = atoi(memberArr1[i].c_str());
        info->isAward = monthDic[memberArr1[i]].isMember("is_award") ? monthDic[memberArr1[i]]["is_award"].asBool():true;
        info->lDays = monthDic[memberArr1[i]].isMember("cur_login") ? monthDic[memberArr1[i]]["cur_login"].asInt():1;
        info->content = monthDic[memberArr1[i]].isMember("content") ? monthDic[memberArr1[i]]["content"].asString():"";
        
        info->hide = monthDic[memberArr1[i]].isMember("hide") ? monthDic[memberArr1[i]]["hide"].asBool():false;
        
        Json::Value award = monthDic[memberArr1[i]]["award"];
        
        if (award.isMember("card"))
        {
            Json::Value jcardDic = award["card"];
            Json::Value::Members cardNames = jcardDic.getMemberNames();
            
            for (int j=0; j<jcardDic.size(); j++)
            {
                RwdWuJiangCard stWuJiangCard;
                stWuJiangCard.strCardId = cardNames[j];
                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                info->WujiangCardList.push_back(stWuJiangCard);
            }
        }
        
        if (award.isMember("guild_item"))
        {
            Json::Value jguildItemDic = award["guild_item"];
            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
            
            for (int j=0; j<jguildItemDic.size(); j++)
            {
                RwdItemCard stItemCard;
                stItemCard.strCardId = guildItemNames[j];
                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                info->ItemCardList.push_back(stItemCard);
            }
        }
        
        m_pRwdMonthInArr->addObject(info);
    }
    return m_pRwdMonthInArr;
}

int CGameData::getRwdMonthDay()
{
    int rt = 0;
    
    Json::Value js = HttpComm::Inst()->getAwardMonthDic();
    if (js.isMember("month_login"))
    {
        rt = js["month_login"].asInt();
    }
    return rt;
}

bool sortDays(RwdBonusInf *inf1,RwdBonusInf *inf2)
{
    if (inf1 && inf2)
    {
        return inf1->rewardDays < inf2->rewardDays;
    }
    return false;
}

//累积天数奖励
CCMutableArray<RwdBonusInf*> * CGameData::getRwdBonusInfArr()
{
    m_pRwdBonusInfArr->removeAllObjects();
    
    Json::Value bonusDic = HttpComm::Inst()->getAwardBonusDic();
    if (bonusDic.empty())
        return m_pRwdBonusInfArr;
    
    Json::Value::Members memberArr = bonusDic.getMemberNames();
    for (int i=0; i<bonusDic.size(); i++)
    {
        RwdBonusInf* pBonusInf = new RwdBonusInf();
        pBonusInf->rewardDays = atoi(memberArr[i].c_str());
        pBonusInf->iDays = bonusDic[memberArr[i]]["login_days"].asInt();
        pBonusInf->isAward = bonusDic[memberArr[i]].isMember("is_award") ? bonusDic[memberArr[i]]["is_award"].asBool():true;
        
        Json::Value jawardDic = bonusDic[memberArr[i]]["award"];
        if (jawardDic.isMember("hide")) {
            Json::Value hideArr = jawardDic["hide"];
            
            for ( int j =0; j< hideArr.size(); j++) {
                pBonusInf->hideInf.push_back(hideArr[j].asString());
            }
        }
        
        pBonusInf->awardDsp = jawardDic.isMember("award_dsp") ? jawardDic["award_dsp"].asString() : "";
        
        pBonusInf->needShare = jawardDic.isMember("is_share") ? jawardDic["is_share"].asBool():false;
        
        pBonusInf->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
        
        pBonusInf->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
        pBonusInf->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
        pBonusInf->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
        pBonusInf->isAward = jawardDic.isMember("is_award") ? jawardDic["is_award"].asBool():true;

        
        if (jawardDic.isMember("card"))
        {
            Json::Value jcardDic = jawardDic["card"];
            Json::Value::Members cardNames = jcardDic.getMemberNames();
            
            for (int j=0; j<jcardDic.size(); j++)
            {
                RwdWuJiangCard stWuJiangCard;
                stWuJiangCard.strCardId = cardNames[j];
                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                pBonusInf->WujiangCardList.push_back(stWuJiangCard);
            }
        }
        
        if (jawardDic.isMember("guild_item"))
        {
            Json::Value jguildItemDic = jawardDic["guild_item"];
            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
            
            for (int j=0; j<jguildItemDic.size(); j++)
            {
                RwdItemCard stItemCard;
                stItemCard.strCardId = guildItemNames[j];
                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                pBonusInf->ItemCardList.push_back(stItemCard);
            }
        }
        
        m_pRwdBonusInfArr->addObject(pBonusInf);
        pBonusInf->release();
    }
    
    CCMutableArray<RwdBonusInf*>::sortArray(m_pRwdBonusInfArr,sortDays);
    
    return m_pRwdBonusInfArr;
}


CCMutableArray<RwdBuchangInf*> * CGameData::getRwdBuchangInfArr()
{
    {
        m_pRwdBuchangInfArr->removeAllObjects();
        
        Json::Value bonusDic = HttpComm::Inst()->getAwardBuchangDic();
        
        if (bonusDic.empty())
        {
            return m_pRwdBuchangInfArr;
        }
        
        Json::Value::Members memberArr = bonusDic.getMemberNames();
        for (int i=0; i<bonusDic.size(); i++)
        {
            RwdBuchangInf* pBuchangInf = new RwdBuchangInf();
            pBuchangInf->strDate = memberArr[i];
            pBuchangInf->strContent = bonusDic[memberArr[i]].isMember("content")? bonusDic[memberArr[i]]["content"].asString() : "";
            pBuchangInf->bCanGet = bonusDic[memberArr[i]].isMember("is_award")? bonusDic[memberArr[i]]["is_award"].asBool() : false;
            
            Json::Value jawardDic = bonusDic[memberArr[i]]["award"];
            pBuchangInf->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
            pBuchangInf->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
            pBuchangInf->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
            pBuchangInf->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
            pBuchangInf->isAward = jawardDic.isMember("is_award") ? jawardDic["is_award"].asBool():true;

            
            if (jawardDic.isMember("card"))
            {
                Json::Value jcardDic = jawardDic["card"];
                Json::Value::Members cardNames = jcardDic.getMemberNames();
                
                for (int j=0; j<jcardDic.size(); j++)
                {
                    RwdWuJiangCard stWuJiangCard;
                    stWuJiangCard.strCardId = cardNames[j];
                    stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
                    pBuchangInf->WujiangCardList.push_back(stWuJiangCard);
                }
            }
            
            if (jawardDic.isMember("guild_item"))
            {
                Json::Value jguildItemDic = jawardDic["guild_item"];
                Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
                
                for (int j=0; j<jguildItemDic.size(); j++)
                {
                    RwdItemCard stItemCard;
                    stItemCard.strCardId = guildItemNames[j];
                    stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
                    pBuchangInf->ItemCardList.push_back(stItemCard);
                }
            }
            
            m_pRwdBuchangInfArr->addObject(pBuchangInf);
            pBuchangInf->release();
        }
    }
    
    return m_pRwdBuchangInfArr;
}

//活动奖励列表
RwdHuodongInf* CGameData::getRwdAwardInfo(Json::Value jawardDic,RwdHuodongInf* pHuodongInf){
    pHuodongInf->awardDsp = jawardDic.isMember("award_dsp") ? jawardDic["award_dsp"].asString() : "";
    pHuodongInf->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
    pHuodongInf->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
    pHuodongInf->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
    pHuodongInf->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
    pHuodongInf->isAward = jawardDic.isMember("is_award") ? jawardDic["is_award"].asBool():true;
    
    
    if (jawardDic.isMember("card"))
    {
        Json::Value jcardDic = jawardDic["card"];
        Json::Value::Members cardNames = jcardDic.getMemberNames();
        
        for (int j=0; j<jcardDic.size(); j++)
        {
            RwdWuJiangCard stWuJiangCard;
            stWuJiangCard.strCardId = cardNames[j];
            stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
            pHuodongInf->WujiangCardList.push_back(stWuJiangCard);
        }
    }
    
    if (jawardDic.isMember("guild_item"))
    {
        Json::Value jguildItemDic = jawardDic["guild_item"];
        Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
        
        for (int j=0; j<jguildItemDic.size(); j++)
        {
            RwdItemCard stItemCard;
            stItemCard.strCardId = guildItemNames[j];
            stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
            pHuodongInf->ItemCardList.push_back(stItemCard);
        }
    }
    return pHuodongInf;
}

bool sortSys(RwdHuodongInf *inf1,RwdHuodongInf *inf2)
{
    if (inf1 && inf2)
    {
        return !inf1->bCanGet;
    }
    return false;
}

CCMutableArray<RwdHuodongInf*> * CGameData::getRwdHuodongInfArr()
{
    m_pRwdHuodongInfArr->removeAllObjects();
    CCMutableArray<RwdHuodongInf*> *pRwdHuodongInfArr = new CCMutableArray<RwdHuodongInf*>();
    pRwdHuodongInfArr->removeAllObjects();
    
    Json::Value bonusDic = HttpComm::Inst()->getAwardHuodongDic();
    
    if (!bonusDic.empty())
    {
        Json::Value::Members memberArr = bonusDic.getMemberNames();
        for (int i=0; i<bonusDic.size(); i++){
            Json::Value jawardDic;
            if (bonusDic[memberArr[i]].isMember("award")) {
                RwdHuodongInf* pHuodongInf = new RwdHuodongInf();
                pHuodongInf->awardType = "3";
                pHuodongInf->strDate = memberArr[i];
                pHuodongInf->strContent = bonusDic[memberArr[i]].isMember("content")? bonusDic[memberArr[i]]["content"].asString() : "";
                pHuodongInf->bCanGet = bonusDic[memberArr[i]].isMember("is_award")? bonusDic[memberArr[i]]["is_award"].asBool() : false;
                jawardDic = bonusDic[memberArr[i]]["award"];
                
                pHuodongInf->awardDays = 0;
                pHuodongInf = getRwdAwardInfo(jawardDic,pHuodongInf);
                 pRwdHuodongInfArr->addObject(pHuodongInf);
               // m_pRwdHuodongInfArr->addObject(pHuodongInf);
                pHuodongInf->release();
            }else{
                Json::Value dayDic = bonusDic[memberArr[i]];
                Json::Value::Members memberArr2 = dayDic.getMemberNames();
                for(int j=0;j<dayDic.size();j++){
                    RwdHuodongInf* pHuodongInf = new RwdHuodongInf();
                    pHuodongInf->awardType = "3";
                    pHuodongInf->strDate = memberArr[i];
                    pHuodongInf->strContent = dayDic[memberArr2[j]].isMember("content")? dayDic[memberArr2[j]]["content"].asString() : "";
                    pHuodongInf->awardDays = atoi(memberArr2[j].c_str());
                    pHuodongInf->loginCounts = dayDic[memberArr2[j]].isMember("login_count")? dayDic[memberArr2[j]]["login_count"].asInt() : 0;
                    pHuodongInf->bCanGet = dayDic[memberArr2[j]].isMember("is_award")? dayDic[memberArr2[j]]["is_award"].asBool() : false;
                    Json::Value jawardDic = dayDic[memberArr2[j]]["award"];
                    pHuodongInf = getRwdAwardInfo(jawardDic, pHuodongInf);
                    //m_pRwdHuodongInfArr->addObject(pHuodongInf);
                     pRwdHuodongInfArr->addObject(pHuodongInf);
                    pHuodongInf->release();
                    
                }
                
            }
        }
        
    }
    
    bonusDic = HttpComm::Inst()->getAwardBuchangDic();
    
    if (bonusDic.empty())
    {
        for (int ii = 0; ii < pRwdHuodongInfArr->count(); ii++) {
            RwdHuodongInf* pHuodongInf = (RwdHuodongInf*)(pRwdHuodongInfArr->getObjectAtIndex(ii));
            if (!(pHuodongInf->bCanGet)) {
                m_pRwdHuodongInfArr->addObject(pHuodongInf);
            }
        }
        for (int ii = 0; ii < pRwdHuodongInfArr->count(); ii++) {
            RwdHuodongInf* pHuodongInf = (RwdHuodongInf*)(pRwdHuodongInfArr->getObjectAtIndex(ii));
            if (pHuodongInf->bCanGet) {
                m_pRwdHuodongInfArr->addObject(pHuodongInf);
            }
        }
        pRwdHuodongInfArr->removeAllObjects();
        delete pRwdHuodongInfArr;
        return m_pRwdHuodongInfArr;
    }
  
    
    Json::Value::Members memberArr = bonusDic.getMemberNames();
    for (int i=0; i<bonusDic.size(); i++){
        Json::Value jawardDic;
        if (bonusDic[memberArr[i]].isMember("award")) {
            RwdHuodongInf* pHuodongInf = new RwdHuodongInf();
            pHuodongInf->awardType = "2";
            pHuodongInf->strDate = memberArr[i];
            pHuodongInf->strContent = bonusDic[memberArr[i]].isMember("content")? bonusDic[memberArr[i]]["content"].asString() : "";
            pHuodongInf->bCanGet = bonusDic[memberArr[i]].isMember("is_award")? bonusDic[memberArr[i]]["is_award"].asBool() : false;
            jawardDic = bonusDic[memberArr[i]]["award"];
            
            pHuodongInf->awardDays = 0;
            pHuodongInf = getRwdAwardInfo(jawardDic,pHuodongInf);
            
            pRwdHuodongInfArr->addObject(pHuodongInf);
            pHuodongInf->release();
        }else{
            Json::Value dayDic = bonusDic[memberArr[i]];
            Json::Value::Members memberArr2 = dayDic.getMemberNames();
            for(int j=0;j<dayDic.size();j++){
                RwdHuodongInf* pHuodongInf = new RwdHuodongInf();
                pHuodongInf->awardType = "2";
                pHuodongInf->strDate = memberArr[i];
                pHuodongInf->strContent = dayDic[memberArr2[j]].isMember("content")? dayDic[memberArr2[j]]["content"].asString() : "";
                pHuodongInf->awardDays = atoi(memberArr2[j].c_str());
                pHuodongInf->loginCounts = dayDic[memberArr2[j]].isMember("login_count")? dayDic[memberArr2[j]]["login_count"].asInt() : 0;
                pHuodongInf->bCanGet = dayDic[memberArr2[j]].isMember("is_award")? dayDic[memberArr2[j]]["is_award"].asBool() : false;
                Json::Value jawardDic = dayDic[memberArr2[j]]["award"];
                pHuodongInf = getRwdAwardInfo(jawardDic, pHuodongInf);
                pRwdHuodongInfArr->addObject(pHuodongInf);
                pHuodongInf->release();
                
            }
        }
    }
 //   CCMutableArray<RwdHuodongInf*>::sortArray(m_pRwdHuodongInfArr,sortSys);
    for (int ii = 0; ii < pRwdHuodongInfArr->count(); ii++) {
        RwdHuodongInf* pHuodongInf = (RwdHuodongInf*)(pRwdHuodongInfArr->getObjectAtIndex(ii));
        if (!(pHuodongInf->bCanGet)) {
            m_pRwdHuodongInfArr->addObject(pHuodongInf);
        }
    }
    for (int ii = 0; ii < pRwdHuodongInfArr->count(); ii++) {
        RwdHuodongInf* pHuodongInf = (RwdHuodongInf*)(pRwdHuodongInfArr->getObjectAtIndex(ii));
        if (pHuodongInf->bCanGet) {
            m_pRwdHuodongInfArr->addObject(pHuodongInf);
        }
    }
    pRwdHuodongInfArr->removeAllObjects();
    delete pRwdHuodongInfArr;
//    Json::Value awardConfig = HttpComm::Inst()->getAwardConfigDic();
//    if (!awardConfig.isMember("system_bonus"))
//        return m_pRwdHuodongInfArr;
//
//    Json::Value bonusDic2 = awardConfig["system_bonus"];
//    Json::Value::Members memberArr = bonusDic2.getMemberNames();
//    for (int i=0; i<bonusDic2.size(); i++)
//    {
////        long lCurTime = TimeTransform::currentTime();
////        long curTime2 = getCurrentTime();
////        
////        tm* time = new gmtime(&lCurTime);
////        
////        string strDate2 = string("2013-08-08"); //(可以变换成"20130808")
////        long lTime = atol(strDate2.c_str());
////        //lCurTime =  (lCurTime / 3600 + 8) / 24;
//        
//        RwdHuodongInf* pHuodongInf = new RwdHuodongInf();
//        pHuodongInf->strDate = memberArr[i];
//        pHuodongInf->strContent = bonusDic2[memberArr[i]].isMember("content")? bonusDic2[memberArr[i]]["content"].asString() : "";
//        
//        Json::Value jawardDic = bonusDic2[memberArr[i]]["award"];
//        pHuodongInf->lCoin = jawardDic.isMember("coin") ? jawardDic["coin"].asInt() : 0;
//        pHuodongInf->lGold = jawardDic.isMember("gold") ? jawardDic["gold"].asInt() : 0;
//        pHuodongInf->lGacha_pt = jawardDic.isMember("gacha_pt") ? jawardDic["gacha_pt"].asInt() : 0;
//        pHuodongInf->lWealth = jawardDic.isMember("wealth") ? jawardDic["wealth"].asInt() : 0;
//        
//        if (jawardDic.isMember("card"))
//        {
//            Json::Value jcardDic = jawardDic["card"];
//            Json::Value::Members cardNames = jcardDic.getMemberNames();
//            
//            for (int j=0; j<jcardDic.size(); j++)
//            {
//                RwdWuJiangCard stWuJiangCard;
//                stWuJiangCard.strCardId = cardNames[j];
//                stWuJiangCard.lv = jcardDic[cardNames[j]].isMember("lv") ? jcardDic[cardNames[j]]["lv"].asInt() : 1;
//                pHuodongInf->WujiangCardList.push_back(stWuJiangCard);
//            }
//        }
//        
//        if (jawardDic.isMember("guild_item"))
//        {
//            Json::Value jguildItemDic = jawardDic["guild_item"];
//            Json::Value::Members guildItemNames = jguildItemDic.getMemberNames();
//            
//            for (int j=0; j<jguildItemDic.size(); j++)
//            {
//                RwdItemCard stItemCard;
//                stItemCard.strCardId = guildItemNames[j];
//                stItemCard.num = jguildItemDic[guildItemNames[j]].isMember("num") ? jguildItemDic[guildItemNames[j]]["num"].asInt() : 1;
//                pHuodongInf->ItemCardList.push_back(stItemCard);
//            }
//        }
//        
//        pHuodongInf->bCanGetToday = false;
//        m_pRwdHuodongInfArr->addObject(pHuodongInf);
//        pHuodongInf->release();
//    }

    return m_pRwdHuodongInfArr;
}


//******************************************************************************
// getRewardMemberList
//******************************************************************************
CCMutableArray<CRewardMember*> *CGameData::getRewardMemberList()
{    
    Json::Value reqArr = HttpComm::Inst()->getRewardMemberDic();
    if(reqArr.empty())
        return 0;
    
    m_pRewardMemberArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    int targetContribution = 0;
    for (int i = 0; i < m_pRewardTaskArr->count(); i++) {
        CPostRewardBaseInfo* rewardInfo = m_pRewardTaskArr->getObjectAtIndex(i);
        if (rewardInfo->name == m_selectedTaskId)
        {
            targetContribution = rewardInfo->contribution;
            break;
        }
    }
    
    for(int i = 0; i < reqArr.size(); i++){
        Json::Value memberInfoDic = reqArr[i];
        CRewardMember* pMemberInfo = new CRewardMember();
        
        pMemberInfo->done_contribution = memberInfoDic["contribution"].asInt();
        pMemberInfo->bRecieveReward = memberInfoDic["is_award"].asBool();
        pMemberInfo->target_contribution = targetContribution;
        pMemberInfo->bFinishReward = pMemberInfo->done_contribution >= pMemberInfo->target_contribution ? true : false;
        
        if (memberInfoDic.isMember("title_name"))
            snprintf(pMemberInfo->title_name, 99, "%s", memberInfoDic["title_name"].asString().c_str());
        else
            snprintf(pMemberInfo->title_name, 99, "%s", "初出茅庐");
        
        snprintf(pMemberInfo->name, 99, "%s", memberInfoDic["name"].asString().c_str());
        snprintf(pMemberInfo->mid, 99, "%s", memberInfoDic["cid"].asString().c_str());
        
        pMemberInfo->status = memberInfoDic["guildrole"].asInt();
        pMemberInfo->bePingbi = memberInfoDic["is_screen"].asBool();
        pMemberInfo->VoiceOn = memberInfoDic["voice_status"].asBool();
        pMemberInfo->contibution = memberInfoDic["contribute"].asInt();
        pMemberInfo->bFriend = memberInfoDic["is_friend"].asBool();
        pMemberInfo->user_elem = memberInfoDic["country"].asInt();
        pMemberInfo->lv = memberInfoDic["lv"].asInt();
        pMemberInfo->login_time = memberInfoDic["login_time"].asInt();
        
        Json::Value leader = memberInfoDic["leader_card"];
        string strCid = leader["cid"].asString();
        pMemberInfo->card_info.cid = atoi(strCid.c_str());
        pMemberInfo->card_info.lv = leader["lv"].asInt();
        pMemberInfo->card_info.sk_lv = leader["sk_lv"].asInt();
        
        //card info
        if(m_allBaseCardsDic.isMember(strCid) == false){
            CCLog("!!! undefined card: %s", strCid.c_str());
            continue;
        }
        
        Json::Value card = m_allBaseCardsDic[strCid];
        
        strncpy(pMemberInfo->card_info.name, card["name"].asString().c_str(), 99);
        pMemberInfo->card_info.category = atoi(card["category"].asString().c_str());
        pMemberInfo->card_info.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
        
        pMemberInfo->card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
        
        for (int z=0; z<MaxAwakeSkillCnt; z++)
            pMemberInfo->card_info.AwakeSkillArr[z] = 0;
        
        if (card.isMember("awake_skid"))
        {
            Json::Value AwakeSkillArr = card["awake_skid"];
            for (int z=0; z<AwakeSkillArr.size(); z++)
            {
                if (z >= MaxAwakeSkillCnt)
                    break;
                
                pMemberInfo->card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
            }
        }
        
        pMemberInfo->card_info.star = atoi(card["star"].asString().c_str());
        pMemberInfo->card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
        pMemberInfo->card_info.skid = atoi(card["skid"].asString().c_str());
        pMemberInfo->card_info.ctype = atoi(card["ctype"].asString().c_str());
        pMemberInfo->card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid;
        pMemberInfo->card_info.cost = card["cost"].asInt();
        pMemberInfo->card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
        pMemberInfo->card_info.max_lv = card["max_lv"].asInt();
        
        pMemberInfo->card_info.pl_attack =leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
        pMemberInfo->card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
        pMemberInfo->card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
        
        pMemberInfo->card_info.plus = pMemberInfo->card_info.pl_attack + pMemberInfo->card_info.pl_hp + pMemberInfo->card_info.pl_recover;
        
        long recoverbase = card["recover"].asInt();
        float fRecoverGrowth = card["recover_growth"].asDouble();
        pMemberInfo->card_info.recover = recoverbase + (pMemberInfo->card_info.lv - 1)*fRecoverGrowth + pMemberInfo->card_info.pl_recover*RECPLUSGROWTH;
        
        long hpbase = card["hp"].asInt();
        float fHpGrowth = card["hp_growth"].asDouble();
        pMemberInfo->card_info.hp = hpbase + (pMemberInfo->card_info.lv - 1)*fHpGrowth + pMemberInfo->card_info.pl_hp*HPPLUSGROWTH;
        
        long attackbase = card["attack"].asInt();
        float fAtkGrowth = card["attack_growth"].asDouble();
        pMemberInfo->card_info.attack = attackbase + (pMemberInfo->card_info.lv - 1)*fAtkGrowth + pMemberInfo->card_info.pl_attack*ATKPLUSGROWTH;
        
        m_pRewardMemberArr->addObject(pMemberInfo);
        pMemberInfo->release();
    }

    return m_pRewardMemberArr;
}


//******************************************************************************
// getRewardTaskList
//******************************************************************************
CCMutableArray<CPostRewardBaseInfo*>* CGameData::getRewardTaskList()
{
    m_pRewardTaskArr->removeAllObjects();
    Json::Value rewardTaskDic = HttpComm::Inst()->getRewarTaskDic();
    if (!rewardTaskDic.empty())
    {
        Json::Value itmCfgDic = HttpComm::Inst()->getCfgDic()["guild_config"]["guild_task_shop"];
        
        for (int i = 0; i < rewardTaskDic.size(); i++) {
            Json::Value oneRewardTask = rewardTaskDic[i];
            
            CPostRewardBaseInfo* rewardInfo = new CPostRewardBaseInfo;
            
            rewardInfo->name = oneRewardTask["task_id"].asString();
            rewardInfo->completeDeadLine = oneRewardTask["end_time"].asInt();
            rewardInfo->applyDeadLine = rewardInfo->completeDeadLine - 12 * 60 * 60;
            rewardInfo->ensureYuanBao = oneRewardTask["coin"].asInt();
            rewardInfo->MaxJoinPerson = oneRewardTask["person"].asInt();
            rewardInfo->CurJoinPerson = oneRewardTask["add_num"].asInt();
            rewardInfo->contribution = oneRewardTask["contribution"].asInt();
            rewardInfo->status = oneRewardTask["my_join"].asInt();
            
            Json::Value awardItemDic = oneRewardTask["award"];
            Json::Value::Members memberArr = awardItemDic.getMemberNames();
            rewardInfo->itemId = memberArr[0];
            rewardInfo->itemCount = awardItemDic[memberArr[0]].asInt();
            
            if (!itmCfgDic.empty()) {
                rewardInfo->strImgName = itmCfgDic[rewardInfo->itemId]["item_img"].asString();
                rewardInfo->strItemDsp = itmCfgDic[rewardInfo->itemId]["item_dsp"].asString();
            }
            
            m_pRewardTaskArr->addObject(rewardInfo);
            rewardInfo->release();
        }
    }

    return m_pRewardTaskArr;
}

//******************************************************************************
// getFriendList
//******************************************************************************
CCMutableArray<CFriendInf*> * CGameData::getFriendList()
{
    if(HttpComm::Inst()->getIsFriendListChange() == false)
        return m_pFriendInfArr;
    Json::Value reqArr = HttpComm::Inst()->getFriendListDic()["friends"];
    if(reqArr.empty())
        return 0;
    
    m_iCurFirendNum = HttpComm::Inst()->getFriendListDic()["now_num"].asInt();
    m_iMaxFirendNum = HttpComm::Inst()->getFriendListDic()["max_num"].asInt();
    
    m_pFriendInfArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllBaseCards) < HttpComm::Inst()->getDicTime(enDicCard)) {
        m_allSmalDicTime[enSmallDicAllBaseCards] = HttpComm::Inst()->getDicTime(enDicCard);
        m_allBaseCardsDic = HttpComm::Inst()->getCardDic();
    }
    
    for(int i = 0; i < reqArr.size(); i++){
        Json::Value friendInfDic = reqArr[i];
        CFriendInf* pFriendInf = new CFriendInf();
        
        if (friendInfDic.isMember("title_name"))
            strncpy(pFriendInf->title_name, friendInfDic["title_name"].asString().c_str(), 99);
        else
            strncpy(pFriendInf->title_name, "初出茅庐", 99);
        
        strncpy(pFriendInf->name, friendInfDic["name"].asString().c_str(), 99);
        
        strncpy(pFriendInf->fid, friendInfDic["fid"].asString().c_str(), 99);
        
        pFriendInf->user_elem = friendInfDic["country"].asInt();
        pFriendInf->lv = friendInfDic["lv"].asInt();
        pFriendInf->login_time = friendInfDic["login_time"].asInt();
        
        Json::Value leader = friendInfDic["leader_card"];
        string strCid = leader["cid"].asString();
        pFriendInf->leadercard_cid = atoi(strCid.c_str());
        pFriendInf->leadercardlv = leader["lv"].asInt();
        pFriendInf->sk_lv = leader["sk_lv"].asInt();
        
        //card info
        if(m_allBaseCardsDic.isMember(strCid) == false){
            CCLog("!!! undefined card: %s", strCid.c_str());
            continue;
        }
        
        Json::Value card = m_allBaseCardsDic[strCid];
        
        strncpy(pFriendInf->card_info.name, card["name"].asString().c_str(), 99);
        
        pFriendInf->card_info.cid = pFriendInf->leadercard_cid;
        pFriendInf->card_info.category = atoi(card["category"].asString().c_str());
        pFriendInf->card_info.sub_category = card.isMember("sub_category") ? atoi(card["sub_category"].asString().c_str()) : enGeneralTp_Invalidbn;
        
        
        pFriendInf->card_info.i_AwakeNum = leader.isMember("awake_index") ? leader["awake_index"].asInt() : 0;
        
        for (int z=0; z<MaxAwakeSkillCnt; z++)
            pFriendInf->card_info.AwakeSkillArr[z] = 0;
        
        if (card.isMember("awake_skid"))
        {
            Json::Value AwakeSkillArr = card["awake_skid"];
            for (int z=0; z<AwakeSkillArr.size(); z++)
            {
                if (z >= MaxAwakeSkillCnt)
                    break;
                
                pFriendInf->card_info.AwakeSkillArr[z] = AwakeSkillArr[z].asInt();
            }
        }
        
        pFriendInf->card_info.lv = pFriendInf->leadercardlv;
        pFriendInf->card_info.sk_lv = pFriendInf->sk_lv;
        pFriendInf->card_info.ugp_max_star = atoi(card["target_star"].asString().c_str());
        pFriendInf->card_info.star = atoi(card["star"].asString().c_str());
        pFriendInf->card_info.skid = atoi(card["skid"].asString().c_str());
        pFriendInf->card_info.ctype = atoi(card["ctype"].asString().c_str());
        pFriendInf->card_info.stype = card.isMember("stype") ? atoi(card["stype"].asString().c_str()) : enElementInvalid;
        pFriendInf->card_info.cost = card["cost"].asInt();
        pFriendInf->card_info.leader_skid = atoi(card["leader_skid"].asString().c_str());
        pFriendInf->card_info.max_lv = card["max_lv"].asInt();
        pFriendInf->card_info.cou_name = card.isMember("country") ? card["country"].asInt() : 0;//卡片的国家属性
        
        pFriendInf->card_info.pl_attack =leader.isMember("pl_attack") ? leader["pl_attack"].asInt() : 0;
        pFriendInf->card_info.pl_hp = leader.isMember("pl_hp") ? leader["pl_hp"].asInt() : 0;
        pFriendInf->card_info.pl_recover = leader.isMember("pl_recover") ? leader["pl_recover"].asInt() : 0;
        
        pFriendInf->card_info.plus = pFriendInf->card_info.pl_attack+pFriendInf->card_info.pl_hp+ pFriendInf->card_info.pl_recover;
        
        long recoverbase = card["recover"].asInt();
        float fRecoverGrowth = card["recover_growth"].asDouble();
        pFriendInf->card_info.recover = recoverbase + (pFriendInf->card_info.lv - 1) * fRecoverGrowth+pFriendInf->card_info.pl_recover*RECPLUSGROWTH;
        
        long hpbase = card["hp"].asInt();
        float fHpGrowth = card["hp_growth"].asDouble();
        pFriendInf->card_info.hp = hpbase + (pFriendInf->card_info.lv - 1) * fHpGrowth+pFriendInf->card_info.pl_hp*HPPLUSGROWTH;
        
        long attackbase = card["attack"].asInt();
        float fAtkGrowth = card["attack_growth"].asDouble();
        pFriendInf->card_info.attack = attackbase + (pFriendInf->card_info.lv - 1) * fAtkGrowth+pFriendInf->card_info.pl_attack*ATKPLUSGROWTH;
        
        if (card.isMember("is_elu_target") && (card["is_elu_target"].asInt() == 1)) {
            pFriendInf->card_info.bIsEluTgt = true;
        }
        else{
            pFriendInf->card_info.bIsEluTgt = false;
        }
        
        if (card.isMember("is_wake_target") && (card["is_wake_target"].asInt() == 1)) {
            pFriendInf->card_info.bIsWkTgt = true;
        }
        else{
            pFriendInf->card_info.bIsWkTgt = false;
        }
        
        m_pFriendInfArr->addObject(pFriendInf);
        pFriendInf->release();
    }
    HttpComm::Inst()->setIsFriendListChange(false);
    
    MailBoxAgt::Inst()->updateFriendInf();
    
    return m_pFriendInfArr;
}

CCMutableArray<CMAARequestMasterInfo*> * CGameData::getCMAARequestMasterList()
{
    //...
    return m_pMAARequestMasterInfoArr;
}

bool CGameData::isAppendCost()
{
    if (CUserInfo *pUserInfo = getUsrInfo())
    {
        return pUserInfo->hours_append_cost || pUserInfo->month_append_cost;
    }
    return false;
}

void CGameData::getGuildAwardText(std::string &szText)
{
    Json::Value temp = HttpComm::Inst()->getGuildAward();
    if (!temp.empty())
    {
        char cBuf[1024];
        std::string szTemp; //szText = "得到了";
        int nCount = 0;
        Json::Value::Members itemtemp = temp.getMemberNames();
        for (int i = 0; i < itemtemp.size(); ++i)
        {
            if (temp[itemtemp[i]].isMember("content"))
            {
                szTemp = temp[itemtemp[i]]["content"].asString();
            }
            if (temp[itemtemp[i]].isMember("num"))
            {
                nCount = temp[itemtemp[i]]["num"].asInt();
            }
            
            if (i!=(itemtemp.size()-1))
            {
                snprintf(cBuf,sizeof(cBuf),"%sx%d,",szTemp.c_str(),nCount);
            }
            else
            {
                snprintf(cBuf,sizeof(cBuf),"%sx%d",szTemp.c_str(),nCount);
            }
            
            szText += cBuf;
            if (0==i)
            {
                szText = CGameData::Inst()->getLanguageValue("get_item_append_csot")+szText;
            }
        }
    }
    HttpComm::Inst()->clearGuildAward();
}

bool CGameData::sendAppendCostRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->sendAppendCostRequest();
        bRet = true;
    }
    return bRet;
}

//******************************************************************************
// isNewbie
//******************************************************************************
bool CGameData::isNewbie()
{
    return HttpComm::Inst()->isNewbie();
}

//******************************************************************************
// isInviteCodeRight
//******************************************************************************
bool CGameData::isInviteCodeRight() {
    return HttpComm::Inst()->isRightInviteCode();
}

//******************************************************************************
// OCAccountYes
//******************************************************************************
bool CGameData::OCAccountYes()
{
    return HttpComm::Inst()->isOcAcntAvail();
}

//******************************************************************************
// QQAccountYes
//******************************************************************************
bool CGameData::QQAccountYes()
{
    return HttpComm::Inst()->isQQAcntAvail();
}

//******************************************************************************
// SinaAccountYes
//******************************************************************************
bool CGameData::SinaAccountYes()
{
    return HttpComm::Inst()->isSinaAcntAvail();
}

//******************************************************************************
// FacebookAccountYes
//******************************************************************************
bool CGameData::FacebookAccountYes()
{
    return HttpComm::Inst()->isFbAcntAvail();
}

//******************************************************************************
// getNewbieStep
//******************************************************************************
int CGameData::getNewbieStep()
{
    return HttpComm::Inst()->getNewbieStep();
}

//******************************************************************************
// chkLogin
//******************************************************************************
bool CGameData::chkLogin()
{
    if((HttpComm::Inst()->getDicTime(enDicLogin) > 0)
       && (HttpComm::Inst()->getDicTime(enDicCfg) > 0)) {
        return true;
    }
    else {
        return false;
    }
}

//******************************************************************************
// getPwUpSucType
//******************************************************************************
int CGameData::getPwUpSucType()
{
    string str;
    if(chkLogin() == false){
        Json::Value pwUpDic = HttpComm::Inst()->getNewbieDic();
        if (pwUpDic.empty()) {
            return enPwupSucType_Null;
        }
        str = pwUpDic["success_type"].asString();
    }
    else {
        str = HttpComm::Inst()->getPwUpDicSuccessType();
    }
        
    int iRet = enPwupSucType_Null;
        
    if(!str.compare("normal")){
        iRet = enPwupSucType_Normal;
    }
    else if(!str.compare("big")){
        iRet = enPwupSucType_Big;
    }
    else if(!str.compare("super")){
        iRet = enPwupSucType_Super;
    }

    return iRet;
}

bool CGameData::isPaying()
{
//    if([gGameData iReqType] == enReqPay){
//        return true;
//    }
//    else{
//        return false;
//    }
    return false;
}

//******************************************************************************
// isIapBestSell
//******************************************************************************
bool CGameData::isIapBestSell(const char* iap_id)
{
    Json::Value dicShop = HttpComm::Inst()->getShopDic();
    bool bRet = false;
    
    if(!dicShop.empty()){        
        
        int iRet = dicShop[iap_id].asInt();
        
        if(iRet == 1)
            bRet = true;
    }

    return bRet;
}

//******************************************************************************
// isIapValuePack
//******************************************************************************
bool CGameData::isIapValuePack(const char* iap_id)
{
    Json::Value dicShop = HttpComm::Inst()->getShopDic();
    bool bRet = false;
    
    if(!dicShop.empty()){
        
        int iRet = dicShop[iap_id].asInt();
        
        if(iRet == 2)
            bRet = true;
    }
    
    return bRet;
}

int CGameData::getAPCountDownTime()
{
    CUserInfo* pUsrInfo = getUsrInfo();
    if (pUsrInfo == NULL) {
        return 0;
    }
    else {
        return pUsrInfo->acPoint_upd_time - HttpComm::Inst()->getServerNow();
    }
}

int CGameData::getCountDownTime()
{
    CUserInfo* pUsrInfo = getUsrInfo();
    if (pUsrInfo == NULL) {
        return 0;
    }
    else {
        return pUsrInfo->stamina_upd_time - HttpComm::Inst()->getServerNow();
    }
}

const char* CGameData::getNoticeInviteAward() {
    if(!HttpComm::Inst()->getNoticeInviteAward().empty())
        return HttpComm::Inst()->getNoticeInviteAward().c_str();
    else
        return "";
}

const char* CGameData::getNoticeString(){
    return HttpComm::Inst()->getNoticeStr().c_str();
}

const char* CGameData::getNoticeGiftString()
{
    return HttpComm::Inst()->getNoticeGiftStr().c_str();
}

const char* CGameData::getLendInfo()
{
    return HttpComm::Inst()->getLendInfo().c_str();
}

const char* CGameData::getUrlDepDevice(const char *url)
{
    char tempUrl[255] = {'\0'};
    
    if(OcProxy::Inst()->isIpadDevice()){
        snprintf(tempUrl, 254, "%s%s0_%s", netServerURL, "static/html/", url);
    }else{
        snprintf(tempUrl, 254, "%s%s1_%s", netServerURL, "static/html/", url);
    }

    strcpy(retUrl, tempUrl);
    return retUrl;
}

void CGameData::maybeGotoAppstoreWeb()
{
#if !defined(kPltfmPp)
    if (m_iCurNmlFloor == 2) {
        
        OcProxy::Inst()->goAppStoreEvaluate();
    }
#endif
}

//******************************************************************************
// getCompGachaInfo
//******************************************************************************
CCMutableArray<CompGachaInfo *> *CGameData::getCompGachaInfo()
{
    if(m_compGachaInfoArr)
    {
        m_compGachaInfoArr->removeAllObjects();
    }

    Json::Value dic = HttpComm::Inst()->getCompGachaDic();
    Json::Value::Members tempArray = dic.getMemberNames();
    
    Json::Value compGachaDic = dic[tempArray[0]];
    
    Json::Value::Members numberArray = compGachaDic.getMemberNames();
    
    for(int i = 0;i<numberArray.size();i++)
    {
        for(int j = 0;j<numberArray.size()-1-i;i++)
        {
            int num1 = atoi(numberArray[j].c_str());
            int num2 = atoi(numberArray[j+1].c_str());
            if(num2<num1)
            {
                char temp[10];
                memcpy(temp, &numberArray[j], sizeof(numberArray[j]));
                memcpy(&numberArray[j], &numberArray[j+1], sizeof(numberArray[j+1]));
                memcpy(&numberArray[j+1], temp, sizeof(temp));
            }
        }
    }
    
    for(int i = 0; i < numberArray.size(); i++)
    {
        for(int ii=0; ii < numberArray.size(); ii++)
        {
            if(atoi(numberArray[ii].c_str()) == atoi(numberArray[i].c_str()))
            {
                CompGachaInfo *tempInfo = new CompGachaInfo();
                
                Json::Value oneCpDic = compGachaDic[numberArray[ii]];
                tempInfo->claimTimes = oneCpDic["claim_times"].asInt();
                tempInfo->rest_exchange_time = oneCpDic["rest_exchange_time"].asInt();
                
                Json::Value materialDic = oneCpDic["id_material"];
                Json::Value::Members array = materialDic.getMemberNames();
                for(int j = 0; j < array.size(); j++)
                {
                    CompGachaCardInfo *tempCardInfo =  new CompGachaCardInfo();
                    tempCardInfo->cid = atoi(array[j].c_str());
                    tempCardInfo->isOwe = materialDic[array[j]]["is_light"].asBool();
                    tempCardInfo->isNew = materialDic[array[j]]["is_new"].asBool();
                    
                    tempInfo->material->addObject(tempCardInfo);
                }
                
                tempInfo->targetCid = atoi(oneCpDic["id_card"].asString().c_str());
                tempInfo->isHide = oneCpDic["hide"].asBool();
                tempInfo->isTop = oneCpDic["top"].asBool();
                tempInfo->position = atoi(numberArray[ii].c_str());
                if(!tempInfo->isTop)
                    m_compGachaInfoArr->addObject(tempInfo);
                else 
                    m_compGachaInfoArr->insertObjectAtIndex(tempInfo, 0);
            }
        }
        
    }
    return m_compGachaInfoArr;
}

//******************************************************************************
// getCompGachaTime
//******************************************************************************
const char * CGameData::getCompGachaTime()
{
    Json::Value dic = HttpComm::Inst()->getCompGachaDic();
    Json::Value::Members tempArray = dic.getMemberNames();
    snprintf(compGachaTime, 99, "%s", tempArray[0].c_str());
    return compGachaTime;
}

//******************************************************************************
// getExchangeCardInfo
//******************************************************************************
ExchangeCardInfo * CGameData::getExchangeCardInfo()
{
    Json::Value dic = HttpComm::Inst()->getExchangeDic();
    snprintf(m_exchangeCardInfo->cid, 99, "%s", dic["cid"].asString().c_str());
    snprintf(m_exchangeCardInfo->uCid, 99, "%s", dic["ucid"].asString().c_str());

    return m_exchangeCardInfo;
}

//******************************************************************************
// getPushInfo
//******************************************************************************
CCMutableArray<PushInfo*> *  CGameData::getPushInfo()
{
    m_pushInfoArr->removeAllObjects();
    
    if(getSmallDicTime(enSmallDicAllDungeons) < HttpComm::Inst()->getDicTime(enDicCfg)) {
        m_allSmalDicTime[enSmallDicAllDungeons] = HttpComm::Inst()->getDicTime(enDicCfg);
        m_allBaseDungeonsDic = HttpComm::Inst()->getCfgDic()["dungeon"];
    }

    Json::Value array = m_allBaseDungeonsDic["push_list"];
    for(int i = 0;i < array.size(); i++)
    {
        PushInfo *tempInfo = new PushInfo();
        Json::Value dic =  array[i];
        tempInfo->pushTime = dic["time"].asInt();
        string pushInfo = dic["msg"].asString();
        memcpy(tempInfo->msg, pushInfo.c_str(), 1000);
        m_pushInfoArr->addObject(tempInfo);
    }
    return m_pushInfoArr;
}

const char* CGameData::getWeiboBackStr()
{
    string weiboStr = HttpComm::Inst()->getWeiboBackStr();
    if(!weiboStr.empty())
        return weiboStr.c_str();
    else
        return "";
}



//******************************************************************************
// getLanguageForKey
//******************************************************************************
const char *CGameData::getLanguageValue(const char *languageKey) {    
    return HttpComm::Inst()->getLanguageValue(languageKey);
}

//******************************************************************************
// clearLoginDefaultData
//******************************************************************************
void CGameData::clearLoginDefaultData()
{
    OcProxy::Inst()->removeObjectForKey("platform");
    OcProxy::Inst()->removeObjectForKey("openid");
    OcProxy::Inst()->removeObjectForKey("access_token");
    OcProxy::Inst()->synchronizeDefaultData();
}

//******************************************************************************
// clearUserDefaultData
//******************************************************************************
void CGameData::clearUserDefaultData() {
    m_pUserCardArr->removeAllObjects();
    
    m_iCurNmlFloor = 0;
    m_iCurNmlRoom = 0;
    m_iCurSangoFloor = 0;
    m_iCurSangoRoom = 0;
    m_bFighting = false;
    m_lFrndLstReqTime = 0;
    m_lRcvMailReqTime = 0;
    
    m_iCurFloor = 0;
    m_iCurFlrStat = 0;
    memset(m_curDgType, 0, sizeof(m_curDgType));
    
    memset(m_TeamInf, 0, sizeof(m_TeamInf));
    clrPreCmtTeamInfo();
    
    comeBackTime = -1;
    m_pFriendHelpArr->removeAllObjects();
    m_pUserCardArr->removeAllObjects();
    m_pShopCardArr->removeAllObjects();
    m_pNmlDgArr->removeAllObjects();
    m_pSangoYYArr->removeAllObjects();
    m_pWelfareInfoArr->removeAllObjects();
    m_pSpecialDgArr->removeAllObjects();
    m_pDgStepArr->removeAllObjects();
    m_pFriendInfArr->removeAllObjects();
    m_pFriendApplyInfArr->removeAllObjects();
    m_pMemberInfArr->removeAllObjects();
    m_pCntryInfArr->removeAllObjects();
    m_pRwdTopMemberInfArr->removeAllObjects();
    m_GachaRetArr->removeAllObjects();
    
    
    m_ParcelChildInfoArr->removeAllObjects();

    m_ParcelItemInfoArr->removeAllObjects();

    
    
    m_pMemberApplyInfArr->removeAllObjects();
    m_pCntryDgInfArr->removeAllObjects();
    m_pItmPkInf->removeAllObjects();
    m_pItmTtlInf->removeAllObjects();
    m_pTitleInfArr->removeAllObjects();
    m_pJiFengRecordArr->removeAllObjects();
    m_pGuildShopItemInfArr->removeAllObjects();
    m_pRewardTaskArr->removeAllObjects();
    m_pRewardMemberArr->removeAllObjects();
    m_pRwdSignInfArr->removeAllObjects();
    m_pRwdLevelUpInfArr->removeAllObjects();
    m_pRwdMonthInArr->removeAllObjects();
    m_pRwdBonusInfArr->removeAllObjects();
    m_pRwdHuodongInfArr->removeAllObjects();
    m_pRwdBuchangInfArr->removeAllObjects();
    m_pGuildItemAwardAff->removeAllObjects();
    m_pMAARequestMasterInfoArr->removeAllObjects();
    
    m_newWeeklyDgStatusDic.clear();
    m_newSpecialDgStatusDic.clear();
    m_allUserDugeonsDic.clear();
    m_allUserCardsDic.clear();
    m_allBaseCardsDic.clear();
    m_allBaseDungeonsDic.clear();
    m_allMonstersDic.clear();
    m_allCardLevelsDic.clear();
    m_cntryConfigDic.clear();
    memset(m_allSmalDicTime, 0, sizeof(long) * enSmallDicMax);

    m_bNeedToReqCollection = true;
    m_bNeedToReqHelper = true;
    m_defaultRangeType = enCardRangeTypeProp;
    m_memberRangeType = enMemberRangeStatus;
    
    clearLoginDefaultData();
    
    OcProxy::Inst()->deleteCookies();
    OcProxy::Inst()->deleteCache();
    HttpComm::Inst()->clearAllData();
    
    OcProxy::Inst()->resetStoreStatus();
    
    releaseUserCardsSprite();
    
    MailBoxAgt::destroy();
}

int CGameData::getLvlUpCoin()
{
    Json::Value dic = HttpComm::Inst()->getTrophiesDic();
    int value = 0;
    
    if(!dic.empty())
    {
        value = dic["lv_up_coin"].asInt();
    }
    
    return value;
}

long CGameData::getDgEndGold()
{
    Json::Value dic = HttpComm::Inst()->getTrophiesDic();
    long value = 0;
    
    if(!dic.empty())
    {
        value = dic.isMember("total_gold") ? dic["total_gold"].asInt() : 0;
    }
    
    return value;
}

long CGameData::getDgEndExp()
{
    Json::Value dic = HttpComm::Inst()->getTrophiesDic();
    long value = 0;
    
    if(!dic.empty())
    {
        value = dic.isMember("total_exp") ? dic["total_exp"].asInt() : 0;
    }
    
    return value;
}

double CGameData::getDgEndExpEffect()
{
    Json::Value dic = HttpComm::Inst()->getTrophiesDic();
    double value = 0;
    
    if(!dic.empty())
    {
        value = dic.isMember("exp_effect") ? dic["exp_effect"].asDouble() : 0;
    }
    
    return value;
}

//******************************************************************************
// clearSessionId
//******************************************************************************
void CGameData::clearSessionId()
{
    HttpComm::Inst()->clearSessionId();
}

//******************************************************************************
// setIsUseChristmasUI
//******************************************************************************
void CGameData::setIsUseChristmasUI(bool bUseChristmasUI)
{
    HttpComm::Inst()->setIsUseChristmasUI(bUseChristmasUI);
}

//******************************************************************************
// getIsUseChristmasUI
//******************************************************************************
bool CGameData::getIsUseChristmasUI()
{
    return HttpComm::Inst()->getIsUseChristmasUI();
}

//******************************************************************************
// getMD5Str
//******************************************************************************
const char* CGameData::getMD5Str()
{
    string stringMd5 = HttpComm::Inst()->getMD5String();
    if (stringMd5.empty()) {
        return NULL;
    }
    else {
        return stringMd5.c_str();
    }
}

//******************************************************************************
// getSessionId
//******************************************************************************
const char* CGameData::getSessionId()
{
    return HttpComm::Inst()->getSessionId().c_str();
}

//******************************************************************************
// setSessionId
//******************************************************************************
void CGameData::setSessionId(const char *sessionId)
{
    return HttpComm::Inst()->setSessionId(sessionId);
}

//******************************************************************************
// getUserId
//******************************************************************************
const char* CGameData::getUserId()
{
    return HttpComm::Inst()->getUserId().c_str();
}

//******************************************************************************
// setUserId
//******************************************************************************
void CGameData::setUserId(const char *userId)
{
    return HttpComm::Inst()->setUserId(userId);
}

//******************************************************************************
// getRwdTopMembers
//******************************************************************************
CCMutableArray<CRwdTopMemberInfo*>* CGameData::getRwdTopMembers()
{
    m_pRwdTopMemberInfArr->removeAllObjects();
    Json::Value RwdTopMemberDic = HttpComm::Inst()->getRewardTopMembersDic();
    if (!RwdTopMemberDic.empty())
    {
        for (int i = 0; i < RwdTopMemberDic.size(); i++)
        {
            Json::Value oneRwdTopMember = RwdTopMemberDic[i];
            
            CRwdTopMemberInfo* rewardInfo = new CRwdTopMemberInfo;
            
            rewardInfo->name = oneRwdTopMember["name"].asString();
            rewardInfo->guild_name = oneRwdTopMember["guild_name"].asString();
            rewardInfo->contribution = oneRwdTopMember["contribution"].asInt();
            rewardInfo->lv = oneRwdTopMember["lv"].asInt();
                    
            m_pRwdTopMemberInfArr->addObject(rewardInfo);
            rewardInfo->release();
        }
    }
    
    return m_pRwdTopMemberInfArr;
}


//******************************************************************************
// getCntryList
//******************************************************************************
CCMutableArray<CCntryBaseInfo*>* CGameData::getCntryList()
{
    static long slCntryListUpdateTm = 0;
    
    if (slCntryListUpdateTm < HttpComm::Inst()->getDicTime(enDicCntryList)) {
        slCntryListUpdateTm = HttpComm::Inst()->getDicTime(enDicCntryList);
    }
    else {
        return m_pCntryInfArr;
    }
    
    Json::Value cntryListDic = HttpComm::Inst()->getCntryListDic();
    
    if (cntryListDic.empty()) {
        return 0;
    }
    
    m_pCntryInfArr->removeAllObjects();
    Json::Value cntryRankDic = cntryListDic["guild"];
    for (int i = 0; i < cntryRankDic.size(); i++) {
        Json::Value cntryInfoDic = cntryRankDic[i];
        CCntryBaseInfo* cntryBaseInfo = new CCntryBaseInfo();
        
        snprintf(cntryBaseInfo->name, 99, "%s", cntryInfoDic["gname"].asString().c_str());
        snprintf(cntryBaseInfo->member_num, 99, "%s", cntryInfoDic["gnumber"].asString().c_str());
        snprintf(cntryBaseInfo->declaration, 199, "%s", cntryInfoDic["gdeclaration"].asString().c_str());
        snprintf(cntryBaseInfo->cid, 99, "%s", cntryInfoDic["gid"].asString().c_str());
        snprintf(cntryBaseInfo->leader_name, 99, "%s", cntryInfoDic["gleader"].asString().c_str());
        cntryBaseInfo->lv = cntryInfoDic["glevel"].asInt();
        cntryBaseInfo->rank = cntryInfoDic["ranking"].asInt();
        cntryBaseInfo->bApply = cntryInfoDic["is_applyguild"].asBool();
        cntryBaseInfo->gexp_total = cntryInfoDic["gexp_total"].asInt();
        
        m_pCntryInfArr->addObject(cntryBaseInfo);
        cntryBaseInfo->release();
    }
    
    if (!cntryListDic["myguild"].empty())
    {
        getCntryInfo();
        CCntryBaseInfo* cntryBaseInfo = new CCntryBaseInfo();
     
        cntryBaseInfo->rank = cntryListDic["myguild"]["ranking"].asInt();

        snprintf(cntryBaseInfo->name, 99, "%s", m_cntryInfo.name);
        snprintf(cntryBaseInfo->member_num, 99, "%d/%d",
                 m_cntryInfo.member_cur_num, m_cntryInfo.member_max_num);
        snprintf(cntryBaseInfo->declaration, 199, "%s", m_cntryInfo.declaration);
        snprintf(cntryBaseInfo->cid, 99, "%s", m_cntryInfo.cid);
        snprintf(cntryBaseInfo->leader_name, 99, "%s", m_cntryInfo.leader_name);
        cntryBaseInfo->lv = m_cntryInfo.lv;
        cntryBaseInfo->bApply = false;
        cntryBaseInfo->gexp_total = m_cntryInfo.cur_exp;
        
        m_pCntryInfArr->addObject(cntryBaseInfo);
        cntryBaseInfo->release();
    }
    
    return m_pCntryInfArr;
}

#pragma mark -
#pragma mark requset HttpComm
//******************************************************************************
// requestIndex
//******************************************************************************
void CGameData::requestIndex(const char* inviteCode)
{
    if (inviteCode == 0) {
        HttpComm::Inst()->reqSessionid();
    }
    else {
        string invite_code(inviteCode);
        HttpComm::Inst()->reqSessionidWithInviteCode(invite_code);
    }
}

//******************************************************************************
// requestInfo
//******************************************************************************
bool CGameData::reqInfoAsync()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->ReqInfoAsync();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// requestALanguageBag
//******************************************************************************
void CGameData::requestALanguageBag() {
    HttpComm::Inst()->reqLanguageBag();
}

//******************************************************************************
// newbieRequest
//******************************************************************************
bool CGameData::newbieRequest(int country, int step, char* ucid)
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
    	string strUcid("");
    	if(ucid!=NULL)
    	{
    		strUcid.append(ucid);
    	}
        
        HttpComm::Inst()->newbieRequest(country,step,strUcid);
        bRet = true;
    }

    return bRet;
}

//******************************************************************************
// newbieRequest
//******************************************************************************
bool CGameData::newbieRequestPwUp()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    if(reqstat != kReqStatRequesting){
        
        getUserCards();
        string base_ucid ;
    	string foods ;
        
        
        for(int i = 0 ; i <m_pUserCardArr->count() ; i ++)
        {
            CUserCardInfo * temp = m_pUserCardArr->getObjectAtIndex(i);
            if(temp -> cid == 49)
            {
                base_ucid = temp -> ucid;
            }
            else if(temp -> cid == 86)
            {
                foods = temp -> ucid;
            }
            
        }
        
        
    	
        
        
//        CCLog("%s , %s" , base_ucid.c_str() , foods.c_str() );
        
        HttpComm::Inst()->newbieRequestPwUp(6,base_ucid,foods);
        bRet = true;
    }

    return bRet;
}

//******************************************************************************
// loginByQQRequest
//******************************************************************************
bool CGameData::loginByTencentRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->loginByTencent();
        bRet = true;
    }

    return bRet;
}

//******************************************************************************
// loginBySinaRequest
//******************************************************************************
bool CGameData::loginBySinaRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->loginBySina();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// loginRequest
//******************************************************************************
bool CGameData::loginRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->loginRequest();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cfgRequest
//******************************************************************************
bool CGameData::cfgRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->cfgRequest();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cfgRequest
//******************************************************************************
bool CGameData::reqCard()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqCard();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cfgRequest
//******************************************************************************
bool CGameData::reqMonster()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqMonster();
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::reqChargeAward()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqChargeAward();
        bRet = true;
    }
    
    return bRet;
}


ChargeAward* CGameData::getChargeAward()
{
    ChargeAward* pChAward = new ChargeAward();
    pChAward->autorelease();
    
    Json::Value chargeDic = HttpComm::Inst()->getChargeAwardDic();
    if (getCommonInfo()->charge_type == 1)
    {
        Json::Value array;
        array = chargeDic["my_awadr_ids"];
        pChAward->m_iAwadrIds = new int[array.size()];
        for (int i =0; i< array.size(); i++)
        {
            pChAward->m_iAwadrIds[i] = array[i].asInt();
        }
        
        pChAward->m_iAwadrNum = array.size();
        
        array = chargeDic["my_oid_ids"];
        pChAward->m_iOidIds = new int[array.size()];
        for (int i =0; i< array.size(); i++)
        {
            pChAward->m_iOidIds[i] = array[i].asInt();
        }
        
        pChAward->m_iOidNum = array.size();
        
        pChAward->m_iOidTotalNum = chargeDic["my_oid_num"].asInt();
    }
    else if (getCommonInfo()->charge_type == 2)
    {
        pChAward->charge_sum = chargeDic["my_sum_charge"].asInt();
    }
    
    return pChAward;
}

//******************************************************************************
// frdRequest
//******************************************************************************
bool CGameData::frdRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->frdRequest();
        bRet = true;
    }
    
    return bRet;
}



//******************************************************************************
// frdExtendRequest
//******************************************************************************
bool CGameData::frdExtendRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->frExtend();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// askCostMonthCard
//******************************************************************************
bool CGameData::askCostMonthCard()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->askCostMonthCard();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// friendListRequest
//******************************************************************************
bool CGameData::friendListRequest()
{
    bool bRet = false;
    
    //10分钟内不再获取好友列表
    if(getCurrentTime() - m_lFrndLstReqTime < 600)
        return false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->friendListReq();
        bRet = true;
    }
    
    return bRet;
}


//******************************************************************************
// newChangeRequest  更改新手状态
//******************************************************************************
bool CGameData::newChangeRequest(const char* str)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string sFid(str);
        HttpComm::Inst()->newTypeChangeReq(str);
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::newGiveWuJiangRequest(string str)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        
        HttpComm::Inst()->newGiveWuJiangReq(str);
        bRet = true;
    }
    
    return bRet;
}



//******************************************************************************
// acceptRequest
//******************************************************************************
bool CGameData::acceptRequest(const char* fid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string sFid(fid);
        HttpComm::Inst()->acceptRequest(sFid);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// refuseRequest
//******************************************************************************
bool CGameData::refuseRequest(const char* fid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string sFid(fid);
        HttpComm::Inst()->refuseRequest(sFid);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// schFriendRequest
//******************************************************************************
bool CGameData::schFriendRequest(const char* fid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string sFid(fid);
        HttpComm::Inst()->schFriendRequest(sFid);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// reqBuyCard
//******************************************************************************
bool CGameData::reqBuyCard(short cid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->buyCard(cid);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// frdRequest
//******************************************************************************
bool CGameData::enterDgRequest(const char* dg_type, char* flrid, char* roomid,
                               const char* hp_ucid, const char* hprid[kHelperMaxCount],
                               const char* helpertp[kHelperMaxCount], long lRty)
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();

    if(reqstat != kReqStatRequesting){
        string strdgtp(dg_type);
        string strfloorid(flrid);
        string strrmid(roomid);
        string strhprid(hprid[0]);
        string strhelpertp(helpertp[0]);
        
        char sRty[255];
        snprintf(sRty,254,"%ld",lRty);
        string strRty(sRty);
        
        if (m_preCmtTeam.mem[0].uid[0] != 0) {
            for (int i = 0; i < 5; i++) {
                setTeamInfo(kMaxTeamCnt, i, m_preCmtTeam.mem[i].uid);
            }
        }

        string new_deck("");
        for (int i=0; i<kMaxTeamCnt-1; i++){
            
            new_deck.append(  m_TeamInf[i].mem[0].uid).append(",")
            .append(m_TeamInf[i].mem[1].uid).append(",")
            .append(m_TeamInf[i].mem[2].uid).append(",")
            .append(m_TeamInf[i].mem[3].uid).append(",")
            .append(m_TeamInf[i].mem[4].uid);
            
            if (i < kMaxTeamCnt-1-1){
                new_deck.append("_");
            }
        }
        
        char buf[32];
        snprintf(buf, sizeof(buf), "&deck_index=%d", m_iCurTeamIdx);
        new_deck.append(buf);
        
        Json::Value dicHelper;
        int index = 0;
        for (int i = 0; i < 5; i++) {
            index = 0;
            dicHelper[i][index] = hprid[i];
            index = 1;
            dicHelper[i][index] = helpertp[i];
        }
        
        Json::FastWriter dicWriter;

        HttpComm::Inst()->reqEnterDungeon(strdgtp, strfloorid, strrmid, m_strCostCoin,
                                          strhprid, hp_ucid,strhelpertp, sRty, new_deck,
                                          dicWriter.write(dicHelper));

        bRet = true;
        m_bNeedToReqCollection = true;
        m_bNeedToReqHelper = true;

        m_iCurFloor = atoi(flrid);
        m_iCurRoom = atoi(roomid);
        strncpy(m_curDgType, dg_type, 19);
        m_iCurFlrStat = getDgFloorStatus(m_curDgType, m_iCurFloor);
    }
    
    return bRet;
}

//******************************************************************************
// dgEndRequest
//******************************************************************************
bool CGameData::dgEndRequest(const char *dg_type, char *flrid, char *roomid, char *dgUid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    if(reqstat != kReqStatRequesting){
        string strdgtp(dg_type);
        string strfloorid(flrid);
        string strrmid(roomid);
        string strdguid(dgUid);
        
        char buf[256];
        snprintf(buf,
                 sizeof(buf),
                 "%lx,%lx,%lx,%lx,%lx,%lx",
                 SaveData::Inst()->getTouchCnt(),
                 SaveData::Inst()->getTotalCombo(),
                 SaveData::Inst()->getMaxCombo(),
                 SaveData::Inst()->getTotalDamage(),
                 SaveData::Inst()->getMaxDamage(),
                 SaveData::Inst()->getUseSkillCnt());
        string strScore = buf;
        
        HttpComm::Inst()->dgEndRequest(strdgtp, strfloorid, strrmid, strdguid, strScore);
        bRet = true;
    }

    return bRet;
}
/*
 *返回是否成功发送战败结算请求
 *
 */
bool CGameData::dgEndForFanTiRequest(const char *dg_type, char *flrid, char *roomid, char *dgUid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    if(reqstat != kReqStatRequesting){
        string strdgtp(dg_type);
        string strfloorid(flrid);
        string strrmid(roomid);
        string strdguid(dgUid);
        
        char buf[256];
        snprintf(buf,
                 sizeof(buf),
                 "%lx,%lx,%lx,%lx,%lx,%lx",
                 SaveData::Inst()->getTouchCnt(),
                 SaveData::Inst()->getTotalCombo(),
                 SaveData::Inst()->getMaxCombo(),
                 SaveData::Inst()->getTotalDamage(),
                 SaveData::Inst()->getMaxDamage(),
                 SaveData::Inst()->getUseSkillCnt());
        string strScore = buf;
        
      //  HttpComm::Inst()->dgEndRequest(strdgtp, strfloorid, strrmid, strdguid, strScore);
        HttpComm::Inst()->dgEndForFanTiRequest(strdgtp, strfloorid, strrmid, strdguid, strScore);
        bRet = true;
    }
    
    return bRet;
}


//******************************************************************************
// allCardRequest
//******************************************************************************
bool CGameData::allCardRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->getAllCardReq();
        bRet = true;
    }

    return bRet;
}






//******************************************************************************
// commitTeamRequest
//******************************************************************************
bool CGameData::commitTeamRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    if(reqstat != kReqStatRequesting)
    {
        std::string new_deck("");
        
        for (int i=0; i<kMaxTeamCnt-1; i++){

            new_deck.append(  m_TeamInf[i].mem[0].uid).append(",")
            .append(m_TeamInf[i].mem[1].uid).append(",")
            .append(m_TeamInf[i].mem[2].uid).append(",")
            .append(m_TeamInf[i].mem[3].uid).append(",")
            .append(m_TeamInf[i].mem[4].uid);

            if (i < kMaxTeamCnt-1-1){
                new_deck.append("_");
            }
        }
        
        char buf[32];
        snprintf(buf, sizeof(buf), "&deck_index=%d", m_iCurTeamIdx);
        new_deck.append(buf);
        
        HttpComm::Inst()->commitTeamReq(new_deck);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// checkInviteCodeRequest
//******************************************************************************
bool CGameData::checkInviteCodeRequest(const char *inviteCode)
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    if(reqstat != kReqStatRequesting){
        string invite_code(inviteCode);
        HttpComm::Inst()->checkInviteCodeReq(invite_code);
        bRet = true;
    }

    return bRet;
}


//******************************************************************************
// powerUpRequest
//******************************************************************************
bool CGameData::powerUpRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
    	std::string base_ucid(m_CombineQueue[0].uid);
    	std::string foods("");
    	foods.append(m_CombineQueue[1].uid).append(",").append(m_CombineQueue[2].uid).append(",").append(m_CombineQueue[3].uid).append(",").append(m_CombineQueue[4].uid).append(",").append(m_CombineQueue[5].uid);
        HttpComm::Inst()->powerUpReq(base_ucid,foods);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// niepanRequest
//******************************************************************************
bool CGameData::niepanRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
    	std::string base_ucid(m_CombineQueue[0].uid);
    	std::string foods("");
    	foods.append(m_CombineQueue[1].uid).append(",").append(m_CombineQueue[2].uid).append(",").append(m_CombineQueue[3].uid).append(",").append(m_CombineQueue[4].uid).append(",").append(m_CombineQueue[5].uid);
        HttpComm::Inst()->niepanReq(base_ucid,foods);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// evolutionRequest
//******************************************************************************
bool CGameData::evolutionRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
    	std::string base_ucid(m_CombineQueue[0].uid);
    	std::string foods("");
    	foods.append(m_CombineQueue[1].uid).append(",").append(m_CombineQueue[2].uid).append(",").append(m_CombineQueue[3].uid).append(",").append(m_CombineQueue[4].uid).append(",").append(m_CombineQueue[5].uid);
        
        HttpComm::Inst()->evolutionReq(base_ucid,foods);
        
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}

//******************************************************************************
// sevoRequest
//******************************************************************************
bool CGameData::sevoRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        std::string base_ucid(m_CombineQueue[0].uid);
        std::string foods("");
        foods.append(m_CombineQueue[1].uid).append(",").append(m_CombineQueue[2].uid).append(",").append(m_CombineQueue[3].uid).append(",").append(m_CombineQueue[4].uid).append(",").append(m_CombineQueue[5].uid);
        
        HttpComm::Inst()->sevoReq(base_ucid,foods);
        
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}

//******************************************************************************
// wakeRequest
//******************************************************************************
bool CGameData::wakeRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        std::string base_ucid(m_CombineQueue[0].uid);
        std::string foods("");
        foods.append(m_CombineQueue[1].uid).append(",").append(m_CombineQueue[2].uid).append(",").append(m_CombineQueue[3].uid).append(",").append(m_CombineQueue[4].uid).append(",").append(m_CombineQueue[5].uid);
        
        HttpComm::Inst()->wakeReq(base_ucid,foods);
        
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}

//******************************************************************************
// sellRequest
//******************************************************************************
bool CGameData::sellRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        char buf[500] = {0};
        for(int i = 1; i < MaxCombineCnt; i++){
            if(m_CombineQueue[i].uid[0] == 0)
                break;
            
            if(i > 1){
                strcat(buf, ",");
            }
            strcat(buf, m_CombineQueue[i].uid);
        }
        
        std::string ucids(buf);
        HttpComm::Inst()->sellReq(ucids);
        
        bRet = true;
    }
        
    return bRet;
}


//******************************************************************************
// addFriendRequest
//******************************************************************************
bool CGameData::addFriendRequest(const char* fid)
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
    	string strFid(fid);
        HttpComm::Inst()->addFriendReq(strFid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// freeGachaRequest
//******************************************************************************
bool CGameData::freeGachaRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->freeGachaReq();
        
        bRet = true;
        m_bNeedToReqCollection = true;
        
    }
    
    return bRet;
}

//******************************************************************************
// delFriendRequest
//******************************************************************************
bool CGameData::delFriendRequest(const char* fid)
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string strFid(fid);
        HttpComm::Inst()->delFriendRequest(strFid);
        
        bRet = true;
    }
    
    return bRet;
}


//******************************************************************************
// freeGachaRequest
//******************************************************************************
bool CGameData::reqlistRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqListRequest();
        bRet = true;
    }
    
    return bRet;
}


//******************************************************************************
// chargeGachaRequest
//******************************************************************************
bool CGameData::chargeGachaRequest(bool bSuiTang, string strItem,long curTime , bool free)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        char sRty[255];
        string strRty("");
        if (curTime != 0) {
            snprintf(sRty,254,"%ld",curTime);
            strRty = string(sRty);
        }
        HttpComm::Inst()->chargeGachaReq(bSuiTang, strItem,strRty , free);
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}

//bool CGameData::chargeGachaRequest_Other(int OtherGacha, string strItem)
//{
//    bool bRet = false;
//    
//    int reqstat = HttpComm::Inst()->getReqStatus();
//    
//    if(reqstat != kReqStatRequesting){
//        HttpComm::Inst()->chargeGachaReq_Other(OtherGacha, strItem);
//        bRet = true;
//        m_bNeedToReqCollection = true;
//    }
//    
//    return bRet;
//}

bool CGameData::chargeGachaRequest_Other(int OtherGacha, string strItem ,long curtime , bool free)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        
        char sRty[255];
        string strRty("");
        if (curtime != 0) {
            snprintf(sRty,254,"%ld",curtime);
            strRty = string(sRty);
        }

        HttpComm::Inst()->chargeGachaReq_Other(OtherGacha, strItem,strRty , free);
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}

//******************************************************************************
// multiChargeGachaRequest
//******************************************************************************
//bool CGameData::multiChargeGachaRequest(string strItem)
//{
//    bool bRet = false;
//    
//    int reqstat = HttpComm::Inst()->getReqStatus();
//    
//    if(reqstat != kReqStatRequesting){
//        HttpComm::Inst()->multiChargeGachaReq(strItem);
//        bRet = true;
//        m_bNeedToReqCollection = true;
//    }
//    
//    return bRet;
//}

bool CGameData::multiChargeGachaRequest(string strItem,long curtime)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        
        char sRty[255];
        string strRty("");
        if (curtime != 0) {
            snprintf(sRty,254,"%ld",curtime);
            strRty = string(sRty);
        }
        
        HttpComm::Inst()->multiChargeGachaReq(strItem,strRty);
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}


//******************************************************************************
// multiFreeGachaRequest
//******************************************************************************
bool CGameData::multiFreeGachaRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->multiFreeGachaReq();
        bRet = true;
        m_bNeedToReqCollection = true;
    }
    
    return bRet;
}


//******************************************************************************
// compGachaConfigRequest
//******************************************************************************
bool CGameData::compGachaConfigRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->compGachaConfigReq();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// exchangeCardRequest
//******************************************************************************
bool CGameData::exchangeCardRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        string strtime(getCompGachaTime());
        
        char buf[32] = {'\0'};
        snprintf(buf, 31, "%d", compGachaNumber);
        string strnumber(buf);
        
        HttpComm::Inst()->exchangeCardReq(strtime, strnumber);
        bRet = true;
        m_bNeedToReqCollection = true;
    }
        
    return bRet;
}

//******************************************************************************
// packExtendRequest
//******************************************************************************
bool CGameData::packExtendRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->packExtendRequest();
    	bRet = true;
    }
    
    return bRet;
}

bool CGameData::reqWeiboBackRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
       HttpComm::Inst()->weiboBackReq();
    	bRet = true;
    }
    
    return bRet;
    
}

//******************************************************************************
// collectionRequest
//******************************************************************************
bool CGameData::collectionRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->collectionRequest();
    	bRet = true;
    }
    
    m_bNeedToReqCollection = false;
    return bRet;
}


//******************************************************************************
// recoverStaminaRequest
//******************************************************************************
bool CGameData::recoverStaminaRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->recoverStaminaRequest();
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// reviveRequest
//******************************************************************************
bool CGameData::reviveRequest(long lRty,string strReviveCostId)
{
    bool bRet = false;

    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        char sRty[255];
        snprintf(sRty,254,"%ld",lRty);
        string strRty(sRty);
        
        HttpComm::Inst()->reviveRequest(strRty,strReviveCostId);
    	bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// createCountry
//******************************************************************************
bool CGameData::createCountryRequest(const char *name, const char *declaration)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strName(name);
        string strDecalration(declaration);
        FbdWords::Ins()->chkLegal(strName);
        FbdWords::Ins()->chkLegal(strDecalration);
        HttpComm::Inst()->createCountryRequest(strName, strDecalration);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// setViceLeaderRequest
//******************************************************************************
bool CGameData::setViceLeaderRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->setViceLeaderRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::setUserVoiceOnOff(const char *uid,bool voiceOn)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->setUserVoiceOnOff(strUid,voiceOn);
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::changeMemberPingbi(const char* uid,bool pingbi)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->changeMemberPingbi(strUid,pingbi);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// deleteViceLeaderRequest
//******************************************************************************
bool CGameData::deleteViceLeaderRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->deleteViceLeaderRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// getMemberApplyListRequest
//******************************************************************************
bool CGameData::getMemberApplyListRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->getMemberApplyListRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// getMemberListRequest
//******************************************************************************
bool CGameData::getMemberListRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->getMemberListRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// accepMemberRequest
//******************************************************************************
bool CGameData::acceptMemberRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->acceptMemberRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// refuseMemberRequest
//******************************************************************************
bool CGameData::refuseMemberRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->refuseMemberRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// deleteMemberRequest
//******************************************************************************
bool CGameData::deleteMemberRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->deleteMemberRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// giveUpLeaderRequest
//******************************************************************************
bool CGameData::giveUpLeaderRequest(const char *uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->giveUpLeaderRequest(strUid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// setDeclarationRequest
//******************************************************************************
bool CGameData::setDeclarationRequest(const char *declaration)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strDeclaration(declaration);
        FbdWords::Ins()->chkLegal(strDeclaration);
        HttpComm::Inst()->setDeclarationRequest(strDeclaration);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// deleteCountryRequest
//******************************************************************************
bool CGameData::deleteCountryRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->deleteCountryRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// unDeleteCountryRequest
//******************************************************************************
bool CGameData::unDeleteCountryRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->unDeleteCountryRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// setNameCardRequest
//******************************************************************************
bool CGameData::setNameCardRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting
        && strlen(m_nameCard) > 0) {
        HttpComm::Inst()->setNameCardRequest(string(m_nameCard));
        
        bRet = true;
    }
    
    return bRet;
}


//setNamePayItemCard
bool CGameData::useItemCardRename(string strItem)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->useItemCardRename(strItem, string(m_nameCard));
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// levelUpCountryRequest
//******************************************************************************
bool CGameData::levelUpCountryRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->levelUpCountryRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// levelUpBuffRequest
//******************************************************************************
bool CGameData::levelUpBuffRequest(int buffType)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->levelUpBuffRequest(buffType);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// assignOneSupplyRequest
//******************************************************************************
bool CGameData::assignOneSupplyRequest(const char *mid, long supply)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        Json::Value assignDic;

        char buf [100];
        snprintf(buf, 99, "%ld", supply);

        if (mid)
            assignDic[mid] = buf;
        else {
            for (int i = 0; i < m_pMemberInfArr->count(); i++) {
                CMember* memberInfo = m_pMemberInfArr->getObjectAtIndex(i);
                assignDic[memberInfo->mid] = buf;
            }
        }

        HttpComm::Inst()->assignOneSupplyRequest(assignDic.toStyledString());
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// assignAllSupplyRequest
//******************************************************************************
bool CGameData::assignAllSupplyRequest(long supply)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->assignAllSupplyRequest(supply);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// assignOutSupplyRequest
//******************************************************************************
bool CGameData::assignOutSupplyRequest(long supply)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->assignOutSupplyRequest(supply);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// assignWeekSupplyRequest
//******************************************************************************
bool CGameData::assignWeekSupplyRequest(long supply)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->assignWeekSupplyRequest(supply);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// searchCountryRequest
//******************************************************************************
bool CGameData::searchCountryRequest(const char* cidOrName)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strCidOrName(cidOrName);
        HttpComm::Inst()->searchCountryRequest(strCidOrName);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// getAllCountryRequest
//******************************************************************************
bool CGameData::getAllCountriesRequest(int type)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->getAllCountriesRequest(type);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// deleteMailRequest
//******************************************************************************
bool CGameData::deleteMailRequest(const char* uid, long  timeStamp)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        HttpComm::Inst()->deleteMail(strUid, timeStamp);
        
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::deleteMailAllRequest()
{
    bool bRet = false;
    int reqstat = HttpComm::Inst()->getReqStatus();
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->DeleteMailAll();
        
        bRet = true;
    }
    return bRet;
}
//******************************************************************************
// sendMailAllRequest
//******************************************************************************
bool CGameData::sendMailAllRequest(const char* msg)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strMsg(msg);
        FbdWords::Ins()->chkLegal(strMsg);
        HttpComm::Inst()->sendMailAllRequest(strMsg);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// sendMailRequest
//******************************************************************************
bool CGameData::sendMailRequest(const char* uid, const char* msg)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strUid(uid);
        string strMsg(msg);
        FbdWords::Ins()->chkLegal(strMsg);
        HttpComm::Inst()->sendMailRequest(strUid, strMsg);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// rcvMailRequest
//******************************************************************************
bool CGameData::rcvMailRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->rcvMailRequest(MailBoxAgt::Inst()->getLastRcvTime());
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// quitCountryRequest
//******************************************************************************
bool CGameData::quitCountryRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->quitCountryRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// applyCountryRequest
//******************************************************************************
bool CGameData::applyCountryRequest(const char* cid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strCid(cid);
        HttpComm::Inst()->applyCountryRequest(strCid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// donateCoinRequest
//******************************************************************************
bool CGameData::donateCoinRequest(int coin)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->donateCoinRequest(coin);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// exchangeWelfareRequest
//******************************************************************************
bool CGameData::exchangeWelfareRequest(const char* wid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        string strWid(wid);
        HttpComm::Inst()->exchangeWelfareRequest(strWid);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// recoverPowerRequest
//******************************************************************************
bool CGameData::recoverPowerRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->recoverPowerRequest();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cntryDgCfgReq
//******************************************************************************
bool CGameData::cntryDgCfgReq()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->cntryDgCfgReq();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// getCntryDgInfArr
//******************************************************************************
CCMutableArray<CCntryDgInfo* > * CGameData::getCntryDgInfArr()
{
    m_pCntryDgInfArr->removeAllObjects();
    
    Json::Value cntryDgCfg = HttpComm::Inst()->getCntryDgCfgDic();
    
    if (cntryDgCfg.empty()) {
        return m_pCntryDgInfArr;
    }
    
    Json::Value arr = cntryDgCfg["get_guild_dungeon_config"];
    
    for (int i =0; i< arr.size(); i++) {
        CCntryDgInfo* pCntryDgInf = new CCntryDgInfo();
        
        string str = arr[i]["name"].asString();
        if (!str.empty())
            strncpy(pCntryDgInf->dg_name, str.c_str(), 99);
        
        str = arr[i]["dungeon_id"].asString();
        if (!str.empty())
            strncpy(pCntryDgInf->dg_id, str.c_str(), 99);
        
        str = arr[i]["dungeon_type"].asString();
        if (!str.empty())
            snprintf(pCntryDgInf->dg_type, 99, "%s", str.c_str());
        
        pCntryDgInf->cid = atoi(arr[i]["cid"].asString().c_str());
        pCntryDgInf->star = arr[i]["star"].asInt();
        pCntryDgInf->contribute = arr[i]["guild_contribute"].asInt();
        
        pCntryDgInf->gulid_coins = arr[i].isMember("minus_coin") ?  arr[i]["minus_coin"].asInt() : 0 ;
        pCntryDgInf->closed_time = arr[i].isMember("close_time") ?  arr[i]["close_time"].asInt() : 0 ;

        
        pCntryDgInf->curBossHp = arr[i]["hp"].asInt();
        pCntryDgInf->totalBossHp = arr[i]["total_hp"].asInt();
        pCntryDgInf->last_time = arr[i]["last_time"].asDouble();
        pCntryDgInf->m_unLimitMaxStar = arr[i].isMember("max_star")? arr[i]["max_star"].asInt() : 0;
        
        string strData = arr[i]["start_time"].asString();
        pCntryDgInf->start_time = getEchoTime(strData);

        pCntryDgInf->user_power = arr[i]["user_power"].asInt();
        pCntryDgInf->bIsFinish = arr[i]["is_finish"].asBool();
        pCntryDgInf->open_time = arr[i]["opentime"].asInt();
        
        pCntryDgInf->end_time = arr[i]["end_time"].asInt();
        pCntryDgInf->cold_time = arr[i]["cold_time"].asInt();
        
        pCntryDgInf->dungeon_tms = arr[i]["dungeon_tms"].asInt();
        pCntryDgInf->dungeon_tms_config = arr[i]["dungeon_tms_config"].asInt();

        long lCurTime = getCurrentTime();
        
        pCntryDgInf->remain_time = pCntryDgInf->open_time + pCntryDgInf->last_time * 3600 - lCurTime;
        
        if(pCntryDgInf->remain_time < 0)
            pCntryDgInf->remain_time = 0;
        
        m_pCntryDgInfArr->addObject(pCntryDgInf);
        pCntryDgInf->release();
    }
    
    sortCntryDgInfArr();
    
    return m_pCntryDgInfArr;
}

//******************************************************************************
// sortCntryDgInfArr
//******************************************************************************
void CGameData::sortCntryDgInfArr()
{
    bool bHasJiFeng = false;
    for (int i = 0; i < m_pCntryDgInfArr->count(); i++)
    {
        CCntryDgInfo* pCntryDgInf = m_pCntryDgInfArr->getObjectAtIndex(i);
        if (strcmp(pCntryDgInf->dg_type, "1") == 0)
        {
            bHasJiFeng = true;
            if (i != 0)
            {
                pCntryDgInf->retain();
                m_pCntryDgInfArr->removeObjectAtIndex(i);
                m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 0);
                pCntryDgInf->release();
            }
            break;
        }
    }
    bool bHasLuanWu = false;
    for (int i = 0; i < m_pCntryDgInfArr->count(); i++)
    {
        CCntryDgInfo* pCntryDgInf = m_pCntryDgInfArr->getObjectAtIndex(i);
        if (strcmp(pCntryDgInf->dg_type, "2") == 0)
        {
            if (!bHasJiFeng && i != 0)
            {
                bHasLuanWu = true;
                pCntryDgInf->retain();
                m_pCntryDgInfArr->removeObjectAtIndex(i);
                m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 0);
                pCntryDgInf->release();
            }
            else if (bHasJiFeng && i != 1)
            {
                bHasLuanWu = true;
                pCntryDgInf->retain();
                m_pCntryDgInfArr->removeObjectAtIndex(i);
                m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 1);
                pCntryDgInf->release();
            }
            break;
        }
    }
    for (int i = 0; i < m_pCntryDgInfArr->count(); i++) {
        CCntryDgInfo* pCntryDgInf = m_pCntryDgInfArr->getObjectAtIndex(i);
        if (strcmp(pCntryDgInf->dg_type, "3") == 0) {
            if (bHasJiFeng) {
                if (!bHasLuanWu ) {
                    pCntryDgInf->retain();
                    m_pCntryDgInfArr->removeObjectAtIndex(i);
                    m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 1);
                    pCntryDgInf->release();
                }else{
                    pCntryDgInf->retain();
                    m_pCntryDgInfArr->removeObjectAtIndex(i);
                    m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 2);
                    pCntryDgInf->release();
                }
            }else{
                if (!bHasLuanWu) {
                    pCntryDgInf->retain();
                    m_pCntryDgInfArr->removeObjectAtIndex(i);
                    m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 0);
                    pCntryDgInf->release();
                }else{
                    pCntryDgInf->retain();
                    m_pCntryDgInfArr->removeObjectAtIndex(i);
                    m_pCntryDgInfArr->insertObjectAtIndex(pCntryDgInf, 1);
                    pCntryDgInf->release();
                }
            }
        }
    }
}

//******************************************************************************
// cntryDgRankReq
//******************************************************************************
bool CGameData::cntryDgRankReq(string dg_id, string dg_type)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        if (dg_type == "1" || dg_type == "2" )
            HttpComm::Inst()->cntryJiFLuanWRankReq(dg_id);
        else
            HttpComm::Inst()->cntryDgRankReq(dg_id);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cntryDgOpenReq
//******************************************************************************
bool CGameData::cntryDgOpenReq(string dg_id,bool bIsTongMEnglianShou)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->cntryDgOpenReq(dg_id,bIsTongMEnglianShou);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// getCntryCurrentInfo
//******************************************************************************
bool CGameData::getCntryCurrentInfo()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->getCntryCurrentInfo();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cntryDgJoinReq
//******************************************************************************
bool CGameData::cntryDgJoinReq(string dg_id, string rtyid,bool bIsTongMEnglianShou)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        std::string new_deck("");
        for (int i = 0; i < 5; i++) {
            new_deck.append(getTeamInfByIdx(m_iCurTeamIdx)[i].uid);
            
            if (i < 4)
                new_deck.append(",");
        }

        HttpComm::Inst()->cntryDgJoinReq(dg_id, rtyid, new_deck,bIsTongMEnglianShou);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// cntryDgJoinReq
//******************************************************************************
bool CGameData::cntryDgEndReq(string dg_id, string combo,bool bIsTongMEnglianShou)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
       
        HttpComm::Inst()->cntryDgEndReq(dg_id, combo,bIsTongMEnglianShou);
        
        bRet = true;
    }
    
    return bRet;
}






//******************************************************************************
// useItemReq
//******************************************************************************
bool CGameData::useItemReq(string strItem, long lCnt)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->useItemReq(strItem, lCnt);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// useCompoundReq
//******************************************************************************


bool CGameData::useCompoundReq(string type, string newID)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->useCompoundReq(type, newID);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// useStoneReq
//******************************************************************************
bool CGameData::useStoneReq(string strItem, long lCnt)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting) {
        HttpComm::Inst()->useStoneReq(strItem, lCnt);
        
        bRet = true;
    }
    
    return bRet;
}

//bool CGameData::useItemAppendCsot(const char *pcItemID, long lCnt)
//{
//    bool bRet = false;
//    int reqstat = HttpComm::Inst()->getReqStatus();
//    
//    if (reqstat != kReqStatRequesting) {
//        //HttpComm::Inst()->useItemReq(strItem, lCnt);
//        
//        bRet = true;
//    }
//    
//    return bRet;
//}

//******************************************************************************
// getGiftReq
//******************************************************************************
bool CGameData::getGiftReq(string strCDKey)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->getGiftReq(strCDKey);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// postRewardRequest
//******************************************************************************
bool CGameData::postRewardRequest(long lContribution, int iDeadline, int iPerson,
                                  int iCoin, string itemId, int itemCount)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->postRewardReq(lContribution, iDeadline, iPerson,
                                        iCoin, itemId, itemCount);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// rewardTopMemberRequest
//******************************************************************************
bool CGameData::rewardTopMemberRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->rewardTopMemberReq();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// rewardTaskListRequest
//******************************************************************************
bool CGameData::rewardTaskListRequest()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->rewardTaskListReq();
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// acceptRewardTaskRequest
//******************************************************************************
bool CGameData::acceptRewardTaskRequest(string taskId)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->acceptRewardTaskReq(taskId);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// recieveRewardRequest
//******************************************************************************
bool CGameData::recieveRewardRequest(string taskId)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        HttpComm::Inst()->recieveRewardReq(taskId);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// rewardMemberListRequest
//******************************************************************************
bool CGameData::rewardMemberListRequest(string taskId)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        m_selectedTaskId = taskId;
        HttpComm::Inst()->rewardMemberListReq(taskId);
        
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// sendMsg
//******************************************************************************
bool CGameData::sendMsg(string strMsg)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if (reqstat != kReqStatRequesting)
    {
        FbdWords::Ins()->chkLegal(strMsg);
        
        string realStr = replaceString(strMsg, ";", "%3B");      // ;  对全角输入状态下处理 ｜ 半角输入状态下正常显示
        realStr = replaceString(realStr, "&", "%26");            // &  对半角输入状态下处理 ｜ 全角输入状态下正常显示

        HttpComm::Inst()->sendMsg(realStr);
        // printf("-------------tongmeng>%s", strMsg.c_str());
        // printf("-------------tongmeng2>%s", realStr.c_str());
        bRet = true;
    }
    
    return bRet;
}

// string 替换
string CGameData::replaceString(string in_str, string src, string dest)
{
    string out_str = in_str;//初始化输出字符串
    int pos = in_str.find(src);
    while (pos != string::npos) {  //string::npos表示没查到
        out_str = in_str.replace(pos, src.length(), dest);//如果查到了就用函数replace替换
        //replace的一参表示要替换起始位置，二参表示要替换的长度，三参表示要替换成什么
        pos = in_str.find(src); //find表示在in_str内查找src_str
    }
    return out_str;
}

void CGameData::startMonitorChatRx()
{
    HttpComm::Inst()->startMonitorChatRx();
}

void CGameData::stopMonitorChatRx()
{
    HttpComm::Inst()->stopMonitorChatRx();
}

bool CGameData::isNewChatRxCame()
{
    return HttpComm::Inst()->isNewChatRxCame();
}

string CGameData::getChatRxString()
{
    return HttpComm::Inst()->getChatRxString();
}

void CGameData::pushErrMsgToChatLst()
{
    HttpComm::Inst()->pushErrMsgToChatLst();
}

bool CGameData::haveNewMsg()
{
    if (getCntryInfo()->last_chat_time > getLocalChatTime()) {
        return true;
    }
    else{
        return false;
    }
}

void CGameData::resetLoadCardStep()
{
    m_iLoadCardsStep = 0;
}

// loadUserCardsStep()
bool CGameData::loadUserCardsStep(int iCount)
{
    CCMutableArray<CUserCardInfo*> * arrUserCards = getUserCards();
    
    string str = OcProxy::Inst()->getMachineType();
    if ((str == "iPhone 3G" || str == "iPhone 3GS" || str == "iTouch3" || str == "iTouch4")
        && (arrUserCards->count() > 300)
        )
        return true;
    
    if (m_iLoadCardsStep == 0)
    {
        if(m_lyUserCards)
            return true;
        
        m_lyUserCards = CCLayer::node();
        m_lyUserCards->retain();
    }
    
    
    int iMax = (m_iLoadCardsStep+1)*iCount > arrUserCards->count() ? arrUserCards->count() :(m_iLoadCardsStep+1)*iCount;
    
    for (int i = iCount*m_iLoadCardsStep; i < iMax; i++)
    {
        CUserCardInfo* pUserCardInf = arrUserCards->getObjectAtIndex(i);
        
        CCSprite* sp = getHeadSprite(pUserCardInf->cid);
        if(sp){
            m_lyUserCards->addChild(sp);
        }
    }
    
     m_iLoadCardsStep++;
    return iMax >= arrUserCards->count();
}

//******************************************************************************
// loadUserCardsSprite
//******************************************************************************
void CGameData::loadUserCardsSprite()
{
    CCMutableArray<CUserCardInfo*> * arrUserCards = getUserCards();
    
    string str = OcProxy::Inst()->getMachineType();
    if ((str == "iPhone 3G" || str == "iPhone 3GS" || str == "iTouch3" || str == "iTouch4")
        && (arrUserCards->count() > 300)
        )
        return;
    
    if(m_lyUserCards)
        return;
    
    m_lyUserCards = CCLayer::node();
    m_lyUserCards->retain();
    
    for (int i = 0; i < arrUserCards->count(); i++) {
        CUserCardInfo* pUserCardInf = arrUserCards->getObjectAtIndex(i);
        
        CCSprite* sp = getHeadSprite(pUserCardInf->cid);
        if(sp){
            m_lyUserCards->addChild(sp);
        }
    }
}

//******************************************************************************
// releaseUserCardsSprite
//******************************************************************************
void CGameData::releaseUserCardsSprite()
{
    if(m_lyUserCards){
        m_lyUserCards->release();
        m_lyUserCards = NULL;
        
        m_iLoadCardsStep = 0;
    }
}

const char * CGameData::getAppUrl()
{
    return HttpComm::Inst()->getAppUrl();
}


//设置刚抽到将的名称
void CGameData::setGachaCardName(const char *name)
{
    strncpy(gachaCardName, name, 99);
}

const char *CGameData::getGachaCardName()
{
    return gachaCardName;
}

const char* CGameData::getBundleId()
{
    return OcProxy::Inst()->getBundleId();
}

const char* CGameData::getBundleName()
{
    return OcProxy::Inst()->getBundleName();
}

void CGameData::setFrndLstReqTime(long lTime)
{
    m_lFrndLstReqTime = lTime;
}

void CGameData::setRcvMailReqTime()
{
    m_lRcvMailReqTime = getCurrentTime();
}

string CGameData::getCntryDgRankStr()
{
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntryDgRankDic();
    
    string strRet("");
    
    if (!cntryDgRankDic.empty()) {
        strRet = cntryDgRankDic["get_sorted_guildcontribute"].asString();
    }
    
    return strRet;
}

string CGameData::getCntryBossLastKillName()
{
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntryDgRankDic();
    
    string strRet("");
    if (!cntryDgRankDic.empty())
    {
        CCLOG("cntryDgRankDic = %s", cntryDgRankDic.toStyledString().c_str());
        strRet = cntryDgRankDic["kill_boss_username"].asString();
    }
    
    return strRet;
}

CCMutableArray<CCntryDgRankInfo* > * CGameData::getCntryDgRankInfArr()
{
    CCMutableArray<CCntryDgRankInfo* >* pCntryDgRankInfArr = new CCMutableArray<CCntryDgRankInfo* >();
    pCntryDgRankInfArr->autorelease();
    
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntryDgRankDic();

    if (!cntryDgRankDic.empty()) {
        Json::Value array = cntryDgRankDic["guildsorted"];
        
        for (int i = 0; i < array.size(); i++) {
            Json::Value oneItem = array[i];
            
            CCntryDgRankInfo* pInf = new CCntryDgRankInfo();
            
            int iZero = 0;
            pInf->lDamage = oneItem[iZero].asInt();
            pInf->strUid = oneItem[1].asString();
            pInf->strName = oneItem[2].asString();
            
            pCntryDgRankInfArr->addObject(pInf);
            pInf->release();
        }
    }

    return pCntryDgRankInfArr;
}

int CGameData::getMySpecialDgRank(string dgType)
{
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntrySpecialDgRankDic(dgType);
    if (!cntryDgRankDic.empty()) {
        Json::Value myRankDic = cntryDgRankDic["my_guild"];
        if (!myRankDic.empty())
        {
            return myRankDic["my_ranking"].asInt();
        }
        
        return 0;
    }
    
    return 0;
}

CSpecialDgRankInfo* CGameData::getMySpecialDgRankInf(string dgType)
{
    CSpecialDgRankInfo* dgRankInfo = new CSpecialDgRankInfo();
    dgRankInfo->autorelease();
    
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntrySpecialDgRankDic(dgType);
    if (!cntryDgRankDic.empty()) {
        Json::Value myRankDic = cntryDgRankDic["my_guild"];
        if (!myRankDic.empty())
        {
            Json::Value myRankInfo = myRankDic["guild_info"];
            dgRankInfo->strCid = myRankInfo["gid"].asString();
            dgRankInfo->strName = myRankInfo["name"].asString();
            dgRankInfo->strTime = myRankInfo["time"].asString();
        }
        
    }
    
    return dgRankInfo;
}

CCMutableArray<CSpecialDgRankInfo* > * CGameData::getSpecialDgRankInfArr(string dgType)
{
    CCMutableArray<CSpecialDgRankInfo* >* pSpecialDgRankInfArr = new CCMutableArray<CSpecialDgRankInfo* >();
    pSpecialDgRankInfArr->autorelease();
    
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntrySpecialDgRankDic(dgType);
    
    if (!cntryDgRankDic.empty()) {
        Json::Value array = cntryDgRankDic["guildsorted"];
        
        for (int i = 0; i < array.size(); i++) {
            Json::Value oneItem = array[i];
        
            CSpecialDgRankInfo* pInf = new CSpecialDgRankInfo();
            
            pInf->strTime = oneItem["time"].asString();
            pInf->strCid = oneItem["gid"].asString();
            pInf->strName = oneItem["name"].asString();
            
            pSpecialDgRankInfArr->addObject(pInf);
            pInf->release();
        }
    }
    
    return pSpecialDgRankInfArr;
}

long CGameData::getMyDamageFromRankInf()
{
    long lRet = 0;
    
    Json::Value cntryDgRankDic = HttpComm::Inst()->getCntryDgRankDic();
    if (!cntryDgRankDic.empty()) {
        if (cntryDgRankDic.isMember("mycontribute")) {
            lRet = cntryDgRankDic["mycontribute"].asInt();
        }
    }
    
    return lRet;
}

CntryDgEndInf* CGameData::getCntryDgEndInf()
{
    Json::Value cntry_end = HttpComm::Inst()->getCntryDgEndDic();
    
    CntryDgEndInf* pCntryDgEndInf = NULL;
    
    if (!cntry_end.empty()) {
        pCntryDgEndInf = new CntryDgEndInf();
        pCntryDgEndInf->autorelease();
        
        bool bBossDead = false;
        if (cntry_end["data"]["is_boss_dead"].empty())
            pCntryDgEndInf->whokillwho = 0;
        else
            bBossDead = cntry_end["data"]["is_boss_dead"].asBool();
        
        if (bBossDead)
            pCntryDgEndInf->whokillwho = 1;
        else
            pCntryDgEndInf->whokillwho = 2;
        
        
        pCntryDgEndInf->playAtk = cntry_end["data"]["monster_attack"].asInt();
        pCntryDgEndInf->gold = cntry_end["data"]["gold"].asInt();
        pCntryDgEndInf->wealth = cntry_end["data"]["wealth"].asInt();
        pCntryDgEndInf->contribution = cntry_end["data"]["contribution_award"].asInt();
        
        pCntryDgEndInf->lstItem.clear();
        
        Json::Value itmDic = cntry_end["data"]["item_award_list"];
        if (!itmDic.empty()) {
            Json::Value::Members itmIdArr = itmDic.getMemberNames();
            
            for (int i = 0; i < itmIdArr.size(); i++) {
                ItemCardInf TtlInf;
                TtlInf.strItemId = itmIdArr[i];
                TtlInf.lCnt = itmDic[TtlInf.strItemId]["num"].asInt();
                TtlInf.strResName = itmDic[TtlInf.strItemId]["img"].asString();
                
                pCntryDgEndInf->lstItem.push_back(TtlInf);
            }
        }
    }
    
    return pCntryDgEndInf;
}

string CGameData::getCurDgType()
{
    return m_curDgType;
}

int CGameData::getCurFloor()
{
    return m_iCurFloor;
}

int CGameData::getCurRoom()
{
    return m_iCurRoom;
}

void CGameData::initAfterLogin()
{
    CUserInfo* pUsrInf = getUsrInfo();
    
    if (pUsrInf->login_time > 0){
        //初始化信箱
        MailBoxAgt::Inst()->initMailBox(m_usrInfo.uid);
    }

    // 初始化 push
    isCntryCdRemind = CGameData::Inst()->getUsrInfo()->push_guild_charge_cold;
    isActivityRemind = CGameData::Inst()->getUsrInfo()->push_special_dunegon;
    isTwoUpRemind = CGameData::Inst()->getUsrInfo()->push_double_drop;
    intPowerRemind = CGameData::Inst()->getUsrInfo()->push_stamina_num;
    
    //初始化当前编队
    bool b_haveLocalData = true;
    
    string str = "CurTeamIdxOf";
    str.append(m_usrInfo.uid);
    
    m_iCurTeamIdx = CCUserDefault::sharedUserDefault()->getIntegerForKey(str.c_str(), 0);
    m_iCurTeamIdx = ((m_iCurTeamIdx >= 0) && (m_iCurTeamIdx < kMaxTeamCnt))? m_iCurTeamIdx : 0;
    
    char infokey1[130];
    sprintf(infokey1, "TeamInfo0");
    strcat(infokey1, CGameData::Inst()->getUsrInfo()->uid);
    
    const char* teamInfo0 = OcProxy::Inst()->getValueForKey(infokey1);
    
    if ((teamInfo0 == NULL) || (strlen(teamInfo0) == 0))
    {
        b_haveLocalData = false;
    }
    
    if (b_haveLocalData)
    {
        //init all team info
        for (int i=0; i<kMaxTeamCnt-1; i++)
        {
            for (int j=0; j<kMaxMemCntOfTeam-1; j++)
            {
                char infokey[130];
                sprintf(infokey, "TeamInfo%d",i*kMaxMemCntOfTeam+j);
                strcat(infokey, CGameData::Inst()->getUsrInfo()->uid);
                
                strncpy(m_TeamInf[i].mem[j].uid, OcProxy::Inst()->getValueForKey(infokey), 99);
                
                CUserCardInfo *card = getUsrCardInfByUcid((char*)OcProxy::Inst()->getValueForKey(infokey));
                if(card == NULL)
                {
                    m_TeamInf[i].mem[j].cost = 0;
                    m_TeamInf[i].mem[j].uid[0] = 0;
                }
                else
                {
                    m_TeamInf[i].mem[j].cost = card->cost;
                }
            }
            
            if (strlen( m_TeamInf[i].mem[0].uid) == 0)
            {
                strncpy(m_TeamInf[i].mem[0].uid, m_TeamInf[m_iCurTeamIdx].mem[0].uid, 99);
                char infokey[130];
                sprintf(infokey, "TeamInfo%d",i*kMaxMemCntOfTeam+0);
                strcat(infokey, CGameData::Inst()->getUsrInfo()->uid);
                OcProxy::Inst()->setValueForKey(m_TeamInf[i].mem[0].uid, infokey);
            }
        }
    }
    
    else
    {
        Json::Value array;
        if(isNewbie())
        {
            array = HttpComm::Inst()->getNewbieDic()["user_decks"];
            m_iCurTeamIdx = HttpComm::Inst()->getNewbieDic()["deck_index"].asInt();
        }
        else
        {
            array = HttpComm::Inst()->getLoginDic()["user_decks"];
            m_iCurTeamIdx = HttpComm::Inst()->getLoginDic()["deck_index"].asInt();
        }
        
        for (int i =0; i< array.size(); i++)
        {
            Json::Value oneTeam = array[i];
            
            for (int j=0; j < oneTeam.size(); j++)
            {
                strncpy(m_TeamInf[i].mem[j].uid, oneTeam[j].asString().c_str(), 99);
                
                CUserCardInfo *card = getUsrCardInfByUcid((char*)oneTeam[j].asString().c_str());
                if(card == NULL)
                {
                    m_TeamInf[i].mem[j].cost = 0;
                    m_TeamInf[i].mem[j].uid[0] = 0;
                }
                else
                    m_TeamInf[i].mem[j].cost = card->cost;
            }
        }
    }
    
    
    for (int i=0; i<kMaxTeamCnt-1; i++)
    {
        for (int j=0; j<kMaxMemCntOfTeam-1; j++)
        {
            if (strlen( m_TeamInf[i].mem[0].uid) == 0)
            {
                strncpy(m_TeamInf[i].mem[0].uid, m_TeamInf[m_iCurTeamIdx].mem[0].uid, 99);
                char infokey[130];
                sprintf(infokey, "TeamInfo%d",i*kMaxMemCntOfTeam+0);
                strcat(infokey, CGameData::Inst()->getUsrInfo()->uid);
                OcProxy::Inst()->setValueForKey(m_TeamInf[i].mem[0].uid, infokey);
            }
        }
    }
}

int CGameData::getCurTeamIdx()
{
    return m_iCurTeamIdx;
}

void CGameData::saveAllTeamInfo()
{
    string str = "CurTeamIdxOf";
    str.append(m_usrInfo.uid);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey(str.c_str(), m_iCurTeamIdx);
    CCUserDefault::sharedUserDefault()->flush();
    
    for (int i=0; i<kMaxTeamCnt-1; i++)
    {
        for (int j=0; j<kMaxMemCntOfTeam-1; j++)
        {
            char infokey[130] = {0};
            sprintf(infokey, "TeamInfo%d",i*kMaxMemCntOfTeam+j);
            strcat(infokey, CGameData::Inst()->getUsrInfo()->uid);
            
            OcProxy::Inst()->removeObjectForKey(infokey);
        }
    }
}

void CGameData::setCurTeamIdx(int idx)
{
    m_iCurTeamIdx = idx;
}

stUid* CGameData::getCurTeamInf()
{
    return m_TeamInf[m_iCurTeamIdx].mem;
}

bool CGameData::getIsSameCardInTeam()
{
    for (int i = 0; i < kMaxTeamCnt - 2; i++) {
        for (int j = 0; j < kMaxMemCntOfTeam - 1; j++) {
            if (m_TeamInf[i].mem[j].uid[0] == 0)
            {   // 没有武将
                continue;
            }
            
            for (int a = i + 1; a < kMaxMemCntOfTeam - 1; a++) {
                for (int b = 0; b < kMaxMemCntOfTeam - 1; b++) {
                    if (m_TeamInf[a].mem[b].uid[0] == 0)
                    {   // 没有武将
                        continue;
                    }
                    
                    if (strncmp(m_TeamInf[i].mem[j].uid, m_TeamInf[a].mem[b].uid, 99) == 0)
                    {   // 有相同的武将
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

stUid* CGameData::getTeamInfByIdx(int idx)
{
    return m_TeamInf[idx].mem;
}

int CGameData::getCurTeamCost()
{
    int currentCost = 0;
    for (int i = 0; i < kMaxMemCntOfTeam; i++) {
        currentCost += m_TeamInf[m_iCurTeamIdx].mem[i].cost;
    }
    
    return currentCost;
}

void CGameData::setTeamInfo(int iTeamIdx, int iMemIdx, char *ucid)
{
    iTeamIdx = ((iTeamIdx >= 0) && (iTeamIdx < kMaxTeamCnt))? iTeamIdx : m_iCurTeamIdx;
    
    strncpy(m_TeamInf[iTeamIdx].mem[iMemIdx].uid, ucid, 99);
    
    if (iTeamIdx == kTmpTeamIdx) {
        return;
    }
    
    CUserCardInfo *card = getUsrCardInfByUcid(ucid);
    if(card == NULL)
        m_TeamInf[iTeamIdx].mem[iMemIdx].cost = 0;
    else
        m_TeamInf[iTeamIdx].mem[iMemIdx].cost = card->cost;
}

void CGameData::updateTeamInfo()
{
    for (int i = 0; i < kMaxMemCntOfTeam; i++) {
        if((strncmp(m_TeamInf[m_iCurTeamIdx].mem[i].uid, m_CombineQueue[0].uid, 99)) == 0) {
            CUserCardInfo *card = getUsrCardInfByUcid(m_CombineQueue[0].uid);
            if(card == NULL)
                m_TeamInf[m_iCurTeamIdx].mem[i].cost = 0;
            else
                m_TeamInf[m_iCurTeamIdx].mem[i].cost = card->cost;
        }
    }
}

bool CGameData::isOneOfTeam(char* ucid)
{
    if (ucid == 0) {
        return false;
    }
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < kMaxMemCntOfTeam; j++) {
            if((strncmp(m_TeamInf[i].mem[j].uid, ucid, 99)) == 0) {
                return true;
            }
        }
    }
    
    return false;
}

unsigned short CGameData::getCurCostSum()
{
    unsigned short ushSum = 0;
    if (stUid *pTeamInfo = getTeamInfByIdx(getCurTeamIdx()))
    {
        for (int i = 0; i < kMaxTeamCnt-1; ++i)
        {
            if (CUserCardInfo* pUsrCardInfo = getUsrCardInfByUcid(pTeamInfo[i].uid))
            {
                ushSum += pUsrCardInfo->cost;
            }
        }
    }
    return ushSum;
}

unsigned short CGameData::getMaxCostSum()
{
    if (CUserInfo *pUsrInfo = getUsrInfo())
    {
        return pUsrInfo->cost;
    }
    return 0;
}

unsigned short CGameData::getTeamMaxStar()
{
    unsigned short ushMaxStar = 0;
    if (stUid *pTeamInfo = getTeamInfByIdx(getCurTeamIdx()))
    {
        for (int i = 0; i < kMaxTeamCnt-1; ++i)
        {
            if (CUserCardInfo* pUsrCardInfo = getUsrCardInfByUcid(pTeamInfo[i].uid))
            {
                ushMaxStar = MAX(pUsrCardInfo->star,ushMaxStar);
            }
        }
    }
    return ushMaxStar;
}

unsigned short CGameData::getFriendMaxStar()
{
    //CGameData::Inst()->getFriendHelpArr();
    if (CUserCardInfo *pUsrCardInfo = getHelperCardInfo(0))
    {
        return pUsrCardInfo->star;
    }
    return 0;
}


bool CGameData::isInTeamForIdx(char* ucid, int idx)
{
    if (ucid == 0)
    {
        return false;
    }
    

    for (int j = 0; j < kMaxMemCntOfTeam; j++)
    {
        if((strncmp(m_TeamInf[idx].mem[j].uid, ucid, 99)) == 0)
        {
                return true;
        }
    }

    
    return false;
}

void CGameData::resetTeam(int iTeamIdx)
{
    iTeamIdx = ((iTeamIdx >= 0) && (iTeamIdx < kMaxTeamCnt))? iTeamIdx : m_iCurTeamIdx;
    
    for (int j = 1; j < kMaxMemCntOfTeam; j++) {
        m_TeamInf[iTeamIdx].mem[j].uid[0] = 0;
        m_TeamInf[iTeamIdx].mem[j].cost = 0;
    }
}

bool CGameData::hasSameUcidInTeamInfo()
{
    for (int i = 0; i < 4; i++) {
        char* tempUcid = m_TeamInf[m_iCurTeamIdx].mem[i].uid;
        if (tempUcid[0] != 0) {
            for (int j = i + 1; j < 5; j++) {
                char* tempUcid2 = m_TeamInf[m_iCurTeamIdx].mem[j].uid;
                if (strncmp(tempUcid, tempUcid2, 99) == 0) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void CGameData::setUseTmpTeam(bool bUse)
{
    m_bUseTmpTeam = bUse;
    
    if (!bUse) {
        for (int j = 0; j < kMaxMemCntOfTeam; j++) {
            m_TeamInf[kTmpTeamIdx].mem[j].uid[0] = 0;
            m_TeamInf[kTmpTeamIdx].mem[j].cost = 0;
        }
        
        m_strCostCoin = "";
    }
}

bool CGameData::getIsUseTmpTeam()
{
    return m_bUseTmpTeam;
}

stUid* CGameData::getTmpTeamInf()
{
    return m_TeamInf[kTmpTeamIdx].mem;
}

void CGameData::setPreCmtTeamInfo(int iMemIdx, char *ucid, int cost)
{
    strncpy(m_preCmtTeam.mem[iMemIdx].uid, ucid, 99);
    m_preCmtTeam.mem[iMemIdx].cost = cost;
}

void CGameData::clrPreCmtTeamInfo()
{
    memset(&m_preCmtTeam, 0, sizeof(m_preCmtTeam));
}

void CGameData::setCostCoin(long lCostCoin)
{
    char buf[16];
    
    snprintf(buf, 15, "%ld", lCostCoin);
    
    m_strCostCoin = buf;
}

bool CGameData::needToUpdateCfg()
{
    return HttpComm::Inst()->isNeedToUpdateCfg();
}

bool CGameData::needToUpdateCard()
{
    return HttpComm::Inst()->isNeedToUpdateCard();
}

bool CGameData::needToUpdateMonster()
{
    return HttpComm::Inst()->isNeedToUpdateMonster();
}

int CGameData::getSpecialDgTimes(int iFloorId)
{
    int iRet = 0;
    
    char buf[10];
    snprintf(buf, 9, "%d", iFloorId);
    
    Json::Value usrdic = HttpComm::Inst()->getUsrInfoDic();
    if (!usrdic.empty()) {
        if (usrdic.isMember("specialdg_times")) {
            if (usrdic["specialdg_times"].isMember(buf)) {
                iRet = usrdic["specialdg_times"][buf].asInt();
            }
        }
    }
    
    return iRet;
}

ItemCardInf* CGameData::getItemCardByID(string strItemId)
{
    ItemCardInf* pItemCard = new ItemCardInf();
    pItemCard->autorelease();
    
    Json::Value gldCfgDic = HttpComm::Inst()->getCfgDic()["guild_config"];
    if (gldCfgDic.empty())
    {
        pItemCard->strResName = "";
        pItemCard->strType = "";
        pItemCard->strItemName = "";
        pItemCard->strItemDsp = "";
        pItemCard->lNeedLv = 0;
        pItemCard->lNeedCbu = 0;
        return pItemCard;
    }
    
    Json::Value itmCfgDic = gldCfgDic["guild_item_config"];
    if (!itmCfgDic.empty())
    {
        if (itmCfgDic.isMember(strItemId))
        {
            pItemCard->strResName = itmCfgDic[strItemId]["itemid"].asString();
            pItemCard->strType = itmCfgDic[strItemId]["type"].asString();
            pItemCard->strItemName = itmCfgDic[strItemId]["content"].asString();
            pItemCard->strItemDsp = itmCfgDic[strItemId]["item_dsp"].asString();
//            pItemCard->lNeedLv = itmCfgDic[strItemId]["need_lv"].asInt();
//            pItemCard->lNeedCbu = itmCfgDic[strItemId]["need_cbu"].asInt();
            if (itmCfgDic[strItemId].isMember("need_lv"))
                pItemCard->lNeedLv = itmCfgDic[strItemId]["need_lv"].asInt();
            else pItemCard->lNeedLv = 0;
            if (itmCfgDic[strItemId].isMember("need_cbu"))
                pItemCard->lNeedCbu = itmCfgDic[strItemId]["need_cbu"].asInt();
            else pItemCard->lNeedCbu = 0;
            
            
//            if (itmCfgDic[strItemId].isMember("cost_up_time"))
//                pItemCard->cost_up_time = itmCfgDic[strItemId]["cost_up_time"].asInt();
//            else pItemCard->cost_up_time = 0;
//            if (itmCfgDic[strItemId].isMember("cost_up_num"))
//                pItemCard->cost_up_num = itmCfgDic[strItemId]["cost_up_num"].asInt();
//            else pItemCard->cost_up_num = 0;
            
            
        }
        else{
            pItemCard->strResName = "";
            pItemCard->strType = "";
            pItemCard->strItemName = "";
            pItemCard->strItemDsp = "";
            pItemCard->lNeedLv = 0;
            pItemCard->lNeedCbu = 0;
//            pItemCard->cost_up_time = 0;
//            pItemCard->cost_up_num
        }
    }

    return pItemCard;
}


CCMutableArray<ItemCardInf* > * CGameData::getItemCardTotalInf()
{
    static long slCntryInfoObtainTm = 0;
    static long slCfgDicUpdateTm = 0;
    
    bool bUpdate = false;
    
    if(HttpComm::Inst()->getDicTime(enDicCfg) > slCfgDicUpdateTm){
        bUpdate = true;
        slCfgDicUpdateTm = HttpComm::Inst()->getDicTime(enDicCfg);
    }
    else if (slCntryInfoObtainTm < HttpComm::Inst()->getDicTime(enDicCntryInf)) {
        bUpdate = true;
        slCntryInfoObtainTm = HttpComm::Inst()->getDicTime(enDicCntryInf);
    }
    
    if (!bUpdate) {
        return m_pItmTtlInf;
    }
    
    Json::Value cntryDic = HttpComm::Inst()->getCntryInfoDic();
    if (cntryDic.empty()) {
        return m_pItmTtlInf;
    }
    
    Json::Value gldCfgDic = HttpComm::Inst()->getCfgDic()["guild_config"];
    if (gldCfgDic.empty()) {
        return m_pItmTtlInf;
    }
    
    m_pItmTtlInf->removeAllObjects();

    Json::Value usrInfoDic = HttpComm::Inst()->getUsrInfoDic();
    if (!usrInfoDic.empty()) {
        Json::Value itmDic = usrInfoDic["guild_item_record"];
        
        CCLog("card info = %s", itmDic.toStyledString().c_str());
        
        
        if (!itmDic.empty()) {
            Json::Value::Members itmIdArr = itmDic.getMemberNames();
            
            for (int i = 0; i < itmIdArr.size(); i++) {
                ItemCardInf* pTtlInf = new ItemCardInf();
                pTtlInf->strItemId = itmIdArr[i];
                pTtlInf->lCnt = itmDic[pTtlInf->strItemId].asInt();
                
                if (pTtlInf->lCnt == 0) {
                    pTtlInf->release();
                    continue;
                }

                Json::Value itmCfgDic = gldCfgDic["guild_item_config"];
                if (!itmCfgDic.empty())
                {
                    if (itmCfgDic.isMember(pTtlInf->strItemId))
                    {
                        pTtlInf->strResName = itmCfgDic[pTtlInf->strItemId]["itemid"].asString();
                        pTtlInf->strType = itmCfgDic[pTtlInf->strItemId]["type"].asString();
                        pTtlInf->strItemName = itmCfgDic[pTtlInf->strItemId]["content"].asString();
                        pTtlInf->strItemDsp = itmCfgDic[pTtlInf->strItemId]["item_dsp"].asString();
//                        pTtlInf->lNeedLv = itmCfgDic[pTtlInf->strItemId]["need_lv"].asInt();
//                        pTtlInf->lNeedCbu =itmCfgDic[pTtlInf->strItemId]["need_cbu"].asInt();
                        if (itmCfgDic[pTtlInf->strItemId].isMember("need_lv"))
                            pTtlInf->lNeedLv = itmCfgDic[pTtlInf->strItemId]["need_lv"].asInt();
                        else pTtlInf->lNeedLv = 0;
                        if (itmCfgDic[pTtlInf->strItemId].isMember("need_cbu"))
                            pTtlInf->lNeedCbu =itmCfgDic[pTtlInf->strItemId]["need_cbu"].asInt();
                        else pTtlInf->lNeedCbu = 0;
                        
//                        if (itmCfgDic[pTtlInf->strItemId].isMember("cost_up_time"))
//                            pTtlInf->cost_up_time = itmCfgDic[pTtlInf->strItemId]["cost_up_time"].asInt();
//                        else pTtlInf->cost_up_time = 0;
//                        if (itmCfgDic[pTtlInf->strItemId].isMember("cost_up_num"))
//                            pTtlInf->cost_up_num = itmCfgDic[pTtlInf->strItemId]["cost_up_num"].asInt();
//                        else pTtlInf->cost_up_num = 0;
                        
                        
                    }
                    else
                    {
                        pTtlInf->strResName = "";
                        pTtlInf->strType = "";
                        pTtlInf->strItemName = "";
                        pTtlInf->strItemDsp = "";
                        pTtlInf->lNeedLv = 0;
                        pTtlInf->lNeedCbu = 0;
//                        pTtlInf->cost_up_time = 0;
//                        pTtlInf->cost_up_num = 0;
                    }
                }

                m_pItmTtlInf->addObject(pTtlInf);
                pTtlInf->release();
            }
        }
    }
    
    return m_pItmTtlInf;
}

GiftInfo *CGameData::getGiftInfo()
{
    GiftInfo* pGiftInf = new GiftInfo();
    pGiftInf->autorelease();
    
    Json::Value giftDic = HttpComm::Inst()->getGiftDic();
    
    snprintf(pGiftInf->name, 99, "%s", giftDic["name"].asString().c_str());
    
    pGiftInf->gacha_pt = 0;
    if (giftDic.isMember("gacha_pt"))
        pGiftInf->gacha_pt = giftDic["gacha_pt"].asInt();
    
    pGiftInf->coin = 0;
    if (giftDic.isMember("coin"))
        pGiftInf->coin = giftDic["coin"].asInt();
    
    pGiftInf->gold = 0;
    if (giftDic.isMember("gold"))
        pGiftInf->gold = giftDic["gold"].asInt();
    
    pGiftInf->wealth = 0;
    if (giftDic.isMember("wealth"))
        pGiftInf->wealth = giftDic["wealth"].asInt();
    
    pGiftInf->strUcidList.clear();
    if (giftDic.isMember("new_cards"))
    {
        Json::Value cardsArr = giftDic["new_cards"];
        
        for (int i = 0; i < cardsArr.size(); i++) {
            string strUcid = cardsArr[i]["ucid"].asString();
            pGiftInf->strUcidList.push_back(strUcid);
        }
    }
    
    if (giftDic.isMember("guild_item"))
    {
        Json::Value guildItem = giftDic["guild_item"];
        Json::Value::Members itemtemp = guildItem.getMemberNames();
        for (int i = 0; i < (int)itemtemp.size(); ++i)
        {
            std::string szName;
            unsigned int unCount = 0;
            if (!itemtemp[i].empty())
            {
                if (ItemCardInf *pItemCardInfo = getItemCardByID(itemtemp[i]))
                {
                    szName = pItemCardInfo->strItemName;
                }
            }
            if (guildItem[itemtemp[i]].isMember("num"))
            {
                unCount = guildItem[itemtemp[i]]["num"].asUInt();
            }
            if (!szName.empty() && 0<unCount)
            {
                pGiftInf->m_vrGuildItem.push_back(std::pair<std::string,unsigned int>(szName,unCount));
            }
        }
    }
    
    return pGiftInf;
}

CCMutableArray<ItemCardInf* > * CGameData::getItemCardPkInf()
{
    m_pItmPkInf->removeAllObjects();
    
    CCMutableArray<ItemCardInf* > * pTtlInfArr = getItemCardTotalInf();
    
    for (int i = 0; i < pTtlInfArr->count()  ; i++)
    {
        ItemCardInf* pTtlInf = pTtlInfArr->getObjectAtIndex(i  );
        long lCnt = pTtlInf->lCnt;
        
        //while (lCnt > 0) {
            ItemCardInf* pItmPkInf = new ItemCardInf();
            pItmPkInf->strItemId = pTtlInf->strItemId;
            pItmPkInf->strResName = pTtlInf->strResName;
            pItmPkInf->strType = pTtlInf->strType;
            pItmPkInf->strItemName = pTtlInf->strItemName;
            pItmPkInf->strItemDsp = pTtlInf->strItemDsp;
            pItmPkInf->lNeedLv = pTtlInf->lNeedLv;
            pItmPkInf->lNeedCbu = pTtlInf->lNeedCbu;
            
            pItmPkInf->lCnt = lCnt;//(lCnt > kItemPackLimitNum)? kItemPackLimitNum : lCnt;
            
           // lCnt -= pItmPkInf->lCnt;
        
//            pItmPkInf->cost_up_num = pTtlInf->cost_up_num;
//            pItmPkInf->cost_up_time = pTtlInf->cost_up_time;
        
            m_pItmPkInf->addObject(pItmPkInf);
            pItmPkInf->release();
        //}
    }
    
    return m_pItmPkInf;
}

UseItemAward* CGameData::getUseItemAward()
{
    Json::Value useItemAwardDic = HttpComm::Inst()->getUseItemDic();
    
    if (useItemAwardDic.empty()) {
        return NULL;
    }
    
    UseItemAward* pUseItemAward = new UseItemAward();
    pUseItemAward->autorelease();
    pUseItemAward->strUcidLst.clear();
    
    if (useItemAwardDic.isMember("coin")) {
        pUseItemAward->coin = useItemAwardDic["coin"].asInt();
    }
    
    if (useItemAwardDic.isMember("exp")) {
        pUseItemAward->exp = useItemAwardDic["exp"].asInt();
    }
    
    if (useItemAwardDic.isMember("gold")) {
        pUseItemAward->gold = useItemAwardDic["gold"].asInt();
    }
    
    if (useItemAwardDic.isMember("wealth")) {
        pUseItemAward->wealth = useItemAwardDic["wealth"].asInt();
    }
    
    if (useItemAwardDic.isMember("ucontribution")) {
        pUseItemAward->ucontribution = useItemAwardDic["ucontribution"].asInt();
    }
    
    if (useItemAwardDic.isMember("gcontribution")) {
        pUseItemAward->gcontribution = useItemAwardDic["gcontribution"].asInt();
    }
    
    if (useItemAwardDic.isMember("stamina")) {
        pUseItemAward->stamina = useItemAwardDic["stamina"].asInt();
    }
    
    if (useItemAwardDic.isMember("new_cards")) {
        Json::Value cardsArr = useItemAwardDic["new_cards"];
        
        for (int i = 0; i < cardsArr.size(); i++) {
            string strUcid = cardsArr[i]["ucid"].asString();
            pUseItemAward->strUcidLst.push_back(strUcid);
        }
    }
    
    pUseItemAward->cost_up = 0;
    if (useItemAwardDic.isMember("cost_up")) {
        pUseItemAward->cost_up = useItemAwardDic["cost_up"].asInt();
    }
    
    pUseItemAward->cost_time  = 0;
    if (useItemAwardDic.isMember("cost_time"))
    {
        pUseItemAward->cost_time = useItemAwardDic["cost_time"].asInt();
    }
    
    
    return pUseItemAward;
}


bool CGameData::reqTitleList()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->titleListReq();
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::reqSetTitleName(string title_id)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->setTitleNameReq(title_id);
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::getJiFengRecordReq()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->getJiFengRecordReq();
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::reqGuildItemShop()
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqGuildItemShop();
        bRet = true;
    }
    
    return bRet;
}

CCMutableArray<GuildShopItemInf*>* CGameData::getGuildShopItemInfArr()
{
    m_pGuildShopItemInfArr->removeAllObjects();
    
    Json::Value guildShopItemDic = HttpComm::Inst()->getGuildItemShopDic();
    if (!guildShopItemDic.empty()) {
        if (guildShopItemDic.isMember("item_shop")) {
            Json::Value itemArr = guildShopItemDic["item_shop"];
            
            for (int i = 0; i < itemArr.size(); i++) {
                GuildShopItemInf* pShopItem = new GuildShopItemInf();
                
                pShopItem->lCoin = itemArr[i]["coin"].asInt();
                pShopItem->lCntrbt = itemArr[i]["contribution"].asInt();
                pShopItem->lLv = itemArr[i]["lv"].asInt();
                pShopItem->strItemId = itemArr[i]["item_id"].asString();
                pShopItem->strItemName = itemArr[i]["name"].asString();
                pShopItem->strImgName = itemArr[i]["item_img"].asString();
                pShopItem->strItemDsp = itemArr[i]["item_dsp"].asString();
                
                m_pGuildShopItemInfArr->addObject(pShopItem);
                pShopItem->release();
            }
        }
    }
    
    return m_pGuildShopItemInfArr;
}

long CGameData::getGuildItemInStoreCnt(string strItemId)
{
    long lCnt = 0;
    Json::Value cntryUserDic = HttpComm::Inst()->getCntryInfoDic()["guild_user"];
    
    if (cntryUserDic.isMember("task_items")) {
        lCnt = cntryUserDic["task_items"][strItemId].asInt();
    }
    
    return lCnt;
}

bool CGameData::reqBuyGuildItem(string strItemId, long lNum)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqBuyGuildItem(strItemId, lNum);
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::reqGetAwardBonus(string bonus, string award_key, string loginDays)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqGetAwardBonus(bonus, award_key,loginDays);
        bRet = true;
    }
    
    return bRet;
}

bool CGameData::assignGuildCoin(string strUid, int iCoin)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->assignGuildCoin(strUid, iCoin);
        bRet = true;
    }
    
    return bRet;
}
bool CGameData::getOrderNo(string productId,string uid)
{
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->getOrderNo(productId, uid);
        bRet = true;
    }
    
    return bRet;
}
CCMutableArray<TitleInf* > * CGameData::getTitleInfArr()
{
    m_pTitleInfArr->removeAllObjects();
    
    Json::Value ttlLstDic = HttpComm::Inst()->getTitleListDic()["title_conf"];
    Json::Value::Members idArr = ttlLstDic.getMemberNames();
    string strEquip = getUsrInfo()->title_name;

    if (!ttlLstDic.empty()){
        for (int i = 0; i < ttlLstDic.size(); i++) {
            TitleInf* pTitleInf = new TitleInf();
            
            pTitleInf->strTitleId = idArr[i];
            
            Json::Value oneTtlInf = ttlLstDic[pTitleInf->strTitleId];
            
            pTitleInf->strTitleName = oneTtlInf["name"].asString();
            pTitleInf->strTitleDsp = oneTtlInf["title_dsp"].asString();
            pTitleInf->strTitleDspEx = oneTtlInf["title_dsp_ex"].asString();
            int max_task_num = oneTtlInf["max_task_num"].asInt();
            int task_num = oneTtlInf["task_num"].asInt();

            if (oneTtlInf["pass"].asBool()) {
                if (strEquip == pTitleInf->strTitleName) {
                    pTitleInf->iEquipSt = 2;
                }
                else{
                    pTitleInf->iEquipSt = 1;
                }
            }
            else{
                pTitleInf->iEquipSt = 0;
            }
            
            char buf[16];
            snprintf(buf, sizeof(buf), "(%d/%d)", task_num, max_task_num);
            pTitleInf->strTitleName.append(buf);
            

            Json::Value detailDic = oneTtlInf["detail"];
            Json::Value::Members detailIdArr = detailDic.getMemberNames();
            
            for (int i = 0 ; i < detailDic.size(); i++) {
                Json::Value oneDetail = detailDic[detailIdArr[i]];
                
                TitleQuestInf questinf;
                
                int max_num = oneDetail["max_num"].asInt();
                int num = oneDetail["num"].asInt();
                snprintf(buf, sizeof(buf), "(%d/%d)", num, max_num);
                
                questinf.strQuest = oneDetail["name"].asString();
                questinf.strQuest.append(buf);
                
                if (num >= max_num) {
                    questinf.bPass = true;
                }
                else{
                    questinf.bPass = false;
                }
                
                pTitleInf->TtlQstLst.push_back(questinf);
            }
            
            m_pTitleInfArr->addObject(pTitleInf);
            pTitleInf->release();
        }
    }
    
    return m_pTitleInfArr;
}



#pragma mark -
#pragma mark ChargeAward

ChargeAward::ChargeAward()
{
    m_iAwadrIds = NULL;
    m_iOidIds = NULL;
}

ChargeAward::~ChargeAward()
{
    if (m_iAwadrIds!= NULL)
    {
        delete []m_iAwadrIds;
        m_iAwadrIds = NULL;
    }
    
    if (m_iOidIds!= NULL)
    {
        delete []m_iOidIds;
        m_iOidIds = NULL;
    }
}

CCMutableArray<JiFengRecordInf* > * CGameData::getJiFengRecordArr()
{
    m_pJiFengRecordArr->removeAllObjects();
    
    Json::Value JiFengRecordDic = HttpComm::Inst()->getJiFengRecordDic()["hp"];
    if (JiFengRecordDic.empty() == false) {
        for (int i = 0; i < JiFengRecordDic.size(); i++) {
            Json::Value oneItem = JiFengRecordDic[i];
            
            JiFengRecordInf* pRecordInf = new JiFengRecordInf();
            pRecordInf->strUid = oneItem[(unsigned int)0].empty()? "": oneItem[(unsigned int)0].asString();
            pRecordInf->lRecentScore = oneItem[1].empty()? 0: oneItem[1].asInt();
            pRecordInf->lLastScore = oneItem[2].empty()? 0: oneItem[2].asInt();
            pRecordInf->strName = oneItem[3].empty()? "": oneItem[3].asString();
            
            m_pJiFengRecordArr->addObject(pRecordInf);
            pRecordInf->release();
        }
    }

    return m_pJiFengRecordArr;
}

long CGameData::getGuildCoin()
{
    return HttpComm::Inst()->getGuildCoin();
}

void CGameData::checkIsUseExternal()
{
    if (!chkLogin())
        return;
    
    if (m_commonInfo.list_external.size() == 0)
        return;
    
    for (list<string>::iterator itEt = m_commonInfo.list_external.begin();
         itEt != m_commonInfo.list_external.end();
         ++itEt) {
        string strExternal = *itEt;
        if (OcProxy::Inst()->checkIsUseExternal(strExternal.c_str())){}
//        exit(0);
    }
}

bool CGameData::GetUploadAward(ConfigData data)
{
    bool rt = false;
    
    int req = HttpComm::Inst()->getReqStatus();
    if(req != kReqStatRequesting){
        HttpComm::Inst()->GetUploadAward(data);
        rt = true;
    }
    return rt;
}

string CGameData::GetUploadedAward()
{
    Json::Value value = HttpComm::Inst()->GetUploadedAward();
    
    string rt = value["msg"].asString();
    return rt;
}

string CGameData::GetFightName(int floor_id, int room_id, const char* type, int floor_index)
{
    string rt;
    if (strcmp(type, "normal") == 0)
    {
        CDgRoomInfo* info = m_pNmlDgArr->getObjectAtIndex(floor_id-1)->m_pRoomArr->getObjectAtIndex(room_id-1);
        rt = info->name;
    }
    else if (strcmp(type, "special") == 0 || strcmp(type, "weekly") == 0)
    {
        CDgRoomInfo* info = m_pSpecialDgArr->getObjectAtIndex(floor_index)->m_pRoomArr->getObjectAtIndex(room_id-1);
        rt = info->name;
    }
    else if (strcmp(type, "sgyy") == 0)
    {
        CDgRoomInfo* info = m_pSangoYYArr->getObjectAtIndex(floor_id-1)->m_pRoomArr->getObjectAtIndex(room_id-1);
        rt = info->name;
    }
    return rt;
}

bool CGameData::ReqOtherRecord(const char* dungeon_id, const char* type)
{
    bool rt = false;
    int req = HttpComm::Inst()->getReqStatus();
    if(req != kReqStatRequesting){
        HttpComm::Inst()->ReqOtherRecord(dungeon_id, type);
        rt = true;
    }
    return rt;
}

string CGameData::GetOtherRecordUrlVid()
{
    Json::Value js = HttpComm::Inst()->GetOtherRecordUrlVid();
    string rt = js["vid"]["video_url"].asString();
    return rt;
}

string CGameData::getFirstXChars(string strInput, uint16_t uiCnt)
{
    string strRet("");
    char buf[16];
    
    const char* pPos = strInput.c_str();
    while (uiCnt > 0) {
        uint16_t uiOneCharLen = 0;
        char curChar = *pPos;
        
        if ((curChar & 0x80) == 0) {
            uiOneCharLen = 1;
        }
        else{
            do{
                uiOneCharLen++;
                curChar <<= 1;
            }while ((curChar & 0x80) == 0x80);
        }
        
        memcpy(buf, pPos, uiOneCharLen);
        buf[MIN(uiOneCharLen, 16)] = 0;
        
        strRet.append(buf);
        
        uiCnt--;
        pPos += uiOneCharLen;
    }
    
    return strRet;
}

void CGameData::addBigImageByRGBA4444(const char *fileName)
{
    CCTexture2DPixelFormat oldFormat = CCTexture2D::defaultAlphaPixelFormat();
    CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fileName);
    CCTexture2D::setDefaultAlphaPixelFormat(oldFormat);
}

string CGameData::getPPBilloNo()
{
    Json::Value billoNoDic = HttpComm::Inst()->getPPBilloNoDic();
    string billoNo = (billoNoDic)["billno"].asString();
    return billoNo;
}


bool CGameData::reqPPBilloNo(const char* item_id)
{
    CCLog("itemt id  %s ",item_id);
    bool bRet = false;
    
    int reqstat = HttpComm::Inst()->getReqStatus();
    
    if(reqstat != kReqStatRequesting){
        HttpComm::Inst()->reqPPBilloNo(item_id);
        bRet = true;
    }
    return bRet;
}







