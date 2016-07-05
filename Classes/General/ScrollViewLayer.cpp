#include "ScrollViewLayer.h"
#include "CShowCardInf.h"
#include "CGameData.h"
//#include "CSkillLib.h"
#include "MainScene.h"
//#include "TextNode.h"
#include "TitleBar.h"
#include "Package.h"
#include "PackCard.h"
#include "OtherMain.h"
#include "OtherCardLayer.h"
#include "OcProxy.h"
#include "TipsAwakeSkill.h"
#include "ui_maa_basic.h"

#include "Arrow.h"


#define TAB_SCROLL_VIEW_BG              100         //背景
#define TAB_SCROLL_VIEW_TORCH           200         //火炬
//#define TAB_SCROLL_VIEW_SOUECE_BUTTON   300         //来源button
#define TAB_SCROLL_VIEW_RETURN_BUTTON   300         //返回button
#define TAB_SCROLL_VIEW_CALL            400         //内容索引计数
#define SCROLL_VIEW_CELL_COUNT          3           //内容cell数量  注意:value = (n*2+1)

#define TAB_SCROLL_LAYER                500         //scroll层

#define kNewbie                         99987
static const int nTagSpeedMovedTo = 1;      //加速变化标志

CScrollViewLayer::CScrollViewLayer()
{
    b_showAwakeInfo = false;
    m_bMoved = false;
    m_fMovedX = 0.0f;
    m_unCurShowCardIndex = 1;
    m_pLayer = NULL;
    m_bShowMoved = true;
    setIsVisible(false);
    createBasicChild();
}

CScrollViewLayer::~CScrollViewLayer()
{
}

void CScrollViewLayer::createBasicChild()
{
    //绘画背景
    if (!getChildByTag(TAB_SCROLL_VIEW_BG))
    {
        if (CCSprite* spBg = CCSprite::spriteWithFile("show_bg.jpg"))
        {
            spBg->setScale(1.0/0.8);
            spBg->setPosition(CCPointMake(320, 480));
            addChild(spBg,0,TAB_SCROLL_VIEW_BG);
            m_szSize = spBg->getContentSize();
            m_szSize.width *= spBg->getScale();
            m_szSize.height *= spBg->getScale();
        }
    }
    
    //返回button
    if (!getChildByTag(TAB_SCROLL_VIEW_RETURN_BUTTON))
    {
        if (m_pLayer && dynamic_cast<OtherMain*>(m_pLayer))
        {
            CCSprite *backBtn = CCSprite::spriteWithSpriteFrameName("id_back.png");
            backBtn->setPosition(ccp(54,766));
            addChild(backBtn,0,TAB_SCROLL_VIEW_RETURN_BUTTON);
        }
    }
    
    //绘画火炬
    if (!getChildByTag(TAB_SCROLL_VIEW_TORCH))
    {
        if (CCParticleTorch *pTorch = CCParticleTorch::node())
        {
            pTorch->setPosition(CCPointMake(50, 492));
            pTorch->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
            addChild(pTorch,0,TAB_SCROLL_VIEW_TORCH);
        }
    }
    
    if (!getChildByTag(TAB_SCROLL_VIEW_TORCH+1))
    {
        if (CCParticleTorch *pTorch = CCParticleTorch::node())
        {
            pTorch = CCParticleTorch::node();
            pTorch->setPosition(CCPointMake(590, 492));
            pTorch->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
            addChild(pTorch,0,TAB_SCROLL_VIEW_TORCH+1);
        }
    }
    
    //初始来源button
    TitleBar *pTitleBar_ = (TitleBar*)getChildByTag(kLaiYuan);
    if (!pTitleBar_) pTitleBar_ = (TitleBar*)getChildByTag(kXiangQing);
    if (!pTitleBar_)
    {
        if (TitleBar *pTitleBar = TitleBar::spriteWithSpriteFrameName("id_Reset.png"))
        {
            pTitleBar->setPosition(CCPointMake(580, 766));
            pTitleBar->setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
            pTitleBar->SetTitlePosition(CCPointMake(12, 15));
            addChild(pTitleBar,0,kLaiYuan);
            pTitleBar->setIsVisible(false);
            CCTouchDispatcher::sharedDispatcher()->removeDelegate(pTitleBar);
        }
    }
    
    //create cell
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (!getChildByTag(TAB_SCROLL_VIEW_CALL+i))
        {
            if (CShowCardInf *pNodeViewCall = new CShowCardInf())
            {
                addChild(pNodeViewCall,0,TAB_SCROLL_VIEW_CALL+i);
                pNodeViewCall->setUseScrollView(true);
            }
        }
    }
}

void CScrollViewLayer::loadBody(CShowCardInf *pNodeViewCall,unsigned int unIndex)
{
    if (CUserCardInfo *pUserCardInfo = getUserCardInfo(unIndex))
    {
        pNodeViewCall->loadBody(pUserCardInfo->cid);
    }
    else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(unIndex))
    {
        pNodeViewCall->loadBody(pCardBaseInfo->cid);
    }
}

