//
//  AgoraMusicContentCenter.h
//  AgoraMusicContentCenter
//
//  Created by dingyusong on 2022/6/1.
//  Copyright © 2022 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol AgoraRtcMediaPlayerProtocol;
@protocol AgoraRtcMediaPlayerDelegate;

/**
 * Modes for playing songs.
 */
typedef NS_ENUM(NSUInteger, AgoraMusicPlayMode) {
    /**
     * 0: The music player is in the origin mode, which means playing the original song.
     */
    AgoraMusicPlayModeOriginal = 0,

    /**
     * The music player is in the accompany mode, which means playing the accompaniment only.
     */
    AgoraMusicPlayModeAccompany = 1,
    /**
     * 2: The music player is in the lead sing mode, which means playing the lead vocals.
     */
    AgoraMusicPlayModeLeadsing = 2,
};

/**
 * The status of preload request
 */
typedef NS_ENUM(NSUInteger, AgoraMusicContentCenterPreloadState) {
    /**
     * 0: No error occurs and preload succeeds.
     */
    AgoraMusicContentCenterPreloadStateOK = 0,

    /**
     * 1: A general error occurs.
     */
    AgoraMusicContentCenterPreloadStateError = 1,

    /**
     * 2: The media file is preloading.
     */
    AgoraMusicContentCenterPreloadStatePreloading = 2,
    
    /**
     * 3: The media file is removed.
     */
    AgoraMusicContentCenterPreloadStateRemoveCache = 3,
};

/**
 * the status of search or get top list request
 */
typedef NS_ENUM(NSUInteger, AgoraMusicContentCenterStateReason) {
    /**
     * 0: No error occurs and request succeeds.
     */
    AgoraMusicContentCenterStateReasonOK = 0,
    /**
     * 1: The gateway error. There are several possible reasons:
     *  - Token is expired. Check if your token is expired.
     *  - Token is invalid. Check the type of token you passed in.
     *  - Network error. Check your network.
     */
    AgoraMusicContentCenterStateReasonError = 1,
    /**
     * 2: The gateway error. There are several possible reasons:
     *  - Token is expired. Check if your token is expired.
     *  - Token is invalid. Check the type of token you passed in.
     *  - Network error. Check your network.
     */
    AgoraMusicContentCenterStateReasonErrorGateway = 2,
    /**
     * 3: Permission and resource error. There are several possible reasons:
     *  - Your appid may not have the mcc permission. Please contact technical support 
     *  - The resource may not exist. Please contact technical support
     */
    AgoraMusicContentCenterStateReasonErrorPermissionAndResource = 3,
    /**
     * 4: Internal data parse error. Please contact technical support
     */
    AgoraMusicContentCenterStateReasonErrorInternalDataParse = 4,
    /**
     * 5: Music loading error. Please contact technical support
     */
    AgoraMusicContentCenterStateReasonErrorMusicLoading = 5,
    /**
     * 6: Music decryption error. Please contact technical support
     */
    AgoraMusicContentCenterStateReasonErrorMusicDecryption = 6,
    /**
     * 7: Http internal error. Please retry later.
     */
    AgoraMusicContentCenterStateReasonErrorHttpInternalError = 7,
};

typedef NS_ENUM(NSUInteger, AgoraMusicCacheStatusType) {
    /**
     * 0: The media file is already cached.
     */
    AgoraMusicCacheStatusTypeCached = 0,

    /**
     * 1: The media file is being cached.
     */
    AgoraMusicCacheStatusTypeCaching = 1,
};

NS_ASSUME_NONNULL_BEGIN

__attribute__((visibility("default"))) @interface AgoraMusicCacheInfo : NSObject
/**
 * The songCode of the music
 */
@property(nonatomic, assign) NSInteger songCode;

/**
 * The cache status of the music
 */
@property(nonatomic, assign) AgoraMusicCacheStatusType statusType;
@end

__attribute__((visibility("default"))) @interface AgoraMusicChartInfo : NSObject
/**
 * Name of the music chart
 */
@property (nonatomic, copy) NSString *chartName;
/**
 * Id of the music chart, which is used to get music list
 */
@property (nonatomic, assign) NSInteger identify;
@end

__attribute__((visibility("default"))) @interface AgoraMvProperty : NSObject
/**
 * The resolution of the mv
 */
@property (nonatomic, copy) NSString *resolution;
/**
 * The bandwidth of the mv
 */
@property (nonatomic, copy) NSString *bandwidth;
@end

