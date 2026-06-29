//
//  JEncryptUtility.m
//  JuggleIM
//
//  Created by Fei Li on 2026/6/18.
//

#import "JEncryptUtility.h"
#import <CommonCrypto/CommonCrypto.h>
#import <Security/Security.h>

#define JAES256KeyLength 32
#define JAESGCMNonceLength 12
#define JAESGCMTagLength 16

static void JClampX25519Scalar(uint8_t scalar[32]);
static BOOL JIsAllZero(const uint8_t *bytes, size_t length);
static BOOL JConstantTimeEqual(const uint8_t *lhs, const uint8_t *rhs, size_t length);
static BOOL JAES256EncryptBlock(const uint8_t key[32], const uint8_t input[16], uint8_t output[16]);
static void JIncrementGCMCounter(uint8_t counter[16]);
static void JXorBlock(uint8_t out[16], const uint8_t lhs[16], const uint8_t rhs[16]);
static void JRightShiftBlock(uint8_t block[16]);
static void JGCMMultiply(uint8_t x[16], const uint8_t y[16]);
static void JGHASHUpdate(uint8_t y[16], const uint8_t h[16], const uint8_t *bytes, size_t length);
static void JStoreUInt64BE(uint8_t out[8], uint64_t value);
static BOOL JBuildAES256GCMTag(const uint8_t key[32], const uint8_t nonce[12], NSData *_Nullable aad, const uint8_t *ciphertext, size_t ciphertextLength, uint8_t tag[16]);
static NSData *_Nullable JAES256GCMCrypt(NSData *data, NSData *key, NSData *nonce);
static void JX25519(uint8_t out[32], const uint8_t scalar[32], const uint8_t point[32]);

@implementation JEncryptUtility

+ (nullable NSData *)generateX25519PrivateKey {
    uint8_t privateKey[32];
    int status = SecRandomCopyBytes(kSecRandomDefault, sizeof(privateKey), privateKey);
    if (status != errSecSuccess) {
        return nil;
    }
    JClampX25519Scalar(privateKey);
    return [NSData dataWithBytes:privateKey length:sizeof(privateKey)];
}

+ (nullable NSData *)x25519PublicKeyFromPrivateKey:(NSData *)privateKey {
    static const uint8_t basePoint[32] = {9};
    return [self x25519ScalarMultWithPrivateKey:privateKey publicKey:[NSData dataWithBytes:basePoint length:sizeof(basePoint)] rejectAllZeroResult:NO];
}

+ (nullable NSData *)x25519SharedSecretWithPrivateKey:(NSData *)privateKey publicKey:(NSData *)publicKey {
    return [self x25519ScalarMultWithPrivateKey:privateKey publicKey:publicKey rejectAllZeroResult:YES];
}

+ (nullable NSData *)x25519ScalarMultWithPrivateKey:(NSData *)privateKey
                                         publicKey:(NSData *)publicKey
                               rejectAllZeroResult:(BOOL)rejectAllZeroResult {
    if (privateKey.length != 32 || publicKey.length != 32) {
        return nil;
    }

    uint8_t scalar[32];
    uint8_t point[32];
    uint8_t result[32];
    [privateKey getBytes:scalar length:sizeof(scalar)];
    [publicKey getBytes:point length:sizeof(point)];
    JClampX25519Scalar(scalar);
    JX25519(result, scalar, point);

    if (rejectAllZeroResult && JIsAllZero(result, sizeof(result))) {
        memset(scalar, 0, sizeof(scalar));
        memset(point, 0, sizeof(point));
        memset(result, 0, sizeof(result));
        return nil;
    }

    NSData *data = [NSData dataWithBytes:result length:sizeof(result)];
    memset(scalar, 0, sizeof(scalar));
    memset(point, 0, sizeof(point));
    memset(result, 0, sizeof(result));
    return data;
}

+ (nullable NSData *)generateAES256Key {
    uint8_t key[JAES256KeyLength];
    int status = SecRandomCopyBytes(kSecRandomDefault, sizeof(key), key);
    if (status != errSecSuccess) {
        return nil;
    }
    return [NSData dataWithBytes:key length:sizeof(key)];
}

+ (nullable NSData *)generateAESGCMNonce {
    uint8_t nonce[JAESGCMNonceLength];
    int status = SecRandomCopyBytes(kSecRandomDefault, sizeof(nonce), nonce);
    if (status != errSecSuccess) {
        return nil;
    }
    return [NSData dataWithBytes:nonce length:sizeof(nonce)];
}

