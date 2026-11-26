#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "frame_buffer.h"
#include "keyboard.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
/* Total number of entries in the x86 IDT (interrupts 0 to 255) */

#define INTERRUPTS_KEYBOARD 33
/* Keyboard interrupt number after PIC remapping (IRQ1 = 32 + 1 = 33) */

#define INPUT_BUFFER_SIZE 256
/* Maximum number of characters our circular keyboard buffer can hold */



volatile u8int input_buffer[INPUT_BUFFER_SIZE];
/*volitile circular array that tstores typed ASCII chars*/

volatile u32int write_index = 0;
/*index used by keyboard interrupt to write the next char*/

volatile u32int read_index = 0;
/* where getc() reads next unread char*/



struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
/* The full 256-entry Interrupt Descriptor Table array in memory */

struct IDT idt;
/* Pointer structure holding the IDT's total size and starting memory address */

/* Fills in one 64-bit entry in the IDT table so interrupt 'index' jumps to function 'address' */
void interrupts_init_descriptor(s32int index, u32int address)
{
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
    /* Store the upper 16 bits of the handler function's memory address (bits 16..31) */

    idt_descriptors[index].offset_low = (address & 0xFFFF);
    /* Store the lower 16 bits of the handler function's memory address (bits 0..15) */

    idt_descriptors[index].segment_selector = 0x08;
    /* Point to the kernel's code segment in the GDT (entry 1 * 8 bytes = 0x08) */

    idt_descriptors[index].reserved = 0x00;
    /* Reserved byte must always be 0 */

    /*
     * Build the type_and_attr settings byte:
     * (0x01 << 7) -> P = 1 (entry is active/present in memory)
     * (0x00 << 6) | (0x00 << 5) -> DPL = 00 (ring 0 kernel privilege level)
     * 0x0E -> 0b1110 (32-bit interrupt gate, which automatically disables further interrupts while running)
     */
    idt_descriptors[index].type_and_attr =
        (0x01 << 7) |
        (0x00 << 6) | (0x00 << 5) |
        0x0E;
}

/* Registers our keyboard handler in the IDT, loads the IDT into the CPU, and turns on keyboard IRQ1 */
void interrupts_install_idt(void)
{
    interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int) interrupt_handler_33);
    /* Connect interrupt 33 to our assembly wrapper interrupt_handler_33 */

    idt.address = (u32int) &idt_descriptors;
    /* Save the starting memory address of the IDT array */

    idt.size = (sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT) - 1;
    /* Calculate the IDT limit (total bytes - 1 = 2047, as required by the CPU's lidt instruction) */

    load_idt((u32int) &idt);
    /* Tell the CPU to load our IDT table using the assembly lidt instruction */

    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
    /* Move Master PIC interrupts to 32..39 and Slave PIC interrupts to 40..47 */

    outb(0x21, 0xFD);
    /* Mask all Master PIC interrupts except IRQ1 keyboard (0xFD = 11111101 in binary, so timer IRQ0 is blocked) */

    outb(0xA1, 0xFF);
    /* Mask all Slave PIC interrupts (0xFF = 11111111 in binary) */
}

/* Main C interrupt handler called by assembly whenever a registered interrupt triggers */
void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack)
{
    u8int input;
    /* Raw scan code byte read from the keyboard hardware */

    u8int ascii;
    /* Converted ASCII character matching the pressed key */



    u32int next_write;
    /*next write position in circular buffer - used to see if buffer is full*/



    switch (interrupt) {
        case INTERRUPTS_KEYBOARD:
            /* Keep reading while bit 0 of keyboard status port 0x64 is 1 (meaning data is waiting) */
            while ((inb(0x64) & 1)) {
                input = keyboard_read_scan_code();
                /* Read the raw scan code byte from keyboard data port 0x60 */

                /* Only process key presses (bit 7 is 0); ignore key releases (bit 7 is 1 / >= 0x80) */
                if (!(input & 0x80)) {
                    if (input <= KEYBOARD_MAX_ASCII) {
                        ascii = keyboard_scan_code_to_ascii(input);
                        /* Translate the hardware scan code into a readable ASCII character */



                        if (ascii != 0) {
                            /* used to find next write spot and only store info if buffer is not full alr */
                            next_write = (write_index + 1) % INPUT_BUFFER_SIZE;
                            if (next_write != read_index) { /*if the next space is not the charecter in the buffer to be read next */
                                input_buffer[write_index] = ascii;
                                /* store the typed char */
                                write_index = next_write;
                                /* move write pos forward*/
                            }


                        }
                    }
                }
            }

            pic_acknowledge(interrupt);
            /* Send End-Of-Interrupt (EOI) to the PIC so it can send the next keypress */
            break;

        default:
            break;
    }
}




/* wait until a key is pressed and returns next char from circ buffer */
u8int getc(void)
{
    u8int c;
    /* var to hold char that was read from the buffer */

    /* wait until the write_index moves ahead of read (meaning at least 1 charecter is unread and ready) */
    while (read_index == write_index) {
        asm volatile("hlt"); /*pauses CPU until next hardware interrupt*/
    }

    c = input_buffer[read_index];
    /* read the next charceter from teh buffer */

    read_index = (read_index + 1) % INPUT_BUFFER_SIZE;
    /* advance read pointer - wrap around when at it = input buffer size */

    return c;
    /* return char to caller */
}

/* read a line of user input into buffer - echo chars on screen and stop at new line*/
u32int readline(char *buffer, u32int max_len)
{
    u32int count = 0;
    /* tracking number of chars in caller's line buffer */

    u8int c;
    /* hold chars returned by getc() */

    /* leaves room for at least one extra char for safety  */
    if (max_len == 0) {
        return 0;
    }

    while (1) {
        c = getc();
        /* wait for and read next key pressed from circular buffer*/

        /* enter pressed */
        /* move cursor to new line - null terminate string - return length of line */
        if (c == '\n') {
            fb_newline();
            buffer[count] = '\0';
            return count;
        }
        /* backspace pressed */
        /* only erase the char if there is at least 1 char typed on current prompt line */
        else if (c == '\b') {
            if (count > 0) {
                count--;
                fb_backspace();
            }
        }
        /* printable char */
        /* store in buffer with the safety space*/
        else if (count < max_len - 1) {
            buffer[count] = (char)c;
            count++;
            fb_write_char((char)c);
        }
    }
}