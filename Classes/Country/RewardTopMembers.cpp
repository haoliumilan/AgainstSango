//
//  RewardTopMembers.cpp
//  AgainstWar
//
//  Created by user on 13-8-5.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RewardTopMembers.h"


RewardTopMembers::RewardTopMembers(CCObject* target, SEL_CallFuncO selector)
{
    m_dlgNoTip = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_topMemberList = new CListLayer(this, callfuncO_selector(RewardTopMembers::cbList));
    addChild(m_topMemberList, -1);
    m_topMemberList->release();
    m_topMemberList->setReloadFunc(callfuncO_selector(RewardTopMembers::insertBaoKuItemByIdx));
    
    showTopMemberList();
}

RewardTopMembers:: ~RewardTopMembers()
{
    
}

void RewardTopMembers::cbList(CCObject* pObj)
{
    
}


void RewardTopMembers::insertBaoKuItemByIdx(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(CGameData::Inst()->getRwdTopMembers()->getObjectAtIndex(i),i+1);
    m_topMemberList->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
}


void RewardTopMembers::showAlertNoTopDlg()
{
    if (m_dlgNoTip == NULL)
    {
        m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(RewardTopMembers::cbAlertDlg));
        addChild(m_dlgNoTip);
        m_dlgNoTip->release();
        m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("reward_top_wupaiming"));
        m_dlgNoTip->setPosition(CCPoint(960, 400));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
        m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
}

void RewardTopMembers::cbAlertDlg()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
    
    m_ret.iBtnSel = 0;
    (m_pListener->*m_pfnSelector)(&m_ret);
}


void RewardTopMembers::showTopMemberList()
{
    
    CMenuBtn* newMenuBtn;
    CCMutableArray<CRwdTopMemberInfo*>* TopMemberInfoArr = CGameData::Inst()->getRwdTopMembers();
    
    if (TopMemberInfoArr->count() <= 0)
    {
        showAlertNoTopDlg();
        return;
    }
    
    for (int i = 0; i < TopMemberInfoArr->count(); i++)
    {
        if (i >= m_topMemberList->getPreTopShow()
            && i <= m_topMemberList->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(TopMemberInfoArr->getObjectAtIndex(i), i+1);
        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_topMemberList->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
        
    }
    
}