+ (nullable NSData *)aes256GCMEncryptData:(NSData *)data
                                      key:(NSData *)key
                                    nonce:(NSData *)nonce
              additionalAuthenticatedData:(nullable NSData *)additionalAuthenticatedData
                                      tag:(NSData *_Nullable *_Nullable)tag {
    if (tag != NULL) {
        *tag = nil;
    }
    if (key.length != JAES256KeyLength || nonce.length != JAESGCMNonceLength) {
        return nil;
    }

    NSData *ciphertext = JAES256GCMCrypt(data, key, nonce);
    if (!ciphertext) {
        return nil;
    }

    uint8_t keyBytes[JAES256KeyLength];
    uint8_t nonceBytes[JAESGCMNonceLength];
    uint8_t tagBytes[JAESGCMTagLength];
    [key getBytes:keyBytes length:sizeof(keyBytes)];
    [nonce getBytes:nonceBytes length:sizeof(nonceBytes)];
    BOOL didBuildTag = JBuildAES256GCMTag(keyBytes,
                                          nonceBytes,
                                          additionalAuthenticatedData,
                                          ciphertext.bytes,
                                          ciphertext.length,
                                          tagBytes);
    if (!didBuildTag) {
        memset(keyBytes, 0, sizeof(keyBytes));
        memset(nonceBytes, 0, sizeof(nonceBytes));
        memset(tagBytes, 0, sizeof(tagBytes));
        return nil;
    }

    if (tag != NULL) {
        *tag = [NSData dataWithBytes:tagBytes length:sizeof(tagBytes)];
    }
    memset(keyBytes, 0, sizeof(keyBytes));
    memset(nonceBytes, 0, sizeof(nonceBytes));
    memset(tagBytes, 0, sizeof(tagBytes));
    return ciphertext;
}

+ (nullable NSData *)aes256GCMDecryptData:(NSData *)data
                                      key:(NSData *)key
                                    nonce:(NSData *)nonce
              additionalAuthenticatedData:(nullable NSData *)additionalAuthenticatedData
                                      tag:(NSData *)tag {
    if (key.length != JAES256KeyLength || nonce.length != JAESGCMNonceLength || tag.length != JAESGCMTagLength) {
        return nil;
    }

    uint8_t keyBytes[JAES256KeyLength];
    uint8_t nonceBytes[JAESGCMNonceLength];
    uint8_t expectedTag[JAESGCMTagLength];
    [key getBytes:keyBytes length:sizeof(keyBytes)];
    [nonce getBytes:nonceBytes length:sizeof(nonceBytes)];
    BOOL didBuildTag = JBuildAES256GCMTag(keyBytes,
                                          nonceBytes,
                                          additionalAuthenticatedData,
                                          data.bytes,
                                          data.length,
                                          expectedTag);
    if (!didBuildTag) {
        memset(keyBytes, 0, sizeof(keyBytes));
        memset(nonceBytes, 0, sizeof(nonceBytes));
        memset(expectedTag, 0, sizeof(expectedTag));
        return nil;
    }
    if (!JConstantTimeEqual(expectedTag, tag.bytes, sizeof(expectedTag))) {
        memset(keyBytes, 0, sizeof(keyBytes));
        memset(nonceBytes, 0, sizeof(nonceBytes));
        memset(expectedTag, 0, sizeof(expectedTag));
        return nil;
    }

    NSData *plaintext = JAES256GCMCrypt(data, key, nonce);
    memset(keyBytes, 0, sizeof(keyBytes));
    memset(nonceBytes, 0, sizeof(nonceBytes));
    memset(expectedTag, 0, sizeof(expectedTag));
    return plaintext;
}

+ (NSString *)calcPubKeysSHA256Base64WithInfoList:(NSArray<JE2EEInfo *> *)infoList {
    if (!infoList || infoList.count == 0) {
        return @"";
    }
    
    // 1. 按 userId + deviceId 字母升序排序
    NSArray<JE2EEInfo *> *sortedList = [infoList sortedArrayUsingComparator:^NSComparisonResult(JE2EEInfo *obj1, JE2EEInfo *obj2) {
        NSString *str1 = [obj1.userId stringByAppendingString:obj1.deviceId];
        NSString *str2 = [obj2.userId stringByAppendingString:obj2.deviceId];
        return [str1 compare:str2];
    }];
    
    // 2. 遍历取出 pubKey 依次拼接
    NSMutableData *concatPubData = [NSMutableData data];
    for (JE2EEInfo *info in sortedList) {
        if (info.pubKey && info.pubKey.length > 0) {
            [concatPubData appendData:info.pubKey];
        }
    }
    
    // 3. 计算 SHA256
    NSData *sha256Data = [self sha256Hash:concatPubData];
    
    // 4. Base64编码返回
    return [sha256Data base64EncodedStringWithOptions:0];
}

