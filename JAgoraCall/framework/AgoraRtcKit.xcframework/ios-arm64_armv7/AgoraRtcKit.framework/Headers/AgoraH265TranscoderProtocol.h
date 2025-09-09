//
//  AgoraH265TranscoderProtocol.h
//  AgoraH265TranscoderProtocol
//
//  Copyright (c) 2022 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol AgoraH265TranscoderDelegate;

@protocol AgoraH265TranscoderProtocol <NSObject>

/**
 * Enable transcoding for a channel.
 * @param token The token for authentication.
 * @param channel The unique channel name for the AgoraRTC session in the string format.
 * @param uid  User ID.
 * @return
 * -  0: Success.
 * - <0: Failure.
 */
- (NSInteger)enableTranscodeWithToken:(NSString* _Nonnull)token 
                      channel:(NSString* _Nonnull)channel 
                          uid:(NSUInteger)uid NS_SWIFT_NAME(enableTranscode(token:channel:uid:));

/**
 * Query the transcoded channel of a channel.
 * @param token The token for authentication.
 * @param channel The unique channel name for the AgoraRTC session in the string format.
 * @param uid  User ID.
 * @return
 * -  0: Success.
 * - <0: Failure.
 */
- (NSInteger)queryChannelWithToken:(NSString* _Nonnull)token 
                           channel:(NSString* _Nonnull)channel 
                               uid:(NSUInteger) uid NS_SWIFT_NAME(queryChannel(token:channel:uid:));

/**
 * Trigger channel transcoding.
 * @param token The token for authentication.
 * @param channel The unique channel name for the AgoraRTC session in the string format.
 * @param uid  User ID.
 * @return
 * -  0: Success.
 * - <0: Failure.
 */
- (NSInteger)triggerTranscodeWithToken:(NSString* _Nonnull)token 
                               channel:(NSString* _Nonnull)channel 
                                   uid:(NSUInteger)uid NS_SWIFT_NAME(triggerTranscode(token:channel:uid:));
                                   
/**
 * Register a AgoraH265TranscoderDelegate object.
 * @param delegate AgoraH265TranscoderDelegate.
 * @return
 * -  0: Success.
 * - <0: Failure.
 */
- (NSInteger)registerTranscoderDelegate:(id<AgoraH265TranscoderDelegate>_Nullable)delegate NS_SWIFT_NAME(registerTranscoderDelegate(_:));

/**
 * Unregister a AgoraH265TranscoderDelegate object.
 * @param delegate AgoraH265TranscoderDelegate.
 * @return
 * -  0: Success.
 * - <0: Failure.
 */
- (NSInteger)unregisterTranscoderDelegate:(id<AgoraH265TranscoderDelegate>_Nullable)delegate NS_SWIFT_NAME(unregisterTranscoderDelegate(_:));

@end