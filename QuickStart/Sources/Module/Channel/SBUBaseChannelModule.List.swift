//
//  SBUBaseChannelModule.List.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2021/09/30.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

/// Event methods for the views updates and performing actions from the list component.
public protocol SBUBaseChannelModuleListDelegate: SBUCommonDelegate {
    /// Called when the `listComponent` is about to draw a cell for a particular row.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - cell: The table view cell that the list component going to use when drawing the row.
    ///    - indexPath: An index path locating the row in table view of `listComponent`
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        willDisplay cell: UITableViewCell,
        forRowAt indexPath: IndexPath
    )
    
    /// Called when the message cell was tapped in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - message: The message that was tapped.
    ///    - indexPath: An index path locating the row in table view of `listComponent
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didTapMessage message: JMessage,
        forRowAt indexPath: IndexPath
    )
    
    /// Called when the message cell was long tapped in the `listComponent`.
    /// - Note: As a default, it shows menu items for `message`. Please refer to ``SBUBaseChannelModule/List/showMessageMenu(on:forRowAt:)``
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - message: The message that was long tapped.
    ///    - indexPath: An index path locating the row in table view of `listComponent
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didLongTapMessage message: JMessage,
        reaction: JMessageReaction?,
        forRowAt indexPath: IndexPath
    )
    
    /// Called when the user profile was tapped in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - user: The `SBUUser` of user profile that was tapped.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didTapUserProfile user: SBUUser
    )
    
    /// Called when the message cell was tapped in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - JMessage: The message that was tapped.
    ///    - cell: The table view cell that the selected cell.
    ///    - indexPath: An index path locating the row in table view of `listComponent
    ///
    /// - Since: 3.4.0
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didTapVoiceMessage JMessage: JMessage,
        cell: UITableViewCell,
        forRowAt indexPath: IndexPath
    )
    
    /// Called when the `scrollView` was scrolled.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - scrollView: The `scrollView`.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didScroll scrollView: UIScrollView
    )
    
    /// Called when the `scrollBottomView`was tapped in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - animated: if it's `true`, the list component will be scrolled while animating
    func baseChannelModuleDidTapScrollToButton(
        _ listComponent: SBUBaseChannelModule.List,
        animated: Bool
    )
    
    /// Called when the `scrollBottomView`was tapped in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - options: options for scroll position. see also  ``SBUScrollOptions``.
    ///    - animated: if it's `true`, the list component will be scrolled while animating
    /// - Since: 3.13.0
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didSelectScrollToBottonWithOptions options: SBUScrollOptions,
        animated: Bool
    )
    
    /// Called when the retry button was selected from the `listComponent`.
    /// - Parameter listComponent: `SBUBaseChannelModule.List` object.
    func baseChannelModuleDidSelectRetry(_ listComponent: SBUBaseChannelModule.List)
    
    // MARK: Menu
    
    /// Ccalled when a user selects the *retry* menu item of a `failedMessage` in the `listComponent`
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - failedMessage: The failed message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapRetryFailedMessage failedMessage: JMessage)
    
    /// Called when a user selects the *delete* menu item of a `failedMessage` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - failedMessage: The failed message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapDeleteFailedMessage failedMessage: JMessage)
    
    /// Called when a user selects the *copy* menu item of a `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapCopyMessage message: JMessage)
 
    /// Called when a user selects the *delete* menu item of a `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapDeleteMessage message: JMessage)
    
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapRecallMessage message: JMessage)
    
    /// Called when a user selects the *edit* menu item of a `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapEditMessage message: JMessage)
    
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapForwardMessage message: JMessage)
    
    /// Called when a user selects the *save* menu item of a `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapSaveMessage message: JMessage)
    
    /// Called when a user selects the *reply* menu item of a `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message that the selected menu item belongs to.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapReplyMessage message: JMessage)
    
    /// Called when a user *reacts* to a `message` in the `listComponent` with an emoji.
    /// - Parameters:
    ///    - listComponent: An object of ``SBUBaseChannelModule/List``.
    ///    - message: The message that the user reacted with an emoji to.
    ///    - key: The key value of the emoji.
    ///    - selected: Determines whether the emoji has already been used for the message.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didReactToMessage message: JMessage,
        withEmoji key: String,
        selected: Bool
    )
    
    /// Called when the user selects the *more emoji* button on a message in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: An object of ``SBUBaseChannelModule/List``.
    ///    - message: The message that the user wants to react with more emojis.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didTapMoreEmojisOnMessage message: JMessage
    )
    
    /// Called when the ``SBUMenuSheetViewController`` instance is dismissed.
    /// - Parameters:
    ///    - listComponent: An object of ``SBUBaseChannelModule/List``.
    ///    - cell: The `UITableViewCell` object that includes the message displayed through ``SBUMenuSheetViewController``.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didDismissMenuForCell cell: UITableViewCell
    )

    /// Called when failed scroll to `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - messageId: The message id for which scrolling was attempted.
    ///    - needToSearch: If `true`, reloads the collection based on the message information if not found by message ID.
    /// - Since: 3.13.0
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didFailScrollToMessageId messageId: Int64, needToSearch: Bool)
    
    /// Called when failed scroll to `message` in the `listComponent`.
    /// - Parameters:
    ///    - listComponent: A ``SBUBaseChannelModule/List`` object.
    ///    - message: The message for which scrolling was attempted
    ///    - needToSearch: If `true`, reloads the collection based on the message information if not found by message ID.
    /// - Since: 3.13.0
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didFailScrollToMessage message: JMessage, needToSearch: Bool)
}

extension SBUBaseChannelModuleListDelegate {
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didLongTapMessage message: JMessage,
        forRowAt indexPath: IndexPath
    ) {
        listComponent.showMessageMenu(on: message, forRowAt: indexPath)
    }
}

/// Methods to get data source for the list component.
public protocol SBUBaseChannelModuleListDataSource: AnyObject {
    /// Ask the data source to return the `JConversationInfo` object.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: `JConversationInfo` object.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, conversationInfoForTableView tableView: UITableView) -> JConversationInfo?
    
    /// Ask the data source to return the message list sent successfully.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: The array of `JMessage` object that are sent successfully.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, sentMessagesInTableView tableView: UITableView) -> [JMessage]
    
