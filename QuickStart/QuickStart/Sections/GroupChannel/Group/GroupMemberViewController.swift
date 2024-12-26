//
//  GroupMemberViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/24.
//

import Foundation
import UIKit

public class GroupMemberViewController: BaseTableListViewController {
    var groupId: String = ""
    var users: [JUserInfo] = []
    
    public override func loadView() {
        super.loadView()
        loadGroupMembers()
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
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton

    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
    
    private func loadGroupMembers() {
        HttpManager.shared.getGroupMembers(
            groupId: groupId,
            count: 100) { errorCode, resultOffset, userInfoList in
                DispatchQueue.main.async {
                    if let userInfoList = userInfoList, errorCode == 0 {
                        self.users = userInfoList
                        self.tableView.reloadData()
                    }
                }
            }
    }
}

extension GroupMemberViewController: UITableViewDataSource, UITableViewDelegate {
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
                type: .members,
                user: jcUser
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
    }
}
