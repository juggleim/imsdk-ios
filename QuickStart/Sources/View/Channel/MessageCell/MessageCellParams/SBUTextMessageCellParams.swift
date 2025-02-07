//
//  SBUUserMessageCellParams.swift
//  SendbirdUIKit
//
//  Created by Jaesung Lee on 2021/07/19.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import JuggleIM

public class SBUTextMessageCellParams: SBUBaseMessageCellParams {
    public var userMessage: JMessage? {
        self.message
    }
    public let useReaction: Bool
    public let withTextView: Bool
    
    /// The boolean value that decides whether to enable a long press on a reaction emoji.
    /// If `true`, a member list for each reaction emoji is shown. 
    /// - Since: 3.19.0
    public let enableEmojiLongPress: Bool

    /// The boolean value to indicates that the message cell should hide suggested replies.
    /// If it's `true`, never show the suggested replies view even the `JMessage/ExtendedMessagePayload` has the reply `option` values.
    /// - Since: 3.11.0
    public let shouldHideSuggestedReplies: Bool
    
    /// The boolean value to indicates that the message cell should hide form type message.
    /// If it's `true`, never show the form type message view even the `JMessage/ExtendedMessagePayload` has the `forms` values.
    /// - Since: 3.11.0
    public let shouldHideFormTypeMessage: Bool

    public init(
        message: JMessage,
        reaction: JMessageReaction? = nil,
        hideDateView: Bool,
        useMessagePosition: Bool,
        groupPosition: MessageGroupPosition = .none,
        receiptState: SBUMessageReceiptState = .none,
        useReaction: Bool = false,
        withTextView: Bool,
        isThreadMessage: Bool = false,
        joinedAt: Int64 = 0,
        messageOffsetTimestamp: Int64 = 0,
        shouldHideSuggestedReplies: Bool = true,
        shouldHideFormTypeMessage: Bool = true,
        enableEmojiLongPress: Bool = true
    ) {
        self.useReaction = useReaction
        self.withTextView = withTextView
        self.shouldHideSuggestedReplies = shouldHideSuggestedReplies
        self.shouldHideFormTypeMessage = shouldHideFormTypeMessage
        self.enableEmojiLongPress = enableEmojiLongPress
        
        var messagePosition: MessagePosition = .left
        if useMessagePosition {
            let isMyMessage = JIM.shared().currentUserId == message.senderUserId
            messagePosition = isMyMessage ? .right : .left
        }
        
        super.init(
            message: message,
            reaction: reaction,
            hideDateView: hideDateView,
            messagePosition: messagePosition,
            groupPosition: groupPosition,
            receiptState: receiptState,
            isThreadMessage: isThreadMessage,
            joinedAt: joinedAt,
            messageOffsetTimestamp: messageOffsetTimestamp
        )
    }
}
