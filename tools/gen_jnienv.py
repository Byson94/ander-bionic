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
""")

# Generate a proxy stub for each slot
for ret, name, args in slots:
    if args is None:
        # reserved
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