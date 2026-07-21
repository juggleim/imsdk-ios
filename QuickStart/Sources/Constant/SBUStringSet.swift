//
//  SBUStringSet.swift
//  SendbirdUIKit
//
//  Created by Tez Park on 05/03/2020.
//  Copyright © 2020 Sendbird, Inc. All rights reserved.
//
//  swiftlint:disable missing_docs
//  swiftlint:disable identifier_name

import UIKit
import JuggleIM

public class SBUStringSet {
    // MARK: - Common
    public static var Cancel = NSLocalizedString("Cancel", comment: "")
    public static var OK = NSLocalizedString("Confirm", comment: "")
    public static var Retry = NSLocalizedString("Retry", comment: "")
    public static var Save = NSLocalizedString("Save", comment: "")
    public static var Copy = NSLocalizedString("Copy", comment: "")
    public static var Delete = NSLocalizedString("Delete", comment: "")
    public static var Forward = NSLocalizedString("Forward", comment: "")
    public static var Recall = NSLocalizedString("Recall", comment: "")
    public static var Edit = NSLocalizedString("Edit", comment: "")
    public static var Remove = NSLocalizedString("Remove", comment: "")
    public static var Camera = NSLocalizedString("Camera", comment: "")
    public static var PhotoVideoLibrary = NSLocalizedString("Photos", comment: "")
    public static var Document = NSLocalizedString("Files", comment: "")
    public static var VoiceCall = NSLocalizedString("Voice Call", comment: "")
    public static var VideoCall = NSLocalizedString("Video Call", comment: "")
    public static var Loading = NSLocalizedString("Loading...", comment: "")
    public static var Invite = NSLocalizedString("Invite", comment: "")
    public static var TakePhoto = NSLocalizedString("Take photo", comment: "")
    public static var ChoosePhoto = NSLocalizedString("Choose photo", comment: "")
    public static var RemovePhoto = NSLocalizedString("Remove photo", comment: "")
    public static var ViewLibrary = NSLocalizedString("View library", comment: "")
    public static var Search = NSLocalizedString("Search", comment: "")
    public static var Settings = NSLocalizedString("Settings", comment: "")
    public static var Reply = NSLocalizedString("Reply", comment: "")
    public static var Submit = NSLocalizedString("Submit", comment: "") // 3.11.0
    
    // MARK: - Alert
    public static var Alert_Delete = NSLocalizedString("Delete this message?", comment: "") //"Are you sure you want to delete?"
    public static var Alert_Recall = NSLocalizedString("Recall this message?", comment: "") //"Are you sure you want to recall?"
    public static var Alert_Delete_MultipleFilesMessage: (Int) -> String = {
        return String(format: NSLocalizedString("Do you want to delete all %d photos?", comment: ""), $0)
    }
    public static var Alert_Allow_Camera_Access = NSLocalizedString("Please allow camera usage from settings", comment: "")
    public static var Alert_Allow_PhotoLibrary_Access = NSLocalizedString("Please Allow PhotoLibrary Access", comment: "")
    public static var Alert_Allow_PhotoLibrary_Access_Message = NSLocalizedString("PhotoLibrary access required to get your photos and videos", comment: "")
    /// A text used to ask the user permission for microphone usage.
    public static var Alert_Allow_Microphone_Access = NSLocalizedString("Please allow microphone usage from settings", comment: "")

    // MARK: - Date Format
    public static var Date_Yesterday = NSLocalizedString("Yesterday", comment: "")
    public static var Date_Year: (Int) -> String = { interval in
        let format = NSLocalizedString((interval > 1) ? "%lld years" : "%lld year", comment: "")
        return String.localizedStringWithFormat(format, interval)
    }
    public static var Date_Day: (Int) -> String = { interval in
        let format = NSLocalizedString((interval > 1) ? "%lld days" : "%lld day", comment: "")
        return String.localizedStringWithFormat(format, interval)
    }
    public static var Date_Month: (Int) -> String = { interval in
        let format = NSLocalizedString((interval > 1) ? "%lld months" : "%lld month", comment: "")
        return String.localizedStringWithFormat(format, interval)
    }
    public static var Date_Hour: (Int) -> String = { interval in
        let format = NSLocalizedString("%lld h", comment: "")
        return String.localizedStringWithFormat(format, interval)
    }
    public static var Date_Min: (Int) -> String = { interval in
        let format = NSLocalizedString("%lld min", comment: "")
        return String.localizedStringWithFormat(format, interval)
    }
    public static var Date_Ago = NSLocalizedString("ago", comment: "")
    public static var Date_On = NSLocalizedString("on", comment: "")

