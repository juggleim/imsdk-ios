//
//  JuggleIMConstInternal.h
// JuggleIM
//
//  Created by Nathan on 2023/12/1.
//

#define JPlatform @"iOS"
#define JDeviceCompany @"Apple"
#define JIMVersion @"1.9.2"
#define jNaviAppKey @"x-appkey"
#define jNaviToken @"x-token"

#define JThumbnailWidth 240
#define JThumbnailHeight 240
#define jThumbnailQuality 0.3

typedef NS_ENUM(NSUInteger, JErrorCodeInternal) {
    JErrorCodeInternalNone = 0,
    // AppKey is missing.
    JErrorCodeInternalAppKeyEmpty = 11001,
    // Token is missing.
    JErrorCodeInternalTokenEmpty = 11002,
    // AppKey does not exist.
    JErrorCodeInternalAppKeyInvalid = 11003,
    // Token is invalid.
    JErrorCodeInternalTokenIllegal = 11004,
    // Token is unauthorized.
    JErrorCodeInternalTokenUnauthorized = 11005,
    // Token has expired.
    JErrorCodeInternalTokenExpired = 11006,
    // App is prohibited.
    JErrorCodeInternalAppProhibited = 11009,
    // User is prohibited.
    JErrorCodeInternalUserProhibited = 11010,
    // User was kicked offline by another client.
    JErrorCodeInternalUserKickedByOtherClient = 11011,
    // User logged out.
    JErrorCodeInternalUserLogOut = 11012,
    
    // Not a friend relationship.
    JErrorCodeInternalNotFriend = 12009,
    // No operation permission.
    JErrorCodeInternalNoOperationPermission = 12010,
    // Message does not exist.
    JErrorCodeInternalRemoteMessageNotExist = 12011,
    // Duplicate favorite message.
    JErrorCodeInternalAddDuplicateFavoriteMessage = 12012,
    // E2EE keys hash mismatch. Refresh the public key list from the server.
    JErrorCodeInternalPubKeysHashMismatch = 12013,
    
    // Group does not exist.
    JErrorCodeInternalGroupNotExist = 13001,
    // Not a group member.
    JErrorCodeInternalNotGroupMember = 13002,
    
    // Chatroom does not exist.
    JErrorCodeInternalChatroomNotExist = 14005,
    // Chatroom has been destroyed.
    JErrorCodeInternalChatroomDestroyed = 14006,
    
    // There is an unfinished call in the conversation.
    JErrorCodeInternalCallConversationBinded = 16011,
    
    // Connection is forbidden.
    JErrorCodeInternalConnectForbidden = 21000,
    JErrorCodeInternalWebSocketFailure = 21001,
    JErrorCodeInternalNaviFailure = 21002,
    JErrorCodeInternalInvalidParam = 21003,
    // Operation timed out.
    JErrorCodeInternalOperationTimeOut = 21004,
    // Connection is unavailable.
    JErrorCodeInternalConnectionUnavailable = 21005,
    // Server setting error.
    JErrorCodeInternalServerSetError = 21006,
    // Connection already exists.
    JErrorCodeInternalConnectionAlreadyExist = 21007,
    // Friend does not exist.
    JErrorCodeInternalFriendNotExist = 21009,
    
    // Message does not exist.
    JErrorCodeInternalMessageNotExist = 22001,
    // Message has already been recalled.
    JErrorCodeInternalMessageAlreadyRecalled = 22002,
    // Message upload failed.
    JErrorCodeInternalMessageUploadError = 22003,
    // The key and value in recalled message extras are not NSString.
    JErrorCodeInternalRecallExtrasTypeNotString = 22004,
    // Log does not exist.
    JErrorCodeInternalLogNotExist = 22005,
    // Peer public key was not found.
    JErrorCodeInternalOtherSideE2EEInvalid = 22006,
    // Local public key has not been generated.
    JErrorCodeInternalLocalPublicKeyInvalid = 22007,
    
    // Failed to batch set chatroom attributes.
    JErrorCodeInternalChatroomBatchSetAttributeFail = 24001,
    
    // Failed to join the LiveKit room.
    JErrorCodeInternalJoinLiveKitFail = 25001
};

typedef NS_ENUM(NSUInteger, JMediaType) {
    JMediaTypeFile,
    JMediaTypeImage,
    JMediaTypeVoice,
    JMediaTypeVideo
};
