//
//  SBUGroupChannelModule.List.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2021/09/30.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import AVFAudio
import JuggleIM

/// Event methods for the views updates and performing actions from the list component in a group channel.
public protocol SBUGroupChannelModuleListDelegate: SBUBaseChannelModuleListDelegate {
    
    /// Called when tapped emoji in the cell.
    /// - Parameters:
    ///   - emojiKey: emoji key
    ///   - messageCell: Message cell object
    func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapEmoji emojiKey: String, messageCell: SBUBaseMessageCell)
    
    /// Called when long tapped emoji in the cell.
    /// - Parameters:
    ///   - emojiKey: emoji key
    ///   - messageCell: Message cell object
    func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didLongTapEmoji emojiKey: String, messageCell: SBUBaseMessageCell)
    
    /// Called when tapped the cell to get more emoji
    /// - Parameters:
    ///   - messageCell: Message cell object
    func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapMoreEmojiForCell messageCell: SBUBaseMessageCell)
    
    /// Called when tapped the mentioned nickname in the cell.
    /// - Parameters:
    ///    - user: The`SBUUser` object from the tapped mention.
    func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapMentionUser user: SBUUser)
}

/// Methods to get data source for list component in a group channel.
public protocol SBUGroupChannelModuleListDataSource: SBUBaseChannelModuleListDataSource {
    /// Ask to data source to return the highlight info
    /// - Parameters:
    ///    - listComponent: `SBUGroupChannelModule.List` object.
    ///    - tableView: `UITableView` object from list component.
    /// - Returns: `SBUHightlightMessageInfo` object.
    func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, highlightInfoInTableView tableView: UITableView) -> SBUHighlightMessageInfo?
}

extension SBUGroupChannelModule {
    /// A module component that represent the list of ``SBUGroupChannelModule``.
    @objc(SBUGroupChannelModuleList)
    @objcMembers
    open class List: SBUBaseChannelModule.List, SBUVoicePlayerDelegate {

        // MARK: - UI properties (Public)
        
        /// A high light information of the message with ID and updated time.
        public var highlightInfo: SBUHighlightMessageInfo? {
            self.dataSource?.groupChannelModule(self, highlightInfoInTableView: self.tableView)
        }
        
        /// When message have highlightInfo, it is used to make sure it has been animated.
        /// - Since: 3.4.0
        public var isHighlightInfoAnimated: Bool = false
        
        // MARK: - Logic properties (Public)
        /// The object that acts as the delegate of the list component. The delegate must adopt the `SBUGroupChannelModuleListDelegate`.
        public weak var delegate: SBUGroupChannelModuleListDelegate? {
            get { self.baseDelegate as? SBUGroupChannelModuleListDelegate }
            set { self.baseDelegate = newValue }
        }
        
        /// The object that acts as the data source of the list component. The data source must adopt the `SBUGroupChannelModuleListDataSource`.
        public weak var dataSource: SBUGroupChannelModuleListDataSource? {
            get { self.baseDataSource as? SBUGroupChannelModuleListDataSource }
            set { self.baseDataSource = newValue }
        }
        
        // MARK: Private properties
        public var voicePlayer: SBUVoicePlayer?
        var voiceFileInfos: [String: SBUVoiceFileInfo] = [:]
        var currentVoiceFileInfo: SBUVoiceFileInfo?
        var currentVoiceContentView: SBUVoiceContentView?
        var currentVoiceContentIndexPath: IndexPath?
        var cellDict: [String: AnyClass] = [:]
        
        var shouldRedrawTypingBubble: Bool = false

        /// Configures component with parameters.
        /// - Parameters:
        ///   - delegate: `SBUGroupChannelModuleListDelegate` type listener
        ///   - dataSource: The data source that is type of `SBUGroupChannelModuleListDataSource`
        ///   - theme: `SBUChannelTheme` object
        open func configure(
            delegate: SBUGroupChannelModuleListDelegate,
            dataSource: SBUGroupChannelModuleListDataSource,
            theme: SBUChannelTheme
        ) {
            self.delegate = delegate
            self.dataSource = dataSource
            self.theme = theme
            
            self.setupViews()
            self.setupLayouts()
            self.setupStyles()
        }
        
