//
//  JFileMessage.h
// JuggleIM
//
//  Created by Nathan on 2023/12/26.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JFileMessage : JMediaMessageContent
/// File name
@property (nonatomic, copy) NSString *name;
/// File size, in bytes
@property (nonatomic, assign) long long size;
/// File type
@property (nonatomic, copy) NSString *type;
/// Extra field
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
