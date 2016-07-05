//
//  AddFriend.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_AddFriend_h
#define AgainstWar_AddFriend_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;


class CUserCardInfo;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class AddFriendRet : public CCObject
{
public:
    char fid[100];
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class AddFriend : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    AddFriendRet    m_ret;
    bool            m_bFriend;
public:
    AddFriend(CCObject* target, SEL_CallFuncO selector);
    
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void show();
    void hide();
    
    void setIsUseSaveData(bool bUseSaveData);
private:
    void procAsFriend();
    void procAsOther();
    void ProcFriendInfBarFromSaveData();    // 使用备份数据
};

#endif
