#include "ui_appretice_request_master.h"
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

using namespace CocosDenshion;

#define UI_TAB_MAA_ARM_LIST     100
#define UI_TAB_MAA_CONFIRM      200
#define UI_TAB_MAA_COLOR        300
#define UI_TAB_MAA_CARD_INFO    400
#define UI_TAB_MAA_SEEK_USER    500


CUIAppreticeRequestMaster::CUIAppreticeRequestMaster()
{
    m_pBasic = NULL;
    setIsVisible(false);
}

CUIAppreticeRequestMaster::~CUIAppreticeRequestMaster()
{
}

CUIAppreticeRequestMaster * CUIAppreticeRequestMaster::create(CCLayer *pBasic)
{
    if (pBasic)
    {
        if (CUIAppreticeRequestMaster *m_pUIAppreticeRequestMaster = new CUIAppreticeRequestMaster())
        {
            m_pUIAppreticeRequestMaster->m_pBasic = pBasic;
            return m_pUIAppreticeRequestMaster;
        }
    }
    return NULL;
}

void CUIAppreticeRequestMaster::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
    
    if (CListLayer *pListLayer = getListLayer())
    {
        //test
        if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
        {
            int nCount = pFriendList->count();
            for (int i = 0; i < nCount; ++i)
            {
                if (i <= pListLayer->getPreBottomShow() && i >= pListLayer->getPreTopShow())
                {
                    if (CMenuBtn *pMenuBtn = new CMenuBtn(pFriendList->getObjectAtIndex(i),enFriendRangePro))
                    {
                        pListLayer->push_back(pMenuBtn);
                    }
                }
                else
                {
                    pListLayer->push_back(NULL);
                }
            }
        }
        
        pListLayer->updatePosition();
        pListLayer->setPosition(ccp(640,pListLayer->getPosition().y));
        pListLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,pListLayer->getPosition().y)),0.8f));
    }
}

void CUIAppreticeRequestMaster::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}

bool CUIAppreticeRequestMaster::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

CListLayer * CUIAppreticeRequestMaster::getListLayer()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_ARM_LIST))
    {
        return (CListLayer*)pNode;
    }
    
    if (CListLayer *pListLayer = new CListLayer(this, callfuncO_selector(CUIAppreticeRequestMaster::onTouchEndedArmList)))
    {
        pListLayer->setReloadFunc(callfuncO_selector(CUIAppreticeRequestMaster::onReloadCellArmList));
        addChild(pListLayer, 0, UI_TAB_MAA_ARM_LIST);
        return pListLayer;
    }
    return NULL;
}

CConfirmLayer * CUIAppreticeRequestMaster::getConfirmLayer()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_CONFIRM))
    {
        return (CConfirmLayer*)pNode;
    }
    
    if (CConfirmLayer *pConfirmLayer = new CConfirmLayer(this, callfuncO_selector(CUIAppreticeRequestMaster::onConfirmFunc)))
    {
        addChild(pConfirmLayer, 2, UI_TAB_MAA_CONFIRM);
        return pConfirmLayer;
    }
    return NULL;
}

CCLayerColor * CUIAppreticeRequestMaster::getLayerColor()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_COLOR))
    {
        return (CCLayerColor*)pNode;
    }
    
    if (CCLayerColor *pLayerColor =  CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 196), 640, 960))
    {
        addChild(pLayerColor,2,UI_TAB_MAA_COLOR);
        return pLayerColor;
    }
    return NULL;
}

CCObject * CUIAppreticeRequestMaster::getCurOPCall()
{
    //test
    CFriendInf *pOPObject = NULL;
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
        {
            pOPObject = pFriendList->getObjectAtIndex(pConfirmLayer->getOPIndex());
        }
    }
    return pOPObject;
}

CShowCardInf * CUIAppreticeRequestMaster::getShowCardInfo()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_CARD_INFO))
    {
        return (CShowCardInf*)pNode;
    }
    
    if (CShowCardInf *pShowCardInf = new CShowCardInf())
    {
        addChild(pShowCardInf, 3, UI_TAB_MAA_CARD_INFO);
        return pShowCardInf;
    }
    return NULL;
}

CUISeekUser * CUIAppreticeRequestMaster::getSeekUser()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_SEEK_USER))
    {
        return (CUISeekUser*)pNode;
    }
    
    if (CUISeekUser *pSeekUser = CUISeekUser::create())
    {
        addChild(pSeekUser, 3, UI_TAB_MAA_SEEK_USER);
        return pSeekUser;
    }
    return NULL;
}

void CUIAppreticeRequestMaster::hideConfirmLayer()
{
    if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
    {
        removeChild(pConfirmLayer,true);
    }
    if (CCLayerColor *pLayerColor = getLayerColor())
    {
        removeChild(pLayerColor,true);
    }
}

