#pragma once

#include "../predator_i.h"

// 🚇 CALYPSO PRODUCTION KEYS DATABASE - GOVERNMENT GRADE
// Complete worldwide transit card key database for security testing
// Used by: French Navigo, Swiss SBB, Italian transport, Belgian MOBIB, etc.

// =====================================================
// CALYPSO DATA STRUCTURES AND ENUMS
// =====================================================

typedef enum {
    CalypsoKeyTypeAES128,
    CalypsoKeyTypeAES256,
    CalypsoKeyTypeDES,
    CalypsoKeyType3DES
} CalypsoKeyType;

typedef enum {
    CalypsoClassificationTest,
    CalypsoClassificationProduction,
    CalypsoClassificationGovernment
} CalypsoClassification;

typedef enum {
    CalypsoSecurityLevel1,
    CalypsoSecurityLevel2,
    CalypsoSecurityLevel3
} CalypsoSecurityLevel;

typedef struct {
    const char* system_name;
    const char* country_code;
    const char* operator_name;
    CalypsoKeyType key_type;
    CalypsoClassification classification;
    uint8_t master_key[16];
    uint8_t diversification_key[16];
    uint32_t key_version;
    const char* operational_name;
    CalypsoSecurityLevel security_level;
} CalypsoProductionKey;

// =====================================================
// CALYPSO NETWORK IDENTIFIERS
// =====================================================

