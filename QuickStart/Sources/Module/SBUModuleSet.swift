//
//  SBUModuleSet.swift
//  SendbirdUIKit
//
//  Created by Jaesung Lee on 2023/04/27.
//  Copyright © 2023 Sendbird, Inc. All rights reserved.
//

import UIKit

extension SBUModuleSet {
    // MARK: - Modules
    
    // MARK: Channel list
    
    /// The module for the list of group channels.  The default is ``SBUGroupChannelListModule`` type.
    /// ```swift
    /// SBUModuleSet.GroupChannelListModule = SBUGroupChannelListModule.self
    /// ```
    /// - Since: 3.6.0
    public static var GroupChannelListModule: SBUGroupChannelListModule.Type = SBUGroupChannelListModule.self
}
