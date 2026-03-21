
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <jni.h>
#include "include/jnienv.h"
#include "include/ipc.h"

// Global socket fd set by main
extern int g_socket_fd;
extern uint32_t g_current_call_id;

// Generic JNIEnv proxy — sends method name + raw args over socket
// Java side handles it with the real JNIEnv
static int64_t proxy_jnienv(const char *method, const void *args, uint32_t args_len) {
    uint32_t mlen  = strlen(method) + 1;
    uint32_t total = mlen + args_len;
    uint8_t *payload = malloc(total);
    memcpy(payload, method, mlen);
    if (args_len > 0) memcpy(payload + mlen, args, args_len);
    msg_send(g_socket_fd, MSG_JNIENV, g_current_call_id, payload, total);
    free(payload);

    // Wait for JNIENV_RETURN
    MsgHeader hdr;
    uint8_t buf[65536];
    while (1) {
        if (msg_recv(g_socket_fd, &hdr, buf, sizeof(buf)) < 0) return 0;
        if (hdr.type == MSG_JNIENV_RETURN && hdr.id == g_current_call_id) {
            int64_t result = 0;
            if (hdr.data_len >= 8) memcpy(&result, buf, 8);
            return result;
        }
    }
}

static jint proxy_GetVersion(JNIEnv * a0) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetVersion", NULL, 0);
    return (jint)(intptr_t)r;
}

static jclass proxy_FindClass(JNIEnv* a0, const char* a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("FindClass", NULL, 0);
    return (jclass)(intptr_t)r;
}

static jmethodID proxy_FromReflectedMethod(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("FromReflectedMethod", NULL, 0);
    return (jmethodID)(intptr_t)r;
}

static jfieldID proxy_FromReflectedField(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("FromReflectedField", NULL, 0);
    return (jfieldID)(intptr_t)r;
}

static jobject proxy_ToReflectedMethod(JNIEnv* a0, jclass a1, jmethodID a2, jboolean a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ToReflectedMethod", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jclass proxy_GetSuperclass(JNIEnv* a0, jclass a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetSuperclass", NULL, 0);
    return (jclass)(intptr_t)r;
}

static jboolean proxy_IsAssignableFrom(JNIEnv* a0, jclass a1, jclass a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("IsAssignableFrom", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jobject proxy_ToReflectedField(JNIEnv* a0, jclass a1, jfieldID a2, jboolean a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ToReflectedField", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jint proxy_Throw(JNIEnv* a0, jthrowable a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("Throw", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_ThrowNew(JNIEnv * a0, jclass a1, const char * a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ThrowNew", NULL, 0);
    return (jint)(intptr_t)r;
}

static jthrowable proxy_ExceptionOccurred(JNIEnv* a0) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ExceptionOccurred", NULL, 0);
    return (jthrowable)(intptr_t)r;
}

static void proxy_ExceptionDescribe(JNIEnv* a0) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ExceptionDescribe", NULL, 0);
}

static void proxy_ExceptionClear(JNIEnv* a0) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ExceptionClear", NULL, 0);
}

static void proxy_FatalError(JNIEnv* a0, const char* a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("FatalError", NULL, 0);
}

static jint proxy_PushLocalFrame(JNIEnv* a0, jint a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("PushLocalFrame", NULL, 0);
    return (jint)(intptr_t)r;
}