    /// Ask the data source to return the message list includes the sent, the failed and the pending.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: The array of `JMessage` object including the sent, the failed and the pending.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, fullMessagesInTableView tableView: UITableView) -> [JMessage]
    
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, reactionListInTableView tableView: UITableView) -> [JMessageReaction]
    
    /// Ask the data source to return whether the `tableView` has next data.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: Whether the `tableView` has next data.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, hasNextInTableView tableView: UITableView) -> Bool
    
    /// Ask the data source to return the last seen index path
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: The last seen `IndexPath`.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, lastSeenIndexPathIn tableView: UITableView) -> IndexPath?
    
    /// Ask the data source to return the starting point
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: The starting point.
    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, startingPointIn tableView: UITableView) -> Int64?
    
    /// A data source function that returns the parent view controller, which displays the `menuItems` through ``SBUMenuSheetViewController``.
    /// - Parameters:
    ///    - listComponent: An object of ``SBUBaseChannelModule/List``.
    ///    - menuItems: The objects of ``SBUMenuItem`` that are used in ``SBUMenuSheetViewController``.
    func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        parentViewControllerDisplayMenuItems menuItems: [SBUMenuItem]
    ) -> UIViewController?
    
    /// Ask the data source to return the `SBUPendingMessageManager` object.
    /// - Parameters:
    ///    - listComponent: `SBUBaseChannelModule.List` object.
    ///    - cell: `UITableViewCell` object from list component.
    /// - Returns: (`SBUPendingMessageManager` object, `isThreadMessageMode` object of view model)
    /// - Since: 3.3.0
//    func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, pendingMessageManagerForCell cell: UITableViewCell) -> (SBUPendingMessageManager?, Bool?)
}

extension SBUBaseChannelModule {
    /// A module component that represent the list of `SBUBaseChannelModule`.
    @objc(SBUBaseChannelModuleList)
    @objcMembers
    open class List: UIView, UITableViewDelegate, UITableViewDataSource {
        
        // MARK: - UI properties (Public)
        
        /// The table view to show messages in the channel
        public var tableView = UITableView()
        
        /// A view that shows when there is no message in the channel.
        /// The default view type is ``SBUEmptyView``.
        public var emptyView: UIView? {
            didSet { self.tableView.backgroundView = self.emptyView }
        }
        
        /// A view that shows the state of the channel such as frozen state.
        /// The default view type is ``UIView``.
        public var channelStateBanner: UIView?
        
        /// A view that indicates a new received message.
        /// If you use a view that inherits `SBUNewMessageInfo`, you can change the button and their action.
        /// - NOTE: You can use the customized view and a view that inherits `SBUNewMessageInfo`.
        public var newMessageInfoView: UIView?
        
        /// A view that scrolls table view to the bottom.
        /// The default view type is ``UIView``.
        public var scrollBottomView: UIView?
        
        /// A view that shows profile of the user.
        /// If you do not want to use the user profile feature, please set this value to nil.
        /// - NOTE: To use the custom user profile view, set this to the custom view created using `SBUUserProfileViewProtocol`.
        public var userProfileView: UIView?
        
        /// The object that acts as the base delegate of the list component. The base delegate must adopt the `SBUBaseChannelModuleListDelegate`.
        public weak var baseDelegate: SBUBaseChannelModuleListDelegate?
        
        /// The object that acts as the base data source of the list component. The base data source must adopt the `SBUBaseChannelModuleListDataSource`.
        public weak var baseDataSource: SBUBaseChannelModuleListDataSource?
        
        /// The object that is used as the theme of the list component. The theme must adopt the `SBUChannelTheme` class.
        public var theme: SBUChannelTheme?
        
        // MARK: - UI properties (Private)
        private lazy var defaultEmptyView: SBUEmptyView? = {
            let emptyView = SBUEmptyView()
            emptyView.type = EmptyViewType.none
            emptyView.delegate = self
            return emptyView
        }()
        
        private lazy var defaultChannelStateBanner: UIView? = {
            let label = UILabel()
            label.textAlignment = .center
            label.text = SBUStringSet.Channel_State_Banner_Frozen
            label.layer.masksToBounds = true
            label.layer.cornerRadius = 5
            label.isHidden = true
            return label
        }()
        