void CScrollViewLayer::show(CUserCardInfo *pUserCardInfo)
{
    int nIndex = getIndex(pUserCardInfo);
    if (-1==nIndex)return;
    show(nIndex);
}

void CScrollViewLayer::show(CCardBaseInfo *pCardBaseInfo)
{
    int nIndex = getIndex(pCardBaseInfo);
    if (-1==nIndex)return;
    show(nIndex);
}

void CScrollViewLayer::show(unsigned int unIndex)
{
    
    
//    struct timeval tv1;
//    gettimeofday(&tv1,NULL);
//    CCLog("%ld ++++1+++++ %ld" , tv1.tv_sec , tv1.tv_usec);

    
    
    unsigned int unSumCount = count();
    if (unSumCount<=unIndex)return;
    m_unCurShowCardIndex = unIndex;
    m_bMoved = false;
    m_fMovedX = 0.0f;
    
    
//    struct timeval tv2;
//    gettimeofday(&tv2,NULL);
//    CCLog("%ld ++++2+++++ %ld" , tv2.tv_sec , tv2.tv_usec);
    
    //create basic child
    createBasicChild();
    
    
    
    
//    struct timeval tv3;
//    gettimeofday(&tv3,NULL);
//    CCLog("%ld ++++3+++++ %ld" , tv3.tv_sec , tv3.tv_usec);
    
    //设置cell
//    int nNextIndex = m_unCurShowCardIndex;
//    int nUpIndex = m_unCurShowCardIndex;
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT ; ++i)
    {
        if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+i))
        {
            pNodeViewCall->stopAllActions();
            pNodeViewCall->hide();
            if ((m_unCurShowCardIndex+i-1)<unSumCount)
            {
                if (SCROLL_VIEW_CELL_COUNT/2>i) //减
                {
//                    nUpIndex = getIndexDelBeIndex((unsigned int)nUpIndex);
//                    if (0>nUpIndex){}
//                    else if (CUserCardInfo *pUserCardInfo = getUserCardInfo((unsigned int)nUpIndex))
//                    {
//                        pNodeViewCall->show(pUserCardInfo);
//                    }
//                    else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo((unsigned int)nUpIndex))
//                    {
//                        pNodeViewCall->show(pCardBaseInfo);
//                    }
                }
                else if (SCROLL_VIEW_CELL_COUNT/2==i)   //中
                {
                    if (CUserCardInfo *pUserCardInfo = getUserCardInfo(m_unCurShowCardIndex))
                    {
                        pNodeViewCall->show(pUserCardInfo);
                        MainScene::Inst()->setTitle(pUserCardInfo->cid, pUserCardInfo->star,  pUserCardInfo->ugp_max_star, pUserCardInfo->name, pUserCardInfo->category, pUserCardInfo->sub_category);
                    }
                    else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(m_unCurShowCardIndex))
                    {
                        pNodeViewCall->show(pCardBaseInfo);
                        MainScene::Inst()->setTitle(pCardBaseInfo->cid, pCardBaseInfo->star,  pCardBaseInfo->ugp_max_star, pCardBaseInfo->name, pCardBaseInfo->category, pCardBaseInfo->sub_category);
                    }
                    
                }
                else if (SCROLL_VIEW_CELL_COUNT>i)      //增
                {
//                    nNextIndex = getIndexAddBeIndex((unsigned int)nNextIndex);
//                    if (0>nNextIndex){}
//                    else if (CUserCardInfo *pUserCardInfo = getUserCardInfo((unsigned int)nNextIndex))
//                    {
//                        pNodeViewCall->show(pUserCardInfo);
//                    }
//                    else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo((unsigned int)nNextIndex))
//                    {
//                        pNodeViewCall->show(pCardBaseInfo);
//                    }
                }
            }
            pNodeViewCall->setPosition(ccp(m_szSize.width*(i-1),pNodeViewCall->getPosition().y));
        }
    }
    
    
    
//    struct timeval tv4;
//    gettimeofday(&tv4,NULL);
//    CCLog("%ld ++++4+++++ %ld" , tv4.tv_sec , tv4.tv_usec);
    
    
    OcProxy::Inst()->showUsedMemory();
    
    
//    
//    struct timeval tv10;
//    gettimeofday(&tv10,NULL);
//    CCLog("%ld ++++10+++++ %ld" , tv10.tv_sec , tv10.tv_usec);
    
    //绘画标题
    if (CUserCardInfo *pUserCardInfoTitle = getCurUserCardInfo())
    {
        MainScene::Inst()->setTitle(pUserCardInfoTitle->cid, pUserCardInfoTitle->star,pUserCardInfoTitle->ugp_max_star, pUserCardInfoTitle->name, pUserCardInfoTitle->category, pUserCardInfoTitle->sub_category);
    }
    else if (CCardBaseInfo *pCardBaseInfoTitle = getCurCardBaseInfo())
    {
        MainScene::Inst()->setTitle(pCardBaseInfoTitle->cid, pCardBaseInfoTitle->star ,pCardBaseInfoTitle->ugp_max_star, pCardBaseInfoTitle->name, pCardBaseInfoTitle->category, pCardBaseInfoTitle->sub_category);
    }
    
    
    
