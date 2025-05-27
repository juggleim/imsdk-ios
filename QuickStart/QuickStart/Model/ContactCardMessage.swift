//
//  ContactCardMessage.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation
import JuggleIM

class ContactCardMessage: JMessageContent {
    static let messageType = "jgd:contactcard"
    let userIdString = "user_id"
    let nameString = "name"
    let portraitString = "portrait"
    
    var userId: String = ""
    var name: String = ""
    var portrait: String = ""
    
    init(userInfo: JUserInfo) {
        self.userId = userInfo.userId
        self.name = userInfo.userName ?? ""
        self.portrait = userInfo.portrait ?? ""
    }
    
    override init() {
        super.init()
    }
    
    override class func contentType() -> String {
        return messageType
    }
    
    override class func flags() -> JMessageFlag {
        return [.isCountable, .isSave]
    }
    
    override func conversationDigest() -> String {
        "[个人名片]"
    }
    
    override func encode() -> Data {
        let dic = [
            userIdString: self.userId,
            nameString: self.name,
            portraitString: self.portrait
        ]
        if let data = try? JSONSerialization.data(withJSONObject: dic) {
            return data
        } else {
            return Data()
        }
    }
    
    override func decode(_ data: Data) {
        guard let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            return
        }
        if let userId = json[userIdString] as? String {
            self.userId = userId
        }
        if let name = json[nameString] as? String {
            self.name = name
        }
        if let portrait = json[portraitString] as? String {
            self.portrait = portrait
        }
    }
}
