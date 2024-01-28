//
//  JVideoMessage.h
//  JetIM
//
//  Created by Nathan on 2024/1/25.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JVideoMessage : JMessageContent
/// 视频消息视频的远端地址
@property (nonatomic, copy) NSString *url;
/// 视频封面图远端地址
@property (nonatomic, copy) NSString *snapshotUrl;
/// 视频高度
@property (nonatomic, assign) int height;
/// 视频宽度
@property (nonatomic, assign) int width;
/// 视频大小，单位：KB
@property (nonatomic, assign) long long size;
/// 视频时长，单位：秒
@property (nonatomic, assign) int duration;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
