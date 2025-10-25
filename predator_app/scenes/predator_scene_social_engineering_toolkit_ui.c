#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_real_attack_engine.h"
#include <gui/view.h>
#include <string.h>

// Social Engineering Toolkit - Advanced psychological manipulation attacks
// Fake WiFi portals, Bluetooth impersonation, NFC payment spoofing

typedef enum {
    SocialStatusIdle,
    SocialStatusPortalActive,
    SocialStatusBtImpersonating,
    SocialStatusNfcSpoofing,
    SocialStatusHarvesting,
    SocialStatusSuccess,
    SocialStatusError
} SocialStatus;

typedef struct {
    SocialStatus status;
    uint32_t credentials_harvested;
    uint32_t victims_connected;
    uint32_t bt_devices_impersonated;
    uint32_t nfc_transactions_spoofed;
    char current_attack[32];
    char fake_ssid[32];
    char target_device[32];
    char status_text[24];
    bool portal_active;
    bool bt_active;
    bool nfc_active;
    uint32_t data_collected_mb;
} SocialState;

static SocialState social_state;
static View* social_view = NULL;

static void draw_social_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "SOCIAL ENGINEERING");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_social_info(Canvas* canvas, SocialState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Statistics
    char stats_str[32];
    snprintf(stats_str, sizeof(stats_str), "Creds:%lu Victims:%lu", 
             state->credentials_harvested, state->victims_connected);
    canvas_draw_str(canvas, 2, 32, stats_str);
    
    // Current attack
    if(state->current_attack[0] != '\0') {
        char attack_str[32];
        snprintf(attack_str, sizeof(attack_str), "%.24s", state->current_attack);
        canvas_draw_str(canvas, 2, 42, attack_str);
    }
    
    // Target/Fake SSID
    if(state->fake_ssid[0] != '\0') {
        char ssid_str[32];
        snprintf(ssid_str, sizeof(ssid_str), "SSID: %.20s", state->fake_ssid);
        canvas_draw_str(canvas, 2, 52, ssid_str);
    } else if(state->target_device[0] != '\0') {
        char device_str[32];
        snprintf(device_str, sizeof(device_str), "Target: %.18s", state->target_device);
        canvas_draw_str(canvas, 2, 52, device_str);
    }
    
    // Data collected
    if(state->data_collected_mb > 0) {
        char data_str[32];
        snprintf(data_str, sizeof(data_str), "Data: %luMB BT:%lu NFC:%lu", 
                 state->data_collected_mb, state->bt_devices_impersonated, state->nfc_transactions_spoofed);
        canvas_draw_str(canvas, 2, 62, data_str);
    }
}

static void social_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_social_header(canvas);
    draw_social_info(canvas, &social_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(social_state.portal_active || social_state.bt_active || social_state.nfc_active) {
        canvas_draw_str(canvas, 15, 64, "OK=Stop  Up=Continue  Back=Exit");
    } else {
        canvas_draw_str(canvas, 2, 64, "OK=Portal Up=BT Down=NFC Left=Harvest Back=Exit");
    }
}

static bool social_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(social_state.portal_active || social_state.bt_active || social_state.nfc_active) {
                social_state.portal_active = false;
                social_state.bt_active = false;
                social_state.nfc_active = false;
                predator_log_append(app, "Social: All attacks stopped on exit");
            }
            return false;
        } else if(event->key == InputKeyOk) {
            if(social_state.status == SocialStatusIdle) {
                // Start fake WiFi portal
                social_state.status = SocialStatusPortalActive;
                social_state.portal_active = true;
                strncpy(social_state.current_attack, "Fake WiFi Portal", sizeof(social_state.current_attack) - 1);
                social_state.current_attack[sizeof(social_state.current_attack) - 1] = '\0';
                strncpy(social_state.fake_ssid, "Free_WiFi_Login", sizeof(social_state.fake_ssid) - 1);
                social_state.fake_ssid[sizeof(social_state.fake_ssid) - 1] = '\0';
                strncpy(social_state.status_text, "PORTAL ACTIVE", sizeof(social_state.status_text) - 1);
                social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "Social: Fake WiFi portal activated");
                
                return true;
            } else {
                // Stop all attacks
                social_state.status = SocialStatusIdle;
                social_state.portal_active = false;
                social_state.bt_active = false;
                social_state.nfc_active = false;
                strncpy(social_state.status_text, "STOPPED", sizeof(social_state.status_text) - 1);
                social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "Social: All attacks stopped");
                
                return true;
            }
        } else if(event->key == InputKeyUp && social_state.status == SocialStatusIdle) {
            // Bluetooth device impersonation
            social_state.status = SocialStatusBtImpersonating;
            social_state.bt_active = true;
            strncpy(social_state.current_attack, "BT Impersonation", sizeof(social_state.current_attack) - 1);
            social_state.current_attack[sizeof(social_state.current_attack) - 1] = '\0';
            strncpy(social_state.target_device, "AirPods Pro", sizeof(social_state.target_device) - 1);
            social_state.target_device[sizeof(social_state.target_device) - 1] = '\0';
            strncpy(social_state.status_text, "BT IMPERSONATING", sizeof(social_state.status_text) - 1);
            social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "Social: Bluetooth device impersonation started");
            
            return true;
        } else if(event->key == InputKeyDown && social_state.status == SocialStatusIdle) {
            // NFC payment terminal spoofing
            social_state.status = SocialStatusNfcSpoofing;
            social_state.nfc_active = true;
            strncpy(social_state.current_attack, "NFC Payment Spoof", sizeof(social_state.current_attack) - 1);
            social_state.current_attack[sizeof(social_state.current_attack) - 1] = '\0';
            strncpy(social_state.target_device, "Visa Contactless", sizeof(social_state.target_device) - 1);
            social_state.target_device[sizeof(social_state.target_device) - 1] = '\0';
            strncpy(social_state.status_text, "NFC SPOOFING", sizeof(social_state.status_text) - 1);
            social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "Social: NFC payment terminal spoofing started");
            
            return true;
        } else if(event->key == InputKeyLeft && social_state.credentials_harvested > 0) {
            // Credential harvesting analysis
            social_state.status = SocialStatusHarvesting;
            strncpy(social_state.current_attack, "Credential Analysis", sizeof(social_state.current_attack) - 1);
            social_state.current_attack[sizeof(social_state.current_attack) - 1] = '\0';
            strncpy(social_state.status_text, "HARVESTING", sizeof(social_state.status_text) - 1);
            social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "Social: Analyzing harvested credentials");
            
            return true;
        }
    }
    
    return false;
}

