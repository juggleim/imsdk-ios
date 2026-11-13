# JuggleIM iOS SDK (Swift Package)

This repository distributes the prebuilt JuggleIM iOS SDK through Swift Package Manager.
SDK source code and issues live in [imsdk-ios](https://github.com/juggleim/imsdk-ios).

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

Call plugins are separate packages: [jzegocall-ios-spm](https://github.com/Juggleim/jzegocall-ios-spm) (Zego), [jagoracall-ios-spm](https://github.com/Juggleim/jagoracall-ios-spm) (Agora) and [jlivekitcall-ios-spm](https://github.com/Juggleim/jlivekitcall-ios-spm) (LiveKit). Each plugin works with any JuggleIM in the same major version starting from the minimum listed in its README, so JuggleIM can be upgraded on its own.

## License

Copyright © JuggleIM. See [LICENSE](./LICENSE).
