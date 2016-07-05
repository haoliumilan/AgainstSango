#ifndef __TeamInfoScrollViewLayer_h__
#define __TeamInfoScrollViewLayer_h__

#include "cocos2d.h"
using namespace cocos2d;


class TeamInf;
class CTeamInfoScrollViewLayer : public CCLayer
{
public:
    CTeamInfoScrollViewLayer();
    virtual ~CTeamInfoScrollViewLayer();
    static CTeamInfoScrollViewLayer * create(CCObject* pTarget,SEL_CallFuncO selector);
    void show(unsigned int unIndex);
    void hide();
    TeamInf * getCurTeamInfo();
    
protected:
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onExit();
    
private:
    TeamInf * getTeamInfo(unsigned int unIndex);
    int getIndexAddBeIndex(unsigned int unIndex);  //指定索引的下一个存在的索引
    int getIndexDelBeIndex(unsigned int unIndex);  //指定索引的上一个存在的索引
    unsigned int count()const;
    bool beMovedAdd();
    bool beMovedDel();
    void callFuncMovedEndAdd();
    void callFuncMovedEndDel();
    void createBasicChild(CCObject* pTarget,SEL_CallFuncO selector);
    void timeShowAdd(ccTime time);
    void timeShowDel(ccTime time);
    void showTeamInfo(TeamInf *pTeamInfo,unsigned int unTeamIndex);
    
    
private:
    CCPoint                             m_posMovedTouch;
    CCSize                              m_szSize;
    int                                 m_unCurShowTeamInfoIndex;
    bool                                m_bMoved;
    float                               m_fMovedX;
};

#endif
