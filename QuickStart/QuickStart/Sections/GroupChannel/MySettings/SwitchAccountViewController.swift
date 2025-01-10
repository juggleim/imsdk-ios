//
//  SwitchAccountViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation

class SwitchAccountViewController: UIViewController {
    var accounts: [JCUser] = []
    var switchAccount: JCUser?
    let titleLabel: UILabel = {
        let title = UILabel()
        title.text = "点击以切换账号"
        return title
    }()
    
    let tableView: UITableView = {
        let table = UITableView()
        table.bounces = false
        table.alwaysBounceVertical = false
        table.separatorStyle = .none
//        table.rowHeight = UITableView.automaticDimension
        return table
    }()
    
    let addButton: UIButton = {
        let button = UIButton()
        button.setTitle("添加账号", for: .normal)
        button.backgroundColor = UIColor(hex: "#0091FF")
        button.layer.cornerRadius = 5
        button.addTarget(self, action: #selector(onAdd), for: .touchUpInside)
        return button
    }()
    
    let cancelButton: UIButton = {
        let button = UIButton()
        button.setTitle("取消", for: .normal)
        button.backgroundColor = UIColor(hex: "#0091FF")
        button.layer.cornerRadius = 5
        button.addTarget(self, action: #selector(onCancel), for: .touchUpInside)
        return button
    }()
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.backgroundColor = UIColor.white
        setupViews()
        setupLayouts()
        loadAccounts()
        JIM.shared().connectionManager.add(self)
    }
    
    func loadAccounts() {
        var accounts: [Dictionary<String, Dictionary<String, String>>]
        if let a = UserDefaults.loadAccounts() as? [Dictionary<String, Dictionary<String, String>>] {
            accounts = a
        } else {
            accounts = []
        }
        var users: [JCUser] = []
        let loopNumber = min(3, accounts.count)
        for account in accounts[..<loopNumber] {
            if let phone = account.keys.first,
               let infoDic = account[phone] {
                let user = JCUser()
                user.userId = infoDic["id"] ?? ""
                user.userName = infoDic["name"] ?? ""
                user.portrait = infoDic["portrait"] ?? ""
                user.phoneNumber = phone
                users.append(user)
            }
        }
        self.accounts = users
        self.tableView.reloadData()
    }
    
    func setupViews() {
        self.view.addSubview(self.titleLabel)
        self.view.addSubview(self.tableView)
        self.view.addSubview(self.addButton)
        self.view.addSubview(self.cancelButton)
        
        self.tableView.dataSource = self
        self.tableView.delegate = self
    }
    
    func setupLayouts() {
        titleLabel.sbu_constraint(equalTo: self.view, leading: 16, trailing: -16, top: 50)
        titleLabel.sbu_constraint(height: 70)
        titleLabel.textAlignment = .center
        titleLabel.font = UIFont.boldSystemFont(ofSize: 18)
        
        tableView.sbu_constraint(equalTo: titleLabel, leading: 0, trailing: 0, top: 70)
        tableView.sbu_constraint(height: 250)
        
        addButton.sbu_constraint(equalTo: tableView, leading: 0, trailing: 0, top: 270)
        addButton.sbu_constraint(height: 44)
        
        cancelButton.sbu_constraint(equalTo: addButton, leading: 0, trailing: 0, top: 50)
        cancelButton.sbu_constraint(height: 44)
    }
    
    @objc func onAdd() {
        self.switchAccount = nil
        if JIM.shared().connectionManager.getConnectionStatus() == .disconnected {
            self.dismissAndSwitchAccount()
        } else {
            JIM.shared().connectionManager.disconnect(false)
        }
    }
    
    @objc func onCancel() {
        self.dismiss(animated: true)
    }
    
    private func dismissAndSwitchAccount() {
        self.dismiss(animated: true)
        NotificationCenter.default.post(name: NSNotification.Name("SwitchAccount"), object: self.switchAccount)
    }
}

extension SwitchAccountViewController: UITableViewDelegate, UITableViewDataSource {
    func numberOfSections(in tableView: UITableView) -> Int {
        self.accounts.count
    }
    
    func tableView(_ tableView: UITableView, viewForFooterInSection section: Int) -> UIView? {
        nil
    }
    
    func tableView(_ tableView: UITableView, heightForFooterInSection section: Int) -> CGFloat {
        15
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        1
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        70
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let user = self.accounts[indexPath.section]
        let cell = SwitchAccountCell()
        var inUse = false
        if indexPath.section == 0 {
            inUse = true
        }
        cell.configure(user: user, inUse: inUse)
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if indexPath.section == 0 {
            self.dismiss(animated: true)
        } else {
            self.switchAccount = self.accounts[indexPath.section]
            if JIM.shared().connectionManager.getConnectionStatus() == .disconnected {
                self.dismissAndSwitchAccount()
            } else {
                JIM.shared().connectionManager.disconnect(false)
            }
        }
    }
}

extension SwitchAccountViewController: JConnectionDelegate {
    public func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {
        if status == .disconnected {
            self.dismissAndSwitchAccount()
        }
    }
}
