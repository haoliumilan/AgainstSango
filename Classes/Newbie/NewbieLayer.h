//
//  NewbieLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_NewbieLayer_h
#define AgainstWar_NewbieLayer_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class ShowSelCountry;
class CommDlg;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class NewbieLayer : public CCLayer
{
    int             m_iCountry;
    ShowSelCountry* m_showSelCtry;
    
    CommDlg*        m_commDlg;
public:
    NewbieLayer();
    
    void show();

    virtual void onExit();
    
private:
    void btnCallback(CCObject* sender);
    
    void cbShowSelCtry(CCObject* );
    
    void update(ccTime dt);
    
    void cbCommDlg(CCObject* );
};

#endif