static jobject proxy_PopLocalFrame(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("PopLocalFrame", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_NewGlobalRef(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewGlobalRef", NULL, 0);
    return (jobject)(intptr_t)r;
}

static void proxy_DeleteGlobalRef(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("DeleteGlobalRef", NULL, 0);
}

static void proxy_DeleteLocalRef(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("DeleteLocalRef", NULL, 0);
}

static jboolean proxy_IsSameObject(JNIEnv* a0, jobject a1, jobject a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("IsSameObject", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jobject proxy_NewLocalRef(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewLocalRef", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jint proxy_EnsureLocalCapacity(JNIEnv* a0, jint a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("EnsureLocalCapacity", NULL, 0);
    return (jint)(intptr_t)r;
}

static jobject proxy_AllocObject(JNIEnv* a0, jclass a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("AllocObject", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_NewObject(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewObject", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_NewObjectV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewObjectV", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_NewObjectA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewObjectA", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jclass proxy_GetObjectClass(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetObjectClass", NULL, 0);
    return (jclass)(intptr_t)r;
}

static jboolean proxy_IsInstanceOf(JNIEnv* a0, jobject a1, jclass a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("IsInstanceOf", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jmethodID proxy_GetMethodID(JNIEnv* a0, jclass a1, const char* a2, const char* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetMethodID", NULL, 0);
    return (jmethodID)(intptr_t)r;
}

static jobject proxy_CallObjectMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallObjectMethod", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_CallObjectMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallObjectMethodV", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_CallObjectMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallObjectMethodA", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jboolean proxy_CallBooleanMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallBooleanMethod", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jboolean proxy_CallBooleanMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallBooleanMethodV", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jboolean proxy_CallBooleanMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallBooleanMethodA", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jbyte proxy_CallByteMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallByteMethod", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jbyte proxy_CallByteMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallByteMethodV", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jbyte proxy_CallByteMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallByteMethodA", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jchar proxy_CallCharMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallCharMethod", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jchar proxy_CallCharMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallCharMethodV", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jchar proxy_CallCharMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallCharMethodA", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jshort proxy_CallShortMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallShortMethod", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jshort proxy_CallShortMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallShortMethodV", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jshort proxy_CallShortMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallShortMethodA", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jint proxy_CallIntMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallIntMethod", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_CallIntMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallIntMethodV", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_CallIntMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallIntMethodA", NULL, 0);
    return (jint)(intptr_t)r;
}

static jlong proxy_CallLongMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallLongMethod", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jlong proxy_CallLongMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallLongMethodV", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jlong proxy_CallLongMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallLongMethodA", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jfloat proxy_CallFloatMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallFloatMethod", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jfloat proxy_CallFloatMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallFloatMethodV", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jfloat proxy_CallFloatMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallFloatMethodA", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jdouble proxy_CallDoubleMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallDoubleMethod", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static jdouble proxy_CallDoubleMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallDoubleMethodV", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static jdouble proxy_CallDoubleMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallDoubleMethodA", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static void proxy_CallVoidMethod(JNIEnv* a0, jobject a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallVoidMethod", NULL, 0);
}

static void proxy_CallVoidMethodV(JNIEnv* a0, jobject a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallVoidMethodV", NULL, 0);
}

static void proxy_CallVoidMethodA(JNIEnv* a0, jobject a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallVoidMethodA", NULL, 0);
}

static jfieldID proxy_GetFieldID(JNIEnv* a0, jclass a1, const char* a2, const char* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetFieldID", NULL, 0);
    return (jfieldID)(intptr_t)r;
}

static jobject proxy_GetObjectField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetObjectField", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jboolean proxy_GetBooleanField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetBooleanField", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jbyte proxy_GetByteField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetByteField", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jchar proxy_GetCharField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetCharField", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jshort proxy_GetShortField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetShortField", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jint proxy_GetIntField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetIntField", NULL, 0);
    return (jint)(intptr_t)r;
}

static jlong proxy_GetLongField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetLongField", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jfloat proxy_GetFloatField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetFloatField", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jdouble proxy_GetDoubleField(JNIEnv* a0, jobject a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetDoubleField", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static void proxy_SetObjectField(JNIEnv* a0, jobject a1, jfieldID a2, jobject a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetObjectField", NULL, 0);
}

static void proxy_SetBooleanField(JNIEnv* a0, jobject a1, jfieldID a2, jboolean a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetBooleanField", NULL, 0);
}

static void proxy_SetByteField(JNIEnv* a0, jobject a1, jfieldID a2, jbyte a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetByteField", NULL, 0);
}

static void proxy_SetCharField(JNIEnv* a0, jobject a1, jfieldID a2, jchar a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetCharField", NULL, 0);
}

static void proxy_SetShortField(JNIEnv* a0, jobject a1, jfieldID a2, jshort a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetShortField", NULL, 0);
}

