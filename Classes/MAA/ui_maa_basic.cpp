#include "ui_maa_basic.h"
#include "CGameData.h"
#include "MainScene.h"
#include "TitleBar.h"
#include "FriendMain.h"
//#include "CListLayer.h"
#include "CMenuBtn.h"
#include "SimpleAudioEngine.h"
#include "CConfirmLayer.h"
#include "OcProxy.h"
#include "CShowCardInf.h"
#include "ui_seek_user_id.h"

#define UI_TAB_MAA_BASIC        -11899
#define UI_TAB_MAA_TITLE_BAR    -11898
#define UI_TAB_MAA_SORT         -11897
#define UI_TAB_MAA_NUM_BG       -11896
#define UI_TAB_MAA_SEEK_ID      -11895
#define UI_TAB_MAA_REFRESH      -11894
#define UI_TAB_MAA_SEEK_USER    -11893

CUIMAABasic::CUIMAABasic()
{
    m_pBasic = NULL;
    m_selectorFuncBack = NULL;
    setIsVisible(false);
}

CUIMAABasic::~CUIMAABasic()
{
}

void CUIMAABasic::show()
{
    assert(false);
}

void CUIMAABasic::hide()
{
}

bool CUIMAABasic::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void CUIMAABasic::createBackMenuItem()
{
    if (!getChildByTag(UI_TAB_MAA_BASIC))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("id_back.png"))
        {
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("id_back.png"),
                                                                          CCSprite::spriteWithSpriteFrameName("id_back.png"),
                                                                          this,
                                                                          menu_selector(CUIMAABasic::btnCallBack));
            pMenuItem->setAnchorPoint(CCPointZero);
            pMenuItem->setPosition(CCPointMake(-529, 731));
            pMenuItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
            
            CCMenu* pMenu = CCMenu::menuWithItems(pMenuItem,NULL);
            pMenu->setPosition(CCPointZero);
            addChild(pMenu,1,UI_TAB_MAA_BASIC);
        }
    }
}

void CUIMAABasic::setBackMenuItemFunc(SEL_MenuHandler selector)
{
    m_selectorFuncBack = selector;
}

void CUIMAABasic::createTitleBar(const char *pchText)
{
    if (!getChildByTag(UI_TAB_MAA_TITLE_BAR))
    {
        if (TitleBar *pTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png"))
        {
            pTitle->setTitleString(pchText);
            pTitle->setPosition(CCPointMake(-210, 766));
            addChild(pTitle,1,UI_TAB_MAA_TITLE_BAR);
            pTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        }
    }
}

void CUIMAABasic::createSortMenuItem()
{
    if (!getChildByTag(UI_TAB_MAA_SORT))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("id_Reset.png"))
        {
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("id_Reset.png"),
                                                                          CCSprite::spriteWithSpriteFrameName("id_Reset.png"),
                                                                          this,
                                                                          menu_selector(CUIMAABasic::btnSort));
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_title"), 26);
            pTextNode->setPosition(ccp(53, 10));
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(CCPoint(-1000, 731));
            addChild(pMenu, 1, UI_TAB_MAA_SORT);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731)),0.8f));
        }
    }
}

void CUIMAABasic::createNumBg(unsigned int unCurCount, unsigned int unSumCount)
{
    if (!getChildByTag(UI_TAB_MAA_NUM_BG))
    {
        if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("fr_another.png"))
        {
            char chBuf[128];
            sprintf(chBuf, "%d/%d",unCurCount,unSumCount);
            if (TextNode* frInfoLabel = TextNode::textWithString(chBuf, 24))
            {
                frInfoLabel->setPosition(ccp(120, 18));
                pSprite->addChild(frInfoLabel);
            }
            pSprite->setPosition(CCPointMake(535, 167));
            addChild(pSprite, 1, UI_TAB_MAA_NUM_BG);
        }
    }
}

