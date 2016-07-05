//
//  TitleBar.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_TitleBar_h
#define AgainstWar_TitleBar_h

#include "cocos2d.h"

using namespace cocos2d;

#define kTitleBarSprite    1000
#define kTitleBarLabel     1001

class TextNode;
class TitleBar :public CCSprite, public CCTouchDelegate
{
    
private:
    void removePreTitle();  // 如果当前titleBar上面已经add了Sprite或label，则remove掉
public:
    void setTitleString(const char* str);   // 就是add label
    void setTitleStringLab(const char* str); //add系统文字label
    void setTitleSprite(const char* name);  // 如果能取到Sprite，add Sprite；如果取不到，add label
    void SetTitlePosition(CCPoint pos);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

    static TitleBar* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
    
    ~TitleBar();
};

class TitleBarNoAction :public CCSprite, public CCTouchDelegate
{

public:
    static TitleBarNoAction* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
};


#endif
