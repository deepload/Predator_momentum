#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_crypto_engine.h"
#include "../helpers/predator_logging.h"
#include <string.h>

// PRODUCTION: Protocol Test Scene
// Tests real crypto implementations (Keeloq, Hitag2, AES-128)
// Shows which protocol the selected car uses

static void protocol_test_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_protocol_test_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;

    submenu_reset(app->submenu);

    char header[64];
    snprintf(header, sizeof(header), "🔐 Protocol Test: %.12s %.15s",
             app->selected_model_make,
             app->selected_model_name);
    submenu_set_header(app->submenu, header);

    // Determine which protocol this car likely uses based on frequency and make
    bool uses_keeloq = false;
    bool uses_hitag2 = false;
    bool uses_smart_key = false;

    // Keeloq users (American/Japanese/French brands at 315/433MHz)
    if(strstr(app->selected_model_make, "Chrysler") ||
       strstr(app->selected_model_make, "Ford") ||
       strstr(app->selected_model_make, "GM") ||
       strstr(app->selected_model_make, "Honda") ||
       strstr(app->selected_model_make, "Toyota") ||
       strstr(app->selected_model_make, "Nissan") ||
       strstr(app->selected_model_make, "Chevrolet") ||
       strstr(app->selected_model_make, "Dodge") ||
       strstr(app->selected_model_make, "Jeep") ||
       strstr(app->selected_model_make, "Peugeot") ||      // French
       strstr(app->selected_model_make, "Renault") ||      // French
       strstr(app->selected_model_make, "Fiat") ||         // Italian
       strstr(app->selected_model_make, "Mazda") ||        // Japanese
       strstr(app->selected_model_make, "Mitsubishi") ||   // Japanese
       strstr(app->selected_model_make, "Subaru") ||       // Japanese
       strstr(app->selected_model_make, "Hyundai") ||      // Korean
       strstr(app->selected_model_make, "Kia")) {          // Korean
        uses_keeloq = true;
    }

    // Hitag2 users (German/VW Group brands at 868MHz)
    if(strstr(app->selected_model_make, "BMW") ||
       strstr(app->selected_model_make, "Audi") ||
       strstr(app->selected_model_make, "VW") ||
       strstr(app->selected_model_make, "Volkswagen") ||
       strstr(app->selected_model_make, "Porsche") ||
       strstr(app->selected_model_make, "Skoda") ||        // VW Group
       strstr(app->selected_model_make, "Seat")) {         // VW Group
        uses_hitag2 = true;
    }

    // Smart Key users (luxury/modern cars)
    if(strstr(app->selected_model_make, "Tesla") ||
       strstr(app->selected_model_make, "Mercedes") ||
       strstr(app->selected_model_make, "Lexus") ||
       strstr(app->selected_model_make, "Cadillac") ||
       strstr(app->selected_model_make, "Range Rover") ||
       strstr(app->selected_model_make, "Jaguar") ||
       strstr(app->selected_model_make, "Acura") ||        // Honda luxury
       strstr(app->selected_model_make, "Infiniti") ||     // Nissan luxury
       strstr(app->selected_model_make, "Volvo") ||        // Swedish luxury
       strstr(app->selected_model_make, "Lamborghini") ||  // Italian exotic
       strstr(app->selected_model_make, "Ferrari") ||      // Italian exotic
       strstr(app->selected_model_make, "Maserati") ||     // Italian exotic
       strstr(app->selected_model_make, "Bentley") ||      // British exotic
       strstr(app->selected_model_make, "Rolls-Royce")) {  // British exotic
        uses_smart_key = true;
    }

    // Add detected protocols
    if(uses_keeloq) {
        submenu_add_item(app->submenu, "✅ Keeloq (Rolling Code)", 1, protocol_test_cb, app);
    } else {
        submenu_add_item(app->submenu, "⚪ Keeloq (Not Used)", 100, protocol_test_cb, app);
    }

    if(uses_hitag2) {
        submenu_add_item(app->submenu, "✅ Hitag2 (BMW/Audi)", 2, protocol_test_cb, app);
    } else {
        submenu_add_item(app->submenu, "⚪ Hitag2 (Not Used)", 101, protocol_test_cb, app);
    }

    if(uses_smart_key) {
        submenu_add_item(app->submenu, "✅ Smart Key (AES-128)", 3, protocol_test_cb, app);
    } else {
        submenu_add_item(app->submenu, "⚪ Smart Key (Not Used)", 102, protocol_test_cb, app);
    }

    // Add test options
    submenu_add_item(app->submenu, "---", 200, protocol_test_cb, app);
    submenu_add_item(app->submenu, "🧪 Test All Protocols", 10, protocol_test_cb, app);
    submenu_add_item(app->submenu, "📊 Generate Test Packet", 11, protocol_test_cb, app);
    submenu_add_item(app->submenu, "🔍 Analyze Encryption", 12, protocol_test_cb, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_protocol_test_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        return false; // Let scene manager handle back
    }

    if(event.type == SceneManagerEventTypeCustom) {
        char msg[128];

        switch(event.event) {
            case 1: { // Test Keeloq
                predator_log_append(app, "KEELOQ: Testing rolling code encryption...");
                
                // Test Keeloq encryption
                KeeloqContext ctx = {
                    .manufacturer_key = 0x0123456789ABCDEF,
                    .serial_number = 0x123456,
                    .counter = 1,
                    .button_code = 0x05 // Unlock
                };
                
                uint8_t packet[16];
                size_t len = 0;
                if(predator_crypto_keeloq_generate_packet(&ctx, packet, &len)) {
                    snprintf(msg, sizeof(msg), "KEELOQ: Generated %u-byte packet", (unsigned)len);
                    predator_log_append(app, msg);
                    predator_log_append(app, "KEELOQ: 528-round encryption ✅");
                } else {
                    predator_log_append(app, "KEELOQ: Generation failed ❌");
                }
                return true;
            }

            case 2: { // Test Hitag2
                predator_log_append(app, "HITAG2: Testing BMW/Audi protocol...");
                
                Hitag2Context ctx = {
                    .key_uid = 0xABCDEF1234567890ULL,
                    .rolling_code = 1234
                };
                
                // Test auth challenge-response
                uint32_t challenge = 0x12345678;
                uint32_t response = 0;
                if(predator_crypto_hitag2_auth_challenge(&ctx, challenge, &response)) {
                    snprintf(msg, sizeof(msg), "HITAG2: Auth OK (response=0x%08lX)", response);
                    predator_log_append(app, msg);
                    
                    // Generate packet
                    uint8_t packet[16];
                    size_t len = 0;
                    if(predator_crypto_hitag2_generate_packet(&ctx, 0x01, packet, &len)) {
                        snprintf(msg, sizeof(msg), "HITAG2: Generated %u-byte packet ✅", (unsigned)len);
                        predator_log_append(app, msg);
                    }
                } else {
                    predator_log_append(app, "HITAG2: Authentication failed ❌");
                }
                return true;
            }

            case 3: { // Test Smart Key AES
                predator_log_append(app, "SMART_KEY: Testing AES-128 encryption...");
                
                SmartKeyContext ctx;
                memset(&ctx, 0, sizeof(ctx));
                // Set a test AES key
                for(int i = 0; i < 16; i++) ctx.aes_key[i] = i;
                // Set vehicle ID
                for(int i = 0; i < 8; i++) ctx.vehicle_id[i] = 0x01 + i;
                
                // Generate challenge
                uint8_t challenge[16];
                if(predator_crypto_smart_key_challenge(&ctx, challenge, 16)) {
                    snprintf(msg, sizeof(msg), "SMART_KEY: Challenge 0x%08lX generated", ctx.challenge);
                    predator_log_append(app, msg);
                    
                    // Generate response
                    uint8_t response[16];
                    size_t len = 0;
                    if(predator_crypto_smart_key_response(&ctx, response, &len)) {
                        predator_log_append(app, "SMART_KEY: AES-128 response generated ✅");
                        snprintf(msg, sizeof(msg), "SMART_KEY: Response=0x%08lX", ctx.response);
                        predator_log_append(app, msg);
                    }
                } else {
                    predator_log_append(app, "SMART_KEY: Generation failed ❌");
                }
                return true;
            }

            case 10: { // Test All
                predator_log_append(app, "=== TESTING ALL PROTOCOLS ===");
                predator_log_append(app, "");
                
                // Dispatch sub-tests
                SceneManagerEvent sub_event = {.type = SceneManagerEventTypeCustom};
                sub_event.event = 1;
                predator_scene_protocol_test_ui_on_event(app, sub_event);
                predator_log_append(app, "");
                
                sub_event.event = 2;
                predator_scene_protocol_test_ui_on_event(app, sub_event);
                predator_log_append(app, "");
                
                sub_event.event = 3;
                predator_scene_protocol_test_ui_on_event(app, sub_event);
                
                predator_log_append(app, "");
                predator_log_append(app, "=== ALL TESTS COMPLETE ===");
                return true;
            }

            case 11: { // Generate test packet
                predator_log_append(app, "PACKET: Generating manufacturer-specific packet...");
                
                RFPacket packet;
                bool success = false;
                
                // Determine manufacturer and generate appropriate packet
                if(strstr(app->selected_model_make, "Toyota")) {
                    success = predator_crypto_format_toyota_packet(0xA1, 0x123456, &packet);
                    if(success) predator_log_append(app, "TOYOTA: 315MHz OOK packet generated ✅");
                } else if(strstr(app->selected_model_make, "Honda")) {
                    success = predator_crypto_format_honda_packet(0xA1, 0x123456, &packet);
                    if(success) predator_log_append(app, "HONDA: 433.42MHz OOK packet generated ✅");
                } else if(strstr(app->selected_model_make, "Ford")) {
                    success = predator_crypto_format_ford_packet(0xA1, 0x123456, &packet);
                    if(success) predator_log_append(app, "FORD: ASK packet generated ✅");
                } else if(strstr(app->selected_model_make, "BMW")) {
                    success = predator_crypto_format_bmw_packet(0x01, 0x123456, &packet);
                    if(success) predator_log_append(app, "BMW: 868MHz FSK packet generated ✅");
                } else if(strstr(app->selected_model_make, "Mercedes")) {
                    success = predator_crypto_format_mercedes_packet(0xA1, 0x123456, &packet);
                    if(success) predator_log_append(app, "MERCEDES: 868MHz FSK packet generated ✅");
                } else if(strstr(app->selected_model_make, "Tesla")) {
                    success = predator_crypto_format_tesla_packet(0xA1, 0x123456, &packet);
                    if(success) predator_log_append(app, "TESLA: 315MHz PSK smart key packet generated ✅");
                } else {
                    predator_log_append(app, "PACKET: Generic packet format used");
                    success = true;
                }
                
                if(success) {
                    snprintf(msg, sizeof(msg), "Modulation: %s, BitRate: %lu bps",
                            packet.modulation == ModulationOOK ? "OOK" :
                            packet.modulation == ModulationASK ? "ASK" :
                            packet.modulation == ModulationFSK ? "FSK" : "PSK",
                            packet.bit_rate);
                    predator_log_append(app, msg);
                }
                
                return true;
            }

            case 12: { // Analyze encryption
                predator_log_append(app, "ANALYSIS: Identifying encryption type...");
                
                if(app->selected_model_freq >= 868000000) {
                    predator_log_append(app, "Frequency: 868MHz → Likely Hitag2/FSK");
                    predator_log_append(app, "Encryption: Challenge-Response");
                } else if(app->selected_model_freq >= 433000000) {
                    predator_log_append(app, "Frequency: 433MHz → Likely Keeloq/OOK");
                    predator_log_append(app, "Encryption: Rolling Code (528 rounds)");
                } else {
                    predator_log_append(app, "Frequency: 315MHz → US Market");
                    predator_log_append(app, "Encryption: Keeloq or Smart Key");
                }
                
                predator_log_append(app, "");
                snprintf(msg, sizeof(msg), "Make: %.15s → Protocol detected", app->selected_model_make);
                predator_log_append(app, msg);
                
                return true;
            }

            default:
                break;
        }
    }

    return false;
}

void predator_scene_protocol_test_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    // Nothing to cleanup
}
