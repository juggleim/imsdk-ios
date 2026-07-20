//
//  JConversationTagInfoContainer.h
//  JuggleIM
//
//  Created by Fei Li on 2026/4/29.
//

#import <Foundation/Foundation.h>
#import "JConversationTagInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConversationTagInfoContainer : NSObject
@property (nonatomic, strong) JConversationTagInfo *tagInfo;
@property (nonatomic, assign) BOOL isAdd;
@end

NS_ASSUME_NONNULL_END
