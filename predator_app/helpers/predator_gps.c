#include "predator_gps.h"
#include "../predator_i.h"
#include "../predator_uart.h"
#include "predator_string.h"
#include <furi.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration for GPS debug tracking function
extern void predator_gps_debug_track_nmea(const char* nmea);

#define GPS_UART_BAUD PREDATOR_GPS_UART_BAUD
#define GPS_BUFFER_SIZE 512

void predator_gps_rx_callback(uint8_t* buf, size_t len, void* context) {
    PredatorApp* app = (PredatorApp*)context;
    
    // Process GPS NMEA data
    if(len > 0 && buf != NULL) {
        // Null-terminate the buffer to ensure we can use string functions safely
        char temp_buf[GPS_BUFFER_SIZE + 1];
        size_t copy_len = len < GPS_BUFFER_SIZE ? len : GPS_BUFFER_SIZE;
        memcpy(temp_buf, buf, copy_len);
        temp_buf[copy_len] = '\0';
        
        // Look for NMEA sentences (support both $GP and $GN prefixes)
        char* line_start = temp_buf;
        char* line_end = NULL;
        
        // Process each complete NMEA sentence in the buffer
        while ((line_end = strchr(line_start, '\n')) != NULL) {
            // Null-terminate this line
            *line_end = '\0';
            
            // If line starts with $ and contains at least 6 chars, process it
            if (line_start[0] == '$' && strlen(line_start) >= 6) {
                // Check for GPS or GNSS sentences
                if (strncmp(line_start, "$GP", 3) == 0 || 
                    strncmp(line_start, "$GN", 3) == 0) {
                    
                    app->gps_connected = true;
                    
                    // Track NMEA sentence in debug system
                    predator_gps_debug_track_nmea(line_start);
                    
                    // Parse the complete NMEA sentence using our robust parser
                    predator_gps_parse_nmea(app, line_start);
                }
            }
            
            // Move to the start of the next line
            line_start = line_end + 1;
        }
    }
}

void predator_gps_init(PredatorApp* app) {
    // Check GPS power switch state (front left switch must be down)
    furi_hal_gpio_init(PREDATOR_GPS_POWER_SWITCH, GpioModeInput, GpioPullUp, GpioSpeedLow);
    if(furi_hal_gpio_read(PREDATOR_GPS_POWER_SWITCH)) {
        app->gps_connected = false;
        FURI_LOG_W("Predator", "GPS power switch is off (switch down to enable)");
        return; // GPS switch is up (using internal battery)
    }
    
    // Initialize UART for GPS communication on pins 13,14
    app->gps_uart = predator_uart_init(PREDATOR_GPS_UART_TX_PIN, PREDATOR_GPS_UART_RX_PIN, GPS_UART_BAUD, predator_gps_rx_callback, app);
    
    if (app->gps_uart == NULL) {
        FURI_LOG_E("Predator", "Failed to initialize GPS UART");
        app->gps_connected = false;
        return;
    }
    
    FURI_LOG_I("Predator", "GPS UART initialized, waiting for satellite data");
    
    app->gps_connected = false;
    app->latitude = 0.0f;
    app->longitude = 0.0f;
    app->satellites = 0;
    
    // Send GPS module wake-up and configuration commands
    // These commands help ensure the module is in NMEA mode and reporting all satellites
    const char* config_cmds[] = {
        "$PMTK001,0,3*30\r\n",      // Wake up
        "$PMTK220,100*2F\r\n",      // Set position update rate to 10Hz
        "$PMTK314,0,1,0,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n", // Enable GPRMC, GPGGA, GPGSV
        "$PMTK313,1*2E\r\n",        // Enable SBAS satellite search
        "$PMTK301,2*2E\r\n",        // Enable SBAS to be used for DGPS
        "$PMTK286,1*23\r\n"         // Enable AIC (anti-interference)
    };
    
    // Send configuration commands to GPS module
    for (size_t i = 0; i < sizeof(config_cmds)/sizeof(config_cmds[0]); i++) {
        if (app->gps_uart) {
            predator_uart_tx(app->gps_uart, (uint8_t*)config_cmds[i], strlen(config_cmds[i]));
            furi_delay_ms(100); // Short delay between commands
        }
    }
}

void predator_gps_deinit(PredatorApp* app) {
    if(app->gps_uart) {
        predator_uart_deinit(app->gps_uart);
        app->gps_uart = NULL;
    }
    app->gps_connected = false;
}

void predator_gps_update(PredatorApp* app) {
    // GPS data is updated via UART callback
    UNUSED(app);
}

