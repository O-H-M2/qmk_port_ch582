#include "quantum.h"

#include "wait.h"
#include "spi_master.h"

#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

static const uint16_t col_values[MATRIX_COLS] = COLS;
static const uint8_t rows_values[MATRIX_ROWS] = ROWS;
static const int msize = MATRIX_ROWS * sizeof(matrix_row_t);
static matrix_row_t prev_matrix[MATRIX_ROWS];

static inline uint8_t read_rows(void) {

//    writePinLow(SPI_LATCH_PIN);
//    matrix_output_select_delay();
//    writePinHigh(SPI_LATCH_PIN);

    spi_start(SPI_CS_PIN, false, SPI_MODE, SPI_DIVISOR);
    spi_status_t read_result = spi_read();
    spi_stop();
     if (read_result > 0) {
       uprintf("rows %u\n",(uint8_t) read_result);
     }
    if (read_result >= 0) {
       return (uint8_t) read_result;
     } else{
       return 0;
     }
}

static inline void shift_out(uint16_t value) {

  uint8_t message[2]  = {(value >> 8) & 0xFF ,(uint8_t)(value & 0xFF) };

 // writePinLow(SPI_LATCH_PIN);
  spi_start(SPI_CS_PIN, false, SPI_MODE, SPI_DIVISOR);
  spi_transmit(message,2);
  spi_stop();
  //writePinHigh(SPI_LATCH_PIN);
  //matrix_output_select_delay();

}

static inline void select_col(uint8_t col) {
    shift_out(col_values[col]);
}

void matrix_init_custom(void) {

#ifdef CONSOLE_ENABLE
  wait_ms(3000);
#endif
  matrix_io_delay();
  spi_init();
  matrix_io_delay();

  setPinOutput(SPI_CS_PIN);
  writePinHigh(SPI_CS_PIN);
  matrix_io_delay();
 // spi_start(SPI_LATCH_PIN, true, 3, SPI_DIVISOR);
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {

    uint8_t crow;
    memset(current_matrix, 0, msize);


    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        select_col(col);

        uint8_t rows = read_rows();
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
             crow = rows_values[row];
             current_matrix[row] |= (((rows & (1 << crow))? 1 : 0) << col);
            }
        }
    bool changed = (memcmp(current_matrix, prev_matrix, msize) != 0);
    memcpy(prev_matrix, current_matrix, msize);
    return changed;
}
