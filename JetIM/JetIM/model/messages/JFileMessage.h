//
//  JFileMessage.h
//  JetIM
//
//  Created by Nathan on 2023/12/26.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JFileMessage : JMessageContent
/// 文件名称
@property (nonatomic, copy) NSString *name;
/// 文件的远端地址
@property (nonatomic, copy) NSString *url;
/// 文件大小，单位 KB
@property (nonatomic, assign) long long size;
/// 文件类型
@property (nonatomic, copy) NSString *type;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
