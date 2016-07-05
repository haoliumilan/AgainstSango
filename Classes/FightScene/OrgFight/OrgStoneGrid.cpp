//
//  OrgStoneGrid.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgStoneGrid.h"
#include "SimpleAudioEngine.h"
#include "CSkillLib.h"
#include "OrgSaveData.h"
#include "OcProxy.h"

using namespace CocosDenshion;

#define kStoneBkTag 124
#define kGuangQuanTag           0x3346

static float   gfSpSize = 636.0/6;
static CCPoint gPtStart(320 - gfSpSize * 2.5, gfSpSize * 0.5);

//******************************************************************************
// Constructor
//******************************************************************************
OrgStoneGrid::OrgStoneGrid(CCObject* target, SEL_CallFunc selector):m_bStartFlg(false)
{
    m_spHold = NULL;
    m_iCombo = 0;
    m_fAdditionalDur = 0.0;
    m_fMaxMvDur = kStoneMvDur2 + m_fAdditionalDur;
    m_bInitMatrixFlg = false;
    m_iSndTag = 0;

    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    //back ground
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("stonebk.png");
    if(sp){
        sp->setScale(2.0);
        sp->setPositionInPixels(CCPointMake(320, 265));
        addChild(sp, -1, kStoneBkTag);
        sp->setIsVisible(false);
    }
    
    //hand
    m_spHand1 = CCSprite::spriteWithSpriteFrameName("hand1.png");
    addChild(m_spHand1, 10);
    m_spHand2 = CCSprite::spriteWithSpriteFrameName("hand2.png");
    addChild(m_spHand2, 10);
    m_spHandProgrs = CCSprite::spriteWithSpriteFrameName("handprogrs.png");
    m_spHandProgrs->setAnchorPoint(CCPointMake(0, 0));
    addChild(m_spHandProgrs, 10);
    setHand(CCPointMake(-1000, -1000), 1.0);
}

//******************************************************************************
// ~OrgStoneGrid
//******************************************************************************
OrgStoneGrid::~OrgStoneGrid()
{
    
}


//******************************************************************************
// enableTouch
//******************************************************************************
void OrgStoneGrid::enableTouch(bool bTouch)
{
    if(bTouch){
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    }
    else{
        setIsTouchEnabled(false);
    }
}

//******************************************************************************
// initMatrix
//******************************************************************************
void OrgStoneGrid::initMatrix()
{
    CGameState::Instance()->setState(enStateChgStone);
    
    CCPoint pt, ptSrc;
    

    if (OrgSaveData::Inst()->getIsUseSaveData())
    {
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                m_stoneNodeArray[j][i].iElement = OrgSaveData::Inst()->getStone(j, i);
            }
        }
    }
    else
    {
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                m_stoneNodeArray[j][i].iElement = randElement();
            }
        }
        
        while(1){
            if(chkErasable() == false)
                break;
            
            for(int i = 0; i < kMaxGridColumnCnt; i ++){
                for(int j = 0; j < kMaxGridRowCnt; j ++){
                    if(m_stoneNodeArray[j][i].bErasable == true){
                        m_stoneNodeArray[j][i].iElement = randElement();
                    }
                }
            }
        }
    }
    
    //create stones
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            // sprite
            m_stoneNodeArray[j][i].spStone = getSpriteByElement(m_stoneNodeArray[j][i].iElement);
            ptSrc.x = gPtStart.x + i * gfSpSize;
            ptSrc.y = gPtStart.y + (j + kMaxGridRowCnt) * gfSpSize;
            pt.x = gPtStart.x + i * gfSpSize;
            pt.y = gPtStart.y + j * gfSpSize;
            
            if(m_stoneNodeArray[j][i].spStone){
                m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.3, pt));
            }
            
            m_stoneNodeArray[j][i].bAlive = true;
        }
    }
    
    m_bInitMatrixFlg = true;
    
    CCNode* node = getChildByTag(kStoneBkTag);
    if(node)
        node->setIsVisible(true);
}