    // MARK: - Channel List
    public static var ChannelList_Header_Title = NSLocalizedString("Channels", comment: "")
    public static var ChannelList_Last_File_Message = NSLocalizedString("uploaded a file.", comment: "")

    // MARK: - Channel
    public static var Channel_Name_Default = NSLocalizedString("Group Channel", comment: "")
    public static var Channel_Name_No_Members = NSLocalizedString("(No members)", comment: "")
    public static var Channel_Header_LastSeen = NSLocalizedString("Last seen", comment: "")
    
    @available(*, deprecated, renamed: "Channel_Typing") // 3.0.0
    public static var Channel_Header_Typing: ([JUserInfo]) -> String {
        { Channel_Typing($0) }
    }
    
    public static var Channel_Typing: ([JUserInfo]) -> String = { members in
        var nickname1 = "Member"
        var nickname2 = "Member"
        
        switch members.count {
        case 1:
            if let userName = members[0].userName {
                nickname1 = userName
            }
            return String.localizedStringWithFormat(NSLocalizedString("%@ is typing...", comment: ""), nickname1)
        case 2:
            if let userName1 = members[0].userName {
                nickname1 = userName1
            }
            if let userName2 = members[1].userName {
                nickname2 = userName2
            }
            return String.localizedStringWithFormat(NSLocalizedString("%@ and %@ are typing...", comment: ""), nickname1, nickname2)
        default:
            return NSLocalizedString("Several people are typing...", comment: "")
        }
    }
    public static var Channel_Success_Download_file = NSLocalizedString("File saved.", comment: "")
    public static var Channel_Failure_Download_file = NSLocalizedString("Couldn’t download file.", comment: "")
    public static var Channel_Failure_Open_file = NSLocalizedString("Couldn’t open file.", comment: "")
    public static var Channel_New_Message_File = NSLocalizedString("uploaded a file", comment: "")
    public static var Channel_New_Message: (Int) -> String = { count in
        switch count {
        case 1:
            return NSLocalizedString("1 new message", comment: "")
        case 2...99:
            return String.localizedStringWithFormat(NSLocalizedString("%d new messages", comment: ""), count)
        case 100...:
            return NSLocalizedString("99+ new messages", comment: "")
        default:
            return ""
        }
    }
    public static var Channel_State_Banner_Frozen = NSLocalizedString("Channel frozen", comment: "")
    
    // MARK: - Open Channel
    public static var Open_Channel_Name_Default = NSLocalizedString("Open Channel", comment: "")
    public static var Open_Channel_Participants = NSLocalizedString("Participants", comment: "")
    public static var Open_Channel_Participants_Count: (Int) -> String = { count in
        switch count {
        case 1:
            return NSLocalizedString("1 participant", comment: "")
        default:
            return String.localizedStringWithFormat(NSLocalizedString("%d participants", comment: ""), count)
        }
    }

    // MARK: - Notification Channel
    /// Custom type of the notification channel: `"SENDBIRD_NOTIFICATION_CHANNEL_NOTIFICATION"`
    public static let Notification_Channel_CustomType = "SENDBIRD_NOTIFICATION_CHANNEL_NOTIFICATION"
    
    /// Specifies the URL of the notification channel in a string form that belongs to the user with the specified `userId`: `"SENDBIRD_NOTIFICATION_CHANNEL_NOTIFICATION_{userId}"`
    public static var Notification_Channel_URL: (_ userId: String) -> String = { userId in
        return "\(SBUStringSet.Notification_Channel_CustomType)_\(userId)"
    }
    
    /// The default name of the notification channel: `"Notifications"`
    public static var Notification_Channel_Name_Default = NSLocalizedString("Notifications", comment: "")

