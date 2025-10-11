#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// ZERO-DAY EXPLOIT DATABASE - ULTIMATE FLIPPER ZERO
// Latest exploits from 2024/2025: Pwn2Own, Black Hat, DEF CON, DarkWeb

// 2024/2025 Zero-Day Categories
typedef enum {
    // Automotive Zero-Days
    ZeroDay_Tesla_TPMS_RCE,           // Pwn2Own 2024/2025 - Buffer overflow
    ZeroDay_Rolling_Code_2024,         // DarkWeb firmware leak - Single capture
    ZeroDay_CAN_Bus_Injection,         // Unauthenticated message injection
    ZeroDay_Keyless_Entry_Relay,       // Extended range relay (unlimited)
    ZeroDay_Immobilizer_Bypass,        // Cryptographic weakness
    ZeroDay_Smart_Key_Clone,           // BLE/UWB protocol flaw
    
    // Wireless Zero-Days
    ZeroDay_BLE_5_Auth_Bypass,         // BLE 5.0-5.3 authentication bypass
    ZeroDay_WiFi_WPA3_Dragonblood,     // Dragonblood attack variants
    ZeroDay_Zigbee_Install_Code,       // Install code bruteforce
    ZeroDay_LoRaWAN_Replay,            // LoRaWAN replay without counter
    ZeroDay_Thread_Commissioning,      // Thread network join bypass
    ZeroDay_Matter_Pairing,            // Matter protocol vulnerability
    
    // RFID/NFC Zero-Days
    ZeroDay_MIFARE_DESFire_Keys,       // DESFire master key recovery
    ZeroDay_HID_iCLASS_Elite,          // iCLASS Elite downgrade
    ZeroDay_EMV_Relay_Distance,        // EMV relay unlimited distance
    ZeroDay_NTAG_Authentication,       // NTAG password bypass
    
    // Infrastructure Zero-Days
    ZeroDay_Traffic_Light_Timing,      // Traffic signal manipulation
    ZeroDay_SCADA_DNP3,                // DNP3 unauthenticated commands
    ZeroDay_Modbus_TCP,                // Modbus TCP no auth
    ZeroDay_BACnet_Exploit,            // Building automation compromise
    
    // IoT Zero-Days
    ZeroDay_Camera_Backdoor,           // IP camera default credentials DB
    ZeroDay_Smart_Lock_Bypass,         // Smart lock protocol flaws
    ZeroDay_Garage_Door_Replay,        // Fixed code garage doors
    ZeroDay_Alarm_System_Jam,          // Alarm RF jamming
} ZeroDayExploit;

// Exploit Target Types
typedef enum {
    ExploitTarget_Automotive,
    ExploitTarget_SmartHome,
    ExploitTarget_Industrial,
    ExploitTarget_Infrastructure,
    ExploitTarget_Mobile,
    ExploitTarget_IoT,
} ExploitTargetType;

// Exploit Severity
typedef enum {
    Severity_Critical,      // Remote code execution, full compromise
    Severity_High,          // Authentication bypass, privilege escalation
    Severity_Medium,        // Information disclosure, DoS
    Severity_Low,           // Minor security issue
} ExploitSeverity;

// Exploit Information
typedef struct {
    ZeroDayExploit exploit_id;
    const char* name;
    const char* cve_id;           // CVE number if disclosed
    ExploitTargetType target;
    ExploitSeverity severity;
    uint16_t year_discovered;
    bool publicly_disclosed;
    const char* source;           // Pwn2Own, Black Hat, DarkWeb, etc.
    const char* description;
} ZeroDayInfo;

// Initialization
bool predator_zero_days_init(PredatorApp* app);
void predator_zero_days_deinit(PredatorApp* app);

// Database Access
const ZeroDayInfo* predator_zero_days_get_info(ZeroDayExploit exploit);
uint32_t predator_zero_days_count(void);
void predator_zero_days_list_all(PredatorApp* app);
void predator_zero_days_list_by_target(PredatorApp* app, ExploitTargetType target);

// Exploit Execution
bool predator_zero_days_execute(PredatorApp* app, ZeroDayExploit exploit);
bool predator_zero_days_test_vulnerability(PredatorApp* app, ZeroDayExploit exploit);

// AUTOMOTIVE ZERO-DAYS
bool predator_zero_day_tesla_tpms_rce(PredatorApp* app);
bool predator_zero_day_rolling_code_2024(PredatorApp* app, uint32_t manufacturer_id);
bool predator_zero_day_can_injection(PredatorApp* app, uint32_t can_id, const uint8_t* data, size_t len);
bool predator_zero_day_keyless_relay(PredatorApp* app, float distance_meters);
bool predator_zero_day_immobilizer(PredatorApp* app, uint8_t immobilizer_type);
bool predator_zero_day_smart_key_clone(PredatorApp* app);

// WIRELESS ZERO-DAYS
bool predator_zero_day_ble5_bypass(PredatorApp* app);
bool predator_zero_day_wpa3_dragonblood(PredatorApp* app);
bool predator_zero_day_zigbee_install_code(PredatorApp* app);
bool predator_zero_day_lorawan_replay(PredatorApp* app);
bool predator_zero_day_thread_join(PredatorApp* app);
bool predator_zero_day_matter_pairing(PredatorApp* app);

// RFID/NFC ZERO-DAYS
bool predator_zero_day_desfire_keys(PredatorApp* app);
bool predator_zero_day_hid_iclass_elite(PredatorApp* app);
bool predator_zero_day_emv_relay(PredatorApp* app);
bool predator_zero_day_ntag_bypass(PredatorApp* app);

// INFRASTRUCTURE ZERO-DAYS
bool predator_zero_day_traffic_light(PredatorApp* app);
bool predator_zero_day_scada_dnp3(PredatorApp* app);
bool predator_zero_day_modbus_tcp(PredatorApp* app);
bool predator_zero_day_bacnet(PredatorApp* app);

// IOT ZERO-DAYS
bool predator_zero_day_camera_backdoor(PredatorApp* app);
bool predator_zero_day_smart_lock(PredatorApp* app);
bool predator_zero_day_garage_door(PredatorApp* app);
bool predator_zero_day_alarm_system(PredatorApp* app);

// Advanced Exploitation
bool predator_zero_day_heap_spray(PredatorApp* app, const uint8_t* shellcode, size_t len);
bool predator_zero_day_rop_chain(PredatorApp* app);
bool predator_zero_day_return_oriented_programming(PredatorApp* app);
bool predator_zero_day_buffer_overflow(PredatorApp* app, size_t overflow_size);

// Exploit Development
bool predator_zero_day_fuzzing_engine(PredatorApp* app);
bool predator_zero_day_symbolic_execution(PredatorApp* app);
bool predator_zero_day_taint_analysis(PredatorApp* app);

// Underground Research
bool predator_zero_day_darkweb_databcase(PredatorApp* app);
bool predator_zero_day_exploit_marketplace(PredatorApp* app);
bool predator_zero_day_vulnerability_research(PredatorApp* app);
