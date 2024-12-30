//
//  SBUGroupChannelSettingsViewController.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 05/02/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

open class SBUGroupChannelSettingsViewController: SBUBaseChannelSettingsViewController, SBUGroupChannelSettingsModuleHeaderDelegate, SBUGroupChannelSettingsModuleHeaderDataSource, SBUGroupChannelSettingsModuleListDelegate, SBUGroupChannelSettingsModuleListDataSource, SBUGroupChannelSettingsViewModelDelegate {
    
    // MARK: - UI Properties (Public)
    public var headerComponent: SBUGroupChannelSettingsModule.Header? {
        get { self.baseHeaderComponent as? SBUGroupChannelSettingsModule.Header }
        set { self.baseHeaderComponent = newValue }
    }
    public var listComponent: SBUGroupChannelSettingsModule.List? {
        get { self.baseListComponent as? SBUGroupChannelSettingsModule.List }
        set { self.baseListComponent = newValue }
    }
    
    // MARK: - Logic properties (Public)
    public var viewModel: SBUGroupChannelSettingsViewModel? {
        get { self.baseViewModel as? SBUGroupChannelSettingsViewModel }
        set { self.baseViewModel = newValue }
    }
    
    public override var conversationInfo: JConversationInfo? { self.viewModel?.conversationInfo }
    
    // MARK: - Lifecycle
    @available(*, unavailable, renamed: "SBUGroupChannelSettingsViewController(channelURL:)")
    required public init?(coder: NSCoder) {
        super.init(coder: coder)
        fatalError()
    }
    
    @available(*, unavailable, renamed: "SBUGroupChannelSettingsViewController(channelURL:)")
    public override init(nibName nibNameOrNil: String?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
        fatalError()
    }
    
    /// If you have channel object, use this initialize function.
    /// - Parameter channel: Channel object
    required public init(conversationInfo: JConversationInfo) {
        super.init(nibName: nil, bundle: nil)
        SBULog.info("")
        
        self.createViewModel(conversationInfo: conversationInfo)
        self.headerComponent = SBUModuleSet.GroupChannelSettingsModule.HeaderComponent.init()
        self.listComponent = SBUModuleSet.GroupChannelSettingsModule.ListComponent.init()
    }
    
    // MARK: - ViewModel
    open override func createViewModel(conversationInfo: JConversationInfo? = nil) {
        self.baseViewModel = SBUGroupChannelSettingsViewModel(
            conversationInfo: conversationInfo,
            delegate: self
        )
    }
    
    // MARK: - Sendbird UIKit Life cycle
    open override func setupViews() {
        // Header component
        self.headerComponent?.configure(delegate: self, dataSource: self, theme: self.theme)
        
        self.navigationItem.titleView = self.headerComponent?.titleView
        self.navigationItem.leftBarButtonItem = self.headerComponent?.leftBarButton
        self.updateRightBarButton()
        
        // List component
        self.listComponent?.configure(
            delegate: self,
            dataSource: self,
            theme: self.theme
        )
        
        if let listComponent = self.listComponent {
            self.view.addSubview(listComponent)
        }
    }
    
    // MARK: - Actions
    
    /// If you want to use a custom userListViewController, override it and implement it.
    open func showMemberList() {
//        guard let channel = self.channel else { return }
//        let memberListVC = SBUViewControllerSet.GroupUserListViewController.init(
//            channel: channel,
//            userListType: .members
//        )
//        self.navigationController?.pushViewController(memberListVC, animated: true)
    }
    
