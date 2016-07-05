//
//  OrgSaveData.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-2-19.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgSaveData.h"
#include "CGameData.h"
#include "md5.h"
#include "OcProxy.h"

OrgSaveData* OrgSaveData::s_pcIf = NULL;

//******************************************************************************
// 加密解密
//******************************************************************************
static char gRpTable1[64] = {
    60, 54, 35, 58, 53, 9,  16, 59,
    12, 15, 45, 4,  27, 55, 50, 23,
    37, 19, 47, 56, 33, 32, 25, 48,
    44, 13, 43, 62, 2,  34, 21, 5,
    40, 52, 39, 20, 31, 24, 36, 17,
    46, 49, 61, 3,  10, 38, 41, 63,
    14, 22, 8,  57, 51, 6,  7,  1,
    42, 30, 26, 0,  29, 28, 11, 18
};

static char gRpTable2[64] = {
    59, 55, 28, 43, 11, 31, 53, 54,
    50,  5, 44, 62,  8, 25, 48,  9,
    6, 39, 63, 17, 35, 30, 49, 15,
    37, 22, 58, 12, 61, 60, 57, 36,
    21, 20, 29,  2, 38, 16, 45, 34,
    32, 46, 56, 26, 24, 10, 40, 18,
    23, 41, 14, 52, 33,  4,  1, 13,
    19, 51,  3,  7,  0, 42, 27, 47
};

static void rp_enc(unsigned char* pBuf, int iBufLen)
{
    int i = 0;
    
    while (i < iBufLen) {
        
        if(pBuf[i] < 64){
            pBuf[i] = gRpTable1[pBuf[i]];
        }
        
        i++;
    }
}

static void rp_dec(unsigned char* pBuf, int iBufLen)
{
    int i = 0;
    
    while (i < iBufLen) {
        
        if(pBuf[i] < 64){
            pBuf[i] = gRpTable2[pBuf[i]];
        }
        
        i++;
    }
}

//******************************************************************************
// 插入,提取md
//******************************************************************************
static int mdpos[16] = {
//    10, 55, 88, 123,
//    234, 456, 666, 678,
//    711, 734, 792, 992,
//    1008, 1872, 2133, 2231
    10, 35, 67, 88,
    124, 125, 136, 137,
    248, 259, 260, 291,
    310, 383, 384, 400,
};

static void insertmd(char* pBuf, int iBufLen, const unsigned char* pMd)
{
    int iTotalSize = iBufLen + 16;
    char *pTmpBuf = (char*) malloc(iTotalSize);
    
    if(pTmpBuf){
        int iMdIdx = 0;
        char* pBufPos = pBuf;
        
        for(int i = 0; i < iTotalSize; i++){
            if((iMdIdx < 16) && (i == mdpos[iMdIdx])){
                pTmpBuf[i] = pMd[iMdIdx];
                iMdIdx++;
            }
            else{
                pTmpBuf[i] = *pBufPos;
                pBufPos++;
            }
        }
        
        memcpy(pBuf, pTmpBuf, iTotalSize);
        free(pTmpBuf);
    }
}

static void pickupmd(char* pBuf, int iBufLen, unsigned char* pMd)
{
    char *pTmpBuf = (char*) malloc(iBufLen);
    
    if(pTmpBuf){
        int iMdIdx = 0;
        int iTmpBufIdx = 0;
        
        for(int i = 0; i < iBufLen; i++){
            if((iMdIdx < 16) && (i == mdpos[iMdIdx])){
                pMd[iMdIdx] = pBuf[i];
                iMdIdx++;
            }
            else{
                pTmpBuf[iTmpBufIdx] = pBuf[i];
                iTmpBufIdx++;
            }
        }
        
        memcpy(pBuf, pTmpBuf, iBufLen);
        free(pTmpBuf);
    }
}

