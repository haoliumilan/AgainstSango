//
//  TitleItem.cpp
//  test
//
//  Created by Zhang Hao on 13-5-20.
//
//

#include "TitleItem.h"
#include "TextNode.h"
#include "CGameData.h"

TitleItem::TitleItem(TitleInf* pTtlInf)
{
    m_enEquipSt = enTtlEquipStNull;
    m_mnUse = NULL;
    m_spMnCvr = NULL;
    m_pListener = NULL;
    m_pfnSelector = NULL;
    m_spTtlDspBg1 = NULL;
    m_spTtlDspBg2 = NULL;
    m_spTtlDspBg3 = NULL;
    m_lyTxt = NULL;
    m_szTtlDspBg.height = 0;
    m_szTtlDspBg.width = 0;
    
    m_strTitleId = pTtlInf->strTitleId;

    m_spTtlNameBg = CCSprite::spriteWithFile("titlenamebg.png");
    if (m_spTtlNameBg) {
        addChild(m_spTtlNameBg);
        
        m_spTtlNameBg->setAnchorPoint(ccp(0.5, 1));
        m_spTtlNameBg->setPosition(CCPointZero);
        
        setContentSizeInPixels(m_spTtlNameBg->getContentSizeInPixels());
        
        //称号名
        m_txtTitleName = TextNode::textWithString(pTtlInf->strTitleName.c_str(), 40);
        m_txtTitleName->setPosition(CCPointMake(66, m_spTtlNameBg->getContentSizeInPixels().height * 0.5));
        m_txtTitleName->setAnchorPoint(ccp(0, 0.5));
        m_txtTitleName->setColor(ccc3(177,177,177));
        m_txtTitleName->setShadowColor(ccBLACK);
        m_spTtlNameBg->addChild(m_txtTitleName);
    }
    
    m_spTtlDspBg1 = CCSprite::spriteWithFile("titledspbg1.png");
    if (m_spTtlDspBg1) {
        addChild(m_spTtlDspBg1, -1);
        
        m_spTtlDspBg1->setAnchorPoint(ccp(0.5, 1));
        
        CCPoint pt(0,0);
        if (m_spTtlNameBg){
            pt.y = 12 - m_spTtlNameBg->getContentSize().height;
        }
        
        m_spTtlDspBg1->setPosition(pt);
        m_spTtlDspBg1->setIsVisible(false);
        
        m_szTtlDspBg = m_spTtlDspBg1->getContentSizeInPixels();
    }
    
    m_spTtlDspBg2 = CCSprite::spriteWithFile("titledspbg2.png");
    if (m_spTtlDspBg2) {
        addChild(m_spTtlDspBg2, -1);
        
        m_spTtlDspBg2->setAnchorPoint(ccp(0.5, 1));
        
        CCPoint pt(0,0);
        if (m_spTtlNameBg){
            pt.y = 13 - m_spTtlNameBg->getContentSize().height - m_szTtlDspBg.height;
        }
        
        m_spTtlDspBg2->setPosition(pt);
        m_spTtlDspBg2->setIsVisible(false);
        
        m_TxtStartPos.x = 80 - m_spTtlDspBg2->getContentSizeInPixels().width*0.5;
        m_TxtStartPos.y = pt.y + 16;

        drawTitleDspTxt(pTtlInf);

        float fScale = 1.0;
        if (m_lyTxt->getContentSizeInPixels().height > m_spTtlDspBg2->getContentSizeInPixels().height) {
            fScale = m_lyTxt->getContentSizeInPixels().height / m_spTtlDspBg2->getContentSizeInPixels().height;
        }
        
        m_spTtlDspBg2->setScaleY(fScale);

        m_lyTxt->setIsVisible(false);
        
        m_szTtlDspBg.height += m_spTtlDspBg2->getContentSizeInPixels().height * fScale;
    }
    
    m_spTtlDspBg3 = CCSprite::spriteWithFile("titledspbg3.png");
    if (m_spTtlDspBg3) {
        addChild(m_spTtlDspBg3, -1);
        
        m_spTtlDspBg3->setAnchorPoint(ccp(0.5, 1));
        
        CCPoint pt(0,0);
        if (m_spTtlNameBg){
            pt.y = 16 - m_spTtlNameBg->getContentSize().height - m_szTtlDspBg.height;
        }
        
        m_spTtlDspBg3->setPosition(pt);
        m_spTtlDspBg3->setIsVisible(false);
        
        m_szTtlDspBg.height += m_spTtlDspBg3->getContentSizeInPixels().height;
    }
}

