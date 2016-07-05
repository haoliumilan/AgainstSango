//
//  StoneGrid.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "StoneGrid.h"
#include "SimpleAudioEngine.h"
#include "Arrow.h"
#include "MsgBox.h"
#include "MainScene.h"
#include "CSkillLib.h"
#include "SaveData.h"
#include "OcProxy.h"
#include "TextNode.h"
#include "RecordHelper.h"
#include "FightMngr.h"

using namespace CocosDenshion;


static float   gfSpSize = 636.0/6;
static CCPoint gPtStart(320 - gfSpSize * 2.5, gfSpSize * 0.5);


#define kTimerAnimTag           0x88
#define kStnPlusTag             0x111
#define kPlusLbTagBase          0x2345
#define kGuangQuanTag           0x3346


#define StoneGridMask_Element   0x0F
#define StoneGridMask_Movable   0x10
#define StoneGridMask_Start     0x20
#define StoneGridMask_End       0x40


//     0111 1111
//      |||    |------- element
//      |||------------ Movable
//      ||------------- StartStone
//      |-------------- EndStone

typedef struct{
    int     iPicType;   //1: line0001.png, 2: line0002.png, 3: line0003.png
    int     iRotation;
    int     iElem;
}stGuideMatrix;

static stGuideMatrix gGuideMatrix[5][kMaxGridRowCnt][kMaxGridColumnCnt] = {
    {
        {{0, 0, enElementHeart}, {0, 0, enElementHeart}, {3, 180, enElementFire}, {2, 0, enElementFire}, {3, 90, enElementWood}, {0, 0, enElementFire}},
        {{0, 0, enElementDark}, {0, 0, enElementDark}, {1, 270, enElementHeart}, {0, 0, enElementWood}, {2, 90, enElementDark}, {0, 0, enElementWood}},
        {{0, 0, enElementWater}, {0, 0, enElementFire}, {0, 0, enElementWood}, {0, 0, enElementLight}, {3, 270, enElementWater}, {3, 90, enElementWater}},
        {{0, 0, enElementWater}, {0, 0, enElementFire}, {0, 0, enElementWood}, {0, 0, enElementWood}, {0, 0, enElementDark}, {2, 90, enElementWater}},
        {{2, 0, enElementDark}, {2, 0, enElementWater}, {2, 0, enElementFire}, {2, 0, enElementWood}, {2, 0, enElementLight}, {3, 0, enElementDark}}
    },
    {
        {{0, 0, enElementDark}, {0, 0, enElementDark}, {0, 0, enElementHeart}, {0, 0, enElementHeart}, {2, 90, enElementFire}, {0, 0, enElementHeart}},
        {{0, 0, enElementLight}, {0, 0, enElementLight}, {0, 0, enElementDark}, {3, 180, enElementWood}, {3, 0, enElementHeart}, {0, 0, enElementWater}},
        {{0, 0, enElementWood}, {0, 0, enElementWood}, {0, 0, enElementLight}, {2, 90, enElementFire}, {0, 0, enElementWood}, {0, 0, enElementWater}},
        {{0, 0, enElementFire}, {0, 0, enElementFire}, {0, 0, enElementWood}, {3, 270, enElementWood}, {3, 90, enElementFire}, {0, 0, enElementFire}},
        {{0, 0, enElementWater}, {0, 0, enElementWater}, {0, 0, enElementFire}, {0, 0, enElementWood}, {1, 270, enElementFire}, {0, 0, enElementWater}}
    },
    {
        {{0, 0, enElementDark}, {3, 180, enElementFire}, {3, 90, enElementDark}, {0, 0, enElementLight}, {0, 0, enElementWood}, {0, 0, enElementDark}},
        {{0, 0, enElementDark}, {2, 90, enElementWood}, {2, 90, enElementDark}, {0, 0, enElementFire}, {0, 0, enElementWood}, {0, 0, enElementLight}},
        {{3, 180, enElementWood}, {2<<16|2, 0<<16|90, enElementDark}, {2<<16|1, 0<<16|270, enElementHeart}, {3, 90, enElementHeart}, {3, 180, enElementWater}, {2, 0, enElementLight}},
        {{3, 270, enElementWater}, {3, 0, enElementWater}, {0, 0, enElementLight}, {2, 90, enElementHeart}, {2, 90, enElementWood}, {0, 0, enElementWater}},
        {{0, 0, enElementWater}, {0, 0, enElementDark}, {0, 0, enElementLight}, {3, 270, enElementWood}, {3, 0, enElementWood}, {0, 0, enElementWater}}
    },
    {
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}}
    },
    {
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}},
        {{1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}, {1, 0, enElementFire}}
    },
};


//正常
static int gNewbieGrid1[kMaxGridRowCnt][kMaxGridColumnCnt] = {
    {enElementFire, enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater},
    {enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater, enElementFire},
    {enElementWood, enElementHeart, enElementFire, enElementDark, enElementLight, enElementHeart},
    {enElementHeart, enElementLight, enElementWater, enElementWood, enElementWater, enElementWood},
    {enElementFire, enElementWater, enElementFire, enElementFire, enElementHeart | 0x50,  enElementFire| 0x30}
};

//相克
static int gNewbieGrid2[kMaxGridRowCnt][kMaxGridColumnCnt] = {
    {enElementFire, enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater},
    {enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater, enElementFire},
    {enElementWood, enElementHeart, enElementFire, enElementDark, enElementLight, enElementHeart},
    {enElementHeart, enElementLight, enElementWater, enElementWood, enElementWater, enElementWood},
    {enElementFire, enElementHeart, enElementWater, enElementWater, enElementHeart | 0x50,  enElementWater| 0x30}
};


//AOE
static int gNewbieGrid3[kMaxGridRowCnt][kMaxGridColumnCnt] = {
    {enElementWater, enElementFire, enElementWood, enElementLight, enElementDark, enElementHeart},
    {enElementWater, enElementFire, enElementWood, enElementLight, enElementDark, enElementHeart},
    {enElementWood, enElementLight | 0x30, enElementHeart | 0x10, enElementDark | 0x50, enElementFire, enElementWater},
    {enElementHeart, enElementFire, enElementWater, enElementLight, enElementWater, enElementWood},
    {enElementHeart, enElementFire, enElementWater, enElementLight, enElementWater, enElementWood}
};



static int gNewbieGrid4[kMaxGridRowCnt][kMaxGridColumnCnt] =
{
    {enElementFire,enElementWater,enElementWood,enElementHeart,enElementFire,
        enElementWater},
    {enElementWater,enElementWood,enElementHeart,enElementWater|0x10,enElementWater|0x10,
        enElementFire|0x30},
    {enElementWood,enElementHeart,enElementWood,enElementWater|0x10,enElementWood,
        enElementHeart},
    {enElementHeart,enElementLight,enElementWater,enElementWood|0x10,enElementWater,
        enElementWood},
    {enElementFire,enElementFire,enElementWood|0x50,enElementWater|0x10,enElementWood
        ,enElementWood},
};


static int gNewbieGrid5[kMaxGridRowCnt][kMaxGridColumnCnt] = {
    {enElementFire, enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater},
    {enElementWater, enElementWood, enElementHeart, enElementFire, enElementWater, enElementFire},
    {enElementWood, enElementHeart, enElementFire, enElementDark, enElementLight, enElementHeart},
    {enElementFire, enElementLight, enElementWater, enElementWood, enElementWater, enElementWood},
    {enElementHeart, enElementHeart, enElementWood, enElementHeart, enElementWood| 0x30 ,  enElementFire | 0x50 }
};


void StoneGrid::CheckInterrupt(CCObject* obj)
{
    if(
       //(CGameState::Instance()->getState() != enStateWaitTouch)&&
       (CGameState::Instance()->getState() != enStateTouching))
    {
        return;
    }
    
    if(m_spHold != NULL){
        CCPoint nodePos = m_spHold->getPositionInPixels();
        int iColumn, iRow;
        iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
        iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
        
        m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setOpacity(255);
        
        CCNode* spPlus = m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getChildByTag(kStnPlusTag);
        if (spPlus) {
            ((CCSprite*)spPlus)->setOpacity(255);
        }
        
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    m_ptCurTouch.x = -1000;
    m_ptCurTouch.y = -1000;
    
    if(m_bFreeFlg == false)
        touchEndProc();
}

//******************************************************************************
// Constructor
//******************************************************************************
StoneGrid::StoneGrid(CCObject* target, SEL_CallFunc selector):m_bStartFlg(false)
{
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(StoneGrid::CheckInterrupt),  // 处理的消息的回调函数
                                                           "CHECK_TOUCH_MOVE_INTERRUPT",                    // 感兴趣的消息名称
                                                            NULL);
    
    gNewbieGrid5[4][5]=enElementFire | 0x50;
    gNewbieGrid5[4][4]=enElementWood | 0x30;
    
    m_ArrowLayer = NULL;
    m_pMsgBox = NULL;
    m_spHold = NULL;
    m_spNewbieStart = NULL;
    m_pArrow = NULL;
    m_pLongArrow = NULL;
    m_iCombo = 0;
    m_iMaxStoneNum = enElementMax;
    m_fAdditionalDur = 0.0;
    m_fMaxMvDur = kStoneMvDur2 + m_fAdditionalDur;
    m_bFreeFlg = false;
    m_bInitMatrixFlg = false;
    m_iNewbieStep = 0;
    m_iSndTag = 0;
    m_newBie4Through = 0;
    m_bUserDid = false;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    if (SaveData::Inst()->getIsUseSaveData()) {
        m_stoneCount = SaveData::Inst()->getStoneCount();
        for (int i = 0; i < m_stoneCount; i++) {
            m_stoneElement[i] = SaveData::Inst()->getStoneElement(i);
        }
    }
    else {
        CAvailDiamond* availDiamond = CGameData::Inst()->getCurAvailDiamond();
        m_stoneCount = availDiamond->cnt;   
        
        for (int i = 0; i < m_stoneCount; i++) {
            m_stoneElement[i] = availDiamond->buf[i];
        }
    }
    
    //back ground
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("stonebk.png");
    sp->setScale(2.0);
    sp->setPositionInPixels(CCPointMake(320, 265));
    addChild(sp, -1);
    
    //hand
    m_spHand1 = CCSprite::spriteWithSpriteFrameName("hand1.png");
    addChild(m_spHand1, 10);
    m_spHand2 = CCSprite::spriteWithSpriteFrameName("hand2.png");
    addChild(m_spHand2, 10);
    m_spHandProgrs = CCSprite::spriteWithSpriteFrameName("handprogrs.png");
    m_spHandProgrs->setAnchorPoint(CCPointMake(0, 0));
    addChild(m_spHandProgrs, 10);
    setHand(CCPointMake(-1000, -1000), 1.0);
    
    //newbie guide
    if(CGameData::Inst()->isNewbie()){
        m_spNewbieStart = CCSprite::spriteWithFile("fr_light_circular.png");
        if(m_spNewbieStart){
            addChild(m_spNewbieStart, 1);
            m_spNewbieStart->setPosition(CCPointMake(-1000, -1000));
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                               CCFadeIn::actionWithDuration(0.5),
                                                                               NULL);
            m_spNewbieStart->runAction(CCRepeatForever::actionWithAction(action));
        }
    }
}

//******************************************************************************
// ~StoneGrid
//******************************************************************************
StoneGrid::~StoneGrid()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this,"CHECK_TOUCH_MOVE_INTERRUPT");
}


//******************************************************************************
// enableTouch
//******************************************************************************
void StoneGrid::enableTouch(bool bTouch)
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
void StoneGrid::initMatrix(int newbieStep)
{
    m_bUserDid = false;
    CGameState::Instance()->setState(enStateChgStone);
    
    m_iNewbieStep = newbieStep;
    
    switch (newbieStep) {
        case 0:
            if (SaveData::Inst()->getIsUseSaveData()) {
                initMatrixNml(false, true);
            }
            else {
                initMatrixNml(false);
            }
            break;
        case 1:
            initMatrixNewbie(newbieStep, true);
            break;
        case 5:
            m_iNewbieStep5 = 1;
        case 2:
        case 3:
        case 4:
            initMatrixNewbie(newbieStep, false);
        default:
            break;
    }
    
    m_bInitMatrixFlg = true;
}

