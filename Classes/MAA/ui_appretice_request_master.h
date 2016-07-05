#ifndef __ui_appretice_request_master__
#define __ui_appretice_request_master__

#include "cocos2d.h"
USING_NS_CC;
class CListLayer;
class CConfirmLayer;
class CShowCardInf;
class CUISeekUser;

//徒弟请求师
class CUIAppreticeRequestMaster : public cocos2d::CCLayer
{
private:
    CUIAppreticeRequestMaster();
    virtual ~CUIAppreticeRequestMaster();
    
public:
    static CUIAppreticeRequestMaster * create(CCLayer *pBasic);
    
    void show();
    void hide();
    
protected:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    CListLayer * getListLayer();
    CConfirmLayer * getConfirmLayer();
    CCLayerColor * getLayerColor();
    CCObject * getCurOPCall();
    CShowCardInf * getShowCardInfo();
    CUISeekUser * getSeekUser();
   
    void createBasicChild();
    void hideConfirmLayer();
    void onTouchEndedArmList(CCObject* pObj);
    void onReloadCellArmList(CCObject* pObj);
    void btnCallBack(CCObject* pSender);
    void btnCallLine(CCObject* pSender);
    void onConfirmFunc(CCObject* pObj);
    void onUpdateMail(ccTime dt);
    void onSeekID(CCObject *pObj);
    void onRefresh(CCObject *pObj);
    void onSeekResult(CCObject *pObj);
    void onSeekBack(CCObject *pObj);
    
private:
    CCLayer                 *m_pBasic;
    
};


#define fa_return(n)if (!(n)){assert(false);return;}

//增加返回
extern void _AddBackMenuItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc);
//增加标题
extern void _AddTitleBar(CCNode *pAddToNode, const char *pchText);
//增加排序
extern void _AddSortMenuItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc);
//增加当前/容量框
extern void _AddNumBg(CCNode *pAddToNode, unsigned int unCurCount, unsigned int unSumCount);
//增加查找ID Item
extern void _AddSeekIDItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc);
//增加刷新 Item
extern void _AddRefreshItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc);


#endif
