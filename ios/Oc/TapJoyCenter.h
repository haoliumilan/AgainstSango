//
//  TapJoyCenter.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-9-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TapJoyCenter : NSObject
{
    bool isRequseting;
    int tapJoyRepStatus;
}
+ (TapJoyCenter *)shareInstance;
-(void)getTapPoint;
-(void)setUserId:(NSString *)uid;
-(void)backToGame;
-(void)resetTapjoyStatus;
- (void)showOfferWall;
@property(nonatomic,readwrite) int tapJoyRepStatus;
@end
