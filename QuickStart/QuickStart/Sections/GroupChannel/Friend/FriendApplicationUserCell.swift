//
//  FriendApplicationUserCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/18.
//

import Foundation
import UIKit

class FriendApplicationUserCell: BaseUserCell {
    var user: JCUser?
    
    public lazy var actionButton: UIButton = {
        let button = UIButton()
        button.isHidden = false
        button.addTarget(self, action: #selector(onClick), for: .touchUpInside)
        return button
    }()
    
    override func setupViews() {
        super.setupViews()
        
        self.baseStackView.setHStack([
            self.userImageView,
            self.nicknameLabel,
            self.userIdLabel,
            self.operatorLabel,
            self.actionButton,
        ])
    }
    
    override func setupStyles() {
        super.setupStyles()
        
        self.actionButton.setTitleColor(theme.checkboxOnColor, for: .normal)
        self.actionButton.setTitleColor(theme.checkboxOffColor, for: .selected)
    }
    
    override func configure(type: UserListType, user: JCUser, isChecked: Bool = false, operatorMode: Bool = false) {
        super.configure(type: type, user: user, isChecked: isChecked, operatorMode: operatorMode)
        
        self.user = user
        
        switch user.friendApplicationStatus {
        case .outgoingApply:
            self.actionButton.setTitle("申请中", for: .selected)
            self.actionButton.isSelected = true
            self.actionButton.isUserInteractionEnabled = false
        case .outgoingAccept:
            self.actionButton.setTitle("对方已接受", for: .selected)
            self.actionButton.isSelected = true
            self.actionButton.isUserInteractionEnabled = false
        case .outgoingExpired:
            self.actionButton.setTitle("无应答", for: .selected)
            self.actionButton.isSelected = true
            self.actionButton.isUserInteractionEnabled = false
        case .incomingApply:
            self.actionButton.setTitle("同意", for: .normal)
            self.actionButton.isSelected = false
            self.actionButton.isUserInteractionEnabled = true
        case .incomingAccept:
            self.actionButton.setTitle("已接受", for: .selected)
            self.actionButton.isSelected = true
            self.actionButton.isUserInteractionEnabled = false
        case .incomingExpired:
            self.actionButton.setTitle("已超时", for: .selected)
            self.actionButton.isSelected = true
            self.actionButton.isUserInteractionEnabled = false
        case .none:
            break
        }
    }
    
    override func setupLayouts() {
        super.setupLayouts()
        self.actionButton.sbu_constraint(width: 120)
    }
    
    @objc private func onClick() {
        if user?.friendApplicationStatus == .incomingApply {
            if let userId = user?.userId {
                SBULoading.start()
                HttpManager.shared.confirmFriend(userId: userId) { [self] code in
                    DispatchQueue.main.async {
                        SBULoading.stop()
                        if code == 0 {
                            self.user?.friendApplicationStatus = .incomingAccept
                            self.configure(type: .friendApplication, user: self.user!)
                        }
                    }
                }
            }
        }
    }
}

