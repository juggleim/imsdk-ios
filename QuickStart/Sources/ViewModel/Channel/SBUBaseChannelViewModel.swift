//
//  SBUBaseChannelViewModel.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2021/07/22.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import AVKit
import JetIM


/// Methods to get data source for the `SBUBaseChannelViewModel`.
public protocol SBUBaseChannelViewModelDataSource: AnyObject {
    /// Asks to data source whether the channel is scrolled to bottom.
    /// - Parameters:
    ///    - viewModel: `SBUBaseChannelViewModel` object.
    ///    - channel: `JConversationInfo` object.
    /// - Returns:
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        isScrollNearBottomInChannel channel: JConversationInfo?
    ) -> Bool
}

/// Methods for notifying the data updates from the `SBUBaseChannelViewModel`.
public protocol SBUBaseChannelViewModelDelegate: SBUCommonViewModelDelegate {
    /// Called when the the channel has been changed.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        didChangeChannel channel: JConversationInfo?
    )
    
    /// Called when the channel has received a new message.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        didReceiveNewMessage message: JMessage,
        forConversation channel: JConversationInfo
    )
    
    /// Called when the channel should finish editing mode
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        shouldFinishEditModeForChannel channel: JConversationInfo
    )
    
    /// Called when the channel should be dismissed.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        shouldDismissForChannel channel: JConversationInfo?
    )
    
    /// Called when the messages has been changed. If they're the first loaded messages, `initialLoad` is `true`.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        didChangeMessageList messages: [JMessage],
        needsToReload: Bool,
        initialLoad: Bool
    )
    
    /// Called when the messages has been deleted.
    /// - Since: 3.4.0
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        deletedMessages messages: [JMessage]
    )
    
    /// Called when it should be updated scroll status for messages.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        shouldUpdateScrollInMessageList messages: [JMessage],
        keepsScroll: Bool
    )
    
    /// Called when it has updated the reaction event for a message.
    func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        forMessage message: JMessage
    )
}

open class SBUBaseChannelViewModel: NSObject {
    // MARK: - Constant
    let defaultFetchLimit: Int = 30
    
    // MARK: - Logic properties (Public)
    /// The current channel object. It's `JConversationInfo` type.
    public internal(set) var conversationInfo: JConversationInfo?
    /// The starting point of the message list in the `channel`.
    public internal(set) var startingPoint: Int64?
    
    /// This object has a list of all success messages synchronized with the server.
    @SBUAtomic public internal(set) var messageList: [JMessage] = []
    /// This object has a list of all messages.
    @SBUAtomic public internal(set) var fullMessageList: [JMessage] = []
    
    /// This object is used to check if current user is an operator.
    public var isOperator: Bool {
//        if let groupChannel = self.channel as? GroupChannel {
//            return groupChannel.myRole == .operator
//        } else if let openChannel = self.channel as? OpenChannel {
//            guard let userId = SBUGlobals.currentUser?.userId else { return false }
//            return openChannel.isOperator(userId: userId)
//        }
        return false
    }
    
    // MARK: - Logic properties (Private)
    weak var baseDataSource: SBUBaseChannelViewModelDataSource?
    weak var baseDelegate: SBUBaseChannelViewModelDelegate?
    
    let prevLock = NSLock()
    let nextLock = NSLock()
    let initialLock = NSLock()
    
    var isInitialLoading = false
    var isScrollToInitialPositionFinish = false
    
    @SBUAtomic var isLoadingNext = false
    @SBUAtomic var isLoadingPrev = false
    
    var isTransformedList: Bool = true
    var isThreadMessageMode: Bool = false
    
    // MARK: - LifeCycle
    public override init() {
        super.init()
        
//        SendbirdChat.addConnectionDelegate(
//            self,
//            identifier: "\(SBUConstant.connectionDelegateIdentifier).\(self.description)"
//        )
    }
    
    func reset() {
//        self.messageCache = nil
        self.resetMessageListParams()
        self.isScrollToInitialPositionFinish = false
    }
    
    deinit {
        self.baseDelegate = nil
        self.baseDataSource = nil
        
//        SendbirdChat.removeConnectionDelegate(
//            forIdentifier: "\(SBUConstant.connectionDelegateIdentifier).\(self.description)"
//        )
    }
    
    // MARK: - Channel related
    