    // MARK: - Channel Setting
    public static var ChannelSettings_Header_Title = NSLocalizedString("Channel Details", comment: "")
    public static var ChannelSettings_Change_Name = NSLocalizedString("Edit name", comment: "")
    public static var ChannelSettings_Change_Image = NSLocalizedString("Change channel image", comment: "")
    public static var ChannelSettings_Enter_New_Name = NSLocalizedString("Enter a name", comment: "")
    public static var ChannelSettings_Enter_New_Channel_Name = NSLocalizedString("Enter channel name", comment: "")
    public static var ChannelSettings_Notifications = NSLocalizedString("Notifications", comment: "")
    public static var ChannelSettings_Top = NSLocalizedString("Pin conversation", comment: "")
    public static var ChannelSettings_Clear = NSLocalizedString("Clear chat history", comment: "")
    public static var ChannelSettings_Notifications_On = NSLocalizedString("On", comment: "")
    public static var ChannelSettings_Notifications_Off = NSLocalizedString("Off", comment: "")
    public static var ChannelSettings_Notifications_Mentiones_Only = NSLocalizedString("Mentions only", comment: "")
    
    public static var ChannelSettings_Members_Title = NSLocalizedString("Members", comment: "")
    public static var ChannelSettings_Participants_Title = NSLocalizedString("Participants", comment: "")
    public static var ChannelSettings_Members: (UInt) -> String = { count in
        switch count {
        case 0:
            return NSLocalizedString("members", comment: "")
        default:
            return String.localizedStringWithFormat(NSLocalizedString("%u members", comment: ""), count)
        }
    }
    public static var ChannelSettings_Leave = NSLocalizedString("Leave channel", comment: "")
    public static var ChannelSettings_Delete = NSLocalizedString("Delete channel", comment: "")
    public static var ChannelSettings_Delete_Question_Mark = NSLocalizedString("Delete channel?", comment: "")
    public static var ChannelSettings_Delete_Description = NSLocalizedString("Once deleted, this channel can't be restored.", comment: "")
    public static var ChannelSettings_Search = NSLocalizedString("Search in channel", comment: "")
    
    public static var ChannelSettings_Moderations = NSLocalizedString("Moderations", comment: "")
    public static var ChannelSettings_Operators = NSLocalizedString("Operators", comment: "")
    public static var ChannelSettings_Muted_Members = NSLocalizedString("Muted members", comment: "")
    public static var ChannelSettings_Muted_Participants = NSLocalizedString("Muted participants", comment: "") // 3.0.0
    public static var ChannelSettings_Banned_Users = NSLocalizedString("Banned users", comment: "")
    public static var ChannelSettings_Freeze_Channel = NSLocalizedString("Freeze channel", comment: "")
    
    public static var ChannelSettings_URL = NSLocalizedString("URL", comment: "")
    
    // MARK: Channel push settings
    public static var ChannelPushSettings_Header_Title = NSLocalizedString("Notifications", comment: "")
    public static var ChannelPushSettings_Notification_Title = NSLocalizedString("Notifications", comment: "")
    public static var ChannelPushSettings_Item_All = NSLocalizedString("All new messages", comment: "")
    public static var ChannelPushSettings_Item_Mentions_Only = NSLocalizedString("Mentions only", comment: "")
    public static var ChannelPushSettings_Notification_Description = NSLocalizedString("Turn on push notifications if you wish to be notified when messages are delivered to this channel.", comment: "")

    // MARK: - Message Input
    public static var MessageInput_Text_Placeholder = NSLocalizedString("Enter message", comment: "")
    public static var MessageInput_Text_Unavailable = NSLocalizedString("Chat is unavailable in this channel", comment: "")
    public static var MessageInput_Text_Muted = NSLocalizedString("You are muted", comment: "")
    public static var MessageInput_Text_Reply = NSLocalizedString("Reply to message", comment: "")
    public static var MessageInput_Reply_To: (String) -> String = { quotedMessageNickname in
        return String.localizedStringWithFormat(NSLocalizedString("Reply to %@", comment: ""), quotedMessageNickname)
    }
    public static var MessageInput_Quote_Message_Photo = NSLocalizedString("Photo", comment: "")
    public static var MessageInput_Quote_Message_GIF = NSLocalizedString("GIF", comment: "")
    public static var MessageInput_Quote_Message_Video = NSLocalizedString("Video", comment: "")

