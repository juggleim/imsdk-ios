//
//  CallTheme.h
//  QuickStart
//
//  Created by Fei Li on 2024/11/1.
//

#ifndef CallTheme_h
#define CallTheme_h

#define ColorFromRGB(rgbValue)                                                                               \
    [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16)) / 255.0                                               \
                    green:((float)((rgbValue & 0x00FF00) >> 8)) / 255.0                                                \
                     blue:((float)((rgbValue & 0x0000FF) >> 0)) / 255.0                                                \
                    alpha:1.0]

#define CallHeaderLength 80.0f
#define CallButtonLength 64.0f
#define CallLabelHeight 25.0f
#define CallVerticalMargin 32.0f
#define CallHorizontalMargin 25.0f
#define CallInsideMargin 5.0f
#define CallFloatingBoardWidth 82
#define CallFloatingBoardHeight 114

#define CallTopGGradientHeight 100
#define CallBottomGradientHeight (([UIScreen mainScreen].bounds.size.height == 812) ? 242.0f : 208.0f)
//  判断是否为iphoneX，是iphoneX，底部按钮整体上移34
#define CallExtraSpace (([UIScreen mainScreen].bounds.size.height == 812) ? 34.0f : 0.0f)
// iphoneX顶部额外加上statusbar的30（实际刘海的高度）
#define CallStatusBarHeight (([UIScreen mainScreen].bounds.size.height == 812) ? 30.0f : 0.0f)

#endif /* CallTheme_h */
