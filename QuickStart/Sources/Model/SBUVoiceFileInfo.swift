//
//  SBUVoiceFileInfo.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 2023/02/08.
//  Copyright © 2023 Sendbird, Inc. All rights reserved.
//

import UIKit
import JetIM

/// This is a structure that has the essential information of a voice message such as file name, file path, play time and so on.
public class SBUVoiceFileInfo: NSObject {
    // for send message
    var fileName: String?
    var filePath: URL?
    
    // for play
    var playtime: Double?
    var currentPlayTime: Double = 0 // ms
    var isPlaying: Bool = false
    
    init(fileName: String? = nil,
         filePath: URL? = nil,
         playtime: Double? = nil,
         currentPlayTime: Double = 0) {
        super.init()
        
        self.fileName = fileName
        self.filePath = filePath
        self.playtime = playtime
        self.currentPlayTime = currentPlayTime
    }
    
    /// Creates voice file info with `JMessage` object
    /// - Parameter message: `JMessage` object
    /// - Returns: ``SBUVoiceFileInfo`` obejct; If the `message` is not the *voice* message, it returns `nil`
    public static func createVoiceFileInfo(with message: JMessage) -> SBUVoiceFileInfo? {
        if let voiceMessage = message.content as? JVoiceMessage {
            let time: Double = Double(voiceMessage.duration)
            return SBUVoiceFileInfo(
                fileName: SBUStringSet.VoiceMessage.fileName,
                playtime: time
            )
        }
        
        return nil
    }
}
