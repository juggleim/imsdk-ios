//
//  JEncryptUtility.h
//  JuggleIM
//
//  Created by Fei Li on 2026/6/18.
//

#import <Foundation/Foundation.h>
#import "JE2EEInfo.h"

NS_ASSUME_NONNULL_BEGIN

@interface JEncryptUtility : NSObject

+ (nullable NSData *)generateX25519PrivateKey;
+ (nullable NSData *)x25519PublicKeyFromPrivateKey:(NSData *)privateKey;
+ (nullable NSData *)x25519SharedSecretWithPrivateKey:(NSData *)privateKey publicKey:(NSData *)publicKey;

+ (nullable NSData *)generateAES256Key;
+ (nullable NSData *)generateAESGCMNonce;
+ (nullable NSData *)aes256GCMEncryptData:(NSData *)data
                                      key:(NSData *)key
                                    nonce:(NSData *)nonce
              additionalAuthenticatedData:(nullable NSData *)additionalAuthenticatedData
                                      tag:(NSData *_Nullable *_Nullable)tag;
+ (nullable NSData *)aes256GCMDecryptData:(NSData *)data
                                      key:(NSData *)key
                                    nonce:(NSData *)nonce
              additionalAuthenticatedData:(nullable NSData *)additionalAuthenticatedData
                                      tag:(NSData *)tag;

/// Sort infoList by deviceId in ascending order, concatenate all pubKey values, calculate SHA256, and return a base64 string.
/// @param infoList Original JE2EEInfo array.
+ (NSString *)calcPubKeysSHA256Base64WithInfoList:(NSArray<JE2EEInfo *> *)infoList;

+ (NSData *)deriveAES256KeyFromSharedSecret:(NSData *)sharedSecret;

@end

NS_ASSUME_NONNULL_END
