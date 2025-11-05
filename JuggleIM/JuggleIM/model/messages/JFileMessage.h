//
//  JFileMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JFileMessage : JMediaMessageContent
/// 文件名称
@property (nonatomic, copy) NSString *name;
/// 文件大小，单位：Byte
@property (nonatomic, assign) long long size;
/// 文件类型
@property (nonatomic, copy) NSString *type;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
