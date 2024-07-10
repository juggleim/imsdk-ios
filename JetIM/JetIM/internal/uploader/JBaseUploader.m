//
//  JBaseUploader.m
//  JetIM
//
//  Created by 郑开 on 2024/7/10.
//

#import "JBaseUploader.h"

@implementation JBaseUploader

- (instancetype)init{
    self = [super init];
    if (self) {
        self.progressCallBackInterval = 500;
        self.lastProgressCallBackTime = 0;
    }
    return self;
}

-(void)star{
    
}

-(void)cancel{
    
}

-(void)notifyProgress:(int)progress{
    long long currentTime =  [[NSDate date] timeIntervalSince1970]*1000;
    if(currentTime - self.lastProgressCallBackTime >= self.progressCallBackInterval){
        self.lastProgressCallBackTime = currentTime;
        if(self.JUploadProgress){
            self.JUploadProgress(progress);
        }
    }
}

-(void)notifySuccess:(NSString *)url{
    if(self.JUploadSuccess){
        self.JUploadSuccess(url);
    }
}

-(void)notifyFail{
    if(self.JUploadError){
        self.JUploadError();
    }
}

-(void)notifyCancel{
    if(self.JUploadCancel){
        self.JUploadCancel();
    }
}
@end
