//
// Copyright (c) 2016-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//

#import "SRError.h"

#import "JIMSRWebSocket.h"

NS_ASSUME_NONNULL_BEGIN

NSError *JIMSRErrorWithDomainCodeDescription(NSString *domain, NSInteger code, NSString *description)
{
    return [NSError errorWithDomain:domain code:code userInfo:@{ NSLocalizedDescriptionKey: description }];
}

NSError *JIMSRErrorWithCodeDescription(NSInteger code, NSString *description)
{
    return JIMSRErrorWithDomainCodeDescription(JIMSRWebSocketErrorDomain, code, description);
}

NSError *JIMSRErrorWithCodeDescriptionUnderlyingError(NSInteger code, NSString *description, NSError *underlyingError)
{
    return [NSError errorWithDomain:JIMSRWebSocketErrorDomain
                               code:code
                           userInfo:@{ NSLocalizedDescriptionKey: description,
                                       NSUnderlyingErrorKey: underlyingError }];
}

NSError *JIMSRHTTPErrorWithCodeDescription(NSInteger httpCode, NSInteger errorCode, NSString *description)
{
    return [NSError errorWithDomain:JIMSRWebSocketErrorDomain
                               code:errorCode
                           userInfo:@{ NSLocalizedDescriptionKey: description,
                                       JIMSRHTTPResponseErrorKey: @(httpCode) }];
}

NS_ASSUME_NONNULL_END
