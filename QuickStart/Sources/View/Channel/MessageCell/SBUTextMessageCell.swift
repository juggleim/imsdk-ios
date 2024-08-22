//
//  SBUUserMessageCell.swift
//  SendbirdUIKit
//
//  Created by Harry Kim on 2020/02/20.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

@IBDesignable
open class SBUTextMessageCell: SBUContentBaseMessageCell, SBUTextMessageTextViewDelegate {
    // MARK: - Public property
    public lazy var messageTextView: UIView = SBUTextMessageTextView()
    
    public var userMessage: JMessage? {
        self.message
    }
    
    // + ------------ +
    // | reactionView |
    // + ------------ +
    /// A ``SBUSelectableStackView`` that contains `reactionView`.
    public private(set) var additionContainerView: SBUSelectableStackView = {
        let view = SBUSelectableStackView()
        return view
    }()
    
    // MARK: - Quick Reply
    
    /// The boolean value whether the ``suggestedReplyView`` instance should appear or not. The default is `true`
    /// - Important: If it's true, ``suggestedReplyView`` never appears even if the ``userMessage`` has quick reply options.
    /// - Since: 3.11.0
    public private(set) var shouldHideSuggestedReplies: Bool = true
    
    // MARK: - Form Type Message
    
    /// The boolean value whether the ``formViews`` instance should appear or not. The default is `true`
    /// - Important: If it's true, ``formViews`` never appears even if the ``userMessage`` has `forms`.
    /// - Since: 3.11.0
    public private(set) var shouldHideFormTypeMessage: Bool = true

    // MARK: - View Lifecycle
    open override func setupViews() {
        super.setupViews()
        
        (self.messageTextView as? SBUTextMessageTextView)?.delegate = self

        // + --------------- +
        // | messageTextView |
        // + --------------- +
        // | reactionView    |
        // + --------------- +
        
        self.mainContainerView.setStack([
            self.messageTextView,
            self.additionContainerView.setStack([
            ])
        ])
    }
    
    open override func setupLayouts() {
        super.setupLayouts()
    }
    
    open override func setupActions() {
        super.setupActions()

        if let messageTextView = self.messageTextView as? SBUTextMessageTextView {
            messageTextView.longPressHandler = { [weak self] _ in
                guard let self = self else { return }
                self.onLongPressContentView(sender: nil)
            }
        }
        
        self.messageTextView.addGestureRecognizer(self.contentLongPressRecognizer)
        self.messageTextView.addGestureRecognizer(self.contentTapRecognizer)

    }

    open override func setupStyles() {
        super.setupStyles()
        
        let isWebviewVisible = false//!self.webView.isHidden
        self.additionContainerView.leftBackgroundColor = isWebviewVisible
            ? self.theme.contentBackgroundColor
            : self.theme.leftBackgroundColor
        self.additionContainerView.leftPressedBackgroundColor = isWebviewVisible
            ? self.theme.pressedContentBackgroundColor
            : self.theme.leftPressedBackgroundColor
        self.additionContainerView.rightBackgroundColor = isWebviewVisible
            ? self.theme.contentBackgroundColor
            : self.theme.rightBackgroundColor
        self.additionContainerView.rightPressedBackgroundColor = isWebviewVisible
            ? self.theme.pressedContentBackgroundColor
            : self.theme.rightPressedBackgroundColor

        self.additionContainerView.setupStyles()
        
        self.additionContainerView.layer.cornerRadius = 8
    }
    
    // MARK: - Common
    open override func configure(with configuration: SBUBaseMessageCellParams) {
        guard let configuration = configuration as? SBUTextMessageCellParams else { return }
        guard let message = configuration.userMessage else { return }
        // Set using reaction
        self.useReaction = configuration.useReaction
        self.enableEmojiLongPress = configuration.enableEmojiLongPress
        
        self.useQuotedMessage = configuration.useQuotedMessage
        
        self.useThreadInfo = configuration.useThreadInfo
        self.shouldHideSuggestedReplies = configuration.shouldHideSuggestedReplies
        self.shouldHideFormTypeMessage = configuration.shouldHideFormTypeMessage

        // Configure Content base message cell
        super.configure(with: configuration)
        
        // Set up message position of additionContainerView(reactionView)
        self.additionContainerView.position = self.position
        self.additionContainerView.isSelected = false
        
        // Set up SBUUserMessageTextView
        if let messageTextView = messageTextView as? SBUTextMessageTextView, configuration.withTextView {
            messageTextView.configure(
                model: SBUTextMessageTextViewModel(
                    message: message,
                    position: configuration.messagePosition
                )
            )
        }
    }
    
    @available(*, deprecated, renamed: "configure(with:)") // 2.2.0
    open func configure(_ message: JMessage,
                        hideDateView: Bool,
                        groupPosition: MessageGroupPosition,
                        receiptState: SBUMessageReceiptState?,
                        useReaction: Bool) {
        let configuration = SBUTextMessageCellParams(
            message: message,
            hideDateView: hideDateView,
            useMessagePosition: true,
            groupPosition: groupPosition,
            receiptState: receiptState ?? .none,
            useReaction: false,
            withTextView: true
        )
        self.configure(with: configuration)
    }
    
    @available(*, deprecated, renamed: "configure(with:)") // 2.2.0
    open func configure(_ message: JMessage,
                        hideDateView: Bool,
                        receiptState: SBUMessageReceiptState?,
                        groupPosition: MessageGroupPosition,
                        withTextView: Bool) {
        let configuration = SBUTextMessageCellParams(
            message: message,
            hideDateView: hideDateView,
            useMessagePosition: true,
            groupPosition: groupPosition,
            receiptState: receiptState ?? .none,
            useReaction: self.useReaction,
            withTextView: withTextView
        )
        self.configure(with: configuration)
    }
    
    /// Adds highlight attribute to the message
    open override func configure(highlightInfo: SBUHighlightMessageInfo?) {
        // Only apply highlight for the given message, that's not edited (updatedAt didn't change)
        guard let message = self.message,
              message.clientMsgNo == highlightInfo?.messageId,
              message.timestamp == highlightInfo?.updatedAt else { return }

        guard let messageTextView = messageTextView as? SBUTextMessageTextView else { return }

        messageTextView.configure(
            model: SBUTextMessageTextViewModel(
                message: message,
                position: position,
                highlightKeyword: highlightInfo?.keyword
            )
        )
    }
    
    // MARK: - Action
    public override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)
        self.additionContainerView.isSelected = selected
    }

    @objc
    open func onTapWebview(sender: UITapGestureRecognizer) {
    }
    
    // MARK: - Mention
    /// As a default, it calls `groupChannelModule(_:didTapMentionUser:)` in ``SBUGroupChannelModuleListDelegate``.
    open func textMessageTextView(_ textView: SBUTextMessageTextView, didTapMention user: SBUUser) {
        self.mentionTapHandler?(user)
    }

}
