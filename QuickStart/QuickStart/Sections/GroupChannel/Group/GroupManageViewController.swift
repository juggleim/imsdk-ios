//
//  GroupManageViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/25.
//

import Foundation

class GroupManageViewController: BaseTableListViewController {
    var groupId: String = ""
    var mute: Int = 0
    var historyMessageVisible: Int = 0
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
    }
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "群管理"
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton

    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
}

extension GroupManageViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        3
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        if indexPath.row == 0 {
            let cell = getArrowCell()
            cell.leftLabel.text = "变更群主"
            return cell
        } else if indexPath.row == 1 {
            let cell = getSwitchCell()
            cell.leftLabel.text = "群组全局禁言"
            cell.switchButton.isOn = (mute != 0)
            cell.switchButton.addTarget(self, action: #selector(onSetMute(_:)), for: .valueChanged)
            return cell
        } else if indexPath.row == 2 {
            let cell = getSwitchCell()
            cell.leftLabel.text = "新人入群查看历史"
            cell.switchButton.isOn = (historyMessageVisible != 0)
            cell.switchButton.addTarget(self, action: #selector(onSetHistory(_:)), for: .valueChanged)
            return cell
        }
        return UITableViewCell()
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.row == 0 {
            let vc = GroupMemberViewController()
            vc.groupId = self.groupId
            vc.type = .callback
            vc.includeSelf = false
            vc.delegate = self
            self.navigationController?.pushViewController(vc, animated: true)
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
    
    @objc private func onSetMute(_ sender: UISwitch) {
        SBULoading.start()
        let mute = sender.isOn ? 1 : 0
        HttpManager.shared.muteGroup(groupId: self.groupId, isMute: mute) { code in
            DispatchQueue.main.async {
                if code != 0 {
                    print("set group mute error, code is \(code)")
                }
                SBULoading.stop()
            }
        }
    }
    
    @objc private func onSetHistory(_ sender: UISwitch) {
        SBULoading.start()
        let isVisible = sender.isOn ? 1 : 0
        HttpManager.shared.setGroupHistoryMessageVisible(groupId: self.groupId, isVisible: isVisible) { code in
            DispatchQueue.main.async {
                if code != 0 {
                    print("set history message visible error, code is \(code)")
                }
                SBULoading.stop()
            }
        }
    }
}

extension GroupManageViewController: GroupMemberVCDelegate {
    func memberDidSelect(_ member: JUserInfo) {
        SBULoading.start()
        HttpManager.shared.changeGroupOwner(groupId: self.groupId, ownerId: member.userId) { code in
            DispatchQueue.main.async {
                if code != 0 {
                    print("set history message visible error, code is \(code)")
                }
                SBULoading.stop()
                self.navigationController?.popViewController(animated: true)
            }
        }
    }
}

