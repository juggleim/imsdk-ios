//
//  MultiCallUserCell.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/10.
//

#import "MultiCallUserCell.h"

@interface MultiCallUserCell ()

@property(nonatomic, strong) JCallMember *model;
@property(nonatomic, assign) JCallStatus callStatus;

@end

@implementation MultiCallUserCell

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // create our image view so that is matches the height and width of this
        // cell
        self.headerImageView = [[UIImageView alloc] initWithFrame:CGRectZero];
        self.headerImageView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
        self.headerImageView.contentMode = UIViewContentModeScaleAspectFill;
        self.headerImageView.clipsToBounds = YES;

        // add a white frame around the image
        self.headerImageView.layer.borderWidth = 1;
        self.headerImageView.layer.borderColor = [UIColor whiteColor].CGColor;
        self.headerImageView.layer.cornerRadius = 4;
        self.headerImageView.layer.masksToBounds = YES;
        self.headerImageView.backgroundColor = [[UIColor clearColor] colorWithAlphaComponent:1.0];
        // Define how the edges of the layer are rasterized for each of the four
        // edges
        // (left, right, bottom, top) if the corresponding bit is set the edge will
        // be antialiased
        //
        self.headerImageView.layer.edgeAntialiasingMask =
            kCALayerLeftEdge | kCALayerRightEdge | kCALayerBottomEdge | kCALayerTopEdge;
        [self.headerImageView setImage:[UIImage imageNamed:@"callDefaultPortrait"]];
//        [self.headerImageView
//            setPlaceholderImage:[RCKitUtility imageNamed:@"default_portrait_msg" ofBundle:@"RongCloud.bundle"]];
        self.headerImageView.hidden = YES;
        [[self contentView] addSubview:self.headerImageView];

        self.nameLabel = [[UILabel alloc] initWithFrame:CGRectZero];
        self.nameLabel.textColor = [UIColor whiteColor];
        self.nameLabel.textAlignment = NSTextAlignmentCenter;
        self.nameLabel.backgroundColor = [UIColor clearColor];
        self.nameLabel.font = [UIFont systemFontOfSize:14];
        self.nameLabel.hidden = YES;
        [[self contentView] addSubview:self.nameLabel];
        self.statusView = [[UIImageView alloc] initWithFrame:CGRectZero];
        self.statusView.hidden = YES;
        [[self contentView] addSubview:self.statusView];

        self.contentView.backgroundColor = [UIColor clearColor];
    }
    return self;
}

- (void)setModel:(JCallMember *)model status:(JCallStatus)callStatus {
    _model = model;
    self.callStatus = callStatus;

    CGFloat nameLabelHeight = 16;
    CGFloat insideMargin = 5;

    if (callStatus == JCallStatusIncoming) {
        nameLabelHeight = 0;
        insideMargin = 0;
    }
    [self resetLayout:nameLabelHeight insideMargin:insideMargin];

//    [self.headerImageView setImageURL:[NSURL URLWithString:model.userInfo.portraitUri]];
    [self.nameLabel setText:model.userInfo.userName];
    if (callStatus == JCallStatusIncoming || callStatus == JCallStatusOutgoing ||
        callStatus == JCallStatusConnecting) {
        self.statusView.image = [UIImage imageNamed:@"callConnecting"];
        self.headerImageView.alpha = 0.5;
    } else {
        self.statusView.image = nil;
        self.statusView.hidden = YES;
        self.headerImageView.alpha = 1.0;
    }
}

- (void)resetLayout:(CGFloat)nameLabelHeight insideMargin:(CGFloat)insideMargin {
    CGFloat minLength = MIN(self.bounds.size.width, self.bounds.size.height - nameLabelHeight - insideMargin);

    self.headerImageView.frame = CGRectMake((self.bounds.size.width - minLength) / 2, 0, minLength, minLength);
    self.headerImageView.hidden = NO;

    self.nameLabel.frame =
        CGRectMake(0, self.bounds.size.height - nameLabelHeight, self.bounds.size.width, nameLabelHeight);
    if (nameLabelHeight > 0) {
        self.nameLabel.hidden = NO;
    } else {
        self.nameLabel.hidden = YES;
    }

    self.statusView.frame =
        CGRectMake((self.bounds.size.width - 17) / 2, (self.headerImageView.frame.size.width - 4) / 2, 17, 4);
    self.statusView.hidden = NO;
}

@end