    // MARK: - Message
    public static var Message_Edited = NSLocalizedString("(edited)", comment: "")
    public static var Message_System = NSLocalizedString("System message", comment: "")
    public static var Message_Unknown_Title = NSLocalizedString("(Unknown message type)", comment: "")
    public static var Message_Unknown_Description = NSLocalizedString("Can't read this message.", comment: "")
    public static var Message_Replied_To: (String, String) -> String = { replierNickname, quotedMessageNickname in
        return String.localizedStringWithFormat(NSLocalizedString("%@ replied to %@", comment: ""), replierNickname, quotedMessageNickname)
    }
    public static var Message_You = NSLocalizedString("You", comment: "")
    
    /// - Since: 3.3.0
    public static var Message_Replied_Users_Count: (Int, Bool) -> String = { repliedUsersCount, countLimit in
        switch repliedUsersCount {
        case 1:
            return NSLocalizedString("1 reply", comment: "")
        case 2...99:
            return String.localizedStringWithFormat(NSLocalizedString("%d replies", comment: ""), repliedUsersCount)
        case 100...:
            return countLimit ? NSLocalizedString("99+ replies", comment: "") : String.localizedStringWithFormat(NSLocalizedString("%d replies", comment: ""), repliedUsersCount)
        default:
            return ""
        }
    }
    
    /// - Since: 3.3.0
    public static var Message_Reply_Cannot_Found_Original = NSLocalizedString("Couldn't find the original message for this reply.", comment: "")
    
    /// - Since: 3.3.0
    public static var Message_Unavailable = NSLocalizedString("Message unavailable", comment: "")
    
    /// - Since: 3.12.0
    public static var Message_Typers_Count: (Int) -> String = { numberOfTypers in
        switch numberOfTypers {
        case 1...SBUConstant.maxNumberOfTypers:
            let remainingTypersCount = numberOfTypers - SBUConstant.maxNumberOfProfileImages
            return "+\(remainingTypersCount)"
        case (SBUConstant.maxNumberOfTypers + 1)...:
            return "+99"
        default:
            return ""
        }
    }
    
    /// - Since: 3.5.0
    public static var Notification_Template_Error_Title = NSLocalizedString("(Template error)", comment: "")
    public static var Notification_Template_Error_Subtitle = NSLocalizedString("Can't read this notification.", comment: "")

    // MARK: - Empty
    public static var Empty_No_Channels = NSLocalizedString("No channels", comment: "")
    public static var Empty_No_Messages = NSLocalizedString("No messages", comment: "")
    public static var Empty_No_Notifications = NSLocalizedString("No notifications", comment: "")
    public static var Empty_No_Users = NSLocalizedString("No users", comment: "")
    public static var Empty_No_Groups = NSLocalizedString("No groups", comment: "")
    public static var Empty_No_Muted_Members = NSLocalizedString("No muted members", comment: "")
    public static var Empty_No_Muted_Participants = NSLocalizedString("No muted participants", comment: "")
    public static var Empty_No_Banned_Users = NSLocalizedString("No banned users", comment: "")
    public static var Empty_Search_Result = NSLocalizedString("No results found", comment: "")
    public static var Empty_Wrong = NSLocalizedString("Something went wrong", comment: "")

    // MARK: - Create Channel
    public static var CreateChannel_Create: (Int) -> String = { count in
        switch count {
        case 0:
            return NSLocalizedString("Create", comment: "")
        default:
            return String.localizedStringWithFormat(NSLocalizedString("Create %d", comment: ""), count)
        }
    }
    public static var CreateChannel_Header_Title = NSLocalizedString("New Channel", comment: "")
    public static var CreateChannel_Header_Select_Members = NSLocalizedString("Select members", comment: "")
    public static var CreateChannel_Header_Title_Profile = NSLocalizedString("New channel profile", comment: "")
    
