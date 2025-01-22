//
//  NewFriendsUserCell.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/20.
//

import Foundation
import UIKit

class NewFriendsUserCell: BaseUserCell {
    public lazy var unreadCount = UIButton()
    
    public let unreadCountSize: CGFloat = 20
    
    @SBUThemeWrapper(theme: SBUTheme.groupChannelCellTheme)
    public var channelCellTheme: SBUGroupChannelCellTheme
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        addObserver()
    }
    
    init() {
        super.init(style: .default, reuseIdentifier: nil)
        addObserver()
    }
    
    deinit {
        removeObserver()
    }
    
    override func setupViews() {
        super.setupViews()
        
        self.baseStackView.setHStack([
            self.userImageView,
            self.nicknameLabel,
            self.userIdLabel,
            self.operatorLabel,
            self.unreadCount,
        ])
        
        self.unreadCount.isUserInteractionEnabled = false
    }
    
    override func setupStyles() {
        super.setupStyles()
        
        self.unreadCount.backgroundColor = channelCellTheme.unreadCountBackgroundColor
        self.unreadCount.setTitleColor(channelCellTheme.unreadCountTextColor, for: .normal)
        self.unreadCount.titleLabel?.font = channelCellTheme.unreadCountFont
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        self.unreadCount.contentEdgeInsets.left = 6.0
        self.unreadCount.contentEdgeInsets.right = 6.0
        self.unreadCount.layer.cornerRadius = unreadCountSize / 2
    }
    
    override func setupLayouts() {
        super.setupLayouts()
        
        self.unreadCount.sbu_constraint(equalTo: self.baseStackView, top: 9, bottom: 9)
    }
    
    override func configure(type: UserListType, user: JCUser, isChecked: Bool = false, operatorMode: Bool = false) {
        super.configure(type: type, user: user, isChecked: isChecked, operatorMode: operatorMode)
        
        if type == .newFriend {
            var count: Int32 = 0
            let conversation = JConversation(conversationType: .system, conversationId: GlobalConst.friendConversationId)
            if let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) {
                count = conversationInfo.unreadCount
            }
            self.updateUnreadCount(Int(count))
        } else if type == .group {
            self.unreadCount.isHidden = true
        }
    }
    
    private func updateUnreadCount(_ count: Int) {
        switch count {
        case 0:
            self.unreadCount.isHidden = true
        case 1...9:
            self.unreadCount.setTitle(String(count), for: .normal)
            self.unreadCount
                .sbu_constraint(width: unreadCountSize, height: unreadCountSize)
            self.unreadCount.isHidden = false
        case 1...99:
            self.unreadCount.setTitle(String(count), for: .normal)
            self.unreadCount
                .sbu_constraint(width: 30, height: unreadCountSize)
            self.unreadCount.isHidden = false
        case 100...:
            self.unreadCount.setTitle("99+", for: .normal)
            self.unreadCount
                .sbu_constraint(width: 40, height: unreadCountSize)
            self.unreadCount.isHidden = false
        default:
            break
        }
    }
    
    private func addObserver() {
        NotificationCenter.default.addObserver(self, selector: #selector(countDidUpdate(notification:)), name: NSNotification.Name(rawValue: "FriendApplicationCountNtf"), object: nil)
    }
    
    private func removeObserver() {
        NotificationCenter.default.removeObserver(self, name: NSNotification.Name(rawValue: "FriendApplicationCountNtf"), object: nil)
    }
    
    @objc private func countDidUpdate(notification: NSNotification) {
        if let count = notification.object as? Int {
            updateUnreadCount(count)
        }
    }
}
