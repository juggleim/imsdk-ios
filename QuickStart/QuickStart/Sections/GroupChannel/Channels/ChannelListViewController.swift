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
            title: "添加好友",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let createGroupItem = SBUActionSheetItem(
            title: "创建群组",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let scanItem = SBUActionSheetItem(
            title: "扫一扫",
            color: SBUTheme.channelSettingsTheme.itemTextColor,
            image: nil
        ) {}
        let cancelItem = SBUActionSheetItem(
            title: "取消",
            color: SBUTheme.channelSettingsTheme.itemColor
        ) {}
        SBUActionSheet.show(
            items: [searchUserItem, createGroupItem, scanItem],
            cancelItem: cancelItem,
            delegate: self
        )
    }
    
    func searchUser() {
        let okButton = SBUAlertButtonItem(title: "确认") {[weak self] phoneNumber in
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
        let cancelButton = SBUAlertButtonItem(title: "取消") { _ in }
        SBUAlertView.show(
            title: "添加好友",
            needInputField: true,
            placeHolder: "输入手机号码",
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
    
    func createGroup() {
        let createGroupVC = CreateGroupViewController()
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
