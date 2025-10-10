#pragma once

typedef enum {
    // ELON'S TESLA SECURITY SUITE - TOP PRIORITY
    SubmenuIndexTeslaSecuritySuite,
    
    // ELON'S ALL CAR BRANDS SECURITY SUITE - SUPERIORITY
    SubmenuIndexCarBrandSecuritySuite,
    
    // ELON'S USER-FRIENDLY FLIPPER ZERO UI
    SubmenuIndexUserFriendlyUI,
    
    // UNIVERSAL CAR HACKER - STANDALONE APP
    SubmenuIndexUniversalCarHacker,
    
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
    
    // Advanced menu categories
    SubmenuIndexAdvancedRF,
    SubmenuIndexAdvancedNFC,
    SubmenuIndexAdvancedWiFi,
    SubmenuIndexAdvancedBT,
    SubmenuIndexAutomotiveAdvanced,
    SubmenuIndexInfrared,
    SubmenuIndexUSBAttacks,
    SubmenuIndexNetworkAttacks,
    SubmenuIndexCryptoTools,
    
    // Advanced RF/SubGHz indexes
    SubmenuIndexSpectrumAnalyzer,
    SubmenuIndexProtocolAnalyzer,
    SubmenuIndexSignalReplay,
    SubmenuIndexFrequencyJammer,
    SubmenuIndexTPMS,
    SubmenuIndexLoRa,
    SubmenuIndexZigbee,
    SubmenuIndexZWave,
    
    // Advanced NFC indexes
    SubmenuIndexMifareNested,
    SubmenuIndexMifareHardnested,
    SubmenuIndexMifareDarkside,
    SubmenuIndexDESFire,
    SubmenuIndexNTAGCrack,
    SubmenuIndexEMVRelay,
    SubmenuIndexHIDiCLASS,
    
    // Advanced WiFi indexes
    SubmenuIndexWPA3,
    SubmenuIndexWPSPixie,
    SubmenuIndexKARMA,
    SubmenuIndexMANA,
    SubmenuIndexBeaconFlood,
    SubmenuIndexHiddenSSID,
    SubmenuIndexRogueAP,
    SubmenuIndexClientTrack,
    
    // Advanced Bluetooth indexes
    SubmenuIndexBLEGATTFuzz,
    SubmenuIndexBLEMITM,
    SubmenuIndexBLERelay,
    SubmenuIndexBLEPairingBypass,
    SubmenuIndexBLEHIDInject,
    SubmenuIndexBluebug,
    SubmenuIndexBluesnarf,
    SubmenuIndexBTFingerprint,
    
    // Automotive advanced indexes
    SubmenuIndexCANSniffer,
    SubmenuIndexCANFuzz,
    SubmenuIndexCANReplay,
    SubmenuIndexUDS,
    SubmenuIndexOBD,
    SubmenuIndexTPMSSpoof,
    SubmenuIndexImmobilizer,
    SubmenuIndexECUIdent,
    
    // Infrared indexes
    SubmenuIndexIRTVPower,
    SubmenuIndexIRAC,
    SubmenuIndexIRProjector,
    SubmenuIndexIRCapture,
    SubmenuIndexIRReplay,
    SubmenuIndexIRDatabase,
    
    // USB attack indexes
    SubmenuIndexRubberDucky,
    SubmenuIndexBadUSB,
    SubmenuIndexHIDKeyboard,
    SubmenuIndexHIDMouse,
    SubmenuIndexMassStorage,
    SubmenuIndexSerialFuzz,
    
    // Network attack indexes
    SubmenuIndexARPSpoof,
    SubmenuIndexDNSSpoof,
    SubmenuIndexMITM,
    SubmenuIndexSSLStrip,
    SubmenuIndexPortScan,
    SubmenuIndexPacketCapture,
    
    // Crypto tool indexes
    SubmenuIndexEntropyAnalysis,
    SubmenuIndexFreqAnalysis,
    SubmenuIndexXORDetect,
    SubmenuIndexBruteForce,
    SubmenuIndexDictAttack,
    SubmenuIndexHashCalc,
} SubmenuIndex;
