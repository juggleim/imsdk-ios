//
//  JuggleIM.h
//  JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>

//! Project version number for JuggleIM.
FOUNDATION_EXPORT double JuggleIMVersionNumber;

//! Project version string for JuggleIM.
FOUNDATION_EXPORT const unsigned char JuggleIMVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <JuggleIM/PublicHeader.h>


#import <JuggleIM/JIM.h>
#import <JuggleIM/JMediaMessageContent.h>
#import <JuggleIM/JTextMessage.h>
#import <JuggleIM/JImageMessage.h>
#import <JuggleIM/JFileMessage.h>
#import <JuggleIM/JVoiceMessage.h>
#import <JuggleIM/JVideoMessage.h>
#import <JuggleIM/JRecallInfoMessage.h>
#import <JuggleIM/JMergeMessage.h>
#import <JuggleIM/JThumbnailPackedImageMessage.h>
#import <JuggleIM/JSnapshotPackedVideoMessage.h>
#import <JuggleIM/JMessageOptions.h>
#import <JuggleIM/JConversationMentionInfo.h>
#import <JuggleIM/JMessageUploadProvider.h>
#import <JuggleIM/JMessageMentionInfo.h>
#import <JuggleIM/JUserInfoProtocol.h>
#import <JuggleIM/JGroupInfo.h>
#import <JuggleIM/JUserInfo.h>
#import <JuggleIM/JGroupMessageReadInfo.h>
#import <JuggleIM/JConnectionProtocol.h>
#import <JuggleIM/JConversationProtocol.h>
#import <JuggleIM/JConversation.h>
#import <JuggleIM/JConversationInfo.h>
#import <JuggleIM/JMessage.h>
#import <JuggleIM/JMessageProtocol.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JMessageContent.h>
#import <JuggleIM/JChatroomInfo.h>
#import <JuggleIM/JChatroomProtocol.h>
#import <JuggleIM/JChatroomMember.h>
#import <JuggleIM/JChatroomInfoOptions.h>
#import <JuggleIM/JTimePeriod.h>
#import <JuggleIM/JGetMessageOptions.h>
#import <JuggleIM/JQueryMessageOptions.h>
#import <JuggleIM/JPushData.h>
#import <JuggleIM/JSearchConversationsResult.h>
#import <JuggleIM/JCallProtocol.h>
#import <JuggleIM/JCallMediaRoom.h>
#import <JuggleIM/JCallMediaUser.h>
#import <JuggleIM/JCallMediaEngineProtocol.h>
#import <JuggleIM/JCallMediaRoomConfig.h>
#import <JuggleIM/JCallSession.h>
#import <JuggleIM/JCallInfo.h>
#import <JuggleIM/JCallFinishNotifyMessage.h>
#import <JuggleIM/JMessageReactionItem.h>
#import <JuggleIM/JMessageReaction.h>
#import <JuggleIM/JGroupMember.h>
#import <JuggleIM/JGetConversationOptions.h>
#import <JuggleIM/JUnknownMessage.h>
#import <JuggleIM/JGetFavoriteMessageOption.h>
#import <JuggleIM/JFavoriteMessage.h>
#import <JuggleIM/JUltEncryptProtocol.h>
#import <JuggleIM/JGroupMessageMemberReadDetail.h>
#import <JuggleIM/JGroupMessageReadInfoDetail.h>
#import <JuggleIM/JPostProtocol.h>
#import <JuggleIM/JPost.h>
#import <JuggleIM/JPostMedia.h>
#import <JuggleIM/JPostReaction.h>
#import <JuggleIM/JPostComment.h>
