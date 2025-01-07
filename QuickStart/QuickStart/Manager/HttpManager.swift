//
//  HttpManager.swift
//  QuickStart
//
//  Created by Nathan on 2024/7/30.
//

import Foundation
import JuggleIM

@objc class HttpManager: NSObject {
    @objc static let shared = HttpManager()
    
    static var domain: String {
        GlobalConfig.demoServer
    }
    
    static let smsLoginString = "/sms_login"
    static let phoneString = "phone"
    static let codeString = "code"
    static let msgString = "msg"
    static let dataString = "data"
    static let userIdString = "user_id"
    static let authorizationString = "authorization"
    static let appKey = "appkey"
    static let nickNameString = "nickname"
    static let avatarString = "avatar"
    static let statusString = "status"
    static let imTokenString = "im_token"
    
    static let searchString = "/users/search"
    static let itemsString = "items"
    static let isFriendString = "is_friend"
    
    static let friendAddString = "/friends/add"
    static let friendIdString = "friend_id"
    
    static let friendApplyString = "/friends/apply"
    static let startString = "start"
    static let orderString = "order"
    
    static let friendDelString = "/friends/del"
    static let friendIdsString = "friend_ids"
    
    static let friendsConfrimString = "/friends/confirm"
    static let sponsorIdString = "sponsor_id"
    static let isAgreeString = "is_agree"
    
    static let friendListString = "/friends/list"
    static let countString = "count"
    
    static let friendsApplicationsString = "/friends/applications"
    static let targetUserString = "target_user"
    static let isSponsorString = "is_sponsor"
    
    static let myGroupsString = "/groups/mygroups"
    static let groupIdString = "group_id"
    static let groupNameString = "group_name"
    static let groupPortraitString = "group_portrait"
    
    static let usersUpdateString = "/users/update"
    
    static let groupsAddString = "/groups/add"
    static let membersString = "members"
    
    static let groupsUpdateString = "/groups/update"
    
    static let groupsDissolveString = "/groups/dissolve"
    
    static let groupsMuteString = "/groups/management/setmute"
    static let isMuteString = "is_mute"
    
    static let setHisMsgVisibleString = "/groups/management/sethismsgvisible"
    
    static let groupAnnouncementString = "/groups/setgrpannouncement"
    static let contentString = "content"
    
    static let getGroupAnnouncementString = "/groups/getgrpannouncement"
    
    static let setGroupDisplayNameString = "/groups/setdisplayname"
    static let groupDisplayNameString = "grp_display_name"
    
    static let groupsChgownerString = "/groups/management/chgowner"
    static let ownerIdString = "owner_id"
    
    static let groupsInviteString = "/groups/invite"
    static let memberIdsString = "member_ids"
    
    static let groupsQuitString = "/groups/quit"
    
    static let groupsMembersDelString = "/groups/members/del"
    
    static let groupsMembersListString = "/groups/members/list"
    static let limitString = "limit"
    static let offsetString = "offset"
    
    static let groupsInfoString = "/groups/info"
    static let memberCountString = "member_count"
    static let ownerString = "owner"
    static let myRoleString = "my_role"
    static let groupManagementString = "group_management"
    static let groupMuteString = "group_mute"
    static let maxAdminCountString = "max_admin_count"
    static let adminCountString = "admin_count"
    static let groupVerifyTypeString = "group_verify_type"
    static let groupHisMsgVisibleString = "group_his_msg_visible"
    
    static let qrcodeConfirmString = "/login/qrcode/confirm"
    static let idString = "id"
    
    static let groupsQrcodeString = "/groups/qrcode"
    static let qrcodeString = "qr_code"
    
    static let groupsApplyString = "/groups/apply"
    
    static let usersQrcodeString = "/users/qrcode"
    
    static let usersInfoString = "/users/info"
    
    static let unknownError = 505
    static let emptyCode = 444
    static let success = 0
    
    enum HttpMethod {
        case get
        case post
    }
    
