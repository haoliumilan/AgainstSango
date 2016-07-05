//
//  MailBoxAgt.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-9.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "MailBoxAgt.h"
#include "OcProxy.h"
#include "HttpComm.h"
#include "jsoncpp.h"
#include "CGameData.h"

#define kMailLastRcvTimeKey "LASTRCVTIME"

MailBoxAgt* MailBoxAgt::s_pcIf = NULL;

//******************************************************************************
// initMailBox
//******************************************************************************
void MailBoxAgt::initMailBox(const char* uid)
{
    if(m_bInitFlg)
        return;
    
    m_lLastRcvTime = 0;
    m_strMyUid = uid;
    
    if(m_strMyUid.empty())
        return;
    
    m_pMailSenderInfArr = new CCMutableArray<CMailSenderInf*>();
    
    string filepath = OcProxy::Inst()->getDocumentPath();
    filepath += m_strMyUid;
    filepath += "mail";
    
    FILE* pFile = fopen(filepath.c_str(), "rb");
    
    if(!pFile)
        return;
    
    CMailSenderInf* pSenderInf;
    
    int nLen;
    
    while (1) {
        pSenderInf = new CMailSenderInf();
        
        nLen = fread(&(pSenderInf->senderInf), 1, sizeof(pSenderInf->senderInf), pFile);
        
        if(nLen < sizeof(pSenderInf->senderInf)){
            pSenderInf->release();
            break;
        }
        
        m_lLastRcvTime = MAX(m_lLastRcvTime, pSenderInf->senderInf.rcv_time);
        
        m_pMailSenderInfArr->addObject(pSenderInf);
        pSenderInf->release();
    }
    
    m_bInitFlg = true;
}

//******************************************************************************
// updateMailBox
//******************************************************************************
void MailBoxAgt::updateMailBox()
{
    Json::Value rcvMailDic = HttpComm::Inst()->getRcvMailDic();
    if(rcvMailDic.empty())
        return;
    
    CMailSenderInf sender;
    for (int i = 0; i< rcvMailDic.size(); i++) {
        string strFid("");
        if (rcvMailDic[i].isMember("uid")) {
            strFid = rcvMailDic[i]["uid"].asString();
            if(strFid.empty())
                memset(sender.senderInf.fid, 0, 100);
            else
                strncpy(sender.senderInf.fid, strFid.c_str(), 99);
        }
        else
            memset(sender.senderInf.fid, 0, 100);
        
        sender.senderInf.rcv_time = rcvMailDic[i]["datetime"].asInt();

        if (rcvMailDic[i].isMember("name")) {
            string strName = rcvMailDic[i]["name"].asString();
            if(strName.empty())
                memset(sender.senderInf.name, 0, 100);
            else
                strncpy(sender.senderInf.name, strName.c_str(), 99);
        }
        else
            memset(sender.senderInf.name, 0, 100);
        
        sender.senderInf.user_elem = rcvMailDic[i]["country"].asInt();
        
        sender.senderInf.lcard_cid = atoi(rcvMailDic[i]["leader_card"]["cid"].asString().c_str());
        sender.senderInf.lcard_lv = rcvMailDic[i]["leader_card"]["lv"].asInt();
        sender.senderInf.lcard_sklv = rcvMailDic[i]["leader_card"]["sk_lv"].asInt();
        sender.senderInf.pl_hp = rcvMailDic[i]["leader_card"]["pl_hp"].asInt();
        sender.senderInf.pl_attack = rcvMailDic[i]["leader_card"]["pl_attack"].asInt();
        sender.senderInf.pl_recover = rcvMailDic[i]["leader_card"]["pl_recover"].asInt();
        sender.senderInf.plus = sender.senderInf.pl_hp + sender.senderInf.pl_attack + sender.senderInf.pl_recover;
        sender.senderInf.mail_status = 1;
        
        CMailSenderInf* pSenderInf = getExistMail(sender.senderInf.rcv_time, strFid);
        if(pSenderInf){
            *pSenderInf = sender;
        }
        else{
            saveSenderInf(sender);
        }
        
        m_strCurMailText = rcvMailDic[i]["mail_msg"].asString();
        saveMailText(sender, m_strCurMailText);
    }
    
    sortByRcvTime();
    
    flush();
}

