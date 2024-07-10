//
//  JUploadManager.h
//  JetIM
//
//  Created by 郑开 on 2024/7/9.
//

#import <Foundation/Foundation.h>
#import "JMessageUploadProvider.h"
#import "JetIMCore.h"

@interface JUploadManager : NSObject<JMessageUploadProvider>


- (instancetype)initWithCore:(JetIMCore *)core;

@end

