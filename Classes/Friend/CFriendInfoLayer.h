//
//  CFriendInfo.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CFriendInfo_h
#define AgainstWar_CFriendInfo_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"

using namespace cocos2d;
class CListLayer;
class CScrollBar;
class CUserCardInfo;
class CShowCardInf;
class TextNode;

enum FriendInfoLayerType {
     friInfo_delete = 1,
     friInfo_accept,
     friInfo_refuse,
     friInfo_addFriend,
};

#define enFILayerBtnOk         111
#define enFILayerBtnCancel     112

class  CFriendInfoLayerRet:public CCObject
{
public:
    int     iBtnSel;
};

class  CFriendInfoLayer:public CCLayer{
            
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CFriendInfoLayerRet m_ret;
public:
    char friendAddId[50];
    void enterWithType(FriendInfoLayerType type,CFriendInf *fId);
    void enter();
    void exit();
    void backFromShow();
    void clearFriendList();
    CFriendInfoLayer(CCObject* target, SEL_CallFuncO selector);
    FriendInfoLayerType getLayerType();
    bool addFriend();

private:
    TextNode *titleLabel;
    TextNode *infoLabel;
    CCLayerColor *m_background;
    CCMenuItem *sureItem;
    CCMenuItem *cancelItem;
    CListLayer *friendInfolist;
    void menuCallback(CCObject *pSender);
    void cbList(CCObject* pObj);
    CUserCardInfo *cardInfo;
    CShowCardInf *m_showCard;
    CCMenuItem *m_backItem;
    FriendInfoLayerType layerType;
    CFriendInf *showFrInfo;
};

#endif
