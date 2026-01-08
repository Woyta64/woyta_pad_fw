cmake_minimum_required(VERSION 3.13)

# Načtení utility pro Pico SDK
include(pico_sdk_import.cmake)

project(woyta_pad_fw C CXX)

# Inicializace SDK
pico_sdk_init()

# --- KONFIGURACE ---
# Defaultní klávesnice, pokud není v CLionu zadána jinak
set(KEYBOARD "woyta-pad" CACHE STRING "Target keyboard directory name")

message(STATUS "Building for keyboard: ${KEYBOARD}")

# Cesty
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(KEYBOARD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/keyboards/${KEYBOARD})

set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/generated_config.h)

add_custom_command(
        OUTPUT ${GENERATED_HEADER}
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_config.py
        ${KEYBOARD_DIR}/keyboard.json
        ${GENERATED_HEADER}
        DEPENDS ${KEYBOARD_DIR}/keyboard.json ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_config.py
        COMMENT "Generating hardware config from JSON..."
)

# Přidáme generovaný soubor jako dependency, aby se vytvořil před kompilací
add_custom_target(generate_config DEPENDS ${GENERATED_HEADER})

# Kontrola existence klávesnice
if(NOT EXISTS ${KEYBOARD_DIR})
    message(FATAL_ERROR "Keyboard '${KEYBOARD}' not found in keyboards/ folder!")
endif()

# --- TARGET ---
add_executable(woyta_pad
    ${CORE_DIR}/main.c
    ${CORE_DIR}/matrix.h
    ${CORE_DIR}/matrix.c
    ${CORE_DIR}/tusb_config.h
    ${CORE_DIR}/usb_descriptors.c
)

add_dependencies(woyta_pad generate_config)

target_include_directories(woyta_pad PRIVATE
    ${CORE_DIR}
    ${KEYBOARD_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# --- LINKOVÁNÍ ---
# Povolení USB (TinyUSB) a vypnutí UART (pokud chceš debug přes USB)
pico_enable_stdio_usb(woyta_pad 1)
pico_enable_stdio_uart(woyta_pad 0)

target_link_libraries(woyta_pad 
    pico_stdlib 
    tinyusb_device
    tinyusb_board
)

# Vygenerování .uf2 souboru pro drag & drop flashování
pico_add_extra_outputs(woyta_pad)
