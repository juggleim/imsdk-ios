//
//  SBUCacheManager.Template.swift
//  QuickStart
//
//  Created by Tez Park on 2023/02/26.
//  Copyright © 2023 SendBird, Inc. All rights reserved.
//

import UIKit

extension SBUCacheManager {
    class Template {
        static let diskCache = DiskCacheForTemplate(cacheType: "templates")
        static let cachePath = diskCache.cachePathURL()
        static let cacheKey = "templates"
        
        // MARK: - TemplateList token (updated time)
        static var lastToken: String {
            get { self.loadLastTokenKey() }
            set { self.saveLastTokenKey(newValue) }
        }
    
        
        
        static func removeTemplate(forKey key: String) {
            self.diskCache.remove(key: key)
        }
        
        // MARK: lastTokenKey
        static func loadLastTokenKey() -> String {
            return self.diskCache.loadLastTokenKey()
        }
        
        static func saveLastTokenKey(_ value: String) {
            self.diskCache.saveLastTokenKey(value)
        }
        
        // MARK: Reset
        static func resetCache() {
            self.diskCache.resetCache()
        }
    }
    
    class TemplateImage {
        static let memoryCache = MemoryCacheForTemplateImageSize()
        
        static func save(messageId: Int64, viewIndex: Int, size: CGSize) {
            self.memoryCache.set(messageId: messageId, viewIndex: viewIndex, size: size)
        }
        
        static func load(messageId: Int64, viewIndex: Int) -> CGSize? {
            self.memoryCache.get(messageId: messageId, viewIndex: viewIndex)
        }
    }
}

extension SBUCacheManager {
    struct DiskCacheForTemplate {
        // MARK: - Properties
        let fileManager = FileManager.default
        let cacheType: String
        let diskQueue = DispatchQueue(label: "\(SBUConstant.bundleIdentifier).queue.diskcache.template", qos: .background)
        var fileSemaphore = DispatchSemaphore(value: 1)
        
        let lastTokenKey = "sbu_template_list_updated_at"
        
        // MARK: - Initializers
        init(cacheType: String) {
            self.cacheType = cacheType
            
            do {
                try self.createDirectoryIfNeeded()
            } catch {
                SBULog.error(error.localizedDescription)
            }
        }
        
        func createDirectoryIfNeeded() throws {
            let cachePath = self.cachePathURL().path
            
            if self.fileManager.fileExists(atPath: cachePath) {
                return
            }
            
            try self.fileManager.createDirectory(
                atPath: cachePath,
                withIntermediateDirectories: true,
                attributes: nil
            )
        }
        
        func cacheExists(key: String) -> Bool {
            return fileManager.fileExists(atPath: self.pathForKey(key))
        }

        
        func set(key: String, data: NSData, completionHandler: SBUCacheCompletionHandler? = nil) {
            diskQueue.sync {
                self.fileSemaphore.wait()
                defer { self.fileSemaphore.signal() }
                
                let filePath = URL(fileURLWithPath: self.pathForKey(key))
                
                do {
                    let subPath = filePath.deletingLastPathComponent()
                    try self.fileManager.createDirectory(
                        atPath: subPath.path,
                        withIntermediateDirectories: true,
                        attributes: nil
                    )
                } catch {
                    SBULog.error(error.localizedDescription)
                    DispatchQueue.main.async {
                        completionHandler?(nil, nil)
                    }
                    return
                }
                
                data.write(to: filePath, atomically: true)
                DispatchQueue.main.async {
                    completionHandler?(filePath, data)
                }
            }
        }
        
        func remove(key: String) {
            diskQueue.sync {
                self.fileSemaphore.wait()
                defer { self.fileSemaphore.signal() }
                
                do {
                    let path = self.pathForKey(key)
                    let fileManager = self.fileManager
                    try fileManager.removeItem(atPath: path)
                } catch {
                    SBULog.error("Could not remove file: \(error)")
                }
            }
        }
        
        func removePath() {
            diskQueue.sync {
                self.fileSemaphore.wait()
                defer { self.fileSemaphore.signal() }
                
                do {
                    let path = self.cachePathURL()
                    let fileManager = self.fileManager
                    try fileManager.removeItem(at: path)
                } catch {
                    SBULog.error("Could not remove path: \(error)")
                }
            }
        }
        
        func cachePathURL() -> URL {
            guard let cacheDirectoryURL = try? FileManager.default.url(
                for: .cachesDirectory,
                in: .userDomainMask,
                appropriateFor: nil,
                create: true) else { return URL(fileURLWithPath: "") }
            
            let cachePathURL = cacheDirectoryURL.appendingPathComponent("\(self.cacheType)/")
            return cachePathURL
        }
        
        func pathForKey(_ key: String) -> String {
            let cachePathURL = cachePathURL()
            let fullPath = cachePathURL.appendingPathComponent(key)
            return fullPath.path
        }
        
        // MARK: lastTokenKey
        func loadLastTokenKey() -> String {
            return self.diskQueue.sync {
                self.fileSemaphore.wait()
                defer { self.fileSemaphore.signal() }
                
                let cachePathURL = cachePathURL()
                let filePath = cachePathURL.appendingPathComponent(lastTokenKey)
                guard let retrievedString = try? String(contentsOf: filePath, encoding: .utf8) else {
                    if let storedValue = UserDefaults.standard.string(forKey: lastTokenKey) {
                        // for backward
                        UserDefaults.standard.removeObject(forKey: lastTokenKey)
                        self.saveLastTokenKey(storedValue)
                        return storedValue
                    }
                    return ""
                }
                return retrievedString
            }
        }
        
        func saveLastTokenKey(_ value: String) {
            self.diskQueue.async {
                do {
                    try self.createDirectoryIfNeeded()
                    let cachePathURL = cachePathURL()
                    let filePath = cachePathURL.appendingPathComponent(lastTokenKey)
                    try value.write(to: filePath, atomically: true, encoding: .utf8)
                } catch {
                    SBULog.error("Error writing to file: lastTokenKey value")
                }
            }
        }
        
        // MARK: reset
        func resetCache() {
            self.removePath()
        }
    }
    
    class MemoryCacheForTemplateImageSize {
        var sizeCache: [String: CGSize] = [:] // The key is a combination of the message ID and the index of view item.
        
        func generateKey(messageId: Int64, viewIndex: Int) -> String {
            "\(messageId)_\(viewIndex)"
        }
        
        func set(messageId: Int64, viewIndex: Int, size: CGSize) {
            self.sizeCache[self.generateKey(messageId: messageId, viewIndex: viewIndex)] = size
        }
        
        func get(messageId: Int64, viewIndex: Int) -> CGSize? {
            self.sizeCache[self.generateKey(messageId: messageId, viewIndex: viewIndex)]
        }
    }
}
