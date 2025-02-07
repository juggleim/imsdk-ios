//
//  SBUReactionCollectionViewCell.swift
//  SendbirdUIKit
//
//  Created by Harry Kim on 2020/04/28.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit

enum SBUReactionCellType {
    /// 0. Used for views that are attached to the bottom of the message bubble (SBUMessageReactionView)
    case messageReaction
    /// 1.1. Used for the menu to add a reaction by long touching the message bubble (SBUMenuSheetViewController)
    /// 1.2. Used for the menu that is exposed when pressing the emoji plus button on the message bubble (SBUEmojiListViewController)
    case messageMenu
    /// 2. Used for the view that appears when long-touching an emoji from the emoji list at the bottom of the message bubble. (SBUReactionsViewController)
    /// Used to view the list of users who have emoji
    case reactions
}

class SBUReactionCollectionViewCell: SBUCollectionViewCell {
    lazy var containerView = {
        SBUStackView(axis: .horizontal, alignment: .center, spacing: 4)
    }()
    
    lazy var emojiView = {
        UILabel(frame: .zero)
//        let view = UIImageView(frame: .zero)
//        view.contentMode = .scaleAspectFit
//        return view
    }()
    
    lazy var countLabel = {
        UILabel(frame: .zero)
    }()
    
    lazy var lineView = {
        UIView(frame: .zero)
    }()
    
    lazy var leftMarginView = {
        let view = UIView(frame: .zero)
        view.isHidden = true
        return view
    }()
    
    lazy var rightMarginView = {
        let view = UIView(frame: .zero)
        view.isHidden = true
        return view
    }()

    @IBOutlet weak var emojiImageViewRatioConstraint: NSLayoutConstraint?
    
    var layoutConstraints = [NSLayoutConstraint]() {
        didSet {
            NSLayoutConstraint.deactivate(oldValue)
            NSLayoutConstraint.activate(self.layoutConstraints)
        }
    }
    
    var type: SBUReactionCellType = .messageMenu
    
    var maxCountString: String {
        type == .messageReaction ? "99" : "99+"
    }

    @SBUThemeWrapper(theme: SBUTheme.componentTheme)
    var theme: SBUComponentTheme
    
    var needsSideMargin: Bool = false
    
    var count: Int?

    override var isSelected: Bool {
        didSet {
            self.setSelected()
        }
    }
    
    override func prepareForReuse() {
        super.prepareForReuse()
        self.leftMarginView.isHidden = true
        self.rightMarginView.isHidden = true
        self.needsSideMargin = false
    }

    override func setupViews() {
        super.setupViews()
        
        self.containerView.setHStack([
            self.leftMarginView,
            self.emojiView,
            self.countLabel,
            self.rightMarginView
        ])
        
        self.contentView.addSubview(self.containerView)
        self.contentView.addSubview(self.lineView)
    }
    
    override func setupLayouts() {
        super.setupLayouts()
        
        var constraints = [
            self.containerView.sbu_constraint_lessThan_v2(height: 40),
            self.leftMarginView.sbu_constraint_v2(width: 0),
            self.rightMarginView.sbu_constraint_v2(width: 0),
            self.lineView.sbu_constraint_v2(equalTo: self.contentView, left: 0, right: 0, bottom: 0),
            self.lineView.sbu_constraint_v2(height: 3),
        ].flatMap { $0 }
        
        constraints += [
            self.emojiView.heightAnchor.constraint(equalTo: self.emojiView.widthAnchor, multiplier: 1.0)
        ]
        
        switch type {
        case .messageReaction:
            constraints += [
                self.containerView.sbu_constraint_v2(equalTo: self.contentView, left: 8, right: 8, top: 5),
                self.containerView.sbu_constraint_v2(height: 20),
                
            ].flatMap { $0 }
            
        case .messageMenu:
            constraints += [
                self.containerView.sbu_constraint_v2(equalTo: self.contentView, left: 3, right: 3, top: 3),
                self.containerView.sbu_constraint_v2(height: 38),
            ].flatMap { $0 }
            
        case .reactions:
            constraints += [
                self.containerView.sbu_constraint_v2(equalTo: self.contentView, left: 0, right: 0, top: 0),
                self.containerView.sbu_constraint_v2(height: 28),
            ].flatMap { $0 }
        }
        
        self.layoutConstraints = constraints
        
        self.emojiView.setContentHuggingPriority(UILayoutPriority(751), for: .horizontal)
        self.emojiView.setContentHuggingPriority(UILayoutPriority(751), for: .vertical)
        self.countLabel.setContentHuggingPriority(UILayoutPriority(751), for: .horizontal)
        self.countLabel.setContentHuggingPriority(UILayoutPriority(751), for: .vertical)
        
        self.emojiView.setContentCompressionResistancePriority(UILayoutPriority(750), for: .horizontal)
        self.emojiView.setContentCompressionResistancePriority(UILayoutPriority(750), for: .vertical)
        self.countLabel.setContentCompressionResistancePriority(UILayoutPriority(751), for: .horizontal)
        self.countLabel.setContentCompressionResistancePriority(UILayoutPriority(751), for: .vertical)
    }

