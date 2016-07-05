#ifndef _ACCELEROMETER_TEST_H_
#define _ACCELEROMETER_TEST_H_

#include "cocos2d.h"


#define FriendGachaType 1
#define UnusualGachaType 2
#define CompGachaType 3
#define FriendGacha10Type 4
#define UnusualGacha10Type 5

using namespace cocos2d;

enum picflag{
    NOTOUGH=0,
    GACHAPIC,
    INFPIC,
};
class GachaGetRet :public CCObject{
        
public:
    int gachaType;
    bool isGoCompGacha;
    GachaGetRet() : isGoCompGacha(false) {}
};

class GachaGet: public CCLayer
{
protected:
    CCSprite* m_pBg;
    CCSprite* m_pHand;
    CCSprite* m_pHandTxt;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    GachaGetRet m_ret;
//    CCMenuItem *m_shareBtn;
    
    int gachaType;
    bool m_isGotUnknowCompGachaHero;
    bool m_isGachaCardPlus;
    
    bool m_isFromCardItem;
public:
	GachaGet(void);
    GachaGet(int type, char ucidbuf[100],CCObject* listener,SEL_CallFuncO,bool isGachaCardPlus, bool isFromCardItem = false);
	~GachaGet(void);

    virtual void didAccelerate(CCAcceleration* pAccelerationValue);

	//virtual std::string title();
	virtual void onEnter();
    
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
   // void handAction(CCObject* pSender);
    void handScroll();
    void exchangeSuperHero();
    void onExit();
    
    void dl2CallBack(CCObject *sender);
    
    int gachaStar;
    
    void setIsGotUnknowCompGachaHero(bool isOk);
    
    
private:
    
    char ucid[100];
    picflag pflag;
    float handValue;

    void closeSelf();
    void infCouldBeShow();
    void showUsrInf();
    void shareToWeibo();
    void showTorchParticle();
    void goCompGacha();
    void dropEgg();
    void screwOut();
    void cbScrewOut();
    
    // yang share sanguo
    void shareSanGuo();
};



#endif
