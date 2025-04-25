# Rust Example for libv2root

This directory contains a Rust implementation demonstrating how to use the `libv2root.so` library to interact with V2Ray. The code initializes V2Ray, parses a VLESS configuration, tests connection latency, pings a server, starts/stops the V2Ray service, and resets network proxies using the `libloading` crate for safe FFI.

## Prerequisites
- **Files**: Ensure `libv2root.so` (Linux/WSL) or `libv2root.dll` (Windows), `v2ray`, and `config.json` are in the project root (`../../`).
- **Dependencies** (Linux/WSL):
  ```bash
  sudo apt install libjansson4 libssl-dev
  ```
- **Rust**: Install via `rustup`.
  ```bash
  rustc --version
  ```

## Build
Compile the code:
```bash
cd v2root_test_rust
cargo build
```

For Windows, replace `libv2root.so` with `libv2root.dll` in `main.rs` and ensure it’s in the PATH or project root.

## Run
Run the executable:
```bash
LD_LIBRARY_PATH=../../ cargo run
```

For Windows, ensure `libv2root.dll` is in the same directory or system PATH:
```bash
cargo run
```

## Notes
- The code uses the `libloading` crate for dynamic loading of `libv2root.so`.
- If you encounter a "cannot open shared object file" error, verify `LD_LIBRARY_PATH` or PATH settings.
- Check `../../v2root.log` for debugging if errors occur.
- The `Cargo.toml` file specifies dependencies.