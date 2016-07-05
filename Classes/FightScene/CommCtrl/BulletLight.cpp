//
//  BulletLight.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-7-30.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "BulletLight.h"

static float bezierat( float a, float b, float c, float d, ccTime t )
{
	return (powf(1-t,3) * a +
			3*t*(powf(1-t,2))*b +
			3*powf(t,2)*(1-t)*c +
			powf(t,3)*d );
}

BulletLight::BulletLight()
{
    m_fElapse = 0.0f;
    m_fDelay = 0.0f;
    m_fDur = 0.0f;
    m_Index = 0;
    m_startPt = CCPointZero;
    m_sConfig.endPosition = CCPointZero;
    m_sConfig.controlPoint_1 = CCPointZero;
    m_sConfig.controlPoint_2 = CCPointZero;
    m_color4 = ccc4(0, 0, 0, 0);
}

void BulletLight::shoot(const CCPoint& srcPt, const CCPoint& destPt, const ccColor4B& color4, ccTime fDelay, ccTime fDur)
{
    m_sConfig.controlPoint_1.x = (srcPt.x + destPt.x) * 0.5 - srcPt.x;
    m_sConfig.controlPoint_1.y = destPt.y - srcPt.y;
	m_sConfig.controlPoint_2 = m_sConfig.controlPoint_1;
	m_sConfig.endPosition = ccpSub(destPt, srcPt);
    
    m_startPt = srcPt;
    m_color4 = color4;
    m_fDelay = fDelay;
    m_fDur = fDur;
    m_fElapse = 0.0f;
    m_Index = 0;
    m_pointPath[m_Index++] = m_startPt;
    
    unscheduleUpdate();
    scheduleUpdate();
}


void BulletLight::line()
{
    if (m_Index<3 || m_Index > 1024) {
		return;
	}
	CCPoint *p = m_pointPath ;
	//CCPoint vertexMiddle[1024]={ccp(0.0, 0.0)};
    CCPoint*vertexMiddle = new CCPoint[m_Index] ;
	//memset(vertexMiddle, 0, sizeof(vertexMiddle)) ;
	memcpy(vertexMiddle, p, sizeof(CCPoint)*m_Index);
	
	CCPoint pt = ccpSub(p[m_Index-1], p[m_Index-2]);
	GLfloat angle = ccpToAngle(pt);
	vertexMiddle[m_Index-1].x += cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexMiddle[m_Index-1].y += sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    
	CCPoint vertexTop[1024]={ccp(0.0, 0.0)};
	
	vertexTop[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexTop[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	
	GLint count1 = 1;
	
	for (int i = (m_Index-2); i>0; --i) {
		float w = i*0.5f ;
		if (w < 3)w = 3 ;
		else if(w > 8)w = 8 ;
		triangle(&(vertexTop[count1]),p[i],p[i-1], w);
		count1++;
	}
	vertexTop[count1++] = p[0];
    
	CCPoint vertexBottom[1024]={ccp(0.0, 0.0)};
	
	vertexBottom[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	vertexBottom[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
	
	GLint count2 = 1;
	
	for (int i = (m_Index-2); i>0; --i) {
		float w = i*0.5f ;
		if (w < 3)w = 3 ;
		else if(w > 8)w =8 ;
		triangle(&(vertexBottom[count2]),p[i],p[i-1], -w);
		count2++;
	}
	vertexBottom[count2++] = p[0];
	
	CCPoint vertexTriangle[512] ={ccp(0.0, 0.0)} ;
	CCPoint*vertexCpy = vertexTriangle;
	GLubyte lineColors[1024] = {0};
	GLubyte*lineCpy = lineColors ;
	//1
	*vertexCpy = vertexMiddle[m_Index-1] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexMiddle[m_Index-2] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexTop[1] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	
	*vertexCpy = vertexMiddle[m_Index-1] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	*vertexCpy = vertexBottom[1] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	*vertexCpy = vertexMiddle[m_Index-2] ;
	*lineCpy = m_color4.r; lineCpy++ ;
	*lineCpy = m_color4.g; lineCpy++ ;
	*lineCpy = m_color4.b; lineCpy++ ;
	*lineCpy = m_color4.a; lineCpy++ ;
	vertexCpy++ ;
	
	for (int i = 0; i < m_Index-2; i++) {
		//上半个四边形
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexTop[i+1] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		
		*vertexCpy = vertexTop[i+1] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexTop[i+2] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		
		//下半个四边形
		*vertexCpy = vertexBottom[i+1] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexBottom[i+2] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		
		*vertexCpy = vertexMiddle[m_Index-(i+2)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexBottom[i+2] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
		*vertexCpy = vertexMiddle[m_Index-(i+3)] ;
		*lineCpy = m_color4.r; lineCpy++ ;
		*lineCpy = m_color4.g; lineCpy++ ;
		*lineCpy = m_color4.b; lineCpy++ ;
		*lineCpy = m_color4.a; lineCpy++ ;
		vertexCpy++ ;
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glVertexPointer(2, GL_FLOAT, 0, vertexTriangle);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, lineColors);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, (GLint)((vertexCpy-vertexTriangle-1)));
    
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    
    delete [] vertexMiddle ;
}

void BulletLight::eraseTail()
{
    int iDstCnt = BltLgtPtNum * 0.2;
    while ((m_Index >= 3) && (iDstCnt > 0)) {
        CCPoint*p = m_pointPath ;
        m_Index-- ;
        memmove(p, &(p[1]), sizeof(CCPoint)*(m_Index)) ;
        
        iDstCnt--;
    }
}

void BulletLight::triangle(CCPoint* vertex, CCPoint p1, CCPoint p2, GLfloat w)
{
    CCPoint pt = ccpSub(p1, p2);
	GLfloat angle = ccpToAngle(pt);
	
	GLfloat x = sinf(angle) * w;
	GLfloat y = cosf(angle) * w;
	vertex->x = p1.x+x;
	vertex->y = p1.y-y;
}

void BulletLight::update(ccTime dt)
{
    m_fElapse += dt;
    
    if (m_fElapse < m_fDelay)
        return;
    
    float fDt = m_fElapse - m_fDelay;
    
    if (fDt > m_fDur) {
        fDt = m_fDur;
        //unscheduleUpdate();
        eraseTail();
        return;
    }
    
    m_Index = 0;
    float fDstPerc = fDt / m_fDur;
    int iStartPt = (fDstPerc > 0.3)? (BltLgtPtNum * (fDstPerc - 0.3)):0;
    for (int i = iStartPt; i < BltLgtPtNum * fDstPerc; i++) {
        float fPerc = i * 1.0 / BltLgtPtNum;
        
        float xa = 0;
        float xb = m_sConfig.controlPoint_1.x;
        float xc = m_sConfig.controlPoint_2.x;
        float xd = m_sConfig.endPosition.x;
        
        float ya = 0;
        float yb = m_sConfig.controlPoint_1.y;
        float yc = m_sConfig.controlPoint_2.y;
        float yd = m_sConfig.endPosition.y;
        
        
        float x = bezierat(xa, xb, xc, xd, fPerc);
        float y = bezierat(ya, yb, yc, yd, fPerc);
        
        m_pointPath[m_Index++] = ccpAdd(m_startPt, ccp(x, y));
    }
    
    //CCLOG("m_Index = %d", m_Index);
}

void BulletLight::draw()
{
    line();
}


