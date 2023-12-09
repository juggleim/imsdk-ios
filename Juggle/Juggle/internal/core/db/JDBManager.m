//
//  JDBManager.m
//  Juggle
//
//  Created by Nathan on 2023/12/8.
//

#import "JDBManager.h"
#import "JDBHelper.h"

#define kJuggle @"juggle"

@implementation JDBManager

- (void)openIMDB:(NSString *)appKey
          userId:(NSString *)userId {
    NSString *path =
            NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES)[0];
    path = [path stringByAppendingPathComponent:kJuggle];
    path = [path stringByAppendingPathComponent:appKey];
    path = [path stringByAppendingPathComponent:userId];
    
    if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
        [[NSFileManager defaultManager] createDirectoryAtPath:path
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
    [[JDBHelper sharedInstance] openDB:path];
}

@end
