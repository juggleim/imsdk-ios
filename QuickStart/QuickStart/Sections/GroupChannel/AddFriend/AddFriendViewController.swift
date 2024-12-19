//
//  AddFriendViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/1.
//

import Foundation
import UIKit
import JuggleIM

class AddFriendViewController: BaseTableListViewController {
    var users: [JCUser]?
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "Add Friend"
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(
            AddFriendUserCell.self,
            forCellReuseIdentifier: AddFriendUserCell.sbu_className
        )
        if let users = users {
            if users.isEmpty {
                self.emptyView.reloadData(.noMembers)
            }
        } else {
            self.emptyView.reloadData(.noMembers)
        }
    }
    
//    private func addFriend(_ userId: String, _ completion: @escaping (Bool) -> Void) {
//        HttpManager.shared.addFriend(userId: userId) { code in
//            if code == HttpManager.success {
//                completion(true)
//            } else {
//                completion(false)
//            }
//        }
//    }
    
    private func applyFriend(_ userId: String, _ completion: @escaping (Bool) -> Void) {
        HttpManager.shared.applyFriend(userId: userId) { code in
            if code == HttpManager.success {
                completion(true)
            } else {
                completion(false)
            }
        }
    }
}

extension AddFriendViewController: UITableViewDataSource, UITableViewDelegate {
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
        let cell = tableView.dequeueReusableCell(withIdentifier: AddFriendUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let userCell = cell as? AddFriendUserCell, let user = self.users?[indexPath.row] {
            userCell.configure(
                type: .addFriend,
                user: user,
                isChecked: user.isFriend
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.users?[indexPath.row],
              let defaultCell = self.tableView.cellForRow(at: indexPath)
                as? AddFriendUserCell else { return }
        
        if user.isFriend {
            let conversation = JConversation(conversationType: .private, conversationId: user.userId)
            let defaultConversationInfo = JConversationInfo()
            defaultConversationInfo.conversation = conversation
            let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) ?? defaultConversationInfo
            self.tabBarController?.tabBar.isHidden = true
            let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
            self.navigationController?.pushViewController(channelVC, animated: true)
            return
        }
        self.loadingIndicator.startAnimating()
        self.view.isUserInteractionEnabled = false
        applyFriend(user.userId) { isSuccess in
            DispatchQueue.main.async {
                self.loadingIndicator.stopAnimating()
                self.view.isUserInteractionEnabled = true
                if isSuccess {
                    user.isFriend = true
                    defaultCell.selectUser(true)
                    self.navigationController?.popViewController(animated: true)
                }
            }
        }
    }
}
