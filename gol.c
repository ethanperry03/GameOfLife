/* Ethan Perry - Project 1: Conway's Game of Life
 * main.c is the driver of this program. This is where most of the variables are declared
 * and stored. First, the parse function is called to error check user input. Then, the
 * input file is read, storing the board and accompanying data. The simulation is then ran
 * and output is given via those functions. The main purpose of main is to read in user 
 * command line input and process that data so that the program can accurately run.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gol_cmd.h"
#include "gol_io.h"
#include "gol_sim.h"

int main(int argv, char** argc) {
    // declare data to hold cmd line information
    int wrap, show, speed;
    char* filename = NULL;
    // initialize int** to store board information after read file       
    int** board;
    // store information about the board dimensions and simulation steps
    int row, col, iter; 

    // parse command line argument and store success/failure status
    int status = parse_cmd(argv, argc, &filename, &wrap, &show, &speed);
    // if it failed, return -1
    if(status == -1){
        printf("program failed for the above reason(s)\n");
        return -1;
    }

    // read the file and return row, col, iter by reference, store board
    board = read_file(filename, &row, &col, &iter);

    // simulate the game of life passing the board and the necessary 
    // information for simulating and output
    simulate_board(board, row, col, iter, wrap, show, speed);

    return 0;
}