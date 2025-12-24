//
//  MainChannelTabbarController.swift
//  SendbirdUIKit-Sample
//
//  Created by Tez Park on 2020/09/11.
//  Copyright © 2020 SendBird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

enum TabType {
    case channels, contacts, bots, mySettings
}

class MainChannelTabbarController: UITabBarController {
    let channelsViewController = ChannelListViewController(conversationTypes: [NSNumber(value: JConversationType.private.rawValue), NSNumber(value: JConversationType.group.rawValue), NSNumber(value: JConversationType.publicService.rawValue)])
    let contactListViewController = ContactListViewController()
    let botListViewController = BotListViewController()
    let settingsViewController = MySettingsViewController()
    
    var channelsNavigationController = UINavigationController()
    var contactListNavigationController = UINavigationController()
    var botListNavigationController = UINavigationController()
    var mySettingsNavigationController = UINavigationController()
    
    var theme: SBUComponentTheme = SBUTheme.componentTheme
    var isDarkMode: Bool = false

    // MARK: - Life cycle
    override func viewDidLoad() {
        super.viewDidLoad()
        
        channelsViewController.headerComponent?.titleView = UIView()
        channelsViewController.headerComponent?.leftBarButton = self.createLeftTitleItem(text: "消息")
        
        self.channelsNavigationController = UINavigationController(
            rootViewController: channelsViewController
        )
        self.contactListNavigationController = UINavigationController(
            rootViewController: contactListViewController
        )
//        self.groupListNavigationController = UINavigationController(
//            rootViewController: groupListViewController
//        )
        self.botListNavigationController = UINavigationController(
            rootViewController: botListViewController
        )
        self.mySettingsNavigationController = UINavigationController(
            rootViewController: settingsViewController
        )
        
        let tabbarItems = [self.channelsNavigationController, self.contactListNavigationController, self.botListNavigationController, self.mySettingsNavigationController]
        self.viewControllers = tabbarItems
        
        self.setupStyles()
        self.loadTotalUnreadMessageCount()
        
        JIM.shared().conversationManager.add(self)
        JIM.shared().connectionManager.add(self)
    }
    
    override func viewDidDisappear(_ animated: Bool) {
        super.viewDidDisappear(animated)
    }
    
    deinit {
        
    }
    
    public func setupStyles() {
        self.theme = SBUTheme.componentTheme
        
        self.tabBar.barTintColor = self.isDarkMode
            ? SBUColorSet.background600
            : .white
        self.tabBar.tintColor = self.isDarkMode
            ? SBUColorSet.primary200
            : SBUColorSet.primary300
        channelsViewController.navigationItem.leftBarButtonItem = self.createLeftTitleItem(
            text: "消息"
        )
        channelsViewController.tabBarItem = self.createTabItem(type: .channels)
        
        contactListViewController.navigationItem.leftBarButtonItem = self.createLeftTitleItem(
            text: "通讯录"
        )
        contactListViewController.tabBarItem = self.createTabItem(type: .contacts)
        
//        groupListViewController.navigationItem.leftBarButtonItem = self.createLeftTitleItem(
//            text: "Groups"
//        )
//        groupListViewController.tabBarItem = self.createTabItem(type: .groups)
        
        botListViewController.navigationItem.leftBarButtonItem = self.createLeftTitleItem(
            text: "智能体"
        )
        botListViewController.tabBarItem = self.createTabItem(type: .bots)
        
        settingsViewController.navigationItem.leftBarButtonItem = self.createLeftTitleItem(
            text: "我"
        )
        settingsViewController.tabBarItem = self.createTabItem(type: .mySettings)
        
        self.channelsNavigationController.navigationBar.barStyle = self.isDarkMode
            ? .black
            : .default
        self.contactListNavigationController.navigationBar.barStyle = self.isDarkMode
            ? .black
            : .default
//        self.groupListNavigationController.navigationBar.barStyle = self.isDarkMode
//            ? .black
//            : .default
        self.botListNavigationController.navigationBar.barStyle = self.isDarkMode
            ? .black
            : .default
        self.mySettingsNavigationController.navigationBar.barStyle = self.isDarkMode
            ? .black
            : .default
    }
    
    // MARK: - SDK related
    func loadTotalUnreadMessageCount() {
        var friendCount: Int32 = 0
        let conversation = JConversation(conversationType: .system, conversationId: GlobalConst.friendConversationId)
        if let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) {
            friendCount = conversationInfo.unreadCount
            self.setFriendBadgeCount(friendCount)
        }
        
