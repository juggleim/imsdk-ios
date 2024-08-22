//
//  SBUModuleSet.Deprecated.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2021/09/01.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit

public class SBUModuleSet {
    // MARK: - Properties
     static var shared: SBUModuleSet = SBUModuleSet()
    
    // MARK: - Modules
    
    // Channel list
    
    /// The module for the list of group channels.  The default type is ``SBUGroupChannelListModule`` type.
    @available(*, deprecated, message: "Use `GroupChannelListModule`")
    public static var groupChannelListModule: SBUGroupChannelListModule {
        get {
            let module = shared.groupChannelListModule ?? Self.GroupChannelListModule.init()
            if shared.groupChannelListModule == nil {
                shared.groupChannelListModule = module
            }
            return module
        }
        set { shared.groupChannelListModule = newValue }
    }
    
    
    
    // MARK: - Initialize
    public init(
        groupChannelListModule: SBUGroupChannelListModule? = nil
    ) {
        self.groupChannelListModule = groupChannelListModule
    }
    
    // MARK: - Category
    private var groupChannelListModule: SBUGroupChannelListModule?
}

extension SBUModuleSet {
    @available(*, unavailable, renamed: "init(groupChannelListModule:)") // 3.1.0
    public convenience init(
        channelListModule: SBUGroupChannelListModule = SBUGroupChannelListModule()
    ) {
        
        self.init(
            groupChannelListModule: SBUModuleSet.GroupChannelListModule.init()
        )
    }
}