static void proxy_SetIntField(JNIEnv* a0, jobject a1, jfieldID a2, jint a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetIntField", NULL, 0);
}

static void proxy_SetLongField(JNIEnv* a0, jobject a1, jfieldID a2, jlong a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetLongField", NULL, 0);
}

static void proxy_SetFloatField(JNIEnv* a0, jobject a1, jfieldID a2, jfloat a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetFloatField", NULL, 0);
}

static void proxy_SetDoubleField(JNIEnv* a0, jobject a1, jfieldID a2, jdouble a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetDoubleField", NULL, 0);
}

static jmethodID proxy_GetStaticMethodID(JNIEnv* a0, jclass a1, const char* a2, const char* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticMethodID", NULL, 0);
    return (jmethodID)(intptr_t)r;
}

static jobject proxy_CallStaticObjectMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticObjectMethod", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_CallStaticObjectMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticObjectMethodV", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jobject proxy_CallStaticObjectMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticObjectMethodA", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jboolean proxy_CallStaticBooleanMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticBooleanMethod", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jboolean proxy_CallStaticBooleanMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticBooleanMethodA", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jbyte proxy_CallStaticByteMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticByteMethod", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jbyte proxy_CallStaticByteMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticByteMethodV", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jbyte proxy_CallStaticByteMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticByteMethodA", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jchar proxy_CallStaticCharMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticCharMethod", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jchar proxy_CallStaticCharMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticCharMethodV", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jchar proxy_CallStaticCharMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticCharMethodA", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jshort proxy_CallStaticShortMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticShortMethod", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jshort proxy_CallStaticShortMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticShortMethodV", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jshort proxy_CallStaticShortMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticShortMethodA", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jint proxy_CallStaticIntMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticIntMethod", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_CallStaticIntMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticIntMethodV", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_CallStaticIntMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticIntMethodA", NULL, 0);
    return (jint)(intptr_t)r;
}

static jlong proxy_CallStaticLongMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticLongMethod", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jlong proxy_CallStaticLongMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticLongMethodV", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jlong proxy_CallStaticLongMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticLongMethodA", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jfloat proxy_CallStaticFloatMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticFloatMethod", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jfloat proxy_CallStaticFloatMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticFloatMethodV", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jfloat proxy_CallStaticFloatMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticFloatMethodA", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jdouble proxy_CallStaticDoubleMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticDoubleMethod", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static jdouble proxy_CallStaticDoubleMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticDoubleMethodV", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static jdouble proxy_CallStaticDoubleMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticDoubleMethodA", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static void proxy_CallStaticVoidMethod(JNIEnv* a0, jclass a1, jmethodID a2, ...) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticVoidMethod", NULL, 0);
}

static void proxy_CallStaticVoidMethodV(JNIEnv* a0, jclass a1, jmethodID a2, va_list a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticVoidMethodV", NULL, 0);
}

static void proxy_CallStaticVoidMethodA(JNIEnv* a0, jclass a1, jmethodID a2, const jvalue* a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("CallStaticVoidMethodA", NULL, 0);
}

static jobject proxy_GetStaticObjectField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticObjectField", NULL, 0);
    return (jobject)(intptr_t)r;
}

static jboolean proxy_GetStaticBooleanField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticBooleanField", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jbyte proxy_GetStaticByteField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticByteField", NULL, 0);
    return (jbyte)(intptr_t)r;
}

static jchar proxy_GetStaticCharField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticCharField", NULL, 0);
    return (jchar)(intptr_t)r;
}

static jshort proxy_GetStaticShortField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticShortField", NULL, 0);
    return (jshort)(intptr_t)r;
}

static jint proxy_GetStaticIntField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticIntField", NULL, 0);
    return (jint)(intptr_t)r;
}

static jlong proxy_GetStaticLongField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticLongField", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jfloat proxy_GetStaticFloatField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticFloatField", NULL, 0);
    return (jfloat)(intptr_t)r;
}

