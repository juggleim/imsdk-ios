//
//  JMoment.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JMomentMedia.h>
#import <JuggleIM/JMomentReaction.h>
#import <JuggleIM/JMomentComment.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMoment : NSObject
/// Moment id
@property (nonatomic, copy) NSString *momentId;
/// Text content of the moment
@property (nonatomic, copy) NSString *content;
/// Media content list of the moment (images or videos)
@property (nonatomic, copy) NSArray <JMomentMedia *> *mediaArray;
/// Moment sender
@property (nonatomic, strong) JUserInfo *userInfo;
/// Moment reaction list (supports multiple reaction types)
@property (nonatomic, copy) NSArray <JMomentReaction *> *reactionArray;
/// Moment comment list (contains at most the latest 10 entries here)
@property (nonatomic, copy) NSArray <JMomentComment *> *commentArray;
/// Moment created time
@property (nonatomic, assign) long long createTime;

+ (JMoment *)momentWith:(NSDictionary *)dictionary;
@end

NS_ASSUME_NONNULL_END
