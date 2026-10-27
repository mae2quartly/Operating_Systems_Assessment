#include <stdio.h> 
#include "game.h" //includes the header file just made to link all of the function declartions and the size constant for the grid

void init_board(char *board) {
    for (int i = 0; i < SIZE * SIZE; i++) { 
        //loop through the grid (loop 9 times) to populate all available cells
        *(board + i) = '1' + i; 
        //use offset to store the value of the cells at their memory locations
    } 
} 

void print_board(const char *board) { 
    printf("\n"); //formatting
    for (int r = 0; r < SIZE; r++) { 
        //for loop iterating through the rows in the grid
        for (int c = 0; c < SIZE; c++) { 
            //for loop iterating through the columns in the grid
            printf(" %c ", *(board + r * SIZE + c)); 
            //prints the values by looking through the line they are saved as in RAM
            if (c < SIZE - 1) printf("|"); 
            //prints the vertical lines seperating the cells 
        }
        printf("\n");
        //formatting newlines
        if (r < SIZE - 1) printf("---|---|---\n"); 
        //prints horizontal lines seperating the cells 
    }
    printf("\n"); 
    //formatting newlines
} 

int make_move(char *board, int cell, char player) { 
    if (cell < 1 || cell > 9) return 0; 
    //checks the number inside the row is between 1-9 as we set it before returning false if incorrect
    char *target = board + (cell - 1); 
    //finds memory pointer of the specific cell being asked for 
    if (*target == 'X' || *target == 'O') return 0; 
    //checks the dereferenced value to see if the cell has been taken already or if it can be filled
    *target = player; 
    //writes an x or a o for a play
    return 1; 
    //returns true if the whole sequence was carried out and recorded 
} 

int check_win(const char *board) {
    int wins[8][3] = { 
        //array storing all of the winning combinations to check through
        {0,1,2}, {3,4,5}, {6,7,8}, 
        //top middle and bottom row
        {0,3,6}, {1,4,7}, {2,5,8}, 
        //left middle and right column 
        {0,4,8}, {2,4,6} 
        //diagonal combinations
    }; 
    for (int i = 0; i < 8; i++) { 
        //loop evaluating through the combinations
        if (*(board + wins[i][0]) == *(board + wins[i][1]) && 
        //checks if first cell matches second cell
            *(board + wins[i][1]) == *(board + wins[i][2])) { 
            //checks to see if second cell matches third cell 
            return 1; 
            //returns true if all cells match an a winning line is found 
        } 
    } 
    return 0;
    //returns false if no winning combinations have been found after checking all 8 options 
}