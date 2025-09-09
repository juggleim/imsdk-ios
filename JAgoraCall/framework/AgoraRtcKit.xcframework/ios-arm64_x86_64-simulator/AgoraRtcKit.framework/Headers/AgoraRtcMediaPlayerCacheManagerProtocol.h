//
//  Copyright (c) 2020 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol AgoraRtcMediaPlayerCacheManagerProtocol <NSObject>

/**
 * Get shared cacheManager instance.
 * @return cacheManager instance.
 */
+ (instancetype)sharedInstance NS_SWIFT_NAME(sharedInstance());
/**
 * Remove all media resource cache files.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)removeAllCaches NS_SWIFT_NAME(removeAllCaches());
/**
 * Remove the latest media resource cache file.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)removeOldCache NS_SWIFT_NAME(removeOldCache());
/**
 * Remove the cache file by uri, setting by MediaSource.
 * @param uri URI，identify the uniqueness of the property, Set from `MeidaSource`
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)removeCacheByUri:(NSString *)uri NS_SWIFT_NAME(removeCache(byUri:));
/**
 * Set cache file path that files will be saved to.
 * @param cacheDir cacheDir path.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setCacheDir:(NSString *)cacheDir NS_SWIFT_NAME(setCacheDir(_:));
/**
 * Set the maximum number of cached files.
 * @param count maximum number of cached files.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setMaxCacheFileCount:(NSInteger)count NS_SWIFT_NAME(setMaxCacheFileCount(_:));
/**
 * Set the total size of the largest cache file.
 * @param cacheSize total size of the largest cache file.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)setMaxCacheFileSize:(NSInteger)cacheSize NS_SWIFT_NAME(setMaxCacheFileSize(_:));
/**
 * Set whether the player will clean up the cache in the order of resource usage.
 * @param enable enable the player to automatically clear the cache.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (int)enableAutoRemoveCache:(BOOL)enable NS_SWIFT_NAME(enableAutoRemoveCache(_:));
/**
 * Get the cache directory you have set.
 * @return cacheDir
 */
- (NSString *)cacheDir NS_SWIFT_NAME(cacheDir());
/**
 * Get the maximum number of cached files.
 * @return
 * > 0: file count.
 * - < 0: Failure.
 */
- (NSInteger)maxCacheFileCount NS_SWIFT_NAME(maxCacheFileCount());
/**
 * Get the total size of the largest cache file
 * @return
 * > 0: file size.
 * - < 0: Failure.
 */
- (NSInteger)maxCacheFileSize NS_SWIFT_NAME(maxCacheFileSize());
/**
 * Get the number of all cache files.
 * @return
 * > 0: file count.
 * - < 0: Failure.
 */
- (NSInteger)cacheFileCount NS_SWIFT_NAME(cacheFileCount());

@end

NS_ASSUME_NONNULL_END