__attribute__((visibility("default"))) @interface AgoraClimaxSegment : NSObject
/**
 * The start time of climax segment
 */
@property(nonatomic, assign) NSInteger startTimeMs;
/**
 * The end time of climax segment
 */
@property(nonatomic, assign) NSInteger endTimeMs;
@end

/**
 * The music info
 */
__attribute__((visibility("default"))) @interface AgoraMusic : NSObject
/**
 * The songCode of music
 */
@property(nonatomic, assign) NSInteger songCode;
/**
 * The type of music
 * 1, mp3 with instrumental accompaniment and original
 * 2, mp3 only with instrumental accompaniment
 * 3, mp3 only with original
 * 4, mp4 with instrumental accompaniment and original
 * 5, mv only
 * 6, new type mp4 with instrumental accompaniment and original
 * detail at document of music media center
 */
@property(nonatomic, assign) NSInteger type;
/**
 * The pitch type of music. 
 * 1, xml lyric has pitch
 * 2, lyric has no pitch
 */
@property(nonatomic, assign) NSInteger pitchType;
/**
 * The name of music
 */
@property(nonatomic, copy) NSString* name;
/**
 * The singer of music
 */
@property(nonatomic, copy) NSString* singer;
/**
 * The poster url of music
 */
@property(nonatomic, copy) NSString* poster;

/**
 * The release time of music
 */
@property(nonatomic, copy) NSString* releaseTime;
/**
 * The duration (in seconds) of music
 */
@property(nonatomic, assign) NSInteger durationS;
/**
 * The lyric list of music
 * 0, xml
 * 1, lrc
 */
@property(nonatomic, strong) NSArray<NSNumber *>* lyricList;
/**
 * The mv property list of music
 */
@property(nonatomic, strong) NSArray<AgoraMvProperty *>* mvPropertyList;
/**
 * The climax segment list of music
 */
@property(nonatomic, strong) NSArray<AgoraClimaxSegment *>* climaxSegmentList;
@end

/**
 * The music collection info
 */
__attribute__((visibility("default"))) @interface AgoraMusicCollection : NSObject
/**
 * This page contains how many AgoraMusic object
 */
@property(nonatomic, assign) NSInteger count;
/**
 * Total number of this search result or total number of the type music sources 
 */
@property(nonatomic, assign) NSInteger total;
/**
 * This  current page number
 */
@property(nonatomic, assign) NSInteger page;
/**
 * This request page size
 */
@property(nonatomic, assign) NSInteger pageSize;
/**
 * This music list of the request result
 */
@property(nonatomic, strong) NSArray<AgoraMusic *>* musicList;
@end

/**
 * The request event delegate callback
 */
@protocol AgoraMusicContentCenterEventDelegate <NSObject>
/**
 * The music chart result callback; occurs when getMusicCharts method is called.
 * 
 * @param requestId The request id is same as that returned by getMusicCharts.
 * @param result The result of music chart collection
 * @param reason The status of the request. See MusicContentCenterStateReason
 */
- (void)onMusicChartsResult:(NSString *)requestId result:(NSArray<AgoraMusicChartInfo*> *)result reason:(AgoraMusicContentCenterStateReason)reason;

/**
 * Music collection, occurs when getMusicCollectionByMusicChartId or searchMusic method is called.
 * 
 * @param requestId The request id is the same with that returned by getMusicCollectionByMusicChartId or searchMusic
 * @param result The result of music collection
 * @param reason The status of the request. See MusicContentCenterStateReason
 */
- (void)onMusicCollectionResult:(NSString *)requestId result:(AgoraMusicCollection *)result reason:(AgoraMusicContentCenterStateReason)reason;

/**
 * Lyric url callback of getLyric, occurs when getLyric is called
 * 
 * @param requestId The request id is same as that returned by getLyric
 * @param songCode Song code
 * @param lyricUrl  The lyric url of this music
 * @param reason The status of the request. See MusicContentCenterStateReason 
 */
- (void)onLyricResult:(NSString*)requestId songCode:(NSInteger)songCode lyricUrl:(NSString* _Nullable)lyricUrl reason:(AgoraMusicContentCenterStateReason)reason;

/**
 * Simple info callback of getSongSimpleInfo, occurs when getSongSimpleInfo is called
 *
 * @param requestId The request id is same as that returned by getSongSimpleInfo.
 * @param songCode Song code
 * @param simpleInfo The metadata of the music.
 * @param reason The status of the request. See MusicContentCenterStateReason
 */
