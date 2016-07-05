//
//  UIDevice+ProcessesAdditions.m
//  Cocos2d_ios
//
//  Created by Liu Hao on 13-6-24.
//
//

#import "UIDevice+ProcessesAdditions.h"
#import <sys/sysctl.h>
#import <Foundation/NSProcessInfo.h>


@implementation UIDevice(ProcessesAdditions)

- (NSArray *)runningProcesses {
    
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
    size_t miblen = 4;
    
    size_t size;
    int st = sysctl(mib, miblen, NULL, &size, NULL, 0);
    
    struct kinfo_proc * process = NULL;
    struct kinfo_proc * newprocess = NULL;
    
    do {
        
        size += size / 10;
        newprocess = realloc(process, size);
        
        if (!newprocess){
            
            if (process){
                free(process);
            }
            
            return nil;
        }
        
        process = newprocess;
        st = sysctl(mib, miblen, process, &size, NULL, 0);
        
    } while (st == -1 && errno == ENOMEM);
    
    if (st == 0){
        
        if (size % sizeof(struct kinfo_proc) == 0){
            int nprocess = size / sizeof(struct kinfo_proc);
            
            if (nprocess){
                
                NSMutableArray * array = [[NSMutableArray alloc] init];
                
                for (int i = nprocess - 1; i >= 0; i--)
                {
                    NSString * processID = [[NSString alloc] initWithFormat:@"%d", process[i].kp_proc.p_pid];
                    NSString * processName = [[NSString alloc] initWithFormat:@"%s", process[i].kp_proc.p_comm];
                    NSDictionary * dict = [[NSDictionary alloc] initWithObjects:[NSArray arrayWithObjects:processID, processName, nil]
                                                                        forKeys:[NSArray arrayWithObjects:@"ProcessID", @"ProcessName", nil]];
                    [processID release];
                    [processName release];
                    [array addObject:dict];
                    [dict release];
                }
                
                free(process);
                return [array autorelease];
            }
        }
    }
    return nil;
}


UIAlertView *g_alert = nil;
-(void)OutsideZhuiAlarm:(NSString*)pszTitle :(NSString*)pszMessage :(NSString*)pszOk
{
    if (!g_alert)
    {
//        g_alert = [[UIAlertView alloc] initWithTitle:@"警告" message:@"检测到该设备安装了作弊软件 \n请删除该应用重新登录。" delegate:self cancelButtonTitle:@"退出程序" otherButtonTitles:nil];
        g_alert = [[UIAlertView alloc] initWithTitle:pszTitle message:pszMessage delegate:self cancelButtonTitle:pszOk otherButtonTitles:nil];
        [g_alert show];
        //[g_alert release];
        //g_alert = nil;
    }
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (g_alert==alertView)
    {
        [g_alert release];
        g_alert = nil;
        exit(0);
    }
}


@end
