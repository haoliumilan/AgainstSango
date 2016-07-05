//
//  BonusItem.h
//  test
//
//  Created by mac on 13-6-5.
//
//

#ifndef __test__BonusItem__
#define __test__BonusItem__

#include "cocos2d.h"
#include "CGameData.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class BonusItem : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    int             m_iNo;
    bool            m_bShow;
    
    JiFengRecordInf m_inf;
public:
    BonusItem(int iNo, CCObject* target, SEL_CallFuncO selector);
    
    void    show(JiFengRecordInf* pInf);
    void    hide();
    
private:
    void    cbBtn(CCObject*);
};

#endif /* defined(__test__BonusItem__) */
