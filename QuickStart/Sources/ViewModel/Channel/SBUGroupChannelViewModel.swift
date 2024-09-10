//
//  SBUGroupChannelViewModel.swift
//  SendbirdUIKit
//
//  Created by Hoon Sung on 2021/02/15.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import Foundation
import JuggleIM
import simd

public protocol SBUGroupChannelViewModelDataSource: SBUBaseChannelViewModelDataSource {
    /// Asks to data source to return the array of index path that represents starting point of channel.
    /// - Parameters:
    ///    - viewModel: `SBUGroupChannelViewModel` object.
    ///    - channel: `GroupChannel` object from `viewModel`
    /// - Returns: The array of `IndexPath` object representing starting point.
    func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        startingPointIndexPathsForChannel channel: JConversationInfo?
    ) -> [IndexPath]?
}

public protocol SBUGroupChannelViewModelDelegate: SBUBaseChannelViewModelDelegate {
    /// Called when the channel has received mentional member list. Please refer to `loadSuggestedMentions(with:)` in `SBUGroupChannelViewModel`.
    /// - Parameters:
    ///   - viewModel: `SBUGroupChannelViewModel` object.
    ///   - members: Mentional members
    func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didReceiveSuggestedMentions members: [SBUUser]?
    )
    
    func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didFinishUploadingFileAt index: Int,
        multipleFilesMessageRequestId requestId: String
    )
    
    /// Called when a message verified as a stream message is updated.
    /// - Parameters:
    ///   - viewModel: SBUGroupChannelViewModel` object.
    ///   - message: stream message
    ///   - channel: `GroupChannel` object from `viewModel`
    /// - Since: 3.20.0
    func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didReceiveStreamMessage message: JMessage,
        forChannel channel: JConversationInfo
    )
}

open class SBUGroupChannelViewModel: SBUBaseChannelViewModel {
    // MARK: - Logic properties (Public)
    public weak var delegate: SBUGroupChannelViewModelDelegate? {
        get { self.baseDelegate as? SBUGroupChannelViewModelDelegate }
        set { self.baseDelegate = newValue }
    }
    
    public weak var dataSource: SBUGroupChannelViewModelDataSource? {
        get { self.baseDataSource as? SBUGroupChannelViewModelDataSource }
        set { self.baseDataSource = newValue }
    }


    // MARK: - Logic properties (private)
    var debouncer: SBUDebouncer?
    var suggestedMemberList: [SBUUser]?
    var hasPreviousMessage = true
    
    // MARK: - LifeCycle
    public init(conversationInfo: JConversationInfo? = nil,
                delegate: SBUGroupChannelViewModelDelegate? = nil,
                dataSource: SBUGroupChannelViewModelDataSource? = nil) {
        super.init()
    
        self.delegate = delegate
        self.dataSource = dataSource
        
        if let conversationInfo = conversationInfo {
            self.conversationInfo = conversationInfo
        }
        
        self.startingPoint = startingPoint
        
        self.debouncer = SBUDebouncer(
            debounceTime: SBUGlobals.userMentionConfig?.debounceTime ?? SBUDebouncer.defaultTime
        )
        JIM.shared().conversationManager.clearUnreadCount(by: conversationInfo?.conversation) {
        } error: { errorCode in
        }
        JIM.shared().messageManager.add(self)
        self.loadPrevMessages()
    }
    
    // MARK: - Message
    
    /// Updates a multiple files message cell of the given index of a multiple files message.
    /// - Parameters:
    ///    - requestId: the requestId of the multiple files message.
    ///    - index: the index of the cell of a multiple files message to update.
    /// - Since: 3.10.0
    open func updateMultipleFilesMessageCell(requestId: String, index: Int) {
        self.delegate?.groupChannelViewModel(
            self,
            didFinishUploadingFileAt: index,
            multipleFilesMessageRequestId: requestId
        )
    }
    
