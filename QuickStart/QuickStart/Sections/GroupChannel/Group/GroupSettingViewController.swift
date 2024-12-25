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
    
    override func viewDidLoad() {
        super.viewDidLoad()
        //TODO: loadGroupInfo
    }
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "群组信息"
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
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
            //TODO: 非管理员 2
            return 3
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
                let cell = getArrowCell()
                cell.leftLabel.text = "群成员"
                return cell
            }
        } else if indexPath.section == 1 {
            if indexPath.row == 0 {
                // 群名称
                let cell = getArrowCell()
                cell.leftLabel.text = "群名称"
                return cell
            } else if indexPath.row == 1 {
                // 群公告
                let cell = getArrowCell()
                cell.leftLabel.text = "群公告"
                return cell
            } else if indexPath.row == 2 {
                // 群管理
                let cell = getArrowCell()
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
                let cell = getArrowCell()
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
    
    private func updateGroupName() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] newChannelName in
            guard let self = self else { return }
            guard let newChannel = newChannelName as? String else { return }
            
            let trimmedChannelName = newChannel.trimmingCharacters(in: .whitespacesAndNewlines)
            guard trimmedChannelName.count > 0 else { return }
            
            if let groupId = self.conversationInfo?.conversation.conversationId {
                HttpManager.shared.updateGroup(groupId: groupId, name: newChannel) { code in
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
        vc.conversationInfo = self.conversationInfo
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
    
    private func getSwitchCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
//        cell.delegate = self
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
