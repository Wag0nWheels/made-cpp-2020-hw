#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64(double number) {
    return *((uint64_t*)(&number));
}

bool getBit(const uint64_t number, const uint8_t index) {
    uint64_t mask = 1 << index;
    uint64_t temp = mask & number;
    temp >>= index;
    return temp;
}

// проверка на нулевые значения
bool it_is_0(const uint64_t number, uint8_t index_right, uint8_t index_left) {
    uint8_t count = 0;
    while (getBit(number, index_right) == 0 && index_right <= index_left) {
        count++;
        index_right++;
    }
    return count == 11;
}

// проверка на единичные значения
bool it_is_1(const uint64_t number, uint8_t index_right, uint8_t index_left) {
    uint8_t total = index_left - index_right + 1;
    uint8_t count = 0;
    while (getBit(number, index_right) == 1 && index_right <= index_left) {
        count++;
        index_right++;
    }
    return count == total;
}


/**
 * Checkers here:
 */

bool checkForPlusZero(uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero(uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf(uint64_t number) {
    return number == 0x7ff0000000000000;
}

bool checkForMinusInf(uint64_t number) {
    return number == 0xfff0000000000000;
}

bool checkForPlusNormal(uint64_t number) {
    if (getBit(number, 63) == 0) {
        if (!it_is_1(number, 52, 62)) {
            if (!it_is_0(number, 52, 62)) {
                return true;
            }
        }
    }
}

bool checkForMinusNormal(uint64_t number) {
    if (getBit(number, 63) == 1) {
        if (!it_is_1(number, 52, 62)) {
            if (!it_is_0(number, 52, 62)) {
                return true;
            }
        }
    }
}

bool checkForPlusDenormal(uint64_t number) {
    if (getBit(number, 63) == 0) {
        if (it_is_0(number, 52, 62)) {
            if (!it_is_0(number, 0, 51)) {
                return true;
            }
        }
    }
}

bool checkForMinusDenormal(uint64_t number) {
    if (getBit(number, 63) == 1) {
        if (it_is_0(number, 52, 62)) {
            if (!it_is_0(number, 0, 51)) {
                return true;
            }
        }
    }
}

bool checkForSignalingNan(uint64_t number) {
    if (it_is_1(number, 52, 62)) {
        if (!it_is_0(number, 0, 51)) {
            if (getBit(number, 51) == 0) {
                return true;
            }
        }
    }
}

bool checkForQuietNan(uint64_t number) {
    if (it_is_1(number, 52, 62)) {
        if (getBit(number, 51) == 1) {
            return true;
        }
    }
}


void classify(double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}

