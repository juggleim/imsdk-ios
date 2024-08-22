//
//  ViewController.swift
//  QuickStart
//
//  Created by Nathan on 2024/5/14.
//

import UIKit
import JuggleIM

enum ButtonType: Int {
    case signIn
}

class ViewController: UIViewController {
    // MARK: - Properties
    @IBOutlet weak var connectView: ConnectView!
    
    var titleLabel: UILabel { connectView.titleLabel }
    var phoneNumberTextField: UITextField { connectView.phoneNumberTextField }
    var verifyCodeTextField: UITextField { connectView.verifyCodeTextField }
    var signInButton: UIButton { connectView.signInButton }
    
    @IBOutlet weak var versionLabel: UILabel!
    
    @IBOutlet weak var loadingIndicator: UIActivityIndicatorView! {
        didSet {
            loadingIndicator.stopAnimating()
        }
    }
    
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
        
        signInButton.tag = ButtonType.signIn.rawValue
        signInButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        
        UserDefaults.saveIsLightTheme(true)
        
        self.connectView.isHidden = false
        self.connectView.alpha = 1
        
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
        
        HttpManager.shared.login(phoneNumber: phoneNumber, verifyCode: verifyCode) { code, jcUser, token in
            if code == 0 {
                guard let token = token else {
                    return
                }
                ProfileManager.shared.currentUserInfo = jcUser
                JIM.shared().connectionManager.connect(withToken: token)
            } else {
                DispatchQueue.main.async {
                    self.loadingIndicator.stopAnimating()
                    self.view.isUserInteractionEnabled = true
                }
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