//******************************************************************************
// initGuideMatrix
//******************************************************************************
void StoneGrid::initGuideMatrix(int idx)
{
    idx = (idx < 0)? 0: idx;
    idx = (idx > 4)? 4: idx;
    
    if (!m_ArrowLayer) {
        m_ArrowLayer = CCLayer::node();
    }
    
    CCPoint pt, ptSrc;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            int iPicType = gGuideMatrix[idx][j][i].iPicType;
            int iPicType1 = iPicType >> 16;
            int iPicType2 = iPicType & 0xFFFF;
            int iRotation = gGuideMatrix[idx][j][i].iRotation;
            int iRotation1 = iRotation >> 16;
            int iRotation2 = iRotation & 0xFFFF;
            
            int iElem =gGuideMatrix[idx][j][i].iElem;
            //sprite
            m_stoneNodeArray[j][i].spStone = getSpriteByElement(iElem);
            m_stoneNodeArray[j][i].iElement=iElem;
            m_stoneNodeArray[j][i].x = i;
            m_stoneNodeArray[j][i].y = j;
            
            ptSrc.x = gPtStart.x + i * gfSpSize;
            ptSrc.y = gPtStart.y + (j + kMaxGridRowCnt) * gfSpSize;
            pt.x = gPtStart.x + i * gfSpSize;
            pt.y = gPtStart.y + j * gfSpSize;
            
            if(m_stoneNodeArray[j][i].spStone){
                m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.3, pt));
            }
            
            if (iPicType1!=0) {
                char bufFilename[20];
                sprintf(bufFilename, "line000%d.png",iPicType1);
                CCSprite* sp = CCSprite::spriteWithFile(bufFilename);
                m_ArrowLayer->addChild(sp,10);
                sp->setTag(1);
                if (iRotation1!=0) {
                    sp->setRotation(iRotation1);
                }
                if (sp) {
                    sp->setPositionInPixels(ptSrc);
                    sp->runAction(CCMoveTo::actionWithDuration(0.3, pt));
                    
                }
            }
            if (iPicType2!=0) {
                char bufFilename[20];
                sprintf(bufFilename, "line000%d.png",iPicType2);
                CCSprite* sp = CCSprite::spriteWithFile(bufFilename);
                m_ArrowLayer->addChild(sp,10);
                sp->setTag(1);
                if (iRotation2!=0) {
                    sp->setRotation(iRotation2);
                }
                if (sp) {
                    sp->setPositionInPixels(ptSrc);
                    sp->runAction(CCMoveTo::actionWithDuration(0.3, pt));
                }
            }
        }
    }
    addChild(m_ArrowLayer);
    
    m_bInitMatrixFlg = true;
}

//******************************************************************************
// removeArrowLayer
//******************************************************************************
void StoneGrid::removeArrowLayer()
{
    if (m_ArrowLayer) {
        removeChild(m_ArrowLayer, true);
        m_ArrowLayer = NULL;
    }
}


int StoneGrid::getOddsAwake10(float oddf, float nums)
{
    float f = powf((1.0f - oddf), nums);
    float f2 = (1.0f - f) * 100;
    int ret = (int)f2;
    return ret;
}

//******************************************************************************
// initMatrixNml
//******************************************************************************
void StoneGrid::initMatrixNml(bool bErasable, bool bUseSaveData)
{
    CCPoint pt, ptSrc;
    m_bUserDid = false;
    if (bUseSaveData)
    {
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                m_stoneNodeArray[j][i].iElement = SaveData::Inst()->getStone(j, i);
                m_stoneNodeArray[j][i].iPlus = SaveData::Inst()->getStonePlus(j, i);
                m_stoneNodeArray[j][i].iBlackCover = SaveData::Inst()->getStoneBlackCover(j,i);
                m_stoneNodeArray[j][i].iParam1 = SaveData::Inst()->getstoneParam1(j,i);
                m_stoneNodeArray[j][i].iParam2 = SaveData::Inst()->getstoneParam2(j,i);
                m_stoneNodeArray[j][i].x = i;
                m_stoneNodeArray[j][i].y = j;
            }
        }
    }
    else
    {
        Player* pPlayer = (Player*)m_pListener;

        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                m_stoneNodeArray[j][i].iElement = randElement();
                
                if ((m_stoneNodeArray[j][i].iElement < enElementInvalid)
                    && (m_stoneNodeArray[j][i].iElement > enElementHeart)
                    && (SaveData::Inst()->getrand() <   getOddsAwake10(0.2f, (float)(pPlayer->getNumByAwakeId(m_stoneNodeArray[j][i].iElement + enAwakeSkill_9)))))
                    m_stoneNodeArray[j][i].iPlus = 1;
                else
                    m_stoneNodeArray[j][i].iPlus = 0;
                
                m_stoneNodeArray[j][i].iBlackCover = 0;
                m_stoneNodeArray[j][i].iParam1 = 0;
                m_stoneNodeArray[j][i].iParam2 = 0;
                m_stoneNodeArray[j][i].x = i;
                m_stoneNodeArray[j][i].y = j;
                
            }
        }
        
        while(!bErasable){
            if(chkErasable() == false)
                break;
            
            for(int i = 0; i < kMaxGridColumnCnt; i ++){
                for(int j = 0; j < kMaxGridRowCnt; j ++){
                    if(m_stoneNodeArray[j][i].bErasable == true)
                    {
                        m_stoneNodeArray[j][i].iElement = randElement();
                        
                        if ((m_stoneNodeArray[j][i].iElement < enElementInvalid)
                             && (m_stoneNodeArray[j][i].iElement > enElementHeart)
                             && (SaveData::Inst()->getrand() <   getOddsAwake10(0.2f, (float)(pPlayer->getNumByAwakeId(m_stoneNodeArray[j][i].iElement + enAwakeSkill_9)))))
                            m_stoneNodeArray[j][i].iPlus = 1;
                        else
                            m_stoneNodeArray[j][i].iPlus = 0;

                        m_stoneNodeArray[j][i].iBlackCover = 0;
                        m_stoneNodeArray[j][i].iParam1 = 0;
                        m_stoneNodeArray[j][i].iParam2 = 0;
                        m_stoneNodeArray[j][i].x = i;
                        m_stoneNodeArray[j][i].y = j;
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
                
                if (m_stoneNodeArray[j][i].iPlus == 1) {
                    CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                    if (spPlus) {
                        m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                        spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                                m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                    }
                }
                
                if (m_stoneNodeArray[j][i].iBlackCover == 1)
                {
                    m_stoneNodeArray[j][i].spStone->setColor(ccBLACK);
                }
            }
            
            m_stoneNodeArray[j][i].bAlive = true;
        }
    }
}

//******************************************************************************
// initMatrixNewbie
//******************************************************************************
void StoneGrid::initMatrixNewbie(int newbieStep, bool bDrop)
{
    CCPoint pt, ptSrc;
    bool bMovable;
    
    if(m_bInitMatrixFlg){
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                removeChild(m_stoneNodeArray[j][i].spStone, true);
                m_stoneNodeArray[j][i].spStone = NULL;
            }
        }
    }
    
    //create stones
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            bMovable = false;
            switch (newbieStep) {
                case 1:
                    m_stoneNodeArray[j][i].iElement = gNewbieGrid1[j][i] & StoneGridMask_Element;
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(gNewbieGrid1[j][i] & StoneGridMask_Element);
                    if(gNewbieGrid1[j][i] & StoneGridMask_Movable){
                        bMovable = true;
                    }
                    break;
                case 2:
                    m_stoneNodeArray[j][i].iElement = gNewbieGrid2[j][i] & StoneGridMask_Element;
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(gNewbieGrid2[j][i] & StoneGridMask_Element);
                    if(gNewbieGrid2[j][i] & StoneGridMask_Movable){
                        bMovable = true;
                    }
                    break;
                case 3:
                    m_stoneNodeArray[j][i].iElement = gNewbieGrid3[j][i] & StoneGridMask_Element;
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(gNewbieGrid3[j][i] & StoneGridMask_Element);
                    if(gNewbieGrid3[j][i] & StoneGridMask_Movable){
                        bMovable = true;
                    }
                    break;
                case 4:
                    m_stoneNodeArray[j][i].iElement = gNewbieGrid4[j][i] & StoneGridMask_Element;
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(gNewbieGrid4[j][i] & StoneGridMask_Element);
                    if(gNewbieGrid4[j][i] & StoneGridMask_Movable){
                        bMovable = true;
                    }
                    break;
                case 5:
                    m_stoneNodeArray[j][i].iElement = gNewbieGrid5[j][i] & StoneGridMask_Element;
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(gNewbieGrid5[j][i] & StoneGridMask_Element);
                    if(gNewbieGrid5[j][i] & StoneGridMask_Movable){
                        bMovable = true;
                    }
                    break;
                    
                default:
                    break;
            }
            
            if(bMovable){
            }
            else{
                if(m_stoneNodeArray[j][i].spStone)
                    m_stoneNodeArray[j][i].spStone->setOpacity(100);
            }
            
            ptSrc.x = gPtStart.x + i * gfSpSize;
            ptSrc.y = gPtStart.y + (j + kMaxGridRowCnt) * gfSpSize;
            pt.x = gPtStart.x + i * gfSpSize;
            pt.y = gPtStart.y + j * gfSpSize;
            
            if(m_stoneNodeArray[j][i].spStone){
                if(bDrop){
                    m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                    m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.3, pt));
                }
                else {
                    m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                }
            }
            
            m_stoneNodeArray[j][i].bAlive = true;
            m_stoneNodeArray[j][i].iPlus = 0;
            m_stoneNodeArray[j][i].iBlackCover = 0;
            m_stoneNodeArray[j][i].iParam1 = 0;
            m_stoneNodeArray[j][i].iParam2 = 0;
            m_stoneNodeArray[j][i].x = i;
            m_stoneNodeArray[j][i].y = j;
        }
    }
}

//******************************************************************************
// resetLink
//******************************************************************************
void StoneGrid::resetLink()
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

void StoneGrid::saveAllStones()
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].spStone)
            {
                SaveData::Inst()->setStone(j, i, m_stoneNodeArray[j][i].iElement);
                SaveData::Inst()->setStonePlus(j, i, m_stoneNodeArray[j][i].iPlus);
                SaveData::Inst()->setStoneBlackCover(j,i,m_stoneNodeArray[j][i].iBlackCover);
                SaveData::Inst()->setstoneParam1(j, i, m_stoneNodeArray[j][i].iParam1);
                SaveData::Inst()->setstoneParam2(j, i, m_stoneNodeArray[j][i].iParam2);
            }
        }
    }
}

