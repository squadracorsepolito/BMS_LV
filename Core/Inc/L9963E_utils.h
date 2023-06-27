#ifndef L9963E_UTILS_H
#define L9963E_UTILS_H

#include <inttypes.h>

#define L9963_VRES 0.000089f

#define CELLS_N 7
#define GPIOS_N 7
#define ENABLED_CELLS (L9963E_CELL1 | L9963E_CELL2 | L9963E_CELL3 | L9963E_CELL4 | L9963E_CELL12 | L9963E_CELL13 | L9963E_CELL14)

void L9963E_utils_init(void);
void L9963E_utils_read_cells(uint8_t read_gpio);
uint16_t const* L9963E_utils_get_gpios(uint8_t *len);
uint16_t const* L9963E_utils_get_cells(uint8_t *len);
float L9963E_utils_get_cell_mv(uint8_t index);
float L9963E_utils_get_batt_mv(void);

#endif // L9963E_UTILS_H