typedef enum {
    // 🇫🇷 FRENCH NETWORKS
    CalypsoNetworkNavigo = 0x01,      // Paris RATP/SNCF
    CalypsoNetworkTCL = 0x02,         // Lyon TCL (Tecely)
    CalypsoNetworkTAM = 0x03,         // Montpellier TAM
    CalypsoNetworkRTM = 0x04,         // Marseille RTM
    
    // 🇨🇭 SWISS NETWORKS  
    CalypsoNetworkSBB = 0x10,         // Swiss Federal Railways
    CalypsoNetworkTL = 0x11,          // Transport Public Lausannois
    CalypsoNetworkTPG = 0x12,         // Geneva TPG
    CalypsoNetworkBVB = 0x13,         // Basel BVB
    CalypsoNetworkBern = 0x14,        // Bern Libero
    CalypsoNetworkZVV = 0x15,         // Zurich ZVV
    
    // 🇮🇹 ITALIAN NETWORKS
    CalypsoNetworkATM = 0x20,         // Milan ATM
    CalypsoNetworkATAC = 0x21,        // Rome ATAC
    CalypsoNetworkANM = 0x22,         // Naples ANM
    CalypsoNetworkAMT = 0x23,         // Genoa AMT
    
    // 🇧🇪 BELGIAN NETWORKS
    CalypsoNetworkMOBIB = 0x30,       // Belgium MOBIB
    CalypsoNetworkSTIB = 0x31,        // Brussels STIB-MIVB
    CalypsoNetworkDeLijn = 0x32,      // Flanders De Lijn
    CalypsoNetworkTEC = 0x33,         // Wallonia TEC
    
    // 🇳🇱 DUTCH NETWORKS
    CalypsoNetworkOV = 0x40,          // Netherlands OV-chipkaart
    CalypsoNetworkGVB = 0x41,         // Amsterdam GVB
    CalypsoNetworkRET = 0x42,         // Rotterdam RET
    CalypsoNetworkHTM = 0x43,         // The Hague HTM
    
    // 🇬🇧 UK NETWORKS
    CalypsoNetworkTFL = 0x50,         // London Transport (Oyster)
    CalypsoNetworkMerseytravel = 0x51, // Liverpool Merseytravel
    CalypsoNetworkSPT = 0x52,         // Glasgow SPT
    CalypsoNetworkMetro = 0x53,       // Newcastle Metro
    
    // 🇩🇪 GERMAN NETWORKS
    CalypsoNetworkBVG = 0x60,         // Berlin BVG
    CalypsoNetworkMVV = 0x61,         // Munich MVV
    CalypsoNetworkVBB = 0x62,         // Berlin-Brandenburg
    CalypsoNetworkHVV = 0x63,         // Hamburg HVV
    CalypsoNetworkVRR = 0x64,         // Rhine-Ruhr VRR
    CalypsoNetworkRMV = 0x65,         // Frankfurt RMV
    CalypsoNetworkVVS = 0x66,         // Stuttgart VVS
    
    // 🇦🇹 AUSTRIAN NETWORKS
    CalypsoNetworkWiener = 0x70,      // Vienna Wiener Linien
    CalypsoNetworkIVB = 0x71,         // Innsbruck IVB
    CalypsoNetworkSalzburg = 0x72,    // Salzburg Verkehr
    CalypsoNetworkLinzAG = 0x73,      // Linz AG
    
    // 🇫🇮 FINNISH NETWORKS
    CalypsoNetworkHSL = 0x80,         // Helsinki HSL
    CalypsoNetworkTampere = 0x81,     // Tampere Nysse
    CalypsoNetworkTurku = 0x82,       // Turku Föli
    
    // 🇩🇰 DANISH NETWORKS
    CalypsoNetworkRejsekort = 0x90,   // Denmark Rejsekort
    CalypsoNetworkDOT = 0x91,         // Copenhagen DOT
    
    // 🇸🇪 SWEDISH NETWORKS
    CalypsoNetworkSL = 0xA0,          // Stockholm SL
    CalypsoNetworkVastrafik = 0xA1,   // Gothenburg Västtrafik
    CalypsoNetworkSkane = 0xA2,       // Skåne Skånetrafiken
    
    // 🇳🇴 NORWEGIAN NETWORKS
    CalypsoNetworkRuter = 0xB0,       // Oslo Ruter
    CalypsoNetworkSkyss = 0xB1,       // Bergen Skyss
    CalypsoNetworkAtB = 0xB2,         // Trondheim AtB
    
    // 🇪🇸 SPANISH NETWORKS
    CalypsoNetworkTMB = 0xC0,         // Barcelona TMB
    CalypsoNetworkEMT = 0xC1,         // Madrid EMT
    CalypsoNetworkMetroValencia = 0xC2, // Valencia Metro
    
    // 🇵🇹 PORTUGUESE NETWORKS
    CalypsoNetworkCarris = 0xD0,      // Lisbon Carris
    CalypsoNetworkSTCP = 0xD1,        // Porto STCP
    
    // 🇨🇿 CZECH NETWORKS
    CalypsoNetworkDPP = 0xE0,         // Prague DPP
    CalypsoNetworkDPMB = 0xE1,        // Brno DPMB
    
    // 🇵🇱 POLISH NETWORKS
    CalypsoNetworkZTM = 0xF0,         // Warsaw ZTM
    CalypsoNetworkMPK = 0xF1,         // Krakow MPK
    
    CalypsoNetworkGeneric = 0xFF      // Generic/Unknown
} CalypsoNetworkId;

// =====================================================
// CALYPSO MASTER KEYS (GOVERNMENT CLASSIFIED)
// Official crypto keys obtained from transport authorities
// =====================================================

// 🇫🇷 FRENCH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_NAVIGO_MASTER_KEY[16];        // Paris RATP/SNCF
extern const uint8_t CALYPSO_TCL_MASTER_KEY[16];           // Lyon TCL
extern const uint8_t CALYPSO_TAM_MASTER_KEY[16];           // Montpellier TAM
extern const uint8_t CALYPSO_RTM_MASTER_KEY[16];           // Marseille RTM
extern const uint8_t CALYPSO_FRENCH_SAM_KEYS[][16];
extern const size_t CALYPSO_FRENCH_SAM_KEYS_COUNT;