void CUIAppreticeRequestMaster::createBasicChild()
{
    //返回
    _AddBackMenuItem(this,this,menu_selector(CUIAppreticeRequestMaster::btnCallBack));
    
    //标题
    _AddTitleBar(this,CGameData::Inst()->getLanguageValue("mma_request_master"));
    
    //排序
    _AddSortMenuItem(this,this,menu_selector(CUIAppreticeRequestMaster::btnCallLine));
    
    //容量框
//    _AddNumBg(this,1,5);
    
    //查找
    _AddSeekIDItem(this,this,menu_selector(CUIAppreticeRequestMaster::onSeekID));
    
    //刷新
    _AddRefreshItem(this,this,menu_selector(CUIAppreticeRequestMaster::onRefresh));
}

void CUIAppreticeRequestMaster::btnCallBack(CCObject* pSender)
{
    if (FriendMain* _pBasic =  dynamic_cast<FriendMain*>(m_pBasic))
    {
        _pBasic->btnCallback(pSender);
    }
}

void CUIAppreticeRequestMaster::btnCallLine(CCObject* pSender)
{
}

void CUIAppreticeRequestMaster::onConfirmFunc(CCObject* pObj)
{
    //播放声音
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
    
    fa_return(pObj);
    CCfmLayerRet* pRet = (CCfmLayerRet*) pObj;

    //test
    CFriendInf *pOPFriendInfo = NULL;
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
        {
            pOPFriendInfo = pFriendList->getObjectAtIndex(pConfirmLayer->getOPIndex());
        }
    }
    fa_return(pOPFriendInfo);
    
    //send mail
    if (0==pRet->iBtnSel)
    {
        OcProxy::Inst()->showMailViewController(pOPFriendInfo->name);
        schedule(schedule_selector(CUIAppreticeRequestMaster::onUpdateMail));
    }
    else if (1==pRet->iBtnSel)
    {
        //...
    }
    else if (2==pRet->iBtnSel)
    {
        if (CShowCardInf *pShowCardInf = getShowCardInfo())
        {
            pShowCardInf->show(&pOPFriendInfo->card_info,false);
            MainScene::Inst()->showBottomPanel(false);
        }
    }
    else if (3==pRet->iBtnSel)
    {
        hideConfirmLayer();
    }
}

void CUIAppreticeRequestMaster::onTouchEndedArmList(CCObject* pObj)
{
    fa_return(pObj);
    
    //播放声音
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
    
    //show color child
    if (CCLayerColor *pLayerColor = getLayerColor())
    {
        pLayerColor->setIsVisible(true);
    }
    
    //test
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        CListLayerRet* pRet = (CListLayerRet*)pObj;
        CFriendInf *pFriendInf = pFriendList->getObjectAtIndex(pRet->iBtnSel);
        if (CConfirmLayer * pConfirm = getConfirmLayer())
        {
            pConfirm->show(&pFriendInf->card_info, pFriendInf->name, enCConfirmlayer_maa_arm,pRet->iBtnSel);
        }
    }
}

void CUIAppreticeRequestMaster::onReloadCellArmList(CCObject* pObj)
{
    if (CListLayer *pListLayer = getListLayer())
    {
        //test
        if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
        {
            CListLayerRet* pRet = (CListLayerRet*)pObj;
            CFriendInf *pFriendInf = pFriendList->getObjectAtIndex(pRet->iBtnSel);
            CMenuBtn *btn = new CMenuBtn(pFriendInf,enFriendRangePro);
            pListLayer->reloadItemByIdx(btn, pRet->iBtnSel);
            btn->release();
        }
    }
}

void CUIAppreticeRequestMaster::onUpdateMail(ccTime dt)
{
    if (OcProxy::Inst()->getIsHasSentMail())
    {
        unschedule(schedule_selector(CUIAppreticeRequestMaster::onUpdateMail));
        const char *pchMailText = OcProxy::Inst()->getMailText();
        if (0<strlen(pchMailText))
        {
            //test
            if (CFriendInf *pOPObject = (CFriendInf*)getCurOPCall())
            {
                if (CGameData::Inst()->sendMailRequest(pOPObject->fid,pchMailText))
                {
                    hideConfirmLayer();
                }
            }
        }
        OcProxy::Inst()->deleteMailViewController();
    }
}

void CUIAppreticeRequestMaster::onSeekID(CCObject *pObj)
{
    hide();
    setIsVisible(true);
    if (CUISeekUser *pSeekUser = getSeekUser())
    {
        pSeekUser->show(CGameData::Inst()->getLanguageValue("mma_seek_master_title"),
                        CGameData::Inst()->getLanguageValue("mma_seek_master_hint"),
                        this,
                        callfuncO_selector(CUIAppreticeRequestMaster::onSeekResult),
                        callfuncO_selector(CUIAppreticeRequestMaster::onSeekBack)
                        );
    }
}

void CUIAppreticeRequestMaster::onRefresh(CCObject *pObj)
{
}

void CUIAppreticeRequestMaster::onSeekResult(CCObject *pObj)
{
}

void CUIAppreticeRequestMaster::onSeekBack(CCObject *pObj)
{
    setIsVisible(false);
    show();
}