bool TitleItem::procTouch(CCPoint glPoint)
{
    bool bRet = false;
    CCPoint nodePos = convertToNodeSpace(glPoint);
    
    if (m_spTtlNameBg) {
        CCPoint ptTitleBg = m_spTtlNameBg->getPosition();
        CCSize szTitleBg = m_spTtlNameBg->getContentSize();
        
        bool bTitleTouched =
        CCRect::CCRectContainsPoint(CCRectMake(ptTitleBg.x - szTitleBg.width*0.5,
                                               ptTitleBg.y - szTitleBg.height,
                                               szTitleBg.width,
                                               szTitleBg.height),
                                    nodePos);
        
        if (bTitleTouched) {
            if (m_spTtlDspBg1) {
                bool bVisible = m_spTtlDspBg1->getIsVisible();
                m_spTtlDspBg1->setIsVisible(!bVisible);
                
                if(m_spTtlDspBg2)
                    m_spTtlDspBg2->setIsVisible(!bVisible);
                
                if(m_spTtlDspBg3)
                    m_spTtlDspBg3->setIsVisible(!bVisible);
                
                if (m_mnUse)
                    m_mnUse->setIsVisible(!bVisible);

                m_lyTxt->setIsVisible(!bVisible);
                
                CCSize contentSz = getContentSizeInPixels();
                if (bVisible) {
                    contentSz.height -= m_szTtlDspBg.height;
                }
                else{
                    contentSz.height += m_szTtlDspBg.height;
                }
                
                setContentSizeInPixels(contentSz);
            }
            
            bRet = true;
        }
    }
    
    return bRet;
}

void TitleItem::setEquipSt(enTtlEquipSt eqpst)
{
    m_enEquipSt = eqpst;
    
    switch (eqpst) {
        case enTtlEquiped:
            m_txtTitleName->setColor(ccc3(255,216,0));
            break;
        case enTtlNotEquiped:
            m_txtTitleName->setColor(ccc3(100,216,0));
            break;
        default:
            m_txtTitleName->setColor(ccc3(177,177,177));
            break;
    }
    
    updateMenuBtn();
}

enTtlEquipSt TitleItem::getEquipSt()
{
    return m_enEquipSt;
}

void TitleItem::updateMenuBtn()
{
    if (m_enEquipSt == enTtlEquipStNull)
        return;
    
    if (m_spTtlNameBg == NULL)
        return;

    if (m_mnUse == NULL) {
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        
        if (spMenu1 && spMenu2) {
            m_mnUse = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(TitleItem::cbUse) );
            m_mnUse->setAnchorPoint(CCPointZero);
            float fPosY = m_TxtStartPos.y - m_lyTxt->getContentSizeInPixels().height - spMenu1->getContentSizeInPixels().height;
            m_mnUse->setPositionInPixels(ccp(-spMenu1->getContentSizeInPixels().width * 0.5, fPosY - 6));
            
            m_lyTxt->setContentSizeInPixels(CCSizeMake(450, m_TxtStartPos.y - fPosY));
            
            CCMenu* menu = CCMenu::menuWithItems(m_mnUse, NULL);
            m_lyTxt->addChild(menu);
            menu->setPosition(CCPointZero);
            
            TextNode* txtUse = TextNode::textWithString(CGameData::Inst()->getLanguageValue("deadDlgtip2"), 36);
            txtUse->setPosition(CCPointMake(38, 22));
            txtUse->setAnchorPoint(CCPointZero);
            txtUse->setColor(ccWHITE);
            txtUse->setShadowColor(ccBLACK);
            m_mnUse->addChild(txtUse);
            
            m_spMnCvr = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
            if(m_spMnCvr){
                m_mnUse->addChild(m_spMnCvr);
                m_spMnCvr->setAnchorPoint(CCPointZero);
                m_spMnCvr->setPositionInPixels(ccp(2, 8));
            }
        }
    }
    
    if (m_enEquipSt == enTtlNotEquiped){
        if (m_mnUse)
            m_mnUse->setIsEnabled(true);
        
        if(m_spMnCvr)
            m_spMnCvr->setIsVisible(false);
    }
    else{
        if (m_mnUse)
            m_mnUse->setIsEnabled(false);
        
        if(m_spMnCvr)
            m_spMnCvr->setIsVisible(true);
    }
}

