#include "TeamInfoScrollViewLayer.h"
#include "TeamInf.h"
#include "CGameData.h"
//#include "CSkillLib.h"
#include "MainScene.h"
//#include "TextNode.h"
#include "TitleBar.h"
#include "OcProxy.h"

#define UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL       100
#define UI_TAB_SCROLL_VIEW_SHARE_MENU           200
#define UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT     3

CTeamInfoScrollViewLayer::CTeamInfoScrollViewLayer()
{
    m_unCurShowTeamInfoIndex = 0;
    m_bMoved = false;
    m_fMovedX = 0.0f;
    m_szSize = CCSize(634.0f,0.0f);
    setIsVisible(false);
    
}

CTeamInfoScrollViewLayer::~CTeamInfoScrollViewLayer()
{
    
}

CTeamInfoScrollViewLayer * CTeamInfoScrollViewLayer::create(CCObject* pTarget,SEL_CallFuncO selector)
{
    if (CTeamInfoScrollViewLayer *pScrollViewLayer = new CTeamInfoScrollViewLayer())
    {
        pScrollViewLayer->createBasicChild(pTarget,selector);
        return pScrollViewLayer;
    }
    return NULL;
}

void CTeamInfoScrollViewLayer::show(unsigned int unIndex)
{
    unsigned int unCountSum = count();
    if (unIndex>=unCountSum)return;
    
    m_unCurShowTeamInfoIndex = unIndex;
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            pTeamInfo->stopAllActions();
            if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2>i) //减
            {
                //schedule(schedule_selector(CTeamInfoScrollViewLayer::timeShowDel), 0.02f);
                timeShowDel(0);
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2==i)   //中
            {
                showTeamInfo(pTeamInfo,m_unCurShowTeamInfoIndex);
                pTeamInfo->setPosition(ccp(m_szSize.width*(i-1),pTeamInfo->getPosition().y));
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT>i)      //增
            {
                //schedule(schedule_selector(CTeamInfoScrollViewLayer::timeShowAdd), 0.02f);
                timeShowAdd(0);
            }
        }
    }
    
    if (!getIsVisible())
    {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
        setIsVisible(true);
    }
    
#ifdef _DEBUG
    printf("CTeamInfoScrollViewLayer::show(%d)\n",unIndex);
    OcProxy::Inst()->showUsedMemory();
#endif
}

void CTeamInfoScrollViewLayer::hide()
{
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            pTeamInfo->hide();
        }
    }
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}

bool CTeamInfoScrollViewLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bMoved = false;
    m_fMovedX = 0.0f;
    CCPoint touchLocation = pTouch->locationInView(pTouch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
//    //
    //printf("%d",touchLocation.x);    
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            if (!pTeamInfo->getIsVisible())pTeamInfo->setIsVisible(true);
        }
    }
    m_posMovedTouch = touchLocation;
    m_bMoved = true;
    return true;
}

void CTeamInfoScrollViewLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_bMoved && !CGameData::Inst()->isNewbie())
    {
        CCPoint touchLocation = pTouch->locationInView(pTouch->view());
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        m_fMovedX = m_posMovedTouch.x - touchLocation.x;
        m_posMovedTouch = touchLocation;
        
        bool bShowCardInfo = false;
        for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
        {
            if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL +i))
            {
                if (pTeamInfo->isShowCardInfo())
                {
                    bShowCardInfo = true;
                    break;
                }
            }
        }
        
        if (!bShowCardInfo)
        {
            for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
            {
                if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL +i))
                {
                    CCPoint posSource = pTeamInfo->getPosition();
                    //pTeamInfo->setPosition(ccp(posSource.x-convertToNodeSpace(ccp(m_fMovedX,0)).x,posSource.y));
                    pTeamInfo->setPosition(ccp(posSource.x-m_fMovedX,posSource.y));
                }
            }
        }

    }
}

void CTeamInfoScrollViewLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    bool bShowCardInfo = false;
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL +i))
        {
            if (pTeamInfo->isShowCardInfo())
            {
                bShowCardInfo = true;
                break;
            }
        }
    }
    
    if (!bShowCardInfo && m_bMoved && !CGameData::Inst()->isNewbie())
    {
        float fSourceX = 0.0f;
        float fWidth = 0.0f;
        float fMovedAdd = 0.0f;
        float fMovedDel = 0.0f;
        static const float fMovedSpeedUp = 16.0f;     //加速常量值
        TeamInf *pCurTeamInf = getCurTeamInfo();
        assert(pCurTeamInf);
        static const int nTagSpeedMovedTo = 1;      //加速变化标志
        
        for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
        {
            if (TeamInf *pNode = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
            {
                CCPoint posSource = pNode->getPosition();
                fSourceX = m_szSize.width*(i-1);
                fWidth = m_szSize.width/2.0f;
                fMovedAdd = fSourceX - m_szSize.width;
                fMovedDel = fSourceX + m_szSize.width;
                CCAction *pActionMovedTo = NULL;
                
                //如果没结束加速时
                if (pNode->getActionByTag(nTagSpeedMovedTo))
                {
                    //continue;
                    break;
                }
                
                //加速移动
                if (fMovedSpeedUp<m_fMovedX)       //前移
                {
                    if (!beMovedAdd())
                    {
                        pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fSourceX, posSource.y)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurTeamInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.03f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CTeamInfoScrollViewLayer::callFuncMovedEndAdd)),
                                                                 NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                    }
                }
                else if (-fMovedSpeedUp>m_fMovedX)   //后移
                {
                    if (!beMovedDel())
                    {
                        pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fSourceX, posSource.y)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurTeamInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.03f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CTeamInfoScrollViewLayer::callFuncMovedEndDel)),
                                                                 NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedDel, posSource.y)),
                                                                 NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                    }
                }
                
                //普通移动
                else if (fSourceX>posSource.x)   //前移
                {
                    if ((fSourceX-posSource.x)<fWidth || !beMovedAdd())
                    {
                        pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fSourceX, posSource.y)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurTeamInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.03f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CTeamInfoScrollViewLayer::callFuncMovedEndAdd)),
                                                                 NULL);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 NULL);
                        }
                    }
                }
                else if (fSourceX<posSource.x)  //后移
                {
                    if ((posSource.x-fSourceX)<fWidth || !beMovedDel())
                    {
                        pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fSourceX, posSource.y)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurTeamInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.03f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CTeamInfoScrollViewLayer::callFuncMovedEndDel)),
                                                                 NULL);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedDel, posSource.y)),
                                                                 NULL);
                        }
                    }
                }
                
                if (pActionMovedTo)
                {
                    pNode->stopAllActions();
                    pNode->runAction(pActionMovedTo);
                }
            }
        }
    }
    
    if (TeamInf *pTeamInf = getCurTeamInfo())
    {
        if (CCNode *pNode = pTeamInf->getChildByTag(7000))
        {
            pNode->runAction(CCScaleTo::actionWithDuration(0.1f, 0.0f));
        }
    }
    

    
}

void CTeamInfoScrollViewLayer::onExit()
{
    CCLayer::onExit();
}

TeamInf * CTeamInfoScrollViewLayer::getTeamInfo(unsigned int unIndex)
{
    return (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+unIndex);
}

TeamInf * CTeamInfoScrollViewLayer::getCurTeamInfo()
{
    return (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2);
}

int CTeamInfoScrollViewLayer::getIndexAddBeIndex(unsigned int unIndex)
{
    if ((unIndex+1)<count())
    {
        return unIndex+1;
    }
    return -1;
}

int CTeamInfoScrollViewLayer::getIndexDelBeIndex(unsigned int unIndex)
{
    if (0<unIndex)
    {
        return unIndex-1;
    }
    return -1;
}

unsigned int CTeamInfoScrollViewLayer::count()const
{
    return kMaxTeamCnt-1;
}

bool CTeamInfoScrollViewLayer::beMovedAdd()
{
     return m_bMoved && -1<getIndexAddBeIndex(m_unCurShowTeamInfoIndex);
}

bool CTeamInfoScrollViewLayer::beMovedDel()
{
    return m_bMoved && -1<getIndexDelBeIndex(m_unCurShowTeamInfoIndex);
}

void CTeamInfoScrollViewLayer::callFuncMovedEndAdd()
{
    show(MIN(getIndexAddBeIndex(m_unCurShowTeamInfoIndex),count()-1));
}

