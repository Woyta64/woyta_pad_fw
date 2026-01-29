#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"

// Standard HID Keycode for 'A'
#define HID_KEY_A 0x04
#include "matrix.h"
#include "generated_config.h"

// Import the keymap from keymap.c
// We assume 4 layers max, but we only use [0] for now.
extern const uint8_t keymap[4][MATRIX_ROWS][MATRIX_COLS];

// --- CALLBACKS (Required for linking) ---
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) { return 0; }
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

// --- KEYBOARD TASK ---
void hid_task(void) {
    if (!tud_hid_ready()) return;

    // 1. Scan the hardware
    matrix_scan();

    uint8_t keycode[6] = {0};
    uint8_t count = 0;

    // 2. Iterate over every switch
    for (int r = 0; r < MATRIX_ROWS; r++) {
        for (int c = 0; c < MATRIX_COLS; c++) {
            // If physical switch is pressed...
            if (matrix_is_on(r, c)) {

                // ... Look up what key that is on Layer 0
                uint8_t code = keymap[0][r][c];

                // If it's a valid key and we have room in the packet
                if (code != 0 && count < 6) {
                    keycode[count] = code;
                    count++;
                }
            }
        }
    }

    // PRESS 'A'
    uint8_t keycode[6] = { HID_KEY_A };
    tud_hid_keyboard_report(0, 0, keycode);
}

int main() {
    stdio_init_all();
    tusb_init();      // Init USB Stack
    matrix_init(); // Uses generated pins now
    tusb_init();


    while (true) {
        tud_task();
        hid_task();
    }
    return 0;
}