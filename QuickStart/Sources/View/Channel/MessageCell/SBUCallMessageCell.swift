//
//  SBUCallMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation

open class SBUCallMessageCell: SBUTextMessageCell {
    public var iconView: UIImageView = {
        let imageView = UIImageView()
        imageView.contentMode = .center
        imageView.clipsToBounds = true
        imageView.image = UIImage(named: "callIcon")
        return imageView
    }()

//    open override func setupLayouts() {
//        super.setupLayouts()
//        
//        
//    }
    
    open override func configure(with configuration: SBUBaseMessageCellParams) {
        super.configure(with: configuration)
        
        let message = configuration.message
        if let messageTextView = messageTextView as? SBUTextMessageTextView {
            messageTextView.configure(
                model: SBUTextMessageTextViewModel(
                    message: message,
                    position: configuration.messagePosition
                )
            )
            messageTextView.addSubview(self.iconView)
            
            self.iconView.sbu_constraint(width: 30, height: 30)
            
            let iconRightConstraint = self.iconView.rightAnchor.constraint(lessThanOrEqualTo: self.messageTextView.rightAnchor, constant: 0)
            iconRightConstraint.isActive = true
            
            messageTextView.textRightConstraint?.isActive = false
            messageTextView.textRightConstraint = messageTextView.textView.rightAnchor.constraint(lessThanOrEqualTo: self.iconView.leftAnchor)
            messageTextView.textRightConstraint?.isActive = true
        }
    }
}
