#ifndef FMTIO_H
#define FMTIO_H

//If the output appears messy, change the encoding of this file to match your terminal's encoding.
#include <iostream>
#include <string>
#include "colors.h"
#include "constants.h"
#include "types.h"

extern int itr;
extern Boolean2DArray barrier;
extern Double2DArray ux, uy;

void fmtprint(Double2DArray arr);
void fmtout(std::string content, std::string color, std::string background_color);

void fmtprint(Double2DArray arr) {
    std::cout<<"\033[2J\033[1;1H";
    std::cout<<parameters + "Iteration: " + std::to_string(itr)<<std::endl;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (barrier[i][j] == true) fmtout("█", BLACK, WHITE_BG);
            else if (arr[i][j] > 0.07) fmtout("█", RED, WHITE_BG);
            else if (0.06 < arr[i][j] and arr[i][j] < 0.07) fmtout("■", RED, WHITE_BG);
            else if (0.05 < arr[i][j] and arr[i][j] < 0.06) fmtout("≡", RED, WHITE_BG);
            else if (0.04 < arr[i][j] and arr[i][j] < 0.05) fmtout("=", RED, WHITE_BG);
            else if (0.03 < arr[i][j] and arr[i][j] < 0.04) fmtout("+", RED, WHITE_BG);
            else if (0.02 < arr[i][j] and arr[i][j] < 0.03) fmtout("-", RED, WHITE_BG);
            else if (0.01 < arr[i][j] and arr[i][j] < 0.02) fmtout(":", RED, WHITE_BG);
            else if (0.001 < arr[i][j] and arr[i][j] < 0.01) fmtout(".", RED, WHITE_BG);
            else if (-0.01 < arr[i][j] and arr[i][j] < -0.001) fmtout(".", BLUE, WHITE_BG);
            else if (-0.02 < arr[i][j] and arr[i][j] < -0.01) fmtout(":", BLUE, WHITE_BG);
            else if (-0.03 < arr[i][j] and arr[i][j] < -0.02) fmtout("-", BLUE, WHITE_BG);
            else if (-0.04 < arr[i][j] and arr[i][j] < -0.03) fmtout("+", BLUE, WHITE_BG);
            else if (-0.05 < arr[i][j] and arr[i][j] < -0.04) fmtout("=", BLUE, WHITE_BG);
            else if (-0.06 < arr[i][j] and arr[i][j] < -0.05) fmtout("≡", BLUE, WHITE_BG);
            else if (-0.07 < arr[i][j] and arr[i][j] < -0.06) fmtout("■", BLUE, WHITE_BG);
            else if (arr[i][j] < -0.07) fmtout("█", BLUE, WHITE_BG);
            else fmtout("█", WHITE, WHITE_BG);
        }
        std::cout<<std::endl;
    }
}

void fmtout(std::string content, std::string color, std::string background_color) {
    std::cout << background_color << color << content << RESET << BLACK_BG;
}

#endif //FMTIO_H