        // MARK: - LifeCycle
        
        open override func setupViews() {
            super.setupViews()
            
            self.register(cell: SBUTextMessageCell(), contentType: JTextMessage.contentType())
            self.register(cell: SBURecallMessageCell(), contentType: JRecallInfoMessage.contentType())
            self.register(cell: SBUMediaMessageCell(), contentType: JImageMessage.contentType())
            self.register(cell: SBUMediaMessageCell(), contentType: JVoiceMessage.contentType())
            self.register(cell: SBUMediaMessageCell(), contentType: JVideoMessage.contentType())
            self.register(cell: SBUMediaMessageCell(), contentType: JFileMessage.contentType())
            self.register(cell: SBUCallMessageCell(), contentType: JCallFinishNotifyMessage.contentType())
            
            if let newMessageInfoView = self.newMessageInfoView {
                newMessageInfoView.isHidden = true
                self.addSubview(newMessageInfoView)
            }

            if let scrollBottomView = self.scrollBottomView {
                scrollBottomView.isHidden = true
                self.addSubview(scrollBottomView)
            }
            
            self.voicePlayer = SBUVoicePlayer(delegate: self)
        }
        
        open override func setupLayouts() {
            super.setupLayouts()
            
            self.channelStateBanner?
                .sbu_constraint(equalTo: self, leading: 8, trailing: -8, top: 8)
                .sbu_constraint(height: 24)
            
            (self.newMessageInfoView as? SBUNewMessageInfo)?
                .sbu_constraint(equalTo: self, bottom: 8, centerX: 0)
            
            self.scrollBottomView?
                .sbu_constraint(
                    width: SBUConstant.scrollBottomButtonSize.width,
                    height: SBUConstant.scrollBottomButtonSize.height
                )
                .sbu_constraint(equalTo: self, trailing: -16, bottom: 8)
        }
        
        /// Updates styles of the views in the list component with the `theme`.
        /// - Parameters:
        ///   - theme: The object that is used as the theme of the list component. The theme must adopt the `SBUChannelTheme` class. The default value is `nil` to use the stored value.
        ///   - componentTheme: The object that is used as the theme of some UI component in the list component such as `scrollBottomView`. The theme must adopt the `SBUComponentTheme` class. The default value is `SBUTheme.componentTheme`
        open override func updateStyles(theme: SBUChannelTheme? = nil, componentTheme: SBUComponentTheme = SBUTheme.componentTheme) {
            super.updateStyles(theme: theme, componentTheme: componentTheme)
            
            if let scrollBottomView = self.scrollBottomView {
                setupScrollBottomViewStyle(scrollBottomView: scrollBottomView, theme: componentTheme)
            }
            
            (self.newMessageInfoView as? SBUNewMessageInfo)?.setupStyles()
            (self.emptyView as? SBUEmptyView)?.setupStyles()
        }
        
        // MARK: - Scroll View
        open override func setScrollBottomView(hidden: Bool) {
            let hasNext = self.dataSource?.baseChannelModule(self, hasNextInTableView: self.tableView) ?? false
            let isHidden = hidden && !hasNext
            guard self.scrollBottomView?.isHidden != isHidden else { return }
            self.scrollBottomView?.isHidden = isHidden
        }
        
        open override func scrollViewDidScroll(_ scrollView: UIScrollView) {
            super.scrollViewDidScroll(scrollView)
            
            self.setScrollBottomView(hidden: isScrollNearByBottom)
        }
        
        // MARK: - EmptyView
        
        // MARK: - Menu
        @available(*, deprecated, renamed: "calculateMessageMenuCGPoint(indexPath:position:)")
        public func calculatorMenuPoint(
            indexPath: IndexPath,
            position: MessagePosition
        ) -> CGPoint {
            self.calculateMessageMenuCGPoint(indexPath: indexPath, position: position)
        }
        
