//
//  FriendListViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/2.
//

import Foundation
import UIKit
import JuggleIM

class FriendListViewController: BaseTableListViewController {
    var users: [JCUser]?
    
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
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        self.tabBarController?.tabBar.isHidden = false
        loadFriends()
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(
            BaseUserCell.self,
            forCellReuseIdentifier: BaseUserCell.sbu_className
        )
    }
    
    private func loadFriends() {
        HttpManager.shared.getFriends { code, friends in
            DispatchQueue.main.async {
                self.users = friends
                if let friends = friends, !friends.isEmpty {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.none)
                } else {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.noMembers)
                }
            }
        }
    }
    
    @objc func onClickMenu() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] phoneNumber in
            guard let phoneNumber = phoneNumber as? String else { return }
            HttpManager.shared.searchUser(phoneNumber: phoneNumber) { code, jcUser in
                DispatchQueue.main.async {
                    self?.tabBarController?.tabBar.isHidden = true
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
            title: "Add friend",
            needInputField: true,
            placeHolder: "Enter phone number",
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
}

extension FriendListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        5
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        2
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if section == 0 {
            return 1
        }
        return self.users?.count ?? 0
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {

        var user: JCUser
        
        if indexPath.section == 0 {
            user = JCUser()
            user.userId = "New Friends"
            user.userName = "New Friends"
            
            var cell = NewFriendsUserCell()
            cell.selectionStyle = .none
            cell.configure(type: .friendList, user: user)
            return cell
        } else {
            var cell = tableView.dequeueReusableCell(withIdentifier: BaseUserCell.sbu_className)
            cell?.selectionStyle = .none
            user = self.users?[indexPath.row] ?? JCUser()
            if let userCell = cell as? BaseUserCell {
                userCell.configure(
                    type: .friendList,
                    user: user,
                    isChecked: user.isFriend
                )
            }
            return cell ?? UITableViewCell()
        }
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.section == 1 {
            guard let user = self.users?[indexPath.row] else {
                return
            }
            let conversation = JConversation(conversationType: .private, conversationId: user.userId)
            let defaultConversationInfo = JConversationInfo()
            defaultConversationInfo.conversation = conversation
            let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) ?? defaultConversationInfo
            self.tabBarController?.tabBar.isHidden = true
            let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
            self.navigationController?.pushViewController(channelVC, animated: true)
        } else if indexPath.section == 0 {
            self.tabBarController?.tabBar.isHidden = true
            let vc = FriendApplicationListViewController()
            self.navigationController?.pushViewController(vc, animated: true)
        }
    }
    
    func tableView(_ tableView: UITableView, trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath) -> UISwipeActionsConfiguration? {
        if indexPath.section == 1 {
            guard let user = self.users?[indexPath.row] else {
                return nil
            }
            var actions: [UIContextualAction] = []
            let deleteAction = UIContextualAction(style: .destructive, title: "删除") { _, _, actionHandler in
                HttpManager.shared.deleteFriends(userIds: [user.userId]) { code in
                    if code == 0 {
                        self.loadFriends()
                    } else {
                        SBULog.error("delete friend error, code is \(code)")
                    }
                }
            }
            actions.append(deleteAction)
            return UISwipeActionsConfiguration(actions: actions)
        }
        return nil
    }
    
}

