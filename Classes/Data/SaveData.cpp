//
//  SaveData.mm
//  AgainstWar
//
//  Created by Zhang Hao on 12-10-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "SaveData.h"
#include "CSkillLib.h"
#include "md5.h"
#include "OcProxy.h"
#include "CSkillLib2.h"

#define kSaveDataFilename       @"001.bin"
#define kSaveDataTmpFilename    @"002.bin"

SaveData* SaveData::s_pcIf = NULL;

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
    10, 55, 88, 123,
    234, 456, 666, 678,
    711, 734, 792, 992,
    1008, 1872, 2133, 2231
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
void SaveData::init()
{
    snprintf(m_filePath, 999, "%s/001.bin", CCFileUtils::getWriteablePath().c_str());
    snprintf(m_tempFilePath, 999, "%s/002.bin", CCFileUtils::getWriteablePath().c_str());
    
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
                    
                    int iSizeOfStepInf = MIN(sizeof(SaveData_StepInf), nLen - 16) ;
                    memcpy(&m_StepInf, pchBuf, iSizeOfStepInf);
                    
                    int iSizeOfPlayerInf = MIN(sizeof(SaveData_PlayerInf), nLen - 16 - iSizeOfStepInf);
                    memcpy(&m_PlayerInf, pchBuf + iSizeOfStepInf, iSizeOfPlayerInf);
                    
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
void SaveData::destorySavedFile()
{
    remove(m_filePath);
    
    m_bValid = false;
    m_bUseSaveData = false;
}

//******************************************************************************
// destorySavedData
//******************************************************************************
void SaveData::destorySavedData()
{
    memset(&m_StepInf, 0, sizeof(SaveData_StepInf));
    memset(&m_PlayerInf, 0, sizeof(SaveData_PlayerInf));
}

//******************************************************************************
// flush
//******************************************************************************
void SaveData::flush()
{
    int idatasize = sizeof(SaveData_StepInf) + sizeof(SaveData_PlayerInf);
    
    char *pchBuf = (char*) malloc(idatasize + 16);
    
    if(pchBuf){
        memcpy(pchBuf, &m_StepInf, sizeof(SaveData_StepInf));
        memcpy(pchBuf+ sizeof(SaveData_StepInf), &m_PlayerInf, sizeof(SaveData_PlayerInf));

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
bool SaveData::hasValidSavedData()
{
    return m_bValid;
}

//******************************************************************************
// getIsUseSaveData
//******************************************************************************
bool SaveData::getIsUseSaveData()
{
    return m_bUseSaveData;
}

//******************************************************************************
// setIsUseSaveData
//******************************************************************************
void SaveData::setIsUseSaveData(bool isUseSaveData)
{
    m_bUseSaveData = isUseSaveData;
}

//******************************************************************************
// initStepInfo
//******************************************************************************
void SaveData::initStepInfo(unsigned char cSkillType)
{    
    if (m_bUseSaveData) {
        return;
    }

//    memset(&m_StepInf, 0, sizeof(SaveData_StepInf));
    m_StepInf.m_cDgSkillType = cSkillType;
    m_StepInf.m_iMoment = 0;
    m_StepInf.aiming_index = kInvalidValue;
    m_StepInf.poison_cd = 0;
    m_StepInf.poison_point = 0;
    
    CCMutableArray<CDgStepInfo*> * pStepInfoArr = CGameData::Inst()->getDgStepInfo(true);
    m_StepInf.stepcnt = pStepInfoArr->count();
    
    for (int i = 0; i < pStepInfoArr->count(); i++) {
        // 每一回合
        CDgStepInfo* pStepInfo = pStepInfoArr->getObjectAtIndex(i);
        m_StepInf.enemygrps[i].goldbox = pStepInfo->gold_box;
        CCMutableArray<CDgMonsterInfo*> * pMonInfoArr = pStepInfo->m_pMonsterArr;
        m_StepInf.enemygrps[i].enemycnt = pMonInfoArr->count();
        for (int j = 0; j < pStepInfo->m_pMonsterArr->count(); j++) {
            // 每一个怪
            CDgMonsterInfo* pMonInfo = pMonInfoArr->getObjectAtIndex(j);
            SaveData_Enemy* enemyInfo = &(m_StepInf.enemygrps[i].enemies[j]);
            enemyInfo->cid = pMonInfo->cid;
            enemyInfo->base_hp = pMonInfo->hp;
            enemyInfo->cur_hp = pMonInfo->hp;
            enemyInfo->drop_exp = pMonInfo->drop_exp;
            enemyInfo->base_armor = pMonInfo->defense;
            enemyInfo->cur_armor = pMonInfo->defense;
            enemyInfo->drop_gold = pMonInfo->drop_gold;
            enemyInfo->attack = pMonInfo->attack;
            enemyInfo->drop_plus = pMonInfo->drop_plus;
            enemyInfo->shSecondElement = pMonInfo->mtype;
            
            CCardBaseInfo* pCardInf = CGameData::Inst()->getCardBaseInfByCid(pMonInfo->drop_cid);
            if(pCardInf){
                enemyInfo->drop_star = pCardInf->star;
            }
            else {
                enemyInfo->drop_star = 0;
            }

            enemyInfo->element = pMonInfo->ctype;
            enemyInfo->element_default = pMonInfo->ctype;
            
            for (int w=0; w<ENEMYSKILLMAX; w++)
            {
                enemyInfo->skidArr[w] = pMonInfo->skidArr[w];
                enemyInfo->skill_occurTimes[w] = -1;
                enemyInfo->runCount[w] = 0;
            }
            
            enemyInfo->iSkillUseLastID = 0;
            
            for (int w=0; w<ENEMYBUFFMAX; w++)
            {
                enemyInfo->buffArr[w] = pMonInfo->buffArr[w];
            }
            
            enemyInfo->round_max = pMonInfo->turn;
            enemyInfo->cur_round = pMonInfo->init_attack_turn;
            
            for (int w=0; w<MAX_BUFF_ITEMS; w++)
            {
                enemyInfo->buffId[w] = 0;
                enemyInfo->buffCd[w] = 0;
                enemyInfo->buffShow[w] = 0;
            }
         }
    }
    
    for (int i = 0; i < kRandLstLen; i++) {
        m_StepInf.m_cRandLst[i] = rand() % 100;
    }
    m_StepInf.m_cRandIdx = 0;
    m_StepInf.is_select_revive = 0;
    m_StepInf.is_iFreeRevive = 0;
    
    CommonInfo* pComInf = CGameData::Inst()->getCommonInfo();
    if (pComInf->is_select_revive)
    {
        m_StepInf.is_select_revive = 1;
    }
    
    if (pComInf->vec_revive_minus_coin.size() == 3) {
        m_StepInf.revive_minus_coin_arr[0] = pComInf->vec_revive_minus_coin[0];
        m_StepInf.revive_minus_coin_arr[1] = pComInf->vec_revive_minus_coin[1];
        m_StepInf.revive_minus_coin_arr[2] = pComInf->vec_revive_minus_coin[2];
    }
    
}


void SaveData::setEnemySkillOccTimes(short occTimes, short skillId,int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].skill_occurTimes[skillId] = occTimes;
}

void SaveData::setEnemyRunCount(short runCount, short skillId,int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].runCount[skillId] = runCount;
}

//******************************************************************************
// setEnemyCurHp
//******************************************************************************
void SaveData::setEnemyCurHp(int curHp, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].cur_hp = curHp;
}

