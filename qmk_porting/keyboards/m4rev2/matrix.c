// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

#include "spi_master.h"

static const uint16_t col_values[MATRIX_COLS] = COLS;
static const pin_t row_pins[MATRIX_ROWS] = ROWS;
static uint8_t message[2] ;
static const int msize = MATRIX_ROWS * sizeof(matrix_row_t);
static matrix_row_t prev_matrix[MATRIX_ROWS];

static inline uint8_t read_rows(void) {
  uint8_t r = readPin(row_pins[0]);

  for (uint8_t row = 1; row < MATRIX_ROWS; row++) {
    r |= (readPin(row_pins[row]) << row);
   }
  return r;
}

static inline void shift_out(uint16_t value) {


  message[0] =   (uint8_t)(value >> 8) & 0xFF;
  message[1] =   (uint8_t)(value & 0xFF) ;

  spi_start(SPI_CS_PIN, SPI_LSBFIRST, SPI_MODE, SPI_DIVISOR);
  spi_transmit(message,2);
  spi_stop();
  matrix_io_delay();

}

static inline void select_col(uint8_t col) {
    shift_out(col_values[col]);

}

void matrix_init_custom(void) {

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    setPinInputLow(row_pins[row]);
  }
  matrix_io_delay();
  spi_init();
  matrix_io_delay();

}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {

    memset(current_matrix, 0, msize);

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);
        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
             current_matrix[row] |= (((rows & (1 << row))? 1 : 0) << col);
            }
        }
    bool changed = (memcmp(current_matrix, prev_matrix, msize) != 0);
    memcpy(prev_matrix, current_matrix, msize);
    return changed;
}
