#pragma once

#include <stdbool.h>
#include <stdint.h>

// Forward declare core app to avoid circular dependency
struct PredatorApp;

typedef enum {
    // MAJOR GOVERNMENT MARKETS
    PredatorRegionUS,           // United States
    PredatorRegionEU,           // European Union
    PredatorRegionCH,           // Switzerland
    PredatorRegionJP,           // Japan
    PredatorRegionCN,           // China
    PredatorRegionCA,           // Canada
    PredatorRegionAU,           // Australia
    PredatorRegionUK,           // United Kingdom
    PredatorRegionDE,           // Germany
    PredatorRegionFR,           // France
    PredatorRegionIT,           // Italy
    PredatorRegionES,           // Spain
    PredatorRegionNL,           // Netherlands
    PredatorRegionSE,           // Sweden
    PredatorRegionNO,           // Norway
    PredatorRegionDK,           // Denmark
    PredatorRegionFI,           // Finland
    PredatorRegionBE,           // Belgium
    PredatorRegionAT,           // Austria
    PredatorRegionPL,           // Poland
    PredatorRegionCZ,           // Czech Republic
    PredatorRegionHU,           // Hungary
    PredatorRegionRO,           // Romania
    PredatorRegionBG,           // Bulgaria
    PredatorRegionGR,           // Greece
    PredatorRegionPT,           // Portugal
    PredatorRegionIE,           // Ireland
    PredatorRegionLU,           // Luxembourg
    PredatorRegionMT,           // Malta
    PredatorRegionCY,           // Cyprus
    PredatorRegionSI,           // Slovenia
    PredatorRegionSK,           // Slovakia
    PredatorRegionLV,           // Latvia
    PredatorRegionLT,           // Lithuania
    PredatorRegionEE,           // Estonia
    PredatorRegionHR,           // Croatia
    PredatorRegionRS,           // Serbia
    PredatorRegionBA,           // Bosnia and Herzegovina
    PredatorRegionME,           // Montenegro
    PredatorRegionMK,           // North Macedonia
    PredatorRegionAL,           // Albania
    PredatorRegionKS,           // Kosovo
    PredatorRegionMD,           // Moldova
    PredatorRegionUA,           // Ukraine
    PredatorRegionBY,           // Belarus
    PredatorRegionRU,           // Russia
    PredatorRegionKZ,           // Kazakhstan
    PredatorRegionUZ,           // Uzbekistan
    PredatorRegionKG,           // Kyrgyzstan
    PredatorRegionTJ,           // Tajikistan
    PredatorRegionTM,           // Turkmenistan
    PredatorRegionAZ,           // Azerbaijan
    PredatorRegionAM,           // Armenia
    PredatorRegionGE,           // Georgia
    PredatorRegionTR,           // Turkey
    PredatorRegionIL,           // Israel
    PredatorRegionSA,           // Saudi Arabia
    PredatorRegionAE,           // United Arab Emirates
    PredatorRegionQA,           // Qatar
    PredatorRegionKW,           // Kuwait
    PredatorRegionBH,           // Bahrain
    PredatorRegionOM,           // Oman
    PredatorRegionJO,           // Jordan
    PredatorRegionLB,           // Lebanon
    PredatorRegionSY,           // Syria
    PredatorRegionIQ,           // Iraq
    PredatorRegionIR,           // Iran
    PredatorRegionAF,           // Afghanistan
    PredatorRegionPK,           // Pakistan
    PredatorRegionIN,           // India
    PredatorRegionBD,           // Bangladesh
    PredatorRegionLK,           // Sri Lanka
    PredatorRegionMV,           // Maldives
    PredatorRegionNP,           // Nepal
    PredatorRegionBT,           // Bhutan
    PredatorRegionMM,           // Myanmar
    PredatorRegionTH,           // Thailand
    PredatorRegionLA,           // Laos
    PredatorRegionKH,           // Cambodia
    PredatorRegionVN,           // Vietnam
    PredatorRegionMY,           // Malaysia
    PredatorRegionSG,           // Singapore
    PredatorRegionBN,           // Brunei
    PredatorRegionID,           // Indonesia
    PredatorRegionTL,           // East Timor
    PredatorRegionPH,           // Philippines
    PredatorRegionTW,           // Taiwan
    PredatorRegionKR,           // South Korea
    PredatorRegionKP,           // North Korea
    PredatorRegionMN,           // Mongolia
    PredatorRegionNZ,           // New Zealand
    PredatorRegionFJ,           // Fiji
    PredatorRegionPG,           // Papua New Guinea
    PredatorRegionSB,           // Solomon Islands
    PredatorRegionVU,           // Vanuatu
    PredatorRegionNC,           // New Caledonia
    PredatorRegionPF,           // French Polynesia
    PredatorRegionWS,           // Samoa
    PredatorRegionTO,           // Tonga
    PredatorRegionKI,           // Kiribati
    PredatorRegionTV,           // Tuvalu
    PredatorRegionNR,           // Nauru
    PredatorRegionPW,           // Palau
    PredatorRegionFM,           // Micronesia
    PredatorRegionMH,           // Marshall Islands
    PredatorRegionMX,           // Mexico
    PredatorRegionGT,           // Guatemala
    PredatorRegionBZ,           // Belize
    PredatorRegionSV,           // El Salvador
    PredatorRegionHN,           // Honduras
    PredatorRegionNI,           // Nicaragua
    PredatorRegionCR,           // Costa Rica
    PredatorRegionPA,           // Panama
    PredatorRegionCU,           // Cuba
    PredatorRegionJM,           // Jamaica
    PredatorRegionHT,           // Haiti
    PredatorRegionDO,           // Dominican Republic
    PredatorRegionPR,           // Puerto Rico
    PredatorRegionTT,           // Trinidad and Tobago
    PredatorRegionBB,           // Barbados
    PredatorRegionGD,           // Grenada
    PredatorRegionVC,           // Saint Vincent and the Grenadines
    PredatorRegionLC,           // Saint Lucia
    PredatorRegionDM,           // Dominica
    PredatorRegionAG,           // Antigua and Barbuda
    PredatorRegionKN,           // Saint Kitts and Nevis
    PredatorRegionBS,           // Bahamas
    PredatorRegionCO,           // Colombia
    PredatorRegionVE,           // Venezuela
    PredatorRegionGY,           // Guyana
    PredatorRegionSR,           // Suriname
    PredatorRegionGF,           // French Guiana
    PredatorRegionBR,           // Brazil
    PredatorRegionEC,           // Ecuador
    PredatorRegionPE,           // Peru
    PredatorRegionBO,           // Bolivia
    PredatorRegionPY,           // Paraguay
    PredatorRegionUY,           // Uruguay
    PredatorRegionAR,           // Argentina
    PredatorRegionCL,           // Chile
    PredatorRegionFK,           // Falkland Islands
    PredatorRegionEG,           // Egypt
    PredatorRegionLY,           // Libya
    PredatorRegionTN,           // Tunisia
    PredatorRegionDZ,           // Algeria
    PredatorRegionMA,           // Morocco
    PredatorRegionEH,           // Western Sahara
    PredatorRegionMR,           // Mauritania
    PredatorRegionML,           // Mali
    PredatorRegionBF,           // Burkina Faso
    PredatorRegionNE,           // Niger
    PredatorRegionTD,           // Chad
    PredatorRegionSD,           // Sudan
    PredatorRegionSS,           // South Sudan
    PredatorRegionET,           // Ethiopia
    PredatorRegionER,           // Eritrea
    PredatorRegionDJ,           // Djibouti
    PredatorRegionSO,           // Somalia
    PredatorRegionKE,           // Kenya
    PredatorRegionUG,           // Uganda
    PredatorRegionRW,           // Rwanda
    PredatorRegionBI,           // Burundi
    PredatorRegionTZ,           // Tanzania
    PredatorRegionMZ,           // Mozambique
    PredatorRegionMW,           // Malawi
    PredatorRegionZM,           // Zambia
    PredatorRegionZW,           // Zimbabwe
    PredatorRegionBW,           // Botswana
    PredatorRegionNA,           // Namibia
    PredatorRegionZA,           // South Africa
    PredatorRegionLS,           // Lesotho
    PredatorRegionSZ,           // Eswatini
    PredatorRegionMG,           // Madagascar
    PredatorRegionMU,           // Mauritius
    PredatorRegionSC,           // Seychelles
    PredatorRegionKM,           // Comoros
    PredatorRegionYT,           // Mayotte
    PredatorRegionRE,           // Réunion
    PredatorRegionSH,           // Saint Helena
    PredatorRegionAO,           // Angola
    PredatorRegionCD,           // Democratic Republic of the Congo
    PredatorRegionCG,           // Republic of the Congo
    PredatorRegionCF,           // Central African Republic
    PredatorRegionCM,           // Cameroon
    PredatorRegionGQ,           // Equatorial Guinea
    PredatorRegionST,           // São Tomé and Príncipe
    PredatorRegionGA,           // Gabon
    PredatorRegionGH,           // Ghana
    PredatorRegionTG,           // Togo
    PredatorRegionBJ,           // Benin
    PredatorRegionNG,           // Nigeria
    PredatorRegionSN,           // Senegal
    PredatorRegionGM,           // Gambia
    PredatorRegionGW,           // Guinea-Bissau
    PredatorRegionGN,           // Guinea
    PredatorRegionSL,           // Sierra Leone
    PredatorRegionLR,           // Liberia
    PredatorRegionCI,           // Côte d'Ivoire
    PredatorRegionCV,           // Cape Verde
    PredatorRegionAuto,         // Auto-detect
    PredatorRegionUnblock,      // Unrestricted (Government/Professional)
} PredatorRegion;

