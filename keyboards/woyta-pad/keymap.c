#include <stdint.h>
#include "keycodes.h"
#include "generated_config.h"

// [LAYERS][ROWS][COLS]
const uint8_t keymap[4][MATRIX_ROWS][MATRIX_COLS] = {

    // --- LAYER 0: Base Layer (Numpad Style) ---
    [0] = {
        { KC_7,    KC_8,    KC_9,    KC_0,    KC_0 },
        { KC_4,    KC_5,    KC_6,    KC_UP,   KC_0  },
        { KC_1,    KC_2,    KC_3,    KC_DOWN, KC_ENTER},
        { KC_0,    KC_0,    KC_DOT,  KC_LEFT, KC_RIGHT}
    },
};