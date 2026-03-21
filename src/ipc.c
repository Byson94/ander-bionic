#include "include/ipc.h"
#include "include/symbols.h"
#include "include/jnienv.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int g_socket_fd = -1;
uint32_t g_current_call_id = 0;

#define BUF_SIZE 65536

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

int msg_recv(int fd, MsgHeader *hdr, void *buf, uint32_t buf_size) {
    if (read_all(fd, hdr, sizeof(MsgHeader)) < 0) return -1;
    if (hdr->data_len > buf_size) return -1;
    if (hdr->data_len > 0 && read_all(fd, buf, hdr->data_len) < 0) return -1;
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
    uint8_t *buf = malloc(BUF_SIZE);
    if (!buf) return;

    fprintf(stdout, "ipc_serve: starting loop\n");
    fflush(stdout);

    while (1) {
        fprintf(stdout, "ipc_serve: waiting for message\n");
        fflush(stdout);

        if (msg_recv(client_fd, &hdr, buf, BUF_SIZE) < 0) {
            fprintf(stderr, "client disconnected\n");
            break;
        }

        switch (hdr.type) {
            case MSG_CALL: {
                char *symbol = (char *)buf;
                uint32_t sym_len = strlen(symbol) + 1;

                // Args start after symbol name, each arg is 8 bytes
                uint64_t *args = (uint64_t *)(buf + sym_len);
                uint32_t argc  = (hdr.data_len - sym_len) / 8;

                fprintf(stderr, "CALL: %s argc=%u\n", symbol, argc);
                fflush(stderr);

                void *sym = symbols_find(symbol);
                if (!sym) {
                    const char *err = "symbol not found";
                    msg_send(client_fd, MSG_ERROR, hdr.id, err, strlen(err) + 1);
                    break;
                }

                g_current_call_id = hdr.id;

                typedef long long (*jni_fn_t)(JNIEnv*, jobject, ...);
                jni_fn_t fn = (jni_fn_t)sym;

                long long result;
                switch (argc) {
                    case 0: result = fn(jnienv_get(), NULL); break;
                    case 1: result = fn(jnienv_get(), NULL, args[0]); break;
                    case 2: result = fn(jnienv_get(), NULL, args[0], args[1]); break;
                    case 3: result = fn(jnienv_get(), NULL, args[0], args[1], args[2]); break;
                    case 4: result = fn(jnienv_get(), NULL, args[0], args[1], args[2], args[3]); break;
                    case 5: result = fn(jnienv_get(), NULL, args[0], args[1], args[2], args[3], args[4]); break;
                    case 6: result = fn(jnienv_get(), NULL, args[0], args[1], args[2], args[3], args[4], args[5]); break;
                    case 7: result = fn(jnienv_get(), NULL, args[0], args[1], args[2], args[3], args[4], args[5], args[6]); break;
                    case 8: result = fn(jnienv_get(), NULL, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]); break;
                    default:
                        fprintf(stderr, "too many args: %u\n", argc);
                        result = 0;
                }

                uint64_t ret = (uint64_t)result;
                msg_send(client_fd, MSG_RETURN, hdr.id, &ret, sizeof(ret));
                break;
            }
            case MSG_LIST_SYMBOLS: {
                char *out = malloc(BUF_SIZE);
                if (!out) break;
                ListCtx ctx = { out, 0, BUF_SIZE };
                symbols_foreach(collect_java_symbols, &ctx);
                out[ctx.offset++] = '\0';
                msg_send(client_fd, MSG_RETURN, hdr.id, out, ctx.offset);
                free(out);
                break;
            }
            default:
                fprintf(stderr, "unknown message type: 0x%02x\n", hdr.type);
                break;
        }
    }

    free(buf);
}