//    struct timeval tv6;
//    gettimeofday(&tv6,NULL);
//    CCLog("%ld ++++6+++++ %ld" , tv6.tv_sec , tv6.tv_usec);
    
    
    //进入画面动画
    if (!this->getIsVisible())
    {
        setIsTouchEnabled(true);
        if (m_pLayer)
        {
            if (dynamic_cast<Package*>(m_pLayer))
            {
                CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
            }
            else
            {
                CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority-1, true);
            }
        }
        this->setIsVisible(true);
        
        CCLayerColor* lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 255), 640, 960);
        addChild(lcCover, 10);
        lcCover->runAction(CCFadeOut::actionWithDuration(0.3));
    }
    
    
//    struct timeval tv7;
//    gettimeofday(&tv7,NULL);
//    CCLog("%ld ++++7+++++ %ld" , tv7.tv_sec , tv7.tv_usec);
    
    
    //set source button
    setSourceButton(0);
    schedule(schedule_selector(CScrollViewLayer::addOherShow), 0.3f);
    schedule(schedule_selector(CScrollViewLayer::delOherShow), 0.3f);
    
    MainScene::Inst()->setGenRangeBtnIsVisble(false);
    MainScene::Inst()->showBottomPanel(false);
    m_bMoved = false;
    m_fMovedX = 0.0f;
    
    
    
//    struct timeval tv8;
//    gettimeofday(&tv8,NULL);
//    CCLog("%ld ++++8+++++ %ld" , tv8.tv_sec , tv8.tv_usec);
    
    
    
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi)
    {
        CCSprite * ttt = CCSprite::spriteWithFile("newbie-PowerUpSkill.png");
        addChild(ttt);
        ttt->setTag(989898);
        ttt->setPosition(ccp(320, 80));
//        ttt->setScale(1.3f);
        ttt->runAction(CCBlink::actionWithDuration(100, 500));
        
        
        
        CCSprite * ttt1 = CCSprite::spriteWithFile("newbie-PowerUpType.png");
        addChild(ttt1);
        ttt1->setTag(989899);
//        ttt->setScale(1.3f);
        ttt1->setPosition(ccp(70, 690));
        ttt1->runAction(CCBlink::actionWithDuration(100, 500));
        
        
    }
    
    
    if(MainScene::Inst()->new_CardAdd)
    {
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(CScrollViewLayer::newbie1),
                                            CGameData::Inst()->getLanguageValue("WuJiangJia_new24"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    }
    
    
//    struct timeval tv9;
//    gettimeofday(&tv9,NULL);
//    CCLog("%ld ++++9+++++ %ld" , tv9.tv_sec , tv9.tv_usec);
    
}

void CScrollViewLayer::addOherShow(ccTime dt)
{
    unschedule(schedule_selector(CScrollViewLayer::addOherShow));
    if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+2))
    {
        int nAddCardIndex = getIndexAddBeIndex(m_unCurShowCardIndex);
        if (0>nAddCardIndex){}
        else if (CUserCardInfo *pUserCardInfo = getUserCardInfo(nAddCardIndex))
        {
            pNodeViewCall->show(pUserCardInfo);
        }
        else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(nAddCardIndex))
        {
            pNodeViewCall->show(pCardBaseInfo);
        }
        pNodeViewCall->setPosition(ccp(m_szSize.width*(1),pNodeViewCall->getPosition().y));
    }
}

void CScrollViewLayer::delOherShow(ccTime dt)
{
    unschedule(schedule_selector(CScrollViewLayer::delOherShow));
    if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+0))
    {
        int nDelCardIndex = getIndexDelBeIndex(m_unCurShowCardIndex);
        if (0>nDelCardIndex){}
        else if (CUserCardInfo *pUserCardInfo = getUserCardInfo(nDelCardIndex))
        {
            pNodeViewCall->show(pUserCardInfo);
        }
        else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(nDelCardIndex))
        {
            pNodeViewCall->show(pCardBaseInfo);
        }
        pNodeViewCall->setPosition(ccp(m_szSize.width*(-1),pNodeViewCall->getPosition().y));
    }
}