//******************************************************************************
// init
//******************************************************************************
void OrgSaveData::init()
{
    snprintf(m_filePath, 999, "%s/003.bin", CCFileUtils::getWriteablePath().c_str());
    snprintf(m_tempFilePath, 999, "%s/004.bin", CCFileUtils::getWriteablePath().c_str());
    
    FILE* pFile = fopen(m_filePath, "rb");
    
    if(pFile == NULL){
        pFile = fopen(m_tempFilePath, "rb");
    }
    
    if(pFile){
        fseek(pFile, 0, SEEK_END); //文件指针移到文件尾
        int nLen = ftell(pFile);  //得到当前指针位置, 即是文件的长度
        rewind(pFile);
        
        char *pchBuf = (char*) malloc(sizeof(char)*nLen+1);
        if(pchBuf){
            //读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
            nLen = fread(pchBuf, sizeof(char), nLen, pFile);
            
            if(nLen > 16){
                unsigned char smd[16] = {0};
                
                //解密
                rp_dec((unsigned char*)pchBuf, nLen);
                
                //提取md5
                pickupmd(pchBuf, nLen, smd);
                
                MD5 md5;
                md5.update(pchBuf, (nLen - 16));
                const unsigned char* dmd = md5.digest();
                
                int icmp = memcmp(smd, dmd, 16);
                
                if(icmp == 0){
                    
                    int iSizeOfOrgFightInf = MIN(sizeof(SaveData_OrgFightInf), nLen - 16) ;
                    memcpy(&m_OrgFightInf, pchBuf, iSizeOfOrgFightInf);
                                        
                    m_bValid = true;
                }
                else{
                    m_bValid = false;
                }
            }
            else{
                m_bValid = false;
            }
            
            free(pchBuf);
        }
        else{
            m_bValid = false;
        }
        
        fclose(pFile);
    }
    else{
        m_bValid = false;
    }
    
    m_bUseSaveData = m_bValid;
}

//******************************************************************************
// destorySavedFile
//******************************************************************************
void OrgSaveData::destorySavedFile()
{
    remove(m_filePath);
    
    m_bValid = false;
    m_bUseSaveData = false;
}

//******************************************************************************
// destorySavedData
//******************************************************************************
void OrgSaveData::destorySavedData()
{
    memset(&m_OrgFightInf, 0, sizeof(SaveData_OrgFightInf));
}

//******************************************************************************
// flush
//******************************************************************************
void OrgSaveData::flush()
{
    int idatasize = sizeof(SaveData_OrgFightInf);
    
    char *pchBuf = (char*) malloc(idatasize + 16);
    
    if(pchBuf){
        memcpy(pchBuf, &m_OrgFightInf, sizeof(SaveData_OrgFightInf));
        
        MD5 md5;
        md5.update(pchBuf, idatasize);
        
        const unsigned char* md = md5.digest();
        
        //md5分散插入
        insertmd(pchBuf, idatasize, md);
        
        //加密
        rp_enc((unsigned char*)pchBuf, idatasize + 16);
        
        FILE* pFile = fopen(m_tempFilePath, "wb");
        
        if(pFile){
            fwrite(pchBuf, idatasize + 16, 1, pFile);
            fclose(pFile);
            
            remove(m_filePath);
            rename(m_tempFilePath, m_filePath);
        }
        
        free(pchBuf);
    }
    
    m_bUseSaveData = false; // 如果备份文件中写入了最近的数据，战斗将不用从备份数据中获取数据
    CCLog("Save new fight data");
}

//******************************************************************************
// hasValidSavedData
//******************************************************************************
bool OrgSaveData::hasValidSavedData()
{
    return m_bValid;
}

//******************************************************************************
// getIsUseSaveData
//******************************************************************************
bool OrgSaveData::getIsUseSaveData()
{
    return m_bUseSaveData;
}

//******************************************************************************
// setIsUseSaveData
//******************************************************************************
void OrgSaveData::setIsUseSaveData(bool isUseSaveData)
{
    m_bUseSaveData = isUseSaveData;
}

