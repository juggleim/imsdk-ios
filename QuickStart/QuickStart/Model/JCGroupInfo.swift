//
//  JCGroupInfo.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/26.
//

import Foundation
import JuggleIM

enum GroupRole: Int {
    case member = 0
    case owner = 1
    case admin = 2
}

class JCGroupInfo: JGroupInfo {
    var memberCount: Int = 0
    // 只包含前 20 个成员
    var members: [JCUser] = []
    var owner: JCUser?
    var myRole: GroupRole = .member
    var mute: Int = 0
    var maxAdminCount: Int = 10
    var adminCount: Int = 0
    var groupVerifyType: Int = 0
    var historyMessageVisible: Int = 0
    var groupDisplayName: String?
}
