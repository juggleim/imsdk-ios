//
//  SelectFriendViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/27.
//

import Foundation

protocol SelectFriendVCDelegate: AnyObject {
    func friendsDidSelect(_: [JCUser])
}

class SelectFriendViewController: BaseTableListViewController {
    var users: [JCUser] = []
    var existedUsers: [JCUser] = []
    var selectedUsers: Set<JCUser> = []
    weak var delegate: SelectFriendVCDelegate?
    
    override func loadView() {
        super.loadView()
        loadFriends()
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(BaseUserCell.self, forCellReuseIdentifier: BaseUserCell.sbu_className)
    }
    
    override func configNavigationItem() {
        super.configNavigationItem()
        let rightItem =  UIBarButtonItem(
            title: "Select",
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
        self.navigationController?.popViewController(animated: true)
        self.delegate?.friendsDidSelect(Array(selectedUsers))
    }
    
    private func updateMenu() {
        if selectedUsers.isEmpty {
            self.navigationItem.rightBarButtonItem?.isEnabled = false
        } else {
            self.navigationItem.rightBarButtonItem?.isEnabled = true
        }
    }
    
    private func loadFriends() {
        HttpManager.shared.getFriends { code, users in
            DispatchQueue.main.async {
                if let users = users {
                    self.users = users
                    self.tableView.reloadData()
                }
            }
        }
    }
}

extension SelectFriendViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.users.count
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: BaseUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        let user = self.users[indexPath.row]
        let jcUser = JCUser()
        jcUser.userId = user.userId
        jcUser.userName = user.userName
        jcUser.portrait = user.portrait
        if let userCell = cell as? BaseUserCell {
            userCell.configure(
                type: .createGroup,
                user: jcUser
            )
            var exist = false
            for existedUser in self.existedUsers {
                if existedUser.userId == user.userId {
                    exist = true
                    break
                }
            }
            if exist {
                userCell.isUserInteractionEnabled = false
                userCell.checkboxButton.isEnabled = false
            }
        }
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let user = self.users[indexPath.row]
        guard let defaultCell = self.tableView.cellForRow(at: indexPath)
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
