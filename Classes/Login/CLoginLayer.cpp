#include "CLoginLayer.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "CommDlg.h"
#include "TextNode.h"
#include "OcProxy.h"

#define INVITATION_CODE_MAX_LENGTH 8

using namespace cocos2d;

CLoginLayer::CLoginLayer()
{
    m_CurrentSelector = NULL;
    m_commDlg = NULL;
    m_ocDlg = NULL;
    m_textField = NULL;
    m_spInviCode = NULL;
    m_invCodeErrorDlg = NULL;
    m_bShowInvCodeErrorDlg = false;
    m_btnQQ = NULL;
    m_btnSina = NULL;
    m_loginbg = NULL;
    
    m_iReqType = enLoginReqType_Null;
    m_bShowKeyboard = false;
    
    m_btnMenu=0;
    
}

void CLoginLayer::onEnter() {
    CCLayer::onEnter();
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

void CLoginLayer::onExit() {
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    CCLayer::onExit();
}

void CLoginLayer::enter()
{
    enterLogin();
}


void CLoginLayer::exit()
{
    
}


void CLoginLayer::enterLogin()
{
    removeAllChildrenWithCleanup(true);
    
#if defined(kPltfmPp)
    int logintype=CGameData::Inst()->getLoginType();
    switch (logintype) {
        case enLoginReqType_Null:
        {
            CCMenuItem*       m_btn1 = NULL;
            if(m_commDlg != NULL)
            {
                removeChild(m_commDlg, true);
                m_commDlg = NULL;
            }
            m_loginbg = CCSprite::spriteWithFile("pploginbg.jpg");
            addChild(m_loginbg, 3);
            m_loginbg->setPosition(ccp(320, 480));
            newBtn(m_btn1,"","pplogin.png",menu_selector(CLoginLayer::sendPPLogin),CCPointMake(125, 540));
            m_btnMenu = CCMenu::menuWithItems(m_btn1, NULL);
            addChild(m_btnMenu, 8);
            if(m_btnMenu)
            {
                m_btnMenu->setPosition(CCPointMake(0, 0));
            }
            break;
        }
        case enLoginReqType_Pp:
            schedule(schedule_selector(CLoginLayer::waitPpLogin), 0.5);
            OcProxy::Inst()->showPpLogin();
            break;
    }
#else
    
    CCMenuItem*       m_btn1 = NULL;
    CCMenuItem*       m_btn2 = NULL;
    CCMenuItem*       m_btn3 = NULL;
    CCMenuItem*       m_btn4 = NULL;
    
    m_bTextViewEdit = true;
    
    int logintype=CGameData::Inst()->getLoginType();
    switch (logintype) {
        case enLoginReqType_Null:
            m_loginbg = CCSprite::spriteWithFile("loginbg.jpg");
            addChild(m_loginbg, 3);
            m_loginbg->setPosition(ccp(320, 480));

            if(m_commDlg != NULL)
            {
                removeChild(m_commDlg, true);
                m_commDlg = NULL;
            }
            
            if (CGameData::Inst()->OCAccountYes()) {
                newBtn(m_btn1,"","noaccountlogin.png",menu_selector(CLoginLayer::showOCDialog),CCPointMake(125, 540));
            }
            if (CGameData::Inst()->SinaAccountYes()) {
                if (m_btn1==NULL) {
                    newBtn(m_btn1,"","sinalogin.png",menu_selector(CLoginLayer::sendSinaLogin),CCPointMake(125, 540));
                    m_btnSina = m_btn1;
                }
                else
                {
                    newBtn(m_btn2,"","sinalogin.png",menu_selector(CLoginLayer::sendSinaLogin),CCPointMake(125, 440));
                    m_btnSina = m_btn2;
                }

            }
            if (CGameData::Inst()->QQAccountYes()) {
                if (m_btn1==NULL) {
                    newBtn(m_btn1,"","QQlogin.png",menu_selector(CLoginLayer::sendQQLogin),CCPointMake(125, 540));
                    m_btnQQ = m_btn1;
                }
                else if  (m_btn2==NULL)
                {
                    newBtn(m_btn2,"","QQlogin.png",menu_selector(CLoginLayer::sendQQLogin),CCPointMake(125, 440));
                    m_btnQQ = m_btn2;
                }
                else
                {
                    newBtn(m_btn3,"","QQlogin.png",menu_selector(CLoginLayer::sendQQLogin),CCPointMake(125, 340));
                    m_btnQQ = m_btn3;
                }
            }
            
            m_textField = CCTextFieldTTF::textFieldWithPlaceHolder("", CCSizeMake(255, 80), CCTextAlignmentLeft, "huakangfont.ttf", 26);
            m_textField->setDelegate(this);
            m_textField->setColor(ccBLACK);
            m_textField->setAnchorPoint(CCPointMake(0, 0));
            m_textField->setPosition(CCPointMake(278, 642));
            addChild(m_textField, 9);
            
            if (CGameData::Inst()->getIsUseInviteCode()) {
                newBtn(m_btn4, "", "invitationCode_bg.png", menu_selector(CLoginLayer::setKeyboard), CCPointMake(122, 650));
                
                m_btnMenu = CCMenu::menuWithItems(m_btn4, m_btn1, m_btn2, m_btn3, NULL);
            }
            else {
                m_btnMenu = CCMenu::menuWithItems(m_btn1, m_btn2, m_btn3, NULL);
            }
            
            m_btnMenu->setPosition(ccp(-1000, -1000));
            addChild(m_btnMenu, 8);
            
            if(m_btnMenu)
            {
                m_btnMenu->setPosition(CCPointMake(0, 0));
            }
            
            break;
        case enLoginReqType_TencentLogin:
        case enLoginReqType_SinaLogin:
        case enLoginReqType_OcLogin:
            sendLoginFromFile();
            break;
        default:
            break;
    }
#endif
}

void CLoginLayer::sendPPLogin()
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    OcProxy::Inst()->showPpLogin();
    schedule(schedule_selector(CLoginLayer::waitPpLogin), 0.5);
}

