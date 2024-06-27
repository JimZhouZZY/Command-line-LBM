#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Iterations
constexpr int MAX_ITR = 100000000;
constexpr int ITR_SHOW_GAP = 250;

// Grid Specifications
constexpr int HEIGHT = 50;
constexpr int WIDTH = 100;

// Mathematics Constants
constexpr double PI = 3.14159265359;

// Precalculate some commonly used values to speed up operations
constexpr double one36th = 1.0 / 36.0;
constexpr double four9th = 4.0 / 9.0;
constexpr double one9th = 1.0 / 9.0;

// Directions
constexpr std::array<std::array<int, 2>, 9> dir = {{
    std::array<int, 2>{0, 0},
    std::array<int, 2>{0, 1},
    std::array<int, 2>{-1, 1},
    std::array<int, 2>{-1, 0},
    std::array<int, 2>{-1, -1},
    std::array<int, 2>{0, -1},
    std::array<int, 2>{1, -1},
    std::array<int, 2>{1, 0},
    std::array<int, 2>{1, 1},
}};

// Equation coefficient matrix, corresponding to: weight, ux, uy, u2, uxuy, ux2, uy2
constexpr std::array<std::array<double, 7>, 9> c = {{
    std::array<double, 7>{four9th, 0, 0, 0, 0, 0, 0},
    std::array<double, 7>{one9th, 1, 0, 0, 0, 1, 0},
    std::array<double, 7>{one36th, 1, 1, 1, 1, 0, 0},
    std::array<double, 7>{one9th, 0, 1, 0, 0, 0, 1},
    std::array<double, 7>{one36th, -1, 1, 1, -1, 0, 0},
    std::array<double, 7>{one9th, -1, 0, 0, 0, 1, 0},
    std::array<double, 7>{one36th, -1, -1, 1, 1, 0, 0},
    std::array<double, 7>{one9th, 0, -1, 0, 0, 0, 1},
    std::array<double, 7>{one36th, 1, -1, 1, -1, 0, 0}
}};

// FLuid Parameters
const double viscosity = 0.020;
const double u0 = 0.15;

// Construct a string showing parameters
const std::string parameters = "Grid Size: " + std::to_string(HEIGHT) + "x" + std::to_string(WIDTH) + "    " 
                        + "u0: " + std::to_string(u0) + "    " 
                        + "Viscosity: " + std::to_string(viscosity) + "    ";

#endif // CONSTANTS_H