//******************************************************************************
// startNewRound
//******************************************************************************
void StoneGrid::startNewRound()
{
    m_iCombo = 0;
    m_bStartFlg = false;
    m_bFreeFlg = false;
    m_fMaxMvDur = kStoneMvDur2 + m_fAdditionalDur;
    m_ptCurTouch.x = -1000;
    m_ptCurTouch.y = -1000;
    m_bUserDid = false;
    
    if(!m_bInitMatrixFlg)
        return;
    
    if(m_iNewbieStep == 4)
    {
        newBieLineArr[0].origin = m_stoneNodeArray[1][3].spStone->getPosition();
        newBieLineArr[0].size = m_stoneNodeArray[1][3].spStone->getContentSize();
        
        newBieLineArr[1].origin = m_stoneNodeArray[1][4].spStone->getPosition();
        newBieLineArr[1].size = m_stoneNodeArray[1][4].spStone->getContentSize();
        
        newBieLineArr[2].origin = m_stoneNodeArray[1][5].spStone->getPosition();
        newBieLineArr[2].size = m_stoneNodeArray[1][5].spStone->getContentSize();
        
        newBieLineArr[3].origin = m_stoneNodeArray[2][3].spStone->getPosition();
        newBieLineArr[3].size = m_stoneNodeArray[2][3].spStone->getContentSize();
        
        newBieLineArr[4].origin = m_stoneNodeArray[3][3].spStone->getPosition();
        newBieLineArr[4].size = m_stoneNodeArray[3][3].spStone->getContentSize();
        
        newBieLineArr[5].origin = m_stoneNodeArray[4][2].spStone->getPosition();
        newBieLineArr[5].size = m_stoneNodeArray[4][2].spStone->getContentSize();
        
        newBieLineArr[6].origin = m_stoneNodeArray[4][3].spStone->getPosition();
        newBieLineArr[6].size = m_stoneNodeArray[4][3].spStone->getContentSize();
    }
    
    if(m_iNewbieStep == 0){
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone){
                    m_stoneNodeArray[j][i].spStone->setOpacity(255);
                    SaveData::Inst()->setStone(j, i, m_stoneNodeArray[j][i].iElement);
                    SaveData::Inst()->setStonePlus(j, i, m_stoneNodeArray[j][i].iPlus);
                    SaveData::Inst()->setStoneBlackCover(j,i,m_stoneNodeArray[j][i].iBlackCover);
                    SaveData::Inst()->setstoneParam1(j, i, m_stoneNodeArray[j][i].iParam1);
                    SaveData::Inst()->setstoneParam2(j, i, m_stoneNodeArray[j][i].iParam2);
                    CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                    if (spPlus) {
                        ((CCSprite*)spPlus)->setOpacity(255);
                    }
                }
            }
        }
        
        
    }
    
    if((m_iNewbieStep == 1) || (m_iNewbieStep == 2)){
        m_spNewbieStart->setIsVisible(true);
        m_spNewbieStart->setPosition(m_stoneNodeArray[4][5].spStone->getPosition());
        
        if(m_pArrow)
            removeChild(m_pArrow, true);
        
        m_pArrow = new Arrow(80, 196);
        m_pArrow->setPosition(m_stoneNodeArray[4][4].spStone->getPosition());
        addChild(m_pArrow, 10);
        m_pArrow->release();
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        m_pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(m_iNewbieStep == 3){
        m_spNewbieStart->setIsVisible(true);
        m_spNewbieStart->setPosition(m_stoneNodeArray[2][1].spStone->getPosition());
        
        if(m_pArrow)
            removeChild(m_pArrow, true);
        
        m_pArrow = new Arrow(180, 196);
        m_pArrow->setPosition(m_stoneNodeArray[2][3].spStone->getPosition());
        addChild(m_pArrow, 10);
        m_pArrow->release();
        m_pArrow->setRotation(180);
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        m_pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(m_iNewbieStep == 4)
    {
        m_spNewbieStart->setIsVisible(true);
        m_spNewbieStart->setPosition(m_stoneNodeArray[1][5].spStone->getPosition());
        if(m_pLongArrow)
        {
            removeChild(m_pLongArrow, true);
            m_pLongArrow = NULL;
        }
        
        m_pLongArrow = CCSprite::spriteWithFile("long_arrow.png");
        m_pLongArrow->setPosition(ccp(440, 325));
        addChild(m_pLongArrow, 10);
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCScaleTo::actionWithDuration(0.5, 1.03),
                                                                           CCScaleTo::actionWithDuration(0.5,0.97),
                                                                           NULL);
        m_pLongArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if(m_iNewbieStep == 5){
        CCPoint ptStart;
        switch (m_iNewbieStep5) {
            case 0:
                ptStart = m_stoneNodeArray[4][5].spStone->getPosition();
                
                m_pArrow = new Arrow(80, 196);
                m_pArrow->setPosition(m_stoneNodeArray[4][4].spStone->getPosition());
                break;
            case 1:
                ptStart = m_stoneNodeArray[4][4].spStone->getPosition();
                
                m_pArrow = new Arrow(80, 196);
                m_pArrow->setRotation(180);
                m_pArrow->setPosition(m_stoneNodeArray[4][5].spStone->getPosition());
                break;
            case 2:
                ptStart = m_stoneNodeArray[4][3].spStone->getPosition();
                if(m_spNewbieStart)
                    removeChild(m_spNewbieStart, true);
                m_spNewbieStart = CCSprite::spriteWithFile("fr_light_square.png");
                if(m_spNewbieStart){
                    addChild(m_spNewbieStart, 1);
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeOut::actionWithDuration(0.5),
                                                                                       CCFadeIn::actionWithDuration(0.5),
                                                                                       NULL);
                    m_spNewbieStart->runAction(CCRepeatForever::actionWithAction(action));
                }
                
                
                m_pArrow = new Arrow(80, 196);
                m_pArrow->setPosition(m_stoneNodeArray[4][2].spStone->getPosition());
                break;
            default:
                ptStart = CCPointMake(-1000, -1000);
                m_pArrow = NULL;
                break;
        }
        
        m_spNewbieStart->setIsVisible(true);
        m_spNewbieStart->setPosition(ptStart);
        
        if(m_pArrow){
            addChild(m_pArrow, 10);
            m_pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               NULL);
            m_pArrow->runAction(CCRepeatForever::actionWithAction(action));
        }
        
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone){
                    if(gNewbieGrid5[j][i] & StoneGridMask_Movable){
                        m_stoneNodeArray[j][i].spStone->setOpacity(255);
                    }
                    else {
                        m_stoneNodeArray[j][i].spStone->setOpacity(100);
                    }
                }
            }
        }
    }
    
    //remove plus lable
    while (getChildByTag(kPlusLbTagBase)) {
        removeChildByTag(kPlusLbTagBase, true);
    }
    
    //remove guangquan sprite
    while (getChildByTag(kGuangQuanTag)) {
        removeChildByTag(kGuangQuanTag, true);
    }
    
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool StoneGrid::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(m_pMsgBox){
        removeChild(m_pMsgBox, true);
        m_pMsgBox = NULL;
        MainScene::Inst()->removeTouchTip();
        
        return false;
    
    }

    
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
    
    
    
    if(CGameData::Inst()->isNewbie() && MainScene::Inst()->m_spTouchTip != NULL)
    return false;
    
    if(m_iNewbieStep == 0){
    }
    else if(m_iNewbieStep == 1){
        if(gNewbieGrid1[iRow][iColumn] & StoneGridMask_Start){
        }
        else{
            return false;
        }
    }
    else if(m_iNewbieStep == 2){
        if(gNewbieGrid2[iRow][iColumn] & StoneGridMask_Start){
        }
        else{
            return false;
        }
    }
    else if(m_iNewbieStep == 3){
        if(gNewbieGrid3[iRow][iColumn] & StoneGridMask_Start){
        }
        else{
            return false;
        }
    }
    else if(m_iNewbieStep == 4){
        if(gNewbieGrid4[iRow][iColumn] & StoneGridMask_Start){
        }
        else{
            return false;
        }
    }
    else if(m_iNewbieStep == 5){
        if(gNewbieGrid5[iRow][iColumn] & StoneGridMask_Start){
        }
        else{
            return false;
        }
    }
    
    
    if(m_spHold != NULL){
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    int iElem = m_stoneNodeArray[iRow][iColumn].iElement;
    m_spHold = getSpriteByElement(iElem);
    if (m_spHold) {
        CCPoint pt = m_stoneNodeArray[iRow][iColumn].spStone->getPositionInPixels();
        pt.y += m_spHold->getContentSize().height /4;
        m_spHold->setPositionInPixels(pt);
        
        m_stoneNodeArray[iRow][iColumn].spStone->setOpacity(100);
        
        if (m_stoneNodeArray[iRow][iColumn].iPlus == 1) {
            CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
            if (spPlus) {
                m_spHold->addChild(spPlus, 0, kStnPlusTag);
                spPlus->setPosition(ccp(m_spHold->getContentSize().width*0.7,
                                        m_spHold->getContentSize().width*0.3));
            }
            
            CCNode* sp = m_stoneNodeArray[iRow][iColumn].spStone->getChildByTag(kStnPlusTag);
            if (sp) {
                ((CCSprite*)sp)->setOpacity(100);
            }
        }
    }
    
    m_iStartRow = iRow;
    m_iStartColumn = iColumn;
    m_iCurRow = iRow;
    m_iCurColumn = iColumn;
    
    if(m_spNewbieStart)
        m_spNewbieStart->setIsVisible(false);
    
    return true;
}

void StoneGrid::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if((CGameState::Instance()->getState() != enStateWaitTouch)
       &&(CGameState::Instance()->getState() != enStateTouching))
        return;
    
    if(m_spHold == NULL)
        return;
    
    m_bUserDid = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    m_ptCurTouch = nodePos;
    
    int iColumn, iRow;
    float float_iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
    float float_iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;

    iColumn = float_iColumn;
    iRow = float_iRow;
    if(iColumn < 0)
        iColumn = 0;
    
    if(iColumn >= kMaxGridColumnCnt)
        iColumn = kMaxGridColumnCnt -1;
    
    if(iRow < 0)
        iRow = 0;
    
    if(iRow >= kMaxGridRowCnt)
        iRow = kMaxGridRowCnt -1;
    
    if(((m_iNewbieStep >= 1) && (m_iNewbieStep <= 2))
       ||(m_iNewbieStep == 5)){
        iRow = 4;
    }
    
    if(m_iNewbieStep == 3)
    {
        iRow = 2;
    }
    
    
    //    if(m_iNewbieStep == 4)
    //        iRow = 2;
    
    CCPoint pt = nodePos;
    pt.y += m_spHold->getContentSize().height /4;
    
    
    //限定路径
    if(m_iNewbieStep == 4)
    {
        bool isInLine = false;
        for(int i=0;i<7;i++)
        {
            CCPoint pt = newBieLineArr[i].origin;
            CCSize sz = newBieLineArr[i].size;
            
            bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                               pt.y - sz.height * 0.5,
                                                               sz.width,
                                                               sz.height),
                                                    nodePos);
            
            
            if(bRet){
                isInLine |= true;
                m_newBie4Through |=(1<<i);
            }
        }
        
        if(isInLine)
        {
            m_spHold->setPositionInPixels(pt);
            
        }
        
    }else
        
        m_spHold->setPositionInPixels(pt);
    
    if((iColumn != m_iCurColumn) || (iRow != m_iCurRow ))
    {
        Player* pPlayer = (Player*)m_pListener;
        pPlayer->RecordControl(false,0);

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
        
        if(m_iNewbieStep == 0){
        }
        else if(m_iNewbieStep == 1){
            if(gNewbieGrid1[iRow][iColumn] & StoneGridMask_Movable){
            }
            else{
                return;
            }
        }
        else if(m_iNewbieStep == 2){
            if(gNewbieGrid2[iRow][iColumn] & StoneGridMask_Movable){
            }
            else{
                return;
            }
        }
        else if(m_iNewbieStep == 3){
            //  CCLog("row %d column %d gNewBieGrid %d",iRow,iColumn,gNewbieGrid3[iRow][iColumn]);
            if(gNewbieGrid3[iRow][iColumn] & StoneGridMask_Movable){
                
            }
            else{
                //CCLog("step 3 move");
                return;
            }
        }
        else if(m_iNewbieStep == 4){
            if(gNewbieGrid4[iRow][iColumn] & StoneGridMask_Movable){
            }
            else{
                return;
            }
        }
        else if(m_iNewbieStep == 5){
            if(gNewbieGrid5[iRow][iColumn] & StoneGridMask_Movable){
            }
            else{
                return;
            }
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
//        if (m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover == 1)
//        {
//            
//            m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor();
//            
//            m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover = 0;
//            m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setColor(ccWHITE);
//            
//            
//            
//
//        }
        
        if (m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().r == 0
            && m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().g == 0
            && m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().b == 0)
        {
            
            
            m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover = 0;
            m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setColor(ccWHITE);
            
            
            
            
        }
        
        m_iCurRow = iRow;
        m_iCurColumn = iColumn;
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonemv));
    }
}

void StoneGrid::ccTouchEnded(CCTouch* touch, CCEvent* event)
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
        
        CCNode* spPlus = m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getChildByTag(kStnPlusTag);
        if (spPlus) {
            ((CCSprite*)spPlus)->setOpacity(255);
        }
        
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    m_ptCurTouch.x = -1000;
    m_ptCurTouch.y = -1000;
    
    if(m_bFreeFlg == false)
        touchEndProc();
}

void StoneGrid::changeMaxDurByTime(float dt)
{
    m_fAdditionalDur += dt;
}

//******************************************************************************
// update
//******************************************************************************
void StoneGrid::update(ccTime dt)
{
    m_fElapse += dt;
    
    if(m_fElapse > m_fMaxMvDur){
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(timeout));
        
        touchEndProc();
    }
    else if(m_fElapse > (m_fMaxMvDur - kStoneMvDur1)){
        float fPercent = (m_fMaxMvDur - m_fElapse) / kStoneMvDur1;
        if(m_iNewbieStep == 4)
            setHand(m_spHold->getPosition(), fPercent);
        else
            setHand(m_ptCurTouch, fPercent);
    }
    else {
        setHand(CCPointMake(-1000, -1000), 1.0);
    }
}

