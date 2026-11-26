#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** outb:
 *  sends given data to I/o port
 *  port = The I/O port to send the data to
 *  data = The data byte to send to the I/O port
 */

void outb(unsigned short port, unsigned char data);

/* read and return a single byte from specified I/O port */
unsigned char inb(unsigned short port);

#endif 