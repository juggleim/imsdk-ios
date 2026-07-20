//
//  JGetConversationOptions.h
//  JuggleIM
//
//  Created by Fei Li on 2025/2/19.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIMConst.h>

@interface JGetConversationOptions : NSObject
/// Conversation type list; nil means all types
@property (nonatomic, copy) NSArray <NSNumber *> *conversationTypes;
/// Fetch count
@property (nonatomic, assign) int count;
/// Fetch timestamp; 0 means the current time
@property (nonatomic, assign) long long timestamp;
/// Fetch direction
@property (nonatomic, assign) JPullDirection direction;
/// Tag id; nil means no tag restriction
@property (nonatomic, copy) NSString *tagId;
/// YES means fetch without top conversations.
@property (nonatomic, assign) BOOL ignoreTop;
@end
