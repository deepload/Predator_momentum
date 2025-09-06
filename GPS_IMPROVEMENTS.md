# GPS Satellite Detection Improvements

This document details the improvements made to the GPS satellite detection functionality in the Predator app for Flipper Zero.

## Problem Summary

The original implementation suffered from inconsistent satellite detection which resulted in:
- Difficulty acquiring GPS signals
- Poor satellite count accuracy
- Intermittent position fixes
- Slow time-to-first-fix (TTFF)

## Technical Improvements

### 1. Enhanced NMEA Parsing

```c
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
            
            // Parse the complete NMEA sentence
            predator_gps_parse_nmea(app, line_start);
        }
    }
}
```

### 2. Multi-Sentence Type Support

The improved parser now handles multiple NMEA sentence types:

#### GSV (Satellites in View)
```c
// GSV = GPS Satellites in View
// Format: $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
//         $GPGSV,num_msgs,msg_num,num_sats,...
if(strncmp(sentence, "$GPGSV", 6) == 0 || strncmp(sentence, "$GNGSV", 6) == 0) {
    // Process satellite count information
    uint32_t sats_in_view = atoi(token);
    if(sats_in_view > app->satellites) {
        app->satellites = sats_in_view;
    }
}
```

#### GGA (Global Positioning System Fix Data)
```c
// Parse GGA sentence (primary position data)
if(strncmp(sentence, "$GPGGA", 6) == 0 || strncmp(sentence, "$GNGGA", 6) == 0) {
    // Process position and satellite data
    // Field 7 contains satellite count
    app->satellites = atoi(token);
}
```

#### RMC (Recommended Minimum Data)
```c
// RMC = Recommended Minimum specific GPS/Transit data
if(strncmp(sentence, "$GPRMC", 6) == 0 || strncmp(sentence, "$GNRMC", 6) == 0) {
    // Field 2 contains fix validity
    if(token[0] == 'A') {
        valid_fix = true;
        app->gps_connected = true;
    }
}
```

### 3. GPS Module Configuration

The initialization now sends specific commands to configure the GPS module optimally:

```c
// Send GPS module wake-up and configuration commands
const char* config_cmds[] = {
    "$PMTK001,0,3*30\r\n",      // Wake up
    "$PMTK220,100*2F\r\n",      // Set position update rate to 10Hz
    "$PMTK314,0,1,0,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n", // Enable GPRMC, GPGGA, GPGSV
    "$PMTK313,1*2E\r\n",        // Enable SBAS satellite search
    "$PMTK301,2*2E\r\n",        // Enable SBAS to be used for DGPS
    "$PMTK286,1*23\r\n"         // Enable AIC (anti-interference)
};
```

### 4. Robust Error Handling

Added proper error handling throughout the GPS code:

```c
if (app->gps_uart == NULL) {
    FURI_LOG_E("Predator", "Failed to initialize GPS UART");
    app->gps_connected = false;
    return;
}

// Properly handle string operations
size_t copy_len = len < GPS_BUFFER_SIZE ? len : GPS_BUFFER_SIZE;
memcpy(temp_buf, buf, copy_len);
temp_buf[copy_len] = '\0';
```

## Diagnostic Tools

### GPS Debug Screen

Added a dedicated diagnostic screen with:
- Real-time NMEA sentence display
- Message type counters (GGA, RMC, GSV)
- Satellite count and position information
- Fix validity indicators

```c
void predator_gps_debug_track_nmea(const char* nmea) {
    if(!gps_debug_state || !nmea) return;
    
    // Save most recent NMEA sentence
    strncpy(gps_debug_state->last_nmea, nmea, sizeof(gps_debug_state->last_nmea) - 1);
    
    // Track message types
    if(strncmp(nmea, "$GPGGA", 6) == 0 || strncmp(nmea, "$GNGGA", 6) == 0) {
        gps_debug_state->gga_count++;
    } else if(strncmp(nmea, "$GPRMC", 6) == 0 || strncmp(nmea, "$GNRMC", 6) == 0) {
        gps_debug_state->rmc_count++;
        // Check fix validity
        gps_debug_state->last_fix_valid = (*token == 'A');
    } else if(strncmp(nmea, "$GPGSV", 6) == 0 || strncmp(nmea, "$GNGSV", 6) == 0) {
        gps_debug_state->gsv_count++;
    }
}
```

## Performance Improvements

### Signal Acquisition Time
- Reduced time-to-first-fix by sending optimal configuration
- Added support for both GPS and GLONASS via $GN sentences

### Accuracy Enhancements
- Proper handling of N/S and E/W indicators for coordinate signs
- Fixed coordinate format conversions from DDMM.MMMM to DD.DDDD

### Satellite Counting
- More accurate satellite count via GSV parsing
- Cross-verification between GGA and GSV data

## Testing Your GPS

1. Navigate to the GPS Tracker screen
2. Press right to enter the GPS Debug screen
3. Monitor satellite count and NMEA messages
4. Ensure your device has a clear view of the sky
5. Wait for satellite count to increase (can take 30-90 seconds)

With these improvements, your Predator app should now reliably detect satellites for GPS tracking functionality.
