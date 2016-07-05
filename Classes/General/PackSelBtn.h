//
//  PackSelBtn.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-30.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_PackSelBtn_h
#define AgainstWar_PackSelBtn_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;
class TextNode;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class PackSelBtn : public CCLayer
{
    CCSprite*   m_spBtnOk;
    CCSprite*   m_spBtnClear;
    CCSprite*   m_spCoverOk;
    CCSprite*   m_spCoverClear;
    
    int         m_iSelCnt;
    int         m_iTotalExp;
    int         m_iGoldPerCard;
    int         m_allPlus;
    int         m_basePlus;
    
    int         m_iGoldSell;

    TextNode* m_lbText1;
    TextNode* m_lbText2;
    TextNode* m_lbText3;
    
public:
    PackSelBtn(char* capacity);
    PackSelBtn(char* capacity, int goldPerCard, int plus);
    
    int  getGoldSell();
    
    void add1cardUp(int exp, int plus);
    void rmv1cardUp(int exp, int plus);
    
    void add1cardSell(int gold);
    void rmv1cardSell(int gold);
    
    void setCardNum(char* capacity);
    
    void clear();
    
    virtual void onExit();
    
private:
    void initBtn();
};

#endif
