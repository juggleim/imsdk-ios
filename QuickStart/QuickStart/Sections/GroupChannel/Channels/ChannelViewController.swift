//
//  ChannelViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/8/7.
//

import Foundation

class ChannelViewController: SBUGroupChannelViewController {
    override func viewDidLoad() {
        super.viewDidLoad()
        self.listComponent?.register(cell: GroupNotifyMessageCell(), contentType: GroupNotifyMessage.contentType())
    }
}
