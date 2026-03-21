# Ander Bionic

A collection of required libraries from AOSP and Android NDK for running ARM Bionic natively on x86_64 Linux. This implementation requires `qemu-aarch64` program from the `qemu-user` program for doing CPU translations from ARM64 to x86_64.

## Launcher

The `launcher` binary in the root of this project is supposed to be used for loading the `.so` files that are compiled against bionic like this:

```bash
$ qemu-aarch64 -L ./sysroot ./launcher /path/to/your.so
```
