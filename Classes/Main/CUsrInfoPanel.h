//
//  CUsrInfoPanel.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CUsrInfoPanel_h
#define AgainstWar_CUsrInfoPanel_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"

using namespace cocos2d;

class ActNumTTF;
class TextNode;

class CUsrInfoPanel : public CCLayer
{
    CCSprite*       m_spCntryInfoBg;
    CCSprite*       m_spUserElem;
    CCSprite*       m_spExpBar;
    CCSprite*       m_spStaminaBar;
    CCSprite*       m_spGoldOrSupply;
    ActNumTTF*      m_lbGoldOrSupply;
    ActNumTTF*      m_lbContribution;
    CCSprite*       m_spStaminaBuff;
    
    CCLabelTTF*     m_lbName;
    ActNumTTF*      m_lbLv;
    ActNumTTF*      m_lbCoin;
    TextNode*       m_lbAcPointTime;
    ActNumTTF*      m_lbAcPointValue;
    TextNode*       m_lbStaminaTime;
    ActNumTTF*      m_lbStaminaValue;
    CUserInfo*      m_pUsrInf;
    CCountryInfo*   m_pCntryInfo;
    TextNode*       m_txtTitleName;
    
    float  countDown;
    int  tempStamina;
    int  baseStamina;
    
    float m_acPointCountDown;
    int   m_tempAcPoint;
    int   m_baseAcPoint;
    
    long baseTime;
    
    int             m_iCountry;
    
    //个人信息动态变化 量
    int u_count;
    bool isJustLogin;
    
    string          m_strCdTip;
    bool            m_bShowCntryInfo;
public:
    CUsrInfoPanel();
    
    void cleanCurrentInfo();
    void showCntryInfo();
    void showUserInfo();
    void showData();
    bool getIsShowCntryInfo();
    void updateCntryNameCard();
private:
    void showAcPoint();
    void updateAcPoint();
    void updateTime(ccTime dt);
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnter();
    virtual void onExit();
    
    //===========liang add=========================
    void setExpInfo();
    void showExpInfo(CCPoint pos);
    
    void showCountryDetail(CCPoint pos);
    void setCountryDetail();
    void crtCountryDetailLayer();
    
    
    CCSprite* ExpLayer;
    void crtExpLayer();

    TextNode* curLevel;// m_lbName;
    TextNode* curLvExp;
    TextNode* nextLvExp;
    //============liang=====================
    
    CCSprite* m_CounDLayer;
    TextNode* m_tcountrydetail;
    
};

#endif
