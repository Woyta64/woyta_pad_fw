#include "matrix.h"
#include "pico/stdlib.h"
#include "generated_config.h"

static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const uint8_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

// State buffer
static bool matrix_state[MATRIX_ROWS][MATRIX_COLS];

/* * ========================================================================
 * LOGIC FOR ROW2COL
 * ========================================================================
 */
#if defined(DIODE_DIRECTION_ROW2COL)

void matrix_init(void) {
    // 1. Initialize COLS (Input with Pull-Up)
    for (int i = 0; i < MATRIX_COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }
    // 2. Initialize ROWS (Input / Float)
    for (int i = 0; i < MATRIX_ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_IN);
    }
}

void matrix_scan(void) {
    for (int r = 0; r < MATRIX_ROWS; r++) {
        // Drive Row LOW
        gpio_set_dir(row_pins[r], GPIO_OUT);
        gpio_put(row_pins[r], 0);
        sleep_us(10);

        // Check Cols
        for (int c = 0; c < MATRIX_COLS; c++) {
            // If Col reads 0, it's pressed
            bool is_pressed = (gpio_get(col_pins[c]) == 0);
            matrix_state[r][c] = is_pressed;
        }

        // Release Row
        gpio_set_dir(row_pins[r], GPIO_IN);
    }
}

/* * ========================================================================
 * LOGIC FOR COL2ROW
 * ========================================================================
 */
#elif defined(DIODE_DIRECTION_COL2ROW)

void matrix_init(void) {
    // 1. Initialize ROWS (Input with Pull-Up)
    for (int i = 0; i < MATRIX_ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_IN);
        gpio_pull_up(row_pins[i]);
    }
    // 2. Initialize COLS (Input / Float)
    for (int i = 0; i < MATRIX_COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
    }
}

void matrix_scan(void) {
    for (int c = 0; c < MATRIX_COLS; c++) {
        // Drive Column LOW
        gpio_set_dir(col_pins[c], GPIO_OUT);
        gpio_put(col_pins[c], 0);
        sleep_us(10);

        // Check Rows
        for (int r = 0; r < MATRIX_ROWS; r++) {
            // If Row reads 0, it's pressed
            bool is_pressed = (gpio_get(row_pins[r]) == 0);
            matrix_state[r][c] = is_pressed;
        }

        // Release Column
        gpio_set_dir(col_pins[c], GPIO_IN);
    }
}

#else
#error "DIODE_DIRECTION must be defined as ROW2COL or COL2ROW in keyboard.json"
#endif

// Shared Helper
bool matrix_is_on(uint8_t row, uint8_t col) {
    if (row >= MATRIX_ROWS || col >= MATRIX_COLS) return false;
    return matrix_state[row][col];
}