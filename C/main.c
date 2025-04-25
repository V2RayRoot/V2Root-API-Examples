#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        return 1;
    }

    int (*init_v2ray)(const char*, const char*) = dlsym(lib, "init_v2ray");
    int (*parse_config_string)(const char*, int, int) = dlsym(lib, "parse_config_string");
    int (*test_config_connection)(const char*, int*, int, int) = dlsym(lib, "test_config_connection");
    int (*ping_server)(const char*, int) = dlsym(lib, "ping_server");
    int (*start_v2ray)(int, int) = dlsym(lib, "start_v2ray");
    int (*stop_v2ray)() = dlsym(lib, "stop_v2ray");
    int (*reset_network_proxy)() = dlsym(lib, "reset_network_proxy");

    char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "Failed to load functions: %s\n", error);
        dlclose(lib);
        return 1;
    }

    int result = init_v2ray(config_file, v2ray_path);
    if (result != 0) {
        fprintf(stderr, "Failed to initialize V2Ray: %d\n", result);
        dlclose(lib);
        return 1;
    }
    printf("V2Ray initialized successfully!\n");

    result = parse_config_string(config_str, http_port, socks_port);
    if (result != 0) {
        fprintf(stderr, "Failed to parse config string: %d\n", result);
        dlclose(lib);
        return 1;
    }
    printf("Configuration parsed successfully!\n");

    int latency = 0;
    result = test_config_connection(config_str, &latency, http_port, socks_port);
    if (result != 0) {
        fprintf(stderr, "Connection test failed: %d\n", result);
        dlclose(lib);
        return 1;
    }
    printf("Connection test successful, latency: %d ms\n", latency);

    int ping_result = ping_server(server_address, server_port);
    if (ping_result < 0) {
        fprintf(stderr, "Ping server failed: %d\n", ping_result);
        dlclose(lib);
        return 1;
    }
    printf("Ping successful, latency: %d ms\n", ping_result);

    int pid = start_v2ray(http_port, socks_port);
    if (pid < 0) {
        fprintf(stderr, "Failed to start V2Ray: %d\n", pid);
        dlclose(lib);
        return 1;
    }
    printf("V2Ray started with PID: %d\n", pid);

    printf("Waiting for 20 seconds...\n");
    sleep(20);

    result = stop_v2ray();
    if (result != 0) {
        fprintf(stderr, "Failed to stop V2Ray: %d\n", result);
        dlclose(lib);
        return 1;
    }
    printf("V2Ray stopped successfully!\n");

    result = reset_network_proxy();
    if (result != 0) {
        fprintf(stderr, "Failed to reset network proxy: %d\n", result);
        dlclose(lib);
        return 1;
    }
    printf("Network proxy reset successfully!\n");

    dlclose(lib);
    return 0;
}