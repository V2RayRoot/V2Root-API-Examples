# C++ Example for libv2root

This directory contains a C++ implementation demonstrating how to use the `libv2root.so` library to interact with V2Ray. The code initializes V2Ray, parses a VLESS configuration, tests connection latency, pings a server, starts/stops the V2Ray service, and resets network proxies using modern C++ features.

## Prerequisites
- **Files**: Ensure `libv2root.so` (Linux/WSL) or `libv2root.dll` (Windows), `v2ray`, and `config.json` are in the project root (`../`).
- **Dependencies** (Linux/WSL):
  ```bash
  sudo apt install libjansson4 libssl-dev
  ```
- **Compiler**: G++ or compatible (e.g., MSVC or MinGW for Windows).
  ```bash
  g++ --version
  ```

## Build
Compile the code:
```bash
g++ -o v2root_test_cpp main.cpp -ldl
```

For Windows, use `libv2root.dll` and adjust the linker (e.g., with MinGW):
```bash
g++ -o v2root_test_cpp main.cpp -L. -lv2root
```

## Run
Run the executable:
```bash
LD_LIBRARY_PATH=../ ./v2root_test_cpp
```

For Windows, ensure `libv2root.dll` is in the same directory or system PATH:
```bash
./v2root_test_cpp
```

## Notes
- The code uses `dlopen`/`dlsym` for dynamic loading of `libv2root.so`.
- If you encounter a "cannot open shared object file" error, verify `LD_LIBRARY_PATH` or PATH settings.
- Check `../v2root.log` for debugging if errors occur.