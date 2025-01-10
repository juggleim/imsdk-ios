//
//  SBUGroupChannelViewController.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 03/02/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//

import UIKit
import Photos
import AVKit
import SafariServices
import PhotosUI
import MobileCoreServices
import JuggleIM

@objcMembers
open class SBUGroupChannelViewController: SBUBaseChannelViewController, SBUGroupChannelViewModelDelegate, SBUGroupChannelModuleHeaderDelegate, SBUGroupChannelModuleListDelegate, SBUGroupChannelModuleListDataSource, SBUGroupChannelModuleInputDelegate, SBUGroupChannelModuleInputDataSource, SBUGroupChannelViewModelDataSource, SBUMentionManagerDataSource, SBUVoiceMessageInputViewDelegate {

    // MARK: - UI properties (Public)
    public var headerComponent: SBUGroupChannelModule.Header? {
        get { self.baseHeaderComponent as? SBUGroupChannelModule.Header }
        set { self.baseHeaderComponent = newValue }
    }
    public var listComponent: SBUGroupChannelModule.List? {
        get { self.baseListComponent as? SBUGroupChannelModule.List }
        set { self.baseListComponent = newValue }
    }
    public var inputComponent: SBUGroupChannelModule.Input? {
        get { self.baseInputComponent as? SBUGroupChannelModule.Input }
        set { self.baseInputComponent = newValue }
    }
    
    public var voiceMessageInputView = SBUVoiceMessageInputView()
    
    public var highlightInfo: SBUHighlightMessageInfo?
    
    // MARK: - Logic properties (Public)
    public var viewModel: SBUGroupChannelViewModel? {
        get { self.baseViewModel as? SBUGroupChannelViewModel }
        set { self.baseViewModel = newValue }
    }
    
    public var conversationInfo: JConversationInfo? { self.viewModel?.conversationInfo }
    
    public private(set) var newMessagesCount: Int = 0
    
    override var isDisableChatInputState: Bool {
        didSet {
            (self.baseInputComponent?.messageInputView as? SBUMessageInputView)?.setDisableChatInputState(isDisableChatInputState)
        }
    }
    
    /// An error handler that is called when any one of the files size in multiple files message exceeds the file size limit.
    /// If needed, override this handler to show your custom alert view.
    /// - Since: 3.10.0
    open func multipleFilesMessageFileSizeErrorHandler(_ message: String) {
        SBULog.error("Did receive error: \(message)")
        
        DispatchQueue.main.async {
            SBUAlertView.show(
                title: message,
                confirmButtonItem: SBUAlertButtonItem(
                    title: SBUStringSet.OK,
                    completionHandler: { _ in
                    }
                ), cancelButtonItem: nil
            )
        }
    }
    
    // MARK: - Logic properties (Private)
    
    // MARK: - Lifecycle
    
    /// If you have channel object, use this initialize function. And, if you have own message list params, please set it. If not set, it is used as the default value.
    ///
    /// See the example below for params generation.
    /// ```
    ///     let params = MessageListParams()
    ///     params.includeMetaArray = true
    ///     params.includeReactions = true
    ///     params.includeThreadInfo = true
    ///     ...
    /// ```
    /// - note: The `reverse` and the `previousResultSize` properties in the `MessageListParams` are set in the UIKit. Even though you set that property it will be ignored.
    /// - Parameter channel: Channel object
    /// - Since: 1.0.11
    required public override init(conversationInfo: JConversationInfo) {
        super.init(conversationInfo: conversationInfo)
        
        self.headerComponent = SBUModuleSet.GroupChannelModule.HeaderComponent.init()
        self.listComponent = SBUModuleSet.GroupChannelModule.ListComponent.init()
        self.inputComponent = SBUModuleSet.GroupChannelModule.InputComponent.init()
    }
    
    open override var preferredStatusBarStyle: UIStatusBarStyle {
        theme.statusBarStyle
    }
    
