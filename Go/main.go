package main

import "C"
import (
	"fmt"
	"time"
	"unsafe"
)

func main() {
	configFile := "./config.json"
	v2rayPath := "./v2ray"
	configStr := "vless://uuid@example.com:443?type=tcp&security=tls&sni=example.com"
	httpPort := C.int(2300)
	socksPort := C.int(2301)
	serverAddress := "example.com"
	serverPort := C.int(8080)

	configFileC := C.CString(configFile)
	v2rayPathC := C.CString(v2rayPath)
	result := C.init_v2ray(configFileC, v2rayPathC)
	C.free(unsafe.Pointer(configFileC))
	C.free(unsafe.Pointer(v2rayPathC))

	if result != 0 {
		fmt.Printf("Failed to initialize V2Ray: %d\n", result)
		return
	}
	fmt.Println("V2Ray initialized successfully!")

	configStrC := C.CString(configStr)
	result = C.parse_config_string(configStrC, httpPort, socksPort)
	if result != 0 {
		fmt.Printf("Failed to parse config string: %d\n", result)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Println("Configuration parsed successfully!")

	var latency C.int
	result = C.test_config_connection(configStrC, &latency, httpPort, socksPort)
	if result != 0 {
		fmt.Printf("Connection test failed: %d\n", result)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Printf("Connection test successful, latency: %d ms\n", latency)

	serverAddressC := C.CString(serverAddress)
	pingResult := C.ping_server(serverAddressC, serverPort)
	C.free(unsafe.Pointer(serverAddressC))
	if pingResult < 0 {
		fmt.Printf("Ping server failed: %d\n", pingResult)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Printf("Ping successful, latency: %d ms\n", pingResult)

	pid := C.start_v2ray(httpPort, socksPort)
	if pid < 0 {
		fmt.Printf("Failed to start V2Ray: %d\n", pid)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Printf("V2Ray started with PID: %d\n", pid)

	fmt.Println("Waiting for 20 seconds...")
	time.Sleep(20 * time.Second)

	result = C.stop_v2ray()
	if result != 0 {
		fmt.Printf("Failed to stop V2Ray: %d\n", result)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Println("V2Ray stopped successfully!")

	result = C.reset_network_proxy()
	if result != 0 {
		fmt.Printf("Failed to reset network proxy: %d\n", result)
		C.free(unsafe.Pointer(configStrC))
		return
	}
	fmt.Println("Network proxy reset successfully!")

	C.free(unsafe.Pointer(configStrC))
}