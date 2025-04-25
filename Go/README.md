# Go Example for libv2root

This directory contains a Go implementation demonstrating how to use the `libv2root.so` library to interact with V2Ray. The code initializes V2Ray, parses a VLESS configuration, tests connection latency, pings a server, starts/stops the V2Ray service, and resets network proxies using `cgo`.

## Prerequisites
- **Files**: Ensure `libv2root.so` (Linux/WSL) or `libv2root.dll` (Windows), `v2ray`, and `config.json` are in the project root (`../`).
- **Dependencies** (Linux/WSL):
  ```bash
  sudo apt install libjansson4 libssl-dev
  ```
- **Go**: Version 1.18 or higher.
  ```bash
  go version
  ```

## Build
Compile the code:
```bash
go build -o v2root_test_go
```

For Windows, ensure `libv2root.dll` is used in the code and available in the PATH or project root.

## Run
Run the executable:
```bash
LD_LIBRARY_PATH=../ ./v2root_test_go
```

For Windows, ensure `libv2root.dll` is in the same directory or system PATH:
```bash
./v2root_test_go
```

## Notes
- The code uses `cgo` to interface with `libv2root.so`.
- If you encounter a "cannot find -lv2root" error, verify `LD_LIBRARY_PATH` or PATH settings.
- Check `../v2root.log` for debugging if errors occur.
- The `go.mod` file is included for module management.