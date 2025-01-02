//
//  SettingsLanguageViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

import Foundation

class SettingsLanguageViewController: BaseTableListViewController {
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "设置语言"
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

extension SettingsLanguageViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        2
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let language = UserDefaults.loadLanguage()
        let cell: BaseSelectTableViewCell
        if indexPath.row == language.rawValue {
            cell = BaseSelectTableViewCell(selected: true)
        } else {
            cell = BaseSelectTableViewCell(selected: false)
        }
        
        if indexPath.row == 0 {
            cell.leftLabel.text = "中文"
        } else if indexPath.row == 1 {
            cell.leftLabel.text = "英文"
        }
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        UserDefaults.saveLanguage(GlobalConst.SettingLanguage(rawValue: indexPath.row) ?? .Chinese)
        self.navigationController?.popViewController(animated: true)
    }
}
