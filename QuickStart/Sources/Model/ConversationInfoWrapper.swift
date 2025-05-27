//
//  ConversationInfoWrapper.swift
//  QuickStart
//
//  Created by Fei Li on 2025/4/15.
//

import Foundation

public struct ConversationInfoWrapper: Hashable {
    public static func == (lhs: ConversationInfoWrapper, rhs: ConversationInfoWrapper) -> Bool {
        let result =
        lhs.conversationInfo?.conversation.conversationType == rhs.conversationInfo?.conversation.conversationType
        &&
        lhs.conversationInfo?.conversation.conversationId == rhs.conversationInfo?.conversation.conversationId
        && lhs.conversationInfo?.unreadCount == rhs.conversationInfo?.unreadCount
        && lhs.conversationInfo?.hasUnread == rhs.conversationInfo?.hasUnread
        && lhs.conversationInfo?.sortTime == rhs.conversationInfo?.sortTime
        && lhs.conversationInfo?.lastMessage?.messageId == rhs.conversationInfo?.lastMessage?.messageId
        && lhs.conversationInfo?.lastMessage?.content?.conversationDigest() == rhs.conversationInfo?.lastMessage?.content?.conversationDigest()
        && lhs.conversationInfo?.isTop == rhs.conversationInfo?.isTop
        && lhs.conversationInfo?.mute == rhs.conversationInfo?.mute
        && lhs.conversationInfo?.draft == rhs.conversationInfo?.draft
        return result
    }
    
    public func hash(into hasher: inout Hasher) {
        hasher.combine(conversationInfo?.conversation.conversationType)
        hasher.combine(conversationInfo?.conversation.conversationId)
    }
    
    var conversationInfo: JConversationInfo?
}
