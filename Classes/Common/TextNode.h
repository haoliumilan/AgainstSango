//
//  TextNode.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-7-30.
//  Copyright (c) 2012年 __OneClick__. All rights reserved.
//

#ifndef AgainstWar_TextNode_h
#define AgainstWar_TextNode_h

#include "cocos2d.h"

using namespace cocos2d;


class TextNode :public CCNode
{
    CCLabelTTF*     m_lbTTF;
    CCLabelTTF*     m_lbBgTTF;
    
    CCLabelBMFont*  m_lbBM;
    
    CCSize          m_tDimensions;
    CCTextAlignment m_eAlignment;
    float           m_fFontSize;
    CCPoint         m_AnchorPoint;
    
    ccColor3B       m_sColor;
    ccColor3B       m_sShadowColor;
    
    bool            m_bLimitMv;
    bool            m_bLimitHeightMv;
    float           m_fLimitWidth;
    float           m_fLimitLeft;
    float           m_nFontHeight,m_nRowNum;
    
    TextNode(const char *label, const CCSize& dimensions, 
             CCTextAlignment alignment, float fontSize);
    TextNode(const char *label, float fontSize);
    TextNode();
    
public:
    static TextNode* textWithString(const char *label, const CCSize& dimensions, 
                                    CCTextAlignment alignment, float fontSize);
    static TextNode* textWithString(const char *label, float fontSize);
    virtual void onExit();
    virtual void visit();
    
    void setAnchorPoint(const CCPoint& point);
    
    void setColor(const ccColor3B& color3);
    void setShadowColor(const ccColor3B& color3);

    void setString(const char *label);
    string GetTitle();
    
    void runActionInside(CCAction* action);
    //void runActionInsideEx(CCAction* action);
    
    
    void stopAllActionsInside();
    void setOpacity(GLubyte var);
    
    void moveInLimitedHeight(int nFontHeight,int nRowNum);
    void moveInLimitedWidth(float fLeftLimit, float fRightLimit, bool bRightAlign = false);
    void addNodeChild(CCNode *pNode);
};

#endif
