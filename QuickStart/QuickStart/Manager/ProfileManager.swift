//
//  ProfileManager.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation

class ProfileManager: NSObject {
    static let shared = ProfileManager()
    
    var currentUserInfo: JCUser?
}
