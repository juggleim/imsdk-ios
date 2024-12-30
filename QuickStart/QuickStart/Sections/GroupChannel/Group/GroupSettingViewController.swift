//
//  GroupSettingViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/23.
//

import Foundation
import JuggleIM

class GroupSettingViewController: BaseTableListViewController {
    var conversationInfo: JConversationInfo?
    var groupInfo: JCGroupInfo?
    var deleteButton: UIButton?
    lazy var headerView: UserListCollectionView = {
        let rect: CGRect = CGRect(x: 0, y: 0, width: Int(UIScreen.main.bounds.size.width), height: 0)
        let header = UserListCollectionView(frame: rect)
        header.groupId = self.conversationInfo?.conversation.conversationId ?? ""
        header.userListCollectionViewDelegate = self
        return header
    } ()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setHeaderView()
        setFooterView()
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        loadGroupInfo()
    }
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "群组信息"
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
    
    private func loadGroupInfo() {
        if let groupId = self.conversationInfo?.conversation.conversationId {
            HttpManager.shared.getGroupInfo(groupId: groupId) { code, groupInfo in
                if code == 0 {
                    self.groupInfo = groupInfo
                    DispatchQueue.main.async {
                        self.tableView.reloadData()
                        self.setHeaderView()
                    }
                }
            }
        }
    }
    
    private func setHeaderView() {
        self.headerView.isAllowAdd = true
        var hasDelete: Bool = false
        if self.groupInfo?.myRole == .admin || self.groupInfo?.myRole == .owner {
            hasDelete = true
        }
        self.headerView.isAllowDelete = hasDelete
        if let userList = self.groupInfo?.members {
            self.headerView.reloadData(userList)
        }
        self.headerView.frame = CGRect(x: 0, y: 0, width: self.view.bounds.size.width, height: self.headerView.collectionViewLayout.collectionViewContentSize.height)
        var frame: CGRect = self.headerView.frame
        frame.size.height += 14
        self.tableView.tableHeaderView = UIView(frame: frame)
        self.tableView.tableHeaderView?.addSubview(self.headerView)
        
        let separatorLine = UIView(frame: CGRect(x: 10, y: frame.size.height - 1, width: frame.size.width - 10, height: 1))
        separatorLine.backgroundColor = UIColor(red: 0xf0/255.0, green: 0xf0/255.0, blue: 0xf6/255.0, alpha: 1.0)
        self.tableView.tableHeaderView?.addSubview(separatorLine)
    }
    
    private func setFooterView() {
        let view = UIView(frame: CGRect(x: 0, y: 0, width: self.view.bounds.size.width, height: 150))
        let button = UIButton(frame: CGRect(x: 10, y: 29, width: self.view.bounds.size.width-20.0, height: 42))
        button.backgroundColor = UIColor(red: 235.0 / 255.0, green: 70.0 / 255.0, blue: 72.0 / 255.0, alpha: 1.0)
        button.setTitle("退出并删除", for: .normal)
        button.layer.cornerRadius = 5.f
        button.layer.borderWidth = 0.5
        button.layer.borderColor = UIColor(red: 0xcc / 255.0, green: 0x44 / 255.0, blue: 0x45 / 255.0, alpha: 1.0).cgColor
        view.addSubview(button)
        button.addTarget(self, action: #selector(onDelete), for: .touchUpInside)
        
        self.deleteButton = button
        self.tableView.tableFooterView = view
    }
    
    @objc func onDelete() {
        if let groupId = self.conversationInfo?.conversation.conversationId {
            HttpManager.shared.quitGroup(groupId: groupId) { code in
                if let conversation = self.conversationInfo?.conversation {
                    JIM.shared().conversationManager.deleteConversationInfo(by: conversation) {
                        self.navigationController?.popToRootViewController(animated: true)
                    } error: { errorCode in
                    }

                }
            }
        }
    }
}

