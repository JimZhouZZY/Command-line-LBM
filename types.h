#ifndef TYPES_H
#define TYPES_H

#include <array>
#include "constants.h"

// Define type of arrays
using Double2DArray = std::array<std::array<double, WIDTH>, HEIGHT>;
using Boolean2DArray = std::array<std::array<bool, WIDTH>, HEIGHT>;

#endif //TYPES_H