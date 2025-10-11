#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_models_hardcoded.h"
#include "../helpers/predator_car_attacks.h"

// LIGHTWEIGHT CAR BRANDS SECURITY - MEMORY OPTIMIZED
// Essential car brand testing without heavy memory usage

static void car_brands_lite_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    const char* brands[] = {"BMW", "Mercedes", "Audi", "Ford", "Toyota", "Honda"};
    
    if(index < 6) {
        char log_msg[64];
        snprintf(log_msg, sizeof(log_msg), "🚗 %s Security Analysis", brands[index]);
        predator_log_append(app, log_msg);
        
        // Find and test first model of this brand
        size_t model_count = predator_models_get_hardcoded_count();
        for(size_t i = 0; i < model_count; i++) {
            const PredatorCarModel* model = predator_models_get_hardcoded(i);
            if(model && strstr(model->make, brands[index])) {
                char model_log[80];
                snprintf(model_log, sizeof(model_log), "Testing: %s %s", model->make, model->model);
                predator_log_append(app, model_log);
                
                // Perform lightweight attack
                predator_car_attack_rolling_code(app, i);
                break;
            }
        }
    }
}

void predator_scene_car_brands_lite_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "CARS");
    
    // ONU OPTIMIZATION
    submenu_add_item(app->submenu, "BMW", 0, car_brands_lite_callback, app);
    submenu_add_item(app->submenu, "Mercedes", 1, car_brands_lite_callback, app);
    submenu_add_item(app->submenu, "Audi", 2, car_brands_lite_callback, app);
    submenu_add_item(app->submenu, "Ford", 3, car_brands_lite_callback, app);
    submenu_add_item(app->submenu, "Toyota", 4, car_brands_lite_callback, app);
    submenu_add_item(app->submenu, "Honda", 5, car_brands_lite_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    predator_log_append(app, "Cars activated");
    FURI_LOG_I("Cars", "Cars entered");
}

bool predator_scene_car_brands_lite_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_car_brands_lite_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    FURI_LOG_I("CarBrandsLite", "Car Brands Lite Security Suite exited");
}
