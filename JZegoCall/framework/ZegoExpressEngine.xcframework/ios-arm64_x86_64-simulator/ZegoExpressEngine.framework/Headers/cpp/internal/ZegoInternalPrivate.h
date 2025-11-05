#pragma once

#include "./include/zego-express-defines.h"

ZEGO_BEGIN_DECLS

#ifndef ZEGOEXP_EXPLICIT
extern void zego_express_handle_api_call_result(const char *func_name, int error_code);
extern void zego_express_post_windows_message(void *message);
extern void zego_register_recv_windows_message_callback(void *callback, void *user_context);
extern void zego_express_set_platform_language(enum zego_platform_language);
extern void zego_express_custom_log(const char *log, const char *module);
extern void zego_register_recv_zego_callback_task(void *callback, void *user_context);
extern void zego_express_post_zego_callback_task(void *callback_task);

#else
typedef void (*pfnzego_express_handle_api_call_result)(const char *func_name, int error_code);
typedef void (*pfnzego_express_post_windows_message)(void *message);
typedef void (*pfnzego_register_recv_windows_message_callback)(void *callback, void *user_context);
typedef void (*pfnzego_express_set_platform_language)(enum zego_platform_language);
typedef void (*pfnzego_express_custom_log)(const char *log, const char *module);
typedef void (*pfnzego_register_recv_zego_callback_task)(void *callback, void *user_context);
typedef void (*pfnzego_express_post_zego_callback_task)(void *callback_task);

#endif

ZEGO_END_DECLS
