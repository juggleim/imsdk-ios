<div align="center">
  <img src="README-assets/logo.png" alt="JuggleIM" width="120" />

  <h1>JuggleIM iOS SDK</h1>

  <p><strong>面向 iOS 的高性能开源 IM SDK。</strong><br/>
基于 WebSocket 之上的自定义二进制协议构建，开箱即支持聊天、群组、直播聊天室、RTC 信令和朋友圈/动态能力。</p>

  <p>
    <a href="#为什么选择-juggleim">为什么选择 JuggleIM</a> ·
    <a href="#功能特性">功能特性</a> ·
    <a href="#快速开始">快速开始</a> ·
    <a href="#文档">文档</a> ·
    <a href="#生态">生态</a> ·
    <a href="#社区">社区</a>
  </p>

  <p>
    <a href="./README.md">English</a> | 简体中文
  </p>
</div>

---

## 为什么选择 JuggleIM

JuggleIM 为 JuggleIM 实时消息体系提供 iOS 客户端 SDK，适合需要连接管理、可靠消息投递、自定义消息类型和可选通话能力的生产级聊天应用。

- 支持自定义服务地址和 appKey 初始化。
- 支持连接、重连、会话、历史消息、未读数和回执。
- 支持文本、图片、文件、自定义消息和业务通知消息。
- 支持单聊、群聊、聊天室和直播通话流程。
- 可按需集成 Zego、Agora 和 LiveKit 通话插件。

## 功能特性

- 核心 IM 能力：连接管理、重连、消息收发、历史消息和未读状态。
- 消息类型：文本、图片、文件、自定义消息和通知消息。
- 扩展能力：自定义消息类型、推送 Token 注册和通话集成。
- Demo 应用：QuickStart 提供登录、聊天、群组、自定义消息和通话流程示例。

## 快速开始

### 安装

如果你是基于当前仓库做本地开发，可以在 App 工程的 Podfile 中使用本地路径引入：

```ruby
pod 'JuggleIM', :path => './JuggleIM'
pod 'JZegoCall', :path => './JZegoCall'         # 可选
```

### 初始化

在 `AppDelegate` 中初始化 SDK：

```swift
import JuggleIM

JIM.shared().setServerUrls(["wss://your-im-server"])
JIM.shared().setConsoleLogLevel(.verbose)
JIM.shared().initWithAppKey("your_app_key")
```

### 连接并发送消息

使用后端签发的 IM token 连接：

```swift
JIM.shared().connectionManager.connect(withToken: "user_im_token")
```

发送一条文本消息：

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

### 推送 Token

如果需要推送，在登录后注册 APNs 和 VoIP Token：

```swift
JIM.shared().connectionManager.registerDeviceToken(deviceToken)
JIM.shared().connectionManager.registerVoIPToken(pushCredentials.token)
```

## 仓库结构

| 目录 | 说明 |
| --- | --- |
| `JuggleIM` | 核心 IM SDK |
| `QuickStart` | iOS Demo 应用和业务示例 |
| `JZegoCall` | Zego 通话插件 |
| `JAgoraCall` | Agora 通话插件 |
| `JLiveKitCall` | LiveKit 通话插件 |

## 运行 Demo

1. 使用 Xcode 打开 `QuickStart/QuickStart.xcworkspace`。
2. 确保 CocoaPods 依赖已安装。
3. 运行 `QuickStart` scheme。
4. 按你的环境配置 `appKey`、IM 服务地址和可选通话凭证。

可以重点查看这些入口：

- `QuickStart/QuickStart/AppDelegate.swift`
- `QuickStart/QuickStart/ViewController.swift`
- `QuickStart/QuickStart/Sections/GroupChannel`

## 文档

- 官方文档：<https://www.juggle.im/>

## 生态

| 项目 | 说明 |
| --- | --- |
| [im-web-sdk](https://github.com/Juggleim/im-web-sdk) | Web SDK |
| [im-server](https://github.com/Juggleim/im-server) | 自托管 IM 后端 |
| [im-admin](https://github.com/Juggleim/im-admin) | 管理控制台 |
| [im-android-sdk](https://github.com/Juggleim/im-android-sdk) | Android SDK |
| [im-ios-sdk](https://github.com/Juggleim/im-ios-sdk) | iOS SDK |
| [web-im-demo](https://github.com/Juggleim/web-im-demo) | Web 集成示例 |

## 社区

- Telegram 中文群：<https://t.me/juggleim_zh>
- Telegram English 群：<https://t.me/juggleim_en>
- 微信群邀请：<https://downloads.juggleim.com/xiaoshan.jpg>

## 许可证

Copyright © JuggleIM。基于 [Apache License 2.0](./LICENSE) 许可发布。