        private lazy var defaultScrollBottomView: UIView? = {
            let view: UIView = UIView(frame: CGRect(origin: .zero, size: SBUConstant.scrollBottomButtonSize))
            let theme = SBUTheme.componentTheme
            
            view.backgroundColor = .clear
            view.layer.shadowColor = theme.shadowColor.withAlphaComponent(0.5).cgColor
            view.layer.shadowOffset = CGSize(width: 0, height: 5)
            view.layer.shadowOpacity = 0.5
            view.layer.shadowRadius = 5
            view.layer.masksToBounds = false
            
            let scrollBottomButton = UIButton(frame: CGRect(origin: .zero, size: SBUConstant.scrollBottomButtonSize))
            scrollBottomButton.layer.cornerRadius = scrollBottomButton.frame.height / 2
            scrollBottomButton.clipsToBounds = true
            
            scrollBottomButton.setImage(
                SBUIconSetType.iconChevronDown.image(
                    with: theme.scrollBottomButtonIconColor,
                    to: SBUIconSetType.Metric.iconChevronDown
                ),
                for: .normal
            )
            scrollBottomButton.backgroundColor = theme.scrollBottomButtonBackground
            scrollBottomButton.setBackgroundImage(UIImage.from(color: theme.scrollBottomButtonHighlighted), for: .highlighted)
            
            scrollBottomButton.addTarget(self, action: #selector(self.onTapScrollToBottom), for: .touchUpInside)
            view.addSubview(scrollBottomButton)
            
            scrollBottomButton
                .sbu_constraint(equalTo: view, leading: 0, trailing: 0, top: 0, bottom: 0)

            return view
        }()
        
        // MARK: - Logic properties (Public)
        
        /// The current channel object from `baseChannelModule(_:channelForTableView:)` data source method.
        public var conversationInfo: JConversationInfo? {
            self.baseDataSource?.baseChannelModule(self, conversationInfoForTableView: self.tableView)
        }
        
        /// The array of sent messages in the channel. The value is returned by `baseChannelModule(_:sentMessagesInTableView:)` data source method.
        public var sentMessages: [JMessage] {
            self.baseDataSource?.baseChannelModule(self, sentMessagesInTableView: self.tableView) ?? []
        }
        
        /// The array of all messages includes the sent, the failed and the pending. The value is returned by `baseChannelModule(_:fullMessagesInTableView:)` data source method.
        public var fullMessageList: [JMessage] {
            self.baseDataSource?.baseChannelModule(self, fullMessagesInTableView: self.tableView) ?? []
        }
        
        public var reactionList: [JMessageReaction] {
            self.baseDataSource?.baseChannelModule(self, reactionListInTableView: self.tableView) ?? []
        }
        
        // MARK: - Logic properties (Private)
        
        /// The object that is used as the cell animation debouncer.
        lazy var cellAnimationDebouncer: SBUDebouncer = SBUDebouncer()
        
        var isTransformedList: Bool = true
        var isTableViewReloading = false
        var needReload = false
        
        // MARK: - LifeCycle
        required public init?(coder: NSCoder) {
            super.init(coder: coder)
        }
        
        public override init(frame: CGRect) {
            super.init(frame: frame)
        }
        
        deinit {
            SBULog.info(#function)
        }
        
        /// Set values of the views in the list component when it needs.
        open func setupViews() {
            // empty view
            if self.emptyView == nil {
                self.emptyView = self.defaultEmptyView
            }
            
            // table view
            self.tableView.delegate = self
            self.tableView.dataSource = self
            
            self.tableView.separatorStyle = .none
            self.tableView.allowsSelection = false
            self.tableView.keyboardDismissMode = .interactive
            self.tableView.bounces = false
            self.tableView.alwaysBounceVertical = false
            
            self.emptyView?.transform = CGAffineTransform(scaleX: 1, y: -1)
            self.tableView.backgroundView = self.emptyView
            self.tableView.transform = CGAffineTransform(scaleX: 1, y: -1)
            
            self.tableView.rowHeight = UITableView.automaticDimension
            self.tableView.estimatedRowHeight = 44.0
            self.tableView.sectionHeaderHeight = 0
            
            self.addSubview(self.tableView)
            
            // channel state & common
            if self.channelStateBanner == nil {
                self.channelStateBanner = self.defaultChannelStateBanner
            }
            
            if let channelStateBanner = self.channelStateBanner {
                self.addSubview(channelStateBanner)
            }
            
            if self.newMessageInfoView == nil {
                self.newMessageInfoView = SBUNewMessageInfo()
            }
            
            if self.scrollBottomView == nil {
                self.scrollBottomView = self.defaultScrollBottomView
            }
            
            if self.userProfileView == nil {
                self.userProfileView = SBUUserProfileView(delegate: self)
            }
        }
        
        /// Sets layouts of the views in the list component.
        open func setupLayouts() {
            self.tableView.sbu_constraint(equalTo: self, left: 0, right: 0, top: 0, bottom: 0)
        }
        
        /// Sets styles of the views in the list component with the `theme`. If set theme parameter as `nil`, it uses the stored value.
        /// - Parameter theme: The object that is used as the theme of the list component. The theme must adopt the `SBUChannelTheme` class.
        open func setupStyles(theme: SBUChannelTheme? = nil) {
            if let theme = theme {
                self.theme = theme
            }
            if let channelStateBanner = channelStateBanner as? UILabel {
                channelStateBanner.textColor = theme?.channelStateBannerTextColor
                channelStateBanner.font = theme?.channelStateBannerFont
                channelStateBanner.backgroundColor = theme?.channelStateBannerBackgroundColor
            }
            self.tableView.backgroundColor = self.theme?.backgroundColor
        }
        
        /// Updates styles of the views in the list component with the `theme`.
        /// - Parameters:
        ///   - theme: The object that is used as the theme of the list component. The theme must adopt the `SBUChannelTheme` class.
        ///   - componentTheme: The object that is used as the theme of some UI component in the list component such as `scrollBottomView`. The theme must adopt the `SBUComponentTheme` class. The default value is `SBUTheme.componentTheme`
        open func updateStyles(theme: SBUChannelTheme? = nil, componentTheme: SBUComponentTheme = SBUTheme.componentTheme) {
            self.setupStyles(theme: theme)
            
            if let userProfileView = self.userProfileView as? SBUUserProfileView {
                userProfileView.setupStyles()
            }
        }
        
        /// Sets the styles of `scrollBottomView`.
        /// - Parameters:
        ///   - scrollBottomView: The `scrollBottomView` object.
        ///   - theme: The object that is used as the theme of the `scrollBottomView`. The theme must adopt the `SBUComponentTheme` class. The default value is `SBUTheme.componentTheme`.
        public func setupScrollBottomViewStyle(
            scrollBottomView: UIView,
            theme: SBUComponentTheme = SBUTheme.componentTheme
        ) {
            self.layer.shadowColor = theme.shadowColor.withAlphaComponent(0.5).cgColor
            
            guard let scrollBottomButton = scrollBottomView.subviews.first as? UIButton else { return }
            
            scrollBottomButton.layer.cornerRadius = scrollBottomButton.frame.height / 2
            scrollBottomButton.clipsToBounds = true
            
            scrollBottomButton.setImage(
                SBUIconSetType.iconChevronDown.image(
                    with: theme.scrollBottomButtonIconColor,
                    to: SBUIconSetType.Metric.iconChevronDown
                ),
                for: .normal)
            scrollBottomButton.backgroundColor = theme.scrollBottomButtonBackground
            scrollBottomButton.setBackgroundImage(
                UIImage.from(color: theme.scrollBottomButtonHighlighted),
                for: .highlighted
            )
        }
        
        /// Updates hidden state of the `scrollBottomView`.
        open func setScrollBottomView(hidden: Bool) {
            self.scrollBottomView?.isHidden = hidden
        }
        
        // MARK: - TableView
        /// Reloads table view. This method corresponds to `UITableView reloadData()`.
        public func reloadTableView(needsToLayout: Bool = true) {
            if let groupChannelModuleList = self as? SBUGroupChannelModule.List {
                groupChannelModuleList.shouldRedrawTypingBubble = groupChannelModuleList.decideToRedrawTypingBubble()
            }
            
            Thread.executeOnMain { [weak self] in
                if let isTableViewReloading = self?.isTableViewReloading, isTableViewReloading == true {
                    self?.needReload = true
                    return
                }
                self?.isTableViewReloading = true
                DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) {
                    self?.isTableViewReloading = false
                    if let needReload = self?.needReload, needReload == true {
                        self?.needReload = false
                        self?.reloadTableView()
                    }
                }
                SBULog.info("SBUBaseChannelViewController reloadTableView")
                self?.tableView.reloadData()
                if needsToLayout {
                    self?.tableView.layoutIfNeeded()
                }
            }
        }
        
        // MARK: - EmptyView
        
        /// Update the `emptyView` according its type.
        /// - Parameter type: The value of `EmptyViewType`.
        public func updateEmptyView(type: EmptyViewType) {
            if let emptyView = self.emptyView as? SBUEmptyView {
                emptyView.reloadData(type)
            }
        }
        
        // MARK: - Menu
        /// Displays the menu of the message located on the given `indexPath` value.
        /// It internally decides whether to show a *context menu*, a menu for *a failed message*, or a *sheet menu*.
        /// - Parameters:
        ///    - message: The `JMessage` object that corresponds to the message of the menu to show.
        ///    - indexPath: The value of the `UITableViewCell` where the `message` is located.
        open func showMessageMenu(on message: JMessage, reaction: JMessageReaction? = nil, forRowAt indexPath: IndexPath) {
            switch message.messageState {
            case .unknown, .sending, .uploading:
                break
            case .fail:
                // shows failed message menu
                showFailedMessageMenu(on: message)
            default:
                // sent
                guard let cell = self.tableView.cellForRow(at: indexPath) else {
                    SBULog.error("Couldn't find cell for row at \(indexPath)")
                    return
                }
                cell.isSelected = true
                if true {
                    // shows menu sheet view controller
                    self.showMessageMenuSheet(for: message, reaction: reaction, cell: cell)
                } else {
                    self.showMessageContextMenu(for: message, cell: cell, forRowAt: indexPath)
                }
            }
        }
        
        /// Displays the menu of a message that failed to send.
        /// - NOTE: The event delegate methods, ``SBUBaseChannelModuleListDelegate/baseChannelModule(_:didTapRetryFailedMessage:)`` and ``SBUBaseChannelModuleListDelegate/baseChannelModule(_:didTapDeleteFailedMessage:)`` , are called when the items in the menu are tapped.
        /// - Parameter message: The `JMessage` object that corresponds to the message of the menu to show.
        open func showFailedMessageMenu(on message: JMessage) {
            let retryItem = SBUActionSheetItem(
                title: SBUStringSet.Retry,
                color: self.theme?.menuItemTintColor
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapRetryFailedMessage: message)
                self.baseDelegate?.baseChannelModuleDidTapScrollToButton(self, animated: true)
            }
            let deleteItem = SBUActionSheetItem(
                title: SBUStringSet.Delete,
                color: self.theme?.deleteItemColor
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapDeleteFailedMessage: message)
            }
            let cancelItem = SBUActionSheetItem(
                title: SBUStringSet.Cancel,
                color: self.theme?.cancelItemColor,
                completionHandler: nil
            )
            
