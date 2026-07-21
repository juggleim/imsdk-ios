//
//  JCGroupInfo.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/26.
//

import Foundation
import JuggleIM

@objc enum GroupRole: Int {
    case member = 0
    case owner = 1
    case admin = 2
    case notMember = 3
}

class JCGroupInfo: JGroupInfo {
    @objc var memberCount: Int = 0
    // Includes only the first 20 members
    var members: [JCUser] = []
    var owner: JCUser?
    @objc var myRole: GroupRole = .member
    var mute: Int = 0
    var maxAdminCount: Int = 10
    var adminCount: Int = 0
    var groupVerifyType: Int = 0
    var historyMessageVisible: Int = 0
    var groupDisplayName: String?
}
