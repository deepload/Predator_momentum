#include "../predator_i.h"
#include "../helpers/predator_scada_real.h"
#include "../helpers/predator_logging.h"

// GLOBAL GOVERNMENT CONTRACTS - ALL COUNTRIES SUPPORTED
static void government_contracts_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    FURI_LOG_I("GovContracts", "🌍 Government contract selected: Index %lu", index);
    
    switch(index) {
        case 0: // North America
            FURI_LOG_I("GovContracts", "🇺🇸🇨🇦🇲🇽 North America Contracts");
            predator_log_append(app, "🇺🇸 USA: Critical Infrastructure Security");
            predator_log_append(app, "🇨🇦 Canada: Power Grid Assessment");
            predator_log_append(app, "🇲🇽 Mexico: Border Security Systems");
            scada_government_demonstration(app);
            break;
            
        case 1: // Europe
            FURI_LOG_I("GovContracts", "🇪🇺 European Union Contracts");
            predator_log_append(app, "🇨🇭 Switzerland: Traffic Light Security");
            predator_log_append(app, "🇩🇪 Germany: Industrial Control Security");
            predator_log_append(app, "🇫🇷 France: Nuclear Plant Security");
            predator_log_append(app, "🇬🇧 UK: Financial Infrastructure");
            predator_log_append(app, "🇮🇹 Italy: Transportation Security");
            scada_government_demonstration(app);
            break;
            
        case 2: // Asia-Pacific
            FURI_LOG_I("GovContracts", "🌏 Asia-Pacific Contracts");
            predator_log_append(app, "🇯🇵 Japan: Smart City Security");
            predator_log_append(app, "🇰🇷 South Korea: 5G Infrastructure");
            predator_log_append(app, "🇦🇺 Australia: Mining Security");
            predator_log_append(app, "🇸🇬 Singapore: Port Security");
            predator_log_append(app, "🇮🇳 India: Railway Security");
            scada_government_demonstration(app);
            break;
            
        case 3: // Middle East & Africa
            FURI_LOG_I("GovContracts", "🌍 Middle East & Africa Contracts");
            predator_log_append(app, "🇸🇦 Saudi Arabia: Oil Infrastructure");
            predator_log_append(app, "🇦🇪 UAE: Smart City Security");
            predator_log_append(app, "🇮🇱 Israel: Defense Systems");
            predator_log_append(app, "🇿🇦 South Africa: Mining Security");
            predator_log_append(app, "🇪🇬 Egypt: Suez Canal Security");
            scada_government_demonstration(app);
            break;
            
        case 4: // South America
            FURI_LOG_I("GovContracts", "🌎 South America Contracts");
            predator_log_append(app, "🇧🇷 Brazil: Energy Grid Security");
            predator_log_append(app, "🇦🇷 Argentina: Agricultural Security");
            predator_log_append(app, "🇨🇱 Chile: Mining Infrastructure");
            predator_log_append(app, "🇨🇴 Colombia: Oil Pipeline Security");
            predator_log_append(app, "🇵🇪 Peru: Mining Operations");
            scada_government_demonstration(app);
            break;
            
        case 5: // Nordic Countries
            FURI_LOG_I("GovContracts", "❄️ Nordic Countries Contracts");
            predator_log_append(app, "🇸🇪 Sweden: Energy Infrastructure");
            predator_log_append(app, "🇳🇴 Norway: Oil Platform Security");
            predator_log_append(app, "🇩🇰 Denmark: Wind Farm Security");
            predator_log_append(app, "🇫🇮 Finland: Nuclear Security");
            predator_log_append(app, "🇮🇸 Iceland: Geothermal Security");
            scada_government_demonstration(app);
            break;
            
        case 6: // Eastern Europe
            FURI_LOG_I("GovContracts", "🏛️ Eastern Europe Contracts");
            predator_log_append(app, "🇵🇱 Poland: Border Security");
            predator_log_append(app, "🇨🇿 Czech Republic: Nuclear Plants");
            predator_log_append(app, "🇭🇺 Hungary: Gas Pipeline Security");
            predator_log_append(app, "🇷🇴 Romania: Energy Infrastructure");
            predator_log_append(app, "🇺🇦 Ukraine: Critical Infrastructure");
            scada_government_demonstration(app);
            break;
            
        case 7: // Southeast Asia
            FURI_LOG_I("GovContracts", "🌴 Southeast Asia Contracts");
            predator_log_append(app, "🇹🇭 Thailand: Smart Grid Security");
            predator_log_append(app, "🇲🇾 Malaysia: Port Security");
            predator_log_append(app, "🇮🇩 Indonesia: Oil & Gas Security");
            predator_log_append(app, "🇵🇭 Philippines: Island Security");
            predator_log_append(app, "🇻🇳 Vietnam: Manufacturing Security");
            scada_government_demonstration(app);
            break;
    }
}

void predator_scene_government_contracts_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🌍 GLOBAL GOVERNMENT CONTRACTS");
    
    // ALL REGIONS SUPPORTED FOR MAXIMUM SALES
    submenu_add_item(app->submenu, "🇺🇸🇨🇦🇲🇽 North America", 0, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🇪🇺 Europe (27 Countries)", 1, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🌏 Asia-Pacific", 2, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🌍 Middle East & Africa", 3, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🌎 South America", 4, government_contracts_callback, app);
    submenu_add_item(app->submenu, "❄️ Nordic Countries", 5, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🏛️ Eastern Europe", 6, government_contracts_callback, app);
    submenu_add_item(app->submenu, "🌴 Southeast Asia", 7, government_contracts_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("GovContracts", "🌍 Global Government Contracts UI entered");
    predator_log_append(app, "🌍 Ready for WORLDWIDE government sales");
}

bool predator_scene_government_contracts_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_government_contracts_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    FURI_LOG_I("GovContracts", "🌍 Global Government Contracts UI exited");
}