//******************************************************************************
// setEnemyCurState
//******************************************************************************
void SaveData::setEnemyCurState(int curState, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].state = curState;
}

void SaveData::setEnemyElement(int elem, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].element = elem;
}

//******************************************************************************
// setEnemyCurRound
//******************************************************************************
void SaveData::setEnemyCurRound(int curRound, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].cur_round = curRound;
}

void SaveData::setEnemyLastSkillUse(int skillId, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].iSkillUseLastID = skillId;
}

//******************************************************************************
// setEnemyCurArmor
//******************************************************************************
void SaveData::setEnemyCurArmor(long curArmor, int iStep, int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].cur_armor = curArmor;
}

/*
 *记录敌将当前回合受到的总伤害
 */
void SaveData::setEnemyBeAttrack(int beAtkNum,int iStep,int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].enemyHurt = beAtkNum;
}

void SaveData::setEnemyBuffbuffId(int index, short buffId, int iStep,int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].buffId[index] = buffId;
}

void SaveData::setEnemyBuffbuffCd(int index, short buffCd, int iStep,int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].buffCd[index] = buffCd;
}

void SaveData::setEnemyBuffbuffShow(int index, short buffShow, int iStep,int idx)
{
    m_StepInf.enemygrps[iStep].enemies[idx].buffShow[index] = buffShow;
}

//******************************************************************************
// getEnemyGroupData
//******************************************************************************
SaveData_EnemyGrp* SaveData::getEnemyGroupData(int iStep)
{
    return &m_StepInf.enemygrps[iStep];
}

//******************************************************************************
// getEnemyGroupData
//******************************************************************************
int SaveData::getStepCountMax()
{
    return m_StepInf.stepcnt;
}

//******************************************************************************
// setDungeonUid
//******************************************************************************
void SaveData::setDungeonUid(const char *dungeonUid)
{
    snprintf(m_StepInf.dungeon_uid, 31, "%s", dungeonUid);
}

//******************************************************************************
// getDungeonUid
//******************************************************************************
const char* SaveData::getDungeonUid()
{
    return m_StepInf.dungeon_uid;
}

//******************************************************************************
// setFightSceneType
//******************************************************************************
void SaveData::setFightSceneType(short fightSceneType)
{
    m_StepInf.fightScene_type = fightSceneType;
}

//******************************************************************************
// getFightSceneType
//******************************************************************************
short SaveData::getFightSceneType()
{
    return m_StepInf.fightScene_type;
}

//******************************************************************************
// setDgType
//******************************************************************************
void SaveData::setDgType(const char *dgType)
{
    snprintf(m_StepInf.dgtype, 31, "%s", dgType);
}

//******************************************************************************
// setDgType
//******************************************************************************
const char* SaveData::getDgType()
{
    return m_StepInf.dgtype;
}

//******************************************************************************
// setAimingIndex
//******************************************************************************
void SaveData::setAimingIndex(int index)
{
    m_StepInf.aiming_index = index;
}

//******************************************************************************
// setDgType
//******************************************************************************
int SaveData::getAimingIndex()
{
    return m_StepInf.aiming_index;
}

//******************************************************************************
// setPoisonCd
//******************************************************************************
void SaveData::setPoisonCd(bool isPoisoned)
{
    m_StepInf.poison_cd = isPoisoned;
}

//******************************************************************************
// getPoisonCd
//******************************************************************************
bool SaveData::getPoisonCd()
{
    return m_StepInf.poison_cd;
}

//******************************************************************************
// setPoisonPoint
//******************************************************************************
void SaveData::setPoisonPoint(int poisonPoint)
{
    m_StepInf.poison_point = poisonPoint;
}

//******************************************************************************
// getPoisonCd
//******************************************************************************
int SaveData::getPoisonPoint()
{
    return m_StepInf.poison_point;
}

//******************************************************************************
// setSickPoint
//******************************************************************************
void SaveData::setSickPoint(int sickPoint)
{
    m_StepInf.sick_point = sickPoint;
}

//******************************************************************************
// getSickPoint
//******************************************************************************
int SaveData::getSickPoint()
{
    return m_StepInf.sick_point;
}

//******************************************************************************
// setDelayCd
//******************************************************************************
//void SaveData::setDelayCd(bool isDelay)
//{
//    m_StepInf.delay_cd = isDelay;
//}

//******************************************************************************
// getDelayCd
//******************************************************************************
//bool SaveData::getDelayCd()
//{
//    return m_StepInf.delay_cd;
//}

