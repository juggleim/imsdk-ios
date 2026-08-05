#import "UITextViewAndPlaceholder.h"
#import "OCConst.h"

@interface UITextViewAndPlaceholder ()

@property (nonatomic, weak) UILabel *placeholderLabel;

@end

@implementation UITextViewAndPlaceholder

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (instancetype)initWithFrame:(CGRect)frame {

    self = [super initWithFrame:frame];

    if (self) {

        self.backgroundColor = [UIColor clearColor];

        UILabel *placeholderLabel = [[UILabel alloc] init]; // Add a placeholder label

        placeholderLabel.backgroundColor = [UIColor clearColor];

        placeholderLabel.numberOfLines = 0; // Allow multiline wrapping

        [self addSubview:placeholderLabel];

        self.placeholderLabel = placeholderLabel; // Save the label

        self.myPlaceholderColor = ColorFromRGB(0x999999); // Default placeholder color

        self.font = [UIFont systemFontOfSize:16]; // Default font

        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(textDidChange)
                                                     name:UITextViewTextDidChangeNotification
                                                   object:self]; // Observe text changes

        // Allow vertical dragging
        [self setScrollEnabled:YES];
        self.userInteractionEnabled = YES;
        self.showsVerticalScrollIndicator = YES;
        CGSize size = CGSizeMake(0, 600.0f);
        [self setContentSize:size];
    }

    return self;
}

- (void)setTextAlignment:(NSTextAlignment)textAlignment {
    [super setTextAlignment:textAlignment];
    self.placeholderLabel.textAlignment = textAlignment;
}

- (void)setFont:(UIFont *)font {
    [super setFont:font];
    self.placeholderLabel.font = font;
}

- (void)setContentOffset:(CGPoint)contentOffset {
    contentOffset = CGPointMake(0, 0);
    [super setContentOffset:contentOffset];
}

- (void)layoutSubviews {

    [super layoutSubviews];

    // Calculate height from text

    CGSize maxSize = CGSizeMake(self.frame.size.width, MAXFLOAT);

    CGFloat height =
        [self.myPlaceholder boundingRectWithSize:maxSize
                                         options:NSStringDrawingUsesFontLeading | NSStringDrawingUsesLineFragmentOrigin
                                      attributes:@{
                                          NSFontAttributeName : self.placeholderLabel.font
                                      }
                                         context:nil]
            .size.height;

    CGRect frame = CGRectMake(5, 8, self.frame.size.width - 10, height);

    self.placeholderLabel.frame = frame;
}

- (void)setMyPlaceholder:(NSString *)myPlaceholder {

    _myPlaceholder = [myPlaceholder copy];

    // Set text

    self.placeholderLabel.text = myPlaceholder;

    // Recalculate subview frames

    [self setNeedsLayout];
}

- (void)setMyPlaceholderColor:(UIColor *)myPlaceholderColor {

    _myPlaceholderColor = myPlaceholderColor;

    // Set color

    self.placeholderLabel.textColor = myPlaceholderColor;
}

- (void)setText:(NSString *)text {

    [super setText:text];

    [self textDidChange]; //This is the UITextViewTextDidChangeNotification callback
}

- (void)textDidChange {

    self.placeholderLabel.hidden = self.hasText;
}

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:UITextViewTextDidChangeNotification];
}

@end