typedef enum {
    PredatorFeatureUnknown = 0,
    PredatorFeatureSubGhz315Tx,
    PredatorFeatureSubGhz433Tx,
    PredatorFeatureSubGhz868Tx,
    PredatorFeatureSubGhz915Tx,
    PredatorFeatureWifiScan,
    PredatorFeatureWifiDeauth,
    PredatorFeatureWifiEvilTwin,
    PredatorFeatureBleScan,
    PredatorFeatureBleSpam,
    PredatorFeatureCarTeslaCharge,
    PredatorFeatureCarJamming,
    PredatorFeatureRfidClone,
    PredatorFeatureRfidBruteforce,
    PredatorFeatureGpsTracker,
    PredatorFeatureWardriving,
} PredatorFeature;

// Initialize compliance engine (detect region, load settings). Safe to call multiple times.
void predator_compliance_init(struct PredatorApp* app);

// Set/get region (manual override)
void predator_compliance_set_region(struct PredatorApp* app, PredatorRegion region);
PredatorRegion predator_compliance_get_region(struct PredatorApp* app);

// Return a short region code string
const char* predator_compliance_region_str(PredatorRegion region);

// Check if a feature is allowed to run live given region and authorization.
// Returns true if live operation is allowed; false means restrict to simulation/demonstration.
bool predator_compliance_is_feature_allowed(struct PredatorApp* app, PredatorFeature feature, bool authorized);
