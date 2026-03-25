#pragma once
#include <jni.h>

#define MAX_DIRECT_BUFS 256

typedef struct {
    int64_t  arm_handle;
    jobject  host_jobject;
} ProxyDirectBuf;

extern ProxyDirectBuf g_direct_bufs[MAX_DIRECT_BUFS];
extern int            g_direct_buf_count;

JNIEnv *jnienv_get(void);