void CScrollViewLayer::showMoveAdd(ccTime dt)
{
    unsigned int unIndex = MIN(getIndexAddBeIndex(m_unCurShowCardIndex),count()-1);
    unsigned int unSumCount = count()-1;
    
    if (unSumCount<unIndex)return;
    m_bShowMoved = false;
    m_unCurShowCardIndex = unIndex;
    CShowCardInf *aryShowCardInf[SCROLL_VIEW_CELL_COUNT];
    memset(aryShowCardInf,0,sizeof(aryShowCardInf));
    
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if ((SCROLL_VIEW_CELL_COUNT-1)==i)
        {
            if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL))
            {
                pNodeViewCall->stopAllActions();
                pNodeViewCall->hide();
                int nAddCardIndex = getIndexAddBeIndex(m_unCurShowCardIndex);
                if (0>nAddCardIndex){}
                else if (CUserCardInfo *pUserCardInfo = getUserCardInfo(nAddCardIndex))
                {
                    pNodeViewCall->show(pUserCardInfo);
                }
                else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(nAddCardIndex))
                {
                    pNodeViewCall->show(pCardBaseInfo);
                }
                pNodeViewCall->setPosition(ccp(m_szSize.width*(i-1),pNodeViewCall->getPosition().y));
                aryShowCardInf[0] = pNodeViewCall;
            }
        }
        else if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+i+1))
        {
            pNodeViewCall->setPosition(ccp(m_szSize.width*(i-1),pNodeViewCall->getPosition().y));
            aryShowCardInf[i+1] = pNodeViewCall;
            pNodeViewCall->stopAllActions();
        }
    }
    
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (aryShowCardInf[i])
        {
            if (0==i)
            {
                aryShowCardInf[i]->setTag(TAB_SCROLL_VIEW_CALL+SCROLL_VIEW_CELL_COUNT-1);
            }
            else
            {
                aryShowCardInf[i]->setTag(TAB_SCROLL_VIEW_CALL+i-1);
            }
        }
    }
    m_bShowMoved = true;
    
    //绘画标题
    if (CUserCardInfo *pUserCardInfoTitle = getCurUserCardInfo())
    {
        MainScene::Inst()->setTitle(pUserCardInfoTitle->cid, pUserCardInfoTitle->star,pUserCardInfoTitle->ugp_max_star, pUserCardInfoTitle->name, pUserCardInfoTitle->category, pUserCardInfoTitle->sub_category);
    }
    else if (CCardBaseInfo *pCardBaseInfoTitle = getCurCardBaseInfo())
    {
        MainScene::Inst()->setTitle(pCardBaseInfoTitle->cid, pCardBaseInfoTitle->star,pCardBaseInfoTitle->ugp_max_star, pCardBaseInfoTitle->name, pCardBaseInfoTitle->category, pCardBaseInfoTitle->sub_category);
    }
    
    setSourceButton(0);
    MainScene::Inst()->setGenRangeBtnIsVisble(false);
    MainScene::Inst()->showBottomPanel(false);
}

void CScrollViewLayer::showMoveDel(ccTime dt)
{
    unsigned int unIndex = MAX(getIndexDelBeIndex(m_unCurShowCardIndex),0);
    unsigned int unSumCount = count()-1;
    
    if (unSumCount<unIndex)return;
    m_bShowMoved = false;
    m_unCurShowCardIndex = unIndex;
    CShowCardInf *aryShowCardInf[SCROLL_VIEW_CELL_COUNT];
    memset(aryShowCardInf,0,sizeof(aryShowCardInf));
    
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (0==i)
        {
            if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+SCROLL_VIEW_CELL_COUNT-1))
            {
                pNodeViewCall->stopAllActions();
                pNodeViewCall->hide();
                int nDelCardIndex = getIndexDelBeIndex(m_unCurShowCardIndex);
                if (0>nDelCardIndex){}
                else if (CUserCardInfo *pUserCardInfo = getUserCardInfo(nDelCardIndex))
                {
                    pNodeViewCall->show(pUserCardInfo);
                }
                else if (CCardBaseInfo *pCardBaseInfo = getCardBaseInfo(nDelCardIndex))
                {
                    pNodeViewCall->show(pCardBaseInfo);
                }
                pNodeViewCall->setPosition(ccp(m_szSize.width*(i-1),pNodeViewCall->getPosition().y));
                aryShowCardInf[SCROLL_VIEW_CELL_COUNT-1] = pNodeViewCall;
            }
        }
        else if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+i-1))
        {
            pNodeViewCall->setPosition(ccp(m_szSize.width*(i-1),pNodeViewCall->getPosition().y));
            aryShowCardInf[i-1] = pNodeViewCall;
            pNodeViewCall->stopAllActions();
        }
    }
    
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (aryShowCardInf[i])
        {
            if ((SCROLL_VIEW_CELL_COUNT-1)==i)
            {
                aryShowCardInf[i]->setTag(TAB_SCROLL_VIEW_CALL);
            }
            else
            {
                aryShowCardInf[i]->setTag(TAB_SCROLL_VIEW_CALL+i+1);
            }
        }
    }
    m_bShowMoved = true;
    
    //绘画标题
    if (CUserCardInfo *pUserCardInfoTitle = getCurUserCardInfo())
    {
        MainScene::Inst()->setTitle(pUserCardInfoTitle->cid, pUserCardInfoTitle->star,pUserCardInfoTitle->ugp_max_star, pUserCardInfoTitle->name, pUserCardInfoTitle->category, pUserCardInfoTitle->sub_category);
    }
    else if (CCardBaseInfo *pCardBaseInfoTitle = getCurCardBaseInfo())
    {
        MainScene::Inst()->setTitle(pCardBaseInfoTitle->cid, pCardBaseInfoTitle->star,pCardBaseInfoTitle->ugp_max_star, pCardBaseInfoTitle->name, pCardBaseInfoTitle->category, pCardBaseInfoTitle->sub_category);
    }
    
    setSourceButton(0);
    MainScene::Inst()->setGenRangeBtnIsVisble(false);
    MainScene::Inst()->showBottomPanel(false);
}