    open override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
    }
    
    open override func viewDidLoad() {
        super.viewDidLoad()
        self.navigationController?.interactivePopGestureRecognizer?.delegate = nil
        self.updateChannelTitle()
        self.inputComponent?.updateMessageInputModeState()
        self.updateVoiceMessageInputMode()
    }
    open override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        
        self.listComponent?.pauseAllVoicePlayer()
    }
    
    open override func applicationWillResignActivity() {
        self.resetVoiceMessageInput(for: true)
        self.listComponent?.pauseAllVoicePlayer()
    }
    
    open override func willPresentSubview() {
        // TODO: Voice Message - Unify policy
        self.listComponent?.pauseAllVoicePlayer()
    }
    
    open override func onClickBack() {
        if let conversation = self.conversationInfo?.conversation,
           let textView = (self.inputComponent?.messageInputView as? SBUMessageInputView)?.textView,
           let draft = textView.text {
            if draft.count > 0 {
                JIM.shared().conversationManager.setDraft(draft, in: conversation)
            } else {
                JIM.shared().conversationManager.clearDraft(in: conversation)
            }
        }
        super.onClickBack()
    }
    
    deinit {
        SBULog.info("")
        
        // Clear typing message when exiting the channel.
//        self.viewModel?.clearTypingMessage()
    }
    
    // MARK: - ViewModel
    open override func createViewModel(conversationInfo: JConversationInfo? = nil) {
        guard conversationInfo != nil else {
            SBULog.error("conversationInfo parameter must be set.")
            return
        }
        
        self.baseViewModel = SBUGroupChannelViewModel(
            conversationInfo: conversationInfo,
            delegate: self,
            dataSource: self
        )
        
        if let messageInputView = self.baseInputComponent?.messageInputView as? SBUMessageInputView {
            messageInputView.setMode(.none)
        }
    }
    
    // MARK: - Sendbird UIKit Life cycle
    open override func setupViews() {
        super.setupViews()
        
        self.headerComponent?
            .configure(delegate: self, theme: self.theme)
        self.listComponent?
            .configure(delegate: self, dataSource: self, theme: self.theme)
        self.inputComponent?
            .configure(delegate: self, dataSource: self, mentionManagerDataSource: self, theme: self.theme)
    }
    
    open override func setupLayouts() {
        super.setupLayouts()

        if let listComponent = listComponent {
            listComponent.translatesAutoresizingMaskIntoConstraints = false
            
            tableViewTopConstraint?.isActive = false
            tableViewBottomConstraint?.isActive = false
            tableViewLeftConstraint?.isActive = false
            tableViewRightConstraint?.isActive = false
            
            self.tableViewTopConstraint = listComponent.topAnchor.constraint(
                equalTo: self.view.topAnchor,
                constant: 0
            )
            self.tableViewBottomConstraint = listComponent.bottomAnchor.constraint(
                equalTo: self.inputComponent?.topAnchor ?? self.view.bottomAnchor,
                constant: 0
            )
            self.tableViewLeftConstraint = listComponent.leftAnchor.constraint(
                equalTo: self.view.leftAnchor, constant: 0
            )
            self.tableViewRightConstraint = listComponent.rightAnchor.constraint(
                equalTo: self.view.rightAnchor, constant: 0
            )
            
            tableViewTopConstraint?.isActive = true
            tableViewBottomConstraint?.isActive = true
            tableViewLeftConstraint?.isActive = true
            tableViewRightConstraint?.isActive = true
        }
        
        if let inputComponent = self.inputComponent {
            inputComponent.translatesAutoresizingMaskIntoConstraints = false
            
            messageInputViewTopConstraint?.isActive = false
            messageInputViewBottomConstraint?.isActive = false
            messageInputViewLeftConstraint?.isActive = false
            messageInputViewRightConstraint?.isActive = false
            
            self.messageInputViewTopConstraint = inputComponent.topAnchor.constraint(
                equalTo: self.listComponent?.bottomAnchor ?? self.view.bottomAnchor,
                constant: 0
            )
            self.messageInputViewBottomConstraint = inputComponent.bottomAnchor.constraint(
                equalTo: self.view.bottomAnchor,
                constant: 0
            )
            self.messageInputViewLeftConstraint = inputComponent.leftAnchor.constraint(
                equalTo: self.view.leftAnchor,
                constant: 0
            )
            self.messageInputViewRightConstraint = inputComponent.rightAnchor.constraint(
                equalTo: self.view.rightAnchor,
                constant: 0
            )
            
            messageInputViewTopConstraint?.isActive = true
            messageInputViewBottomConstraint?.isActive = true
            messageInputViewLeftConstraint?.isActive = true
            messageInputViewRightConstraint?.isActive = true
        }
    }
    
    open override func setupStyles() {
        super.setupStyles()
    }
    
    open override func updateStyles(needsToLayout: Bool) {
        self.setupStyles()
        super.updateStyles()
        
        self.headerComponent?.updateStyles(theme: self.theme)
        self.listComponent?.updateStyles(theme: self.theme)
        
        self.listComponent?.reloadTableView(needsToLayout: needsToLayout)
    }
    
    open override func updateStyles() {
        self.updateStyles(needsToLayout: true)
    }

    // MARK: - New message count

    /// This function increases the new message count.
    @discardableResult
    public override func increaseNewMessageCount() -> Bool {
        guard let viewModel = viewModel else { return false }
        guard !baseChannelViewModel(viewModel, isScrollNearBottomInChannel: viewModel.conversationInfo) else { return false }
        
        guard super.increaseNewMessageCount() else { return false }
        
        self.updateNewMessageInfo(hidden: false)
        self.newMessagesCount += 1
        
        if let newMessageInfoView = self.listComponent?.newMessageInfoView as? SBUNewMessageInfo {
            newMessageInfoView.updateCount(count: self.newMessagesCount) { [weak self] in
                guard let self = self else { return }
                guard let listComponent = self.listComponent else { return }
                
                let position = SBUGlobals.scrollPostionConfiguration.groupChannel.scrollToNewMessage
                let options = SBUScrollOptions(
                    count: self.newMessagesCount,
                    position: position,
                    isInverted: listComponent.tableView.isInverted
                )
                
                self.baseChannelModule(
                    listComponent,
                    didSelectScrollToBottonWithOptions: options,
                    animated: true
                )
            }
        }
        return true
    }
    
    // MARK: - Message: Menu
    
    /// Calculates the `CGPoint` value that indicates where to draw the message menu in the group channel screen.
    /// - Parameters:
    ///   - indexPath: IndexPath
    ///   - position: Message position
    /// - Returns: `CGPoint` value
    /// - Since: 1.2.5
    @available(*, deprecated, message: "Please use `calculateMessageMenuCGPoint(indexPath:position:)` in `SBUGroupChannelModule.List`") // 3.1.2
    public func calculatorMenuPoint(
        indexPath: IndexPath,
        position: MessagePosition
    ) -> CGPoint {
        guard let listComponent = listComponent else {
            SBULog.error("listComponent is not set up.")
            return .zero
        }
        
        return listComponent.calculateMessageMenuCGPoint(indexPath: indexPath, position: position)
    }
    
    @available(*, deprecated, message: "Please use `showMessageContextMenu(message:cell:forRowAt:)` in `SBUGroupChannelModule.List`") // 3.1.2
    open override func showMenuModal(_ cell: UITableViewCell, indexPath: IndexPath, message: JMessage) {
        self.listComponent?.showMessageContextMenu(for: message, cell: cell, forRowAt: indexPath)
    }
    
    @available(*, deprecated, message: "Please use `showMessageContextMenu(message:cell:forRowAt:)` in `SBUGroupChannelModule.List`") // 3.1.2
    public override func showMenuModal(_ cell: UITableViewCell,
                                       indexPath: IndexPath,
                                       message: JMessage,
                                       types: [MessageMenuItem]?) {
        self.listComponent?.showMessageContextMenu(for: message, cell: cell, forRowAt: indexPath)
    }
    
    open override func showChannelSettings() {
        guard let conversationInfo = self.conversationInfo else { return }
        
        if conversationInfo.conversation.conversationType == .private {
            let channelSettingsVC = SBUViewControllerSet.GroupChannelSettingsViewController.init(conversationInfo: conversationInfo)
            self.navigationController?.pushViewController(channelSettingsVC, animated: true)
        } else if conversationInfo.conversation.conversationType == .group {
            let groupSettingVC = GroupSettingViewController()
            groupSettingVC.conversationInfo = conversationInfo
            self.navigationController?.pushViewController(groupSettingVC, animated: true)
        }
    }
    
    open override func showMessageThread(
        channelURL: String,
        parentMessageId: Int64,
        parentMessageCreatedAt: Int64? = 0,
        startingPoint: Int64? = 0
    ) {
        if (parentMessageCreatedAt ?? 0) < (self.conversationInfo?.sortTime ?? 0) {
            SBULog.warning(SBUStringSet.Message_Reply_Cannot_Found_Original)
            return
        }
        
//        var parentMessage: JMessage?
//        if let fullMessageList = self.viewModel?.fullMessageList {
//            parentMessage = fullMessageList.first(where: { $0.clientMsgNo == parentMessageId })
//        }
           
//        let messageThreadVC = SBUViewControllerSet.MessageThreadViewController.init(
//            channelURL: channelURL,
//            parentMessage: parentMessage,
//            parentMessageId: parentMessageId,
//            delegate: self,
//            startingPoint: startingPoint,
//            voiceFileInfos: self.listComponent?.voiceFileInfos
//        )
//        self.navigationController?.pushViewController(messageThreadVC, animated: true)
    }
    
    // MARK: - PHPickerViewControllerDelegate
    
    open override func imagePickerControllerDidCancel(_ picker: UIImagePickerController) {
        super.imagePickerControllerDidCancel(picker)
        
        if let messageInputView = self.baseInputComponent?.messageInputView as? SBUMessageInputView {
            messageInputView.setMode(.none)
        }
    }
    
    @available(iOS 14, *)
    override open func picker(_ picker: PHPickerViewController, didFinishPicking results: [PHPickerResult]) {
        picker.dismiss(animated: true)
        
//        guard results.count <= SBUAvailable.multipleFilesMessageFileCountLimit else {
//            self.errorHandler("Up to \(SBUAvailable.multipleFilesMessageFileCountLimit) can be attached.")
//            return
//        }
        
        // Picked multiple files
        if results.count > 1 {
            checkAllFileSizes(results) { [weak self] isValid in
                if isValid {
                    self?.handleMultipleFiles(results)
                } else {
                    self?.multipleFilesMessageFileSizeErrorHandler(SBUStringSet.FileUpload.Error.exceededSizeLimit)
                }
            }
            return
        }
        
        // Picked a single file
        results.forEach {
            let itemProvider = $0.itemProvider
            
            /// !! Warining !!
            /// Since the image identifier includes the gif identifier, the check of the gif type should take precedence over the image type comparison.
            
            // GIF
            if itemProvider.hasItemConformingToTypeIdentifier(UTType.gif.identifier) {
                if let inputComponent = self.baseInputComponent {
                    inputComponent.pickGIFFile(itemProvider: itemProvider)
                    return
                }
            }
            
            // image
            else if itemProvider.hasItemConformingToTypeIdentifier(UTType.image.identifier) {
                if let inputComponent = self.baseInputComponent {
                    inputComponent.pickImageFile(itemProvider: itemProvider)
                    return
                }
            }
            
            // video
            else if itemProvider.hasItemConformingToTypeIdentifier(UTType.movie.identifier) {
                if let inputComponent = self.baseInputComponent {
                    inputComponent.pickVideoFile(itemProvider: itemProvider)
                    return
                }
            }
        }
    }
    
    @available(iOS 14, *)
    private func handleMultipleFiles(_ results: [PHPickerResult]) {
        guard let inputComponent = self.baseInputComponent as? SBUGroupChannelModule.Input else {
            return
        }
        
        // Group picked files depending on file type.
        let (imageAndGIFs, videos) = self.groupFilesByMimeType(results)
        
        // Handle images+GIFs.
        if imageAndGIFs.count > 0 {
            
            // multiple (image + gif) -> send a multipleFilesMessage
            if imageAndGIFs.count > 1 {
                inputComponent.pickMultipleImageFiles(itemProviders: imageAndGIFs)
            }
            
            // single image / gif -> send a JMessage
            else if imageAndGIFs.count == 1 {
                let itemProvider = imageAndGIFs.first!
                
                // GIF
                if itemProvider.hasItemConformingToTypeIdentifier(UTType.gif.identifier) {
                    if let inputComponent = self.baseInputComponent {
                        inputComponent.pickGIFFile(itemProvider: itemProvider)
                    }
                }
                
                // image
                else if itemProvider.hasItemConformingToTypeIdentifier(UTType.image.identifier) {
                    if let inputComponent = self.baseInputComponent {
                        inputComponent.pickImageFile(itemProvider: itemProvider)
                    }
                }
            }
        }
        
        // Handle videos.
        if videos.count > 0 {
            // video(s) selected -> send N JMessages
            videos.forEach { itemProvider in
                inputComponent.pickVideoFile(itemProvider: itemProvider)
            }
        }
    }
    
    // MARK: PHPicker Util Methods.
    /// Presents `UIImagePickerController`. If `SBUGlobals.UsingPHPicker`is `true`, it presents `PHPickerViewController` in iOS 14 or later.
    /// - NOTE: If you want to use customized `PHPickerConfiguration`, please override this method.
    /// - Since: 3.10.0
    open override func showPhotoLibraryPicker() {
        if #available(iOS 14, *), SBUGlobals.isPHPickerEnabled {
            var pickerFilter: [PHPickerFilter] = []
            pickerFilter += [.images]
            pickerFilter += [.videos]

            var configuration = PHPickerConfiguration()
            configuration.filter = .any(of: pickerFilter)

            let picker = PHPickerViewController(configuration: configuration)
            picker.delegate = self
            self.present(picker, animated: true, completion: nil)
            return
        }

        let sourceType: UIImagePickerController.SourceType = .photoLibrary
        var mediaType: [String] = []
        mediaType += [String(kUTTypeImage), String(kUTTypeGIF)]
        mediaType += [String(kUTTypeMovie)]

        if UIImagePickerController.isSourceTypeAvailable(sourceType) {
            let imagePickerController = UIImagePickerController()
            imagePickerController.delegate = self
            imagePickerController.sourceType = sourceType
            imagePickerController.mediaTypes = mediaType
            self.present(imagePickerController, animated: true, completion: nil)
        }
    }

    @available(iOS 14, *)
    private func checkAllFileSizes(_ results: [PHPickerResult], completion: @escaping (Bool) -> Void) {
        var areAllFileSizesValid = true
        
        let group = DispatchGroup()
        for result in results {
            if !areAllFileSizesValid {
                group.leave()
                completion(false)
                return
            }
            
            group.enter()
            
            let itemProvider = result.itemProvider
            
            if itemProvider.hasItemConformingToTypeIdentifier(UTType.gif.identifier) {
                self.checkGifSize(itemProvider: itemProvider) { isValid in
                    if !isValid { areAllFileSizesValid = false }
                    group.leave()
                }
                
            } else if itemProvider.hasItemConformingToTypeIdentifier(UTType.image.identifier) {
                self.checkImageSize(itemProvider: itemProvider) { isValid in
                    if !isValid { areAllFileSizesValid = false }
                    group.leave()
                }
                
            } else {
                self.checkVideoSize(itemProvider: itemProvider) { isValid in
                    if !isValid { areAllFileSizesValid = false }
                    group.leave()
                }
            }
        }
        
        group.notify(queue: .main) {
            completion(areAllFileSizesValid)
        }
    }
    
    @available(iOS 14, *)
    private func checkImageSize(itemProvider: NSItemProvider, completion: @escaping (Bool) -> Void) {
        itemProvider.loadItem(forTypeIdentifier: UTType.image.identifier, options: [:]) { (item, _) in
            if let url = item as? URL {
                completion(url.isFileSizeUploadable)
            } else {
                completion(false)
            }
        }
    }
    
    @available(iOS 14, *)
    private func checkGifSize(itemProvider: NSItemProvider, completion: @escaping (Bool) -> Void) {
        itemProvider.loadFileRepresentation(forTypeIdentifier: UTType.gif.identifier) { item, _ in
            if let gifURL = item {
                completion(gifURL.isFileSizeUploadable)
            } else {
                completion(false)
            }
        }
    }

    @available(iOS 14, *)
    private func checkVideoSize(itemProvider: NSItemProvider, completion: @escaping (Bool) -> Void) {
        itemProvider.loadFileRepresentation(forTypeIdentifier: UTType.movie.identifier) { item, error in
            if let videoURL = item {
                completion(videoURL.isFileSizeUploadable)
            } else {
                if let error = error {
                    SBULog.error("Failed to read video file. \(error)")
                }
                completion(false)
            }
        }
    }
    
    /// Groups picked files by file type.
    /// - Returns a tuple - (an array of images + GIFs, an array of videos)
    @available(iOS 14, *)
    private func groupFilesByMimeType(_ results: [PHPickerResult]) -> ([NSItemProvider], [NSItemProvider]) {
        var imageAndGIFs = [NSItemProvider]()
        var videos = [NSItemProvider]()
        
        results.forEach {
            let itemProvider = $0.itemProvider
                
            if itemProvider.hasItemConformingToTypeIdentifier(UTType.gif.identifier) ||
                itemProvider.hasItemConformingToTypeIdentifier(UTType.image.identifier) {
                imageAndGIFs.append(itemProvider)
            }
            
            // Group videos
            else if itemProvider.hasItemConformingToTypeIdentifier(UTType.movie.identifier) {
                videos.append(itemProvider)
            }
        }
        
        return (imageAndGIFs, videos)
    }
    
    // MARK: - VoiceMessageInput
    open override func showVoiceMessageInput() {
        super.showVoiceMessageInput()
        
        let canvasView = self.navigationController?.view ?? self.view
        
        self.voiceMessageInputView.show(delegate: self, canvasView: canvasView)
        self.navigationController?.interactivePopGestureRecognizer?.isEnabled = false
    }
    
    open override func dismissVoiceMessageInput() {
        super.dismissVoiceMessageInput()
        
        self.voiceMessageInputView.dismiss()
        self.navigationController?.interactivePopGestureRecognizer?.isEnabled = true
    }
    
    func updateVoiceMessageInputMode() {
//        let showAlertCompletionHandler: ((String) -> Void) = { title in
//            self.resetVoiceMessageInput()
//            
//            SBUAlertView.show(
//                title: title,
//                confirmButtonItem: SBUAlertButtonItem(
//                    title: SBUStringSet.OK,
//                    completionHandler: { _ in
//                        self.dismissVoiceMessageInput()
//                    }
//                ), cancelButtonItem: nil
//            ) {
//                self.dismissVoiceMessageInput()
//            }
//        }
        
//        var title = ""
        
        // Frozen
//        let isOperator = self.channel?.myRole == .operator
//        let isBroadcast = self.channel?.isBroadcast ?? false
//        let isFrozen = self.channel?.isFrozen ?? false
//        if !isBroadcast, !isOperator && isFrozen, self.voiceMessageInputView.isShowing {
//            title = SBUStringSet.VoiceMessage.Alert.frozen
//        }
//
//        let isMuted = self.channel?.myMutedState == .muted
//        if (!isFrozen || (isFrozen && isOperator)), isMuted, self.voiceMessageInputView.isShowing {
//            title = SBUStringSet.VoiceMessage.Alert.muted
//        }
//
//        if title.count > 0 {
//            showAlertCompletionHandler(title)
//        }
    }
    
    open override func resetVoiceMessageInput(for resignActivity: Bool = false) {
        super.resetVoiceMessageInput(for: resignActivity)
        
        self.voiceMessageInputView.reset(for: resignActivity)
    }
    
    // MARK: - SBUGroupChannelViewModelDelegate
    open override func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        didChangeChannel channel: JConversationInfo?
    ) {
        guard channel != nil else {
            // channel deleted
            if self.navigationController?.viewControllers.last == self {
                // If leave is called in the ChannelSettingsViewController, this logic needs to be prevented.
                self.onClickBack()
            }
            return
        }
    }
    
    open override func baseChannelViewModel(
        _ viewModel: SBUBaseChannelViewModel,
        deletedMessages messages: [JMessage]
    ) {
//        for message in messages {
//            self.listComponent?.pauseVoicePlayer(cacheKey: message.cacheKey)
//        }
    }
    
    open func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didReceiveSuggestedMentions members: [SBUUser]?) {
        let members = members ?? []
        self.inputComponent?.handlePendingMentionSuggestion(with: members)
    }
    
    public func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didFinishUploadingFileAt index: Int,
        multipleFilesMessageRequestId requestId: String
    ) {
        self.baseListComponent?.reloadMultipleFilesMessageCollectionViewCell(
            requestId: requestId,
            index: index
        )
    }
    
    open func groupChannelViewModel(
        _ viewModel: SBUGroupChannelViewModel,
        didReceiveStreamMessage message: JMessage,
        forChannel channel: JConversationInfo
    ) {
        self.listComponent?.updateStreamMessage(message)
    }
    
    // MARK: - SBUGroupChannelModuleHeaderDelegate
    open override func baseChannelModule(_ headerComponent: SBUBaseChannelModule.Header, didTapLeftItem leftItem: UIBarButtonItem) {
        self.onClickBack()
    }
    
    open override func baseChannelModule(_ headerComponent: SBUBaseChannelModule.Header, didTapRightItem rightItem: UIBarButtonItem) {
        super.baseChannelModule(headerComponent, didTapRightItem: rightItem)
        
        self.showChannelSettings()
    }
    
    // MARK: - SBUGroupChannelModuleListDelegate
    open func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapEmoji emojiKey: String, messageCell: SBUBaseMessageCell) {
//        guard let currentUser = SBUGlobals.currentUser,
//              let message = messageCell.message else { return }
        
//        let shouldSelect = message.reactions.first { $0.key == emojiKey }?
//            .userIds.contains(currentUser.userId) == false
//        self.viewModel?.setReaction(message: message, emojiKey: emojiKey, didSelect: shouldSelect)
    }
    
    open func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didLongTapEmoji emojiKey: String, messageCell: SBUBaseMessageCell) {
//        guard let conversationInfo = self.conversationInfo,
//              let message = messageCell.message else { return }
        
//        let reaction = message.reactions.first { $0.key == emojiKey }
//        let reactionsVC = SBUReactionsViewController(
//            channel: channel,
//            message: message,
//            selectedReaction: reaction
//        )
//        reactionsVC.delegate = self
//        reactionsVC.modalPresentationStyle = UIModalPresentationStyle.custom
//        reactionsVC.transitioningDelegate = self
//        self.present(reactionsVC, animated: true)
    }
    
    open func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapMoreEmojiForCell messageCell: SBUBaseMessageCell) {
        self.dismissKeyboard()
        
        guard let message = messageCell.message else { return }
        self.showEmojiListModal(message: message)
    }
    
    open func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, didTapMentionUser user: SBUUser) {
        self.showUserProfile(user: user)
    }
    
    open override func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didTapVoiceMessage mediaMessage: JMessage, cell: UITableViewCell, forRowAt indexPath: IndexPath) {
        super.baseChannelModule(listComponent, didTapVoiceMessage: mediaMessage, cell: cell, forRowAt: indexPath)
        
        if let cell = cell as? SBUBaseMessageCell {
            self.listComponent?.updateVoiceMessage(cell, message: mediaMessage, indexPath: indexPath)
        }
    }
    
    open override func baseChannelModuleDidTapScrollToButton(_ listComponent: SBUBaseChannelModule.List, animated: Bool) {
        guard self.baseViewModel?.fullMessageList.isEmpty == false else { return }
        self.newMessagesCount = 0
        
        super.baseChannelModuleDidTapScrollToButton(listComponent, animated: animated)
    }
    
    open override func baseChannelModule(
        _ listComponent: SBUBaseChannelModule.List,
        didSelectScrollToBottonWithOptions options: SBUScrollOptions,
        animated: Bool
    ) {
        guard self.baseViewModel?.fullMessageList.isEmpty == false else { return }
        self.newMessagesCount = 0
        
        super.baseChannelModule(
            listComponent,
            didSelectScrollToBottonWithOptions: options,
            animated: animated
        )
    }
    
    open override func baseChannelModule(_ listComponent: SBUBaseChannelModule.List, didScroll scrollView: UIScrollView) {
        super.baseChannelModule(listComponent, didScroll: scrollView)
        
        self.lastSeenIndexPath = nil
        
        if listComponent.isScrollNearByBottom {
            self.newMessagesCount = 0
            self.updateNewMessageInfo(hidden: true)
        }
    }
    
    // MARK: - SBUGroupChannelModuleListDataSource
    open func groupChannelModule(_ listComponent: SBUGroupChannelModule.List, highlightInfoInTableView tableView: UITableView) -> SBUHighlightMessageInfo? {

        guard self.viewModel?.isInitialLoading == false else { return nil }
        
        return self.highlightInfo
    }

    // MARK: - SBUGroupChannelModuleInputDelegate
    open override func baseChannelModule(_ inputComponent: SBUBaseChannelModule.Input, didUpdateFrozenState isFrozen: Bool) {
        self.listComponent?.channelStateBanner?.isHidden = !isFrozen
    }
    
    open func groupChannelModule(_ inputComponent: SBUGroupChannelModule.Input, didPickImageURL imageUrl: URL?) {
        self.viewModel?.sendImageMessage(url: imageUrl)
    }
    
    open func groupChannelModule(_ inputComponent: SBUGroupChannelModule.Input, didPickImage image: UIImage) {
        self.viewModel?.sendImageMessage(image: image)
    }
    
    open func groupChannelModule(_ inputComponent: SBUGroupChannelModule.Input, didPickFileData fileData: Data?, fileName: String, mimeType: String, parentMessage: JMessage?) {
        
        if let messageInputView = self.baseInputComponent?.messageInputView as? SBUMessageInputView {
            messageInputView.setMode(.none)
        }
        
        self.viewModel?.sendMediaMessage(
            fileData: fileData,
            fileName: fileName,
            mimeType: mimeType,
            parentMessage: parentMessage
        )
    }
    
    open func groupChannelModule(
        _ inputComponent: SBUGroupChannelModule.Input,
        didTapSend text: String,
        mentionedMessageTemplate: String,
        mentionedUserIds: [String],
        parentMessage: JMessage?
    ) {
        self.viewModel?.sendTextMessage(
            text: text,
            mentionedMessageTemplate: mentionedMessageTemplate,
            mentionedUserIds: mentionedUserIds,
            parentMessage: parentMessage
        )
    }
    
    open func groupChannelModule(
        _ inputComponent: SBUGroupChannelModule.Input,
        didTapEdit text: String,
        mentionedMessageTemplate: String,
        mentionedUserIds: [String]
    ) {
        guard let message = self.baseViewModel?.inEditingMessage else { return }
//        self.viewModel?.updateUserMessage(
//            message: message,
//            text: text,
//            mentionedMessageTemplate: mentionedMessageTemplate,
//            mentionedUserIds: mentionedUserIds
//        )
    }
    
    open func groupChannelModule(
        _ inputComponent: SBUGroupChannelModule.Input,
        willChangeMode mode: SBUMessageInputMode,
        message: JMessage?,
        mentionedMessageTemplate: String,
        mentionedUserIds: [String]
    ) { }
    
    open func groupChannelModule(_ inputComponent: SBUGroupChannelModule.Input, shouldLoadSuggestedMentions filterText: String) {
        self.viewModel?.loadSuggestedMentions(with: filterText)
    }
    
    open func groupChannelModuleShouldStopSuggestingMention(_ inputComponent: SBUGroupChannelModule.Input) {
        self.viewModel?.cancelLoadingSuggestedMentions()
    }
    
    open func groupChannelModuleDidTapVoiceMessage(_ inputComponent: SBUGroupChannelModule.Input) {
        SBUPermissionManager.shared.requestRecordAcess { [weak self] in
            guard let self = self else { return }
            self.showVoiceMessageInput()
        } onDenied: {
            SBULog.info("Record permission was denied")
            self.showPermissionAlert(forType: .record)
            return
        }
    }
    
    open override func baseChannelModuleDidStartTyping(_ inputComponent: SBUBaseChannelModule.Input) {
//        self.viewModel?.startTypingMessage()
    }
    
    open override func baseChannelModuleDidEndTyping(_ inputComponent: SBUBaseChannelModule.Input) {
//        self.viewModel?.endTypingMessage()
//        self.inputComponent?.dismissSuggestedMentionList()
    }
    
    // MARK: - SBUGroupChannelViewModelDataSource
    open func groupChannelViewModel(_ viewModel: SBUGroupChannelViewModel,
                                    startingPointIndexPathsForChannel channel: JConversationInfo?) -> [IndexPath]? {
        return self.listComponent?.tableView.indexPathsForVisibleRows
    }
    
    // MARK: - SBUMentionManagerDataSource
    open func mentionManager(_ manager: SBUMentionManager, suggestedMentionUsersWith filterText: String) -> [SBUUser] {
        return self.viewModel?.suggestedMemberList ?? []
    }
        
    // MARK: - SBUVoiceMessageInputViewDelegate
    open func voiceMessageInputViewDidTapCacel(_ inputView: SBUVoiceMessageInputView) {
        self.dismissVoiceMessageInput()
    }
    
    open func voiceMessageInputView(
        _ inputView: SBUVoiceMessageInputView,
        willStartToRecord voiceFileInfo: SBUVoiceFileInfo
    ) {
        self.willPresentSubview()
    }
    
    open func voiceMessageInputView(
        _ inputView: SBUVoiceMessageInputView,
        didTapSend voiceFileInfo: SBUVoiceFileInfo
    ) {
        var parentMessage: JMessage?
        if let messageInputView = self.baseInputComponent?.messageInputView as? SBUMessageInputView {
            switch messageInputView.option {
            case .quoteReply(let message):
                parentMessage = message
            default:
                break
            }
            messageInputView.setMode(.none)
        }
        
        self.dismissVoiceMessageInput()
        self.viewModel?.sendVoiceMessage(voiceFileInfo: voiceFileInfo, parentMessage: parentMessage)
    }
}