    override func setupStyles() {
        super.setupStyles()
        
        self.contentView.backgroundColor = .clear
        
        switch type {
        case .messageReaction:
            self.layer.borderColor = UIColor.clear.cgColor
            self.layer.borderWidth = 1
            self.layer.cornerRadius = 15
            self.countLabel.textColor = self.theme.reactionBoxEmojiCountColor
            self.countLabel.font = self.theme.reactionBoxEmojiCountFont
            self.countLabel.textAlignment = .center
            self.emojiView.font = UIFont.systemFont(ofSize: 13)

            self.lineView.isHidden = true
            
        case .messageMenu:
            self.layer.cornerRadius = 8
            self.lineView.isHidden = true
            self.emojiView.font = UIFont.systemFont(ofSize: 30)
            
        case .reactions:
            self.layer.cornerRadius = 0
            self.countLabel.textColor = self.theme.emojiCountColor
            self.countLabel.font = self.theme.emojiCountFont
            
            self.lineView.isHidden = false
            self.lineView.backgroundColor = self.theme.emojiSelectedUnderlineColor
        }
        
        self.setSelected()
    }

    override func setupActions() {
        super.setupActions()
        
        let contentLongPressRecognizer = UILongPressGestureRecognizer(
            target: self,
            action: #selector(self.onLongPressEmoji(sender:))
        )
        self.contentView.addGestureRecognizer(contentLongPressRecognizer)
    }

    // MARK: - Action
    var emojiLongPressHandler: (() -> Void)?

    @objc
    func onLongPressEmoji(sender: UILongPressGestureRecognizer) {
        if sender.state == .began {
            self.emojiLongPressHandler?()
        }
    }

    func configure(
        type: SBUReactionCellType,
        emoji: String?,
        count: Int? = nil,
        needsSideMargin: Bool? = false
    ) {
        self.type = type
        self.setCount(count)
        
        self.needsSideMargin = needsSideMargin ?? false

        self.emojiView.text = emoji
//        if let urlString = url, !urlString.isEmpty {
//            self.emojiImageView.loadImage(
//                urlString: urlString,
//                errorImage: SBUIconSetType.iconQuestion.image(to: SBUIconSetType.Metric.iconEmojiSmall),
//                subPath: SBUCacheManager.PathType.reaction
//            )
//        } else {
//            self.emojiImageView.image = SBUIconSetType.iconQuestion.image(to: SBUIconSetType.Metric.iconEmojiSmall)
//        }
                
        self.setupLayouts()
        self.setupStyles()
        self.setSelected()
        self.setNeedsLayout()
    }

    func setCount(_ count: Int?) {
        self.count = count
        
        guard let count = count else {
            self.countLabel.isHidden = true
            if needsSideMargin {
                self.leftMarginView.isHidden = false
                self.rightMarginView.isHidden = false
            }
            return
        }
        
        self.countLabel.isHidden = false
        self.leftMarginView.isHidden = true
        self.rightMarginView.isHidden = true

        self.countLabel.text = count > 99 ? self.maxCountString : String(count)
    }

    private func setSelected() {
        switch type {
        case .messageReaction:
            self.backgroundColor = isSelected && (count != nil)
                ? theme.reactionBoxSelectedEmojiBackgroundColor
                : theme.reactionBoxEmojiBackgroundColor
        case .messageMenu:
            self.backgroundColor = isSelected
                ? theme.emojiListSelectedBackgroundColor
                : .clear
        case .reactions:
            self.backgroundColor = .clear
            self.countLabel.textColor = isSelected
                ? theme.emojiSelectedCountColor
                : theme.emojiCountColor
            self.lineView.alpha = isSelected ? 1 : 0
        }
    }
}
