//
//  SBUGroupChannelListViewController.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 03/02/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JetIM

@objcMembers
open class SBUGroupChannelListViewController: SBUBaseChannelListViewController, SBUGroupChannelListModuleHeaderDelegate, SBUGroupChannelListModuleListDelegate, SBUGroupChannelListModuleListDataSource, SBUCommonViewModelDelegate, SBUGroupChannelListViewModelDelegate {
    
    // MARK: - UI Properties (Public)
    public var headerComponent: SBUGroupChannelListModule.Header? {
        get { self.baseHeaderComponent as? SBUGroupChannelListModule.Header }
        set { self.baseHeaderComponent = newValue }
    }
    public var listComponent: SBUGroupChannelListModule.List? {
        get { self.baseListComponent as? SBUGroupChannelListModule.List }
        set { self.baseListComponent = newValue }
    }
    
    @SBUThemeWrapper(theme: SBUTheme.groupChannelListTheme)
    public var theme: SBUGroupChannelListTheme

    /// The default view type is ``SBUCreateChannelTypeSelector``.
    public lazy var createChannelTypeSelector: UIView? = nil
    
    // MARK: - Logic properties (Public)
    public var viewModel: SBUGroupChannelListViewModel? {
        get { self.baseViewModel as? SBUGroupChannelListViewModel }
        set { self.baseViewModel = newValue }
    }
    
    /// This object has a list of all channels.
    public var channelList: [JConversationInfo] { self.viewModel?.channelList ?? [] }
    
    /// This is a property that allows you to show the channel type selector when creating a channel. (default: `true`)
    /// - Since: 3.0.0
    public var enableCreateChannelTypeSelector: Bool = true
    
    // MARK: - Lifecycle
    @available(*, unavailable, renamed: "SBUGroupChannelListViewController()")
    required public init?(coder: NSCoder) {
        super.init(coder: coder)
        fatalError()
    }
    
    @available(*, unavailable, renamed: "SBUGroupChannelListViewController()")
    public override init(nibName nibNameOrNil: String?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
        fatalError()
    }
    
    /// This function initialize the class without `channelListQuery`.
    public init() {
        super.init(nibName: nil, bundle: nil)
        
        self.createViewModel()
        self.headerComponent = SBUModuleSet.GroupChannelListModule.HeaderComponent.init()
        self.listComponent = SBUModuleSet.GroupChannelListModule.ListComponent.init()
    }
    
    open override func viewDidLoad() {
        super.viewDidLoad()
        self.navigationController?.interactivePopGestureRecognizer?.delegate = nil
        self.viewModel?.initChannelList()
    }
    
