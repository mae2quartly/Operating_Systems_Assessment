global outb                         
;makes label outb visible outside of the file 
global inb
;makes label inb visible outside of the file

outb:
;send a byte to an I/O port (esp = return address)
    mov al, [esp + 8]               
    ;move the data to be sent into the al register (esp + 8 = the data byte)
    mov dx, [esp + 4]               
    ;move the address of the I/O port into the dx register (esp + 4 = I/O port)
    out dx, al                      
    ;send the data to the I/O port
    ret                             
    ;return to the calling function

inb:
;returns a byte from the given I/O port (esp = return address)
    mov dx, [esp + 4]
    ;move the address of the I/O port to the dx register (esp + 4 = I/O port)
    in al, dx
    ;read a byte from the I/O port and store it in the al register
    ret
    ;return to the calling function