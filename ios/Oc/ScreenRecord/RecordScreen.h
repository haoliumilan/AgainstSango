//
//  RecordScreen.h
//  AgainstWar
//
//  Created by user on 13-5-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "THCapture.h"

@interface RecordScreen : NSObject
{
    
}

+ (RecordScreen*)shareInstance;

- (void)startRecord;
- (void)stopRecord;
- (void)cancelRecord;
- (void)pauseRecord;
- (void)resumeRecord;

@end
