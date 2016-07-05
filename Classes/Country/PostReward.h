//
//  PostReward.h
//  AgainstWar
//
//  Created by user on 13-7-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__PostReward__
#define __AgainstWar__PostReward__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CScrollBar.h"
#include "ItemCard.h"
#include "MainScene.h"
#include "TextNode.h"
#include "CountryDialog.h"
#include "RareGacha10Show.h"
#include "TitleBar.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "Dialog2.h"
#include "CommDlg.h"
#include "NoFriendLayer.h"
#include "TipsPopInfo.h"

USING_NS_CC;

enum enPostRewardReqType
{
    en_reqAcceptReward = 0,
    en_reqRecieveReward,
};

class PostRewardRet : public CCObject
{
public:
    int     iBtnSel;
    string  taskId;
};

class UseResultDialog;
class CommDlg;
class Dialog1;


class PostReward : public CCLayer
{
    CCObject*                       m_pListener;
    SEL_CallFuncO                   m_pfnSelector;
    PostRewardRet                   m_ret;
    
    int                             m_listSel;       // List select index
    CListLayer*                     m_listPostReward; //悬赏列表
    
    Dialog2*                        m_dlgTip;
    NoFriendLayer*                  m_dlgNoTip;
    
    CommDlg*                        m_commDlg;
    enPostRewardReqType             m_curReqType;
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    
    void showAlertNoTaskDlg();
    void cbAlertDlg();
    
    void showPostReward();
    void showPostRewardList();
    void insertRewardByIdx(CCObject *pObj);
    void cbJoinPostReward(CCObject* pObj);
    void showJoinPostRewardTips(long lCoin);
    void cbJoinPostRewardTips(CCObject *pObj);
    void reqAcceptRewardTask();
    void xuanShangBaoKu(CCObject* pObj);
    void faBuXuanShang(CCObject* pObj);
    
    void cbList(CCObject* pObj);
    void cbListBegin(CCObject* pObj);
    void cbListEnd(CCObject* pObj);
    
    int m_iTouchSel;
    
public:
    PostReward(CCObject* target, SEL_CallFuncO selector);
    ~PostReward();
    
private:    
    void cbShowRewardInfo(CCObject* pObj);
    void cbRecieveReward(CCObject* pObj);
    
    void reqRecieveRewardTask();
};


#endif /* defined(__AgainstWar__PostReward__) */



