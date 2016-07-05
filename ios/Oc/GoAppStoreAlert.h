//
//  GoAppStoreAlert.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface GoAppStoreAlert : NSObject<UIAlertViewDelegate>
{
    UIAlertView *m_alertView;
}

- (void)goToAppStoreEvaluate;
- (void)goToAppStoreUpdate;
@end
