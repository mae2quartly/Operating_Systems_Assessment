#ifndef INCLUDE_INTERRUPTS
#define INCLUDE_INTERRUPTS

#include "type.h"

/* tells CPU where IDT is located in memory and how big it is */
struct IDT
{
    u16int size;        
    /* size of the IDT table */
    u32int address;     
    /* start memory address - where the IDT array is */
} __attribute__((packed));

/* represents single entry in IDT table, points to one interrupt handler */
struct IDTDescriptor {
    /* lower 32bits of entry */
    u16int offset_low;          
    /* lower 16bits of handler function's address */
    u16int segment_selector;    
    /* point to the kernel code segment in the GDT - CPU knows what permissions to run */

    /* upper 32bits of the entry */
    u8int  reserved;            
    /* unused space set to 0 */
    u8int  type_and_attr;       
    /* settings - marks entry as active, sets priviledge level and 32bit gate type */
    u16int offset_high;         
    /*upper 16bits of handlers function memory address */
} __attribute__((packed));

/* store all general purpose registers saved on stack before c is run */
struct cpu_state {
    u32int eax;
    u32int ebx;
    u32int ecx;
    u32int edx;
    u32int ebp;
    u32int esi;
    u32int edi;
} __attribute__((packed));

/* stores values automaticallu pushed onto the stack when interrupt occurs */
struct stack_state {
    u32int error_code;  
    /* eror code pushed by cpu */
    u32int eip;         
    /* address of code running before interrupt */
    u32int cs;          
    /* code segment register before the interrupt */
    u32int eflags;      
    /* CPU flags register before the interrupt */
} __attribute__((packed));

/*wait and return the next unread ASCII character from the keyboard buffer */
u8int getc(void);

/*read a line of text into buffer and end with '\0' */
u32int readline(char *buffer, u32int max_len);

/* main c function that decides what to do when interrupt fires */
void interrupt_handler(struct cpu_state cpu, u32int interrupt, struct stack_state stack);

/* set up idt entries, loads the idt into cpu, and turn on keyboard interrupts */
void interrupts_install_idt(void);

/* assembley helper functions */
void load_idt(u32int idt_address);
void interrupt_handler_33(void);
void interrupt_handler_14(void);

#endif 