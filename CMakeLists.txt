cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)

project(woyta_pad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# --- DEFINITIONS ---
# Define the executable and the source files for the "Spam A Bot" test
add_executable(woyta_pad
        core/main.c
        core/usb_descriptors.c
)

# --- INCLUDES ---
# Tell the compiler where to look for header files (like tusb_config.h)
target_include_directories(woyta_pad PRIVATE
        core
)

# --- LIBRARIES ---
# Link necessary libraries
target_link_libraries(woyta_pad
        pico_stdlib         # GPIO, Time, UART
        tinyusb_device      # The USB Stack
        tinyusb_board       # Board-specific USB glue
)

# --- USB CONFIG ---
# Disable the SDK's default USB-Serial (CDC) driver because
# we are manually handling the USB stack in main.c
pico_enable_stdio_usb(woyta_pad 0)
pico_enable_stdio_uart(woyta_pad 1)

# --- OUTPUT ---
# Create the .uf2 file for flashing
pico_add_extra_outputs(woyta_pad)