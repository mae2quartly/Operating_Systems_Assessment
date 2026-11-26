#include "pic.h"
#include "io.h"

/* move pic interrupt numbers so they dont clash with cpu interrupts */
void pic_remap(s32int offset1, s32int offset2)
{
    unsigned char a1, a2;   
    /* variables to record which hardware is currently blocked or allowed */

    /* read and save current on or off interrupt masks from both chips */
    a1 = inb(PIC_1_DATA);
    a2 = inb(PIC_2_DATA);

    /* send restart/init command to both pic and say send a 4th setup byte later */
    outb(PIC_1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    outb(PIC_2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    /* tell master pic to start its interrupts at offset 1 and slave pic at offset2 */
    outb(PIC_1_DATA, (u8int)offset1);
    outb(PIC_2_DATA, (u8int)offset2);

    /* wire 2 chips together */
    outb(PIC_1_DATA, 4);
    outb(PIC_2_DATA, 2);

    /* set both chips to standard x84 pc mode */
    outb(PIC_1_DATA, PIC_ICW4_8086);
    outb(PIC_2_DATA, PIC_ICW4_8086);

    /* put original saved on/off interrupt masks back so unwanted interrupts dont appear */
    outb(PIC_1_DATA, a1);
    outb(PIC_2_DATA, a2);
}

/* sends an eoi signal to pic after handling interrupt */
void pic_acknowledge(u32int interrupt)
{
    /* check and see if the interrupt number is outside the pic range - if it is do nothing */
    if (interrupt < PIC_1_OFFSET || interrupt > PIC_2_END) {
        return;
    }

    /* if the interrupt came from the slave pic send a done signal to slave chip first */
    if (interrupt >= PIC_2_OFFSET) {
        outb(PIC_2_COMMAND_PORT, PIC_ACKNOWLEDGE);
    }

    /* always send done to the master pic chip */
    outb(PIC_1_COMMAND_PORT, PIC_ACKNOWLEDGE);
}