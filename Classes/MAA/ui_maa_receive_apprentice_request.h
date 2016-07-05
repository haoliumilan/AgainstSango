#ifndef __ui_maa_receive_apprentice_request__
#define __ui_maa_receive_apprentice_request__
#include "ui_maa_basic.h"

//收徒请求
class CUIMAAReceiveApprenticeRequest : public CUIMAABasic
{
private:
    CUIMAAReceiveApprenticeRequest();
    virtual ~CUIMAAReceiveApprenticeRequest();
    
public:
    static CUIMAAReceiveApprenticeRequest * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
private:
    void createBasicChild();
    //Call列表
    void createCallList();
    
protected:
    //call列表-回调
    virtual void onTouchEndedCallList(CCObject *pSender);
    virtual void onReloadCellList(CCObject *pSender);
    
protected:
    CListLayer * getCallList();
    
};

#endif
