//
//  UserDefaults+Ext.swift
//  SendbirdUIKit-Sample
//
//  Created by Tez Park on 2020/07/02.
//  Copyright © 2020 SendBird, Inc. All rights reserved.
//

import UIKit

extension UserDefaults {
    static func loadPhoneNumber() -> String? {
        return UserDefaults.standard.string(forKey: "phone_number")
    }
    static func savePhoneNumber(_ phoneNumber: String) {
        UserDefaults.standard.set(phoneNumber, forKey: "phone_number")
    }
    
    static func loadVerifyCode() -> String? {
        return UserDefaults.standard.string(forKey: "verify_code")
    }
    static func saveVerifyCode(_ verifyCode: String) {
        UserDefaults.standard.set(verifyCode, forKey: "verify_code")
    }
    
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
}
