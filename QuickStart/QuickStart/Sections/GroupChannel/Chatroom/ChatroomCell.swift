//
//  ChatroomCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/9/5.
//

import Foundation
import UIKit

class ChatroomCell: SBUTableViewCell {
    // MARK: - UI properties
    public lazy var nameLabel = UILabel()
    
    @SBUThemeWrapper(theme: SBUTheme.userCellTheme)
    public var theme: SBUUserCellTheme
    
    // MARK: - View Lifecycle
    override class func awakeFromNib() {
        super.awakeFromNib()
    }
    
    override func setupViews() {
        self.contentView.addSubview(self.nameLabel)
    }
    
    override func setupLayouts() {
        self.nameLabel.frame = CGRect(x: 16, y: 0, width: self.bounds.width - 32, height: self.bounds.height)
    }
    
    override func updateLayouts() {
        super.updateLayouts()
        
        self.setupLayouts()
    }
    
    override func setupStyles() {
        self.backgroundColor = theme.backgroundColor
        
        self.nameLabel.textColor = theme.nicknameTextColor
        self.nameLabel.font = theme.nicknameTextFont
    }
    
    // MARK: - Common
    open func configure(name: String) {
        self.nameLabel.text = name
    }
}
