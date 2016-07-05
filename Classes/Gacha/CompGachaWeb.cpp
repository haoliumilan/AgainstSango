//
//  CompGachaWeb.mm
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CompGachaWeb.h"
#include "TextNode.h"
#include "MainScene.h"
#include "OcProxy.h"

CompGachaWeb::CompGachaWeb()
{
    CCSprite *webBack = CCSprite::spriteWithFile("cg_explain.png");
    addChild(webBack);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,-129, true);
    
    sureBtn = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    addChild(sureBtn);
    sureBtn->setPosition(ccp(0, -215));
    
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPoint(0, -2));
    itemLabel->setAnchorPoint(ccp(0.05,-0.4));
    sureBtn->addChild(itemLabel);
    
}


void CompGachaWeb::enter()
{
  //  OcProxy::Inst()->showCompGachaWebView(CGameData::Inst()->getUrlDepDevice(CGameData::Inst()->getCommonInfo()->compgacha_note_url));
    
      OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                       CGameData::Inst()->getUrlDepDevice(CGameData::Inst()->getCommonInfo()->compgacha_note_url),
                                       CCRect(65*2, 166*2, 190*2, 176*2),
                                       true);
}

bool CompGachaWeb::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->locationInView( pTouch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCPoint pt = sureBtn->getPosition();
    CCSize sz = sureBtn->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                       pt.y - sz.height * 0.5,
                                                       sz.width,
                                                       sz.height),
                                            pos);
    if(bRet)
    {
        exit();
        MainScene::Inst()->enableBottomPanel(true);
        
    }else {
        
    }
    return true;
}

void CompGachaWeb::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
}
void CompGachaWeb::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}


void CompGachaWeb::exit()
{
    OcProxy::Inst()->removeWebview();
    this->removeFromParentAndCleanup(true);
}

void CompGachaWeb::onExit()
{
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCLayer::onExit();
}


