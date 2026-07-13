//
//  JFavoriteMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/13.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JMessage.h>

NS_ASSUME_NONNULL_BEGIN

@interface JFavoriteMessage : NSObject
/// Message
@property (nonatomic, strong) JMessage *message;
/// Favorited time
@property (nonatomic, assign) long long createdTime;
@end

NS_ASSUME_NONNULL_END
