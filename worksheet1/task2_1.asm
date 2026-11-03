%include "asm_io.inc"               
;import pre-written functions/routines

segment .data                       
;storage for pre-defined strings used to be outputted during the task
    prompt_name db "Please can you enter your name: ", 0
    prompt_count db "Please may you enter a number between 51 and 99: ", 0
    welcome_msg db "Welcome to Assembly, ", 0
    err_2small db "Error reported; the number input in must be greater than 50", 10, 0
    err_2large db "Error reported; the number must be less than 100", 10, 0

segment .bss                        
;memory space for variables to be filled during the task process
    name resb 64                   
    ;reserves 64 1-byte spaces for the user to enter their name in charecters
    count resd 1                    
    ;reserves one 4-byte space in memory for the count to be saved into

segment .text                       
;all intructions for the task follow this header 
    global asm_main                 
    ;makes asm_main global and therefore visible for driver.c during linking

asm_main:
    enter 0, 0                      
    ;create a clear stack workspace for local execution to be managed and saved
    pusha                           
    ;takes a snapshot of all registers before the task as to not overwrite any data driver.c was holding previously

    ;ask for and save user's name
    mov eax, prompt_name            
    ;load address of Enter your... (prompt_name) into eax 
    call print_string               
    ;print the stored string on the terminal screen 
    mov edi, name                   
    ;load the starting memory address of the saved space to store the entered name

read_name_loop:
;loop to read name chrecter by charecter and save each one
    call read_char                  
    ;read 1 character typed by the user and save it's ASCII byte code
    cmp al, 10                      
    ;ensure the ASCII code isn't 10 (in which case it is an enter key)
    je done_name                    
    ;if ASCII code = 10 jump to done_name label 
    mov [edi], al                   
    ;if enter hasn't been pressed the typed chrecter sitting in al is copied to the memory box being indicated 
    inc edi                         
    ;move to the next saved memory box 
    jmp read_name_loop              
    ;repeat loop to do the same process to the next character
done_name:
;function called after enter is hit
    mov byte [edi], 0               
    ;put a 0 at the end of the string as a null terminator so print_string knows where to stop reading

    ;ask for a count number and save
    mov eax, prompt_count           
    ;load address for Enter a number between... onto eax 
    call print_string               
    ;print string in prompt_count
    call read_int                   
    ;read the complete integer inputted and place it into eax
    mov [count], eax                
    ;store entered number into variable count

    ;validate the input range of count
    cmp eax, 50                     
    ;compare the saved number to 50
    jle too_small                   
    ;if the number is smaller than 50 jump to execute too_small
    cmp eax, 100                    
    ;compare the saved number to 100
    jge too_large                   
    ;if the number is larger than 100 jump to execute too_large

    mov ecx, [count]                
    ;copy the validated count number into ecx (loop countdown register)

;loop to print the welcome message 
print_welcome_loop:
    push ecx                        
    ;save current loop count onto the stack to avoid overwriting
    
    mov eax, welcome_msg            
    ;load address of Welcome to... 
    call print_string               
    ;print the string held at welcome_msg
    mov eax, name                   
    ;load address of the users inputted name 
    call print_string               
    ;print the name held at address
    call print_nl                   
    ;print a new line

    pop ecx                         
    ;retrieve saved loop counter from stack back into ecx
    loop print_welcome_loop         
    ;ecx is decremented by 1 
    ;if ecx is still greater than 0 the task jumps back up to print_welcome_loop
    ;omce ecx is 0 the execution moves on 

    jmp end_program                 
    ;skip pass error handler to exit cleanly if they are not needed 

;error handlers for inputted numbers
too_small:
    mov eax, err_2small              
    ;load address of too_small
    call print_string               
    ;print data held at address
    jmp end_program                 
    ;jump to exit

too_large:
    mov eax, err_2large              
    ;load address of too_large
    call print_string               
    ;print error message 

;clean exit from the code 
end_program:
    popa                            
    ;restore registers back to their original state based off the snapshot taken previously
    mov eax, 0                      
    ;return status is set to 0 indictaing success to driver.c
    leave                           
    ;destroy the local function stack frame workspace
    ret                             
    ;return control back to driver.c