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
    
    // MARK: Channel
    
    /// The base module for channels. The default is `SBUBaseChannelModule` type.
    /// ```swift
    /// SBUModuleSet.BaseChannelModule = SBUBaseChannelModule.self
    /// ```
    /// - Since: 3.6.0
    public static var BaseChannelModule: SBUBaseChannelModule.Type = SBUBaseChannelModule.self
    
    /// The module for group channels. The default is `SBUGroupChannelModule` type.
    /// ```swift
    /// SBUModuleSet.GroupChannelModule = SBUGroupChannelModule.self
    /// ```
    /// - Since: 3.6.0
    public static var GroupChannelModule: SBUGroupChannelModule.Type = SBUGroupChannelModule.self
    
}
