
#ifndef __ZEGO_EXPRESS_RANGE_SCENE_TEAM_H__
#define __ZEGO_EXPRESS_RANGE_SCENE_TEAM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Join team.
///
/// Available since: 3.1.0
/// Description: Join team.
/// Use case: Often used in meta world scenarios, after join the same team, team members can see and hear each other.
/// When to call: After login scene by [loginScene].
/// Restrictions: None.
///
/// @param range_scene_handle Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param config Team Param.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_team_join_team(
    int range_scene_handle, zego_seq *seq, struct zego_team_param config);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_team_join_team)(
    int range_scene_handle, zego_seq *seq, struct zego_team_param config);
#endif

/// Leave team.
///
/// Available since: 3.1.0
/// Description: Join team.
/// Use case: Often used in meta world scenarios, after join the same team, team members can see and hear each other.
/// When to call: After join team by [joinTeam].
/// Restrictions: None.
///
/// @param range_scene_handle Range scene handle. Obtained through [getHandle] of ZegoRangeScene instance.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param team_id Team ID.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_team_leave_team(int range_scene_handle,
                                                                         zego_seq *seq,
                                                                         unsigned int team_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_team_leave_team)(int range_scene_handle,
                                                                          zego_seq *seq,
                                                                          unsigned int team_id);
#endif

/// The callback triggered when the team connection state changes.
///
/// Available since: 3.1.0
/// Description: This callback is triggered when the connection status of the team changes, and the reason for the change is notified.
/// Use cases: Developers can use this callback to determine the status of the current user in the team.
/// When to trigger:
///  1. The developer will receive this notification when calling the [joinTeam], [leaveTeam] functions.
///  2. This notification may also be received when the network condition of the user's device changes (SDK will automatically join the team when reconnected, please refer to [Does ZEGO SDK support a fast reconnection for temporary disconnection] for details](https://docs.zegocloud.com/faq/reconnect?product=ExpressVideo&platform=all).
/// Restrictions: None.
/// Caution: If the connection is being requested for a long time, the general probability is that the user's network is unstable.
/// Related APIs: [joinTeam]、[leaveTeam]
///
/// @param range_scene_handle Range scene instance handle.
/// @param team_id Team ID.
/// @param state Changed team state.
/// @param error_code Error code, For details, please refer to [Common Error Codes](https://docs.zegocloud.com/article/5548).
/// @param user_context context of user
typedef void (*zego_on_range_scene_team_team_state_update)(int range_scene_handle,
                                                           unsigned int team_id,
                                                           enum zego_team_state state,
                                                           zego_error error_code,
                                                           void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_team_team_state_update_callback(
    zego_on_range_scene_team_team_state_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_team_team_state_update_callback)(
    zego_on_range_scene_team_team_state_update callback_func, void *user_context);
#endif

/// The callback triggered when the number of other users in the team increases or decreases.
///
/// Available since: 3.1.0
/// Description: When other users in the room are online or offline, which causes the user list in the room to change, the developer will be notified through this callback.
/// Use cases: Developers can use this callback to update the user list display in the room in real time.
/// When to trigger:
///   1. When the user join the room for the first time, if there are other users in the team, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd], and `userList` is the other users in the team at this time.
///   2. The user is already in the team. If another user join the team through the [joinTeam] function, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeAdd].
///   3. If other users leave the team through the [leaveTeam] function, the SDK will trigger a callback notification with `updateType` being [ZegoUpdateTypeDelete].
/// Related APIs: [joinTeam]、[leaveTeam]
///
/// @param range_scene_handle Range scene instance handle.
/// @param team_id Team ID.
/// @param update_type Update type (add/delete).
/// @param user_list List of users changed in the current team.
/// @param user_count List count of users changed in the current team.
/// @param user_context context of user.
typedef void (*zego_on_range_scene_team_team_member_update)(
    int range_scene_handle, unsigned int team_id, enum zego_update_type update_type,
    const struct zego_user *user_list, unsigned int user_count, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_team_team_member_update_callback(
    zego_on_range_scene_team_team_member_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_team_team_member_update_callback)(
    zego_on_range_scene_team_team_member_update callback_func, void *user_context);
#endif

/// Join team result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param team_id Team ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_team_join_team)(int range_scene_handle, zego_seq seq,
                                                   int error_code, unsigned int team_id,
                                                   void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_team_join_team_callback(
    zego_on_range_scene_team_join_team callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_team_join_team_callback)(
    zego_on_range_scene_team_join_team callback_func, void *user_context);
#endif

/// Leave team result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param team_id Team ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_team_leave_team)(int range_scene_handle, zego_seq seq,
                                                    int error_code, unsigned int team_id,
                                                    void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_team_leave_team_callback(
    zego_on_range_scene_team_leave_team callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_team_leave_team_callback)(
    zego_on_range_scene_team_leave_team callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