    // MARK: - Load Messages
    public override func loadInitialMessages(
        startingPoint: Int64?,
        showIndicator: Bool,
        initialMessages: [JMessage]?
    ) {
        SBULog.info("""
            loadInitialMessages,
            startingPoint : \(String(describing: startingPoint)),
            initialMessages : \(String(describing: initialMessages))
            """
        )
        
        // Caution in function call order
        self.reset()
        self.clearMessageList()
        
        if self.hasNext() {
            // Hold on to most recent messages in cache for smooth scrolling.
//            setupCache()
        }
        
        self.delegate?.shouldUpdateLoadingState(showIndicator)
        
//        self.messageCollection?.startCollection(
//            cacheResultHandler: { [weak self] cacheResult, error in
//                guard let self = self else { return }
//                
//                defer { self.displaysLocalCachedListFirst = false }
//                
//                if let error = error {
//                    self.delegate?.didReceiveError(error, isBlocker: false)
//                    return
//                }
//                
//                // prevent empty view showing
//                if cacheResult == nil || cacheResult?.isEmpty == true { return }
//                
//                self.isInitialLoading = true
//                
//                self.upsertMessagesInList(
//                    messages: cacheResult,
//                    needReload: self.displaysLocalCachedListFirst
//                )
//                
//            }, apiResultHandler: { [weak self] apiResult, error in
//                guard let self = self else { return }
//                
//                self.loadInitialPendingMessages()
//                
//                if let error = error {
//                    self.delegate?.shouldUpdateLoadingState(false)
//                    
//                    self.delegate?.didReceiveError(error, isBlocker: false)
//                    
//                    return
//                }
//        
//                if self.initPolicy == .cacheAndReplaceByApi {
//                    self.clearMessageList()
//                }
//                
//                self.isInitialLoading = false
//                self.upsertMessagesInList(messages: apiResult, needReload: true)
//            })
    }
    
    func loadInitialPendingMessages() {
    }
    
    public override func loadPrevMessages() {
        guard self.prevLock.try() else {
            SBULog.info("Prev message already loading")
            return
        }

        SBULog.info("[Request] Prev message list")
        
        let count = self.messageList.count
        let startTime = count > 0 ? self.messageList[count-1].timestamp : 0
        JIM.shared().messageManager.getLocalAndRemoteMessages(from: self.conversationInfo?.conversation, startTime: startTime, count: Int32(defaultFetchLimit), direction: .older) { localMessageList, needRemote in
            self.upsertMessagesInList(messages: localMessageList, needReload: true)
            if let count = localMessageList?.count {
                SBULog.info("[Request] Prev message list local count is \(count), needRemote is \(needRemote)")
                if count < self.defaultFetchLimit && !needRemote {
                    self.hasPreviousMessage = false
                }
            }
            if (!needRemote) {
                self.prevLock.unlock()
                SBULog.info("Prev message list local unlock")
            }
        } remoteMessageBlock: { remoteMessageList in
            SBULog.info("[Request] Prev message list remote count is \(remoteMessageList?.count ?? 0)")
            if let count = remoteMessageList?.count, count < self.defaultFetchLimit {
                self.hasPreviousMessage = false
            }
            self.upsertMessagesInList(messages: remoteMessageList, needReload: true)
            self.prevLock.unlock()
            SBULog.info("Prev message list remote unlock")
        } error: { errorCode in
            self.prevLock.unlock()
            SBULog.info("Prev message list error unlock")
        }
    }
    
    /// Loads next messages from `lastUpdatedTimestamp`.
    public override func loadNextMessages() {
//        guard self.nextLock.try() else {
//            SBULog.info("Next message already loading")
//            return
//        }
//
//        guard let messageCollection = self.messageCollection else { return }
//        self.isLoadingNext = true
//
//        messageCollection.loadNext { [weak self] messages, error in
//            guard let self = self else { return }
//            defer {
//                self.nextLock.unlock()
//                self.isLoadingNext = false
//            }
//
//            if let error = error {
//                self.delegate?.didReceiveError(error, isBlocker: false)
//                return
//            }
//            guard let messages = messages else { return }
//
//            SBULog.info("[Next message Response] \(messages.count) messages")
//
//            self.delegate?.baseChannelViewModel(
//                self,
//                shouldUpdateScrollInMessageList: messages,
//                forContext: nil,
//                keepsScroll: true
//            )
//            self.upsertMessagesInList(messages: messages, needReload: true)
//        }
    }

    // MARK: - Resend
    
    override public func deleteResendableMessage(_ message: JMessage, needReload: Bool) {
        super.deleteResendableMessage(message, needReload: needReload)
    }
    

    
    // MARK: - Typing
//    public func startTypingMessage() {
////        guard let channel = self.channel as? J else { return }
////
////        SBULog.info("[Request] Start typing")
////        channel.startTyping()
//    }
//
//    public func endTypingMessage() {
////        guard let channel = self.channel as? GroupChannel else { return }
////
////        SBULog.info("[Request] End typing")
////        channel.endTyping()
//    }
//
//    // MARK: - Typing Indicator Message
//    private func updateTypingIndicatorMessage() {
////        guard let channel = self.channel as? GroupChannel, let collection = self.messageCollection else { return }
////
////        // One or more user is typing.
////        if let typers = channel.getTypingUsers(),
////            typers.isEmpty == false,
////            let typingMessage = SBUTypingIndicatorMessage.make(["": ""]) {
////            // if hasNext is true, don't show typing bubble.
////            if collection.hasNext { return }
////
////            let truncatedTypers = Array(typers.prefix(3))
////            typingMessage.typingIndicatorInfo = SBUTypingIndicatorInfo(
////                typers: truncatedTypers,
////                numberOfTypers: typers.count
////            )
////
////            self.typingMessageManager.typingMessages[channel.channelURL] = typingMessage
////        }
////        // No user is typing.
////        else {
////            self.clearTypingMessage()
////        }
////
////        self.sortAllMessageList(needReload: false)  // tableview reload is handled in GroupChannelVC
//    }
//
//    func clearTypingMessage() {
//        guard let channel = self.channel as? GroupChannel else { return }
//        self.typingMessageManager.typingMessages.removeValue(forKey: channel.channelURL)
//    }
    