void CLoginLayer::waitPpLogin(ccTime dt) {
    if(OcProxy::Inst()->getPpLoginStatus() == 1) //登陆成功
    {
        OcProxy::Inst()->resetPpLogin();
        OcProxy::Inst()->savePpData();
        unschedule(schedule_selector(CLoginLayer::waitPpLogin));
        sendLoginFromFile();
    }
    else if(OcProxy::Inst()->getPpLoginStatus() == -1) //异常关闭页面
    {
        removeAllChildrenWithCleanup(true);
        OcProxy::Inst()->resetPpLogin();
        
        CCMenuItem*       m_btn1 = NULL;
        if(m_commDlg != NULL)
        {
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        m_loginbg = CCSprite::spriteWithFile("pploginbg.jpg");
        addChild(m_loginbg, 3);
        m_loginbg->setPosition(ccp(320, 480));
        newBtn(m_btn1,"","pplogin.png",menu_selector(CLoginLayer::sendPPLogin),CCPointMake(125, 540));
        m_btnMenu = CCMenu::menuWithItems(m_btn1, NULL);
        addChild(m_btnMenu, 8);
        if(m_btnMenu)
        {
            m_btnMenu->setPosition(CCPointMake(0, 0));
        }
        OcProxy::Inst()->showPpLogin();
    }
}

void CLoginLayer::sendQQLogin()
{
    if (m_bShowKeyboard) {
        hideKeyboard();
        return;
    }

    m_CurrentSelector = callfunc_selector(CLoginLayer::sendQQLogin);
    
    if (m_bShowInvCodeErrorDlg) {
        showInvCodeErrorDlg();
        return;
    }
     
    if (!CGameData::Inst()->isInviteCodeRight()
        && m_textField
        && (m_textField->getCharCount() > 0)) {
        
        m_bShowInvCodeErrorDlg = true;
    }
        
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    if(CGameData::Inst()->loginByTencentRequest())
    {
        m_btnMenu->setIsTouchEnabled(false);
        m_iReqType = enLoginReqType_TencentLogin;
        scheduleUpdate();
    }
}

void CLoginLayer::sendSinaLogin()
{
    if (m_bShowKeyboard) {
        hideKeyboard();
        return;
    }

    m_CurrentSelector = callfunc_selector(CLoginLayer::sendSinaLogin);
    
    if (m_bShowInvCodeErrorDlg) {
        showInvCodeErrorDlg();
        return;
    }
    
    if (!CGameData::Inst()->isInviteCodeRight()
        && (m_textField)
        && (m_textField->getCharCount() > 0)) {
        m_bShowInvCodeErrorDlg = true;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    if(CGameData::Inst()->loginBySinaRequest())
    {
        m_iReqType = enLoginReqType_SinaLogin;
         m_btnMenu->setIsTouchEnabled(false);
        scheduleUpdate();
    }
}

void CLoginLayer::sendLoginFromFile()
{
    
    this->exit();
    beginLogin(); 
    //暂时注掉微博关注
    // CGameData::Inst()->createWeiboShipsRequest();
}

void  CLoginLayer::newBtn(CCMenuItem *&btn,const char *label,const char *pic,SEL_MenuHandler selector,const CCPoint& newPosition)
{
    CCSprite* spMenu1 = CCSprite::spriteWithFile(pic);
    CCSprite* spMenu2 = CCSprite::spriteWithFile(pic);
    
    btn = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, selector);
    btn->setPosition(newPosition);
    btn->setAnchorPoint(CCPointZero);
    
    TextNode* lbText = TextNode::textWithString(label, CCSizeMake(520, 100), CCTextAlignmentCenter, 42);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(25, 15));
    lbText->setShadowColor(ccBLACK);
    btn->addChild(lbText, 2);
}


void CLoginLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if (comState == kReqHoldOn) {
        return;
    }
    if (comState == kReqCancel) {
        if(m_commDlg)
        {
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        unscheduleUpdate();
        m_btnMenu->setIsTouchEnabled(true);
        return;
    }
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this,  callfuncO_selector(CLoginLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    unscheduleUpdate();
    m_btnMenu->setIsTouchEnabled(true);
    
    if(comState == kReqStatOk){

        switch (m_iReqType) {
                //登录成功
            case enLoginReqType_TencentLogin:
            case enLoginReqType_SinaLogin:
                sendLoginFromFile();
                break;
                
            default:
                break;
        }
    }
    else {
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(CLoginLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


//******************************************************************************
// cbCommDlg
//******************************************************************************
void CLoginLayer::cbCommDlg(CCObject* pObj)
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    loginFailed();
}

void CLoginLayer::closeCommDlg(CCObject* pObj)
{
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
}

void CLoginLayer::loginFailed()
{
    int logintype=CGameData::Inst()->getLoginType();
    if(logintype==enLoginReqType_Null)
    {
        this->enterLogin();
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(logintype==enLoginReqType_OcLogin||CGameData::Inst()->getReqStat() != 403)
    {
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(CLoginLayer::beginLogin), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        } 
        return;
    }
    
    if(CGameData::Inst()->getReqStat() == 403)
    {
        CGameData::Inst()->clearLoginDefaultData();
        
        if(m_commDlg == NULL){
            switch (logintype) {
                case enLoginReqType_TencentLogin:
                case enLoginReqType_Pp:
                case enLoginReqType_SinaLogin:
                    m_commDlg = new CommDlg(this, callfuncO_selector(CLoginLayer::closeCommDlg), enCommDlgTp_retry);
                    break;
                    
                default:
                    break;
            }

            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        } 
    }
    
    CCMenuItem*       m_btn1;
    
    if(!m_loginbg){
#if defined(kPltfmPp)
        m_loginbg = CCSprite::spriteWithFile("pploginbg.jpg");
#else
        m_loginbg = CCSprite::spriteWithFile("loginbg.jpg");
#endif
        addChild(m_loginbg);
        m_loginbg->setPosition(ccp(320, 480));
    }
    switch (logintype) {
        case enLoginReqType_TencentLogin:
            if(m_btnQQ == NULL){
                newBtn(m_btn1,"","QQlogin.png",menu_selector(CLoginLayer::sendQQLogin),CCPointMake(125, 480));
                m_btnQQ = m_btn1;
            }
            break;
            
        case enLoginReqType_SinaLogin:
            if(m_btnSina == NULL){
                newBtn(m_btn1,"","sinalogin.png",menu_selector(CLoginLayer::sendSinaLogin),CCPointMake(125, 480));
                m_btnSina = m_btn1;
            }
            break;
        case enLoginReqType_Pp:
            newBtn(m_btn1,"","pplogin.png",menu_selector(CLoginLayer::sendPPLogin),CCPointMake(125, 540));
            break;
            
        default:
            break;
    }
    
    if(m_btnMenu == NULL){
        m_btnMenu = CCMenu::menuWithItems( m_btn1, NULL);
        
        if(m_btnMenu)
            addChild(m_btnMenu, 8);
    }
    
    if(m_btnMenu){
        m_btnMenu->setPosition(CCPointMake(0, 0));
    }
 
}


void CLoginLayer::beginLogin() 
{
    bool requestInviteCode = false;
    if (m_textField) {
        if (m_textField->getCharCount() >= 7) {
            requestInviteCode = true;
            CGameData::Inst()->requestIndex(m_textField->getString());
        }
    }

    if (!requestInviteCode) {
        CGameData::Inst()->requestIndex();
        
    }
    
    if(CGameData::Inst()->getReqStat() != kReqStatOk){
        loginFailed(); 
        return;
    }
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    MainScene::Inst()->loginFinished();
}


void CLoginLayer::showOCDialog()
{
    if (m_bShowKeyboard) {
        hideKeyboard();
        return;
    }

    m_CurrentSelector = callfunc_selector(CLoginLayer::showOCDialog);
    
    if (m_bShowInvCodeErrorDlg) {
        showInvCodeErrorDlg();
        return;
    }
    
    if (m_ocDlg == NULL) {
        m_ocDlg = new Dialog2(this,
                              callfuncO_selector(CLoginLayer::cbOCDlg),
                              CGameData::Inst()->getLanguageValue("loginLayerTip1"),
                              CGameData::Inst()->getLanguageValue("loginLayerTip2"),
                              enDl2TwoBtn);
        addChild(m_ocDlg, 100);
        m_ocDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_ocDlg->release();
        m_ocDlg->setScale(0.0);
        m_ocDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
}

void CLoginLayer::cbOCDlg(CCObject* pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){    
        sendLoginFromFile();     
    }
    else {
        if (!CGameData::Inst()->isInviteCodeRight()
            && m_textField->getCharCount() > 0) {
            m_bShowInvCodeErrorDlg = true;
        }
    }
    removeChild(m_ocDlg, true);
    m_ocDlg = NULL;
}

void CLoginLayer::showInvCodeErrorDlg() {
    if (m_invCodeErrorDlg == NULL) {
        m_invCodeErrorDlg = new Dialog2(this,
                                        callfuncO_selector(CLoginLayer::cbInvCodeErrorDlg),
                                        CGameData::Inst()->getLanguageValue("invCode_error"),
                                        CGameData::Inst()->getLanguageValue("invCode_error_dlg"),
                                        enDl2TwoBtn);
        addChild(m_invCodeErrorDlg, 100);
        m_invCodeErrorDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_invCodeErrorDlg->release();
        m_invCodeErrorDlg->setScale(0.0);
        m_invCodeErrorDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
    }
}

void CLoginLayer::cbInvCodeErrorDlg(CCObject* pObj) {
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        // 修改邀请码
    }
    else {
        // 继续登入
        m_bShowInvCodeErrorDlg = false;
        (this->*m_CurrentSelector)();
    }
    
    removeChild(m_invCodeErrorDlg, true);
    m_invCodeErrorDlg = NULL;
}

void CLoginLayer::startCheckInviteCode(const char *invCode) {
    m_bTextViewEdit = false;
    if(CGameData::Inst()->checkInviteCodeRequest(invCode))
    {
        if(m_commDlg != NULL)
        {
            removeChild(m_commDlg, true);
            m_commDlg = NULL;
        }
        
        if(CGameData::Inst()->getReqStat() != kReqStatOk)
        {
            if(m_commDlg == NULL){
                m_commDlg = new CommDlg(this, callfuncO_selector(CLoginLayer::cbChkInvCode), enCommDlgTp_retry);
                addChild(m_commDlg, 1000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
                m_commDlg->setScale(0.0);
                m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            } 
        }
        else
        {
            checkInviteCode();
        }
    }   
}

void CLoginLayer::cbChkInvCode(CCObject* pObj)
{
    startCheckInviteCode(m_textField->getString());
}

void CLoginLayer::checkInviteCode() {
    m_bTextViewEdit = false;
    if (CGameData::Inst()->isInviteCodeRight()) {
        m_bShowInvCodeErrorDlg = false;
        setInviCodeOk();
    }
    else {
        m_bShowInvCodeErrorDlg = true;
        setInviCodeError();
    }
}

void CLoginLayer::setInviCodeOk() {
    if (m_spInviCode != NULL) {
        m_spInviCode->removeFromParentAndCleanup(true);
        m_spInviCode = NULL;
    }
    m_spInviCode = CCSprite::spriteWithSpriteFrameName("fr_selected.png");
    m_spInviCode->setPosition(CCPointMake(544, 680));
    addChild(m_spInviCode, 8);
    m_bTextViewEdit = true;
}

void CLoginLayer::setInviCodeError() {
    if (m_spInviCode != NULL) {
        m_spInviCode->removeFromParentAndCleanup(true);
        m_spInviCode = NULL;
    }
    m_spInviCode = CCSprite::spriteWithFile("error_sign.png");
    m_spInviCode->setPosition(CCPointMake(544, 680));
    addChild(m_spInviCode, 8);
    m_bTextViewEdit = true;
}

void CLoginLayer::setKeyboard() {
    if (m_bTextViewEdit) {
        if (!m_bShowKeyboard) {
            showKeyboard();
        }
        else {
            hideKeyboard();
        }
    }
}

void CLoginLayer::showKeyboard() {
    if (m_bTextViewEdit) {
        m_bShowInvCodeErrorDlg = false;
        if (m_spInviCode) {
            removeChild(m_spInviCode, true);
            m_spInviCode = NULL;
        }
        
        CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
        pGlView->setIMEKeyboardASCIICapable();
        m_textField->attachWithIME();
        m_bShowKeyboard = true;
    }   
}

void CLoginLayer::hideKeyboard() {
    m_textField->detachWithIME();
    CCEGLView *pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
    m_bShowKeyboard = false;
    
    if (m_textField->getCharCount() > 0) {
        startCheckInviteCode(m_textField->getString());
    }
}

#pragma mark -
#pragma mark CTouchDelegate
bool CLoginLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (m_bShowKeyboard) {
        hideKeyboard();
    }    
    
    return true;
}

#pragma mark -
#pragma mark CCTextFieldDelegate
bool CLoginLayer::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    if (!m_bTextViewEdit) {
        return true;
    }
    if (sender->getCharCount() + nLen > INVITATION_CODE_MAX_LENGTH) {
        return true;
    }
    if ((*text <= 57 && *text >= 48) || (*text <= 90 && *text >= 65) || (*text <= 122 && *text >= 97)) {
        // 邀请码仅限为数字+大小写字母
//        if (sender->getCharCount() + nLen == INVITATION_CODE_MAX_LENGTH) {
//            char chInvCode[100];
//            snprintf(chInvCode, 99, "%s%s", sender->getString(), text);
//            startCheckInviteCode(chInvCode);
//        }
        return false;
    }
    else {
        return true;
    }    
}

bool CLoginLayer::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen) {
    CCLog("onTextFieldDeleteBackward text = %s, nLen = %d, char count = %d char = %s", delText, nLen, sender->getCharCount(), sender->getString());
    if (!m_bTextViewEdit) {
        return true;
    }
    
    return false;
}


