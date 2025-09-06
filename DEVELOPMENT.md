# Predator App Development Guide

This document provides guidelines and information for developers working on or contributing to the Predator app for Flipper Zero.

## Project Structure

```
predator_app/
├── helpers/                # Hardware abstraction layer modules
│   ├── predator_esp32.*   # ESP32 communication interface
│   ├── predator_gps.*     # GPS parsing and tracking functions
│   └── predator_subghz.*  # RF signal generation for car testing
├── scenes/                # UI and interaction components
│   ├── predator_scene.h   # Scene declarations and config
│   ├── predator_scene_*.c # Individual scene implementations
├── predator.c             # Main application entry point
├── predator_i.h           # Internal app structure and definitions
├── predator_uart.h        # UART communication abstractions
└── application.fam        # Application manifest for build system
```

## Development Environment Setup

### Required Tools

- **Git**: Version control system
- **Python 3.6+**: For build scripts and tools
- **ufbt**: Flipper Zero Build Tool
- **Visual Studio Code** (recommended): With C/C++ extensions

### Initial Setup

```bash
# Install ufbt
pip install --upgrade ufbt

# Clone the repository
git clone https://your-repository-url/predator.git
cd predator

# Initialize ufbt in project
cd predator_app
ufbt init
```

## Build System

### Building with ufbt

```bash
# Build app only
ufbt

# Build with verbose output
ufbt -v

# Build and launch to a connected Flipper
ufbt launch

# Clean build artifacts
ufbt -c
```

### Integration with Firmware

To integrate into a full firmware build:

```bash
# Clone firmware repository
git clone --recursive https://github.com/Next-Flip/Momentum-Firmware.git
cd Momentum-Firmware

# Copy Predator app to applications_user
mkdir -p applications_user
cp -r /path/to/predator/predator_app applications_user/predator

# Build full firmware with Predator
./fbt COMPACT=1
```

## Coding Standards

### C Code Style

- **Indentation**: 4 spaces
- **Bracing**: Opening brace on same line as control statement
- **Naming**:
  - Functions: `lowercase_with_underscores`
  - Variables: `lowercase_with_underscores`
  - Constants: `UPPERCASE_WITH_UNDERSCORES`
  - Types/Structs: `PascalCase`
- **File Headers**: Include brief description and copyright notice

### Code Organization

- **Function Length**: Keep functions short and focused
- **Globals**: Minimize global variables
- **Modularity**: Separate concerns into appropriate files
- **Comments**: Comment complex logic and public API functions

### Example Function

```c
/**
 * @brief Parse NMEA sentence from GPS module
 * @param app Application context
 * @param sentence NMEA sentence to parse
 * @return true if sentence was successfully parsed
 */
bool predator_gps_parse_nmea(PredatorApp* app, const char* sentence) {
    if(!app || !sentence) return false;
    
    // Process GPS data
    if(strncmp(sentence, "$GPGGA", 6) == 0) {
        // Implementation details
        return true;
    }
    
    return false;
}
```

## Adding New Features

### Adding a New Scene

1. Create a new file in `scenes/` directory:
   ```c
   #include "../predator_i.h"
   #include "predator_scene.h"
   
   void predator_scene_my_feature_on_enter(void* context) {
       PredatorApp* app = context;
       // Initialize UI elements
   }
   
   bool predator_scene_my_feature_on_event(void* context, SceneManagerEvent event) {
       PredatorApp* app = context;
       bool consumed = false;
       // Handle events
       return consumed;
   }
   
   void predator_scene_my_feature_on_exit(void* context) {
       PredatorApp* app = context;
       // Clean up resources
   }
   ```

2. Add the scene to `predator_scene_config.h`:
   ```c
   ADD_SCENE(predator, my_feature, MyFeature)
   ```

3. Add navigation to your scene from another scene.

### Adding Hardware Support

1. Create new helper files in `helpers/` directory
2. Add initialization in `predator.c`
3. Update app structure in `predator_i.h` if needed

## Debugging

### Debug Tools

- Use `FURI_LOG_*` macros for debug output:
  ```c
  FURI_LOG_I("Tag", "Information message");
  FURI_LOG_D("Tag", "Debug message");
  FURI_LOG_E("Tag", "Error message: %d", error_code);
  ```

- Add debug views to visualize internal state

### Testing

- Create a test script using ufbt:
  ```bash
  ufbt launch ARGS=--test
  ```

- Monitor logs via CLI when connected to PC

## Pull Request Guidelines

1. **Branch Naming**: Use descriptive names like `feature/car-models` or `fix/gps-parsing`
2. **Commits**: Write meaningful commit messages
3. **Testing**: Test your changes on actual hardware if possible
4. **Documentation**: Update relevant documentation
5. **Code Style**: Ensure code follows project standards
6. **Pull Request Description**: Include purpose and testing details

## Hardware Development

### Pin Assignments

| Module | Pins | Description |
|--------|------|-------------|
| ESP32 UART | 15,16 | Main WiFi/BT module comms |
| GPS UART | 13,14 | GPS module communication |
| A07 RF | External | SubGHz signal transmission |

### Communication Protocols

- **UART**: Used for ESP32 and GPS modules
  - ESP32: 115200 baud
  - GPS: 9600 baud

- **SubGHz**: Used for car remote signals
  - Frequencies: 315MHz, 433.92MHz, 868.35MHz

## Architecture Overview

1. **Application Core** (`predator.c`, `predator_i.h`)
   - Manages app lifecycle
   - Connects components
   - Handles UI navigation

2. **Scene System** (`scenes/`)
   - Implements UI screens
   - Handles user interaction
   - Manages state transitions

3. **Hardware Helpers** (`helpers/`)
   - Abstract hardware interfaces
   - Implement protocol logic
   - Process data from peripherals

4. **View Components**
   - Uses Flipper Zero's GUI components
   - Popup, Submenu, TextInput, Widget

## Performance Considerations

1. **Memory Usage**
   - Avoid large static buffers
   - Free resources promptly
   - Use stack allocation when possible

2. **Battery Efficiency**
   - Implement sleep modes
   - Reduce transmission power when appropriate
   - Optimize polling intervals

3. **Responsive UI**
   - Don't block main thread
   - Keep critical sections short
   - Use async processing for long operations

## Contributing Workflow

1. **Issue Tracking**
   - Check existing issues before starting work
   - Create new issues for bugs or features
   - Use labels to categorize issues

2. **Development Cycle**
   - Fork the repository
   - Create feature branch
   - Develop and test changes
   - Submit pull request
   - Address review comments

3. **Release Process**
   - Version tagged in git
   - Compiled FAP file attached to release
   - Changelog updated

## License and Attribution

- Include appropriate copyright headers
- Maintain list of contributors
- Follow open source license requirements
- Attribute third-party code used

---

For questions or assistance, please open an issue on the project repository or contact the maintainers.
