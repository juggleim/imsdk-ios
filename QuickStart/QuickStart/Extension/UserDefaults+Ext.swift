//
//  UserDefaults+Ext.swift
//  SendbirdUIKit-Sample
//
//  Created by Tez Park on 2020/07/02.
//  Copyright © 2020 SendBird, Inc. All rights reserved.
//

import UIKit

extension UserDefaults {
    
    static func loadIsLightTheme() -> Bool {
        return UserDefaults.standard.bool(forKey: "is_light_theme")
    }
    static func saveIsLightTheme(_ isLight: Bool) {
        UserDefaults.standard.set(isLight, forKey: "is_light_theme")
    }
    
    static func loadLanguage() -> GlobalConst.SettingLanguage {
        let value = UserDefaults.standard.integer(forKey: "setting_language")
        return GlobalConst.SettingLanguage(rawValue: value) ?? .Chinese
    }
    static func saveLanguage(_ language: GlobalConst.SettingLanguage) {
        UserDefaults.standard.set(language.rawValue, forKey: "setting_language")
    }
    
    static func loadAppKey() -> String? {
        return UserDefaults.standard.string(forKey: "app_key")
    }
    static func saveAppKey(_ appKey: String) {
        UserDefaults.standard.set(appKey, forKey: "app_key")
    }
    
    static func loadIMServer() -> String? {
        return UserDefaults.standard.string(forKey: "im_server")
    }
    static func saveIMServer(_ appKey: String) {
        UserDefaults.standard.set(appKey, forKey: "im_server")
    }
    
    static func loadDemoServer() -> String? {
        return UserDefaults.standard.string(forKey: "demo_server")
    }
    static func saveDemoServer(_ appKey: String) {
        UserDefaults.standard.set(appKey, forKey: "demo_server")
    }
    
    static func loadAccounts() -> [Any]? {
        return UserDefaults.standard.array(forKey: "accounts")
    }
    static func saveAccounts(_ accounts: [Any]) {
        UserDefaults.standard.set(accounts, forKey: "accounts")
    }
    
    static func loadIsAutoLogin() -> Bool? {
        return UserDefaults.standard.bool(forKey: "is_auto_login")
    }
    static func saveIsAutoLogin(_ isAutoLogin: Bool) {
        UserDefaults.standard.set(isAutoLogin, forKey: "is_auto_login")
    }
}
