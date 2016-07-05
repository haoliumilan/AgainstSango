#ifndef __Weixinback_SCENE_H__
#define __Weixinback_SCENE_H__


#include "cocos2d.h"

using namespace cocos2d;

class CommDlg;
class TextNode;

class Weixinback : public cocos2d::CCLayer
{
private:
    TextNode    *tipLabel;
    int         m_iReqType;
    CCSprite    *sureItem;
    CommDlg     *m_commDlg;
public:
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
	LAYER_NODE_FUNC(Weixinback);
    
    void weiXinSuccess();
    
    void menuCloseCallback(CCObject* pSender);
    
    void update(ccTime dt);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void cbCommDlg(CCObject* sender);
};




#endif
