//
//  MySettingsViewController.swift
//  SendbirdUIKit-Sample
//
//  Created by Tez Park on 2020/09/11.
//  Copyright © 2020 SendBird, Inc. All rights reserved.
//

import UIKit
import Photos
import MobileCoreServices
import JuggleIM

enum MySettingsCellType: Int {
    case signOut
}

open class MySettingsViewController: UIViewController, UINavigationControllerDelegate {

    // MARK: - Property
    lazy var userInfoView = UserInfoTitleView()
    lazy var tableView = UITableView()
    
    var theme: SBUChannelSettingsTheme = SBUTheme.channelSettingsTheme
    
    // MARK: - Life cycle
    open override func loadView() {
        super.loadView()
        
        // tableView
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.bounces = false
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.register(
            type(of: MySettingsCell()),
            forCellReuseIdentifier: MySettingsCell.sbu_className
        )
        self.tableView.tableHeaderView = self.userInfoView
        self.tableView.sectionHeaderHeight = UITableView.automaticDimension
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 44.0
        self.view.addSubview(self.tableView)
        
        // autolayout
        self.setupLayouts()
        
        // styles
        self.setupStyles()
    }
    
    /// This function handles the initialization of autolayouts.
    open func setupLayouts() {
        self.userInfoView.translatesAutoresizingMaskIntoConstraints = false
        self.tableView.translatesAutoresizingMaskIntoConstraints = false
        
        var layoutConstraints: [NSLayoutConstraint] = []
        
        layoutConstraints.append(self.userInfoView.leadingAnchor.constraint(
            equalTo: self.view.leadingAnchor,
            constant: 0)
        )
        layoutConstraints.append(self.userInfoView.trailingAnchor.constraint(
            equalTo: self.view.trailingAnchor,
            constant: 0)
        )

        layoutConstraints.append(self.tableView.leadingAnchor.constraint(
            equalTo: self.view.leadingAnchor,
            constant: 0)
        )
        layoutConstraints.append(self.tableView.trailingAnchor.constraint(
            equalTo: self.view.trailingAnchor,
            constant: 0)
        )
        layoutConstraints.append(self.tableView.topAnchor.constraint(
            equalTo: self.view.topAnchor,
            constant: 0)
        )
        layoutConstraints.append(self.tableView.bottomAnchor.constraint(
            equalTo: self.view.bottomAnchor,
            constant: 0)
        )
        
        NSLayoutConstraint.activate(layoutConstraints)
    }
    
    /// This function handles the initialization of styles.
    open func setupStyles() {
        self.theme = SBUTheme.channelSettingsTheme
        
        self.navigationController?.navigationBar.setBackgroundImage(
            UIImage.from(color: theme.navigationBarTintColor),
            for: .default
        )
        self.navigationController?.navigationBar.shadowImage = UIImage.from(
            color: theme.navigationShadowColor
        )
        self.navigationController?.sbu_setupNavigationBarAppearance(
            tintColor: theme.navigationBarTintColor
        )
        
        self.view.backgroundColor = theme.backgroundColor
        self.tableView.backgroundColor = theme.backgroundColor
    }

    open override func viewDidLayoutSubviews() {
        super.viewDidLayoutSubviews()
        
        if let headerView = tableView.tableHeaderView {
            
            let height = headerView.systemLayoutSizeFitting(UIView.layoutFittingCompressedSize).height
            var headerFrame = headerView.frame
            
            if height != headerFrame.size.height {
                headerFrame.size.height = height
                headerView.frame = headerFrame
                tableView.tableHeaderView = headerView
            }
        }
    }
    
    open override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        let userId = JIM.shared().currentUserId
        if let user = JIM.shared().userInfoManager.getUserInfo(userId) {
            self.userInfoView.configure(user: user)
        } else {
            let user = JUserInfo()
            user.userId = userId
            self.userInfoView.configure(user: user)
        }
    }

    open override func viewDidLoad() {
        super.viewDidLoad()
        JIM.shared().connectionManager.add(self)
    }
    
    open override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.tabBarController?.tabBar.isHidden = false
    }

    open override var preferredStatusBarStyle: UIStatusBarStyle {
        return theme.statusBarStyle
    }
    
    // MARK: - Actions
    open func changeDarkThemeSwitch(isOn: Bool) {
        SBUTheme.set(theme: isOn ? .dark : .light)
        
        guard let tabbarController = self.tabBarController as? MainChannelTabbarController else { return }
        tabbarController.updateTheme(isDarkMode: isOn)
        self.userInfoView.setupStyles()
        self.tableView.reloadData()
    }
    
    /// Sign out and dismiss tabbarController,
    func signOutAction() {
        if JIM.shared().connectionManager.getConnectionStatus() == .disconnected {
            self.tabBarController?.dismiss(animated: true, completion: nil)
            return
        }
        JIM.shared().connectionManager.disconnect(false)
    }
}


// MARK: - UITableView relations
extension MySettingsViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let rowValue = indexPath.row
        let type = MySettingsCellType(rawValue: rowValue)
        switch type {
        case .signOut:
            self.signOutAction()
        default:
            break
        }
    }
    
    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        guard let cell = tableView.dequeueReusableCell(
            withIdentifier: MySettingsCell.sbu_className
            ) as? MySettingsCell else { fatalError() }

        cell.selectionStyle = .none
        
        let isDarkMode = (self.tabBarController as? MainChannelTabbarController)?.isDarkMode ?? false

        let rowValue = indexPath.row
        if let type = MySettingsCellType(rawValue: rowValue) {
            cell.configure(type: type, isDarkMode: isDarkMode)

            switch type {
//                case .darkTheme:
//                    cell.switchAction = { [weak self] isOn in
//                        self?.changeDarkThemeSwitch(isOn: isOn)
//                    }
                case .signOut: break
            }
        }

        return cell
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 1
    }
}

extension MySettingsViewController : JConnectionDelegate {
    public func dbDidOpen() {
        
    }
    
    public func dbDidClose() {
        
    }
    
    public func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {
        if status == .disconnected {
            self.tabBarController?.dismiss(animated: true, completion: nil)
        }
    }
}
