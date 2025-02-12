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
    case qrcode
//    case setLanguage
    case push
    case globalDisturb
    case privacyPolicy
    case userAgreement
}

open class MySettingsViewController: UIViewController, UINavigationControllerDelegate {

    // MARK: - Property
//    lazy var rightBarButton: UIBarButtonItem = {
//        let rightItem =  UIBarButtonItem(
//            title: SBUStringSet.Edit,
//            style: .plain,
//            target: self,
//            action: #selector(onClickEdit)
//        )
//        rightItem.setTitleTextAttributes([.font : SBUFontSet.button2], for: .normal)
//        return rightItem
//    }()
    
    lazy var userInfoView = UserInfoTitleView()
    lazy var tableView = UITableView()
    
    var theme: SBUChannelSettingsTheme = SBUTheme.channelSettingsTheme
    
    // MARK: - Constant
    private let actionSheetIdEdit = 1
    private let actionSheetIdPicker = 2
    
    // MARK: - Life cycle
    open override func loadView() {
        super.loadView()
        
        // navigation bar
//        self.navigationItem.rightBarButtonItem = self.rightBarButton
        
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(tapUserInfo))
        self.userInfoView.addGestureRecognizer(tapGesture)
        
        // tableView
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.bounces = false
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.tableHeaderView = self.userInfoView
        self.tableView.sectionHeaderHeight = UITableView.automaticDimension
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 44.0
        self.setFooterView()
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
        
//        self.rightBarButton.tintColor = theme.rightBarButtonTintColor
        
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

