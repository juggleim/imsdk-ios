//
//  FriendNotifyMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation

class FriendNotifyMessageCell: BaseNotifyMessageCell {
    override func configure(with configuration: SBUBaseMessageCellParams) {
        super.configure(with: configuration)
        
        var tip = ""
        if let friendNotifyMessage = configuration.message.content as? FriendNotifyMessage {
            let opName = friendNotifyMessage.type == 0 ? "added" : "accepted"
            let userId = configuration.message.conversation.conversationId
            var userName: String = userId
            if let user = JIM.shared().userInfoManager.getUserInfo(userId) {
                userName = user.userName ?? ""
            }
            tip = "\(userName) \(opName) you as a friend"
            if configuration.message.direction == .send {
                tip = "You \(opName) \(userName) as a friend"
            }
        }
        self.messageLabel.text = tip
    }
}
