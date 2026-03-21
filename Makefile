# $NDK_CLANG is expected to passed like this:
# make NDK_CLANG=$NDK_CLANG
CC   = $(NDK_CLANG)/aarch64-linux-android29-clang
SRCS = src/main.c src/ipc.c src/symbols.c src/jnienv.c
OUT  = launcher

$(OUT): $(SRCS)
	$(CC) -I src/include -o $(OUT) $(SRCS) -ldl

clean:
	rm -f $(OUT)