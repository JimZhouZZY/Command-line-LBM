#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H

#include "constants.h"
#include <array>
#include "types.h"

// Fill a 2D array with given value
inline void fill2DArray(Double2DArray& arr, double value){
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            arr[i][j] = value;
        }
    }
}

inline void roll(Double2DArray& arr, int length, int axis) {
    int shiftRows = length * (1 - axis);
    int shiftCols = length * axis;
    Double2DArray rolledArr = {};
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            rolledArr[i][j] = arr[(i + shiftRows + HEIGHT) % HEIGHT][(j - shiftCols + WIDTH) % WIDTH];
        }
    }
    arr = rolledArr;
}

inline void roll(Boolean2DArray& arr, int length, int axis) {
    int shiftRows = length * (1 - axis);
    int shiftCols = length * axis;
    Boolean2DArray rolledArr = {};
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++ j) {
            rolledArr[i][j] = arr[(i + shiftRows + HEIGHT) % HEIGHT][(j - shiftCols + WIDTH) % WIDTH];
        }
    }
    arr = rolledArr;
}

inline Double2DArray roll_and_return(Double2DArray arr, int length, int axis) {
    int shiftRows = length * (1 - axis);
    int shiftCols = length * axis;
    Double2DArray rolledArr = {};
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++ j) {
            rolledArr[i][j] = arr[(i + shiftRows + HEIGHT) % HEIGHT][(j - shiftCols + WIDTH) % WIDTH];
        }
    }
    return rolledArr;
}

#endif // ARRAY_UTILS_H