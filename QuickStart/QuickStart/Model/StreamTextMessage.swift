//
//  StreamTextMessage.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/26.
//

import Foundation

class StreamTextMessage: JMessageContent {
    var content: String?
    var streamId: String?
    
    override class func contentType() -> String {
        return "jgs:text"
    }
    
    override class func flags() -> JMessageFlag {
        return .isStatus
    }
    
    override func decode(_ data: Data) {
        guard let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            return
        }
        if let content = json["content"] as? String {
            self.content = content
        }
        if let streamId = json["stream_msg_id"] as? String {
            self.streamId = streamId
        }
    }
}