//******************************************************************************
// initPlayerInfo
//******************************************************************************
void SaveData::initPlayerInfo(bool cntryBuff)
{
    if (m_bUseSaveData) {
        return;
    }
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    stUid* pTeamInfo;
    if (CGameData::Inst()->getIsUseTmpTeam()) {
        pTeamInfo = CGameData::Inst()->getTmpTeamInf();
    }
    else{
        pTeamInfo = CGameData::Inst()->getCurTeamInf();
    }
    
    int iTeamIndex = 0; // 编队序号 0,1,2,3,4五个编队
    int iCardIndex = 0; // 编队中武将的序号 0,1,2,3,4,5六个武将每个编队
    for (int i = 0; i < 30; i++)
    {
        iTeamIndex = i / 6;
        iCardIndex = i % 6;
        if (CGameData::Inst()->getSelectedHelperCount() == 1)
        {   //  不是全队伍限定战场
            if (iTeamIndex >= 1)
            {
                break;
            }
        }
        else
        {   // 全队伍限定战场
            if (iCardIndex == 0)
            {
                pTeamInfo = CGameData::Inst()->getTeamInfByIdx(iTeamIndex);
            }
        }
        
        CUserCardInfo* pUsrCardInfo;
        
        string strUcid = pTeamInfo[iCardIndex].uid;
        string::size_type pos(0);
        pos = strUcid.find(",");
        if (pos == string::npos) {
            if(iCardIndex == 5){//friend card
                CGameData::Inst()->getFriendHelpArr();
                pUsrCardInfo = CGameData::Inst()->getHelperCardInfo(iTeamIndex);
                
                CFriendHelpInfo* pHelperInf = CGameData::Inst()->getSelectedHelperInfo(0);
                if(strcmp(pHelperInf->type, "other") == 0)
                    pUsrCardInfo->leader_skid = 0;
            }
            else{
                if (pTeamInfo[iCardIndex].uid[0] == 0) {
                    // 编队对应位置没有武将
                    m_PlayerInf.card_info[i].cid = 0;
                    m_PlayerInf.card_info[i].plus= 0;
                    continue;
                }
                
                pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pTeamInfo[iCardIndex].uid);
            }
        }
        else{
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByRentStr(strUcid);
        }
        
        SaveData_Card *cardInfo = &(m_PlayerInf.card_info[i]);
        cardInfo->cid = pUsrCardInfo->cid;
        cardInfo->plus = pUsrCardInfo->plus;
        cardInfo->star = pUsrCardInfo->star;
        cardInfo->lv = pUsrCardInfo->lv;
        cardInfo->lv_max = pUsrCardInfo->max_lv;
        
        ////////////////////////
        int iAwakeHp = 0;
        int iAwakeRec = 0;
        int iAwakeAtk = 0;
        
        cardInfo->AwakeNum = pUsrCardInfo->i_AwakeNum;
        
        for (int i = 0; i < MaxAwakeSkillCnt; ++i)cardInfo->AwakeSkillArr[i] = enAwakeSkill_NULL;
        
        for (int i=0; i<MaxAwakeSkillCnt; i++)
        {
            cardInfo->AwakeSkillArr[i] = pUsrCardInfo->AwakeSkillArr[i];
            if (i < pUsrCardInfo->i_AwakeNum)
            {
                stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(cardInfo->AwakeSkillArr[i]);
                switch (pAwakeSkill->iType)
                {
                    case enAwakeSkillTp_valChg_hp:
                        iAwakeHp += (int)pAwakeSkill->fEffect;
                        break;
                        
                    case enAwakeSkillTp_valChg_atk:
                        iAwakeAtk += (int)pAwakeSkill->fEffect;
                        break;
                        
                    case enAwakeSkillTp_valChg_rcv:
                        iAwakeRec += (int)pAwakeSkill->fEffect;
                        break;
                        
                    default:
                        break;
                }
            }

        }
        
        if (cntryBuff)
        {
            cardInfo->hp = (pUsrCardInfo->hp * (1 + cntryInfo->buffInfo[enBuffType_Hp].effect)) + iAwakeHp;
            cardInfo->recover = pUsrCardInfo->recover * (1 + cntryInfo->buffInfo[enBuffType_Recover].effect) + iAwakeRec;
            cardInfo->attack = pUsrCardInfo->attack * (1 + cntryInfo->buffInfo[enBuffType_Attack].effect) + iAwakeAtk;
        }else{
            cardInfo->hp = pUsrCardInfo->hp + iAwakeHp;
            cardInfo->recover = pUsrCardInfo->recover + iAwakeRec;
            cardInfo->attack = pUsrCardInfo->attack + iAwakeAtk;
        }
        
        cardInfo->element = pUsrCardInfo->ctype;
        cardInfo->sub_element = pUsrCardInfo->stype;
        cardInfo->category = pUsrCardInfo->category;
        cardInfo->sub_category = pUsrCardInfo->sub_category;
        cardInfo->man_skill_id = pUsrCardInfo->skid;
        cardInfo->auto_skill_id = pUsrCardInfo->leader_skid;
        
        cardInfo->m_couPorperty = pUsrCardInfo->cou_name;//国家属性
        
        if(pUsrCardInfo->bIsEluTgt)
            cardInfo->isEluTgt = 1;
        else
            cardInfo->isEluTgt = 0;
        
        if(pUsrCardInfo->bIsWkTgt)
            cardInfo->isWkTgt = 1;
        else
            cardInfo->isWkTgt = 0;
        
        stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pUsrCardInfo->skid);
        if((pUsrCardInfo->skid > 0) && (pManSkill != NULL)){
            cardInfo->skill_cd_max = pManSkill->cTurn - (pUsrCardInfo->sk_lv -1);
        }
        else {
            cardInfo->skill_cd_max = kInvalidValue;
        }
        cardInfo->cur_skill_cd = cardInfo->skill_cd_max;
        
        snprintf(cardInfo->card_name, 31, "%s", pUsrCardInfo->name);
    }
    
    CAvailDiamond* availDiamond = CGameData::Inst()->getCurAvailDiamond();
    m_PlayerInf.stone_count = availDiamond->cnt;
    for (int i = 0; i < availDiamond->cnt; i++) {
        SaveData::Inst()->setStoneElement(i, availDiamond->buf[i]);
    }
    
    // 好友信息
    CFriendHelpInfo* pHelpInf = NULL;
    for (int i = 0; i < kHelperMaxCount; i++) {
        pHelpInf = CGameData::Inst()->getSelectedHelperInfo(i);
        if (pHelpInf)
        {   // 如果不是全队伍限定战场，就只保存一个援军
            snprintf(m_PlayerInf.helper_info[i].helper_uid, 31, "%s", pHelpInf->fid);
            snprintf(m_PlayerInf.helper_info[i].helper_type, 31, "%s", pHelpInf->type);
            snprintf(m_PlayerInf.helper_info[i].helper_name, 31, "%s", pHelpInf->name);
            m_PlayerInf.helper_info[i].helper_country = pHelpInf->user_elem;
            m_PlayerInf.helper_info[i].helper_lv = pHelpInf->lv;
            m_PlayerInf.helper_info[i].gacha_pt_add = pHelpInf->gacha_pt;
        }
    }
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    m_PlayerInf.gacha_pt_all = pUsrInf->gacha_pt;
    
    m_PlayerInf.stamina_recover_time = CGameData::Inst()->getStaminaRecoverTime() * 60 * (1 - cntryInfo->buffInfo[enBuffType_Stamina].effect);
    m_PlayerInf.acPoint_recover_time = CGameData::Inst()->getActionPointRecoverTime();
    
    CommonInfo* commonInfo = CGameData::Inst()->getCommonInfo();
    m_PlayerInf.revive_coin = commonInfo->revive_coin;
    m_PlayerInf.fightScene_change = commonInfo->bg_change;
    m_PlayerInf.bUseChristmasFightScene = commonInfo->bChrisBg;
    m_PlayerInf.dungeon_clear_coin = commonInfo->dungeon_clear_coin;
    if (commonInfo->list_external.size() > 0)
    {
        for (list<string>::iterator itEn = commonInfo->list_external.begin();
             itEn != commonInfo->list_external.end();
             ++itEn) {
            string strExternal = *itEn;
            snprintf(m_PlayerInf.external_list, 299, "%s%s,",
                     m_PlayerInf.external_list, strExternal.c_str());
        }
    }
    
    m_PlayerInf.cur_coin = pUsrInf->coin;
    m_PlayerInf.cur_cost = pUsrInf->cost;
    m_PlayerInf.cur_lv = pUsrInf->lv;
    m_PlayerInf.cur_max_statina = pUsrInf->max_stamina;
    
    m_PlayerInf.bUseChristmasUI = CGameData::Inst()->getIsUseChristmasUI();
        
    if (CGameData::Inst()->getMD5Str()) {
        snprintf(m_PlayerInf.md5_str, 63, "%s", CGameData::Inst()->getMD5Str());
    }
    
    if (CGameData::Inst()->getSessionId())
    {
        snprintf(m_PlayerInf.session_id, 63, "%s", CGameData::Inst()->getSessionId());
    }
    
    if (CGameData::Inst()->getUserId())
    {
        snprintf(m_PlayerInf.user_id, 63, "%s", CGameData::Inst()->getUserId());
    }
    
    m_PlayerInf.cur_step = 0;
    m_PlayerInf.cur_itemcnt = 0;
    m_PlayerInf.cur_dggold = 0;
    
    m_PlayerInf.wudi_cd = 0;
    m_PlayerInf.sheild_cd = 0;
    m_PlayerInf.armor_effect = 1.0;
    m_PlayerInf.recover_cd = 0;
    m_PlayerInf.recover_effect = 0;
    m_PlayerInf.revive_time = 0;
    m_PlayerInf.fanji_cd = 0;
    m_PlayerInf.fanji_effect = 0.0;
    m_PlayerInf.aoe_cd = 0;
    
    m_PlayerInf.ulTouchCnt = 0;     //战斗转珠次数
    m_PlayerInf.ulCurTouchCnt = 0;     //战斗转珠次数
    m_PlayerInf.ulMaxCombo = 0;     //最大combo数
    m_PlayerInf.ulTotalCombo = 0;   //combo总数
    m_PlayerInf.ulMaxDamage = 0;    //单次转珠的最大伤害
    m_PlayerInf.ulTotalDamage = 0;  //输出的总伤害
    m_PlayerInf.ulCurDamage = 0;    //当前伤害值
    m_PlayerInf.ulUseSkillCnt = 0;  //本场使用技能次数
    
    m_PlayerInf.iEnhanceCD = 0;
    m_PlayerInf.iEnhanceElem = enElementMax;
    m_PlayerInf.fEnhanceFactor = 1.0f;
    
    m_PlayerInf.iEnWujiangCD = 0;
    m_PlayerInf.iEnWujiangType = enGeneralTp_Null;       //用于武将类型攻击增加：主将技
    m_PlayerInf.fEnWujiangFactor = 1.0f;
    
    m_PlayerInf.isReplaced = false;
    m_PlayerInf.position = 0;
    
    m_PlayerInf.isManSkill_129 = false;
    m_PlayerInf.beCanFei = 0;
}

