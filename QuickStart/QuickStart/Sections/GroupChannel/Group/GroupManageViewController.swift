//
//  GroupManageViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/25.
//

import Foundation

class GroupManageViewController: BaseTableListViewController {
    var conversationInfo: JConversationInfo?
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
    }
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "群管理"
    }
}

extension GroupManageViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        2
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        if indexPath.row == 0 {
            let cell = getSwitchCell()
            cell.leftLabel.text = "群组全局禁言"
            return cell
        } else if indexPath.row == 1 {
            let cell = getSwitchCell()
            cell.leftLabel.text = "新人入群查看历史"
            return cell
        }
        return UITableViewCell()
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
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
}

