#ifndef __Sanguoshare_SCENE_H__
#define __Sanguoshare_SCENE_H__


#include "cocos2d.h"
#import "MainScene.h"
class Weixinback;
class Dialog1;
class Alipayback;

using namespace cocos2d;

class Sanguoshare : public cocos2d::CCLayer
{
private:
    CCObject*           m_pListener;
    SEL_CallFunc        m_pfnSelector;
    
    CCObject*           theMainListener;
    CCObject*           theFightListener;
    int                 shareType;
    
    Weixinback* weixin;
    Alipayback* mAlipay;
    Dialog1*  weixinUnLoadDlg;

public:
    static Sanguoshare* pInstance;
    static Sanguoshare* Instance(void);
    
    ~Sanguoshare();
    virtual bool init();
    
    void shareSanGuoCallback(CCObject* target, SEL_CallFunc selector,int type=0);
    
    void weixincallback(int i);
    
    void alipaycallback(int i);
    //weibo
    void callweibo();
    
    void shareSanGuoInit( CCObject*  target,bool isFight = false);
    
    
    //  setter_getter shareType
    
    void setShareType(int type) {   shareType  =type;      };
    int  retunShareType()       {   return     shareType;  };
    
    // unload_weixin;
    
    void showWeixinUnLoadDlg();
    void weixinUnLoadDlgCallback();
    
    void setMainMenu(bool ft);
};


#define theshareSanguo   Sanguoshare::Instance()

#endif 
