//
//  SBUParentMessageInfoReactionView.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2022/11/12.
//  Copyright © 2022 Sendbird, Inc. All rights reserved.
//

import UIKit

open class SBUParentMessageInfoReactionView: SBUMessageReactionView {
    open override func setupStyles() {
        super.setupStyles()
        
        self.layer.borderWidth = 0
    }

    open override func configure(
        maxWidth: CGFloat,
        useReaction: Bool,
        reactions: [JMessageReactionItem],
        enableEmojiLongPress: Bool
    ) {
        let params = SBUMessageReactionViewParams(
            maxWidth: maxWidth,
            useReaction: useReaction,
            reactions: reactions,
            enableEmojiLongPress: enableEmojiLongPress,
            message: nil
        )
        
        self.configure(configuration: params)
    }
    
    open override func configure(configuration: SBUMessageReactionViewParams) {
        guard configuration.useReaction else {
            self.collectionViewMinWidthContraint?.isActive = false
            self.isHidden = true
            return
        }

        self.collectionViewMinWidthContraint?.isActive = true
        self.maxWidth = configuration.maxWidth
        self.message = configuration.message
        self.reactions = configuration.reactions
        self.emojiList = EmojiManager.getAllEmojis()
        self.enableEmojiLongPress = configuration.enableEmojiLongPress
        
        self.sameCellWidth = true

        let hasMoreEmoji = self.hasMoreEmoji()
        let cellSizes = reactions.reduce(0) {
            $0 + self.getCellSize(count: $1.userInfoList.count).width
        }

        var width: CGFloat = cellSizes
            + CGFloat(reactions.count - 1) * layout.minimumLineSpacing
            + layout.sectionInset.left + layout.sectionInset.right
        if hasMoreEmoji {
            width += self.getCellSize(count: 0).width + layout.minimumLineSpacing
        }
        self.collectionViewMinWidthContraint?.constant = width < maxWidth ? width : maxWidth
        self.collectionView.reloadData()
        self.collectionView.layoutIfNeeded()
        self.collectionViewHeightConstraint?.constant = self.collectionView
            .collectionViewLayout.collectionViewContentSize.height
        if self.collectionViewHeightConstraint?.constant == 0 {
            self.collectionViewHeightConstraint?.constant = 46
        }
    }
    
    open override func getCellSize(count: Int) -> CGSize {
        return CGSize(width: 54, height: 30)
    }
    
    open override func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        guard !reactions.isEmpty else { return 1 }
        
        let hasMoreEmoji = self.hasMoreEmoji()
        if hasMoreEmoji {
            return self.reactions.count + 1
        } else {
            return self.reactions.count
        }
    }
}