        if let user = ProfileManager.shared.currentUserInfo {
            self.userInfoView.configure(user: user)
        } else {
            let user = JCUser()
            user.userId = JIM.shared().currentUserId
            self.userInfoView.configure(user: user)
        }
    }

    open override func viewDidLoad() {
        super.viewDidLoad()
        JIM.shared().connectionManager.add(self)
    }

    open override var preferredStatusBarStyle: UIStatusBarStyle {
        return theme.statusBarStyle
    }
    
    // MARK: - Actions
    /// Open the user edit action sheet.
    @objc func onClickEdit() {
        let changeNameItem = SBUActionSheetItem(
            title: "修改昵称",
            color: theme.itemTextColor,
            image: nil
        ) {}
        let changeImageItem = SBUActionSheetItem(
            title: "修改头像",
            color: theme.itemTextColor,
            image: nil
        ) {}
        let cancelItem = SBUActionSheetItem(
            title: SBUStringSet.Cancel,
            color: theme.itemColor
        ) {}
        SBUActionSheet.show(
            items: [changeNameItem, changeImageItem],
            cancelItem: cancelItem,
            identifier: actionSheetIdEdit,
            delegate: self
        )
    }
    
    /// Open the nickname change popup.
    public func changeNickname() {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] newNickname in
            guard let nickname = newNickname as? String,
                nickname.trimmingCharacters(in: .whitespacesAndNewlines).count > 0
                else { return }
            
            HttpManager.shared.updateUserInfo(userId: JIM.shared().currentUserId, name: nickname) { code in
                if code != HttpManager.success {
                    return
                }
                DispatchQueue.main.async { [weak self] in
                    if let userInfo = ProfileManager.shared.currentUserInfo {
                        userInfo.userName = nickname
                        self?.userInfoView.configure(user: userInfo)
                    }
                }
            }
        }
        let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
        SBUAlertView.show(
            title: "修改昵称",
            needInputField: true,
            placeHolder: "输入昵称",
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
    
    /// Open the user image selection menu.
    public func selectUserImage() {
        let cameraItem = SBUActionSheetItem(
            title: SBUStringSet.Camera,
            image: SBUIconSet.iconCamera.sbu_with(tintColor: theme.itemColor),
            completionHandler: nil
        )
        let libraryItem = SBUActionSheetItem(
            title: SBUStringSet.PhotoVideoLibrary,
            image: SBUIconSet.iconPhoto.sbu_with(tintColor: theme.itemColor),
            completionHandler: nil
        )
        let cancelItem = SBUActionSheetItem(
            title: SBUStringSet.Cancel,
            color: theme.itemColor,
            completionHandler: nil
        )
        SBUActionSheet.show(
            items: [cameraItem, libraryItem],
            cancelItem: cancelItem,
            identifier: actionSheetIdPicker,
            delegate: self
        )
    }
    
    open func changeDarkThemeSwitch(isOn: Bool) {
        SBUTheme.set(theme: isOn ? .dark : .light)
        
        guard let tabbarController = self.tabBarController as? MainChannelTabbarController else { return }
        tabbarController.updateTheme(isDarkMode: isOn)
        self.userInfoView.setupStyles()
        self.tableView.reloadData()
    }
    
    @objc func switchAccountAction() {
        let vc = SwitchAccountViewController()
        vc.modalPresentationStyle = .fullScreen
        self.present(vc, animated: true)
    }
    
    /// Sign out and dismiss tabbarController,
    @objc func signOutAction() {
        UserDefaults.saveIsAutoLogin(false)
        if JIM.shared().connectionManager.getConnectionStatus() == .disconnected {
            self.tabBarController?.dismiss(animated: true, completion: nil)
            return
        }
        JIM.shared().connectionManager.disconnect(false)
    }
    
    private func setFooterView() {
        let view = UIView(frame: CGRect(x: 0, y: 0, width: self.view.bounds.size.width, height: 150))
        
        let switchButton = UIButton(frame: CGRect(x: 10, y: 29, width: self.view.bounds.size.width-20.0, height: 42))
        switchButton.backgroundColor = UIColor(red: 31.0 / 255.0, green: 124.0 / 255.0, blue: 252.0 / 255.0, alpha: 1.0)
        switchButton.setTitle("切换账号", for: .normal)
        switchButton.layer.cornerRadius = 5.f
        switchButton.layer.borderWidth = 0.5
        switchButton.layer.borderColor = UIColor(red: 0xcc / 255.0, green: 0x44 / 255.0, blue: 0x45 / 255.0, alpha: 1.0).cgColor
        view.addSubview(switchButton)
        switchButton.addTarget(self, action: #selector(switchAccountAction), for: .touchUpInside)
        
        let quitButton = UIButton(frame: CGRect(x: 10, y: 80, width: self.view.bounds.size.width-20.0, height: 42))
        quitButton.backgroundColor = UIColor(red: 235.0 / 255.0, green: 70.0 / 255.0, blue: 72.0 / 255.0, alpha: 1.0)
        quitButton.setTitle("退出登录", for: .normal)
        quitButton.layer.cornerRadius = 5.f
        quitButton.layer.borderWidth = 0.5
        quitButton.layer.borderColor = UIColor(red: 0xcc / 255.0, green: 0x44 / 255.0, blue: 0x45 / 255.0, alpha: 1.0).cgColor
        view.addSubview(quitButton)
        quitButton.addTarget(self, action: #selector(signOutAction), for: .touchUpInside)
        self.tableView.tableFooterView = view
    }
    
    @objc private func tapUserInfo() {
        let vc = SettingsMeViewController()
        vc.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(vc, animated: true)
    }
}

// MARK: - UITableView relations
extension MySettingsViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let rowValue = indexPath.row
        let type = MySettingsCellType(rawValue: rowValue)
        switch type {
        case .qrcode:
            let conversation = JConversation(conversationType: .private, conversationId: JIM.shared().currentUserId)
            let vc = QRCodeViewController(conversation: conversation)
            vc.hidesBottomBarWhenPushed = true
            self.navigationController?.pushViewController(vc, animated: true)
//        case .setLanguage:
//            let vc = SettingsLanguageViewController()
//            vc.hidesBottomBarWhenPushed = true
//            self.navigationController?.pushViewController(vc, animated: true)
        case .push:
            let vc = SettingsPushViewController()
            vc.hidesBottomBarWhenPushed = true
            self.navigationController?.pushViewController(vc, animated: true)
        case .globalDisturb:
            let vc = SettingsDisturbViewController()
            vc.hidesBottomBarWhenPushed = true
            self.navigationController?.pushViewController(vc, animated: true)
        case .privacyPolicy:
            let vc = SettingsWebViewController(url: "https://www.juggle.im/jc/privacy.html", titleString: "隐私协议")
            vc.hidesBottomBarWhenPushed = true
            self.navigationController?.pushViewController(vc, animated: true)
        case .userAgreement:
            let vc = SettingsWebViewController(url: "https://www.juggle.im/jc/user.html", titleString: "用户协议")
            vc.hidesBottomBarWhenPushed = true
            self.navigationController?.pushViewController(vc, animated: true)
        case .none:
            break
        }
    }
    
    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let rowValue = indexPath.row
        if let type = MySettingsCellType(rawValue: rowValue) {
            switch type {
            case .qrcode:
                let cell = getArrowCell()
                cell.leftLabel.text = "我的二维码"
                return cell
//            case .setLanguage:
//                let cell = getArrowCell()
//                cell.leftLabel.text = "语言设置"
//                return cell
            case .push:
                let cell = getArrowCell()
                cell.leftLabel.text = "推送设置"
                return cell
            case .globalDisturb:
                let cell = getArrowCell()
                cell.leftLabel.text = "全局免打扰"
                return cell
            case .privacyPolicy:
                let cell = getArrowCell()
                cell.leftLabel.text = "隐私协议"
                return cell
            case .userAgreement:
                let cell = getArrowCell()
                cell.leftLabel.text = "用户协议"
                return cell
            }
        }
        return UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 5
    }
    
    private func getArrowCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.DefaultStyle)
        return cell
    }
    
    private func getSwitchCell() -> BaseSettingTableViewCell {
        let cell = BaseSettingTableViewCell()
        cell.setCellStyle(.SwitchStyle)
        return cell
    }
}