//******************************************************************************
// initOrgFightInfo
//******************************************************************************
void OrgSaveData::initOrgFightInfo()
{
    if (m_bUseSaveData) {
        return;
    }
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    stUid* pTeamInfo = CGameData::Inst()->getCurTeamInf();
    CCMutableArray<CUserCardInfo*> * pUsrCardArr = CGameData::Inst()->getUserCards();
    
    for (int i = 0; i < 5; i++) {
        CUserCardInfo* pUsrCardInfo;
        
        const char* szUcid = pTeamInfo[i].uid;
        if (*szUcid == 0) {
            // 编队对应位置没有武将
            m_OrgFightInf.card_info[i].cid = 0;
            continue;
        }
        
        for(int i = 0; i < pUsrCardArr->count(); i++) {
            pUsrCardInfo = pUsrCardArr->getObjectAtIndex(i);
            
            if((strcmp(szUcid, pUsrCardInfo->ucid)) == 0)
                break;
        }
        
        OrgSaveData_Card *cardInfo = &(m_OrgFightInf.card_info[i]);
        cardInfo->cid = pUsrCardInfo->cid;
        cardInfo->attack = pUsrCardInfo->attack * (1 + cntryInfo->buffInfo[enBuffType_Attack].effect);
        cardInfo->hp = pUsrCardInfo->hp * (1 + cntryInfo->buffInfo[enBuffType_Hp].effect);
        cardInfo->element = pUsrCardInfo->ctype;
                
        snprintf(cardInfo->card_name, 31, "%s", pUsrCardInfo->name);
    }
    
    m_OrgFightInf.stamina_recover_time = CGameData::Inst()->getStaminaRecoverTime() * 60 * (1 - cntryInfo->buffInfo[enBuffType_Stamina].effect);
    m_OrgFightInf.acPoint_recover_time = CGameData::Inst()->getActionPointRecoverTime();
    
    CommonInfo* commonInfo = CGameData::Inst()->getCommonInfo();
    if (commonInfo->list_external.size() > 0)
    {
        for (list<string>::iterator itEn = commonInfo->list_external.begin();
             itEn != commonInfo->list_external.end();
             ++itEn) {
            string strExternal = *itEn;
            snprintf(m_OrgFightInf.external_list, 299, "%s%s,",
                     m_OrgFightInf.external_list, strExternal.c_str());
        }
    }
    
    if (CGameData::Inst()->getMD5Str()) {
        snprintf(m_OrgFightInf.md5_str, 63, "%s", CGameData::Inst()->getMD5Str());
    }
    
    if (CGameData::Inst()->getSessionId())
    {
        snprintf(m_OrgFightInf.session_id, 63, "%s", CGameData::Inst()->getSessionId());
    }
    
    if (CGameData::Inst()->getUserId()) 
    {
        snprintf(m_OrgFightInf.user_id, 63, "%s", CGameData::Inst()->getUserId());
    }
    
    m_OrgFightInf.combo_index = 0;
    
    m_OrgFightInf.bUseChristmasUI = CGameData::Inst()->getIsUseChristmasUI();
    
    for (int i = 0; i < kRandLstLen; i++) {
        m_OrgFightInf.m_cRandLst[i] = rand() % 100;
    }
    m_OrgFightInf.m_cRandIdx = 0;
}

//******************************************************************************
// setStone
//******************************************************************************
void OrgSaveData::setStone(int iRow, int iColumn, char elem)
{
    m_OrgFightInf.stonegrid[iRow][iColumn] = elem;
}

//******************************************************************************
// getStone
//******************************************************************************
char OrgSaveData::getStone(int iRow, int iColumn)
{
    return m_OrgFightInf.stonegrid[iRow][iColumn];
}

//******************************************************************************
// getComboIndex
//******************************************************************************
short OrgSaveData::getComboIndex()
{
    return m_OrgFightInf.combo_index;
}

//******************************************************************************
// setComboIndex
//******************************************************************************
void OrgSaveData::setComboIndex(short comboIndex)
{
    m_OrgFightInf.combo_index = comboIndex;
}

//******************************************************************************
// setComboCount
//******************************************************************************
void OrgSaveData::setMaxCombo(short comboCount)
{
    m_OrgFightInf.maxCombo = comboCount;
}

//******************************************************************************
// getComboCount
//******************************************************************************
short OrgSaveData::getMaxCombo()
{
    return m_OrgFightInf.maxCombo;
}


void OrgSaveData::setIsTongMengLianShouChang(bool bisTMLS)
{
    m_OrgFightInf.m_bisTMLS = bisTMLS;
}

