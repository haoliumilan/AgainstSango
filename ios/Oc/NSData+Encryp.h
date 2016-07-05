//
//  Encryption.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-11-13.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

void *NewBase64Decode(
                      const char *inputBuffer,
                      size_t length,
                      size_t *outputLength);

char *NewBase64Encode(
                      const void *inputBuffer,
                      size_t length,
                      bool separateLines,
                      size_t *outputLength);

@interface NSData (Encryption)
- (NSData *)AES256EncryptWithKey:(NSString *)key;
- (NSData *)AES256DecryptWithKey:(NSString *)key;
+ (NSData *)dataFromBase64String:(NSString *)aString;
//- (NSString *)base64EncodedString;
@end