void CTeamInfoScrollViewLayer::callFuncMovedEndDel()
{
    show(MAX(getIndexDelBeIndex(m_unCurShowTeamInfoIndex),0));
}

void CTeamInfoScrollViewLayer::createBasicChild(CCObject* pTarget,SEL_CallFuncO selector)
{
    //create cell
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (!getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            if (TeamInf *pTeamInfo = new TeamInf(pTarget,selector,true))
            {
                if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2==i)   //中
                {
                    //pTeamInfo->setIsTouchEnabled(true);
                    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(pTeamInfo, 0, false);
                    pTeamInfo->setIsVisible(true);
                }
                else
                {
                    pTeamInfo->setIsVisible(false);
                }
                addChild(pTeamInfo,0,UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i);
            }
        }
    }
    
    //分享...
    if (!getChildByTag(UI_TAB_SCROLL_VIEW_SHARE_MENU))
    {
        if (CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shareWeibo.png"))
        {
            CCMenuItem *pShareBtn =  CCMenuItemImage::itemFromNormalSprite(CCSprite::spriteWithSpriteFrameName("shareWeibo.png"),
                                                                           CCSprite::spriteWithSpriteFrameName("shareWeibo.png"),
                                                                           getCurTeamInfo(), menu_selector(TeamInf::shareSanGuo));    //注意
            pShareBtn->setAnchorPoint(CCPointZero);
            CCMenu *shareMenu = CCMenu::menuWithItem(pShareBtn);
            addChild(shareMenu,0,UI_TAB_SCROLL_VIEW_SHARE_MENU);
            shareMenu->setPosition(CCPointZero);
            pShareBtn->setScale(0.5);
            pShareBtn->setPosition(ccp(320, 740));
            
            if(CGameData::Inst()->getLoginType() == enLoginReqType_TencentLogin){
                shareMenu ->setIsVisible(CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
                
            }else if(CGameData::Inst()->getLoginType() ==  enLoginReqType_SinaLogin){
                shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
                
            }else {
                shareMenu->setIsVisible(CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg);
            }
        }
    }
    
}

void CTeamInfoScrollViewLayer::timeShowAdd(ccTime time)
{
    unschedule(schedule_selector(CTeamInfoScrollViewLayer::timeShowAdd));
    int nIndexAdd = m_unCurShowTeamInfoIndex;
    for (int i = 0;i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2>i) //减
            {
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2==i)   //中
            {
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT>i)      //增
            {
                nIndexAdd = getIndexAddBeIndex(nIndexAdd);
                if (-1!=nIndexAdd)
                {
                    showTeamInfo(pTeamInfo,nIndexAdd);
                    pTeamInfo->setPosition(ccp(m_szSize.width*(i-1),pTeamInfo->getPosition().y));
                }
                else
                {
                    pTeamInfo->hide();
                }

            }
        }
    }
}

void CTeamInfoScrollViewLayer::timeShowDel(ccTime time)
{
    unschedule(schedule_selector(CTeamInfoScrollViewLayer::timeShowDel));
    int nIndexDel = m_unCurShowTeamInfoIndex;
    for (int i = 0; i < UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT; ++i)
    {
        if (TeamInf *pTeamInfo = (TeamInf*)getChildByTag(UI_TAB_SCROLL_VIEW_TEAM_INFO_CELL+i))
        {
            if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2>i) //减
            {
                nIndexDel = getIndexDelBeIndex(nIndexDel);
                if (-1!=nIndexDel)
                {
                    showTeamInfo(pTeamInfo,nIndexDel);
                    pTeamInfo->setPosition(ccp(m_szSize.width*(i-1),pTeamInfo->getPosition().y));
                }
                else
                {
                    pTeamInfo->hide();
                }
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT/2==i)   //中
            {
            }
            else if (UI_SCROLL_VIEW_TEAM_INFO_CELL_COUNT>i)      //增
            {
            }
        }
    }
}

void CTeamInfoScrollViewLayer::showTeamInfo(TeamInf *pTeamInfo,unsigned int unTeamIndex)
{
    if (pTeamInfo)
    {
        pTeamInfo->showTeamInfoList(unTeamIndex);
        CGameData::Inst()->setCurTeamIdx(m_unCurShowTeamInfoIndex);
    }
}

















