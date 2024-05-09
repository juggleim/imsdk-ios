//
//  JImageMessage.h
//  JetIM
//
//  Created by Nathan on 2023/12/3.
//

#import <JetIM/JetIM.h>
#import "JMediaMessageContent.h"

NS_ASSUME_NONNULL_BEGIN

@interface JImageMessage : JMediaMessageContent
/// 缩略图的远端地址
@property (nonatomic, copy) NSString *thumbnailUrl;
/// 图片高度
@property (nonatomic, assign) int height;
/// 图片宽度
@property (nonatomic, assign) int width;
/// 图片大小，单位：KB
@property (nonatomic, assign) long long size;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
