#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint8_t current_page;
    uint8_t selected_car;
    uint8_t animation_frame;
    bool command_sent;
} CarModelsView;

// Car models data
typedef struct {
    const char* make;
    const char* model;
    uint32_t frequency;
    const char* remote_type;
} CarModelInfo;

// List of supported car models
static const CarModelInfo car_models[] = {
    // Standard models from predator_subghz.c
    {"Toyota", "Camry 2018-2022", 433920000, "Rolling Code"},
    {"Honda", "Civic 2016-2022", 433420000, "Rolling Code"},
    {"Ford", "F-150 2018+", 315000000, "Fixed Code"},
    {"Chevrolet", "Silverado 2019+", 315000000, "Fixed Code"},
    {"BMW", "5-Series 2018+", 433920000, "Rolling Code"},
    {"Mercedes", "C-Class 2020+", 433920000, "Rolling Code"},
    {"Audi", "Q5 2018-2022", 868350000, "Rolling Code"},
    {"Volkswagen", "Golf 2019+", 433920000, "Rolling Code"},
    {"Nissan", "Rogue 2020+", 433920000, "Fixed Code"},
    {"Hyundai", "Tucson 2019+", 433920000, "Rolling Code"},
    {"Kia", "Sportage 2020+", 433920000, "Rolling Code"},
    {"Tesla", "Model 3", 315000000, "Smart Key"},
    {"Subaru", "Forester 2019+", 433920000, "Rolling Code"},
    {"Jeep", "Grand Cherokee 2018+", 315000000, "Fixed Code"},
    {"Chrysler", "300 2019+", 315000000, "Fixed Code"},
    {"Dodge", "Charger 2020+", 315000000, "Fixed Code"},
    {"Cadillac", "Escalade 2021+", 315000000, "Smart Key"},
    {"Lexus", "RX 350 2019+", 433920000, "Rolling Code"},
    {"Infiniti", "QX60 2020+", 315000000, "Smart Key"},
    {"Acura", "MDX 2020+", 433420000, "Rolling Code"},
    {"Mazda", "CX-5 2019+", 433920000, "Rolling Code"},
    {"Mitsubishi", "Outlander 2020+", 433920000, "Fixed Code"},
    {"Porsche", "Cayenne 2018+", 433920000, "Rolling Code"},
    {"Range Rover", "Evoque 2020+", 433920000, "Smart Key"},
    {"Jaguar", "F-Pace 2021+", 433920000, "Smart Key"},
    {"Volvo", "XC60 2019+", 433920000, "Rolling Code"},
    {"Fiat", "500X 2020+", 433920000, "Fixed Code"},
    {"Peugeot", "3008 2020+", 433920000, "Rolling Code"},
    {"Renault", "Koleos 2020+", 433920000, "Fixed Code"},
    {"Skoda", "Kodiaq 2020+", 433920000, "Rolling Code"},
    
    // Additional luxury models
    {"Lamborghini", "Urus 2021+", 433920000, "Smart Key"},
    {"Ferrari", "Roma 2021+", 433920000, "Smart Key"},
    {"Maserati", "Levante 2020+", 433920000, "Smart Key"},
    {"Bentley", "Bentayga 2021+", 433920000, "Smart Key"},
    {"Rolls Royce", "Ghost 2021+", 433920000, "Smart Key"},
    {"Aston Martin", "Vantage 2020+", 433920000, "Smart Key"},
    {"McLaren", "GT 2021+", 433920000, "Smart Key"},
    {"Bugatti", "Chiron 2020+", 433920000, "Smart Key"},
    {"Genesis", "G80 2021+", 433920000, "Smart Key"},
    {"Land Rover", "Defender 2020+", 433920000, "Smart Key"},
};

#define CAR_MODELS_COUNT (sizeof(car_models) / sizeof(car_models[0]))
#define ITEMS_PER_PAGE 3

