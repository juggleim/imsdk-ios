//
//  SelectUserViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/1.
//

import Foundation
import UIKit

class SelectUserViewController: UIViewController {
    var users: [JCUser]?
    var selectedUsers: Set<JCUser> = []
    let tableView = UITableView()
    let emptyView = SBUEmptyView()
    
    override func loadView() {
        super.loadView()
        configNavigationItem()
        configTableView()
        configEmptyView()
    }
    
    private func configNavigationItem() {
        let titleView = SBUNavigationTitleView()
        titleView.text = "Select members"
        titleView.textAlignment = .center
        self.navigationItem.titleView = titleView
        
        let addFriendButton = UIBarButtonItem(
            title: "Add Friend",
            style: .plain,
            target: self,
            action: #selector(onTapRightBarButton)
        )
        addFriendButton.setTitleTextAttributes([.font: SBUFontSet.button2], for: .normal)
        self.navigationItem.rightBarButtonItem = addFriendButton
    }
    
    private func configTableView() {
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.bounces = false
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.backgroundView = self.emptyView
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 44.0
        self.tableView.sectionHeaderHeight = 0
        self.tableView.register(
            SelectUserCell.self,
            forCellReuseIdentifier: SelectUserCell.sbu_className
        )
        self.view.addSubview(self.tableView)
        self.tableView.frame = self.view.frame
        if let users = users {
            if users.isEmpty {
                self.emptyView.reloadData(.noMembers)
                self.navigationItem.rightBarButtonItem = nil
            }
        } else {
            self.emptyView.reloadData(.noMembers)
            self.navigationItem.rightBarButtonItem = nil
        }
    }
    
    private func configEmptyView() {
        emptyView.type = EmptyViewType.none
    }
    
    private func isSelectedUser(_ user: JCUser) -> Bool {
        return self.selectedUsers.contains(where: { $0.userId == user.userId })
    }
    
    private func updateRightBarButton() {
        if self.selectedUsers.count > 0 {
            self.configNavigationItem()
        } else {
            self.navigationItem.rightBarButtonItem = nil
        }
    }
    
    @objc private func onTapRightBarButton() {
        
    }
}

extension SelectUserViewController: UITableViewDataSource, UITableViewDelegate {
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
        let cell = tableView.dequeueReusableCell(withIdentifier: SelectUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let userCell = cell as? SelectUserCell, let user = self.users?[indexPath.row] {
            userCell.configure(
                type: .createChannel,
                user: user
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.users?[indexPath.row],
              let defaultCell = self.tableView.cellForRow(at: indexPath)
                as? SelectUserCell else { return }
        
        if let index = self.selectedUsers.firstIndex(of: user) {
            self.selectedUsers.remove(at: index)
        } else {
            self.selectedUsers.insert(user)
        }
        
        let isSelected = self.isSelectedUser(user)
        defaultCell.selectUser(isSelected)
        
        self.updateRightBarButton()
    }
}
