#ifndef __ui_appretice_request_master__
#define __ui_appretice_request_master__

#include "ui_maa_basic.h"

class CListLayer;
class CConfirmLayer;
class CShowCardInf;
class CUISeekUser;

//拜 师
class CUIMAARequestMaster : public CUIMAABasic
{
private:
    CUIMAARequestMaster();
    virtual ~CUIMAARequestMaster();
    
public:
    static CUIMAARequestMaster * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
protected:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    CListLayer * getListLayer();
    CConfirmLayer * getConfirmLayer();
    CCLayerColor * getLayerColor();
    CCObject * getCurOPCall();
    CShowCardInf * getShowCardInfo();
   
    void createBasicChild();
    void hideConfirmLayer();
    virtual void onTouchEndedArmList(CCObject* pSender);
    virtual void onReloadCellArmList(CCObject* pSender);
    virtual void btnCallLine(CCObject* pSender);
    virtual void onConfirmFunc(CCObject* pSender);
    virtual void onUpdateMail(ccTime dt);
    
};

#endif
