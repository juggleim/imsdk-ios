//
//  JIM.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JConnectionProtocol.h>
#import <JuggleIM/JMessageProtocol.h>
#import <JuggleIM/JConversationProtocol.h>
#import <JuggleIM/JUserInfoProtocol.h>
#import <JuggleIM/JChatroomProtocol.h>
#import <JuggleIM/JCallProtocol.h>
#import <JuggleIM/JMomentProtocol.h>

@protocol JConnectionProtocol;
@protocol JMessageProtocol;
@protocol JConversationProtocol;

NS_ASSUME_NONNULL_BEGIN

@interface JIM : NSObject

+ (instancetype)shared;

/*!
 Initializes IM SDK.

 @param appKey The App Key obtained after creating the application.
 @discussion
 You must call this method before using any SDK feature.
 Initialization only needs to be performed once during app lifecycle.
 */
- (void)initWithAppKey:(NSString *)appKey;

/*!
 Sets server URLs.

 @param serverUrls The server URLs.
 */
- (void)setServerUrls:(NSArray <NSString *> *)serverUrls;

/// Sets the console log output level.
/// @param level The log level.
- (void)setConsoleLogLevel:(JLogLevel)level;

/*!
 Gets SDK version.
 */
- (NSString *)getSDKVersion;

- (NSString *)getDeviceId;

// The time difference between server time and local time (server time - local time).
- (long long)getTimeDifference;

/// Connection manager.
@property (nonatomic, strong) id<JConnectionProtocol> connectionManager;
/// Message manager.
@property (nonatomic, strong) id<JMessageProtocol> messageManager;
/// Conversation manager.
@property (nonatomic, strong) id<JConversationProtocol> conversationManager;
/// User info manager.
@property(nonatomic, strong) id<JUserInfoProtocol> userInfoManager;
/// Chatroom manager.
@property (nonatomic, strong) id<JChatroomProtocol> chatroomManager;
/// Callback queue for all SDK delegates. The main queue is used by default if this is not specified.
@property (nonatomic, strong) dispatch_queue_t delegateQueue;
/// Current app key.
@property (nonatomic, copy, readonly) NSString *appKey;
/// Current user ID.
@property (nonatomic, copy, readonly) NSString *currentUserId;
/// Call manager.
@property (nonatomic, strong) id<JCallProtocol> callManager;
/// Moment manager.
@property (nonatomic, strong) id<JMomentProtocol> momentManager;

/// Generates a log zip file for the specified time range and returns its local path.
/// - Parameters:
///   - startTime: Start time, in milliseconds.
///   - endTime: End time, in milliseconds.
- (NSString *)generateZipFile:(long long)startTime
                      endTime:(long long)endTime;

/// Deletes a generated log zip file.
/// - Parameter fileName: The local file path.
- (void)removeZipFile:(NSString *)fileName;
@end

NS_ASSUME_NONNULL_END
