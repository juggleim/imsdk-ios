//
//  SBUBaseChannelListViewController.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2020/11/17.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

/**
 - Note: When you create channel list for open channels, please inherit `SBUBaseChannelListViewController`
 
 - Important: You have to avoid using extension because of non-@objc issue
 
 ```swift
 class CustomizedOpenChannelListViewController: SBUBaseChannelListViewController, UITableViewDataSource, UITableViewDelegate
 ```
 */

open class SBUBaseChannelListViewController: SBUBaseViewController {
    
    // MARK: - UI Properties (Public)
    public var baseHeaderComponent: SBUBaseChannelListModule.Header?
    public var baseListComponent: SBUBaseChannelListModule.List?
    
    // MARK: - Logic properties (Public)
    public var baseViewModel: SBUBaseChannelListViewModel?
    
    // MARK: - Life cycle
    deinit {
        SBULog.info("")
        self.baseViewModel = nil
        self.baseHeaderComponent = nil
        self.baseListComponent = nil
    }
    
    // MARK: - Sendbird UIKit Life cycle
    open override func setupViews() {
        self.navigationItem.titleView = self.baseHeaderComponent?.titleView
        self.navigationItem.leftBarButtonItem = self.baseHeaderComponent?.leftBarButton
        self.navigationItem.rightBarButtonItem = self.baseHeaderComponent?.rightBarButton
        
        if let listComponent = self.baseListComponent {
            self.view.addSubview(listComponent)
        }
    }
    
    open override func setupLayouts() {
        self.baseListComponent?.sbu_constraint(equalTo: self.view, left: 0, right: 0, top: 0, bottom: 0)
    }
    
    open override func updateStyles() {
        self.setupStyles()
        
        self.baseListComponent?.reloadTableView()
    }
    
    /// This is a function that shows the channelViewController.
    ///
    /// If you want to use a custom channelViewController, override it and implement it.
    /// - Parameters:
    ///   - conversationInfo:
    open func showChannel(conversationInfo: JConversationInfo) {
    }
}
