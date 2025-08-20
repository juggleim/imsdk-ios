//
//  JDataConverter.m
//  JuggleUlt
//
//  Created by Fei Li on 2025/8/20.
//

#import "JDataConverter.h"
#import "secret.h"
#import "JUtility.h"
#import "JLogger.h"

@interface JDataConverter ()
@property (nonatomic, strong) NSString *session;
@property (nonatomic, assign) BOOL active;
@end

@implementation JDataConverter

+ (nonnull instancetype)converter {
    JDataConverter *converter = [[JDataConverter alloc] init];
    converter.session = [JUtility getUUID];

    return converter;
}

- (NSData *)getPubKey {
    unsigned char payload[1024];
    uintptr_t len;
    int r = generate_client_pubkey_payload_ffi([self.session UTF8String], payload, &len);
    if (r != 0) {
        JLogE(@"CON-Enc", @"generate_client_pubkey_payload_ffi error, %d", r);
    }
    NSData *pubKey = [NSData dataWithBytes:payload length:len];
    JLogI(@"CON-Enc", @"getPubKey");
    return pubKey;
}

- (void)storeSharedKey:(NSData *)key {
    if (key.length == 0) {
        self.active = NO;
        JLogI(@"CON-Enc", @"storeSharedKey key is null");
        return;
    }
    self.active = YES;
    int r = store_shared_key_with_payload_ffi([self.session UTF8String], key.bytes, key.length);
    JLogI(@"CON-Enc", @"storeSharedKey");
    if (r != 0) {
        JLogE(@"CON-Enc", @"store_shared_key_with_payload_ffi error, %d", r);
    }
}

- (NSData *)decrypt:(NSData *)data {
    if (self.active) {
        NSMutableData *md = [NSMutableData dataWithLength:data.length];
        unsigned char *payload = (unsigned char *)md.mutableBytes;
        memset(payload, 0, data.length);
        
        uintptr_t len;
        int r = decrypt_with_session_ffi([self.session UTF8String], data.bytes, data.length, payload, &len, data.length);
        if (r != 0) {
            JLogE(@"CON-Enc", @"decode error, %d", r);
            return nil;
        } else {
            NSData *d = [NSData dataWithBytes:payload length:len];
            return d;
        }
    } else {
        return data;
    }
}

- (NSData *)encrypt:(NSData *)data {
    if (self.active) {
        NSMutableData *md = [NSMutableData dataWithLength:data.length+100];
        unsigned char *payload = (unsigned char *)md.mutableBytes;
        memset(payload, 0, data.length+100);
        uintptr_t len;
        int r = encrypt_with_session_ffi([self.session UTF8String], data.bytes, data.length, payload, &len, data.length+100);
        if (r != 0) {
            JLogE(@"CON-Enc", @"encode error, %d", r);
            return nil;
        } else {
            NSData *d = [NSData dataWithBytes:payload length:len];
            return d;
        }
    } else {
        return data;
    }
}

- (NSString *)session {
    if (!_session) {
        _session = [JUtility getUUID];
    }
    return _session;
}

@end