    /// This function loads channel information and message list.
    /// - Parameters:
    ///   - channelURL: channel url
    ///   - messageListParams: (Optional) The parameter to be used when getting channel information.
    public func loadChannel(channelURL: String, completionHandler: ((JConversationInfo?, JErrorCode?) -> Void)? = nil) {}
    
    /// This function refreshes channel.
    public func refreshChannel() {}
    
    // MARK: - Load Messages
    
    /// Loads initial messages in channel.
    /// `NOT` using `initialMessages` here since `MessageCollection` handles messages from db.
    /// Only used in `SBUOpenChannelViewModel` where `MessageCollection` is not suppoorted.
    ///
    /// - Parameters:
    ///   - startingPoint: Starting point to load messages from, or `nil` to load from the latest. (`Int64.max`)
    ///   - showIndicator: Whether to show indicator on load or not.
    ///   - initialMessages: Custom messages to start the messages from.
    public func loadInitialMessages(
        startingPoint: Int64?,
        showIndicator: Bool,
        initialMessages: [JMessage]?
    ) {}
    
    /// Loads previous messages.
    public func loadPrevMessages() {}
    
    /// Loads next messages from `lastUpdatedTimestamp`.
    public func loadNextMessages() {}
    
    /// This function resets list and reloads message lists.
    public func reloadMessageList() {
        self.loadInitialMessages(
            startingPoint: nil,
            showIndicator: false,
            initialMessages: []
        )
    }
    
    // MARK: - Message
    
    /// Sends a user message with text and parentMessageId.
    /// - Parameters:
    ///    - text: String value
    ///    - parentMessage: The parent message. The default value is `nil` when there's no parent message.
    open func sendTextMessage(text: String, parentMessage: JMessage? = nil) {
        let text = text.trimmingCharacters(in: .whitespacesAndNewlines)
        let textMessage = JTextMessage(content: text)
        let message = JIM.shared().messageManager.sendMessage(textMessage, in: conversationInfo?.conversation) { sendMessage in
            
        } error: { code , errorMessage in
            
        }
        if let message = message {
            self.upsertMessagesInList(messages: [message], needReload: true)
        }
    }
    
    /// Sends a user message with mentionedMessageTemplate and mentionedUserIds.
    /// - Parameters:
    ///    - mentionedMessageTemplate: Mentioned message string value that is generated by `text` and `mentionedUsers`.
    ///    - mentionedUserIds: Mentioned user Id array
    ///    - parentMessage: The parent message. The default value is `nil` when there's no parent message.
    /// ```swift
    /// print(text) // "Hi @Nickname"
    /// print(mentionedMessageTemplate) // "Hi @{UserID}"
    /// print(mentionedUserIds) // ["{UserID}"]
    /// ```
    open func sendTextMessage(text: String, mentionedMessageTemplate: String, mentionedUserIds: [String], parentMessage: JMessage? = nil) {
        //        let messageParams = UserMessageCreateParams(message: text)
        //
        //        SBUGlobalCustomParams.userMessageParamsSendBuilder?(messageParams)
        //
        //        if let parentMessage = parentMessage,
        //           SendbirdUI.config.groupChannel.channel.replyType != .none {
        //            messageParams.parentMessageId = parentMessage.messageId
        //            messageParams.isReplyToChannel = true
        //        }
        //        messageParams.mentionedMessageTemplate = mentionedMessageTemplate
        //        messageParams.mentionedUserIds = mentionedUserIds
        //        self.sendUserMessage(messageParams: messageParams, parentMessage: parentMessage)
    }
    
