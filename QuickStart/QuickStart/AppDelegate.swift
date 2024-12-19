//
//  AppDelegate.swift
//  QuickStart
//
//  Created by Nathan on 2024/5/14.
//

import UIKit
import JuggleIM
import PushKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate, UNUserNotificationCenterDelegate, PKPushRegistryDelegate {

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        
        print("didFinishLaunchingWithOptions")
        
        JIM.shared().setServerUrls(["wss://ws.juggleim.com"])
        let appKey = "nsw3sue72begyv7y"
        HttpManager.shared.setAppKey(appKey)
        JIM.shared().setConsoleLogLevel(.verbose)
        JIM.shared().initWithAppKey(appKey)
        JIM.shared().messageManager.registerContentType(GroupNotifyMessage.self)
        JIM.shared().messageManager.registerContentType(FriendNotifyMessage.self)
        CallCenter.shared().initZegoEngine(with: 1881186044, appSign: "")
        SBULog.logType = LogType.error.rawValue | LogType.warning.rawValue | LogType.info.rawValue
        
//        let pushRegistry = PKPushRegistry(queue: .main)
//        pushRegistry.delegate = self
//        pushRegistry.desiredPushTypes = [.voIP]
        
        UNUserNotificationCenter.current().getNotificationSettings { settings in
            switch settings.authorizationStatus {
            case .authorized:
                DispatchQueue.main.async {
                    UIApplication.shared.registerForRemoteNotifications()
                }
            case .notDetermined:
                UNUserNotificationCenter.current().requestAuthorization(options: [.badge, .alert, .sound]) { granted, error in
                    if granted {
                        DispatchQueue.main.async {
                            UIApplication.shared.registerForRemoteNotifications()
                        }
                    }
                }
            default:
                break
            }
        }
        UNUserNotificationCenter.current().delegate = self
        
        return true
    }
    
    func application(_ application: UIApplication, didRegisterForRemoteNotificationsWithDeviceToken deviceToken: Data) {
        JIM.shared().connectionManager.registerDeviceToken(deviceToken)
        SBULog.info("token did register")
    }
  
    func userNotificationCenter(_ center: UNUserNotificationCenter, didReceive response: UNNotificationResponse, withCompletionHandler completionHandler: @escaping () -> Void) {
        print("userNotificationCenter userInfo is \(response.notification.request.content.userInfo)")
        completionHandler()
    }
    
    func application(_ application: UIApplication, didFailToRegisterForRemoteNotificationsWithError error: any Error) {
        SBULog.info("fail to register token, error is \(error.localizedDescription)")
    }
    
    func pushRegistry(_ registry: PKPushRegistry, didUpdate pushCredentials: PKPushCredentials, for type: PKPushType) {
        JIM.shared().connectionManager.registerVoIPToken(pushCredentials.token)
        SBULog.info("voip token did register")

//        let deviceToken = pushCredentials.token.map { String(format: "%02x", $0) }.joined()
//        print("VoIP 推送令牌: \(deviceToken)")
    }
    
    func pushRegistry(_ registry: PKPushRegistry, didReceiveIncomingPushWith payload: PKPushPayload, for type: PKPushType, completion: @escaping () -> Void) {
        print("收到 VoIP 推送: \(payload.dictionaryPayload)")
    }

    // MARK: UISceneSession Lifecycle

    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        // Called when a new scene session is being created.
        // Use this method to select a configuration to create the new scene with.
        return UISceneConfiguration(name: "Default Configuration", sessionRole: connectingSceneSession.role)
    }

    func application(_ application: UIApplication, didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
        // Called when the user discards a scene session.
        // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
        // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
    }


}

