//
//  GroupNotifyMessage.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/7.
//

import Foundation
import JuggleIM

enum GroupNotifyType: Int {
    case addMember = 1
    case removeMember = 2
    case rename = 3
    case other
}

let messageType = "jgd:grpntf"
let membersString = "members"
let userIdString = "user_id"
let avatarString = "avatar"
let nicknameString = "nickname"
let typeString = "type"
let operatorString = "operator"
let nameString = "name"
let digestString = "[群通知]"

class GroupNotifyMessage: JMessageContent {
    var type: GroupNotifyType = .other
    var members: [JUserInfo] = []
    var operatorInfo: JUserInfo? = nil
    var name: String = ""
    
    override class func contentType() -> String {
        return messageType
    }
    
    override class func flags() -> JMessageFlag {
        return .isSave
    }
    
    override func encode() -> Data {
        var membersArray: [[String: String]] = []
        for userInfo in members {
            let member = jsonFromUserInfo(userInfo)
            membersArray.append(member)
        }
        
        var content: [String: Any] = [membersString: membersArray, typeString: type.rawValue, nameString: name]
        if let operatorInfo = operatorInfo {
            let operatorJson = jsonFromUserInfo(operatorInfo)
            content[operatorString] = operatorJson
        }
        
        let data = try! JSONSerialization.data(withJSONObject: content)
        return data
    }
    
    override func decode(_ data: Data) {
        guard let json = try! JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            return
        }
        if let type = json[typeString] as? Int {
            self.type = GroupNotifyType(rawValue: type) ?? .other
        }
        if let membersJson = json[membersString] as? [[String: Any]] {
            var members: [JUserInfo] = []
            for member in membersJson {
                let userInfo = userInfoFromJson(member)
                members.append(userInfo)
            }
            self.members = members
        }
        if let operatorJson = json[operatorString] as? [String: String] {
            self.operatorInfo = userInfoFromJson(operatorJson)
        }
        if let name = json[nameString] as? String {
            self.name = name
        }
    }
    
    override func conversationDigest() -> String {
        return digestString
    }
    
    override var description: String {
        var isSender = false
        if let currentUserId = ProfileManager.shared.currentUserInfo?.userId,
           let operatorId = operatorInfo?.userId, currentUserId == operatorId {
            isSender = true
        }
        let sender = isSender ? "你" : (operatorInfo?.userName ?? "")
        var userList: String = ""
        for member in members {
            if let name = member.userName {
                userList.append(name)
                userList.append(", ")
            }
        }
        if !userList.isEmpty {
            userList.removeLast(2)
        }
        
        switch type {
        case .addMember:
            return "\(sender) 邀请 \(userList) 加入群聊"
        case .removeMember:
            return "\(sender) 将 \(userList) 移除群聊"
        case .rename:
            return "\(sender) 修改群名称为 \"\(name)\""
        case .other:
            return ""
        }
    }
    
    private func userInfoFromJson(_ json: [String: Any]) -> JUserInfo {
        let userInfo = JUserInfo()
        if let userId = json[userIdString] as? String {
            userInfo.userId = userId
        }
        if let name = json[nicknameString] as? String {
            userInfo.userName = name
        }
        if let portrait = json[avatarString] as? String {
            userInfo.portrait = portrait
        }
        return userInfo
    }
    
    private func jsonFromUserInfo(_ userInfo: JUserInfo) -> [String: String] {
        var member = [userIdString: userInfo.userId]
        if let name = userInfo.userName {
            member[nicknameString] = name
        }
        if let portrait = userInfo.portrait {
            member[avatarString] = portrait
        }
        return member
    }
}
