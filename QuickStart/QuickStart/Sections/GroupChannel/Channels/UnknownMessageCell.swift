//
//  UnknownMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation

class UnknownMessageCell: BaseNotifyMessageCell {
    override func configure(with configuration: SBUBaseMessageCellParams) {
        self.messageLabel.numberOfLines = 0
        self.messageLabel.textAlignment = .center
        self.messageLabel.text = "无法识别的消息"
        self.layoutIfNeeded()
    }
}