    // MARK: - Mention
    
    /// Loads mentionable member list.
    /// When the suggested list is received, it calls `groupChannelViewModel(_:didReceiveSuggestedMembers:)` delegate method.
    /// - Parameter filterText: The text that is used as filter while searching for the suggested mentions.
//    public func loadSuggestedMentions(with filterText: String) {
//        self.debouncer?.add { [weak self] in
//            guard let self = self else { return }
//
//            if let channel = self.channel as? GroupChannel {
//                if channel.isSuper {
//                    guard let config = SBUGlobals.userMentionConfig else {
//                        SBULog.error("`SBUGlobals.userMentionConfig` is `nil`")
//                        return
//                    }
//
//                    guard SendbirdUI.config.groupChannel.channel.isMentionEnabled else {
//                        SBULog.error("User mention features are disabled. See `SBUGlobals.isMentionEnabled` for more information")
//                        return
//                    }
//
//                    let params = MemberListQueryParams()
//                    params.nicknameStartsWithFilter = filterText
//
//                    // +1 is buffer for when the current user is included in the search results
//                    params.limit = UInt(config.suggestionLimit) + 1
//                    self.query = channel.createMemberListQuery(params: params)
//
//                    self.query?.loadNextPage { [weak self] members, _ in
//                        guard let self = self else { return }
//                        self.suggestedMemberList = SBUUser.convertUsers(members)
//                        self.delegate?.groupChannelViewModel(
//                            self,
//                            didReceiveSuggestedMentions: self.suggestedMemberList
//                        )
//                    }
//                } else {
//                    guard channel.members.count > 0 else {
//                        self.suggestedMemberList = nil
//                        self.delegate?.groupChannelViewModel(self, didReceiveSuggestedMentions: nil)
//                        return
//                    }
//
//                    let sortedMembers = channel.members.sorted { $0.nickname.lowercased() < $1.nickname.lowercased() }
//                    let matchedMembers = sortedMembers.filter {
//                        return $0.nickname.lowercased().hasPrefix(filterText.lowercased())
//                    }
//                    let memberCount = matchedMembers.count
//                    // +1 is buffer for when the current user is included in the search results
//                    let limit = (SBUGlobals.userMentionConfig?.suggestionLimit ?? 0) + 1
//                    let splitCount = min(memberCount, Int(limit))
//
//                    let resultMembers = Array(matchedMembers[0..<splitCount])
//                    self.suggestedMemberList = SBUUser.convertUsers(resultMembers)
//                    self.delegate?.groupChannelViewModel(
//                        self,
//                        didReceiveSuggestedMentions: self.suggestedMemberList
//                    )
//                }
//            }
//        }
//    }
//
//    /// Cancels loading the suggested mentions.
//    public func cancelLoadingSuggestedMentions() {
//        self.debouncer?.cancel()
//    }
    
    // MARK: - Common
    public override func hasPrevious() -> Bool {
        return hasPreviousMessage
    }
    
    override func reset() {
//        self.markAsRead()
        
        super.reset()
    }
}

extension SBUGroupChannelViewModel : JMessageDelegate {
    public func messageDidReceive(_ message: JMessage!) {
        if !message.conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
//        self.delegate?.baseChannelViewModel(
//            self,
//            shouldUpdateScrollInMessageList: [message],
//            keepsScroll: true
//        )
        self.upsertMessagesInList(messages: [message], needReload: true)
        
        JIM.shared().conversationManager.clearUnreadCount(by: conversationInfo?.conversation) {
        } error: { code in
        }
    }
    
    public func messageDidRecall(_ message: JMessage!) {
        
    }
    
    public func messageDidDelete(_ conversation: JConversation!, clientMsgNos: [NSNumber]!) {
        
    }
    
    public func messageDidClear(_ conversation: JConversation!, timestamp: Int64, senderId: String!) {
        
    }
}
