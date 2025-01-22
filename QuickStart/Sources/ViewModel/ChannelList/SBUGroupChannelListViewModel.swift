//
//  SBUGroupChannelListViewModel.swift
//  SendbirdUIKit
//
//  Created by Hoon Sung on 2021/05/17.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import Foundation
import JuggleIM

public protocol SBUGroupChannelListViewModelDelegate: SBUBaseChannelListViewModelDelegate {
    /// Called when the channe list has been changed.
    /// - Parameters:
    ///    - viewModel: `SBUGroupChannelListViewModel` object.
    ///    - channels: The changed channels.
    ///    - needsToReload: If it's `true`, it needs to reload the view.
    func groupChannelListViewModel(
        _ viewModel: SBUGroupChannelListViewModel,
        didChangeChannelList channels: [JConversationInfo]?,
        needsToReload: Bool
    )
    
    /// Called when a specific channel has been updated.
    /// - Parameters:
    ///    - viewModel: `SBUGroupChannelListViewModel` object.
    ///    - channel: The updated channel.
    func groupChannelListViewModel(
        _ viewModel: SBUGroupChannelListViewModel,
        didUpdateChannel channel: JConversationInfo
    )
}

open class SBUGroupChannelListViewModel: SBUBaseChannelListViewModel {
    // MARK: - Constants
    static let channelLoadLimit: Int32 = 20
    static let notificationChannelLoadLimit: UInt = 100
    
    // MARK: - Property (Public)
    public var conversationInfoList: [JConversationInfo] = []
    
    // MARK: - Property (private)
    weak var delegate: SBUGroupChannelListViewModelDelegate? {
        get { self.baseDelegate as? SBUGroupChannelListViewModelDelegate }
        set { self.baseDelegate = newValue }
    }
    
    var conversationTypes: [NSNumber]?
    
    var historyComplete = false
    
    // MARK: - Life Cycle
    
    /// This function initializes the ViewModel.
    /// - Parameters:
    ///   - delegate: This is used to receive events that occur in the view model
    ///   - channelListQuery: This is used to use customized channelListQuery.
    public init(
        conversationTypes: [NSNumber]?,
        delegate: SBUGroupChannelListViewModelDelegate? = nil
    ) {
        super.init(delegate: delegate)
        self.conversationTypes = conversationTypes
        JIM.shared().conversationManager.add(self)
        
        self.initChannelList()
    }
    
    deinit {
        self.reset()
    }
    
    // MARK: - List handling
    
    /// This function initialize the channel list. the channel list will reset.
    public override func initChannelList() {
        super.initChannelList()
    }
    
    /// This function loads the channel list. If the reset value is `true`, the channel list will reset.
    /// - Parameter reset: To reset the channel list
    public override func loadNextChannelList(reset: Bool) {
        super.loadNextChannelList(reset: reset)
        
        if reset {
            self.reset()
        }
        
        if historyComplete {
            return
        }
        
        let count = self.conversationInfoList.count
        let startTime = count > 0 ? self.conversationInfoList[count-1].sortTime : 0
        let newConversationInfoList: [JConversationInfo]
        if let types = self.conversationTypes {
            newConversationInfoList = JIM.shared().conversationManager.getConversationInfoList(withTypes: types, count: SBUGroupChannelListViewModel.channelLoadLimit, timestamp: startTime, direction: .older)
        } else {
            newConversationInfoList = JIM.shared().conversationManager.getConversationInfoList(byCount: SBUGroupChannelListViewModel.channelLoadLimit, timestamp: startTime, direction: .older)
        }
        if newConversationInfoList.count < SBUGroupChannelListViewModel.channelLoadLimit {
            self.historyComplete = true
        }
        
        self.conversationInfoList.append(contentsOf: newConversationInfoList)
        
        self.delegate?.groupChannelListViewModel(self, didChangeChannelList: self.conversationInfoList, needsToReload: true)
    }
    
    /// This function resets channelList
    public override func reset() {
        super.reset()
        
        self.conversationInfoList = []
        self.historyComplete = false
    }
    
    // MARK: - SDK Relations
    public func deleteConversationInfo(_ conversationInfo: JConversationInfo) {
        SBULog.info("[Request] delete conversationInfo")
        self.setLoading(true, true)
        
        JIM.shared().conversationManager.deleteConversationInfo(by: conversationInfo.conversation) {
            DispatchQueue.main.async { [weak self] in
                self?.setLoading(false, false)
            }
        } error: { code in
            DispatchQueue.main.async { [weak self] in
                self?.setLoading(false, false)
                if code != .none {
                    self?.setLoading(false, false)
                    self?.delegate?.didReceiveError(code, isBlocker: false)
                }
            }
        }
    }
    