//******************************************************************************
// flush
//******************************************************************************
void MailBoxAgt::flush()
{
    string filepath = OcProxy::Inst()->getDocumentPath();
    filepath += m_strMyUid;
    filepath += "mail";
    
    remove(filepath.c_str());
    
    FILE* pFile = fopen(filepath.c_str(), "wb");
    
    if(!pFile)
        return;
    
    for (int i = 0; i < m_pMailSenderInfArr->count(); i++) {
        CMailSenderInf* pSendInf = m_pMailSenderInfArr->getObjectAtIndex(i);
        
        fwrite(&(pSendInf->senderInf), sizeof(pSendInf->senderInf), 1, pFile);
    }
    
    fclose(pFile);
}

//******************************************************************************
//更新好友信息
//******************************************************************************
void MailBoxAgt::updateFriendInf()
{
    CCMutableArray<CFriendInf*> * pFrndLst = CGameData::Inst()->getFriendList();
    CFriendInf* pFrndInf;
    
    CCMutableArray<CMailSenderInf*> * tmpArr = CCMutableArray<CMailSenderInf*>::arrayWithArray(m_pMailSenderInfArr);
    
    for (int i = 0; i < pFrndLst->count(); i++) {
        pFrndInf = pFrndLst->getObjectAtIndex(i);
        
        int j = 0;
        while (j < tmpArr->count()) {
            CMailSenderInf* pSendInf = tmpArr->getObjectAtIndex(j);
            
            if(strncmp(pFrndInf->fid, pSendInf->senderInf.fid, 99) == 0){
                pSendInf->senderInf.lcard_cid = pFrndInf->leadercard_cid;
                pSendInf->senderInf.lcard_lv = pFrndInf->leadercardlv;
                pSendInf->senderInf.lcard_sklv = pFrndInf->sk_lv;

                pSendInf->senderInf.pl_hp = pFrndInf->card_info.pl_hp;
                pSendInf->senderInf.pl_attack = pFrndInf->card_info.pl_attack;
                pSendInf->senderInf.pl_recover = pFrndInf->card_info.pl_recover;
                pSendInf->senderInf.plus = pFrndInf->card_info.plus;

                strncpy(pSendInf->senderInf.name, pFrndInf->name, 99);
                
                tmpArr->removeObjectAtIndex(j);
            }
            else{
                j++;
            }
        }
    }
    
    if(tmpArr->count() > 0)
        tmpArr->removeAllObjects();
}

//******************************************************************************
//取得邮件总数
//******************************************************************************
int MailBoxAgt::getMailCnt()
{
    return m_pMailSenderInfArr->count();
}

//******************************************************************************
//取得没读的邮件总数
//******************************************************************************
int MailBoxAgt::getNewMailCnt()
{
    int newMailCnt = 0;
    if (m_pMailSenderInfArr) {
        for (int i = 0; i < m_pMailSenderInfArr->count(); i++) {
            CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(i);
            if (pSenderInf->senderInf.mail_status == 1)
                newMailCnt++;
        }
    }
    
    return newMailCnt;
}

//******************************************************************************
//取得最后收件时间
//******************************************************************************
long MailBoxAgt::getLastRcvTime()
{
    return m_lLastRcvTime;
}

//******************************************************************************
//取邮件
//******************************************************************************
CMailSenderInf* MailBoxAgt::getMailInfAtIdx(int idx)
{
    if(idx >= m_pMailSenderInfArr->count())
        return NULL;
    
    CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(idx);
    
    return pSenderInf;
}

string MailBoxAgt::getMailTextAtIdx(int idx)
{
    if(idx >= m_pMailSenderInfArr->count())
        return "";
    
    char buf[100];
    CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(idx);
    snprintf(buf, 99, "ID%s%ld", pSenderInf->senderInf.fid, pSenderInf->senderInf.rcv_time);
    
    string strRet = CCUserDefault::sharedUserDefault()->getStringForKey(buf);
    
    return strRet;
}

