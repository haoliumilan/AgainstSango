//
//  PublishWeiboLayer.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_PublishWeiboLayer_h
#define AgainstWar_PublishWeiboLayer_h
#include "cocos2d.h"
#include "CGamedata.h"

using namespace cocos2d;



class PublishWeiboLayerRet :public CCObject
{
  public:
    int iSel;
};


class CommDlg;
class TextNode;

class PublishWeiboLayer :public CCLayer {
        
public:
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    PublishWeiboLayerRet  m_ret;
    enWeiboType m_weiboType;
    CCSprite *tipBack;
    TextNode *tipLabel;
    CommDlg *m_commDlg;
    CCSprite *sureItem;
    CCSprite *backItem;
    CCSprite *publishItem;
    bool isPublish;
    int m_iReqType;
    void menuCallback(CCObject *sender);
    PublishWeiboLayer(CCObject *listner,SEL_CallFuncO selector);
    void showPublishViewWithType(enWeiboType type);
    void removePublishView();
    void update(ccTime dt);
    void showWeiboBackTips();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void cbCommDlg(CCObject* sender);
};


#endif
