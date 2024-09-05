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
    
    public lazy var baseStackView: UIStackView = {
        let stackView = SBUStackView(
            axis: .horizontal,
            alignment: .fill,
            spacing: 16.0
        )
        stackView.distribution = .fill
        return stackView
    }()
    
    public lazy var portraitView: UIImageView = {
        let imageView = UIImageView()
        imageView.contentMode = .center
        imageView.clipsToBounds = true
        imageView.isUserInteractionEnabled = true
        return imageView
    }()
    
    public lazy var nameLabel = UILabel()
    
    public var separateView = UIView()
    
    @SBUThemeWrapper(theme: SBUTheme.userCellTheme)
    public var theme: SBUUserCellTheme
    
    internal private(set) var userImageSize: CGFloat = 32
    
    // MARK: - View Lifecycle
    override class func awakeFromNib() {
        super.awakeFromNib()
    }
    
    override func setupViews() {
        self.baseStackView.setHStack([
            self.portraitView,
            self.nameLabel
        ])
        
        self.contentView.addSubview(self.baseStackView)
        self.contentView.addSubview(self.separateView)
    }
    
    override func setupLayouts() {
        self.baseStackView
            .sbu_constraint(
                equalTo: self.contentView,
                leading: 16,
                trailing: -16,
                top: 8,
                bottom: 8
            )
            .sbu_constraint(height: userImageSize)

        self.portraitView
            .sbu_constraint(width: userImageSize, height: userImageSize)
        
        self.nameLabel
            .setContentHuggingPriority(.required, for: .horizontal)
        
//        self.nameLabel.frame = CGRect(x: 16, y: 0, width: self.bounds.width - 32, height: self.bounds.height)
        self.separateView
            .sbu_constraint(equalTo: self.nameLabel, leading: 0)
            .sbu_constraint(
                equalTo: self.contentView,
                trailing: -0.5,
                bottom: 0.5
            )
            .sbu_constraint(height: 0.5)
    }
    
    override func updateLayouts() {
        super.updateLayouts()
        
        self.setupLayouts()
    }
    
    override func setupStyles() {
        self.backgroundColor = theme.backgroundColor
        
        self.portraitView.layer.cornerRadius = userImageSize/2
        self.portraitView.backgroundColor = theme.userPlaceholderBackgroundColor
        
        self.portraitView.image = SBUIconSetType.iconSupergroup.image(
            with: self.theme.userPlaceholderTintColor,
            to: SBUIconSetType.Metric.defaultIconSize
        )
        self.portraitView.layer.cornerRadius = userImageSize/2
        self.portraitView.backgroundColor = self.theme.mutedStateBackgroundColor
        
        self.nameLabel.textColor = theme.nicknameTextColor
        self.nameLabel.font = theme.nicknameTextFont
        
        self.separateView.backgroundColor = theme.separateColor
        
        self.backgroundColor = theme.backgroundColor
    }
    
    // MARK: - Common
    open func configure(name: String) {
        self.nameLabel.text = name
    }
}
