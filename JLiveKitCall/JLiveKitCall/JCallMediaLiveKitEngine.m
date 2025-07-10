//
//  JCallMediaLiveKitEngine.m
//  JLiveKitCall
//
//  Created by Fei Li on 2025/7/7.
//

#import "JCallMediaLiveKitEngine.h"
#import <LiveKitClient/LiveKitClient-Swift.h>
#import <JuggleIM/JuggleIMConst.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

@interface JCallMediaLiveKitEngine () <RoomDelegate>
@property (nonatomic, strong) Room *room;
@property (nonatomic, weak) id<JCallMediaEngineDelegate> delegate;
@property (nonatomic, assign) BOOL enableCamera;
@property (nonatomic, strong) NSMapTable *viewTable;
@property (nonatomic, strong) VideoView *previewVideoView;
@property (nonatomic, strong) LocalVideoTrack *frontTrack;
@property (nonatomic, strong) LocalVideoTrack *backTrack;
@end

@implementation JCallMediaLiveKitEngine

#pragma mark - JCallMediaEnginProtocol
- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int, NSDictionary *))completeBlock {
    [self.room connectWithUrl:config.url
                        token:config.token
               connectOptions:nil
                  roomOptions:nil
            completionHandler:^(NSError * _Nullable error) {
        int errorCode = 0;
        if (!error) {
            [self.room.localParticipant setMicrophoneWithEnabled:YES
                                                  captureOptions:nil
                                                  publishOptions:nil
                                               completionHandler:^(LocalTrackPublication * _Nullable publication, NSError * _Nullable error) {
                if (error) {
                    NSLog(@"setMicrophoneWithEnabled after connect error, %@", error);
                }
            }];
            if (self.enableCamera) {
                CameraCaptureOptions *options = [[CameraCaptureOptions alloc] initWithDeviceType:nil
                                                                                          device:nil
                                                                                        position:AVCaptureDevicePositionFront
                                                                                 preferredFormat:nil
                                                                                      dimensions:[[Dimensions alloc] initWithWidth:1280 height:720]
                                                                                             fps:30];
                [self.room.localParticipant setCameraWithEnabled:YES
                                                  captureOptions:options
                                                  publishOptions:nil
                                               completionHandler:^(LocalTrackPublication * _Nullable publication, NSError * _Nullable error) {
                    if (error) {
                        NSLog(@"setCameraWithEnabled after connect error, %@", error);
                    }
                }];
            }
        } else {
            NSLog(@"join livekit room error, %@", error);
            errorCode = JErrorCodeJoinLiveKitFail;
        }
        if (completeBlock) {
            completeBlock(errorCode, nil);
        }
    }];
}

- (void)enableCamera:(BOOL)isEnable {
    self.enableCamera = isEnable;
    if (self.room.connectionState == ConnectionStateConnected) {
        [self.room.localParticipant setCameraWithEnabled:isEnable captureOptions:nil publishOptions:nil completionHandler:^(LocalTrackPublication * _Nullable publication, NSError * _Nullable error) {
                    
        }];
    }
}

- (void)leaveRoom:(NSString *)roomId { 
    [self.room disconnectWithCompletionHandler:^{
    }];
}

- (void)muteMicrophone:(BOOL)isMute { 
    [self.room.localParticipant setMicrophoneWithEnabled:!isMute
                                          captureOptions:nil
                                          publishOptions:nil
                                       completionHandler:^(LocalTrackPublication * _Nullable publication, NSError * _Nullable error) {
        if (error) {
            NSLog(@"muteMicrophone error, %@", error);
        }
    }];
}

- (void)muteSpeaker:(BOOL)isMute { 
    //TODO: 上层实现
}

- (void)setSpeakerEnable:(BOOL)isEnable { 
    //TODO: 上层实现
}

- (void)setVideoView:(UIView *)view roomId:(NSString *)roomId userId:(NSString *)userId {
    dispatch_async(dispatch_get_main_queue(), ^{
        VideoView *videoView = [self.viewTable objectForKey:userId];
        if (!videoView) {
            return;
        }
        for (UIView *subView in view.subviews) {
            [subView removeFromSuperview];
        }
        videoView.frame = view.bounds;
        [view addSubview:videoView];
    });
}

- (void)startPreview:(UIView *)view {
    dispatch_async(dispatch_get_main_queue(), ^{
        for (UIView *subView in view.subviews) {
            [subView removeFromSuperview];
        }
        self.previewVideoView.frame = view.bounds;
        [view addSubview:self.previewVideoView];
//        [self.frontTrack startWithCompletionHandler:^(NSError * _Nullable error) {
//            dispatch_async(dispatch_get_main_queue(), ^{
//                self.previewView.frame = view.frame;
//            });
//        }];
//        self.previewView.track = self.frontTrack;
    });
}

