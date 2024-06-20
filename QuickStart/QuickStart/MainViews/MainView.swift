//
//  MainView.swift
//  QuickStart
//
//  Created by Nathan on 2024/5/14.
//

import UIKit

class MainView: NibCustomView {
    @IBOutlet weak var homeStackView: UIStackView! {
        willSet {
            newValue.alpha = 0
        }
    }
    
    @IBOutlet weak var user1View: MainItemView! {
        willSet {
            newValue.titleLabel.text = "User1"
            newValue.descriptionLabel.isHidden = true
            newValue.unreadCountLabel.isHidden = true
        }
    }
    
    @IBOutlet weak var user2View: MainItemView! {
        willSet {
            newValue.titleLabel.text = "User2"
            newValue.descriptionLabel.isHidden = true
            newValue.unreadCountLabel.isHidden = true
        }
    }
    
    @IBOutlet weak var user3View: MainItemView! {
        willSet {
            newValue.titleLabel.text = "User3"
            newValue.descriptionLabel.isHidden = true
            newValue.unreadCountLabel.isHidden = true
        }
    }
    
    @IBOutlet weak var user4View: MainItemView! {
        willSet {
            newValue.titleLabel.text = "User4"
            newValue.descriptionLabel.isHidden = true
            newValue.unreadCountLabel.isHidden = true
        }
    }
}
