//
//  SBUMessageCellProtocol.swift
//  SendbirdUIKit
//
//  Created by Jaesung Lee on 2021/07/23.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import Foundation

/// The protocol to configure message cells. It conforms to `SBUViewLifeCycle`
///
/// - Since: 2.2.0
public protocol SBUMessageCellProtocol {
    
    /// This function configure a cell using informations.
    /// - Parameter configuration: `SBUJMessageCellParams` object.
//    func configure(with configuration: SBUJMessageCellParams)
    
    /// Adds highlight attribute to the message
    func configure(highlightInfo: SBUHighlightMessageInfo?)
}
