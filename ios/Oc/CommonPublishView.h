//
//  CommonPublishView.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CommonPublishView__
#define __AgainstWar__CommonPublishView__

#include <iostream>
#include "cocos2d.h"
#include "CGamedata.h"

using namespace cocos2d;

class CommonPublishView :public CCObject
{
private:
    std::string  m_strWeiboContent;
public:
    static CommonPublishView* Inst();
    void showPublishView(enWeiboType weiboType);
    void setPublishViewHiden(bool isHide);
    void removePublishView();
    const char *getWeiboContent();
};

#endif /* defined(__AgainstWar__CommonPublishView__) */
