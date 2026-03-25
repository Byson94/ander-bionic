#include "include/ipc.h"
#include "include/symbols.h"
#include "include/jnienv.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <ffi.h>

int g_socket_fd = -1;
uint32_t g_current_call_id = 0;

// LibFFI helpers
static ffi_type *sig_char_to_ffi(char c) {
    switch (c) {
        case 'Z': return &ffi_type_uint8;
        case 'B': return &ffi_type_sint8;
        case 'C': return &ffi_type_uint16;
        case 'S': return &ffi_type_sint16;
        case 'I': return &ffi_type_sint32;
        case 'J': return &ffi_type_sint64;
        case 'F': return &ffi_type_float;
        case 'D': return &ffi_type_double;
        case 'V': return &ffi_type_void;
        default:  return &ffi_type_pointer;
    }
}

static ffi_type *parse_ret_type(const char *sig) {
    const char *p = strchr(sig, ')');
    if (!p) return &ffi_type_pointer;
    p++;
    if (*p == '[' || *p == 'L') return &ffi_type_pointer;
    return sig_char_to_ffi(*p);
}

// Fills arg_types[0..], returns total count including env+cls
static int parse_arg_types(const char *sig, ffi_type **arg_types) {
    arg_types[0] = &ffi_type_pointer; // JNIEnv*
    arg_types[1] = &ffi_type_pointer; // jclass
    int count = 2;
    const char *p = sig + 1; // skip '('
    while (*p && *p != ')') {
        if (*p == '[') {
            while (*p == '[') p++;
            if (*p == 'L') { while (*p && *p != ';') p++; }
            arg_types[count++] = &ffi_type_pointer;
        } else if (*p == 'L') {
            while (*p && *p != ';') p++;
            arg_types[count++] = &ffi_type_pointer;
        } else {
            arg_types[count++] = sig_char_to_ffi(*p);
        }
        if (*p) p++;
    }
    return count;
}

// IO helpers
static int write_all(int fd, const void *buf, size_t len) {
    const uint8_t *p = buf;
    while (len > 0) {
        ssize_t n = write(fd, p, len);
        if (n <= 0) return -1;
        p += n; len -= n;
    }
    return 0;
}

static int read_all(int fd, void *buf, size_t len) {
    uint8_t *p = buf;
    while (len > 0) {
        ssize_t n = read(fd, p, len);
        if (n == 0) return -1;
        if (n < 0) {
            perror("read_all");
            return -1;
        }
        p += n; len -= n;
    }
    return 0;
}

int msg_send(int fd, uint8_t type, uint32_t id, const void *data, uint32_t len) {
    MsgHeader hdr = { type, id, len };
    if (write_all(fd, &hdr, sizeof(hdr)) < 0) return -1;
    if (len > 0 && write_all(fd, data, len) < 0) return -1;
    return 0;
}

static int msg_recv_dyn(int fd, MsgHeader *hdr, uint8_t **buf_out) {
    if (read_all(fd, hdr, sizeof(MsgHeader)) < 0) {
        fprintf(stderr, "[ipc] recv: header read failed\n");
        fflush(stderr);
        return -1;
    }
    if (hdr->data_len == 0) {
        *buf_out = NULL;
        return 0;
    }
    *buf_out = malloc(hdr->data_len);
    if (!*buf_out) {
        fprintf(stderr, "[ipc] recv: OOM for dlen=%u\n", hdr->data_len);
        fflush(stderr);
        return -1;
    }
    if (read_all(fd, *buf_out, hdr->data_len) < 0) {
        fprintf(stderr, "[ipc] recv: body read failed\n");
        fflush(stderr);
        free(*buf_out);
        *buf_out = NULL;
        return -1;
    }
    return 0;
}

typedef struct { char *buf; uint32_t offset; uint32_t capacity; } ListCtx;

static void collect_java_symbols(const char *name, void *udata) {
    ListCtx *ctx = udata;
    if (strncmp(name, "Java_", 5) != 0) return;
    size_t len = strlen(name) + 1;
    if (ctx->offset + len + 1 >= ctx->capacity) return;
    memcpy(ctx->buf + ctx->offset, name, len);
    ctx->offset += len;
}

