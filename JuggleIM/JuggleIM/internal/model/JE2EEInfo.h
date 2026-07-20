//
//  JE2EEInfo.h
//  JuggleIM
//
//  Created by Fei Li on 2026/6/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JE2EEInfo : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *deviceId;
@property (nonatomic, strong) NSData *pubKey;

@end

NS_ASSUME_NONNULL_END
