//
//  SBUImageContentView.swift
//  SendbirdUIKit
//
//  Created by Hoon Sung on 2021/03/18.
//  Copyright © 2021 Sendbird, Inc. All rights reserved.
//

import UIKit
import JetIM

open class SBUImageContentView: SBUBaseFileContentView {
    public var imageView: UIImageView = {
        let imageView = UIImageView()
        imageView.clipsToBounds = true
        imageView.contentMode = .scaleAspectFill
        return imageView
    }()
    
    public var iconImageView: UIImageView = {
        let imageView = UIImageView()
        imageView.clipsToBounds = true
        imageView.contentMode = .center
        return imageView
    }()
    
    public var widthConstraint: NSLayoutConstraint?
    public var heightConstraint: NSLayoutConstraint?
    
    // MARK: - Properties (Private)
    private var loadImageSession: URLSessionTask? {
        willSet {
            loadImageSession?.cancel()
        }
    }
    
    open override func setupViews() {
        self.layer.cornerRadius = 12
        self.layer.borderColor = UIColor.clear.cgColor
        self.layer.borderWidth = 1
        self.clipsToBounds = true
        
        self.addSubview(self.imageView)
        self.addSubview(self.iconImageView)
    }
    
    open override func setupLayouts() {
        self.imageView.sbu_constraint(
            equalTo: self,
            left: 0,
            right: 0,
            top: 0,
            bottom: 0,
            priority: .defaultLow
        )
        
        self.setupSizeContraint()
        
        self.iconImageView
            .sbu_constraint(equalTo: self, centerX: 0, centerY: 0)
            .sbu_constraint(width: 48, height: 48)
        self.iconImageView.layoutIfNeeded()
    }
    
    open func setupSizeContraint() {
        self.widthConstraint?.isActive = false
        self.heightConstraint?.isActive = false
        
        self.widthConstraint = self.imageView.widthAnchor.constraint(
            equalToConstant: SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize.width
        )
        self.heightConstraint = self.imageView.heightAnchor.constraint(
            equalToConstant: SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize.height
        )

        self.widthConstraint?.isActive = true
        self.heightConstraint?.isActive = true
    }
    
    open override func configure(message: JMessage, position: MessagePosition) {
        if self.message?.messageId != message.messageId ||
            self.message?.timestamp != message.timestamp {
            self.imageView.image = nil
        }
        
        super.configure(message: message, position: position)

        let imageOption: UIImageView.ImageOption
        let urlString: String
        let thumbnailSize: CGSize
        
        if let imageMessage = message.content as? JImageMessage {
            imageOption = .imageToThumbnail
            urlString = imageMessage.thumbnailUrl
            if (imageMessage.width > 0 && imageMessage.height > 0) {
                thumbnailSize = CGSize(width: Int(imageMessage.width), height: Int(imageMessage.height))
            } else {
                thumbnailSize = SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize
            }
        } else if let videoMessage = message.content as? JVideoMessage {
            imageOption = .videoURLToImage
            urlString = videoMessage.snapshotUrl
            thumbnailSize = SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize
        } else {
            imageOption = .imageToThumbnail
            urlString = ""
            thumbnailSize = SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize
        }
        
        self.resizeImageView(by: thumbnailSize)
        self.loadImageSession = self.imageView.loadImage(
            urlString: urlString,
            option: imageOption,
            thumbnailSize: thumbnailSize
        ) { [weak self] _ in
            DispatchQueue.main.async {
                self?.setFileIcon()
            }
        }
        self.setFileIcon()
    }
    
    public func setImage(_ image: UIImage?, size: CGSize? = nil) {
        if let size = size {
            self.resizeImageView(by: size)
        }

        self.imageView.image = image
        self.setFileIcon()
    }
    
    public func setFileIcon() {
        self.iconImageView.backgroundColor = nil
        if self.imageView.image == nil {
            self.iconImageView.image = SBUIconSetType.iconPhoto.image(
                with: theme.fileMessagePlaceholderColor,
                to: SBUIconSetType.Metric.defaultIconSizeVeryLarge
            )
        } else {
            self.iconImageView.image = nil
        }
    }
    
    open func resizeImageView(by size: CGSize) {
        self.widthConstraint?.constant = min(
            size.width,
            SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize.width
        )
        self.heightConstraint?.constant = min(
            size.height,
            SBUGlobals.messageCellConfiguration.groupChannel.thumbnailSize.height
        )
    }
}