    /// Sends a file message with file data, file name, mime type.
    /// - Parameters:
    ///   - fileData: `Data` class object
    ///   - fileName: file name. Used when displayed in channel list.
    ///   - mimeType: file's mime type.
    ///   - parentMessage: The parent message. The default value is `nil` when there's no parent message.
    open func sendFileMessage(fileData: Data?, fileName: String, mimeType: String, parentMessage: JMessage? = nil) {
//        guard let fileData = fileData else { return }
//        let messageParams = JMessageCreateParams(file: fileData)
//        messageParams.fileName = fileName
//        messageParams.mimeType = mimeType
//        messageParams.fileSize = UInt(fileData.count)
//
//        // Image size
//        if let image = UIImage(data: fileData) {
//            let thumbnailSize = ThumbnailSize.make(maxSize: image.size)
//            messageParams.thumbnailSizes = [thumbnailSize]
//        }
//
//        // Video thumbnail size
//        else if let asset = fileData.getAVAsset() {
//            let avAssetImageGenerator = AVAssetImageGenerator(asset: asset)
//            avAssetImageGenerator.appliesPreferredTrackTransform = true
//            let cmTime = CMTimeMake(value: 2, timescale: 1)
//            if let cgImage = try? avAssetImageGenerator.copyCGImage(at: cmTime, actualTime: nil) {
//                let image = UIImage(cgImage: cgImage)
//                let thumbnailSize = ThumbnailSize.make(maxSize: image.size)
//                messageParams.thumbnailSizes = [thumbnailSize]
//            }
//        }
//
//        SBUGlobalCustomParams.JMessageParamsSendBuilder?(messageParams)
//
//        if let parentMessage = parentMessage,
//           SendbirdUI.config.groupChannel.channel.replyType != .none {
//            messageParams.parentMessageId = parentMessage.messageId
//            messageParams.isReplyToChannel = true
//        }
//        self.sendJMessage(messageParams: messageParams, parentMessage: parentMessage)
    }
    
    /// Sends a voice message with ``SBUVoiceFileInfo`` object that contains essential information of a voice message.
    /// - Parameters:
    ///   - voiceFileInfo: ``SBUVoiceFileInfo`` class object
    ///   - parentMessage: The parent message. The default value is `nil` when there's no parent message.
    open func sendVoiceMessage(voiceFileInfo: SBUVoiceFileInfo, parentMessage: JMessage? = nil) {
//        guard let filePath = voiceFileInfo.filePath,
//              let fileName = voiceFileInfo.fileName,
//              let fileData = SBUCacheManager.File.diskCache.get(fullPath: filePath) else { return }
//        let playtime = String(Int(voiceFileInfo.playtime ?? 0))
//        let durationMetaArray = MessageMetaArray(key: SBUConstant.voiceMessageDurationKey, value: [playtime])
//        let typeMetaArray = MessageMetaArray(key: SBUConstant.internalMessageTypeKey, value: [SBUConstant.voiceMessageType])
//
//        let messageParams = JMessageCreateParams(file: fileData)
//        messageParams.fileName = fileName // Maintain the file name used for recording to erase the recording file cache
//        messageParams.mimeType = "\(SBUConstant.voiceMessageType);\(SBUConstant.voiceMessageTypeVoiceParameter)"
//        messageParams.fileSize = UInt(fileData.count)
//        messageParams.metaArrays = [durationMetaArray, typeMetaArray]
//
//        SBUGlobalCustomParams.voiceJMessageParamsSendBuilder?(messageParams)
//
//        if let parentMessage = parentMessage,
//           SendbirdUI.config.groupChannel.channel.replyType != .none {
//            messageParams.parentMessageId = parentMessage.messageId
//            messageParams.isReplyToChannel = true
//        }
//
//        self.sendJMessage(messageParams: messageParams, parentMessage: parentMessage)
    }
    