//******************************************************************************
// getSpriteByElement
//******************************************************************************
CCSprite* StoneGrid::getSpriteByElement(int iElement)
{    
    char bufFilename[20];
    if((iElement < 0) || (iElement >= enElementMax))
        return NULL;
    
    if (OcProxy::Inst()->getBoolValueForKey("caidan"))
    {
        CCSprite* sp = NULL;
        if (iElement == enElementInvalid)
        {
            sp = CCSprite::spriteWithSpriteFrameName("stone6.png");
        }
        else
        {
            sprintf(bufFilename, "caidan%d.png", iElement);
            sp = CCSprite::spriteWithFile(bufFilename);
        }
        
        if (sp) {
            addChild(sp);
            sp->setTag(0);
        }
        
        return sp;
    }
    
    if (OcProxy::Inst()->is568HighScreen() && !OcProxy::Inst()->isIpadDevice()) // if iphone5
    {
        sprintf(bufFilename, "stone%d_iphone5.png", iElement);
        CCSprite* sp = NULL;
        
        if (iElement == enElementInvalid)
        {
            sp = CCSprite::spriteWithSpriteFrameName("stone6.png");
        }
        else
        {
            sp = CCSprite::spriteWithFile(bufFilename);
        }
        
        if (sp) {
            addChild(sp);
            sp->setTag(0);
        }
        
        return sp;
    }
    else
    {
        sprintf(bufFilename, "stone%d.png", iElement);
        CCSprite* sp = NULL;
//        if (iElement == enElementInvalid)
//        {
//            sp = CCSprite::spriteWithFile(bufFilename);
//        }
//        else
        {
            sp = CCSprite::spriteWithSpriteFrameName(bufFilename);
        }
        addChild(sp);
        sp->setTag(0);
        return sp;
        
    }
}

//******************************************************************************
// srchGrp
//******************************************************************************
bool StoneGrid::srchGrp()
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
void StoneGrid::sch1Grp(PStoneNode node, int iElement)
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
bool StoneGrid::chkErasable()
{
    int iRow, iColum;
    int iStart, iEnd;
    
    bool bRet = false;
    
    resetLink();
    
    //search column
    for(iColum = 0; iColum < kMaxGridColumnCnt; iColum ++)
    {
        iRow = 0;
        iStart = 0;
        iEnd = 0;
        while(iRow < kMaxGridRowCnt-1){
            
            if(m_stoneNodeArray[iRow][iColum].iElement.get() == m_stoneNodeArray[iRow+1][iColum].iElement.get()){
                iEnd = iRow + 1;
            }
            
            if((m_stoneNodeArray[iRow][iColum].iElement.get() != m_stoneNodeArray[iRow+1][iColum].iElement.get())
               ||(iRow == kMaxGridRowCnt -2)){
                
                if(iEnd - iStart >= 2)
                {
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
    for(iRow = 0; iRow < kMaxGridRowCnt; iRow ++)
    {
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

void StoneGrid::updateAwakeAtkRatio(int idx)
{
    Player* pPlayer = (Player*)m_pListener;
    if (pPlayer->getCurManSkillID() == enManSkill_255 ) {    // 横向 6消 时 处理的武将技  （这里只是借此方法）
        switch (idx)
        {
            case enElementFire:
            {
                if (pPlayer->getEnhanceH6CD() == 1) {
                    float factor = pPlayer->getEnhanceH6Factor();
                    if (factor > 0)
                        pPlayer->updateAwakeAtk(factor, enElementFire);
                }
            }
                break;
            default:
                break;
        }

    }
    
    switch (idx)
    {
        case enElementHeart:
        {
            Player* pPlayer = (Player*)m_pListener;
            
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_9);
            if (cout > 0)
                //pPlayer->updateAwakeAtk(1.0001f * cout, enElementHeart);
                pPlayer->fengYinUpdate(cout);
        }
            break;
            
        case enElementFire:
        {
            Player* pPlayer = (Player*)m_pListener;
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_4);
            if (cout > 0)
                pPlayer->updateAwakeAtk(0.1f * cout, enElementFire);
        }
            break;
            
        case enElementWater:
        {
            Player* pPlayer = (Player*)m_pListener;
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_5);
            if (cout > 0)
                pPlayer->updateAwakeAtk(0.1f * cout, idx);
        }
            break;
            
        case enElementWood:
        {
            Player* pPlayer = (Player*)m_pListener;
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_6);
            if (cout > 0)
                pPlayer->updateAwakeAtk(0.1f * cout, idx);
        }
            break;
            
        case enElementLight:
        {
            Player* pPlayer = (Player*)m_pListener;
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_7);
            if (cout > 0)
                pPlayer->updateAwakeAtk(0.1f * cout, idx);
        }
            break;
            
        case enElementDark:
        {
            Player* pPlayer = (Player*)m_pListener;
            int cout = pPlayer->getNumByAwakeId(enAwakeSkill_8);
            if (cout > 0)
                pPlayer->updateAwakeAtk(0.1f * cout, idx);
        }
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// erase
//******************************************************************************
void StoneGrid::erase(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::erase));
    
    if(m_iGrpNo <= 0){
        drop();
        return;
    }
    
    char buf[50];
    m_iCombo++;
    
    m_iGrpNo--;
    
    m_curEraseCnt = m_grpList[m_iGrpNo].size();
    m_fCurErasePlus = 1.0f;
    
    int iRand = rand() % (m_grpList[m_iGrpNo].size() -1);

    bool bChkFullColumn = false;
    
    for (StoneNodeIter it = m_grpList[m_iGrpNo].begin(); it != m_grpList[m_iGrpNo].end(); ++it)
    {
        PStoneNode node = *it;
        
        if (!(node->x >=kMaxGridColumnCnt || node->x<0 || node->y<0 || node->y>=kMaxGridRowCnt))
        {
            if ((!bChkFullColumn) && node->x == 0)
            {
                int y = node->y;
                if (m_stoneNodeArray[y][0].iElement.get() == m_stoneNodeArray[y][1].iElement.get()
                    && m_stoneNodeArray[y][0].iElement.get() == m_stoneNodeArray[y][2].iElement.get()
                    && m_stoneNodeArray[y][0].iElement.get() == m_stoneNodeArray[y][3].iElement.get()
                    && m_stoneNodeArray[y][0].iElement.get() == m_stoneNodeArray[y][4].iElement.get()
                    && m_stoneNodeArray[y][0].iElement.get() == m_stoneNodeArray[y][5].iElement.get()
                    )
                {
                    bChkFullColumn = true;
                    updateAwakeAtkRatio (m_stoneNodeArray[y][0].iElement);
                }
            }
        }
        
        if(node->spStone){
            node->spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.1),
                                                         CCScaleTo::actionWithDuration(0.2, 0.5),
                                                         CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.1, 1.0),
                                                                               CCFadeOut::actionWithDuration(0.2)),
                                                         NULL));
            
            if (node->iElement >= enElementHeart && node->iElement <= enElementInvalid)
            {
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
            
            CCNode* spPlus = node->spStone->getChildByTag(kStnPlusTag);
            if (spPlus) {
                spPlus->runAction(CCFadeOut::actionWithDuration(0.5));
            }
        }
        node->bAlive = false;
        
        m_curEraseElement = node->iElement;
        if (node->iPlus == 1) {
            m_fCurErasePlus += 0.06;
            node->iPlus = 0;
        }
        
        if (node->iBlackCover == 1)
        {
            node->iBlackCover = 0;
        }
        
        if (node->iParam1 == enSpecialStoneBomb)
        {
            node->iParam1 = 0;
            node->iParam2 = 0;
        }
        
        if((iRand == 0) && (node->spStone))
        {
            m_curErasePos = node->spStone->getPositionInPixels();
        }
        iRand--;
    }
    
    // 属性加号效果
    if (m_fCurErasePlus > 1.0) {
        char buf[10];
        snprintf(buf, 9, "x%.2f", (ceil(m_fCurErasePlus*100))*0.01);
        TextNode* lbPlus = TextNode::textWithString(buf, 24);
        lbPlus->setAnchorPoint(ccp(0.5, 0.5));
        addChild(lbPlus, 1, kPlusLbTagBase + m_iCombo);
        lbPlus->setPositionInPixels(CCPointMake(m_curErasePos.x,
                                                m_curErasePos.y - 15));
        
        switch (m_curEraseElement)
        {
            case enElementHeart:
                lbPlus->setShadowColor(ccc3(243,40,221));
                break;
                
            case enElementWater:
                lbPlus->setShadowColor(ccc3(20,106,208));
                break;
                
            case enElementFire:
                lbPlus->setShadowColor(ccc3(231,19,10));
                break;
                
            case enElementWood:
                lbPlus->setShadowColor(ccc3(44,105,5));
                break;
                
            case enElementLight:
                lbPlus->setShadowColor(ccc3(236,190,25));
                break;
                
            case enElementDark:
                lbPlus->setShadowColor(ccc3(170,0,240));
                break;
                
            default:
                break;
        }

        lbPlus->setScale(1.5);
        lbPlus->runAction(CCScaleTo::actionWithDuration(0.1, 1));
    }
    
    // combo效果
    if(m_iCombo > 1){
        sprintf(buf, "*+,-./%d", m_iCombo);
        CCLabelAtlas* lb = CCLabelAtlas::labelWithString(buf, "lbcombo2.png", 15, 21, '*');
        addChild(lb, 1);
        lb->setAnchorPoint(CCPointMake(0.5, 0.0));
        lb->setPositionInPixels(m_curErasePos);
        m_lbComboList.push_back(lb);
        lb->setScale(1.5);
        lb->runAction(CCScaleTo::actionWithDuration(0.1, 1));
        
        lb = CCLabelAtlas::labelWithString(buf, "lbcombo.png", 15, 21, '*');
        addChild(lb, 1);
        lb->setAnchorPoint(CCPointMake(0.5, 0.0));
        lb->setPositionInPixels(m_curErasePos);
        m_lbComboList.push_back(lb);
        lb->setScale(1.5);
        lb->runAction(CCScaleTo::actionWithDuration(0.1, 1));
        lb->runAction(CCRepeatForever::actionWithAction((CCActionInterval*) CCBlink::actionWithDuration(1, 5)));
        
    }
    
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
    
    schedule(schedule_selector(StoneGrid::erase), 0.5);
    
    
    (m_pListener->*m_pfnSelector)();
}

//******************************************************************************
// drop
//******************************************************************************
void StoneGrid::showSkillEffect(CCPoint &point,float fEffect,enElementDef emElement/* = enElementMax*/)
{
    // combo数/攻击属性数 触发技能效果
    if (fEffect > 1.1)
    {
        char buf[10];
        snprintf(buf, 9, "x%.2f", (ceil(fEffect*100))*0.01);
        TextNode* lb = TextNode::textWithString(buf, 24);
        lb->setAnchorPoint(ccp(0.5, 1.0));
        if (60>point.x) point.x += 14;
        else if (580<point.x)point.x += 5;
        if (m_fCurErasePlus > 1.0)
        {
            lb->setPositionInPixels(CCPointMake(point.x,
                                                point.y - 25));
        }
        else
        {
            lb->setPositionInPixels(point);
        }
        if (580<=point.x)point.x -= 110;
        else point.x += 110;
        
        char chBuf[32];
        sprintf(chBuf, "stone%ds.png", emElement);
        if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName(chBuf))
        {
            pSprite->setScale(0.9f);
            pSprite->setAnchorPoint(ccp(1.0f,0.5f));
            pSprite->setPosition(ccp(-2.0f,12.5f));
            lb->addNodeChild(pSprite);
        }
        
        addChild(lb, 1, kPlusLbTagBase + m_iCombo);
        lb->setShadowColor(ccBLACK);
        lb->setScale(1.5);
        lb->runAction(CCScaleTo::actionWithDuration(0.1, 1));
    }
}