        let totalCount = JIM.shared().conversationManager.getTotalUnreadCount()
        let uintCount = UInt(totalCount - friendCount)
        self.setUnreadMessagesCount(uintCount)
    }
    
    // MARK: - Create items
    func createLeftTitleItem(text: String) -> UIBarButtonItem {
        let titleLabel = UILabel()
        titleLabel.text = text
        titleLabel.font = UIFont.systemFont(ofSize: 18.0, weight: .bold)
        titleLabel.textColor = theme.titleColor
        return UIBarButtonItem.init(customView: titleLabel)
    }
    
    func createTabItem(type: TabType) -> UITabBarItem {
        let iconSize = CGSize(width: 24, height: 24)
        let title: String
        let icon: UIImage?
        let tag: Int
        switch type {
        case .channels:
            title = "消息"
            icon = UIImage(named: "iconChatFilled")?.resize(with: iconSize)
            tag = 0
        case .contacts:
            title = "通讯录"
            icon = UIImage(named: "iconMembersCustom")?.resize(with: iconSize)
            tag = 1
//        case .groups:
//            title = "Groups"
//            icon = UIImage(named: "imgGroupchannel")?.resize(with: iconSize)
//            tag = 2
        case .bots:
            title = "智能体"
            icon = UIImage(named: "iconBot")?.resize(with: iconSize)
            tag = 3
        case .mySettings:
            title = "我"
            icon = UIImage(named: "iconSettingsFilled")?.resize(with: iconSize)
            tag = 4
        }
        let item = UITabBarItem(title: title, image: icon, tag: tag)
        return item
    }
    
    
    // MARK: - Common
    func setUnreadMessagesCount(_ totalCount: UInt) {
        var badgeValue: String?
        
        if totalCount == 0 {
            badgeValue = nil
        } else if totalCount > 99 {
            badgeValue = "99+"
        } else {
            badgeValue = "\(totalCount)"
        }
        
        self.channelsViewController.tabBarItem.badgeColor = SBUColorSet.error300
        self.channelsViewController.tabBarItem.badgeValue = badgeValue
        self.channelsViewController.tabBarItem.setBadgeTextAttributes(
            [
                NSAttributedString.Key.foregroundColor : isDarkMode
                    ? SBUColorSet.onlight01
                    : SBUColorSet.ondark01,
                NSAttributedString.Key.font : SBUFontSet.caption4
            ],
            for: .normal
        )
    }
    
    private func setFriendBadgeCount(_ totalCount: Int32) {
        var badgeValue: String?
        
        if totalCount == 0 {
            badgeValue = nil
        } else if totalCount > 99 {
            badgeValue = "99+"
        } else {
            badgeValue = "\(totalCount)"
        }
        
        self.contactListViewController.tabBarItem.badgeColor = SBUColorSet.error300
        self.contactListViewController.tabBarItem.badgeValue = badgeValue
        self.contactListViewController.tabBarItem.setBadgeTextAttributes(
            [
                NSAttributedString.Key.foregroundColor : isDarkMode
                    ? SBUColorSet.onlight01
                    : SBUColorSet.ondark01,
                NSAttributedString.Key.font : SBUFontSet.caption4
            ],
            for: .normal
        )
    }
    
    func updateTheme(isDarkMode: Bool) {
        self.isDarkMode = isDarkMode
        
        self.setupStyles()
        self.channelsViewController.setupStyles()
        self.settingsViewController.setupStyles()

        self.channelsViewController.listComponent?.reloadTableView()
        
        self.loadTotalUnreadMessageCount()
    }
    
    private func updateFriendBadgeCount(_ conversationInfoList: [JConversationInfo]) {
        for i in 0..<conversationInfoList.count {
            let conversationInfo = conversationInfoList[i]
            if conversationInfo.conversation.conversationType == .system
                && conversationInfo.conversation.conversationId == GlobalConst.friendConversationId {
                self.setFriendBadgeCount(conversationInfo.unreadCount)
                NotificationCenter.default.post(name: NSNotification.Name(rawValue: "FriendApplicationCountNtf"), object: Int(conversationInfo.unreadCount))
                break
            }
        }
    }
}

extension MainChannelTabbarController: JConversationDelegate {
    func conversationInfoDidAdd(_ conversationInfoList: [JConversationInfo]!) {
        self.updateFriendBadgeCount(conversationInfoList)
    }
    
    func conversationInfoDidUpdate(_ conversationInfoList: [JConversationInfo]!) {
        self.updateFriendBadgeCount(conversationInfoList)
    }
    
    func conversationInfoDidDelete(_ conversationInfoList: [JConversationInfo]!) {
        
    }
    
    func totalUnreadMessageCountDidUpdate(_ count: Int32) {
        let conversation = JConversation(conversationType: .system, conversationId: GlobalConst.friendConversationId)
        let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation)
        let friendCount = conversationInfo?.unreadCount ?? 0
        let uintCount = UInt(count - friendCount)
        self.setUnreadMessagesCount(uintCount)
    }
}

extension MainChannelTabbarController: JConnectionDelegate {
    func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {
        SBULog.info("connection status change, status is \(status.rawValue)")
        if (status == .disconnected && code == .userKickedByOtherClient) {
            self.dismiss(animated: true)
        }
    }
}
