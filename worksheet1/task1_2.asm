%include "asm_io.inc"               
;import useful functions/routines

segment .data                       
;storage for pre-defined variables that are constant when the code is compiled
    msg1 db "Enter a number: ", 0   
    ;string to be held in memory, db sets each charecter to be stored as a 1 byte charecter
    ;the 0 is added at the end as print_string will use this to see when to stop printing the string
    msg2 db "The sum of ", 0        
    ;string of charecters saved in RAM to be used later 
    msg3 db " and ", 0              
    ;string of charecters saved in RAM to be used later
    msg4 db " is: ", 0              
    ;string of charecters saved in RAM to be used later

segment .bss                        
;storage for variables that will be filled later in the code (empty memory reserved for now)
    integer1 resd 1                 
    ;4-byte empty variable memory slot for later named integer1
    integer2 resd 1                 
    ;4-byte empty variable memory slot for later named integer2
    result   resd 1                 
    ;4-byte empty variable memory slot used to store the sum of integer1 and integer2

segment .text                       
;executable code is from below this line onwards
    global asm_main                 
    ;make asm_main global so it can be seen by driver.c when linking

asm_main:
;the function that is run when driver.c calls asm_main
    enter 0, 0                      
    ;creates a clear stack in memory for operations to be run and held
    pusha                           
    ;creates a snapshot of all registers before driver.c begins to avoid overwriting data during task

    ;take input of a number from a user 
    mov eax, msg1                   
    ;copies the address of the string Enter a... into eax scratchpad
    call print_string               
    ;print the text at the address to the terminal
    call read_int                   
    ;pause execution and take the inputted number and saves it in eax scratchpad
    mov [integer1], eax             
    ;copies the number from eax to the memory held for integer1

    ;take input of second number from user 
    mov eax, msg1                   
    ;copies the address of the string Enter a... into eax scratchpad
    call print_string               
    ;print the text at the address to the terminal
    call read_int                   
    ;pause execution and take the inputted number and saves it in eax scratchpad
    mov [integer2], eax             
    ;copies the number from eax to the memory held for integer2

    ;complete addition
    mov eax, [integer1]             
    ;copy number held in integer1 to calculation scratchpad
    add eax, [integer2]             
    ;add number held in integer2 to the value currently stored within the calculation scratchpad
    mov [result], eax               
    ;copy the final sum in the eax calculator to the memory space held for sum

    ;print formatted string using saved strings
    mov eax, msg2                   
    ;put The sum of... (msg2) into eax
    call print_string               
    ;print the message 
    mov eax, [integer1]             
    ;copy the first inputted number from its memory location into eax
    call print_int                  
    ;print the first value 
    mov eax, msg3                   
    ;put "and" (msg3) into eax
    call print_string               
    ;print msg3
    mov eax, [integer2]             
    ;copy the second inputted number from its memory location into eax
    call print_int                  
    ;print the second number value 
    mov eax, msg4                   
    ;put is (msg4) into eax
    call print_string               
    ;print msg4
    mov eax, [result]               
    ;copy the caculated total value from memory space into eax
    call print_int                  
    ;print total final value
    call print_nl                   
    ;print a new line

    popa                            
    ;restore the original register values from the snapshot taken before the task
    mov eax, 0                      
    ;use a 0 on the calculator screen to signal success to driver.c
    leave                           
    ;destroy local workspace frame
    ret                             
    ;return control to driver.c