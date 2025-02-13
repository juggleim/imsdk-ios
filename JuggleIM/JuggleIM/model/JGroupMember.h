//
//  JGroupMember.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/13.
//

#import <Foundation/Foundation.h>

@interface JGroupMember : NSObject

@property (nonatomic, copy) NSString *groupId;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *groupDisplayName;
@property (nonatomic, strong) NSDictionary <NSString *, NSString *> *extraDic;

@end