        /// Calculates the `CGPoint` value that indicates where to draw the message menu in the group channel screen.
        /// - Parameters:
        ///   - indexPath: The index path of the selected message cell
        ///   - position: Message position
        /// - Returns: `CGPoint` value
        open func calculateMessageMenuCGPoint(
            indexPath: IndexPath,
            position: MessagePosition
        ) -> CGPoint {
            let rowRect = self.tableView.rectForRow(at: indexPath)
            let rowRectInSuperview = self.tableView.convert(
                rowRect,
                to: UIApplication.shared.currentWindow
            )
            
            let originX = (position == .right) ? rowRectInSuperview.width : rowRectInSuperview.origin.x
            let menuPoint = CGPoint(x: originX, y: rowRectInSuperview.origin.y)
            
            return menuPoint
        }
        
        open override func createMessageMenuItems(for message: JMessage) -> [SBUMenuItem] {
            var items = super.createMessageMenuItems(for: message)
            
            if message.content is JTextMessage {
                let reply = self.createReplyMenuItem(for: message)
                items.append(reply)
            }
            
            return items
        }
        
        open override func showMessageContextMenu(for message: JMessage, cell: UITableViewCell, forRowAt indexPath: IndexPath) {
            let messageMenuItems = self.createMessageMenuItems(for: message)
            guard !messageMenuItems.isEmpty else { return }
            
            guard let cell = cell as? SBUBaseMessageCell else { return }
            let menuPoint = self.calculateMessageMenuCGPoint(indexPath: indexPath, position: cell.position)
            SBUMenuView.show(items: messageMenuItems, point: menuPoint) {
                cell.isSelected = false
            }
        }
        
        // MARK: - Actions
        
        /// Sets gestures in message cell.
        /// - Parameters:
        ///   - cell: The message cell
        ///   - message: message object
        ///   - indexPath: Cell's indexPath
        open func setMessageCellGestures(_ cell: SBUBaseMessageCell, message: JMessage, indexPath: IndexPath) {
            cell.tapHandlerToContent = { [weak self] in
                guard let self = self else { return }
                self.setTapGesture(cell, message: message, indexPath: indexPath)
            }
            
            cell.longPressHandlerToContent = { [weak self] in
                guard let self = self else { return }
                self.setLongTapGesture(cell, message: message, indexPath: indexPath)
            }
        }
        
        // MARK: - TableView: Cell
        open func register(cell: SBUBaseMessageCell, contentType: String) {
            self.cellDict[contentType] = type(of: cell)
            self.tableView.register(type(of: cell), forCellReuseIdentifier: contentType)
        }
        
