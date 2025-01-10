//
//  SwitchAccountViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation

class SwitchAccountViewController: UIViewController {
    var accounts: [JCUser] = []
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
    }
    
    func loadAccounts() {
        DispatchQueue.main.asyncAfter(deadline: .now()+0.3) {
            let user1 = JCUser()
            user1.userId = "userId1"
            user1.userName = "userName1"
            user1.portrait = ""
            
            let user2 = JCUser()
            user2.userId = "userId2"
            user2.userName = "userName2"
            user2.portrait = ""
            
            let user3 = JCUser()
            user3.userId = "userId3"
            user3.userName = "userName3"
            user3.portrait = ""
            
            self.accounts = [user1, user2, user3]
            self.tableView.reloadData()
        }
        
        
        
//        if let a = UserDefaults.loadAccounts(),
//           a.count > 0 {
//            for index in 0...2 {
//                if index < a.count-1 {
//                    let user = JIM.shared().userInfoManager.get
//                }
//            }
//        }
        
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
        
    }
    
    @objc func onCancel() {
        self.dismiss(animated: true)
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
}
