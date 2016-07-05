//
//  RecordScreen.m
//  AgainstWar
//
//  Created by user on 13-5-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import "RecordScreen.h"

@interface RecordScreen()
{
    
}
@end

static RecordScreen *m_recordCntent = NULL;

@implementation RecordScreen



+(RecordScreen*)shareInstance
{
    if (m_recordCntent == NULL)
    {
        m_recordCntent = [[RecordScreen alloc] init];
    }
    return m_recordCntent;
}



- (void)startRecord
{
    [[THCapture shareInstance] startRecording];
}

- (void)stopRecord
{
     [[THCapture shareInstance] stopRecording];
}

- (void)cancelRecord
{
    [[THCapture shareInstance] cancelRecord];
}

- (void)pauseRecord
{
    
    [[THCapture shareInstance] pauseRecord];
    
}

- (void)resumeRecord
{
    [[THCapture shareInstance] resumeRecord];
}



@end
