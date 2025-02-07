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
    var groupInfo: JCGroupInfo?
    
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
        self.markAsRead()
        JIM.shared().messageManager.add(self as JMessageDelegate)
        JIM.shared().messageManager.add(self as JMessageReadReceiptDelegate)
        self.loadPrevMessages()
        self.loadGroupInfo()
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
        
        let option = JGetMessageOptions()
        option.startTime = startTime
        option.count = Int32(defaultFetchLimit)
        JIM.shared().messageManager.getMessages(self.conversationInfo?.conversation, direction: .older, option: option) { messageList, timestamp, hasMore, errorCode in
            SBULog.info("[Request] Prev message list count is \(messageList?.count ?? 0), hasMore is \(hasMore), errorCode is \(errorCode)")
            self.upsertMessagesInList(messages: messageList, needReload: true)
            self.loadMessageReaction(messages: messageList)
            if let messageList = messageList {
                self.sendReceipt(messageList)
            }
            self.hasPreviousMessage = hasMore
            self.prevLock.unlock()
            SBULog.info("Prev message list unlock")
        }
        
//        JIM.shared().messageManager.getMessages(self.conversationInfo?.conversation, direction: .older, option: option) { localMessageList, errorCode in
//            SBULog.info("[Request] Prev message list local count is \(localMessageList?.count ?? 0)")
//            self.upsertMessagesInList(messages: localMessageList, needReload: true)
//            if let localMessageList = localMessageList {
//                self.sendReceipt(localMessageList)
//            }
//        } remoteMessageBlock: { remoteMessageList, timestamp, hasMore, errorCode in
//            SBULog.info("[Request] Prev message list remote count is \(remoteMessageList?.count ?? 0), hasMore is \(hasMore)")
//            self.upsertMessagesInList(messages: remoteMessageList, needReload: true)
//            if let remoteMessageList = remoteMessageList {
//                self.sendReceipt(remoteMessageList)
//            }
//            self.hasPreviousMessage = hasMore
//            self.prevLock.unlock()
//            SBULog.info("Prev message list remote unlock")
//        }