    /// Sends a file message with messageParams.
    ///
    /// You can send a file message by setting various properties of MessageParams.
    /// - Parameters:
    ///    - messageParams: `JMessageCreateParams` class object
    ///    - parentMessage: The parent message. The default value is `nil` when there's no parent message.
    /// - Since: 1.0.9
//    open func sendFileMessage(messageParams: JMessageCreateParams, parentMessage: JMessage? = nil) {
//        guard let channel = self.channel else { return }
//
//        SBULog.info("[Request] Send file message")
//
//        // for voice message
//        let fileName = messageParams.fileName ?? ""
//
//        if SBUUtils.getFileType(by: messageParams.mimeType ?? "") == .voice {
//            let extensiontype = URL(fileURLWithPath: fileName).pathExtension
//            if extensiontype.count > 0 {
//                messageParams.fileName = "\(SBUStringSet.VoiceMessage.fileName).\(extensiontype)"
//            } else {
//                messageParams.fileName = "\(SBUStringSet.VoiceMessage.fileName)"
//            }
//        }
//
//        var preSendMessage: JMessage?
//        preSendMessage = channel.sendJMessage(
//            params: messageParams,
//            progressHandler: { requestId, _, totalBytesSent, totalBytesExpectedToSend in
//                //// If need reload cell for progress, call reload action in here.
//                guard let requestId = requestId, !requestId.isEmpty else { return }
//                let fileTransferProgress = CGFloat(totalBytesSent)/CGFloat(totalBytesExpectedToSend)
//                SBULog.info("File message transfer progress: \(requestId) - \(fileTransferProgress)")
//            },
//            completionHandler: { [weak self] JMessage, error in
//                if let error = error {
//                    SBULog.error(error.localizedDescription)
//                }
//                self?.sendJMessageCompletionHandler?(JMessage, error)
//            }
//        )
//
//        if let preSendMessage = preSendMessage {
//            switch SBUUtils.getFileType(by: preSendMessage) {
//            case .image:
//                SBUCacheManager.Image.preSave(JMessage: preSendMessage)
//            case .video:
//                SBUCacheManager.Image.preSave(JMessage: preSendMessage) // for Thumbnail
//                SBUCacheManager.File.preSave(JMessage: preSendMessage, fileName: messageParams.fileName)
//            case .voice:
//                // voice file's fileName is "Voice message". not have path extension.
//                let extensiontype = URL(fileURLWithPath: fileName).pathExtension
//                let voiceFileName = "\(SBUStringSet.VoiceMessage.fileName).\(extensiontype)"
//                let tempFileName = "\(fileName).\(extensiontype)"
//
//                SBUCacheManager.File.preSave(JMessage: preSendMessage, fileName: voiceFileName)
//                SBUCacheManager.File.removeVoiceTemp(fileName: tempFileName)
//            default:
//                SBUCacheManager.File.preSave(JMessage: preSendMessage, fileName: messageParams.fileName)
//            }
//        }
//
//        if let preSendMessage = preSendMessage, self.messageListParams.belongsTo(preSendMessage) {
//            preSendMessage.parentMessage = parentMessage
//            self.pendingMessageManager.upsertPendingMessage(
//                channelURL: self.channel?.channelURL,
//                message: preSendMessage,
//                forMessageThread: self.isThreadMessageMode
//            )
//
//            self.pendingMessageManager.addFileInfo(
//                requestId: preSendMessage.requestId,
//                params: messageParams,
//                forMessageThread: self.isThreadMessageMode
//            )
//        } else {
//            SBULog.info("A filtered file message has been sent.")
//        }
//
//        self.sortAllMessageList(needReload: true)
//
//        let context = MessageContext(source: .eventMessageSent, sendingStatus: .succeeded)
//        self.baseDelegate?.baseChannelViewModel(
//            self,
//            shouldUpdateScrollInMessageList: self.fullMessageList,
//            forContext: context,
//            keepsScroll: false
//        )
//    }
    
    /// Updates a user message with message object.
    /// - Parameters:
    ///   - message: `UserMessage` object to update
    ///   - text: String to be updated
    /// - Since: 1.0.9
//    public func updateUserMessage(message: UserMessage, text: String) {
//        let text = text.trimmingCharacters(in: .whitespacesAndNewlines)
//        let messageParams = UserMessageUpdateParams(message: text)
//        
//        SBUGlobalCustomParams.userMessageParamsUpdateBuilder?(messageParams)
//        messageParams.mentionedMessageTemplate = ""
//        messageParams.mentionedUserIds = []
//        
//        self.updateUserMessage(message: message, messageParams: messageParams)
//    }
    
    func handlePendingResendableMessage<Message: JMessage>(_ message: Message?, _ error: JErrorCode?) { }
    
