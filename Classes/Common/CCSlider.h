/*
 *  CCSlider.h
 *  CCSlider
 *
 *  Created by Hao Liu on 9/2/11.
 *  Copyright 2011 Five Minutes. All rights reserved.
 *
 */
#ifndef __CC_SLIDER_H__
#define __CC_SLIDER_H__

#include "cocos2d.h"
USING_NS_CC;

class CCSlider : public CCNode, public CCTouchDelegate
{
public:
	float m_minValue;
	float m_maxValue;
	float m_value;
	
	CCObject* m_target; 
	SEL_MenuHandler m_selector;
	
	bool m_trackingTouch;                     //滑块是否被选中
	bool m_trackTouchOutsideContent;          //选中滑块后是否可以在非拉条区域内控制滑块
	bool m_evaluateFirstTouch;                //第一次触摸拉条对滑块是否有影响
	bool m_enabled;                           //是否可以控制滑块
	
	float m_width;                            //平行于拉条方向控制拉块的有效范围
	float m_height;                           //垂直于拉条方向控制拉块的有效范围
	
	CCSprite* m_track;                        //拉条
	CCSprite* m_knob;                         //滑块
	CCProgressTimer* m_bar;                   //有色变长拉条
	
public:
	CCSlider();
	~CCSlider();
	virtual void onEnter();
	virtual void onExit();
	bool initWithTrackImage(const char* track, const char* bar, const char* knob, CCObject* target, SEL_MenuHandler selector);

	void setHeight(float value);
	void setHorizontalPadding(float value);
	void setTrackTouchOutsideContent(bool tof);
	void setEvaluateFirstTouch(bool tof);
	void setEnabled(bool tof);
	void setMinValue(float value);
	void setMaxValue(float value);
	void setValue(float value);
	float getValue();
	
	bool containsPoint(CCPoint loc);
	
	virtual void registerWithTouchDispatcher();
	virtual void removeWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
private:
	bool knobTouched(CCPoint loc);
	void setValueByX(float xpos);
	
};

#endif