//
//  SBUQuotedBaseMessageViewParams.swift
//  SendbirdUIKit
//
//  Created by Jaesung Lee on 2021/07/21.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

/// `SBUQuotedBaseMessageViewParams` is a class that defines the parameters for a quoted message view.
/// This includes properties such as the message ID, position, sender's nickname, and the text of the quoted message.
/// It also includes a property to determine if the message cell should show its quoted message view.
public class SBUQuotedBaseMessageViewParams {
    // MARK: Public
    /// The ID of the quoted message.
    /// - Since: 2.2.0
    public let messageId: String
    
    /// The position of the quoted message.
    /// - Since: 2.2.0
    public let messagePosition: MessagePosition
    
    /// The sender nickname of the quoted message.
    /// - Since: 2.2.0
    public let quotedMessageNickname: String
    
    /// The sender nickname of the reply message.
    /// - Since: 2.2.0
    public let replierNickname: String
    
    /// The text of the quoted message.
    public let text: String?
    
    /// If `true`, the message cell shows its quoted message view.
    public let useQuotedMessage: Bool
    
    // MARK: Read-only properties for file message
    
    /// The file URL of the quoted message.
    /// - Since: 2.2.0
    public var urlString: String? {
        switch messageType {
        case .fileMessage(_, _, let urlString): return urlString
        case .multipleFilesMessage(_, _, let urlString): return urlString
        default: return nil
        }
    }
    
    /// The file name of the quoted message.
    /// - Since: 2.2.0
    public var fileName: String? {
        switch messageType {
        case .fileMessage(let name, _, _): return name
        case .multipleFilesMessage(let name, _, _): return name
        default: return nil
        }
    }
    
    /// The file type of the quoted message.
    /// - Since: 2.2.0
    public var fileType: String? {
        switch messageType {
        case .fileMessage(_, let type, _): return type
        case .multipleFilesMessage(_, let type, _): return type
        default: return nil
        }
    }
    
    /// if `true`, the quoted message is type of `FileMessage`.
    /// - Since: 2.2.0
    public var isFileType: Bool {
        switch messageType {
        case .fileMessage, .multipleFilesMessage: return true
        default: return false
        }
    }
    
    /// The creation time of the quoted message
    /// - Since: 3.2.3
    public private(set) var quotedMessageCreatedAt: Int64?
    
    /// The creation time of the message.
    /// - Since: 3.3.0
    public private(set) var messageCreatedAt: Int64?
    
    /// Time the current user joined the channel.
    /// - Since: 3.3.0
    public private(set) var joinedAt: Int64 = 0
    
    /// Message offset of a channel. User can only see messages after this offset.
    /// - Since: 3.9.1
    public internal(set) var messageOffsetTimestamp: Int64 = 0
    
    /// Gets messageFileType with message.
    ///
    /// Checking step:
    /// 1. message.type
    /// 2. message.metaArrays
    ///
    /// - Since: 3.4.0
    public var messageFileType: SBUMessageFileType? {
        guard let fileType = fileType else { return nil }
        
        let type = SBUUtils.getFileType(by: fileType)

        return type
    }
    
    // MARK: - Internal (only for Swift)
    let message: JMessage
    
    let messageType: QuotedMessageType
    
    /// Initializes a new instance of the `BaseMessage` class.
    ///
    /// - Parameters:
    ///   - message: The base message.
    ///   - position: The position of the message.
    ///   - useQuotedMessage: A Boolean value indicating whether to use the quoted message.
    ///   - joinedAt: The time the current user joined the channel. Default value is 0.
    ///   - messageOffsetTimestamp: The message offset of a channel. User can only see messages after this offset. Default value is 0.
    public init(
        message: JMessage,
        position: MessagePosition,
        useQuotedMessage: Bool,
        joinedAt: Int64 = 0,
        messageOffsetTimestamp: Int64 = 0
    ) {
        self.message = message
        self.messageId = message.referredMsg.messageId
        if let quotedMessageSenderId = message.referredMsg.senderUserId {
            let isRepliedToMe = quotedMessageSenderId == JIM.shared().currentUserId
            self.quotedMessageNickname = isRepliedToMe
            ? SBUStringSet.Message_You
            : JIM.shared().userInfoManager.getUserInfo(quotedMessageSenderId).userName ?? ""
        } else {
            self.quotedMessageNickname = SBUStringSet.User_No_Name
        }
        
        let isRepliedByMe = message.senderUserId == JIM.shared().currentUserId
        self.replierNickname = isRepliedByMe
        ? SBUStringSet.Message_You
        : JIM.shared().userInfoManager.getUserInfo(message.senderUserId).userName ?? SBUStringSet.User_No_Name
        
        
        self.messageType = .textMessage
        self.messagePosition = position
        self.useQuotedMessage = useQuotedMessage
        
        if let parentMessage = message.referredMsg {
            self.quotedMessageCreatedAt = parentMessage.timestamp
        }
        
        self.messageCreatedAt = message.timestamp
        
        self.joinedAt = joinedAt
        self.messageOffsetTimestamp = messageOffsetTimestamp
        
        if let textMessage = message.referredMsg.content as? JTextMessage {
            self.text = textMessage.content
        } else {
            self.text = SBUStringSet.Message_Unavailable
        }
    }
}
