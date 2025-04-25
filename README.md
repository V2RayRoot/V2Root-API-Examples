# V2Root-API-Examples

This repository provides a collection of example codes in multiple programming languages (C, C++, Go, Rust) demonstrating how to interact with the `libv2root` library to manage V2Ray services. The examples cover key functionalities such as initializing V2Ray, parsing configurations, testing connection latency, pinging servers, starting/stopping the V2Ray service, and resetting network proxies. This project is ideal for developers looking to integrate V2Ray into their applications using a C-based API across different programming environments.

## Features
- **Multi-language Support**: Implementations in C, C++, Go, and Rust, showcasing cross-language compatibility with `libv2root`.
- **Comprehensive V2Ray Operations**: Initialize V2Ray, parse VLESS configurations, test connections, ping servers, and manage service lifecycle.
- **Sample Configuration**: Includes a `config.json` file with a VLESS setup for quick testing.
- **Cross-platform**: Tested on Linux/WSL, with notes for Windows users (use `libv2root.dll` instead of `libv2root.so`).

## Repository Structure
- `C/`: C implementation with dynamic loading of `libv2root.so`.
- `Cpp/`: C++ implementation with enhanced error handling and modern C++ features.
- `Go/`: Go implementation using `cgo` for seamless integration with the C API.
- `Rust/`: Rust implementation leveraging the `libloading` crate for safe FFI.
- `config.json`: Sample VLESS configuration file for testing.
- `.gitignore`: Excludes binary files (`libv2root.so`, `v2ray`, etc.) from version control.
- `README.md`: This file, providing an overview and setup instructions.

Each language folder contains a `README.md` with specific build and run instructions.

## Getting Started
To use these examples, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/V2RayRoot/V2Root-API-Examples.git
   cd V2Root-API-Examples
   ```

2. **Prepare Dependencies**:
   - **Linux/WSL**:
     Install required libraries:
     ```bash
     sudo apt install libjansson4 libssl-dev
     ```
   - **Windows**:
     Use `libv2root.dll` instead of `libv2root.so`. Ensure `libjansson` and `libssl` are available (e.g., via MSYS2 or pre-built binaries).

3. **Place Required Files**:
   - Copy `libv2root.so` (Linux) or `libv2root.dll` (Windows) and `v2ray` executable to the project root.
   - The provided `config.json` is pre-configured for a VLESS setup. Modify it if needed.

4. **Build and Run**:
   Navigate to the desired language folder (e.g., `C/`, `Go/`) and follow the instructions in the respective `README.md` to compile and execute the example.

## Platform Notes
- **Linux/WSL**:
  - Ensure `libv2root.so` is in the project root or set `LD_LIBRARY_PATH`:
    ```bash
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
    ```
  - Tested on Ubuntu-based WSL environments.

- **Windows**:
  - Replace `libv2root.so` with `libv2root.dll` in the code and project directory.
  - Ensure `libv2root.dll` is in the same directory as the executable or in the system PATH.
  - Use a compatible build environment (e.g., MSYS2, MinGW, or Visual Studio).

## Example Usage
Each example performs the following steps:
1. Initializes V2Ray with `init_v2ray`.
2. Parses a VLESS configuration using `parse_config_string`.
3. Tests connection latency with `test_config_connection`.
4. Pings the server with `ping_server`.
5. Starts V2Ray service on ports 2300 (HTTP) and 2301 (SOCKS) using `start_v2ray`.
6. Waits for 20 seconds.
7. Stops the service with `stop_v2ray`.
8. Resets network proxies with `reset_network_proxy`.

See language-specific `README.md` files for detailed build/run commands.

## Contact
For questions or feedback, open an issue on GitHub or contact the repository maintainers.