//******************************************************************************
// resetLink
//******************************************************************************
void OrgStoneGrid::resetLink()
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            //reset value
            m_stoneNodeArray[j][i].bSearched = false;
            m_stoneNodeArray[j][i].bAlive = true;
            m_stoneNodeArray[j][i].bErasable = false;
            
            //left column
            if(i == 0){
                m_stoneNodeArray[j][i].left = NULL;
            }
            else{
                m_stoneNodeArray[j][i].left = &m_stoneNodeArray[j][i-1];
            }
            
            //right column
            if(i == kMaxGridColumnCnt - 1){
                m_stoneNodeArray[j][i].right = NULL;
            }
            else{
                m_stoneNodeArray[j][i].right = &m_stoneNodeArray[j][i+1];
            }
            
            //bottom row
            if(j == 0){
                m_stoneNodeArray[j][i].bottom = NULL;
            }
            else{
                m_stoneNodeArray[j][i].bottom = &m_stoneNodeArray[j-1][i];
            }
            
            //top row
            if(j == kMaxGridRowCnt - 1){
                m_stoneNodeArray[j][i].top = NULL;
            }
            else{
                m_stoneNodeArray[j][i].top = &m_stoneNodeArray[j+1][i];
            }
        }
    }
}

//******************************************************************************
// startNewRound
//******************************************************************************
void OrgStoneGrid::startNewRound()
{
    m_iCombo = 0;
    m_bStartFlg = false;
    m_fMaxMvDur = kStoneMvDur2 + m_fAdditionalDur;
    m_ptCurTouch.x = -1000;
    m_ptCurTouch.y = -1000;

    if(!m_bInitMatrixFlg)
        return;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(m_stoneNodeArray[j][i].spStone){
                m_stoneNodeArray[j][i].spStone->setOpacity(255);
                OrgSaveData::Inst()->setStone(j, i, m_stoneNodeArray[j][i].iElement);
            }
        }
    }
    
    //remove guangquan sprite
    while (getChildByTag(kGuangQuanTag)) {
        removeChildByTag(kGuangQuanTag, true);
    }
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool OrgStoneGrid::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if((CGameState::Instance()->getState() != enStateWaitTouch)
       &&(CGameState::Instance()->getState() != enStateTouching))
        return false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    int iColumn, iRow;
    iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
    iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
    
    if((iColumn >= kMaxGridColumnCnt) || (iRow >= kMaxGridRowCnt))
        return false;
    
    if(m_spHold != NULL){
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    int iElem = m_stoneNodeArray[iRow][iColumn].iElement;
    m_spHold = getSpriteByElement(iElem);
    CCPoint pt = m_stoneNodeArray[iRow][iColumn].spStone->getPositionInPixels();
    pt.y += m_spHold->getContentSize().height /4;
    m_spHold->setPositionInPixels(pt);
    
    m_stoneNodeArray[iRow][iColumn].spStone->setOpacity(100);
    
    m_iStartRow = iRow;
    m_iStartColumn = iColumn;
    m_iCurRow = iRow;
    m_iCurColumn = iColumn;
    
    return true;
}

void OrgStoneGrid::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if((CGameState::Instance()->getState() != enStateWaitTouch)
       &&(CGameState::Instance()->getState() != enStateTouching))
        return;
    
    if(m_spHold == NULL)
        return;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    m_ptCurTouch = nodePos;
    
    int iColumn, iRow;
    float float_iColumn, float_iRow;
    float_iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
    float_iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
    iColumn = float_iColumn;
    iRow    = float_iRow;
    
    if(iColumn < 0)
        iColumn = 0;
    
    if(iColumn >= kMaxGridColumnCnt)
        iColumn = kMaxGridColumnCnt -1;
    
    if(iRow < 0)
        iRow = 0;
    
    if(iRow >= kMaxGridRowCnt)
        iRow = kMaxGridRowCnt -1;
    
    CCPoint pt = nodePos;
    pt.y += m_spHold->getContentSize().height /4;
    
    m_spHold->setPositionInPixels(pt);
    
    if((iColumn != m_iCurColumn) || (iRow != m_iCurRow )){
        // 如果是斜的滑动
        if (iColumn != m_iCurColumn)
        {
            if (float_iRow - m_iCurRow > 0.9 )
            {
                iRow = m_iCurRow + 1;
            }
            else if (float_iRow - m_iCurRow < 0.1)
            {
                iRow = m_iCurRow - 1;
            }
        }
        else if (iRow != m_iCurRow)
        {
            if (float_iColumn - m_iCurColumn > 0.9)
            {
                iColumn = m_iCurColumn + 1;
            }
            else if (float_iColumn - m_iCurColumn < 0.1)
            {
                iColumn = m_iCurColumn - 1;
            }
        }
        if(iColumn < 0)
            iColumn = 0;
        
        if(iColumn >= kMaxGridColumnCnt)
            iColumn = kMaxGridColumnCnt -1;
        
        if(iRow < 0)
            iRow = 0;
        
        if(iRow >= kMaxGridRowCnt)
            iRow = kMaxGridRowCnt -1;
        
        if(m_bStartFlg == false ){
            m_bStartFlg = true;
            
            m_fElapse = 0.0;
            scheduleUpdate();
        }
        
        //start count down
        if(CGameState::Instance()->getState() == enStateWaitTouch){
            CGameState::Instance()->setState(enStateTouching);
        }
        
        //交换
        pt.x = gPtStart.x + iColumn * gfSpSize;
        pt.y = gPtStart.y + iRow * gfSpSize;
        m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->runAction(CCMoveTo::actionWithDuration(0.05, pt));
        
        pt.x = gPtStart.x + m_iCurColumn * gfSpSize;
        pt.y = gPtStart.y + m_iCurRow * gfSpSize;
        m_stoneNodeArray[iRow][iColumn].spStone->runAction(CCMoveTo::actionWithDuration(0.05, pt));
        
        StoneNode tempNode = m_stoneNodeArray[m_iCurRow][m_iCurColumn];
        m_stoneNodeArray[m_iCurRow][m_iCurColumn] = m_stoneNodeArray[iRow][iColumn];
        m_stoneNodeArray[iRow][iColumn] = tempNode;
        
        m_iCurRow = iRow;
        m_iCurColumn = iColumn;
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonemv));
    }
}


