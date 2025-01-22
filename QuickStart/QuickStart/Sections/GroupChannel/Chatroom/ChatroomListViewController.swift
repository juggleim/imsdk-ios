//
//  ChatroomListViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2024/9/5.
//

import UIKit
import JuggleIM

class ChatroomListViewController : BaseTableListViewController {
    
    static let chatroomIdList = ["chatroom1001",
                             "chatroom1002",
                             "chatroom1003",
                             "chatroom1004",
                             "chatroom1005"]
    
    static let chatroomNameList = ["chatroom 1",
                               "chatroom 2",
                               "chatroom 3",
                               "chatroom 4",
                               "chatroom 5"]
    
    override func loadView() {
        super.loadView()
        JIM.shared().chatroomManager.add(self)
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(
            ChatroomCell.self,
            forCellReuseIdentifier: ChatroomCell.sbu_className
        )
    }
}

extension ChatroomListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 5
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = ChatroomCell()
        cell.configure(name: ChatroomListViewController.chatroomNameList[indexPath.row])

        return cell
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let chatroomId = ChatroomListViewController.chatroomIdList[indexPath.row]

        JIM.shared().chatroomManager.joinChatroom(chatroomId)
    }
}

extension ChatroomListViewController: JChatroomDelegate {
    func chatroomDidJoin(_ chatroomId: String!) {
        let conversation = JConversation(conversationType: .chatroom, conversationId: chatroomId)
        let defaultConversationInfo = JConversationInfo()
        defaultConversationInfo.conversation = conversation
        let channelVC = ChannelViewController.init(conversationInfo: defaultConversationInfo)
        channelVC.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
    
    func chatroomDidQuit(_ chatroomId: String!) {
        
    }
    
    func chatroomJoinFail(_ chatroomId: String!, errorCode: JErrorCode) {
        
    }
    
    func chatroomQuitFail(_ chatroomId: String!, errorCode: JErrorCode) {
        
    }
    
    func chatroomDidKick(_ chatroomId: String!) {
        
    }
    
    func chatroomDidDestroy(_ chatroomId: String!) {
        
    }
}

