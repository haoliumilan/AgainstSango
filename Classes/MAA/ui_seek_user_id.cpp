#include "ui_seek_user_id.h"
#include "CommonDef.h"
#include "TextNode.h"


#define UI_TAB_WINDOW_LAYER         100
#define UI_TAB_TEXT_FIELD_TTF       200

CUISeekUser::CUISeekUser()
{
    setIsVisible(false);
    m_pBasic = NULL;
    m_pSeekFunc = NULL;
    m_pBackFunc = NULL;
}

CUISeekUser * CUISeekUser::create()
{
    if (CUISeekUser *pSeekUser = new CUISeekUser())
    {
        pSeekUser->setIsVisible(false);
        return pSeekUser;
    }
    return NULL;
}

void CUISeekUser::show(const char *pchTitle, const char *pchHint, CCObject *pBasic, SEL_MenuHandler pSeekFunc, SEL_MenuHandler pBackFunc)
{
    fa_return(pchTitle);
    fa_return(pchHint);
    fa_return(pBasic);
    fa_return(pSeekFunc);
    
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_pBasic = pBasic;
    m_pSeekFunc = pSeekFunc;
    m_pBackFunc = pBackFunc;
    
    createBasicChild(pchTitle,pchHint);
    
    if (CCTextFieldTTF *pTextFieldTTF = getTextFieldTTF())
    {
        pTextFieldTTF->setString("");
        CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
        pGlView->setIMEKeyboardNumber();
        pTextFieldTTF->attachWithIME();
    }
}

void CUISeekUser::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
}

bool CUISeekUser::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (CCTextFieldTTF *pTextFieldTTF = getTextFieldTTF())
    {
        if (CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView())
        {
            if (pGlView->getIMEKeyboardState())
            {
                pTextFieldTTF->detachWithIME();
            }
            else
            {
                pTextFieldTTF->attachWithIME();
            }
        }
    }
    return true;
}

bool CUISeekUser::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    
    if (sender->getCharCount() + nLen > 9) {
        return true;
    }
    if (*text <= 57 && *text >= 48) {
        return false;
    }
    else {
        return true;
    }
}

bool CUISeekUser::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}

void CUISeekUser::createBasicChild(const char *pchTitle, const char *pchHint)
{
    if (!getChildByTag(UI_TAB_WINDOW_LAYER))
    {
        if (CCLayer *pWinLayer = CCLayer::node())
        {
            if (CCSprite *pSprite = CCSprite::spriteWithFile("fr_window3.png"))
            {
                pSprite->setScale(2.0f);
                pSprite->setPosition(ccp(320, 0));
                pWinLayer->addChild(pSprite);
            }
            
            if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("fr_id field.png"))
            {
                pSprite->setPosition(ccp(320, 250-(120+160)));
                pWinLayer->addChild(pSprite);
            }
            
            if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("fr_id field.png"))
            {
                pSprite->setPosition(ccp(320, 370-(120+160)));
                pWinLayer->addChild(pSprite);
            }
            
            //提示
            if (TextNode *tintLabel = TextNode::textWithString(pchHint, 26))
            {
                tintLabel->setPosition(ccp(320, 300-(120+160)));
                tintLabel->setColor(ccc3(82, 48, 11));
                pWinLayer->addChild(tintLabel);
            }
            
            //自己ID
            if (CUserInfo *pUserInfo = CGameData::Inst()->getUsrInfo())
            {
                char chBuf[128];
                sprintf(chBuf, "%s%s",CGameData::Inst()->getLanguageValue("searchFriendTip1"),pUserInfo->uid);
                if (TextNode *pTextNode = TextNode::textWithString(chBuf, 26))
                {
                    pTextNode->setColor(ccc3(82, 28, 11));
                    pTextNode->setPosition(ccp(320, 370-(120+160)));
                    pWinLayer->addChild(pTextNode);
                }
            }
            
            //seek bun
            if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fr_button.png"))
            {
                CCSprite *pSprite1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                CCSprite *pSprite2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
                
                //text
                TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("searchFriendTip3"),
                                                               CCSizeMake(127, 30),
                                                               CCTextAlignmentCenter,
                                                               kBtnTitleHeight);
                pTextNode->setShadowColor(ccBLACK);
                pTextNode->setPosition(CCPoint(pSprite1->getContentSize().width * 0.5 - 2, pSprite1->getContentSize().height * 0.5 + 2));
                
                //item
                CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(pSprite1, pSprite2,this,menu_selector(CUISeekUser::seekMenuCall));
                pMenuItem->addChild(pTextNode);
                pMenuItem->setAnchorPoint(ccp(0, 0));
                
                //menu
                CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
                pMenu->setPosition(ccp(216+24, 140-(120+160)-5));
                pMenu->setAnchorPoint(ccp(0, 0));
                pWinLayer->addChild(pMenu);
            }
            
            addChild(pWinLayer,0,UI_TAB_WINDOW_LAYER);
            pWinLayer->setPosition(ccp(-700, 565));
            pWinLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 565)), 0.8f));
        }
    }
    
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_seek_master_title"));
}

CCTextFieldTTF * CUISeekUser::getTextFieldTTF()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_WINDOW_LAYER))
    {
        if (CCNode *_pNode = pNode->getChildByTag(UI_TAB_TEXT_FIELD_TTF))
        {
            return (CCTextFieldTTF*)_pNode;
        }
        
        if (CCTextFieldTTF * pTextFieldTTF = CCTextFieldTTF::textFieldWithPlaceHolder("",CCSize(640,80) ,CCTextAlignmentCenter,"huakangfont.ttf", 26))
        {
            pTextFieldTTF->setPosition(ccp(320, 250-(120+160)));
            pTextFieldTTF->setDelegate(this);
            pNode->addChild(pTextFieldTTF,0,UI_TAB_TEXT_FIELD_TTF);
            return pTextFieldTTF;
        }
    }
    return NULL;
}

void CUISeekUser::seekMenuCall(CCObject *pSender)
{
    if (m_pBasic && m_pSeekFunc)
    {
        if (CCTextFieldTTF *pTextFieldTTF = getTextFieldTTF())
        {
            if (0<strlen(pTextFieldTTF->getString()))
            {
                pTextFieldTTF->detachWithIME();
                (m_pBasic->*m_pSeekFunc)(pSender);
            }
        }
    }
}

void CUISeekUser::btnCallBack(CCObject *pSender)
{
    hide();
    if (m_pBasic && m_pBackFunc)
    {
        (m_pBasic->*m_pBackFunc)(pSender);
    }
}





