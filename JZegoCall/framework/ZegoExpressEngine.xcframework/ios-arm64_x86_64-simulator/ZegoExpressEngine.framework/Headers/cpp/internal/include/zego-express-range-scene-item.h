
#ifndef __ZEGO_EXPRESS_RANGE_SCENE_ITEM_H__
#define __ZEGO_EXPRESS_RANGE_SCENE_ITEM_H__

#include "zego-express-defines.h"

ZEGO_BEGIN_DECLS
/// Create item.
///
/// Available: since 3.1.0
/// Description: Create item.
/// Use cases: Item competition in the meta world.
/// When to call: After receive login Scene success, before [logoutScene].
/// Caution:
///   1. Items belong to range scene, not a certain user. When a user successfully binds an item, it only means that the user has the temporary right to use the item.
///   2. An item is allowed to have one or more bound users, and the principle of first-come, first-served is followed when applying for bound items.
///  3.  When multiple users bind an item at the same time, their changes to the item follow the cas principle.
///   4. When creating an item, it is allowed to specify whether to bind the item after the creation is successful.
///   5. When the item is created successfully, users within the range of the item will receive the [onItemEnterView] callback notification.
/// Related APIs: Users can call [destroyItem] to destroy item.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param param Item param.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_create_item(
    int range_scene_handle, zego_seq *seq, struct zego_item_param param);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_create_item)(
    int range_scene_handle, zego_seq *seq, struct zego_item_param param);
#endif

/// Destroy item.
///
/// Available: since 3.1.0
/// Description: Destroy item.
/// Use cases: Item competition in the meta world.
/// When to call: After receive after login Scene success , before [logoutScene].
/// Caution: When an item is destroyed, users within the range of the item will be notified by the [onItemLeaveView] callback.
/// Related APIs: Users can call [createItem] to create item.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param item_id Item ID.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_destroy_item(int range_scene_handle,
                                                                           zego_seq *seq,
                                                                           long long item_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_destroy_item)(int range_scene_handle,
                                                                            zego_seq *seq,
                                                                            long long item_id);
#endif

/// Bind item.
///
/// Available: since 3.1.0
/// Description: Bind item.
/// Use cases: Item competition in the meta world.
/// When to call: After receive after login Scene success, before [logoutScene].
/// Caution: When an item is successfully bound, users within the range of the item will be notified by the [onItemBindUpdate] callback.
/// Related APIs: Users can call [unbindItem] to unbind item.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param item_id Item ID.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_bind_item(int range_scene_handle,
                                                                        zego_seq *seq,
                                                                        long long item_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_bind_item)(int range_scene_handle,
                                                                         zego_seq *seq,
                                                                         long long item_id);
#endif

/// Unbind item.
///
/// Available: since 3.1.0
/// Description: Unbind item.
/// Use cases: Item competition in the meta world.
/// When to call: After receive [bindItem], before [logoutScene].
/// Caution: When an item is successfully unbound, users within the scope of the item will be notified by the [onItemUnbindUpdate] callback.
/// Related APIs: Users can call [bindItem] to bind item.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param item_id Item ID.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_unbind_item(int range_scene_handle,
                                                                          zego_seq *seq,
                                                                          long long item_id);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_unbind_item)(int range_scene_handle,
                                                                           zego_seq *seq,
                                                                           long long item_id);
#endif

/// Update item status.
///
/// Available: since 3.1.0
/// Description: Developers can call this function to update the item status.
/// When to call: After [onBindItem], before [unbindItem].
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param item_id Item ID.
/// @param position Item's current location.
/// @param channel The channel to which the status belongs, starting from 0, cannot exceed the maximum channel number.
/// @param status Current status data.
/// @param status_length Current status data length.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_update_item_status(
    int range_scene_handle, zego_seq *seq, long long item_id, struct zego_position position,
    unsigned int channel, const unsigned char *status, unsigned int status_length);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_update_item_status)(
    int range_scene_handle, zego_seq *seq, long long item_id, struct zego_position position,
    unsigned int channel, const unsigned char *status, unsigned int status_length);