        /// Configures cell with message for a particular row.
        /// - Parameters:
        ///    - messageCell: `SBUBaseMessageCell` object.
        ///    - message: The message for `messageCell`.
        ///    - indexPath: An index path representing the `messageCell`
        open func configureCell(_ messageCell: SBUBaseMessageCell, message: JMessage, forRowAt indexPath: IndexPath) {
            // NOTE: to disable unwanted animation while configuring cells
            UIView.setAnimationsEnabled(false)

            let isSameDay = self.checkSameDayAsNextMessage(
                currentIndex: indexPath.row,
                fullMessageList: fullMessageList
            )
            let receiptState = SBUUtils.getReceiptState(of: message)

            if let contactCardCell = messageCell as? ContactCardMessageCell {
                let configuration = SBUTextMessageCellParams(
                    message: message,
                    hideDateView: isSameDay,
                    useMessagePosition: true,
                    groupPosition: self.getMessageGroupingPosition(currentIndex: indexPath.row),
                    receiptState: receiptState,
                    useReaction: false,
                    withTextView: true,
                    joinedAt: 0,
                    messageOffsetTimestamp: 0,
                    shouldHideSuggestedReplies: false,
                    shouldHideFormTypeMessage: false,
                    enableEmojiLongPress: false
                )
                configuration.shouldHideFeedback = true
                contactCardCell.configure(with: configuration)
                self.setMessageCellGestures(contactCardCell, message: message, indexPath: indexPath)
            } else if let callMessageCell = messageCell as? SBUCallMessageCell {
                let configuration = SBUTextMessageCellParams(
                    message: message,
                    hideDateView: isSameDay,
                    useMessagePosition: true,
                    groupPosition: self.getMessageGroupingPosition(currentIndex: indexPath.row),
                    receiptState: receiptState,
                    useReaction: false,
                    withTextView: true,
                    joinedAt: 0,
                    messageOffsetTimestamp: 0,
                    shouldHideSuggestedReplies: false,
                    shouldHideFormTypeMessage: false,
                    enableEmojiLongPress: false
                )
                configuration.shouldHideFeedback = true
                callMessageCell.configure(with: configuration)
            } else if let textMessageCell = messageCell as? SBUTextMessageCell {
                let shouldHideSuggestedReplies = false

                let configuration = SBUTextMessageCellParams(
                    message: message,
                    hideDateView: isSameDay,
                    useMessagePosition: true,
                    groupPosition: self.getMessageGroupingPosition(currentIndex: indexPath.row),
                    receiptState: receiptState,
                    useReaction: false,
                    withTextView: true,
                    joinedAt: 0,
                    messageOffsetTimestamp: 0,
                    shouldHideSuggestedReplies: shouldHideSuggestedReplies,
                    shouldHideFormTypeMessage: false,
                    enableEmojiLongPress: false
                )
                configuration.shouldHideFeedback = true
                textMessageCell.configure(with: configuration)
                textMessageCell.configure(highlightInfo: self.highlightInfo)

                self.setMessageCellAnimation(textMessageCell, message: message, indexPath: indexPath)
                self.setMessageCellGestures(textMessageCell, message: message, indexPath: indexPath)
            } else if let mediaMessageCell = messageCell as? SBUMediaMessageCell {
                let voiceFileInfo = self.voiceFileInfos[message.cacheKey] ?? nil
                let configuration = SBUMediaMessageCellParams(
                    message: message,
                    hideDateView: isSameDay,
                    useMessagePosition: true,
                    groupPosition: self.getMessageGroupingPosition(currentIndex: indexPath.row),
                    receiptState: receiptState,
                    useReaction: false,
                    joinedAt: 0,
                    messageOffsetTimestamp: 0,
                    voiceFileInfo: voiceFileInfo,
                    enableEmojiLongPress: false
                )
                configuration.shouldHideFeedback = true
                
                if voiceFileInfo != nil {
                    self.currentVoiceFileInfo = nil
                    self.currentVoiceContentView = nil
                }
                
                mediaMessageCell.configure(with: configuration)
                mediaMessageCell.configure(highlightInfo: self.highlightInfo)
                self.setMessageCellAnimation(mediaMessageCell, message: message, indexPath: indexPath)
                self.setMessageCellGestures(mediaMessageCell, message: message, indexPath: indexPath)
                self.setMediaMessageCellImage(mediaMessageCell, mediaMessage: message)
                
                if let voiceFileInfo = voiceFileInfo,
                   voiceFileInfo.isPlaying == true,
                   let voiceContentView = mediaMessageCell.baseFileContentView as? SBUVoiceContentView {
                    
                    self.currentVoiceContentIndexPath = indexPath
                    self.currentVoiceFileInfo = voiceFileInfo
                    self.currentVoiceContentView = voiceContentView
                }
            } else {
                let configuration = SBUBaseMessageCellParams(message: message, hideDateView: isSameDay, receiptState: receiptState)
                messageCell.configure(with: configuration)
            }

            UIView.setAnimationsEnabled(true)

            // TODO: Move to `setMessageCellGestures`?
            messageCell.userProfileTapHandler = { [weak messageCell, weak self] in
                guard let self = self else { return }
                guard let cell = messageCell else { return }
                guard let senderId = cell.message?.senderUserId else { return }
                guard let sender = JIM.shared().userInfoManager.getUserInfo(senderId) else { return }
                self.setUserProfileTapGesture(SBUUser(user: sender))
            }

            // Reaction action
            messageCell.emojiTapHandler = { [weak messageCell, weak self] emojiKey in
                guard let self = self else { return }
                guard let cell = messageCell else { return }
                self.delegate?.groupChannelModule(self, didTapEmoji: emojiKey, messageCell: cell)
            }

            messageCell.emojiLongPressHandler = { [weak messageCell, weak self] emojiKey in
                guard let self = self else { return }
                guard let cell = messageCell else { return }
                self.delegate?.groupChannelModule(self, didLongTapEmoji: emojiKey, messageCell: cell)
            }

            messageCell.moreEmojiTapHandler = { [weak messageCell, weak self] in
                guard let self = self else { return }
                guard let cell = messageCell else { return }
                self.delegate?.groupChannelModule(self, didTapMoreEmojiForCell: cell)
            }

            messageCell.mentionTapHandler = { [weak self] user in
                guard let self = self else { return }
                self.delegate?.groupChannelModule(self, didTapMentionUser: user)
            }

//            messageCell.submitFormHandler = { [weak self] form, cell in
//                guard let self = self else { return }
//                self.delegate?.groupChannelModule(self, didSubmit: form, messageCell: cell)
//            }
        }
        
