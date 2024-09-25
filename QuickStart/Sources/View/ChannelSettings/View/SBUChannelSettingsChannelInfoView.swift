//
//  SBUChannelSettingsChannelInfoView.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2020/10/06.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

// TODO: Need improvement
public class SBUChannelSettingsChannelInfoView: SBUView {
    public lazy var stackView = UIStackView()
    public lazy var coverImage = SBUCoverImageView()
    public lazy var channelNameField = UITextField()
    public lazy var lineView = UIView()
    
    @SBUThemeWrapper(theme: SBUTheme.channelSettingsTheme)
    var theme: SBUChannelSettingsTheme
    
    var conversationInfo: JConversationInfo?
    
    let kCoverImageSize: CGFloat = 64.0
    
    var lineViewBottomConstraint: NSLayoutConstraint?
    var urlLineViewBottomConstraint: NSLayoutConstraint?
     
    override init() {
        super.init(frame: .zero)
    }
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        
        self.setupViews()
        self.setupLayouts()
        self.setupStyles()
    }
    
    @available(*, unavailable, renamed: "SBUChannelSettingsChannelInfoView(frame:)")
    required convenience init?(coder: NSCoder) {
        fatalError()
    }

    open override func setupViews() {
        super.setupViews()
        
        self.channelNameField.textAlignment = .center
        let paddingView: UIView = UIView(frame: CGRect(x: 0, y: 0, width: 16, height: 0))
        self.channelNameField.leftView = paddingView
        self.channelNameField.leftViewMode = .always
        self.channelNameField.rightView = paddingView
        self.channelNameField.rightViewMode = .always
        self.channelNameField.returnKeyType = .done
        self.channelNameField.isUserInteractionEnabled = false
        
        self.coverImage.clipsToBounds = true
        
        self.stackView.alignment = .center
        self.stackView.axis = .vertical
        self.stackView.spacing = 7
        self.stackView.alignment = .center
        self.stackView.addArrangedSubview(self.coverImage)
        self.stackView.addArrangedSubview(self.channelNameField)
        self.addSubview(stackView)
        self.addSubview(lineView)
        
    }
    
    open override func setupLayouts() {
        super.setupLayouts()
        
        self.coverImage
            .sbu_constraint(width: kCoverImageSize, height: kCoverImageSize)
        
        self.stackView
            .sbu_constraint_equalTo(
                leftAnchor: self.safeAreaLayoutGuide.leftAnchor, 
                left: 0,
                rightAnchor: self.safeAreaLayoutGuide.rightAnchor,
                right: 0,
                topAnchor: self.topAnchor, 
                top: 20
            )
        
        self.lineView
            .sbu_constraint_equalTo(
                leftAnchor: self.safeAreaLayoutGuide.leftAnchor, 
                left: 16,
                rightAnchor: self.safeAreaLayoutGuide.rightAnchor,
                right: 16,
                topAnchor: self.stackView.bottomAnchor,
                top: 20
            )
            .sbu_constraint(height: 0.5)
        
        self.lineViewBottomConstraint?.isActive = false
        self.lineViewBottomConstraint = self.lineView.bottomAnchor.constraint(equalTo: self.bottomAnchor, constant: 0)
    }
    
    open override func setupStyles() {
        super.setupStyles()
        
        self.backgroundColor = .clear
            
        self.lineView.backgroundColor = theme.cellSeparateColor

        self.channelNameField.font = theme.userNameFont
        self.channelNameField.textColor = theme.userNameTextColor
    }
    
    open override func layoutSubviews() {
        super.layoutSubviews()
        
        self.coverImage.layer.cornerRadius = kCoverImageSize / 2
        self.coverImage.layer.borderColor = UIColor.clear.cgColor
        self.coverImage.layer.borderWidth = 1
    }
    
    open func configure(conversationInfo: JConversationInfo?) {
        self.conversationInfo = conversationInfo
        
        guard let conversationInfo = self.conversationInfo else {
            self.coverImage.setPlaceholder(type: .iconUser)
            return
        }
        
        var url = ""
        var name = ""
        if (conversationInfo.conversation.conversationType == .private) {
            if let user = JIM.shared().userInfoManager.getUserInfo(conversationInfo.conversation.conversationId) {
                url = user.portrait ?? ""
                name = user.userName ?? ""
            }
        } else if (conversationInfo.conversation.conversationType == .group) {
            if let group = JIM.shared().userInfoManager.getGroupInfo(conversationInfo.conversation.conversationId) {
                url = group.portrait ?? ""
                name = group.groupName ?? ""
            }
        }
        
        // Cover image
        if url.count > 0 {
            self.coverImage.setImage(withCoverURL: url)
        } else {
            self.coverImage.setPlaceholder(type: .iconUser)
        }
        self.channelNameField.text = name
        
        self.lineViewBottomConstraint?.isActive = true
    }
}
