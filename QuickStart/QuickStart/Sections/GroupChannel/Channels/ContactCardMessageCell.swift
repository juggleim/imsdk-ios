//
//  ContactCardMessageCell.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation

class ContactCardMessageCell: SBUContentBaseMessageCell {
    var nameLabel: UILabel = UILabel()
    var userProfileView: SBUMessageProfileView = SBUMessageProfileView()
    var userContainerView: SBUSelectableStackView = SBUSelectableStackView()
    var typeLabel: UILabel = UILabel()
    var seperationLine: UIView = UIView()
    
    override func setupViews() {
        super.setupViews()
        
        self.mainContainerView.setVStack([
            self.userContainerView.setHStack([
                self.userProfileView,
                self.nameLabel
            ]),
            self.seperationLine,
            self.typeLabel
        ])
    }
    
    override func setupLayouts() {
        super.setupLayouts()
        
        let mainContainerWidth = UIScreen.main.bounds.size.width * 0.6
        let lineWidth = mainContainerWidth - 32
        let nameLabelWidth = lineWidth - 12 - 45
        
        mainContainerView.sbu_constraint(width: mainContainerWidth)
        mainContainerView.stackView.spacing = 12
        userContainerView.stackView.spacing = 12
        userContainerView.sbu_constraint(equalTo: mainContainerView, top: 12)
        
        nameLabel.sbu_constraint(width: nameLabelWidth, height: 45)
        userProfileView.sbu_constraint(equalTo: mainContainerView, leading: 16)
        
        seperationLine.sbu_constraint(width: lineWidth, height: 0.5)
        
        typeLabel.sbu_constraint(width: lineWidth, height: 20)
        typeLabel.sbu_constraint(equalTo: mainContainerView, leading: 16)
        typeLabel.sbu_constraint(equalTo: mainContainerView, bottom: 12)
    }
    
    override func setupStyles() {
        super.setupStyles()
        
//        mainContainerView.backgroundColor = UIColor(hex: "ebebeb")
        
        userProfileView.theme = self.theme
        userProfileView.setupStyles()
        
        typeLabel.font = UIFont.systemFont(ofSize: 13.0, weight: .regular)
        
    }
    
    override func setupActions() {
        super.setupActions()
        
        self.mainContainerView.addGestureRecognizer(self.contentTapRecognizer)
    }
    
    override func configure(with configuration: SBUBaseMessageCellParams) {
        super.configure(with: configuration)
        
        guard let contactCard = configuration.message.content as? ContactCardMessage else {
            return
        }
        nameLabel.text = contactCard.name
        
        var image: UIImage? = nil
        if contactCard.portrait.count == 0 {
            image = PortraitUtil.defaultPortraitImage(with: contactCard.userId, name: contactCard.name, type: .private)
        }
        userProfileView.configure(
            urlString: contactCard.portrait,
            imageSize: 45,
            uiImage: image
        )
        
        typeLabel.text = "个人名片"
        
        if configuration.messagePosition == .left {
            nameLabel.textColor = self.theme.userMessageLeftTextColor
            typeLabel.textColor = self.theme.mentionLeftTextColor
            seperationLine.backgroundColor = self.theme.mentionLeftTextColor
        } else {
            nameLabel.textColor = self.theme.userMessageRightTextColor
            typeLabel.textColor = self.theme.mentionRightTextColor
            seperationLine.backgroundColor = self.theme.mentionRightTextColor
        }
    }
}
