//
//  SBUBaseChannelListViewModel.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2022/08/21.
//  Copyright © 2022 Sendbird, Inc. All rights reserved.
//

import UIKit
import JuggleIM

public protocol SBUBaseChannelListViewModelDelegate: SBUCommonViewModelDelegate {}

open class SBUBaseChannelListViewModel: NSObject {
    
    // MARK: - Property (private)
    weak var baseDelegate: SBUBaseChannelListViewModelDelegate?
    var isLoading = false
    
    // MARK: - Life Cycle
    
    /// This function initializes the ViewModel.
    /// - Parameters:
    ///   - delegate: This is used to receive events that occur in the view model
    ///   - channelListQuery: This is used to use customized channelListQuery.
    public init(
        delegate: SBUBaseChannelListViewModelDelegate?
    ) {
        self.baseDelegate = delegate
        
        super.init()
    }
    
    deinit {
        self.reset()
    }
    
    // MARK: - List handling
    
    /// This function initialize the channel list. the channel list will reset.
    public func initChannelList() {
        SBULog.info("[Request] Next channel List")
        loadNextChannelList(reset: true)
    }
    
    /// This function loads the channel list. If the reset value is `true`, the channel list will reset.
    /// - Parameter reset: To reset the channel list
    public func loadNextChannelList(reset: Bool) {
        SBULog.info("[Request] Next channel List")
    }
    
    /// This function resets channelList
    public func reset() {}
    
    // MARK: - SDK Relations
    
    // MARK: - Common
    
    /// This is used to check the loading status and control loading indicator.
    /// - Parameters:
    ///   - loadingState: Set to true when the list is loading.
    ///   - showIndicator: If true, the loading indicator is started, and if false, the indicator is stopped.
    private func setLoading(_ loadingState: Bool, _ showIndicator: Bool) {
        self.isLoading = loadingState
        
        self.baseDelegate?.shouldUpdateLoadingState(showIndicator)
    }
}
