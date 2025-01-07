//
//  QRCodeViewController.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/6.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>
@class JCGroupInfo;

NS_ASSUME_NONNULL_BEGIN

@interface QRCodeViewController : UIViewController
- (instancetype)initWithConversation:(JConversation *)conversation;

@property (nonatomic, strong) JCGroupInfo *group;
@end

NS_ASSUME_NONNULL_END
