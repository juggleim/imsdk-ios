//
//  SBUGroupChannelListModuleList.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2021/09/01.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

/// Event methods for the views updates and performing actions from the list component in the group channel list.
public protocol SBUGroupChannelListModuleListDelegate: SBUBaseChannelListModuleListDelegate {
    /// Called when selected leave button in the swipped cell.
    /// - Parameters:
    ///    - listComponent: `SBUGroupChannelListModule.List` object.
    ///    - channel: The channel that was selected.
    func groupChannelListModule(_ listComponent: SBUGroupChannelListModule.List, didSelectLeave conversationInfo: JConversationInfo)
    
    func groupChannelListModule(
        _ listComponent: SBUGroupChannelListModule.List,
        didSelectMute isMute:Bool,
        conversationInfo: JConversationInfo
    )
    
    func groupChannelListModule(
        _ listComponent: SBUGroupChannelListModule.List,
        didSelectUnread isUnread:Bool,
        conversationInfo: JConversationInfo
    )
}

/// Methods to get data source for the list component in the group channel list.
public protocol SBUGroupChannelListModuleListDataSource: SBUBaseChannelListModuleListDataSource {}

extension SBUGroupChannelListModule {
    /// A module component that represent the list of `SBUGroupChannelListModule`.
    @objc(SBUGroupChannelListModuleList)
    @objcMembers 
    open class List: SBUBaseChannelListModule.List {
        
        // MARK: - UI properties (Public)
        /// The object that is used as the theme of the list component. The theme must adopt the `SBUGroupChannelListTheme` class.
        public var theme: SBUGroupChannelListTheme?
        
        // MARK: - Logic properties (Public)
        /// The object that acts as the delegate of the list component. The delegate must adopt the `SBUGroupChannelListModuleListDelegate`.
        public weak var delegate: SBUGroupChannelListModuleListDelegate? {
            get { self.baseDelegate as? SBUGroupChannelListModuleListDelegate }
            set { self.baseDelegate = newValue }
        }
        
        /// The object that acts as the data source of the list component. The data source must adopt the `SBUGroupChannelListModuleListDataSource`.
        public weak var dataSource: SBUGroupChannelListModuleListDataSource? {
            get { self.baseDataSource as? SBUGroupChannelListModuleListDataSource }
            set { self.baseDataSource = newValue }
        }
        
        enum Section: CaseIterable {
            case main
        }
        
        lazy var diffableDataSource: UITableViewDiffableDataSource<Section, ConversationInfoWrapper> = {
            let source = UITableViewDiffableDataSource<Section, ConversationInfoWrapper>(tableView: self.tableView) { tableView, indexPath, item in
                var cell: SBUBaseChannelCell?
                if let channelCell = self.channelCell {
                    cell = tableView.dequeueReusableCell(
                        withIdentifier: channelCell.sbu_className
                    ) as? SBUBaseChannelCell
                } else if let customCell = self.customCell {
                    cell = tableView.dequeueReusableCell(
                        withIdentifier: customCell.sbu_className
                    ) as? SBUBaseChannelCell
                } else {
                    cell = SBUBaseChannelCell()
                }
                
                cell?.selectionStyle = .none
                
                self.configureCell(cell, conversationInfoWrapper: item)
                
                return cell ?? UITableViewCell()
            }
            return source
        }()

        // MARK: - LifeCycle
        @available(*, unavailable, renamed: "SBUGroupChannelListModule.List()")
        required public init?(coder: NSCoder) { super.init(coder: coder) }
        
        @available(*, unavailable, renamed: "SBUGroupChannelListModule.List()")
        public override init(frame: CGRect) { super.init(frame: frame) }
        
        deinit {
            SBULog.info("")
        }
        
