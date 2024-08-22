//
//  JUploadManager.h
// JuggleIM
//
//  Created by 郑开 on 2024/7/9.
//

#import <Foundation/Foundation.h>
#import "JMessageUploadProvider.h"
#import "JIMCore.h"

@interface JUploadManager : NSObject<JMessageUploadProvider>


- (instancetype)initWithCore:(JIMCore *)core;

@end

