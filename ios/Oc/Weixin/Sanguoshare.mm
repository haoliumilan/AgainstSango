#include "Sanguoshare.h"
#include "SimpleAudioEngine.h"
#include "AppController.h"
#include "ios_sheet.h"
#import "EAGLView.h"
#import "MainScene.h"
#import "Weixinback.h"
#import "Dialog1.h"
#import "Alipayback.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#endif


using namespace CocosDenshion;

static ios_sheet * xxsheet=NULL;

Sanguoshare* Sanguoshare::pInstance=NULL;

/************************************************************************/
/*
 游戏中的信息
 */
/************************************************************************/

Sanguoshare* Sanguoshare::Instance(void){
    if (pInstance==NULL) {
        pInstance=new Sanguoshare();
    }
    return pInstance;
}


Sanguoshare::~Sanguoshare(){
    
    pInstance->release();
    [xxsheet release];
    
}





bool Sanguoshare::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    m_pListener     =   NULL;
    m_pfnSelector   =   NULL;
    theMainListener =   NULL;
    shareType       =   0;
    weixin          =   NULL;
    mAlipay         =   NULL;
    theFightListener=   NULL;
    weixinUnLoadDlg =   NULL;
    
    return true;
}


void Sanguoshare::shareSanGuoCallback(CCObject* target, SEL_CallFunc selector,int tpe)
{
    if(CGameData::Inst()->isSoundOn)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btnok.caf");
    
    this->m_pListener   = target;
    this->m_pfnSelector = selector;
    
    if (xxsheet==NULL) {
        xxsheet=[[ios_sheet alloc] initWithNibName:nil bundle:nil];
        [[EAGLView sharedEGLView] addSubview:(xxsheet.view)];
    }
    
    [xxsheet setWxTpe:tpe];
    [xxsheet boxSheet:tpe];
    
}

//******************************************************************************
// weixiback
//******************************************************************************

void Sanguoshare::weixincallback(int i){

    /// 处理微信发送出去后
    if (!theMainListener)
        return;
    if (i==0)
    {
        if (!weixin)
        {
            weixin=Weixinback::node();
            ((MainScene*)theMainListener)->addChild(weixin,200000);
        }
        
        weixin->weiXinSuccess();
               
    }else{
        
        //  CCMessageBox("快去分享吧","分享不成功");
        
    }
    
}

void Sanguoshare::alipaycallback(int i){
    if (!theMainListener)
        return;
    /* if (!mAlipay)
     {
     mAlipay=Alipayback::node();
     ((MainScene*)theMainListener)->addChild(mAlipay,200000);
     }
     mAlipay->alipaySuccess(i);*/
    if (theFightListener) {
        ((FightMngr*)theFightListener)->showAlipayOver(i);
        theFightListener = NULL;
        return;
    }
    ((MainScene*)theMainListener)->enableBottomPanel(false);
    ((MainScene*)theMainListener)->cbAlipay(i);
}

void Sanguoshare::callweibo(){
    
    
    if (m_pfnSelector&&m_pListener) {
        (m_pListener->*m_pfnSelector)();
    }

}

void Sanguoshare:: shareSanGuoInit( CCObject* target,bool isFight){
    if (!isFight)
        this->theMainListener=target;
    else
        this->theFightListener=target;
    
}

//******************************************************************************
// weixinunload
//******************************************************************************
void Sanguoshare::showWeixinUnLoadDlg(){
    
    
    if (theMainListener) {
        
        weixinUnLoadDlgCallback();
        
        weixinUnLoadDlg = new Dialog1(this, callfuncO_selector(Sanguoshare::weixinUnLoadDlgCallback),CGameData::Inst()->getLanguageValue("weixinunload") ,false);
        weixinUnLoadDlg->setPosition(CCPointMake(320, 480));
        ((MainScene*)theMainListener)->addChild(weixinUnLoadDlg);
        weixinUnLoadDlg->setScale(0.0);
        weixinUnLoadDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        weixinUnLoadDlg->release();
        
        
    }
    
    
}
void Sanguoshare::weixinUnLoadDlgCallback(){
    
    if (weixinUnLoadDlg) {
        weixinUnLoadDlg->removeFromParentAndCleanup(true);
        weixinUnLoadDlg= NULL;
    }
    
}



void Sanguoshare::setMainMenu(bool ft){
    if (theMainListener) {
        ((MainScene*)theMainListener)->enableBottomPanel(ft);
    }
    
}









