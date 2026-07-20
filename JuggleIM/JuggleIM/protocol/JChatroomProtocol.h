//
//  JChatroomProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JChatroomInfoOptions.h>
#import <JuggleIM/JChatroomInfo.h>

@protocol JChatroomDelegate <NSObject>

/// Current user joined the chatroom.
/// - Parameter chatroomId: Chatroom ID.
- (void)chatroomDidJoin:(NSString *)chatroomId;

/// Current user left the chatroom.
/// - Parameter chatroomId: Chatroom ID.
- (void)chatroomDidQuit:(NSString *)chatroomId;

/// Failed to join the chatroom.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - errorCode: Error code.
- (void)chatroomJoinFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

/// Failed to leave the chatroom.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - errorCode: Error code.
- (void)chatroomQuitFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

/// Current user was kicked from the chatroom.
/// - Parameter chatroomId: Chatroom ID.
- (void)chatroomDidKick:(NSString *)chatroomId;

/// Chatroom was destroyed.
/// - Parameter chatroomId: Chatroom ID.
- (void)chatroomDidDestroy:(NSString *)chatroomId;

@end

@protocol JChatroomAttributesDelegate <NSObject>

/// Chatroom attributes were updated.
/// - Parameters:
///   - attributes: Updated chatroom attribute list.
///   - chatroomId: Chatroom ID.
- (void)attributesDidUpdate:(NSDictionary <NSString *, NSString *> *)attributes
                forChatroom:(NSString *)chatroomId;

/// Chatroom attributes were deleted.
/// - Parameters:
///   - attributes: Deleted chatroom attribute list.
///   - chatroomId: Chatroom ID.
- (void)attributesDidDelete:(NSDictionary <NSString *, NSString *> *)attributes
                forChatroom:(NSString *)chatroomId;

@end

@protocol JChatroomProtocol <NSObject>

/// Joins a chatroom.
/// - Parameters:
///   - chatroomId: Chatroom ID.
- (void)joinChatroom:(NSString *)chatroomId;

/// Joins a chatroom.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - count: Number of historical messages to fetch when joining the chatroom.
- (void)joinChatroom:(NSString *)chatroomId
    prevMessageCount:(int)count;

/// Joins a chatroom.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - count: Number of historical messages to fetch when joining the chatroom.
///   - isAutoCreate: Whether to automatically create the chatroom when it does not exist. The default is not to create it.
- (void)joinChatroom:(NSString *)chatroomId
    prevMessageCount:(int)count
        isAutoCreate:(BOOL)isAutoCreate;

/// Leaves a chatroom.
/// - Parameter chatroomId: Chatroom ID.
- (void)quitChatroom:(NSString *)chatroomId;

/// Gets chatroom information.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - option: Fetch option.
///   - successBlock: Success callback.
///   - errorBlock: Failure callback.
//- (void)getChatroomInfo:(NSString *)chatroomId
//                 option:(JChatroomInfoOptions *)option
//                success:(void (^)(JChatroomInfo *chatroomInfo))successBlock
//                  error:(void (^)(JErrorCode code))errorBlock;

/// Sets chatroom attributes.
/// - Parameters:
///   - attributes: Chatroom attributes. Keys and values are strings. Up to 100 different attributes can be set. Keys not set by the current user cannot be operated on by the client and return JErrorCodeChatroomKeyUnauthorized.
///   - chatroomId: Chatroom ID.
///   - completeBlock: Completion callback.
///                    JErrorCodeNone means all attributes were set successfully.
///                    Other codes mean some keys failed to be set. All failed keys are returned with their corresponding error codes, which can be found in the JErrorCode definitions.
- (void)setAttributes:(NSDictionary <NSString *, NSString *> *)attributes
          forChatroom:(NSString *)chatroomId
             complete:(void (^)(JErrorCode code, NSDictionary<NSString *, NSNumber *> *failedKeys))completeBlock;

/// Deletes chatroom attributes.
/// - Parameters:
///   - keys: Attribute key list to delete. Keys not set by the current user cannot be deleted.
///   - chatroomId: Chatroom ID.
///   - completeBlock: Completion callback.
///                    JErrorCodeNone means all attributes were deleted successfully.
///                    Other codes mean some keys failed to be deleted. All failed keys are returned with their corresponding error codes, which can be found in the JErrorCode definitions.
- (void)removeAttributes:(NSArray <NSString *> *)keys
             forChatroom:(NSString *)chatroomId
                complete:(void (^)(JErrorCode code, NSDictionary<NSString *, NSNumber *> *failedKeys))completeBlock;

/// Gets all chatroom attributes.
/// - Parameters:
///   - chatroomId: Chatroom ID.
///   - completeBlock: Completion callback. JErrorCodeNone indicates success.
- (void)getAllAttributesFromChatroom:(NSString *)chatroomId
                            complete:(void (^)(JErrorCode code, NSDictionary <NSString *, NSString *> *attributes))completeBlock;

- (void)addDelegate:(id<JChatroomDelegate>)delegate;

- (void)addAttributesDelegate:(id<JChatroomAttributesDelegate>)delegate;

@end
