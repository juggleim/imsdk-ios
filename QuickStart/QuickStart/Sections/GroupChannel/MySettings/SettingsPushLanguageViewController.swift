//
//  SettingsPushLanguageViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

import Foundation

class SettingsPushLanguageViewController: BaseTableListViewController {
    
    var language: GlobalConst.SettingLanguage = .Chinese
    
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "推送语言设置"
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        SBULoading.start()
        JIM.shared().connectionManager.getLanguage { code, lg in
            SBULoading.stop()
            if let lg = lg, lg.starts(with: "en") {
                self.language = .English
            }
            self.tableView.reloadData()
        }
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
}

extension SettingsPushLanguageViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        2
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
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
        var lg: String = "zh-Hans-CN"
        if indexPath.row == 1 {
            lg = "en_US"
        }
        SBULoading.start()
        JIM.shared().connectionManager.setLanguage(lg) { code in
            SBULoading.stop()
            if code == .none {
                self.navigationController?.popViewController(animated: true)
            }
        }
    }
}
