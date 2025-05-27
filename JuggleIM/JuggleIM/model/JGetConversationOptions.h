//
//  JGetConversationOptions.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/19.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIMConst.h>

@interface JGetConversationOptions : NSObject
/// 会话类型列表，nil 表示全部类型
@property (nonatomic, copy) NSArray <NSNumber *> *conversationTypes;
/// 拉取数量
@property (nonatomic, assign) int count;
/// 拉取时间戳，0 表示当前时间
@property (nonatomic, assign) long long timestamp;
/// 拉取方向
@property (nonatomic, assign) JPullDirection direction;
/// 标签 id，nil 表示不限标签
@property (nonatomic, copy) NSString *tagId;
@end