static jdouble proxy_GetStaticDoubleField(JNIEnv* a0, jclass a1, jfieldID a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStaticDoubleField", NULL, 0);
    return (jdouble)(intptr_t)r;
}

static void proxy_SetStaticObjectField(JNIEnv* a0, jclass a1, jfieldID a2, jobject a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticObjectField", NULL, 0);
}

static void proxy_SetStaticBooleanField(JNIEnv* a0, jclass a1, jfieldID a2, jboolean a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticBooleanField", NULL, 0);
}

static void proxy_SetStaticByteField(JNIEnv* a0, jclass a1, jfieldID a2, jbyte a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticByteField", NULL, 0);
}

static void proxy_SetStaticCharField(JNIEnv* a0, jclass a1, jfieldID a2, jchar a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticCharField", NULL, 0);
}

static void proxy_SetStaticShortField(JNIEnv* a0, jclass a1, jfieldID a2, jshort a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticShortField", NULL, 0);
}

static void proxy_SetStaticIntField(JNIEnv* a0, jclass a1, jfieldID a2, jint a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticIntField", NULL, 0);
}

static void proxy_SetStaticLongField(JNIEnv* a0, jclass a1, jfieldID a2, jlong a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticLongField", NULL, 0);
}

static void proxy_SetStaticFloatField(JNIEnv* a0, jclass a1, jfieldID a2, jfloat a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticFloatField", NULL, 0);
}

static void proxy_SetStaticDoubleField(JNIEnv* a0, jclass a1, jfieldID a2, jdouble a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetStaticDoubleField", NULL, 0);
}

static jstring proxy_NewString(JNIEnv* a0, const jchar* a1, jsize a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewString", NULL, 0);
    return (jstring)(intptr_t)r;
}

static jsize proxy_GetStringLength(JNIEnv* a0, jstring a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringLength", NULL, 0);
    return (jsize)(intptr_t)r;
}

static const jchar* proxy_GetStringChars(JNIEnv* a0, jstring a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringChars", NULL, 0);
    return (const jchar*)(intptr_t)r;
}

static void proxy_ReleaseStringChars(JNIEnv* a0, jstring a1, const jchar* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ReleaseStringChars", NULL, 0);
}

static jstring proxy_NewStringUTF(JNIEnv* a0, const char* a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewStringUTF", NULL, 0);
    return (jstring)(intptr_t)r;
}

static jsize proxy_GetStringUTFLength(JNIEnv* a0, jstring a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringUTFLength", NULL, 0);
    return (jsize)(intptr_t)r;
}

static const char* proxy_GetStringUTFChars(JNIEnv* a0, jstring a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringUTFChars", NULL, 0);
    return (const char*)(intptr_t)r;
}

static void proxy_ReleaseStringUTFChars(JNIEnv* a0, jstring a1, const char* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ReleaseStringUTFChars", NULL, 0);
}

static jsize proxy_GetArrayLength(JNIEnv* a0, jarray a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetArrayLength", NULL, 0);
    return (jsize)(intptr_t)r;
}

static jobjectArray proxy_NewObjectArray(JNIEnv* a0, jsize a1, jclass a2, jobject a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewObjectArray", NULL, 0);
    return (jobjectArray)(intptr_t)r;
}

static jobject proxy_GetObjectArrayElement(JNIEnv* a0, jobjectArray a1, jsize a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetObjectArrayElement", NULL, 0);
    return (jobject)(intptr_t)r;
}

static void proxy_SetObjectArrayElement(JNIEnv* a0, jobjectArray a1, jsize a2, jobject a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("SetObjectArrayElement", NULL, 0);
}

static jbooleanArray proxy_NewBooleanArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewBooleanArray", NULL, 0);
    return (jbooleanArray)(intptr_t)r;
}

static jbyteArray proxy_NewByteArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewByteArray", NULL, 0);
    return (jbyteArray)(intptr_t)r;
}

static jcharArray proxy_NewCharArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewCharArray", NULL, 0);
    return (jcharArray)(intptr_t)r;
}

static jshortArray proxy_NewShortArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewShortArray", NULL, 0);
    return (jshortArray)(intptr_t)r;
}