        /// Configures component with parameters.
        /// - Parameters:
        ///   - delegate: `SBUGroupChannelListModuleListDelegate` type listener
        ///   - dataSource: The data source that is type of `SBUGroupChannelListModuleListDataSource`
        ///   - theme: `SBUGroupChannelListTheme` object
        open func configure(delegate: SBUGroupChannelListModuleListDelegate,
                            dataSource: SBUGroupChannelListModuleListDataSource,
                            theme: SBUGroupChannelListTheme) {
            
            self.delegate = delegate
            self.dataSource = dataSource
            self.theme = theme
            
            self.setupViews()
            self.setupLayouts()
            self.setupStyles()
        }
        
        /// Set values of the views in the list component when it needs.
        open override func setupViews() {
            super.setupViews()
            
            // register cell
            if self.channelCell == nil {
                self.register(channelCell: SBUGroupChannelCell())
            }
        }
        
        /// Sets up style with theme. If set theme parameter is nil value, using the stored theme.
        /// - Parameter theme: `SBUGroupChannelListTheme` object
        open func setupStyles(theme: SBUGroupChannelListTheme? = nil) {
            if let theme = theme {
                self.theme = theme
            }
            self.tableView.backgroundColor = self.theme?.backgroundColor
            
            (self.emptyView as? SBUEmptyView)?.setupStyles()
        }
        
        public override func reloadTableView() {
            var wrappers: [ConversationInfoWrapper] = []
            if let list = self.conversationInfoList {
                for conversationInfo in list {
                    var wrapper = ConversationInfoWrapper()
                    wrapper.conversationInfo = conversationInfo
                    wrappers.append(wrapper)
                }
            }
            
            var snapshot = NSDiffableDataSourceSnapshot<Section, ConversationInfoWrapper>()
            snapshot.appendSections([.main])
            snapshot.appendItems(wrappers, toSection: .main)
            self.diffableDataSource.apply(snapshot, animatingDifferences: false)
        }
        
        // MARK: - TableView
        
        /// Creates leave contextual action for a particular swipped cell.
        /// - Parameter indexPath: An index path representing the `channelCell`
        /// - Returns: `UIContextualAction` object.
        public func leaveContextualAction(with indexPath: IndexPath) -> UIContextualAction? {
            guard let conversationInfo = self.conversationInfoList?[indexPath.row] else { return nil }
            
            let size = tableView.visibleCells[0].frame.height
            let itemSize: CGFloat = 40.0
            
            let leaveAction = UIContextualAction(
                style: .normal,
                title: "删除"
            ) { [weak self] _, _, actionHandler in
                guard let self = self else { return }
                self.delegate?.groupChannelListModule(self, didSelectLeave: conversationInfo)
                actionHandler(true)
            }
            
            let leaveTypeView = UIImageView(
                frame: CGRect(
                    x: (size-itemSize)/2,
                    y: (size-itemSize)/2,
                    width: itemSize,
                    height: itemSize
                ))
            leaveTypeView.layer.cornerRadius = itemSize/2
            leaveTypeView.backgroundColor = self.theme?.leaveBackgroundColor
            leaveTypeView.image = SBUIconSetType.iconLeave.image(
                with: self.theme?.leaveTintColor,
                to: SBUIconSetType.Metric.defaultIconSize
            )
            leaveTypeView.contentMode = .center
            
            leaveAction.image = leaveTypeView.asImage()
            leaveAction.backgroundColor = self.theme?.alertBackgroundColor
            
            return leaveAction
        }
        