//******************************************************************************
// getIsUseChristmasUI
//******************************************************************************
bool SaveData::getIsUseChristmasUI()
{
    return m_PlayerInf.bUseChristmasUI;
}

bool  SaveData::isReplace(){
    return m_PlayerInf.isReplaced;
}

int SaveData::getPosition(){
    return m_PlayerInf.position;
}

void SaveData::setReplace(bool isReplace){
    m_PlayerInf.isReplaced = isReplace;
}

void SaveData::setPosition(int position){
    m_PlayerInf.position = position;
}


//******************************************************************************
// isManSkill_129
//******************************************************************************
bool  SaveData::isManSkill_129(){
    return m_PlayerInf.isManSkill_129;
}

void SaveData::setManSkill_129(bool isManSkill_129){
    m_PlayerInf.isManSkill_129 = isManSkill_129;
}

//******************************************************************************
// getHelperIndexForAddFriend
//******************************************************************************
int SaveData::getHelperIndexForAddFriend()
{
    for (int i = 0; i < kHelperMaxCount; i++) {
        if (strcmp(m_PlayerInf.helper_info[i].helper_type, "other") == 0)
        {   // 有义军，果断加好友
            return i;
        }
    }
    
    return 0;
}

//******************************************************************************
// getAllGachaPoint
//******************************************************************************
int SaveData::getAllGachaPoint()
{
    int iGachaPoint = 0;
    for (int i = 0; i < kHelperMaxCount; i++) {
        iGachaPoint += m_PlayerInf.helper_info[i].gacha_pt_add;
    }
    
    return iGachaPoint;
}

//******************************************************************************
// getHelperIndexForAddFriend
//******************************************************************************
SaveData_Card* SaveData::getHelperCardForAddFried()
{
    int index = getHelperIndexForAddFriend() * 6 + 5;
    return &m_PlayerInf.card_info[index];
}

//******************************************************************************
// getHelperIndexForAddFriend
//******************************************************************************
SaveData_Helper* SaveData::getHelperInfoForAddFriend()
{
    int index = getHelperIndexForAddFriend();
    return getHelperInfo(index);
}

//******************************************************************************
// getHelperInfo
//******************************************************************************
SaveData_Helper* SaveData::getHelperInfo(int index)
{
    if (index < 0 || index >= kHelperMaxCount)
    {
        index = 0;
    }
    
    return &m_PlayerInf.helper_info[index];
}