+ (NSData *)sha256Hash:(NSData *)data {
    unsigned char digest[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256(data.bytes, (CC_LONG)data.length, digest);
    return [NSData dataWithBytes:digest length:CC_SHA256_DIGEST_LENGTH];
}

+ (NSData *)deriveAES256KeyFromSharedSecret:(NSData *)sharedSecret {
    if (!sharedSecret) {
        return nil;
    }
    
    NSData *salt = [@"protocol-v1" dataUsingEncoding:NSUTF8StringEncoding];
    NSData *info = [@"envelope-v1" dataUsingEncoding:NSUTF8StringEncoding];

    // ===== Extract =====

    unsigned char prk[CC_SHA256_DIGEST_LENGTH];

    CCHmac(kCCHmacAlgSHA256,
           salt.bytes,
           salt.length,
           sharedSecret.bytes,
           sharedSecret.length,
           prk);

    // ===== Expand =====

    NSMutableData *okm =
        [NSMutableData dataWithLength:32];

    unsigned char t[CC_SHA256_DIGEST_LENGTH];

    NSMutableData *data =
        [NSMutableData data];

    if (info) {
        [data appendData:info];
    }

    uint8_t counter = 1;
    [data appendBytes:&counter length:1];

    CCHmac(kCCHmacAlgSHA256,
           prk,
           sizeof(prk),
           data.bytes,
           data.length,
           t);

    memcpy(okm.mutableBytes, t, 32);

    return okm;
}

@end

static void JClampX25519Scalar(uint8_t scalar[32]) {
    scalar[0] &= 248;
    scalar[31] &= 127;
    scalar[31] |= 64;
}

static BOOL JIsAllZero(const uint8_t *bytes, size_t length) {
    uint8_t accumulator = 0;
    for (size_t i = 0; i < length; i++) {
        accumulator |= bytes[i];
    }
    return accumulator == 0;
}

static BOOL JConstantTimeEqual(const uint8_t *lhs, const uint8_t *rhs, size_t length) {
    uint8_t diff = 0;
    for (size_t i = 0; i < length; i++) {
        diff |= lhs[i] ^ rhs[i];
    }
    return diff == 0;
}

static BOOL JAES256EncryptBlock(const uint8_t key[32], const uint8_t input[16], uint8_t output[16]) {
    size_t moved = 0;
    CCCryptorStatus status = CCCrypt(kCCEncrypt,
                                      kCCAlgorithmAES,
                                      kCCOptionECBMode,
                                      key,
                                      JAES256KeyLength,
                                      NULL,
                                      input,
                                      16,
                                      output,
                                      16,
                                      &moved);
    return status == kCCSuccess && moved == 16;
}

static void JIncrementGCMCounter(uint8_t counter[16]) {
    for (int i = 15; i >= 12; i--) {
        counter[i]++;
        if (counter[i] != 0) {
            break;
        }
    }
}

static void JXorBlock(uint8_t out[16], const uint8_t lhs[16], const uint8_t rhs[16]) {
    for (int i = 0; i < 16; i++) {
        out[i] = lhs[i] ^ rhs[i];
    }
}

static void JRightShiftBlock(uint8_t block[16]) {
    uint8_t carry = 0;
    for (int i = 0; i < 16; i++) {
        uint8_t nextCarry = block[i] & 1;
        block[i] = (block[i] >> 1) | (uint8_t)(carry << 7);
        carry = nextCarry;
    }
}

static void JGCMMultiply(uint8_t x[16], const uint8_t y[16]) {
    uint8_t z[16] = {0};
    uint8_t v[16];
    memcpy(v, y, sizeof(v));

    for (int i = 0; i < 128; i++) {
        uint8_t bit = (x[i / 8] >> (7 - (i % 8))) & 1;
        if (bit) {
            for (int j = 0; j < 16; j++) {
                z[j] ^= v[j];
            }
        }

        BOOL lsb = (v[15] & 1) != 0;
        JRightShiftBlock(v);
        if (lsb) {
            v[0] ^= 0xe1;
        }
    }

    memcpy(x, z, sizeof(z));
}

static void JGHASHUpdate(uint8_t y[16], const uint8_t h[16], const uint8_t *bytes, size_t length) {
    size_t offset = 0;
    while (offset < length) {
        uint8_t block[16] = {0};
        size_t blockLength = MIN((size_t)16, length - offset);
        memcpy(block, bytes + offset, blockLength);
        JXorBlock(y, y, block);
        JGCMMultiply(y, h);
        offset += blockLength;
    }
}

static void JStoreUInt64BE(uint8_t out[8], uint64_t value) {
    for (int i = 7; i >= 0; i--) {
        out[i] = (uint8_t)(value & 0xff);
        value >>= 8;
    }
}

static BOOL JBuildAES256GCMTag(const uint8_t key[32], const uint8_t nonce[12], NSData *_Nullable aad, const uint8_t *ciphertext, size_t ciphertextLength, uint8_t tag[16]) {
    uint8_t zero[16] = {0};
    uint8_t h[16];
    uint8_t y[16] = {0};
    uint8_t j0[16] = {0};
    memcpy(j0, nonce, 12);
    j0[15] = 1;

    if (!JAES256EncryptBlock(key, zero, h)) {
        return NO;
    }

    if (aad.length > 0) {
        JGHASHUpdate(y, h, aad.bytes, aad.length);
    }
    if (ciphertextLength > 0) {
        JGHASHUpdate(y, h, ciphertext, ciphertextLength);
    }

    uint8_t lengthBlock[16] = {0};
    JStoreUInt64BE(lengthBlock, (uint64_t)aad.length * 8);
    JStoreUInt64BE(lengthBlock + 8, (uint64_t)ciphertextLength * 8);
    JGHASHUpdate(y, h, lengthBlock, sizeof(lengthBlock));

    uint8_t encryptedJ0[16];
    if (!JAES256EncryptBlock(key, j0, encryptedJ0)) {
        memset(h, 0, sizeof(h));
        memset(y, 0, sizeof(y));
        return NO;
    }

    JXorBlock(tag, encryptedJ0, y);
    memset(h, 0, sizeof(h));
    memset(y, 0, sizeof(y));
    memset(encryptedJ0, 0, sizeof(encryptedJ0));
    return YES;
}

static NSData *_Nullable JAES256GCMCrypt(NSData *data, NSData *key, NSData *nonce) {
    if (key.length != JAES256KeyLength || nonce.length != JAESGCMNonceLength) {
        return nil;
    }

    uint8_t keyBytes[JAES256KeyLength];
    uint8_t counter[16] = {0};
    [key getBytes:keyBytes length:sizeof(keyBytes)];
    [nonce getBytes:counter length:JAESGCMNonceLength];
    counter[15] = 1;

    NSMutableData *output = [NSMutableData dataWithLength:data.length];
    const uint8_t *inputBytes = data.bytes;
    uint8_t *outputBytes = output.mutableBytes;
    size_t offset = 0;

    while (offset < data.length) {
        JIncrementGCMCounter(counter);
        uint8_t streamBlock[16];
        if (!JAES256EncryptBlock(keyBytes, counter, streamBlock)) {
            memset(keyBytes, 0, sizeof(keyBytes));
            memset(counter, 0, sizeof(counter));
            return nil;
        }

        size_t blockLength = MIN((size_t)16, data.length - offset);
        for (size_t i = 0; i < blockLength; i++) {
            outputBytes[offset + i] = inputBytes[offset + i] ^ streamBlock[i];
        }
        memset(streamBlock, 0, sizeof(streamBlock));
        offset += blockLength;
    }

    memset(keyBytes, 0, sizeof(keyBytes));
    memset(counter, 0, sizeof(counter));
    return output;
}

static int64_t JLoad3(const uint8_t *input) {
    int64_t result = input[0];
    result |= ((int64_t)input[1]) << 8;
    result |= ((int64_t)input[2]) << 16;
    return result;
}

static int64_t JLoad4(const uint8_t *input) {
    int64_t result = input[0];
    result |= ((int64_t)input[1]) << 8;
    result |= ((int64_t)input[2]) << 16;
    result |= ((int64_t)input[3]) << 24;
    return result;
}

static void JFeFromBytes(int64_t h[10], const uint8_t s[32]) {
    int64_t h0 = JLoad4(s);
    int64_t h1 = JLoad3(s + 4) << 6;
    int64_t h2 = JLoad3(s + 7) << 5;
    int64_t h3 = JLoad3(s + 10) << 3;
    int64_t h4 = JLoad3(s + 13) << 2;
    int64_t h5 = JLoad4(s + 16);
    int64_t h6 = JLoad3(s + 20) << 7;
    int64_t h7 = JLoad3(s + 23) << 5;
    int64_t h8 = JLoad3(s + 26) << 4;
    int64_t h9 = (JLoad3(s + 29) & 8388607) << 2;
    int64_t carry[10];

    carry[9] = (h9 + (int64_t)(1 << 24)) >> 25;
    h0 += carry[9] * 19;
    h9 -= carry[9] << 25;
    carry[1] = (h1 + (int64_t)(1 << 24)) >> 25;
    h2 += carry[1];
    h1 -= carry[1] << 25;
    carry[3] = (h3 + (int64_t)(1 << 24)) >> 25;
    h4 += carry[3];
    h3 -= carry[3] << 25;
    carry[5] = (h5 + (int64_t)(1 << 24)) >> 25;
    h6 += carry[5];
    h5 -= carry[5] << 25;
    carry[7] = (h7 + (int64_t)(1 << 24)) >> 25;
    h8 += carry[7];
    h7 -= carry[7] << 25;

    carry[0] = (h0 + (int64_t)(1 << 25)) >> 26;
    h1 += carry[0];
    h0 -= carry[0] << 26;
    carry[2] = (h2 + (int64_t)(1 << 25)) >> 26;
    h3 += carry[2];
    h2 -= carry[2] << 26;
    carry[4] = (h4 + (int64_t)(1 << 25)) >> 26;
    h5 += carry[4];
    h4 -= carry[4] << 26;
    carry[6] = (h6 + (int64_t)(1 << 25)) >> 26;
    h7 += carry[6];
    h6 -= carry[6] << 26;
    carry[8] = (h8 + (int64_t)(1 << 25)) >> 26;
    h9 += carry[8];
    h8 -= carry[8] << 26;

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

static void JFeToBytes(uint8_t s[32], const int64_t h[10]) {
    int64_t h0 = h[0];
    int64_t h1 = h[1];
    int64_t h2 = h[2];
    int64_t h3 = h[3];
    int64_t h4 = h[4];
    int64_t h5 = h[5];
    int64_t h6 = h[6];
    int64_t h7 = h[7];
    int64_t h8 = h[8];
    int64_t h9 = h[9];

    int64_t q = (19 * h9 + (((int64_t)1) << 24)) >> 25;
    q = (h0 + q) >> 26;
    q = (h1 + q) >> 25;
    q = (h2 + q) >> 26;
    q = (h3 + q) >> 25;
    q = (h4 + q) >> 26;
    q = (h5 + q) >> 25;
    q = (h6 + q) >> 26;
    q = (h7 + q) >> 25;
    q = (h8 + q) >> 26;
    q = (h9 + q) >> 25;

    h0 += 19 * q;

    int64_t carry0 = h0 >> 26;
    h1 += carry0;
    h0 -= carry0 << 26;
    int64_t carry1 = h1 >> 25;
    h2 += carry1;
    h1 -= carry1 << 25;
    int64_t carry2 = h2 >> 26;
    h3 += carry2;
    h2 -= carry2 << 26;
    int64_t carry3 = h3 >> 25;
    h4 += carry3;
    h3 -= carry3 << 25;
    int64_t carry4 = h4 >> 26;
    h5 += carry4;
    h4 -= carry4 << 26;
    int64_t carry5 = h5 >> 25;
    h6 += carry5;
    h5 -= carry5 << 25;
    int64_t carry6 = h6 >> 26;
    h7 += carry6;
    h6 -= carry6 << 26;
    int64_t carry7 = h7 >> 25;
    h8 += carry7;
    h7 -= carry7 << 25;
    int64_t carry8 = h8 >> 26;
    h9 += carry8;
    h8 -= carry8 << 26;
    int64_t carry9 = h9 >> 25;
    h9 -= carry9 << 25;

    s[0] = (uint8_t)(h0 >> 0);
    s[1] = (uint8_t)(h0 >> 8);
    s[2] = (uint8_t)(h0 >> 16);
    s[3] = (uint8_t)((h0 >> 24) | (h1 << 2));
    s[4] = (uint8_t)(h1 >> 6);
    s[5] = (uint8_t)(h1 >> 14);
    s[6] = (uint8_t)((h1 >> 22) | (h2 << 3));
    s[7] = (uint8_t)(h2 >> 5);
    s[8] = (uint8_t)(h2 >> 13);
    s[9] = (uint8_t)((h2 >> 21) | (h3 << 5));
    s[10] = (uint8_t)(h3 >> 3);
    s[11] = (uint8_t)(h3 >> 11);
    s[12] = (uint8_t)((h3 >> 19) | (h4 << 6));
    s[13] = (uint8_t)(h4 >> 2);
    s[14] = (uint8_t)(h4 >> 10);
    s[15] = (uint8_t)(h4 >> 18);
    s[16] = (uint8_t)(h5 >> 0);
    s[17] = (uint8_t)(h5 >> 8);
    s[18] = (uint8_t)(h5 >> 16);
    s[19] = (uint8_t)((h5 >> 24) | (h6 << 1));
    s[20] = (uint8_t)(h6 >> 7);
    s[21] = (uint8_t)(h6 >> 15);
    s[22] = (uint8_t)((h6 >> 23) | (h7 << 3));
    s[23] = (uint8_t)(h7 >> 5);
    s[24] = (uint8_t)(h7 >> 13);
    s[25] = (uint8_t)((h7 >> 21) | (h8 << 4));
    s[26] = (uint8_t)(h8 >> 4);
    s[27] = (uint8_t)(h8 >> 12);
    s[28] = (uint8_t)((h8 >> 20) | (h9 << 6));
    s[29] = (uint8_t)(h9 >> 2);
    s[30] = (uint8_t)(h9 >> 10);
    s[31] = (uint8_t)(h9 >> 18);
}

static void JFeCopy(int64_t h[10], const int64_t f[10]) {
    for (int i = 0; i < 10; i++) {
        h[i] = f[i];
    }
}

static void JFeOne(int64_t h[10]) {
    h[0] = 1;
    for (int i = 1; i < 10; i++) {
        h[i] = 0;
    }
}

static void JFeZero(int64_t h[10]) {
    for (int i = 0; i < 10; i++) {
        h[i] = 0;
    }
}

static void JFeAdd(int64_t h[10], const int64_t f[10], const int64_t g[10]) {
    for (int i = 0; i < 10; i++) {
        h[i] = f[i] + g[i];
    }
}

static void JFeSub(int64_t h[10], const int64_t f[10], const int64_t g[10]) {
    for (int i = 0; i < 10; i++) {
        h[i] = f[i] - g[i];
    }
}

static void JFeMul(int64_t h[10], const int64_t f[10], const int64_t g[10]) {
    int64_t f0 = f[0];
    int64_t f1 = f[1];
    int64_t f2 = f[2];
    int64_t f3 = f[3];
    int64_t f4 = f[4];
    int64_t f5 = f[5];
    int64_t f6 = f[6];
    int64_t f7 = f[7];
    int64_t f8 = f[8];
    int64_t f9 = f[9];
    int64_t g0 = g[0];
    int64_t g1 = g[1];
    int64_t g2 = g[2];
    int64_t g3 = g[3];
    int64_t g4 = g[4];
    int64_t g5 = g[5];
    int64_t g6 = g[6];
    int64_t g7 = g[7];
    int64_t g8 = g[8];
    int64_t g9 = g[9];
    int64_t g1_19 = 19 * g1;
    int64_t g2_19 = 19 * g2;
    int64_t g3_19 = 19 * g3;
    int64_t g4_19 = 19 * g4;
    int64_t g5_19 = 19 * g5;
    int64_t g6_19 = 19 * g6;
    int64_t g7_19 = 19 * g7;
    int64_t g8_19 = 19 * g8;
    int64_t g9_19 = 19 * g9;
    int64_t f1_2 = 2 * f1;
    int64_t f3_2 = 2 * f3;
    int64_t f5_2 = 2 * f5;
    int64_t f7_2 = 2 * f7;
    int64_t f9_2 = 2 * f9;
    int64_t h0 = f0 * g0 + f1_2 * g9_19 + f2 * g8_19 + f3_2 * g7_19 + f4 * g6_19 + f5_2 * g5_19 + f6 * g4_19 + f7_2 * g3_19 + f8 * g2_19 + f9_2 * g1_19;
    int64_t h1 = f0 * g1 + f1 * g0 + f2 * g9_19 + f3 * g8_19 + f4 * g7_19 + f5 * g6_19 + f6 * g5_19 + f7 * g4_19 + f8 * g3_19 + f9 * g2_19;
    int64_t h2 = f0 * g2 + f1_2 * g1 + f2 * g0 + f3_2 * g9_19 + f4 * g8_19 + f5_2 * g7_19 + f6 * g6_19 + f7_2 * g5_19 + f8 * g4_19 + f9_2 * g3_19;
    int64_t h3 = f0 * g3 + f1 * g2 + f2 * g1 + f3 * g0 + f4 * g9_19 + f5 * g8_19 + f6 * g7_19 + f7 * g6_19 + f8 * g5_19 + f9 * g4_19;
    int64_t h4 = f0 * g4 + f1_2 * g3 + f2 * g2 + f3_2 * g1 + f4 * g0 + f5_2 * g9_19 + f6 * g8_19 + f7_2 * g7_19 + f8 * g6_19 + f9_2 * g5_19;
    int64_t h5 = f0 * g5 + f1 * g4 + f2 * g3 + f3 * g2 + f4 * g1 + f5 * g0 + f6 * g9_19 + f7 * g8_19 + f8 * g7_19 + f9 * g6_19;
    int64_t h6 = f0 * g6 + f1_2 * g5 + f2 * g4 + f3_2 * g3 + f4 * g2 + f5_2 * g1 + f6 * g0 + f7_2 * g9_19 + f8 * g8_19 + f9_2 * g7_19;
    int64_t h7 = f0 * g7 + f1 * g6 + f2 * g5 + f3 * g4 + f4 * g3 + f5 * g2 + f6 * g1 + f7 * g0 + f8 * g9_19 + f9 * g8_19;
    int64_t h8 = f0 * g8 + f1_2 * g7 + f2 * g6 + f3_2 * g5 + f4 * g4 + f5_2 * g3 + f6 * g2 + f7_2 * g1 + f8 * g0 + f9_2 * g9_19;
    int64_t h9 = f0 * g9 + f1 * g8 + f2 * g7 + f3 * g6 + f4 * g5 + f5 * g4 + f6 * g3 + f7 * g2 + f8 * g1 + f9 * g0;
    int64_t carry[10];

    carry[0] = (h0 + (int64_t)(1 << 25)) >> 26;
    h1 += carry[0];
    h0 -= carry[0] << 26;
    carry[4] = (h4 + (int64_t)(1 << 25)) >> 26;
    h5 += carry[4];
    h4 -= carry[4] << 26;

    carry[1] = (h1 + (int64_t)(1 << 24)) >> 25;
    h2 += carry[1];
    h1 -= carry[1] << 25;
    carry[5] = (h5 + (int64_t)(1 << 24)) >> 25;
    h6 += carry[5];
    h5 -= carry[5] << 25;

    carry[2] = (h2 + (int64_t)(1 << 25)) >> 26;
    h3 += carry[2];
    h2 -= carry[2] << 26;
    carry[6] = (h6 + (int64_t)(1 << 25)) >> 26;
    h7 += carry[6];
    h6 -= carry[6] << 26;

    carry[3] = (h3 + (int64_t)(1 << 24)) >> 25;
    h4 += carry[3];
    h3 -= carry[3] << 25;
    carry[7] = (h7 + (int64_t)(1 << 24)) >> 25;
    h8 += carry[7];
    h7 -= carry[7] << 25;

    carry[4] = (h4 + (int64_t)(1 << 25)) >> 26;
    h5 += carry[4];
    h4 -= carry[4] << 26;
    carry[8] = (h8 + (int64_t)(1 << 25)) >> 26;
    h9 += carry[8];
    h8 -= carry[8] << 26;

    carry[9] = (h9 + (int64_t)(1 << 24)) >> 25;
    h0 += carry[9] * 19;
    h9 -= carry[9] << 25;

    carry[0] = (h0 + (int64_t)(1 << 25)) >> 26;
    h1 += carry[0];
    h0 -= carry[0] << 26;

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

static void JFeSquare(int64_t h[10], const int64_t f[10]) {
    JFeMul(h, f, f);
}

static void JFeMul121666(int64_t h[10], const int64_t f[10]) {
    static const int64_t c[10] = {121666, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    JFeMul(h, f, c);
}

static void JFeInvert(int64_t out[10], const int64_t z[10]) {
    int64_t t0[10];
    int64_t t1[10];
    int64_t t2[10];
    int64_t t3[10];
    int i;

    JFeSquare(t0, z);
    JFeSquare(t1, t0);
    JFeSquare(t1, t1);
    JFeMul(t1, z, t1);
    JFeMul(t0, t0, t1);
    JFeSquare(t2, t0);
    JFeMul(t1, t1, t2);
    JFeSquare(t2, t1);
    for (i = 1; i < 5; i++) {
        JFeSquare(t2, t2);
    }
    JFeMul(t1, t2, t1);
    JFeSquare(t2, t1);
    for (i = 1; i < 10; i++) {
        JFeSquare(t2, t2);
    }
    JFeMul(t2, t2, t1);
    JFeSquare(t3, t2);
    for (i = 1; i < 20; i++) {
        JFeSquare(t3, t3);
    }
    JFeMul(t2, t3, t2);
    JFeSquare(t2, t2);
    for (i = 1; i < 10; i++) {
        JFeSquare(t2, t2);
    }
    JFeMul(t1, t2, t1);
    JFeSquare(t2, t1);
    for (i = 1; i < 50; i++) {
        JFeSquare(t2, t2);
    }
    JFeMul(t2, t2, t1);
    JFeSquare(t3, t2);
    for (i = 1; i < 100; i++) {
        JFeSquare(t3, t3);
    }
    JFeMul(t2, t3, t2);
    JFeSquare(t2, t2);
    for (i = 1; i < 50; i++) {
        JFeSquare(t2, t2);
    }
    JFeMul(t1, t2, t1);
    JFeSquare(t1, t1);
    for (i = 1; i < 5; i++) {
        JFeSquare(t1, t1);
    }
    JFeMul(out, t1, t0);
}

static void JFeCSwap(int64_t f[10], int64_t g[10], uint8_t b) {
    int64_t mask = -(int64_t)b;
    for (int i = 0; i < 10; i++) {
        int64_t x = mask & (f[i] ^ g[i]);
        f[i] ^= x;
        g[i] ^= x;
    }
}

static void JX25519(uint8_t out[32], const uint8_t scalar[32], const uint8_t point[32]) {
    int64_t x1[10];
    int64_t x2[10];
    int64_t z2[10];
    int64_t x3[10];
    int64_t z3[10];
    int64_t tmp0[10];
    int64_t tmp1[10];
    uint8_t e[32];
    memcpy(e, scalar, sizeof(e));
    JClampX25519Scalar(e);

    uint8_t u[32];
    memcpy(u, point, sizeof(u));
    u[31] &= 127;
    JFeFromBytes(x1, u);
    JFeOne(x2);
    JFeZero(z2);
    JFeCopy(x3, x1);
    JFeOne(z3);

    uint8_t swap = 0;
    for (int pos = 254; pos >= 0; pos--) {
        uint8_t bit = (e[pos / 8] >> (pos & 7)) & 1;
        swap ^= bit;
        JFeCSwap(x2, x3, swap);
        JFeCSwap(z2, z3, swap);
        swap = bit;

        JFeSub(tmp0, x3, z3);
        JFeSub(tmp1, x2, z2);
        JFeAdd(x2, x2, z2);
        JFeAdd(z2, x3, z3);
        JFeMul(z3, tmp0, x2);
        JFeMul(z2, z2, tmp1);
        JFeSquare(tmp0, tmp1);
        JFeSquare(tmp1, x2);
        JFeAdd(x3, z3, z2);
        JFeSub(z2, z3, z2);
        JFeMul(x2, tmp1, tmp0);
        JFeSub(tmp1, tmp1, tmp0);
        JFeSquare(z2, z2);
        JFeMul121666(z3, tmp1);
        JFeSquare(x3, x3);
        JFeAdd(tmp0, tmp0, z3);
        JFeMul(z3, x1, z2);
        JFeMul(z2, tmp1, tmp0);
    }

    JFeCSwap(x2, x3, swap);
    JFeCSwap(z2, z3, swap);
    JFeInvert(z2, z2);
    JFeMul(x2, x2, z2);
    JFeToBytes(out, x2);

    memset(e, 0, sizeof(e));
    memset(u, 0, sizeof(u));
}
