%include "asm_io.inc"               
;import pre-written functions for input and output 

segment .data                       
;storage for charecter strings to be used in the output of the code
    prompt_start db "Please enter a number between 1-100 to start the index at: ", 0
    prompt_end db "Pleas enter a number between 1-100 to end the index at: ", 0
    msg_result db "Sum of the range specified: ", 0
    err_msg db "Error reported: Please specify a valid range", 10, 0

segment .bss                        
;storage for 
    array resd 100              
    ;save 100 4-byte empty spaces in memory for 100 integers in the array
    start_idx resd 1              
    ;save 4-byte space for the users choice of where to start the index
    end_idx resd 1              
    ;save 4-byte space for the users choice of where to end the index
    sum resd 1              
    ;saved space for the 4-byte space of the value of the summed index given 

segment .text                       
;start of executable instructions 
    global asm_main                 
    ;make asm_main global so it is visible to driver.c for linking

asm_main:
    enter 0, 0                      
    ;create a clear stack function workspace 
    pusha                           
    ;take a snapshot of all registers to restore the previous state after the task 

    ;set up to populate array 
    mov ecx, 100                    
    ;set loop counter to 100 ready to decrement 
    mov edi, array                  
    ;load the starting address of the saved space if the array into edi
    mov ebx, 1                      
    ;set the starting number of the array in ebx

init_loop:
    mov [edi], ebx                  
    ;store the number currently in ebx into the array slot currently being pointed to
    add edi, 4                      
    ;move the pointer forward 4-bytes to point to the next spot for the array values
    inc ebx                         
    ;increment the number value next to be saved by 1
    loop init_loop                  
    ;decrement ecx by 1 
    ;loop back until ecx = 0 

    ;get user inputs for the start and end of the range to be summed
    mov eax, prompt_start           
    ;load memory address of the string to be printed into eax
    call print_string               
    ;display the string
    call read_int                   
    ;read the customer input from the keyboard
    mov [start_idx], eax            
    ;save the starting number into the pre-saved variable

    mov eax, prompt_end             
    ;load memory address of the string to be printed into eax
    call print_string               
    ;display string
    call read_int                   
    ;read the cutomer input from the keyboard
    mov [end_idx], eax              
    ;save the ending number into the pre-saved variable 

    ;validate range
    mov eax, [start_idx]            
    ;copy the inputted starting index saved number into register
    cmp eax, 1                      
    ;compare the number against 1 
    jl invalid_range                
    ;if its less than one return an error by jumping to error 

    mov ebx, [end_idx]              
    ;copy the inputted end index saved number into register
    cmp ebx, 100                    
    ;compare number against 100 
    jg invalid_range                
    ;if its more than 100 return an error by jumping to error handler 

    cmp eax, ebx                    
    ;compare start number vs end number
    jg invalid_range                
    ;if start number is graeter than end number jumps to the error handler 

    ;find byte offset and iteration counter
    dec eax                         
    ;take 1 off the start index to convert to offset 0 
    imul eax, 4                     
    ;times by 4 to find the exact size in ram and therefore the exact byte displacement
    mov edi, array                  
    ;load base memroy address of the array into register
    add edi, eax                    
    ;add the byte offset to edi so noe it points directly to the value at start_index-1

    ;find iteration count
    mov ecx, [end_idx]              
    ;copy end index into register 
    sub ecx, [start_idx]            
    ;subtract the start index from the value saved in the register 
    inc ecx                         
    ;add 1 to get total inclusive element count to sum 

    mov eax, 0                      
    ;clear calculator reigster to 0 to hold a running count

sum_range_loop:
    add eax, [edi]                  
    ;add the int value at the current memory pointer into the register 
    add edi, 4                      
    ;move pointer forward by the amount of memroy an int takes up 
    loop sum_range_loop             
    ;decrement register by 1 and repeat loop until all elements are added to register

    mov [sum], eax                  
    ;save calculated sum into the variable saved as sum 

    ;output result
    mov eax, msg_result             
    ;load memory address of the result string into register 
    call print_string               
    ;print string
    mov eax, [sum]                  
    ;copy range total into register
    call print_int                  
    ;print integer sum
    call print_nl                   
    ;print a new line
    jmp end_program                 
    ;skip error handler 

;error handler and exit program 
invalid_range:
    mov eax, err_msg                
    ;load memory address of error string into register 
    call print_string               
    ;print error sentence

end_program:
    popa                            
    ;restore original registers from snapshot 
    mov eax, 0                      
    ;return 0 (success) to driver.c 
    leave                           
    ;destroy funtion workspace stack
    ret                             
    ;control returned to driver.c