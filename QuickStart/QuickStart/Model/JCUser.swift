//
//  JCUser.swift
//  QuickStart
//
//  Created by Nathan on 2024/7/30.
//

import Foundation
import JuggleIM

enum FriendApplicationStatus {
    case outgoingApply
    case outgoingAccept
    case outgoingExpired
    case incomingApply
    case incomingAccept
    case incomingExpired
}

class JCUser: JUserInfo {
    var phoneNumber: String?
    var isFriend: Bool = false
    var friendApplicationStatus: FriendApplicationStatus?
}