bool OrgSaveData::getIsTongMengLianShouChang()
{
    return m_OrgFightInf.m_bisTMLS;
}

//******************************************************************************
// getMD5Str
//******************************************************************************
const char* OrgSaveData::getMD5Str()
{
    return m_OrgFightInf.md5_str;
}

//******************************************************************************
// getSessionId
//******************************************************************************
const char* OrgSaveData::getSessionId()
{
    return m_OrgFightInf.session_id;
}

//******************************************************************************
// getUserId
//******************************************************************************
const char* OrgSaveData::getUserId()
{
    return m_OrgFightInf.user_id;
}

//******************************************************************************
// getFightId
//******************************************************************************
const char* OrgSaveData::getFightId()
{
    return m_OrgFightInf.fight_id;
}

//******************************************************************************
// setFightId
//******************************************************************************
void OrgSaveData::setFightId(const char *fightId)
{
    snprintf(m_OrgFightInf.fight_id, 31, "%s", fightId);
}
//******************************************************************************
// getIsUseChristmasUI
//******************************************************************************
bool OrgSaveData::getIsUseChristmasUI()
{
    return m_OrgFightInf.bUseChristmasUI;
}

//******************************************************************************
// getFightType
//******************************************************************************
const char* OrgSaveData::getFightType()
{
    return m_OrgFightInf.fight_type;
}

//******************************************************************************
// setFightType
//******************************************************************************
void OrgSaveData::setFightType(const char *fightType)
{
    snprintf(m_OrgFightInf.fight_type, 31, "%s", fightType);
}

//******************************************************************************
// getFightName
//******************************************************************************
const char* OrgSaveData::getFightName()
{
    return m_OrgFightInf.fight_name;
}

//******************************************************************************
// setFightName
//******************************************************************************
void OrgSaveData::setFightName(const char *fightName)
{
    snprintf(m_OrgFightInf.fight_name, 31, "%s", fightName);
}

//******************************************************************************
// getEnemyCid
//******************************************************************************
short OrgSaveData::getEnemyCid()
{
    return m_OrgFightInf.enemy_cid;
}

//******************************************************************************
// setEnemyCid
//******************************************************************************
void OrgSaveData::setEnemyCid(short enemyCid)
{
    m_OrgFightInf.enemy_cid = enemyCid;
}

//******************************************************************************
// setEnemyCid
//******************************************************************************
OrgSaveData_Card* OrgSaveData::getCardInfo(int idx)
{
    return &(m_OrgFightInf.card_info[idx]);
}

//******************************************************************************
// getStaminaRecoverTime
//******************************************************************************
short OrgSaveData::getStaminaRecoverTime()
{
    return m_OrgFightInf.stamina_recover_time;
}

//******************************************************************************
// getAcPointRecoverTime
//******************************************************************************
short OrgSaveData::getAcPointRecoverTime()
{
    return m_OrgFightInf.acPoint_recover_time;
}

//******************************************************************************
// checkIsUseExternal
//******************************************************************************
void OrgSaveData::checkIsUseExternal()
{    
    if (strlen(m_OrgFightInf.external_list) == 0)
        return;
    
    string strExternal(m_OrgFightInf.external_list);
    string strSub;
    std::string::size_type posBegin(0);
    std::string::size_type posEnd(0);
    while (1) {
		if((posEnd = strExternal.find(",", posBegin)) != std::string::npos)
        {
            strSub = strExternal.substr(posBegin, posEnd - posBegin);
            
            if(OcProxy::Inst()->checkIsUseExternal(strSub.c_str())){}
                //exit(0);
            
            posBegin = posEnd + 1;
        }
        else
            break;
    }    

}

//******************************************************************************
// getrand
//******************************************************************************
unsigned char OrgSaveData::getrand()
{
    m_OrgFightInf.m_cRandIdx %= kRandLstLen;
    
    unsigned char ucRand = m_OrgFightInf.m_cRandLst[m_OrgFightInf.m_cRandIdx];
    
    m_OrgFightInf.m_cRandIdx++;
    
    return ucRand;
}


