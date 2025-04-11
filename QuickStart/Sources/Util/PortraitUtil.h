#import <UIKit/UIKit.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JGroupInfo.h>
#import <JuggleIM/JConversation.h>

@interface PortraitUtil : NSObject
+ (UIImage *)defaultPortraitImageWith:(NSString *)pId
                                 name:(NSString *)name
                                 type:(JConversationType)type;
@end
