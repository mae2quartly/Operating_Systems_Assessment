#include <stdio.h>

//ex3
int compare(const int *arr1, const int *arr2, int length) {
    //passing  parameters (2 array addresses that will only be read and not changed by teh function) into the compare function
    if (arr1 == NULL || arr2 == NULL) {
        printf("Error - NULL pointer)");
        return 0;
    }
    //protecting against NULL pointers being input in

    for (int i = 0; i < length; i++) {
        //loop through arrays value by value
        if (arr1[i] != arr2[i]) {
            //if the values in the same spot in different arrays dont match 
            return 0; 
            //return false on the first incorrect match
        }
    }
    
    return 1; 
    //if they all match then return true

}   

//ex5
void swap(int *x, int *y) {
    //create swap function and specifies parameters
    int temp = *x; //store the value at x temporarily by dereferncing x to get value
    *x = *y; //value at address y is copied into address x by dereferencing y and overwriting memory location
    *y = temp; //copy saved temp value into address y to finalise swap
}

//ex6 
void print_array(int *arr, int width, int height) {
    //create a function to print an array and define the parameters that will be passed to be used
    for (int r = 0; r < height; r++) {
        //create a for loop to iterate through the array rows to print it
        for (int c = 0; c < width; c++) {
            //create a for loop inside the row loop to iterate through the columns to print the array
            printf("%d ", *(arr + r * width + c)); 
            //prints the memory position of the element in the array, finds its int value and prints it (followed by a space for formatting)
        }
        printf("\n"); //newline
    }
}


int main(void) {

    //ex1
    int n = 10;
    // local variable n is set to 10 
    int *ptr_at_n = &n; 
    // pointer variable storing the memory address of n 
    *ptr_at_n += 1; 
    // modifies the value held at n by increasing the value by 1 
    printf("n = %d\n", n); 
    //prints the value held at n (which has now increased by 1)


    //ex2
    int arr [3] = {10, 30, 2000};
    // creates an array with three values inside and sets the values 
    int *ptr = arr;
    // points to the element at teh start of the array I just created 

    for (int i = 0; i < 3; i++) {
        //creates a loop to loop through the array one by one and stop at the end as an array starts at i = 0 not i=1
        printf("Element %d - Value: %d and Address: %p\n", i, *(ptr + i), (void *)(ptr + i)); 
        //element - expects an integer and is filled by the i value, value - gets filled by the value stored at the memory location 
        //address - expects a hex memory address which is filled by finding the exact memory adress of an item and casting the int pointer to a generic pointer
    }

    //ex3
    int listX[4] = {1, 2, 3, 4};
    int listY[4] = {1, 2, 3, 4};
    int listZ[4] = {1, 2, 9, 4}; 
    // create arrays and plant data to test both outcomes of check

    int test1 = compare(listX, listY, 4);
    printf("List X and list Y test: %d (Expected: 1)\n", test1);
    //compare the arrays that are teh same 

    int test2 = compare(listX, listZ, 4);
    printf("List X and list Z test: %d (Expected: 0)\n", test2);
    //compare the arrays that are not the same

    //ex4
    FILE *file = fopen("foo.txt", "r"); 
    //open file foo.txt in read mode and assigns the file location to a pointer variable named file

    if (file == NULL) {
        printf("Error opening file - File is NULL");
        return 1;
        //protect code against null file
    }

    int num = 0;
    int sum = 0; 
    //declare integer variables and set values held to 0 to begin

    while (fscanf(file, "%d", &num) == 1){
        sum += num;
        //read through the integers until the end of the file that is open
        //sum the numbers (add them together) as the file is read
    }

    fclose(file); 
    printf("File sum of foo.txt coontents: %d (expected : 154)\n", sum); 
    //print the value saved in sum and what is expected to be the sum to compare the code
    
    //ex5
    int valueA = 5;
    int valueB = 99; 
    //sets two values to two integer variables to be swapped
    printf("Before the swap function: Value of A = %d, Value of B = %d\n", valueA, valueB); 
    //shows the user what the beginning values are before the swap is implemented

    swap(&valueA, &valueB); 
    //swap the values by passing the memory addresses of the values

    printf("Values after the swap: value at A = %d, value at B = %d\n", valueA, valueB); 
    //print the final values held after the swap function

    //ex6
    int grid[2][3] = {
        {1, 2, 3}, 
        {4, 5, 6}
    }; 
    //creating a 2D array named grid

    printf("2D array:\n"); 
    //formatting
    print_array((int *)grid, 3, 2); 
    //call the print array function, passing the grid created flattened to one memory pointer and specifying the dimensions

    return 0;

}

