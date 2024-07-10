//
//  SBUChannelTitleView.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 25/02/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JetIM

public class SBUChannelTitleView: UIView {
    // MARK: - Public
    public var conversationInfo: JConversationInfo?
    
    @SBUThemeWrapper(theme: SBUTheme.componentTheme)
    public var theme: SBUComponentTheme
    
    // MARK: - Private
    public lazy var contentView = UIView()
    public lazy var coverImage = SBUCoverImageView()
    public lazy var stackView = UIStackView()
    public lazy var titleLabel = UILabel()
    public lazy var statusField = UITextField()
    public lazy var onlineStateIcon = UIView()

    private let kCoverImageSize: CGFloat = 34.0
    
    /// - Since: 3.5.8
    var isChatNotificationChannelUsed: Bool = false
    
    var contentHeightConstant: NSLayoutConstraint?
    
    // MARK: - Life cycle
    override public init(frame: CGRect) {
        super.init(frame: frame)
        
        self.setupViews()
        self.setupLayouts()
    }
    
    @available(*, unavailable, renamed: "SBUChannelTitleView.init(frame:)")
    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }

    func setupViews() {
        self.coverImage.clipsToBounds = true
        self.coverImage.frame = CGRect(x: 0, y: 0, width: kCoverImageSize, height: kCoverImageSize)
        
        self.titleLabel.textAlignment = .left
        
        self.onlineStateIcon = UIView(frame: CGRect(x: 0, y: 0, width: 6, height: 6))
        
        self.statusField.textAlignment = .left
        self.statusField.leftView = self.onlineStateIcon
        self.statusField.leftViewMode = .never
        self.statusField.isUserInteractionEnabled = false
        
        self.stackView.alignment = .center
        self.stackView.axis = .vertical
        self.stackView.addArrangedSubview(self.titleLabel)
        self.stackView.addArrangedSubview(self.statusField)
        
        self.contentView.addSubview(self.coverImage)
        self.contentView.addSubview(self.stackView)
        self.addSubview(self.contentView)
    }
    
    func setupLayouts() {
        self.contentView.sbu_constraint(equalTo: self, left: 0, right: 0, top: 0, bottom: 0)
        
        self.contentHeightConstant = self.contentView.heightAnchor.constraint(
            equalToConstant: self.bounds.height
        )
        self.contentHeightConstant?.priority = .defaultLow
        NSLayoutConstraint.sbu_activate(baseView: self.contentView, constraints: [
            contentHeightConstant
        ])
        
        NSLayoutConstraint.sbu_activate(baseView: self.coverImage, constraints: [
            self.coverImage.leftAnchor.constraint(equalTo: self.contentView.leftAnchor, constant: 5),
            self.coverImage.widthAnchor.constraint(equalToConstant: kCoverImageSize),
            self.coverImage.heightAnchor.constraint(equalToConstant: kCoverImageSize),
            self.coverImage.centerYAnchor.constraint(equalTo: self.centerYAnchor, constant: 0),
        ])
        
        NSLayoutConstraint.sbu_activate(baseView: self.stackView, constraints: [
            self.stackView.leftAnchor.constraint(
                equalTo: self.coverImage.rightAnchor,
                constant: 8
            ),
            self.stackView.heightAnchor.constraint(
                equalTo: self.coverImage.heightAnchor,
                multiplier: 1.0
            ),
            self.stackView.rightAnchor.constraint(
                equalTo: self.contentView.rightAnchor,
                constant: 5),
            self.stackView.centerYAnchor.constraint(
                equalTo: self.centerYAnchor,
                constant: 0)
        ])
        
        NSLayoutConstraint.sbu_activate(baseView: self.titleLabel, constraints: [
            self.titleLabel.widthAnchor.constraint(
                equalTo: self.stackView.widthAnchor,
                multiplier: 1.0
            )
        ])
        
        NSLayoutConstraint.sbu_activate(baseView: self.statusField, constraints: [
            self.statusField.widthAnchor.constraint(
                equalTo: self.stackView.widthAnchor,
                multiplier: 1.0
            ),
            self.statusField.heightAnchor.constraint(equalToConstant: 12)
        ])
    }
    
    func setupStyles() {
        self.onlineStateIcon.backgroundColor = theme.titleOnlineStateColor
        
        // When used in ChatNotification, set the style in headerComponent.
        if !self.isChatNotificationChannelUsed {
            self.titleLabel.font = theme.titleFont
            self.titleLabel.textColor = theme.titleColor
        }

        self.statusField.font = theme.titleStatusFont
        self.statusField.textColor = theme.titleStatusColor
    }
    
    public override func layoutSubviews() {
        super.layoutSubviews()

        self.onlineStateIcon.layer.cornerRadius = self.onlineStateIcon.frame.width/2
        
        self.coverImage.layer.cornerRadius = kCoverImageSize/2
        self.coverImage.layer.borderColor = UIColor.clear.cgColor
        self.coverImage.layer.borderWidth = 1
        
        self.setupStyles()
    }

    // MARK: - Common
    public func configure(conversationInfo: JConversationInfo?, title: String?) {
        self.conversationInfo = conversationInfo
        self.titleLabel.text = ""

        self.loadCoverImage()
        self.statusField.isHidden = true
        
        guard title == nil else {
            self.titleLabel.text = title
            self.updateChannelStatus(conversationInfo: conversationInfo)
            return
        }

        if let conversationId = conversationInfo?.conversation.conversationId {
            if let conversationType = conversationInfo?.conversation.conversationType {
                if conversationType == .private {
                    self.titleLabel.text = JIM.shared().userInfoManager.getUserInfo(conversationId)?.userName
                } else if conversationType == .group {
                    self.titleLabel.text = JIM.shared().userInfoManager.getGroupInfo(conversationId)?.groupName
                } else {
                    self.titleLabel.text = ""
                }
            }
        }
        
        self.updateChannelStatus(conversationInfo: conversationInfo)
    }
    
    func loadCoverImage() {
        guard let conversationInfo = self.conversationInfo else {
            self.coverImage.setPlaceholder(type: .iconUser, iconSize: CGSize(width: 40, height: 40))
            return
        }
        if conversationInfo.conversation.conversationType == .private {
            if let portrait = JIM.shared().userInfoManager.getUserInfo(conversationInfo.conversation.conversationId)?.portrait {
                self.coverImage.setImage(withCoverURL: portrait)
            } else {
                self.coverImage.setPlaceholder(type: .iconUser, iconSize: CGSize(width: 40, height: 40))
            }
        } else if conversationInfo.conversation.conversationType == .group {
            if let portrait = JIM.shared().userInfoManager.getGroupInfo(conversationInfo.conversation.conversationId)?.portrait {
                self.coverImage.setImage(withCoverURL: portrait)
            } else {
                self.coverImage.setPlaceholder(type: .iconUser, iconSize: CGSize(width: 40, height: 40))
            }
        } else {
            self.coverImage.setPlaceholder(type: .iconUser, iconSize: CGSize(width: 40, height: 40))
        }
    }
    
    public func updateChannelStatus(conversationInfo: JConversationInfo?) {
        self.statusField.leftViewMode = .never
    }
    
    // MARK: - Util
//    private func buildTypingIndicatorString(channel: GroupChannel) -> String? {
//        guard let typingMembers = channel.getTypingUsers(),
//            !typingMembers.isEmpty else { return nil }
//        return SBUStringSet.Channel_Typing(typingMembers)
//    }
    
    public override var intrinsicContentSize: CGSize {
        // NOTE: this is under assumption that this view is used in
        // navigation and / or stack view to shrink but keep max width
        CGSize(width: 100000, height: self.frame.height)
    }
}
