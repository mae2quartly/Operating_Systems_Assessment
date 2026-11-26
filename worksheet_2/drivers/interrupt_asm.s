extern interrupt_handler
;tells NASM that interrupt_handler is defined in an external C file (interrupts.c)

%macro no_error_code_interrupt_handler 1
;reusable template for interrupts where the CPU does NOT automatically push an error code
global interrupt_handler_%1
;makes this specific handler (e.g. interrupt_handler_33) visible to C code
interrupt_handler_%1:
    push dword 0
    ;push a fake 0 error code onto the stack so the stack layout matches interrupts that do have an error code
    push dword %1
    ;push the interrupt number onto the stack so C knows which device triggered it
    jmp common_interrupt_handler
    ;jump to the shared assembly code that saves registers and calls C
%endmacro

%macro error_code_interrupt_handler 1
;reusable template for CPU errors where the CPU ALREADY pushes an error code automatically
global interrupt_handler_%1
;makes this specific handler (e.g. interrupt_handler_14) visible to C code
interrupt_handler_%1:
    push dword %1
    ;push the interrupt number onto the stack (error code is already on the stack)
    jmp common_interrupt_handler
    ;jump to the shared assembly code that saves registers and calls C
%endmacro

common_interrupt_handler:
;shared wrapper that pauses what the CPU was doing, runs our C interrupt_handler, and resumes safely
    ;save all general-purpose CPU registers onto the stack so C doesn't overwrite them
    push eax
    push ebx
    push ecx
    push edx
    push ebp
    push esi
    push edi

    call interrupt_handler
    ;call the C function interrupt_handler(cpu_state, interrupt, stack_state)

    ;restore all general-purpose CPU registers in exact reverse order
    pop edi
    pop esi
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax

    add esp, 8
    ;remove the 2 values we pushed earlier (interrupt number + error code = 8 bytes) to clean up the stack
    iret
    ;special interrupt return: restores EIP, CS, and EFLAGS so the interrupted program continues normally

error_code_interrupt_handler 14
;generate handler for interrupt 14 (CPU page fault, which pushes an error code)
no_error_code_interrupt_handler 33
;generate handler for interrupt 33 (IRQ1 keyboard keypress, which has no error code)