    var currentUserId = ""
    var currentAuthorization = ""
    var currentAppKey = ""
    
    func setAppKey(_ appKey : String) {
        currentAppKey = appKey;
    }
    
    @objc func qrcodeConfirm(
        qrcodeString: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.qrcodeConfirmString)
        let dict = [Self.idString: qrcodeString]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    func login(
        phoneNumber: String,
        verifyCode: String,
        completion: @escaping ((Int, JCUser?, String?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.smsLoginString)
        let dict = [Self.phoneString: phoneNumber, Self.codeString: verifyCode]
        let req = getRequest(url: urlString, method: .post, needAuthorization: false, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil, nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error) { code, json in
                if code != Self.success {
                    completion(code, nil, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("login error, data is not available")
                    completion(Self.unknownError, nil, nil)
                    return
                }
                let jcUser: JCUser = JCUser()
                if let userId = responseData[Self.userIdString] as? String {
                    jcUser.userId = userId
                    self?.currentUserId = userId
                } else {
                    print("login error, userId is not available")
                    completion(Self.unknownError, nil, nil)
                    return
                }
                if let authorization = responseData[Self.authorizationString] as? String {
                    self?.currentAuthorization = authorization
                }
                if let nickname = responseData[Self.nickNameString] as? String {
                    jcUser.userName = nickname
                }
                if let avatar = responseData[Self.avatarString] as? String {
                    jcUser.portrait = avatar
                }
                guard let token = responseData[Self.imTokenString] as? String else {
                    print("login error, token is not available")
                    completion(Self.unknownError, nil, nil)
                    return
                }
                jcUser.phoneNumber = phoneNumber
                completion(0, jcUser, token)
            }
        }
        task.resume()
    }
    
    @objc func getUserInfo(
        userId: String,
        completion: @escaping ((Int, JCUser?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.usersInfoString)
        let dic = [Self.userIdString: userId]
        let req = getRequest(url: urlString, method: .get, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get user error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                
                let jcUser = self?.jcUserFrom(json: responseData)
                completion(0, jcUser)
            })
        }
        task.resume()
    }
    