//******************************************************************************
// getGachaPtAll
//******************************************************************************
short SaveData::getGachaPtAll()
{
    return m_PlayerInf.gacha_pt_all;
}

//******************************************************************************
// getStaminaRecoverTime
//******************************************************************************
short SaveData::getStaminaRecoverTime()
{
    return m_PlayerInf.stamina_recover_time;
}

//******************************************************************************
// getStaminaRecoverTime
//******************************************************************************
short SaveData::getAcPointRecoverTime()
{
    return m_PlayerInf.acPoint_recover_time;
}

//******************************************************************************
// getReviveCoin
//******************************************************************************
short SaveData::getReviveCoin()
{
    return m_PlayerInf.revive_coin;
}

//******************************************************************************
// getWudiCd
//******************************************************************************
short SaveData::getWudiCd()
{
    return m_PlayerInf.wudi_cd;
}

//******************************************************************************
// setWudiCd
//******************************************************************************
void SaveData::setWudiCd(short wudiCd)
{
    m_PlayerInf.wudi_cd = wudiCd;
}

//******************************************************************************
// getWudiElement
//******************************************************************************
short SaveData::getWudiElement()
{
    return m_PlayerInf.wudi_element;
}

//******************************************************************************
// getWudiType
//******************************************************************************
char SaveData::getWudiType()
{
    return m_PlayerInf.wudi_type;
}

//******************************************************************************
// setWudiType
//******************************************************************************
void SaveData::setWudiType(char wudiType)
{
    m_PlayerInf.wudi_type = wudiType;
}

//******************************************************************************
// setWudiElement
//******************************************************************************
void SaveData::setWudiElement(short wudiElement)
{
    m_PlayerInf.wudi_element = wudiElement;
}

//******************************************************************************
// setFanjiCd
//******************************************************************************
void SaveData::setFanjiCd(short fanjiCd)
{
    m_PlayerInf.fanji_cd = fanjiCd;
}

//******************************************************************************
// getFanjiCd
//******************************************************************************
short SaveData::getFanjiCd()
{
    return m_PlayerInf.fanji_cd;
}

//******************************************************************************
// setIsFanjiMiansi
//******************************************************************************
void SaveData::setIsFanjiMiansi(bool bMiansi)
{
    m_PlayerInf.fanji_miansi = bMiansi;
}

//******************************************************************************
// getIsFanjiMiansi
//******************************************************************************
bool SaveData::getIsFanjiMiansi()
{
    return m_PlayerInf.fanji_miansi;
}

//******************************************************************************
// setFanjiEffect
//******************************************************************************
void SaveData::setFanjiEffect(float fanjiEffect)
{
    m_PlayerInf.fanji_effect = fanjiEffect;
}

//******************************************************************************
// getFanjiEffect
//******************************************************************************
float SaveData::getFanjiEffect()
{
    return m_PlayerInf.fanji_effect;
}

//******************************************************************************
// getFanjiElement
//******************************************************************************
short SaveData::getFanjiElement()
{
    return m_PlayerInf.fanji_element;
}

//******************************************************************************
// setFanjiElement
//******************************************************************************
void SaveData::setFanjiElement(short fanjiElement)
{
    m_PlayerInf.fanji_element = fanjiElement;
}

//******************************************************************************
// getAOECd
//******************************************************************************
short SaveData::getAOECd()
{
    return m_PlayerInf.aoe_cd;
}

//******************************************************************************
// setAOECd
//******************************************************************************
void SaveData::setAOECd(short aoeCd)
{
    m_PlayerInf.aoe_cd = aoeCd;
}

//******************************************************************************
// getSheildCd
//******************************************************************************
short SaveData::getSheildCd()
{
    return m_PlayerInf.sheild_cd;
}

//******************************************************************************
// setSheildCd
//******************************************************************************
void SaveData::setSheildCd(short sheildCd)
{
    m_PlayerInf.sheild_cd = sheildCd;
}

//******************************************************************************
// getArmorEffect
//******************************************************************************
float SaveData::getArmorEffect()
{
    return m_PlayerInf.armor_effect;
}

//******************************************************************************
// setArmorEffect
//******************************************************************************
void SaveData::setArmorEffect(float armorEffect)
{
    m_PlayerInf.armor_effect = armorEffect;
}

//******************************************************************************
// getRecoverCd
//******************************************************************************
short SaveData::getRecoverCd()
{
    return m_PlayerInf.recover_cd;
}

//******************************************************************************
// setRecoverCd
//******************************************************************************
void SaveData::setRecoverCd(short recoverCd)
{
    m_PlayerInf.recover_cd = recoverCd;
}

//******************************************************************************
// getRecoverEffect
//******************************************************************************
float SaveData::getRecoverEffect()
{
    return m_PlayerInf.recover_effect;
}

//******************************************************************************
// setRecoverEffect
//******************************************************************************
void SaveData::setRecoverEffect(float recoverEffect)
{
    m_PlayerInf.recover_effect = recoverEffect;
}



//******************************************************************************
// getRecoverWuHiangCd
//******************************************************************************
short SaveData::getRecoverWuHiangCd()
{
    return m_PlayerInf.recoverWuJiang_cd;
}


//******************************************************************************
// setRecoverWuJiangCd
//******************************************************************************
void SaveData::setRecoverWuJiangCd(short recoverCd)
{
    m_PlayerInf.recoverWuJiang_cd = recoverCd;
}

//******************************************************************************
// getRecoverWuJiangEffect
//******************************************************************************
float SaveData::getRecoverWuJiangEffect()
{
    return m_PlayerInf.recoverWuJiang_effect;
}

//******************************************************************************
// setRecoverWuJiangEffect
//******************************************************************************
void SaveData::setRecoverWuJiangEffect(float recoverEffect)
{
    m_PlayerInf.recoverWuJiang_effect = recoverEffect;
}



//******************************************************************************
// getCurCoin
//******************************************************************************
long SaveData::getCurCoin()
{
    return m_PlayerInf.cur_coin;
}

//******************************************************************************
// setCurCoin
//******************************************************************************
void SaveData::setCurCoin(long curCoin)
{
    m_PlayerInf.cur_coin = curCoin;
}

//******************************************************************************
// getHelperSkillId
//******************************************************************************
short SaveData::getHelperSkillId()
{
    return m_PlayerInf.card_info[5].auto_skill_id;
}

