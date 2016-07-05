//
//  MailView.cpp
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "MailView.h"
#include "MailViewTexture.h"
#import "AppController.h"
#include "OcProxy.h"

static MailView* pmailview = NULL;
static MailViewTexture* m_mailviewtexture = NULL;

MailView::MailView()
{

}

MailView::~MailView()
{

}

MailView* MailView::Inst()
{
    if (pmailview == NULL)
    {
        pmailview = new MailView();
    }
    return pmailview;
}


void MailView::ShowMailTexture(const char* ch)
{
    NSString* nstexture = [[[NSString alloc] initWithUTF8String:ch] autorelease];
   
    if (OcProxy::Inst()->isIpadDevice())
    {
        m_mailviewtexture = [[[MailViewTexture alloc] initWithFrame:CGRectMake(170, 380, 440, 240) withText:nstexture ] autorelease];
        AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
        [((UIViewController*) appCtr.viewController).view addSubview:m_mailviewtexture];
    }
    else{
        m_mailviewtexture = [[[MailViewTexture alloc] initWithFrame:CGRectMake(50, 176+OcProxy::Inst()->deltaHeightOf568(), 220, 120) withText:nstexture] autorelease];
        AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
        [((UIViewController* )appCtr.viewController).view addSubview:m_mailviewtexture];
    }
}
void MailView::SetVisibleOrNot(bool a)
{
    [m_mailviewtexture setHidden:a];
}
void MailView::RemoveView()
{
    if (m_mailviewtexture)
    {
        [m_mailviewtexture removeFromSuperview];
        m_mailviewtexture = NULL;
    }
}













