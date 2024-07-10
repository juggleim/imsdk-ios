//
//  JUploadEnum.h
//  JetIM
//
//  Created by 郑开 on 2024/7/9.
//

#ifndef JUploadEnum_h
#define JUploadEnum_h

//上传类型
typedef NS_ENUM(NSInteger, JUploadFileType) {
    JUploadFileType_Default = 0,    //默认
    JUploadFileType_Image = 1,      //图片
    JUploadFileType_Audio = 2,      //音频
    JUploadFileType_Video = 3,      //视频
    JUploadFileType_File = 4        //文件
};

//上传通道类型
typedef NS_ENUM(NSInteger, JUploadOssType) {
    JUploadOssType_Default = 0,     //默认
    JUploadOssType_QiNiu = 1,       //七牛
    JUploadOssType_S3 = 2,          //S3
    JUploadOssType_MinIO = 3,      //MinIO
    JUploadOssType_OSS = 4          //OSS
};

#endif /* JUploadEnum_h */
