#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "matrix.h"
#include "encoder.h"
#include "generated_config.h"

// Import the keymap from keymap.c
// We assume 4 layers max, but we only use [0] for now.
extern const uint8_t keymap[4][MATRIX_ROWS][MATRIX_COLS];

// --- CALLBACKS (Required for linking) ---
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) { return 0; }
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {}

bool timer_callback(struct repeating_timer *t) {
#if ENCODER_COUNT > 0
    encoder_read();
#endif
    return true;
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

    #if ENCODER_COUNT > 0
        for (int i = 0; i < ENCODER_COUNT; i++) {
            int32_t delta = encoder_get_delta(i);

            // Simple Hardcoded Map for Test:
            // Encoder 0: Volume Up/Down
            if (i == 0) {
                if (delta > 0) {
                    // Send Consumer Control for Vol Up (Next Step)
                    // For now, let's just type '+' to prove it works
                    if (count < 6) keycode[count++] = 0x2E; // KC_EQUAL (+)
                } else if (delta < 0) {
                    if (count < 6) keycode[count++] = 0x2D; // KC_MINUS (-)
                }
            }
        }
    #endif

    // 3. Send the list of pressed keys to Windows
    tud_hid_keyboard_report(0, 0, keycode);
}

int main() {
    stdio_init_all();
    matrix_init(); // Uses generated pins now
    tusb_init();

    #if ENCODER_COUNT > 0
        encoder_init();
        struct repeating_timer timer;
        add_repeating_timer_us(-1000, timer_callback, NULL, &timer); // 1000us = 1ms
    #endif

    while (true) {
        tud_task();
        hid_task();
    }
    return 0;
}