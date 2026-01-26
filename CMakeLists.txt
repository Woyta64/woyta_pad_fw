cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)

project(woyta_pad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# --- 1. KEYBOARD SELECTION ---
if(NOT DEFINED kb)
    set(kb "woyta-pad")
    message(STATUS "No keyboard specified, using default: ${kb}")
endif()

set(KEYBOARD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/keyboards/${kb})
message(STATUS "Building for keyboard: ${kb}")
message(STATUS "Keyboard directory: ${KEYBOARD_DIR}")

# --- 2. DEFINE PATHS ---
set(GENERATED_HEADERS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(GENERATED_KEYBOARD_HEADER ${GENERATED_HEADERS_DIRECTORY}/generated_config.h)

# --- 3. GENERATION COMMAND ---
add_custom_command(
        OUTPUT ${GENERATED_KEYBOARD_HEADER}
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_config.py
        ${KEYBOARD_DIR}/keyboard.json
        ${GENERATED_KEYBOARD_HEADER}
        DEPENDS ${KEYBOARD_DIR}/keyboard.json
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_config.py
        COMMENT "Generating config header from ${kb}/keyboard.json..."
)

# --- 4. BUILD EXECUTABLE ---
add_executable(woyta_pad
        core/main.c
        core/usb_descriptors.c
        core/matrix.c
        ${KEYBOARD_DIR}/keymap.c
        ${GENERATED_KEYBOARD_HEADER}
)

# --- 5. INCLUDE DIRECTORIES ---
target_include_directories(woyta_pad PRIVATE
        core
        ${KEYBOARD_DIR}
        ${GENERATED_HEADERS_DIRECTORY}
)

# --- 6. LIBRARIES ---
target_link_libraries(woyta_pad
        pico_stdlib
        tinyusb_device
        tinyusb_board
)

# --- 7. USB CONFIG ---
pico_enable_stdio_usb(woyta_pad 0)
pico_enable_stdio_uart(woyta_pad 1)

# --- 8. OUTPUT ---
pico_add_extra_outputs(woyta_pad)