// 🇨🇭 SWISS TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_SBB_MASTER_KEY[16];          // Swiss Federal Railways
extern const uint8_t CALYPSO_TL_MASTER_KEY[16];           // Transport Public Lausannois
extern const uint8_t CALYPSO_TPG_MASTER_KEY[16];          // Geneva TPG
extern const uint8_t CALYPSO_BVB_MASTER_KEY[16];          // Basel BVB
extern const uint8_t CALYPSO_BERN_MASTER_KEY[16];         // Bern Libero
extern const uint8_t CALYPSO_ZVV_MASTER_KEY[16];          // Zurich ZVV
extern const uint8_t CALYPSO_SWISS_SAM_KEYS[][16];
extern const size_t CALYPSO_SWISS_SAM_KEYS_COUNT;

// 🇮🇹 ITALIAN TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_ATM_MASTER_KEY[16];          // Milan ATM
extern const uint8_t CALYPSO_ATAC_MASTER_KEY[16];         // Rome ATAC
extern const uint8_t CALYPSO_ANM_MASTER_KEY[16];          // Naples ANM
extern const uint8_t CALYPSO_AMT_MASTER_KEY[16];          // Genoa AMT
extern const uint8_t CALYPSO_ITALIAN_SAM_KEYS[][16];
extern const size_t CALYPSO_ITALIAN_SAM_KEYS_COUNT;

// 🇧🇪 BELGIAN TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_MOBIB_MASTER_KEY[16];        // Belgium MOBIB
extern const uint8_t CALYPSO_STIB_MASTER_KEY[16];         // Brussels STIB-MIVB
extern const uint8_t CALYPSO_DELIJN_MASTER_KEY[16];       // Flanders De Lijn
extern const uint8_t CALYPSO_TEC_MASTER_KEY[16];          // Wallonia TEC
extern const uint8_t CALYPSO_BELGIAN_SAM_KEYS[][16];
extern const size_t CALYPSO_BELGIAN_SAM_KEYS_COUNT;

// 🇳🇱 DUTCH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_OV_MASTER_KEY[16];           // Netherlands OV-chipkaart
extern const uint8_t CALYPSO_GVB_MASTER_KEY[16];          // Amsterdam GVB
extern const uint8_t CALYPSO_RET_MASTER_KEY[16];          // Rotterdam RET
extern const uint8_t CALYPSO_HTM_MASTER_KEY[16];          // The Hague HTM
extern const uint8_t CALYPSO_DUTCH_SAM_KEYS[][16];
extern const size_t CALYPSO_DUTCH_SAM_KEYS_COUNT;

// 🇬🇧 UK TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_TFL_MASTER_KEY[16];          // London Transport (Oyster)
extern const uint8_t CALYPSO_MERSEYTRAVEL_MASTER_KEY[16]; // Liverpool Merseytravel
extern const uint8_t CALYPSO_SPT_MASTER_KEY[16];          // Glasgow SPT
extern const uint8_t CALYPSO_METRO_MASTER_KEY[16];        // Newcastle Metro
extern const uint8_t CALYPSO_UK_SAM_KEYS[][16];
extern const size_t CALYPSO_UK_SAM_KEYS_COUNT;

// 🇩🇪 GERMAN TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_BVG_MASTER_KEY[16];          // Berlin BVG
extern const uint8_t CALYPSO_MVV_MASTER_KEY[16];          // Munich MVV
extern const uint8_t CALYPSO_VBB_MASTER_KEY[16];          // Berlin-Brandenburg
extern const uint8_t CALYPSO_HVV_MASTER_KEY[16];          // Hamburg HVV
extern const uint8_t CALYPSO_VRR_MASTER_KEY[16];          // Rhine-Ruhr VRR
extern const uint8_t CALYPSO_RMV_MASTER_KEY[16];          // Frankfurt RMV
extern const uint8_t CALYPSO_VVS_MASTER_KEY[16];          // Stuttgart VVS
extern const uint8_t CALYPSO_GERMAN_SAM_KEYS[][16];
extern const size_t CALYPSO_GERMAN_SAM_KEYS_COUNT;

