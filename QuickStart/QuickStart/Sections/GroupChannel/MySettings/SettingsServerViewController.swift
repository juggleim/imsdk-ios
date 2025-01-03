//
//  SettingsServerViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/3.
//

import Foundation

class SettingsServerViewController: UIViewController {
    let titleView = SBUNavigationTitleView()
    let appKeyField: UITextField = {
        let field = UITextField()
        field.placeholder = " App Key"
        field.layer.borderWidth = 1
        return field
    }()
    let serverField: UITextField = {
        let field = UITextField()
        field.placeholder = " Server url（以 http:// 或者 https:// 开头）"
        field.layer.borderWidth = 1
        return field
    }()
    let saveButton: UIButton = {
        let button = UIButton()
        button.setTitle("保存", for: .normal)
        button.backgroundColor = UIColor(hex: "#0091FF")
        button.addTarget(self, action: #selector(onSave), for: .touchUpInside)
        return button
    }()
    let cancelButton: UIButton = {
        let button = UIButton()
        button.setTitle("取消", for: .normal)
        button.backgroundColor = UIColor(hex: "#0091FF")
        button.addTarget(self, action: #selector(onCancel), for: .touchUpInside)
        return button
    }()
    override func loadView() {
        super.loadView()
        self.view.backgroundColor = UIColor.white
        setupViews()
//        configNavigationItem()
        setupLayouts()
    }
    
    func setupViews() {
        self.view.addSubview(self.appKeyField)
        self.view.addSubview(self.serverField)
        self.view.addSubview(self.saveButton)
        self.view.addSubview(self.cancelButton)
    }
    
//    func configNavigationItem() {
//        titleView.textAlignment = .center
//        titleView.text = "服务器设置"
//        self.navigationItem.titleView = titleView
//        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
//        self.navigationItem.leftBarButtonItem = leftButton
//        let rightButton = UIBarButtonItem(title: "保存", style: .plain, target: self, action: #selector(onSave))
//        self.navigationItem.rightBarButtonItem = rightButton
//    }
    
    func setupLayouts() {
        self.appKeyField.translatesAutoresizingMaskIntoConstraints = false
        self.serverField.translatesAutoresizingMaskIntoConstraints = false
        self.saveButton.translatesAutoresizingMaskIntoConstraints = false
        self.cancelButton.translatesAutoresizingMaskIntoConstraints = false
        var layoutConstraints: [NSLayoutConstraint] = []
        layoutConstraints.append(self.appKeyField.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 60))
        layoutConstraints.append(self.appKeyField.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20))
        layoutConstraints.append(self.appKeyField.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20))
        layoutConstraints.append(self.appKeyField.heightAnchor.constraint(equalToConstant: 44))
        layoutConstraints.append(self.serverField.topAnchor.constraint(equalTo: self.appKeyField.bottomAnchor, constant: 10))
        layoutConstraints.append(self.serverField.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20))
        layoutConstraints.append(self.serverField.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20))
        layoutConstraints.append(self.serverField.heightAnchor.constraint(equalToConstant: 44))
        layoutConstraints.append(self.saveButton.topAnchor.constraint(equalTo: self.serverField.bottomAnchor, constant: 10))
        layoutConstraints.append(self.saveButton.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20))
        layoutConstraints.append(self.saveButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20))
        layoutConstraints.append(self.saveButton.heightAnchor.constraint(equalToConstant: 44))
        layoutConstraints.append(self.cancelButton.topAnchor.constraint(equalTo: self.saveButton.bottomAnchor, constant: 10))
        layoutConstraints.append(self.cancelButton.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 20))
        layoutConstraints.append(self.cancelButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -20))
        layoutConstraints.append(self.cancelButton.heightAnchor.constraint(equalToConstant: 44))
        NSLayoutConstraint.activate(layoutConstraints)
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
    
    @objc func onSave() {
        guard let appKey = appKeyField.text, let server = serverField.text?.lowercased() else {
            return
        }
        if appKey.isEmpty ||
            server.isEmpty ||
            !checkServerPrefix(server: server) {
            SBUAlertView.show(
                title: "输入不合法",
                confirmButtonItem: SBUAlertButtonItem(
                    title: SBUStringSet.OK,
                    completionHandler: { _ in
                    }
                ), cancelButtonItem: nil)
            return
        } else {
            GlobalConfig.appKey = appKey
            GlobalConfig.demoServer = server
            HttpManager.shared.setAppKey(appKey)
            JIM.shared().initWithAppKey(appKey)
            JIM.shared().setServerUrls([GlobalConfig.imServer])
            self.dismiss(animated: true)
        }
    }
    
    @objc func onCancel() {
        self.dismiss(animated: true)
    }
    
    private func checkServerPrefix(server: String?) -> Bool {
        if let server = server {
            if server.hasPrefix("https://") || server.hasPrefix("http://") {
                return true
            }
        }
        return false
    }
}
