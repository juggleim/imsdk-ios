//
//  SettingsLanguageViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

import Foundation

enum SettingsLanguageType {
    case global
    case push
}

class SettingsLanguageViewController: BaseTableListViewController {
    var type: SettingsLanguageType = .global
    
    override func configNavigationItem() {
        super.configNavigationItem()
        if type == .global {
            self.titleView.text = "语言设置"
        } else if type == .push {
            self.titleView.text = "推送语言设置"
        }
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
            cell.leftLabel.text = "English"
        }
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if self.type == .global {
            UserDefaults.saveLanguage(GlobalConst.SettingLanguage(rawValue: indexPath.row) ?? .Chinese)
            self.navigationController?.popViewController(animated: true)
        } else if self.type == .push {
            
        }
    }
}
