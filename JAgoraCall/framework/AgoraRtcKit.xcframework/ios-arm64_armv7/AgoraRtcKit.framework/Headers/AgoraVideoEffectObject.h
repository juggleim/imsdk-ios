//  AgoraVideoEffectObject.h
//  AgoraVideoEffect
//
//  Copyright (c) 2023 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AgoraObjects.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * @brief The protocol for video effect operations.
 *
 * @since v4.6.0
 */
@protocol AgoraVideoEffectObject <NSObject>

#pragma mark - Enumerations
/**
 * @brief Types of video effect nodes that can be applied.
 *
 * @since v4.6.0
 */
typedef NS_ENUM(NSUInteger, AgoraVideoEffectNodeId) {
    /** Beauty effect node. */
    AgoraVideoEffectNodeBeauty      = 1 << 0, 
    /** Style makeup effect node. */
    AgoraVideoEffectNodeStyleMakeup = 1 << 1, 
    /** Filter effect node. */
    AgoraVideoEffectNodeFilter      = 1 << 2  
};

/**
 * @brief Actions that can be performed on video effect nodes.
 *
 * @since v4.6.0
 */
typedef NS_ENUM(NSInteger, AgoraVideoEffectAction) {
    /** Save the current parameters of the video effect. */
    AgoraVideoEffectActionSave  = 1, 
    /** Reset the video effect to its default parameters. */
    AgoraVideoEffectActionReset = 2  
};

#pragma mark - Core Operations
/**
 * @brief Adds or updates video effects with specified node ID and template.
 *
 * @since v4.6.0
 *
 * @param nodeId The unique identifier or combination of video effect nodes. See AgoraVideoEffectNodeId.
 *               Example:
 *               - Single effect: `VIDEO_EFFECT_NODE_ID::BEAUTY`
 *               - Combined effects: `VIDEO_EFFECT_NODE_ID::BEAUTY | VIDEO_EFFECT_NODE_ID::STYLE_MAKEUP`
 * @note Priority Rules:
 * - The `STYLE_MAKEUP` node takes precedence over `FILTER` parameters.
 * - To apply `FILTER` parameters, first remove the `STYLE_MAKEUP` node:
 *   @code{.objc}
 *   [self removeVideoEffectWithNodeId:AgoraVideoEffectNodeStyleMakeup];
 *   [self addOrUpdateVideoEffectWithNodeId:AgoraVideoEffectNodeFilter templateName:@"template name"];
 *   @endcode
 * @param templateName The name of the effect template. If set to null or empty string, the SDK automatically loads the default parameter configuration from the resource bundle.
 * @return
 * - 0: Success.
 * - <0: Failure.
 */
- (int)addOrUpdateVideoEffectWithNodeId:(NSUInteger)nodeId
                          templateName:(NSString *)templateName
NS_SWIFT_NAME(addOrUpdateVideoEffect(nodeId:templateName:));

/**
 * @brief Removes a video effect with specified node ID.
 *
 * @since v4.6.0
 *
 * @param nodeId The unique identifier of the video effect node to remove. See AgoraVideoEffectNodeId.
 * 
 * @return
 * - 0: Success.
 * - <0: Failure.
 */
- (int)removeVideoEffectWithNodeId:(NSUInteger)nodeId
NS_SWIFT_NAME(removeVideoEffect(nodeId:));

#pragma mark - Effect Operations


/**
 * @brief Performs an action on a specified video effect node.
 *
 * @since v4.6.0
 *
 * @param nodeId The unique identifier of the video effect node. See AgoraVideoEffectNodeId.
 * @param actionId The action to perform on the video effect. See AgoraVideoEffectAction.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)performVideoEffectActionWithNodeId:(NSUInteger)nodeId
                                actionId:(AgoraVideoEffectAction)actionId
NS_SWIFT_NAME(performVideoEffectAction(nodeId:actionId:));

#pragma mark - Parameter Configuration
/**
 * @brief Sets a float parameter for the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category of the parameter option.
 * @param key The key name of the parameter.
 * @param floatValue The float value to set.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setVideoEffectFloatParamWithOption:(NSString *)option
                                    key:(NSString *)key
                              floatValue:(float)floatValue
NS_SWIFT_NAME(setVideoEffectFloatParam(option:key:floatValue:));

/**
 * @brief Sets an integer parameter for the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category of the parameter option.
 * @param key The key name of the parameter.
 * @param intValue The integer value to set.
 *
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setVideoEffectIntParamWithOption:(NSString *)option
                                    key:(NSString *)key
                               intValue:(int)intValue
NS_SWIFT_NAME(setVideoEffectIntParam(option:key:intValue:));

/**
 * @brief Sets a boolean parameter for the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category of the parameter option.
 * @param key The key name of the parameter.
 * @param boolValue The boolean value to set.
 * @return
 * - 0: Success.
 * - <0: Failure.
 */
- (int)setVideoEffectBoolParamWithOption:(NSString *)option
                                    key:(NSString *)key
                              boolValue:(BOOL)boolValue
NS_SWIFT_NAME(setVideoEffectBoolParam(option:key:boolValue:));

#pragma mark - Parameter Retrieval
/**
 * @brief Gets a float parameter from the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category of the parameter option.
 * @param key The key name of the parameter.
 *
 * @return
 * - The float value of the parameter if it exists.
 * - 0.0f if the parameter does not exist or an error occurs.
 */
- (float)getVideoEffectFloatParamWithOption:(NSString *)option
                                    key:(NSString *)key
NS_SWIFT_NAME(getVideoEffectFloatParam(option:key:));

/**
 * @brief Gets an integer parameter from the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category identifier of the parameter.
 * @param key The key of the parameter.
 * @return
 * - The integer value of the parameter if it exists.
 * - 0 if the parameter does not exist or an error occurs.
 */
- (int)getVideoEffectIntParamWithOption:(NSString *)option
                                    key:(NSString *)key
NS_SWIFT_NAME(getVideoEffectIntParam(option:key:));

/**
 * @brief Gets a boolean parameter from the video effect.
 *
 * @since v4.6.0
 *
 * @param option The category identifier of the parameter.
 * @param key The key of the parameter.
 * @return
 * - true: The parameter is enabled.
 * - false: The parameter is disabled or does not exist.
 */
- (BOOL)getVideoEffectBoolParamWithOption:(NSString *)option
                                    key:(NSString *)key
NS_SWIFT_NAME(getVideoEffectBoolParam(option:key:));

@end

NS_ASSUME_NONNULL_END