// IPC loop
void ipc_serve(int client_fd) {
    MsgHeader hdr;

    fprintf(stdout, "ipc_serve: starting loop\n");
    fflush(stdout);

    while (1) {
        uint8_t *buf = NULL;
        if (msg_recv_dyn(client_fd, &hdr, &buf) < 0) {
            fprintf(stderr, "client disconnected\n");
            break;
        }

        switch (hdr.type) {
            case MSG_CALL: {
                char *symbol = (char *)buf;
                uint32_t sym_len = strlen(symbol) + 1;
                char *signature = (char *)(buf + sym_len);
                uint32_t sig_len = strlen(signature) + 1;
                uint8_t *args_payload = buf + sym_len + sig_len;
                uint32_t argc = (hdr.data_len - sym_len - sig_len) / 8;

                void *sym = symbols_find(symbol);
                if (!sym) {
                    const char *err = "symbol not found";
                    msg_send(client_fd, MSG_ERROR, hdr.id, err, strlen(err) + 1);
                    free(buf);
                    break;
                }

                g_current_call_id = hdr.id;

                // Build CIF from signature
                ffi_type *arg_types[32];
                int total_args = parse_arg_types(signature, arg_types);
                ffi_type *ret_type = parse_ret_type(signature);

                ffi_cif cif;
                if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, total_args,
                                ret_type, arg_types) != FFI_OK) {
                    fprintf(stderr, "[ipc] ffi_prep_cif failed for %s %s\n", symbol, signature);
                    fflush(stderr);
                    uint64_t zero = 0;
                    msg_send(client_fd, MSG_RETURN, hdr.id, &zero, sizeof(zero));
                    free(buf);
                    break;
                }

                // Each slot is 8 bytes, arg_vals[i] points into it
                uint8_t storage[32 * 8] = {0};
                void *arg_vals[32];

                // env and cls
                JNIEnv *env = jnienv_get();
                void   *cls = NULL;
                arg_vals[0] = &env;
                arg_vals[1] = &cls;

                // Walk signature to fill remaining args from payload
                const char *p = signature + 1;
                int ai = 2; // arg index (after env+cls)
                int pi = 0; // payload index
                while (*p && *p != ')' && ai < 32) {
                    uint8_t *slot = storage + ai * 8;
                    uint64_t raw;
                    memcpy(&raw, args_payload + pi * 8, 8);
                    pi++;

                    if (*p == '[') {
                        while (*p == '[') p++;
                        if (*p == 'L') { while (*p && *p != ';') p++; }
                        void *ptr = (void*)(uintptr_t)raw;
                        memcpy(slot, &ptr, sizeof(void*));
                    } else if (*p == 'L') {
                        while (*p && *p != ';') p++;
                        void *ptr = (void*)(uintptr_t)raw;
                        memcpy(slot, &ptr, sizeof(void*));
                    } else if (*p == 'F') {
                        float f; memcpy(&f, &raw, sizeof(float));
                        memcpy(slot, &f, sizeof(float));
                    } else if (*p == 'D') {
                        double d; memcpy(&d, &raw, sizeof(double));
                        memcpy(slot, &d, sizeof(double));
                    } else if (*p == 'J') {
                        int64_t j = (int64_t)raw;
                        memcpy(slot, &j, sizeof(int64_t));
                    } else if (*p == 'I') {
                        int32_t iv = (int32_t)raw;
                        memcpy(slot, &iv, sizeof(int32_t));
                    } else if (*p == 'Z' || *p == 'B') {
                        uint8_t bv = (uint8_t)raw;
                        memcpy(slot, &bv, 1);
                    } else if (*p == 'S' || *p == 'C') {
                        uint16_t sv = (uint16_t)raw;
                        memcpy(slot, &sv, 2);
                    }
                    arg_vals[ai] = slot;
                    ai++;
                    fprintf(stderr, "[ipc] arg parse: p='%c' ai=%d pi=%d raw=0x%llx\n",
                        *p, ai-1, pi-1, (unsigned long long)raw);
                    fflush(stderr);
                    if (*p) p++;
                }

                fprintf(stderr, "[ipc] calling %s sig=%s argc=%u\n", symbol, signature, argc);
                for (int i = 0; i < total_args; i++) {
                    uint64_t v;
                    memcpy(&v, arg_vals[i], 8);
                    fprintf(stderr, "[ipc]   arg[%d]=0x%llx\n", i, (unsigned long long)v);
                }
                fflush(stderr);

                // Call
                uint8_t ret_storage[16] = {0};
                ffi_call(&cif, FFI_FN(sym), ret_storage, arg_vals);

                // Serialize return
                uint64_t ret = 0;
                if (ret_type == &ffi_type_float) {
                    float f; memcpy(&f, ret_storage, sizeof(float));
                    memcpy(&ret, &f, sizeof(float));
                } else if (ret_type == &ffi_type_double) {
                    memcpy(&ret, ret_storage, sizeof(double));
                } else if (ret_type != &ffi_type_void) {
                    memcpy(&ret, ret_storage, sizeof(uint64_t));
                }

                msg_send(client_fd, MSG_RETURN, hdr.id, &ret, sizeof(ret));
                free(buf);
                break;
            }
            case MSG_ARRAY_DATA: {
                uint64_t arm_token;
                uint32_t byte_len;
                memcpy(&arm_token, buf,                   sizeof(arm_token));
                memcpy(&byte_len,  buf + sizeof(uint64_t), sizeof(uint32_t));
                uint8_t *data = buf + sizeof(uint64_t) + sizeof(uint32_t);

                void *host_buf = malloc(byte_len);
                if (!host_buf) {
                    fprintf(stderr, "[ipc] MSG_ARRAY_DATA: OOM\n");
                    free(buf);
                    break;
                }
                memcpy(host_buf, data, byte_len);

                // Check if token already registered, update buffer if so
                int found = 0;
                for (int i = 0; i < g_direct_buf_count; i++) {
                    if (g_direct_bufs[i].arm_handle == (int64_t)arm_token) {
                        // Free old buffer, update with new data
                        free((void*)(uintptr_t)g_direct_bufs[i].host_jobject);
                        g_direct_bufs[i].host_jobject = (jobject)(uintptr_t)host_buf;
                        found = 1;
                        break;
                    }
                }
                if (!found && g_direct_buf_count < MAX_DIRECT_BUFS) {
                    int idx = g_direct_buf_count++;
                    g_direct_bufs[idx].arm_handle   = (int64_t)arm_token;  // ← store TOKEN
                    g_direct_bufs[idx].host_jobject = (jobject)(uintptr_t)host_buf; // ← store buf ptr here
                }

                msg_send(client_fd, MSG_ARRAY_ACK, 0, &arm_token, sizeof(arm_token));
                free(buf);
                break;
            }
            case MSG_LIST_SYMBOLS: {
                uint32_t out_size = 65536;
                char *out = malloc(out_size);
                if (!out) break;
                ListCtx ctx = { out, 0, out_size };
                symbols_foreach(collect_java_symbols, &ctx);
                out[ctx.offset++] = '\0';
                msg_send(client_fd, MSG_RETURN, hdr.id, out, ctx.offset);
                free(out);
                free(buf);
                break;
            }
            default:
                fprintf(stderr, "unknown message type: 0x%02x\n", hdr.type);
                free(buf);
                break;
        }
    }
}
