#pragma once
#include <stdint.h>

extern int      g_socket_fd;
extern uint32_t g_current_call_id;

// Message types
#define MSG_CALL          0x01  // (Java) Launcher: call JNI symbol
#define MSG_RETURN        0x02  // (Launcher) Java: return value
#define MSG_JNIENV        0x03  // (Launcher) Java: JNIEnv call needed
#define MSG_JNIENV_RETURN 0x04  // (Java) Launcher: JNIEnv result
#define MSG_ERROR         0x05  // either direction

// Fixed header for every message
typedef struct {
    uint8_t  type;
    uint32_t id; // request id, for matching responses
    uint32_t data_len; // bytes of payload following this header
} __attribute__((packed)) MsgHeader;

// IO helpers
int msg_send(int fd, uint8_t type, uint32_t id, const void *data, uint32_t len);
int msg_recv(int fd, MsgHeader *hdr, void *buf, uint32_t buf_size);

// Main IPC loop
void ipc_serve(int client_fd);