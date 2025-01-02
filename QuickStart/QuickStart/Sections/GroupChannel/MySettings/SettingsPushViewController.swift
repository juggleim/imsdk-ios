//
//  SettingsPushViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

import Foundation

class SettingsPushViewController: BaseTableListViewController {
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "推送设置"
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
}

extension SettingsPushViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        1
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.DefaultStyle)
        cell.leftLabel.text = "推送语言设置"
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let vc = SettingsLanguageViewController()
        vc.type = .push
        self.navigationController?.pushViewController(vc, animated: true)
    }
}