extension MySettingsViewController : JConnectionDelegate {
    public func dbDidOpen() {
        
    }
    
    public func dbDidClose() {
        
    }
    
    public func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {
        DispatchQueue.main.asyncAfter(deadline: .now()+0.2) {
            if status == .disconnected {
                self.tabBarController?.dismiss(animated: true, completion: nil)
            }
        }
    }
}

// MARK: SBUActionSheetDelegate
extension MySettingsViewController: SBUActionSheetDelegate {
    public func didSelectActionSheetItem(index: Int, identifier: Int) {
        if identifier == actionSheetIdEdit {
            let type = ChannelEditType.init(rawValue: index)
            switch type {
            case .name:
                self.changeNickname()
            case .image:
                self.selectUserImage()
            default:
                break
            }
        } else {
            let type = MediaResourceType.init(rawValue: index)
            var sourceType: UIImagePickerController.SourceType = .photoLibrary
            let mediaType: [String] = [String(kUTTypeImage)]

            switch type {
            case .camera:
                sourceType = .camera
            case .library:
                sourceType = .photoLibrary
            default:
                break
            }

            if type != .document {
                if UIImagePickerController.isSourceTypeAvailable(sourceType) {
                    let imagePickerController = UIImagePickerController()
                    imagePickerController.delegate = self
                    imagePickerController.sourceType = sourceType
                    imagePickerController.mediaTypes = mediaType
                    self.present(imagePickerController, animated: true, completion: nil)
                }
            }
        }
    }
}


// MARK: UIImagePickerViewControllerDelegate
extension MySettingsViewController: UIImagePickerControllerDelegate {
    public func imagePickerController(
        _ picker: UIImagePickerController,
        didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {

        picker.dismiss(animated: true) { [weak self] in
            guard let originalImage = info[.originalImage] as? UIImage else { return }

            self?.userInfoView.coverImage.image = originalImage
            
            JIM.shared().messageManager.uploadImage(originalImage) { url in
                HttpManager.shared.updateUserInfo(
                    userId: JIM.shared().currentUserId,
                    portrait: url) { code in
                        if code != HttpManager.success {
                            return
                        }
                        DispatchQueue.main.async { [weak self] in
                            if let userInfo = ProfileManager.shared.currentUserInfo {
                                userInfo.portrait = url
                                self?.userInfoView.configure(user: userInfo)
                            }
                        }
                    }
            } error: { code in
            }
        }
    }
}
