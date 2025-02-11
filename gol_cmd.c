/* Ethan Perry - Project 1: Conway's Game of Life - gol_cmd.c
 * This file deals with all command line input from the user and properly error checks
 * every possible input robustly. Any errors are checked, error output is given, and
 * the program terminates. All the data given by the user is returned to main with
 * the functions implemented in this file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gol_cmd.h"

/* parse_cmd(int, char**, char**, int*, int*, int*);
 * Parse command line function takes in the argv/argc values from user input, and acts
 * as a hub function to error check all the input, and return all the data to main (by pointer)
 * @param argv: the number of arguments given in the command line 
 * @param argc: the array of strings containing all of the text given in the command line
 * @param infile: a pointer to a string that will point to user input file string
 * @param pwrap: int pointer to return wrap code value to main
 * @param pshow: int pointer to return show code value to main
 * @param pspeed: int pointer to return speed code value to main
 * @return: 0 if all input was sucessful, -1 if any portion of error checking failed
 */
int parse_cmd(int argv, char** argc, char** infile, int* pwrap, int* pshow, int* pspeed) {
    // for formatting
    printf("\n");

    // Ensure all options are entered into command line
    if (argv < 4 || argv > 5) {
        printf("error: %d parameters received\nexpected -> three or four\n\n", argv - 1);
        return -1;
    }

    // set infile to expected file name from cmd line input
    *infile = argc[1];
    int validFileFLag = check_file(*infile);
    
    // check that string in the wrap/nowrap input field is valid
    int wrapVal = check_wrap(argc[2]);
    
    // check that string in the show/hide field is valid, collect speed if approp.
    int speedVal = 0;
    int showVal = check_show(argc[3]);

    // error check if show was called without speed param
    if (showVal == 1 && argv == 4){
        printf("error: no speed paramter was provided\n\n");
        speedVal = -1;
    }
    // if a speed param was given in cmd line
    if (argv == 5) {
        speedVal = check_speed(argc[4], showVal);
    }
    // if any of the return flags are -1, exit
    if (validFileFLag == -1 || wrapVal == -1 || showVal == -1 || speedVal == -1) {
        exit(-1);
    }
    else {
        // assign vals to pointers to return them "by reference"
        *pwrap = wrapVal;
        *pshow = showVal;
        *pspeed = speedVal;   
    }

    // return 0 if input was valid
    return 0;
}

/* check_file(char*);
 * This function checks that the cmd line input for the file name is inputted
 * correctly by the user. A file pointer is initialized, and if not valid, 
 * the program prints and error and returns the error flag value of -1. If
 * the file name is valid, the program continues
 * @param filename: string containing the user input file name
 * @return returnVal: 0 for succesful file opening, -1 for failure
*/
int check_file(char* filename) {
    // int for return flag
    int returnVal = 0;

    // open file to read
    FILE* infile = fopen(filename, "r");
    // check if file pointer is null (indicating invalid file)
    if (infile == NULL) {
        printf("error: '%s' is an invalid file\n", filename);
        printf("ensure file exists and entered correctly\n\n");
        returnVal = -1;
        exit(-1);
    }
    // close file pointer
    fclose(infile);

    // return success/failure flag
    return returnVal;
}

/* check_wrap(char*);
 * Check wrap reads the commnand line argument dictating wrap/nowrap for the game.
 * wrap = 1, nowrap = 0, failure = -1
 * @param wrapString: string containing user input string for wrapping value
 * @return wrapVal: indicated wrap/nowrap or error value of -1
*/
int check_wrap(char* wrapString) {
    // check first input part
    int wrapVal;
    // if string was wrap, return a 1
    if (strcmp(wrapString, "wrap") == 0) {
        wrapVal = 1;
    }
    // else if string was nowrap, return a 0
    else if (strcmp(wrapString, "nowrap") == 0) {
        wrapVal = 0;
    }
    // else invalid, print invalid input message
    else {
        printf("error: '%s' is not a valid 'wrap' parameter\n", wrapString);
        printf("enter -> (wrap/nowrap)\n\n");
        wrapVal = -1;
        exit(-1);
    }
    // return code
    return wrapVal;
}

/* check_show(char*);
 * Check show reads the commnand line argument dictating show/hide for the output.
 * show = 1, hide = 0, failure = -1
 * @param showString: string containing user input string for show/hide value
 * @return showVal: indicated show/hide (1/0) or error value of -1
*/
int check_show(char* showString) {
    int showVal;
    // Check if input param is show
    if(strcmp(showString, "show") == 0) {
        showVal = 1;
    }
    // else if, check if input param is hide
    else if (strcmp(showString, "hide") == 0) {
        showVal = 0;
    }
    // else, error
    else {
        printf("error: '%s' is not a valid 'show' parameter\n", showString);
        printf("enter -> (show/hide)\n\n");
        showVal = -1;
        exit(-1);
    }
    // return code val
    return showVal;
}

/* check_speed(char*, int);
 * Check speed reads the commnand line argument dictating the speed as well as the
 * the show value. If show is 0 (hide), then no speed param is needed. There is an error
 * given is the user inputted hide and a speed. If the user inputted show, then the proper
 * checking of speed is done. slow = 3fps, med = 10fps, fast = 30fps
 * @param speedString: string containing user input string for speed value (slow/med/fast)
 * @param showVal: does this given input command line want to show the simulation given a speed
 * @return speedVal: indicated the frame per second rate for the speed, if provided correctly, 
 * and -1 if hide (speed) or show (invalid speed) was entered
*/
int check_speed(char* speedString, int showVal) {
    int speedVal = 0;
    // if showval was 0 (hide) but you are checking speed, give error
    if (showVal == 0) {
        printf("error: hide does not take a speed parameter '%s'\n\n", speedString);
        speedVal = -1;
        exit(-1);
    }
    // else, show was called so speed is a valid input
    // see fps calculation within simulate_board in gol_sim.c
    else {
        // if speed is slow, fps = 3
        if (strcmp(speedString, "slow") == 0) {
            speedVal = 3;
        }
        // else if speed is medium, fps = 10
        else if (strcmp(speedString, "med") == 0) {
            speedVal = 10;
        }
        // else if speed is fast, fps = 30
        else if (strcmp(speedString, "fast") == 0) {
            speedVal = 30;
        }
        // else, error and print error message
        else {
            printf("error: '%s' is not a valid 'speed' parameter\n", speedString);
            printf("enter -> (fast/med/slow)\n\n");
            speedVal = -1;
            exit(-1);
        }
    }
    // return back the speed val code
    return speedVal;
}