extension GroupSettingViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        4
    }
    
    func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        5
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if section == 0 {
            return 1
        } else if section == 1 {
            if self.groupInfo?.myRole == .owner || self.groupInfo?.myRole == .admin {
                return 4
            } else {
                return 3
            }
        } else if section == 2 {
            return 2
        } else if section == 3 {
            return 1
        }
        return 0
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        if indexPath.section == 0 {
            if indexPath.row == 0 {
                // 群成员
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "群成员"
                if let count = self.groupInfo?.memberCount, count > 0 {
                    cell.rightLabel.text = "\(count)"
                }
                return cell
            }
        } else if indexPath.section == 1 {
            if indexPath.row == 0 {
                // 群名称
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "群名称"
                cell.rightLabel.text = self.groupInfo?.groupName ?? ""
                return cell
            } else if indexPath.row == 1 {
                // 群公告
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "群公告"
                return cell
            } else if indexPath.row == 2 {
                // 群昵称
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "我在本群的昵称"
                cell.rightLabel.text = self.groupInfo?.groupDisplayName ?? ""
                return cell
            } else if indexPath.row == 3 {
                // 群管理
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "群管理"
                return cell
            }
        } else if indexPath.section == 2 {
            if indexPath.row == 0 {
                // 消息免打扰
                let cell = getSwitchCell()
                cell.leftLabel.text = "消息免打扰"
                cell.switchButton.addTarget(self, action: #selector(onSetMute(_:)), for: .valueChanged)
                if let mute = self.conversationInfo?.mute {
                    cell.switchButton.isOn = mute
                }
                return cell
            } else if indexPath.row == 1 {
                // 会话置顶
                let cell = getSwitchCell()
                cell.leftLabel.text = "会话置顶"
                cell.switchButton.addTarget(self, action: #selector(onSetTop(_:)), for: .valueChanged)
                if let top = self.conversationInfo?.isTop {
                    cell.switchButton.isOn = top
                }
                return cell
            }
        } else if indexPath.section == 3 {
            if indexPath.row == 0 {
                // 清除聊天记录
                let cell = getRightLabelArrowCell()
                cell.leftLabel.text = "清除聊天记录"
                return cell
            }
        }
        return UITableViewCell()
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.section == 0 {
            if indexPath.row == 0 {
                selectGroupMember()
            }
        } else if indexPath.section == 1 {
            if indexPath.row == 0 {
                updateGroupName()
            } else if indexPath.row == 1 {
                groupAnnouncement()
            } else if indexPath.row == 2 {
                groupNickname()
            } else if indexPath.row == 3 {
                groupManage()
            }
        } else if indexPath.section == 3 {
            if indexPath.row == 0 {
                clearMessages()
            }
        }
    }
    
    private func selectGroupMember() {
        let vc = GroupMemberViewController()
        vc.groupId = self.conversationInfo?.conversation.conversationId ?? ""
        self.navigationController?.pushViewController(vc, animated: true)
    }
    
    private func groupNickname() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] newNickname in
            guard let self = self else { return }
            guard let newNickname = newNickname as? String else { return }
            
            let trimmedNickname = newNickname.trimmingCharacters(in: .whitespacesAndNewlines)
            guard trimmedNickname.count > 0 else { return }
            
            if let groupId = self.conversationInfo?.conversation.conversationId {
                HttpManager.shared.setGroupDisplayName(groupId: groupId, displayName: trimmedNickname) { code in
                    DispatchQueue.main.async {
                        self.groupInfo?.groupDisplayName = trimmedNickname
                        self.tableView.reloadData()
                    }
                }
            }
        }
        let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
        
        SBUAlertView.show(
            title: SBUStringSet.ChannelSettings_Change_Name,
            needInputField: true,
            placeHolder: SBUStringSet.ChannelSettings_Enter_New_Name,
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton,
            delegate: self
        )
    }
    
    private func updateGroupName() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] newChannelName in
            guard let self = self else { return }
            guard let newChannel = newChannelName as? String else { return }
            
            let trimmedChannelName = newChannel.trimmingCharacters(in: .whitespacesAndNewlines)
            guard trimmedChannelName.count > 0 else { return }
            
            if let groupId = self.conversationInfo?.conversation.conversationId {
                HttpManager.shared.updateGroup(groupId: groupId, name: trimmedChannelName) { code in
                    DispatchQueue.main.async {
                        self.groupInfo?.groupName = trimmedChannelName
                        self.tableView.reloadData()
                    }
                }
            }
        }
        let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
        
        SBUAlertView.show(
            title: SBUStringSet.ChannelSettings_Change_Name,
            needInputField: true,
            placeHolder: SBUStringSet.ChannelSettings_Enter_New_Name,
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton,
            delegate: self
        )
    }
    
    private func groupAnnouncement() {
        let vc = GroupAnnouncementViewController()
        vc.groupId = self.conversationInfo?.conversation.conversationId ?? ""
        self.navigationController?.pushViewController(vc, animated: true)
    }
    
    private func groupManage() {
        let vc = GroupManageViewController()
        vc.groupId = self.conversationInfo?.conversation.conversationId ?? ""
        vc.mute = self.groupInfo?.mute ?? 0
        vc.historyMessageVisible = self.groupInfo?.historyMessageVisible ?? 0
        self.navigationController?.pushViewController(vc, animated: true)
    }
    
    private func clearMessages() {
        SBULoading.start()
        JIM.shared().messageManager.clearMessages(in: self.conversationInfo?.conversation, startTime: 0) {
            SBULoading.stop()
        } error: { code in
            SBULoading.stop()
        }
    }
    
    private func getArrowCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.DefaultStyle)
        return cell
    }
    
    private func getRightLabelArrowCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.RightLabelStyle)
        return cell
    }
    
    private func getSwitchCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.SwitchStyle)
        return cell
    }
    
    @objc func onSetMute(_ sender: UISwitch) {
        SBULoading.start()
        JIM.shared().conversationManager.setMute(sender.isOn, conversation: self.conversationInfo?.conversation) {
            SBULoading.stop()
        } error: { code in
            SBULoading.stop()
        }
    }
    
    @objc func onSetTop(_ sender: UISwitch) {
        SBULoading.start()
        JIM.shared().conversationManager.setTop(sender.isOn, conversation: self.conversationInfo?.conversation) {
            SBULoading.stop()
        } error: { code in
            SBULoading.stop()
        }
    }
}

