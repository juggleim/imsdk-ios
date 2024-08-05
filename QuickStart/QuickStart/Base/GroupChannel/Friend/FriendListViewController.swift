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
    
    override func loadView() {
        super.loadView()
        loadFriends()
    }
    
    open override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.tabBarController?.tabBar.isHidden = false
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
                    self.emptyView.reloadData(.noMembers)
                }
            }
        }
    }
}

extension FriendListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.users?.count ?? 0
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: BaseUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let userCell = cell as? BaseUserCell, let user = self.users?[indexPath.row] {
            userCell.configure(
                type: .friendList,
                user: user,
                isChecked: user.isFriend
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.users?[indexPath.row] else {
            return
        }
        let conversation = JConversation(conversationType: .private, conversationId: user.userId)
        let defaultConversationInfo = JConversationInfo()
        defaultConversationInfo.conversation = conversation
        let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) ?? defaultConversationInfo
        let channelVC = SBUViewControllerSet.GroupChannelViewController.init(conversationInfo: conversationInfo)
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
}