void TitleItem::setUseCb(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

void TitleItem::cbUse(CCObject* sender)
{
    if(m_pListener && m_pfnSelector)
        (m_pListener->*m_pfnSelector)(this);
}

void TitleItem::drawTitleDspTxt(TitleInf* pTtlInf)
{
    if (m_lyTxt == NULL) {
        m_lyTxt = CCLayer::node();
        addChild(m_lyTxt);
    }
    
    CCPoint pt = m_TxtStartPos;
    int iFontSize = 24;
    
    //描述1
    if (pTtlInf->strTitleDsp.empty() == false) {
        TextNode* txtDsp = TextNode::textWithString(pTtlInf->strTitleDsp.c_str(), iFontSize);
        if (txtDsp->getContentSizeInPixels().width > 450) {
            txtDsp = TextNode::textWithString(pTtlInf->strTitleDsp.c_str(),
                                              CCSizeMake(450, iFontSize * 2 + 2),
                                              CCTextAlignmentLeft,
                                              iFontSize);
        }
        
        txtDsp->setAnchorPoint(ccp(0, 1));
        txtDsp->setPosition(pt);
        txtDsp->setColor(ccc3(251, 203, 11));
        txtDsp->setShadowColor(ccBLACK);
        m_lyTxt->addChild(txtDsp, 1);
        
        pt.y -= txtDsp->getContentSizeInPixels().height;
    }
    
    //描述2
    if (pTtlInf->strTitleDspEx.empty() == false) {
        TextNode* txtDsp = TextNode::textWithString(pTtlInf->strTitleDspEx.c_str(), iFontSize);
        if (txtDsp->getContentSizeInPixels().width > 450) {
            txtDsp = TextNode::textWithString(pTtlInf->strTitleDspEx.c_str(),
                                              CCSizeMake(450, iFontSize * 2 + 2),
                                              CCTextAlignmentLeft,
                                              iFontSize);
        }
        
        txtDsp->setAnchorPoint(ccp(0, 1));
        txtDsp->setPosition(pt);
        txtDsp->setColor(ccc3(251, 203, 11));
        txtDsp->setShadowColor(ccBLACK);
        m_lyTxt->addChild(txtDsp, 1);
        
        pt.y -= txtDsp->getContentSizeInPixels().height;
    }
    
    if ((pTtlInf->strTitleDsp.empty() == false) || (pTtlInf->strTitleDspEx.empty() == false)){
        pt.y -= 10;
    }
    
    //任务列表
    int i = 0;
    for (list<TitleQuestInf>::iterator it = pTtlInf->TtlQstLst.begin(); it != pTtlInf->TtlQstLst.end(); ++it){
        TitleQuestInf questInf = *it;
        
        TextNode* txtDsp = TextNode::textWithString(questInf.strQuest.c_str(), iFontSize);
        if (txtDsp->getContentSizeInPixels().width > 450) {
            txtDsp = TextNode::textWithString(questInf.strQuest.c_str(),
                                              CCSizeMake(450, iFontSize * 2 + 2),
                                              CCTextAlignmentLeft,
                                              iFontSize);
        }
        
        txtDsp->setPosition(pt);
        txtDsp->setAnchorPoint(ccp(0, 1));
        txtDsp->setColor(ccWHITE);
        txtDsp->setShadowColor(ccBLACK);
        m_lyTxt->addChild(txtDsp, 1);
        
        //对勾
        if (questInf.bPass) {
            CCSprite* spDuiGou = CCSprite::spriteWithFile("titlequestok.png");
            if (spDuiGou) {
                m_lyTxt->addChild(spDuiGou);
                spDuiGou->setAnchorPoint(ccp(0, 1));
                spDuiGou->setPosition(CCPointMake(pt.x - 30, pt.y));
            }
        }
        
        i++;
        pt.y -= txtDsp->getContentSizeInPixels().height;
    }
    
    m_lyTxt->setContentSizeInPixels(CCSizeMake(450, m_TxtStartPos.y - pt.y));
    
    setEquipSt((enTtlEquipSt)(pTtlInf->iEquipSt));
}





