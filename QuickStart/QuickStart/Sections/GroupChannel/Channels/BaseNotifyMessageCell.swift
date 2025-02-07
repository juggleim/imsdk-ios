//
//  BaseNotifyMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation
import UIKit

class BaseNotifyMessageCell: SBUBaseMessageCell {
    
    // MARK: - Public property
    public var messageLabel = UILabel()
    
    // MARK: - View Lifecycle
    open override func setupViews() {
        super.setupViews()
        self.messageContentView.addSubview(self.messageLabel)
    }
    
    open override func setupLayouts() {
        super.setupLayouts()
        
        self.messageLabel.sbu_constraint(
            equalTo: self.messageContentView,
            left: 28,
            right: 27,
            top: 0,
            bottom: 0
        )
    }
    
    open override func layoutSubviews() {
        super.layoutSubviews()
        let message = self.messageLabel.text ?? ""
        let attributes: [NSAttributedString.Key: Any] = [
            .font: theme.adminMessageFont,
            .foregroundColor: theme.adminMessageTextColor
        ]
        
        let attributedString = NSMutableAttributedString(string: message, attributes: attributes)
        let paragraphStyle = NSMutableParagraphStyle()
        paragraphStyle.alignment = .center
        paragraphStyle.lineSpacing = 8
        attributedString.addAttribute(
            .paragraphStyle,
            value: paragraphStyle,
            range: NSRange(location: 0, length: attributedString.length)
        )
        
        self.messageLabel.attributedText = attributedString
    }
    
    // MARK: - Common
    open override func configure(with configuration: SBUBaseMessageCellParams) {
        // Configure Content base message cell
        super.configure(with: configuration)

        // Set up message label
        self.messageLabel.numberOfLines = 0
        self.messageLabel.textAlignment = .center
        self.messageLabel.text = configuration.message.content.description
        self.layoutIfNeeded()
    }
    
    // MARK: - Action
    open override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)
    }
}
