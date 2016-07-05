//
//  MemberInfoLayer.h
//  AgainstWar
//
//  Created by Liu Hao on 13-1-9.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MemberInfoLayer__
#define __AgainstWar__MemberInfoLayer__

#include <iostream>
#include "cocos2d.h"
#include "CGameData.h"

using namespace cocos2d;

#define enMemLayBtn_OK      1001
#define enMemLayBtn_Cancel  1002

enum MemberInfoLayerType {
    memLayerType_none = 0,
    memLayerType_accept,
    memLayerType_refuse,
    memLayerType_applyFriend,
    memLayerType_delete,
    memLayerType_setViceleader,
    memLayerType_deleteViceleader,
    memLayerType_giveUpLeader,
    memLayerType_applyCountry,
};

class TextNode;

class MemberInfoLayer : public CCLayer
{
public:
    MemberInfoLayer(CCObject* target, SEL_MenuHandler selector);
    ~MemberInfoLayer();
    
    void showFriendInfoLayer(MemberInfoLayerType memInfoLayerType, CFriendInf *friendInfo);
    void showMemberInfoLayer(MemberInfoLayerType memInfoLayerType, CMember *memberInfo);
    void showCntryInfoLayer(MemberInfoLayerType memInfoLayerType, CCntryBaseInfo *cntryInfo);
    MemberInfoLayerType getMemLayerType();
private:
    virtual void onEnter();
    virtual void onExit();
    
    void newBtn(CCMenuItem *&menuItem, const char *label,
                const CCPoint& position, int tag);
    
    void setTitleString(TextNode* &tnTitle, TextNode* &tnInfo);
    
    void showSameInfoLayer();

private:
    CCObject*           m_pListener;
    SEL_MenuHandler     m_pfnSelector;
    
    MemberInfoLayerType m_memLayerType;
    
};

#endif /* defined(__AgainstWar__MemberInfoLayer__) */
