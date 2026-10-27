#ifndef GAME_H //guard - checking to ensure gameh hasnt already been defined to prevent it being included twice 
#define GAME_H //guard - defines game h so the block only runs once during the compilation of the code

#define SIZE 3 
//set the size of the grid for the game as a constant value of 3 (3x3) 

//functions included in the code and the pointers that are passed to each of them
void init_board(char *board); 
//function using a charecter array pointer to fill the cells with numbers 
void print_board(const char *board); 
//function using a constant charecter array pointer to print the formatted grid for playing
int make_move(char *board, int cell, char player);
//the function that will update the cells the user picks to play with an x or a o 
int check_win(const char *board);
//fucntion checking all of the winning scenarios to see if a player has won the game yet

#endif
//marks end of conditional compliation block