extension GroupSettingViewController: SBUAlertViewDelegate {
    func didDismissAlertView() {
    }
}

extension GroupSettingViewController: UserListCollectionViewDelegate {
    func addButtonDidClick() {
        let vc = SelectFriendViewController()
        if let members = self.groupInfo?.members {
            vc.existedUsers = members
        }
        vc.delegate = self
        self.navigationController?.pushViewController(vc, animated: true)
    }
    
    func deleteButtonDidClick() {
        if let groupId = self.groupInfo?.groupId {
            let vc = GroupMemberSelectViewController()
            vc.groupId = groupId
            vc.delegate = self
            self.navigationController?.pushViewController(vc, animated: true)
        }
    }
    
    func userDidClick(_ userId: String) {
        
    }
}

extension GroupSettingViewController: SelectFriendVCDelegate {
    func friendsDidSelect(_ friends: [JCUser]) {
        var userIds: [String] = []
        for user in friends {
            userIds.append(user.userId)
        }
        guard let groupId = self.groupInfo?.groupId else {
            return
        }
        SBULoading.start()
        HttpManager.shared.groupInvite(groupId: groupId, userIdList: userIds) { code in
            DispatchQueue.main.async {
                SBULoading.stop()
                self.loadGroupInfo()
            }
        }
    }
}

extension GroupSettingViewController: GroupMemberSelectVCDelegate {
    func membersDidSelect(type: GroupMemberSelectType, members: [JUserInfo]) {
        var userIds: [String] = []
        for user in members {
            userIds.append(user.userId)
        }
        guard let groupId = self.groupInfo?.groupId else {
            return
        }
        SBULoading.start()
        HttpManager.shared.deleteGroupMembers(groupId: groupId, userIdList: userIds) { code in
            DispatchQueue.main.async {
                SBULoading.stop()
                self.loadGroupInfo()
            }
        }
    }
}
