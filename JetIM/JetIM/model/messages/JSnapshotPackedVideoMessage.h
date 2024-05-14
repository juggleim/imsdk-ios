//
//  JSnapshotPackedVideoMessage.h
//  JetIM
//
//  Created by Nathan on 2024/5/14.
//

#import <JetIM/JetIM.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JSnapshotPackedVideoMessage : JMediaMessageContent
/// 视频封面图远端地址
@property (nonatomic, strong) UIImage *snapshotImage;
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