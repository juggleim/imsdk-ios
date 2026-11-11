# JuggleIM iOS SDK (Swift Package)

This repository distributes the prebuilt JuggleIM iOS SDK through Swift Package Manager.
SDK source code and issues live in [im-ios-sdk](https://github.com/Juggleim/im-ios-sdk).

## Installation

In Xcode: **File > Add Package Dependencies…**, enter the repository URL:

```
https://github.com/Juggleim/juggleim-ios-spm
```

Choose **Up to Next Major Version** and add the `JuggleIM` library to your app target.

Or in `Package.swift`:

```swift
dependencies: [
    .package(url: "https://github.com/Juggleim/juggleim-ios-spm", from: "1.9.2")
],
targets: [
    .target(name: "YourApp", dependencies: [
        .product(name: "JuggleIM", package: "juggleim-ios-spm")
    ])
]
```

Requirements: iOS 12.0 or later.

## Usage

```swift
import JuggleIM
```

```objc
#import <JuggleIM/JuggleIM.h>
```

## Call plugins

The Zego, Agora and LiveKit call plugins are currently distributed through CocoaPods only.

## License

Copyright © JuggleIM. See [LICENSE](./LICENSE).
