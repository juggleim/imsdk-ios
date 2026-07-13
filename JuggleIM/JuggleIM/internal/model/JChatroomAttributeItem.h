//
//  JChatroomAttributeItem.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/29.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, JChatroomAttrOptType) {
    JChatroomAttrOptType_Default = 0,    //Default
    JChatroomAttrOptType_Update = 1,      //Update
    JChatroomAttrOptType_Delete = 2,      //Delete
};

@interface JChatroomAttributeItem : NSObject
@property (nonatomic, copy) NSString *key;
@property (nonatomic, copy) NSString *value;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) int code;
@property (nonatomic, assign) long long timestamp;
@property (nonatomic, assign) JChatroomAttrOptType type;
@end
