//
//  OtherMain.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_OtherMain_h
#define AgainstWar_OtherMain_h

#include "cocos2d.h"
#include "CGamedata.h"

using namespace cocos2d;

class CListLayer;
class CScrollBar;
class SettingLayer;
class Package;
class OtherCardLayer;
class CShowCardInf;
class RaidersLayer;
class CommDlg;
class TitleBar;
class CMenuBtn;
class Dialog2;
class CNotice;
class CDKeyExchange;
class MyVideoLayer;
class CScrollViewLayer;
class RemindLayer;

enum othermainScreen {
    ots_seeting = 1,
    ots_card ,
    ots_raider,
    ots_about,
    ots_cdkey,
    ots_remind,
    };

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OtherMain : public CUiLayerBase
{
    friend class CScrollViewLayer;
public:
    OtherMain();
    
    virtual void enter();
    virtual void exit();
    virtual void onExit();
    
    float cardLayerY;
    
    void enterMyVideo();    // from first layer to my video
private:
    CListLayer *    mainList;
    TitleBar *      m_spTitle;
    CCMenuItem *    m_backItem;
    SettingLayer *  m_settingLayer;
    RemindLayer *   m_remindLayer;
    MyVideoLayer*   m_upload_layer;
    
    Package*        m_package;
    OtherCardLayer* cardLayer;
    CShowCardInf *  m_showCardInfo;
    CScrollViewLayer    *m_pScrollViewLayer;
    
    RaidersLayer *  textLayer;
    CommDlg*        m_commDlg;
    CCMenuItem      *m_shareBtn;
    CCSprite*       numberTips;
    CMenuBtn*       m_qqBind;
    CMenuBtn*       m_sinaBind;
    CMenuBtn*       m_helpBtn;
    Dialog2*        m_confirmSwitchUser;
    int m_curScene;
    int m_iReqType;
    bool            m_bFromHome;
    CDKeyExchange*  m_pCDKeyDialog;
    
    
    void cbCommDlg(CCObject* );
    void innerEnter();
    void innerExit();
    void cbList(CCObject* pObj);
    void btnCallback(CCObject* sender);
    void cbSettignLayer(CCObject *sender);
    void cbRemindLayer(CCObject *sender);
    void cbOtherCardLayer(CCObject *sender);
    void update(ccTime ct);
    void dl2CallBack(CCObject *sender);
    void cbDL2SwitchUser(CCObject* OKorCancel);
    
    void enterCollection();
    
    void showCDKeyDialog();
    void cbCDKeyDialog();
    void exchangeCDKey();
    void showExchangeSuccessDlg();
    void cbExchangeSuccessDlg();
    
    void getAllCards();
    
    void sendQQLogin();
    void sendSinaLogin();
    
    void showBindResult();
    void bindSuccess();
    void shareToWeibo();
    // yang share sanguo
    void shareSanGuo();
    
    void initMainList();
    void showTips();
        
    // upload video 
    void enterUploadView();
    void cbUploadVideo();
    
    void enterVideoStrategy();
    
};

#endif
