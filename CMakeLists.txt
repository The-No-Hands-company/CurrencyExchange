cmake_minimum_required(VERSION 3.16)
project(CurrencyExchange VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt automation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform-specific settings
if(WIN32)
    set(PLATFORM_LIBS "")
    set(PLATFORM_DEFS "WIN32_LEAN_AND_MEAN")
elseif(UNIX)
    if(APPLE)
        set(PLATFORM_LIBS "")
        set(PLATFORM_DEFS "")
        set(MACOSX_BUNDLE TRUE)
    else()
        set(PLATFORM_LIBS "")
        set(PLATFORM_DEFS "")
    endif()
endif()

# Find required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network LinguistTools)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/network
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

# Source files organized by category
set(MAIN_SOURCES
    src/main.cpp
)

set(UI_SOURCES
    src/ui/mainwindow.cpp
    include/ui/mainwindow.h
)

set(CORE_SOURCES
    src/core/theme.cpp
    include/core/theme.h
    src/core/language.cpp
    include/core/language.h
)

set(NETWORK_SOURCES
    src/network/currencyapimanager.cpp
    include/network/currencyapimanager.h
)

set(UTILS_SOURCES
    # Add utility sources here as they're created
)

# Combine all sources
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${UI_SOURCES}
    ${CORE_SOURCES}
    ${NETWORK_SOURCES}
    ${UTILS_SOURCES}
)

# Resource files
set(RESOURCE_FILES
    resources/resources.qrc
    resources/translations.qrc
)

# Translation files
set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/translations/bitcoin_es.ts
)

# Create commands to generate QM files
qt_add_translations(${PROJECT_NAME}
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Copy QM files to the translations directory
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/translations/bitcoin_es.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/bitcoin_es.qm
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/translations/bitcoin_es.qm
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bitcoin_es.qm
)

# Add custom target for translations
add_custom_target(copy_translations ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources/translations/bitcoin_es.qm
)

# Create executable
qt_add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${RESOURCE_FILES})

# Add dependencies
add_dependencies(${PROJECT_NAME} copy_translations)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

# Copy translations to the binary directory during build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/translations/bitcoin_es.qm"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations/"
)

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Configure application icon and platform-specific settings
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    
    # Use compiler-appropriate linker flags
    if(MINGW)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-mwindows")
    elseif(MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    endif()
endif()

# Add resource files if they exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc")
    set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC ON)
endif()

# Add compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PLATFORM_DEFS}
    QT_USE_QSTRINGBUILDER
    QT_NO_OPENSSL
)

# Installation
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install Qt plugins for Windows
if(WIN32)
    install(DIRECTORY "${Qt6Core_DIR}/../../../plugins"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        FILES_MATCHING PATTERN "*.dll"
    )
endif()

# Configure deployment scripts
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/deploy_linux.sh.in"
    "${CMAKE_CURRENT_BINARY_DIR}/deploy_linux.sh"
    @ONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/scripts/deploy_macos.sh.in"
    "${CMAKE_CURRENT_BINARY_DIR}/deploy_macos.sh"
    @ONLY
)

# Make deployment scripts executable on Unix
if(UNIX)
    execute_process(
        COMMAND chmod +x "${CMAKE_CURRENT_BINARY_DIR}/deploy_linux.sh"
        COMMAND chmod +x "${CMAKE_CURRENT_BINARY_DIR}/deploy_macos.sh"
    )
endif()

# Add custom targets for deployment
add_custom_target(deploy_linux
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/deploy_linux.sh"
    DEPENDS ${PROJECT_NAME}
)

add_custom_target(deploy_macos
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/deploy_macos.sh"
    DEPENDS ${PROJECT_NAME}
)

if(WIN32)
    add_custom_target(deploy_windows
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/deploy.bat"
        DEPENDS ${PROJECT_NAME}
    )
endif()

# Finalize executable
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

# Print build information
message(STATUS "=== Currency Exchange Calculator Build Configuration ===")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "=========================================================") 