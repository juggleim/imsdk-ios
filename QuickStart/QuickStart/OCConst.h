//
//  OCConst.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/23.
//

#ifndef OCConst_h
#define OCConst_h

#define ColorFromRGB(rgbValue)                                                                               \
    [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16)) / 255.0                                               \
                    green:((float)((rgbValue & 0x00FF00) >> 8)) / 255.0                                                \
                     blue:((float)((rgbValue & 0x0000FF) >> 0)) / 255.0                                                \
                    alpha:1.0]

#endif /* OCConst_h */