void CUIMAABasic::createSeekIDItem()
{
    if (!getChildByTag(UI_TAB_MAA_SEEK_ID))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fr_long button.png"))
        {
            CCSprite *pSprite1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite *pSprite2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(pSprite1,
                                                                          pSprite2,
                                                                          this,
                                                                          menu_selector(CUIMAABasic::onSeekID));
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("maa_seek_id"), pSprite1->getContentSize(),CCTextAlignmentCenter,36);
            pTextNode->setShadowColor(ccBLACK);
            pTextNode->setPosition(CCPointMake(pSprite1->getContentSize().width * 0.5,pSprite1->getContentSize().height * 0.5));
            pMenuItem->setScale(0.85f);
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(ccp(-262, 144));
            this->addChild(pMenu, 1, UI_TAB_MAA_SEEK_ID);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(310, 144)),0.8f));
        }
    }
    createSeekUser();
}

void CUIMAABasic::createRefreshItem()
{
    if (!getChildByTag(UI_TAB_MAA_REFRESH))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fr_long button.png"))
        {
            CCSprite *pSprite1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite *pSprite2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(pSprite1,
                                                                          pSprite2,
                                                                          this,
                                                                          menu_selector(CUIMAABasic::onSeekResult));
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("maa_refresh"), pSprite1->getContentSize(),CCTextAlignmentCenter,36);
            pTextNode->setShadowColor(ccBLACK);
            pTextNode->setPosition(CCPointMake(pSprite1->getContentSize().width * 0.5,pSprite1->getContentSize().height * 0.5));
            pMenuItem->setScale(0.85f);
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(ccp(-100,144));
            this->addChild(pMenu, 1, UI_TAB_MAA_REFRESH);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(472, 144)),0.8f));
        }
    }
}

void CUIMAABasic::createSeekUser()
{
    if (!getChildByTag(UI_TAB_MAA_SEEK_USER))
    {
        if (CUISeekUser *pSeekUser = CUISeekUser::create())
        {
            addChild(pSeekUser, 3, UI_TAB_MAA_SEEK_USER);
        }
    }
}

void CUIMAABasic::btnCallBack(CCObject* pSender)
{
    if (m_selectorFuncBack)
    {
        (m_pBasic->*m_selectorFuncBack)(this);
    }
}

void CUIMAABasic::btnSort(CCObject *pSender)
{
}

void CUIMAABasic::onSeekID(CCObject *pObj)
{
    hide();
    setIsVisible(true);
    createSeekUser();
    if (CUISeekUser *pSeekUser = getSeekUser())
    {
        pSeekUser->show(CGameData::Inst()->getLanguageValue("maa_seek_master_title"),
                        CGameData::Inst()->getLanguageValue("maa_seek_master_hint"),
                        this,
                        callfuncO_selector(CUIMAABasic::onSeekResult),
                        callfuncO_selector(CUIMAABasic::onSeekBack)
                        );
    }
}

void CUIMAABasic::onRefresh(CCObject *pObj)
{
}

void CUIMAABasic::onSeekResult(CCObject *pObj)
{
}

void CUIMAABasic::onSeekBack(CCObject *pObj)
{
    setIsVisible(false);
    show();
}

CUISeekUser * CUIMAABasic::getSeekUser()
{
    return (CUISeekUser*)getChildByTag(UI_TAB_MAA_SEEK_USER);
}

#include <sys/time.h>
#include <sstream>
extern uint64_t NowUsec()
{
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec);
}

uint64_t g_StopwatchStart = 0;
extern void  StopwatchStart()
{
#ifdef DEBUG
    printf("-------开始计时-------\n");
    g_StopwatchStart = NowUsec();
#endif
}

extern uint64_t StopwatchStop()
{
#ifdef DEBUG
    if (0<g_StopwatchStart)
    {
        uint64_t end = NowUsec();
        std::ostringstream oss;
        oss << (unsigned long)(end - g_StopwatchStart);
        printf("----花费[%s]微秒----\n",oss.str().c_str());
        uint64_t t = end - g_StopwatchStart;
        g_StopwatchStart = 0;
        return t;
    }
#endif
    return 0;
}