static jintArray proxy_NewIntArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewIntArray", NULL, 0);
    return (jintArray)(intptr_t)r;
}

static jlongArray proxy_NewLongArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewLongArray", NULL, 0);
    return (jlongArray)(intptr_t)r;
}

static jfloatArray proxy_NewFloatArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewFloatArray", NULL, 0);
    return (jfloatArray)(intptr_t)r;
}

static jdoubleArray proxy_NewDoubleArray(JNIEnv* a0, jsize a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewDoubleArray", NULL, 0);
    return (jdoubleArray)(intptr_t)r;
}

static jboolean* proxy_GetBooleanArrayElements(JNIEnv* a0, jbooleanArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetBooleanArrayElements", NULL, 0);
    return (jboolean*)(intptr_t)r;
}

static jbyte* proxy_GetByteArrayElements(JNIEnv* a0, jbyteArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetByteArrayElements", NULL, 0);
    return (jbyte*)(intptr_t)r;
}

static jchar* proxy_GetCharArrayElements(JNIEnv* a0, jcharArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetCharArrayElements", NULL, 0);
    return (jchar*)(intptr_t)r;
}

static jshort* proxy_GetShortArrayElements(JNIEnv* a0, jshortArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetShortArrayElements", NULL, 0);
    return (jshort*)(intptr_t)r;
}

static jint* proxy_GetIntArrayElements(JNIEnv* a0, jintArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetIntArrayElements", NULL, 0);
    return (jint*)(intptr_t)r;
}

static jlong* proxy_GetLongArrayElements(JNIEnv* a0, jlongArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetLongArrayElements", NULL, 0);
    return (jlong*)(intptr_t)r;
}

static jfloat* proxy_GetFloatArrayElements(JNIEnv* a0, jfloatArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetFloatArrayElements", NULL, 0);
    return (jfloat*)(intptr_t)r;
}

static jdouble* proxy_GetDoubleArrayElements(JNIEnv* a0, jdoubleArray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetDoubleArrayElements", NULL, 0);
    return (jdouble*)(intptr_t)r;
}

static jint proxy_UnregisterNatives(JNIEnv* a0, jclass a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("UnregisterNatives", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_MonitorEnter(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("MonitorEnter", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_MonitorExit(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("MonitorExit", NULL, 0);
    return (jint)(intptr_t)r;
}

static jint proxy_GetJavaVM(JNIEnv* a0, JavaVM** a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetJavaVM", NULL, 0);
    return (jint)(intptr_t)r;
}

static void proxy_GetStringRegion(JNIEnv* a0, jstring a1, jsize a2, jsize a3, jchar* a4) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringRegion", NULL, 0);
}

static void proxy_GetStringUTFRegion(JNIEnv* a0, jstring a1, jsize a2, jsize a3, char* a4) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringUTFRegion", NULL, 0);
}

static void* proxy_GetPrimitiveArrayCritical(JNIEnv* a0, jarray a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetPrimitiveArrayCritical", NULL, 0);
    return (void*)(intptr_t)r;
}

static void proxy_ReleasePrimitiveArrayCritical(JNIEnv* a0, jarray a1, void* a2, jint a3) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ReleasePrimitiveArrayCritical", NULL, 0);
}

static const jchar* proxy_GetStringCritical(JNIEnv* a0, jstring a1, jboolean* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetStringCritical", NULL, 0);
    return (const jchar*)(intptr_t)r;
}

static void proxy_ReleaseStringCritical(JNIEnv* a0, jstring a1, const jchar* a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ReleaseStringCritical", NULL, 0);
}

static jweak proxy_NewWeakGlobalRef(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewWeakGlobalRef", NULL, 0);
    return (jweak)(intptr_t)r;
}

static void proxy_DeleteWeakGlobalRef(JNIEnv* a0, jweak a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("DeleteWeakGlobalRef", NULL, 0);
}

static jboolean proxy_ExceptionCheck(JNIEnv* a0) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("ExceptionCheck", NULL, 0);
    return (jboolean)(intptr_t)r;
}

