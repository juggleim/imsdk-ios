//
//  SettingsWebViewController.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/21.
//

import Foundation
import WebKit

class SettingsWebViewController: UIViewController {
    var url: String?
    var titleString: String?
    lazy var webView: WKWebView = {
        let webView = WKWebView(frame: .zero)
        webView.translatesAutoresizingMaskIntoConstraints = false
        webView.scrollView.alwaysBounceVertical = false
        webView.scrollView.alwaysBounceHorizontal = false
        webView.scrollView.isScrollEnabled = true
        webView.clipsToBounds = true
        return webView
    }()
    
    let titleView = SBUNavigationTitleView()
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.setupView()
        self.configNavigationItem()
        self.loadHTML()
    }
    
    init(url: String, titleString: String) {
        self.url = url
        self.titleString = titleString
        super.init(nibName: nil, bundle: nil)
        self.setupView()
        self.configNavigationItem()
        self.loadHTML()
    }
    
    private func setupView() {
        self.view.addSubview(self.webView)
        self.view.backgroundColor = .clear

        NSLayoutConstraint.activate([
            self.webView.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 0),
            self.webView.leadingAnchor.constraint(equalTo: self.view.leadingAnchor, constant: 0),
            self.webView.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: 0),
            self.webView.bottomAnchor.constraint(equalTo: self.view.bottomAnchor, constant: 0)
        ])
    }
    
    private func configNavigationItem() {
        self.titleView.textAlignment = .center
        self.navigationItem.titleView = titleView
        self.titleView.text = self.titleString
        let leftButton = SBUBarButtonItem.backButton(target: self, selector: #selector(onTapLeftBarButton))
        self.navigationItem.leftBarButtonItem = leftButton
    }
    
    private func loadHTML() {
        if let url = url, let u = URL(string: url) {
            let request = URLRequest(url: u)
            self.webView.load(request)
        }
    }
    
    @objc func onTapLeftBarButton() {
        self.navigationController?.popViewController(animated: true)
    }
}
