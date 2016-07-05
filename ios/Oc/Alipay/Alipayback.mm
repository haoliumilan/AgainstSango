#include "Sanguoshare.h"
#include "SimpleAudioEngine.h"
#include "Alipayback.h"
#include "CommonDef.h"
#import "MainScene.h"
#import "CGameData.h"
#import "CommonDef.h"
#import "sys/utsname.h"
#include "TextNode.h"
#import "AppController.h"

using namespace CocosDenshion;


CCScene* Alipayback::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	Alipayback *layer = Alipayback::node();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}


bool Alipayback::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    this->setIsVisible(false);
    
    
    TextNode *lbText;
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("publishWeiboTip"), CCSizeMake(500, 40), CCTextAlignmentCenter, 55);
    lbText->setPosition(CCPointMake(320, 650));
    lbText->setColor(kTitleColor);
    addChild(lbText, 1);
    
    
    
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    sp->setPosition(ccp(320, 480));
    if(sp){
        sp->setScale(2.0);
        addChild(sp);
    }
    
    
    sureItem = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    addChild(sureItem,10);
    sureItem->setPosition(ccp(320, 300));
    sureItem->setTag(13);
    
    
    
    char buf[64];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
    TextNode* itemLabel3 = TextNode::textWithString(buf,
                                                    CCSizeMake(127, 30),
                                                    CCTextAlignmentCenter,
                                                    kBtnTitleHeight);
    itemLabel3->setColor(ccc3(255, 255, 255));
    itemLabel3->setShadowColor(ccBLACK);
    itemLabel3->setPosition(CCPointMake(sureItem->getContentSize().width * 0.5 - 2,
                                        sureItem->getContentSize().height * 0.5 + 2));
    sureItem->addChild(itemLabel3);
    
    
    tipLabel = TextNode::textWithString(" ", CCSizeMake(400, 150), CCTextAlignmentCenter, 30);
    addChild(tipLabel,10);
    tipLabel->setColor(kInfoColor);
    tipLabel->setPosition(ccp(320, 480));
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -199, true);
    
    return true;
}

void Alipayback::alipaySuccess(int i){
    this->setIsVisible(true);
    theshareSanguo->setMainMenu(false);
    if (i==0) {
        tipLabel->setString("pay okay!");
    }else{
        tipLabel->setString("pay fail!");
    }
    
}


void Alipayback::menuCloseCallback(CCObject* pSender)
{
  //  MainScene::Inst()->enableBottomPanel(true);
     theshareSanguo->setMainMenu(true);
    this->setIsVisible(false);
}


bool Alipayback::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!getIsVisible()) {
        
        return false;
    }
    
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    
    CCNode *node3 = getChildByTag(13);
    if(node3 == NULL)
        return true;
    
    CCPoint pt = node3->getPosition();
    CCSize sz = node3->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                       pt.y - sz.height * 0.5,
                                                       sz.width,
                                                       sz.height),
                                            pos);
    if(bRet)//返回
    {
        menuCloseCallback(NULL);
        
    }
    return true;
}