bool predator_gps_parse_nmea(PredatorApp* app, const char* sentence) {
    if(!app || !sentence) return false;
    
    // GSV sentence contains satellite info
    if(strncmp(sentence, "$GPGSV", 6) == 0 || strncmp(sentence, "$GNGSV", 6) == 0) {
        // GSV = GPS Satellites in View
        // Format: $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
        //         $GPGSV,num_msgs,msg_num,num_sats,...

        // Get the 4th field (number of satellites)
        char* sats_field = predator_get_next_field(sentence, 3, ',');
        if(sats_field && strlen(sats_field) > 0) {
            // This field is the total number of satellites in view
            uint32_t sats_in_view = atoi(sats_field);
            if(sats_in_view > app->satellites) {
                app->satellites = sats_in_view;
            }
            if(sats_in_view > 0) {
                app->gps_connected = true;
            }
        }
        
        return true;
    }
    
    // Parse GGA sentence (primary position data)
    if(strncmp(sentence, "$GPGGA", 6) == 0 || strncmp(sentence, "$GNGGA", 6) == 0) {
        // Parse GGA data
        
        // Process latitude (field 2 and 3)
        char* lat_str = predator_get_next_field(sentence, 2, ',');
        char* ns_indicator = predator_get_next_field(sentence, 3, ',');
        
        if(lat_str && strlen(lat_str) > 0 && ns_indicator && (*ns_indicator == 'N' || *ns_indicator == 'S')) {
            char ns = *ns_indicator;
            
            // Convert DDMM.MMMM to decimal degrees
            char* dot = strchr(lat_str, '.');
            if(dot) {
                int dot_pos = dot - lat_str;
                if(dot_pos >= 2) {
                    char deg_part[10] = {0};
                    char min_part[15] = {0};
                    
                    strncpy(deg_part, lat_str, dot_pos - 2);
                    strcpy(min_part, lat_str + dot_pos - 2);
                    
                    float degrees = strtof(deg_part, NULL);
                    float minutes = strtof(min_part, NULL);
                    
                    app->latitude = degrees + (minutes / 60.0f);
                    
                    // Apply N/S sign
                    if(ns == 'S') app->latitude = -app->latitude;
                    
                    app->gps_connected = true;
                }
            }
        }
        
        // Process longitude (field 4 and 5)
        char* lon_str = predator_get_next_field(sentence, 4, ',');
        char* ew_indicator = predator_get_next_field(sentence, 5, ',');
        
        if(lon_str && strlen(lon_str) > 0 && ew_indicator && (*ew_indicator == 'E' || *ew_indicator == 'W')) {
            char ew = *ew_indicator;
            
            // Convert DDDMM.MMMM to decimal degrees
            char* dot = strchr(lon_str, '.');
            if(dot) {
                int dot_pos = dot - lon_str;
                if(dot_pos >= 2) {
                    char deg_part[10] = {0};
                    char min_part[15] = {0};
                    
                    strncpy(deg_part, lon_str, dot_pos - 2);
                    strcpy(min_part, lon_str + dot_pos - 2);
                    
                    float degrees = strtof(deg_part, NULL);
                    float minutes = strtof(min_part, NULL);
                    
                    app->longitude = degrees + (minutes / 60.0f);
                    
                    // Apply E/W sign
                    if(ew == 'W') app->longitude = -app->longitude;
                    
                    app->gps_connected = true;
                }
            }
        }
        
        // Get number of satellites (field 7)
        char* sats_str = predator_get_next_field(sentence, 7, ',');
        if(sats_str && strlen(sats_str) > 0) {
            app->satellites = atoi(sats_str);
            app->gps_connected = true;
        }
        
        return true;
    }
    
    // RMC sentence contains the recommended minimum data
    if(strncmp(sentence, "$GPRMC", 6) == 0 || strncmp(sentence, "$GNRMC", 6) == 0) {
        // RMC = Recommended Minimum specific GPS/Transit data
        // We parse this to get status information and backup position
        
        // Get status field (field 2: A=active, V=void)
        char* status_field = predator_get_next_field(sentence, 2, ',');
        if(status_field && *status_field == 'A') {
            app->gps_connected = true;
            // We could also parse additional fields here if needed
        }
        
        return true;
    }
    
    return false;
}

bool predator_gps_get_coordinates(PredatorApp* app, float* lat, float* lon) {
    if(!app || !lat || !lon) return false;
    
    *lat = app->latitude;
    *lon = app->longitude;
    
    return app->gps_connected && app->satellites > 0;
}

uint32_t predator_gps_get_satellites(PredatorApp* app) {
    if(!app) return 0;
    return app->satellites;
}

bool predator_gps_is_connected(PredatorApp* app) {
    if(!app) return false;
    return app->gps_connected;
}