        open override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
            guard indexPath.row < self.fullMessageList.count else {
                SBULog.error("The index is out of range.")
                return .init()
            }
            
            let message = fullMessageList[indexPath.row]
            
            let cell: UITableViewCell
            if self.cellDict[message.contentType] != nil {
                cell = tableView.dequeueReusableCell(withIdentifier: message.contentType) ?? UITableViewCell()
            } else {
                cell = tableView.dequeueReusableCell(withIdentifier: JMessageContent.contentType()) ?? UITableViewCell()
            }
            cell.contentView.transform = CGAffineTransform(scaleX: 1, y: -1)
            cell.selectionStyle = .none
            
            guard let messageCell = cell as? SBUBaseMessageCell else {
                SBULog.error("There are no message cells!")
                return cell
            }
            
            self.configureCell(messageCell, message: message, forRowAt: indexPath)
            
            return cell
        }
        
        open override func tableView(_ tableView: UITableView, didEndDisplaying cell: UITableViewCell, forRowAt indexPath: IndexPath) {
            guard let mediaMessageCell = cell as? SBUMediaMessageCell,
                  mediaMessageCell.baseFileContentView is SBUVoiceContentView else { return }
        }
        
        /// Sets animation in message cell.
        /// - Parameters:
        ///   - cell: The message cell
        ///   - message: message object
        ///   - indexPath: Cell's indexPath
        open func setMessageCellAnimation(_ messageCell: SBUBaseMessageCell, message: JMessage, indexPath: IndexPath) {
            if message.clientMsgNo == highlightInfo?.messageId,
               message.timestamp == highlightInfo?.updatedAt,
               self.highlightInfo?.animated == true,
               self.isHighlightInfoAnimated == false {
                self.cellAnimationDebouncer.add {
                    messageCell.messageContentView.animate(.shakeUpDown)
                    self.isHighlightInfoAnimated = true
                }
            }
        }
        
        /// Checks if a typing bubble is already displayed on screen.
        /// - returns: `true` if a SBUTypingIndicatorMessageCell was not previoulsy being displayed on screen, `false` if a SBUTypingIndicatorMessageCell was already being displayed.
        /// - Since: 3.12.0
        func decideToRedrawTypingBubble() -> Bool {
            return false
//            for cell in tableView.visibleCells where cell is SBUTypingIndicatorMessageCell {
//                return false
//            }
//            return true
        }
        
        // MARK: - Menu
        
    }
}

// MARK: - Voice message
extension SBUGroupChannelModule.List {
    func pauseVoicePlayer() {
        self.currentVoiceFileInfo?.isPlaying = false
        self.voicePlayer?.pause()
    }

    func pauseVoicePlayer(cacheKey: String) {
        if let voiceFileInfo = self.voiceFileInfos[cacheKey],
           voiceFileInfo.isPlaying == true {
            voiceFileInfo.isPlaying = false
            self.voicePlayer?.pause()
        }
    }
    
