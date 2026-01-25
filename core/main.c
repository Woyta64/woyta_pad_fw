#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "tusb.h"

// Standard HID Keycode for 'A'
#define HID_KEY_A 0x04

// --- REQUIRED CALLBACKS (Must be present to link) ---

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}

void hid_task(void) {
    if (!tud_hid_ready()) return;

    static uint32_t start_ms = 0;
    if (to_ms_since_boot(get_absolute_time()) - start_ms < 1000) return;
    start_ms += 1000;

    // PRESS 'A'
    uint8_t keycode[6] = { HID_KEY_A };
    tud_hid_keyboard_report(0, 0, keycode);
}

int main() {
    stdio_init_all();
    tusb_init();      // Init USB Stack

    while (true) {
        tud_task(); // Handle USB events
        hid_task(); // Run hid spam logic
    }
    return 0;
}