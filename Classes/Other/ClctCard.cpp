//
//  ClctCard.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-12-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "ClctCard.h"
#include "CGameData.h"

//******************************************************************************
// Constructor
//******************************************************************************
ClctCard::ClctCard(int cid, int status)
{
    m_spHead = NULL;
    
    m_iCid = cid;
    m_iStatus = status;
}


//******************************************************************************
// show
//******************************************************************************
void ClctCard::show()
{
    if(m_spHead != NULL)
        return;
    
    if(m_iStatus == 0){
        m_spHead = CCSprite::spriteWithFile("fr_query_box.PNG");
    }
    else{
        m_spHead = CGameData::Inst()->getHeadSprite(m_iCid);
    }
    
    if(m_spHead){
        addChild(m_spHead);
        m_spHead->setAnchorPoint(ccp(0, 1));
        
        //究极金边
        if(m_iStatus != 0 ) {
            CCardBaseInfo *pUsrCardInfo = CGameData::Inst()->getCardBaseInfByCid(m_iCid);
            
            if(pUsrCardInfo) {
                CCSprite* sp1 = NULL;
                if (pUsrCardInfo->bIsEluTgt) {
                    sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
                }
                else if (pUsrCardInfo->bIsWkTgt) {
                    sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
                }
                
                if (sp1) {
                    m_spHead->addChild(sp1);
                    sp1->setAnchorPoint(CCPointMake(0, 1));
                    sp1->setPosition(ccp(4, m_spHead->getContentSize().height-4));
                }
            }
        }
        
        char number[20];
        sprintf(number, "./%d",m_iCid);
        
        CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png");
        
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* lbNo = CCLabelAtlas::labelWithString(number,
                                                               "lvnum.png",
                                                               sz.width / 18,
                                                               sz.height, '(');
            sz = m_spHead->getContentSize();
            m_spHead->addChild(lbNo, 1);
            lbNo->setAnchorPoint(ccp(0.5, 0.0));
            lbNo->setPosition(ccp(sz.width * 0.5, 0));
        }
        
        if(m_iStatus == 1)
        {
            m_spHead->setOpacity(128);
            
        }
    }
}

//******************************************************************************
// hide
//******************************************************************************
void ClctCard::hide()
{
    removeAllChildrenWithCleanup(true);
    
    m_spHead = NULL;
}
