//
//  RewardMain.h
//  AgainstWar
//
//  Created by user on 13-8-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RewardMain__
#define __AgainstWar__RewardMain__

#include "cocos2d.h"
#include "CGameData.h"

#include "CommDlg.h"

USING_NS_CC;

enum EN_RWDMAIN_SCREEN
{
  EN_RWDSIGHLAYER = 0,
  EN_RWDLEVELUP,
  EN_RWDMONTH,
  EN_RWDLEIJI,
  EN_RWDHUODONG,
  //EN_RWDBUCHANG,
  EN_RWD_MAX,
};


enum enRawReqType{
    enRawReqType_Null,
    enRawReqType_add_award,
    enRawReqType_add_lv_5,
};


class RwdSighLayer;
class RwdLevelUpLayer;
class RwdMonthLayer;
class RwdBonusLayer;
class RwdHuodongLayer;
class RwdBuchangLayer;

class RewardMain : public CUiLayerBase
{
    RwdSighLayer*     m_rewardSignLayer;
    RwdLevelUpLayer*  m_rewardLevelUpLayer;
    RwdMonthLayer*    m_rewardMonthLayer;
    RwdBonusLayer*    m_rewardLeiJiLayer;
    RwdHuodongLayer*  m_rewardHuodongLayer;
    RwdBuchangLayer*  m_rewardBuchangLayer;
    
    
//    EN_RWDMAIN_SCREEN  m_curScreen;
    
    CCSprite* m_RightArrow;
    CCSprite* m_LeftArrow;
    
    CCSprite* signNum;
    
    int       defaultLabel;             //默认标签
    
    int       signUpNum;
    int       levelUpNum;
    int       monthsNum;
    int       bonusNum;
    int       sysNum;
    
public:
    
    int             m_iReqType;
    
    virtual void enter();
    virtual void exit();
    
    RewardMain();
    ~RewardMain();
    
    void setLabel(int label);
    void showLabel(int label);
    
    
    void update(ccTime dt);
    CommDlg*        m_commDlg;
    void cbCommDlg(CCObject* );
    
    void newbie(CCObject* pObj);
    void newbie1(CCObject* pObj);
    
    void newbie2(CCObject* pObj);
    void newbie3(CCObject* pObj);

    
private:
    
    void createAllBtn();
    string GetButtonLabel(int _index);
    
    void innerExit();
    
    void showRwdSignLayer();
    void showLevelUpLayer();
    void showMonthLayer();
    void showLeiJiLayer();
    void showHuodongLayer();
    void showBuchangLayer();
    
    
    
    void cbRwdSignLayer(CCObject* pObj);
    void cbRwdLevelUpLayer(CCObject* obj);
    void cbRwdMonthLayer(CCObject* obj);
    void cbRwdLeijiLayer(CCObject* obj);
    void cbRwdHuodongLayer(CCObject* obj);
    void cbRwdBuchangLayer(CCObject* obj);
    
    
    void cbScrollHorizontal(CCObject* obj);
};



#endif /* defined(__AgainstWar__RewardMain__) */