//******************************************************************************
// drop
//******************************************************************************
void StoneGrid::drop(bool autoClear)
{
    PStoneNode node;
    CCPoint ptDest;
    CCPoint ptSrc;
    
    int k = 0;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        k = 0;
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].bAlive == false)
            {
                if(m_stoneNodeArray[j][i].spStone)
                {
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
                    if((node != NULL) && (node->bAlive == true))
                    {
                        m_stoneNodeArray[j][i].spStone = node->spStone;
                        node->spStone = NULL;
                        m_stoneNodeArray[j][i].iElement = node->iElement;
                        m_stoneNodeArray[j][i].iPlus = node->iPlus;
                        m_stoneNodeArray[j][i].bAlive = true;
                        m_stoneNodeArray[j][i].x = i;
                        m_stoneNodeArray[j][i].y = j;
                        
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
                
                if(node == NULL)
                {
                    if(((m_iNewbieStep >= 1) && (m_iNewbieStep <= 2))
                       || (m_iNewbieStep == 5))
                    {
                        m_stoneNodeArray[j][i].iElement = i;
                    }
                    else if(m_iNewbieStep == 3)
                    {
                        m_stoneNodeArray[j][i].iElement = randElement();
                    }
                    else if(m_iNewbieStep == 4)
                    {
                        m_stoneNodeArray[j][i].iElement = randElement();
                    }
                    else
                    {
                        m_stoneNodeArray[j][i].iElement = randElement();
                    }
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(m_stoneNodeArray[j][i].iElement);
                    m_stoneNodeArray[j][i].bAlive = true;
                    
                    Player* pPlayer = (Player*)m_pListener;
                    if ((m_stoneNodeArray[j][i].iElement < enElementInvalid)
                        && (m_stoneNodeArray[j][i].iElement > enElementHeart)
                        && (SaveData::Inst()->getrand() <   getOddsAwake10(0.2f, (float)(pPlayer->getNumByAwakeId(m_stoneNodeArray[j][i].iElement + enAwakeSkill_9)))))
                        m_stoneNodeArray[j][i].iPlus = 1;
                    else
                        m_stoneNodeArray[j][i].iPlus = 0;
        
                    m_stoneNodeArray[j][i].iBlackCover = 0;
                    m_stoneNodeArray[j][i].iParam1 = 0;
                    m_stoneNodeArray[j][i].iParam2 = 0;
                    m_stoneNodeArray[j][i].x = i;
                    m_stoneNodeArray[j][i].y = j;
                    
                    ptSrc.x = ptDest.x;
                    ptSrc.y = gPtStart.y + (kMaxGridRowCnt + k) * gfSpSize;
                    if(m_stoneNodeArray[j][i].spStone)
                    {
                        if (m_stoneNodeArray[j][i].iPlus == 1) {
                            CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                            if (spPlus) {
                                m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                                spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                                        m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                            }
                        }
                        
                        m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                        m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.15, ptDest));
                    }
                    k++;
                }
            }
        }
    }
    if (autoClear) {
        schedule(schedule_selector(StoneGrid::dropCb), 0.25);
    }else{
        schedule(schedule_selector(StoneGrid::chgState), 0.25);
    }
}

void StoneGrid::onlyDrop()
{
    unschedule(schedule_selector(StoneGrid::onlyDrop));
    PStoneNode node;
    CCPoint ptDest;
    CCPoint ptSrc;
    
    int k = 0;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        k = 0;
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].bAlive == false)
            {
                if(m_stoneNodeArray[j][i].spStone)
                {
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
                        m_stoneNodeArray[j][i].iPlus = node->iPlus;
                        m_stoneNodeArray[j][i].bAlive = true;
                        m_stoneNodeArray[j][i].iParam1 = node->iParam1;
                        m_stoneNodeArray[j][i].iParam2 = node->iParam2;
                        m_stoneNodeArray[j][i].x = i;
                        m_stoneNodeArray[j][i].y = j;
                        
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
                
                if(node == NULL)
                {

                    m_stoneNodeArray[j][i].iElement = randElement();
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(m_stoneNodeArray[j][i].iElement);
                    m_stoneNodeArray[j][i].bAlive = true;
                    
                    Player* pPlayer = (Player*)m_pListener;
                    if ((m_stoneNodeArray[j][i].iElement < enElementInvalid)
                        && (m_stoneNodeArray[j][i].iElement > enElementHeart)
              && (SaveData::Inst()->getrand() <   getOddsAwake10(0.2f, (float)(pPlayer->getNumByAwakeId(m_stoneNodeArray[j][i].iElement + enAwakeSkill_9)))))
                        m_stoneNodeArray[j][i].iPlus = 1;
                    else
                        m_stoneNodeArray[j][i].iPlus = 0;
     
                    m_stoneNodeArray[j][i].iBlackCover = 0;
                    m_stoneNodeArray[j][i].iParam1 = 0;
                    m_stoneNodeArray[j][i].iParam2 = 0;
                    m_stoneNodeArray[j][i].x = i;
                    m_stoneNodeArray[j][i].y = j;
                    
                    ptSrc.x = ptDest.x;
                    ptSrc.y = gPtStart.y + (kMaxGridRowCnt + k) * gfSpSize;
                    if(m_stoneNodeArray[j][i].spStone)
                    {
                        if (m_stoneNodeArray[j][i].iPlus == 1) {
                            CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                            if (spPlus) {
                                m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                                spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                                        m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                            }
                        }
                        
                        m_stoneNodeArray[j][i].spStone->setPositionInPixels(ptSrc);
                        m_stoneNodeArray[j][i].spStone->runAction(CCMoveTo::actionWithDuration(0.15, ptDest));
                    }
                    k++;
                }
            }
        }
    }
}

void StoneGrid::chgState(ccTime dt){
    unschedule(schedule_selector(StoneGrid::chgState));
    CGameState::Instance()->setState(enStateWaitTouch);
    //(m_pListener->*m_pfnSelector)();
}


//******************************************************************************
// dropCb
//******************************************************************************
void StoneGrid::dropCb(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::dropCb));
    
    if(srchGrp())
        erase(0);
    else {
        CGameState::Instance()->setState(enStateClrCombo);
        
        for(int i = 0; i < kMaxGridColumnCnt; i ++)
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone){
                    m_stoneNodeArray[j][i].spStone->setOpacity(100);
                    
                    CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                    if (spPlus) {
                        ((CCSprite*)spPlus)->setOpacity(100);
                    }
                }
            }
        
        
        m_curLb = NULL;
        m_curLb2 = NULL;
        m_iPlusLbIdx = 0;
        clrCombo();
    }
}


//******************************************************************************
// clrCombo
//******************************************************************************
void StoneGrid::clrCombo()
{
    unschedule(schedule_selector(StoneGrid::clrCombo));

//plus
    m_iPlusLbIdx++;
    
    CCNode* nodePlus = getChildByTag(kPlusLbTagBase + m_iPlusLbIdx);
    if (m_iPlusLbIdx == 1) {
        if (nodePlus) {
            nodePlus->setTag(kPlusLbTagBase);
            
            ((TextNode*)nodePlus)->runActionInside(CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.5),
                                                                   CCFadeOut::actionWithDuration(0.1),
                                                                   NULL));
            schedule(schedule_selector(StoneGrid::clrCombo), 0.5);
            return;
        }
        else{
            m_iPlusLbIdx++;
        }
    }
    
    // 根据combo数或者消除宝石属性数来对攻击力加成的技能，如果主将和友军加成的属性不相同，（kPlusLbTagBase + m_iPlusLbIdx）这个tag对应的node可能不止2个
    while (getChildByTag(kPlusLbTagBase + m_iPlusLbIdx)) {
        nodePlus = getChildByTag(kPlusLbTagBase + m_iPlusLbIdx);
        if (nodePlus) {
            nodePlus->setTag(kPlusLbTagBase);
            
            ((TextNode*)nodePlus)->runActionInside(CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.5),
                                                                       CCFadeOut::actionWithDuration(0.1),
                                                                       NULL));
        }
    }
    
//combo
    if(m_curLb != NULL){
        removeChild(m_curLb, true);
        m_curLb = NULL;
    }
    
    if(m_curLb2 != NULL){
        removeChild(m_curLb2, true);
        m_curLb2 = NULL;
    }
    
    if(m_lbComboList.size() == 0){                
        CGameState::Instance()->setState(enStatePlayerAttackPrepared);
        (m_pListener->*m_pfnSelector)();
        return;
    }
    
    m_curLb = m_lbComboList.front();
    reorderChild(m_curLb, 2);
    m_curLb->runAction(CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.5),
                                           CCFadeOut::actionWithDuration(0.1),
                                           CCCallFunc::actionWithTarget(this, callfunc_selector(StoneGrid::clrCombo)),
                                           NULL));
    m_lbComboList.pop_front();
    
    m_curLb2 = m_lbComboList.front();
    reorderChild(m_curLb2, 2);
    m_curLb2->runAction(CCSequence::actions(CCScaleTo::actionWithDuration(0.4, 1.5),
                                            CCFadeOut::actionWithDuration(0.1),
                                            NULL));
    m_lbComboList.pop_front();
    
    (m_pListener->*m_pfnSelector)();
}

//******************************************************************************
// getCurEraseCnt
//******************************************************************************
int StoneGrid::getCurEraseCnt()
{
    return m_curEraseCnt;
}

//******************************************************************************
// getCurErasePos
//******************************************************************************
CCPoint& StoneGrid::getCurErasePos()
{
    return m_curErasePos;
}

//******************************************************************************
// getCurEraseElement
//******************************************************************************
int StoneGrid::getCurEraseElement()
{
    return m_curEraseElement;
}

//******************************************************************************
// getCurErasePlus
//******************************************************************************
float StoneGrid::getCurErasePlus()
{
    return m_fCurErasePlus;
}

float StoneGrid::checkStoneGrid()
{
    CCAnimation* animation;
    CCActionInterval*  action;
    char buf[100];
    CCPoint pt;
    
    int atkPt = 0;
    
    bool needDrop = false;
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if (m_stoneNodeArray[j][i].iParam1 == enSpecialStoneBomb)
            {
                m_stoneNodeArray[j][i].iParam2--;
                
                if (m_stoneNodeArray[j][i].iParam2 <= 0)
                {
                    pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                    m_stoneNodeArray[j][i].iParam1 = 0;
                    m_stoneNodeArray[j][i].iParam2 = 0;
                    m_stoneNodeArray[j][i].spStone->removeFromParentAndCleanup(true);
                    m_stoneNodeArray[j][i].spStone = NULL;
                    m_stoneNodeArray[j][i].bAlive = false;
                    
                    needDrop = true;
                    
                    atkPt += 1000;
                    
                    animation = CCAnimation::animation();
                    for( int i=1;i<=7;i++)
                    {
                        sprintf(buf, "zhuzbao000%d.png", i);
                        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                        animation->addFrame(frame);
                    }
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                    animation->addFrame(frame);
                    action = CCAnimate::actionWithDuration(0.6, animation, false);
                    
                    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("zhuzbao0001.png");
                    spAnim->setPositionInPixels(pt);
                    spAnim->setScale(2.0);
                    addChild(spAnim, 2);
                    spAnim->runAction(action);
                    m_tmpObjLst.push_back(spAnim);
                }
                else
                {
                    CCSprite* spNum = (CCSprite*)m_stoneNodeArray[j][i].spStone->getChildByTag(20130928);
                    if (spNum)
                        spNum->removeFromParentAndCleanup(true);
                    
                    sprintf(buf, "bombstone%d.png", m_stoneNodeArray[j][i].iParam2);
                    spNum = CCSprite::spriteWithSpriteFrameName(buf);
                    if (spNum)
                    {
                        spNum->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.8f,m_stoneNodeArray[j][i].spStone->getContentSize().height*0.76f));
                        m_stoneNodeArray[j][i].spStone->addChild(spNum,1,20130928);
                    }
                }
            }
        }
    }
    
    if (needDrop)
    {
        Player* pPlayer = (Player*)m_pListener;
        CAttackInfo atk;
        atk.clear();
        atk.iElement = enElementZero;
        atk.iAttackPt = atkPt;
        pPlayer->beDirAtk(&atk);
        
        schedule(schedule_selector(StoneGrid::cbChgStone), 0.6);
        if(CGameData::Inst()->isSoundOn)
        {
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
        }
        
        schedule(schedule_selector(StoneGrid::onlyDrop), 0.3);
        
        return 1.0f;
    }
    
    return 0.0f;
}

