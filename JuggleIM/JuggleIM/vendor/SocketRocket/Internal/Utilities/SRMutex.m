//
// Copyright (c) 2016-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//

#import "SRMutex.h"

#import <pthread/pthread.h>

NS_ASSUME_NONNULL_BEGIN

SRMutex JIMSRMutexInitRecursive(void)
{
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutexattr_t attributes;

    pthread_mutexattr_init(&attributes);
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &attributes);
    pthread_mutexattr_destroy(&attributes);

    return mutex;
}

void JIMSRMutexDestroy(SRMutex mutex)
{
    pthread_mutex_destroy(mutex);
    free(mutex);
}

__attribute__((no_thread_safety_analysis))
void JIMSRMutexLock(SRMutex mutex)
{
    pthread_mutex_lock(mutex);
}

__attribute__((no_thread_safety_analysis))
void JIMSRMutexUnlock(SRMutex mutex)
{
    pthread_mutex_unlock(mutex);
}

NS_ASSUME_NONNULL_END
