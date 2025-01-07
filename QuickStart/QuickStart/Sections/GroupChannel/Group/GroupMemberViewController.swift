//
//  GroupMemberViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/24.
//

import Foundation
import UIKit

public enum GroupMemberVCSelectType {
    // 点击进入会话页面
    case chat
    // 点击回调
    case callback
}

public protocol GroupMemberVCDelegate: AnyObject {
    func memberDidSelect(_ member: JUserInfo)
}

public class GroupMemberViewController: BaseTableListViewController {
    var groupId: String = ""
    var type: GroupMemberVCSelectType = .chat
    var users: [JUserInfo] = []
    var includeSelf: Bool = true
    weak var delegate: GroupMemberVCDelegate?
    
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
        self.titleView.text = "群成员"
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
                    if var userInfoList = userInfoList, errorCode == 0 {
                        if !self.includeSelf {
                            for (index, user) in userInfoList.enumerated() {
                                if user.userId == JIM.shared().currentUserId {
                                    userInfoList.remove(at: index)
                                    break
                                }
                            }
                        }
            
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
        let user = self.users[indexPath.row]
        if self.type == .chat {
            let vc = PersonDetailViewController()
            vc.userId = user.userId
            self.navigationController?.pushViewController(vc, animated: true)
        } else if self.type == .callback {
            self.delegate?.memberDidSelect(user)
            self.navigationController?.popViewController(animated: true)
        }
    }
}
