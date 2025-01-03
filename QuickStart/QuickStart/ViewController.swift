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
    
    var serverSettingButton: UIButton = {
        let button = UIButton()
        button.setTitle("服务器设置", for: .normal)
        button.titleLabel?.font = UIFont.systemFont(ofSize: 12)
        button.setTitleColor(UIColor(hex: "#0091FF"), for: .normal)
        button.titleLabel?.adjustsFontSizeToFitWidth = true
        button.addTarget(self, action: #selector(onServerSetting), for: .touchUpInside)
        return button
    }()
    
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
        
        JIM.shared().connectionManager.add(self)
        
        signInButton.tag = ButtonType.signIn.rawValue
        signInButton.addTarget(self, action: #selector(onTapButton(_:)), for: .touchUpInside)
        
        UserDefaults.saveIsLightTheme(true)
        
        self.connectView.isHidden = false
        self.connectView.alpha = 1
        
        let coreVersion: String = JIM.shared().getSDKVersion()
        let uikitVersion: String = JuggleUI.version
        versionLabel.text = "UIKit v\(uikitVersion)\tSDK v\(coreVersion)"
        
        phoneNumberTextField.text = UserDefaults.loadPhoneNumber()
        verifyCodeTextField.text = UserDefaults.loadVerifyCode()
        
        self.view.addSubview(self.serverSettingButton)
        
        self.serverSettingButton.sbu_constraint(width: 100)
        var layoutConstraints: [NSLayoutConstraint] = []
        layoutConstraints.append(self.serverSettingButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor, constant: -10))
        layoutConstraints.append(self.serverSettingButton.topAnchor.constraint(equalTo: self.view.topAnchor, constant: 40))
        NSLayoutConstraint.activate(layoutConstraints)
        
        guard phoneNumberTextField.text != nil,
              verifyCodeTextField.text != nil else {
            return
        }
        signinAction()
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
        
        let regex = "^1[3,4,5,6,7,8,9][0-9]{9}$"
        let predicate = NSPredicate(format: "SELF MATCHES %@", regex)
        let isPhoneNumber = predicate.evaluate(with: phoneNumber)
        
        if !isPhoneNumber {
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
        
        UserDefaults.savePhoneNumber(phoneNumber)
        UserDefaults.saveVerifyCode(verifyCode)
        
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
    
    @objc func onServerSetting() {
        let vc = SettingsServerViewController()
        vc.modalPresentationStyle = .fullScreen
        self.present(vc, animated: true)
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