void OrgStoneGrid::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    if((CGameState::Instance()->getState() != enStateWaitTouch)
       &&(CGameState::Instance()->getState() != enStateTouching))
        return;
    
    if(m_spHold != NULL){
        CCPoint nodePos = m_spHold->getPositionInPixels();
        int iColumn, iRow;
        iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
        iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
        
        m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setOpacity(255);
        
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    m_ptCurTouch.x = -1000;
    m_ptCurTouch.y = -1000;
    
    touchEndProc();
}

//******************************************************************************
// update
//******************************************************************************
void OrgStoneGrid::update(ccTime dt)
{
    m_fElapse += dt;
    
    if(m_fElapse > m_fMaxMvDur){
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(timeout));
        
        touchEndProc();
    }
    else if(m_fElapse > (m_fMaxMvDur - kStoneMvDur1)){
        float fPercent = (m_fMaxMvDur - m_fElapse) / kStoneMvDur1;

        setHand(m_ptCurTouch, fPercent);
    }
    else {
        setHand(CCPointMake(-1000, -1000), 1.0);
    }
}

//******************************************************************************
// getSpriteByElement
//******************************************************************************
CCSprite* OrgStoneGrid::getSpriteByElement(int iElement)
{
    char bufFilename[20];
    
    if((iElement < 0) || (iElement >= enElementMax-2))
        return NULL;
    
    if (OcProxy::Inst()->is568HighScreen()) // if iphone5
    {
        sprintf(bufFilename, "stone%d_iphone5.png", iElement);
        CCSprite* sp = CCSprite::spriteWithFile(bufFilename);
        
        if (sp) {
            addChild(sp);
            sp->setTag(0);
        }
        
        return sp;
    }
    else
    {
        sprintf(bufFilename, "stone%d.png", iElement);
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName(bufFilename);

        if (sp) {
            addChild(sp);
            sp->setTag(0);
        }
        
        return sp;
        
    }
}


//******************************************************************************
// srchGrp
//******************************************************************************
bool OrgStoneGrid::srchGrp()
{
    if(chkErasable() == false)
        return false;
    
    m_iGrpNo = 0;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(m_stoneNodeArray[j][i].bErasable == false)
                continue;
            
            if(m_stoneNodeArray[j][i].bSearched == true)
                continue;
            
            m_grpList[m_iGrpNo].clear();
            sch1Grp(&m_stoneNodeArray[j][i], m_stoneNodeArray[j][i].iElement);
            m_iGrpNo++;
        }
    }
    
    return true;
}

