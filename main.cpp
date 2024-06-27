#include <iostream>
#include <math.h>
#include <array>
#include <string>
#include "array_utils.h"
#include "fmtio.h"
#include "types.h"

// Iterations
int itr = 0;

// Omega
double omega = 1 / (3 * viscosity + 0.5);

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
double feq(int k, int i, int j);
void print();
void out(std::string content, std::string color);
Double2DArray curl(Double2DArray ux, Double2DArray uy);

int main() {
    init();
    for(int i = 0; i < MAX_ITR; ++i) {
        if (itr % ITR_SHOW_GAP == 0) fmtprint(curl(ux, uy));
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
        n[1][i][0] = one9th * (1 + 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[2][i][0] = one36th * (1 + 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[4][i][0] = one36th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[5][i][0] = one9th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
        n[6][i][0] = one36th * (1 - 3 * u0 + 4.5 * u0 * u0 - 1.5 * u0 * u0);
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