    func searchUser(
        phoneNumber: String,
        completion: @escaping ((Int, JCUser?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.searchString)
        let dict = [HttpManager.phoneString: phoneNumber]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("search user error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                guard let items = responseData[Self.itemsString] as? [Dictionary<String, Any>] else {
                    print("search user error, items is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if items.isEmpty {
                    completion(Self.emptyCode, nil)
                    return
                }
                let item = items[0]
                let jcUser = self?.jcUserFrom(json: item)
                completion(0, jcUser)
            })
        }
        task.resume()
    }
    
    @objc func applyFriend(
        userId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendApplyString)
        let dict = [Self.friendIdString: userId]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    func deleteFriends(
        userIds: [String],
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendDelString)
        let dict = [Self.friendIdsString: userIds]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    func confirmFriend(
        userId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendsConfrimString)
        let dict: [String : Any] = [Self.sponsorIdString: userId, Self.isAgreeString: true]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    func addFriend(
        userId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendAddString)
        let dict = [Self.userIdString: self.currentUserId, Self.friendIdString: userId]
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    @objc func getUserQRCode(
        completion: @escaping ((Int, String?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.usersQrcodeString)
        let req = getRequest(url: urlString, method: .get, params: nil)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get user qrcode error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if let qrcode = responseData[Self.qrcodeString] as? String {
                    completion(Self.success, qrcode)
                } else {
                    print("get user qrcode error, qr_code is not available")
                    completion(Self.unknownError, nil)
                }
            })
        }
        task.resume()
    }
    
    @objc func getGroupQRCode(
        groupId: String,
        completion: @escaping ((Int, String?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsQrcodeString)
        let dict = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get group qrcode error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if let qrcode = responseData[Self.qrcodeString] as? String {
                    completion(Self.success, qrcode)
                } else {
                    print("get group qrcode error, qr_code is not available")
                    completion(Self.unknownError, nil)
                }
            })
        }
        task.resume()
    }
    
    func getFriendsApplications(
        start: Int,
        count: Int,
        completion: @escaping ((Int, [JCUser]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendsApplicationsString)
        let dict: [String: Any] = [Self.startString: start, Self.countString: 100, Self.orderString: 0]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get friends applications error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                guard let items = responseData[Self.itemsString] as? [Dictionary<String, Any>] else {
                    print("get friends applications error, items is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if items.isEmpty {
                    completion(Self.emptyCode, nil)
                    return
                }
                var friends: [JCUser] = []
                for item in items {
                    guard let targetUserDic = item[Self.targetUserString] as? Dictionary<String, Any> else {
                        print("get friends applications error, target user is not available")
                        completion(Self.unknownError, nil)
                        return
                    }
                    let targetUser = JCUser()
                    if let userId = targetUserDic[Self.userIdString] as? String {
                        targetUser.userId = userId
                    }
                    if let nickname = targetUserDic[Self.nickNameString] as? String {
                        targetUser.userName = nickname
                    }
                    if let avatar = targetUserDic[Self.avatarString] as? String {
                        targetUser.portrait = avatar
                    }
                    
                    let isSponsor = item[Self.isSponsorString] as? Bool ?? false
                    let status = item[Self.statusString] as? Int ?? 0
                    if isSponsor == true {
                        switch status {
                        case 0:
                            targetUser.friendApplicationStatus = .outgoingApply
                        case 1:
                            targetUser.friendApplicationStatus = .outgoingAccept
                        case 3:
                            targetUser.friendApplicationStatus = .outgoingExpired
                        default:
                            targetUser.friendApplicationStatus = .outgoingApply
                        }
                    } else {
                        switch status {
                        case 0:
                            targetUser.friendApplicationStatus = .incomingApply
                        case 1:
                            targetUser.friendApplicationStatus = .incomingAccept
                        case 3:
                            targetUser.friendApplicationStatus = .incomingExpired
                        default:
                            targetUser.friendApplicationStatus = .incomingApply
                        }
                    }
                    friends.append(targetUser)
                }
                completion(0, friends)
            })
        }
        task.resume()
    }
    
    func getFriends(
        completion: @escaping ((Int, [JCUser]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendListString)
        let dict: [String: Any] = [Self.userIdString: self.currentUserId, Self.countString: 100]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get friends error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                guard let items = responseData[Self.itemsString] as? [Dictionary<String, Any>] else {
                    print("get friends error, items is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if items.isEmpty {
                    completion(Self.emptyCode, nil)
                    return
                }
                var friends: [JCUser] = []
                for item in items {
                    let jcUser: JCUser = JCUser()
                    if let userId = item[Self.userIdString] as? String {
                        jcUser.userId = userId
                    }
                    if let nickname = item[Self.nickNameString] as? String {
                        jcUser.userName = nickname
                    }
                    if let avatar = item[Self.avatarString] as? String {
                        jcUser.portrait = avatar
                    }
                    if let phone = item[Self.phoneString] as? String {
                        jcUser.phoneNumber = phone
                    }
                    jcUser.isFriend = true
                    friends.append(jcUser)
                }
                completion(0, friends)
            })
        }
        task.resume()
    }
    
    @objc func getGroupInfo(
        groupId: String,
        completion: @escaping ((Int, JCGroupInfo?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsInfoString)
        let dict: [String: Any] = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get groups error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                let groupInfo = JCGroupInfo()
                if let groupId = responseData[Self.groupIdString] as? String {
                    groupInfo.groupId = groupId
                }
                if let name = responseData[Self.groupNameString] as? String {
                    groupInfo.groupName = name
                }
                if let portrait = responseData[Self.groupPortraitString] as? String {
                    groupInfo.portrait = portrait
                }
                if let memberCount = responseData[Self.memberCountString] as? Int {
                    groupInfo.memberCount = memberCount
                }
                if let members = responseData[Self.membersString] as? [Dictionary<String, Any>] {
                    for member in members {
                        let user = JCUser()
                        if let userId = member[Self.userIdString] as? String {
                            user.userId = userId
                        }
                        if let nickname = member[Self.nickNameString] as? String {
                            user.userName = nickname
                        }
                        if let avatar = member[Self.avatarString] as? String {
                            user.portrait = avatar
                        }
                        groupInfo.members.append(user)
                    }
                }
                if let owner = responseData[Self.ownerString] as? Dictionary<String, Any> {
                    let user = JCUser()
                    if let userId = owner[Self.userIdString] as? String {
                        user.userId = userId
                    }
                    if let nickname = owner[Self.nickNameString] as? String {
                        user.userName = nickname
                    }
                    if let avatar = owner[Self.avatarString] as? String {
                        user.portrait = avatar
                    }
                    groupInfo.owner = user
                }
                if let myRole = responseData[Self.myRoleString] as? Int {
                    groupInfo.myRole = GroupRole(rawValue: myRole) ?? .member
                }
                if let management = responseData[Self.groupManagementString] as? Dictionary<String, Any> {
                    if let mute = management[Self.groupMuteString] as? Int {
                        groupInfo.mute = mute
                    }
                    if let maxAdminCount = management[Self.maxAdminCountString] as? Int {
                        groupInfo.maxAdminCount = maxAdminCount
                    }
                    if let adminCount = management[Self.adminCountString] as? Int {
                        groupInfo.adminCount = adminCount
                    }
                    if let groupVerifyType = management[Self.groupVerifyTypeString] as? Int {
                        groupInfo.groupVerifyType = groupVerifyType
                    }
                    if let hisMsgVisible = management[Self.groupHisMsgVisibleString] as? Int {
                        groupInfo.historyMessageVisible = hisMsgVisible
                    }
                }
                if let displayName = responseData[Self.groupDisplayNameString] as? String {
                    groupInfo.groupDisplayName = displayName
                }
                completion(0, groupInfo)
            })
        }
        task.resume()
    }
    
    func getMyGroups(
        completion: @escaping ((Int, [JGroupInfo]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.myGroupsString)
        let dict: [String: Any] = [Self.countString: 100]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get groups error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                guard let items = responseData[Self.itemsString] as? [Dictionary<String, Any>] else {
                    print("get groups error, items is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                if items.isEmpty {
                    completion(Self.emptyCode, nil)
                    return
                }
                var groups: [JGroupInfo] = []
                for item in items {
                    let group = JGroupInfo()
                    if let groupId = item[Self.groupIdString] as? String {
                        group.groupId = groupId
                    }
                    if let name = item[Self.groupNameString] as? String {
                        group.groupName = name
                    }
                    if let portrait = item[Self.groupPortraitString] as? String {
                        group.portrait = portrait
                    }
                    groups.append(group)
                }
                completion(0, groups)
            })
        }
        task.resume()
    }
    
    func updateUserInfo(
        userId: String,
        name: String? = nil,
        portrait: String? = nil,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.usersUpdateString)
        var dict = [Self.userIdString: userId]
        if let name = name {
            dict[Self.nickNameString] = name
        }
        if let portrait = portrait {
            dict[Self.avatarString] = portrait
        }
        let req = getRequest(url: urlString, method: .post, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(0)
            })
        }
        task.resume()
    }
    
    func getGroupMembers(
        groupId: String,
        count: Int,
        offset: String? = nil,
        completion: @escaping ((Int, String, [JUserInfo]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsMembersListString)
        
        let dict: [String: Any] = [Self.groupIdString: groupId, Self.limitString: count]
        let req = getRequest(url: urlString, method: .get, params: dict)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, "", nil)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, "", nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get group members error, data is not available")
                    completion(Self.unknownError, "", nil)
                    return
                }
                guard let items = responseData[Self.itemsString] as? [Dictionary<String, Any>] else {
                    print("get group members error, items is not available")
                    completion(Self.unknownError, "", nil)
                    return
                }
                if items.isEmpty {
                    completion(Self.emptyCode, "", nil)
                    return
                }
                var users: [JUserInfo] = []
                for item in items {
                    let user = JUserInfo()
                    if let userId = item[Self.userIdString] as? String {
                        user.userId = userId
                    }
                    if let name = item[Self.nickNameString] as? String {
                        user.userName = name
                    }
                    if let portrait = item[Self.avatarString] as? String {
                        user.portrait = portrait
                    }
                    users.append(user)
                }
                guard let resultOffset = responseData[Self.offsetString] as? String else {
                    completion(Self.success, "", users)
                    return
                }
                completion(Self.success, resultOffset, users)
            })
        }
        task.resume()
    }
    
    func deleteGroupMembers(
        groupId: String,
        userIdList: [String],
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsMembersDelString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.memberIdsString: userIdList]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func quitGroup(
        groupId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsQuitString)
        let dic: [String: Any] = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    @objc func groupApply(
        groupId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsApplyString)
        let dic: [String: Any] = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func groupInvite(
        groupId: String,
        userIdList: [String],
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsInviteString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.memberIdsString: userIdList]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func setGroupDisplayName(
        groupId: String,
        displayName: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.setGroupDisplayNameString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.groupDisplayNameString: displayName]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    @objc func getGroupAnnouncement(
        groupId: String,
        completion: @escaping ((Int, String) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.getGroupAnnouncementString)
        let dic: [String: Any] = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .get, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, "")
            return
        }
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, "")
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("get group announcement error, data is not available")
                    completion(Self.unknownError, "")
                    return
                }
                if let content = responseData[Self.contentString] as? String {
                    completion(Self.success, content)
                    return
                } else {
                    print("get group announcement error, content string is not available")
                    completion(Self.unknownError, "")
                    return
                }
            })
        }
        task.resume()
    }
    
    @objc func setGroupAnnouncement(
        groupId: String,
        content: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupAnnouncementString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.contentString: content]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func changeGroupOwner(
        groupId: String,
        ownerId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsChgownerString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.ownerIdString: ownerId]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func setGroupHistoryMessageVisible(
        groupId: String,
        isVisible: Int,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.setHisMsgVisibleString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.groupHisMsgVisibleString: isVisible]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func muteGroup(
        groupId: String,
        isMute: Int,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsMuteString)
        let dic: [String: Any] = [Self.groupIdString: groupId, Self.isMuteString: isMute]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func dissolveGroup(
        groupId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsDissolveString)
        let dic: [String: Any] = [Self.groupIdString: groupId]
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func updateGroup(
        groupId: String,
        name: String,
        portrait: String? = nil,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsUpdateString)
        
        var dic: [String: Any] = [Self.groupIdString: groupId, Self.groupNameString: name]
        if let portrait = portrait, !portrait.isEmpty {
            dic[Self.groupPortraitString] = portrait
        }
        
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code)
                    return
                }
                completion(Self.success)
            })
        }
        task.resume()
    }
    
    func createGroup(
        name: String,
        portrait: String? = nil,
        members: [String],
        completion: @escaping ((Int, JGroupInfo?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.groupsAddString)
        
        var membersValue: [[String: String]] = []
        for userId in members {
            let userIdDic = [Self.userIdString: userId]
            membersValue.append(userIdDic)
        }
        var dic: [String: Any] = [Self.groupNameString: name, Self.membersString: membersValue]
        if let portrait = portrait, !portrait.isEmpty {
            dic[Self.groupPortraitString] = portrait
        }
        
        let req = getRequest(url: urlString, method: .post, params: dic)
        guard let request = req.urlRequest, req.isSuccess else {
            completion(Self.unknownError, nil)
            return
        }
        
        let task = URLSession(configuration: .default).dataTask(with: request) { [weak self] data, response, error in
            self?.errorCheck(data: data, response: response, error: error, completion: { code, json in
                if code != Self.success {
                    completion(code, nil)
                    return
                }
                guard let responseData = json?[Self.dataString] as? Dictionary<String, Any> else {
                    print("create group error, data is not available")
                    completion(Self.unknownError, nil)
                    return
                }
                guard let groupId = responseData[Self.groupIdString] as? String else {
                    print("create group error, groupId is nil")
                    completion(Self.unknownError, nil)
                    return
                }
                let groupInfo = JGroupInfo()
                groupInfo.groupId = groupId
                groupInfo.groupName = name
                groupInfo.portrait = portrait
                completion(Self.success, groupInfo)
            })
        }
        task.resume()
    }
    
    // MARK: - Private
    private func getRequest(
        url: String,
        method: HttpMethod,
        needAuthorization: Bool = true,
        params: [String: Any]?
    ) -> (isSuccess: Bool, urlRequest: URLRequest?) {
        var urlString = url
        if method == .get, let params = params {
            urlString = urlString.appending("?")
            for (paramKey, paramValue) in params {
                urlString = urlString.appending("\(paramKey)").appending("=").appending("\(paramValue)")
                urlString = urlString.appending("&")
            }
            urlString.removeLast()
        }
        guard let requestUrl = URL(string: urlString) else {
            print("get request error, url string is \(urlString)")
            return (false, nil)
        }
        var request = URLRequest(url: requestUrl)
        switch method {
        case .get:
            request.httpMethod = "GET"
        case .post:
            request.httpMethod = "POST"
        }
        if needAuthorization && self.currentAuthorization.isEmpty {
            print("get request error, currentAuthorization is empty, url is \(url)")
            return (false, nil)
        }
        request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
        request.setValue(self.currentAppKey, forHTTPHeaderField: Self.appKey)
        if method == .post, let params = params, !params.isEmpty {
            request.httpBody = try? JSONSerialization.data(withJSONObject: params)
        }
        return (true, request)
    }
    
    private func errorCheck(
        data: Data?,
        response: URLResponse?,
        error: Error?,
        completion: ((Int, Dictionary<String, Any>?) -> Void)
    ) {
        let json: Dictionary<String, Any>
        if let error = error {
            print("login error: \(error)")
            completion(Self.unknownError, nil)
            return
        }
        if let httpResponse = response as? HTTPURLResponse {
            if httpResponse.statusCode != 200 {
                print("login error, http code is \(httpResponse.statusCode)")
                completion(httpResponse.statusCode, nil)
                return
            }
        } else {
            print("login error, response is not a HTTPURLResponse")
            completion(Self.unknownError, nil)
            return
        }
        if let data = data {
            guard let j = try? JSONSerialization.jsonObject(with: data) as? Dictionary<String, Any> else {
                print("login error, JSONSerialization fail")
                completion(Self.unknownError, nil)
                return
            }
            json = j
            if let code = json[Self.codeString] as? Int {
                if code != Self.success {
                    print("login error, json code is \(code), msg is \(json[Self.msgString] ?? "")")
                    completion(Self.unknownError, json)
                    return
                }
            }
        } else {
            print("login error, data is nil")
            completion(Self.unknownError, nil)
            return
        }
        completion(Self.success, json)
    }
    
    private func jcUserFrom(json: Dictionary<String, Any>) -> JCUser {
        let jcUser: JCUser = JCUser()
        if let userId = json[Self.userIdString] as? String {
            jcUser.userId = userId
        }
        if let nickname = json[Self.nickNameString] as? String {
            jcUser.userName = nickname
        }
        if let avatar = json[Self.avatarString] as? String {
            jcUser.portrait = avatar
        }
        if let phone = json[Self.phoneString] as? String {
            jcUser.phoneNumber = phone
        }
        if let isFriend = json[Self.isFriendString] as? Bool {
            jcUser.isFriend = isFriend
        }
        return jcUser
    }
}