void CScrollViewLayer::stopMove(ccTime dt)
{
    m_bMoved = false;
    m_fMovedX = 0.0f;
}

void CScrollViewLayer::hide()
{
    for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL +i))
        {
            pNodeViewCall->hide();
        }
    }
    
    MainScene::Inst()->setGenRangeBtnIsVisble(true);
    this->setIsVisible(false);
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    setIsTouchEnabled(false);
    if(CGameData::Inst()->isNewbie())MainScene::Inst()->enableBottomPanel(false);
    MainScene::Inst()->setTitle(0, 0, 0, 0 , 0, 0);
    
    printf("hide[CScrollViewLayer] cur child count [%d]\n",getChildrenCount());
}

int CScrollViewLayer::getIndex(CUserCardInfo *pUserCardInfo)
{
    if (pUserCardInfo && m_pLayer)
    {
        if (Package* pPackage =  dynamic_cast<Package*>(m_pLayer))
        {
            std::vector<PackCard*>::iterator itvr = pPackage->m_packCardArr->begin(), endvr = pPackage->m_packCardArr->end();
            for (int nIndex = 0; itvr != endvr; ++itvr, ++nIndex)
            {
                if ((*itvr)->getUserCardInfo()==pUserCardInfo)
                {
                    return (*itvr)->getTag()-Package::getCardTagBase();
                }
            }
        }
    }
    
    return -1;
}

PackCard * CScrollViewLayer::getPackCard(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (Package* pPackage =  dynamic_cast<Package*>(m_pLayer))
        {
            std::vector<PackCard*>::iterator itvr = pPackage->m_packCardArr->begin(), endvr = pPackage->m_packCardArr->end();
            for (int nIndex = 0; itvr != endvr; ++itvr, ++nIndex)
            {
                if (((*itvr)->getTag()-Package::getCardTagBase())==unIndex)
                {
                    return (*itvr);
                }
            }
        }
    }

    return NULL;
}

CUserCardInfo * CScrollViewLayer::getUserCardInfo(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (dynamic_cast<Package*>(m_pLayer))
        {
            if (PackCard *pPackCard = getPackCard(unIndex))
            {
                return pPackCard->getUserCardInfo();
            }
        }
    }
    return NULL;
}

int CScrollViewLayer::getIndex(CCardBaseInfo *pCardBaseInfo)
{
    if (m_pLayer)
    {
        if (OtherMain* pOtherMain =  dynamic_cast<OtherMain*>(m_pLayer))
        {
            if (pOtherMain->cardLayer &&pOtherMain->cardLayer->getChildByTag(pCardBaseInfo->cid+OtherCardLayer::getCardTagBase()-1))
            {
                return pCardBaseInfo->cid-1;
            }
        }
    }
    return -1;
}

CCardBaseInfo * CScrollViewLayer::getCardBaseInfo(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (OtherMain *pOtherMain = dynamic_cast<OtherMain*>(m_pLayer))
        {
            if (pOtherMain->cardLayer && pOtherMain->cardLayer->isShowClctCard(unIndex+1))
            {
                return CGameData::Inst()->getCardBaseInfByCid(unIndex+1);
            }
        }
    }
    return NULL;
}

CCardBaseInfo * CScrollViewLayer::getBeContentNextCardBaseInfo(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (OtherMain *pOtherMain = dynamic_cast<OtherMain*>(m_pLayer))
        {
            for (int i = unIndex; i < kMaxCardNum; ++i)
            {
                if (pOtherMain->cardLayer && pOtherMain->cardLayer->isShowClctCard(i+1))
                {
                    if (CCardBaseInfo *pCardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(i+1))
                    {
                        return pCardBaseInfo;
                    }
                }
            }
        
        }
    }
    return NULL;
}

CCardBaseInfo * CScrollViewLayer::getCurCardBaseInfo()
{
    return getCardBaseInfo(m_unCurShowCardIndex);
}

PackCard * CScrollViewLayer::getCurPackCard()
{
    return getPackCard(m_unCurShowCardIndex);
}

CUserCardInfo * CScrollViewLayer::getCurUserCardInfo()
{
    return getUserCardInfo(m_unCurShowCardIndex);
}

CShowCardInf * CScrollViewLayer::getCurShowCardInfo()
{
    return (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+SCROLL_VIEW_CELL_COUNT/2);
}

unsigned int CScrollViewLayer::count()const
{
    if (m_pLayer)
    {
        if (Package* pPackage =  dynamic_cast<Package*>(m_pLayer))
        {
            return pPackage->m_packCardArr->count();
        }
        else if (dynamic_cast<OtherMain*>(m_pLayer))
        {
            return  kMaxCardNum;
        }
    }
    return 0;
}