            SBUActionSheet.show(
                items: [retryItem, deleteItem],
                cancelItem: cancelItem
            )
        }
        
        /// Displays an alert for deleting a message.
        /// - NOTE: The event delegate method, ``SBUBaseChannelModuleListDelegate/baseChannelModule(_:didTapDeleteMessage:)``, is called when an item in the menu is tapped.
        /// - Parameters:
        ///    - message: The message that is to be deleted.
        ///    - oneTimeTheme: The theme applied to the alert. If there's no set theme, the default theme in Sendbird UIKit is used.
        open func showDeleteMessageAlert(on message: JMessage, oneTimeTheme: SBUComponentTheme? = nil) {
            let deleteButton = SBUAlertButtonItem(
                title: SBUStringSet.Delete,
                color: self.theme?.alertRemoveColor
            ) { [weak self, message] _ in
                guard let self = self else { return }
                SBULog.info("[Request] Delete message: \(message.description)")
                self.baseDelegate?.baseChannelModule(self, didTapDeleteMessage: message)
            }
            
            let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
            
            let title = SBUStringSet.Alert_Delete
            
            SBUAlertView.show(
                title: title,
                oneTimetheme: oneTimeTheme,
                confirmButtonItem: deleteButton,
                cancelButtonItem: cancelButton
            )
        }
        
        open func showRecallMessageAlert(on message: JMessage, oneTimeTheme: SBUComponentTheme? = nil) {
            let recallButton = SBUAlertButtonItem(
                title: SBUStringSet.Recall,
                color: self.theme?.alertRemoveColor
            ) { [weak self, message] _ in
                guard let self = self else { return }
                SBULog.info("[Request] Remove message: \(message.description)")
                self.baseDelegate?.baseChannelModule(self, didTapRecallMessage: message)
            }
            
            let cancelButton = SBUAlertButtonItem(title: SBUStringSet.Cancel) { _ in }
            
            let title = SBUStringSet.Alert_Recall
            
            SBUAlertView.show(
                title: title,
                oneTimetheme: oneTimeTheme,
                confirmButtonItem: recallButton,
                cancelButtonItem: cancelButton
            )
        }
        
