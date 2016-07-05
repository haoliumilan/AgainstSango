//
//  TextNode.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-7-30.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "TextNode.h"

#define kTextNodeTTF
//#define kTextNodeBM

const char* gszTtfFilename = "huakangfont.ttf";

#define UI_TAB_NEW_CHILD        400
//******************************************************************************
// Constructor
//******************************************************************************
TextNode::TextNode(const char *label, const CCSize& dimensions, 
                   CCTextAlignment alignment, float fontSize)
{
    m_lbTTF = NULL;
    m_lbBgTTF = NULL;
    m_lbBM = NULL;
    m_bLimitMv = false;
    m_bLimitHeightMv = false;
    
    m_tDimensions = dimensions;
    m_eAlignment = alignment;
    m_fFontSize = fontSize;
    m_AnchorPoint = CCPointMake(0.5, 0.5);
    
#if defined (kTextNodeTTF)
    m_lbTTF = CCLabelTTF::labelWithString(label, dimensions, alignment, gszTtfFilename, fontSize);
    m_lbTTF->setPosition(CCPointZero);
    m_lbTTF->setColor(ccWHITE);
    addChild(m_lbTTF, 1);
    
    setContentSizeInPixels(m_lbTTF->getContentSize());

#elif defined (kTextNodeBM)
    m_lbBM = CCLabelBMFont::labelWithString(label, "avqest.fnt");
    m_lbBM->setPosition(CCPointZero);
    m_lbBM->setAlignment(alignment);
    m_lbBM->setWidth(dimensions.width);
    addChild(m_lbBM);
#endif
}

TextNode::TextNode(const char *label, float fontSize)
{
    m_lbTTF = NULL;
    m_lbBgTTF = NULL;
    m_lbBM = NULL;
    m_bLimitMv = false;
    m_bLimitHeightMv = false;
    
    m_tDimensions = CCSizeMake(0, 0);
    m_eAlignment = CCTextAlignmentLeft;
    m_fFontSize = fontSize;
    m_AnchorPoint = CCPointMake(0.5, 0.5);
    
#if defined (kTextNodeTTF)
    m_lbTTF = CCLabelTTF::labelWithString(label, gszTtfFilename, fontSize);
    m_lbTTF->setPosition(CCPointZero);
    m_lbTTF->setColor(ccWHITE);
    addChild(m_lbTTF, 1);
    
    setContentSizeInPixels(m_lbTTF->getContentSize());
    
#elif defined (kTextNodeBM)
    m_lbBM = CCLabelBMFont::labelWithString(label, "avqest.fnt");
    m_lbBM->setPosition(CCPointZero);
    addChild(m_lbBM);
    
    setContentSizeInPixels(m_lbBM->getContentSize());
#endif
}

//******************************************************************************
// textWithString
//******************************************************************************
TextNode* TextNode::textWithString(const char *label, const CCSize& dimensions, 
                                   CCTextAlignment alignment, float fontSize)
{
    //当 width height 确实时,根据显示文字变化大小
    float fWidthCount = dimensions.width/fontSize;
    float fHeightCount = dimensions.height/fontSize;
    float flLen = strlen(label)/3;
    if (flLen>(fWidthCount*fHeightCount))
    {
        if (0<flLen && 0<fHeightCount)
        {
            flLen /= fHeightCount;
            if (flLen>fWidthCount)
            {
                fontSize *= (fWidthCount/flLen);
                fontSize += 1;
            }
        }
    }
    
    TextNode *pRet = new TextNode(label, dimensions, alignment, fontSize);
    
    if(pRet){
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet)
    return NULL;
}

TextNode *TextNode::textWithString(const char *label, float fontSize) {
    TextNode *pRet = new TextNode(label, fontSize);
    
    if(pRet){
        pRet->autorelease();
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet)
    return NULL;
}

//******************************************************************************
// setAnchorPoint
//******************************************************************************
void TextNode::setAnchorPoint(const CCPoint& point)
{
    m_AnchorPoint = point;
   
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->setAnchorPoint(point);
    }
    
    if(m_lbBgTTF){
        m_lbBgTTF->setAnchorPoint(point);
    }
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->setAnchorPoint(point);
    }
#endif
}

//******************************************************************************
// setColor
//******************************************************************************
void TextNode::setColor(const ccColor3B& color3)
{
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->setColor(color3);
    }
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->setColor(color3);
    }
#endif
}

//******************************************************************************
// setShadowColor
//******************************************************************************
void TextNode::setShadowColor(const ccColor3B& color3)
{
#if defined (kTextNodeTTF)
    if(m_lbBgTTF){
        m_lbBgTTF->setColor(color3);
    }
    else{
        if (m_tDimensions.width == 0 && m_tDimensions.height == 0) {
            m_lbBgTTF = CCLabelTTF::labelWithString(m_lbTTF->getString(), gszTtfFilename, m_fFontSize);
            m_lbBgTTF->setPosition(CCPointMake(2, -2));
            m_lbBgTTF->setColor(color3);
            m_lbBgTTF->setAnchorPoint(m_AnchorPoint);
            addChild(m_lbBgTTF);
        }
        else {
            m_lbBgTTF = CCLabelTTF::labelWithString(m_lbTTF->getString(),m_tDimensions, m_eAlignment, gszTtfFilename, m_fFontSize);
            m_lbBgTTF->setPosition(CCPointMake(2, -2));
            m_lbBgTTF->setColor(color3);
            m_lbBgTTF->setAnchorPoint(m_AnchorPoint);
            addChild(m_lbBgTTF);
        }
    }
    
#endif
}

