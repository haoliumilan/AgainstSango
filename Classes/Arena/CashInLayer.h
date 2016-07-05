//
//  CashInLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-24.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CashInLayer_h
#define AgainstWar_CashInLayer_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "TitleBar.h"
#include "CShowCardInf.h"


using namespace cocos2d;
class CUserInfo;
class TextNode;
class ActNumTTF;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CashInLayer : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    int    showTime;
    int    m_endTotalCards;             //获得卡牌总数量
    CShowCardInf*   m_cshowCardinfo;
    CCSprite*       m_ParcelTipsLayer;
  
    
private:
    void showAwardInfo();
    void showTongguanInfo();
    void showGetCardInfo();
    void showLvlUpInfo();
    
    void OnNumAnimTimer(ccTime dt);
    void cbCoinNameAct();
    void cbExpNameAct();
    void cbUpExpNameAct();
    void cbGuildAwardAct();
    
    int m_lvlUp;
    int m_cointUp;
    TitleBar *m_spTitle;
    CCLayerColor *moveLayer;
    int m_preCost;
    int m_preStamina;
    
    TextNode *floorNameLabel;
    TextNode *roomNameLabel;
    
    TextNode *coinNameLabel;
    ActNumTTF *coinLabel;
    
    TextNode *expNameLabel;
    CCSprite *spExpEffect;
    ActNumTTF *expLabel;
    
    TextNode *upExpNameLabel;
    ActNumTTF *upExpLabel;
    
    TextNode *lbGuildAward;
    ActNumTTF *numGuildAward;
    
    int             m_iTouch;

    int m_desCoin;
    int m_desExp;
    float m_tempCoin;
    float m_tempExp;
    int m_sCount;
    int m_stopCount;
    CCActionInterval *X2Action;
    CCActionInterval *ExpX2Action;
    CCActionInterval *DropX2Action;
    bool isX2Money;
    bool isX2Exp;
    bool isX2Drop;
    double exp_effect;
public:
    CashInLayer(CCObject* target, SEL_CallFunc selector);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

  //  void show(const char* floorname, const char* roomname, int coin, int exp,int lvlup,int coinUp);
    void show(const char* floorname, const char* roomname, int coin,
              int exp, int preLv, int preCost, int preStamina, double expEffect);
    virtual void onExit();
    void runMoneyX2Anim();
    void runExpX2Anim();
    void runDropX2AnimWithDelay(float dt);
    
    void setIsUseSaveData(bool bUseSaveData);
    
    
};

#endif
