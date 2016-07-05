/*
 *  CCSlider.cpp
 *  CCSlider
 *
 *  Created by Hao Liu on 9/2/11.
 *  Copyright 2011 Five Minutes. All rights reserved.
 *
 */

#include "CCSlider.h"
#include <iostream>

CCSlider::CCSlider()
{
	
}

CCSlider::~CCSlider()
{
	m_bar->release();
	m_track->release();
	m_knob->release();
}

void CCSlider::onEnter()
{
	this->registerWithTouchDispatcher();
	CCNode::onEnter();
}

void CCSlider::onExit()
{
	this->removeWithTouchDispatcher();
	CCNode::onExit();
}

bool CCSlider::initWithTrackImage(const char* track, const char* bar, const char* knob, CCObject* target, SEL_MenuHandler selector)
{

	m_track = CCSprite::spriteWithFile(track);
	m_track->retain();
	m_bar = CCProgressTimer::progressWithFile(bar);
	m_bar->retain();
	m_bar->setType(kCCProgressTimerTypeHorizontalBarLR);
	m_knob = CCSprite::spriteWithFile(knob);
	m_knob->retain();
	m_target = target;
	m_selector = selector;
	m_minValue = 0;
	m_maxValue = 100;
	
	this->addChild(m_track);
	this->addChild(m_bar);
	this->addChild(m_knob);	
	
	m_width = m_track->getContentSize().width;
	m_height = 45;
	
	m_enabled = true;
	
	
	return true;
}

void CCSlider::setHeight(float value)
{
	m_height = value;
}

void CCSlider::setHorizontalPadding(float value)
{
	m_width = m_track->getContentSize().width + value * 2;
}

void CCSlider::setTrackTouchOutsideContent(bool tof)
{
	m_trackTouchOutsideContent = tof;
}

void CCSlider::setEvaluateFirstTouch(bool tof)
{
	m_evaluateFirstTouch = tof;
}

void CCSlider::setEnabled(bool tof)
{
	m_enabled = tof;
}

void CCSlider::setMinValue(float value)
{
	m_minValue = value;
}

void CCSlider::setMaxValue(float value)
{
	m_maxValue = value;
}

void CCSlider::setValue(float value)
{
	value = value < m_minValue ? m_minValue : value;
	value = value > m_maxValue ? m_maxValue : value;
	
	m_value = value;
	float x = (m_value - m_minValue) / (m_maxValue - m_minValue) * m_track->getContentSize().width;
	m_knob->setPosition(ccp(x - m_track->getContentSize().width/2, m_knob->getPosition().y));
	
	m_bar->setPercentage((m_value - m_minValue) / (m_maxValue - m_minValue) * 100);
	m_bar->setPosition(ccp(0, 0));
	//m_bar->setPercentage((m_value - m_minValue) / (m_maxValue - m_minValue) * 100);
//	m_bar->setPosition(ccp(-m_bar->getContentSize().width * (100 - m_bar->getPercentage()) / 100, 0));
	//float starttrack = m_track->getPosition().x - m_track->getContentSize().width/2;
//	m_bar->setScaleX((m_knob->getPosition().x - starttrack) / m_bar->getContentSize().width);
//	m_bar->setPosition(ccp((m_knob->getPosition().x - starttrack) / 2 + starttrack, m_bar->getPosition().y));
	//m_bar->setScaleX((m_value - m_minValue) / (m_maxValue - m_minValue));
//	float starttrack = m_track->getPosition().x - m_track->getContentSize().width/2;
//	m_bar->setPosition(ccp(starttrack + (m_knob->getPosition().x - starttrack)/2, m_bar->getPosition().y));		
	
//	cout << "ccsilder add = " <<this;
//	if (m_target)
//		(m_target->*m_selector)(this);
}

float CCSlider::getValue()
{
	return m_value;
}

bool CCSlider::containsPoint(CCPoint loc)
{
	loc = this->convertToNodeSpace(loc);
	CCRect rect = CCRectMake(-m_width/2, -m_height/2, m_width, m_height);
	return CCRect::CCRectContainsPoint(rect, loc);
}

void CCSlider::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,-100, true);
}

void CCSlider::removeWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool CCSlider::ccTouchBegan(CCTouch* touch, CCEvent* event)
{   
	if (!this->getIsVisible() || !m_enabled)
		return false;
	
	CCPoint loc = touch->locationInView(touch->view());
	loc = CCDirector::sharedDirector()->convertToGL(loc);
	
	if (containsPoint(loc))
	{
		if (m_evaluateFirstTouch)
		{
			setValueByX(convertToNodeSpace(loc).x);
			m_trackingTouch = true;
			return true;
		}
		else 
		{
			m_trackingTouch = knobTouched(loc);
			return m_trackingTouch;
		}
	}
	return false;
}

void CCSlider::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	if (!this->getIsVisible() || !m_enabled)
		return;
	
	if (m_trackingTouch)
	{
		CCPoint loc = touch->locationInView(touch->view());
		loc = CCDirector::sharedDirector()->convertToGL(loc);
		
		if (m_trackTouchOutsideContent)
			setValueByX(convertToNodeSpace(loc).x);
		else 
		{
			if(containsPoint(loc))
				setValueByX(convertToNodeSpace(loc).x);
			else 
				m_trackingTouch = false;
		}

	}
	
}

void CCSlider::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_trackingTouch = false;
}

bool CCSlider::knobTouched(CCPoint loc)
{
	if (containsPoint(loc))
	{
		loc = convertToNodeSpace(loc);
		return fabs(m_knob->getPosition().x - loc.x) < m_knob->getContentSize().width/2;
	}
	return false;
}

void CCSlider::setValueByX(float xpos)
{
	xpos = xpos < -m_track->getContentSize().width/2 ? -m_track->getContentSize().width/2 : xpos;
	xpos = xpos > m_track->getContentSize().width/2 ? m_track->getContentSize().width/2 : xpos;
	m_knob->setPosition(ccp(xpos, m_knob->getPosition().y));
	m_value = (xpos + m_track->getContentSize().width/2) / m_track->getContentSize().width * (m_maxValue - m_minValue) + m_minValue;
	
	m_bar->setPercentage((m_value - m_minValue) / (m_maxValue - m_minValue) * 100);
	m_bar->setPosition(ccp(0, 0));
	//m_bar->setPercentage((m_value - m_minValue) / (m_maxValue - m_minValue) * 100);
//	m_bar->setPosition(ccp(-m_bar->getContentSize().width * (100 - m_bar->getPercentage()) / 100, 0));
	//float starttrack = m_track->getPosition().x - m_track->getContentSize().width/2;
//	m_bar->setScaleX((m_knob->getPosition().x - starttrack) / m_bar->getContentSize().width);
//	m_bar->setPosition(ccp((m_knob->getPosition().x - starttrack) / 2 + starttrack, m_bar->getPosition().y));
	//m_bar->setScaleX((m_value - m_minValue) / (m_maxValue - m_minValue));
//	float starttrack = m_track->getPosition().x - m_track->getContentSize().width/2;
//	m_bar->setPosition(ccp(starttrack + (m_knob->getPosition().x - starttrack)/2, m_bar->getPosition().y));		
	if (m_target)
		(m_target->*m_selector)(this);
}