//******************************************************************************
// getHelperSkillId
//******************************************************************************
short SaveData::getLeaderSkillId()
{
    return m_PlayerInf.card_info[0].auto_skill_id;
}

//******************************************************************************
// setStone
//******************************************************************************
void SaveData::setStone(int iRow, int iColumn, char elem)
{
    m_PlayerInf.stonegrid[iRow][iColumn] = elem;
}

//******************************************************************************
// getStone
//******************************************************************************
char SaveData::getStone(int iRow, int iColumn)
{
    return m_PlayerInf.stonegrid[iRow][iColumn];
}

void SaveData::setStoneBlackCover(int iRow, int iColumn, char blackCover)
{
    m_PlayerInf.stonegridblackcover[iRow][iColumn] = blackCover;
}

char SaveData::getStoneBlackCover(int iRow, int iColumn)
{
    return m_PlayerInf.stonegridblackcover[iRow][iColumn];
}

void SaveData::setstoneParam1(int iRow, int iColumn, int param1)
{
    m_PlayerInf.stonegridparam1[iRow][iColumn] = param1;
}

int SaveData::getstoneParam1(int iRow, int iColumn)
{
    return m_PlayerInf.stonegridparam1[iRow][iColumn];
}

void SaveData::setstoneParam2(int iRow, int iColumn, int param2)
{
    m_PlayerInf.stonegridparam2[iRow][iColumn] = param2;
}

int SaveData::getstoneParam2(int iRow, int iColumn)
{
    return m_PlayerInf.stonegridparam2[iRow][iColumn];
}

//******************************************************************************
// setStone
//******************************************************************************
void SaveData::setStonePlus(int iRow, int iColumn, char plus)
{
    m_PlayerInf.stonegridplus[iRow][iColumn] = plus;
}

//******************************************************************************
// getStone
//******************************************************************************
char SaveData::getStonePlus(int iRow, int iColumn)
{
    return m_PlayerInf.stonegridplus[iRow][iColumn];
}

//******************************************************************************
// setCardInfo
//******************************************************************************
void SaveData::setCardInfo(int idx,SaveData_Card card)
{
    m_PlayerInf.card_info[getCardCurIndex(idx)] = card;
}

//******************************************************************************
// getCardInf
//******************************************************************************
int SaveData::getCardCurIndex(int index)
{
    int iCurIndex = index;
    if (m_StepInf.m_cDgSkillType == 2)
    {
        int iCurTeamIndex = MIN(m_PlayerInf.cur_step, 4);
        iCurIndex += iCurTeamIndex * 6;
    }
    
    return iCurIndex;
}

//******************************************************************************
// getCardInfo
//******************************************************************************
SaveData_Card* SaveData::getCardInfo(int idx, bool bCrash)
{
    if (bCrash)
    {
        return &(m_PlayerInf.card_info[idx]);
    }
    else
    {
        return &(m_PlayerInf.card_info[getCardCurIndex(idx)]);
    }
}

//******************************************************************************
// getCardInfo2
//******************************************************************************
SaveData_Card SaveData::getCardInfo2(int idx)
{
    return m_PlayerInf.card_info[getCardCurIndex(idx)];
}

//******************************************************************************
// setSkillCD
//******************************************************************************
void SaveData::setSkillCD(int idx, int cd)
{
    m_PlayerInf.card_info[getCardCurIndex(idx)].cur_skill_cd = cd;
}

//******************************************************************************
//setFengYinCD
//******************************************************************************
void SaveData::setFengYinCD(int idx, short cd)
{
    m_PlayerInf.card_info[getCardCurIndex(idx)].FengYinCD = cd;
}

//setState
void SaveData::setState(int idx, int state)
{
    m_PlayerInf.card_info[getCardCurIndex(idx)].state = state;
}

//******************************************************************************
// getCurLv
//******************************************************************************
short SaveData::getCurLv()
{
    return m_PlayerInf.cur_lv;
}

//******************************************************************************
// getCurCost
//******************************************************************************
short SaveData::getCurCost()
{
    return m_PlayerInf.cur_cost;
}

//******************************************************************************
// getCurStatina
//******************************************************************************
short SaveData::getCurStatina()
{
    return m_PlayerInf.cur_max_statina;
}

//******************************************************************************
// getDungeonClearCoin
//******************************************************************************
short SaveData::getDungeonClearCoin()
{
    return m_PlayerInf.dungeon_clear_coin;
}

//******************************************************************************
// getCurFloorStatus
//******************************************************************************
char SaveData::getCurFloorStatus()
{
    return m_PlayerInf.cur_floor_status;
}

//******************************************************************************
// setCurFloorStatus
//******************************************************************************
void SaveData::setCurFloorStatus(char curFloorStatus)
{
    m_PlayerInf.cur_floor_status = curFloorStatus;
}

//******************************************************************************
// setFloorId
//******************************************************************************
void SaveData::setFloorId(int floor_id)
{
    m_PlayerInf.floor_id = floor_id;
}

//******************************************************************************
// getFloorId
//******************************************************************************
int SaveData::getFloorId()
{
    return m_PlayerInf.floor_id;
}

//******************************************************************************
// setFloorName
//******************************************************************************
void SaveData::setFloorName(const char *floorName)
{
    snprintf(m_PlayerInf.floor_name, 31, "%s", floorName);
}

//******************************************************************************
// getFloorName
//******************************************************************************
const char* SaveData::getFloorName()
{
    return m_PlayerInf.floor_name;
}

//******************************************************************************
// setRoomId
//******************************************************************************
void SaveData::setRoomId(int room_id)
{
    m_PlayerInf.room_id = room_id;
}

//******************************************************************************
// getRoomId
//******************************************************************************
int SaveData::getRoomId()
{
    return m_PlayerInf.room_id;
}

//******************************************************************************
// setRoomName
//******************************************************************************
void SaveData::setRoomName(const char *roomName)
{
    snprintf(m_PlayerInf.room_name, 31, "%s", roomName);
}

//******************************************************************************
// getRoomName
//******************************************************************************
const char* SaveData::getRoomName()
{
    return m_PlayerInf.room_name;
}

//******************************************************************************
// setCurStep
//******************************************************************************
void SaveData::setCurStep(int iStep)
{
    m_PlayerInf.cur_step = iStep;
}

//******************************************************************************
// getCurStep
//******************************************************************************
int SaveData::getCurStep()
{
    return m_PlayerInf.cur_step;
}

