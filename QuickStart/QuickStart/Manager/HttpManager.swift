//
//  HttpManager.swift
//  QuickStart
//
//  Created by Nathan on 2024/7/30.
//

import Foundation
import JuggleIM

class HttpManager: NSObject {
    static let shared = HttpManager()
    
    static let domain = "https://appserver.jugglechat.com"
    
    static let smsLoginString = "/sms_login"
    static let phoneString = "phone"
    static let codeString = "code"
    static let msgString = "msg"
    static let dataString = "data"
    static let userIdString = "user_id"
    static let authorizationString = "authorization"
    static let nickNameString = "nickname"
    static let avatarString = "avatar"
    static let statusString = "status"
    static let imTokenString = "im_token"
    
    static let searchString = "/users/search"
    static let itemsString = "items"
    static let isFriendString = "is_friend"
    
    static let friendAddString = "/friends/add"
    static let friendIdString = "friend_id"
    
    static let friendListString = "/friends/list"
    
    static let myGroupsString = "/groups/mygroups"
    static let groupIdString = "group_id"
    static let groupNameString = "group_name"
    static let groupPortraitString = "group_portrait"
    
    static let usersUpdateString = "/users/update"
    
    static let unknownError = 505
    static let emptyCode = 444
    static let success = 0
    
    var currentUserId = ""
    var currentAuthorization = ""
    
    func login(
        phoneNumber: String,
        verifyCode: String,
        completion: @escaping ((Int, JCUser?, String?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.smsLoginString)
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        let dict = [Self.phoneString: phoneNumber, Self.codeString: verifyCode]
        request.httpBody = try? JSONSerialization.data(withJSONObject: dict)
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
    
    func searchUser(
        phoneNumber: String,
        completion: @escaping ((Int, JCUser?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.searchString)
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        if self.currentAuthorization.isEmpty {
            print("search user error, currentAuthorization is empty")
            completion(Self.unknownError, nil)
            return
        } else {
            request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
        }
        let dict = [HttpManager.phoneString: phoneNumber]
        request.httpBody = try? JSONSerialization.data(withJSONObject: dict)
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
                let jcUser: JCUser = JCUser()
                if let userId = item[Self.userIdString] as? String {
                    jcUser.userId = userId
                } else {
                    print("search user error, userId is not available")
                    completion(Self.unknownError, nil)
                    return
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
                if let isFriend = item[Self.isFriendString] as? Bool {
                    jcUser.isFriend = isFriend
                }

                completion(0, jcUser)
            })
        }
        task.resume()
    }
    
    func addFriend(
        userId: String,
        completion: @escaping ((Int) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendAddString)
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        if self.currentAuthorization.isEmpty {
            print("add friend error, currentAuthorization is empty")
            completion(Self.unknownError)
            return
        } else {
            request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
        }
        let dict = [Self.userIdString: self.currentUserId, Self.friendIdString: userId]
        request.httpBody = try? JSONSerialization.data(withJSONObject: dict)
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
    
    func getFriends(
        completion: @escaping ((Int, [JCUser]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.friendListString).appending("?")
            .appending("user_id=\(self.currentUserId)").appending("&").appending("count=100")
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        if self.currentAuthorization.isEmpty {
            print("get friends error, currentAuthorization is empty")
            completion(Self.unknownError, nil)
            return
        } else {
            request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
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
    
    func getMyGroups(
        completion: @escaping ((Int, [JGroupInfo]?) -> Void)
    ) {
        let urlString = Self.domain.appending(Self.myGroupsString).appending("?")
            .appending("count=100")
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        if self.currentAuthorization.isEmpty {
            print("get groups error, currentAuthorization is empty")
            completion(Self.unknownError, nil)
            return
        } else {
            request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
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
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        if self.currentAuthorization.isEmpty {
            print("add friend error, currentAuthorization is empty")
            completion(Self.unknownError)
            return
        } else {
            request.setValue(self.currentAuthorization, forHTTPHeaderField: Self.authorizationString)
        }
        var dict = [Self.userIdString: userId]
        if let name = name {
            dict[Self.nickNameString] = name
        }
        if let portrait = portrait {
            dict[Self.avatarString] = portrait
        }
        request.httpBody = try? JSONSerialization.data(withJSONObject: dict)
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
    
    // MARK: - Private
    func errorCheck(
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
}
