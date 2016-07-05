//
//  ParcelPageTwo.h
//  AgainstWar
//
//  Created by AllenSK on 14-2-13.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ParcelPageTwo__
#define __AgainstWar__ParcelPageTwo__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CScrollBar.h"
#include "ItemCard.h"
#include "MainScene.h"
#include "UseItemCard.h"
#include "TextNode.h"
#include "GachaGet.h"
#include "CShowCardInf.h"
#include "CountryDialog.h"
#include "RareGacha10Show.h"
#include "TitleBar.h"


using namespace cocos2d;

class UseResultDialog;
class CommDlg;
class Dialog1;

enum enParcelPageTwoReqType {
    enParcelPageTwoReqType_Null = 0,
    
    enParcelPageTwoReqType_HeCheng,
    enParcelPageTwoReqType_AllCard,

};



class ParcelPageTwoSelRet : public CCObject
{
public:
    int iBtnSel;
    int iActType;
};

class ParcelPageTwo : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    ParcelPageTwoSelRet   m_ret;
    
    CommDlg*            m_commDlg;
    void cbCommDlg(CCObject* );

    enParcelPageTwoReqType   m_curReqType;
    CShowCardInf *   m_showCardInf;
    bool             m_bIspress;
  
public:
    
    void showInfo(int num);
    
    CCSprite* ExpLayer;
    void crtExpLayer();
    void update(ccTime dt);
    
    ParcelPageTwo(CCObject* target, SEL_CallFuncO selector);
    ~ParcelPageTwo();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    
    virtual void exit();
    virtual void enter();
    
    TitleBar*       m_spTitle;
    CCMenuItem*     m_miBack;
    
    CCMenuItem*     m_HeCheng;

    
    int m_pType;
    int m_pNum;
    
    void btnCallback();
    void btnCallHeCheng();
    
    void aniZhuan();
    void aniZhuanEnd();
    void aniZhuanEnd2();
    
    
    void showItem(int num);
    void showChild(int num);
    
    bool canParcel;
    bool aniPlayIng;
    
    bool success;
private:
    void checkTouchHeadIcon(CCPoint pos);
    
};



#endif /* defined(__AgainstWar__ParcelPageTwo__) */
