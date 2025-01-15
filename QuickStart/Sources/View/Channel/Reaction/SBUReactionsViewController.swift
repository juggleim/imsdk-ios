//
//  SBUReactionsViewController.swift
//  SendbirdUIKit
//
//  Created by Harry Kim on 2020/04/24.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

/// - Since: 3.11.0
protocol SBUReactionsViewControllerDelegate: SBUCommonDelegate {
    /// Called when the user cell was selected in the `tableView`.
    /// - Parameters:
    ///   - viewController: `SBUReactionsViewController` object
    ///   - tableView: `UITableView` object
    ///   - user: The `SBUUser` of user profile that was selected.
    ///   - indexPath: An index path locating the row in table view of `tableView`
    func reactionsViewController(
        _ viewController: SBUReactionsViewController,
        tableView: UITableView,
        didSelect user: SBUUser,
        forRowAt indexPath: IndexPath
    )
    
    /// Called when the user profile was tapped in the `tableView`.
    /// - Parameters:
    ///   - viewController: `SBUReactionsViewController` object
    ///   - user: The `SBUUser` of user profile that was tapped.
    func reactionsViewController(
        _ viewController: SBUReactionsViewController,
        didTapUserProfile user: SBUUser
    )
}

/// Reacted user list
/// - Since: 3.11.0
public class SBUReactionsViewController: SBUBaseViewController, UITableViewDelegate, UITableViewDataSource, UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout, UIScrollViewDelegate, SBUBottomSheetControllerDelegate {

    lazy var collectionView = UICollectionView(frame: .zero, collectionViewLayout: layout)
    let layout: UICollectionViewFlowLayout = SBUCollectionViewFlowLayout()
    let stackView = UIStackView()
    let lineView = UIView(frame: .init(x: 0, y: 0, width: 0, height: 0.5))
    let tableView = UITableView()
    let emojiList = EmojiManager.getAllEmojis()

    var channel: JConversationInfo!
    var selectedReaction: JMessageReactionItem?
    var reactionList: [JMessageReactionItem] = []

    var collectionViewConstraintWidth: NSLayoutConstraint?
    var collectionViewConstraintMaxWidth: NSLayoutConstraint?

    var theme = SBUTheme.componentTheme
    
    weak var delegate: SBUReactionsViewControllerDelegate? // 3.11.0

