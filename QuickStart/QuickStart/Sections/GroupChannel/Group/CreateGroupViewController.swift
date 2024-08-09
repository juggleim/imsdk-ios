//
//  CreateGroupViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/6.
//

import Foundation

import UIKit
import JuggleIM

class CreateGroupViewController: BaseTableListViewController {
    var users: [JCUser]?
    var selectedUsers: Set<JCUser> = []
    
    override func loadView() {
        super.loadView()
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
    
    override func configNavigationItem() {
        super.configNavigationItem()
        let rightItem =  UIBarButtonItem(
            title: "Create",
            style: .plain,
            target: self,
            action: #selector(onClickMenu)
        )
        rightItem.setTitleTextAttributes([.font : SBUFontSet.button2], for: .normal)
        self.navigationItem.rightBarButtonItem = rightItem
        if selectedUsers.isEmpty {
            self.navigationItem.rightBarButtonItem?.isEnabled = false
        }
    }
    
    @objc func onClickMenu() {
        self.loadingIndicator.startAnimating()
        self.view.isUserInteractionEnabled = false
        let currentName = ProfileManager.shared.currentUserInfo?.userName
        var groupName: String = currentName ?? ""
        groupName = groupName.appending(", ")
        
        if selectedUsers.isEmpty {
            return
        }
        
        var members: [String] = []
        for user in selectedUsers {
            if let name = user.userName {
                groupName = groupName.appending(name).appending(", ")
            }
            members.append(user.userId)
        }
        groupName.removeLast()
        groupName.removeLast()
        if groupName.count > 30 {
            groupName = String(groupName.prefix(30))
        }
        
        HttpManager.shared.createGroup(name: groupName, members: members) { code, groupInfo in
            DispatchQueue.main.async {
                self.loadingIndicator.stopAnimating()
                self.view.isUserInteractionEnabled = true
                if code == HttpManager.success {
                    self.navigationController?.popViewController(animated: true)
                }
            }
        }
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
    
    private func updateMenu() {
        if selectedUsers.isEmpty {
            self.navigationItem.rightBarButtonItem?.isEnabled = false
        } else {
            self.navigationItem.rightBarButtonItem?.isEnabled = true
        }
    }
}

extension CreateGroupViewController: UITableViewDataSource, UITableViewDelegate {
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
                type: .createGroup,
                user: user
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.users?[indexPath.row],
              let defaultCell = self.tableView.cellForRow(at: indexPath)
                as? BaseUserCell else { return }
        
        let isSelect: Bool
        if let index = self.selectedUsers.firstIndex(of: user) {
            self.selectedUsers.remove(at: index)
            isSelect = false
        } else {
            self.selectedUsers.insert(user)
            isSelect = true
        }
        defaultCell.selectUser(isSelect)
        updateMenu()
    }
}