    func pauseAllVoicePlayer() {
        self.currentVoiceFileInfo?.isPlaying = false
        self.voicePlayer?.pause()
        
        for (_, value) in self.voiceFileInfos {
            value.isPlaying = false
        }
    }
    
    /// Updates voice message
    ///
    /// - Note: As a default, it's called from `baseChannelModule(_:didTapVoiceMessage:cell:forRowAt:)` delegate method.
    ///
    /// - Parameters:
    ///   - cell: The message cell
    ///   - message: message object
    ///   - indexPath: Cell's indexPath
    ///
    /// - Since: 3.4.0
    func updateVoiceMessage(_ cell: SBUBaseMessageCell, message: JMessage, indexPath: IndexPath) {
        guard let mediaMessageCell = cell as? SBUMediaMessageCell,
              let voiceContentView = mediaMessageCell.baseFileContentView as? SBUVoiceContentView else { return }

        if self.voiceFileInfos[message.cacheKey] == nil {
            voiceContentView.updateVoiceContentStatus(.loading)
        }
        
        guard let voiceMessage = message.content as? JVoiceMessage, let voiceUrl = voiceMessage.url else {
            return
        }

        SBUCacheManager.File.loadFile(
            urlString: voiceUrl,
            cacheKey: message.cacheKey,
            fileName: ""
        ) { [weak self] filePath, _ in

            let playtime: Double = Double(voiceMessage.duration)

            guard let filePath = filePath else {
                self?.pauseAllVoicePlayer()
                voiceContentView.updateVoiceContentStatus(.none, time: playtime)
                return
            }
            if voiceContentView.status == .loading || voiceContentView.status == .none {
                voiceContentView.updateVoiceContentStatus(.prepared)
            }

            var voicefileInfo: SBUVoiceFileInfo?
            if self?.voiceFileInfos[message.cacheKey] == nil {
                voicefileInfo = SBUVoiceFileInfo(
                    fileName: "",
                    filePath: filePath,
                    playtime: playtime,
                    currentPlayTime: 0
                )

                self?.voiceFileInfos[message.cacheKey] = voicefileInfo
            } else {
                voicefileInfo = self?.voiceFileInfos[message.cacheKey]
            }

            var actionInSameView = false
            if let voicefileInfo = voicefileInfo {
                if self?.currentVoiceFileInfo?.isPlaying == true {
                    // updated status of previously contentView
                    let currentPlayTime = self?.currentVoiceFileInfo?.currentPlayTime ?? 0
                    self?.currentVoiceFileInfo?.isPlaying = false
                    self?.currentVoiceContentView?.updateVoiceContentStatus(.pause, time: currentPlayTime)

                    if self?.currentVoiceContentView == voiceContentView {
                        actionInSameView = true
                    }
                }

                self?.voicePlayer?.configure(voiceFileInfo: voicefileInfo)
            }

            if let voicefileInfo = voicefileInfo {
                self?.voicePlayer?.configure(voiceFileInfo: voicefileInfo)
                self?.currentVoiceContentIndexPath = indexPath
            }

            if self?.currentVoiceFileInfo != voicefileInfo {
                self?.pauseAllVoicePlayer()
            }

            self?.currentVoiceFileInfo = voicefileInfo
            self?.currentVoiceContentView = voiceContentView

            switch voiceContentView.status {
            case .none:
                break
            case .loading:
                break
            case .prepared:
                self?.voicePlayer?.play()
            case .playing:
                self?.voicePlayer?.pause()
            case .pause:
                if actionInSameView == true { break }

                let currentPlayTime = self?.currentVoiceFileInfo?.currentPlayTime ?? 0
                self?.voicePlayer?.play(fromTime: currentPlayTime)
            case .finishPlaying:
                self?.voicePlayer?.play()
            }
        }
    }
    
    // MARK: - SBUVoicePlayerDelegate
    /// This method is called when the voice player encounters an error.
    /// - Parameters:
    ///   - player: The `SBUVoicePlayer` that encountered the error.
    ///   - errorStatus: The error status of the `SBUVoicePlayer`.
    public func voicePlayerDidReceiveError(_ player: SBUVoicePlayer, errorStatus: SBUVoicePlayerErrorStatus) {}
    
