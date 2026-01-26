#pragma once

#include <stdint.h>
#include <stdbool.h>

// Initializes the hardcoded GPIO pins
void matrix_init(void);

// Scans the matrix one time (drives columns, reads rows)
void matrix_scan(void);

// Returns true if the switch at [row][col] is currently pressed
bool matrix_is_on(uint8_t row, uint8_t col);