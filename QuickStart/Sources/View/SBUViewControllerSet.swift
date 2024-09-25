//
//  SBUViewControllerSet.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2022/01/17.
//  Copyright © 2022 Sendbird, Inc. All rights reserved.
//
// swiftlint:disable identifier_name
// swiftlint:disable missing_docs

import UIKit

public class SBUViewControllerSet {
    
    /// The view controller for the group channel list.
    public static var GroupChannelListViewController: SBUGroupChannelListViewController.Type = SBUGroupChannelListViewController.self
    
    /// The view controller for the group channel chat.
    public static var GroupChannelViewController: SBUGroupChannelViewController.Type = SBUGroupChannelViewController.self
    
    /// The view controller for the group channel settings.
    public static var GroupChannelSettingsViewController: SBUGroupChannelSettingsViewController.Type = SBUGroupChannelSettingsViewController.self
}

// swiftlint:enable identifier_name
// swiftlint:enable missing_docs
