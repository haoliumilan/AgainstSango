//
//  CCardInfoDlg.h
//  CardDemo
//
//  Created by Hao Zhang on 12-5-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CCardInfoDlg_h
#define CardDemo_CCardInfoDlg_h

#include "cocos2d.h"
#include "Card.h"

using namespace cocos2d;

class TextNode;

class CCardInfoDlg : public CCLayer
{
    CCSprite*   m_spDlg;
    
    TextNode* m_lbName;
    TextNode* m_lbElem;
    TextNode* m_lbAwake;
    TextNode* m_lbSkill;
    TextNode* m_lbRemainCD;
    
    CCSprite* m_spYanChiIcon;
    
    CCardInfoDlg();
public:
    static CCardInfoDlg* initDlg(){
        CCardInfoDlg* pDlg = new CCardInfoDlg();
        
        pDlg->autorelease();
        
        return pDlg;
    }
    
    void setCardInfo(Card* card);
    void setCardInfo(int iElem, int iAttack, const char* cardName);
    void setSkill(int skillID, int cd, bool bYanChi = false);
    void setPosition(const CCPoint& newPosition);
    
    virtual void onExit();
private:
    
};

#endif