- (void)onSongSimpleInfoResult:(NSString*)requestId songCode:(NSInteger)songCode simpleInfo:(NSString* _Nullable)simpleInfo reason:(AgoraMusicContentCenterStateReason)reason;

/**
 * Preload process callback, occurs when preload is called
 *
 * @param requestId The request id is same as that returned by preload.
 * @param songCode Song code
 * @param percent Preload progress (0 ~ 100)
 * @param lyricUrl  The lyric url of this music
 * @param state Preload state; see PreloadState.
 * @param reason The status of the request. See MusicContentCenterStateReason
 */
- (void)onPreLoadEvent:(NSString*)requestId songCode:(NSInteger)songCode percent:(NSInteger)percent lyricUrl:(NSString * _Nullable)lyricUrl state:(AgoraMusicContentCenterPreloadState)state reason:(AgoraMusicContentCenterStateReason)reason;
@end


@class AgoraRtcEngineKit;
__attribute__((visibility("default"))) @interface AgoraMusicContentCenterConfig : NSObject
@property(assign, nonatomic) AgoraRtcEngineKit* _Nullable rtcEngine;
/**
 * The app ID of the project that has enabled the music content center
 */
@property (nonatomic, copy) NSString *appId;
/**
 * music content center need token to connect with server
 */
@property (nonatomic, copy) NSString *token;
/**
 * The user ID when using music content center. It can be different from that of the rtc product.
 */
@property (nonatomic, assign) NSInteger mccUid;
/**
 * The max number which the music content center caches cannot exceed 50.
 */
@property (nonatomic, assign) NSUInteger maxCacheSize;
/**
 * @technical preview
 */
@property(nonatomic, copy) NSString* mccDomain;
/**
 * Event handler to get callback result.
 */
@property(nonatomic, weak) id<AgoraMusicContentCenterEventDelegate> eventDelegate;
@end

@protocol AgoraMusicPlayerProtocol <AgoraRtcMediaPlayerProtocol>
/**
 * Open a media file with specified parameters.
 *
 * @param songCode The identifier of the media file that you want to play.
 * @param startPos The playback position (ms) of the music file.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (NSInteger)openMediaWithSongCode:(NSInteger)songCode startPos:(NSInteger)startPos NS_SWIFT_NAME(openMedia(songCode:startPos:));

/**
* Set the mode for playing songs.
* You can call this method to switch from original to accompaniment or lead vocals.
* If you do not call this method to set the mode, the SDK plays the accompaniment by default.
*
* @param model The playing mode.
* @return
* - 0: Success.
* - < 0: Failure.
*/
- (NSInteger)setPlayMode:(AgoraMusicPlayMode)mode NS_SWIFT_NAME(setPlayMode(mode:));

@end


__attribute__((visibility("default"))) @interface AgoraMusicContentCenter : NSObject

/**
 * Create an AgoraMusicContentCenter instance.
 *   
 * @param config Configurations for the AgoraMusicContentCenter instance. For details, see AgoraMusicContentCenterConfig.
 * @return An shared instance of AgoraMusicContentCenter
*/
+ (instancetype _Nullable)sharedContentCenterWithConfig:(AgoraMusicContentCenterConfig *)config NS_SWIFT_NAME(sharedContentCenter(config:));

/**
*  Renew token of music content center
*  @param token    The new token.
*  @return 
* - 0: Success.
* - < 0: Failure.
*/
- (NSInteger)renewToken:(NSString * _Nonnull)token;

/**
 * Register an event delegate, only the last delegate is working.
 * 
 * @param eventDelegate the object who need AgoraRtcMediaPlayerDelegate method to get the player information ,
 * if you want remove the delegate, just pass nil
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (NSInteger)registerEventDelegate:(id<AgoraMusicContentCenterEventDelegate> _Nullable)eventDelegate;

/**
 * Creates a music player source object and return its pointer.
 * 
 * @param delegate The object who need AgoraRtcMediaPlayerDelegate method to get the player information
 * @return
 * - The pointer to an object who realize the AgoraMusicPlayerProtocol, if the method call succeeds.
 * - The empty pointer NULL, if the method call fails.
 */
- (id<AgoraMusicPlayerProtocol> _Nullable)createMusicPlayerWithDelegate:(id<AgoraRtcMediaPlayerDelegate> _Nullable)delegate NS_SWIFT_NAME(createMusicPlayer(delegate:));

