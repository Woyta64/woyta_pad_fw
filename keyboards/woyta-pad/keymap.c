#include <stdint.h>
#include "keycodes.h"
#include "generated_config.h"

// [LAYERS][ROWS][COLS]
const uint8_t keymap[4][MATRIX_ROWS][MATRIX_COLS] = {

    // --- LAYER 0: Base Layer (Numpad Style) ---
    [0] = {
        { KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC },
        { KC_4,    KC_5,    KC_6,    KC_UP,   KC_DEL  },
        { KC_1,    KC_2,    KC_3,    KC_DOWN, KC_ENTER},
        { TG_CYCLE,KC_0,    KC_DOT,  KC_LEFT, KC_RIGHT}
        // ^ This TG_CYCLE key (Row 3, Col 0) changes layers
    },

    // --- LAYER 1: Navigation & Macros ---
    [1] = {
        { KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_TRNS },
        { KC_HOME, KC_PGUP, KC_VOLU, KC_UP,   KC_TRNS },
        { KC_END,  KC_PGDN, KC_VOLD, KC_DOWN, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
        // ^ TG_CYCLE is KC_TRNS here, so it uses Layer 0's logic (which cycles to next layer)
    },

    // --- LAYER 2: Empty (Safe fallback) ---
    [2] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },

    // --- LAYER 3: Empty ---
    [3] = {
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS }
    },
};