    /// Resends a message with failedMessage object.
    /// - Parameter failedMessage: `JMessage` class based failed object
    /// - Since: 1.0.9
    public func resendMessage(failedMessage: JMessage) {
//        if let failedMessage = failedMessage as? UserMessage {
//            SBULog.info("[Request] Resend failed user message")
//
//            let pendingMessage = self.channel?.resendUserMessage(
//                failedMessage
//            ) { [weak self] message, error in
//                guard let self = self else { return }
//                self.handlePendingResendableMessage(message, error)
//            }
//
//            self.pendingMessageManager.upsertPendingMessage(
//                channelURL: self.channel?.channelURL,
//                message: pendingMessage,
//                forMessageThread: self.isThreadMessageMode
//            )
//
//            if let failedMessage = pendingMessage {
//                self.deleteMessagesInList(
//                    messageIds: [failedMessage.messageId],
//                    excludeResendableMessages: true,
//                    needReload: true
//                )
//            }
//
//        } else if let failedMessage = failedMessage as? JMessage {
//            var data: Data?
//
//            if let fileInfo = self.pendingMessageManager.getFileInfo(
//                requestId: failedMessage.requestId,
//                forMessageThread: self.isThreadMessageMode
//            ) {
//                data = fileInfo.file
//            }
//
//            SBULog.info("[Request] Resend failed file message")
//
//            let pendingMessage = self.channel?.resendJMessage(
//                failedMessage,
//                binaryData: data
//            ) { (_, _, _, _) in
//                //// If need reload cell for progress, call reload action in here.
//                // self.tableView.reloadData()
//            } completionHandler: { [weak self] message, error in
//                guard let self = self else { return }
//                self.handlePendingResendableMessage(message, error)
//            }
//
//            self.pendingMessageManager.upsertPendingMessage(
//                channelURL: self.channel?.channelURL,
//                message: pendingMessage,
//                forMessageThread: self.isThreadMessageMode
//            )
//
//            if let failedMessage = pendingMessage {
//                self.deleteMessagesInList(
//                    messageIds: [failedMessage.messageId],
//                    excludeResendableMessages: true,
//                    needReload: true
//                )
//            }
//        } else if let failedMessage = failedMessage as? MultipleFilesMessage {
//            let groupChannel = self.channel as? GroupChannel
//            groupChannel?.resendMultipleFilesMessage(
//                failedMessage,
//                fileUploadHandler: { _, _, _, _ in },
//                completionHandler: { [weak self] message, error in
//                    guard let self = self else { return }
//                    self.handlePendingResendableMessage(message, error)
//            })
//        }
    }    
    
    /// Deletes a message with message object.
    /// - Parameter message: `JMessage` based class object
    /// - Since: 1.0.9
    public func deleteMessage(message: JMessage) {
//        SBULog.info("[Request] Delete message: \(message.description)")
//        self.channel?.deleteMessage(message, completionHandler: nil)
    }
    
    // MARK: - List
    
    /// This function updates the messages in the list.
    ///
    /// It is updated only if the messages already exist in the list, and if not, it is ignored.
    /// And, after updating the messages, a function to sort the message list is called.
    /// - Parameters:
    ///   - messages: Message array to update
    ///   - needReload: If set to `true`, the tableview will be call reloadData.
    /// - Since: 1.2.5
    public func updateMessagesInList(messages: [JMessage]?, needReload: Bool) {
//        messages?.forEach { message in
//            if let index = SBUUtils.findIndex(of: message, in: self.messageList) {
//                if !self.messageListParams.belongsTo(message) {
//                    self.messageList.remove(at: index)
//                } else {
//                    self.messageList[index] = message
//                }
//            }
//        }
//
//        self.sortAllMessageList(needReload: needReload)
    }
    
    // TODO: Not used
    func filteredForThreadMessageView(messages: [JMessage]?) -> [JMessage]? {
//        let pendingMessages = self.pendingMessageManager.getPendingMessages(
//            channelURL: self.channelURL,
//            forMessageThread: true
//        )
//        let refinedResult = messages?.filter { message in
//            var existInPendingThreadMessage = false
//            pendingMessages.forEach {
//                if $0.requestId == message.requestId {
//                    existInPendingThreadMessage = true
//                }
//            }
//            return !existInPendingThreadMessage
//        }
//        return refinedResult
        return []
    }
    
    /// This function upserts the messages in the list.
    /// - Parameters:
    ///   - messages: Message array to upsert
    ///   - needUpdateNewMessage: If set to `true`, increases new message count.
    ///   - needReload: If set to `true`, the tableview will be call reloadData.
    /// - Since: 1.2.5
    public func upsertMessagesInList(
        messages: [JMessage]?,
        needUpdateNewMessage: Bool = false,
        needReload: Bool
    ) {
        SBULog.info("First : \(String(describing: messages?.first)), Last : \(String(describing: messages?.last))")
        
        messages?.forEach { message in
            if let index = SBUUtils.findIndex(of: message, in: self.messageList) {
                self.messageList.remove(at: index)
            }

            if needUpdateNewMessage {
                guard let conversationInfo = self.conversationInfo else { return }
                self.baseDelegate?.baseChannelViewModel(self, didReceiveNewMessage: message, forConversation: conversationInfo)
            }
            
            self.messageList.append(message)
        }
        
        self.sortAllMessageList(needReload: needReload)
    }
    
