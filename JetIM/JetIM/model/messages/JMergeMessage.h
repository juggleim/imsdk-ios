//
//  JMergeMessage.h
//  JetIM
//
//  Created by Nathan on 2024/3/19.
//

#import <JetIM/JetIM.h>
#import <JetIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMergeMessagePreviewUnit : NSObject
@property (nonatomic, copy) NSString *previewContent;
@property (nonatomic, strong) JUserInfo *sender;
@end

/// 合并转发消息
@interface JMergeMessage : JMessageContent

/// 标题
@property (nonatomic, copy, readonly) NSString *title;
/// 所有被合并的消息 id 列表，不能超过 100 条（所有消息必须来自同一个会话）
@property (nonatomic, copy, readonly) NSArray <NSString *> *messageIdList;
/// 消息气泡上用来预览的被合并消息列表，不能超过 10 条
@property (nonatomic, copy, readonly) NSArray <JMergeMessagePreviewUnit *> *previewList;

/// 构造方法
/// - Parameters:
///   - messageIdList: 合并消息 id 列表（合并消息全集，所有消息必须来自同一个会话。messageIdList.count 小于等于 100，超过部分将被截掉）
///   - previewList: 消息预览列表（用来在消息气泡中做预览，previewList.count 小于等于 10， 超过部分将被截掉）
- (instancetype)initWithTitle:(NSString *)title
                MessageIdList:(NSArray <NSString *> *)messageIdList
                  previewList:(NSArray <JMergeMessagePreviewUnit *> *)previewList;
@end

NS_ASSUME_NONNULL_END