//******************************************************************************
// setCurItemCnt
//******************************************************************************
void SaveData::setCurItemCnt(int iItemCnt)
{
    m_PlayerInf.cur_itemcnt = iItemCnt;
}

//******************************************************************************
// getCurItemCnt
//******************************************************************************
int SaveData::getCurItemCnt()
{
    return m_PlayerInf.cur_itemcnt;
}

//******************************************************************************
// setCurDgGold
//******************************************************************************
void SaveData::setCurDgGold(int iGold)
{
    m_PlayerInf.cur_dggold = iGold;
}

//******************************************************************************
// getCurDgGold
//******************************************************************************
int SaveData::getCurDgGold()
{
    return m_PlayerInf.cur_dggold;
}

//******************************************************************************
// setPlayerCurHp
//******************************************************************************
void SaveData::setPlayerCurHp(long hp)
{
    m_PlayerInf.cur_hp = hp;
}

//******************************************************************************
// getPlayerCurHp
//******************************************************************************
long SaveData::getPlayerCurHp()
{
    return m_PlayerInf.cur_hp;
}

void SaveData::setPlayerBaseHp(long baseHp)
{
    m_PlayerInf.base_hp = baseHp;
}

void SaveData::setPlayerCanfei(bool canfei)
{
    m_PlayerInf.beCanFei = canfei?1:0;
}

short SaveData::getPlayerCanfei()
{
    return m_PlayerInf.beCanFei?1:0;
}

long SaveData::getPlayerBaseHp()
{
    return m_PlayerInf.base_hp;
}

//******************************************************************************
// setStoneCount
//******************************************************************************
void SaveData::setStoneCount(char stoneCount)
{
    m_PlayerInf.stone_count = stoneCount;
}

//******************************************************************************
// getStoneCount
//******************************************************************************
char SaveData::getStoneCount()
{
    return m_PlayerInf.stone_count;
}

//******************************************************************************
// setStoneElement
//******************************************************************************
void SaveData::setStoneElement(int idx, char stoneElement)
{
    m_PlayerInf.stone_element[idx] = stoneElement;
}

//******************************************************************************
// setPlayerCurHp
//******************************************************************************
char SaveData::getStoneElement(int idx)
{
    return m_PlayerInf.stone_element[idx];
}

//******************************************************************************
// setReviveTime
//******************************************************************************
void SaveData::setReviveTime(long reviveTime)
{
    m_PlayerInf.revive_time = reviveTime;
}

//******************************************************************************
// getReviveTime
//******************************************************************************
long SaveData::getReviveTime()
{
    return m_PlayerInf.revive_time;
}


//void SaveData::setReqCachaTime(long l_reqGachaTime){
//    m_PlayerInf.l_requestGacha_time = l_reqGachaTime;
//}
//
//
//long SaveData::getReqGachaTime(){
//    return m_PlayerInf.l_requestGacha_time;
//}



//******************************************************************************
// setFightSceneChange
//******************************************************************************
void SaveData::setFightSceneChange(short fightSceneChange)
{
    m_PlayerInf.fightScene_change = fightSceneChange;
}

//******************************************************************************
// getFightSceneChange
//******************************************************************************
short SaveData::getFightSceneChange()
{
    return m_PlayerInf.fightScene_change;
}

//******************************************************************************
// setIsUseChristmasFightScene
//******************************************************************************
void SaveData::setIsUseChristmasFightScene(bool isUseChristmasFightScene)
{
    m_PlayerInf.bUseChristmasFightScene = isUseChristmasFightScene;
}

//******************************************************************************
// getIsUseChristmasFightScene
//******************************************************************************
bool SaveData::getIsUseChristmasFightScene()
{
    return m_PlayerInf.bUseChristmasFightScene;
}

//******************************************************************************
// getMD5Str
//******************************************************************************
const char* SaveData::getMD5Str()
{
    return m_PlayerInf.md5_str;
}

//******************************************************************************
// getSessionId
//******************************************************************************
const char* SaveData::getSessionId()
{
    return m_PlayerInf.session_id;
}

//******************************************************************************
// getUserId
//******************************************************************************
const char* SaveData::getUserId()
{
    return m_PlayerInf.user_id;
}

//******************************************************************************
// rand
//******************************************************************************
unsigned char SaveData::getrand()
{
    m_StepInf.m_cRandIdx %= kRandLstLen;
    
    unsigned char ucRand = m_StepInf.m_cRandLst[m_StepInf.m_cRandIdx];
    
    m_StepInf.m_cRandIdx++;

    return ucRand;
}

//******************************************************************************
// getQianLi
//******************************************************************************
unsigned char SaveData::getDgSkillType()
{
    return m_StepInf.m_cDgSkillType;
}


void SaveData::setiMoment(int moment)
{
    m_StepInf.m_iMoment = moment;
}

int SaveData::getiMoment()
{
    return m_StepInf.m_iMoment;
}

//******************************************************************************
// checkIsUseExternal
//******************************************************************************
void SaveData::checkIsUseExternal()
{
    if (strlen(m_PlayerInf.external_list) == 0)
        return;

    string strExternal(m_PlayerInf.external_list);
    string strSub;
    std::string::size_type posBegin(0);
    std::string::size_type posEnd(0);
    while (1) {
		if((posEnd = strExternal.find(",", posBegin)) != std::string::npos)
        {
            strSub = strExternal.substr(posBegin, posEnd - posBegin);
            
            if(OcProxy::Inst()->checkIsUseExternal(strSub.c_str())){}
//            exit(0);

            posBegin = posEnd + 1;
        }
        else
            break;
    }    
}

str_mubiaoGx SaveData::getMubiaoGongXianBylv(int lv)
{
    str_mubiaoGx stu;
    
    stu.minMubiaoGX = CGameData::Inst()->getDefaultCntryBu(lv);

    stu.ideadLine = CGameData::Inst()->getCommonInfo()->task_deadline_default;
    stu.iPerson = CGameData::Inst()->getCommonInfo()->task_person_default;
    stu.m_iBaozhengCoin = CGameData::Inst()->getCommonInfo()->task_coin_default;;
    
    return stu;
}