//******************************************************************************
// sch1Grp
//******************************************************************************
void OrgStoneGrid::sch1Grp(PStoneNode node, int iElement)
{
    node->bSearched = true;
    m_grpList[m_iGrpNo].push_back(node);
    
    if((node->left != NULL)
       &&(node->left->bErasable == true)
       &&(node->left->bSearched == false)
       &&(node->left->iElement == iElement))
        sch1Grp(node->left, iElement);
    
    if((node->top != NULL)
       &&(node->top->bErasable == true)
       &&(node->top->bSearched == false)
       &&(node->top->iElement == iElement))
        sch1Grp(node->top, iElement);
    
    if((node->right != NULL)
       &&(node->right->bErasable == true)
       &&(node->right->bSearched == false)
       &&(node->right->iElement == iElement))
        sch1Grp(node->right, iElement);
    
    if((node->bottom != NULL)
       &&(node->bottom->bErasable == true)
       &&(node->bottom->bSearched == false)
       &&(node->bottom->iElement == iElement))
        sch1Grp(node->bottom, iElement);
}

//******************************************************************************
// chkErasable
//******************************************************************************
bool OrgStoneGrid::chkErasable()
{
    int iRow, iColum;
    int iStart, iEnd;
    
    bool bRet = false;
    
    resetLink();
    
    //search column
    for(iColum = 0; iColum < kMaxGridColumnCnt; iColum ++){
        iRow = 0;
        iStart = 0;
        iEnd = 0;
        while(iRow < kMaxGridRowCnt-1){
            
            if(m_stoneNodeArray[iRow][iColum].iElement.get() == m_stoneNodeArray[iRow+1][iColum].iElement.get()){
                iEnd = iRow + 1;
            }
            
            if((m_stoneNodeArray[iRow][iColum].iElement.get() != m_stoneNodeArray[iRow+1][iColum].iElement.get())
               ||(iRow == kMaxGridRowCnt -2)){
                
                if(iEnd - iStart >= 2){
                    for(int i = iStart; i <= iEnd; i++)
                        m_stoneNodeArray[i][iColum].bErasable = true;
                    
                    iEnd++;
                    iStart = iEnd;
                    
                    bRet = true;
                }
                else
                    iStart = iRow + 1;
            }
            
            iRow++;
        }
    }
    
    //search row
    for(iRow = 0; iRow < kMaxGridRowCnt; iRow ++){
        iColum = 0;
        iStart = 0;
        iEnd = 0;
        while(iColum < kMaxGridColumnCnt -1){
            
            if(m_stoneNodeArray[iRow][iColum].iElement.get() == m_stoneNodeArray[iRow][iColum+1].iElement.get()){
                iEnd = iColum + 1;
            }
            
            if((m_stoneNodeArray[iRow][iColum].iElement.get() != m_stoneNodeArray[iRow][iColum+1].iElement.get())
               ||(iColum == kMaxGridColumnCnt -2)){
                
                if(iEnd - iStart >= 2){
                    for(int i = iStart; i <= iEnd; i++)
                        m_stoneNodeArray[iRow][i].bErasable = true;
                    
                    iEnd++;
                    iStart = iEnd;
                    
                    bRet = true;
                }
                else
                    iStart = iColum + 1;
            }
            
            iColum++;
        }
    }
    
    return bRet;
}

//******************************************************************************
// erase
//******************************************************************************
void OrgStoneGrid::erase(ccTime dt)
{
    unschedule(schedule_selector(OrgStoneGrid::erase));
    
    if(m_iGrpNo <= 0){
        drop();
        return;
    }
    
    m_iGrpNo--;
    
    m_curEraseCnt = m_grpList[m_iGrpNo].size();
    
    int iRand = rand() % (m_grpList[m_iGrpNo].size() -1);
    
    for (StoneNodeIter it = m_grpList[m_iGrpNo].begin(); it != m_grpList[m_iGrpNo].end(); ++it)
    {
        PStoneNode node = *it;
        
        if(node->spStone)
        {
            node->spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.1),
                                                         CCScaleTo::actionWithDuration(0.2, 0.5),
                                                         CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.1, 1.0),
                                                                               CCFadeOut::actionWithDuration(0.2)),
                                                         NULL));
            
            if (node->iElement >= enElementHeart && node->iElement <= enElementDark)
            {
                char buf[50];
                snprintf(buf, 49, "guangquan_%d.png", node->iElement.get());
                CCSprite* spGuangQuan = CCSprite::spriteWithSpriteFrameName(buf);
                spGuangQuan->setPosition(node->spStone->getPosition());
                addChild(spGuangQuan, -1, kGuangQuanTag);
                
                spGuangQuan->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.1),
                                                           CCScaleTo::actionWithDuration(0.2, 0.9),
                                                           CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.2, 1.4),
                                                                                 CCFadeOut::actionWithDuration(0.2)),
                                                           NULL));
            }
            
            showEraseAnim(node->iElement, node->spStone->getPosition());

        }
        
        node->bAlive = false;
        
        m_curEraseElement = node->iElement;
        
        if((iRand == 0) && (node->spStone))
            m_curErasePos = node->spStone->getPositionInPixels();
        iRand--;
    }
    
    char buf[50];
    m_iCombo++;
    
    int iComboSnd = m_iCombo;
    
    if(CGameData::Inst()->isSoundOn){
        if(m_iSndTag == enAutoSkill_JiangNan){
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(combojn));
        }
        else{
            if(iComboSnd > 14)
                iComboSnd = 14;
            
            sprintf(buf, "combo%d%s", iComboSnd, kSeSuffix);
            
            SimpleAudioEngine::sharedEngine()->playEffect(buf);
        }
    }
    
    m_grpList[m_iGrpNo].clear();
    
    schedule(schedule_selector(OrgStoneGrid::erase), 0.5);
    
    
    (m_pListener->*m_pfnSelector)();
}


