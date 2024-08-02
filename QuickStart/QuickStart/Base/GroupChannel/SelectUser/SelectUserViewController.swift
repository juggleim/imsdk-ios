//
//  SelectUserViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/1.
//

import Foundation
import UIKit

class SelectUserViewController: UIViewController {
    var users: [JCUser]?
    let tableView = UITableView()
    let emptyView = SBUEmptyView()
    lazy var loadingIndicator = UIActivityIndicatorView(frame: CGRectMake(0, 0, 37, 37))
    
    override func loadView() {
        super.loadView()
        configNavigationItem()
        configTableView()
        configEmptyView()
        configIndicator()
    }
    
    private func configNavigationItem() {
        let titleView = SBUNavigationTitleView()
        titleView.text = "Add friend"
        titleView.textAlignment = .center
        self.navigationItem.titleView = titleView
    }
    
    private func configTableView() {
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.tableView.bounces = false
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.backgroundView = self.emptyView
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 44.0
        self.tableView.sectionHeaderHeight = 0
        self.tableView.register(
            SelectUserCell.self,
            forCellReuseIdentifier: SelectUserCell.sbu_className
        )
        self.view.addSubview(self.tableView)
        self.tableView.frame = self.view.frame
        if let users = users {
            if users.isEmpty {
                self.emptyView.reloadData(.noMembers)
                self.navigationItem.rightBarButtonItem = nil
            }
        } else {
            self.emptyView.reloadData(.noMembers)
            self.navigationItem.rightBarButtonItem = nil
        }
    }
    
    private func configEmptyView() {
        emptyView.type = EmptyViewType.none
    }
    
    private func configIndicator() {
        self.loadingIndicator.style = .large
        self.loadingIndicator.center = self.view.center
        self.view.addSubview(self.loadingIndicator)
    }
    
    private func addFriend(_ userId: String, _ completion: @escaping (Bool) -> Void) {
        HttpManager.shared.addFriend(userId: userId) { code in
            if code == HttpManager.success {
                completion(true)
            } else {
                completion(false)
            }
        }
    }
}

extension SelectUserViewController: UITableViewDataSource, UITableViewDelegate {
    open func tableView(_ tableView: UITableView, viewForHeaderInSection section: Int) -> UIView? {
        nil
    }

    open func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        0
    }
    
    open func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.users?.count ?? 0
    }

    open func tableView(_ tableView: UITableView,
                        cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: SelectUserCell.sbu_className)
        
        cell?.selectionStyle = .none

        if let userCell = cell as? SelectUserCell, let user = self.users?[indexPath.row] {
            userCell.configure(
                type: .createChannel,
                user: user,
                isChecked: user.isFriend
            )
        }
        
        return cell ?? UITableViewCell()
    }
    
    open func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        guard let user = self.users?[indexPath.row],
              let defaultCell = self.tableView.cellForRow(at: indexPath)
                as? SelectUserCell else { return }
        
        if user.isFriend {
            return
        }
        self.loadingIndicator.startAnimating()
        self.view.isUserInteractionEnabled = false
        addFriend(user.userId) { isSuccess in
            DispatchQueue.main.async {
                self.loadingIndicator.stopAnimating()
                self.view.isUserInteractionEnabled = true
                if isSuccess {
                    user.isFriend = true
                    defaultCell.selectUser(true)
                }
            }
        }
    }
}
