#ifndef INCLUDE_FRAME_BUFFER_H
#define INCLUDE_FRAME_BUFFER_H

/* 16 colour palatte defined constants to be called within the code */
#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

/* dimensions of the screen - 80x25 */
#define FB_NUM_COLS 80
#define FB_NUM_ROWS 25

/* moves the cursor to given coordinates */
void fb_move(unsigned short x, unsigned short y);

/* sets active for and back ground colour for text output latet in the code*/
void fb_set_color(unsigned char fg, unsigned char bg);

/* clears screen and resets the cursor to be at a neutal 0,0 position */
void fb_clear(void);

/* writes a single charecter at the position the cursor is currently at and moves the cursor forwards*/
void fb_write_char(char c);

/* writes a null-terminated string on the screen */
void fb_write_string(const char *str);

/* convert int to decimal text and display on the screen */
void fb_write_int(int num);

/* moves the cursor to the start of a newline*/
void fb_newline(void);

/* move the cursor back one and erase the charecter in the space taht was there*/
void fb_backspace(void);

#endif 