- (void)stopPreview {
//    [self.frontTrack stopWithCompletionHandler:^(NSError * _Nullable error) {
//    }];
}

- (void)useFrontCamera:(BOOL)isEnable {
    LocalVideoTrack *track = (LocalVideoTrack *)self.room.localParticipant.videoTracks[0].track;
    CameraCapturer *capturer = (CameraCapturer *)track.capturer;
    [capturer switchCameraPositionWithCompletionHandler:^(BOOL b, NSError * _Nullable error) {
    }];
}

#pragma mark - RoomDelegate
- (void)roomDidConnect:(Room *)room {
}

- (void)room:(Room *)room didDisconnectWithError:(LiveKitError *)error {
    [self.viewTable removeAllObjects];
    self.previewVideoView = nil;
}

- (void)room:(Room *)room localParticipant:(LocalParticipant *)participant didPublishTrack:(LocalTrackPublication *)publication {
    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (publication.track.kind == TrackKindVideo) {
            if ([weakSelf.delegate respondsToSelector:@selector(viewForSelf)]) {
                UIView *view = [weakSelf.delegate viewForSelf];
                if (!view) {
                    return;
                }
                for (UIView *subView in view.subviews) {
                    [subView removeFromSuperview];
                }
                weakSelf.previewVideoView.frame = view.bounds;
                [weakSelf.viewTable setObject:weakSelf.previewVideoView forKey:participant.identity.stringValue];
                [view addSubview:weakSelf.previewVideoView];
            }
            weakSelf.previewVideoView.track = (id<VideoTrack>)publication.track;
        }
    });
}

- (void)room:(Room *)room participant:(RemoteParticipant *)participant didSubscribeTrack:(RemoteTrackPublication *)publication {
    if (publication.track.kind == TrackKindVideo) {
        dispatch_async(dispatch_get_main_queue(), ^{
            VideoView *videoView = [[VideoView alloc] initWithFrame:CGRectZero];
            videoView.translatesAutoresizingMaskIntoConstraints = NO;
            if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
                UIView *view = [self.delegate viewForUserId:participant.identity.stringValue];
                for (UIView *subView in view.subviews) {
                    [subView removeFromSuperview];
                }
                videoView.frame = view.frame;
                [view addSubview:videoView];
            }
            videoView.track = (id<VideoTrack>)publication.track;
            [self.viewTable setObject:videoView forKey:participant.identity.stringValue];
        });
    }
}

- (void)room:(Room *)room participantDidConnect:(RemoteParticipant *)participant {
    if ([self.delegate respondsToSelector:@selector(usersDidJoin:)]) {
        [self.delegate usersDidJoin:@[participant.identity.stringValue]];
    }
}

#pragma mark -
- (Room *)room {
    if (!_room) {
        _room = [[Room alloc] init];
        [_room addDelegate:self];
    }
    return _room;
}

- (NSMapTable *)viewTable {
    if (!_viewTable) {
        _viewTable = [NSMapTable strongToWeakObjectsMapTable];
    }
    return _viewTable;
}

- (LocalVideoTrack *)frontTrack {
    if (!_frontTrack) {
        CameraCaptureOptions *options = [[CameraCaptureOptions alloc] initWithDeviceType:nil
                                                                                  device:nil
                                                                                position:AVCaptureDevicePositionFront
                                                                         preferredFormat:nil
                                                                              dimensions:[[Dimensions alloc] initWithWidth:1280 height:720]
                                                                                     fps:30];
        _frontTrack = [LocalVideoTrack createCameraTrackWithName:nil
                                                         options:options
                                                reportStatistics:NO];
    }
    return _frontTrack;
}

- (LocalVideoTrack *)backTrack {
    if (!_backTrack) {
        CameraCaptureOptions *options = [[CameraCaptureOptions alloc] initWithDeviceType:nil
                                                                                  device:nil
                                                                                position:AVCaptureDevicePositionBack
                                                                         preferredFormat:nil
                                                                              dimensions:[[Dimensions alloc] initWithWidth:1280 height:720]
                                                                                     fps:30];
        _backTrack = [LocalVideoTrack createCameraTrackWithName:nil
                                                        options:options
                                               reportStatistics:NO];
    }
    return _backTrack;
}

- (VideoView *)previewVideoView {
    if (!_previewVideoView) {
        _previewVideoView = [[VideoView alloc] initWithFrame:CGRectZero];
        _previewVideoView.translatesAutoresizingMaskIntoConstraints = NO;
    }
    return _previewVideoView;
}

@end
