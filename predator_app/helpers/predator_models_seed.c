#include "predator_models_seed.h"
#include "predator_models.h"
#include <furi.h>
#include <storage/storage.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static const char* kBrands[] = {
    "Toyota","Volkswagen","Ford","Hyundai","Honda","Chevrolet","Nissan","Kia",
    "Mercedes","BMW","Audi","Renault","Peugeot","Skoda","Volvo","Mazda",
    "Subaru","Mitsubishi","Jeep","Chrysler","Dodge","Cadillac","Lexus",
    "Infiniti","Acura","Porsche","Range Rover","Jaguar","Fiat","Seat",
    "Opel","Vauxhall","Mini","Land Rover","GMC","Ram","Buick","Citroen",
    "Suzuki","Geely","BYD","SAIC","Changan","Great Wall","Škoda","Genesis",
    "Tesla","Bugatti","McLaren","Aston Martin","Bentley","Rolls Royce","Maserati","Ferrari"
};

static const char* kModelCores[] = {
    "Sedan","Hatchback","Wagon","SUV","Crossover","Coupe","Roadster","Pickup",
    "Van","Sport","Touring","Deluxe","Premium","Classic","Hybrid","EV",
    "GT","Limited","Signature","Plus","Pro","SE","LE","RS","XR","LX","EX","GL","GLS","GTS"
};

static const char* kRemoteTypes[] = { "Fixed Code","Rolling Code","Smart Key" };

static uint32_t pick_brand_frequency(const char* brand) {
    if(!strcmp(brand, "Ford") || !strcmp(brand, "Chevrolet") || !strcmp(brand, "Cadillac") ||
       !strcmp(brand, "Dodge") || !strcmp(brand, "Chrysler") || !strcmp(brand, "GMC") ||
       !strcmp(brand, "Ram") || !strcmp(brand, "Buick") || !strcmp(brand, "Jeep")) {
        return 315000000U;
    }
    if(!strcmp(brand, "Audi") || !strcmp(brand, "BMW") || !strcmp(brand, "Mercedes") ||
       !strcmp(brand, "Porsche") || !strcmp(brand, "Jaguar") || !strcmp(brand, "Range Rover") ||
       !strcmp(brand, "Volvo")) {
        return 868350000U;
    }
    return 433920000U;
}

static bool ensure_dir(Storage* storage, const char* path) {
    FS_Error st = storage_common_mkdir(storage, path);
    return (st == FSE_OK || st == FSE_EXIST);
}

size_t predator_models_ensure_seed(Storage* storage, size_t min_count) {
    if(!storage) return 0;

    const char* dir1 = "/ext/apps_data";
    const char* dir2 = "/ext/apps_data/predator";
    const char* path = "/ext/apps_data/predator/car_models.csv";

    ensure_dir(storage, dir1);
    ensure_dir(storage, dir2);

    File* file = storage_file_alloc(storage);
    size_t existing = 0;
    if(storage_file_exists(storage, path) && storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        char buf[256];
        while(existing < min_count) {
            uint16_t rd = storage_file_read(file, buf, sizeof(buf));
            if(rd == 0) break;
            for(uint16_t i = 0; i < rd; i++) if(buf[i] == '\n') existing++;
        }
        storage_file_close(file);
        if(existing >= min_count) {
            storage_file_free(file);
            return existing;
        }
    }

    if(!storage_file_open(file, path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        storage_file_free(file);
        return 0;
    }

    const size_t brand_count = sizeof(kBrands)/sizeof(kBrands[0]);
    const size_t model_count = sizeof(kModelCores)/sizeof(kModelCores[0]);
    const size_t type_count = sizeof(kRemoteTypes)/sizeof(kRemoteTypes[0]);

    char line[256];
    size_t written = 0;
    uint16_t year_start = 2016;

    for(size_t b = 0; b < brand_count && written < min_count; b++) {
        const char* brand = kBrands[b];
        uint32_t base_freq = pick_brand_frequency(brand);

        for(size_t m = 0; m < model_count && written < min_count; m++) {
            const char* core = kModelCores[m];
            for(uint16_t year = year_start; year <= 2025 && written < min_count; year++) {
                uint32_t freq = base_freq;
                if((year % 5) == 0 && base_freq == 433920000U) freq = 315000000U;
                if((year % 7) == 0 && (base_freq == 433920000U || base_freq == 315000000U)) freq = 868350000U;
                const char* rtype = kRemoteTypes[(year + m) % type_count];
                char modelname[64];
                snprintf(modelname, sizeof(modelname), "%s %u+", core, (unsigned)year);
                int n = snprintf(line, sizeof(line), "%s,%s,%lu,%s\n",
                                  brand, modelname, (unsigned long)freq, rtype);
                if(n > 0) {
                    storage_file_write(file, line, (uint16_t)n);
                    written++;
                }
            }
        }
    }

    storage_file_close(file);
    storage_file_free(file);

    return written;
}
