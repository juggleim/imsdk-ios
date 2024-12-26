//
//  BaseUserListViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/5.
//

import Foundation
import UIKit

public class BaseTableListViewController: UIViewController {
    let titleView = SBUNavigationTitleView()
    let tableView = UITableView()
    let emptyView = SBUEmptyView()
    lazy var loadingIndicator = UIActivityIndicatorView(frame: CGRectMake(0, 0, 37, 37))
    
    public override func loadView() {
        super.loadView()
        configNavigationItem()
        configTableView()
        configEmptyView()
        configIndicator()
    }
    
    func configNavigationItem() {
        titleView.textAlignment = .center
        self.navigationItem.titleView = titleView
        
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton
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
    }
    
    private func configEmptyView() {
        emptyView.type = EmptyViewType.none
    }
    
    private func configIndicator() {
        self.loadingIndicator.style = .large
        self.loadingIndicator.center = self.view.center
        self.view.addSubview(self.loadingIndicator)
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
}
