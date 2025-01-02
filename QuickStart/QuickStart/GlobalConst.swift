//
//  GlobalConst.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation

public class GlobalConst {
    public static let friendConversationId = "friend_apply"
    
    public enum SettingLanguage: Int {
        case Chinese = 0
        case English = 1
    }
    
    public enum SettingDisturbTime: Int {
        case noDisturb = 0
        case eightToTwelve = 1
        case nineteenToTwenty = 2
        case twentyThreeToSix = 3
    }
}
