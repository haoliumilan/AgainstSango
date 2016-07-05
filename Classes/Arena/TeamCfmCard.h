//
//  TeamCfmCard.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-2-25.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__TeamCfmCard__
#define __AgainstWar__TeamCfmCard__

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class CUserCardInfo;

enum enCfmCardType{
    enCfmCard_Own,
    enCfmCard_Helper,
    enCfmCard_Rent,
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TeamCfmCard : public CCNode
{
public:
    string  m_ucid;
    int     m_cid;
    bool    m_bLeader;
    int     m_iCardType;
    int     m_star;
    int     m_level;
    
public:
    TeamCfmCard(CUserCardInfo* pUsrCardInfo, bool bRent = false);
};

#endif /* defined(__AgainstWar__TeamCfmCard__) */