//******************************************************************************
// setString
//******************************************************************************
void TextNode::setString(const char *label)
{
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->setString(label);
    }
    
    if(m_lbBgTTF){
        m_lbBgTTF->setString(label);
    }
    
    setContentSizeInPixels(m_lbTTF->getContentSize());
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->setString(label);
    }
#endif
}

//******************************************************************************
// GetTitle
//******************************************************************************
string TextNode::GetTitle()
{
    string rt = "";
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        rt = m_lbTTF->getString();
    }
    if (m_lbBgTTF) {
        rt = m_lbBgTTF->getString();
    }
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        rt = m_lbBM->getString();
    }
#endif
    
    return rt;
}


//******************************************************************************
// runActionInside
//******************************************************************************
void TextNode::runActionInside(CCAction* action)
{
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->runAction(action);
        if (CCNode *pNode = m_lbTTF->getChildByTag(UI_TAB_NEW_CHILD))
        {
            pNode->runAction((CCAction*)action->copyWithZone(NULL));
        }
    }
    
    if (m_lbBgTTF) {
        CCAction* actionCopy = (CCAction*)action->copyWithZone(NULL);
        m_lbBgTTF->runAction(actionCopy);
    }
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->runAction(action);
    }
#endif
}

//******************************************************************************
// runActionInsideEx
//******************************************************************************
//void TextNode::runActionInsideEx(CCAction* action)
//{
//#if defined (kTextNodeTTF)
//    if(m_lbTTF){
//        m_lbTTF->runAction(action);
//        if (CCNode *pNode = getChildByTag(UI_TAB_NEW_CHILD))
//        {
//            pNode->runAction((CCAction*)action->copyWithZone(NULL));
//        }
//    }
//    
//    if (m_lbBgTTF) {
//        CCAction* actionCopy = (CCAction*)action->copyWithZone(NULL);
//        m_lbBgTTF->runAction(actionCopy);
//    }
//    
//#elif defined (kTextNodeBM)
//    if (m_lbBM) {
//        m_lbBM->runAction(action);
//    }
//#endif
//}

//******************************************************************************
// stopAllActions
//******************************************************************************
void TextNode::stopAllActionsInside()
{
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->stopAllActions();
    }
    
    if (m_lbBgTTF) {
        m_lbBgTTF->stopAllActions();
    }
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->stopAllActions();
    }
#endif
}

//******************************************************************************
// setOpacity
//******************************************************************************
void TextNode::setOpacity(GLubyte var)
{
#if defined (kTextNodeTTF)
    if(m_lbTTF){
        m_lbTTF->setOpacity(var);
    }
    if (m_lbBgTTF) {
        m_lbBgTTF->setOpacity(var);
    }
    
#elif defined (kTextNodeBM)
    if (m_lbBM) {
        m_lbBM->setOpacity(var);
    }
#endif
}

//******************************************************************************
// visit
//******************************************************************************
void TextNode::visit()
{
    if (m_bLimitMv) {
        CCPoint pos(0,0);
        float fFactor = 1.0;
        CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
        
        if(winsz.width > 1000){
            fFactor = 4.0;
            pos = CCPointMake(128, 64);
        }
        else if(winsz.width == 768){
            fFactor = 2.0;
            pos = CCPointMake(64, 32);
        }
        else if(winsz.width == 640){
            fFactor = 2.0;
            
            if (winsz.height == 1136) {
                pos = CCPointMake(0, 58);
            }
        }

        glEnable(GL_SCISSOR_TEST);
        glScissor(16 * fFactor + pos.x,
                  -120 * fFactor + pos.y,
                  290  * fFactor,
                  480  * fFactor);
        CCNode::visit();
        glDisable(GL_SCISSOR_TEST);
    }
    else if (m_bLimitHeightMv)
    {
        CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
        float fScale = 1.0f;
        if(winsz.width > 1000){
            fScale = 2.0f;
        }
        else if(winsz.width == 768){
        }
        else if(winsz.width == 320){
            fScale = 0.5f;
        }
        
        float shiftX = getContentSizeInPixels().width * m_AnchorPoint.x;
        float shiftY = getContentSizeInPixels().height * m_AnchorPoint.y;
    
        CCPoint word_pos = convertToWorldSpaceAR(CCPointMake(0, 0));
        glEnable(GL_SCISSOR_TEST);
        glScissor(word_pos.x*CC_CONTENT_SCALE_FACTOR()-shiftX*fScale,
                  word_pos.y*CC_CONTENT_SCALE_FACTOR()-shiftY*fScale+getContentSize().height*fScale-m_nFontHeight*m_nRowNum*fScale,
                  getContentSizeInPixels().width*fScale,
                  m_nFontHeight*m_nRowNum*fScale);
        CCNode::visit();
        glDisable(GL_SCISSOR_TEST);
    }
    else{
        CCNode::visit();
    }
}

