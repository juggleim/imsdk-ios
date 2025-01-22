//
//  ProfileManager.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation

class ProfileManager: NSObject {
    @objc static let shared = ProfileManager()
    
    @objc var currentUserInfo: JCUser?
}
