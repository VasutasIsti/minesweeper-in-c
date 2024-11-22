#include "mswcell.h"

Cell Init_Cell() {
    Cell cell = {false, false, false, 0};
    return cell;
}

bool FlagCell(Cell *cell) {
    // Returns the set state
    cell->isFlaged = !cell->isFlaged;
    return cell->isFlaged;
}