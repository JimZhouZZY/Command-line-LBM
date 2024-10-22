# Command Line LBM
This repository contains a C++ program that utilizes the standard C++ library to simulate simple fluid dynamics using the Lattice-Boltzmann Method.

The program is designed to plot results directly within the command-line interface using ANSI escape sequences. This feature ensures compatibility with tty environments, allowing for straightforward plotting.

![showcase](https://github.com/JimZhouZZY/Command-line-LBM/assets/140597003/9e7727d8-d935-4c08-a2bc-f4a5f2031d54)


## How to set it up?

It is pretty easy to set this program up.

* First, clone this repository
`git clone https://github.com/JimZhouZZY/Command-line-LBM.git`.

* Then, compile by a standard C++ compliler, e.g. clang.`clang main.cpp -o main -std=c++11`.

* Finally, run the program `.\main`.

## Issues you might meet

* Why is it displaying garbled characters?"

    In general, this issue occurs when the encoding of your terminal does not match the encoding of the code (UTF-8). 
    
    First ensure that you can see the correct characters (█■≡) in **fmtio.h**. If you can see the characters, Change the encoding of **fmtio.h** to the encoding of your terminal, and then the issue should be resolved. Else, copy the characters above and replace the garbled characters in your **fmtio.h**, save your files with the encoding format used by your terminal.

    For example, if the language of your terminal is Chinese, then you should change the encoding of **fmtio.h** from the original UTF-8 to GBK.

* Why is it outputing so many ANSI escape codes?

    Windows command does not support ANSI escape codes very well. Please turn to Windows Terminal, Linux or MacOS.

* Why does my output splitted to multiple lines?

    Ensure that your terminal window in wide enough so that the output won't split.

## Preview Vieo

https://github.com/JimZhouZZY/Command-line-LBM/assets/140597003/39d39191-f497-43c8-beb7-a3c82e9e93cc




