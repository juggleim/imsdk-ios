//
//  BaseUserListViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation
import UIKit

class BaseUserListViewController: UIViewController {
    var users: [JCUser]?
    let titleView = SBUNavigationTitleView()
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
    
    func configNavigationItem() {
        titleView.textAlignment = .center
        self.navigationItem.titleView = titleView
    }
    
    func configTableView() {
        self.tableView.bounces = false
        self.tableView.alwaysBounceVertical = false
        self.tableView.separatorStyle = .none
        self.tableView.backgroundView = self.emptyView
        self.tableView.rowHeight = UITableView.automaticDimension
        self.tableView.estimatedRowHeight = 44.0
        self.tableView.sectionHeaderHeight = 0
        self.view.addSubview(self.tableView)
        self.tableView.frame = self.view.frame
        if let users = users {
            if users.isEmpty {
                self.emptyView.reloadData(.noMembers)
            }
        } else {
            self.emptyView.reloadData(.noMembers)
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
}
