//
//  SBURecallMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/6.
//

import Foundation

class SBURecallMessageCell: BaseNotifyMessageCell {
    override func configure(with configuration: SBUBaseMessageCellParams) {
        super.configure(with: configuration)
        var tip = ""
        if message?.direction == .receive {
            var userName = message?.senderUserId ?? ""
            if let user = JIM.shared().userInfoManager.getUserInfo(message?.senderUserId) {
                userName = user.userName ?? userName
            }
            tip = "\(userName) recalled a message"
        } else {
            tip = "You recalled a message"
        }
        self.messageLabel.text = tip
    }
}