void StoneGrid::addStoneSpecial(int src, int Type)
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].iElement == src)
            {
                switch (Type)
                {
                    case enSpecialStoneBomb:
                    if (m_stoneNodeArray[j][i].iParam1 != enSpecialStoneBomb)
                    {
                        m_stoneNodeArray[j][i].iParam1 = Type;
                        m_stoneNodeArray[j][i].iParam2 = 3;
                        
                        CCSprite* spNum = CCSprite::spriteWithSpriteFrameName("bombstone3.png");
                        spNum->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.8f,m_stoneNodeArray[j][i].spStone->getContentSize().height*0.76f));
                        m_stoneNodeArray[j][i].spStone->addChild(spNum,1,20130928);
                        
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
    

}

void StoneGrid::chgStoneNumberFromEnemy(int num,int dst,bool plus)
{
    CGameState::Instance()->setState(enStateChgStone);
    CCAnimation* animation;
    CCActionInterval*  action;
    int tt  = 0;
   
    
    char buf[100];
    if (plus) {
        
        int coutt = 0;
        for(int i = 0; i < kMaxGridColumnCnt; i ++)
        {
            for(int j = 0; j < kMaxGridRowCnt; j ++)
            {
                if ( m_stoneNodeArray[j][i].iPlus != 1 && m_stoneNodeArray[j][i].iElement.get()< enElementInvalid ) {
                    coutt++;
                }
            }
        }
        if (coutt <= 6) {
            for(int i = 0; i < kMaxGridColumnCnt; i ++)
            {
                for(int j = 0; j < kMaxGridRowCnt; j ++)
                {
                    if (m_stoneNodeArray[j][i].iElement.get()< enElementInvalid && m_stoneNodeArray[j][i].iPlus != 1) {
                        m_stoneNodeArray[j][i].iPlus = 1;
                        CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                        if (spPlus) {
                            m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                            spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                                    m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                    }
                }
            }
        }
                schedule(schedule_selector(StoneGrid::cbChgStoneEnemy), 1.0);
                if(CGameData::Inst()->isSoundOn)
                    SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
                return;
        }
        
        do {
            
            int random_hor = SaveData::Inst()->getrand()%6;
            int random_row = SaveData::Inst()->getrand()%5;
          if (m_stoneNodeArray[random_row][random_hor].iElement.get()< enElementInvalid && m_stoneNodeArray[random_row][random_hor].iPlus == 0 ){
                m_stoneNodeArray[random_row][random_hor].iPlus = 1;
                    CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                    if (spPlus) {
                        m_stoneNodeArray[random_row][random_hor].spStone->addChild(spPlus, 0, kStnPlusTag);
                        spPlus->setPosition(ccp(m_stoneNodeArray[random_row][random_hor].spStone->getContentSize().width*0.7,
                                                m_stoneNodeArray[random_row][random_hor].spStone->getContentSize().width*0.3));
                }

                tt++;
            }
        } while (tt < num);
    
        
    }else{
        do {
            
            CCPoint pt;
            int random_hor = SaveData::Inst()->getrand()%6;
            int random_row = SaveData::Inst()->getrand()%5;
           if (m_stoneNodeArray[random_row][random_hor].iElement.get() < enElementInvalid){
           
                if(m_stoneNodeArray[random_row][random_hor].spStone)
                {
                    
                    reorderChild(m_stoneNodeArray[random_row][random_hor].spStone, 1);
                    m_stoneNodeArray[random_row][random_hor].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                    m_tmpObjLst.push_back(m_stoneNodeArray[random_row][random_hor].spStone);
                    pt = m_stoneNodeArray[random_row][random_hor].spStone->getPositionInPixels();
                }
                
                int tempOpacity =m_stoneNodeArray[random_row][random_hor].spStone->getOpacity();
                
                m_stoneNodeArray[random_row][random_hor].spStone = getSpriteByElement(dst);
                if(m_stoneNodeArray[random_row][random_hor].spStone)
                {
                    m_stoneNodeArray[random_row][random_hor].spStone->setPositionInPixels(pt);
                    m_stoneNodeArray[random_row][random_hor].spStone->setOpacity(0);
                    m_stoneNodeArray[random_row][random_hor].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                                    CCFadeTo::actionWithDuration(0.5 , tempOpacity),
                                                                                                    NULL));
                }
                
                m_stoneNodeArray[random_row][random_hor].iElement = dst;
                
                animation = CCAnimation::animation();
                for( int i=1;i<12;i++)
                {
                    sprintf(buf, "stonechg00%02d.png", i);
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                    animation->addFrame(frame);
                }
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                animation->addFrame(frame);
                action = CCAnimate::actionWithDuration(1.0, animation, false);
                
                CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                spAnim->setPositionInPixels(pt);
                spAnim->setScale(2.0);
                addChild(spAnim, 2);
                spAnim->runAction(action);
                m_tmpObjLst.push_back(spAnim);
                tt++;
            }
            
        } while (tt < num);

    }
    
    
    
     schedule(schedule_selector(StoneGrid::cbChgStoneEnemy), 1.0);
     if(CGameData::Inst()->isSoundOn)
    SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));




}
//chgStoneFromEnemy
void StoneGrid::chgStoneFromEnemy(int src, int dst)
{
    CGameState::Instance()->setState(enStateChgStone);
    CCAnimation* animation;
    CCActionInterval*  action;
    char buf[100];
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].iElement == src)
            {
                CCPoint pt;
                
                if(m_stoneNodeArray[j][i].spStone)
                {
                    reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                    m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                    m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                    pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                }
                
                int tempOpacity =m_stoneNodeArray[j][i].spStone->getOpacity();
                
                m_stoneNodeArray[j][i].spStone = getSpriteByElement(dst);
                if(m_stoneNodeArray[j][i].spStone)
                {
                    m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                    m_stoneNodeArray[j][i].spStone->setOpacity(0);
                    m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                  CCFadeTo::actionWithDuration(0.5 , tempOpacity ),
                                                                                  NULL));
                }
                
                m_stoneNodeArray[j][i].iElement = dst;
                
                animation = CCAnimation::animation();
                for( int i=1;i<12;i++)
                {
                    sprintf(buf, "stonechg00%02d.png", i);
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                    animation->addFrame(frame);
                }
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                animation->addFrame(frame);
                action = CCAnimate::actionWithDuration(1.0, animation, false);
                
                CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                spAnim->setPositionInPixels(pt);
                spAnim->setScale(2.0);
                addChild(spAnim, 2);
                spAnim->runAction(action);
                m_tmpObjLst.push_back(spAnim);
            }
        }
    }
    
   schedule(schedule_selector(StoneGrid::cbChgStoneEnemy), 1.0);
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
    }
    
//    for (TmpObjIter it = m_tmpObjLst.begin(); it != m_tmpObjLst.end(); ++it){
//        CCNode* pObj = *it;
//        
//        removeChild(pObj, true);
//    }
//    
//    m_tmpObjLst.clear();
    
}

//StoneShowBlack()
void StoneGrid::StoneShowBlack()
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].spStone)
            {
                m_stoneNodeArray[j][i].spStone->setColor(ccBLACK);
                m_stoneNodeArray[j][i].iBlackCover = 1;
            }
        }
    }
}

void StoneGrid::stoneChangeToLight()
{
    CGameState::Instance()->setState(enStateChgStone);
    CCAnimation* animation;
    CCActionInterval*  action;
    char buf[100];
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if(m_stoneNodeArray[j][i].iElement != enElementHeart && m_stoneNodeArray[j][i].iElement != enElementLight)
            {
                CCPoint pt;
                
                if(m_stoneNodeArray[j][i].spStone)
                {
                    reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                    m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                    m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                    pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                }
                
                int tempOpacity =m_stoneNodeArray[j][i].spStone->getOpacity();
                
                m_stoneNodeArray[j][i].spStone = getSpriteByElement(enElementLight);
                if(m_stoneNodeArray[j][i].spStone)
                {
                    m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                    m_stoneNodeArray[j][i].spStone->setOpacity(0);
                    m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                  CCFadeTo::actionWithDuration(0.5 , tempOpacity ),
                                                                                  NULL));
                }
                
                m_stoneNodeArray[j][i].iElement = enElementLight;
                
                animation = CCAnimation::animation();
                for( int i=1;i<12;i++)
                {
                    sprintf(buf, "stonechg00%02d.png", i);
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                    animation->addFrame(frame);
                }
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                animation->addFrame(frame);
                action = CCAnimate::actionWithDuration(1.0, animation, false);
                
                CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                spAnim->setPositionInPixels(pt);
                spAnim->setScale(2.0);
                addChild(spAnim, 2);
                spAnim->runAction(action);
                m_tmpObjLst.push_back(spAnim);
            }
        }
    }
    
    schedule(schedule_selector(StoneGrid::cbChgStoneEnemy), 1.0);
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
    }
}

//******************************************************************************
// changeCaiDanStone
//******************************************************************************
void StoneGrid::changeCaiDanStone()
{
    CGameState::Instance()->setState(enStateChgStone);
    
    CCAnimation* animation;
    CCActionInterval*  action;
    char buf[100];
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            CCPoint pt;
            
            if(m_stoneNodeArray[j][i].spStone){
                reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
            }
            
            m_stoneNodeArray[j][i].spStone = getSpriteByElement(m_stoneNodeArray[j][i].iElement);
            if(m_stoneNodeArray[j][i].spStone){
                m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                m_stoneNodeArray[j][i].spStone->setOpacity(0);
                m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                              CCFadeIn::actionWithDuration(0.5),
                                                                              NULL));
                
                if (m_stoneNodeArray[j][i].iPlus == 1) {
                    CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                    if (spPlus) {
                        m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                        spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                                m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                    }
                }
                
                if (m_stoneNodeArray[j][i].iBlackCover == 1)
                {
                    m_stoneNodeArray[j][i].spStone->setColor(ccBLACK);
                }
            }
                        
            animation = CCAnimation::animation();
            for( int i=1;i<12;i++){
                sprintf(buf, "stonechg00%02d.png", i);
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                animation->addFrame(frame);
            }
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
            animation->addFrame(frame);
            action = CCAnimate::actionWithDuration(1.0, animation, false);
            
            CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
            spAnim->setPositionInPixels(pt);
            spAnim->setScale(2.0);
            addChild(spAnim, 2);
            spAnim->runAction(action);
            m_tmpObjLst.push_back(spAnim);
        }
    }
    
    schedule(schedule_selector(StoneGrid::cbChgStone), 1.0);
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
    
}

//******************************************************************************
// chgStone
//******************************************************************************
void StoneGrid::chgStone(int src, int dst)
{
    CGameState::Instance()->setState(enStateChgStone);
    
    //寶石刷新
    if((src & 0x0F) > enElementMax){
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone)
                    m_stoneNodeArray[j][i].spStone->runAction(CCFadeOut::actionWithDuration(0.3));
            }
        }
        
        schedule(schedule_selector(StoneGrid::cbClrAllStones), 0.3);
    }
    //寶石變化
    else
    {
        CCAnimation* animation;
        CCActionInterval*  action;
        char buf[100];
        
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].iElement == (src & 0x0F)
                   || (enElementMax == (src & 0x0F) && m_stoneNodeArray[j][i].iElement != (dst & 0x0F))
                   
                   )
                {
                    CCPoint pt;
                    
                    if(m_stoneNodeArray[j][i].spStone){
                        reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                        m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                        m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                        pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                        m_stoneNodeArray[j][i].iPlus = 0;
                    }
                    
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(dst & 0x0F);
                    if(m_stoneNodeArray[j][i].spStone){
                        m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                        m_stoneNodeArray[j][i].spStone->setOpacity(0);
                        m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                      CCFadeIn::actionWithDuration(0.5),
                                                                                      NULL));
                    }
                    
                    m_stoneNodeArray[j][i].iElement = (dst & 0x0F);
                    
                    animation = CCAnimation::animation();
                    for( int i=1;i<12;i++){
                        sprintf(buf, "stonechg00%02d.png", i);
                        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                        animation->addFrame(frame);
                    }
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                    animation->addFrame(frame);
                    action = CCAnimate::actionWithDuration(1.0, animation, false);
                    
                    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                    spAnim->setPositionInPixels(pt);
                    spAnim->setScale(2.0);
                    addChild(spAnim, 2);
                    spAnim->runAction(action);
                    m_tmpObjLst.push_back(spAnim);
                }
                else if(m_stoneNodeArray[j][i].iElement == ((src >> 4) & 0x0F)) {
                    CCPoint pt;
                    
                    if(m_stoneNodeArray[j][i].spStone){
                        reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                        m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                        m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                        pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                        m_stoneNodeArray[j][i].iPlus = 0;
                    }
                    
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement((dst >> 4) & 0x0F);
                    if(m_stoneNodeArray[j][i].spStone){
                        m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                        m_stoneNodeArray[j][i].spStone->setOpacity(0);
                        m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                      CCFadeIn::actionWithDuration(0.5),
                                                                                      NULL));
                    }
                    
                    m_stoneNodeArray[j][i].iElement = ((dst >> 4) & 0x0F);
                    
                    animation = CCAnimation::animation();
                    for( int i=1;i<12;i++){
                        sprintf(buf, "stonechg00%02d.png", i);
                        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                        animation->addFrame(frame);
                    }
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                    animation->addFrame(frame);
                    action = CCAnimate::actionWithDuration(1.0, animation, false);
                    
                    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                    spAnim->setPositionInPixels(pt);
                    spAnim->setScale(2.0);
                    addChild(spAnim, 2);
                    spAnim->runAction(action);
                    m_tmpObjLst.push_back(spAnim);
                }
                
            }
        }
        
        schedule(schedule_selector(StoneGrid::cbChgStone), 1.0);
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
    }
}

