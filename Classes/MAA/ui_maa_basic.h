#ifndef __ui_maa_basic__
#define __ui_maa_basic__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

//查询师傅信息
class CMAASeekManageInfo
{
    
};

//查询徒弟数据
class CMAASeekApprenticeInfo
{
    
};





class CListLayer;
class CUISeekUser;
class CUIMAABasic : public cocos2d::CCLayer
{
public:
    CUIMAABasic();
    virtual ~CUIMAABasic();
    virtual void show();
    virtual void hide();
    void setBackMenuItemFunc(SEL_MenuHandler selector);
    
protected:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    //返回
    void createBackMenuItem();
    //标题
    void createTitleBar(const char *pchText);
    //排序
    void createSortMenuItem();
    //当前/容量框
    void createNumBg(unsigned int unCurCount, unsigned int unSumCount);
    //查找ID Item
    void createSeekIDItem();
    //刷新 Item
    void createRefreshItem();
    
protected:
    //返回-回调
    virtual void btnCallBack(CCObject *pSender);
    //排序-回调
    virtual void btnSort(CCObject *pSender);
    //查找ID-回调
    virtual void onSeekID(CCObject *pSender);
    //刷新-回调
    virtual void onRefresh(CCObject *pSender);
    //查找ID结果-回调
    virtual void onSeekResult(CCObject *pSender);
    //查找ui返回-回调
    virtual void onSeekBack(CCObject *pSender);

protected:
    CUISeekUser * getSeekUser();
    
private:
    void createSeekUser();
    
protected:
    CCObject                 *m_pBasic;
    SEL_MenuHandler          m_selectorFuncBack;
    
};

#define fa_return(n)if (!(n)){assert(false);return;}

#define UI_MAA_REQUEST_MASTER                   0x00000001      //拜 师
#define UI_MAA_RECEIVE_APPRENTICE_REQUEST       0x00000002      //收徒请求
#define UI_MAA_MASTER_MANAGE                    0x00000004      //师傅管理
#define UI_MAA_RECEIVE_APPRENTICE               0x00000008      //收 徒
#define UI_MAA_REQUEST_MASTER_REQUEST           0x00000010      //拜师请求
#define UI_MAA_APPRENTICE_MANAGE                0x00000020      //徒弟管理

//计时
extern uint64_t NowUsec();
extern void StopwatchStart();
extern uint64_t StopwatchStop();

#endif
