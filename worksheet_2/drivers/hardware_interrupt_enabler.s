global enable_hardware_interrupts
;makes enable_hardware_interrupts visible to C code
global disable_hardware_interrupts
;makes disable_hardware_interrupts visible to C code

enable_hardware_interrupts:
;sets the CPU's Interrupt Flag so the processor will respond to hardware devices like the keyboard
    sti
    ;Set Interrupt Flag instruction -turn hardware interrupts on
    ret
    ;return to the C function

disable_hardware_interrupts:
;clears the CPU's Interrupt Flag (IF = 0) so hardware devices cannot interrupt critical code
    cli
    ;clear Interrupt Flag instruction - turns hardware interrupts OFF
    ret
    ;return to the calling C function