//******************************************************************************
// chgStone2
//******************************************************************************
void StoneGrid::chgStone2(int src, int dst)
{
    CGameState::Instance()->setState(enStateChgStone);

    CCAnimation* animation;
    CCActionInterval*  action;
    char buf[100];
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(m_stoneNodeArray[j][i].iElement == (src & 0x0F)
               || (enElementMax == (src & 0x0F))
               
               )
            {
                int random = SaveData::Inst()->getrand()%3;
                // 所有宝珠变化为两种类型得宝珠，而不是三种
                Player* pPlayer = (Player*)m_pListener;
                if (pPlayer->getCurManSkillID() == enManSkill_278 ||
                    pPlayer->getCurManSkillID() == enManSkill_281 ||
                    pPlayer->getCurManSkillID() == enManSkill_283 ||
                    pPlayer->getCurManSkillID() == enManSkill_285)
                    random = SaveData::Inst()->getrand()%2;
                
                CCPoint pt;
                
                if(m_stoneNodeArray[j][i].spStone){
                    reorderChild(m_stoneNodeArray[j][i].spStone, 1);
                    m_stoneNodeArray[j][i].spStone->runAction(CCScaleTo::actionWithDuration(0.5, 0.0));
                    m_tmpObjLst.push_back(m_stoneNodeArray[j][i].spStone);
                    pt = m_stoneNodeArray[j][i].spStone->getPositionInPixels();
                    m_stoneNodeArray[j][i].iPlus = 0;
                }
                if (random == 0) {
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement(dst & 0x0F);
                    m_stoneNodeArray[j][i].iElement = (dst & 0x0F);
                }else if (random == 1){
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement((dst >> 4) & 0x0F);
                    m_stoneNodeArray[j][i].iElement = ((dst >> 4) & 0x0F);
                }else{
                    m_stoneNodeArray[j][i].spStone = getSpriteByElement((dst >> 8) & 0x0F);
                    m_stoneNodeArray[j][i].iElement = ((dst >> 8) & 0x0F);
                }
                
                if(m_stoneNodeArray[j][i].spStone){
                    m_stoneNodeArray[j][i].spStone->setPositionInPixels(pt);
                    m_stoneNodeArray[j][i].spStone->setOpacity(0);
                    m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                                                  CCFadeIn::actionWithDuration(0.5),
                                                                                  NULL));
                }
                
                animation = CCAnimation::animation();
                for( int i=1;i<12;i++){
                    sprintf(buf, "stonechg00%02d.png", i);
                    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
                    animation->addFrame(frame);
                }
                CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
                animation->addFrame(frame);
                action = CCAnimate::actionWithDuration(1.0, animation, false);
                
                CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("stonechg0001.png");
                spAnim->setPositionInPixels(pt);
                spAnim->setScale(2.0);
                addChild(spAnim, 2);
                spAnim->runAction(action);
                m_tmpObjLst.push_back(spAnim);
            }
            
        }
    }
    
    schedule(schedule_selector(StoneGrid::cbChgStone), 1.0);
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(stonechg));
}

//******************************************************************************
// clearStone
//******************************************************************************
int StoneGrid::clearStone(int src, int dst)
{
    CGameState::Instance()->setState(enStateTouchEnd);
    char buf[100];
    int clearCounts=0;
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(enElementOther == dst){
                if (m_stoneNodeArray[j][i].iElement != src) {
                    clearCounts++;
                    CCPoint pt;
                    if(m_stoneNodeArray[j][i].spStone){
                        m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(
                                                                                      CCDelayTime::actionWithDuration(0.1),
                                                                                      CCScaleTo::actionWithDuration(0.2, 0.5),
                                                                                      CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.1, 1.0),
                                                                                                            CCFadeOut::actionWithDuration(0.2)),
                                                                                      NULL));
                        
                        if (m_stoneNodeArray[j][i].iElement >= enElementHeart && m_stoneNodeArray[j][i].iElement <= enElementInvalid)
                        {
                            snprintf(buf, 49, "guangquan_%d.png", m_stoneNodeArray[j][i].iElement.get());
                            CCSprite* spGuangQuan = CCSprite::spriteWithSpriteFrameName(buf);
                            spGuangQuan->setPosition(m_stoneNodeArray[j][i].spStone->getPosition());
                            addChild(spGuangQuan, -1, kGuangQuanTag);
                            
                            spGuangQuan->runAction(CCSequence::actions(
                                                                       CCDelayTime::actionWithDuration(0.1),
                                                                       CCScaleTo::actionWithDuration(0.2, 0.9),
                                                                       CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.2, 1.4),
                                                                                             CCFadeOut::actionWithDuration(0.2)),
                                                                       NULL));
                        }
                        
                        showEraseAnim(m_stoneNodeArray[j][i].iElement, m_stoneNodeArray[j][i].spStone->getPosition());
                        
                        CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                        if (spPlus) {
                            spPlus->runAction(CCFadeOut::actionWithDuration(0.5));
                        }
                    }
                    m_stoneNodeArray[j][i].bAlive = false;
                }
            }else{
                if(m_stoneNodeArray[j][i].iElement == dst) {
                    clearCounts++;
                    CCPoint pt;
                    if(m_stoneNodeArray[j][i].spStone){
                        m_stoneNodeArray[j][i].spStone->runAction(CCSequence::actions(
                                                                                      CCDelayTime::actionWithDuration(0.1),
                                                                                      CCScaleTo::actionWithDuration(0.2, 0.5),
                                                                                      CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.1, 1.0),
                                                                                                            CCFadeOut::actionWithDuration(0.2)),
                                                                                      NULL));
                        
                        if (m_stoneNodeArray[j][i].iElement >= enElementHeart && m_stoneNodeArray[j][i].iElement <= enElementInvalid)
                        {
                            snprintf(buf, 49, "guangquan_%d.png", m_stoneNodeArray[j][i].iElement.get());
                            CCSprite* spGuangQuan = CCSprite::spriteWithSpriteFrameName(buf);
                            spGuangQuan->setPosition(m_stoneNodeArray[j][i].spStone->getPosition());
                            addChild(spGuangQuan, -1, kGuangQuanTag);
                            
                            spGuangQuan->runAction(CCSequence::actions(
                                                                       CCDelayTime::actionWithDuration(0.1),
                                                                       CCScaleTo::actionWithDuration(0.2, 0.9),
                                                                       CCSpawn::actionOneTwo(CCScaleTo::actionWithDuration(0.2, 1.4),
                                                                                             CCFadeOut::actionWithDuration(0.2)),
                                                                       NULL));
                        }
                        
                        showEraseAnim(m_stoneNodeArray[j][i].iElement, m_stoneNodeArray[j][i].spStone->getPosition());
                        
                        CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                        if (spPlus) {
                            spPlus->runAction(CCFadeOut::actionWithDuration(0.5));
                        }
                    }
                    m_stoneNodeArray[j][i].bAlive = false;
                    
                }

            }
            
        }
    }
    schedule(schedule_selector(StoneGrid::dropDelay), 0.5);
    return clearCounts;
}


void StoneGrid::dropDelay(ccTime dt){
    unschedule(schedule_selector(StoneGrid::dropDelay));
    drop(false);
}


//******************************************************************************
// chgStone
//******************************************************************************
void StoneGrid::cbClrAllStones(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::cbClrAllStones));
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            removeChild(m_stoneNodeArray[j][i].spStone, true);
            m_stoneNodeArray[j][i].spStone = NULL;
        }
    }
    
    initMatrixNml(true);
    
    schedule(schedule_selector(StoneGrid::cbClrAllStonesDrop), 0.3);
}

//******************************************************************************
// cbClrAllStonesDrop
//******************************************************************************
void StoneGrid::cbClrAllStonesDrop(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::cbClrAllStonesDrop));
    
    if(srchGrp()){
        CGameState::Instance()->setState(enStateTouchEnd);
        erase(0);
    }
    else{
        CGameState::Instance()->setState(enStateWaitTouch);
    }
}

//******************************************************************************
// cbChgStone
//******************************************************************************

void    StoneGrid::cbChgStoneEnemy(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::cbChgStoneEnemy));
    
    CCLog("%d" , CGameState::Instance()->getState());
    
//    if (CGameState::Instance()->getState() != enStateGameOver && CGameState::Instance()->getState() != enStateChgStone) {
//        CGameState::Instance()->setState(enStateWaitTouch);
//    }
    
    for (TmpObjIter it = m_tmpObjLst.begin(); it != m_tmpObjLst.end(); ++it){
        CCNode* pObj = *it;
        
        removeChild(pObj, true);
    }
    
    m_tmpObjLst.clear();
}

void StoneGrid::cbChgStone(ccTime dt)
{
    unschedule(schedule_selector(StoneGrid::cbChgStone));
    
//    CCLog("%d" , CGameState::Instance()->getState());
    
    if (CGameState::Instance()->getState() != enStateGameOver ) {
        CGameState::Instance()->setState(enStateWaitTouch);
    }
    
    for (TmpObjIter it = m_tmpObjLst.begin(); it != m_tmpObjLst.end(); ++it){
        CCNode* pObj = *it;
        
        removeChild(pObj, true);
    }
    
    m_tmpObjLst.clear();
}


//******************************************************************************
// haveStone
//******************************************************************************
bool StoneGrid::haveStone(int src)
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if( m_stoneNodeArray[j][i].iElement == (src & 0x0F)
               || m_stoneNodeArray[j][i].iElement == ((src >> 4) & 0x0F)
               || enElementMax == (src & 0x0F)
               || m_stoneNodeArray[j][i].iElement == ((src >> 4) & 0x0F)){
                return true;
            }
        }
    }
    return false;
}

bool StoneGrid::haveStone(int src, int dst){
    if (enElementOther == dst) {
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].iElement != src){
                    return true;
                }
            }
        }
    }else{
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].iElement == dst){
                    return true;
                }
            }
        }
    }
    return false;
}

int StoneGrid::getOneBeElement(int nNotElement)const
{
    int nElement = -1;
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            if (nNotElement !=m_stoneNodeArray[j][i].iElement.get() && m_stoneNodeArray[j][i].iElement.get()<=enElementDark)
            {
                nElement = m_stoneNodeArray[j][i].iElement.get();
            }
        }
    }
    return nElement;
}

//******************************************************************************
// setHand
//******************************************************************************
void StoneGrid::setHand(const CCPoint& pos, float fPercent)
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
void StoneGrid::touchEndProc()
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++)
    {
        for(int j = 0; j < kMaxGridRowCnt; j ++)
        {
            m_stoneNodeArray[j][i].x = i;
            m_stoneNodeArray[j][i].y = j;
        }
    }
    
    if(m_spHold != NULL){
        
        CCPoint nodePos = m_spHold->getPosition();
        int iColumn, iRow;
        iColumn = (nodePos.x - (gPtStart.x - gfSpSize / 2)) / gfSpSize;
        iRow = (nodePos.y - (gPtStart.y - gfSpSize / 2)) / gfSpSize;
        
        m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setOpacity(255);
        
        CCNode* spPlus = m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getChildByTag(kStnPlusTag);
        if (spPlus) {
            ((CCSprite*)spPlus)->setOpacity(255);
        }
        
        removeChild(m_spHold, true);
        m_spHold = NULL;
    }
    
    setHand(CCPointMake(-1000, -1000), 1.0);
    unscheduleUpdate();
    
