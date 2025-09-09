//
//  JCallMediaAgoraEngine.m
//  JAgoraCall
//
//  Created by Fei Li on 2025/9/8.
//

#import "JCallMediaAgoraEngine.h"
#import <AgoraRtcKit/AgoraRtcKit.h>
#import <JuggleIM/JIM.h>

@interface JCallMediaAgoraEngine () <AgoraRtcEngineDelegate>
@property (nonatomic, strong) AgoraRtcEngineKit *engine;
@property (nonatomic, weak) id<JCallMediaEngineDelegate> delegate;
@property (nonatomic, copy) NSMutableDictionary <NSString *, NSNumber *> *userDic;
@end

@implementation JCallMediaAgoraEngine

- (void)createAgoraEngineWith:(NSString *)appId {
    [AgoraRtcEngineKit destroy];
    self.engine = [AgoraRtcEngineKit sharedEngineWithAppId:appId delegate:self];
    [self.engine disableVideo];
}

#pragma mark - JCallMediaEnginProtocol
- (void)enableCamera:(BOOL)isEnable {
    if (isEnable) {
        [self.engine enableVideo];
    } else {
        [self.engine disableVideo];
    }
}

- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int, NSDictionary *))completeBlock {
    AgoraRtcChannelMediaOptions *o = [[AgoraRtcChannelMediaOptions alloc] init];
    o.channelProfile = AgoraChannelProfileLiveBroadcasting;
    o.clientRoleType = AgoraClientRoleBroadcaster;
    o.publishMicrophoneTrack = YES;
    o.publishCameraTrack = NO;
    o.autoSubscribeAudio = YES;
    o.autoSubscribeVideo = YES;
    
    int errorCode = [self.engine joinChannelByToken:config.token
                                          channelId:room.roomId
                                        userAccount:user.userId
                                       mediaOptions:o
                                        joinSuccess:^(NSString * _Nonnull channel, NSUInteger uid, NSInteger elapsed) {
        [self.userDic removeAllObjects];
        if (completeBlock) {
            completeBlock(0, nil);
        }
    }];
    if (errorCode != 0 && completeBlock) {
        completeBlock(errorCode, nil);
    }
}


- (void)leaveRoom:(NSString *)roomId { 
    [self.engine leaveChannel:nil];
}


- (void)muteMicrophone:(BOOL)isMute { 
    [self.engine disableAudio];
}


- (void)muteSpeaker:(BOOL)isMute { 
    [self.engine muteAllRemoteAudioStreams:YES];
}


- (void)setSpeakerEnable:(BOOL)isEnable { 
    [self.engine setEnableSpeakerphone:isEnable];
}


- (void)setVideoView:(UIView *)view roomId:(NSString *)roomId userId:(NSString *)userId { 
    if (roomId.length == 0 || userId.length == 0 || !view) {
        return;
    }
    [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull innerUserId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
        if ([innerUserId isEqualToString:userId]) {
            NSUInteger uid = [uidNumber unsignedIntegerValue];
            AgoraRtcVideoCanvas *canvas = [AgoraRtcVideoCanvas new];
            canvas.uid = uid;
            canvas.view = view;
            [self.engine setupRemoteVideo:canvas];
            *stop = YES;
        }
    }];
}


- (void)startPreview:(UIView *)view { 
    AgoraRtcVideoCanvas *canvas = [AgoraRtcVideoCanvas new];
    canvas.view = view;
    [self.engine setupLocalVideo:canvas];
    [self.engine startPreview];
}


- (void)stopPreview { 
    [self.engine stopPreview];
}


- (void)useFrontCamera:(BOOL)isEnable { 
    [self.engine switchCamera];
}

#pragma mark - AgoraRtcEngineDelegate
- (void)rtcEngine:(AgoraRtcEngineKit *)engine didUserInfoUpdatedWithUserId:(NSUInteger)uid userInfo:(AgoraUserInfo *)userInfo {
    [self.userDic setObject:@(uid) forKey:userInfo.userAccount];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didJoinedOfUid:(NSUInteger)uid elapsed:(NSInteger)elapsed {
    [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull userId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
        if (uid == [uidNumber unsignedIntegerValue]) {
            if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
                UIView *view = [self.delegate viewForUserId:userId];
                AgoraRtcVideoCanvas *canvas = [AgoraRtcVideoCanvas new];
                canvas.view = view;
                canvas.uid = uid;
                [self.engine setupRemoteVideo:canvas];
            }
            if ([self.delegate respondsToSelector:@selector(usersDidConnect:)]) {
                [self.delegate usersDidConnect:@[userId]];
            }
        }
    }];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine remoteVideoStateChangedOfUid:(NSUInteger)uid state:(AgoraVideoRemoteState)state reason:(AgoraVideoRemoteReason)reason elapsed:(NSInteger)elapsed {
    [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull userId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
        if (uid == [uidNumber unsignedIntegerValue]) {
            if ([self.delegate respondsToSelector:@selector(userCamaraDidChange:userId:)]) {
                if (state == AgoraVideoRemoteStateStarting) {
                    [self.delegate userCamaraDidChange:YES userId:userId];
                } else if (state == AgoraVideoRemoteStateStopped) {
                    [self.delegate userCamaraDidChange:NO userId:userId];
                }
            }
            *stop = YES;
        }
    }];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didAudioMuted:(BOOL)muted byUid:(NSUInteger)uid {
    [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull userId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
        if (uid == [uidNumber unsignedIntegerValue]) {
            if ([self.delegate respondsToSelector:@selector(userMicrophoneDidChange:userId:)]) {
                if (muted) {
                    [self.delegate userMicrophoneDidChange:NO userId:userId];
                } else {
                    [self.delegate userMicrophoneDidChange:YES userId:userId];
                }
            }
            *stop = YES;
        }
    }];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine reportAudioVolumeIndicationOfSpeakers:(NSArray<AgoraRtcAudioVolumeInfo *> *)speakers totalVolume:(NSInteger)totalVolume {
    NSMutableDictionary<NSString *, NSNumber *> *resultDic = [NSMutableDictionary dictionary];
    for (AgoraRtcAudioVolumeInfo *info in speakers) {
        NSUInteger volume = info.volume * 100 / 255;
        if (info.uid == 0) {
            NSString *userId = JIM.shared.currentUserId;
            if (userId.length > 0) {
                [resultDic setObject:@(volume) forKey:userId];
            }
        } else {
            [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull userId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
                if (info.uid == [uidNumber unsignedIntegerValue]) {
                    NSUInteger volume = info.volume * 100 / 255;
                    [resultDic setObject:@(volume) forKey:userId];
                    *stop = YES;
                }
            }];
        }
    }
    if (resultDic.count > 0 &&
        [self.delegate respondsToSelector:@selector(soundLevelDidUpdate:)]) {
        [self.delegate soundLevelDidUpdate:resultDic];
    }
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine firstRemoteVideoFrameOfUid:(NSUInteger)uid size:(CGSize)size elapsed:(NSInteger)elapsed {
    [self.userDic enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull userId, NSNumber * _Nonnull uidNumber, BOOL * _Nonnull stop) {
        if (uid == [uidNumber unsignedIntegerValue]) {
            if ([self.delegate respondsToSelector:@selector(videoFirstFrameDidRender:)]) {
                [self.delegate videoFirstFrameDidRender:userId];
            }
        }
        *stop = YES;
    }];
}

@end