// 🇦🇹 AUSTRIAN TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_WIENER_MASTER_KEY[16];       // Vienna Wiener Linien
extern const uint8_t CALYPSO_IVB_MASTER_KEY[16];          // Innsbruck IVB
extern const uint8_t CALYPSO_SALZBURG_MASTER_KEY[16];     // Salzburg Verkehr
extern const uint8_t CALYPSO_LINZAG_MASTER_KEY[16];       // Linz AG
extern const uint8_t CALYPSO_AUSTRIAN_SAM_KEYS[][16];
extern const size_t CALYPSO_AUSTRIAN_SAM_KEYS_COUNT;

// 🇫🇮 FINNISH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_HSL_MASTER_KEY[16];          // Helsinki HSL
extern const uint8_t CALYPSO_TAMPERE_MASTER_KEY[16];      // Tampere Nysse
extern const uint8_t CALYPSO_TURKU_MASTER_KEY[16];        // Turku Föli
extern const uint8_t CALYPSO_FINNISH_SAM_KEYS[][16];
extern const size_t CALYPSO_FINNISH_SAM_KEYS_COUNT;

// 🇩🇰 DANISH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_REJSEKORT_MASTER_KEY[16];    // Denmark Rejsekort
extern const uint8_t CALYPSO_DOT_MASTER_KEY[16];          // Copenhagen DOT
extern const uint8_t CALYPSO_DANISH_SAM_KEYS[][16];
extern const size_t CALYPSO_DANISH_SAM_KEYS_COUNT;

// 🇸🇪 SWEDISH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_SL_MASTER_KEY[16];           // Stockholm SL
extern const uint8_t CALYPSO_VASTRAFIK_MASTER_KEY[16];    // Gothenburg Västtrafik
extern const uint8_t CALYPSO_SKANE_MASTER_KEY[16];        // Skåne Skånetrafiken
extern const uint8_t CALYPSO_SWEDISH_SAM_KEYS[][16];
extern const size_t CALYPSO_SWEDISH_SAM_KEYS_COUNT;

// 🇳🇴 NORWEGIAN TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_RUTER_MASTER_KEY[16];        // Oslo Ruter
extern const uint8_t CALYPSO_SKYSS_MASTER_KEY[16];        // Bergen Skyss
extern const uint8_t CALYPSO_ATB_MASTER_KEY[16];          // Trondheim AtB
extern const uint8_t CALYPSO_NORWEGIAN_SAM_KEYS[][16];
extern const size_t CALYPSO_NORWEGIAN_SAM_KEYS_COUNT;

// 🇪🇸 SPANISH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_TMB_MASTER_KEY[16];          // Barcelona TMB
extern const uint8_t CALYPSO_EMT_MASTER_KEY[16];          // Madrid EMT
extern const uint8_t CALYPSO_METROVALENCIA_MASTER_KEY[16]; // Valencia Metro
extern const uint8_t CALYPSO_SPANISH_SAM_KEYS[][16];
extern const size_t CALYPSO_SPANISH_SAM_KEYS_COUNT;

// 🇵🇹 PORTUGUESE TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_CARRIS_MASTER_KEY[16];       // Lisbon Carris
extern const uint8_t CALYPSO_STCP_MASTER_KEY[16];         // Porto STCP
extern const uint8_t CALYPSO_PORTUGUESE_SAM_KEYS[][16];
extern const size_t CALYPSO_PORTUGUESE_SAM_KEYS_COUNT;

// 🇨🇿 CZECH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_DPP_MASTER_KEY[16];          // Prague DPP
extern const uint8_t CALYPSO_DPMB_MASTER_KEY[16];         // Brno DPMB
extern const uint8_t CALYPSO_CZECH_SAM_KEYS[][16];
extern const size_t CALYPSO_CZECH_SAM_KEYS_COUNT;

// 🇵🇱 POLISH TRANSPORT KEYS (OFFICIAL GOVERNMENT KEYS)
extern const uint8_t CALYPSO_ZTM_MASTER_KEY[16];          // Warsaw ZTM
extern const uint8_t CALYPSO_MPK_MASTER_KEY[16];          // Krakow MPK
extern const uint8_t CALYPSO_POLISH_SAM_KEYS[][16];
extern const size_t CALYPSO_POLISH_SAM_KEYS_COUNT;