    /// Changes push trigger option on a channel.
    /// - Parameters:
    ///   - option: Push trigger option to change
    ///   - channel: Channel to change option
    public func mute(_ conversationInfo:JConversationInfo, isMute: Bool) {
        SBULog.info("[Request] mute: \(isMute ? "on" : "off")")
        self.setLoading(true, true)
        
        JIM.shared().conversationManager.setMute(isMute, conversation: conversationInfo.conversation) {
            DispatchQueue.main.async { [weak self] in
                self?.setLoading(false, false)
            }
        } error: { code in
            DispatchQueue.main.async { [weak self] in
                if code != .none {
                    self?.setLoading(false, false)
                    self?.delegate?.didReceiveError(code, isBlocker: false)
                }
            }
        }
    }
    
    public func setUnread(_ conversationInfo:JConversationInfo, isUnread: Bool) {
        SBULog.info("[Request] setUnread: \(isUnread ? "on" : "off")")
        self.setLoading(true, true)
        
        if isUnread {
            JIM.shared().conversationManager.setUnread(conversationInfo.conversation) {
                DispatchQueue.main.async { [weak self] in
                    self?.setLoading(false, false)
                }
            } error: { code in
                DispatchQueue.main.async { [weak self] in
                    if code != .none {
                        self?.setLoading(false, false)
                        self?.delegate?.didReceiveError(code, isBlocker: false)
                    }
                }
            }

        } else {
            JIM.shared().conversationManager.clearUnreadCount(by: conversationInfo.conversation) {
                DispatchQueue.main.async { [weak self] in
                    self?.setLoading(false, false)
                }
            } error: { code in
                DispatchQueue.main.async { [weak self] in
                    if code != .none {
                        self?.setLoading(false, false)
                        self?.delegate?.didReceiveError(code, isBlocker: false)
                    }
                }
            }
        }
    }
    
    // MARK: - Common
    
    /// This is used to check the loading status and control loading indicator.
    /// - Parameters:
    ///   - loadingState: Set to true when the list is loading.
    ///   - showIndicator: If true, the loading indicator is started, and if false, the indicator is stopped.
    private func setLoading(_ loadingState: Bool, _ showIndicator: Bool) {
        self.isLoading = loadingState
        
        self.delegate?.shouldUpdateLoadingState(showIndicator)
    }
    
    private func updateConversationInfoList(_ conversationInfoList: [JConversationInfo]) {
        conversationInfoList.forEach { conversationInfo in
            if conversationInfo.conversation.conversationType == .system
                && conversationInfo.conversation.conversationId == GlobalConst.friendConversationId {
                return
            }
            if let index = SBUUtils.findIndex(ofConversationInfo: conversationInfo, in: self.conversationInfoList) {
                self.conversationInfoList.remove(at: index)
            }
            self.conversationInfoList.insert(conversationInfo, at: 0)
        }
        var topConversationInfoList: [JConversationInfo] = []
        var notTopConversationInfoList: [JConversationInfo] = []
        for conversationInfo in self.conversationInfoList {
            if conversationInfo.isTop {
                topConversationInfoList.append(conversationInfo)
            } else {
                notTopConversationInfoList.append(conversationInfo)
            }
        }
        topConversationInfoList.sort {
            $0.topTime > $1.topTime
        }
        notTopConversationInfoList.sort {
            $0.sortTime > $1.sortTime
        }
        var result: [JConversationInfo] = []
        result.append(contentsOf: topConversationInfoList)
        result.append(contentsOf: notTopConversationInfoList)
        self.conversationInfoList = result
        
        self.delegate?.groupChannelListViewModel(self, didChangeChannelList: self.conversationInfoList, needsToReload: true)
    }
    
    private func deleteConversationInfoList(_ conversationInfoList: [JConversationInfo]) {
        conversationInfoList.forEach { conversationInfo in
            if let index = SBUUtils.findIndex(ofConversationInfo: conversationInfo, in: self.conversationInfoList) {
                self.conversationInfoList.remove(at: index)
            }
        }
        self.delegate?.groupChannelListViewModel(self, didChangeChannelList: self.conversationInfoList, needsToReload: true)
    }
}

extension SBUGroupChannelListViewModel : JConversationDelegate {
    public func conversationInfoDidAdd(_ conversationInfoList: [JConversationInfo]!) {
        updateConversationInfoList(conversationInfoList)
    }
    
    public func conversationInfoDidUpdate(_ conversationInfoList: [JConversationInfo]!) {
        updateConversationInfoList(conversationInfoList)
    }
    
    public func conversationInfoDidDelete(_ conversationInfoList: [JConversationInfo]!) {
        deleteConversationInfoList(conversationInfoList)
    }
    
    public func totalUnreadMessageCountDidUpdate(_ count: Int32) {
        
    }
}
