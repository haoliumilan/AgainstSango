//
//  MailBoxAgt.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-9.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MailBoxAgt__
#define __AgainstWar__MailBoxAgt__

#include "CommonDef.h"
#include "cocos2d.h"

using namespace cocos2d;

// 若要往此结构体中添加元素，要确保总体size不变，否则玩家旧邮件将遭破坏
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
typedef struct{
    unsigned short  lcard_cid;
    unsigned short  lcard_lv;
    unsigned short  lcard_sklv;//
	unsigned char   user_elem;
    char			mail_status; //0: 已读 1: new
	long			rcv_time;
    
    unsigned short  pl_hp;
    unsigned short  pl_attack;
    unsigned short  pl_recover;
    unsigned short  plus;
    
    int             i_awakeNum;
    
	char            fid[100]; //  fid[0] == 0 => 系统邮件
	char            name[100];
}stMailSenderInf;

class CMailSenderInf: public CCObject
{
public:
    stMailSenderInf senderInf;
    
    CMailSenderInf(){
        memset(&senderInf, 0, sizeof(senderInf));
    }
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class MailBoxAgt {
    static MailBoxAgt* s_pcIf;
    
    CCMutableArray<CMailSenderInf*> *m_pMailSenderInfArr;
    
    string  m_strCurMailText;
    
    string  m_strMyUid;
    
    bool    m_bInitFlg;
    
    bool    m_bNeedToSort;
    
    long    m_lLastRcvTime;

public:
	//Get the instance pointer
	static MailBoxAgt* Inst(){
        if(s_pcIf == NULL){
            s_pcIf = new MailBoxAgt();
        }
        
        return s_pcIf;
    }
    
    static void destroy(){
        if(s_pcIf){
            delete s_pcIf;
            s_pcIf = NULL;
        }
    }
    
    ~MailBoxAgt(){
        m_pMailSenderInfArr->release();
    }
    
    //初始化本地邮箱，从文件读取未删除邮件
    void initMailBox(const char* uid);
    
    //添加新邮件到mailbox
    void updateMailBox();
    
    //写入文件
    void flush();
    
    //更新好友信息
    void updateFriendInf();
    
    //取得邮件总数
    int  getMailCnt();
    
    //取得没读的邮件总数
    int getNewMailCnt();
    
    //取得最后收件时间
    long getLastRcvTime();
    
    //取邮件
    CMailSenderInf* getMailInfAtIdx(int idx);
    string getMailTextAtIdx(int idx);
    
    //删邮件
    void delMailAtIdx(int idx);
    void DeleteAllMail();
    
    //设置邮件为已读
    void setRead(int idx);
    
private:
    //判断邮件是否在本地邮箱中已经存在
    CMailSenderInf* getExistMail(long rcvtime, string fid);
    
    void saveSenderInf(CMailSenderInf& senderInf);
    void saveMailText(CMailSenderInf& senderInf, string mailtext);
    
    void sortByRcvTime();
    
private:
    //forbid constructor being called outside
	MailBoxAgt():m_bInitFlg(false), m_bNeedToSort(false){};
    
	//forbid copy constructor being called outside
	MailBoxAgt(const MailBoxAgt& );
	
	//forbid assignment
	MailBoxAgt& operator=(const MailBoxAgt& );
    
};

#endif /* defined(__AgainstWar__MailBoxAgt__) */
