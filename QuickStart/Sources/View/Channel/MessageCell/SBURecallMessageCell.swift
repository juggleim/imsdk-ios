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
            tip = "\(userName) 撤回了一条消息"
        } else {
            tip = "你 撤回了一条消息"
        }
        self.messageLabel.text = tip
    }
}
