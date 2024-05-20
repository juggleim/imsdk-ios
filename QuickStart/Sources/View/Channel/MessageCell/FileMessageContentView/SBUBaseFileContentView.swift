//
//  SBUBaseFileContentView.swift
//  SendbirdUIKit
//
//  Created by Hoon Sung on 2021/03/18.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import JetIM

open class SBUBaseFileContentView: SBUView {
    @SBUThemeWrapper(theme: SBUTheme.messageCellTheme)
    public var theme: SBUMessageCellTheme
    
    public var message: JFileMessage!
    public var position: MessagePosition = .center
    
    open func configure(message: JFileMessage, position: MessagePosition) {
        self.message = message
        self.position = position
    }
}
