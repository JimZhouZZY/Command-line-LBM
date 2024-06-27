#include <iostream>
#include <math.h>
#include <array>
#include <string>
#include "colors.h"

// Iterations
constexpr int MAX_ITR = 100000000;
constexpr int ITR_SHOW_GAP = 250;
int itr = 0;

// Grid Specifications
constexpr int HEIGHT = 50;
constexpr int WIDTH = 100;

using Double2DArray = std::array<std::array<double, WIDTH>, HEIGHT>;
using Boolean2DArray = std::array<std::array<bool, WIDTH>, HEIGHT>;

// Mathematics Constants
constexpr double PI = 3.14159265359;

// Precalculate some commonly used values to speed up operations
constexpr double one36th = 1.0 / 36.0;
constexpr double four9th = 4.0 / 9.0;
constexpr double one9th = 1.0 / 9.0;

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

// FLuid Parameters
double viscosity = 0.020;
double omega = 1 / (3 * viscosity + 0.5);
double u0 = 0.15;

// Construct a string showing parameters
std::string parameters = "Grid Size: " + std::to_string(HEIGHT) + "x" + std::to_string(WIDTH) + "    " 
                        + "u0: " + std::to_string(u0) + "    " 
                        + "Viscosity: " + std::to_string(viscosity) + "    ";

// Microscopic velocity. there are nine directions under the D2Q9 model
/*
           n4                n3                 n2
           +-----------------^----------------xx
           |xx               |             xxx |
           |  x              |           xxx   |
           |    xx           |         xx      |
           |     xxx         |       xxx       |
           |       xxxx      |     xxx         |
           |          xxx    |   xxx           |
           |            xxx  |xxx              |
           |               xxxx                |
        n5 <---------------xn0x---------------->n1
           |             xxx |xx               |
           |           xxx   | xx              |
           |         xxx     |  xxx            |
           |       xxx       |    xxx          |
           |     xxx         |      xx         |
           |    xx           |       xxx       |
           |  xx             |         xx      |
           |xxx              |           xx    |
           xx                |             xx  |
           ------------------v---------------xx|
         n6                 n7                 n8
*/
std::array<Double2DArray, 9> n = {};

// Density
Double2DArray rho = {};

// Macroscopic velocity in x, y direction
Double2DArray ux = {};
Double2DArray ux2 = {};
Double2DArray uy = {};
Double2DArray uy2 = {};
Double2DArray u2 = {};
Double2DArray u2115 = {};
Double2DArray uxuy = {};

// Barrier. there are nine directions under the D2Q9 model, too
Boolean2DArray barrier = {};

// Function Statements
void init();
void stream();
void collide();
void print();
void out(std::string content, std::string color);
double feq(int k, int i, int j);
void fill2DArray(Double2DArray& arr, double value);
void roll(Double2DArray& arr, int length, int axis);
Double2DArray curl(Double2DArray ux, Double2DArray uy);

int main() {
    init();
    for(int i = 0; i < MAX_ITR; ++i) {
        if (itr % ITR_SHOW_GAP == 0) print();
        stream();
        collide();
        itr++;
    }
    std::cout<<"Test OK.";
    return 0;
}


void init() {
    // Initialize microsopic velocities
    fill2DArray(n[0], four9th);
    fill2DArray(n[1], one9th);
    fill2DArray(n[2], one36th);
    fill2DArray(n[3], one9th);
    fill2DArray(n[4], one36th);
    fill2DArray(n[5], one9th);
    fill2DArray(n[6], one36th);
    fill2DArray(n[7], one9th);
    fill2DArray(n[8], one36th);
    /*
    int x = 50, y = 15;
    n[1][y][x] = 100;
    n[2][y][x] = 100;
    n[3][y][x] = 100;
    n[4][y][x] = 100;
    n[5][y][x] = 100;
    n[6][y][x] = 100;
    n[7][y][x] = 100;
    n[8][y][x] = 100;
    */
    // Initialize barrier
    for (int i = 0; i < 16; ++i) {
        // This barrier is a 16px vertical line positioned at the right of the canvas
        barrier[int(HEIGHT / 2 + 8 - i)][25] = true;
    }
}

