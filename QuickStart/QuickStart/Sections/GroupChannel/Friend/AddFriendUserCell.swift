//
//  AddFriendUserCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/9/27.
//

import Foundation
import UIKit

class AddFriendUserCell: BaseUserCell {
    public lazy var addButton: UIButton = {
        let button = UIButton()
        button.isHidden = false
        button.isUserInteractionEnabled = false
        return button
    }()
    
    override func setupViews() {
        super.setupViews()
        
        self.baseStackView.setHStack([
            self.userImageView,
            self.nicknameLabel,
            self.userIdLabel,
            self.operatorLabel,
            self.moreButton,
            self.addButton,
        ])
    }
    
    override func setupStyles() {
        super.setupStyles()
        
        self.addButton.setTitle("添加", for: .normal)
        self.addButton.setTitleColor(theme.checkboxOnColor, for: .normal)
        self.addButton.setTitle("已添加", for: .selected)
        self.addButton.setTitleColor(theme.checkboxOffColor, for: .selected)
    }
    
    override func configure(type: UserListType, user: JCUser, isChecked: Bool = false, operatorMode: Bool = false) {
        super.configure(type: type, user: user, isChecked: isChecked, operatorMode: operatorMode)
        
        self.addButton.isSelected = self.isChecked
    }
    
    override func setupLayouts() {
        super.setupLayouts()
        self.addButton.sbu_constraint(width: 60)
    }
    
    override func selectUser(_ selected: Bool) {
        self.isChecked = selected
        self.addButton.isSelected = selected
    }
}
