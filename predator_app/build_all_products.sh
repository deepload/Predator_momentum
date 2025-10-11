#!/bin/bash

echo "========================================"
echo "PREDATOR MOMENTUM - COMPLETE PRODUCT SUITE"
echo "Building 11 Specialized Security Apps"
echo "========================================"
echo

# Clean previous builds
echo "[1/11] Cleaning previous builds..."
rm -rf build
rm -f *.fap

echo
echo "========================================"
echo "BUILDING GOVERNMENT SUITE"
echo "========================================"

# 1. Government App (ONU Ready)
echo "[1/11] Building Predator Government (ONU Presentation)..."
cp application_government.fam application.fam
ufbt build
if [ -f "build/predator_gov.fap" ]; then
    cp build/predator_gov.fap predator_government.fap
    echo "✅ SUCCESS: predator_government.fap"
else
    echo "❌ FAILED: predator_government.fap"
fi

# 2. WiFi App
echo "[2/11] Building Predator WiFi..."
cp application_wifi.fam application.fam
ufbt build
if [ -f "build/predator_wifi.fap" ]; then
    cp build/predator_wifi.fap predator_wifi.fap
    echo "✅ SUCCESS: predator_wifi.fap"
else
    echo "❌ FAILED: predator_wifi.fap"
fi

# 3. Car App
echo "[3/11] Building Predator Car..."
cp application_car.fam application.fam
ufbt build
if [ -f "build/predator_car.fap" ]; then
    cp build/predator_car.fap predator_car.fap
    echo "✅ SUCCESS: predator_car.fap"
else
    echo "❌ FAILED: predator_car.fap"
fi

# 4. Tesla App
echo "[4/11] Building Predator Tesla..."
cp application_tesla.fam application.fam
ufbt build
if [ -f "build/predator_tesla.fap" ]; then
    cp build/predator_tesla.fap predator_tesla.fap
    echo "✅ SUCCESS: predator_tesla.fap"
else
    echo "❌ FAILED: predator_tesla.fap"
fi

echo
echo "========================================"
echo "BUILDING SPECIALIZED SUITE"
echo "========================================"

# 5. Bluetooth App
echo "[5/11] Building Predator Bluetooth..."
cp application_bluetooth.fam application.fam
ufbt build
if [ -f "build/predator_bluetooth.fap" ]; then
    cp build/predator_bluetooth.fap predator_bluetooth.fap
    echo "✅ SUCCESS: predator_bluetooth.fap"
else
    echo "❌ FAILED: predator_bluetooth.fap"
fi

# 6. RFID App
echo "[6/11] Building Predator RFID..."
cp application_rfid.fam application.fam
ufbt build
if [ -f "build/predator_rfid.fap" ]; then
    cp build/predator_rfid.fap predator_rfid.fap
    echo "✅ SUCCESS: predator_rfid.fap"
else
    echo "❌ FAILED: predator_rfid.fap"
fi

# 7. SubGHz App
echo "[7/11] Building Predator SubGHz..."
cp application_subghz.fam application.fam
ufbt build
if [ -f "build/predator_subghz.fap" ]; then
    cp build/predator_subghz.fap predator_subghz.fap
    echo "✅ SUCCESS: predator_subghz.fap"
else
    echo "❌ FAILED: predator_subghz.fap"
fi

# 8. Infrastructure App
echo "[8/11] Building Predator Infrastructure..."
cp application_infrastructure.fam application.fam
ufbt build
if [ -f "build/predator_infra.fap" ]; then
    cp build/predator_infra.fap predator_infrastructure.fap
    echo "✅ SUCCESS: predator_infrastructure.fap"
else
    echo "❌ FAILED: predator_infrastructure.fap"
fi

echo
echo "========================================"
echo "BUILDING ENTERPRISE SUITE"
echo "========================================"

# 9. Casino App
echo "[9/11] Building Predator Casino..."
cp application_casino.fam application.fam
ufbt build
if [ -f "build/predator_casino.fap" ]; then
    cp build/predator_casino.fap predator_casino.fap
    echo "✅ SUCCESS: predator_casino.fap"
else
    echo "❌ FAILED: predator_casino.fap"
fi

# 10. Network App
echo "[10/11] Building Predator Network..."
cp application_network.fam application.fam
ufbt build
if [ -f "build/predator_network.fap" ]; then
    cp build/predator_network.fap predator_network.fap
    echo "✅ SUCCESS: predator_network.fap"
else
    echo "❌ FAILED: predator_network.fap"
fi

# 11. Crypto App
echo "[11/11] Building Predator Crypto..."
cp application_crypto.fam application.fam
ufbt build
if [ -f "build/predator_crypto.fap" ]; then
    cp build/predator_crypto.fap predator_crypto.fap
    echo "✅ SUCCESS: predator_crypto.fap"
else
    echo "❌ FAILED: predator_crypto.fap"
fi

echo
echo "========================================"
echo "BUILD SUMMARY"
echo "========================================"
echo

echo "GOVERNMENT SUITE:"
[ -f "predator_government.fap" ] && echo "✅ predator_government.fap - ONU Ready" || echo "❌ predator_government.fap - FAILED"

echo
echo "CORE ATTACK SUITE:"
[ -f "predator_wifi.fap" ] && echo "✅ predator_wifi.fap" || echo "❌ predator_wifi.fap - FAILED"
[ -f "predator_car.fap" ] && echo "✅ predator_car.fap" || echo "❌ predator_car.fap - FAILED"
[ -f "predator_tesla.fap" ] && echo "✅ predator_tesla.fap" || echo "❌ predator_tesla.fap - FAILED"
[ -f "predator_bluetooth.fap" ] && echo "✅ predator_bluetooth.fap" || echo "❌ predator_bluetooth.fap - FAILED"
[ -f "predator_rfid.fap" ] && echo "✅ predator_rfid.fap" || echo "❌ predator_rfid.fap - FAILED"
[ -f "predator_subghz.fap" ] && echo "✅ predator_subghz.fap" || echo "❌ predator_subghz.fap - FAILED"

echo
echo "ENTERPRISE SUITE:"
[ -f "predator_infrastructure.fap" ] && echo "✅ predator_infrastructure.fap" || echo "❌ predator_infrastructure.fap - FAILED"
[ -f "predator_casino.fap" ] && echo "✅ predator_casino.fap" || echo "❌ predator_casino.fap - FAILED"
[ -f "predator_network.fap" ] && echo "✅ predator_network.fap" || echo "❌ predator_network.fap - FAILED"
[ -f "predator_crypto.fap" ] && echo "✅ predator_crypto.fap" || echo "❌ predator_crypto.fap - FAILED"

echo
echo "========================================"
echo "DEPLOYMENT READY"
echo "========================================"
echo
echo "🏆 PREDATOR MOMENTUM - COMPLETE PRODUCT SUITE"
echo "📦 11 Specialized Security Applications Built"
echo "🎯 Ready for Market Domination"
echo "🏛️ ONU Presentation: predator_government.fap"
echo
echo "Copy all predator_*.fap files to Flipper Zero /apps/Tools/"
echo
