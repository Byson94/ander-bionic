#include "include/symbols.h"
#include "include/ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <dlfcn.h>

#define SOCKET_PATH "/tmp/ander-launcher.sock"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: launcher <lib1.so> [lib2.so ...]\n");
        return 1;
    }

    symbols_init();
    for (int i = 1; i < argc; i++) {
        if (symbols_load(argv[i]) < 0) return 1;
    }

    // Check for JNI_OnLoad
    for (int i = 1; i < argc; i++) {
        void *jni_onload = symbols_find("JNI_OnLoad");
        if (jni_onload)
            fprintf(stdout, "JNI_OnLoad found: %p\n", jni_onload);
    }

    // Set up socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    unlink(SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(server_fd, 1) < 0) { perror("listen"); return 1; }

    fprintf(stdout, "listening on %s\n", SOCKET_PATH);
    fflush(stdout);

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) { perror("accept"); return 1; }

    fprintf(stdout, "client connected\n");
    fflush(stdout);

    g_socket_fd = client_fd;
    ipc_serve(client_fd);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}