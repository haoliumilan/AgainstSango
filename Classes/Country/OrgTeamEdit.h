//
//  OrgTeamEdit.h
//  AgainstWar
//
//  Created by liang jun on 13-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_OrgTeamEdit_h
#define AgainstWar_OrgTeamEdit_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGameData.h"

using namespace cocos2d;


class CShowCardInf;
class TextNode;
class TeamCfmCard;


#define OFFXTEMAINFO  114.0f

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgTeamEdit : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    CShowCardInf*   m_showCard;
    int             m_iCurSel;
    int             m_iCardCnt;
    
    CCPoint         m_ptTouchBegan;
    TeamCfmCard*    m_pCardHold;
    
    TextNode*       m_txtSkName;
    TextNode*       m_txtSkDsp;

    CCLayer*        m_teamLayer;
    
public:
    OrgTeamEdit(CCObject* target, SEL_CallFunc selector, char* dgName);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    void showFightTeam(int teamIdx);
    
    void menuCallBack(CCObject *pSender);
    void checkCurSel(CCPoint& pos);
    
    bool lookupCidInString(int cid, string& str);
    
    void updateAutoSkill(ccTime dt);
    
    bool isTeamPass();
    void showRentCardsNotPlacedDlg();
    void cbRentCardsNotPlacedDlg();
    
    void writeToTmpTeam();
};

#endif
