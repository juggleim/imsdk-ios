//
//  JuggleIMConst.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

/// Message pull direction.
typedef NS_ENUM(NSUInteger, JPullDirection) {
    JPullDirectionNewer = 0,
    JPullDirectionOlder = 1,
};

// Chatroom member ordering.
typedef NS_ENUM(NSUInteger, JChatroomMemberOrder) {
    // Returns the earliest joined members first.
    JChatroomMemberOrderAscending = 0,
    // Returns the latest joined members first.
    JChatroomMemberOrderDescending = 1
};

typedef NS_ENUM(NSUInteger, JConnectionStatus) {
    // Not connected.
    JConnectionStatusIdle = 0,
    // Connected.
    JConnectionStatusConnected = 1,
    // Disconnected by the user or kicked by the server. SDK will not reconnect.
    JConnectionStatusDisconnected = 2,
    // Connecting.
    JConnectionStatusConnecting = 3,
    // Connection failed. Developer can handle it based on JErrorCode.
    JConnectionStatusFailure
};

typedef NS_ENUM(NSUInteger, JErrorCode) {
    JErrorCodeNone = 0,
    // AppKey is missing.
    JErrorCodeAppKeyEmpty = 11001,
    // Token is missing.
    JErrorCodeTokenEmpty = 11002,
    // AppKey does not exist.
    JErrorCodeAppKeyInvalid = 11003,
    // Token is invalid.
    JErrorCodeTokenIllegal = 11004,
    // Token is unauthorized.
    JErrorCodeTokenUnauthorized = 11005,
    // Token has expired.
    JErrorCodeTokenExpired = 11006,
    // App is prohibited.
    JErrorCodeAppProhibited = 11009,
    // User is prohibited.
    JErrorCodeUserProhibited = 11010,
    // User was kicked offline by another client.
    JErrorCodeUserKickedByOtherClient = 11011,
    // User logged out.
    JErrorCodeUserLogOut = 11012,
    
    // Not a friend.
    JErrorCodeNotFriend = 12009,
    // No operation permission.
    JErrorCodeNoOperationPermission = 12010,
    // Message does not exist.
    JErrorCodeRemoteMessageNotExist = 12011,
    // Duplicate favorite message.
    JErrorCodeAddDuplicateFavoriteMessage = 12012,
    // E2EE keys hash mismatch.
    JErrorCodePubKeysHashMismatch = 12013,
    
    // Group does not exist.
    JErrorCodeGroupNotExist = 13001,
    // Not a group member.
    JErrorCodeNotGroupMember = 13002,
    
    // Chatroom default error.
    JErrorCodeChatroomUnknownError = 14000,
    // Not a chatroom member.
    JErrorCodeNotChatroomMember = 14001,
    // Chatroom attributes are full (maximum 100).
    JErrorCodeChatroomAttributeCountExceed = 14002,
    // No permission to operate this chatroom attribute (the key was not set by the current user).
    JErrorCodeChatroomKeyUnauthorized = 14003,
    // Chatroom attribute does not exist.
    JErrorCodeChatroomAttributeNotExist = 14004,
    // Chatroom does not exist.
    JErrorCodeChatroomNotExist = 14005,
    // Chatroom has been destroyed.
    JErrorCodeChatroomDestroyed = 14006,
    
    // Connection is forbidden.
    JErrorCodeConnectForbidden = 21000,
    // Invalid parameter.
    JErrorCodeInvalidParam = 21003,
    // Operation timed out.
    JErrorCodeOperationTimeOut = 21004,
    // Connection is unavailable.
    JErrorCodeConnectionUnavailable = 21005,
    // Server setting error.
    JErrorCodeServerSetError = 21006,
    // Connection already exists.
    JErrorCodeConnectionAlreadyExist = 21007,
    // Moment request failed.
    JErrorCodeMomentRequestError = 21008,
    // Friend does not exist.
    JErrorCodeFriendNotExist = 21009,
    
    // Message does not exist.
    JErrorCodeMessageNotExist = 22001,
    // Message has already been recalled.
    JErrorCodeMessageAlreadyRecalled = 22002,
    // Message upload failed.
    JErrorCodeMessageUploadError = 22003,
    // The key and value in recalled message extras are not NSString.
    JErrorCodeRecallExtrasTypeNotString = 22004,
    // Peer public key was not found.
    JErrorCodeOtherSideE2EEInvalid = 22006,
    // Local public key has not been generated.
    JErrorCodeLocalPublicKeyInvalid = 22007,
    
    // The downloaded message is not a media message.
    JErrorCodeDownloadNotMediaMessage = 23001,
    // Message download failed.
    JErrorCodeMessageDownloadError = 23006,
    // Message download already exists.
    JErrorCodeDownloadAlreadyExist = 23008,
    // Message download was canceled.
    JErrorCodeDownloadCanceled = 23009,
    
    // Failed to batch set chatroom attributes.
    JErrorCodeChatroomBatchSetAttributeFail = 24001,
    
    // Failed to join the LiveKit room.
    JErrorCodeJoinLiveKitFail = 25001
};

/*!
 Current network status.
 */
typedef NS_ENUM(NSUInteger, JNetworkStatus) {
    /*!
     The network is unavailable.
     */
    JNetworkStatusNotReachable = 0,

    /*!
     The current network is Wi-Fi.
     */
    JNetworkStatusReachableViaWiFi = 1,

    /*!
     Cellular network.
     */
    JNetworkStatusReachableViaWWAN = 2,
};

typedef NS_ENUM(NSUInteger, JLogLevel) {
    JLogLevelNone = 0,
    JLogLevelFatal = 1,
    JLogLevelError = 2,
    JLogLevelWarning = 3,
    JLogLevelInfo = 4,
    JLogLevelDebug = 5,
    JLogLevelVerbose = 6
};

// Pinned conversation sorting rule.
typedef NS_ENUM(NSUInteger, JTopConversationsOrderType) {
    // Sort by pinned time.
    JTopConversationsOrderByTopTime,
    // Sort by conversation message time.
    JTopConversationsOrderByMessageTime
};
