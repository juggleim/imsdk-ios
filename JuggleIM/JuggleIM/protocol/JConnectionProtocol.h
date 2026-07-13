//
//  JConnectionProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//
#import <JuggleIM/JuggleIMConst.h>

@protocol JConnectionDelegate <NSObject>
@optional
/// Callback for connection status changes.
/// - Parameters:
///   - status: Updated status.
///   - code: Connection error code. Valid when status is JConnectionStatusFailure; 0 for other statuses.
///   - extra: Additional information.
- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code
                            extra:(NSString *)extra;
/// Callback when the database is opened. The local database can be opened even the device is offline to view historical messages.
- (void)dbDidOpen;
- (void)dbDidClose;
@end

@protocol JConnectionProtocol <NSObject>
- (void)connectWithToken:(NSString *)token;

- (void)disconnect:(BOOL)receivePush;

- (void)registerDeviceToken:(NSData *)tokenData;

- (void)registerVoIPToken:(NSData *)tokenData;

/// Sets the language. This mainly affects the push language for built-in messages. English and Chinese are currently supported.
/// - Parameters:
///   - language: Language, for example "en-US".
///   - completeBlock: Result callback. JErrorCodeNone indicates success.
- (void)setLanguage:(NSString *)language
           complete:(void (^)(JErrorCode code))completeBlock;

/// Gets the language. This mainly affects the push language for built-in messages. Chinese and English are currently supported.
/// - Parameter completeBlock: Result callback. JErrorCodeNone indicates success.
- (void)getLanguage:(void (^)(JErrorCode code, NSString *language))completeBlock;

/// Sets connection parameters.
/// - Parameter sighKey:
/// - Parameter headers: Connection headers.
- (void)setConnectParams:(NSString *)signKey
                 headers:(NSDictionary <NSString *, NSString *> *)headers;

- (JConnectionStatus)getConnectionStatus;

- (void)addDelegate:(id<JConnectionDelegate>) delegate;

@end
