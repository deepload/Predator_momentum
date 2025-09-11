#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

// Simple Social Engineering menu using a custom view (new UX style)
typedef enum {
    SocialItemCaptivePortal,
    SocialItemPhishingAP,
    SocialItemFakeUpdate,
    SocialItemQRCodeGen,
    SocialItemUSBRubberDucky,
    SocialItemCount,
} SocialItem;

typedef struct {
    View* view;
    uint8_t selected_index;
    uint8_t scroll_position;
} SocialEngineeringView;

static const struct {
    const char* name;
    const char* icon;
} kSocialItems[SocialItemCount] = {
    {"Captive Portal", "🌐"},
    {"Phishing AP", "📡"},
    {"Fake Update", "💻"},
    {"QR Code", "📱"},
    {"USB Rubber Ducky", "🦆"},
};

static void social_eng_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    if(!app) return;

    SocialEngineeringView* state = PREDATOR_GET_MODEL(app->view_dispatcher, SocialEngineeringView);
    if(!state) return;

    canvas_clear(canvas);

    // Title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Social Engineering");
    canvas_draw_line(canvas, 0, 16, 128, 16);

    // List
    canvas_set_font(canvas, FontSecondary);
    const uint8_t items_on_screen = 4;
    if(state->selected_index >= items_on_screen) {
        state->scroll_position = state->selected_index - items_on_screen + 1;
    } else {
        state->scroll_position = 0;
    }

    for(uint8_t i = 0; i < items_on_screen && (i + state->scroll_position) < SocialItemCount; i++) {
        uint8_t idx = i + state->scroll_position;
        uint8_t y = 25 + i * 12;
        if(idx == state->selected_index) {
            canvas_draw_box(canvas, 4, y - 1, 120, 11);
            canvas_set_color(canvas, ColorWhite);
        }
        // Icon + name
        canvas_draw_str(canvas, 6, y + 7, kSocialItems[idx].icon);
        canvas_draw_str(canvas, 20, y + 7, kSocialItems[idx].name);
        canvas_set_color(canvas, ColorBlack);
    }

    // Scroll indicators
    predator_ui_draw_scroll_vertical(canvas, 120, 24, 60, state->scroll_position > 0, (state->scroll_position + items_on_screen) < SocialItemCount);

    // Controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "OK");
}

static bool social_eng_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;

    SocialEngineeringView* state = PREDATOR_GET_MODEL(app->view_dispatcher, SocialEngineeringView);
    if(!state) return consumed;

    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(state->selected_index > 0) { state->selected_index--; consumed = true; }
            break;
        case InputKeyDown:
            if(state->selected_index < SocialItemCount - 1) { state->selected_index++; consumed = true; }
            break;
        case InputKeyOk:
        case InputKeyRight: {
            // Show popup with action info
            const char* title = kSocialItems[state->selected_index].name;
            popup_set_header(app->popup, title, 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Starting...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            app->attack_running = true;
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            consumed = true;
            break; }
        case InputKeyLeft:
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

static View* social_eng_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, social_eng_draw_callback);
    view_set_input_callback(view, social_eng_input_callback);

    SocialEngineeringView* state = malloc(sizeof(SocialEngineeringView));
    state->selected_index = 0;
    state->scroll_position = 0;

    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    return view;
}

static void social_eng_view_free(View* view) {
    view_free(view);
}

void predator_scene_social_engineering_new_on_enter(void* context) {
    PredatorApp* app = context;
    View* view = social_eng_view_alloc(app);

    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_social_engineering_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    return consumed;
}

void predator_scene_social_engineering_new_on_exit(void* context) {
    PredatorApp* app = context;
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        social_eng_view_free(view);
    }
    // Restore standard submenu view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}
