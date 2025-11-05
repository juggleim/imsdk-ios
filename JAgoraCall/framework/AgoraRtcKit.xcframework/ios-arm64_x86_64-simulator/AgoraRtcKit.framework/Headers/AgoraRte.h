/**
 *
 * Agora Real Time Engagement
 * Copyright (c) 2024 Agora IO. All rights reserved.
 *
 */

#import <Foundation/Foundation.h>

@class AgoraRteError;
@class AgoraRteObserver;

/**
 * The InitialConfig class is used to initialize the AgoraRte object.
 * @since v4.4.0
 * @technical preview
 */
__attribute__((visibility("default"))) @interface AgoraRteInitialConfig : NSObject

- (instancetype _Nonnull)init;

@end

/**
 * AgoraRte configuration class. Used to configure the AgoraRte object.
 * @since v4.4.0
 */
__attribute__((visibility("default"))) @interface AgoraRteConfig : NSObject

- (instancetype _Nonnull)init;

/**
 * Set the App ID, used during engine initialization. This field needs to be set before calling [AgoraRte initMediaEngine:error:] to initialize the engine.
 * If not set, the default value is an empty string.
 * @since v4.4.0
 * @param appId Your project's App ID
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 * - AgoraRteOk: Success
 * - AgoraRteErrorInvalidArgument: Indicates that the app_id parameter is empty.
 * @return void
 */
- (void)setAppId:(NSString * _Nullable)appId error:(AgoraRteError * _Nullable)error;

/**
 * Get the set App ID.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *  - AgoraRteOk: Success
 * @return NSString Returns the set App ID value
 */
- (NSString* _Nullable)appId:(AgoraRteError * _Nullable)error;


- (void)setLogFolder:(NSString * _Nullable)logFolder error:(AgoraRteError * _Nullable)error;
- (NSString* _Nullable)logFolder:(AgoraRteError * _Nullable)error;

- (void)setLogFileSize:(size_t)logFileSize error:(AgoraRteError * _Nullable)error;
- (size_t)logFileSize:(AgoraRteError * _Nullable)error;

- (void)setAreaCode:(int32_t)areaCode error:(AgoraRteError * _Nullable)error;
- (int32_t)areaCode:(AgoraRteError * _Nullable)error;

- (void)setCloudProxy:(NSString * _Nullable)cloudProxy error:(AgoraRteError * _Nullable)error;
- (NSString * _Nullable)cloudProxy:(AgoraRteError * _Nullable)error;

/**
 * Set JSON formatted parameters, usually used to set certain private parameters supported by AgoraRte.
 * @since v4.4.0
 * @param jsonParameter JSON formatted parameter set
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidArgument: Indicates that the jsonParameter parameter is empty.
 * @return void
 */
- (void)setJsonParameter:(NSString * _Nullable)jsonParameter error:(AgoraRteError * _Nullable)error;

/**
 * Get the currently configured private parameters of the Rte.
 * @since v4.4.0
 * @param error 
 *   - AgoraRteOk: Success
 * @return NSString The set JSON formatted parameter set
 */
- (NSString * _Nullable)jsonParameter:(AgoraRteError * _Nullable)error;

@end

/**
 * The AgoraRte class, which is the basic interface of the Agora Real Time Engagement SDK.
 * @since v4.4.0
 */
__attribute__((visibility("default"))) @interface AgoraRte : NSObject

/**
 * Create an AgoraRte object from the rtc bridge. Used in scenarios where the rtc engine has already been initialized, which can save the operation of initializing the AgoraRte engine.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: Indicates that the rtc engine instance has not been created or the rtc engine has not been initialized. Unable to bridge AgoraRte from rtc engine.
 * @return AgoraRte object. If the AgoraRte object is invalid, subsequent operations on AgoraRte will return an error.
 */
+ (instancetype _Nonnull)getFromBridge:(AgoraRteError * _Nullable)error;

/**
 * Construct an AgoraRte object.
 * @since v4.4.0
 * @param config AgoraRte object initialization configuration object.
 */
- (instancetype _Nonnull)initWithInitialConfig:(AgoraRteInitialConfig * _Nullable)config;

/**
 * Initialize the engine.
 * @since v4.4.0
 * @param cb Asynchronous callback that returns the result of engine initialization.
 *   - @param err AgoraRteError object may return the following AgoraRteErrorCode
 *     - AgoraRteOk: Success
 *     - AgoraRteErrorDefault: Engine initialization failed, specific error reason can be obtained through [Error message].
 *     - AgoraRteErrorInvalidOperation: AgoraRte object created through GetFromBridge, initialization is not allowed.
 *
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorDefault: Engine initialization failed, specific error description can be obtained through [Error message].
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has been destroyed or is invalid.
 * @return BOOL Returns whether the asynchronous operation was successfully placed in the asynchronous operation queue, not whether the initialization action was successful.
 *   - YES: Asynchronous initialization action executed normally.
 *   - NO: Asynchronous initialization action did not execute normally.
 */
- (BOOL)initMediaEngine:(void (^ _Nullable)(AgoraRteError* _Nullable err) )cb error:(AgoraRteError * _Nullable)error;

/**
 * Get the configuration of AgoraRte object.
 * @since v4.4.0
 * @param config The object used to receive AgoraRte configuration information.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed config object is null.
 * @return BOOL Returns the result of getting the configuration.
 *   - YES: Successfully retrieved.
 *   - NO: Failed to retrieve.
 */
- (BOOL)getConfigs:(AgoraRteConfig *_Nonnull)config error:(AgoraRteError * _Nullable)error;

/**
 * Configure the AgoraRte object.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The passed config object is null.
 * @return BOOL Returns the result of setting the configuration.
 *   - YES: Successfully set.
 *   - NO: Failed to set.
 */
- (BOOL)setConfigs:(AgoraRteConfig * _Nonnull)config error:(AgoraRteError * _Nullable)error;

/**
 * Register AgoraRte observer.
 * @since v4.4.0
 * @param observer The object that observes AgoraRte callback events.
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The registered observer object is null.
 * @return BOOL Returns the result of registering the observer.
 *   - YES: Registration is successful
 *   - NO: Registration failed
 * @technical preview
 */
- (BOOL)registerObserver:(AgoraRteObserver * _Nonnull)observer error:(AgoraRteError * _Nullable)error;

/**
 * Unregister AgoraRte observer.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has been destroyed or is invalid.
 *   - AgoraRteErrorInvalidArgument: The observer object to be unregistered is null.
 * @return BOOL Returns the result of unregistering the observer.
 *   - YES: Unregistration is successful
 *   - NO: Unregistration failed
 * @technical preview
 */
- (BOOL)unregisterObserver:(AgoraRteObserver * _Nullable)observer error:(AgoraRteError * _Nullable)error;


/**
 * Destroy the AgoraRte object. The operation will release all resources used by the Rte object.
 * @since v4.4.0
 * @param error AgoraRteError object may return the following AgoraRteErrorCode
 *   - AgoraRteOk: Success
 *   - AgoraRteErrorInvalidOperation: The corresponding internal AgoraRte object has already been destroyed or is invalid.
 * @return BOOL Returns the result of destroying the AgoraRte object.
 *   - YES: Successfully destroyed
 *   - NO: Failed to destroy
 */
- (BOOL)destroy:(AgoraRteError * _Nullable)error;


@end
