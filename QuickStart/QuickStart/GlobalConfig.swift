//
//  GlobalConfig.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/3.
//

import Foundation

public class GlobalConfig {
    public static var appKey: String {
        get {
            if let s = UserDefaults.loadAppKey() {
                return s
            } else {
                return "xxx"
            }
        }
        set {
            UserDefaults.saveAppKey(newValue)
        }
    }
    
    public static var imServer: String {
        get {
            if let s = UserDefaults.loadIMServer() {
                return s
            } else {
                return "xxx"
            }
        }
        set {
            UserDefaults.saveIMServer(newValue)
        }
    }
    
    public static var demoServer: String {
        get {
            if let s = UserDefaults.loadDemoServer() {
                return s
            } else {
                return "xxx"
            }
        }
        set {
            var s1: String = newValue
            var isHttps = false
            if let range = newValue.range(of: httpsPrefix) {
                isHttps = true
                s1 = newValue.replacingCharacters(in: range, with: "")
            } else if let range = newValue.range(of: httpPrefix) {
                s1 = newValue.replacingCharacters(in: range, with: "")
            }
            if let range = s1.range(of: jimSuffix) {
                s1 = newValue.replacingCharacters(in: range, with: "")
            }
            let demo: String
            let im: String
            if isHttps {
                demo = httpsPrefix + s1 + jimSuffix
                im = wssPrefix + s1
            } else {
                demo = httpPrefix + s1 + jimSuffix
                im = wsPrefix + s1
            }
            UserDefaults.saveDemoServer(demo)
            self.imServer = im
        }
    }
    
    private static let wssPrefix = "wss://"
    private static let wsPrefix = "ws://"
    private static let httpsPrefix = "https://"
    private static let httpPrefix = "http://"
    private static let jimSuffix = "/jim"
}
