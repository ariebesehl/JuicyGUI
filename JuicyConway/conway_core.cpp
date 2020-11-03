
#include "conway_core.h"

ConwayGOL::ConwayGOL(unsigned int _x, unsigned int _y) {
    size_x = _x;
    size_y = _y;
    buf0 = new unsigned int[size_x * size_y];
    buf1 = new unsigned int[size_x * size_y];
    dataPtr = buf0;
    valEmpty = CONWAYGOL_VALUE_EMPTY;
    valAlive = CONWAYGOL_VALUE_ALIVE;
}

ConwayGOL::~ConwayGOL() {
    delete[] buf0;
    delete[] buf1;
}

unsigned int ConwayGOL::Iterate() {
    unsigned int numAlive = 0;
    unsigned int* workPtr = (dataPtr != buf1) ? buf1 : buf0;
    for (unsigned int y = 0; y < size_y; y++) {
        for (unsigned int x = 0; x < size_x; x++) {
            unsigned int nb_x[3], nb_y[3];
            for (unsigned int i = 0; i < 3; i++) {
                nb_x[i] = (((x - 1) + i) == size_x) ? 0 : (((x - 1) + i) > size_x) ? (size_x - 1) : ((x - 1) + i);
                nb_y[i] = (((y - 1) + i) == size_y) ? 0 : (((y - 1) + i) > size_y) ? (size_y - 1) : ((y - 1) + i);
            }

            unsigned int nb_cnt = 0;
            for (unsigned int sb_y = 0; sb_y < 3; sb_y++) {
                for (unsigned int sb_x = 0; sb_x < 3; sb_x++) {
                    if (!(sb_x == 1 && sb_y == 1)) {
                        if (*(dataPtr + nb_x[sb_x] + nb_y[sb_y] * size_x) == valAlive) {
                            nb_cnt++;
                        }
                    }
                }
            }

            if (*(dataPtr + x + y * size_x) == valAlive) {
                    if (nb_cnt > 1 && nb_cnt < 4) {
                        *(workPtr + x + y * size_x) = valAlive;
                        numAlive++;
                    } else {
                        *(workPtr + x + y * size_x) = valEmpty;
                    }
                } else {
                    if (nb_cnt > 2 && nb_cnt < 4) {
                        *(workPtr + x + y * size_x) = valAlive;
                        numAlive++;
                    } else {
                        *(workPtr + x + y * size_x) = valEmpty;
                    }
            }
        }
    }
    dataPtr = (dataPtr != buf1) ? buf1 : buf0;
    return numAlive;
}

unsigned int ConwayGOL::CountAlive() {
    unsigned int countAlive = 0;
    for (unsigned int i = 0; i < size_x * size_y; i++) {countAlive += (dataPtr[i] == valAlive) ? 1 : 0;}
    return countAlive;
}

unsigned int ConwayGOL::FlipCell(unsigned int iX, unsigned int iY) {
    if (checkInput(iX, iY)) {
        if (*(dataPtr + iX + iY * size_x) != valEmpty) {
            *(dataPtr + iX + iY * size_x) = valEmpty;
        } else {
            *(dataPtr + iX + iY * size_x) = valAlive;
        }
        return *(dataPtr + iX + iY * size_x);
    } else {
        return valEmpty;
    }
}


unsigned int ConwayGOL::SetCellValue(unsigned int x, unsigned int y, unsigned int value) {
    if (checkInput(x, y)) {
        *(dataPtr + x + y * size_x) = value;
        return *(dataPtr + x + y * size_x);
    } else {
        return valEmpty;
    }
}

unsigned int ConwayGOL::GetCellValue(unsigned int x, unsigned int y) {
    if (checkInput(x, y)) {
        return *(dataPtr + x + y * size_x);
    } else {
        return valEmpty;
    }
}

bool ConwayGOL::checkInput(unsigned int x, unsigned int y) {
    return ((x < size_x) && (y < size_y));
}
