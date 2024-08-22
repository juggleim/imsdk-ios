//
//  GroupCell.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation
import UIKit
import JuggleIM

class GroupCell: SBUTableViewCell {
    
    // MARK: - UI properties (Public)
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
        imageView.contentMode = .scaleAspectFill
        imageView.clipsToBounds = true
        imageView.isUserInteractionEnabled = true
        return imageView
    }()
    
    public lazy var mutedStateImageView: UIImageView = {
        let imageView = UIImageView()
        imageView.isHidden = true
        imageView.contentMode = .center
        return imageView
    }()
    
    public lazy var nicknameLabel = UILabel()
    
    public lazy var groupIdLabel = UILabel()
    
    public lazy var operatorLabel: UILabel = {
        let label = UILabel()
         label.isHidden = true
         label.textAlignment = .right
         return label
     }()
    
    public lazy var moreButton: UIButton = {
        let button = UIButton()
        button.isHidden = true
        button.addTarget(self, action: #selector(onClickMoreMenu), for: .touchUpInside)
        return button
    }()
    
    public var separateView = UIView()

    @SBUThemeWrapper(theme: SBUTheme.userCellTheme)
    public var theme: SBUUserCellTheme
    
    // MARK: - Properties (Private)
    private var loadImageSession: URLSessionTask? {
        willSet {
            loadImageSession?.cancel()
        }
    }
    
    // MARK: - Logic properties (Public)
    public private(set) var type: UserListType = .none
    
    // MARK: - Logic properties (Private)
    var hasNickname: Bool = true
    
    var userProfileTapHandler: (() -> Void)?
    var moreMenuHandler: (() -> Void)?
    
    internal private(set) var userImageSize: CGFloat = 40
    
    // MARK: - View Lifecycle
    open override func awakeFromNib() {
        super.awakeFromNib()
    }
    
    /// This function handles the initialization of views.
    open override func setupViews() {
        self.operatorLabel.text = SBUStringSet.User_Operator
        self.groupIdLabel.isHidden = true
        self.portraitView.addSubview(self.mutedStateImageView)
        
        self.baseStackView.setHStack([
            self.portraitView,
            self.nicknameLabel,
            self.groupIdLabel,
            self.operatorLabel,
            self.moreButton,
        ])
        
        if case .suggestedMention = self.type {
            self.baseStackView.setCustomSpacing(6.0, after: self.groupIdLabel)
        }
        self.baseStackView.setCustomSpacing(8.0, after: self.operatorLabel)
        
        self.operatorLabel.setContentCompressionResistancePriority(.required, for: .horizontal)
        
        self.contentView.addSubview(self.baseStackView)
        self.contentView.addSubview(self.separateView)
    }
    
    /// This function handles the initialization of actions.
    open override func setupActions() {
        self.portraitView.addGestureRecognizer(UITapGestureRecognizer(
            target: self,
            action: #selector(self.onTapUserProfileView(sender:)))
        )
    }
    
    /// This function handles the initialization of autolayouts.
    open override func setupLayouts() {
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
        
        self.nicknameLabel
            .setContentHuggingPriority(.required, for: .horizontal)
        
        if !self.groupIdLabel.isHidden {
            self.groupIdLabel
                .sbu_constraint(width: 32, priority: .defaultLow)
                .sbu_constraint_greaterThan(width: 32, priority: .defaultLow)
        }
        
        self.mutedStateImageView
            .sbu_constraint(width: userImageSize, height: userImageSize)
            .sbu_constraint(equalTo: self.portraitView, leading: 0, top: 0)
        
        self.moreButton
            .sbu_constraint(width: 24)
        
        self.separateView
            .sbu_constraint(equalTo: self.nicknameLabel, leading: 0)
            .sbu_constraint(
                equalTo: self.contentView,
                trailing: -0.5,
                bottom: 0.5
            )
            .sbu_constraint(height: 0.5)
    }
    
    open override func updateLayouts() {
        super.updateLayouts()
        
        self.setupLayouts()
    }
    
    /// This function handles the initialization of styles.
    open override func setupStyles() {
        self.backgroundColor = theme.backgroundColor

        self.portraitView.layer.cornerRadius = userImageSize/2
        self.portraitView.backgroundColor = theme.userPlaceholderBackgroundColor
        
        self.mutedStateImageView.image = SBUIconSetType.iconMute.image(
            with: self.theme.mutedStateIconColor,
            to: SBUIconSetType.Metric.defaultIconSize
        )
        self.mutedStateImageView.layer.cornerRadius = userImageSize/2
        self.mutedStateImageView.backgroundColor = self.theme.mutedStateBackgroundColor
        
        if case .suggestedMention = self.type, hasNickname == false {
            self.nicknameLabel.textColor = theme.nonameTextColor
        } else {
            self.nicknameLabel.textColor = theme.nicknameTextColor
        }
        self.nicknameLabel.font = theme.nicknameTextFont
        
        self.groupIdLabel.textColor = theme.userIdTextColor
        self.groupIdLabel.font = theme.userIdTextFont
        
        self.operatorLabel.font = theme.subInfoFont
        self.operatorLabel.textColor = theme.subInfoTextColor
        
        self.separateView.backgroundColor = theme.separateColor
        
        self.moreButton.setImage(
            SBUIconSetType.iconMore.image(
                with: theme.moreButtonColor,
                to: SBUIconSetType.Metric.defaultIconSize
            ),
            for: .normal
        )
        self.moreButton.setImage(
            SBUIconSetType.iconMore.image(
                with: theme.moreButtonDisabledColor,
                to: SBUIconSetType.Metric.defaultIconSize
            ),
            for: .disabled
        )
        
        if self.type == .reaction {
            self.backgroundColor = .clear
        } else {
            self.backgroundColor = theme.backgroundColor
        }
    }
    
    // MARK: - Common
    open func configure(type: UserListType,
                        group: JGroupInfo,
                        operatorMode: Bool = false) {
        self.type = type
        
        self.groupIdLabel.text = group.groupId
        
        if let groupName = group.groupName {
            self.nicknameLabel.text = groupName
            self.hasNickname = true
        }
        
        let profileURL = group.portrait ?? ""
        self.loadImageSession = self.portraitView.loadImage(
            urlString: profileURL,
            placeholder: SBUIconSetType.iconSupergroup.image(
                with: self.theme.userPlaceholderTintColor,
                to: SBUIconSetType.Metric.defaultIconSize
            ),
            subPath: SBUCacheManager.PathType.userProfile
        )
        self.portraitView.contentMode = profileURL.count > 0 ? .scaleAspectFill : .center
        
        self.portraitView.backgroundColor = theme.userPlaceholderBackgroundColor

        self.separateView.isHidden = false
        self.moreButton.isHidden = true
        self.moreButton.isEnabled = true
        
        switch type {
        case .members, .participants:
            if operatorMode {
                self.moreButton.isHidden = false
            }
           
        case .operators:
            self.moreButton.isHidden = false
            
        case .muted, .banned:
            self.moreButton.isHidden = false

        case .suggestedMention(let showsUserId):
            self.groupIdLabel.isHidden = !showsUserId
            self.userImageSize = 28
            self.updateLayouts()
        default:
            break
        }
    }
    
    // MARK: - Action
    
    /// This function is used when more menu tap
    @objc
    open func onClickMoreMenu() {
        self.moreMenuHandler?()
    }
    
    /// This function is used when a user profile tap.
    /// - Parameter sender: sender
    @objc
    open func onTapUserProfileView(sender: UITapGestureRecognizer) {
        self.userProfileTapHandler?()
    }

    open override func prepareForReuse() {
        super.prepareForReuse()
    }
}