/**
 * Destroy a music player source object and return result.
 *
 * @param musicPlayer The music player.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (NSInteger)destroyMusicPlayer:(id<AgoraMusicPlayerProtocol>)musicPlayer;

/**
 * Get music chart collection of music.If the method call success, get result from the AgoraMusicContentCenterEventDelegate  - (void)onMusicChartsResult:(NSString *)requestId status:(AgoraMusicContentCenterStatusCode)status result:(NSArray<AgoraMusicChartInfo*> *)result; match the callback  "requestId" parameter to get the  request result.
 * 
 * @return The request identification
 */
- (NSString *)getMusicCharts;

/**
 * Get hot music list by hotType and page info.If the method call success, get result from the AgoraMusicContentCenterEventDelegate  - (void)onMusicCollectionResult:(NSString *)requestId status:(AgoraMusicContentCenterStateReason)status result:(AgoraMusicCollection *)result;  match the callback  "requestId" parameter to get the  request result.
 * 
 * @param musicChartId The music chart id obtained from getMusicCharts.
 * @param page The page of the music chart, starting from 1
 * @param pageSize The page size, max is 50.
 * @param jsonOption The ext param, default is null.
 * @return The request identification
 */
- (NSString *)getMusicCollectionWithMusicChartId:(NSInteger)musicChartId page:(NSInteger)page pageSize:(NSInteger)pageSize jsonOption:(NSString * _Nullable)jsonOption NS_SWIFT_NAME(getMusicCollection(musicChartId:page:pageSize:jsonOption:));

/**
 * Search music by keyword and page info. get result from the AgoraMusicContentCenterEventDelegate  - (void)onMusicCollectionResult:(NSString *)requestId status:(AgoraMusicContentCenterStateReason)status result:(AgoraMusicCollection *)result;  match the callback  "requestId" parameter to get the  request result.
 * 
 * @param keyWord The key word to search.
 * @param page The page of the music search result, starting from 1
 * @param pageSize The page size, max is 50.
 * @param jsonOption The ext param, default is null.
 * @return The request identification
 */
- (NSString *)searchMusicWithKeyWord:(NSString *)keyWord page:(NSInteger)page pageSize:(NSInteger)pageSize jsonOption:(NSString * _Nullable)jsonOption NS_SWIFT_NAME(searchMusic(keyWord:page:pageSize:jsonOption:));

/**
 * Preload a media file with specified parameters.
 * 
 * @deprecated This method is deprecated. Use preload(songCode:) instead.
 * @param songCode The identify of the media file that you want to play.
 * @param jsonOption The ext param, default is null.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (NSInteger)preloadWithSongCode:(NSInteger)songCode jsonOption:(NSString* _Nullable)jsonOption NS_SWIFT_NAME(preload(songCode:jsonOption:))  __attribute__((deprecated("Use preload(songCode:) instead.")));

/**
 * Preload a media file with specified parameters.
 *
 * @param songCode The identify of the media file that you want to play.
 * @return The request identification
 */
- (NSString *)preloadWithSongCode:(NSInteger)songCode NS_SWIFT_NAME(preload(songCode:));

/**
 * Preload a media file with specified parameters.
 *
 * @param songCode The identify of the media file that you want to play.
 * @return
 * - 0: Success.
 * - < 0: Failure.
 */
- (NSInteger)isPreloadedWithSongCode:(NSInteger)songCode NS_SWIFT_NAME(isPreloaded(songCode:));

/**
 * Remove a media file cache
 *
 * @param songCode The identifier of the media file that you want to play.
 * @return
 * - 0: Success; the cached media file is removed.
 * - < 0: Failure.
 */
- (NSInteger)removeCacheWithSongCode:(NSInteger)songCode NS_SWIFT_NAME(removeCache(songCode:));

/**
 * Get media cache files.
 *
 * @return The caches Array contains songCode and status of the music.
 */
- (NSArray *)getCaches NS_SWIFT_NAME(getCaches());

/**
 * Get internal songCodeKey from songCode and jsonOption
 *
 * @param songCode The identifier of the media file.
 * @param jsonOption An extention parameter. The default value is null. it’s a json-format string and the `key` and `value` can be customized according to your scenarios.
 * @return
 * - Internal songCode key, if the method call succeeds.
 * - The number less than zero, if the method call fails.
 */
- (NSInteger)getInternalSongCode:(NSInteger)songCode jsonOption:(NSString * _Nullable)jsonOption  NS_SWIFT_NAME(getInternalSongCode(songCode:jsonOption:));

