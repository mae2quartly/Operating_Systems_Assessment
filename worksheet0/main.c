#include <stdio.h> 
#include "game.h" 

int main(void) { 
    char board[SIZE * SIZE]; 
    //allocated memory for the 3x3 grid 
    char current_player = 'X'; 
    //declares and sets the starting players symbol to x 
    int turns = 0; 
    //initialises the turn tracker for the game at 0 as the cells will be full by turns = 9
    int choice; 
    //initialises the variable to store the players choice in the game 

    init_board(board); 
    //calls the function that initialises the board to begin the game 

    while (turns < 9) { 
        //while loop that runs until cells are all full or game has been won
        print_board(board); 
        //prints the board as it stands in the game into the terminal 
        printf("Ready Player %c, enter cell number between 1 and 9: ", current_player);
        //asks the player who's go it is to make a cell choice 
        if (scanf("%d", &choice) != 1) break; 
        //takes the integer input from player and breaks the cycle if a non-number is inputted 

        if (make_move(board, choice, current_player)) { 
            //passes the inputted instructions into the make move function and checks a 1 is returned for the move to have suceeded 
            turns++; 
            //adds one to the turn counter 
            if (check_win(board)) { 
                //checks to see if a winning condition has been met (a 1 will be returned if yes)
                print_board(board); 
                //prints the winning board if the game is complete
                printf("Congrats, player %c wins!\n", current_player); 
                //confirms the player who has won
                return 0;
                //0 = successful game completion
            } 
            current_player = (current_player == 'X') ? 'O' : 'X'; 
            //swaps the symbol placed on the board by the next player to make a move after the last go
        } else { 
            //else is triggered if the cell is full 
            printf("Invalid move! Try again, Pick an empty cell.\n"); 
        } 
    } 

    print_board(board); 
    //if the game reaches 9 turns with no winning combination found then the board is printed
    printf("Draw!\n"); 
    return 0; 
    
} 