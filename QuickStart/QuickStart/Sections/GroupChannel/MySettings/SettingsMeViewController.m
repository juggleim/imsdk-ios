//
//  SettingsMeViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/22.
//

#import "SettingsMeViewController.h"
#import "OCConst.h"
#import "QuickStart-Swift.h"
#import "UIImage+JImage.h"
#import "EditUserNameViewController.h"

@interface SettingsMeViewController () <UITableViewDataSource, UITableViewDelegate, UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@property (nonatomic, strong) UITableView *tableView;
@end

@implementation SettingsMeViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    [self setupSubviews];
    [self configureNavigationBar];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.tableView reloadData];
}

- (void)setupSubviews {
    self.tableView.bounces = false;
    self.tableView.alwaysBounceVertical = false;
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    self.tableView.rowHeight = UITableViewAutomaticDimension;
    self.tableView.estimatedRowHeight = 44.0;
    self.tableView.sectionHeaderHeight = 0;
    self.tableView.frame = self.view.frame;
    [self.view addSubview:self.tableView];
}

- (void)configureNavigationBar {
    self.navigationItem.title = NSLocalizedString(@"Profile", @"");
    UIBarButtonItem *leftButton = [SBUBarButtonItem backButtonWithTarget:self selector:@selector(leftBarButtonItemPressed:)];
    self.navigationItem.leftBarButtonItem = leftButton;
}

- (void)leftBarButtonItemPressed:(id)sender {
    if (self.navigationController && [self.navigationController.viewControllers.lastObject isEqual:self]) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

#pragma mark - UITableViewDelegate / UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 2;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row == 0) {
        return 88.f;
    }
    return 44.f;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    BaseSettingTableViewCell *cell = [[BaseSettingTableViewCell alloc] init];
    if (indexPath.row == 0) {
        [cell setImageView:cell.rightImageView ImageStr:ProfileManager.shared.currentUserInfo.portrait imageSize:CGSizeMake(65, 65) LeftOrRight:1];
        cell.leftLabel.text = NSLocalizedString(@"Avatar", @"");
    } else if (indexPath.row == 1) {
        [cell setCellStyle:RightLabelStyle];
        cell.leftLabel.text = NSLocalizedString(@"Nickname", @"");
        cell.rightLabel.text = ProfileManager.shared.currentUserInfo.userName;
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row == 0) {
        [self selectUserImage];
    } else if (indexPath.row == 1) {
        [self changeNickname];
    }
}

#pragma mark - private
- (void)selectUserImage {
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"Cancel", @"") style:UIAlertActionStyleCancel handler:nil];
    UIAlertAction *takePictureAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"Take Photo", @"") style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self pushToImagePickerController:UIImagePickerControllerSourceTypeCamera];
    }];
    UIAlertAction *myAlbumAction = [UIAlertAction actionWithTitle:NSLocalizedString(@"My Album", @"") style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [self pushToImagePickerController:UIImagePickerControllerSourceTypePhotoLibrary];
    }];
    
    UIAlertController *controller = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    [controller addAction:cancelAction];
    [controller addAction:takePictureAction];
    [controller addAction:myAlbumAction];
    [self presentViewController:controller animated:YES completion:nil];
}

- (void)changeNickname {
    EditUserNameViewController *vc = [[EditUserNameViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)pushToImagePickerController:(UIImagePickerControllerSourceType)sourceType {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.allowsEditing = YES;
        picker.delegate = self;
        if (sourceType == UIImagePickerControllerSourceTypeCamera) {
            if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
                picker.sourceType = sourceType;
            } else {
                NSLog(@"%@", NSLocalizedString(@"The simulator cannot connect to the camera", @""));
            }
        } else {
            picker.sourceType = sourceType;
        }
        picker.modalPresentationStyle = UIModalPresentationFullScreen;
        [self presentViewController:picker animated:YES completion:nil];
    });
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    [UIApplication sharedApplication].statusBarHidden = NO;

    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];

    if ([mediaType isEqual:@"public.image"]) {
        // Get the original image
        UIImage *originImage = [info objectForKey:UIImagePickerControllerOriginalImage];
        // Get the crop rect
        CGRect captureRect = [[info objectForKey:UIImagePickerControllerCropRect] CGRectValue];
        // Get the cropped image for the crop rect
        UIImage *captureImage =
            [UIImage getSubImage:originImage Rect:captureRect imageOrientation:originImage.imageOrientation];
        UIImage *scaleImage = [UIImage scaleImage:captureImage toScale:0.8];
        NSData *data = UIImageJPEGRepresentation(scaleImage, 0.00001);
        UIImage *image = [UIImage imageWithData:data];
        [self dismissViewControllerAnimated:YES completion:nil];
        [JIM.shared.messageManager uploadImage:image success:^(NSString *url) {
            JCUser *current = ProfileManager.shared.currentUserInfo;
            [HttpManager.shared updateUserInfoWithUserId:JIM.shared.currentUserId name:current.userName portrait:url completion:^(NSInteger code) {
                if (code != 0) {
                    return;
                }
                dispatch_async(dispatch_get_main_queue(), ^{
                    ProfileManager.shared.currentUserInfo.portrait = url;
                    [self.tableView reloadData];
                });
            }];
        } error:^(JErrorCode code) {
            
        }];
    }
}

#pragma mark - Setter && Getter
- (UITableView *)tableView {
    if (!_tableView) {
        _tableView = [[UITableView alloc] init];
        _tableView.delegate = self;
        _tableView.dataSource = self;
        _tableView.scrollEnabled = NO;
        _tableView.backgroundColor = ColorFromRGB(0xf0f0f6);
    }
    return _tableView;
}
@end
