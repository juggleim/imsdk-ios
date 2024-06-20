//
//  ViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/5/14.
//

import UIKit
import JetIM

let token3 = "CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"

enum ButtonType: Int {
    
    case signIn
    case startChatWithVC
    case startChatWithTC
    case startOpenChatWithTC
    case startChatBotWithVC
    case signOut
    case customSamples
}

class ViewController: UIViewController {
    // MARK: - Properties
    @IBOutlet weak var mainView: MainView!
    
    var homeStackView: UIStackView { mainView.homeStackView }
//    var startChatWithViewControllerButton: UIButton { mainView.groupChannelItemView.actionButton }
    var startChatWithTabbarControllerButton: UIButton { mainView.groupChannelItemView.actionButton }
    var startOpenChatWithTabbarControllerButton: UIButton { mainView.openChannelItemView.actionButton }
    var startChatBotWithViewControllerButton: UIButton { mainView.chatBotItemView.actionButton }
    var customSamplesButton: UIButton { mainView.customItemView.actionButton }
    
    @IBOutlet weak var versionLabel: UILabel!

    @IBOutlet weak var loadingIndicator: UIActivityIndicatorView! {
        didSet {
            loadingIndicator.stopAnimating()
        }
    }

    let duration: TimeInterval = 0.4
    
    enum CornerRadius: CGFloat {
        case small = 4.0
        case large = 8.0
    }
    
    
    // MARK: - Lifecycle
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)

        SBUTheme.set(theme: .light)
        GlobalSetCustomManager.setDefault()
        
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
//        startChatWithViewControllerButton.tag = ButtonType.startChatWithVC.rawValue
        startChatWithTabbarControllerButton.tag = ButtonType.startChatWithTC.rawValue
        startOpenChatWithTabbarControllerButton.tag = ButtonType.startOpenChatWithTC.rawValue
        startChatBotWithViewControllerButton.tag = ButtonType.startChatBotWithVC.rawValue
        customSamplesButton.tag = ButtonType.customSamples.rawValue
        
//        startChatWithViewControllerButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        startChatWithTabbarControllerButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        startChatBotWithViewControllerButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        startOpenChatWithTabbarControllerButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        customSamplesButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
 
        UserDefaults.saveIsLightTheme(true)
        
        self.mainView.isHidden = false
        self.mainView.alpha = 1
        self.homeStackView.isHidden = false
        self.homeStackView.alpha = 1
        
        let coreVersion: String = JIM.shared().getSDKVersion()
        let uikitVersion: String = JuggleUI.version
        versionLabel.text = "UIKit v\(uikitVersion)\tSDK v\(coreVersion)"
        
        JIM.shared().connectionManager.add(self)
    }
    
    deinit {
        JIM.shared().connectionManager.remove(self)
    }
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .default
    }
    
    override func traitCollectionDidChange(_ previousTraitCollection: UITraitCollection?) {
        super.traitCollectionDidChange(previousTraitCollection)
    }
    
    // MARK: - Actions
    @IBAction func onEditingChangeTextField(_ sender: UITextField) {
        let color = sender.text?.isEmpty ?? true ? #colorLiteral(red: 0, green: 0, blue: 0, alpha: 0) : #colorLiteral(red: 0.4823529412, green: 0.3254901961, blue: 0.937254902, alpha: 1)
        sender.animateBorderColor(toColor: color, duration: 0.1)
    }
  
    @IBAction func onTapButton(_ sender: UIButton) {
        let type = ButtonType(rawValue: sender.tag)

        switch type {
        case .signIn:
            self.signinAction()
        case .startChatWithVC, .startChatWithTC:
            self.startChatAction(type: type ?? .startChatWithVC)
        case .startOpenChatWithTC:
            self.startOpenChatAction(type: .startOpenChatWithTC)
        case .startChatBotWithVC:
            self.startChatBotAction(type: .startChatBotWithVC)
        case .signOut:
            self.signOutAction()
        case .customSamples:
            self.moveToCustomSamples()
        default:
            break
        }
    }

    func signinAction() {
        JIM.shared().connectionManager.connect(withToken: token3)
    }
    
    func signOutAction() {
        
    }
    
    func startChatAction(type: ButtonType) {
//        if type == .startChatWithVC {
//            JIM.shared().connectionManager.connect(withToken: token3)
//        }
    }
    
    func startChatAction(with payload: NSDictionary) {
        guard let channel: NSDictionary = payload["channel"] as? NSDictionary,
              let channelURL: String = channel["channel_url"] as? String else { return }
        
        let mainVC = SBUGroupChannelListViewController()
        let naviVC = UINavigationController(rootViewController: mainVC)
        naviVC.modalPresentationStyle = .fullScreen
        self.present(naviVC, animated: true) {
//            SendbirdUI.moveToChannel(channelURL: channelURL)
        }
    }
    
    func startOpenChatAction(type: ButtonType) {
//        guard type == .startOpenChatWithTC else { return }
//
//        let mainVC = MainOpenChannelTabbarController()
//        mainVC.modalPresentationStyle = .fullScreen
//        present(mainVC, animated: true)
    }
    
    func startChatBotAction(type: ButtonType) {
//        guard type == .startChatBotWithVC else { return }
//
//        SendbirdUI.startChatWithAIBot(botId: "client_bot", isDistinct: true)
    }
    
    func moveToCustomSamples() {
//        SBUTheme.set(theme: .light)
//        let mainVC = CustomBaseViewController(style: .grouped)
//        let naviVC = UINavigationController(rootViewController: mainVC)
//        naviVC.modalPresentationStyle = .fullScreen
//        present(naviVC, animated: true)
    }
}

extension ViewController: JConnectionDelegate {
    func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {

    }
    
    func dbDidOpen() {
        DispatchQueue.main.async {
            let mainVC = SBUGroupChannelListViewController()
            let naviVC = UINavigationController(rootViewController: mainVC)
            naviVC.modalPresentationStyle = .fullScreen
            self.present(naviVC, animated: true)
        }
    }
    
    func dbDidClose() {
        
    }
}


extension ViewController: UINavigationControllerDelegate {
     public override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return UIInterfaceOrientationMask.portrait
    }
}

extension ViewController {
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        self.view.endEditing(true)
    }
}