#endif

/// Update item status.
///
/// Available: since 3.1.0
/// Description: Developers can call this function to update the item command.
/// When to call: After [onBindItem], before [unbindItem].
/// Restrictions: None.
/// Caution: None.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq The seq executed this time, the seq is valid when the call is successful.
/// @param item_id Item ID.
/// @param position Item's current location.
/// @param channel The channel to which the status belongs, starting from 0, cannot exceed the maximum channel number.
/// @param command Next command data.
/// @param command_length Next command data length.
/// @return Error code, please refer to the error codes document https://doc-en.zego.im/en/5548.html for details.
#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API zego_error EXP_CALL zego_express_range_scene_item_update_item_command(
    int range_scene_handle, zego_seq *seq, long long item_id, struct zego_position position,
    unsigned int channel, const unsigned char *command, unsigned int command_length);
#else
typedef zego_error(EXP_CALL *pfnzego_express_range_scene_item_update_item_command)(
    int range_scene_handle, zego_seq *seq, long long item_id, struct zego_position position,
    unsigned int channel, const unsigned char *command, unsigned int command_length);
#endif

/// Callback notification when item enters the current user's field of view.
///
/// Available: Since 3.1.0
/// Description: Callback notification when item enters the current user's field of view.
/// When to trigger: Item enters the current user's field of view.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param capacity The number of users allowed to bind the item.
/// @param position Item position.
/// @param user_list List of users the item is currently bound to.
/// @param user_list_size The number of users currently bound to the item.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_enter_view)(
    int range_scene_handle, long long item_id, unsigned int capacity, struct zego_position position,
    const char **user_list, unsigned int user_list_size, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_enter_view_callback(
    zego_on_range_scene_item_item_enter_view callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_enter_view_callback)(
    zego_on_range_scene_item_item_enter_view callback_func, void *user_context);
#endif

/// Callback notification when item leave the current user's field of view.
///
/// Available: Since 3.1.0
/// Description: Callback notification when item leave the current user's field of view.
/// When to trigger: Item leave the current user's field of view.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_leave_view)(int range_scene_handle, long long item_id,
                                                         void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_leave_view_callback(
    zego_on_range_scene_item_item_leave_view callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_leave_view_callback)(
    zego_on_range_scene_item_item_leave_view callback_func, void *user_context);
#endif

/// Item binding user change callback notification.
///
/// Available: Since 3.1.0
/// Description: Item binding user change callback notification.
/// When to trigger: Item binding user change.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param userid The user ID of the item binding.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_bind_update)(int range_scene_handle, long long item_id,
                                                          const char *userid, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_bind_update_callback(
    zego_on_range_scene_item_item_bind_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_bind_update_callback)(
    zego_on_range_scene_item_item_bind_update callback_func, void *user_context);
#endif

/// Item unbind user change callback notification.
///
/// Available: Since 3.1.0
/// Description: Item unbind user change callback notification.
/// When to trigger: Item unbind user change.
/// Restrictions: Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param user_list List of users the item unbind.
/// @param user_list_size The number of users who have unbound the item.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_unbind_update)(int range_scene_handle,
                                                            long long item_id,
                                                            const char **user_list,
                                                            unsigned int user_list_size,
                                                            void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_unbind_update_callback(
    zego_on_range_scene_item_item_unbind_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_unbind_update_callback)(
    zego_on_range_scene_item_item_unbind_update callback_func, void *user_context);
#endif

