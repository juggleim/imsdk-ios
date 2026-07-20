//
//  JUploadEnum.h
// JuggleIM
//
//  Created by Zheng Kai on 2024/7/9.
//

#ifndef JUploadEnum_h
#define JUploadEnum_h

//Upload type
typedef NS_ENUM(NSInteger, JUploadFileType) {
    JUploadFileType_Default = 0,    //Default
    JUploadFileType_Image = 1,      //Image
    JUploadFileType_Audio = 2,      //Audio
    JUploadFileType_Video = 3,      //Video
    JUploadFileType_File = 4,       //File
    JUploadFileType_Log = 5         //Log
};

//Upload channel type
typedef NS_ENUM(NSInteger, JUploadOssType) {
    JUploadOssType_Default = 0,     //Default
    JUploadOssType_QiNiu = 1,       //Qiniu
    JUploadOssType_S3 = 2,          //S3
    JUploadOssType_MinIO = 3,      //MinIO
    JUploadOssType_OSS = 4          //OSS
};

#endif /* JUploadEnum_h */
