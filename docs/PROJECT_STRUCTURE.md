# Currency Exchange Calculator

This directory contains the complete source code for the Currency Exchange Calculator application.

## Directory Structure

```
├── src/                    # Source code files
│   ├── main.cpp           # Application entry point
│   ├── ui/                # User interface components
│   │   └── mainwindow.cpp # Main application window
│   ├── core/              # Core application logic
│   │   ├── theme.cpp      # Theme management
│   │   └── language.cpp   # Language and internationalization
│   ├── network/           # Network and API handling
│   │   └── currencyapimanager.cpp # Currency exchange API management
│   └── utils/             # Utility functions and helpers
│
├── include/               # Header files
│   ├── ui/               # UI component headers
│   │   └── mainwindow.h  # Main window header
│   ├── core/             # Core component headers
│   │   ├── theme.h       # Theme management header
│   │   └── language.h    # Language header
│   ├── network/          # Network component headers
│   │   └── currencyapimanager.h # API manager header
│   └── utils/            # Utility headers
│
├── resources/            # Application resources
│   ├── translations/     # Translation files
│   ├── icons/           # Application icons
│   ├── themes/          # Theme files
│   ├── resources.qrc    # Main resource file
│   └── translations.qrc # Translation resource file
│
├── docs/                # Documentation
│   ├── README.md        # Main documentation
│   ├── ROADMAP.md       # Development roadmap
│   └── *.md            # Other documentation files
│
├── scripts/             # Build and deployment scripts
│   ├── deploy.bat       # Windows deployment script
│   ├── deploy_linux.sh.in # Linux deployment template
│   └── deploy_macos.sh.in # macOS deployment template
│
├── tests/               # Unit tests and test files
│
├── build/               # Build output directory (generated)
│
├── CMakeLists.txt       # CMake build configuration
├── README.md            # Project overview
└── LICENSE              # License file
```

## Building the Project

### Prerequisites
- Qt 6.8.1 or later
- CMake 3.16 or later
- C++17 compatible compiler (MSVC 2022, GCC, or Clang)

### Build Steps
```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build the project
cmake --build . --config Release
```

### Build Output
- Executable: `build/bin/CurrencyExchange.exe`
- Libraries: `build/lib/`
- Resources: Automatically copied to build directory

## Development Guidelines

### Code Organization
- **UI Components**: Place in `src/ui/` and `include/ui/`
- **Core Logic**: Place in `src/core/` and `include/core/`
- **Network/API**: Place in `src/network/` and `include/network/`
- **Utilities**: Place in `src/utils/` and `include/utils/`

### Include Paths
- Use relative includes from the include directory root
- Example: `#include "ui/mainwindow.h"` instead of `#include "mainwindow.h"`

### Resource Management
- Place all resources in the `resources/` directory
- Update `resources.qrc` when adding new resources
- Translation files go in `resources/translations/`

### Testing
- Add unit tests to the `tests/` directory
- Follow Qt Test framework conventions
- Test files should mirror the source structure

## Deployment

The project includes automated deployment scripts for different platforms:

- **Windows**: `scripts/deploy.bat`
- **Linux**: `scripts/deploy_linux.sh`
- **macOS**: `scripts/deploy_macos.sh`

These scripts automatically:
- Copy required Qt libraries
- Include necessary plugins
- Package resources and translations
- Create distributable packages