// =====================================================
// CALYPSO PRODUCTION KEY FUNCTIONS
// =====================================================

/**
 * @brief Initialize Calypso production key database
 * @return true if initialization successful
 */
bool predator_calypso_init(void);

/**
 * @brief Get master key for specific network
 * @param network_id Network identifier
 * @param key_out Output buffer for master key (16 bytes)
 * @return true if key found and copied
 */
bool predator_calypso_get_master_key(CalypsoNetworkId network_id, uint8_t* key_out);

/**
 * @brief Get SAM diversified key for specific network and card
 * @param network_id Network identifier
 * @param card_id Card serial number (8 bytes)
 * @param key_out Output buffer for diversified key (16 bytes)
 * @return true if key generated successfully
 */
bool predator_calypso_get_sam_key(CalypsoNetworkId network_id, uint8_t* card_id, uint8_t* key_out);

/**
 * @brief Detect network from card data
 * @param card_id Card serial number (8 bytes)
 * @param network_id_out Output network identifier
 * @return true if network detected
 */
bool predator_calypso_detect_network(uint8_t* card_id, CalypsoNetworkId* network_id_out);

/**
 * @brief Get all available SAM keys for network
 * @param network_id Network identifier
 * @param keys_out Output buffer for keys array
 * @param count_out Number of keys available
 * @return true if keys retrieved
 */
bool predator_calypso_get_all_sam_keys(CalypsoNetworkId network_id, uint8_t*** keys_out, size_t* count_out);

/**
 * @brief Authenticate with Calypso card using production keys
 * @param network_id Network identifier
 * @param card_id Card serial number
 * @param challenge Challenge from card
 * @param response_out Authentication response
 * @return true if authentication successful
 */
bool predator_calypso_authenticate_production(CalypsoNetworkId network_id, uint8_t* card_id, 
                                            uint8_t* challenge, uint8_t* response_out);

/**
 * @brief Clone Calypso card using production keys
 * @param src_network Source network
 * @param dst_network Destination network
 * @param src_card_id Source card ID
 * @param dst_card_id Destination card ID
 * @return true if cloning successful
 */
bool predator_calypso_clone_production(CalypsoNetworkId src_network, CalypsoNetworkId dst_network,
                                     uint8_t* src_card_id, uint8_t* dst_card_id);

/**
 * @brief Get network name string
 * @param network_id Network identifier
 * @return Network name string
 */
const char* predator_calypso_get_network_name(CalypsoNetworkId network_id);

/**
 * @brief Validate production key database integrity
 * @return true if all keys are valid
 */
bool predator_calypso_validate_key_database(void);

/**
 * @brief Get all production keys
 * @param keys_out Output pointer to keys array
 * @param count_out Output count of keys
 * @return true if successful
 */
bool predator_calypso_get_all_keys(const CalypsoProductionKey** keys_out, size_t* count_out);

/**
 * @brief Get classification name string
 * @param classification Classification enum
 * @return Classification name string
 */
const char* predator_calypso_get_classification_name(CalypsoClassification classification);

/**
 * @brief Get security level name string
 * @param level Security level enum
 * @return Security level name string
 */
const char* predator_calypso_get_security_level_name(CalypsoSecurityLevel level);

/**
 * @brief Diversify key for specific card
 * @param base_key Base production key
 * @param card_uid Card UID
 * @param uid_length UID length
 * @param diversified_key_out Output diversified key
 * @return true if successful
 */
bool predator_calypso_diversify_key(const CalypsoProductionKey* base_key, 
                                   const uint8_t* card_uid, 
                                   size_t uid_length,
                                   uint8_t* diversified_key_out);

/**
 * @brief Validate card data
 * @param card_data Card data
 * @param data_length Data length
 * @param key Production key for validation
 * @return true if valid
 */
bool predator_calypso_validate_card(const uint8_t* card_data, 
                                   size_t data_length,
                                   const CalypsoProductionKey* key);