    // MARK: - SBUGroupChannelSettingsModuleHeaderDelegate
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         didUpdateTitleView titleView: UIView?) {
        self.navigationItem.titleView = titleView
    }
    
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         didUpdateLeftItem leftItem: UIBarButtonItem?) {
        self.navigationItem.leftBarButtonItem = leftItem
    }
    
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         didUpdateRightItem rightItem: UIBarButtonItem?) {
        self.navigationItem.rightBarButtonItem = rightItem
    }
    
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         didTapLeftItem leftItem: UIBarButtonItem) {
        self.onClickBack()
    }
    
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         didTapRightItem rightItem: UIBarButtonItem) {
        self.showChannelEditActionSheet()
    }
    
    // MARK: - SBUGroupChannelSettingsModuleHeaderDataSource
    open func groupChannelSettingsModule(_ headerComponent: SBUGroupChannelSettingsModule.Header,
                                         channelNameForTitleView titleView: UIView?) -> String? {
        return self.channelName
    }
    
    // MARK: - SBUGroupChannelSettingsModuleListDelegate
    open func groupChannelSettingsModule(_ listComponent: SBUGroupChannelSettingsModule.List,
                                         didSelectRowAt indexPath: IndexPath) {
    }
    
    open func groupChannelSettingsModuleDidSelectModerations(_ listComponent: SBUGroupChannelSettingsModule.List) {
        self.showModerationList()
    }
    
    open func groupChannelSettingsModuleDidSelectNotifications(_ listComponent: SBUGroupChannelSettingsModule.List) {
        self.selectNotification()
    }
    
    open func groupChannelSettingsModuleDidSelectMembers(_ listComponent: SBUGroupChannelSettingsModule.List) {
        self.showMemberList()
    }
    
    open func groupChannelSettingsModuleDidSelectSearch(_ listComponent: SBUGroupChannelSettingsModule.List) {
        self.showSearch()
    }
    
    open func groupChannelSettingsModuleDidSelectLeave(_ listComponent: SBUGroupChannelSettingsModule.List) {
        self.viewModel?.leaveChannel()
    }
    
    open func groupChannelSettingsModuleDidSwitchNotification(_ listComponent: SBUGroupChannelSettingsModule.List, isMute: Bool) {
        self.viewModel?.mute(isMute)
    }
    
    open func groupChannelSettingsModuleDidSwitchTop(_ listComponent: SBUGroupChannelSettingsModule.List, isTop: Bool) {
        self.viewModel?.setTop(isTop)
    }
    
    open func groupChannelSettingsModuleDidClearMessages(_ listComponent: SBUGroupChannelSettingsModule.List) {
        let okButton = SBUAlertButtonItem(title: SBUStringSet.OK) {[weak self] _ in
            self?.viewModel?.clearMessages()
        }
        let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
        SBUAlertView.show(
            title: "确定清除聊天记录？",
            needInputField: false,
            centerYRatio: 0.75,
            confirmButtonItem: okButton,
            cancelButtonItem: cancelButton
        )
    }
    
    // MARK: - SBUGroupChannelSettingsModuleListDataSource
    open func baseChannelSettingsModule(_ listComponent: SBUBaseChannelSettingsModule.List,
                                        conversationInfoForTableView tableView: UITableView) -> JConversationInfo? {
        return self.conversationInfo
    }
    
    open func baseChannelSettingsModuleIsOperator(_ listComponent: SBUBaseChannelSettingsModule.List) -> Bool {
        return self.viewModel?.isOperator ?? false
    }
    
    // MARK: - SBUGroupChannelSettingsViewModelDelegate
//    open override func baseChannelSettingsViewModel(
//        _ viewModel: SBUBaseChannelSettingsViewModel,
//        didChangeChannel channel: BaseChannel?,
//        withContext context: MessageContext
//    ) {
//        super.baseChannelSettingsViewModel(viewModel, didChangeChannel: channel, withContext: context)
//        
//        self.listComponent?.reloadChannelInfoView()
//        self.listComponent?.reloadTableView()
//    }
//    
//    open func groupChannelSettingsViewModel(_ viewModel: SBUGroupChannelSettingsViewModel,
//                                            didLeaveChannel channel: GroupChannel) {
//        guard let navigationController = self.navigationController,
//              navigationController.viewControllers.count > 1 else {
//                  self.dismiss(animated: true, completion: nil)
//                  return
//              }
//        
//        for viewController in navigationController.viewControllers where viewController is SBUBaseChannelListViewController {
//            navigationController.popToViewController(viewController, animated: true)
//            return
//        }
//        
//        navigationController.popToRootViewController(animated: true)
//    }
}