//    //test
//    initGuideMatrix(2);
    //新手引导时，判断撒手的地方是否为指定结束点
    if(m_bStartFlg){
        
//        if (m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover == 1)
//        {
//            m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover = 0;
//            m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setColor(ccWHITE);
//        }
        
        if (m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().r == 0
            && m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().g == 0
            && m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->getColor().b == 0)
        {
            
            
            m_stoneNodeArray[m_iCurRow][m_iCurColumn].iBlackCover = 0;
            m_stoneNodeArray[m_iCurRow][m_iCurColumn].spStone->setColor(ccWHITE);
            
            
            
            
        }
        
        removeArrowLayer();
        
        bool bWrongMov = false;
        
        if(m_iNewbieStep == 0){
        }
        else if(m_iNewbieStep == 1){
            if(gNewbieGrid1[m_iCurRow][m_iCurColumn] & StoneGridMask_End){
                bWrongMov = false;
            }
            else{
                bWrongMov = true;
                m_spNewbieStart->setIsVisible(true);
            }
        }
        else if(m_iNewbieStep == 2){
            if(gNewbieGrid2[m_iCurRow][m_iCurColumn] & StoneGridMask_End){
                bWrongMov = false;
            }
            else{
                bWrongMov = true;
                m_spNewbieStart->setIsVisible(true);
            }
        }
        else if(m_iNewbieStep == 3){
            if(gNewbieGrid3[m_iCurRow][m_iCurColumn] & StoneGridMask_End){
                bWrongMov = false;
                
            }
            else{
                bWrongMov = true;
                
            }
        }
        else if(m_iNewbieStep == 4){
            if(gNewbieGrid4[m_iCurRow][m_iCurColumn] & StoneGridMask_End){
                // bWrongMov = false;
                //判断路径
                if(m_newBie4Through==127)
                    bWrongMov =false;
                else
                    bWrongMov = true;
            }
            else{
                bWrongMov = true;
                m_spNewbieStart->setIsVisible(true);
            }
            m_newBie4Through = 0;
        }
        else if(m_iNewbieStep == 5){
            if(gNewbieGrid5[m_iCurRow][m_iCurColumn] & StoneGridMask_End){
                bWrongMov = false;
                switch (m_iNewbieStep5) {
                    case 0:
                        //                        gNewbieGrid5[4][5] &= StoneGridMask_Element;
                        //                        gNewbieGrid5[4][5] |= StoneGridMask_End;
                        //                        gNewbieGrid5[4][5] |= StoneGridMask_Movable;
                        //
                        //                        gNewbieGrid5[4][4] &= StoneGridMask_Element;
                        //                        gNewbieGrid5[4][4] |= StoneGridMask_Start;
                        //                        gNewbieGrid5[4][4] |= StoneGridMask_Movable;
                        
                        gNewbieGrid5[4][5]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        gNewbieGrid5[4][4]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        gNewbieGrid5[4][5]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        
                        m_pMsgBox = new MsgBox("commdlg.png",
                                               CGameData::Inst()->getLanguageValue("stoneGridTip1"),
                                               ccc3(255, 255, 255));
                        addChild(m_pMsgBox, 10);
                        m_pMsgBox->release();
                        m_pMsgBox->setPosition(CCPointMake(320, 268));
                        m_pMsgBox->setScale(0.0);
                        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                        
                        MainScene::Inst()->showTouchTip();
                        
                        break;
                    case 1:
                        gNewbieGrid5[4][5] &= StoneGridMask_Element;
                        gNewbieGrid5[4][4] &= StoneGridMask_Element;
                        
                        gNewbieGrid5[4][5]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        gNewbieGrid5[4][4]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        gNewbieGrid5[4][5]=  gNewbieGrid5[4][5]^gNewbieGrid5[4][4];
                        
                        
                        gNewbieGrid5[4][2] |= StoneGridMask_End;
                        gNewbieGrid5[4][2] |= StoneGridMask_Movable;
                        gNewbieGrid5[4][3] |= StoneGridMask_Start;
                        gNewbieGrid5[4][3] |= StoneGridMask_Movable;
                        
                        
                        break;
                    case 2:
                        gNewbieGrid5[4][2] &= StoneGridMask_Element;
                        gNewbieGrid5[4][3] &= StoneGridMask_Element;
                        break;
                    default:
                        break;
                }
                
                m_iNewbieStep5++;
            }
            else{
                bWrongMov = true;
                m_spNewbieStart->setIsVisible(true);
            }
        }
        
        if(bWrongMov){
            setHand(CCPointMake(-1000, -1000), 1.0);
            unscheduleUpdate();
            
            initMatrixNewbie(m_iNewbieStep, false);
            CGameState::Instance()->setState(enStateWaitTouch);
            m_bStartFlg = false;
            return;
        }
        
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone){
                    m_stoneNodeArray[j][i].spStone->setOpacity(255);
                    
                    CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                    if (spPlus) {
                        ((CCSprite*)spPlus)->setOpacity(255);
                    }
                }
            }
        }
        
        //非新手
        CGameState::Instance()->setState(enStateTouchEnd);
        
        if (!CGameData::Inst()->isNewbie())
        {
            FightMngr* pFM = (FightMngr*)(getParent()->getParent());
            pFM->saveCurFightInfo(true);
        }
        
        if(m_pArrow){
            removeChild(m_pArrow, true);
            m_pArrow = NULL;
        }
        
        if(m_pLongArrow)
        {
            removeChild(m_pLongArrow, true);
            m_pLongArrow = NULL;
        }

        actAfterMove(0.05f);
        
        //乱世
        if(m_bFreeFlg){
            CCNode* node = getChildByTag(kTimerAnimTag);
            if(node){
                removeChild(node, true);
            }
            SimpleAudioEngine::sharedEngine()->stopAllEffects();
        }
    }
    else {
        if(m_iNewbieStep != 0){
            m_spNewbieStart->setIsVisible(true);
        }
    }
}

void StoneGrid::actAfterMove(float time)
{
   
  
    
    
    if(srchGrp())
        schedule(schedule_selector(StoneGrid::erase), time);//erase(0);
    else{
        for(int i = 0; i < kMaxGridColumnCnt; i ++){
            for(int j = 0; j < kMaxGridRowCnt; j ++){
                if(m_stoneNodeArray[j][i].spStone){
                    m_stoneNodeArray[j][i].spStone->setOpacity(100);
                    
                    CCNode* spPlus = m_stoneNodeArray[j][i].spStone->getChildByTag(kStnPlusTag);
                    if (spPlus) {
                        ((CCSprite*)spPlus)->setOpacity(100);
                    }
                }
            }
        }
        
        CGameState::Instance()->setState(enStateEnemyAttack);
        (m_pListener->*m_pfnSelector)();
    }
}

//******************************************************************************
// isFreeMode
//******************************************************************************
bool StoneGrid::isFreeMode()
{
    return m_bFreeFlg;
}

//******************************************************************************
// ignitFreeMode
//******************************************************************************
void StoneGrid::ignitFreeMode(float fDur)
{
    m_bFreeFlg = true;
    m_fMaxMvDur = fDur;
    
    m_bStartFlg = true;
    
    m_fElapse = 0.0;
    scheduleUpdate();
    
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    int i;
    animation = CCAnimation::animation();
    CCSprite* spTime = CCSprite::spriteWithSpriteFrameName("timestop0001.png");
    if(spTime){
        addChild(spTime, 10, kTimerAnimTag);
        spTime->setPosition(CCPointMake(320, 815));
        
        for(i = 1; i < 100; i++){
            sprintf(frameName, "timestop00%02d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
            if(frame)
                animation->addFrame(frame);
            else
                break;
        }
        action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
        
        spTime->runAction(CCRepeatForever::actionWithAction(action));
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(clock), true);
    }
}


//******************************************************************************
// setAdditionalDur
//******************************************************************************
void StoneGrid::setAdditionalDur(float fDur)
{
    m_fAdditionalDur = fDur;
}

float StoneGrid::getAdditionalDur()
{
    return m_fAdditionalDur;
}


//******************************************************************************
// randElement
//******************************************************************************
int StoneGrid::randElement()
{
    int iRand = SaveData::Inst()->getrand() % m_stoneCount;
    
    int iElem = m_stoneElement[iRand];
    
    return iElem;
}

//******************************************************************************
// setSoundTag
//******************************************************************************
void StoneGrid::setSoundTag(int tag)
{
    m_iSndTag = tag;
}

//******************************************************************************
// isUserDid
//******************************************************************************
bool StoneGrid::isUserDid()
{
    return m_bUserDid;
}

//******************************************************************************
// enemyResetStone
//******************************************************************************
void StoneGrid::enemyResetStone()
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(m_stoneNodeArray[j][i].spStone)
                m_stoneNodeArray[j][i].spStone->runAction(CCFadeOut::actionWithDuration(0.3));
        }
    }
    
    schedule(schedule_selector(StoneGrid::cbEnemyResetStone), 0.3);
}

//******************************************************************************
// cbEnemyResetStone
//******************************************************************************
void StoneGrid::cbEnemyResetStone()
{
    unschedule(schedule_selector(StoneGrid::cbEnemyResetStone));
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            removeChild(m_stoneNodeArray[j][i].spStone, true);
            m_stoneNodeArray[j][i].spStone = NULL;
        }
    }
    
    initMatrixNml(false);
}

//******************************************************************************
// canGiveStonePlus
//******************************************************************************
bool StoneGrid::canGiveStonePlus(int iElem)
{
    bool bRet = false;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if(((m_stoneNodeArray[j][i].iElement == (iElem & 0x0F))
               || m_stoneNodeArray[j][i].iElement == ((iElem >> 4) & 0x0F))
               && (m_stoneNodeArray[j][i].iPlus == 0)){
                bRet = true;
                break;
            }
        }
    }
    
    return bRet;
}

//******************************************************************************
// isAllElem
//******************************************************************************
bool StoneGrid::isAllElem(int iElem)
{
    bool bRet = true;
    
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if((m_stoneNodeArray[j][i].iElement != (iElem & 0x0F))){
                bRet = false;
                break;
            }
        }
    }
    
    return bRet;
}

//******************************************************************************
// giveStonePlus
//******************************************************************************
void StoneGrid::giveStonePlus(int iElem)
{
    for(int i = 0; i < kMaxGridColumnCnt; i ++){
        for(int j = 0; j < kMaxGridRowCnt; j ++){
            if ((m_stoneNodeArray[j][i].iElement == (iElem & 0x0F))
                && (m_stoneNodeArray[j][i].iPlus == 0)){
                m_stoneNodeArray[j][i].iPlus = 1;
                CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                if (spPlus) {
                    m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                    spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                            m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                    spPlus->setScale(0.1);
                    spPlus->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8f));
                }
            }else if ((m_stoneNodeArray[j][i].iElement == ((iElem >> 4) & 0x0F))
                      && (m_stoneNodeArray[j][i].iPlus == 0)){
                m_stoneNodeArray[j][i].iPlus = 1;
                CCSprite* spPlus = CCSprite::spriteWithFile("stonepls.png");
                if (spPlus) {
                    m_stoneNodeArray[j][i].spStone->addChild(spPlus, 0, kStnPlusTag);
                    spPlus->setPosition(ccp(m_stoneNodeArray[j][i].spStone->getContentSize().width*0.7,
                                            m_stoneNodeArray[j][i].spStone->getContentSize().width*0.3));
                    spPlus->setScale(0.1);
                    spPlus->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0), 0.8f));
                }
            }
        }
    }
}

//******************************************************************************
// getComboNum
//******************************************************************************
int StoneGrid::getComboNum()
{
    return m_iCombo;
}

//******************************************************************************
// showEraseAnim
//******************************************************************************
void StoneGrid::showEraseAnim(int iElement, const CCPoint& pos)
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
CCActionInterval* StoneGrid::getActionFromFile(const char* filename, bool bForever)
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
    
    if (!bForever)
    {
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
    }
    action = CCAnimate::actionWithDuration(i/15.0, animation, false);
    
    return action;
}

//******************************************************************************
// OnExit
//******************************************************************************
void StoneGrid::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}





