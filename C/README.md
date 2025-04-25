# C Example for libv2root

This directory contains a C implementation demonstrating how to use the `libv2root.so` library to interact with V2Ray. The code initializes V2Ray, parses a VLESS configuration, tests connection latency, pings a server, starts/stops the V2Ray service, and resets network proxies.

## Prerequisites
- **Files**: Ensure `libv2root.so` (Linux/WSL) or `libv2root.dll` (Windows), `v2ray`, and `config.json` are in the project root (`../`).
- **Dependencies** (Linux/WSL):
  ```bash
  sudo apt install libjansson4 libssl-dev
  ```
- **Compiler**: GCC or compatible (e.g., MinGW for Windows).
  ```bash
  gcc --version
  ```

## Build
Compile the code:
```bash
gcc -o v2root_test_c main.c -ldl
```

For Windows, use `libv2root.dll` instead of `libv2root.so` and adjust the linker (e.g., with MinGW):
```bash
gcc -o v2root_test_c main.c -L. -lv2root
```

## Run
Run the executable:
```bash
LD_LIBRARY_PATH=../ ./v2root_test_c
```

For Windows, ensure `libv2root.dll` is in the same directory or system PATH:
```bash
./v2root_test_c
```

## Notes
- The code dynamically loads `libv2root.so` using `dlopen`/`dlsym`.
- If you encounter a "cannot open shared object file" error, verify `LD_LIBRARY_PATH` or PATH settings.
- Check `../v2root.log` for debugging if errors occur.