//
//  Dialog1.cpp
//  AgainstWar
//
//  Created by XIII on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "DialogForGacha.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "MainScene.h"

//******************************************************************************
// Constructor
//******************************************************************************
DialogForGacha::DialogForGacha(CCObject* target, SEL_CallFuncO selector,int myValue, Dlg_GachaType type)
{
    const char *title;
    char dspbuf[200];
    char myValueBuf[200];
    char myTimesBuf[200];
    const char *buttonTxt;
    
    switch (type) {
        case Dlg_GachaType_Frd1:
            title=CGameData::Inst()->getLanguageValue("gachaTip1");
            snprintf(dspbuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip1"),
                    CGameData::Inst()->getCommonInfo()->gacha_cost_pt,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip2"));

            
            snprintf(myValueBuf,
                     199,
                    "%s%d",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip3"),
                    myValue);
            
            snprintf(myTimesBuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip4"),
                    myValue/CGameData::Inst()->getCommonInfo()->gacha_cost_pt,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip5"));
            
            if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_pt){
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
            }
            else{
                buttonTxt=0;
            }
            break;
            
        case Dlg_GachaType_Frd10:
            title=CGameData::Inst()->getLanguageValue("gachaDlgTip14");
            
            snprintf(dspbuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip1"),
                    CGameData::Inst()->getCommonInfo()->gacha_cost_pt,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip2"));
            
            snprintf(myValueBuf,
                     199,
                    "%s%d",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip3"),
                    myValue);
            
            snprintf(myTimesBuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip4"),
                    myValue/CGameData::Inst()->getCommonInfo()->gacha_cost_pt,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip5"));
            
            if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_pt){
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
            }
            else{
                buttonTxt=0;
            }
            break;
            

        case Dlg_GachaType_QianJiang:
            
            
            title=CGameData::Inst()->getLanguageValue("dbqj_qianJiangTitle");
            snprintf(dspbuf,
                     199,
                     "%s",
                     CGameData::Inst()->getLanguageValue("dbqj_qianJiangMsg")
                     
                     );
            
            
//            snprintf(myValueBuf,
//                     199,
//                     "%s%d%s",
//                     CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
//                     myValue,
//                     CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
//            
//            if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
//                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip9"), 199);
//                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
//            }
//            else{
//                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip10"), 199);
//                
//                buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
//            }
            
            
            break;
        case Dlg_GachaType_QianJiangST:
            
            
            title=CGameData::Inst()->getLanguageValue("dbqj_qianJiangTitle");
            snprintf(dspbuf,
                     199,
                     "%s",
                     CGameData::Inst()->getLanguageValue("dbqj_qianJiangMsg1")
                     
                     );
            
            
            buttonTxt=CGameData::Inst()->getLanguageValue("dbqj_qianJiangBtn1");
            
            
            break;
        case Dlg_GachaType_NoMoney:
        {
            title=CGameData::Inst()->getLanguageValue("dbqj_qianJiangTitle");
            snprintf(dspbuf,
                     199,
                     "%s",
                     CGameData::Inst()->getLanguageValue("arenaTip8")
                     
                     );
            
            
            buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
        }
            break;
        case Dlg_GachaType_QianJiangOT:
            
            
            title=CGameData::Inst()->getLanguageValue("dbqj_qianJiangTitle");
            snprintf(dspbuf,
                     199,
                     "%s",
                     CGameData::Inst()->getLanguageValue("dbqj_qianJiangMsg1")
                     
                     );
            
            
            buttonTxt=CGameData::Inst()->getLanguageValue("dbqj_qianJiangBtn1");
            
            
            break;
        case Dlg_GachaType_Rare1:
            title=CGameData::Inst()->getLanguageValue("gachaTip3");
            snprintf(dspbuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip6"),
                    CGameData::Inst()->getCommonInfo()->gacha_cost_coin,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip7"));

            
            snprintf(myValueBuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                    myValue,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
            
            if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip9"), 199);
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
            }
            else{
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip10"), 199);
                
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
            }
            break;
            
        case Dlg_GachaType_Rare10:
            title=CGameData::Inst()->getLanguageValue("gachaDlgTip15");
            
            snprintf(dspbuf,
                     199,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip12"),
                    CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt);
            
            snprintf(myValueBuf,
                     199,
                    "%s%d%s",
                    CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                    myValue,
                    CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
            
            if (myValue >= (CGameData::Inst()->getCommonInfo()->gacha_cost_coin * CGameData::Inst()->getCommonInfo()->multi_gacha_cnt)){
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip13"), 199);
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
            }
            else{
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("gachaDlgTip16"), 199);
                
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
            }
            
            break;
            
        case Dlg_GachaType_SuiTang:
            title=CGameData::Inst()->getLanguageValue("suitang_gacha_title");
            
            snprintf(dspbuf,
                     199,
                     CGameData::Inst()->getLanguageValue("suitang_gacha_1"),
                     CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
            
            
            snprintf(myValueBuf,
                     199,
                     "%s%d%s",
                     CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                     myValue,
                     CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
            
            if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("suitang_gacha_2"), 199);
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
            }
            else{
                strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("suitang_gacha_3"), 199);
                
                buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
            }

            break;
            
        case Dlg_GachaType_Other:
            
            
            if (MainScene::Inst()->m_other_Gacha == 1)
            {
                title=CGameData::Inst()->getLanguageValue("other_gacha_title1");
                
                snprintf(dspbuf,
                         199,
                         CGameData::Inst()->getLanguageValue("other_gacha_1_1"),
                         CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
                
                
                snprintf(myValueBuf,
                         199,
                         "%s%d%s",
                         CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                         myValue,
                         CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
                
                if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_2_1"), 199);
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
                }
                else{
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_3_1"), 199);
                    
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
                }
            }
            else if (MainScene::Inst()->m_other_Gacha == 2)
            {
                title=CGameData::Inst()->getLanguageValue("other_gacha_title2");
                
                snprintf(dspbuf,
                         199,
                         CGameData::Inst()->getLanguageValue("other_gacha_1_2"),
                         CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
                
                
                snprintf(myValueBuf,
                         199,
                         "%s%d%s",
                         CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                         myValue,
                         CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
                
                if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_2_2"), 199);
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
                }
                else{
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_3_2"), 199);
                    
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
                }
            }
            else if (MainScene::Inst()->m_other_Gacha == 3)
            {
                title=CGameData::Inst()->getLanguageValue("other_gacha_title3");
                
                snprintf(dspbuf,
                         199,
                         CGameData::Inst()->getLanguageValue("other_gacha_1_3"),
                         CGameData::Inst()->getCommonInfo()->gacha_cost_coin);
                
                
                snprintf(myValueBuf,
                         199,
                         "%s%d%s",
                         CGameData::Inst()->getLanguageValue("gachaDlgTip8"),
                         myValue,
                         CGameData::Inst()->getLanguageValue("gachaDlgTip7"));
                
                if (myValue>=CGameData::Inst()->getCommonInfo()->gacha_cost_coin){
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_2_3"), 199);
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaTip2");
                }
                else{
                    strncpy(myTimesBuf,CGameData::Inst()->getLanguageValue("other_gacha_3_3"), 199);
                    
                    buttonTxt=CGameData::Inst()->getLanguageValue("gachaDlgTip11");
                }
            }

            
            
            
            break;
            
        default:
            break;
    }

    
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    if(sp){
        sp->setScale(2.0);
        addChild(sp);
    }
    
    //ok
    TextNode* lbText = 0;

    
    if(type == Dlg_GachaType_QianJiang)
    {
        sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0 - 120, -180));
            addChild(sp, 0, kDlg1ForGacha);
        }
        
        
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_qianJiangBtn1"), CCSizeMake(200, 36), CCTextAlignmentCenter, 26);
        lbText->setPosition(CCPointMake(-3+1 - 120, -180+3));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        
        
        sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if(sp){
            sp->setPosition(CCPointMake(0 + 120, -180));
            addChild(sp, 0, kDlg1ForGacha + 1);
        }
        
        
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("dbqj_qianJiangBtn11"), CCSizeMake(200, 36), CCTextAlignmentCenter, 26);
        lbText->setPosition(CCPointMake(-3+1 + 120, -180+3));
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);

    }
    else if(type == Dlg_GachaType_NoMoney)
    {
        if (buttonTxt&&selector) {
            sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            if(sp){
                sp->setPosition(CCPointMake(0, -180));
                addChild(sp, 0, kDlg1ForGacha);
            }
            
            lbText = TextNode::textWithString(buttonTxt, CCSizeMake(100, 36), CCTextAlignmentCenter, 26);
            lbText->setPosition(CCPointMake(-3+1, -180+3));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }
    else if(type == Dlg_GachaType_QianJiangST || type == Dlg_GachaType_QianJiangOT)
    {
        if (buttonTxt&&selector) {
            sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            if(sp){
                sp->setPosition(CCPointMake(0, -180));
                addChild(sp, 0, kDlg1ForGacha);
            }
            
            lbText = TextNode::textWithString(buttonTxt, CCSizeMake(100, 36), CCTextAlignmentCenter, 26);
            lbText->setPosition(CCPointMake(-3+1, -180+3));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }
    else
    {
        if (buttonTxt&&selector) {
            sp = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            if(sp){
                sp->setPosition(CCPointMake(0, -180));
                addChild(sp, 0, kDlg1ForGacha);
            }
            
            lbText = TextNode::textWithString(buttonTxt, CCSizeMake(100, 36), CCTextAlignmentCenter, kBtnTitleHeight);
            lbText->setPosition(CCPointMake(-3+1, -180+3));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }

    
    //title
    
    lbText = TextNode::textWithString(title, CCSizeMake(560, 130), CCTextAlignmentCenter, 55);
    lbText->setPosition(CCPointMake(0, 170));
    lbText->setColor(ccc3(122, 37, 8));
    addChild(lbText, 1);
    
    
    //dspbuf
    lbText = TextNode::textWithString(dspbuf, CCSizeMake(460, 130), CCTextAlignmentCenter, 30);
    
    if(type == Dlg_GachaType_QianJiang)
    lbText->setPosition(CCPointMake(0, -300));
    else if(type == Dlg_GachaType_QianJiangST || type == Dlg_GachaType_QianJiangOT)
        lbText->setPosition(CCPointMake(0, -150));
    else
        lbText->setPosition(CCPointMake(0, 80));

    lbText->setColor(ccc3(63, 23, 13));
    addChild(lbText, 1);
    
    //myValue
    lbText = TextNode::textWithString(myValueBuf, CCSizeMake(560, 130), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, -10));
    lbText->setColor(ccc3(63, 23, 13));
    addChild(lbText, 1);
    
    
    //myTimes
    
   
    lbText = TextNode::textWithString(myTimesBuf, CCSizeMake(560, 130), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(0, -80));
    lbText->setColor(ccc3(63, 23, 13));
    addChild(lbText, 1);
    

    
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool DialogForGacha::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* node = getChildByTag(kDlg1ForGacha);
    if(node == NULL)
        return true;
    
    CCPoint pt = node->getPosition();
    CCSize sz = node->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    
    if(bRet){
        m_ret.bOk = true;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    
    
    
    CCNode* node1 = getChildByTag(kDlg1ForGacha + 1);
    if(node1 == NULL)
        return true;
    
    CCPoint pt1 = node1->getPosition();
    CCSize sz1 = node1->getContentSize();
    
    bool bRet1 = CCRect::CCRectContainsPoint(CCRectMake(pt1.x - sz1.width * 0.5,
                                                       pt1.y - sz1.height * 0.5,
                                                       sz1.width,
                                                       sz1.height),
                                            pos);
    
    if(bRet1){
        m_ret.bOk = false;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }

    
    
    return true;
}

//******************************************************************************
// onExit
//******************************************************************************
void DialogForGacha::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}