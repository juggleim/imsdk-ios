//
//  SBUUser.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 26/02/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

public class SBUUser: NSObject {
    public private(set) var userId: String
    public private(set) var nickname: String?
    public private(set) var profileURL: String?
    
    public private(set) var user: JUserInfo? // Chat object
    
    @available(*, deprecated, renamed: "profileURL") // 3.0.0
    public var profileUrl: String? { self.profileURL }
    
    /// This is an operator state property.
    /// - Since: 1.2.0
    public internal(set) var isOperator: Bool = false
    
    /// This is a muted state property.
    /// - Since: 1.2.0
    public internal(set) var isMuted: Bool = false
    
    // MARK: - User
    /// This function initializes using the userId, nickname, and profileURL.
    /// - Parameters:
    ///   - userId: userId
    ///   - nickname: nickname (default: nil), If not set this value, sets with userId.
    ///   - profileURL: profileURL (default: nil)
    public init(userId: String, nickname: String? = nil, profileURL: String? = nil) {
        self.userId = userId
        self.nickname = nickname
        self.profileURL = profileURL
    }
    
    /// This function initializes using the user object.
    /// - Parameter user: User obejct
    public init(user: JUserInfo) {
        self.userId = user.userId
        self.nickname = user.userName
        self.profileURL = user.portrait
        self.user = user
    }
    
    /// This function initializes using the user object, operator state, and muted state.
    /// - Parameters:
    ///   - user: `SBUUser` object
    ///   - isOperator: If the user is the operator, sets the value to `true`.
    ///   - isMuted: If the user is the muted, sets the value to `true`.
    public init(user: SBUUser, isOperator: Bool = false, isMuted: Bool = false) {
        self.userId = user.userId
        self.nickname = user.nickname
        self.profileURL = user.profileURL
        self.isOperator = isOperator
        self.isMuted = isMuted
    }
    
    // MARK: - Common
    /// This method returns the default value if there is no alias value.
    /// - since: 1.0.1
    public func refinedNickname() -> String {
        if let nickname = self.nickname, nickname.count > 0 {
            return nickname
        } else {
            return SBUStringSet.User_No_Name
        }
    }
    
    /// This method returns the mentioned value
    /// - Returns: mentioned nickname
    /// - since: 3.0.0
    public func mentionedNickname() -> String {
        let trigger = "@"
        let refinedNickname = refinedNickname()
        return "\(trigger)\(refinedNickname)"
    }
    
    public override var description: String {
        String(
            format: "UserId:%@, Nickname:%@, ProfileURL:%@, Operator:%d Muted:%d",
            self.userId,
            self.nickname ?? "",
            self.profileURL ?? "",
            self.isOperator,
            self.isMuted
        )
    }
    
    /// This method converts the CoreSDK's user list to UIKit's user list.
    /// - Parameter users: CoreSDK's user list
    /// - Returns: UIKit's user list
    /// - Since: 3.0.0
    public static func convertUsers(_ users: [JUserInfo]?) -> [SBUUser] {
        var sbuUsers: [SBUUser] = []
        
        if let users = users {
            
            for user in users {
                let sbuUser = SBUUser(user: user)
                sbuUsers.append(sbuUser)
            }
        }
        return sbuUsers
    }
}