static jobject proxy_NewDirectByteBuffer(JNIEnv* a0, void* a1, jlong a2) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("NewDirectByteBuffer", NULL, 0);
    return (jobject)(intptr_t)r;
}

static void* proxy_GetDirectBufferAddress(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetDirectBufferAddress", NULL, 0);
    return (void*)(intptr_t)r;
}

static jlong proxy_GetDirectBufferCapacity(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetDirectBufferCapacity", NULL, 0);
    return (jlong)(intptr_t)r;
}

static jobjectRefType proxy_GetObjectRefType(JNIEnv* a0, jobject a1) {
    // TODO: marshal args into payload
    int64_t r = proxy_jnienv("GetObjectRefType", NULL, 0);
    return (jobjectRefType)(intptr_t)r;
}

static const struct JNINativeInterface jni_vtable = {
    .reserved0 = NULL,
    .reserved1 = NULL,
    .reserved2 = NULL,
    .reserved3 = NULL,
    .GetVersion = proxy_GetVersion,
    .FindClass = proxy_FindClass,
    .FromReflectedMethod = proxy_FromReflectedMethod,
    .FromReflectedField = proxy_FromReflectedField,
    .ToReflectedMethod = proxy_ToReflectedMethod,
    .GetSuperclass = proxy_GetSuperclass,
    .IsAssignableFrom = proxy_IsAssignableFrom,
    .ToReflectedField = proxy_ToReflectedField,
    .Throw = proxy_Throw,
    .ThrowNew = proxy_ThrowNew,
    .ExceptionOccurred = proxy_ExceptionOccurred,
    .ExceptionDescribe = proxy_ExceptionDescribe,
    .ExceptionClear = proxy_ExceptionClear,
    .FatalError = proxy_FatalError,
    .PushLocalFrame = proxy_PushLocalFrame,
    .PopLocalFrame = proxy_PopLocalFrame,
    .NewGlobalRef = proxy_NewGlobalRef,
    .DeleteGlobalRef = proxy_DeleteGlobalRef,
    .DeleteLocalRef = proxy_DeleteLocalRef,
    .IsSameObject = proxy_IsSameObject,
    .NewLocalRef = proxy_NewLocalRef,
    .EnsureLocalCapacity = proxy_EnsureLocalCapacity,
    .AllocObject = proxy_AllocObject,
    .NewObject = proxy_NewObject,
    .NewObjectV = proxy_NewObjectV,
    .NewObjectA = proxy_NewObjectA,
    .GetObjectClass = proxy_GetObjectClass,
    .IsInstanceOf = proxy_IsInstanceOf,
    .GetMethodID = proxy_GetMethodID,
    .CallObjectMethod = proxy_CallObjectMethod,
    .CallObjectMethodV = proxy_CallObjectMethodV,
    .CallObjectMethodA = proxy_CallObjectMethodA,
    .CallBooleanMethod = proxy_CallBooleanMethod,
    .CallBooleanMethodV = proxy_CallBooleanMethodV,
    .CallBooleanMethodA = proxy_CallBooleanMethodA,
    .CallByteMethod = proxy_CallByteMethod,
    .CallByteMethodV = proxy_CallByteMethodV,
    .CallByteMethodA = proxy_CallByteMethodA,
    .CallCharMethod = proxy_CallCharMethod,
    .CallCharMethodV = proxy_CallCharMethodV,
    .CallCharMethodA = proxy_CallCharMethodA,
    .CallShortMethod = proxy_CallShortMethod,
    .CallShortMethodV = proxy_CallShortMethodV,
    .CallShortMethodA = proxy_CallShortMethodA,
    .CallIntMethod = proxy_CallIntMethod,
    .CallIntMethodV = proxy_CallIntMethodV,
    .CallIntMethodA = proxy_CallIntMethodA,
    .CallLongMethod = proxy_CallLongMethod,
    .CallLongMethodV = proxy_CallLongMethodV,
    .CallLongMethodA = proxy_CallLongMethodA,
    .CallFloatMethod = proxy_CallFloatMethod,
    .CallFloatMethodV = proxy_CallFloatMethodV,
    .CallFloatMethodA = proxy_CallFloatMethodA,
    .CallDoubleMethod = proxy_CallDoubleMethod,
    .CallDoubleMethodV = proxy_CallDoubleMethodV,
    .CallDoubleMethodA = proxy_CallDoubleMethodA,
    .CallVoidMethod = proxy_CallVoidMethod,
    .CallVoidMethodV = proxy_CallVoidMethodV,
    .CallVoidMethodA = proxy_CallVoidMethodA,
    .GetFieldID = proxy_GetFieldID,
    .GetObjectField = proxy_GetObjectField,
    .GetBooleanField = proxy_GetBooleanField,
    .GetByteField = proxy_GetByteField,
    .GetCharField = proxy_GetCharField,
    .GetShortField = proxy_GetShortField,
    .GetIntField = proxy_GetIntField,
    .GetLongField = proxy_GetLongField,
    .GetFloatField = proxy_GetFloatField,
    .GetDoubleField = proxy_GetDoubleField,
    .SetObjectField = proxy_SetObjectField,
    .SetBooleanField = proxy_SetBooleanField,
    .SetByteField = proxy_SetByteField,
    .SetCharField = proxy_SetCharField,
    .SetShortField = proxy_SetShortField,
    .SetIntField = proxy_SetIntField,
    .SetLongField = proxy_SetLongField,
    .SetFloatField = proxy_SetFloatField,
    .SetDoubleField = proxy_SetDoubleField,
    .GetStaticMethodID = proxy_GetStaticMethodID,
    .CallStaticObjectMethod = proxy_CallStaticObjectMethod,
    .CallStaticObjectMethodV = proxy_CallStaticObjectMethodV,
    .CallStaticObjectMethodA = proxy_CallStaticObjectMethodA,
    .CallStaticBooleanMethod = proxy_CallStaticBooleanMethod,
    .CallStaticBooleanMethodA = proxy_CallStaticBooleanMethodA,
    .CallStaticByteMethod = proxy_CallStaticByteMethod,
    .CallStaticByteMethodV = proxy_CallStaticByteMethodV,
    .CallStaticByteMethodA = proxy_CallStaticByteMethodA,
    .CallStaticCharMethod = proxy_CallStaticCharMethod,
    .CallStaticCharMethodV = proxy_CallStaticCharMethodV,
    .CallStaticCharMethodA = proxy_CallStaticCharMethodA,
    .CallStaticShortMethod = proxy_CallStaticShortMethod,
    .CallStaticShortMethodV = proxy_CallStaticShortMethodV,
    .CallStaticShortMethodA = proxy_CallStaticShortMethodA,
    .CallStaticIntMethod = proxy_CallStaticIntMethod,
    .CallStaticIntMethodV = proxy_CallStaticIntMethodV,
    .CallStaticIntMethodA = proxy_CallStaticIntMethodA,
    .CallStaticLongMethod = proxy_CallStaticLongMethod,
    .CallStaticLongMethodV = proxy_CallStaticLongMethodV,
    .CallStaticLongMethodA = proxy_CallStaticLongMethodA,
    .CallStaticFloatMethod = proxy_CallStaticFloatMethod,
    .CallStaticFloatMethodV = proxy_CallStaticFloatMethodV,
    .CallStaticFloatMethodA = proxy_CallStaticFloatMethodA,
    .CallStaticDoubleMethod = proxy_CallStaticDoubleMethod,
    .CallStaticDoubleMethodV = proxy_CallStaticDoubleMethodV,
    .CallStaticDoubleMethodA = proxy_CallStaticDoubleMethodA,
    .CallStaticVoidMethod = proxy_CallStaticVoidMethod,
    .CallStaticVoidMethodV = proxy_CallStaticVoidMethodV,
    .CallStaticVoidMethodA = proxy_CallStaticVoidMethodA,
    .GetStaticObjectField = proxy_GetStaticObjectField,
    .GetStaticBooleanField = proxy_GetStaticBooleanField,
    .GetStaticByteField = proxy_GetStaticByteField,
    .GetStaticCharField = proxy_GetStaticCharField,
    .GetStaticShortField = proxy_GetStaticShortField,
    .GetStaticIntField = proxy_GetStaticIntField,
    .GetStaticLongField = proxy_GetStaticLongField,
    .GetStaticFloatField = proxy_GetStaticFloatField,
    .GetStaticDoubleField = proxy_GetStaticDoubleField,
    .SetStaticObjectField = proxy_SetStaticObjectField,
    .SetStaticBooleanField = proxy_SetStaticBooleanField,
    .SetStaticByteField = proxy_SetStaticByteField,
    .SetStaticCharField = proxy_SetStaticCharField,
    .SetStaticShortField = proxy_SetStaticShortField,
    .SetStaticIntField = proxy_SetStaticIntField,
    .SetStaticLongField = proxy_SetStaticLongField,
    .SetStaticFloatField = proxy_SetStaticFloatField,
    .SetStaticDoubleField = proxy_SetStaticDoubleField,
    .NewString = proxy_NewString,
    .GetStringLength = proxy_GetStringLength,
    .GetStringChars = proxy_GetStringChars,
    .ReleaseStringChars = proxy_ReleaseStringChars,
    .NewStringUTF = proxy_NewStringUTF,
    .GetStringUTFLength = proxy_GetStringUTFLength,
    .GetStringUTFChars = proxy_GetStringUTFChars,
    .ReleaseStringUTFChars = proxy_ReleaseStringUTFChars,
    .GetArrayLength = proxy_GetArrayLength,
    .NewObjectArray = proxy_NewObjectArray,
    .GetObjectArrayElement = proxy_GetObjectArrayElement,
    .SetObjectArrayElement = proxy_SetObjectArrayElement,
    .NewBooleanArray = proxy_NewBooleanArray,
    .NewByteArray = proxy_NewByteArray,
    .NewCharArray = proxy_NewCharArray,
    .NewShortArray = proxy_NewShortArray,
    .NewIntArray = proxy_NewIntArray,
    .NewLongArray = proxy_NewLongArray,
    .NewFloatArray = proxy_NewFloatArray,
    .NewDoubleArray = proxy_NewDoubleArray,
    .GetBooleanArrayElements = proxy_GetBooleanArrayElements,
    .GetByteArrayElements = proxy_GetByteArrayElements,
    .GetCharArrayElements = proxy_GetCharArrayElements,
    .GetShortArrayElements = proxy_GetShortArrayElements,
    .GetIntArrayElements = proxy_GetIntArrayElements,
    .GetLongArrayElements = proxy_GetLongArrayElements,
    .GetFloatArrayElements = proxy_GetFloatArrayElements,
    .GetDoubleArrayElements = proxy_GetDoubleArrayElements,
    .UnregisterNatives = proxy_UnregisterNatives,
    .MonitorEnter = proxy_MonitorEnter,
    .MonitorExit = proxy_MonitorExit,
    .GetJavaVM = proxy_GetJavaVM,
    .GetStringRegion = proxy_GetStringRegion,
    .GetStringUTFRegion = proxy_GetStringUTFRegion,
    .GetPrimitiveArrayCritical = proxy_GetPrimitiveArrayCritical,
    .ReleasePrimitiveArrayCritical = proxy_ReleasePrimitiveArrayCritical,
    .GetStringCritical = proxy_GetStringCritical,
    .ReleaseStringCritical = proxy_ReleaseStringCritical,
    .NewWeakGlobalRef = proxy_NewWeakGlobalRef,
    .DeleteWeakGlobalRef = proxy_DeleteWeakGlobalRef,
    .ExceptionCheck = proxy_ExceptionCheck,
    .NewDirectByteBuffer = proxy_NewDirectByteBuffer,
    .GetDirectBufferAddress = proxy_GetDirectBufferAddress,
    .GetDirectBufferCapacity = proxy_GetDirectBufferCapacity,
    .GetObjectRefType = proxy_GetObjectRefType,
};

static const struct JNINativeInterface *jni_vtable_ptr = &jni_vtable;
JNIEnv *jnienv_get(void) { return (JNIEnv *)&jni_vtable_ptr; }
