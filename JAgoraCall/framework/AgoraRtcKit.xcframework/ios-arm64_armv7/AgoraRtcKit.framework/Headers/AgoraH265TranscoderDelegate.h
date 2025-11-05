//
//  AgoraH265TranscoderDelegate.h
//  AgoraH265TranscoderDelegate
//
//  Copyright (c) 2022 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraEnumerates.h"

@protocol AgoraH265TranscoderDelegate <NSObject>
@optional
/**
 * Use to notify the result of invoking enableTranscode interface.
 * @param result Result of invoking enableTranscode interface. There are some processing advice below of result.
 * - AgoraH265TranscodeResultRequestInvalid: Channel or uid param have a mistake, you need to check them for correctness.
 * - AgoraH265TranscodeResultUnauthorized: Authentication failed, please check for correctness of token.
 * - AgoraH265TranscodeResultTokenExpired: The token has expired, you need to generate a new token.
 * - AgoraH265TranscodeResultForbidden: You need to contact agora staff to add the vid whitelist.
 * - AgoraH265TranscodeResultNotFound: Indicates that the network may be faulty.
 * - AgoraH265TranscodeResultTooOften: Request is too often, please request again later.
 * - AgoraH265TranscodeResultServerInternalError: The service has an internal error. A request can be made again.
*/
- (void)onEnableTranscode:(AgoraH265TranscodeResult)result NS_SWIFT_NAME(onEnableTranscode(_:));

/**
 * Use to notify the result of invoking queryChannel interface.
 * @param result Result of invoking queryChannel interface. There are some processing advice below of result.
 * - AgoraH265TranscodeResultUnauthorized: Authentication failed, please check for correctness of token.
 * - AgoraH265TranscodeResultTokenExpired: The token has expired, you need to generate a new token.
 * - AgoraH265TranscodeResultNotFound: Indicates that the network may be faulty or the channel param may be is empty.
 * - AgoraH265TranscodeResultTooOften: Request is too often, please request again later.
 * - AgoraH265TranscodeResultServerInternalError: The service has an internal error. A request can be made again.
 * 
 * @param originChannel Origin channel id
 * @param transcodeChannel Transcode channel id
*/
- (void)onQueryChannel:(AgoraH265TranscodeResult)result 
         originChannel:(NSString* _Nullable)originChannel 
      transcodeChannel:(NSString* _Nullable)transcodeChannel NS_SWIFT_NAME(onQueryChannel(_:originChannel:transcodeChannel:));

 /** Use to notify the result of invoking triggerTranscode interface.
  * @param result Result of invoking triggerTranscode interface. There are some processing advice below of result.
  * - AgoraH265TranscodeResultUnauthorized: Authentication failed, please check for correctness of token.
  * - AgoraH265TranscodeResultTokenExpired: The token has expired, you need to generate a new token.
  * - AgoraH265TranscodeResultNotFound: Indicates that the network may be faulty or the channel param may be is empty.
  * - AgoraH265TranscodeResultConflict: The request of trigger transcode is conflicted, please try again.
  * - AgoraH265TranscodeResultTooOften: Request is too often, please request again later.
  * - AgoraH265TranscodeResultServerInternalError: The service has an internal error. A request can be made again.
  * - AgoraH265TranscodeResultServiceUnavailable: May be the number of transcode service is over the limit.
 */
- (void)onTriggerTranscode:(AgoraH265TranscodeResult)result NS_SWIFT_NAME(onTriggerTranscode(_:));
@end