extern void _AddBackMenuItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc)
{
    fa_return(pAddToNode);
    fa_return(pSelectorObj);
    fa_return(selectorFunc);
    
    static int nTag = -11899;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("id_back.png"))
        {
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("id_back.png"),
                                                                          CCSprite::spriteWithSpriteFrameName("id_back.png"),
                                                                          pSelectorObj,
                                                                          selectorFunc);
            pMenuItem->setAnchorPoint(CCPointZero);
            pMenuItem->setPosition(CCPointMake(-529, 731));
            pMenuItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
            
            CCMenu* pMenu = CCMenu::menuWithItems(pMenuItem,NULL);
            pMenu->setPosition(CCPointZero);
            pAddToNode->addChild(pMenu,1,nTag);
        }
    }
}

extern void _AddTitleBar(CCNode *pAddToNode, const char *pchText)
{
    fa_return(pAddToNode);
    fa_return(pchText);
    
    static int nTag = -11898;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (TitleBar *pTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png"))
        {
            pTitle->setTitleString(pchText);
            pTitle->setPosition(CCPointMake(-210, 766));
            pAddToNode->addChild(pTitle,1,nTag);
            pTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
        }
    }
}

extern void _AddSortMenuItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc)
{
    fa_return(pAddToNode);
    fa_return(pSelectorObj);
    fa_return(selectorFunc);
    
    static int nTag = -11897;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("id_Reset.png"))
        {
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("id_Reset.png"),
                                                                          CCSprite::spriteWithSpriteFrameName("id_Reset.png"),
                                                                          pSelectorObj,
                                                                          selectorFunc);
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_title"), 26);
            pTextNode->setPosition(ccp(53, 10));
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(CCPoint(-1000, 731));
            pAddToNode->addChild(pMenu, 1, nTag);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(525, 731)),0.8f));
        }
    }
}

extern void _AddNumBg(CCNode *pAddToNode, unsigned int unCurCount, unsigned int unSumCount)
{
    fa_return(pAddToNode);
    
    static int nTag = -11896;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName("fr_another.png"))
        {
            pAddToNode->addChild(pSprite, 1, nTag);
            pSprite->setPosition(CCPointMake(535, 167));
            
            char chBuf[128];
            sprintf(chBuf, "%d/%d",unCurCount,unSumCount);
            TextNode* frInfoLabel = TextNode::textWithString(chBuf, 24);
            frInfoLabel->setPosition(ccp(120, 18));
            pSprite->addChild(frInfoLabel);
        }
    }
}

extern void _AddSeekIDItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc)
{
    fa_return(pAddToNode);
    fa_return(pSelectorObj);
    fa_return(selectorFunc);
    static int nTag = -11895;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fr_long button.png"))
        {
            CCSprite *pSprite1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite *pSprite2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(pSprite1,
                                                                          pSprite2,
                                                                          pSelectorObj,
                                                                          selectorFunc);
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("mma_seek_id"), pSprite1->getContentSize(),CCTextAlignmentCenter,36);
            pTextNode->setShadowColor(ccBLACK);
            pTextNode->setPosition(CCPointMake(pSprite1->getContentSize().width * 0.5,pSprite1->getContentSize().height * 0.5));
            pMenuItem->setScale(0.85f);
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(ccp(-262, 144));
            //pMenu->setiPriority(-150);
            pAddToNode->addChild(pMenu, 1, nTag);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(310, 144)),0.8f));
        }
    }
}

extern void _AddRefreshItem(CCNode *pAddToNode, CCObject *pSelectorObj, SEL_MenuHandler selectorFunc)
{
    fa_return(pAddToNode);
    fa_return(pSelectorObj);
    fa_return(selectorFunc);
    static int nTag = -11894;
    if (!pAddToNode->getChildByTag(nTag))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("fr_long button.png"))
        {
            CCSprite *pSprite1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCSprite *pSprite2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
            CCMenuItem *pMenuItem = CCMenuItemImage::itemFromNormalSprite(pSprite1,
                                                                          pSprite2,
                                                                          pSelectorObj,
                                                                          selectorFunc);
            pMenuItem->setAnchorPoint(ccp(0, 0));
            TextNode *pTextNode = TextNode::textWithString(CGameData::Inst()->getLanguageValue("mma_refresh"), pSprite1->getContentSize(),CCTextAlignmentCenter,36);
            pTextNode->setShadowColor(ccBLACK);
            pTextNode->setPosition(CCPointMake(pSprite1->getContentSize().width * 0.5,pSprite1->getContentSize().height * 0.5));
            pMenuItem->setScale(0.85f);
            pMenuItem->addChild(pTextNode);
            
            CCMenu *pMenu = CCMenu::menuWithItem(pMenuItem);
            pMenu->setPosition(ccp(-100,144));
            //pMenu->setiPriority(-150);
            pAddToNode->addChild(pMenu, 1, nTag);
            pMenu->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(472, 144)),0.8f));
        }
    }
}





