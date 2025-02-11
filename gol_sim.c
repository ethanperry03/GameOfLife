/* Ethan Perry - Project 1: Conway's Game of Life - gol_sim.c
 * This file allows the simulation to actually run. Simulate board is the driver of the
 * simulation which has the loop and uses the other functions to correctly update the board
 * after each iteration. All of this functionality to go to the n+1th board is defined here, 
 * including different methods for handling wrap vs nowrap.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "gol_sim.h"
#include "gol_io.h"

/* simulate_board(int**, int, int, int, int, int, int);
 * Simulate board drives most of this program. All of the user input data from command line
 * and from the input file get passed here to be used. Given the specifications for the simulation,
 * it will run, and give proper output, and total excecution time. It utalizes several helper functions
 * so see those function implentations when needed.
 * @param board: 2D int array storing the input board from the user file
 * @param row: int storing number of rows
 * @param col: int storing number of cols
 * @param iter: int storing number of iterations
 * @param wrap: int storing code to wrap or no wrap simulation
 * @param show: int storing code to show or hide the simulation
 * @param speed: int storing frame per second value for output speed (if applicable)
 */
void simulate_board(int** board, int row, int col, int iter, int wrap, int show, int speed) {
    int count = 0;
    // this is the second board to oscillate between
    int** flex = create_empty_board(row, col);

    // clear away system to start output
    system("clear");

    // start clock object
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    // while iter is in range
    while(count < iter) {
        // if you are showing each output frame
        if(show == 1) {
            // print the board
            print_board(board, row, col);
            // sleep for 1/fps * 10^6
            usleep((1.0/speed)*1000000);
            // clear screen
            system("clear");            
        }
        // update the board
        update_board(board, flex, row, col, wrap);
        // swap the boards using pointers
        swap_board(&board, &flex);
        // update counter
        count = count + 1;
    }
    // print final board
    print_board(board, row, col);    

    // stop clock and calculate how long it has been
    gettimeofday(&end, NULL);
    // Calculate the elapsed time in seconds and microseconds
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    // If the end microseconds are smaller than the start microseconds, adjust the seconds
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }

    // output length of simulation in nice output
    printf("Total time for %d iterations of %dx%d is %.6f\n", iter, row, col, seconds + microseconds/1000000.0);

    // free memory used within function
    free_array(&flex, row);
    free_array(&board, row);
}

/* swap_board(int***, int***);
 * swap board allows for the old board and 'flex' board to be swapped. This is useful so that
 * when calling update_board within the sim while loop, the code can always pass one board
 * and swap them after. The function uses int*** pointers so that nothing needs to be returned
 * and all that happens is a pointer swap.
 * @param old: address location of the old (Nth) board
 * @param new: address location of the new (N+1th) board 
 */
void swap_board(int*** old, int*** new) {
    // new board pointer has the data in old
    int** temp = *old;
    // old is now pointing at new
    *old = *new;
    // new is now pointing at old
    *new = temp;
}

/* update_board(int**, int**, int, int, int);
 * Update board is the next iteration of the simulation. Given the old board, the rules of
 * the game are applied to each cell, and its new state (dead or alive) is put into the
 * new board. Wrap is supplied so that the rules can be applied differently for the edge cases. 
 * @param old: 2D int array of the old board
 * @param new: 2D int array of the new board
 * @param row: int storing number of rows for each board (used for loop conditions)
 * @param col: int storing number of cols for each board ("")
 * @param wrap: indicating wether board wraps or not, changing the conditions of the game slightly
 * */
void update_board(int** old, int** new, int row, int col, int wrap) {

    int sum = 0;

    // check the board / check the interior if wrapped
    for(int r = 0; r < row; r++) {
        for(int c = 0; c < col; c++) {
            sum = old[(r-1+row)%row][(c-1+col)%col] + old[r][(c-1+col)%col] + 
                  old[(r+1+row)%row][(c-1+col)%col] + old[(r-1+row)%row][c] + 
                  old[(r+1+row)%row][c] + old[(r-1+row)%row][(c+1+col)%col] + 
                  old[r][(c+1+col)%col] + old[(r+1+row)%row][(c+1+col)%col];
            new[r][c] = judgement_day(sum, old[r][c]);
        }
    }
    
    // fix border for no wrap
    if (wrap == 0) {
        update_nowrap(old, new, row, col);
    }
}

