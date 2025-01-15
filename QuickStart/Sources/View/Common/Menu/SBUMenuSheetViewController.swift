//
//  SBUMenuSheetViewController.swift
//  SendbirdUIKit
//
//  Created by Harry Kim on 2020/04/26.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

public class SBUMenuSheetViewController: SBUBaseViewController, UITableViewDelegate, UITableViewDataSource, UICollectionViewDelegate, UICollectionViewDataSource, UICollectionViewDelegateFlowLayout {
    
    /// Emoji reaction bar (in Message menu)
    lazy var collectionView = UICollectionView(
        frame: .init(x: 0, y: 0, width: 0, height: 76),
        collectionViewLayout: layout
    )
    
    let layout: UICollectionViewFlowLayout = SBUCollectionViewFlowLayout()
    var tableView = UITableView()
    let message: JMessage?
    let reaction: JMessageReaction?
    let items: [SBUMenuItem]
    let emojiList: [String] = EmojiManager.getAllEmojis()
    var useReaction: Bool

    let maxEmojiOneLine = 6

    @SBUThemeWrapper(theme: SBUTheme.componentTheme)
    var theme: SBUComponentTheme

    // MARK: - Action
    public var emojiTapHandler: ((_ emojiKey: String, _ setSelect: Bool) -> Void)?
    public var moreEmojiTapHandler: (() -> Void)?
    public var dismissHandler: (() -> Void)?

    // MARK: - Lifecycle
    @available(*, unavailable, renamed: "SBUMenuViewController.init()")
    required public init?(coder: NSCoder) {
        self.message = nil
        self.items = []
        self.useReaction = false
        self.reaction = nil
        super.init(coder: coder)
    }

    /// Use this function when initialize.
    /// - Parameter items: Menu item types
    public init(message: JMessage, reaction: JMessageReaction?, items: [SBUMenuItem], useReaction: Bool) {
        self.message = message
        self.reaction = reaction
        self.items = items
        self.useReaction = useReaction
        super.init(nibName: nil, bundle: nil)
    }

    public override func viewDidLayoutSubviews() {
        self.updateLayouts()
        super.viewDidLayoutSubviews()
    }

    public override func viewDidLoad() {
        super.viewDidLoad()
        if let bottomSheet = self.presentationController as? SBUBottomSheetController {

            bottomSheet.isEnableTop = false
            let window = UIApplication.shared.currentWindow
            let bottomPadding = window?.safeAreaInsets.bottom ?? 0
            bottomSheet.contentHeight = tableView.contentSize.height + bottomPadding
        }
    }

