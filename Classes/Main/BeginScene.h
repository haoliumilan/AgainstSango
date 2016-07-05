#ifndef __BEGIN_SCENE_H__
#define __BEGIN_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;
class CommDlg;
class Dialog2;
class Dialog1;

enum enUpdateLanguageBagState{
    enUpdateLanguageBagNull = 0,
    enUpdateLanguageBagStart,
    enUpdateLanguageBagOver,
};

class BeginScene : public cocos2d::CCLayer
{
    int                 m_iLoadIdx;
    bool                m_bChkInfPass;
    
    bool                m_isFanti;//玩家不读取文档，发送结算请求
    
    TextNode*           m_lbText;
    CCProgressTimer*    m_prgrsBar;
    CCSprite*           m_spPrgrsFr;
    enUpdateLanguageBagState m_updateLanguageBagState;
    CommDlg*            m_commDlg;
    
    Dialog2*            m_dlgSaveData;  // 提示是否读取游戏进度
    
    Dialog1*        m_getFantiAlert1;//返体的提示框
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event){return true;}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	// implement the "static node()" method manually
	LAYER_NODE_FUNC(BeginScene);
    virtual void onEnter();
    virtual void onExit();
    
    

private:
    void loadResource(int idx);
    void update(ccTime dt);
    void updateLanguageBag();
    void goToMainScene();
    void cbCommDlg();
    void showConnectingDlg();
    void showRetryDlg();
    
    bool chkInfo();
    void startToLoadResAndLang();
    
    bool showSaveDataDlg();
    void cbSaveDataDlg(CCObject* pObj);
    void showClickLabel();
    void cbGetNFantiAlert();//返体
};

#endif // __HELLOWORLD_SCENE_H__