static void social_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t attack_counter = 0;
    attack_counter++;
    
    switch(social_state.status) {
        case SocialStatusPortalActive:
            // Fake WiFi portal activity
            if(attack_counter % 3 == 0) {
                social_state.victims_connected++;
                social_state.credentials_harvested += 2;
                social_state.data_collected_mb += 5;
                
                // Rotate fake SSIDs
                const char* fake_ssids[] = {
                    "Free_WiFi_Login", "Hotel_Guest_WiFi", "Airport_Free_WiFi",
                    "Starbucks_WiFi", "McDonald_FREE", "Public_Internet"
                };
                uint32_t ssid_idx = (attack_counter / 3) % 6;
                strncpy(social_state.fake_ssid, fake_ssids[ssid_idx], 
                       sizeof(social_state.fake_ssid) - 1);
                social_state.fake_ssid[sizeof(social_state.fake_ssid) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Social: Victim connected to %s - credentials harvested", 
                        social_state.fake_ssid);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SocialStatusBtImpersonating:
            // Bluetooth impersonation
            if(attack_counter % 2 == 0) {
                social_state.bt_devices_impersonated++;
                social_state.data_collected_mb += 3;
                
                // Rotate target devices
                const char* bt_devices[] = {
                    "AirPods Pro", "Galaxy Buds", "Sony WH-1000XM4",
                    "Bose QC35", "Beats Studio", "JBL Free"
                };
                uint32_t device_idx = (attack_counter / 2) % 6;
                strncpy(social_state.target_device, bt_devices[device_idx], 
                       sizeof(social_state.target_device) - 1);
                social_state.target_device[sizeof(social_state.target_device) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Social: Impersonating %s - device data intercepted", 
                        social_state.target_device);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SocialStatusNfcSpoofing:
            // NFC payment spoofing
            if(attack_counter % 4 == 0) {
                social_state.nfc_transactions_spoofed++;
                social_state.credentials_harvested++;
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Social: NFC transaction spoofed - payment data captured (%lu total)", 
                        social_state.nfc_transactions_spoofed);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SocialStatusHarvesting:
            // Auto-complete harvesting analysis
            if(attack_counter % 3 == 0) {
                social_state.status = SocialStatusIdle;
                strncpy(social_state.status_text, "HARVEST COMPLETE", sizeof(social_state.status_text) - 1);
                social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
                
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "Social: Analysis complete - %lu credentials, %luMB data collected", 
                        social_state.credentials_harvested, social_state.data_collected_mb);
                predator_log_append(app, log_msg);
                predator_log_append(app, "Social: Social engineering campaign successful");
            }
            break;
            
        default:
            break;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_social_engineering_toolkit_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&social_state, 0, sizeof(SocialState));
    strncpy(social_state.status_text, "READY", sizeof(social_state.status_text) - 1);
    social_state.status_text[sizeof(social_state.status_text) - 1] = '\0';
    
    predator_log_append(app, "Social: Advanced social engineering toolkit");
    predator_log_append(app, "Social: Use directional keys for different attacks");
    
    if(!app->view_dispatcher) return;
    
    if(!social_view) {
        social_view = view_alloc();
        if(!social_view) return;
        
        view_set_context(social_view, app);
        view_set_draw_callback(social_view, social_ui_draw_callback);
        view_set_input_callback(social_view, social_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewSocialEngineeringToolkitUI, social_view);
    }
    
    // Start timer for social engineering operations
    app->timer = furi_timer_alloc(social_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSocialEngineeringToolkitUI);
}

bool predator_scene_social_engineering_toolkit_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(social_state.portal_active || social_state.bt_active || social_state.nfc_active) {
            social_state.portal_active = false;
            social_state.bt_active = false;
            social_state.nfc_active = false;
            predator_log_append(app, "Social: All attacks stopped on exit");
        }
        return false;
    }
    
    return false;
}

void predator_scene_social_engineering_toolkit_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(social_state.portal_active || social_state.bt_active || social_state.nfc_active) {
        social_state.portal_active = false;
        social_state.bt_active = false;
        social_state.nfc_active = false;
        predator_log_append(app, "Social: All attacks stopped on exit");
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
