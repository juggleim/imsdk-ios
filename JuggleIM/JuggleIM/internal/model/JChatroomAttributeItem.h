//
//  JChatroomAttributeItem.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/29.
//

#import <Foundation/Foundation.h>

@interface JChatroomAttributeItem : NSObject
@property (nonatomic, copy) NSString *key;
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@end