void stream() {
    roll(n[1], 1, 1);
    roll(n[2], 1, 0);
    roll(n[2], 1, 1);
    roll(n[3], 1, 0);
    roll(n[4], 1, 0);
    roll(n[4], -1, 1);
    roll(n[5], -1, 1);
    roll(n[6], -1, 0);
    roll(n[6], -1, 1);
    roll(n[7], -1, 0);
    roll(n[8], -1, 0);
    roll(n[8], 1, 1);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (barrier[i][j] == true) {
                for (int k = 1; k < 9; ++k) {
                    int opposite_dir = (k + 4) <= 8 ? k + 4 : k - 4;
                    n[k][i + dir[k][0]][j + dir[k][1]] = n[opposite_dir][i][j];
                }
            }
        }
    }
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (barrier[i][j] == true) {
                for (int k = 1; k < 9; ++k) {
                    n[k][i][j] = 0;
                }
            }
        }
    }
}

void collide() {
    omega = 1 / (3 * viscosity + 0.5);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            rho[i][j] = n[0][i][j] + n[1][i][j] + n[2][i][j] + n[3][i][j]
                + n[4][i][j] + n[5][i][j] + n[6][i][j] + n[7][i][j] + n[8][i][j];
            ux[i][j] = (n[1][i][j] + n[2][i][j] + n[8][i][j]
                - n[5][i][j] - n[4][i][j] - n[6][i][j]) / rho[i][j];
            ux2[i][j] = ux[i][j] * ux[i][j];
            uy[i][j] = (n[3][i][j] + n[2][i][j] + n[4][i][j]
                - n[7][i][j] - n[8][i][j] - n[6][i][j]) / rho[i][j];
            uy2[i][j] = uy[i][j] * uy[i][j];
            u2[i][j] = ux2[i][j] + uy2[i][j];
            u2115[i][j] = 1 - 1.5 * u2[i][j];
            uxuy[i][j] = ux[i][j] * uy[i][j];
        }
    }
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            for (int k = 0; k < 9; ++k){
                n[k][i][j] = (1 - omega) * n[k][i][j] + omega * feq(k, i, j);
            }
        }
    }
    for (int i = 0; i < HEIGHT; ++i) {
        //n[0][i][0] = four9th * (1 - 1.5 * u0 * u0);
        n[1][i][0] = one9th * (1 + 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[2][i][0] = one36th * (1 + 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        //n[3][i][0] = one9th * (1 - 1.5 * u0 * u0);
        n[4][i][0] = one36th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[5][i][0] = one9th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[6][i][0] = one36th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        //n[7][i][0] = one9th * (1  - 1.5 * u0 * u0);
        n[8][i][0] = one36th * (1 + 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
    }
}

// Eq function
double feq(int k, int i, int j) {
    return (
        c[k][0] * rho[i][j] * (
            u2115[i][j] + 3 * (
                c[k][1] * ux[i][j] + c[k][2] * uy[i][j]
            )
            + c[k][5] * 4.5 * ux2[i][j] 
            + c[k][6] * 4.5 * uy2[i][j]
            + 4.5 * (
                c[k][3] * u2[i][j] + c[k][4] * 2 * uxuy[i][j]
            )
        )
    );
}

void print() {
    system("cls");
    std::cout<<parameters + "Iteration: " + std::to_string(itr)<<std::endl;
    Double2DArray value = curl(ux, uy); //curl(ux, uy);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (barrier[i][j] == true) out("¡ö", MAGENTA); //"¡ö"std::to_string(u2[i][j])
            else if (value[i][j] > 0.01) out("¡ö", RED);
            else if (value[i][j] < -0.01) out("¡ö", BLUE); //"¡õ"
            else out("¡ö", WHITE);
        }
        std::cout<<std::endl;
    }
    //getchar();
}

void out(std::string content, std::string color) {
    std::cout << color << content << RESET;
}

// Fill a 2D array with given value
void fill2DArray(Double2DArray& arr, double value){
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            arr[i][j] = value;
        }
    }
}

void roll(Double2DArray& arr, int length, int axis) {
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

void roll(Boolean2DArray& arr, int length, int axis) {
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

Double2DArray roll_and_return(Double2DArray arr, int length, int axis) {
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

Double2DArray curl(Double2DArray ux, Double2DArray uy) {
    Double2DArray cur = {};
    Double2DArray uy0 = roll_and_return(uy, -1, 1);
    Double2DArray uy1 = roll_and_return(uy, 1, 1);
    Double2DArray ux0 = roll_and_return(ux, -1, 0);
    Double2DArray ux1 = roll_and_return(ux, 1, 0);
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cur[i][j] = uy0[i][j] - uy1[i][j] - ux0[i][j] + ux1[i][j];
        }
    }
    return cur;
}