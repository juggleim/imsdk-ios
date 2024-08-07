//
//  ChannelListViewController.swift
//  SendbirdUIKit-Sample
//
//  Created by Tez Park on 2020/09/14.
//  Copyright © 2020 SendBird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

class ChannelListViewController: SBUGroupChannelListViewController {

    lazy var rightBarButton: UIBarButtonItem = {
        let rightItem =  UIBarButtonItem(
            title: "Menu",
            style: .plain,
            target: self,
            action: #selector(onClickMenu)
        )
        rightItem.setTitleTextAttributes([.font : SBUFontSet.button2], for: .normal)
        return rightItem
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.navigationItem.rightBarButtonItem = rightBarButton
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        self.tabBarController?.tabBar.isHidden = true
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.tabBarController?.tabBar.isHidden = false
    }
    
    override func showChannel(conversationInfo: JConversationInfo) {
        let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
    
    @objc func onClickMenu() {
        let searchUserItem = SBUActionSheetItem(
            title: "Search user by phone number",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let createGroupItem = SBUActionSheetItem(
            title: "Create new group",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let cancelItem = SBUActionSheetItem(
            title: SBUStringSet.Cancel,
            color: SBUTheme.channelSettingsTheme.itemColor
        ) {}
        SBUActionSheet.show(
            items: [searchUserItem, createGroupItem],
            cancelItem: cancelItem,
            delegate: self
        )
    }
    
    func searchUser() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] phoneNumber in
            guard let phoneNumber = phoneNumber as? String else { return }
            HttpManager.shared.searchUser(phoneNumber: phoneNumber) { code, jcUser in
                DispatchQueue.main.async {
                    let addFriendVC = AddFriendViewController()
                    if let jcUser = jcUser {
                        addFriendVC.users = [jcUser]
                    }
                    self?.navigationController?.pushViewController(addFriendVC, animated: true)
                }
            }
        }
        let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
        SBUAlertView.show(
            title: "Search user by phone number",
            needInputField: true,
            placeHolder: "Enter phone number",
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
    
    func createGroup() {
        let createGroupVC = CreateGroupViewController()
        self.navigationController?.pushViewController(createGroupVC, animated: true)
    }
}

extension ChannelListViewController: SBUActionSheetDelegate {
    public func didSelectActionSheetItem(index: Int, identifier: Int) {
        switch index {
        case 0:
            self.searchUser()
        case 1:
            self.createGroup()
        default:
            break
        }
    }
}
