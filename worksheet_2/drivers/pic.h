#ifndef INCLUDE_PIC_H
#define INCLUDE_PIC_H

#include "type.h"

/* Hardware port numbers for the two PIC chips */
#define PIC_1               0x20    /* Main (master) PIC port */
#define PIC_2               0xA0    /* Secondary (slave) PIC port */
#define PIC_1_COMMAND       PIC_1
#define PIC_1_DATA          (PIC_1 + 1)
#define PIC_2_COMMAND       PIC_2
#define PIC_2_DATA          (PIC_2 + 1)

/* New interrupt numbers after moving them out of the CPU's reserved range */
#define PIC_1_OFFSET        0x20    /* Master PIC interrupts start at 32 (0x20) */
#define PIC_2_OFFSET        0x28    /* Slave PIC interrupts start at 40 (0x28) */
#define PIC_2_END           (PIC_2_OFFSET + 7)

#define PIC_1_COMMAND_PORT  0x20
#define PIC_2_COMMAND_PORT  0xA0
#define PIC_ACKNOWLEDGE     0x20    /* Code sent to tell the PIC we finished handling an interrupt */

/* Setup flags sent when restarting the PIC chips */
#define PIC_ICW1_ICW4       0x01    /* Tell PIC we will send an extra setup byte (ICW4) */
#define PIC_ICW1_SINGLE     0x02    /* Single PIC mode (not used here) */
#define PIC_ICW1_INTERVAL4  0x04    /* Call address interval 4 */
#define PIC_ICW1_LEVEL      0x08    /* Level-triggered mode */
#define PIC_ICW1_INIT       0x10    /* Start PIC initialization */

#define PIC_ICW4_8086       0x01    /* Run in standard x86 PC mode */
#define PIC_ICW4_AUTO       0x02    /* Auto end-of-interrupt mode */
#define PIC_ICW4_BUF_SLAVE  0x08    /* Buffered slave mode */
#define PIC_ICW4_BUF_MASTER 0x0C    /* Buffered master mode */
#define PIC_ICW4_SFNM       0x10    /* Special fully nested mode */

/* Moves hardware interrupts to new numbers so they don't clash with CPU errors */
void pic_remap(s32int offset1, s32int offset2);

/* Tells the PIC hardware we are done handling an interrupt */
void pic_acknowledge(u32int interrupt);

#endif /* INCLUDE_PIC_H */