//        JIM.shared().messageManager.getLocalAndRemoteMessages(from: self.conversationInfo?.conversation, startTime: startTime, count: Int32(defaultFetchLimit), direction: .older) { localMessageList, needRemote in
//            self.upsertMessagesInList(messages: localMessageList, needReload: true)
//            if let count = localMessageList?.count {
//                SBULog.info("[Request] Prev message list local count is \(count), needRemote is \(needRemote)")
//                if count < self.defaultFetchLimit && !needRemote {
//                    self.hasPreviousMessage = false
//                }
//            }
//            if (!needRemote) {
//                self.prevLock.unlock()
//                SBULog.info("Prev message list local unlock")
//            }
//        } remoteMessageBlock: { remoteMessageList in
//            SBULog.info("[Request] Prev message list remote count is \(remoteMessageList?.count ?? 0)")
//            if let count = remoteMessageList?.count, count < self.defaultFetchLimit {
//                self.hasPreviousMessage = false
//            }
//            self.upsertMessagesInList(messages: remoteMessageList, needReload: true)
//            self.prevLock.unlock()
//            SBULog.info("Prev message list remote unlock")
//        } error: { errorCode in
//            self.prevLock.unlock()
//            SBULog.info("Prev message list error unlock")
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
    
    // MARK: - Message related
    public func markAsRead() {
        JIM.shared().conversationManager.clearUnreadCount(by: self.conversationInfo?.conversation) {
        } error: { code in
            if code != .none {
                self.delegate?.didReceiveError(code)
            }
        }
    }
    
    func sendReceipt(_ messages: [JMessage]) {
        if self.conversationInfo?.conversation.conversationType != .private {
            return
        }
        var messageIds: [String] = []
        //只有单聊发，只有收到的消息发，只有没有 hasRead 发
        for message in messages {
            if message.direction == .receive && !message.hasRead {
                messageIds.append(message.messageId)
            }
        }
        if messageIds.count == 0 {
            return
        }
        JIM.shared().messageManager.sendReadReceipt(messageIds, in: self.conversationInfo?.conversation) {
        } error: { code in
            if code != .none {
                self.delegate?.didReceiveError(code)
            }
        }
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
    public func loadSuggestedMentions(with filterText: String) {
        self.debouncer?.add { [weak self] in
            guard let self = self else { return }
            guard let groupInfo = self.groupInfo, groupInfo.members.count > 0 else {
                self.suggestedMemberList = nil
                self.delegate?.groupChannelViewModel(self, didReceiveSuggestedMentions: nil)
                return
            }

            let sortedMembers = groupInfo.members.sorted { $0.userName?.lowercased() ?? "" < $1.userName?.lowercased() ?? "" }
            let matchedMembers = sortedMembers.filter {
                return $0.userName?.lowercased().hasPrefix(filterText.lowercased()) ?? false
            }
            let memberCount = matchedMembers.count
            // +1 is buffer for when the current user is included in the search results
            let limit = 16//(SBUGlobals.userMentionConfig?.suggestionLimit ?? 0) + 1
            let splitCount = min(memberCount, Int(limit))

            let resultMembers = Array(matchedMembers[0..<splitCount])
            self.suggestedMemberList = SBUUser.convertUsers(resultMembers)
            self.delegate?.groupChannelViewModel(
                self,
                didReceiveSuggestedMentions: self.suggestedMemberList
            )
        }
    }

    /// Cancels loading the suggested mentions.
    public func cancelLoadingSuggestedMentions() {
        self.debouncer?.cancel()
    }
    
    // MARK: - Common
    public override func hasPrevious() -> Bool {
        return hasPreviousMessage
    }
    
    override func reset() {
        self.markAsRead()
        
        super.reset()
    }
    
    // MARK: - Private
    private func loadGroupInfo() {
        guard let conversation =  self.conversationInfo?.conversation,
              conversation.conversationType == .group else {
            return
        }
        HttpManager.shared.getGroupInfo(groupId: conversation.conversationId) { code , groupInfo in
            if code == 0 {
                self.groupInfo = groupInfo
            }
        }
    }
}

extension SBUGroupChannelViewModel : JMessageDelegate {
    public func messageDidReceive(_ message: JMessage!) {
        SBULog.info("bot, messageDidReceive")
        if !message.conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        
        if message.contentType == JTextMessage.contentType() {
            var streamId = ""
            if let textMessage = message.content as? JTextMessage,
               !textMessage.extra.isEmpty,
               let jsonData = textMessage.extra.data(using: .utf8)
            {
                if let jsonDic = try? JSONSerialization.jsonObject(with: jsonData) as? [String: String],
                   let stream = jsonDic["stream_msg_id"] {
                    streamId = stream
                }
            }
            if !streamId.isEmpty, let index = SBUUtils.findIndex(ofStreamMessage: streamId, in: self.messageList) {
                self.messageList.remove(at: index)
            }
        }
        
        if let streamText = message.content as? StreamTextMessage,
           let streamId = streamText.streamId {
            var streamContent = ""
            if !streamId.isEmpty, let index = SBUUtils.findIndex(ofStreamMessage: streamId, in: self.messageList) {
                let foundStreamTextMessage = self.messageList[index]
                if let foundStreamText = foundStreamTextMessage.content as? StreamTextMessage,
                   let content = foundStreamText.content {
                    streamContent = content
                }
                self.messageList.remove(at: index)
            }
            streamText.content = streamContent.appending(streamText.content ?? "")
        }
        
        self.upsertMessagesInList(messages: [message], needReload: true)
        self.sendReceipt([message])
        self.markAsRead()
    }
    
    public func messageDidRecall(_ message: JMessage!) {
        if !message.conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        self.updateMessagesInList(messages: [message], needReload: true)
    }
    
    public func messageDidUpdate(_ message: JMessage!) {
        if !message.conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        self.updateMessagesInList(messages: [message], needReload: true)
    }
    
    public func messageDidDelete(_ conversation: JConversation!, clientMsgNos: [NSNumber]!) {
        if !conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        if clientMsgNos.count > 0 {
            var int64List: [Int64] = []
            for clientMsgNo in clientMsgNos {
                int64List.append(clientMsgNo.int64Value)
            }
            self.deleteMessagesInList(clientMsgNos: int64List, needReload: true)
        }
    }
    
