//
//  SettingsDisturbViewController.swift
//  
//
//  Created by Fei Li on 2025/1/2.
//

import Foundation
import JuggleIM

class SettingsDisturbViewController: BaseTableListViewController {
    var disturbTime: GlobalConst.SettingDisturbTime = .noDisturb
    override func configNavigationItem() {
        super.configNavigationItem()
        self.titleView.text = "全局免打扰"
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        SBULoading.start()
        JIM.shared().messageManager.getMuteStatus { code, isMute, timezone, periods in
            SBULoading.stop()
            self.disturbTime = .noDisturb
            if let periods = periods {
                if periods.count > 0 {
                    let period = periods.first
                    if period?.startTime == "08:00" {
                        self.disturbTime = .eightToTwelve
                    } else if period?.startTime == "19:00" {
                        self.disturbTime = .nineteenToTwenty
                    } else if period?.startTime == "23:00" {
                        self.disturbTime = .twentyThreeToSix
                    }
                }
            }
            self.tableView.reloadData()
        }
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
}

extension SettingsDisturbViewController: UITableViewDataSource, UITableViewDelegate {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        4
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let selectRow = self.disturbTime.rawValue

        let cell: BaseSelectTableViewCell
        if indexPath.row == selectRow {
            cell = BaseSelectTableViewCell(selected: true)
        } else {
            cell = BaseSelectTableViewCell(selected: false)
        }
        if indexPath.row == 0 {
            cell.leftLabel.text = "允许通知"
        } else if indexPath.row == 1 {
            cell.leftLabel.text = "08:00 ~ 12:00"
        } else if indexPath.row == 2 {
            cell.leftLabel.text = "19:00 ~ 20:00"
        } else if indexPath.row == 3 {
            cell.leftLabel.text = "23:00 ~ 06:00"
        }
        
        
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        var isMute = true
        var periods: [JTimePeriod] = []
        if indexPath.row == 0 {
            isMute = false
        } else if indexPath.row == 1 {
            let period = JTimePeriod()
            period.startTime = "08:00"
            period.endTime = "12:00"
            periods.append(period)
        } else if indexPath.row == 2 {
            let period = JTimePeriod()
            period.startTime = "19:00"
            period.endTime = "20:00"
            periods.append(period)
        } else if indexPath.row == 3 {
            let period = JTimePeriod()
            period.startTime = "23:00"
            period.endTime = "06:00"
            periods.append(period)
        }
        SBULoading.start()
        JIM.shared().messageManager.setMute(isMute, periods: periods) { code in
            SBULoading.stop()
            if code == .none {
                self.navigationController?.popViewController(animated: true)
            } else {
                
            }
        }
    }
}
