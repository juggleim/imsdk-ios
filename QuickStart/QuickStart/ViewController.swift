//
//  ViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/5/14.
//

import UIKit
import JuggleIM

let token1 = "CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"
let token2 = "CgZhcHBrZXkaINodQgLnbhTbt0SzC8b/JFwjgUAdIfUZTEFK8DvDLgM1"
let token3 = "CgZhcHBrZXkaINMDzs7BBTTZTwjKtM10zyxL4DBWFuZL6Z/OAU0Iajpv"
let token4 = "CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"
let token5 = "CgZhcHBrZXkaIOx2upLCsmsefp8U/KNb52UGnAEu/xf+im3QaUd0HTC2"

enum ButtonType: Int {
    case user1
    case user2
    case user3
    case user4
    case signIn
}

class ViewController: UIViewController {
    // MARK: - Properties
    @IBOutlet weak var connectView: ConnectView!
    
    var titleLabel: UILabel { connectView.titleLabel }
    var phoneNumberTextField: UITextField { connectView.phoneNumberTextField }
    var verifyCodeTextField: UITextField { connectView.verifyCodeTextField }
    var signInButton: UIButton { connectView.signInButton }
    
    @IBOutlet weak var mainView: MainView!
    
    var homeStackView: UIStackView { mainView.homeStackView }
    //    var startChatWithViewControllerButton: UIButton { mainView.groupChannelItemView.actionButton }
    var user1Button: UIButton { mainView.user1View.actionButton }
    var user2Button: UIButton { mainView.user2View.actionButton }
    var user3Button: UIButton { mainView.user3View.actionButton }
    var user4Button: UIButton { mainView.user4View.actionButton }
    
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
        
        user1Button.tag = ButtonType.user1.rawValue
        user2Button.tag = ButtonType.user2.rawValue
        user3Button.tag = ButtonType.user3.rawValue
        user4Button.tag = ButtonType.user4.rawValue
        signInButton.tag = ButtonType.signIn.rawValue
        
        user1Button.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        user2Button.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        user3Button.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        user4Button.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        signInButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        
        UserDefaults.saveIsLightTheme(true)
        
        self.mainView.isHidden = true
        self.mainView.alpha = 0
        self.connectView.isHidden = false
        self.connectView.alpha = 1
        
        self.homeStackView.isHidden = false
        self.homeStackView.alpha = 1
        
        let coreVersion: String = JIM.shared().getSDKVersion()
        let uikitVersion: String = JuggleUI.version
        versionLabel.text = "UIKit v\(uikitVersion)\tSDK v\(coreVersion)"
        
        JIM.shared().connectionManager.add(self)
    }
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .default
    }
    
    override func traitCollectionDidChange(_ previousTraitCollection: UITraitCollection?) {
        super.traitCollectionDidChange(previousTraitCollection)
    }
    
    @IBAction func onTapButton(_ sender: UIButton) {
        loadingIndicator.startAnimating()
        view.isUserInteractionEnabled = false
        
        let type = ButtonType(rawValue: sender.tag)
        
        switch type {
        case .user1:
            JIM.shared().connectionManager.connect(withToken: token1)
        case .user2:
            JIM.shared().connectionManager.connect(withToken: token2)
        case .user3:
            JIM.shared().connectionManager.connect(withToken: token3)
        case .user4:
            JIM.shared().connectionManager.connect(withToken: token4)
        case .signIn:
            signinAction()
        default:
            break
        }
    }
    
    func signinAction() {
        loadingIndicator.startAnimating()
        view.isUserInteractionEnabled = false
        
        let phoneNumber = phoneNumberTextField.text ?? ""
        let verifyCode = verifyCodeTextField.text ?? ""
        
        guard !phoneNumber.isEmpty else {
            phoneNumberTextField.shake()
            phoneNumberTextField.becomeFirstResponder()
            loadingIndicator.stopAnimating()
            view.isUserInteractionEnabled = true
            return
        }
        guard !verifyCode.isEmpty else {
            verifyCodeTextField.shake()
            verifyCodeTextField.becomeFirstResponder()
            loadingIndicator.stopAnimating()
            view.isUserInteractionEnabled = true
            return
        }
        
        HttpManager.shared.login(phoneNumber: phoneNumber, verifyCode: verifyCode) { code, jcUser in
            if code == 0 {
                guard let token = jcUser?.token else {
                    return
                }
                JIM.shared().connectionManager.connect(withToken: token)
            }
        }
    }
}

extension ViewController: JConnectionDelegate {
    func connectionStatusDidChange(_ status: JConnectionStatus, errorCode code: JErrorCode, extra: String!) {

    }
    
    func dbDidOpen() {
        DispatchQueue.main.async {
            self.loadingIndicator.stopAnimating()
            self.view.isUserInteractionEnabled = true
            let mainVC = MainChannelTabbarController()
            mainVC.modalPresentationStyle = .fullScreen
            self.present(mainVC, animated: true)
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
