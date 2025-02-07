//
//  GroupMemberSelectViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

import Foundation
import UIKit
import JuggleIM

@objc public protocol GroupMemberSelectVCDelegate: AnyObject {
    func membersDidSelect(type: GroupMemberSelectType, members: [JUserInfo])
}

@objc public enum GroupMemberSelectType: Int {
    case unknown
    case voiceCall
    case videoCall
}

@objc public class GroupMemberSelectViewController: BaseTableListViewController {
    @objc var groupId: String = ""
    @objc var users: [JUserInfo] = []
    @objc var existedUsers: [JUserInfo] = []
    @objc var selectedUsers: Set<JUserInfo> = []
    @objc var type: GroupMemberSelectType = .unknown
    @objc weak var delegate: GroupMemberSelectVCDelegate?
    
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
        if self.type == .videoCall || self.type == .voiceCall {
            self.dismiss(animated: true)
        } else {
            self.navigationController?.popViewController(animated: true)
        }
        self.delegate?.membersDidSelect(type: self.type, members: Array(selectedUsers))
    }
    
    private func updateMenu() {
        if selectedUsers.isEmpty {
            self.navigationItem.rightBarButtonItem?.isEnabled = false
        } else {
            self.navigationItem.rightBarButtonItem?.isEnabled = true
        }
    }
    
    private func loadGroupMembers() {
        HttpManager.shared.getGroupMembers(
            groupId: groupId,
            count: 100) { errorCode, resultOffset, userInfoList in
                DispatchQueue.main.async {
                    if var userInfoList = userInfoList, errorCode == 0 {
                        for (index, user) in userInfoList.enumerated() {
                            if user.userId == JIM.shared().currentUserId {
                                userInfoList.remove(at: index)
                                break
                            }
                        }
                        self.users = userInfoList
                        self.tableView.reloadData()
                    }
                }
            }
    }
}

extension GroupMemberSelectViewController: UITableViewDataSource, UITableViewDelegate {
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
