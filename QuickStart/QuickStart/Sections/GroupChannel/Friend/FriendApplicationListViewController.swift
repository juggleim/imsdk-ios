//
//  FriendApplicationListViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/18.
//

import Foundation
import UIKit
import JuggleIM

class FriendApplicationListViewController: BaseTableListViewController {
    var users: [JCUser]?
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "New Friends"
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        loadFriendApplications()
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(FriendApplicationUserCell.self, forCellReuseIdentifier: FriendApplicationUserCell.sbu_className)
        if let users = users {
            if users.isEmpty {
                self.emptyView.reloadData(.noMembers)
            }
        } else {
            self.emptyView.reloadData(.noMembers)
        }
    }
    
    private func loadFriendApplications() {
        let conversation = JConversation(conversationType: .system, conversationId: GlobalConst.friendConversationId)
        JIM.shared().conversationManager.clearUnreadCount(by: conversation, success: nil, error: nil)
        HttpManager.shared.getFriendsApplications(start: 0, count: 100) { code, applications in
            DispatchQueue.main.async {
                self.users = applications
                if let applications = applications, !applications.isEmpty {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.none)
                } else {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.noMembers)
                }
            }
        }
    }
}

extension FriendApplicationListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.users?.count ?? 0
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: FriendApplicationUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let userCell = cell as? FriendApplicationUserCell, let user = self.users?[indexPath.row] {
            userCell.configure(
                type: .friendApplication,
                user: user,
                isChecked: user.isFriend
            )
        }
        
        return cell ?? UITableViewCell()
    }
}