//******************************************************************************
// drop
//******************************************************************************
void OrgStoneGrid::drop()
{
    PStoneNode node;
    CCPoint ptDest;
    CCPoint ptSrc;
    
    int k = 0;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        k = 0;
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(m_stoneNodeArray[j][i].bAlive == false){
                if(m_stoneNodeArray[j][i].spStone){
                    removeChild(m_stoneNodeArray[j][i].spStone, true);
                    m_stoneNodeArray[j][i].spStone = NULL;
                }
                
                ptDest.x = gPtStart.x + i * gfSpSize;
                ptDest.y = gPtStart.y + j * gfSpSize;
                
                int m = 1;
                if((j + m) == kMaxGridRowCnt)
                    node = NULL;
                else
                    node = &m_stoneNodeArray[j +m][i];
                
                
                while (node != NULL) {
                    if((node != NULL) && (node->bAlive == true)){
                        m_stoneNodeArray[j][i].spStone = node->spStone;
                        node->spStone = NULL;
                        m_stoneNodeArray[j][i].iElement = node->iElement;
                        m_stoneNodeArray[j][i].bAlive = true;
                        
                        node->bAlive = false;
                        
                        if(m_stoneNodeArray[j][i].spStone)
                            m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.15, ptDest));
                        
                        break;
                    }
                    
                    m++;
                    if((j + m) == kMaxGridRowCnt)
                        node = NULL;
                    else
                        node = &m_stoneNodeArray[j +m][i];
                }
                
                if(node == NULL){
                    m_stoneNodeArray[j][i].iElement = randElement();

                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(m_stoneNodeArray[j][i].iElement);
                    m_stoneNodeArray[j][i].bAlive = true;
                    
                    ptSrc.x = ptDest.x;
                    ptSrc.y = gPtStart.y + (kMaxGridRowCnt + k) * gfSpSize;
                    if(m_stoneNodeArray[j][i].spStone){
                        m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                        m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.15, ptDest));
                    }
                    k++;
                }
            }
        }
    }
    
    schedule(schedule_selector(OrgStoneGrid::dropCb), 0.25);
}


//******************************************************************************
// dropCb
//******************************************************************************
void OrgStoneGrid::dropCb(ccTime dt)
{
    unschedule(schedule_selector(OrgStoneGrid::dropCb));
    
    if(srchGrp())
        erase(0);
    else {
        CGameState::Instance()->setState(enStateClrCombo);
        
        for(int i = 0; i < kMaxGridColumnCnt; i ++)
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone)
                    m_stoneNodeArray[j][i].spStone->setOpacity(100);
            }
        
        
        m_curLb = NULL;
        m_curLb2 = NULL;
        clrCombo();
    }
}


//******************************************************************************
// clrCombo
//******************************************************************************
void OrgStoneGrid::clrCombo()
{
    CGameState::Instance()->setState(enStateClrCombo);
    (m_pListener->*m_pfnSelector)();
    
    CGameState::Instance()->setState(enStatePlayerAttackPrepared);
    (m_pListener->*m_pfnSelector)();
}

//******************************************************************************
// getCurEraseCnt
//******************************************************************************
int OrgStoneGrid::getCurEraseCnt()
{
    return m_curEraseCnt;
}

