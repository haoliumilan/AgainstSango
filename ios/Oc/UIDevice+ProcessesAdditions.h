//
//  UIDevice+ProcessesAdditions.h
//  Cocos2d_ios
//
//  Created by Liu Hao on 13-6-24.
//
//

#import <Foundation/Foundation.h>

@interface UIDevice(ProcessesAdditions)

- (NSArray *)runningProcesses;

-(void)OutsideZhuiAlarm:(NSString*)pszTitle :(NSString*)pszMessage :(NSString*)pszOk;

@end
