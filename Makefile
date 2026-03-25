# $NDK_CLANG is expected to be passed like this:
# make NDK_CLANG=$NDK_CLANG LIBFFI_AARCH64=$LIBFFI_AARCH64
CC      = $(NDK_CLANG)/aarch64-linux-android29-clang
SRCS    = src/main.c src/ipc.c src/symbols.c src/jnienv.c
OUT     = launcher
LIBFFI  = $(LIBFFI_AARCH64)

$(OUT): $(SRCS)
	$(CC) -I src/include -I $(LIBFFI)/include -o $(OUT) $(SRCS) -ldl $(LIBFFI)/lib/libffi.a

clean:
	rm -f $(OUT)