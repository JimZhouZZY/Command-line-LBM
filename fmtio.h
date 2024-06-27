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
void fmtout(std::string content, std::string color);

void fmtprint(Double2DArray arr) {
    std::cout<<"\033[2J\033[1;1H";
    std::cout<<parameters + "Iteration: " + std::to_string(itr)<<std::endl;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (barrier[i][j] == true) fmtout("¨€", MAGENTA);
            else if (arr[i][j] > 0.01) fmtout("¨€", RED);
            else if (arr[i][j] < -0.01) fmtout("¨€", BLUE);
            else fmtout("¨€", WHITE);
        }
        std::cout<<std::endl;
    }
}

void fmtout(std::string content, std::string color) {
    std::cout << color << content << RESET;
}