    // MARK: - Lifecycle
    @available(*, unavailable, renamed: "SBUReactionsViewController.init(channel:message:selectedReaction:)")
    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }

    /// Use this function when initialize.
    /// - Parameter message: BaseMessage types
    init(channel: JConversationInfo, message: JMessage, reaction: JMessageReaction?, selectedReaction: JMessageReactionItem?) {
        super.init(nibName: nil, bundle: nil)
        self.channel = channel
        self.reactionList = reaction?.itemList ?? []
        self.selectedReaction = selectedReaction
    }

    public override func setupViews() {
        // stackView
        self.stackView.axis = .vertical
        self.stackView.alignment = .center

        // tableView
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.alwaysBounceVertical = false
        self.tableView.bounces = false
        self.tableView.separatorStyle = .none
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.isScrollEnabled = false
        self.tableView.register(
            type(of: SBUUserCell()),
            forCellReuseIdentifier: SBUUserCell.sbu_className
        )
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 56

        // collectionView
        self.layout.sectionInset = UIEdgeInsets(top: 16, left: 19, bottom: 0, right: 19)
        self.layout.scrollDirection = .horizontal
        self.layout.minimumLineSpacing = 16

        self.collectionView.dataSource = self
        self.collectionView.delegate = self
        self.collectionView.register(
            SBUReactionCollectionViewCell.self,
            forCellWithReuseIdentifier: SBUReactionCollectionViewCell.sbu_className
        ) // for xib
        self.collectionView.bounces = false
        self.collectionView.showsHorizontalScrollIndicator = false

        if let bottomSheetVC = self.presentationController as? SBUBottomSheetController {
            bottomSheetVC.bottomSheetDelegate = self
        }

        self.view.addSubview(self.stackView)
        self.stackView.addArrangedSubview(self.collectionView)
        self.stackView.addArrangedSubview(self.lineView)
        self.stackView.addArrangedSubview(self.tableView)
    }

    /// This function handles the initialization of autolayouts.
    public override func setupLayouts() {
        self.stackView.sbu_constraint(
            equalTo: self.view,
            left: 0,
            right: 0,
            top: 0,
            bottom: 0
        )
        self.tableView.sbu_constraint(equalTo: self.stackView, left: 0)
        self.lineView
            .sbu_constraint(equalTo: self.stackView, left: 0)
            .sbu_constraint(height: 0.5)

        self.collectionView
            .sbu_constraint(height: 60)
            .sbu_constraint(equalTo: self.view, centerX: 0)
        
        self.collectionViewConstraintWidth?.isActive = false
        self.collectionViewConstraintMaxWidth?.isActive = false

        self.collectionViewConstraintWidth = self.collectionView.widthAnchor.constraint(
            equalToConstant: 0
        )
        self.collectionViewConstraintWidth?.priority = .defaultLow

        self.collectionViewConstraintMaxWidth = self.collectionView.widthAnchor.constraint(
            lessThanOrEqualToConstant: self.view.frame.width
        )

        self.collectionViewConstraintWidth?.isActive = true
        self.collectionViewConstraintMaxWidth?.isActive = true

        self.collectionView.layoutIfNeeded()
        self.view.setNeedsLayout()
    }
    
    public override func updateLayouts() {
        self.collectionViewConstraintWidth?.constant = self.collectionView.contentSize.width
        self.collectionViewConstraintMaxWidth?.constant = self.view.bounds.width
        self.collectionView.layoutIfNeeded()

        if let bottomSheet = self.presentationController as? SBUBottomSheetController {
            self.tableView.isScrollEnabled = bottomSheet.currentSnapPoint == .top
        }
    }

    /// This function handles the initialization of styles.
    public override func setupStyles() {
        self.view.backgroundColor = theme.backgroundColor
        self.lineView.backgroundColor = theme.reactionMenuLineColor
        self.tableView.backgroundColor = .clear
        self.collectionView.backgroundColor = .clear
    }

    public override func viewDidLoad() {
        super.viewDidLoad()
        if let bottomSheet = self.presentationController as? SBUBottomSheetController {

            let window = UIApplication.shared.currentWindow
            let bottomPadding = window?.safeAreaInsets.bottom ?? 0
            bottomSheet.contentHeight = SBUConstant.bottomSheetMaxMiddleHeight + bottomPadding
        }
    }

    public override func viewDidLayoutSubviews() {
        self.updateLayouts()
        
        super.viewDidLayoutSubviews()
    }

    public override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        super.viewWillTransition(to: size, with: coordinator)
        guard let selectedReaction = self.selectedReaction,
              selectedReaction.userInfoList.count > 0 else { return }
        self.tableView.scrollToRow(at: .init(row: 0, section: 0), at: .top, animated: true)
    }

    public override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        if let indexPath = getSelectedIndexPath() {
            collectionView.selectItem(at: indexPath, animated: false, scrollPosition: .bottom)
        }
    }

    public override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        if let indexPath = getSelectedIndexPath() {
            collectionView.scrollToItem(at: indexPath, at: .centeredHorizontally, animated: true)
        }
    }

    // MARK: - Private func
    private func getSelectedIndexPath() -> IndexPath? {
        guard let selectedReaction = self.selectedReaction else { return nil }
        
        return self.reactionList.enumerated().compactMap {
            $0.element.reactionId == selectedReaction.reactionId
                ? IndexPath(item: $0.offset, section: 0)
                : nil
        }.first
    }

    private func getCellSize(count: Int) -> CGSize {
        switch count {
        case 0...9:
            return CGSize(width: 41, height: 44)
        case 10...99:
            return CGSize(width: 49, height: 44)
        default:
            return CGSize(width: 57, height: 44)
        }
    }
    
    /// - Since: 3.11.0
    func getUser(with indexPath: IndexPath) -> SBUUser? {
        guard let selectedReaction = self.selectedReaction,
              indexPath.row < selectedReaction.userInfoList.count else { return nil }
        let jUser = selectedReaction.userInfoList[indexPath.row]
        let user = SBUUser(user: jUser)
        return user
    }
    
    // MARK: - Actions
    
    /// This function sets the user profile tap gesture handling.
    ///
    /// If you do not want to use the user profile function, override this function and leave it empty.
    /// - Parameter user: `SBUUser` object used for user profile configuration
    /// - Since: 3.11.0
    public func setUserProfileTapGesture(_ user: SBUUser) {
        self.delegate?.reactionsViewController(self, didTapUserProfile: user)
    }
    
    // MARK: - UITableView relations
    public func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        guard let selectedReaction = self.selectedReaction else { return 0 }
        
        return selectedReaction.userInfoList.count
    }

    public func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        guard let cell = self.tableView.dequeueReusableCell(
            withIdentifier: SBUUserCell.sbu_className
            ) as? SBUUserCell else { return .init() }

        cell.selectionStyle = .none

        guard let user = getUser(with: indexPath) else { return .init() }
        cell.configure(type: .reaction, user: user)
        cell.userProfileTapHandler = { [weak cell, weak self] in
            guard let self = self else { return }
            guard cell != nil else { return }
            
            self.setUserProfileTapGesture(user)
        }
        return cell
    }
    
    /// - Since: 3.11.0
    public func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = getUser(with: indexPath) else { return }
        
        self.delegate?.reactionsViewController(
            self,
            tableView: tableView,
            didSelect: user,
            forRowAt: indexPath
        )
    }

    // MARK: - UICollectionView relations
    public func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }

    public func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return self.reactionList.count
    }

    public func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        guard let cell = collectionView.dequeueReusableCell(
            withReuseIdentifier: SBUReactionCollectionViewCell.sbu_className,
            for: indexPath) as? SBUReactionCollectionViewCell else { return .init() }

        guard reactionList.count > indexPath.row else { return .init() }
        
        let reaction = self.reactionList[indexPath.row]
        let emoji = self.emojiList.first(where: { EmojiManager.emojiToUtf16($0) == reaction.reactionId })
        
        cell.configure(
            type: .reactions,
            emoji: emoji,
            count: reaction.userInfoList.count
        )
        cell.emojiView.text = emoji
        return cell
    }

    public func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        return self.getCellSize(count: self.reactionList[indexPath.row].userInfoList.count)
    }

    public func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        guard reactionList.count > indexPath.row else { return }

        self.selectedReaction = self.reactionList[indexPath.row]
        self.tableView.reloadData()
        
        guard collectionView.numberOfSections > 0,
              collectionView.numberOfItems(inSection: 0) > indexPath.row,
              let cell = collectionView.cellForItem(at: indexPath)
                as? SBUReactionCollectionViewCell else { return }
        
        cell.setCount(self.reactionList[indexPath.row].userInfoList.count)
    }

    // MARK: - SBUBottomSheetControllerDelegate
    func bottomSheet(moveTo position: SBUBottomSheetSnapPoint) {

        switch position {
        case .top:
            self.tableView.isScrollEnabled = true
            (self.presentationController as? SBUBottomSheetController)?.isEnableMiddle = false
        case .middle:
            self.tableView.isScrollEnabled = false
        default:
            break
        }
    }
}