/* update_nowrap(int**, int**, int, int)
 * Update nowrap is a function taking the N and N+1 iterations of the board so that
 * every boundary case can be dealt with seperately. Before this function, new stores
 * the board if it were to be wrapped. That is correct for every cell except the border.
 * So, since old is still storing the old board, all calculations for the edge are done
 * and just written over on the new board. A loop is done for the top and bottom row and
 * a loop is done for the left and right edge. Then, the four corners are done manually
 * since they are unique cases.
 * @param old: 2D int array which is the Nth iteration of the board
 * @param new: 2D int array which is the N+1th iteration of the board
 * @param: int storing number of rows
 * @param: int storing number of cols
 */
void update_nowrap(int** old, int** new, int row, int col) {
    int sum;
    row = row-1;
    col = col-1;
    // top row and bot row
    for (int c = 1; c < col - 1; c++) {
        sum = old[0][c-1] + old[0][c+1] + old[1][c-1] + old[1][c] + old[1][c+1];
        new[0][c] = judgement_day(sum, old[0][c]);
        sum = old[row][c-1] + old[row][c+1] + old[row-1][c-1] + old[row-1][c] + old[row-1][c+1];
        new[row][c] = judgement_day(sum, old[row][c]);
    }
    // left and right sides
    for (int r = 1; r < row - 1; r++) {
        sum = old[r-1][0] + old[r+1][0] + old[r-1][1] + old[r][1] + old[r+1][1];
        new[r][0] = judgement_day(sum, old[r][0]);
        sum = old[r-1][col-1] + old[r][col] + old[r+1][col] + old[r-1][col] + old[r+1][col];
        new[r][col] = judgement_day(sum, old[r][col]);
    }
    // hard code the four corners
    // top left
    sum = old[0][1] + old[1][0] + old[1][1];
    new[0][0] = judgement_day(sum, old[0][0]);
    // bottom left
    sum = old[row-1][0] + old[row-1][1] + old[row][1];
    new[row][0] = judgement_day(sum, old[row][0]);
    // top right
    sum = old[0][col-1] + old[1][col-1] + old[1][col];
    new[0][col] = judgement_day(sum, old[0][col]);
    // bottom right
    sum = old[row-1][col-1] + old[row][col-1] + old[row-1][col];
    new[row][col] = judgement_day(sum, old[row][col]);
}

/* judgement_day(int, int)
 * Judgement day (cleaverly named) is the function that handle each case for life/death.
 * See the rules for Conway's game of life for rules dictating life/death for next iter of board.
 * @param sum: Stores the number of live cells around the ith, jth cell to figure out state of next iter 
 * @param oldVal: stores old value of ith, jth cell to tell if it was alive or dead.
 * @return: this returns 1 if the cell is to be alive and 0 if the cell is dead in the next iteration.
 */
int judgement_day(int sum, int oldVal) {
    int returnVal;
    // dies from loneliness
    if (sum < 2 && oldVal == 1) {
        returnVal = 0;
    }
    // overpopulation
    else if (sum > 3 && oldVal == 1) {
        returnVal = 0;
    }
    // good living conditions
    else if ((sum == 2 || sum == 3) && oldVal == 1) {
        returnVal = 1;
    }
    // repopulation
    else if (sum == 3 && oldVal == 0) {
        returnVal = 1;
    }
    // no change
    else {
        returnVal = oldVal;
    }
    // return dead/alive status
    return returnVal;
}

/* free_array(int***, int);
 * Free array takes a pointer to a 2D int array (int**) and the number of rows in that
 * given 2D array (board). It loops through each row and frees the array. Once every row
 * of the board is free, thereby freeing all the data from the board, the array itself
 * is deallocated. 
 * Note: I was getting a bizarre error when passing board without a pointer, so this is
 * the solution I knew would work, and ultimately did work. 
 * @param: a pointer to int** array (board)
 * @param: int storing the number of rows for the given board
 */
void free_array(int*** array, int row) {
    // loop through number of rows of this board
    for(int i = 0; i < row; i++) {
        // free array at row 'r'
        free(array[0][i]);
    }
    // free entire array
    free(*array);
}