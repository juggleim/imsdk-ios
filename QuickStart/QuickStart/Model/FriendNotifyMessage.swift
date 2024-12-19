//
//  FriendNotifyMessage.swift
//  QuickStart
//
//  Created by Fei Li on 2024/12/19.
//

import Foundation
import JuggleIM

class FriendNotifyMessage: JMessageContent {
    
    static let messageType = "jgd:friendntf"
    let typeString = "type"
    
    var type: Int = 0
    
    override class func contentType() -> String {
        return messageType
    }
    
    override class func flags() -> JMessageFlag {
        return .isSave
    }
    
    override func encode() -> Data {
        let content: [String: Any] = [typeString: type]
        let data = try! JSONSerialization.data(withJSONObject: content)
        return data
    }
    
    override func decode(_ data: Data) {
        guard let json = try! JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            return
        }
        if let type = json[typeString] as? Int {
            self.type = type
        }
    }
    
    override func conversationDigest() -> String {
        return "好友通知"
    }
}