        /// Calls the ``SBUMenuSheetViewController`` instance in ``UIViewController``, which is returned after ``SBUBaseChannelModuleListDataSource/baseChannelModule(_:parentViewControllerDisplayMenuItems:)`` is called.
        /// - NOTE: To learn about the event delegates in this instance, refer to the event delegate methods in ``SBUBaseChannelModuleListDelegate``.
        /// - Parameters:
        ///    - message: The `JMessage` object  that refers to the message of the menu to display.
        ///    - cell: The `UITableViewCell` object that shows the message.
        open func showMessageMenuSheet(for message: JMessage, reaction: JMessageReaction? = nil, cell: UITableViewCell) {
            let messageMenuItems = self.createMessageMenuItems(for: message)
            
            guard let parentViewController = self.baseDataSource?.baseChannelModule(
                self,
                parentViewControllerDisplayMenuItems: messageMenuItems
            ) else { return }
            
            let useReaction = true//SBUEmojiManager.isReactionEnabled(channel: self.baseChannel)
            
            let menuSheetVC = SBUMenuSheetViewController(message: message, reaction: reaction, items: messageMenuItems, useReaction: useReaction)
            menuSheetVC.modalPresentationStyle = .custom
            menuSheetVC.transitioningDelegate = parentViewController as? UIViewControllerTransitioningDelegate
            parentViewController.present(menuSheetVC, animated: true)
            menuSheetVC.dismissHandler = { [weak self, cell] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didDismissMenuForCell: cell)
            }
            menuSheetVC.emojiTapHandler = { [weak self, message] emojiKey, setSelect in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(
                    self,
                    didReactToMessage: message,
                    withEmoji: emojiKey,
                    selected: setSelect
                )
            }
            menuSheetVC.moreEmojiTapHandler = { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(
                    self,
                    didTapMoreEmojisOnMessage: message
                )
            }
        }
        
        /// Displays ``SBUMenuView`` in the form of a context menu.
        /// - NOTE: To learn about the event delegates in ``SBUMenuView``, refer to the even delegate method of each menu item in ``SBUBaseChannelModuleListDelegate``.
        /// - Parameters:
        ///    - message: The `JMessage` object  that refers to the message of the menu to display.
        ///    - cell: The `UITableViewCell` object that shows the message.
        ///    - indexPath: The `IndexPath` value of the `cell`.
        open func showMessageContextMenu(for message: JMessage, cell: UITableViewCell, forRowAt indexPath: IndexPath) {
            let messageMenuItems = self.createMessageMenuItems(for: message)
            guard !messageMenuItems.isEmpty else { return }
            
            let rowRect = self.tableView.rectForRow(at: indexPath)
            let rowRectInSuperview = self.tableView.convert(
                rowRect,
                to: UIApplication.shared.currentWindow
            )
            SBUMenuView.show(items: messageMenuItems, point: rowRectInSuperview.origin) {
                cell.isSelected = false
            }
        }
        
        /// Creates an array of ``SBUMenuItem`` objects for a `message`.
        /// - Parameter message: The `JMessage` object  that refers to the message of the menu to display.
        /// - Returns: The array of ``SBUMenuItem`` objects for a `message`
        open func createMessageMenuItems(for message: JMessage) -> [SBUMenuItem] {
            let isSentByMe = message.direction == .send
            var items: [SBUMenuItem] = []
            
            switch message.content {
            case is JTextMessage:
                let copy = self.createCopyMenuItem(for: message)
                items.append(copy)
                if isSentByMe {
                    let edit = self.createEditMenuItem(for: message)
                    items.append(edit)
                }
                
            case is JImageMessage, is JVideoMessage, is JFileMessage:
                let save = self.createSaveMenuItem(for: message)
                items.append(save)
                
            
                
//            case is UserMessage:
//                // UserMessage: copy, (edit), (delete)
//                let copy = self.createCopyMenuItem(for: message)
//                items.append(copy)
//                if isSentByMe {
//                    let edit = self.createEditMenuItem(for: message)
//                    let delete = self.createDeleteMenuItem(for: message)
//                    items.append(edit)
//                    items.append(delete)
//                }
//            case let JMessage as JMessage:
//                // JMessage: save, (delete)
//                let save = self.createSaveMenuItem(for: message)
//                if SBUUtils.getFileType(by: JMessage) != .voice {
//                    items.append(save)
//                }
//                if isSentByMe {
//                    let delete = self.createDeleteMenuItem(for: message)
//                    items.append(delete)
//                }
//            case is MultipleFilesMessage:
//                // MultipleFilesMessage: delete
//                if isSentByMe {
//                    let delete = self.createDeleteMenuItem(for: message)
//                    items.append(delete)
//                }
            default:
                // UnknownMessage: (delete)
                break
//                if !isSentByMe {
//                    let delete = self.createDeleteMenuItem(for: message)
//                    items.append(delete)
//                }
            }
            let delete = self.createDeleteMenuItem(for: message)
            items.append(delete)
            
            if isSentByMe {
                let recall = self.createRecallMenuItem(for: message)
                items.append(recall)
            }
            
            let forward = self.createForwardMenuItem(for: message)
            items.append(forward)
            return items
        }
        
        /// Creates a ``SBUMenuItem`` object that allows users to *copy* the `message`.
        /// - Parameter message: The `JMessage` object  that corresponds to the message of the menu item to show.
        /// - Returns: The ``SBUMenuItem`` object for a `message`
        open func createCopyMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Copy,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconCopy.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapCopyMessage: message)
            }
            return menuItem
        }
        
        /// Creates a ``SBUMenuItem`` object that allows users to *delete* the `message`.
        /// - Parameter message: The `JMessage` object  that corresponds to the message of the menu item to show.
        /// - Returns: The ``SBUMenuItem`` object for a `message`
        open func createDeleteMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Delete,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconDelete.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.showDeleteMessageAlert(on: message)
            }
            menuItem.isEnabled = true//message.threadInfo.replyCount == 0
            return menuItem
        }
        
        open func createForwardMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Forward,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconThread.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapForwardMessage: message)
            }
            menuItem.isEnabled = true//message.threadInfo.replyCount == 0
            return menuItem
        }
        
        open func createRecallMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Recall,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconDelete.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.showRecallMessageAlert(on: message)
            }
            menuItem.isEnabled = true//message.threadInfo.replyCount == 0
            return menuItem
        }
        
        /// Creates a ``SBUMenuItem`` object that allows users to *edit* the `message`.
        /// - Parameter message: The `JMessage` object  that corresponds to the message of the menu item to show.
        /// - Returns: The ``SBUMenuItem`` object for a `message`
        open func createEditMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Edit,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconEdit.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapEditMessage: message)
            }
            return menuItem
        }
        
        /// Creates a ``SBUMenuItem`` object that allows users to *save* the `message` when it's a *file message*.
        /// - Parameter message: The `JMessage` object  that corresponds to the message of the menu item to show.
        /// - Returns: The ``SBUMenuItem`` object for a `message`
        open func createSaveMenuItem(for message: JMessage) -> SBUMenuItem {
            let menuItem = SBUMenuItem(
                title: SBUStringSet.Save,
                color: theme?.menuTextColor,
                image: SBUIconSetType.iconDownload.image(
                    with: SBUTheme.componentTheme.alertButtonColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapSaveMessage: message)
            }
            return menuItem
        }
        
        /// Creates a ``SBUMenuItem`` object that allows users to *reply* to a `message`.
        /// - Parameter message: The `JMessage` object  that corresponds to the message of the menu item to show.
        /// - Returns: The ``SBUMenuItem`` object for a `message`
        open func createReplyMenuItem(for message: JMessage) -> SBUMenuItem {
            let isThreadType = false//SendbirdUI.config.groupChannel.channel.replyType == .thread
            let replyMenuTitle = isThreadType
            ? SBUStringSet.MessageThread.Menu.replyInThread
            : SBUStringSet.Reply
            let iconSet = isThreadType
            ? SBUIconSetType.iconThread
            : SBUIconSetType.iconReply
            
            let isEnabled = true//message.parentMessage == nil
            
            let menuItem = SBUMenuItem(
                title: replyMenuTitle,
                color: isEnabled
                ? self.theme?.menuTextColor
                : SBUTheme.componentTheme.actionSheetDisabledColor,
                image: iconSet.image(
                    with: isEnabled
                    ? SBUTheme.componentTheme.alertButtonColor
                    : SBUTheme.componentTheme.actionSheetDisabledColor,
                    to: SBUIconSetType.Metric.iconActionSheetItem
                )
            ) { [weak self, message] in
                guard let self = self else { return }
                self.baseDelegate?.baseChannelModule(self, didTapReplyMessage: message)
            }
            menuItem.isEnabled = isEnabled
            menuItem.transitionsWhenSelected = isThreadType
            return menuItem
        }
        
        // MARK: - Actions
        
        /// Sets up the cell's tap gesture for handling the message.
        /// - Parameters:
        ///   - cell: Message cell object
        ///   - message: Message object
        ///   - indexPath: indexpath of cell
        open func setTapGesture(_ cell: UITableViewCell, message: JMessage, indexPath: IndexPath) {
            if message.content is JVoiceMessage {
                self.baseDelegate?.baseChannelModule(
                    self,
                    didTapVoiceMessage: message,
                    cell: cell,
                    forRowAt: indexPath
                )
            } else {
                self.baseDelegate?.baseChannelModule(
                    self,
                    didTapMessage: message,
                    forRowAt: indexPath
                )
            }
        }
        
        /// This function sets the cell's long tap gesture handling.
        /// - Parameters:
        ///   - cell: Message cell object
        ///   - message: Message object
        ///   - indexPath: indexpath of cell
        open func setLongTapGesture(_ cell: UITableViewCell, message: JMessage, reaction: JMessageReaction? = nil,  indexPath: IndexPath) {
            self.baseDelegate?.baseChannelModule(self, didLongTapMessage: message, reaction: reaction, forRowAt: indexPath)
        }
        
        /// This function sets the user profile tap gesture handling.
        ///
        /// If you do not want to use the user profile function, override this function and leave it empty.
        /// - Parameter user: `SBUUser` object used for user profile configuration
        open func setUserProfileTapGesture(_ user: SBUUser) {
            self.baseDelegate?.baseChannelModule(self, didTapUserProfile: user)
        }
        
        /// Moves scroll to bottom.
        open func onTapScrollToBottom() {
            let position = SBUScrollPostionConfiguration.getConfiguration().scrollToBottom
            let options = SBUScrollOptions(position: position, isInverted: self.tableView.isInverted)
            self.baseDelegate?.baseChannelModule(
                self,
                didSelectScrollToBottonWithOptions: options,
                animated: false
            )
        }
        
        // MARK: - UITableViewDelegate, UITableViewDataSource
        /// Called when the `scrollView` has been scrolled.
        open func scrollViewDidScroll(_ scrollView: UIScrollView) {
            guard scrollView == self.tableView else { return }
            self.baseDelegate?.baseChannelModule(self, didScroll: scrollView)
        }
        
        open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
            nil
        }

        open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
            0
        }

        open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
            return self.fullMessageList.count
        }
        
        open func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
            preconditionFailure("Needs to implement this method")
        }
        
        open func tableView(
            _ tableView: UITableView,
            willDisplay cell: UITableViewCell,
            forRowAt indexPath: IndexPath
        ) {
            self.baseDelegate?.baseChannelModule(self, willDisplay: cell, forRowAt: indexPath)
        }
        
        open func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
            return UITableView.automaticDimension
        }
        
        open func tableView(_ tableView: UITableView, didEndDisplaying cell: UITableViewCell, forRowAt indexPath: IndexPath) {
            
        }
        
        /// Sets images in file message cell. (for not succeeded message)
        /// - Parameters:
        ///   - cell: File message cell
        ///   - JMessage: File message object
        open func setMediaMessageCellImage(_ cell: UITableViewCell, mediaMessage: JMessage) {
//            switch JMessage.messageState {
//            case .fail, .unknown, .sending, .uploading:
////                guard let (pendingMessageManager, isThreadMessage) = self.baseDataSource?.baseChannelModule(self, pendingMessageManagerForCell: cell),
////                      let fileInfo = pendingMessageManager?.getFileInfo(
////                        requestId: JMessage.requestId,
////                        forMessageThread: isThreadMessage ?? false
////                      ),
////                      let type = fileInfo.mimeType, let fileData = fileInfo.file,
////                      SBUUtils.getFileType(by: type) == .image else { return }
////
////                let image = UIImage.createImage(from: fileData)
////                let isAnimatedImage = image?.isAnimatedImage() == true
////
////                if let cell = cell as? SBUBaseMessageCell {
////                    cell.setImage(
////                        isAnimatedImage ? image?.images?.first : image,
////                        size: SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize
////                    )
////                }
//            case .sent:
//                break
//            @unknown default:
//                SBULog.error("unknown Type")
//            }
        }
        
        /// Gets the position of the message to be grouped.
        ///
        /// Only successful messages can be grouped.
        /// - Parameter currentIndex: Index of current message in the message list
        /// - Returns: Position of a message when grouped
        public func getMessageGroupingPosition(currentIndex: Int) -> MessageGroupPosition {
            
            guard currentIndex < self.fullMessageList.count else { return .none }
            
            var prevMessage = self.fullMessageList.count - 1 != currentIndex
            ? self.fullMessageList[currentIndex+1]
            : nil
            var currentMessage = self.fullMessageList[currentIndex]
            var nextMessage = currentIndex != 0
            ? self.fullMessageList[currentIndex-1]
            : nil
            
            if !self.isTransformedList {
                prevMessage = currentIndex != 0
                ? self.fullMessageList[currentIndex-1]
                : nil
                currentMessage = self.fullMessageList[currentIndex]
                nextMessage = self.fullMessageList.count - 1 != currentIndex
                ? self.fullMessageList[currentIndex+1]
                : nil
            }
            
            let succeededPrevMsg = prevMessage?.messageState != .fail
            ? prevMessage
            : nil
            let succeededCurrentMsg = currentMessage.messageState != .fail
            ? currentMessage
            : nil
            let succeededNextMsg = nextMessage?.messageState != .fail
            ? nextMessage
            : nil
            
            // Unit : milliseconds
            let prevTimestamp = Date
                .sbu_from(succeededPrevMsg?.timestamp ?? -1)
                .sbu_toString(dateFormat: SBUDateFormatSet.yyyyMMddhhmm)
            
            let currentTimestamp = Date
                .sbu_from(succeededCurrentMsg?.timestamp ?? -1)
                .sbu_toString(dateFormat: SBUDateFormatSet.yyyyMMddhhmm)
            
            let nextTimestamp = Date
                .sbu_from(succeededNextMsg?.timestamp ?? -1)
                .sbu_toString(dateFormat: SBUDateFormatSet.yyyyMMddhhmm)
            
            // Check sender
            var prevSender = succeededPrevMsg?.senderUserId ?? nil
            var currentSender = succeededCurrentMsg?.senderUserId ?? nil
            var nextSender = succeededNextMsg?.senderUserId ?? nil
            
//            // Check thread info
//            if SendbirdUI.config.groupChannel.channel.replyType == .thread {
//                let prevThreadReplycount = succeededPrevMsg?.threadInfo.replyCount ?? 0
//                let currentThreadReplycount = succeededCurrentMsg?.threadInfo.replyCount ?? 0
//                let nextThreadReplycount = succeededNextMsg?.threadInfo.replyCount ?? 0
//
//                if prevThreadReplycount > 0 {
//                    prevSender = nil
//                }
//                if currentThreadReplycount > 0 {
//                    currentSender = nil
//                }
//                if nextThreadReplycount > 0 {
//                    nextSender = nil
//                }
//            }
            
            if (prevSender != currentSender && nextSender != currentSender) || currentSender == nil {
                return .none
            } else if prevSender == currentSender && nextSender == currentSender {
                if prevTimestamp == nextTimestamp {
                    return .middle
                } else if prevTimestamp == currentTimestamp {
                    return .bottom
                } else if currentTimestamp == nextTimestamp {
                    return .top
                }
            } else if prevSender == currentSender && nextSender != currentSender {
                return prevTimestamp == currentTimestamp ? .bottom : .none
            } else if prevSender != currentSender && nextSender == currentSender {
                return currentTimestamp == nextTimestamp ? .top : .none
            }
            
            return .none
        }
    }
}

