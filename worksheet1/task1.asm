%include "asm_io.inc"               
;imports functions in a toolbox such as print_int, print_nl etc so asm recognises them

segment .data ;declaring initial variables
    num1 dd 15                      
    ;creates a variable holding integer 15 (4 byte integer slot is used)
    num2 dd 25                      
    ;creates a variable holding integer 25

segment .text ;storage declaration complete - everything from here is instructions
    global asm_main                 
    ;make the label public so driver.c can be connected and called to

asm_main: ;starting marker for instruction code for driver.c
    enter 0, 0                      
    ;creates clear memory space in a stack frame for execution tracking
    pusha                           
    ;saves a snapshot of all registers onto the stack to ensure data that driver.c was holding isnt ruined

    mov eax, [num1]                 
    ;15 is copied from it's variable address and it is copied into a register (eax)
    add eax, [num2]                 
    ;the value held at num2 (25) is added to the value stored in eax and the result is saved
    call print_int                  
    ;use the function that has been pre-written to output the outcome to the terminal
    call print_nl                   
    ;prints a new line using a pre-written function

    popa                            
    ;restores the registers back to how they were before the task 
    mov eax, 0                      
    ;returns a 0 for success by placing 0 on eax scratchpad
    leave                           
    ;destroys memory workspace that was set up before task
    ret                             
    ;control returns back to driver.c