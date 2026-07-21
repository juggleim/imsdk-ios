//
//  CallTheme.h
//  QuickStart
//
//  Created by Fei Li on 2024/11/1.
//

#ifndef CallTheme_h
#define CallTheme_h

#import "OCConst.h"

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
//  Check whether the device is iPhone X; if so, move the bottom buttons up by 34
#define CallExtraSpace (([UIScreen mainScreen].bounds.size.height == 812) ? 34.0f : 0.0f)
// Add 30 to the top area of iPhone X for the status bar (the actual notch height)
#define CallStatusBarHeight (([UIScreen mainScreen].bounds.size.height == 812) ? 30.0f : 0.0f)

#endif /* CallTheme_h */