    open override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        self.updateStyles()
    }
    
    open override var preferredStatusBarStyle: UIStatusBarStyle {
        self.theme.statusBarStyle
    }
    
    deinit {
        SBULog.info("")
        self.viewModel = nil
        self.headerComponent = nil
        self.listComponent = nil
    }
    
    // MARK: - ViewModel
    /// Creates the view model.
    /// - Parameter channelListQuery: Customer's own `GroupChannelListQuery` object
    /// - Since: 3.0.0
    open func createViewModel() {
        self.viewModel = SBUGroupChannelListViewModel(
            delegate: self
        )
    }
    
    // MARK: - Sendbird UIKit Life cycle
    open override func setupViews() {
        // Header component
        self.headerComponent?.configure(delegate: self, theme: self.theme)
        
        // List component
        self.listComponent?.configure(delegate: self, dataSource: self, theme: self.theme)
        
        super.setupViews()
    }
    
    open override func setupLayouts() {
        super.setupLayouts()
        
        if let view = self.navigationController?.view,
           let createChannelTypeSelector = self.createChannelTypeSelector {
            createChannelTypeSelector.sbu_constraint(
                equalTo: view,
                leading: 0,
                trailing: 0,
                top: 0,
                bottom: 0
            )
        }
    }
    
    open override func setupStyles() {
        self.setupNavigationBar(
            backgroundColor: self.theme.navigationBarTintColor,
            shadowColor: self.theme.navigationBarShadowColor
        )
        
        self.headerComponent?.setupStyles(theme: self.theme)
        self.listComponent?.setupStyles(theme: self.theme)
        
        self.view.backgroundColor = theme.backgroundColor
    }
    
    open override func updateStyles() {
        super.updateStyles()
    }
    
    // MARK: - Actions (Show)
    /// This is a function that shows the channelViewController.
    ///
    /// If you want to use a custom channelViewController, override it and implement it.
    /// - Parameters:
    ///   - channelURL: channel url for use in channelViewController.
    ///   - messageListParams: If there is a messageListParams set directly for use in Channel, set it up here
    open override func showChannel(channelURL: JConversation) {
//        GroupChannel.getChannel(url: channelURL) { channel, error in
//            guard error == nil, let channel = channel else { return }
//
//            if channel.isChatNotification {
//                let channelVC = SBUViewControllerSet.ChatNotificationChannelViewController.init(
//                    channel: channel,
//                    notificationListParams: messageListParams,
//                    displaysLocalCachedListFirst: true
//                )
//                self.navigationController?.pushViewController(channelVC, animated: true)
//            } else if channel.isFeedChannel() {
//                let channelVC = SBUViewControllerSet.FeedNotificationChannelViewController.init(
//                    channelURL: channelURL,
//                    notificationListParams: messageListParams,
//                    displaysLocalCachedListFirst: true
//                )
//                self.navigationController?.pushViewController(channelVC, animated: true)
//            } else {
//                let channelVC = SBUViewControllerSet.GroupChannelViewController.init(
//                    channelURL: channelURL,
//                    messageListParams: messageListParams,
//                    displaysLocalCachedListFirst: true
//                )
//                self.navigationController?.pushViewController(channelVC, animated: true)
//            }
//        }
    }
    
    // MARK: - Error handling
    private func errorHandler(_ error: JErrorCode) {
        SBULog.error("Did receive error: \(error)")
    }
    
    open override func errorHandler(_ code: NSInteger? = nil) {
        SBULog.error("Did receive error: \(String(describing: code))")
    }

    // MARK: - SBUGroupChannelListModuleHeaderDelegate
    open func baseChannelListModule(
        _ headerComponent: SBUBaseChannelListModule.Header,
        didUpdateTitleView titleView: UIView?
    ) {
        self.navigationItem.titleView = titleView
    }
    
    open func baseChannelListModule(
        _ headerComponent: SBUBaseChannelListModule.Header,
        didUpdateLeftItem leftItem: UIBarButtonItem?
    ) {
        self.navigationItem.leftBarButtonItem = leftItem
    }
    
    open func baseChannelListModule(
        _ headerComponent: SBUBaseChannelListModule.Header,
        didUpdateRightItem rightItem: UIBarButtonItem?
    ) {
        self.navigationItem.rightBarButtonItem = rightItem
    }
    
    open func baseChannelListModule(
        _ headerComponent: SBUBaseChannelListModule.Header,
        didTapLeftItem leftItem: UIBarButtonItem
    ) {
        self.onClickBack()
    }
    
    open func baseChannelListModule(
        _ headerComponent: SBUBaseChannelListModule.Header,
        didTapRightItem rightItem: UIBarButtonItem
    ) {
//        self.showCreateChannelOrTypeSelector()
    }
    
    // MARK: - SBUGroupChannelListModuleListDelegate
    open func baseChannelListModule(
        _ listComponent: SBUBaseChannelListModule.List,
        didSelectRowAt indexPath: IndexPath
    ) {
        guard self.channelList.count > indexPath.row else { return }
        let channel = self.channelList[indexPath.row]
        self.showChannel(channelURL: channel.conversation)
    }
    
    open func baseChannelListModule(
        _ listComponent: SBUBaseChannelListModule.List,
        didDetectPreloadingPosition indexPath: IndexPath
    ) {
        self.viewModel?.loadNextChannelList(reset: false)
    }
    
    open func baseChannelListModuleDidSelectRetry(_ listComponent: SBUBaseChannelListModule.List) {
        self.viewModel?.initChannelList()
    }
    
    public func baseChannelListModuleDidSelectRefresh(_ listComponent: SBUBaseChannelListModule.List) {
        self.viewModel?.loadNextChannelList(reset: true)
    }
    
    open func groupChannelListModule(
        _ listComponent: SBUGroupChannelListModule.List,
        didSelectLeave channel: JConversationInfo
    ) {
//        self.viewModel?.leaveChannel(channel)
    }
    
    // MARK: - SBUGroupChannelListModuleListDataSource
    open func baseChannelListModule(
        _ listComponent: SBUBaseChannelListModule.List,
        channelsInTableView tableView: UITableView
    ) -> [JConversationInfo]? {
        return self.channelList
    }

    
    open func shouldUpdateLoadingState(_ isLoading: Bool) {
        self.showLoading(isLoading)
    }
    
    open func didReceiveError(_ error: JErrorCode, isBlocker: Bool) {
        self.showLoading(false)
        self.errorHandler(error)
        
        if isBlocker {
            self.viewModel?.reset()
            
            self.listComponent?.updateEmptyView(type: .error)
            self.listComponent?.reloadTableView()
        }
    }
    
    // MARK: - SBUGroupChannelListViewModelDelegate
    open func groupChannelListViewModel(_ viewModel: SBUGroupChannelListViewModel,
                                        didChangeChannelList channels: [JConversationInfo]?,
                                        needsToReload: Bool) {
        if let channelList = channels {
            self.listComponent?.updateEmptyView(type: (channelList.count == 0) ? .noChannels : .none)
        }
        
        guard needsToReload else { return }
        
        self.listComponent?.reloadTableView()
    }
    
    open func groupChannelListViewModel(_ viewModel: SBUGroupChannelListViewModel,
                                        didUpdateChannel channel: JConversationInfo) { }
    
    open func groupChannelListViewModel(_ viewModel: SBUGroupChannelListViewModel,
                                        didLeaveChannel channel: JConversationInfo) { }
}