/**
 * Get lyric of the song. get result from the AgoraMusicContentCenterEventDelegate  - (void)onLyricResult:(NSString*)requestId lyricUrl:(NSString*)lyricUrl;  match the callback  "requestId" parameter to get the  request result.
 *
 * @param songCode The identify of the media file that you want to play.
 * @param lyricType The type of the lyric file. may be 0:xml or 1:lrc.
 * @return The request identification
 */
- (NSString *)getLyricWithSongCode:(NSInteger)songCode lyricType:(NSInteger)lyricType NS_SWIFT_NAME(getLyric(songCode:lyricType:));

/**
 * Gets the metadata of a specific music. Once this method is called, the SDK triggers the onSongSimpleInfoResult callback to report the metadata of the music.
 *
 * @param songCode The identify of the media file that you want to play.
 * @return The request identification
 */
- (NSString *)getSongSimpleInfoWithSongCode:(NSInteger)songCode NS_SWIFT_NAME(getSongSimpleInfo(songCode:));

/**
 * If you want AgoraMusicContentCenterEventDelegate methods callback in the mainThread ,you should set enable YES. Default the delegate callback in subthread.
 * - `NO`: (Default)Send the delegate callback in subthread.
 * - `YES`: Send the delegate callback in mainthread.
 */
- (void)enableMainQueueDispatch:(BOOL)enabled;
 
/**
 * Destroy the shared instance of AgoraMusicContentCenter
 * 
 * @note If you call the method, you should call it brefore AgoraRtcEngineKit destroy 
 */
+ (void)destroy;


#pragma mark - Unavailable Delegate Methods
#if TARGET_OS_IPHONE
- (void)onMusicChartsResult:(NSString *)requestId result:(NSArray<AgoraMusicChartInfo*> *)result errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(ios,deprecated=7_0,message="Use onMusicChartsResult:result:statusCode: instead.")));
- (void)onMusicCollectionResult:(NSString *)requestId result:(AgoraMusicCollection *)result errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(ios,deprecated=7_0,message="Use onMusicCollectionResult:result:statusCode: instead.")));
- (void)onLyricResult:(NSString*)requestId songCode:(NSInteger)songCode lyricUrl:(NSString* _Nullable)lyricUrl errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(ios,deprecated=7_0,message="Use onLyricResult:songCode:lyricUrl:statusCode: instead.")));
- (void)onSongSimpleInfoResult:(NSString*)requestId songCode:(NSInteger)songCode simpleInfo:(NSString* _Nullable)simpleInfo errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(ios,deprecated=7_0,message="Use onSongSimpleInfoResult:songCode:simpleInfo:statusCode: instead.")));
- (void)onPreLoadEvent:(NSString*)requestId songCode:(NSInteger)songCode percent:(NSInteger)percent lyricUrl:(NSString * _Nullable)lyricUrl state:(AgoraMusicContentCenterPreloadState)state errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(ios,deprecated=7_0,message="Use onPreLoadEvent:songCode:percent:lyricUrl:status:statusCode: instead.")));
#endif

#if (!(TARGET_OS_IPHONE) && (TARGET_OS_MAC))
- (void)onMusicChartsResult:(NSString *)requestId result:(NSArray<AgoraMusicChartInfo*> *)result errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(macos,deprecated=10_9,message="Use onMusicChartsResult:result:statusCode: instead.")));
- (void)onMusicCollectionResult:(NSString *)requestId result:(AgoraMusicCollection *)result errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(macos,deprecated=10_9,message="Use onMusicCollectionResult:result:statusCode: instead.")));
- (void)onLyricResult:(NSString*)requestId songCode:(NSInteger)songCode lyricUrl:(NSString* _Nullable)lyricUrl errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(macos,deprecated=10_9,message="Use onLyricResult:songCode:lyricUrl:statusCode: instead.")));
- (void)onSongSimpleInfoResult:(NSString*)requestId songCode:(NSInteger)songCode simpleInfo:(NSString* _Nullable)simpleInfo errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(macos,deprecated=10_9,message="Use onSongSimpleInfoResult:songCode:simpleInfo:statusCode: instead.")));
- (void)onPreLoadEvent:(NSString*)requestId songCode:(NSInteger)songCode percent:(NSInteger)percent lyricUrl:(NSString * _Nullable)lyricUrl state:(AgoraMusicContentCenterPreloadState)state errorCode:(AgoraMusicContentCenterStateReason)errorCode __attribute__((availability(macos,deprecated=10_9,message="Use onPreLoadEvent:songCode:percent:lyricUrl:status:statusCode: instead.")));
#endif

@end

NS_ASSUME_NONNULL_END