//******************************************************************************
// getCurErasePos
//******************************************************************************
CCPoint& OrgStoneGrid::getCurErasePos()
{
    return m_curErasePos;
}

//******************************************************************************
// getCurEraseElement
//******************************************************************************
int OrgStoneGrid::getCurEraseElement()
{
    return m_curEraseElement;
}

//******************************************************************************
// setHand
//******************************************************************************
void OrgStoneGrid::setHand(const CCPoint& pos, float fPercent)
{
    if(fPercent > 1.0)
        fPercent = 1.0;
    
    if(fPercent < 0.0)
        fPercent = 0.0;
    
    CCPoint pt = pos;
    pt.y += 120;
    pt.x -= 10;
    
    int i = fPercent * 10;
    if(i % 2 == 0){
        m_spHand1->setIsVisible(true);
        m_spHand2->setIsVisible(false);
        m_spHand1->setPositionInPixels(pt);
    }
    else{
        m_spHand2->setIsVisible(true);
        m_spHand1->setIsVisible(false);
        m_spHand2->setPositionInPixels(pt);
    }
    
    m_spHandProgrs->setPositionInPixels(CCPointMake(pt.x - 18, pt.y - 41));
    
    m_spHandProgrs->setScaleX(fPercent);
}


//******************************************************************************
// touchEndProc
//******************************************************************************
void OrgStoneGrid::touchEndProc()
{
    if(m_spHold != NULL){
        
        CCPoint nodePos = m_spHold->getPosition();
        int iColumn, iRow;
        iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
        iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
        
        m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setOpacity(255);
        
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    setHand(CCPointMake(-1000, -1000), 1.0);
    unscheduleUpdate();
    
    if(m_bStartFlg){
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone)
                    m_stoneNodeArray[j][i].spStone->setOpacity(255);
            }
        }
        
        CGameState::Instance()->setState(enStateTouchEnd);
        
        if(srchGrp())
            schedule(schedule_selector(OrgStoneGrid::erase), 0.05);//erase(0);
        else{
            for(int i = 0; i < kMaxGridColumnCnt; i ++){
                for(int j = 0; j < kMaxGridRowCnt; j ++){
                    if(m_stoneNodeArray[j][i].spStone)
                        m_stoneNodeArray[j][i].spStone->setOpacity(100);
                }
            }
            
            CGameState::Instance()->setState(enStateEnemyAttack);
            (m_pListener->*m_pfnSelector)();
        }
    }
}


//******************************************************************************
// randElement
//******************************************************************************
int OrgStoneGrid::randElement()
{
    int iRand = OrgSaveData::Inst()->getrand() % (enElementMax-2);

    return iRand;
}

//******************************************************************************
// showEraseAnim
//******************************************************************************
void OrgStoneGrid::showEraseAnim(int iElement, const CCPoint& pos)
{
    if (iElement < enElementHeart || iElement > enElementDark)
        return;
    
    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(spAnim);
    spAnim->setPosition(pos);
    spAnim->setTag(kGuangQuanTag);
    
    char buf[100];
    snprintf(buf, 99, "guangquan_%d", iElement);
    
    CCActionInterval* action;
    switch (iElement) {
        case enElementHeart:
            action = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1),
                                              getActionFromFile(buf));
            break;
            
        case enElementWater:
            action = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.2),
                                              getActionFromFile(buf));
            break;
            
        case enElementFire:
            action = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1),
                                              getActionFromFile(buf));
            break;
            
        case enElementWood:
            action = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1),
                                              getActionFromFile(buf));
            break;
            
        case enElementLight:
            action = CCSequence::actionOneTwo(CCDelayTime::actionWithDuration(0.1),
                                              getActionFromFile(buf));
            break;
            
        case enElementDark:
            action = getActionFromFile(buf);
            break;
            
        default:
            break;
    }
    
    spAnim->runAction(action);
    
}

//******************************************************************************
// getActionFromFile
//******************************************************************************
CCActionInterval* OrgStoneGrid::getActionFromFile(const char* filename)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    CCSpriteFrame* frame;
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, "%s_%d.png", filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithSpriteFrameName(buf);
        if (tmpSP){
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
            animation->addFrame(frame);
        }
        else
            break;
        i++;
    }
    
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/15.0, animation, false);
    
    return action;
}

//******************************************************************************
// setSoundTag
//******************************************************************************
void OrgStoneGrid::setSoundTag(int tag)
{
    m_iSndTag = tag;
}





