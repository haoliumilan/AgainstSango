//
//  MemberManageLayer.h
//  AgainstWar
//
//  Created by liang jun on 13-04-25.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MemberManageLayer__
#define __AgainstWar__MemberManageLayer__

#include <iostream>
#include "cocos2d.h"
#include "CListLayer.h"
#include "CommDlg.h"

USING_NS_CC;

enum EN_REQTYPE_MEMBERMANAGE
{
    enCReqTypeVoice = 0,
    enCReqTypePingbi,
};


class MemberManageLayer : public CCLayer
{
public:
    MemberManageLayer(CCObject* pListener, SEL_CallFuncO pfnSelector);
    ~MemberManageLayer();
    
    virtual void onEnter();
    virtual void onExit();
    
   
    
private:
    void cbCommDlg(CCObject* pObj);
    CommDlg*    m_commDlg;
    void update(ccTime dt);
    
    EN_REQTYPE_MEMBERMANAGE m_curReqType;
    void cbList(CCObject* pObj);

    void showMemberList();
    void showMemberItem(int index);
    void showAverageItem();
    void showContributionItem();
    
    void JinYanControl(CCObject* pSender);
    void PingBiControl(CCObject* pSender);
    
    int   m_iUid;
    bool  m_on_off;
    bool  m_bPingbi;
    
public:
    
    
private:
    
    CCObject*           m_pListener;
    SEL_CallFuncO       m_pfnSelector;

    CListLayer*         m_list;
};


#endif
