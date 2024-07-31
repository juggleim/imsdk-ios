//
//  HttpManager.swift
//  QuickStart
//
//  Created by Nathan on 2024/7/30.
//

import Foundation

class HttpManager: NSObject {
    static let shared = HttpManager()
    
    let domain = "https://appserver.jugglechat.com"
    
    let smsLoginString = "/sms_login"
    let phoneString = "phone"
    let codeString = "code"
    let msgString = "msg"
    let dataString = "data"
    let userIdString = "user_id"
    let authorizationString = "authorization"
    let nickNameString = "nick_name"
    let avatarString = "avatar"
    let statusString = "status"
    let imTokenString = "im_token"
    
    let unknownError = 505
    
    func login(
        phoneNumber: String,
        verifyCode: String,
        completion: ((Int, JCUser?) -> Void)? = nil
    ) {
        let urlString = domain.appending(smsLoginString)
        guard let url = URL(string: urlString) else {
            return
        }
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        let dict = [phoneString: phoneNumber, codeString: verifyCode]
        request.httpBody = try? JSONSerialization.data(withJSONObject: dict)
        let task = URLSession(configuration: .default).dataTask(with: request) { data, response, error in
            if let error = error {
                print("login error: \(error)")
                if let completion = completion {
                    completion(self.unknownError, nil)
                }
                return
            }
            if let httpResponse = response as? HTTPURLResponse {
                if httpResponse.statusCode != 200 {
                    print("login error, http code is \(httpResponse.statusCode)")
                    if let completion = completion {
                        completion(httpResponse.statusCode, nil)
                    }
                    return
                }
            } else {
                print("login error, response is not a HTTPURLResponse")
                if let completion = completion {
                    completion(self.unknownError, nil)
                }
                return
            }
            if let data = data {
                guard let json = try? JSONSerialization.jsonObject(with: data) as? Dictionary<String, Any> else {
                    print("login error, JSONSerialization fail")
                    if let completion = completion {
                        completion(self.unknownError, nil)
                    }
                    return
                }
                if let code = json[self.codeString] as? Int {
                    if code != 0 {
                        print("login error, json code is \(code), msg is \(json[self.msgString] ?? "")")
                        if let completion = completion {
                            completion(self.unknownError, nil)
                        }
                        return
                    }
                }
                guard let responseData = json[self.dataString] as? Dictionary<String, Any> else {
                    print("login error, data is not available")
                    if let completion = completion {
                        completion(self.unknownError, nil)
                    }
                    return
                }
                let jcUser: JCUser = JCUser()
                if let userId = responseData[self.userIdString] as? String {
                    jcUser.userId = userId
                } else {
                    print("login error, userId is not available")
                    if let completion = completion {
                        completion(self.unknownError, nil)
                    }
                    return
                }
                if let authorization = responseData[self.authorizationString] as? String {
                    jcUser.authorization = authorization
                }
                if let nickname = responseData[self.nickNameString] as? String {
                    jcUser.userName = nickname
                }
                if let avatar = responseData[self.avatarString] as? String {
                    jcUser.portrait = avatar
                }
                if let token = responseData[self.imTokenString] as? String {
                    jcUser.token = token
                }
                if let completion = completion {
                    completion(0, jcUser)
                }
            } else {
                print("login error, data is nil")
                if let completion = completion {
                    completion(self.unknownError, nil)
                }
                return
            }
        }
        task.resume()
    }
    
}