    // MARK: - Create Open Channel
    public static var CreateOpenChannel_Create = NSLocalizedString("Create", comment: "")
    public static var CreateOpenChannel_Header_Title = NSLocalizedString("New channel", comment: "")
    public static var CreateOpenChannel_ProfileInput_Placeholder = NSLocalizedString("Enter channel name", comment: "")

    // MARK: - Invite Channel
    public static var InviteChannel_Header_Title = NSLocalizedString("Invite users", comment: "")
    public static var InviteChannel_Header_Select_Users = NSLocalizedString("Select users", comment: "")
    public static var InviteChannel_Invite: (Int) -> String = { count in
        switch count {
        case 0:
            return NSLocalizedString("Invite", comment: "")
        default:
            return String.localizedStringWithFormat(NSLocalizedString("Invite %d", comment: ""), count)
        }
    }
    public static var InviteChannel_Register: (Int) -> String = { count in
        switch count {
        case 0:
            return NSLocalizedString("Register", comment: "")
        default:
            return String.localizedStringWithFormat(NSLocalizedString("Register %d", comment: ""), count)
        }
    }

    // MARK: - User List
    public static var UserList_Me = NSLocalizedString("(you)", comment: "")
    public static var UserList_Ban = NSLocalizedString("Ban", comment: "")
    public static var UserList_Unban = NSLocalizedString("Unban", comment: "")
    public static var UserList_Mute = NSLocalizedString("Mute", comment: "")
    public static var UserList_Unmute = NSLocalizedString("Unmute", comment: "")
    public static var UserList_Unregister_Operator = NSLocalizedString("Unregister operator", comment: "")
    public static var UserList_Register_Operator = NSLocalizedString("Register as operator", comment: "")
    public static var UserList_Title_Members = NSLocalizedString("Members", comment: "")
    public static var UserList_Title_Operators = NSLocalizedString("Operators", comment: "")
    public static var UserList_Title_Muted_Members = NSLocalizedString("Muted members", comment: "")
    public static var UserList_Title_Muted_Participants = NSLocalizedString("Muted participants", comment: "") // 3.0.0
    public static var UserList_Title_Banned_Users = NSLocalizedString("Banned users", comment: "")
    public static var UserList_Title_Participants = NSLocalizedString("Participants", comment: "")
    
    // MARK: - User
    public static var User_No_Name = NSLocalizedString("(No name)", comment: "")
    public static var User_Operator = NSLocalizedString("Operator", comment: "")
    
    // MARK: - User profile
    public static var UserProfile_Role_Operator = NSLocalizedString("Operator", comment: "")
    public static var UserProfile_Role_Member = NSLocalizedString("Member", comment: "")
    public static var UserProfile_UserID = NSLocalizedString("User ID", comment: "")
    public static var UserProfile_Message = NSLocalizedString("Message", comment: "")
    public static var UserProfile_Register = NSLocalizedString("Register", comment: "")
    public static var UserProfile_Unregister = NSLocalizedString("Unregister", comment: "")
    public static var UserProfile_Mute = NSLocalizedString("Mute", comment: "")
    public static var UserProfile_Unmute = NSLocalizedString("Unmute", comment: "")
    public static var UserProfile_Ban = NSLocalizedString("Ban", comment: "")
    
    // MARK: - Channel type
    public static var ChannelType_Group = NSLocalizedString("Group", comment: "")
    public static var ChannelType_SuperGroup = NSLocalizedString("Super group", comment: "")
    public static var ChannelType_Broadcast = NSLocalizedString("Broadcast", comment: "")
    
    // MARK: - form type
    public static var FormType_Optional = NSLocalizedString("(optional)", comment: "") // 3.11.0
    public static var FormType_Error_Default = NSLocalizedString("Please check the value", comment: "") // 3.11.0
    
    // MARK: - Feedback
    public static var Feedback_Comment_Title = NSLocalizedString("Provide additional feedback (optional)", comment: "") // 3.15.0
    public static var Feedback_Comment_Placeholder = NSLocalizedString("Leave a comment", comment: "") // 3.15.0
    public static var Feedback_Edit_Comment = NSLocalizedString("Edit comment", comment: "") // 3.15.0
    public static var Feedback_Remove = NSLocalizedString("Remove feedback", comment: "") // 3.15.0
    public static var Feedback_Update_Done = NSLocalizedString("Successfully changed", comment: "") // 3.15.0
    
