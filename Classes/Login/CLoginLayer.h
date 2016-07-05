
#include "cocos2d.h"
#include "CGamedata.h"
#include "Dialog2.h"

class CommDlg;

class CLoginLayer :  public CUiLayerBase, CCTextFieldDelegate
{
    CommDlg*        m_commDlg;
    Dialog2*        m_ocDlg;    //  直接登入的警告提示框
    Dialog2*        m_invCodeErrorDlg;  //  邀请码输入错误的提示框
    int             m_iReqType;
    bool            m_bShowKeyboard;
    CCSprite*       m_spInviCode; // 验证邀请码标识
    bool            m_bTextViewEdit; // 输入框是否可以输入
    bool            m_bShowInvCodeErrorDlg;    //  是否显示邀请码输入错误的提示
    SEL_CallFunc    m_CurrentSelector;  //  当前选中的按钮的回调方法
    
    CCMenuItem*     m_btnQQ;
    CCMenuItem*     m_btnSina;
        
public:
    virtual void onEnter();
    virtual void onExit();
    
    CLoginLayer();
    
    void waitPpLogin(ccTime dt);
    void sendPPLogin();


    void sendQQLogin();
    void sendSinaLogin();
    void sendLoginFromFile();
    
    virtual void enter();
    virtual void exit();
    CCSprite*   m_loginbg;
    CCMenu*     m_btnMenu;
    CCTextFieldTTF*   m_textField;
    void enterLogin();
    
    void  newBtn(CCMenuItem *&btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition);
    
    
    void update(ccTime dt);
    
    void loginFailed();
    
    void beginLogin();
    
    void showOCDialog();
    
    void cbOCDlg(CCObject* pObj);
    
    void setKeyboard();
    void showKeyboard();
    void hideKeyboard();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void setInviCodeOk();
    void setInviCodeError();
    void startCheckInviteCode(const char* invCode);
    void checkInviteCode();
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
private:
    void cbCommDlg(CCObject* );
    void closeCommDlg(CCObject* );
    void showInvCodeErrorDlg();
    void cbInvCodeErrorDlg(CCObject*);
    void cbChkInvCode(CCObject* );
};

