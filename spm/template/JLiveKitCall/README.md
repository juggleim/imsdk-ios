# JLiveKitCall iOS (Swift Package)

This repository distributes JLiveKitCall, the LiveKit call plugin for the JuggleIM iOS SDK, through Swift Package Manager.
SDK source code and issues live in [imsdk-ios](https://github.com/juggleim/imsdk-ios).

## Installation

In Xcode: **File > Add Package Dependencies…**, enter the repository URL:

```
https://github.com/Juggleim/jlivekitcall-ios-spm
```

Choose **Up to Next Major Version** from `__VERSION__` and add the `JLiveKitCall` library to your app target.
It brings in JuggleIM and LiveKit SDK automatically.

Or in `Package.swift`:

```swift
dependencies: [
    .package(url: "https://github.com/Juggleim/jlivekitcall-ios-spm", from: "__VERSION__")
],
targets: [
    .target(name: "YourApp", dependencies: [
        .product(name: "JLiveKitCall", package: "jlivekitcall-ios-spm")
    ])
]
```

JLiveKitCall __VERSION__ works with JuggleIM __JUGGLEIM_MIN__ or later within the same major version (below __JUGGLEIM_NEXT_MAJOR__), and requires LiveKit __DEP_VERSION__ (github.com/livekit/client-sdk-swift).
If your app also depends on `juggleim-ios-spm` directly, any version in that range works, so JuggleIM can be upgraded without changing this package.

Requirements: iOS 13.0 or later.

## Usage

JuggleIM loads the LiveKit engine automatically once JLiveKitCall is linked into your app; no import is needed.

## License

Copyright © JuggleIM. See [LICENSE](./LICENSE).
