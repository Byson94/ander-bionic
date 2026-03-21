#!/usr/bin/env python3
import re, sys

jni_h = open(sys.argv[1]).read()

# Extract struct JNINativeInterface body
match = re.search(r'struct JNINativeInterface \{(.+?)\};', jni_h, re.DOTALL)
body = match.group(1)

# Parse each slot
slots = []
for line in body.splitlines():
    line = line.strip()
    # Match: rettype (*FuncName)(args);
    m = re.match(r'(\S+.*?)\(\*(\w+)\)\((.*?)\);', line)
    if not m:
        # reserved slots
        if 'reserved' in line:
            m2 = re.match(r'void\*\s+(\w+);', line)
            if m2:
                slots.append(('void*', m2.group(1), None))
        continue
    ret = m.group(1).strip()
    name = m.group(2)
    args = m.group(3).strip()
    slots.append((ret, name, args))

# Generate jnienv.c
print("""
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <jni.h>
#include "include/jnienv.h"
#include "include/ipc.h"

extern int g_socket_fd;
extern uint32_t g_current_call_id;

static int read_all_fd(int fd, void *buf, size_t len) {
    uint8_t *p = buf;
    while (len > 0) {
        ssize_t n = read(fd, p, len);
        if (n <= 0) return -1;
        p += n; len -= n;
    }
    return 0;
}

static int64_t proxy_jnienv(const char *method, const void *args, uint32_t args_len) {
    uint32_t mlen  = strlen(method) + 1;
    uint32_t total = mlen + args_len;
    uint8_t *payload = malloc(total);
    memcpy(payload, method, mlen);
    if (args_len > 0) memcpy(payload + mlen, args, args_len);
    msg_send(g_socket_fd, MSG_JNIENV, g_current_call_id, payload, total);
    free(payload);

    int64_t result = 0;
    while (1) {
        MsgHeader hdr;
        if (read_all_fd(g_socket_fd, &hdr, sizeof(MsgHeader)) < 0) break;
        if (hdr.id != g_current_call_id) continue;

        if (hdr.type == MSG_JNIENV_RETURN) {
            uint8_t buf[8] = {0};
            if (hdr.data_len > 0 && read_all_fd(g_socket_fd, buf, hdr.data_len < 8 ? hdr.data_len : 8) < 0) break;
            memcpy(&result, buf, 8);
            break;
        }

        if (hdr.type == MSG_JNIENV_RETURN_DATA) {
            // Dynamically allocate — payload can be larger than 65536
            uint8_t *buf = malloc(hdr.data_len);
            if (!buf) break;
            if (read_all_fd(g_socket_fd, buf, hdr.data_len) < 0) { free(buf); break; }
            fprintf(stderr, "[jnienv] RETURN_DATA dlen=%u\\n", hdr.data_len);
            fflush(stderr);
            // Skip 8-byte handle, return pointer to actual data
            result = (int64_t)(intptr_t)(buf + 8);
            // buf intentionally not freed — caller owns the data
            break;
        }

        // Drain unknown message
        if (hdr.data_len > 0) {
            uint8_t *tmp = malloc(hdr.data_len);
            if (tmp) { read_all_fd(g_socket_fd, tmp, hdr.data_len); free(tmp); }
        }
    }

    return result;
}
""")

MANUAL_OVERRIDES = {
    'GetPrimitiveArrayCritical': """static void* proxy_GetPrimitiveArrayCritical(JNIEnv* a0, jarray a1, jboolean* a2) {
    fprintf(stderr, "[jnienv] GetPrimitiveArrayCritical arr=%p\\n", (void*)a1);
    fflush(stderr);
    int64_t r = proxy_jnienv("GetPrimitiveArrayCritical", &a1, sizeof(a1));
    fprintf(stderr, "[jnienv] GetPrimitiveArrayCritical result=%p\\n", (void*)r);
    fflush(stderr);
    return (void*)(intptr_t)r;
}""",
    'ReleasePrimitiveArrayCritical': """static void proxy_ReleasePrimitiveArrayCritical(JNIEnv* a0, jarray a1, void* a2, jint a3) {
    // Data was already copied — no-op
    (void)a0; (void)a1; (void)a2; (void)a3;
}""",
    'NewDirectByteBuffer': """static jobject proxy_NewDirectByteBuffer(JNIEnv* a0, void* a1, jlong a2) {
    uint32_t cap = (uint32_t)a2;
    uint32_t payload_len = sizeof(a1) + sizeof(a2) + cap;
    uint8_t *payload = malloc(payload_len);
    memcpy(payload,                          &a1, sizeof(a1));
    memcpy(payload + sizeof(a1),             &a2, sizeof(a2));
    memcpy(payload + sizeof(a1) + sizeof(a2), a1, cap);
    int64_t r = proxy_jnienv("NewDirectByteBuffer", payload, payload_len);
    free(payload);
    return (jobject)(intptr_t)r;
}""",
    'NewStringUTF': """static jstring proxy_NewStringUTF(JNIEnv* a0, const char* a1) {
    uint32_t len = a1 ? strlen(a1) + 1 : 1;
    const char *str = a1 ? a1 : "";
    int64_t r = proxy_jnienv("NewStringUTF", str, len);
    return (jstring)(intptr_t)r;
}""",
}

# Generate a proxy stub for each slot
for ret, name, args in slots:
    if args is None:
        # reserved
        continue

    if name in MANUAL_OVERRIDES:
        print(MANUAL_OVERRIDES[name])
        print()
        continue

    # Build arg names: a0, a1, a2...
    arg_list = [a.strip() for a in args.split(',') if a.strip() and a.strip() != 'void']
    named_args = []
    for i, a in enumerate(arg_list):
        if a == '...':
            named_args.append('...')
        else:
            named_args.append(f"{a} a{i}")

    named_args_str = ', '.join(named_args)
    ret_cast = '(void*)' if ret not in ('void', 'jint', 'jsize', 'jboolean', 'jbyte', 'jchar', 'jshort', 'jlong', 'jfloat', 'jdouble') else ''

    print(f"static {ret} proxy_{name}({named_args_str}) {{")
    print(f"    // TODO: marshal args into payload")
    print(f"    int64_t r = proxy_jnienv(\"{name}\", NULL, 0);")
    if ret != 'void':
        print(f"    return ({ret})(intptr_t)r;")
    print(f"}}")
    print()

# Generate the vtable
print("static const struct JNINativeInterface jni_vtable = {")
for ret, name, args in slots:
    if args is None:
        print(f"    .{name} = NULL,")
    else:
        print(f"    .{name} = proxy_{name},")
print("};")
print()
print("static const struct JNINativeInterface *jni_vtable_ptr = &jni_vtable;")
print("JNIEnv *jnienv_get(void) { return (JNIEnv *)&jni_vtable_ptr; }")
