//
//  BonusPanel.h
//  test
//
//  Created by mac on 13-6-5.
//
//

#ifndef __test__BonusPanel__
#define __test__BonusPanel__

#include "cocos2d.h"

using namespace cocos2d;

class BonusList;
class CommDlg;
class CountryDialog;
class TextNode;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class BonusPanel : public CCLayer
{
    BonusList*      m_pBonusList;
    CommDlg*        m_commDlg;
    
    string          m_strUid;
    long            m_lGuildCoin;
    long            m_lAssignCoin;
    
    CCLayer*        m_lyPanel;
    CountryDialog*  m_cntryDialog;
    
    TextNode*       m_txtCoin;
    
public:
    BonusPanel();
    
    void toggleSort();
    
    bool backBtnProc();

private:
    void cbBonusList(CCObject* pObj);
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);

    void cbCntryDlg();
};

#endif /* defined(__test__BonusPanel__) */