    /// This function deletes the messages in the list using the message ids. (Resendable messages are also delete together.)
    /// - Parameters:
    ///   - messageIds: Message id array to delete
    ///   - needReload: If set to `true`, the tableview will be call reloadData.
    /// - Since: 1.2.5
    public func deleteMessagesInList(messageIds: [Int64]?, needReload: Bool) {
        self.deleteMessagesInList(
            messageIds: messageIds,
            excludeResendableMessages: false,
            needReload: needReload
        )
    }
    
    /// This function deletes the messages in the list using the message ids.
    /// - Parameters:
    ///   - messageIds: Message id array to delete
    ///   - excludeResendableMessages: If set to `true`, the resendable messages are not deleted.
    ///   - needReload: If set to `true`, the tableview will be call reloadData.
    /// - Since: 2.1.8
    public func deleteMessagesInList(messageIds: [Int64]?,
                                     excludeResendableMessages: Bool,
                                     needReload: Bool) {
//        guard let messageIds = messageIds else { return }
//
//        // if deleted message contains the currently editing message,
//        // end edit mode.
//        if let editMessage = inEditingMessage,
//           messageIds.contains(editMessage.messageId),
//           let channel = self.channel {
//            self.baseDelegate?.baseChannelViewModel(self, shouldFinishEditModeForChannel: channel)
//        }
//
//        var toBeDeleteIndexes: [Int] = []
//        var toBeDeleteRequestIds: [String] = []
//
//        for (index, message) in self.messageList.enumerated() {
//            for messageId in messageIds {
//                guard message.messageId == messageId,
//                      message.isMessageIdValid else { continue }
//                toBeDeleteIndexes.append(index)
//
//                guard message.isRequestIdValid else { continue }
//
//                switch message {
//                case let userMessage as UserMessage:
//                    let requestId = userMessage.requestId
//                    toBeDeleteRequestIds.append(requestId)
//
//                case let JMessage as JMessage:
//                    let requestId = JMessage.requestId
//                    toBeDeleteRequestIds.append(requestId)
//
//                default: break
//                }
//            }
//        }
//
//        // for remove from last
//        let sortedIndexes = toBeDeleteIndexes.sorted().reversed()
//
//        for index in sortedIndexes {
//            self.messageList.remove(at: index)
//        }
//
//        if excludeResendableMessages {
//            self.sortAllMessageList(needReload: needReload)
//        } else {
//            self.deleteResendableMessages(requestIds: toBeDeleteRequestIds, needReload: needReload)
//        }
    }

    /// This functions deletes the resendable message.
    /// If `baseChannel` is type of `GroupChannel`, it deletes the message by using local caching.
    /// If `baseChannel` is not type of `GroupChannel` that not using local caching, it calls `deleteResendableMessages(requestIds:needReload:)`.
    /// - Parameters:
    ///   - message: The resendable`JMessage` object such as failed message.
    ///   - needReload: If `true`, the table view will call `reloadData()`.
    /// - Since: 2.2.1
    public func deleteResendableMessage(_ message: JMessage, needReload: Bool) {
//        self.deleteResendableMessages(requestIds: [message.requestId], needReload: needReload)
    }

    /// This functions deletes the resendable messages using the request ids.
    /// - Parameters:
    ///   - requestIds: Request id array to delete
    ///   - needReload: If set to `true`, the tableview will be call reloadData.
    /// - Since: 1.2.5
    public func deleteResendableMessages(requestIds: [String], needReload: Bool) {
//        for requestId in requestIds {
//            if requestId.isEmpty { continue }
//
//            self.pendingMessageManager.removePendingMessageAllTypes(
//                channelURL: self.channel?.channelURL,
//                requestId: requestId
//            )
//        }
        
        self.sortAllMessageList(needReload: needReload)
    }
    
    /// This function sorts the all message list. (Included `presendMessages`, `messageList` and `resendableMessages`.)
    /// - Parameter needReload: If set to `true`, the tableview will be call reloadData and, scroll to last seen index.
    /// - Since: 1.2.5
    public func sortAllMessageList(needReload: Bool) {
        // Generate full list for draw

        self.messageList.sort { $0.timestamp > $1.timestamp }
        self.fullMessageList = self.messageList
        
        self.baseDelegate?.shouldUpdateLoadingState(false)
        self.baseDelegate?.baseChannelViewModel(
            self,
            didChangeMessageList: self.fullMessageList,
            needsToReload: needReload,
            initialLoad: self.isInitialLoading
        )
    }
    
