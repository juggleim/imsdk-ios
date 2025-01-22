//
//  ForwardSelectViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import "ForwardSelectViewController.h"
#import <Masonry/Masonry.h>
#import "OCConst.h"
#import "ForwardSelectCell.h"
#import "ForwardAlertView.h"
#import <MBProgressHUD/MBProgressHUD.h>

#define ScreenWidth [UIScreen mainScreen].bounds.size.width
#define ScreenHeight [UIScreen mainScreen].bounds.size.height
#define IS_IPHONEX (ScreenWidth >= 375.0f && ScreenHeight >= 812.0f)
#define ExtraBottomHeight (IS_IPHONEX ? 34 : 0)

static NSString *forwardCellIdentifier = @"forwardCellIdentifier";

@interface ForwardSelectViewController ()<UITableViewDelegate, UITableViewDataSource, ForwardAlertViewDelegate>
@property (nonatomic, strong) JMessageContent *content;
@property (nonatomic, strong) UITableView *tableView;
@property (nonatomic, copy) NSArray <JConversationInfo *> *conversationList;
@end

@implementation ForwardSelectViewController

- (instancetype)initWithMessageContent:(JMessageContent *)content {
    if (self = [super init]) {
        self.content = content;
        self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupData];
    [self setupViews];
    [self setupNavi];
}

#pragma mark - UITableViewDelegate & UITableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.conversationList.count;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 56;
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 40;
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section {
    return 0.01;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 40)];
    UILabel *label = [[UILabel alloc] init];
    label.text = @"最近聊天";
    label.font = [UIFont systemFontOfSize:13.5];
    label.textColor = ColorFromRGB(0x939393);
    [view addSubview:label];
    [label mas_makeConstraints:^(MASConstraintMaker *make) {
                make.leading.mas_equalTo(view).mas_offset(12);
                make.centerY.mas_equalTo(view);
    }];
    return view;
}
 
- (UIView *)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section {
    return nil;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    ForwardSelectCell *cell =
        [self.tableView dequeueReusableCellWithIdentifier:forwardCellIdentifier];
    if (cell == nil) {
        cell = [[ForwardSelectCell alloc] init];
    }
    JConversationInfo *conversation = self.conversationList[indexPath.row];
    [cell setModel:conversation.conversation];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    JConversationInfo *conversation = self.conversationList[indexPath.row];
    ForwardAlertView *alert = [ForwardAlertView alertViewWithConversation:conversation.conversation content:self.content];
    alert.delegate = self;
    [alert show];
}

#pragma mark - ForwardAlertViewDelegate
- (void)forwardAlertView:(ForwardAlertView *)alertView
            conversation:(JConversation *)conversation
    clickedButtonAtIndex:(NSInteger)buttonIndex {
    if (buttonIndex == 1) {
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        MBProgressHUD *hud = [[MBProgressHUD alloc] initWithView:window];
        dispatch_async(dispatch_get_main_queue(), ^{
            hud.label.text = @"发送中";
            [window addSubview:hud];
            [window bringSubviewToFront:hud];
            [hud showAnimated:YES];
        });
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            BOOL isMedia = NO;
            if ([self.content isKindOfClass:[JMediaMessageContent class]]) {
                JMediaMessageContent *media = (JMediaMessageContent *)self.content;
                if (media.url.length == 0) {
                    isMedia = YES;
                }
            }
            if (isMedia) {
                JMessage *m = [JIM.shared.messageManager sendMediaMessage:(JMediaMessageContent *)self.content
                                             inConversation:conversation
                                                   progress:nil
                                                    success:^(JMessage *message) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [hud hideAnimated:YES];
                        [self dismissViewControllerAnimated:NO completion:nil];
                        if ([self.delegate respondsToSelector:@selector(messageDidForward:)]) {
                            [self.delegate messageDidForward:message];
                        }
                    });
                } error:^(JErrorCode errorCode, JMessage *message) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [hud hideAnimated:YES];
                        [self dismissViewControllerAnimated:NO completion:nil];
                        if ([self.delegate respondsToSelector:@selector(messageDidForwardFail:errorCode:)]) {
                            [self.delegate messageDidForwardFail:message errorCode:errorCode];
                        }
                    });
                } cancel:nil];
                dispatch_async(dispatch_get_main_queue(), ^{
                    if ([self.delegate respondsToSelector:@selector(messageWillForward:)]) {
                        [self.delegate messageWillForward:m];
                    }
                });
            } else {
                JMessage *m = [JIM.shared.messageManager sendMessage:self.content inConversation:conversation
                                               success:^(JMessage *message) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [hud hideAnimated:YES];
                        [self dismissViewControllerAnimated:NO completion:nil];
                        if ([self.delegate respondsToSelector:@selector(messageDidForward:)]) {
                            [self.delegate messageDidForward:message];
                        }
                    });
                } error:^(JErrorCode errorCode, JMessage *message) {
                    dispatch_async(dispatch_get_main_queue(), ^{
                        [hud hideAnimated:YES];
                        [self dismissViewControllerAnimated:NO completion:nil];
                        if ([self.delegate respondsToSelector:@selector(messageDidForwardFail:errorCode:)]) {
                            [self.delegate messageDidForwardFail:message errorCode:errorCode];
                        }
                    });
                }];
                dispatch_async(dispatch_get_main_queue(), ^{
                    if ([self.delegate respondsToSelector:@selector(messageWillForward:)]) {
                        [self.delegate messageWillForward:m];
                    }
                });
            }
        });
    }
}

#pragma mark - private
- (void)setupData {
    NSArray <JConversationInfo *> *conversationList = [JIM.shared.conversationManager getConversationInfoListWithTypes:@[@(JConversationTypeGroup), @(JConversationTypePrivate)] count:100 timestamp:0 direction:JPullDirectionOlder];
    self.conversationList = [conversationList copy];
}

- (void)setupViews {
    [self.view addSubview:self.tableView];

    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.view).offset(self.navigationController.navigationBar.frame.size.height);
        make.top.left.right.equalTo(self.view);
        make.bottom.equalTo(self.view).offset(-ExtraBottomHeight);
    }];
}

- (void)setupNavi {
    self.title = @"选择一个聊天";
    UIBarButtonItem *leftBarItem = [[UIBarButtonItem alloc] initWithTitle:@"取消"
                                                                    style:UIBarButtonItemStylePlain
                                                                   target:self
                                                                   action:@selector(onLeftButtonClick:)];
    self.navigationItem.leftBarButtonItem = leftBarItem;
}

- (void)onLeftButtonClick:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (UITableView *)tableView {
    if (!_tableView) {
        _tableView = [[UITableView alloc] init];
        [_tableView setSectionIndexColor:[UIColor darkGrayColor]];
        _tableView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
        [_tableView registerClass:[ForwardSelectCell class] forCellReuseIdentifier:forwardCellIdentifier];
        _tableView.delegate = self;
        _tableView.dataSource = self;
    }
    return _tableView;
}

@end