int CScrollViewLayer::getIndexAddBeIndex(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (dynamic_cast<Package*>(m_pLayer))
        {
            if ((unIndex+1)<count())
            {
                return unIndex+1;
            }
        }
        else if (OtherMain *pOtherMain = dynamic_cast<OtherMain*>(m_pLayer))
        {
            for (int i = unIndex+1; i < kMaxCardNum; ++i)
            {
                if (pOtherMain->cardLayer &&
                    pOtherMain->cardLayer->isShowClctCard(i+1) &&
                    CGameData::Inst()->getCardBaseInfByCid(i+1))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

int CScrollViewLayer::getIndexDelBeIndex(unsigned int unIndex)
{
    if (m_pLayer)
    {
        if (dynamic_cast<Package*>(m_pLayer))
        {
            if (0<unIndex)
            {
                return unIndex-1;
            }
        }
        else if (OtherMain *pOtherMain = dynamic_cast<OtherMain*>(m_pLayer))
        {
            for (int i = unIndex-1; i >= 0; --i)
            {
                if (pOtherMain->cardLayer &&
                    pOtherMain->cardLayer->isShowClctCard(i+1) &&
                    CGameData::Inst()->getCardBaseInfByCid(i+1))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

void CScrollViewLayer::setSourceButton(short shType)
{
    //是否具有来源
    if (CShowCardInf * pShowCardInf = getCurShowCardInfo())
    {
        std::string szSource;
        if (CUserCardInfo *pUserCardInfo = getCurUserCardInfo())
        {
            szSource = pUserCardInfo->find_me;
        }
        else if (CCardBaseInfo *pCardBaseInfo = getCurCardBaseInfo())
        {
            szSource = pCardBaseInfo->find_me;
        }
        
        if (0==shType)  //移动
        {
            TitleBar *pTitleBar = (TitleBar*)getChildByTag(kLaiYuan);
            if (!pTitleBar) pTitleBar = (TitleBar*)getChildByTag(kXiangQing);
            if (pTitleBar)
            {
                if (kXiangQing==pTitleBar->getTag() && !szSource.empty())
                {
                    pTitleBar->setIsVisible(true);
                    pTitleBar -> setTitleString(CGameData::Inst()->getLanguageValue("xiangqing_alone"));
                    pTitleBar -> SetTitlePosition(CCPointMake(12, 15));
                    pTitleBar -> setTag(kXiangQing);
                    if (CCNode *pNode = pShowCardInf->getChildByTag(kSource))
                    {
                        pNode->setScale(1.0f);
                    }
                }
                else if (!szSource.empty())
                {
                    pTitleBar->setIsVisible(true);
                    pTitleBar -> setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
                    pTitleBar -> SetTitlePosition(CCPointMake(12, 15));
                    pTitleBar -> setTag(kLaiYuan);
                }
                else
                {
                    pTitleBar->setIsVisible(false);
                }
            }
        }
        else if (1==shType) //点下
        {
            if (TitleBar *pTitleBar = (TitleBar*)getChildByTag(kLaiYuan))
            {
                if (!szSource.empty())
                {
                    pTitleBar->setIsVisible(true);
                    pTitleBar -> setTitleString(CGameData::Inst()->getLanguageValue("xiangqing_alone"));
                    pTitleBar -> SetTitlePosition(CCPointMake(12, 15));
                    pTitleBar -> setTag(kXiangQing);
                    if (CCNode *pNode = pShowCardInf->getChildByTag(kSource))
                    {
                        pNode->setScale(1.0f);
                    }
                }
                else
                {
                    pTitleBar->setIsVisible(false);
                }
            }
            else if (TitleBar *pTitleBar = (TitleBar*)getChildByTag(kXiangQing))
            {
                if (!szSource.empty())
                {
                    pTitleBar->setIsVisible(true);
                    pTitleBar -> setTitleString(CGameData::Inst()->getLanguageValue("laiyuan_alone"));
                    pTitleBar -> SetTitlePosition(CCPointMake(12, 15));
                    pTitleBar -> setTag(kLaiYuan);
                    if (CCNode *pNode = pShowCardInf->getChildByTag(kSource))
                    {
                        pNode->setScale(0.0f);
                    }
                }
                else
                {
                    pTitleBar->setIsVisible(false);
                }
            }
        }
    }
}

bool CScrollViewLayer::beMovedAdd()
{
    return m_bMoved && -1<getIndexAddBeIndex(m_unCurShowCardIndex);
}

bool CScrollViewLayer::beMovedDel()
{
    return m_bMoved && -1<getIndexDelBeIndex(m_unCurShowCardIndex);
}

CScrollViewLayer * CScrollViewLayer::create(CCLayer *pLayer)
{
    if (pLayer)
    {
        if (CScrollViewLayer *pScrollViewLayer = new CScrollViewLayer())
        {
            pScrollViewLayer->m_pLayer = pLayer;
            return pScrollViewLayer;
        }
    }
    return NULL;
}

void CScrollViewLayer::drawAwakeSkillTip(CCPoint pos, int awakeSkillId, int idx)
{
    char buf[100];
    snprintf(buf, 99, "awakeskill_%d.png", awakeSkillId);
    
    char bufName[100];
    snprintf(bufName, 99, "AwakeSkill_Name_%d", awakeSkillId);
    
    char bufDsp[100];
    snprintf(bufDsp, 99, "AwakeSkill_Dsp_%d", awakeSkillId);
    
    char bufIdx[100];
    snprintf(bufIdx, 99, CGameData::Inst()->getLanguageValue("AwakeSkill_index"), idx);
    
    TipsAwakeSkill* tipAwakeSkill = TipsAwakeSkill::tipWithContent(buf, CGameData::Inst()->getLanguageValue(bufName), CGameData::Inst()->getLanguageValue(bufDsp), bufIdx);
    addChild(tipAwakeSkill, 100, 20131029);
    tipAwakeSkill->setPosition(pos);
    b_showAwakeInfo = true;
}


void CScrollViewLayer::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);

    
    Arrow* pArrow = new Arrow(30, 255);
    
    pArrow->setPosition(CCPointMake(121, 770));
    
    
    
    MainScene::Inst()->addChild(pArrow, 50000,  96996);
    
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));

}


void CScrollViewLayer::newbie1(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    

//    MainScene::Inst()->showBottomPanel(true);
    
    MainScene::Inst()->new_CardAdd = false;
    
}




bool CScrollViewLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi && !MainScene::Inst()->getChildByTag(96996))
    {
        CCSprite * ttt = (CCSprite *)getChildByTag(989898);
        ttt->stopAllActions();
        ttt->setIsVisible(true);
        
        
        CCSprite * ttt1 = (CCSprite *)getChildByTag(989899);
        ttt1->stopAllActions();
        ttt1->setIsVisible(true);

        
        
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(CScrollViewLayer::newbie),
                                            CGameData::Inst()->getLanguageValue("qiangHua_new8"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 280));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));

        
        
        
        return true;
    }
    
    
    
    CCPoint touchLocation = pTouch->locationInView(pTouch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    
    if (!m_bMoved)
    {
        CCPoint screenPos = touchLocation;
        CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
        if(sz.width == 320)
            screenPos = ccpMult(screenPos, 2);
        if(sz.width < 1000)
            screenPos = ccpMult(screenPos, CC_CONTENT_SCALE_FACTOR());
        
        //ipad, iphone5
        screenPos.x -= MainScene::Inst()->getPosition().x;
        screenPos.y -= MainScene::Inst()->getPosition().y;
        
        if (dynamic_cast<Package*>(m_pLayer))
        {
            if (CUserCardInfo *pUserCardInfo = getCurUserCardInfo())
            {
                for (int i=0; i<MaxAwakeSkillCnt; i++)
                {
                    if (pUserCardInfo->AwakeSkillArr[i] <= 0)
                    {
                         break;
                    }
                    if (screenPos.x > 510 && screenPos.y < 736 && screenPos.y > 736 - ((i+1)*40))
                    {
                        drawAwakeSkillTip(ccp(320, 650-(i*40)), pUserCardInfo->AwakeSkillArr[i], i+1);
                        return true;
                    }
                }
            }
        }
        else if (dynamic_cast<OtherMain*>(m_pLayer))
        {
            if (CCardBaseInfo *pCardBaseInfo = getCurCardBaseInfo())
            {
                for (int i=0; i<MaxAwakeSkillCnt; i++)
                {
                    if (pCardBaseInfo->AwakeSkillArr[i] <= 0)
                    {
                        break;
                    }
                    if (screenPos.x > 510 && screenPos.y < 736 && screenPos.y > 736 - ((i+1)*40))
                    {
                        drawAwakeSkillTip(ccp(320, 650-(i*40)), pCardBaseInfo->AwakeSkillArr[i], i+1);
                        return true;
                    }
                }
            }
        }
    }
    
    m_bMoved = false;
    m_fMovedX = 0.0f;
    
    for(int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
    {
        if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+i))
        {
            //began [BG]
            if (CCNode *pNode = getChildByTag(TAB_SCROLL_VIEW_BG))
            {
                CCPoint pos = pNode->convertToNodeSpace(touchLocation);
                CCSize sz = pNode->getContentSizeInPixels();
                if (CCRect::CCRectContainsPoint(CCRectMake(0,0,sz.width,sz.height),pos))
                {
                    m_posMovedTouch = touchLocation;
                    m_bMoved = true;
                    break;
                }
            }
            
            //began [info box]
            if (CCNode *pNode = pNodeViewCall->getChildInfoBox())
            {
                CCPoint pos = pNode->convertToNodeSpace(touchLocation);
                CCSize sz = pNode->getContentSizeInPixels();
                if (CCRect::CCRectContainsPoint(CCRectMake(0,0,sz.width,sz.height),pos))
                {
                    m_posMovedTouch = touchLocation;
                    m_bMoved = true;
                    break;
                }
            }
            
            //began [body]
            if (CCNode *pNode = pNodeViewCall->getChildBody())
            {
                CCPoint pos = pNode->convertToNodeSpace(touchLocation);
                CCSize sz = pNode->getContentSizeInPixels();
                if (CCRect::CCRectContainsPoint(CCRectMake(0,0,sz.width,sz.height),pos))
                {
                    m_posMovedTouch = touchLocation;
                    m_bMoved = true;
                    break;
                }
            }
        }
    }
    
    //if (!m_bMoved)
    {
        //began [source button]
        TitleBar *pTitleBar = (TitleBar*)getChildByTag(kLaiYuan);
        if (!pTitleBar)pTitleBar = (TitleBar*)getChildByTag(kXiangQing);
        if (pTitleBar)
        {
            
            if(MainScene::Inst()->new_FirstPowerUpSeeYuXi)
                return false;
            
            CCSize szSourceButton = pTitleBar->getContentSizeInPixels();
            CCPoint posSourceButton = pTitleBar->convertToNodeSpace(touchLocation);
            if (CCRect::CCRectContainsPoint(CCRectMake(0,0,szSourceButton.width,szSourceButton.height),posSourceButton))
            {
                setSourceButton(1);
                m_bMoved = false;
                m_fMovedX = 0.0f;
            }
        }
        
        //began [return button]
        if (CCNode *pNode = getChildByTag(TAB_SCROLL_VIEW_RETURN_BUTTON))
        {
            CCSize szReturnButton = pNode->getContentSizeInPixels();
            CCPoint posReturnButton = pNode->convertToNodeSpace(touchLocation);
            if (CCRect::CCRectContainsPoint(CCRectMake(0,0,szReturnButton.width,szReturnButton.height),posReturnButton))
            {
                hide();
                m_bMoved = false;
                m_fMovedX = 0.0f;
            }
        }
    }
    return true;
}

void CScrollViewLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi && !MainScene::Inst()->getChildByTag(96996))
        return;
    
    if (m_bMoved && m_bShowMoved)
    {
        CCPoint touchLocation = pTouch->locationInView(pTouch->view());
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        m_fMovedX = m_posMovedTouch.x - touchLocation.x;
        m_posMovedTouch = touchLocation;


        for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
        {
            if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL +i))
            {
                if (pNodeViewCall->getActionByTag(nTagSpeedMovedTo))
                {
                    return;
                }
            }
        }
        //设置cell
        for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
        {
            if (CShowCardInf *pNodeViewCall = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL +i))
            {
                CCPoint posSource = pNodeViewCall->getPosition();
                pNodeViewCall->setPosition(ccp(posSource.x-m_fMovedX,posSource.y));
            }
        }
    }
}

void CScrollViewLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi && !MainScene::Inst()->getChildByTag(96996))
        return;
    
    if (b_showAwakeInfo)
    {
        if (getChildByTag(20131029))
            removeChildByTag(20131029, true);
        
        b_showAwakeInfo = false;
    }
    
    if (m_bMoved && m_bShowMoved)
    {
        float fSourceX = 0.0f;
        float fWidth = 0.0f;
        float fMovedAdd = 0.0f;
        float fMovedDel = 0.0f;
        static const float fMovedSpeedUp = 8.0f;     //加速常量值
        CShowCardInf *pCurShowCardInf = getCurShowCardInfo();
        assert(pCurShowCardInf);
        
        for (int i = 0; i < SCROLL_VIEW_CELL_COUNT; ++i)
        {
            if (CShowCardInf *pNode = (CShowCardInf*)getChildByTag(TAB_SCROLL_VIEW_CALL+i))
            {
                CCPoint posSource = pNode->getPosition();
                fSourceX = m_szSize.width*(i-1);
                fWidth = m_szSize.width/3.5f;
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
                                                             CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurShowCardInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.07f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::showMoveAdd)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                          NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
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
                                                             CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurShowCardInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.07f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::showMoveDel)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                          NULL);
                            pActionMovedTo->setTag(nTagSpeedMovedTo);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.35f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
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
                                                             CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurShowCardInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.07f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::showMoveAdd)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                                 NULL);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedAdd, posSource.y)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                                 NULL);
                        }
                    }
                }
                else if (fSourceX<posSource.x)  //后移
                {
                    if ((posSource.x-fSourceX)<fWidth || !beMovedDel())
                    {
                        pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fSourceX, posSource.y)),
                                                             CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                             NULL);
                    }
                    else
                    {
                        if (pCurShowCardInf==pNode)
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCDelayTime::actionWithDuration(0.07f),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::showMoveDel)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                                 NULL);
                        }
                        else
                        {
                            pActionMovedTo = CCSequence::actions(CCMoveTo::actionWithDuration(0.5f, CCPointMake(fMovedDel, posSource.y)),
                                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(CScrollViewLayer::stopMove)),
                                                                 NULL);
                        }
                    }
                }
                
                if (pActionMovedTo)
                {
                    pNode->stopAllActions();
                    pNode->runAction(pActionMovedTo);
                }
                else
                {
                    m_bMoved = false;
                }
            }
        }
    }
}

void CScrollViewLayer::callFuncMovedEndAdd()
{
    show(MIN(getIndexAddBeIndex(m_unCurShowCardIndex),count()-1));
}

void CScrollViewLayer::callFuncMovedEndDel()
{
    show(MAX(getIndexDelBeIndex(m_unCurShowCardIndex),0));
}