    /// This functions clears current message lists
    ///
    /// - Since: 2.1.0
    public func clearMessageList() {
        self.fullMessageList.removeAll(where: { SBUUtils.findIndex(of: $0, in: messageList) != nil })
        self.messageList = []
    }
    
    // MARK: - MessageListParams
    private func resetMessageListParams() {
//        self.messageListParams = self.customizedMessageListParams?.copy() as? MessageListParams
//            ?? MessageListParams()
//
//        if self.messageListParams.previousResultSize <= 0 {
//            self.messageListParams.previousResultSize = self.defaultFetchLimit
//        }
//        if self.messageListParams.nextResultSize <= 0 {
//            self.messageListParams.nextResultSize = self.defaultFetchLimit
//        }
//
//        self.messageListParams.reverse = true
//        self.messageListParams.includeReactions = SBUEmojiManager.isReactionEnabled(channel: channel)
//
//        self.messageListParams.includeThreadInfo = SBUGlobals.reply.includesThreadInfo
//        self.messageListParams.includeParentMessageInfo = SBUGlobals.reply.includesParentMessageInfo
//        self.messageListParams.replyType = SendbirdUI.config.groupChannel.channel.replyType.filterValue
//
//        self.messageListParams.includeMetaArray = true
    }
    
    // MARK: - Reactions
    /// This function is used to add or delete reactions.
    /// - Parameters:
    ///   - message: `JMessage` object to update
    ///   - emojiKey: set emoji key
    ///   - didSelect: set reaction state
    /// - Since: 1.1.0
    public func setReaction(message: JMessage, emojiKey: String, didSelect: Bool) {
//        if didSelect {
//            SBULog.info("[Request] Add Reaction")
//            self.channel?.addReaction(with: message, key: emojiKey) { reactionEvent, error in
//                // INFO:
//                // In **super group channel limited mode**, current user can only addReaction and never deleteReaction.
//                // If currentUser reacts to an already reacted emoji, the request succeeds, but Chat SDK returns a decoding error (80000).
//                // (the response doesn't contain "updated_at" field, but Chat SDK tries to decode this as a non-optional property)
//                if let error = error {
//                    self.baseDelegate?.didReceiveError(error, isBlocker: false)
//                }
//
//                SBULog.info("[Response] \(reactionEvent?.key ?? "") reaction")
//                guard let reactionEvent = reactionEvent else { return }
//                if reactionEvent.messageId == message.messageId {
//                    message.apply(reactionEvent)
//                }
//                self.baseDelegate?.baseChannelViewModel(self, didUpdateReaction: reactionEvent, forMessage: message)
//            }
//        } else {
//            SBULog.info("[Request] Delete Reaction")
//            self.channel?.deleteReaction(with: message, key: emojiKey) { reactionEvent, error in
//                if let error = error {
//                    self.baseDelegate?.didReceiveError(error, isBlocker: false)
//                }
//
//                SBULog.info("[Response] \(reactionEvent?.key ?? "") reaction")
//                guard let reactionEvent = reactionEvent else { return }
//                if reactionEvent.messageId == message.messageId {
//                    message.apply(reactionEvent)
//                }
//                self.baseDelegate?.baseChannelViewModel(self, didUpdateReaction: reactionEvent, forMessage: message)
//            }
//        }
    }
    
    // MARK: - Common
    
    /// This function checks that have the following list.
    /// - Returns: This function returns `true` if there is the following list.
    public func hasNext() -> Bool { return false }
    
    /// This function checks that have the previous list.
    /// - Returns: This function returns `true` if there is the previous list.
    public func hasPrevious() -> Bool { return false }
    
    public func getStartingPoint() -> Int64? { return .max }
    
//    // MARK: - Cache
//    func setupCache() {
//        guard let channel = channel else { return }
//        self.messageCache = SBUMessageCache(
//            channel: channel,
//            messageListParam: self.messageListParams
//        )
//        self.messageCache?.loadInitial()
//    }
//
//    func flushCache(with messages: [JMessage]) -> [JMessage] {
//        SBULog.info("flushing cache with : \(messages.count)")
//        guard let messageCache = self.messageCache else { return messages }
//
//        let mergedList = messageCache.flush(with: messages)
//        self.messageCache = nil
//
//        return mergedList
//    }
}
