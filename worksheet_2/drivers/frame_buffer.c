#include "frame_buffer.h"
#include "io.h"

/* hardware ports for cursor */
#define FB_COMMAND_PORT         0x3D4   
/* selects which internal VGA register to modify */
#define FB_DATA_PORT            0x3D5   
/* writes the actual value to the selected register */

/* commands to ports */
#define FB_HIGH_BYTE_COMMAND    14      
/* tells VGA hardware we are sending the upper 8 bits of the cursor position */
#define FB_LOW_BYTE_COMMAND     15      
/* tells VGA hardware we are sending the lower 8 bits of the cursor position */

/* set pointer to the start of text vid memory */
static char *fb = (char *) 0x000B8000;  
/* address where text mode reads characters and colors */

/* tracking the current cursor position and colours beign used */
static unsigned short cursor_x = 0;         
/* current column position */
static unsigned short cursor_y = 0;         
/* current row position */
static unsigned char current_fg = FB_WHITE; 
/* active txt colour */
static unsigned char current_bg = FB_BLACK; 
/* active background colour */

/* write a charecter with the specified foreground and background to the passed position (i) */
static void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;                                              
    /* byte 0 - stores the 8-bit ASCII character code */
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);           
    /* byte 1 - packs 4-bit background into upper nibble and 4-bit foreground into lower nibble */
}

/* moves the cursor out of the framebuffer to the passed position */
static void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);            
    /* select VGA cursor high-byte register */
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));           
    /* shift 16 bit position right by 8 - mask to send only upper byte */
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);             
    /* select VGA cursor low-byte register */
    outb(FB_DATA_PORT,    pos & 0x00FF);                    
    /* mask to send only lower 8 bits of the position */
}

/* scrolls the screen up by one line when the cursor passes a low point on the screen */
static void fb_scroll(void)
{
    unsigned int row, col;

    /* move each row up into the row above it (so it seems as though the screen is scrolling)*/
    for (row = 1; row < FB_NUM_ROWS; row++) {               
        /* start at row 1 because 0 gets overwritten and disappears */
        for (col = 0; col < FB_NUM_COLS; col++) {           
            /* loop through columns in the current row */
            unsigned int src = (row * FB_NUM_COLS + col) * 2;           
            /* find byte offset of current cell */
            unsigned int dst = ((row - 1) * FB_NUM_COLS + col) * 2;     
            /* find byte offset of the cell above it */
            fb[dst] = fb[src];                              
            /* copy the character byte up one row */
            fb[dst + 1] = fb[src + 1];                      
            /* copy color attribute byte up one row */
        }
    }

    /* empty the bottom row */
    for (col = 0; col < FB_NUM_COLS; col++) {               
        /* loop across all columns of the last row */
        unsigned int bottom = ((FB_NUM_ROWS - 1) * FB_NUM_COLS + col) * 2;  
        /* byte offset for row 24 at column 'col' */
        fb_write_cell(bottom, ' ', current_fg, current_bg); 
        /* overwrite with a blank space using the current back and foreground colours */
    }

    cursor_y = FB_NUM_ROWS - 1;                             
    /* keep vertical cursor clamped on bottom row */
}

/* moves the cursor to the passed 2D coordinates */
void fb_move(unsigned short x, unsigned short y)
{
    /* ensure x cannot exceed the max column index */
    if (x >= FB_NUM_COLS) {
        x = FB_NUM_COLS - 1;
    }
    /* ensure y cannot exceed the maximum row index */
    if (y >= FB_NUM_ROWS) {
        y = FB_NUM_ROWS - 1;
    }

    cursor_x = x;                                           
    /* update saved column position */
    cursor_y = y;                                           
    /* update saved row position */
    fb_move_cursor(cursor_y * FB_NUM_COLS + cursor_x);      
    /* convert 2d coordinates to 1d cell offset and move hardware cursor  */
}

/* sets the active foreground and background colors for future characters */
void fb_set_color(unsigned char fg, unsigned char bg)
{
    current_fg = fg & 0x0F;                                 
    /* mask to ensure foreground stays within range */
    current_bg = bg & 0x0F;                                 
    /* mask to ensure background stays within range  */
}

/* clears entire screen with spaces and reset cursor to top-left */
void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_NUM_COLS * FB_NUM_ROWS; i++) {       
        /* loop through all char cells on the screen  */
        fb_write_cell(i * 2, ' ', current_fg, current_bg);  
        /* write a space charecter at a byte offset with active colours */
    }
    fb_move(0, 0);                                          
    /* reset both coordinates and cursor back to origin */
}

