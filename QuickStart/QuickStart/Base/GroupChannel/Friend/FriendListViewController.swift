//
//  FriendListViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/2.
//

import Foundation
import UIKit

class FriendListViewController: SelectUserViewController {
    override func loadView() {
        super.loadView()
        loadFriends()
    }
    
    open override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.tabBarController?.tabBar.isHidden = false
    }
    
    private func loadFriends() {
        HttpManager.shared.getFriends { code, friends in
            DispatchQueue.main.async {
                self.users = friends
                if let friends = friends, !friends.isEmpty {
                    self.tableView.reloadData()
                    self.emptyView.reloadData(.none)
                } else {
                    self.emptyView.reloadData(.noMembers)
                }
            }
        }
    }
}