static void car_models_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    CarModelsView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Car Models");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Calculate page details
    uint8_t start_idx = state->current_page * ITEMS_PER_PAGE;
    uint8_t end_idx = MIN(start_idx + ITEMS_PER_PAGE, CAR_MODELS_COUNT);
    uint8_t total_pages = (CAR_MODELS_COUNT + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    
    // Draw cars list
    canvas_set_font(canvas, FontSecondary);
    
    for(uint8_t i = start_idx; i < end_idx; i++) {
        uint8_t y_pos = 25 + (i - start_idx) * 12;
        
        // Highlight selected car
        if(i == state->selected_car) {
            canvas_draw_box(canvas, 2, y_pos - 1, 124, 11);
            canvas_set_color(canvas, ColorWhite);
        }
        
        // Draw car make and model
        char model_text[32];
        snprintf(model_text, sizeof(model_text), "%s %s", 
                car_models[i].make, car_models[i].model);
        canvas_draw_str(canvas, 4, y_pos + 7, model_text);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw car details box for selected car
    predator_ui_draw_status_box(canvas, "Car Details", 5, 62, 118, 16);
    
    const CarModelInfo* selected = &car_models[state->selected_car];
    
    // Format frequency
    char freq_text[16];
    if(selected->frequency >= 1000000) {
        snprintf(freq_text, sizeof(freq_text), "%.2fMHz", selected->frequency / 1000000.0f);
    } else {
        snprintf(freq_text, sizeof(freq_text), "%.0fKHz", selected->frequency / 1000.0f);
    }
    
    // Draw car details
    char detail_text[64];
    snprintf(detail_text, sizeof(detail_text), "%s: %s  Freq: %s", 
            selected->make, selected->remote_type, freq_text);
    canvas_draw_str_aligned(canvas, 64, 70, AlignCenter, AlignCenter, detail_text);
    
    // Draw command sent indicator when button is pressed
    if(state->command_sent) {
        // Draw signal animation
        uint8_t car_x = 90;
        uint8_t car_y = 40;
        
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t radius = 3 + (i * 3);
            canvas_draw_circle(canvas, car_x, car_y, radius);
        }
    }
    
    // Draw page indicator
    char page_text[16];
    snprintf(page_text, sizeof(page_text), "Page %d/%d", state->current_page + 1, total_pages);
    canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignCenter, page_text);
    
    // Draw navigation controls
    if(state->current_page > 0) {
        elements_button_left(canvas, "Prev");
    } else {
        elements_button_left(canvas, "Back");
    }
    
    elements_button_center(canvas, "Send");
    
    if(state->current_page < total_pages - 1) {
        elements_button_right(canvas, "Next");
    }
}

static bool car_models_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    CarModelsView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(state->selected_car > state->current_page * ITEMS_PER_PAGE) {
                state->selected_car--;
                state->command_sent = false;
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            {
                uint8_t page_end = MIN((state->current_page + 1) * ITEMS_PER_PAGE, CAR_MODELS_COUNT) - 1;
                if(state->selected_car < page_end) {
                    state->selected_car++;
                    state->command_sent = false;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyLeft:
            if(state->current_page > 0) {
                state->current_page--;
                state->selected_car = state->current_page * ITEMS_PER_PAGE;
                state->command_sent = false;
                consumed = true;
            } else {
                // Exit on left from first page
                scene_manager_previous_scene(app->scene_manager);
                consumed = true;
            }
            break;
            
        case InputKeyRight:
            {
                uint8_t total_pages = (CAR_MODELS_COUNT + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
                if(state->current_page < total_pages - 1) {
                    state->current_page++;
                    state->selected_car = state->current_page * ITEMS_PER_PAGE;
                    state->command_sent = false;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyOk:
        case InputKeyCenter:
            // Send signal for the selected car
            if(predator_subghz_send_car_command(app, state->selected_car, 0)) {
                state->command_sent = true;
            }
            consumed = true;
            break;
            
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* car_models_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, car_models_view_draw_callback);
    view_set_input_callback(view, car_models_view_input_callback);
    
    // Initialize model
    CarModelsView* state = malloc(sizeof(CarModelsView));
    state->current_page = 0;
    state->selected_car = 0;
    state->animation_frame = 0;
    state->command_sent = false;
    
    view_set_model(view, state);
    view_set_model_free_callback(view, free);
    
    return view;
}

static void car_models_view_free(View* view) {
    view_free(view);
}

void predator_scene_car_models_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize SubGHz
    predator_subghz_init(app);
    
    // Create custom view
    View* view = car_models_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_car_models_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        // Force view refresh for animations when command is sent
        CarModelsView* state = view_get_model(app->view_dispatcher->current_view);
        if(state && state->command_sent) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_models_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up
    predator_subghz_deinit(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        car_models_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}
