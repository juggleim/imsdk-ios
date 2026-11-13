//
//  JUserCommandMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/11/13.
//

#import "JUserCommandMessage.h"

@implementation JUserCommandMessage

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd|JMessageFlagIsUserCmd;
}

@end