    public override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        self.dismissHandler?()
    }
    
    // MARK: - Sendbird UIKit Life cycle
    public override func setupViews() {
        self.view.addSubview(self.tableView)

        // tableView
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.register(
            SBUMenuCell.sbu_loadNib(),
            forCellReuseIdentifier: SBUMenuCell.sbu_className
        ) // for xib

        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 56
        self.tableView.backgroundColor = .clear

        if useReaction {
            self.tableView.tableHeaderView = UIView(frame: .init(x: 0, y: 0, width: 0, height: 76))
            self.tableView.tableHeaderView?.addSubview(self.collectionView)

            // collectionView
            self.layout.scrollDirection = .horizontal
            self.layout.sectionInset = UIEdgeInsets(top: 16, left: 16, bottom: 0, right: 16)
            self.layout.itemSize = SBUConstant.emojiListCollectionViewCellSize

            self.collectionView.dataSource = self
            self.collectionView.delegate = self
            self.collectionView.register(
                SBUReactionCollectionViewCell.self,
                forCellWithReuseIdentifier: SBUReactionCollectionViewCell.sbu_className
            ) // for xib
            self.collectionView.bounces = false
            self.collectionView.showsHorizontalScrollIndicator = false
            self.collectionView.backgroundColor = .clear
        }
    }

    public override func setupLayouts() {
        self.tableView.sbu_constraint(equalTo: self.view, left: 0, right: 0, top: 0, bottom: 0)
        
        if let tableHeaderView = self.tableView.tableHeaderView {
            self.collectionView.sbu_constraint(
                equalTo: tableHeaderView,
                left: 0,
                right: 0,
                top: 0,
                bottom: 0
            )
        }
        
        self.tableView.layoutIfNeeded()
    }
    
    public override func updateLayouts() {
        super.viewDidLayoutSubviews()
        let itemCount: CGFloat = CGFloat(
            maxEmojiOneLine
        )
        if itemCount > 2 {
            let space = (
                self.view.frame.width
                    - layout.sectionInset.left
                    - layout.sectionInset.right
                    - itemCount * layout.itemSize.width
                ) / (itemCount - 1)
            self.layout.minimumLineSpacing = space
        }
    }

    public override func setupStyles() {
        self.view.backgroundColor = theme.backgroundColor
    }

    // MARK: - UITableView relations
    public func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.items.count
    }

    public func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        guard let cell = self.tableView.dequeueReusableCell(
            withIdentifier: SBUMenuCell.sbu_className,
            for: indexPath
            ) as? SBUMenuCell else { return .init() }
        
        let item = self.items[indexPath.row]
        cell.isEnabled = item.isEnabled
        cell.configure(with: item)
        return cell
    }

    public func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        if let cell = tableView.cellForRow(at: indexPath) as? SBUMenuCell {
            if cell.isEnabled {
                cell.tapHandler? { [weak self] transitionsWhenSelected in
                    guard !transitionsWhenSelected else {
                        // automatically dismisses the viewController in viewWillDisappear
                        return
                    }
                    
                    self?.dismiss(animated: true)
                }
            } else {
                tableView.deselectRow(at: indexPath, animated: true)
                return
            }
        } else {
            tableView.deselectRow(at: indexPath, animated: true)
        }
    }

    // MARK: - UICollectionView relations
    public func numberOfSections(in collectionView: UICollectionView) -> Int {
        return 1
    }

    public func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return emojiList.count < 6 ? emojiList.count : 6
    }

    public func collectionView(
        _ collectionView: UICollectionView,
        cellForItemAt indexPath: IndexPath
    ) -> UICollectionViewCell {
        guard let cell = collectionView.dequeueReusableCell(
            withReuseIdentifier: SBUReactionCollectionViewCell.sbu_className,
            for: indexPath) as? SBUReactionCollectionViewCell else { return .init() }

        if indexPath.row == 5 {
            let moreEmoji = SBUIconSetType.iconEmojiMore.image(
                with: theme.addReactionTintColor,
                to: SBUIconSetType.Metric.iconEmojiLarge
            )
            cell.configure(type: .messageMenu, emoji: nil)
            cell.emojiView.text = "more"
            cell.isSelected = false
            return cell
        }

        let emoji = emojiList[indexPath.row]
        cell.configure(type: .messageMenu, emoji: emoji)
        
        let didSelect = reaction?.itemList.first { $0.reactionId == emoji }?.userInfoList.contains(where: { $0.userId == JIM.shared().currentUserId }) ?? false
        cell.isSelected = didSelect

        return cell
    }

    public func collectionView(
        _ collectionView: UICollectionView,
        didSelectItemAt indexPath: IndexPath
    ) {
        
        if indexPath.row == 5 {
            self.moreEmojiTapHandler?()
            self.dismiss(animated: true)
            return
        }

//        guard let currentUserId = JIM.shared().currentUserId else {
//            self.dismiss(animated: true); return
//        }
        
        let currentUserId = JIM.shared().currentUserId

        let emoji = emojiList[indexPath.row]
        //TODO: reaction
        
        
        if let reactionItem = reaction?.itemList.first(where: { $0.reactionId == emoji }) {
            let shouldSelect = reactionItem.userInfoList.contains(where: {$0.userId == currentUserId}) == false
            self.emojiTapHandler?(emoji, shouldSelect)
        } else {
            self.emojiTapHandler?(emoji, true)
        }

        self.dismiss(animated: true)
    }
}
