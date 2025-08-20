#ifndef SECRET_H
#define SECRET_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * 生成客户端公钥，并将结果写入 out_pubkey 指针。
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * out_pubkey: 输出65字节公钥的指针
 * 返回值: 0成功，-1参数错误，-2字符串解析失败
 */
int generate_client_pubkey_ffi(const char *session_id, unsigned char *out_pubkey);

/**
 * 校验并存储共享密钥，通过 signature 验证。
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * server_pubkey: 服务端公钥（65字节指针）
 * signature: 服务端加密数据指针
 * signature_len: 加密数据长度
 * 返回值: 0成功，-1参数错误，-2字符串解析失败，-3校验失败
 */
int store_shared_key_with_verify_ffi(const char *session_id,
                                     const unsigned char *server_pubkey,
                                     const unsigned char *signature,
                                     uintptr_t signature_len);

/**
 * 用会话密钥加密数据，结果写入 out_buf。
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * plaintext: 明文数据指针
 * plaintext_len: 明文长度
 * out_buf: 输出缓冲区指针
 * out_len: 实际输出长度指针
 * buf_cap: 输出缓冲区容量（即 out_buf 分配的字节数，必须 >= 预期加密结果长度，否则返回 -4）
 *   例如：加密时建议分配 buf_cap = 明文长度 + 12（AES-GCM nonce 长度），如明文100字节则 buf_cap 至少为112。
 * 返回值: 0成功，-1参数错误，-2字符串解析失败，-3加密失败，-4缓冲区不足
 */
int encrypt_with_session_ffi(const char *session_id,
                             const unsigned char *plaintext,
                             uintptr_t plaintext_len,
                             unsigned char *out_buf,
                             uintptr_t *out_len,
                             uintptr_t buf_cap);

/**
 * 用会话密钥解密数据，结果写入 out_buf。
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * encrypted_data: 加密数据指针
 * encrypted_len: 加密数据长度
 * out_buf: 输出缓冲区指针
 * out_len: 实际输出长度指针
 * buf_cap: 输出缓冲区容量（即 out_buf 分配的字节数，必须 >= 预期解密结果长度，否则返回 -4）
 *   例如：解密时建议分配 buf_cap = 加密数据长度（通常不会超过原始明文长度），如密文112字节则 buf_cap 至少为112。
 * 返回值: 0成功，-1参数错误，-2字符串解析失败，-3解密失败，-4缓冲区不足
 */
int decrypt_with_session_ffi(const char *session_id,
                             const unsigned char *encrypted_data,
                             uintptr_t encrypted_len,
                             unsigned char *out_buf,
                             uintptr_t *out_len,
                             uintptr_t buf_cap);

/**
 * 生成客户端公钥
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * payload_buf: SecretNegotiate 结构数据的指针
 * payload_len: SecretNegotiate 结构数据长度
 * 返回值: 0成功，-1参数错误，-2字符串解析失败
 */
int generate_client_pubkey_payload_ffi(const char *session_id,
                                       unsigned char *payload_buf,
                                       uintptr_t *payload_len);

/**
 * 校验并生成客户端共享密钥
 * session_id: 会话唯一标识（UTF-8字符串指针）
 * payload_buf: SecretNegotiateAck 结构数据的指针
 * payload_len: SecretNegotiateAck 结构数据长度
 * 返回值: 0成功，-1参数错误，-2字符串解析失败，-3校验失败
 */
int store_shared_key_with_payload_ffi(const char *session_id,
                                      const unsigned char *payload_buf,
                                      uintptr_t payload_len);

#endif  /* SECRET_H */
