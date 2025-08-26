//
//  JCallVideoDenoiseParams.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/26.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// Video denoise mode.
typedef NS_ENUM(NSUInteger, JCallVideoDenoiseMode) {
    /// Turn off video denoise.
    JCallVideoDenoiseModeOff = 0,
    /// Turn on video denoise.
    JCallVideoDenoiseModeOn = 1,
    /// Automatic video noise reduction, where the SDK internally determines whether to enable noise reduction based on the level of noise in the captured footage.
    JCallVideoDenoiseModeAuto = 2
};

/// Video denoise strength.
typedef NS_ENUM(NSUInteger, JCallVideoDenoiseStrength) {
    /// Light denoise strength.
    JCallVideoDenoiseStrengthLight = 1,
    /// Medium denoise strength.
    JCallVideoDenoiseStrengthMedium = 2,
    /// Heavy denoise strength.
    JCallVideoDenoiseStrengthHeavy = 3
};

@interface JCallVideoDenoiseParams : NSObject
@property (nonatomic, assign) JCallVideoDenoiseMode mode;
@property (nonatomic, assign) JCallVideoDenoiseStrength strenth;
@end

NS_ASSUME_NONNULL_END
