#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: launcher <lib1.so> [lib2.so ...]\n");
        return 1;
    }

    void *handles[argc - 1];

    for (int i = 1; i < argc; i++) {
        handles[i - 1] = dlopen(argv[i], RTLD_LAZY | RTLD_GLOBAL);
        if (!handles[i - 1]) {
            fprintf(stderr, "dlopen failed for %s: %s\n", argv[i], dlerror());
            return 1;
        }
        fprintf(stdout, "loaded: %s\n", argv[i]);
    }

    for (int i = 1; i < argc; i++) {
        void *jni_onload = dlsym(handles[i - 1], "JNI_OnLoad");
        if (jni_onload) {
            fprintf(stdout, "JNI_OnLoad found in %s: %p\n", argv[i], jni_onload);
        }
    }

    pause();
    return 0;
}