// MARK: - SBUEmptyViewDelegate
extension SBUBaseChannelModule.List: SBUEmptyViewDelegate {
    /// Reload data from the channel. This function invokes `SBUBaseChannelModuleListDelegate baseChannelModuleDidSelectRetry(_:)`
    open func didSelectRetry() {
        if let emptyView = self.emptyView as? SBUEmptyView {
            emptyView.reloadData(.noMessages)
        }
        
        SBULog.info("[Request] Retry load channel list")
        self.baseDelegate?.baseChannelModuleDidSelectRetry(self)
    }
}

// MARK: - SBUUserProfileViewDelegate
extension SBUBaseChannelModule.List: SBUUserProfileViewDelegate {
    open func didSelectClose() {
        // Implementation
        if let userProfileView = self.userProfileView as? SBUUserProfileViewProtocol {
            userProfileView.dismiss()
        }
    }
    
    open func didSelectMessage(userId: String?) {
        // Implementation
        if let userProfileView = self.userProfileView
            as? SBUUserProfileViewProtocol {
            userProfileView.dismiss()
            if let userId = userId {
//                SendbirdUI.createAndMoveToChannel(userIds: [userId])
            }
        }
    }
}

// MARK: - UITableViewCell
extension SBUBaseChannelModule.List {
    /// This property checks if the scroll is near the bottom of the screen.
    public var isScrollNearByBottom: Bool {
        tableView.contentOffset.y < 10
    }
    