//******************************************************************************
//删邮件
//******************************************************************************
void MailBoxAgt::delMailAtIdx(int idx)
{
    if(idx >= m_pMailSenderInfArr->count())
        return;
    
    char buf[100];
    CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(idx);
    snprintf(buf, 99, "ID%s%ld", pSenderInf->senderInf.fid, pSenderInf->senderInf.rcv_time);
    
    CCUserDefault::sharedUserDefault()->setStringForKey(buf, "");
    CCUserDefault::sharedUserDefault()->flush();
    
    m_pMailSenderInfArr->removeObjectAtIndex(idx);
    flush();
}

void MailBoxAgt::DeleteAllMail()
{
    m_pMailSenderInfArr->removeAllObjects();
    flush();
}

//******************************************************************************
//判断邮件是否在本地邮箱中已经存在
//******************************************************************************
CMailSenderInf* MailBoxAgt::getExistMail(long rcvtime, string fid)
{
    CMailSenderInf* pRet = NULL;
    
    for (int i = 0; i < m_pMailSenderInfArr->count(); i++) {
        CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(i);
        
        if((pSenderInf->senderInf.rcv_time == rcvtime)
           && (strncmp(pSenderInf->senderInf.fid, fid.c_str(), 99) == 0)){
            pRet = pSenderInf;
            break;
        }
    }
    
    return pRet;
}

//******************************************************************************
// saveSenderInf
//******************************************************************************
void MailBoxAgt::saveSenderInf(CMailSenderInf& senderInf)
{
    CMailSenderInf* pSenderInf = new CMailSenderInf();
    
    *pSenderInf = senderInf;
    
    m_pMailSenderInfArr->addObject(pSenderInf);
    
    pSenderInf->release();
    
    m_bNeedToSort = true;
}

//******************************************************************************
// saveMailText
//******************************************************************************
void MailBoxAgt::saveMailText(CMailSenderInf& senderInf, string mailtext)
{
    char buf[100];
    
    snprintf(buf, 99, "ID%s%ld", senderInf.senderInf.fid, senderInf.senderInf.rcv_time);
    
    CCUserDefault::sharedUserDefault()->setStringForKey(buf, mailtext);
    CCUserDefault::sharedUserDefault()->flush();
}

//******************************************************************************
// sortByRcvTime
//******************************************************************************
void MailBoxAgt::sortByRcvTime()
{
    if(!m_bNeedToSort)
        return;
    
    for (int i = 0; i < m_pMailSenderInfArr->count()-1; i++){
        for (int j = i+1; j < m_pMailSenderInfArr->count(); j++){
            CMailSenderInf* pSenderInf1 = m_pMailSenderInfArr->getObjectAtIndex(i);
            long iRcvTime1 = pSenderInf1->senderInf.rcv_time;
            
            CMailSenderInf* pSenderInf2 = m_pMailSenderInfArr->getObjectAtIndex(j);
            long iRcvTime2 = pSenderInf2->senderInf.rcv_time;
            
            if(iRcvTime2 > iRcvTime1){
                pSenderInf1->retain();
                m_pMailSenderInfArr->replaceObjectAtIndex(i, pSenderInf2);
                m_pMailSenderInfArr->replaceObjectAtIndex(j, pSenderInf1);
                pSenderInf1->release();
            }
        }
    }
    
    if(m_pMailSenderInfArr->count() > 0)
        m_lLastRcvTime = m_pMailSenderInfArr->getObjectAtIndex(0)->senderInf.rcv_time;
    
    m_bNeedToSort = false;
}

//******************************************************************************
// setRead
//******************************************************************************
void MailBoxAgt::setRead(int idx)
{
    if(idx >= m_pMailSenderInfArr->count())
        return;
    
    CMailSenderInf* pSenderInf = m_pMailSenderInfArr->getObjectAtIndex(idx);
    pSenderInf->senderInf.mail_status = 0;
    
    flush();
}





