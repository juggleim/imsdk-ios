//
//  SBUGroupChannelViewModel.swift
//  SendbirdUIKit
//
//  Created by Hoon Sung on 2021/02/15.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import Foundation
import JetIM
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
        
        JIM.shared().messageManager.getLocalAndRemoteMessages(from: conversationInfo?.conversation, startTime: 0, count: 20, direction: .older) { localMessages, needRemote in
            self.upsertMessagesInList(messages: localMessages, needReload: true)
        } remoteMessageBlock: { remoteMessages in
            self.upsertMessagesInList(messages: remoteMessages, needReload: true)
        } error: { errorCode in
            
        }

    }
    
    // MARK: - Channel related
    public override func loadChannel(channelURL: String,
                                     completionHandler: ((JConversationInfo?, JErrorCode?) -> Void)? = nil) {
        
        // TODO: loading
//        self.delegate?.shouldUpdateLoadingState(true)
        
//        SendbirdUI.connectIfNeeded { [weak self] _, error in
//            if let error = error {
//                self?.delegate?.didReceiveError(error, isBlocker: true)
//                completionHandler?(nil, error)
//                return
//            }
//
//            SBULog.info("[Request] Load channel: \(String(channelURL))")
//            GroupChannel.getChannel(url: channelURL) { [weak self] channel, error in
//                guard let self = self else {
//                    completionHandler?(nil, error)
//                    return
//                }
//
//                guard self.canProceed(with: channel, error: error) else {
//                    completionHandler?(nil, error)
//                    return
//                }
//
//                self.channel = channel
//                self.channelURL = channel?.channelURL
//                SBULog.info("[Succeed] Load channel request: \(String(describing: self.channel))")
//
//                // background refresh to check if user is banned or not.
//                self.refreshChannel()
//
//                // for updating channel information when the connection state is closed at the time of initial load.
//                if SendbirdChat.getConnectState() == .closed {
//                    let context = MessageContext(source: .eventChannelChanged, sendingStatus: .succeeded)
//                    self.delegate?.baseChannelViewModel(
//                        self,
//                        didChangeChannel: channel,
//                        withContext: context
//                    )
//                    completionHandler?(channel, nil)
//                }
//
//                let cachedMessages = self.flushCache(with: [])
//                self.loadInitialMessages(
//                    startingPoint: self.startingPoint,
//                    showIndicator: true,
//                    initialMessages: cachedMessages
//                )
//            }
//        }
    }
    
    // MARK: - Message
    /// Sends a multiple files message.
    /// - Parameters:
    ///    - fileInfoList: A list of `UploadableFileInfo` that contains information about the files to be included in the multiple files message.
    /// - Since: 3.10.0
//    open func sendMultipleFilesMessage(fileInfoList: [UploadableFileInfo]) {
//        if let channel = self.channel as? GroupChannel {
//            let param = MultipleFilesMessageCreateParams(uploadableFileInfoList: fileInfoList)
//            SBUGlobalCustomParams.multipleFilesMessageParamsSendBuilder?(param)
//
//            let preSendMessage: MultipleFilesMessage?
//            preSendMessage = channel.sendMultipleFilesMessage(
//                params: param,
//                fileUploadHandler: { requestId, index, _, error in
//                    if let error = error {
//                        SBULog.error("Multiple files message - failed to upload file at index [\(index)]. \(error.localizedDescription)")
//                    } else {
//                        SBULog.info("Multiple files message - file at index [\(index)] upload completed.")
//                    }
//
//                    // Update the multipleFilesMessage collection view cell
//                    // when the upload is complete.
//                    self.updateMultipleFilesMessageCell(requestId: requestId, index: index)
//                },
//                completionHandler: { [weak self] multipleFilesMessage, error in
//                    if let error = error {
//                        SBULog.error(error.localizedDescription)
//                    }
//                    self?.sendMultipleFilesMessageCompletionHandler?(multipleFilesMessage, error)
//                })
//
//            // Save each file data to cache.
//            if let preSendMessage = preSendMessage {
//                for (index, fileInfo) in param.uploadableFileInfoList.enumerated() {
//                    SBUCacheManager.Image.preSave(
//                        multipleFilesMessage: preSendMessage,
//                        uploadableFileInfo: fileInfo,
//                        index: index
//                    )
//                }
//            }
//
//            // Upsert pending message to fullMessageList.
//            if let preSendMessage = preSendMessage, self.messageListParams.belongsTo(preSendMessage) {
//              // Upsert pendingMessage.
//              self.pendingMessageManager.upsertPendingMessage(
//                  channelURL: channel.channelURL,
//                  message: preSendMessage
//              )
//            } else {
//                SBULog.info("A filtered file message has been sent.")
//            }
//
//            self.sortAllMessageList(needReload: true)
//            let context = MessageContext(source: .eventMessageSent, sendingStatus: .succeeded)
//            self.baseDelegate?.baseChannelViewModel(
//                self,
//                shouldUpdateScrollInMessageList: self.fullMessageList,
//                forContext: context,
//                keepsScroll: false
//            )
//        }
//    }
    
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
        self.createCollectionIfNeeded(startingPoint: startingPoint ?? .max)
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
//        guard let messageCollection = self.messageCollection else { return }
//        guard self.prevLock.try() else {
//            SBULog.info("Prev message already loading")
//            return
//        }
//
//        SBULog.info("[Request] Prev message list")
//
//        messageCollection.loadPrevious { [weak self] messages, error in
//            guard let self = self else { return }
//            defer {
//                self.prevLock.unlock()
//            }
//
//            if let error = error {
//                self.delegate?.didReceiveError(error, isBlocker: false)
//                return
//            }
//
//            guard let messages = messages, !messages.isEmpty else { return }
//            SBULog.info("[Prev message response] \(messages.count) messages")
//
//            self.delegate?.baseChannelViewModel(
//                self,
//                shouldUpdateScrollInMessageList: messages,
//                forContext: nil,
//                keepsScroll: false
//            )
//            self.upsertMessagesInList(messages: messages, needReload: true)
//        }
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
    private func createCollectionIfNeeded(startingPoint: Int64) {
        // GroupChannel only
        guard let conversationInfo = self.conversationInfo else { return }
    }
    
    override func reset() {
//        self.markAsRead()
        
        super.reset()
    }
}
