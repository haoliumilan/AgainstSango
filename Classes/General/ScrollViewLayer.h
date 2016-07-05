#ifndef __ScrollViewLayer_h__
#define __ScrollViewLayer_h__

#include "cocos2d.h"
using namespace cocos2d;

class CShowCardInf;
class CUserCardInfo;
class CCardBaseInfo;
class PackCard;
class CScrollViewLayer : public CCLayer
{
public:
    CScrollViewLayer();
    virtual ~CScrollViewLayer();
    static CScrollViewLayer * create(CCLayer *pLayer);
    void show(CUserCardInfo *pUserCardInfo);
    void show(CCardBaseInfo *pCardBaseInfo);
    void show(unsigned int unIndex);
    void hide();
    PackCard * getCurPackCard();
    CUserCardInfo * getCurUserCardInfo();
    CUserCardInfo * getStartUserCardInfo();
    PackCard * getStartPackCard();
    
    void drawAwakeSkillTip(CCPoint pos, int awakeSkillId, int idx);
    
    
    void newbie(CCObject* pObj);
    
    void newbie1(CCObject* pObj);

    
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

private:
    
    bool b_showAwakeInfo;
    
    void addOherShow(ccTime dt);
    void delOherShow(ccTime dt);
    int getIndex(CUserCardInfo *pUserCardInfo);
    PackCard * getPackCard(unsigned int unIndex);
    CUserCardInfo * getUserCardInfo(unsigned int unIndex);
    CShowCardInf * getCurShowCardInfo();
    int getIndex(CCardBaseInfo *pCardBaseInfo);
    CCardBaseInfo * getCardBaseInfo(unsigned int unIndex);
    CCardBaseInfo * getBeContentNextCardBaseInfo(unsigned int unIndex);
    CCardBaseInfo * getCurCardBaseInfo();
    int getIndexAddBeIndex(unsigned int unIndex);  //指定索引的下一个存在的索引
    int getIndexDelBeIndex(unsigned int unIndex);  //指定索引的上一个存在的索引
    void showMoveAdd(ccTime dt);
    void showMoveDel(ccTime dt);
    void stopMove(ccTime dt);
    unsigned int count()const;
    void setSourceButton(short shType);     //来源
    bool beMovedAdd();
    bool beMovedDel();
    void callFuncMovedEndAdd();
    void callFuncMovedEndDel();
    void createBasicChild();
    void loadBody(CShowCardInf *pNodeViewCall,unsigned int unIndex);
    
private:
    CCLayer                             *m_pLayer;
    CCPoint                             m_posMovedTouch;
    CCSize                              m_szSize;
    int                                 m_unCurShowCardIndex;
    bool                                m_bMoved;
    float                               m_fMovedX;
    bool                                m_bShowMoved;
    unsigned int                        m_unStartIndex;
    
    
    
};

#endif