    /// This method is called when the voice player starts.
    /// - Parameter player: The `SBUVoicePlayer` that started.
    public func voicePlayerDidStart(_ player: SBUVoicePlayer) {
        let currentPlayTime = self.currentVoiceFileInfo?.currentPlayTime ?? 0
        self.currentVoiceFileInfo?.isPlaying = true
        
        if let indexPath = self.currentVoiceContentIndexPath,
           let cell = self.tableView.cellForRow(at: indexPath) as? SBUMediaMessageCell,
           let voiceContentView = cell.baseFileContentView as? SBUVoiceContentView {
            voiceContentView.updateVoiceContentStatus(.playing, time: currentPlayTime)
        }
    }
    
    /// This method is called when the voice player is paused.
    /// - Parameters:
    ///   - player: The `SBUVoicePlayer` that is paused.
    ///   - voiceFileInfo: The `SBUVoiceFileInfo` of the voice file that is paused.
    public func voicePlayerDidPause(_ player: SBUVoicePlayer, voiceFileInfo: SBUVoiceFileInfo?) {
        let currentPlayTime = self.currentVoiceFileInfo?.currentPlayTime ?? 0
        self.currentVoiceFileInfo?.isPlaying = false

        if let indexPath = self.currentVoiceContentIndexPath,
           let cell = self.tableView.cellForRow(at: indexPath) as? SBUMediaMessageCell,
           let voiceContentView = cell.baseFileContentView as? SBUVoiceContentView {
            voiceContentView.updateVoiceContentStatus(.pause, time: currentPlayTime)
        }
    }
    
    /// This method is called when the voice player stops.
    /// - Parameter player: The `SBUVoicePlayer` that stopped.
    public func voicePlayerDidStop(_ player: SBUVoicePlayer) {
        let time = self.currentVoiceFileInfo?.playtime ?? 0
        self.currentVoiceFileInfo?.isPlaying = false

        if let indexPath = self.currentVoiceContentIndexPath,
           let cell = self.tableView.cellForRow(at: indexPath) as? SBUMediaMessageCell,
           let voiceContentView = cell.baseFileContentView as? SBUVoiceContentView {
            voiceContentView.updateVoiceContentStatus(.finishPlaying, time: time)
        }
    }
    
    /// This method is called when the voice player is reset.
    /// - Parameter player: The `SBUVoicePlayer` that was reset.
    public func voicePlayerDidReset(_ player: SBUVoicePlayer) {}
    
    /// This method is called when the voice player updates play time.
    /// - Parameters:
    ///   - player: The `SBUVoicePlayer` that updated the play time.
    ///   - time: The updated play time.
    public func voicePlayerDidUpdatePlayTime(_ player: SBUVoicePlayer, time: TimeInterval) {
        self.currentVoiceFileInfo?.currentPlayTime = time
        self.currentVoiceFileInfo?.isPlaying = true
        
        if let indexPath = self.currentVoiceContentIndexPath,
           let cell = self.tableView.cellForRow(at: indexPath) as? SBUMediaMessageCell,
           let voiceContentView = cell.baseFileContentView as? SBUVoiceContentView {
            voiceContentView.updateVoiceContentStatus(.playing, time: time)
        }
    }
    
    /// Methods for quickly applying a text value to a stream message
    /// - Parameters:
    ///   - messageId: message id
    ///   - value: message text value
    /// - Since: 3.20.0
    public func updateStreamMessage(_ message: JMessage) {
        let cell = self.tableView.visibleCells
            .compactMap({ $0 as? SBUTextMessageCell })
            .first(where: { $0.message?.messageId == message.messageId })

        Thread.executeOnMain { [weak cell] in
            guard let cell = cell, let messageTextView = cell.messageTextView as? SBUTextMessageTextView else { return }
            messageTextView.configure(
                model: SBUTextMessageTextViewModel(
                    message: message,
                    position: cell.position,
                    isEdited: false
                )
            )
            cell.layoutIfNeeded()
            cell.invalidateIntrinsicContentSize()
        }
    }
}
