#pragma once

typedef enum {
    // Main menu categories
    SubmenuIndexWifiAttacks,
    SubmenuIndexBluetoothAttacks,
    SubmenuIndexSubGhzAttacks,
    SubmenuIndexCarAttacks,
    SubmenuIndexRfidAttacks,
    SubmenuIndexGpsTracker,
    SubmenuIndexWardriving,
    SubmenuIndexSocialEngineering,
    SubmenuIndexQuickPoc,
    SubmenuIndexModuleStatus,
    SubmenuIndexBoardSelection,
    SubmenuIndexSettings,
    SubmenuIndexAbout,
    
    // WiFi attack specific indexes
    SubmenuIndexWifiScan,
    SubmenuIndexWifiDeauth,
    SubmenuIndexWifiEvilTwin,
    SubmenuIndexWifiHandshake,
    SubmenuIndexWifiPmkid,
    
    // Bluetooth attack specific indexes
    SubmenuIndexBleScan,
    SubmenuIndexBleSpam,
    
    // Car attack specific indexes
    SubmenuIndexCarTestResults,
    SubmenuIndexWalkingOpen,
    SubmenuIndexCarTesla,
    SubmenuIndexCarModels,
    SubmenuIndexCarKeyBruteforce,
    SubmenuIndexCarJamming,
    SubmenuIndexCarPassiveOpener,
    
    // SubGHz attack specific indexes
    SubmenuIndexSubGhzJamming,
    SubmenuIndexSubGhzRawSend,
    
    // RFID attack specific indexes
    SubmenuIndexRfidClone,
    SubmenuIndexRfidBruteforce,
    SubmenuIndexRfidFuzzing,
} SubmenuIndex;