/* move the cursor back one cell and erase charecter, updte cursor */
void fb_newline(void)
{
    cursor_x = 0;                                           
    /* cursor position returned to left margin */
    cursor_y++;                                             
    /* move vertical cursor position down by one row */
    if (cursor_y >= FB_NUM_ROWS) {                          
        /* check to see if the cursor has moved off the screen (past 24) */
        fb_scroll();                                        
        /* scroll all rows up by 1 */
    }
    fb_move_cursor(cursor_y * FB_NUM_COLS + cursor_x);      
    /* update blinking cursor to new line */
}

/* moves the cursor back one cell, erases that character, and updates hardware cursor */
void fb_backspace(void)
{
    /*check to see if the backspace means moving up a row */
    if (cursor_x > 0) {
        cursor_x--;                                         
        /* step back one column to left */
    } else if (cursor_y > 0) {
        cursor_y--;                                         
        /* if at coloumn 0 byt not row 0 move up one row */
        cursor_x = FB_NUM_COLS - 1;                         
        /* place cursor at far right of the line above */
    }

    unsigned int index = (cursor_y * FB_NUM_COLS + cursor_x) * 2;   
    /* find byte offset of the char just stepped back onto */
    fb_write_cell(index, ' ', current_fg, current_bg);      
    /* old char erased by overwriting a blank space */
    fb_move_cursor(cursor_y * FB_NUM_COLS + cursor_x);      
    /* move blinking cursor back to erased cell */
}

/* writes a char at cursor position and move the cursor forward */
void fb_write_char(char c)
{
    /* if teh charecter is a newline then jump to next line */
    if (c == '\n') {
        fb_newline();
        return;
    }

    /* if the charecter is backspace delete the previous cell */
    if (c == '\b') {
        fb_backspace();
        return;
    }

    unsigned int index = (cursor_y * FB_NUM_COLS + cursor_x) * 2;   
    /* find byte offset in memory */
    fb_write_cell(index, c, current_fg, current_bg);      
    /* write charcetr and active colours into RAM */

    cursor_x++;                                             
    /*move cursor one row to the right */
    if (cursor_x >= FB_NUM_COLS) {                          
        /* if cursor meets the right edge of the screen */
        cursor_x = 0;                                       
        /* place cursor back onto column 0  */
        cursor_y++;                                         
        /* move down to next row*/
        if (cursor_y >= FB_NUM_ROWS) {                      
            /* if when cursor is dropped it is lower than 24 */
            fb_scroll();                                    
            /* scroll screen up by 1 row */
        }
    }

    fb_move_cursor(cursor_y * FB_NUM_COLS + cursor_x);      
    /* update hardware cursor to match new position */
}

/* write null-terminated C string one character at a time */
void fb_write_string(const char *str)
{
    unsigned int i = 0;                                     
    /* index counter starting at the first charecter of the string */
    while (str[i] != '\0') {                                
        /* loop until null terminator is reached at the end of the string */
        fb_write_char(str[i]);                              
        /* print current char and advance cursor */
        i++;                                                
        /* move to next char position in the array */
    }
}

/* convert integer into ASCIi and print onto screen */
void fb_write_int(int num)
{
    char buf[12];                                           
    /* temp char buffer */
    int i = 0;                                              
    /* index to track how many digits have been extracted to buffer */
    unsigned int n;                                         
    /* unsigned copy of num to prevent signed overflow */

    /* if num = 0 print 0 as loop only runs for n > 0 */
    if (num == 0) {
        fb_write_char('0');
        return;
    }

    /* if the num is negative print - and convert magnitude to unsigned */
    if (num < 0) {
        fb_write_char('-');
        n = (unsigned int)(-(num + 1)) + 1;                 
        /* two's compliment negation to avoid signed overflow */
    } else {
        n = (unsigned int)num;                              
        /* positive number cna be directly casted to unsigned */
    }

    /* extract digits in reverse order (least sig fig first using modulo) */
    while (n > 0) {
        buf[i++] = (char)('0' + (n % 10));                  
        /* convert remainder to ASCII and store in buf*/ 
        n /= 10;                                            
        /* divide by 10 to shift to next decimal place*/
    }

    /* print stored digits reverse to appear left to right */
    while (i > 0) {
        fb_write_char(buf[--i]);                            
        /*decrement i first to point at last valid digit and print */
    }
}