/// Item state update callback.
///
/// Available: Since 3.1.0
/// Description: Item state update callback.
/// When to trigger: Item state update.
/// Restrictions: Do not call the SDK interface in the callback thread.
/// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param position Item's current position.
/// @param channel The channel to which the status belongs.
/// @param status Current status data.
/// @param status_length Current status data length.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_status_update)(
    int range_scene_handle, long long item_id, struct zego_position position, unsigned int channel,
    const unsigned char *status, unsigned int status_length, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_status_update_callback(
    zego_on_range_scene_item_item_status_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_status_update_callback)(
    zego_on_range_scene_item_item_status_update callback_func, void *user_context);
#endif

/// Item command update callback.
///
/// Available: Since 3.1.0
/// Description: Item command update callback.
/// When to trigger: Item command update.
/// Restrictions: Do not call the SDK interface in the callback thread.
/// Caution: This callback is a high-frequency callback, please do not perform time-consuming operations in this callback.
///
/// @param range_scene_handle Range scene instance handle.
/// @param item_id Item ID.
/// @param position Item's current position.
/// @param channel The channel to which the command belongs.
/// @param command Next command data.
/// @param command_length Next command data length.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_item_command_update)(
    int range_scene_handle, long long item_id, struct zego_position position, unsigned int channel,
    const unsigned char *command, unsigned int command_length, void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_item_command_update_callback(
    zego_on_range_scene_item_item_command_update callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_item_command_update_callback)(
    zego_on_range_scene_item_item_command_update callback_func, void *user_context);
#endif

/// Create item result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_create_item)(int range_scene_handle, zego_seq seq,
                                                     int error_code, long long item_id,
                                                     void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_create_item_callback(
    zego_on_range_scene_item_create_item callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_create_item_callback)(
    zego_on_range_scene_item_create_item callback_func, void *user_context);
#endif

/// Destroy item result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_destroy_item)(int range_scene_handle, zego_seq seq,
                                                      int error_code, long long item_id,
                                                      void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_destroy_item_callback(
    zego_on_range_scene_item_destroy_item callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_destroy_item_callback)(
    zego_on_range_scene_item_destroy_item callback_func, void *user_context);
#endif

/// Bind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_bind_item)(int range_scene_handle, zego_seq seq,
                                                   int error_code, long long item_id,
                                                   void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_bind_item_callback(
    zego_on_range_scene_item_bind_item callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_bind_item_callback)(
    zego_on_range_scene_item_bind_item callback_func, void *user_context);
#endif

/// Unbind item result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_unbind_item)(int range_scene_handle, zego_seq seq,
                                                     int error_code, long long item_id,
                                                     void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_unbind_item_callback(
    zego_on_range_scene_item_unbind_item callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_unbind_item_callback)(
    zego_on_range_scene_item_unbind_item callback_func, void *user_context);
#endif

/// Update item status result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_update_item_status)(int range_scene_handle, zego_seq seq,
                                                            int error_code, long long item_id,
                                                            void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_update_item_status_callback(
    zego_on_range_scene_item_update_item_status callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_update_item_status_callback)(
    zego_on_range_scene_item_update_item_status callback_func, void *user_context);
#endif

/// Update item command result callback. Do not call the SDK interface in the callback thread.
///
/// @param range_scene_handle Range scene instance handle.
/// @param seq Sequence.
/// @param error_code Error code, please refer to the error codes document https://docs.zegocloud.com/en/5548.html for details.
/// @param item_id Item ID.
/// @param user_context Context of user.
typedef void (*zego_on_range_scene_item_update_item_command)(int range_scene_handle, zego_seq seq,
                                                             int error_code, long long item_id,
                                                             void *user_context);

#ifndef ZEGOEXP_EXPLICIT
ZEGOEXP_API void EXP_CALL zego_register_range_scene_item_update_item_command_callback(
    zego_on_range_scene_item_update_item_command callback_func, void *user_context);
#else
typedef void(EXP_CALL *pfnzego_register_range_scene_item_update_item_command_callback)(
    zego_on_range_scene_item_update_item_command callback_func, void *user_context);
#endif

ZEGO_END_DECLS

#endif
