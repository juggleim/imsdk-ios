//
//  SelectSingleFriendViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation

protocol SelectSingleFriendVCDelegate: AnyObject {
    func friendDidSelect(_: JCUser)
}

class SelectSingleFriendViewController: BaseTableListViewController {
    var users: [JCUser] = []
    weak var delegate: SelectSingleFriendVCDelegate?
    
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

extension SelectSingleFriendViewController: UITableViewDataSource, UITableViewDelegate {
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
                type: .friendList,
                user: jcUser
            )
        }
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let user = self.users[indexPath.row]
        self.navigationController?.popViewController(animated: true)
        self.delegate?.friendDidSelect(user)
    }
}
