//
//  SwitchAccountCell.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/9.
//

import Foundation

class SwitchAccountCell: UITableViewCell {
    let portraitView = SBUMessageProfileView()
    let nameLabel = UILabel()
    let userIdLabel = UILabel()
    let inUseLabel: UILabel = {
        let title = UILabel()
        title.text = "当前使用"
        title.isHidden = true
        title.textColor = UIColor(hex: "#0086ff")
        return title
    }()
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.setupViews()
        self.setupLayouts()
    }
    
    init() {
        super.init(style: .default, reuseIdentifier: nil)
        self.setupViews()
        self.setupLayouts()
    }
    
    func setupViews() {
        self.contentView.backgroundColor = SBUColorSet.background100
        self.contentView.addSubview(portraitView)
        self.contentView.addSubview(nameLabel)
        self.contentView.addSubview(userIdLabel)
        self.contentView.addSubview(inUseLabel)
    }
    
    func setupLayouts() {
        portraitView.sbu_constraint(equalTo: self.contentView, leading: 16, top: 16)
        portraitView.sbu_constraint(width: 45, height: 45)
        
        nameLabel.sbu_constraint(equalTo: self.contentView, leading: 70, top: 16)
        nameLabel.sbu_constraint(height: 20)
        
        userIdLabel.sbu_constraint(equalTo: self.contentView, leading: 70, top: 39)
        userIdLabel.sbu_constraint(height: 20)
        
        inUseLabel.sbu_constraint(equalTo: self.contentView, trailing: -30, top: 27)
        inUseLabel.textAlignment = .right
        inUseLabel.sbu_constraint(width: 100, height: 20)
    }
    
    func configure(user: JCUser, inUse: Bool) {
        if let portrait = user.portrait {
            portraitView.configure(urlString: portrait, imageSize: 45)
        }
        nameLabel.text = user.userName
        userIdLabel.text = user.userId
        if inUse {
            inUseLabel.isHidden = false
        }
    }
    
}
