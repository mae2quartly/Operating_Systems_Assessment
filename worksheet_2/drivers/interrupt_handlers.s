global load_idt
;make label load_idt visible outside so it can be called by c

load_idt:
;loads the IDT location and size into the CPU's IDTR register (esp = return address)
    mov eax, [esp + 4]
    ;move the memory address of the IDT struct into eax (esp + 4 = first argument passed from C)
    lidt [eax]
    ;tell the CPU to load the IDT table described at the address stored in eax
    ret
    ;return back to the calling C function