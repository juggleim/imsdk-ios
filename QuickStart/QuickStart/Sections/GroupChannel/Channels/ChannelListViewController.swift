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
            image: SBUIconSetType.iconCreate.image(to: SBUIconSetType.Metric.defaultIconSize),
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
    
    override func showChannel(conversationInfo: JConversationInfo) {
        let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
        channelVC.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
    
    @objc func onClickMenu() {
        let searchUserItem = SBUActionSheetItem(
            title: NSLocalizedString("Add Friend", comment: ""),
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let createGroupItem = SBUActionSheetItem(
            title: "CreateGroups",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let scanItem = SBUActionSheetItem(
            title: NSLocalizedString("Scan QR Code", comment: ""),
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let cancelItem = SBUActionSheetItem(
            title: NSLocalizedString("Cancel", comment: ""),
            color: SBUTheme.channelSettingsTheme.itemColor
        ) {}
        SBUActionSheet.show(
            items: [searchUserItem, createGroupItem, scanItem],
            cancelItem: cancelItem,
            delegate: self
        )
    }
    
    func searchUser() {
        let okButton = SBUAlertButtonItem(title: "Confirm") {[weak self] phoneNumber in
            guard let phoneNumber = phoneNumber as? String else { return }
            HttpManager.shared.searchUser(phoneNumber: phoneNumber) { code, jcUser in
                DispatchQueue.main.async {
                    let addFriendVC = AddFriendViewController()
                    if let jcUser = jcUser {
                        addFriendVC.users = [jcUser]
                    }
                    addFriendVC.hidesBottomBarWhenPushed = true
                    self?.navigationController?.pushViewController(addFriendVC, animated: true)
                }
            }
        }
        let cancelButton = SBUAlertButtonItem(title: NSLocalizedString("Cancel", comment: "")) { _ in }
        SBUAlertView.show(
            title: NSLocalizedString("Add Friend", comment: ""),
            needInputField: true,
            placeHolder: "Enter phone number",
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
    
    func createGroup() {
        let createGroupVC = CreateGroupViewController()
        createGroupVC.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(createGroupVC, animated: true)
    }
    
    func scan() {
        let vc = ScanQRCodeViewController()
        vc.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(vc, animated: true)
    }
}

extension ChannelListViewController: SBUActionSheetDelegate {
    public func didSelectActionSheetItem(index: Int, identifier: Int) {
        switch index {
        case 0:
            self.searchUser()
        case 1:
            self.createGroup()
        case 2:
            self.scan()
        default:
            break
        }
    }
}