    /// To keep track of which scrolls tableview.
    func scrollTableView(
        to row: Int,
        at position: UITableView.ScrollPosition = .top,
        animated: Bool = false
    ) {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            if self.tableView.numberOfRows(inSection: 0) <= row ||
                row < 0 {
                return
            }
            
            let isScrollable = !self.fullMessageList.isEmpty
                && row >= 0
                && row < self.fullMessageList.count
            
            if isScrollable {
                self.tableView.scrollToRow(
                    at: IndexPath(row: row, section: 0),
                    at: position,
                    animated: animated
                )
            } else {
                guard self.tableView.contentOffset != .zero else { return }
                self.tableView.setContentOffset(.zero, animated: false)
            }
        }
    }
    
    /// This function keeps the current scroll position with upserted messages.
    /// - Note: Only newly added messages are used for processing.
    /// - Parameter upsertedMessages: upserted messages
    func keepCurrentScroll(for upsertedMessages: [JMessage]) -> IndexPath {
        let firstVisibleIndexPath = tableView
            .indexPathsForVisibleRows?.first ?? IndexPath(row: 0, section: 0)
        var nextInsertedCount = 0
        if let newestMessage = sentMessages.first {
            // only filter out messages inserted at the bottom (newer) of current visible item
            nextInsertedCount = upsertedMessages
                .filter({ $0.timestamp > newestMessage.timestamp })
                .filter({ !SBUUtils.contains(clientMsgNo: $0.clientMsgNo, in: sentMessages) }).count
        }
        
        SBULog.info("New messages inserted : \(nextInsertedCount)")
        return IndexPath(
            row: firstVisibleIndexPath.row + nextInsertedCount,
            section: 0
        )
    }
    
    /// Scrolls tableview to initial position.
    /// If starting point is set, scroll to the starting point at `.middle`.
    func scrollToInitialPosition() {
//        let config = SBUScrollPostionConfiguration.getConfiguration(with: self.baseChannel)
//        
//        guard let startingPoint = self.baseDataSource?.baseChannelModule(self, startingPointIn: self.tableView) else {
//            self.scrollTableView(to: 0, at: config.scrollToInitial.transform(with: self.tableView))
//            return
//        }
//
//        if let index = fullMessageList.firstIndex(where: { $0.timestamp <= startingPoint }) {
//            self.scrollTableView(to: index, at: config.scrollToInitialWithStartingPoint.transform(with: self.tableView))
//        } else {
//            self.scrollTableView(to: fullMessageList.count - 1, at: config.scrollToInitial.transform(with: self.tableView))
//        }
    }
    
    /// Scrolls to the message that is found by `id`.
    /// - Parameters:
    ///    - messageId: The identifier of the message that is wanted to find
    ///    - enablesScrollAnimation: The boolean value whether scrolls to the message with animation or not. If `false`, it *jumps* to the message.
    ///    - enablesMessageAnimation: The boolean value whether animate the message after the scrolling. If `true`, the message is shaked up and down.
    ///    - position: Scroll position value.
    ///    - needToSearch: If `true`, reloads the collection based on the message information if not found by message ID.
    /// - Returns: `false` when it couldn't find message with `id`. If it returns `true`, ``SBUBaseChannelModuleListDelegate/baseChannelModule(_:didScroll:)`` is called.
    @discardableResult
    public func scrollToMessage(
        id messageId: Int64,
        enablesScrollAnimation scrollAnimated: Bool = false,
        enablesMessageAnimation messageAnimated: Bool = false,
        position: SBUScrollPosition = .middle,
        needToSearch: Bool = true
    ) -> Bool {
        guard let row = self.fullMessageList.firstIndex(
            where: { $0.clientMsgNo == messageId }
        ) else {
            SBULog.error("Couldn't find message with ID: \(messageId)")
            self.baseDelegate?.baseChannelModule(self, didFailScrollToMessageId: messageId, needToSearch: needToSearch)
            return false
        }
        
        self.moveToRow(
            row: row,
            scrollAnimated: scrollAnimated,
            messageAnimated: messageAnimated,
            position: position
        )
        return true
    }
    
    /// Scrolls to the message that is found by message id.
    /// If no such message currently exists, it will attempt to initialize the message list with the time of that message.
    /// - Parameters:
    ///    - message: The message that is wanted to find
    ///    - enablesScrollAnimation: The boolean value whether scrolls to the message with animation or not. If `false`, it *jumps* to the message.
    ///    - enablesMessageAnimation: The boolean value whether animate the message after the scrolling. If `true`, the message is shaked up and down.
    ///    - position: Scroll position value.
    ///    - needToSearch: If true, reloads the collection based on the message information if not found by message ID.
    /// - Returns: `false` when it couldn't find message with `id`. If it returns `true`, ``SBUBaseChannelModuleListDelegate/baseChannelModule(_:didScroll:)`` is called.
    @discardableResult
    public func scrollToMessage(
        message: JMessage,
        enablesScrollAnimation scrollAnimated: Bool = false,
        enablesMessageAnimation messageAnimated: Bool = false,
        position: SBUScrollPosition = .middle,
        needToSearch: Bool = true
    ) -> Bool {
        guard let row = self.fullMessageList.firstIndex(
            where: { $0.messageId == message.messageId }
        ) else {
            SBULog.error("Couldn't find message with ID: \(message.messageId)")
            self.baseDelegate?.baseChannelModule(self, didFailScrollToMessage: message, needToSearch: needToSearch)
            return false
        }
        
        self.moveToRow(
            row: row,
            scrollAnimated: scrollAnimated,
            messageAnimated: messageAnimated,
            position: position
        )
        return true
    }
    
    fileprivate func moveToRow(row: Int, scrollAnimated: Bool, messageAnimated: Bool, position: SBUScrollPosition) {
        let indexPath = IndexPath(row: row, section: 0)
        self.tableView.scrollToRow(
            at: indexPath,
            at: position.transform(with: self.tableView),
            animated: scrollAnimated
        )
        defer {
            self.baseDelegate?.baseChannelModule(self, didScroll: self.tableView)
        }
        
        if messageAnimated {
            guard let cell = self.tableView.cellForRow(at: indexPath) as? SBUBaseMessageCell else {
                SBULog.error("The cell for row at \(indexPath) is not `SBUBaseMessageCell`")
                return
            }
            cell.messageContentView.animate(.shakeUpDown)
        }
    }
    
    /// This function checks if the current message and the next message date have the same day.
    /// - Parameters:
    ///   - currentIndex: Current message index
    ///   - fullMessageList: The full message list including failed/pending messages as well as sent messages
    /// - Returns: If `true`, the messages date is same day.
    public func checkSameDayAsNextMessage(currentIndex: Int, fullMessageList: [JMessage]) -> Bool {
        guard currentIndex < fullMessageList.count-1 else { return false }
        
        let currentMessage = fullMessageList[currentIndex]
        let nextMessage = fullMessageList[currentIndex+1]
        
        let curCreatedAt = currentMessage.timestamp
        let nextCreatedAt = nextMessage.timestamp
        
        return Date.sbu_from(nextCreatedAt).isSameDay(as: Date.sbu_from(curCreatedAt))
    }
    
    /// This function checks if the current message and the previous message date have the same day.
    /// - Parameters:
    ///   - currentIndex: Current message index
    ///   - fullMessageList: The full message list including failed/pending messages as well as sent messages
    /// - Returns: If `true`, the messages date is same day.
    public func checkSameDayAsPrevMessage(currentIndex: Int, fullMessageList: [JMessage]) -> Bool {
        guard currentIndex < fullMessageList.count,
              currentIndex > 0 else { return false }
        
        let currentMessage = fullMessageList[currentIndex]
        let prevMessage = fullMessageList[currentIndex-1]
        
        let curCreatedAt = currentMessage.timestamp
        let prevCreatedAt = prevMessage.timestamp
        
        return Date.sbu_from(prevCreatedAt).isSameDay(as: Date.sbu_from(curCreatedAt))
    }
    
    // MARK: - Util
    
    /// Reloads a SBUMultipleFilesMessageCollectionViewCell when a file is finished being uploaded.
    func reloadMultipleFilesMessageCollectionViewCell(requestId: String, index: Int) {
//        for cell in self.tableView.visibleCells {
//            if let cell = cell as? SBUMultipleFilesMessageCell,
//               let message = cell.message,
//               message.requestId == requestId {
//                guard let indexPath = tableView.indexPath(for: cell) else { return }
//                guard let visibleIndexPaths = tableView.indexPathsForVisibleRows else { return }
//                guard visibleIndexPaths.contains(indexPath) else { return }
//
//                let collectionViewIndexPath = IndexPath(item: index, section: 0)
//                cell.uploadedIndices.append(index)
//
//                if Thread.isMainThread {
//                    cell.collectionView.reloadItems(at: [collectionViewIndexPath])
//                    cell.collectionView.layoutIfNeeded()
//                } else {
//                    DispatchQueue.main.async {
//                        cell.collectionView.reloadItems(at: [collectionViewIndexPath])
//                        cell.collectionView.layoutIfNeeded()
//                    }
//                }
//            }
//        }
    }
    
    /// Reload cell if it's visible status.
    /// - Parameter cell: `SBUBaseMessageCell`.
    /// - Since: 3.15.0
    public func reloadCell(_ cell: SBUBaseMessageCell?) {
        guard let cell = cell, let indexPath = tableView.indexPath(for: cell) else { return }
        guard let visibleIndexPaths = tableView.indexPathsForVisibleRows else { return }
        guard visibleIndexPaths.contains(indexPath) else { return }
        self.tableView.reloadRows(at: [indexPath], with: .none)
        self.tableView.layoutIfNeeded()
    }
}
