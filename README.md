<div align="center">
  <img src="README-assets/logo.png" alt="JuggleIM" width="120" />

  <h1>JuggleIM iOS SDK</h1>

  <p><strong>High-performance, open-source IM SDK for iOS.</strong><br/>
  Built on a custom binary protocol over WebSocket. Powers chat, group, live chatroom, RTC signaling and moments out of the box.</p>

  <p>
    <a href="#why-juggleim">Why JuggleIM</a> ·
    <a href="#features">Features</a> ·
    <a href="#quick-start">Quick Start</a> ·
    <a href="#documentation">Docs</a> ·
    <a href="#ecosystem">Ecosystem</a> ·
    <a href="#community">Community</a>
  </p>

  <p>
    English | <a href="./README.zh-CN.md">简体中文</a>
  </p>
</div>

---

## Why JuggleIM

JuggleIM provides the iOS client SDK for the JuggleIM realtime messaging stack. It is designed for production chat apps that need connection management, reliable message delivery, custom message types, and optional call integrations.

- Custom server configuration with app key initialization.
- Connection, reconnection, conversations, history, unread counts, and receipts.
- Text, image, file, custom, and business notification messages.
- One-to-one chat, group chat, chatroom, and live call workflows.
- Optional call plugins for Zego, Agora, and LiveKit.

## Features

- Core IM capabilities: connection management, reconnect, message send/receive, history, and unread state.
- Message types: text, image, file, custom, and notification messages.
- Extension points: custom content types, push token registration, and call integrations.
- Demo app: QuickStart includes login, chat, group, custom message, and call flows.

## Quick Start

### Install

For local development from this repository, add the podspecs by path in your app target:

```ruby
pod 'JuggleIM', :path => './JuggleIM'
pod 'JZegoCall', :path => './JZegoCall'         # optional
```

### Initialize

Initialize the SDK in `AppDelegate`:

```swift
import JuggleIM

JIM.shared().setServerUrls(["wss://your-im-server"])
JIM.shared().setConsoleLogLevel(.verbose)
JIM.shared().initWithAppKey("your_app_key")
```

### Connect and send a message

Connect with the IM token issued by your backend:

```swift
JIM.shared().connectionManager.connect(withToken: "user_im_token")
```

Send a text message:

```swift
let textMessage = JTextMessage(content: "Hello from JuggleIM")
let conversation = JConversation(conversationType: .private, conversationId: "TARGET_USER_ID")

let message = JIM.shared().messageManager.sendMessage(
    textMessage,
    in: conversation
) { sentMessage in
    print("sent:", String(describing: sentMessage))
} error: { code, errorMessage in
    print("error:", code.rawValue, String(describing: errorMessage))
}
```

### Push tokens

If you use push notifications, register APNs and VoIP tokens after login:

```swift
JIM.shared().connectionManager.registerDeviceToken(deviceToken)
JIM.shared().connectionManager.registerVoIPToken(pushCredentials.token)
```

## Repository Layout

| Directory | Description |
| --- | --- |
| `JuggleIM` | Core IM SDK |
| `QuickStart` | iOS demo app and sample business logic |
| `JZegoCall` | Zego call plugin |
| `JAgoraCall` | Agora call plugin |
| `JLiveKitCall` | LiveKit call plugin |

## Run the Demo

1. Open `QuickStart/QuickStart.xcworkspace` in Xcode.
2. Make sure CocoaPods dependencies are installed.
3. Build and run the `QuickStart` scheme.
4. Configure `appKey`, IM server URL, and optional call credentials for your environment.

Useful entry points:

- `QuickStart/QuickStart/AppDelegate.swift`
- `QuickStart/QuickStart/ViewController.swift`
- `QuickStart/QuickStart/Sections/GroupChannel`

## Documentation

- Official docs: <https://www.juggle.im/>

## Ecosystem

| Project | Description |
| --- | --- |
| [im-web-sdk](https://github.com/Juggleim/im-web-sdk) | Web SDK |
| [im-server](https://github.com/Juggleim/im-server) | Self-hosted IM backend |
| [im-admin](https://github.com/Juggleim/im-admin) | Admin console |
| [im-android-sdk](https://github.com/Juggleim/im-android-sdk) | Android SDK |
| [im-ios-sdk](https://github.com/Juggleim/im-ios-sdk) | iOS SDK |
| [web-im-demo](https://github.com/Juggleim/web-im-demo) | Web integration demo |

## Community

- Telegram Chinese group: <https://t.me/juggleim_zh>
- Telegram English group: <https://t.me/juggleim_en>
- WeChat group invite: <https://downloads.juggleim.com/xiaoshan.jpg>

## License

Copyright © JuggleIM. Licensed under the [Apache License 2.0](./LICENSE).