    public class Mention {
        /// "@"
        public static let Trigger_Key: String = "@"
        
        /// e.g., "You can mention up to 10 times at a time."
        public static var Limit_Guide = {
            let limit = SBUGlobals.userMentionConfig?.mentionLimit ?? 10
            return String.localizedStringWithFormat(NSLocalizedString("You can mention up to %d times per message.", comment: ""), limit)
        }()
    }
    
    // MARK: - MessageThreading
    /// - Since: 3.3.0
    public struct MessageThread {
        public struct Menu {
            public static var replyInThread = NSLocalizedString("Reply in thread", comment: "")
        }
        
        public struct MessageInput {
            public static var replyInThread = NSLocalizedString("Reply in thread", comment: "")
            public static var replyToThread = NSLocalizedString("Reply to thread", comment: "")
        }
        
        public struct Header {
            public static var title = NSLocalizedString("Thread", comment: "")
        }
    }
    
    // MARK: - Voice
    public struct VoiceMessage {
        public struct Input {
            /// A text for the cancel button in ``SBUVoiceMessageInputView``.
            public static var cancel = NSLocalizedString("Cancel", comment: "")
        }
        
        public struct Alert {
            /// A text for an alert dialog that's displayed when a channel freezes while a user plays a voice message. The default text is `Channel is frozen.`.
            public static var frozen = NSLocalizedString("Channel is frozen.", comment: "")
            /// A text for an alert dialog that's displayed when a user is muted while playing a voice message. The default text is `You're muted by the operator.`.
            public static var muted = NSLocalizedString("You're muted by the operator.", comment: "")
        }
        
        public struct Preview {
            /// A text that indicates that a quoted message is a voice message. The default text is `Voice message`.
            public static var quotedMessage = NSLocalizedString("Voice message", comment: "")
            /// A text that indicates that a voice message was sent to a group channel and appears in ``SBUGroupChannelCell`` in the group channel list view. The default text is `Voice message`.
            public static var channelList: String {
                get { SBUStringSet.GroupChannel.Preview.voice }
                set { SBUStringSet.GroupChannel.Preview.voice = newValue }
            }
            /// A text that's used in `SBUMessageSearchResultCell` to indicate that a search result is a voice message. The default text is `Voice message`.
            public static var searchResult = NSLocalizedString("Voice message", comment: "")
        }
        /// A text that's used in a name of the voice message file.
        public static var fileName = NSLocalizedString("Voice_message", comment: "")
    }
    
    // MARK: - GroupChannel
    
    /// Represents a set of strings related to `MultipleFilesMessage`.
    /// - since: 3.10.0
    public struct GroupChannel {
        public struct Preview {
            public static var photo = NSLocalizedString("Photo", comment: "")
            public static var gif = NSLocalizedString("GIF", comment: "")
            public static var video = NSLocalizedString("Video", comment: "")
            public static var audio = NSLocalizedString("Audio", comment: "")
            public static var voice = NSLocalizedString("Voice message", comment: "")
            public static var file = NSLocalizedString("File", comment: "")
            public static var multipleFiles = NSLocalizedString("Photo", comment: "")
        }
    }
    
    /// Represents a set of strings related to uploading a file.
    /// - since: 3.10.0
    public struct FileUpload {
        public struct Error {
            //TODO: 
            public static var exceededSizeLimit = NSLocalizedString("The maximum size per file is 1024MB.", comment: "")
//            public static var exceededSizeLimit = "The maximum size per file is \(SBUAvailable.uploadSizeLimitMB)MB."
        }
    }
}

extension SBUStringSet {
    @available(*, deprecated, renamed: "InviteChannel_Register")
    public static var InviteChannel_Add: (Int) -> String = { count in
        InviteChannel_Register(count)
    }
    
}
//  swiftlint:enable missing_docs
//  swiftlint:enable identifier_name