        /// Creates alarm contextual action for a particular swipped cell.
        /// - Parameter indexPath: An index path representing the `channelCell`
        /// - Returns: `UIContextualAction` object.
        public func alarmContextualAction(with indexPath: IndexPath) -> UIContextualAction? {
            guard let conversationInfo = self.conversationInfoList?[indexPath.row] else { return nil }
            
            let size = tableView.visibleCells[0].frame.height
            let itemSize: CGFloat = 40.0
            
            let mute = conversationInfo.mute
            var title = "消息免打扰"
            if mute {
                title = "消息提醒"
            }
            let alarmAction = UIContextualAction(
                style: .normal,
                title: title
            ) { [weak self] _, _, actionHandler in
                guard let self = self else { return }
                self.delegate?.groupChannelListModule(self, didSelectMute: !mute, conversationInfo: conversationInfo)
                actionHandler(true)
            }
            
            let alarmTypeView = UIImageView(
                frame: CGRect(
                    x: (size-itemSize)/2,
                    y: (size-itemSize)/2,
                    width: itemSize,
                    height: itemSize
                ))
            let alarmIcon: UIImage
            
            if mute {
                alarmTypeView.backgroundColor = self.theme?.notificationOnBackgroundColor
                alarmIcon = SBUIconSetType.iconNotificationFilled.image(
                    with: self.theme?.notificationOnTintColor,
                    to: SBUIconSetType.Metric.defaultIconSize
                )
            } else {
                alarmTypeView.backgroundColor = self.theme?.notificationOffBackgroundColor
                alarmIcon = SBUIconSetType.iconNotificationOffFilled.image(
                    with: self.theme?.notificationOffTintColor,
                    to: SBUIconSetType.Metric.defaultIconSize
                )
            }
            alarmTypeView.image = alarmIcon
            alarmTypeView.contentMode = .center
            alarmTypeView.layer.cornerRadius = itemSize/2
            
            alarmAction.image = alarmTypeView.asImage()
            alarmAction.backgroundColor = self.theme?.alertBackgroundColor
            
            return alarmAction
        }
    }
}

// MARK: - UITableView relations
extension SBUGroupChannelListModule.List {
    open override func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        self.delegate?.baseChannelListModule(self, didSelectRowAt: indexPath)
    }
    
    open override func tableView(
        _ tableView: UITableView,
        willDisplay cell: UITableViewCell,
        forRowAt indexPath: IndexPath
    ) {
    }
    
    public override func scrollViewDidScroll(_ scrollView: UIScrollView) {
        let offsetY = scrollView.contentOffset.y
        let contentHeight = scrollView.contentSize.height
        let frameHeight = scrollView.frame.size.height

        // 当滚动到接近当前数据列表末尾时加载更多数据
        if offsetY > contentHeight - frameHeight - 100 {
            self.delegate?.baseChannelListModuledidDetectPreloading(self)
        }
    }
    
    open override func tableView(
        _ tableView: UITableView,
        trailingSwipeActionsConfigurationForRowAt indexPath: IndexPath
    )
    -> UISwipeActionsConfiguration? {
        var actions: [UIContextualAction] = []
        
        guard let conversationInfo = self.conversationInfoList?[indexPath.row] else { return nil }
        let leaveAction = UIContextualAction(style: .destructive, title: "删除") { action, sourceView, completeHandler in
            self.delegate?.groupChannelListModule(self, didSelectLeave: conversationInfo)
            completeHandler(true)
        }
        
        var title = "消息免打扰"
        let mute = conversationInfo.mute
        if mute {
            title = "消息提醒"
        }
        let alarmAction = UIContextualAction(style: .normal, title: title) { action, sourceView, completeHandler in
            self.delegate?.groupChannelListModule(self, didSelectMute: !mute, conversationInfo: conversationInfo)
            completeHandler(true)
        }
        alarmAction.backgroundColor = UIColor.orange
        
        title = "设置未读"
        var unreadFlag = false
        if conversationInfo.unreadCount > 0 || conversationInfo.hasUnread {
            title = "设置已读"
            unreadFlag = true
        }
        let setUnreadAction = UIContextualAction(style: .normal, title: title) { action, sourceView, completeHandler in
            self.delegate?.groupChannelListModule(self, didSelectUnread: !unreadFlag, conversationInfo: conversationInfo)
            completeHandler(true)
        }
        setUnreadAction.backgroundColor = UIColor.blue
        actions.append(leaveAction)
        actions.append(alarmAction)
        actions.append(setUnreadAction)
        
        return UISwipeActionsConfiguration(actions: actions)
    }
}
