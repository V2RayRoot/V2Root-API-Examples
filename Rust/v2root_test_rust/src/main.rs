use libloading::{Library, Symbol};
use std::ffi::CString;
use std::os::raw::{c_char, c_int};
use std::thread;
use std::time::Duration;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let lib_path = "./libv2root.so";
    let v2ray_path = "./v2ray";
    let config_file = "./config.json";

    let lib = unsafe { Library::new(lib_path)? };

    type InitV2RayFn = unsafe extern "C" fn(*const c_char, *const c_char) -> c_int;
    type ParseConfigStringFn = unsafe extern "C" fn(*const c_char, c_int, c_int) -> c_int;
    type TestConfigConnectionFn = unsafe extern "C" fn(*const c_char, *mut c_int, c_int, c_int) -> c_int;
    type PingServerFn = unsafe extern "C" fn(*const c_char, c_int) -> c_int;
    type StartV2RayFn = unsafe extern "C" fn(c_int, c_int) -> c_int;
    type StopV2RayFn = unsafe extern "C" fn() -> c_int;
    type ResetNetworkProxyFn = unsafe extern "C" fn() -> c_int;

    let init_v2ray: Symbol<InitV2RayFn> = unsafe { lib.get(b"init_v2ray")? };
    let parse_config_string: Symbol<ParseConfigStringFn> = unsafe { lib.get(b"parse_config_string")? };
    let test_config_connection: Symbol<TestConfigConnectionFn> = unsafe { lib.get(b"test_config_connection")? };
    let ping_server: Symbol<PingServerFn> = unsafe { lib.get(b"ping_server")? };
    let start_v2ray: Symbol<StartV2RayFn> = unsafe { lib.get(b"start_v2ray")? };
    let stop_v2ray: Symbol<StopV2RayFn> = unsafe { lib.get(b"stop_v2ray")? };
    let reset_network_proxy: Symbol<ResetNetworkProxyFn> = unsafe { lib.get(b"reset_network_proxy")? };

    let config_file_c = CString::new(config_file)?.into_raw();
    let v2ray_path_c = CString::new(v2ray_path)?.into_raw();
    let result = unsafe { init_v2ray(config_file_c, v2ray_path_c) };

    unsafe {
        let _ = CString::from_raw(config_file_c);
        let _ = CString::from_raw(v2ray_path_c);
    }

    if result != 0 {
        println!("Failed to initialize V2Ray: {}", result);
        return Err(format!("Failed to initialize V2Ray: {}", result).into());
    }
    println!("V2Ray initialized successfully!");

    let config_str = "vless://uuid@example.com:443?type=tcp&security=tls&sni=example.com";
    let config_str_c = CString::new(config_str)?.into_raw();
    let http_port = 2300;
    let socks_port = 2301;

    let result = unsafe { parse_config_string(config_str_c, http_port, socks_port) };
    if result != 0 {
        println!("Failed to parse config string: {}", result);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Failed to parse config string: {}", result).into());
    }
    println!("Configuration parsed successfully!");

    let mut latency: c_int = 0;
    let result = unsafe { test_config_connection(config_str_c, &mut latency, http_port, socks_port) };
    if result != 0 {
        println!("Connection test failed: {}", result);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Connection test failed: {}", result).into());
    }
    println!("Connection test successful, latency: {} ms", latency);

    let server_address = "example.com";
    let server_port = 8080;
    let server_address_c = CString::new(server_address)?.into_raw();
    let ping_result = unsafe { ping_server(server_address_c, server_port) };

    unsafe {
        let _ = CString::from_raw(server_address_c);
    }

    if ping_result < 0 {
        println!("Ping server failed: {}", ping_result);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Ping server failed: {}", ping_result).into());
    }
    println!("Ping successful, latency: {} ms", ping_result);

    let pid = unsafe { start_v2ray(http_port, socks_port) };
    if pid < 0 {
        println!("Failed to start V2Ray: {}", pid);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Failed to start V2Ray: {}", pid).into());
    }
    println!("V2Ray started with PID: {}", pid);

    println!("Waiting for 20 seconds...");
    thread::sleep(Duration::from_secs(20));

    let result = unsafe { stop_v2ray() };
    if result != 0 {
        println!("Failed to stop V2Ray: {}", result);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Failed to stop V2Ray: {}", result).into());
    }
    println!("V2Ray stopped successfully!");

    let result = unsafe { reset_network_proxy() };
    if result != 0 {
        println!("Failed to reset network proxy: {}", result);
        unsafe { let _ = CString::from_raw(config_str_c); }
        return Err(format!("Failed to reset network proxy: {}", result).into());
    }
    println!("Network proxy reset successfully!");

    unsafe { let _ = CString::from_raw(config_str_c); }
    lib.close()?;
    Ok(())
}