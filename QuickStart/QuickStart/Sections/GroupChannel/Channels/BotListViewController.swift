//
//  BotListViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/24.
//

import Foundation

class BotListViewController: BaseTableListViewController {
    var bots: [JCUser]?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadBots()
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(
            BaseUserCell.self,
            forCellReuseIdentifier: BaseUserCell.sbu_className
        )
    }
    
    private func loadBots() {
        HttpManager.shared.getBotList { code, bots in
            DispatchQueue.main.async {
                self.bots = bots
                self.tableView.reloadData()
            }
        }
    }
}

extension BotListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.bots?.count ?? 0
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var user: JCUser
        let cell = tableView.dequeueReusableCell(withIdentifier: BaseUserCell.sbu_className)
        cell?.selectionStyle = .none
        user = self.bots?[indexPath.row] ?? JCUser()
        if let userCell = cell as? BaseUserCell {
            userCell.configure(
                type: .friendList,
                user: user,
                isChecked: user.isFriend
            )
        }
        return cell ?? UITableViewCell()
        
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.bots?[indexPath.row] else {
            return
        }
        let conversation = JConversation(conversationType: .private, conversationId: user.userId)
        let defaultConversationInfo = JConversationInfo()
        defaultConversationInfo.conversation = conversation
        let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) ?? defaultConversationInfo
        let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
        channelVC.hidesBottomBarWhenPushed = true
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
}

