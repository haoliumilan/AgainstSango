#ifndef __ui_seek_user_id_h__
#define __ui_seek_user_id_h__

#include "ui_maa_basic.h"
#include "CommonDef.h"
#include "CGamedata.h"

class TextNode;

//搜索ID
class CUISeekUser : public CUIMAABasic,CCTextFieldDelegate
{
public:
    static CUISeekUser * create();
    void show(const char *pchTitle, const char *pchHint, CCObject *m_pBasic, SEL_MenuHandler pSeekFunc, SEL_MenuHandler pBackFunc);
    virtual void hide();
    
protected:
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
private:
    CUISeekUser();
    void createBasicChild(const char *pchTitle, const char *pchHint);
    CCTextFieldTTF * getTextFieldTTF();
    void seekMenuCall(CCObject *pSender);
    void btnCallBack(CCObject *pSender);
    
    
private:
    CCObject                *m_pBasic;
    SEL_MenuHandler         m_pSeekFunc;
    SEL_MenuHandler         m_pBackFunc;
    
    
};
#endif
