/* Ethan Perry - Project 1: Conway's Game of Life - gol_io.c
 * This file actually reads the file, and has functionality to create and display boards.
 * The file reading is important to correctly initialize the board and prepare for the 
 * actual simulation portion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gol_io.h"

/* read_file(char*, int*, int*, int*);
 * Read file opens a file pointer to a valid input file (see check_file for validation).
 * The number of rows, cols, and iterations for the simulation are read in from the first
 * three lines of the file. The int* passed into the function point to these values so they
 * can be later referenced and used back in main. A 2D board array is allocated and data
 * is read in line by line until end of file (see create_empty_board for more details).
 * Then, the board is returned (int**)
 * @param filename: the string containing the name of the user input file
 * @param prow: pointer to the integer storing number of rows for the board
 * @param pcol: pointer to the integer storing number of cols for the board
 * @param psim: pointer to the integer storing number of iterations for the simulation
 * @return: 2D int array storing all the board information from the input file
*/
int** read_file(char* filename, int* prow, int* pcol, int* psim) {
     // open file to read, know it's valid from earlier cmd line parse check
    FILE* infile = fopen(filename, "r");

    // read in top three values from file
    fscanf(infile, "%d\n", prow);
    fscanf(infile, "%d\n", pcol);
    fscanf(infile, "%d\n", psim);

    // create an empty board
    // board = create_empty_board(*prow, *pcol);
    int** tempBoard = create_empty_board(*prow, *pcol);

    int tempRow, tempCol;
    // while the line of the file you read has exactly two data items
    while (fscanf(infile, "%d %d\n", &tempRow, &tempCol) == 2) {
        tempBoard[tempRow][tempCol] = 1;
    }

    // close file pointer
    fclose(infile);
    // return int** (2d) board array
    return tempBoard;
}

/* create_empty_board(int, int)
 * Create empty board takes in dimension input for a ROWxCOL grid, 2D array.
 * A double for loop initializes and allocates the board, filling every grid
 * position with 0's.
 * @param: integer value storing number of rows for the board
 * @param: integer value storing number of cols for the board
 * @return: int** of the 2D int array storing the empty board (RxC) of all 0's
*/
int** create_empty_board(int row, int col) {
    // create board with empty 0's with given row x col
    int** tempBoard = malloc(row * sizeof(int*));
    for(int r = 0; r < row; r++) {
        // loop through and create array at each pointer in tempBoard
        int* temp = malloc(col * sizeof(int));
        // fill the array with 0's
        for(int c = 0; c < col; c++) {
            temp[c] = 0;
        }
        // put temp array at row r
        tempBoard[r] = temp;
    }
    // return board
    return tempBoard;
}

/* print_board(int**, int, int);
 * Print board takes in a int** 2D array (board), along with its dimensions
 * to output to the user. The board is coded using 0/1's integers but the output
 * will be done using some characters to enhance the output experience. 
 * 0 = '-' and 1 = '@'
 * @param: int** storing the 2D integer array board
 * @param: integer value storing number of rows for the board
 * @param: integer value storing number of cols for the board
 */
void print_board(int** board, int row, int col) {
    // loop through each data item (either a 0 or 1)
    for (int r = 0; r < row; r++) {
        // if grid cell data is 1, print @, else print -
        for (int c = 0; c < col; c++) {
            if (board[r][c] == 1)
                printf("%c", '@');
            else
                printf("%c", '-');
        }
        printf("\n");
    }
    printf("\n");
}