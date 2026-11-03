%include "asm_io.inc"               
;import library of pre-written functions 

segment .data                       
;commands to open storage area for fixed data that is pre-initialised
    msg db "The sum of the 1 to100 array: ", 0
    ;charecter string is defined to be called later

segment .bss                        
;commands to open storage for empty storage boxes 
    array resd 100                  
    ;saving 100 4-byte empty integer boxes for the array 
    sum resd 1                    
    ;reserve 1 4-byte empty space for the integer value of the final sum of the array

segment .text                       
;active CPU intructions follow this header 
    global asm_main                 
    ;make asm_main global so the driver.c can see it during linking

asm_main:
;starting execution label called by the CPU
    enter 0, 0                      
    ;creates a clear morkspace on teh stack 
    pusha                           
    ;take a snapshot of all of the registers before the task to avoid overwriting pre-saved data 

    ;fill array with numbers 1-100
    mov ecx, 100                    
    ;make the loop counter that will decrease to 100
    mov edi, array                  
    ;load the first saved memroy address of the first saved value of the array into the register edi
    mov ebx, 1                      
    ;set the starting value of the array to 1 

init_loop:
    mov [edi], ebx                  
    ;opens the array space being pointed at currently and saved the value in ebx into it 
    add edi, 4                      
    ;moves the pointer forward by 4-bytes (an integer space) in memory
    inc ebx                         
    ;increment the number value to be saved in the next empty space in memory by 1 
    loop init_loop                  
    ;decreases counter by 1 and loops back to init_loop if ecx is greater than 0

    ;setup to begin to sum all elements of the array 
    mov ecx, 100                    
    ;sets the loop counter back to 100
    mov edi, array                  
    ;sets the pointer back to the start of the array to work through them again
    mov eax, 0                      
    ;clear eaxcalculator screen to 0 so it can store a running total

sum_loop:
;loop to sum values of array 
    add eax, [edi]                  
    ;add the value currently being pointed at into the calculator register 
    add edi, 4                      
    ;move edi pointer 4-bytes forward to the next array value 
    loop sum_loop                   
    ;decrease the counter by 1 and loops back to the sum loop until all 100 elements are added into the calculator 

    mov [sum], eax                  
    ;save the final sum value from the register to its pre-saved spot in memory 

    ;format output and print to command screen 
    mov eax, msg                    
    ;load memory address of the saved message for the output into the register
    call print_string               
    ;print the prompt string to terminal 
    mov eax, [sum]                  
    ;move the total sum from sum into the register 
    call print_int                  
    ;print the total sum 
    call print_nl                   
    ;print a new line

    ;exit cleanly from the task 
    popa                            
    ;use original snapshot to restore reigsters back to original state 
    mov eax, 0                      
    ;return a 0 exit status showing success to driver.c
    leave                           
    ;destroy local function stack to clear workspace 
    ret                             
    ;return control to driver.c