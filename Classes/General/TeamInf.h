//
//  TeamInf.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-28.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_TeamInf_h
#define AgainstWar_TeamInf_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

#define STARTX_SUB 251.0f
#define OFFX_SUB 76.0f

class MsgBox;
class Dialog2;
class CShowCardInf;
class CommDlg;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TeamInfRet : public CCObject
{
public:
    int     iSel;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TeamInf : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    TeamInfRet      m_ret;
    
    CCSprite*       m_spCard[5];
    int             m_cost[5];
    
    MsgBox*         m_msgbox;
    int             m_iNewbieMsgIdx;
    
    bool            m_bTouchMvFlg;
    Dialog2*        m_resetDlg;
    CShowCardInf*   m_showCard;
    CommDlg*        m_commDlg;
    bool            m_bUseScrollView;
public:
    TeamInf(CCObject* target, SEL_CallFuncO selector,bool bUseScrollView = false);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void hide();
    virtual void onExit();
    void resetTeamInfo();
    void cbResetTeamInfo(CCObject*);
    void showResetDlg();
    void shareToWeibo();
    void dl2CallBack(CCObject *sender);
    // yang share sanguo
    void shareSanGuo();
    void showTeamInfoList(int pageIdx);
    bool isShowCardInfo();
    
    
    bool newBool;
    
private:

    //长按显示详细信息
    void waitShowInfo();
    CCPoint beganPoint;
    bool isLongPress;

    void update(ccTime dt);
    
    void cbCommDlg(CCObject* );
    
    //旧新手引导，不再使用
    //void newbieEnd(CCObject* pObj);
    
    const char* getTeamInfoPrompt();
    
    void updateCost();
    void updateCostText(ccTime dt);
    int                 show_user_cost;              //显示时刻用户cost
    unsigned short      show_hours_append_cost;
    unsigned long       show_hours_endTime;          //限时卡(显示时刻[限时卡]时间)
    unsigned short      show_month_append_cost;
    unsigned long       show_month_endTime;          //月卡(显示时刻[月卡]时间)
    
    
    
    
};

#endif
