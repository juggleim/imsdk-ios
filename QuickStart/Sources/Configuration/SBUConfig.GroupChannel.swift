//
//  SBUConfig.GroupChannel.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2023/06/01.
//  Copyright © 2023 Sendbird, Inc. All rights reserved.
//

import UIKit

extension SBUConfig {
    public class GroupChannel: SBUUpdatableConfigProtocol {
        // MARK: Property
        
        
        /// Channel list configuration set of GroupChannel
        public var channelList: ChannelList = ChannelList()
        
        /// Channel setting configuration set of GroupChannel
        public var setting: Setting = Setting()
        
        // MARK: Logic
        func updateWithDashboardData(_ groupChannel: GroupChannel) {
//            self.channel.updateWithDashboardData(groupChannel.channel)
            self.channelList.updateWithDashboardData(groupChannel.channelList)
            self.setting.updateWithDashboardData(groupChannel.setting)
        }
    }
}

// MARK: - SBUConfig.GroupChannel.Channel

// MARK: - SBUConfig.GroupChannel.ChannelList
extension SBUConfig.GroupChannel {
    public class ChannelList: NSObject, Codable, SBUUpdatableConfigProtocol {
        // MARK: Property
        
        /// If this value is enabled, the channel list shows the typing indicator. The defaut value is `false`.
        @SBUPrioritizedConfig public var isTypingIndicatorEnabled: Bool = false
        
        /// If this value is enabled, the channel list provides receipt state of the sent message. The defaut value is `false`.
        @SBUPrioritizedConfig public var isMessageReceiptStatusEnabled: Bool = false
        
        // MARK: Logic
        override init() {}
        
        func updateWithDashboardData(_ channelList: ChannelList) {
            self._isTypingIndicatorEnabled.setDashboardValue(channelList.isTypingIndicatorEnabled)
            self._isMessageReceiptStatusEnabled.setDashboardValue(channelList.isMessageReceiptStatusEnabled)
        }
    }
}

// MARK: - SBUConfig.GroupChannel.Setting
extension SBUConfig.GroupChannel {
    public class Setting: NSObject, Codable, SBUUpdatableConfigProtocol {
        // MARK: Property
        
        /// Enable the feature to search for messages within a channel. 
        /// - IMPORTANT: This property may have different activation states depending on the application attribute settings,
        ///              so if you want to use this value for function implementation,
        ///              please use the ``SBUAvailable/isSupportMessageSearch()`` method in the ``SBUAvailable`` class.
        @SBUPrioritizedConfig public var isMessageSearchEnabled: Bool = false
        
        // MARK: Logic
        override init() {}
        
        func updateWithDashboardData(_ setting: Setting) {
            self._isMessageSearchEnabled.setDashboardValue(setting.isMessageSearchEnabled)
        }
    }
}
