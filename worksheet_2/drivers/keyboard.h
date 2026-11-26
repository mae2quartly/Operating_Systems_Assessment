#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

#define KEYBOARD_MAX_ASCII 83
/* highest scan code index handled by our lookup table */

#include "type.h"

/* reads one raw hardware scan code byte from keyboard data port 0x60 */
u8int keyboard_read_scan_code(void);

/* translates a raw keyboard scan code into an ASCII character (returns 0 if unmapped or key release) */
u8int keyboard_scan_code_to_ascii(u8int scan_code);

#endif 