//******************************************************************************
// moveInLimitedHeight
//******************************************************************************
void TextNode::moveInLimitedHeight(int nFontHeight,int nRowNum)
{
    m_bLimitHeightMv = true;

    m_nFontHeight = nFontHeight;
    m_nRowNum = nRowNum;
    if (getContentSize().height <= m_nFontHeight*m_nRowNum)
    {
        float fGap = m_nFontHeight*m_nRowNum - getContentSize().height;
        
        setPosition(CCPointMake(getPosition().x, getPosition().y-fGap/2));
        return;
    }
        
    
    float fDur = (getContentSize().height-m_nFontHeight*m_nRowNum) * 0.1;
    
    if(m_lbTTF){
        CCPoint pt = m_lbTTF->getPositionInPixels();
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                           CCMoveBy::actionWithDuration(fDur , ccp(0.0, getContentSize().height-m_nFontHeight*m_nRowNum)),
                                                                           CCDelayTime::actionWithDuration(1.0),
                                                                           CCPlace::actionWithPosition(ccp(pt.x, pt.y)),
                                                                           NULL);
        m_lbTTF->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    if (m_lbBgTTF) {
        CCPoint pt = m_lbBgTTF->getPositionInPixels();
        m_lbBgTTF->setPositionInPixels(ccp(pt.x+2, pt.y));
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                           CCMoveBy::actionWithDuration(fDur , ccp(0.0, getContentSize().height-m_nFontHeight*m_nRowNum+2)),
                                                                           CCDelayTime::actionWithDuration(1.0),
                                                                           CCPlace::actionWithPosition(ccp(pt.x+2, pt.y)),
                                                                           NULL);
        m_lbBgTTF->runAction(CCRepeatForever::actionWithAction(action));
    }
}

//******************************************************************************
// moveInLimitedWidth
//******************************************************************************
void TextNode::moveInLimitedWidth(float fLeftLimit, float fRightLimit, bool bRightAlign)
{
    m_fLimitLeft = fLeftLimit;
    m_fLimitWidth = (fRightLimit - fLeftLimit);
    
    float fLbWidth = getContentSize().width;
    
    if (fLbWidth <= m_fLimitWidth) {
        m_bLimitMv = false;
        
        if (bRightAlign) {
            if(m_lbTTF){
                CCPoint pt = m_lbTTF->getPositionInPixels();
                m_lbTTF->setPositionInPixels(ccp(fLeftLimit + m_fLimitWidth - fLbWidth, pt.y));
            }
            
            if (m_lbBgTTF) {
                CCPoint pt = m_lbBgTTF->getPositionInPixels();
                m_lbBgTTF->setPositionInPixels(ccp(fLeftLimit + m_fLimitWidth - fLbWidth + 2, pt.y));
            }
        }
        return;
    }
    else{
        m_bLimitMv = true;
        
        float fDur = (fLbWidth - m_fLimitWidth) * 0.01;

        if(m_lbTTF){
            CCPoint pt = m_lbTTF->getPositionInPixels();
            m_lbTTF->setPositionInPixels(ccp(fLeftLimit, pt.y));
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                               CCMoveTo::actionWithDuration(fDur , ccp(fLeftLimit + m_fLimitWidth - fLbWidth, pt.y)),
                                                                               CCDelayTime::actionWithDuration(1.0),
                                                                               CCPlace::actionWithPosition(ccp(fLeftLimit, pt.y)),
                                                                               NULL);
            m_lbTTF->runAction(CCRepeatForever::actionWithAction(action));
        }
        
        if (m_lbBgTTF) {
            CCPoint pt = m_lbBgTTF->getPositionInPixels();
            m_lbBgTTF->setPositionInPixels(ccp(fLeftLimit+2, pt.y));
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                               CCMoveTo::actionWithDuration(fDur , ccp(fLeftLimit + m_fLimitWidth - fLbWidth+2, pt.y)),
                                                                               CCDelayTime::actionWithDuration(1.0),
                                                                               CCPlace::actionWithPosition(ccp(fLeftLimit+2, pt.y)),
                                                                               NULL);
            m_lbBgTTF->runAction(CCRepeatForever::actionWithAction(action));
        }
    }
}

//******************************************************************************
// addNodeChild
//******************************************************************************
void TextNode::addNodeChild(CCNode *pNode)
{
    if (pNode)
    {
#if defined (kTextNodeTTF)
        if(m_lbTTF){
            m_lbTTF->removeChildByTag(UI_TAB_NEW_CHILD, true);
            m_lbTTF->addChild(pNode,0,UI_TAB_NEW_CHILD);
        }
        
#elif defined (kTextNodeBM)
        if (m_lbBM) {
            m_lbBM->addChild(pNode);
        }
#endif
    }
}

//******************************************************************************
// onExit
//******************************************************************************
void TextNode::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCNode::onExit();
}

