//
//  EmojiManager.swift
//  QuickStart
//
//  Created by Fei Li on 2025/1/14.
//

import Foundation

class EmojiManager {
    static let emojiList: [String] = ["😀", "😆", "😄", "😁", "🙂", "🙃", "🙁", "😭", "😡", "😬", "😅", "😌", "😓"]
    
    static func getAllEmojis() -> [String] {
        return emojiList
    }
    
    static func utf16ToEmoji(_ utf16String: String) -> String? {
        let components = utf16String.components(separatedBy: "%u")
        var utf16Values: [UInt16] = []
        for component in components {
            if let value = UInt16(component, radix: 16) {
                utf16Values.append(value)
            }
        }
        // 组合两个 UTF-16 代码单元为一个 UTF-32 代码单元
        let utf32Value = (UInt32(utf16Values[0]) << 10) + UInt32(utf16Values[1]) - 0x35fdc00
        if let scalar = UnicodeScalar(utf32Value) {
            return String(scalar)
        }
        return nil
    }
    
    static func emojiToUtf16(_ emoji: String) -> String {
        let utf16CodeUnits = emoji.utf16
        var utf16String = ""
        for codeUnit in utf16CodeUnits {
            utf16String += "%u\(String(codeUnit, radix: 16).uppercased())"
        }
        return utf16String
    }
}
