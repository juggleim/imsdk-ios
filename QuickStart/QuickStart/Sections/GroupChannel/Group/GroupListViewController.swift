//
//  GroupListViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation
import UIKit
import JuggleIM

class GroupListViewController: BaseTableListViewController {
    var groups: [JGroupInfo]?
    
    lazy var rightBarButton: UIBarButtonItem = {
        let rightItem =  UIBarButtonItem(
            image: SBUIconSetType.iconCreate.image(to: SBUIconSetType.Metric.defaultIconSize),
            style: .plain,
            target: self,
            action: #selector(onClickMenu)
        )
        rightItem.setTitleTextAttributes([.font : SBUFontSet.button2], for: .normal)
        return rightItem
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.navigationItem.rightBarButtonItem = rightBarButton
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
//        self.tabBarController?.tabBar.isHidden = false
        loadGroups()
    }
    
    override func configTableView() {
        super.configTableView()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.register(
            GroupCell.self,
            forCellReuseIdentifier: GroupCell.sbu_className
        )
    }
    
    private func loadGroups() {
        HttpManager.shared.getMyGroups(completion: { code, groups in
            DispatchQueue.main.async {
                self.groups = groups
                if let groups = groups, !groups.isEmpty {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.none)
                } else {
                    self.emptyView.reloadData(.noGroups)
                }
            }
        })
    }
    
    @objc func onClickMenu() {
        let createGroupVC = CreateGroupViewController()
        self.navigationController?.pushViewController(createGroupVC, animated: true)
    }
}

extension GroupListViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.groups?.count ?? 0
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: GroupCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let groupCell = cell as? GroupCell, let group = self.groups?[indexPath.row] {
            groupCell.configure(
                type: .createChannel,
                group: group
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let group = self.groups?[indexPath.row] else {
            return
        }
        let conversation = JConversation(conversationType: .group, conversationId: group.groupId)
        let defaultConversationInfo = JConversationInfo()
        defaultConversationInfo.conversation = conversation
        let conversationInfo = JIM.shared().conversationManager.getConversationInfo(conversation) ?? defaultConversationInfo
//        self.tabBarController?.tabBar.isHidden = true
        let channelVC = ChannelViewController.init(conversationInfo: conversationInfo)
        self.navigationController?.pushViewController(channelVC, animated: true)
    }
}