#pragma mark -
#pragma mark 录视频相关
//******************************************************************************
// OneRoundOvrNotify
//******************************************************************************
void SaveData::OneRoundOvrNotify()
{
    m_PlayerInf.ulTouchCnt++;
    
    if (m_PlayerInf.ulCurDamage > m_PlayerInf.ulMaxDamage) {
        m_PlayerInf.ulMaxDamage = m_PlayerInf.ulCurDamage;
    }
    
    m_PlayerInf.ulTotalDamage += m_PlayerInf.ulCurDamage;
    m_PlayerInf.ulCurDamage = 0;
}

//******************************************************************************
// setCurCombo
//******************************************************************************
void SaveData::setCurCombo(unsigned long ulCombo)
{
    if (ulCombo > m_PlayerInf.ulMaxCombo) {
        m_PlayerInf.ulMaxCombo = ulCombo;
    }
    
    m_PlayerInf.ulTotalCombo += ulCombo;
}

//******************************************************************************
// IncCurDamage
//******************************************************************************
void SaveData::IncCurDamage(unsigned long ulDamage)
{
    m_PlayerInf.ulCurDamage += ulDamage;
}

//******************************************************************************
// getMaxCombo
//******************************************************************************
unsigned long SaveData::getMaxCombo()
{
    return m_PlayerInf.ulMaxCombo;
}

//******************************************************************************
// getTotalCombo
//******************************************************************************
unsigned long SaveData::getTotalCombo()
{
    return m_PlayerInf.ulTotalCombo;
}

//******************************************************************************
// getAverageCombo
//******************************************************************************
float SaveData::getAverageCombo()
{
    if(m_PlayerInf.ulTouchCnt == 0)
        return 0;
    
    float rt = (float) m_PlayerInf.ulTotalCombo / m_PlayerInf.ulTouchCnt;

    return rt;
}

//******************************************************************************
// getMaxDamage
//******************************************************************************
unsigned long SaveData::getMaxDamage()
{
    return m_PlayerInf.ulMaxDamage;
}

//******************************************************************************
// getTotalDamage
//******************************************************************************
unsigned long SaveData::getTotalDamage()
{
    return m_PlayerInf.ulTotalDamage;
}

//******************************************************************************
// getAverageDamage
//******************************************************************************
unsigned long SaveData::getAverageDamage()
{
    if(m_PlayerInf.ulTouchCnt == 0)
        return 0;
    
    return m_PlayerInf.ulTotalDamage / m_PlayerInf.ulTouchCnt;
}

//******************************************************************************
// IncUseSkillCnt
//******************************************************************************
void SaveData::IncUseSkillCnt()
{
    m_PlayerInf.ulUseSkillCnt++;
}

//******************************************************************************
// getTouchCnt
//******************************************************************************
unsigned long SaveData::getTouchCnt()
{
    return m_PlayerInf.ulTouchCnt;
}

unsigned long SaveData::getCurTouchCnt()
{
    return m_PlayerInf.ulCurTouchCnt;
}

void SaveData::setCurTouchCnt(unsigned long cnt)
{
    m_PlayerInf.ulCurTouchCnt = cnt;
}

int SaveData::getPlayerEnhanceCD()
{
    return m_PlayerInf.iEnhanceCD;
}

int SaveData::getPlayerEnhanceElem()
{
    return m_PlayerInf.iEnhanceElem;
}

float SaveData::getPlayerEnhanceFactor()
{
    return m_PlayerInf.fEnhanceFactor;
}

int SaveData::getPlayerEnWujiangCD()
{
    return m_PlayerInf.iEnWujiangCD;
}

int SaveData::getPlayerEnWujiangType()
{
    return m_PlayerInf.iEnWujiangType;
}

float SaveData::getPlayerEnWujiangFactor()
{
    return m_PlayerInf.fEnWujiangFactor;
}


int SaveData::getPlayerCountryWujiangCD()
{
    return m_PlayerInf.iCountryWujiangCD;
}

int SaveData::getPlayerCountryWujiangType()
{
    return m_PlayerInf.iCountryWujiangType;
}

float SaveData::getPlayerCountryWujiangFactor()
{
    return m_PlayerInf.fCountryWujiangFactor;
}

void SaveData::setPlayerCountryWujiangCD(int cd)
{
    m_PlayerInf.iCountryWujiangCD = cd;
}

void SaveData::setPlayerCountryWujiangType(int type)
{
    m_PlayerInf.iCountryWujiangType = type;
}

void SaveData::setPlayerCountryWujiangFactor(float factor)
{
    m_PlayerInf.fCountryWujiangFactor = factor;
}



void SaveData::setPlayerEnhanceCD(int cd)
{
    m_PlayerInf.iEnhanceCD = cd;
}

void SaveData::setPlayerEnhanceElem(int elem)
{
    m_PlayerInf.iEnhanceElem = elem;
}

void SaveData::setPlayerEnhanceFactor(float factor)
{
    m_PlayerInf.fEnhanceFactor = factor;
}

void SaveData::setPlayerEnWujiangCD(int cd)
{
    m_PlayerInf.iEnWujiangCD = cd;
}

void SaveData::setPlayerEnWujiangType(int type)
{
    m_PlayerInf.iEnWujiangType = type;
}

void SaveData::setPlayerEnWujiangFactor(float factor)
{
    m_PlayerInf.fEnWujiangFactor = factor;
}

//******************************************************************************
// getUseSkillCnt
//******************************************************************************
unsigned long SaveData::getUseSkillCnt()
{
    return m_PlayerInf.ulUseSkillCnt;
}

//******************************************************************************
// get max hp
//******************************************************************************
unsigned long SaveData::getMaxHp()
{
    return m_PlayerInf.base_hp;
}

//******************************************************************************
// get group attack
//******************************************************************************
unsigned long SaveData::GetGroupAttack()
{
    unsigned long rt = 0;
    for (int i=0; i<5; i++)
    {
        SaveData_Card* card = &(m_PlayerInf.card_info[i]);
        rt += card->attack;
    }
    return rt;
}

void SaveData::setSelectRevive(short is_sel)
{
    m_StepInf.is_select_revive = is_sel;
}

short SaveData::getSelectRevive()
{
    return m_StepInf.is_select_revive;
}

void SaveData::setFreeRevive(int isFreeRevive)
{
    m_StepInf.is_iFreeRevive = isFreeRevive;
}

int  SaveData::getFreeRevive()
{
    return m_StepInf.is_iFreeRevive;
}

str_ReviveMinusCoin* SaveData::getReviveMinusCoinArr()
{
    return m_StepInf.revive_minus_coin_arr;
}


