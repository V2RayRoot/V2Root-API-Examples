#include <dlfcn.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main() {
    const char* lib_path = "./libv2root.so";
    const char* v2ray_path = "./v2ray";
    const char* config_file = "./config.json";
    const char* config_str = "vless://uuid@example.com:443?type=tcp&security=tls&sni=example.com";
    const char* server_address = "example.com";
    int http_port = 2300;
    int socks_port = 2301;
    int server_port = 8080;

    void* lib = dlopen(lib_path, RTLD_LAZY);
    if (!lib) {
        std::cerr << "Failed to load library: " << dlerror() << std::endl;
        return 1;
    }

    typedef int (*InitV2RayFn)(const char*, const char*);
    typedef int (*ParseConfigStringFn)(const char*, int, int);
    typedef int (*TestConfigConnectionFn)(const char*, int*, int, int);
    typedef int (*PingServerFn)(const char*, int);
    typedef int (*StartV2RayFn)(int, int);
    typedef int (*StopV2RayFn)();
    typedef int (*ResetNetworkProxyFn)();

    InitV2RayFn init_v2ray = (InitV2RayFn)dlsym(lib, "init_v2ray");
    ParseConfigStringFn parse_config_string = (ParseConfigStringFn)dlsym(lib, "parse_config_string");
    TestConfigConnectionFn test_config_connection = (TestConfigConnectionFn)dlsym(lib, "test_config_connection");
    PingServerFn ping_server = (PingServerFn)dlsym(lib, "ping_server");
    StartV2RayFn start_v2ray = (StartV2RayFn)dlsym(lib, "start_v2ray");
    StopV2RayFn stop_v2ray = (StopV2RayFn)dlsym(lib, "stop_v2ray");
    ResetNetworkProxyFn reset_network_proxy = (ResetNetworkProxyFn)dlsym(lib, "reset_network_proxy");

    char* error = dlerror();
    if (error != nullptr) {
        std::cerr << "Failed to load functions: " << error << std::endl;
        dlclose(lib);
        return 1;
    }

    int result = init_v2ray(config_file, v2ray_path);
    if (result != 0) {
        std::cerr << "Failed to initialize V2Ray: " << result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "V2Ray initialized successfully!" << std::endl;

    result = parse_config_string(config_str, http_port, socks_port);
    if (result != 0) {
        std::cerr << "Failed to parse config string: " << result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "Configuration parsed successfully!" << std::endl;

    int latency = 0;
    result = test_config_connection(config_str, &latency, http_port, socks_port);
    if (result != 0) {
        std::cerr << "Connection test failed: " << result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "Connection test successful, latency: " << latency << " ms" << std::endl;

    int ping_result = ping_server(server_address, server_port);
    if (ping_result < 0) {
        std::cerr << "Ping server failed: " << ping_result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "Ping successful, latency: " << ping_result << " ms" << std::endl;

    int pid = start_v2ray(http_port, socks_port);
    if (pid < 0) {
        std::cerr << "Failed to start V2Ray: " << pid << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "V2Ray started with PID: " << pid << std::endl;

    std::cout << "Waiting for 20 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(20));

    result = stop_v2ray();
    if (result != 0) {
        std::cerr << "Failed to stop V2Ray: " << result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "V2Ray stopped successfully!" << std::endl;

    result = reset_network_proxy();
    if (result != 0) {
        std::cerr << "Failed to reset network proxy: " << result << std::endl;
        dlclose(lib);
        return 1;
    }
    std::cout << "Network proxy reset successfully!" << std::endl;

    dlclose(lib);
    return 0;
}