    public func messageDidClear(_ conversation: JConversation!, timestamp: Int64, senderId: String!) {
        if !conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        self.reset()
        self.clearMessageList()
        self.sortAllMessageList(needReload: true)
    }
    
    public func messageReactionDidAdd(_ reaction: JMessageReaction!, in conversation: JConversation!) {
        if !conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        if self.messageList.contains(where: { $0.messageId == reaction.messageId}) {
            guard let messages = JIM.shared().messageManager.getMessagesByMessageIds([reaction.messageId]),
                  messages.count > 0,
                  let message = messages.first else {
                return
            }
            if let index = SBUUtils.findIndex(ofReaction: reaction, in: self.reactionList) {
                self.reactionList[index].itemList = mergeReaction(oldItemList: self.reactionList[index].itemList, newItemList: reaction.itemList)
            } else {
                self.reactionList.append(reaction)
            }
            self.updateMessagesInList(messages: [message], needReload: true)
        }
    }
    

    public func messageReactionDidRemove(_ reaction: JMessageReaction!, in conversation: JConversation!) {
        if !conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        if self.messageList.contains(where: { $0.messageId == reaction.messageId}) {
            guard let messages = JIM.shared().messageManager.getMessagesByMessageIds([reaction.messageId]),
                  messages.count > 0,
                  let message = messages.first else {
                return
            }
            if let index = SBUUtils.findIndex(ofReaction: reaction, in: self.reactionList) {
                self.reactionList[index].itemList = removeReaction(oldItemList: self.reactionList[index].itemList, newItemList: reaction.itemList)
            }
            
            self.updateMessagesInList(messages: [message], needReload: true)
        }
    }
    
    private func mergeReaction(oldItemList: [JMessageReactionItem], newItemList: [JMessageReactionItem]) -> [JMessageReactionItem] {
        var result: [JMessageReactionItem] = []
        var needMerge = false
        result.append(contentsOf: oldItemList)
        for newItem in newItemList {
            needMerge = false
            for oldItem in result {
                if oldItem.reactionId == newItem.reactionId {
                    needMerge = true
                    var userInfoList = oldItem.userInfoList
                    for newUserInfo in newItem.userInfoList {
                        if userInfoList.contains(where: { $0.userId == newUserInfo.userId}) {
                            continue
                        }
                        userInfoList.append(newUserInfo)
                    }
                    oldItem.userInfoList = userInfoList
                    break
                }
            }
            if !needMerge {
                result.append(newItem)
            }
        }
        return result
    }
    
    private func removeReaction(oldItemList: [JMessageReactionItem], newItemList: [JMessageReactionItem]) -> [JMessageReactionItem] {
        var result: [JMessageReactionItem] = []
        result.append(contentsOf: oldItemList)
        for newItem in newItemList {
            for oldItem in result {
                if oldItem.reactionId == newItem.reactionId {
                    var userInfoList = oldItem.userInfoList
                    for newUserInfo in newItem.userInfoList {
                        if let index = SBUUtils.findIndex(ofUser: newUserInfo, in: userInfoList) {
                            userInfoList.remove(at: index)
                        }
                    }
                    oldItem.userInfoList = userInfoList
                    break
                }
            }
        }
        result = result.filter { !$0.userInfoList.isEmpty }
        return result
    }
}

extension SBUGroupChannelViewModel: JMessageReadReceiptDelegate {
    public func messagesDidRead(_ messageIds: [String]!, in conversation: JConversation!) {
        if !conversation.isEqual(self.conversationInfo?.conversation) {
            return
        }
        let messages = JIM.shared().messageManager.getMessagesByMessageIds(messageIds)
        if let messages = messages, messages.count > 0 {
            self.upsertMessagesInList(messages: messages, needReload: true)
        }
    }
    
    public func groupMessagesDidRead(_ msgs: [String : JGroupMessageReadInfo]!, in conversation: JConversation!) {
        
    }
}
